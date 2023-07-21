/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Render/3dMath.cpp $
 * $Revision: 121 $
 * $Date: 2002-06-17 08:33:11 +0200 (Mon, 17 Jun 2002) $
 * $Author: relnev $
 *
 * 3d Math routines used by the Renderer lib
 *
 * $Log$
 * Revision 1.3  2002/06/17 06:33:10  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.2  2002/06/09 04:41:25  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 7     7/24/99 4:19p Dave
 * Fixed dumb code with briefing bitmaps. Made d3d zbuffer work much
 * better. Made model code use zbuffer more intelligently.
 * 
 * 6     3/19/99 9:51a Dave
 * Checkin to repair massive source safe crash. Also added support for
 * pof-style nebulae, and some new weapons code.
 * 
 * 6     3/15/99 6:45p Daveb
 * Put in rough nebula bitmap support.
 * 
 * 5     1/29/99 12:47a Dave
 * Put in sounds for beam weapon. A bunch of interface screens (tech
 * database stuff).
 * 
 * 4     12/07/98 5:51p Dave
 * Finally got d3d fog working! Now we just need to tweak values.
 * 
 * 3     12/06/98 2:36p Dave
 * Drastically improved nebula fogging.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 21    3/16/98 4:51p John
 * Added low-level code to clip all polygons against an arbritary plane.
 * Took out all old model_interp_zclip and used this new method instead.  
 * 
 * 20    2/05/98 9:21p John
 * Some new Direct3D code.   Added code to monitor a ton of stuff in the
 * game.
 * 
 * 19    7/28/97 2:21p John
 * changed vecmat functions to not return src.  Started putting in code
 * for inline vector math.    Fixed some bugs with optimizer.
 * 
 * 18    7/03/97 1:32p John
 * took out wacky warp.  Tried to make rotation faster by inlining all its
 * functions and using temp variables to help the optimizer
 * 
 * 17    4/29/97 12:24p Adam
 * JAS:   Added code for delayed point to vec.   Fixed some FRED
 * sequencing problems with g3_start_frame / g3_end_frame.
 * 
 * 16    4/29/97 9:55a John
 * 
 * 15    3/24/97 3:26p John
 * Cleaned up and restructured model_collide code and fvi code.  In fvi
 * made code that finds uvs work..  Added bm_get_pixel to BmpMan.
 * 
 * 14    3/06/97 5:36p Mike
 * Change vec_normalize_safe() back to vec_normalize().
 * Spruce up docking a bit.
 * 
 * 13    3/06/97 10:56a Mike
 * Write error checking version of vm_vec_normalize().
 * Fix resultant problems.
 * 
 * 12    2/17/97 5:18p John
 * Added a bunch of RCS headers to a bunch of old files that don't have
 * them.
 *
 * $NoKeywords: $
 */

#include "2d.h"
#include "3dinternal.h"


#define MIN_Z 0.0f

//Codes a vector.  Returns the codes of a point.
ubyte g3_code_vector(vector * p)
{
	ubyte cc=0;

	if (p->xyz.x > p->xyz.z)
		cc |= CC_OFF_RIGHT;

	if (p->xyz.y > p->xyz.z)
		cc |= CC_OFF_TOP;

	if (p->xyz.x < -p->xyz.z)
		cc |= CC_OFF_LEFT;

	if (p->xyz.y < -p->xyz.z)
		cc |= CC_OFF_BOT;

	if (p->xyz.z < MIN_Z )
		cc |= CC_BEHIND;

	if ( G3_user_clip )	{
		// Check if behind user plane
		if ( g3_point_behind_user_plane(p))	{
			cc |= CC_OFF_USER;
		}
	}

	return cc;
}


//code a point.  fills in the p3_codes field of the point, and returns the codes
ubyte g3_code_vertex(vertex *p)
{
	ubyte cc=0;

	if (p->x > p->z)
		cc |= CC_OFF_RIGHT;

	if (p->y > p->z)
		cc |= CC_OFF_TOP;

	if (p->x < -p->z)
		cc |= CC_OFF_LEFT;

	if (p->y < -p->z)
		cc |= CC_OFF_BOT;

	if (p->z < MIN_Z )
		cc |= CC_BEHIND;

	if ( G3_user_clip )	{
		// Check if behind user plane
		if ( g3_point_behind_user_plane((vector *)&p->x))	{
			cc |= CC_OFF_USER;
		}
	}

	return p->codes = cc;

}

MONITOR( NumRotations );	

ubyte g3_rotate_vertex(vertex *dest,vector *src)
{
#if 0
	vector tempv;
	Assert( G3_count == 1 );
	vm_vec_sub(&tempv,src,&View_position);
	vm_vec_rotate( (vector *)&dest->x, &tempv, &View_matrix );
	dest->flags = 0;	//not projected
	return g3_code_vertex(dest);
#else
	float tx, ty, tz, x,y,z;
	ubyte codes;

	MONITOR_INC( NumRotations, 1 );	

	tx = src->xyz.x - View_position.xyz.x;
	ty = src->xyz.y - View_position.xyz.y;
	tz = src->xyz.z - View_position.xyz.z;

	x = tx * View_matrix.v.rvec.xyz.x;
	x += ty * View_matrix.v.rvec.xyz.y;
	x += tz * View_matrix.v.rvec.xyz.z;

	y = tx * View_matrix.v.uvec.xyz.x;
	y += ty * View_matrix.v.uvec.xyz.y;
	y += tz * View_matrix.v.uvec.xyz.z;

	z = tx * View_matrix.v.fvec.xyz.x;
	z += ty * View_matrix.v.fvec.xyz.y;
	z += tz * View_matrix.v.fvec.xyz.z;

	codes = 0;

	if (x > z)			codes |= CC_OFF_RIGHT;
	if (x < -z)			codes |= CC_OFF_LEFT;
	if (y > z)			codes |= CC_OFF_TOP;
	if (y < -z)			codes |= CC_OFF_BOT;
	if (z < MIN_Z )	codes |= CC_BEHIND;

	dest->x = x;
	dest->y = y;
	dest->z = z;

	if ( G3_user_clip )	{
		// Check if behind user plane
		if ( g3_point_behind_user_plane((vector *)&dest->x))	{
			codes |= CC_OFF_USER;
		}
	}

	dest->codes = codes;

	dest->flags = 0;	// not projected

	return codes;
#endif
}	


ubyte g3_rotate_faraway_vertex(vertex *dest,vector *src)
{	
	Assert( G3_count == 1 );

	MONITOR_INC( NumRotations, 1 );	

	vm_vec_rotate( (vector *)&dest->x, src, &View_matrix );
	dest->flags = 0;	//not projected
	return g3_code_vertex(dest);
}	


//rotates a point. returns codes.  does not check if already rotated
ubyte g3_rotate_vector(vector *dest,vector *src)
{
	vector tempv;

	Assert( G3_count == 1 );

	MONITOR_INC( NumRotations, 1 );	

	vm_vec_sub(&tempv,src,&View_position);
	vm_vec_rotate(dest,&tempv,&View_matrix);
	return g3_code_vector(dest);
}	
		
ubyte g3_project_vector(vector *p, float *sx, float *sy )
{
	float w;

	Assert( G3_count == 1 );

	if ( p->xyz.z <= MIN_Z ) return PF_OVERFLOW;

	w=1.0f / p->xyz.z;

	*sx = (Canvas_width + (p->xyz.x*Canvas_width*w))*0.5f;
	*sy = (Canvas_height - (p->xyz.y*Canvas_height*w))*0.5f;
	return PF_PROJECTED;
}

//projects a point. Checks for overflow.

int g3_project_vertex(vertex *p)
{
	float w;

	Assert( G3_count == 1 );

	if ( p->flags & PF_PROJECTED )
		return p->flags;

	//if ( p->z < MIN_Z ) {
	if ( p->z <= MIN_Z ) {
		p->flags |= PF_OVERFLOW;
	} else {
		// w = (p->z == 0.0f) ? 100.0f : 1.0f / p->z;
		w = 1.0f / p->z;
		p->sx = (Canvas_width + (p->x*Canvas_width*w))*0.5f;
		p->sy = (Canvas_height - (p->y*Canvas_height*w))*0.5f;

		if(gr_screen.mode == GR_GLIDE){
			if ( w > 61439.0f ) w = 61439.0f;
		} else {
			if ( w > 1.0f ) w = 1.0f;		
		}
		p->sw = w;
		p->flags |= PF_PROJECTED;
	}
	
	return p->flags;
}


//from a 2d point, compute the vector through that point
void g3_point_to_vec(vector *v,int sx,int sy)
{
	vector	tempv;

	Assert( G3_count == 1 );

	tempv.xyz.x =  ((float)sx - Canv_w2) / Canv_w2;
	tempv.xyz.y = -((float)sy - Canv_h2) / Canv_h2;
	tempv.xyz.z = 1.0f;

	tempv.xyz.x = tempv.xyz.x * Matrix_scale.xyz.z / Matrix_scale.xyz.x;
	tempv.xyz.y = tempv.xyz.y * Matrix_scale.xyz.z / Matrix_scale.xyz.y;

	vm_vec_normalize(&tempv);
	vm_vec_unrotate(v, &tempv, &Unscaled_matrix);
}

//from a 2d point, compute the vector through that point.
// This can be called outside of a g3_start_frame/g3_end_frame
// pair as long g3_start_frame was previously called.
void g3_point_to_vec_delayed(vector *v,int sx,int sy)
{
	vector	tempv;

	tempv.xyz.x =  ((float)sx - Canv_w2) / Canv_w2;
	tempv.xyz.y = -((float)sy - Canv_h2) / Canv_h2;
	tempv.xyz.z = 1.0f;

	tempv.xyz.x = tempv.xyz.x * Matrix_scale.xyz.z / Matrix_scale.xyz.x;
	tempv.xyz.y = tempv.xyz.y * Matrix_scale.xyz.z / Matrix_scale.xyz.y;

	vm_vec_normalize(&tempv);
	vm_vec_unrotate(v, &tempv, &Unscaled_matrix);
}

vector *g3_rotate_delta_vec(vector *dest,vector *src)
{
	Assert( G3_count == 1 );
	return vm_vec_rotate(dest,src,&View_matrix);
}

//	vms_vector tempv;
//	vms_matrix tempm;
//
//	tempv.xyz.x =  fixmuldiv(fixdiv((sx<<16) - Canv_w2,Canv_w2),Matrix_scale.xyz.z,Matrix_scale.xyz.x);
//	tempv.xyz.y = -fixmuldiv(fixdiv((sy<<16) - Canv_h2,Canv_h2),Matrix_scale.xyz.z,Matrix_scale.xyz.y);
//	tempv.xyz.z = f1_0;
//
//	vm_vec_normalize(&tempv);
//
//	vm_copy_transpose_matrix(&tempm,&Unscaled_matrix);
//
//	vm_vec_rotate(v,&tempv,&tempm);

/*

//from a 2d point, compute the vector through that point
void g3_point_2_vec(vector *v,int sx,int sy)
{
	vector tempv;
	matrix tempm;

	tempv.xyz.x =  fixmuldiv(fixdiv((sx<<16) - Canv_w2,Canv_w2),Matrix_scale.xyz.z,Matrix_scale.xyz.x);
	tempv.xyz.y = -fixmuldiv(fixdiv((sy<<16) - Canv_h2,Canv_h2),Matrix_scale.xyz.z,Matrix_scale.xyz.y);
	tempv.xyz.z = f1_0;

	vm_vec_normalize(&tempv);

	vm_copy_transpose_matrix(&tempm,&Unscaled_matrix);

	vm_vec_rotate(v,&tempv,&tempm);

}

//delta rotation functions
vms_vector *g3_rotate_delta_x(vms_vector *dest,fix dx)
{
	dest->x = fixmul(View_matrix.v.rvec.xyz.x,dx);
	dest->y = fixmul(View_matrix.v.uvec.xyz.x,dx);
	dest->z = fixmul(View_matrix.v.fvec.xyz.x,dx);

	return dest;
}

vms_vector *g3_rotate_delta_y(vms_vector *dest,fix dy)
{
	dest->x = fixmul(View_matrix.v.rvec.xyz.y,dy);
	dest->y = fixmul(View_matrix.v.uvec.xyz.y,dy);
	dest->z = fixmul(View_matrix.v.fvec.xyz.y,dy);

	return dest;
}

vms_vector *g3_rotate_delta_z(vms_vector *dest,fix dz)
{
	dest->x = fixmul(View_matrix.v.rvec.xyz.z,dz);
	dest->y = fixmul(View_matrix.v.uvec.xyz.z,dz);
	dest->z = fixmul(View_matrix.v.fvec.xyz.z,dz);

	return dest;
}



ubyte g3_add_delta_vec(g3s_point *dest,g3s_point *src,vms_vector *deltav)
{
	vm_vec_add(&dest->p3_vec,&src->p3_vec,deltav);

	dest->p3_flags = 0;		//not projected

	return g3_code_point(dest);
}
*/

// calculate the depth of a point - returns the z coord of the rotated point
float g3_calc_point_depth(vector *pnt)
{
	float q;

	q = (pnt->xyz.x - View_position.xyz.x) * View_matrix.v.fvec.xyz.x;
	q += (pnt->xyz.y - View_position.xyz.y) * View_matrix.v.fvec.xyz.y;
	q += (pnt->xyz.z - View_position.xyz.z) * View_matrix.v.fvec.xyz.z;

	return q;
}


