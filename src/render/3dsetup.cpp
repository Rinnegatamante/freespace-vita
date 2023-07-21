/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Render/3dSetup.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Code to setup matrix instancing and viewers
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
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 23    5/24/98 11:32a John
 * safely handled null vector being passed to start_user_clip_plane.
 * 
 * 22    3/18/98 4:53p John
 * Fixed some bugs with docked ships warping out
 * 
 * 21    3/18/98 4:33p John
 * Called vm_vec_normalize_safe to prevent assert from bogus docked
 * objects.
 * 
 * 20    3/16/98 5:02p John
 * Better comments
 * 
 * 19    3/16/98 4:51p John
 * Added low-level code to clip all polygons against an arbritary plane.
 * Took out all old model_interp_zclip and used this new method instead.  
 * 
 * 18    3/10/98 4:19p John
 * Cleaned up graphics lib.  Took out most unused gr functions.   Made D3D
 * & Glide have popups and print screen.  Took out all >8bpp software
 * support.  Made Fred zbuffer.  Made zbuffer allocate dynamically to
 * support Fred.  Made zbuffering key off of functions rather than one
 * global variable.
 * 
 * 17    1/19/98 6:15p John
 * Fixed all my Optimized Build compiler warnings
 * 
 * 16    11/24/97 12:04p John
 * 
 * 15    11/07/97 7:24p John
 * changed lighting to take two ranges.
 * In textest, added test code to draw nebulas
 * 
 * 14    10/29/97 5:05p John
 * Changed dynamic lighting to only rotate and calculate lighting for
 * point lights that are close to an object.  Changed lower framerate cap
 * from 4 to .5.
 * 
 * 13    4/29/97 12:24p Adam
 * JAS:   Added code for delayed point to vec.   Fixed some FRED
 * sequencing problems with g3_start_frame / g3_end_frame.
 * 
 * 12    4/29/97 9:55a John
 * 
 * 11    4/08/97 5:18p John
 * First rev of decent (dynamic, correct) lighting in FreeSpace.
 * 
 * 10    3/24/97 3:26p John
 * Cleaned up and restructured model_collide code and fvi code.  In fvi
 * made code that finds uvs work..  Added bm_get_pixel to BmpMan.
 * 
 * 9     3/10/97 2:25p John
 * Made pofview zbuffer.   Made textest work with new model code.  Took
 * out some unnecessary Asserts in the 3d clipper.
 * 
 * 
 * 8     2/17/97 5:18p John
 * Added a bunch of RCS headers to a bunch of old files that don't have
 * them.
 *
 * $NoKeywords: $
 */


#include "3dinternal.h"
#include "tmapper.h"
#include "2d.h"			// Needed for w,h,aspect of canvas
#include "lighting.h"


matrix		View_matrix;		// The matrix to convert local coordinates to screen
vector		View_position;		// The offset to convert local coordinates to screen
matrix		Unscaled_matrix;	// View_matrix before scaling

matrix		Light_matrix;		// Used to rotate world points into current local coordinates
vector		Light_base;			// Used to rotate world points into current local coordinates

matrix		Eye_matrix;			// Where the viewer's eye is pointing in World coordinates
vector		Eye_position;		// Where the viewer's eye is at in World coordinates

float			View_zoom;			// The zoom factor

vector		Window_scale;		// Scaling for window aspect
vector		Matrix_scale;		// How the matrix is scaled, window_scale * zoom

int			Canvas_width;		// The actual width
int			Canvas_height;		// The actual height

float			Canv_w2;				// Canvas_width / 2
float			Canv_h2;				// Canvas_height / 2

//vertex buffers for polygon drawing and clipping
vertex * Vbuf0[TMAP_MAX_VERTS];
vertex * Vbuf1[TMAP_MAX_VERTS];

#define MAX_INSTANCE_DEPTH	5

struct instance_context {
	matrix m;
	vector p;
	matrix lm;
	vector lb;
} instance_stack[MAX_INSTANCE_DEPTH];

int instance_depth = 0;

int G3_count = 0;
int G3_frame_count = 0;

//start the frame
// Pass true for zbuffer_flag to turn on zbuffering
void g3_start_frame_func(int zbuffer_flag, const char * filename, int lineno)
{
	float s;
	int width, height;
	float aspect;

//Uncomment this to figure out who called g3_start_frame without calling g3_end_frame.
//	mprintf(( "g3_start_frame called from %s, line %d\n", filename, lineno ));

	Assert( G3_count == 0 );
	G3_count++;

	// Clear any user-defined clip planes
	g3_stop_user_clip_plane();

	// Get the values from the 2d...
	width = gr_screen.clip_width;
	height = gr_screen.clip_height;
	aspect = gr_screen.aspect;

	//set int w,h & fixed-point w,h/2
	Canvas_width = width;
	Canv_w2 = (float)width / 2.0f;
	Canvas_height = height;
	Canv_h2 = (float)height / 2.0f;
	
	//compute aspect ratio for this canvas

	s = aspect*(float)Canvas_height/(float)Canvas_width;

	if (s <= 0) {		//scale x
		Window_scale.xyz.x = s;
		Window_scale.xyz.y = 1.0f;
	}
	else {
		Window_scale.xyz.y = 1.0f / s;
		Window_scale.xyz.x = 1.0f;
	}
	
	Window_scale.xyz.z = 1.0f;		//always 1

	init_free_points();

	if (zbuffer_flag)	{
		gr_zbuffer_clear(TRUE);
	} else {
		gr_zbuffer_clear(FALSE);
	}

	G3_frame_count++;

	//init_interface_vars_to_assembler();		//for the texture-mapper

}

//this doesn't do anything, but is here for completeness
void g3_end_frame(void)
{
	G3_count--;
	Assert( G3_count == 0 );

	free_point_num = 0;
//	Assert(free_point_num==0);
}


void scale_matrix(void);

//set view from x,y,z, viewer matrix, and zoom.  Must call one of g3_set_view_*()
void g3_set_view_matrix(vector *view_pos,matrix *view_matrix,float zoom)
{
	Assert( G3_count == 1 );

	View_zoom = zoom;
	View_position = *view_pos;

	View_matrix = *view_matrix;

	Eye_matrix = View_matrix;
	Eye_position = *view_pos;

	scale_matrix();

	Light_matrix = vmd_identity_matrix;
	Light_base.xyz.x = 0.0f;
	Light_base.xyz.y = 0.0f;
	Light_base.xyz.z = 0.0f;

}


//set view from x,y,z & p,b,h, zoom.  Must call one of g3_set_view_*()
void g3_set_view_angles(vector *view_pos,angles *view_orient,float zoom)
{
	matrix tmp;

	Assert( G3_count == 1 );

	vm_angles_2_matrix(&tmp,view_orient);
	g3_set_view_matrix(view_pos,&tmp,zoom);
}


//performs aspect scaling on global view matrix
void scale_matrix(void)
{
	Unscaled_matrix = View_matrix;		//so we can use unscaled if we want

	Matrix_scale = Window_scale;

	if (View_zoom <= 1.0) 		//zoom in by scaling z

		Matrix_scale.xyz.z =  Matrix_scale.xyz.z*View_zoom;

	else {			//zoom out by scaling x&y

		float s = (float)1.0 / View_zoom;

		Matrix_scale.xyz.x = Matrix_scale.xyz.x*s;
		Matrix_scale.xyz.y = Matrix_scale.xyz.y*s;
	}

	//now scale matrix elements

	vm_vec_scale(&View_matrix.v.rvec,Matrix_scale.xyz.x);
	vm_vec_scale(&View_matrix.v.uvec,Matrix_scale.xyz.y);
	vm_vec_scale(&View_matrix.v.fvec,Matrix_scale.xyz.z);
}

ubyte g3_rotate_vertex_popped(vertex *dest,vector *src)
{
	vector tempv;

	Assert( G3_count == 1 );

	Assert( instance_depth > 0 );

	vm_vec_sub(&tempv,src,&instance_stack[0].p);
	vm_vec_rotate( (vector *)&dest->x, &tempv, &instance_stack[0].m );
	dest->flags = 0;	//not projected
	return g3_code_vertex(dest);
}	


//instance at specified point with specified orientation
//if matrix==NULL, don't modify matrix.  This will be like doing an offset   
//if pos==NULL, no position change
void g3_start_instance_matrix(vector *pos,matrix *orient)
{
	vector tempv;
	matrix tempm,tempm2;

	Assert( G3_count == 1 );

	Assert(instance_depth<MAX_INSTANCE_DEPTH);

	instance_stack[instance_depth].m = View_matrix;
	instance_stack[instance_depth].p = View_position;
	instance_stack[instance_depth].lm = Light_matrix;
	instance_stack[instance_depth].lb = Light_base;
	instance_depth++;

	// Make sure orient is valid
	if (!orient) {
		orient = &vmd_identity_matrix;		// Assume no change in orient
	}

	if ( pos )	{
		//step 1: subtract object position from view position
		vm_vec_sub2(&View_position,pos);

		//step 2: rotate view vector through object matrix
		vm_vec_rotate(&tempv,&View_position,orient);
		View_position = tempv;
	} else {
		// No movement, leave View_position alone
	}

	//step 3: rotate object matrix through view_matrix (vm = ob * vm)
	vm_copy_transpose_matrix(&tempm2,orient);

	vm_matrix_x_matrix(&tempm,&tempm2,&View_matrix);
	View_matrix = tempm;


	// Update the lighting matrix
	matrix saved_orient = Light_matrix;
	vector saved_base = Light_base;
	
	if ( pos )	{
		vm_vec_unrotate(&Light_base,pos,&saved_orient );
		vm_vec_add2(&Light_base, &saved_base );
	} else {
		// No movement, light_base doesn't change.
	}

	vm_matrix_x_matrix(&Light_matrix,&saved_orient, orient);

}


//instance at specified point with specified orientation
//if angles==NULL, don't modify matrix.  This will be like doing an offset
void g3_start_instance_angles(vector *pos,angles *orient)
{
	matrix tm;

	Assert( G3_count == 1 );

	if (orient==NULL) {
		g3_start_instance_matrix(pos,NULL);
		return;
	}

	vm_angles_2_matrix(&tm,orient);

	g3_start_instance_matrix(pos,&tm);

}


//pops the old context
void g3_done_instance()
{
	Assert( G3_count == 1 );

	instance_depth--;

	Assert(instance_depth >= 0);

	View_position = instance_stack[instance_depth].p;
	View_matrix = instance_stack[instance_depth].m;
	Light_matrix = instance_stack[instance_depth].lm;
	Light_base = instance_stack[instance_depth].lb;

}

int G3_user_clip = 0;
vector G3_user_clip_normal;
vector G3_user_clip_point;

// Enables clipping with an arbritary plane.   This will be on
// until g3_stop_clip_plane is called or until next frame.
// The points passed should be relative to the instance.  Probably
// that means world coordinates.
/*
  This works like any other clip plane... if this is enabled and you
rotate a point, the CC_OFF_USER bit will be set in the clipping codes.
It is completely handled by most g3_draw primitives, except maybe lines.

  As far as performance, when enabled, it will slow down each point
rotation (or g3_code_vertex call) by a vector subtraction and dot
product.   It won't slow anything down for polys that are completely
clipped on or off by the plane, and will slow each clipped polygon by
not much more than any other clipping we do.
*/
void g3_start_user_clip_plane( vector *plane_point, vector *plane_normal )
{
	float mag = vm_vec_mag( plane_normal );
	if ( (mag < 0.1f) || (mag > 1.5f ) )	{
		// Invalid plane_normal passed in.  Get Allender (since it is
		// probably a ship warp in bug:) or John.   
		Int3();			
		return;
	}

	G3_user_clip = 1;
//	G3_user_clip_normal = *plane_normal;
//	G3_user_clip_point = *plane_point;
//	return;

	vm_vec_rotate(&G3_user_clip_normal, plane_normal, &View_matrix );
	vm_vec_normalize(&G3_user_clip_normal);

	vector tempv;
	vm_vec_sub(&tempv,plane_point,&View_position);
	vm_vec_rotate(&G3_user_clip_point,&tempv,&View_matrix );
}

// Stops arbritary plane clipping
void g3_stop_user_clip_plane()
{
	G3_user_clip = 0;
}

// Returns TRUE if point is behind user plane
int g3_point_behind_user_plane( vector *pnt )
{
	if ( G3_user_clip ) {
		vector tmp;
		vm_vec_sub( &tmp, pnt, &G3_user_clip_point );
		if ( vm_vec_dot( &tmp, &G3_user_clip_normal ) <= 0.0f )	{
			return 1;
		}
	}

	return 0;
}




