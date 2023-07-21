/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Math/spline.cpp $
 * $Revision: 121 $
 * $Date: 2002-06-17 08:33:11 +0200 (Mon, 17 Jun 2002) $
 * $Author: relnev $
 *
 *
 * $Log$
 * Revision 1.4  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.3  2002/06/09 04:41:22  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/03 13:34:33  theoddone33
 * More stuff compiles
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 3     7/08/99 10:53a Dave
 * New multiplayer interpolation scheme. Not 100% done yet, but still
 * better than the old way.
 * 
 * 2     7/06/99 4:24p Dave
 * Mid-level checkin. Starting on some potentially cool multiplayer
 * smoothness crap.
 *  
 *
 * $NoKeywords: $
 */

#include "vecmat.h"
#include "2d.h"
#include "3d.h"
#include "alphacolors.h"
#include "spline.h"

// -------------------------------------------------------------------------------------------------
// SPLINE DEFINES/VARS
//


// -------------------------------------------------------------------------------------------------
// SPLINE FUNCTIONS
//

// integer factorial. =o
int bez_fact(int n)
{
	int idx;
	int product = 1;

	// do eet
	for(idx=1; idx<=n; idx++){
		product *= idx;
	}

	return product;
}

// bez constructor
bez_spline::bez_spline()
{
	int idx;

	// zero all points
	for(idx=0; idx<MAX_BEZ_PTS; idx++){
		pts[idx] = vmd_zero_vector;		
	}
	num_pts = 0;
}

// bez constructor
bez_spline::bez_spline(int _num_pts, vector *_pts[MAX_BEZ_PTS])
{
	bez_set_points(_num_pts, _pts);	
}

// set control points
void bez_spline::bez_set_points(int _num_pts, vector *_pts[MAX_BEZ_PTS])
{
	int idx;

	// store the points
	num_pts = _num_pts;
	for(idx=0; idx<_num_pts; idx++){
		Assert(_pts[idx] != NULL);
		if(_pts[idx] != NULL){
			pts[idx] = *_pts[idx];
		}
	}
}

// blend function
#define COMB(_n, _k)		( (float)bez_fact(_n) / (float)(bez_fact(_k) * bez_fact(_n - _k)) )
float bez_spline::BEZ(int k, int n, float u)
{
	float a = (float)COMB(n, k);
	float b = (float)pow(u, (float)k);
	float c = (float)pow(1.0f - u, (float)(n - k));

	return a * b * c;
}

// get a point on the curve
void bez_spline::bez_get_point(vector *out, float u)
{	
	int idx;
	float bez_val;

	Assert(out != NULL);
	if(out == NULL){
		return;
	}

	// calc
	out->xyz.x = 0.0f;
	out->xyz.y = 0.0f;
	out->xyz.z = 0.0f;
	for(idx=0; idx<num_pts; idx++){
		// bez val
		bez_val = BEZ(idx, num_pts-1, u);

		// x component
		out->xyz.x += pts[idx].xyz.x * bez_val;

		// y component
		out->xyz.y += pts[idx].xyz.y * bez_val;

		// z component
		out->xyz.z += pts[idx].xyz.z * bez_val;
	}
}	

// render a bezier
void bez_spline::bez_render(int divs, color *c)
{
	float inc;
	int idx;
	vertex a, b;
	vector pt;

	// bleh
	if(divs <= 0){
		return;
	}
	inc = 1.0f / (float)divs;

	// draw in red
	gr_set_color_fast(c);

	// draw that many divisions
	bez_get_point(&pt, 0.0f);
	g3_rotate_vertex(&a, &pt);
	for(idx=1; idx<=divs; idx++){
		// second point
		bez_get_point(&pt, (float)idx * inc);
		g3_rotate_vertex(&b, &pt);

		// draw the line
		g3_draw_line(&a, &b);

		// store b
		a = b;
	}

	// draw the control points
	gr_set_color_fast(&Color_bright_green);
	for(idx=0; idx<num_pts; idx++){
		g3_draw_sphere_ez(&pts[idx], 0.75f);
	}
}


// --------------------------------------------------------------------------
// HERMITE splines

// constructor
herm_spline::herm_spline()
{
	int idx;

	// zero all points
	for(idx=0; idx<MAX_HERM_PTS; idx++){
		pts[idx] = vmd_zero_vector;
		d_pts[idx] = vmd_zero_vector;
	}
	num_pts = 0;
}

// constructor
herm_spline::herm_spline(int _num_pts, vector *_pts[MAX_HERM_PTS], vector *_d_pts[MAX_HERM_PTS])
{	
	herm_set_points(_num_pts, _pts, _d_pts);
}

// set the points
void herm_spline::herm_set_points(int _num_pts, vector *_pts[MAX_HERM_PTS], vector *_d_pts[MAX_HERM_PTS])
{
	int idx;

	// store the points
	num_pts = _num_pts;
	for(idx=0; idx<_num_pts; idx++){
		Assert(_pts[idx] != NULL);
		if(_pts[idx] != NULL){
			pts[idx] = *_pts[idx];
		}
		Assert(_d_pts[idx] != NULL);
		if(_d_pts[idx] != NULL){
			d_pts[idx] = *_d_pts[idx];
		}
	}
}
	
// get a point on the hermite curve.
void herm_spline::herm_get_point(vector *out, float u, int k)
{
	float a = ( (2.0f * u * u * u) - (3.0f * u * u) + 1 );
	float b = ( (-2.0f * u * u * u) + (3.0f * u * u) );
	float c = ( (u * u * u) - (2.0f * u * u) + u );
	float d = ( (u * u * u) - (u * u) );

	vector va;
	vm_vec_copy_scale(&va, &pts[k], a);

	vector vb;
	vm_vec_copy_scale(&vb, &pts[k+1], b);

	vector vc;
	vm_vec_copy_scale(&vc, &d_pts[k], c);

	vector vd;
	vm_vec_copy_scale(&vd, &d_pts[k+1], d);

	vm_vec_add(out, &va, &vb);
	vm_vec_add2(out, &vc);
	vm_vec_add2(out, &vd);
}

// the derivative of a point on the hermite curve
void herm_spline::herm_get_deriv(vector *deriv, float u, int k)
{
	float a = ( (6.0f * u * u) - (6.0f * u) );
	float b = ( (-6.0f * u * u) + (6.0f * u) );
	float c = ( (3.0f * u * u) - (4.0f * u) + 1 );
	float d = ( (3.0f * u * u) - (2.0f * u) );

	vector va;
	vm_vec_copy_scale(&va, &pts[k], a);

	vector vb;
	vm_vec_copy_scale(&vb, &pts[k+1], b);

	vector vc;
	vm_vec_copy_scale(&vc, &d_pts[k], c);

	vector vd;
	vm_vec_copy_scale(&vd, &d_pts[k+1], d);

	vm_vec_add(deriv, &va, &vb);
	vm_vec_add2(deriv, &vc);
	vm_vec_add2(deriv, &vd);
}

// render a bezier
void herm_spline::herm_render(int divs, color *clc)
{
	int idx;
	int s_idx;
	float inc = 1.0f / (float)divs;

	vertex a, b, c;
	vector pt, d_pt;

	// draw in red
	gr_set_color_fast(clc);

	// render each section
	for(idx=0; idx<num_pts-1; idx++){
		// render this piece
		herm_get_point(&pt, 0.0f, idx);		
		g3_rotate_vertex(&a, &pt);
		
		// draw the deriv
		herm_get_deriv(&d_pt, 0.0f, idx);
		vm_vec_add2(&d_pt, &pt);
		g3_rotate_vertex(&c, &d_pt);
		g3_draw_line(&a, &c);

		for(s_idx=1; s_idx<divs * 2; s_idx++){
			// second point
			herm_get_point(&pt, (float)s_idx * inc, idx);			
			
			// 2nd point on the line
			g3_rotate_vertex(&b, &pt);

			// draw the line
			g3_draw_line(&a, &b);

			// draw the deriv line
			herm_get_deriv(&d_pt, (float)s_idx * inc, idx);			
			vm_vec_add2(&d_pt, &pt);
			g3_rotate_vertex(&c, &d_pt);
			g3_draw_line(&b, &c);

			// store b
			a = b;
		}		
	}	

	// draw the control points
	gr_set_color_fast(&Color_bright_green);
	for(idx=0; idx<num_pts; idx++){
		g3_draw_sphere_ez(&pts[idx], 0.75f);
	}
}

