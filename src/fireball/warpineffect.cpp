/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Fireball/WarpInEffect.cpp $
 * $Revision: 246 $
 * $Date: 2004-09-20 03:31:45 +0200 (Mon, 20 Sep 2004) $
 * $Author: theoddone33 $
 *
 * Code for rendering the warp in effects for ships
 *
 * $Log$
 * Revision 1.4  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.3  2002/06/17 06:33:08  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.2  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 3     7/22/99 1:22p Dave
 * Enable proper zbuffering for warpin glow effect.
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 23    4/08/98 8:20p John
 * Made "Apex" of warp effect not move.
 * 
 * 22    3/30/98 4:02p John
 * Made machines with < 32 MB of RAM use every other frame of certain
 * bitmaps.   Put in code to keep track of how much RAM we've malloc'd.
 * 
 * 21    3/29/98 12:39p John
 * Made warp in glow page in
 * 
 * 20    3/26/98 5:21p John
 * Added new code to preload all bitmaps at the start of a level.
 * Commented it out, though.
 * 
 * 19    3/18/98 12:36p John
 * Made hardware have nicer looking warp effect
 * 
 * 18    3/16/98 4:51p John
 * Added low-level code to clip all polygons against an arbritary plane.
 * Took out all old model_interp_zclip and used this new method instead.  
 * 
 * 17    3/10/98 4:18p John
 * Cleaned up graphics lib.  Took out most unused gr functions.   Made D3D
 * & Glide have popups and print screen.  Took out all >8bpp software
 * support.  Made Fred zbuffer.  Made zbuffer allocate dynamically to
 * support Fred.  Made zbuffering key off of functions rather than one
 * global variable.
 * 
 * 16    3/04/98 7:07p John
 * Added debug code to try to normalize a zerolength vector.
 * 
 * 15    2/26/98 3:28p John
 * fixed optimize compiler warning
 * 
 * 14    2/24/98 6:36p John
 * Made warp effect draw as a 4 poly cone.
 * 
 * 13    2/22/98 12:19p John
 * Externalized some strings
 * 
 * 12    1/15/98 9:07p John
 * Added noise to warp effect glow.
 * 
 * 11    1/15/98 4:58p John
 * Made warp effect use a looping ani.  Made the scaling up & down be in
 * software.
 * 
 * 10    12/30/97 6:44p John
 * Made g3_Draw_bitmap functions account for aspect of bitmap.
 * 
 * 9     12/08/97 11:15a John
 * added parameter to warpout for life.
 * 
 * 8     12/05/97 3:46p John
 * made ship thruster glow scale instead of being an animation.
 * 
 * 7     12/02/97 3:59p John
 * Added first rev of thruster glow, along with variable levels of
 * translucency, which retquired some restructing of palman.
 * 
 * 6     10/24/97 12:18p John
 * sped up warp effect by decreasing number of polys with distance.
 * 
 * 5     9/15/97 5:45p John
 * took out chunk stuff.
 * made pofview display thrusters as blue polies.
 * 
 * 4     9/12/97 4:02p John
 * put in ship warp out effect.
 * put in dynamic lighting for warp in/out
 * 
 * 3     9/09/97 4:49p John
 * Almost done ship warp in code
 * 
 * 2     9/08/97 8:39a John
 * added in code structure for grid
 * 
 * 1     9/05/97 10:07a John
 *
 * $NoKeywords: $
 */


#include "vecmat.h"
#include "tmapper.h"
#include "2d.h"
#include "3d.h"
#include "bmpman.h"
#include "model.h"
#include "key.h"
#include "physics.h"
#include "floating.h"
#include "model.h"
#include "lighting.h"
#include "object.h"
#include "ship.h"
#include "systemvars.h"
#include "animplay.h"
#include "fireballs.h"
#include "linklist.h"
#include "timer.h"

DCF(norm,"normalize a zero length vector")
{
	if ( Dc_command )	{
		vector tmp = vmd_zero_vector;
		vm_vec_normalize(&tmp);
	}
}

void draw_face( vertex *v1, vertex *v2, vertex *v3 )
{
	vector norm;
	vertex *vertlist[3];

	vm_vec_perp(&norm,(vector *)&v1->x,(vector *)&v2->x,(vector *)&v3->x);
	if ( vm_vec_dot(&norm,(vector *)&v1->x ) >= 0.0 )	{
		vertlist[0] = v3;
		vertlist[1] = v2;
		vertlist[2] = v1;
	} else {
		vertlist[0] = v1;
		vertlist[1] = v2;
		vertlist[2] = v3;
	}

	g3_draw_poly( 3, vertlist, TMAP_FLAG_TEXTURED );

}

void warpin_render(matrix *orient, vector *pos, int texture_bitmap_num, float radius, float life_percent, float max_radius )
{
	int i;

	int saved_gr_zbuffering = gr_zbuffer_get();

//	gr_zbuffering = 0;

	gr_set_bitmap( texture_bitmap_num, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1 );

	float Grid_depth = radius/2.5f;

	vector center;

	vm_vec_scale_add( &center, pos, &orient->v.fvec, -(max_radius/2.5f)/3.0f );

	vector vecs[5];
	vertex verts[5];

	vm_vec_scale_add( &vecs[0], &center, &orient->v.uvec, radius );
	vm_vec_scale_add2( &vecs[0], &orient->v.rvec, -radius );
	vm_vec_scale_add2( &vecs[0], &orient->v.fvec, Grid_depth );

	vm_vec_scale_add( &vecs[1], &center, &orient->v.uvec, radius );
	vm_vec_scale_add2( &vecs[1], &orient->v.rvec, radius );
	vm_vec_scale_add2( &vecs[1], &orient->v.fvec, Grid_depth );

	vm_vec_scale_add( &vecs[2], &center, &orient->v.uvec, -radius );
	vm_vec_scale_add2( &vecs[2], &orient->v.rvec, radius );
	vm_vec_scale_add2( &vecs[2], &orient->v.fvec, Grid_depth );

	vm_vec_scale_add( &vecs[3], &center, &orient->v.uvec, -radius );
	vm_vec_scale_add2( &vecs[3], &orient->v.rvec, -radius );
	vm_vec_scale_add2( &vecs[3], &orient->v.fvec, Grid_depth );

//	vm_vec_scale_add( &vecs[4], &center, &orient->fvec, -Grid_depth );
	vecs[4] = center;
			
	verts[0].u = 0.01f; verts[0].v = 0.01f; 
	verts[1].u = 0.99f; verts[1].v = 0.01f; 
	verts[2].u = 0.99f; verts[2].v = 0.99f; 
	verts[3].u = 0.01f; verts[3].v = 0.99f; 
	verts[4].u = 0.5f; verts[4].v = 0.5f; 

	for (i=0; i<5; i++ )	{
		g3_rotate_vertex( &verts[i], &vecs[i] );
	}

	draw_face( &verts[0], &verts[4], &verts[1] );
	draw_face( &verts[1], &verts[4], &verts[2] );
	draw_face( &verts[4], &verts[3], &verts[2] );
	draw_face( &verts[0], &verts[3], &verts[4] );

	if ( Warp_glow_bitmap != -1 )	{
		gr_set_bitmap( Warp_glow_bitmap, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1 );

		float r = radius;

		int render_it;
		
		#define OUT_PERCENT1 0.80f
		#define OUT_PERCENT2 0.90f

		#define IN_PERCENT1 0.10f
		#define IN_PERCENT2 0.20f

		if ( life_percent < IN_PERCENT1 )	{
			// do nothing
			render_it = 0;
		} else if ( life_percent < IN_PERCENT2 )	{
			r *= ( life_percent-IN_PERCENT1 ) / (IN_PERCENT2-IN_PERCENT1); 
			render_it = 1;
		} else if ( life_percent < OUT_PERCENT1 )	{
			// do nothing
			render_it = 1;
		} else if ( life_percent < OUT_PERCENT2 )	{
			r *= (OUT_PERCENT2 - life_percent) / (OUT_PERCENT2-OUT_PERCENT1);
			render_it = 1;
		} else {
			// do nothing
			render_it = 0;
		}

		if (render_it)	{
			int saved_gr_zbuffering = gr_zbuffer_get();
			gr_zbuffer_set(GR_ZBUFF_READ);

			// Add in noise 
			//float Noise[NOISE_NUM_FRAMES] = { 
			int noise_frame = fl2i(Missiontime/15.0f) % NOISE_NUM_FRAMES;

			r *= (0.40f + Noise[noise_frame]*0.30f);
						
			g3_draw_bitmap( &verts[4], 0,r, TMAP_FLAG_TEXTURED );
			gr_zbuffer_set(saved_gr_zbuffering);
		}
	}

	gr_zbuffer_set( saved_gr_zbuffering );
}







