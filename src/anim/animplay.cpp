/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Anim/AnimPlay.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module for playing back anim files
 *
 * $Log$
 * Revision 1.7  2006/04/26 19:48:58  taylor
 * various big-endian fixes, mainly networking support related
 *
 * Revision 1.6  2005/08/12 08:55:13  taylor
 * sync up talking head fixes from FS2_Open code base (still not 100%)
 *
 * Revision 1.5  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.4  2004/06/11 00:27:06  tigital
 * byte-swapping changes for bigendian systems
 *
 * Revision 1.3  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 9     9/13/99 11:26p Andsager
 * Add debug code to check for poorly sized anis
 * 
 * 8     8/26/99 9:45a Dave
 * First pass at easter eggs and cheats.
 * 
 * 7     7/16/99 1:49p Dave
 * 8 bit aabitmaps. yay.
 * 
 * 6     7/13/99 1:15p Dave
 * 32 bit support. Whee!
 * 
 * 5     6/10/99 10:34a Dave
 * Removed unnecessary assert.
 * 
 * 4     11/30/98 1:07p Dave
 * 16 bit conversion, first run.
 * 
 * 3     10/22/98 6:14p Dave
 * Optimized some #includes in Anim folder. Put in the beginnings of
 * parse/localization support for externalized strings and tstrings.tbl
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 38    6/23/98 4:18p Hoffoss
 * Fixed some bugs with AC release build.
 * 
 * 37    5/18/98 5:59p Hoffoss
 * Made command briefing advanced now once the speech stops and animation
 * has fully played once, whichever is longer.
 * 
 * 36    5/14/98 6:29p Hoffoss
 * Fixed some warnings a release rebuild all turned up.
 * 
 * 35    5/07/98 3:11a Lawrance
 * Implement custom streaming code
 * 
 * 34    4/27/98 3:36p Dave
 * 
 * 33    3/25/98 8:43p Hoffoss
 * Changed anim_play() to not be so damn complex when you try and call it.
 * 
 * 32    2/05/98 9:21p John
 * Some new Direct3D code.   Added code to monitor a ton of stuff in the
 * game.
 * 
 * 31    1/19/98 11:37p Lawrance
 * Fixing Optimization build warnings
 * 
 * 30    1/19/98 3:09p Hoffoss
 * Only free an anim instance if it is actually playing.
 * 
 * 29    1/14/98 6:43p Lawrance
 * Add ref_count to anim struct, so we don't free multiple times
 * 
 * 28    12/30/97 6:44p John
 * Made g3_Draw_bitmap functions account for aspect of bitmap.
 * 
 * 27    12/27/97 2:35p John
 * Restructed some code so that if the memory-mapped file open fails, it
 * will still read it the old-fashioned, non-memory mapped way.
 * 
 * 26    12/24/97 9:10p Lawrance
 * take out some debugging statements
 * 
 * 25    12/24/97 8:57p Lawrance
 * Added anim_ignore_next_frametime()
 * 
 * 24    12/07/97 2:06p Dave
 * Removed some debug frame-checking code.
 * 
 * 23    12/06/97 2:55p Dave
 * 
 * 22    11/29/97 2:05p John
 * made g3_draw_bitmap and g3_draw_rotated bitmap take w&h, not w/2 & h/2,
 * like they used to incorrectly assume.   Added code to model to read in
 * thruster radius's.
 * 
 * 21    11/20/97 5:36p Dave
 * Hooked in a bunch of main hall changes (including sound). Made it
 * possible to reposition (rewind/ffwd) 
 * sound buffer pointers. Fixed animation direction change framerate
 * problem.
 * 
 * 20    11/20/97 4:33p Sandeep
 * ALAN: ensure instance->paused gets initialized
 * 
 * 19    11/20/97 4:12p Lawrance
 * when paused, don't increment time_elapsed
 * 
 * 18    11/19/97 8:28p Dave
 * Hooked in Main Hall screen. Put in Anim support for ping ponging
 * animations as well as general reversal of anim direction.
 * 
 * 17    9/11/97 4:17p Allender
 * use _MAX_PATH instead of MAX_FILENAME_LENGTH for anim_play since it is
 * called from MoviePlayer with full path
 * 
 * 16    9/11/97 10:49a Lawrance
 * improve anim_show_next_frame()
 * 
 * 15    9/10/97 4:59p Lawrance
 * improve comments to anim_play() function
 * 
 * 14    9/03/97 4:19p John
 * changed bmpman to only accept ani and pcx's.  made passing .pcx or .ani
 * to bm_load functions not needed.   Made bmpman keep track of palettes
 * for bitmaps not mapped into game palettes.
 * 
 * 13    8/30/97 2:11p Lawrance
 * allow animations to loop
 * 
 * 12    8/25/97 11:13p Lawrance
 * support framerate independent playback with the option of now advancing
 * more than one frame at a time
 * 
 * 11    8/22/97 8:20a Lawrance
 * display scrolling text properly, with indicator that text exitsts to be
 * scrolled
 * 
 * 10    8/21/97 5:11p Lawrance
 * frame numbering for ANI's now is from 0 -> total_frames-1.
 * 
 * 9     8/19/97 9:30a Lawrance
 * print out reason if file doesn't open right
 * 
 * 8     7/28/97 10:42p Lawrance
 * re-did interface to unpack_frame() to make more general
 * 
 * 7     7/21/97 11:41a Lawrance
 * make playback time of .ani files keyed of frametime
 * 
 * 6     7/20/97 6:57p Lawrance
 * supporting new RLE format
 * 
 * 5     7/11/97 11:54a John
 * added rotated 3d bitmaps.
 * 
 * 4     6/27/97 4:36p Lawrance
 * update pal translation table when gr_screen.signature changes
 * 
 * 3     6/26/97 3:00p Lawrance
 * fix bug with playing 3d anims
 * 
 * 2     6/26/97 12:12a Lawrance
 * supporting anti-aliased bitmap animations
 * 
 * 1     6/23/97 5:09p Lawrance
 * 
 * 24    6/03/97 5:53p Lawrance
 * don't unload bitmap after bm_create
 * 
 * $NoKeywords: $
 */

#include "animplay.h"
#include "linklist.h"
#include "timer.h"
#include "bmpman.h"
#include "2d.h"
#include "3d.h"
#include "grinternal.h"
#include "pcxutils.h"
#include "packunpack.h"
#include "cfile.h"

static color Color_xparent;

anim *first_anim = NULL;
anim_instance anim_free_list;
anim_instance anim_render_list;

#define MAX_ANIM_INSTANCES 25
anim_instance anim_render_instance[MAX_ANIM_INSTANCES];

int Anim_paused;	// global variable to pause the playing back of anims
int Anim_inited = FALSE;

fix t1,t2;

int Anim_ignore_frametime=0;	// flag used to ignore frametime... useful when need to avoid saturated frametimes

// -------------------------------------------------------------------------------------------------
// anim_init() will queue all the anim_render_instance[] elements onto the anim_free_list
//
void anim_init()
{
	int i;

	if ( Anim_inited == TRUE )
		return;

	list_init( &anim_free_list );
	list_init( &anim_render_list );

	// Link all anim render slots into the free list
	for (i=1; i < MAX_ANIM_INSTANCES; i++)	{
		list_append(&anim_free_list, &anim_render_instance[i]);
	}
	
	Anim_paused = 0;
	Anim_inited = TRUE;
}

// -------------------------------------------------------------------------------------------------
// anim_render_all() will display the frames for the currently playing anims
//
void anim_render_all(int screen_id, float frametime)
{
	anim_instance* A;
	anim_instance* temp;

	A = GET_FIRST(&anim_render_list);
	while( A !=END_OF_LIST(&anim_render_list) )	{
		temp = GET_NEXT(A);
		if ( A->screen_id == screen_id ) {
			if ( Anim_ignore_frametime ) {
				frametime = 0.0f;
				Anim_ignore_frametime=0;
			}
			if ( anim_show_next_frame(A, frametime) == -1 ) {
				A->data = NULL;
				anim_release_render_instance(A);	
			}
		}
		A = temp;
	}
}

// -------------------------------------------------------------------------------------------------
// anim_render_one() will display the frames for the passed animation, it will ignore animations which
// do not have the same id as the passed screen_id
//
void anim_render_one(int screen_id, anim_instance *ani, float frametime)
{
	// make sure this guy's screen id matches the passed one
	if(screen_id != ani->screen_id){
		return;
	}

	// otherwise render it	
	if ( Anim_ignore_frametime ) {
		frametime = 0.0f;
		Anim_ignore_frametime=0;
	}
	if ( anim_show_next_frame(ani, frametime) == -1 ) {
		ani->data = NULL;
		anim_release_render_instance(ani);	
	}	
}

MONITOR(NumANIPlayed);

// Setup an anim_play_struct for passing into anim_play().  Will fill in default values, which you
// can then change before calling anim_play().
//
void anim_play_init(anim_play_struct *aps, anim *a_info, int x, int y)
{
	aps->anim_info = a_info;
	aps->x = x;
	aps->y = y;
	aps->start_at = 0;
	aps->stop_at = a_info->total_frames - 1;
	aps->screen_id = 0;
	aps->world_pos = NULL;
	aps->radius = 0.0f;
	aps->framerate_independent = 0;
	aps->color = NULL;
	aps->skip_frames = 1;
	aps->looped = 0;
	aps->ping_pong = 0;
}

// -------------------------------------------------------------------------------------------------
// anim_play() will add an anim instance to the anim_render_list.  This will cause the
// anim to be played at the x,y position specified in the parameter list.
//
// input:
//
//		anim_info	=>	the compressed animation that we should make an instance from
//		x				=>	x position of animation to play at (top left corner)
//		y				=>	y position of animation to play at ( top left corner)
//		start_at		=>	frame number to start at (note: numbering is from 0->num_frames-1)
//		stop_at		=>	frame number to stop at (note: numbering is from 0->num_frames-1)
//		screen_id	=>	OPTIONAL (default value 0): screen signature so animation only plays when
//							anim_render_all() called with that same signature
//		world_pos	=>	OPTIONAL (default value NULL): only give a world pos when you want to
//							play the animation at a 3D location.  You must specify radius when
//							this is non-null.
//		radius		=>	OPTIONAL (default value 0): only needed when the animation is playing
//							as a 3D animation (this is only when world_pos in not NULL).
//		fi				=>	OPTIONAL (default value 0): framerate indepentdent flag, when set TRUE
//							the animation will skip frames if necessary to maintain the fps value
//							associated with the animation
//		color			=>	OPTIONAL (default value NULL) address of an alpha color struct.  Only
//							required when the animation should be drawn with an alpha color.
//		skip_frames	=> OPTIONAL (default value 1) should anim skip frames when doing framerate
//                   independent playback
//		looped		=>	OPTIONAL (default value 0) should anim play looped (ie forever)
//
// returns:
//
//		pointer to instance	=> success
//		NULL						=> if anim anim could not be played
//
anim_instance *anim_play(anim_play_struct *aps)
{
	Assert( aps->anim_info != NULL );
	Assert( aps->start_at >= 0 );
	Assert( aps->stop_at < aps->anim_info->total_frames );
	// Assert( aps->stop_at >= aps->start_at );
	Assert( !(aps->looped && aps->ping_pong) );  // shouldn't have these both set at once

	MONITOR_INC(NumANIPlayed, 1);
	
	// if (aps->ping_pong && !(aps->anim_info->flags & ANF_ALL_KEYFRAMES));

	anim_instance *instance;

	// Find next free anim instance slot on queue
	instance = GET_FIRST(&anim_free_list);
	Assert( instance != &anim_free_list );  // shouldn't have the dummy element

	// remove instance from the free list
	list_remove( &anim_free_list, instance );

	// insert instance onto the end of anim_render_list
	list_append( &anim_render_list, instance );

	aps->anim_info->instance_count++;
	instance->frame_num = -1;
	instance->last_frame_num = -99;
	instance->parent = aps->anim_info;
	instance->data = aps->anim_info->data;
	if ( anim_instance_is_streamed(instance) ) {
		instance->file_offset = instance->parent->file_offset;
	}
	instance->frame = (ubyte *) malloc(instance->parent->width * instance->parent->height * 2);
	Assert( instance->frame != NULL );
	instance->time_elapsed = 0.0f;
	instance->stop_at = aps->stop_at;
	instance->x = aps->x;
	instance->y = aps->y;
	instance->world_pos = aps->world_pos;
	instance->radius = aps->radius;
	instance->framerate_independent = aps->framerate_independent;
	instance->last_bitmap = -1;
	instance->stop_now = FALSE;
	instance->screen_id = aps->screen_id;
	instance->aa_color = aps->color;
	instance->skip_frames = aps->skip_frames;
	instance->looped = aps->looped;
	instance->ping_pong = aps->ping_pong;
	instance->direction = ANIM_DIRECT_FORWARD;
	instance->paused = 0;
	instance->loop_count = 0;
	if ( aps->color == NULL ){
		instance->xlate_pal = 1;
	} else {
		instance->xlate_pal = 0;
	}

	// determining the start_at frame is more complicated, since it must be a key-frame.
	// Futhermore, need to subtract 1 from key-frame number, since frame number is always
	// incremented the first time anim_show_next_frame() is called

	instance->start_at = aps->start_at;

	if ( aps->start_at > 0 ) {
		key_frame *keyp;
		int idx;
		int key = 0;
		int offset = 0;
		int frame_num = aps->start_at;

		keyp = instance->parent->keys;
		idx = 0;
		while (idx < instance->parent->num_keys) {
			if (key == frame_num)
				break;

			key = keyp[idx].frame_num - 1;
			offset = keyp[idx].offset;

			idx++;
		}
		/*while (keyp) {
			if (( (keyp->frame_num-1) <= frame_num) && ( (keyp->frame_num-1) > key)) {  // find closest key
				key = keyp->frame_num-1;
				offset = keyp->offset;
				if ( key == frame_num )
					break;
			}

			keyp = keyp->next;
		}*/

		if (key > instance->frame_num) {  // best key is closer than current position
			instance->frame_num = key;
			if ( anim_instance_is_streamed(instance) ) {
				instance->file_offset = instance->parent->file_offset + offset;
			} else {
				instance->data = instance->parent->data + offset;
			}

		}

		instance->frame_num--;	// required
	}

	return instance;
}

// -----------------------------------------------------------------------------
//	anim_show_next_frame()
//
//	This function is called to blit the next frame of an anim instance to the 
// screen.  This is normally called by the anim_render_all() function.
//
//	input:	instance		=>		pointer to animation instance
//				frametime	=>		time elapsed since last call, in seconds
//
int anim_show_next_frame(anim_instance *instance, float frametime)
{
	int		bitmap_id, bitmap_flags=0, new_frame_num, frame_diff=0, i, n_frames=0,frame_save;
	float		percent_through, decompress_time, render_time, time;
	vertex	image_vertex;
	int aabitmap = 0;
	int bpp = 16;

	Assert( instance != NULL );

	instance->time_elapsed += frametime;

	// Advance to the next frame, if we determine enough time has elapsed.
	if(instance->direction == ANIM_DIRECT_FORWARD)
		n_frames = instance->stop_at - instance->start_at + 1;
	else if(instance->direction == ANIM_DIRECT_REVERSE)
		n_frames = instance->start_at - instance->stop_at + 1;
	time = n_frames / i2fl(instance->parent->fps);

	percent_through = instance->time_elapsed / time;

	if(instance->direction == ANIM_DIRECT_FORWARD)
		new_frame_num = instance->start_at - 1 + fl2i(percent_through * n_frames + 0.5f);
	else
		new_frame_num = instance->start_at - 1 - fl2i(percent_through * n_frames + 0.5f);

	frame_save = instance->frame_num;

	// If framerate independent, use the new_frame_num... unless instance->skip_frames is
	// FALSE, then only advance a maximum of one frame (this is needed since some big animations
	// should just play slower rather than taking the hit of decompressing multiple frames and
	// creating an even greater slowdown	
	if (instance->framerate_independent) {
		if(instance->direction == ANIM_DIRECT_FORWARD){
			if ( new_frame_num > instance->last_frame_num) {
				if ( instance->skip_frames )
					instance->frame_num = new_frame_num;
				else
					instance->frame_num++;
			}
		} else if(instance->direction == ANIM_DIRECT_REVERSE){
			if( new_frame_num < instance->last_frame_num) {
				if ( instance->skip_frames )
					instance->frame_num = new_frame_num;
				else
					instance->frame_num--;
			}
		}
	}
	else {			
		if(instance->direction == ANIM_DIRECT_FORWARD){
			if ( new_frame_num > instance->last_frame_num) {
				instance->frame_num++;
			}
		} else if(instance->direction == ANIM_DIRECT_REVERSE){
			if ( new_frame_num < instance->last_frame_num) {
				instance->frame_num--;
			}
		}			
	}

	if(instance->direction == ANIM_DIRECT_FORWARD){
		if ( instance->frame_num < instance->start_at ) {
			instance->frame_num = instance->start_at;
		}	
	} else if(instance->direction == ANIM_DIRECT_REVERSE){
		if ( instance->frame_num > instance->start_at ) {
			instance->frame_num = instance->start_at;
		}	
	}

	if ( instance->stop_now == TRUE ) {
		return -1;
	}

	// If past the last frame, clamp to the last frame and then set the stop_now flag in the
	// anim instance.  The next iteration, the animation will stop.
	if(instance->direction == ANIM_DIRECT_FORWARD){
		if (instance->frame_num >= instance->stop_at ) {
			if (instance->looped) {										// looped animations
				instance->frame_num = instance->stop_at;
				instance->time_elapsed = 0.0f;
			} else if(instance->ping_pong) {							// pingponged animations
				instance->frame_num = instance->stop_at;
				// instance->time_elapsed = 0.0f;
				anim_reverse_direction(instance);
			} else {															// one-shot animations
				instance->frame_num = instance->stop_at;
				instance->last_frame_num = instance->frame_num;
				instance->stop_now = TRUE;
			}
		}
	} else if(instance->direction == ANIM_DIRECT_REVERSE){
		if (instance->frame_num <= instance->stop_at ) {
			if (instance->looped) {										// looped animations
				instance->frame_num = instance->stop_at;
				instance->time_elapsed = 0.0f;
			} else if(instance->ping_pong) {							// pingponged animations
				instance->frame_num = instance->stop_at;
				// instance->time_elapsed = 0.0f;
				anim_reverse_direction(instance);
			} else {															// one-shot animations
				instance->frame_num = instance->stop_at+1;
				instance->last_frame_num = instance->frame_num;
				instance->stop_now = TRUE;
			}
		}
	}

	if(instance->direction == ANIM_DIRECT_FORWARD){
		if( instance->last_frame_num >= instance->start_at ) {
			frame_diff = instance->frame_num - instance->last_frame_num;		
		} else {
			frame_diff = 1;
		}
	} else if(instance->direction == ANIM_DIRECT_REVERSE){
		if( instance->last_frame_num <= instance->start_at ) {
			frame_diff = instance->last_frame_num - instance->frame_num;
		} else {
			frame_diff = 1;
		}
	}		
	Assert(frame_diff >= 0);
	//	nprintf(("Alan","FRAME DIFF: %d\n",frame_diff));
	Assert( instance->frame_num >= 0 && instance->frame_num < instance->parent->total_frames );

	// if the anim is paused, ignore all the above changes and still display this frame
	if(instance->paused || Anim_paused){
		instance->frame_num = frame_save;
		instance->time_elapsed -= frametime;
		frame_diff = 0;
	}

	if (instance->parent->flags & ANF_XPARENT){
		// bitmap_flags = BMP_XPARENT;
		bitmap_flags = 0;
	} 
	bpp = 16;
	if(instance->aa_color != NULL){
		bitmap_flags |= BMP_AABITMAP;
		aabitmap = 1;
		bpp = 8;
	}	

	if ( frame_diff > 0 ) {
		instance->last_frame_num = instance->frame_num;		

		t1 = timer_get_fixed_seconds();
		for ( i = 0; i < frame_diff; i++ ) {
			anim_check_for_palette_change(instance);			

			// if we're playing backwards, every frame must be a keyframe and we set the data ptr here
			if(instance->direction == ANIM_DIRECT_REVERSE){
				if ( anim_instance_is_streamed(instance) ) {
					instance->file_offset = instance->parent->file_offset + instance->parent->keys[instance->frame_num-1].offset;
				} else {
					instance->data = instance->parent->data + instance->parent->keys[instance->frame_num-1].offset;
				}
			}

			ubyte *temp = NULL;
			int temp_file_offset = 0;			

			// if we're using bitmap polys
			if(Gr_bitmap_poly){
				BM_SELECT_TEX_FORMAT();
			}

			if ( anim_instance_is_streamed(instance) ) {
				if ( instance->xlate_pal ){
					temp_file_offset = unpack_frame_from_file(instance, instance->frame, instance->parent->width*instance->parent->height, instance->parent->palette_translation, aabitmap, bpp);
				} else {
					temp_file_offset = unpack_frame_from_file(instance, instance->frame, instance->parent->width*instance->parent->height, NULL, aabitmap, bpp);
				}
			} else {
				if ( instance->xlate_pal ){
					temp = unpack_frame(instance, instance->data, instance->frame, instance->parent->width*instance->parent->height, instance->parent->palette_translation, aabitmap, bpp);
				} else {
					temp = unpack_frame(instance, instance->data, instance->frame, instance->parent->width*instance->parent->height, NULL, aabitmap, bpp);
				}
			}

			// always go back to screen format
			BM_SELECT_SCREEN_FORMAT();

			if(instance->direction == ANIM_DIRECT_FORWARD){
				if ( anim_instance_is_streamed(instance) ) {
					instance->file_offset = temp_file_offset;
				} else {
					instance->data = temp;
				}
			}			
		}
		t2 = timer_get_fixed_seconds();
	}
	else {
		t2=t1=0;
	}

	// this only happens when the anim is being looped, we need to reset the last_frame_num
	if ( (instance->time_elapsed == 0) && (instance->looped) ) {
		instance->last_frame_num = -1;
		instance->frame_num = -1;
		instance->data = instance->parent->data;
		instance->file_offset = instance->parent->file_offset;
		instance->loop_count++;
	}
		
	decompress_time = f2fl(t2-t1);

	t1 = timer_get_fixed_seconds();
	if ( frame_diff == 0 && instance->last_bitmap != -1 ) {
		bitmap_id = instance->last_bitmap;
	}
	else {
		if ( instance->last_bitmap != -1 ){
			bm_release(instance->last_bitmap);
		}
		bitmap_id = bm_create(16, instance->parent->width, instance->parent->height, instance->frame, bitmap_flags);
	}
	
	if ( bitmap_id == -1 ) {
		// anim has finsished playing, free the instance frame data
		anim_release_render_instance(instance);	
		return -1;

		// NOTE: there is no need to free the instance, since it was pre-allocated as 
		//       part of the anim_free_list
	}
	else {
		gr_set_bitmap(bitmap_id, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		
		// determine x,y to display the bitmap at
		if ( instance->world_pos == NULL ) {
			if ( instance->aa_color == NULL ) {
				gr_bitmap(instance->x, instance->y);
			}
			else {
				gr_set_color_fast( (color*)instance->aa_color );
				gr_aabitmap(instance->x, instance->y);
			}
		}
		else {
			g3_rotate_vertex(&image_vertex,instance->world_pos);
			Assert(instance->radius != 0.0f);
			g3_draw_bitmap(&image_vertex, 0, instance->radius*1.5f, TMAP_FLAG_TEXTURED );
		}

		//bm_release(bitmap_id);
		instance->last_bitmap = bitmap_id;
	}

	t2 = timer_get_fixed_seconds();
	render_time = f2fl(t2-t1);

//	nprintf(("Alan","DECOMPRESS: %.3fms  RENDER: %.3fms\n", decompress_time*1000, render_time*1000));

	return 0;
}

// -----------------------------------------------------------------------------
//	anim_stop_playing()
//
//	Stop an anim instance that is on the anim_render_list from playing
//
int anim_stop_playing(anim_instance* instance)
{
	Assert(instance != NULL);

	if ( anim_playing(instance) ) {
		anim_release_render_instance(instance);
	}
	return 0;
}

// -----------------------------------------------------------------------------
//	anim_release_render_instance()
//
//	Free a particular animation instance that is on the anim_render_list.  Do
// not call this function to free an animation instance in general (use 
// free_anim_instance() for that), only when you want to free an instance
// that is on the anim_render_list
//
void anim_release_render_instance(anim_instance* instance)
{
	Assert( instance != NULL );
	Assert(instance->frame);
	free(instance->frame);
	instance->frame = NULL;
	instance->parent->instance_count--;

	if ( instance->last_bitmap != -1 ) {
		bm_release(instance->last_bitmap); 
		instance->last_bitmap = -1;
	}

	// remove instance from anim_render_list
	list_remove( &anim_render_list, instance );

	// insert instance into the anim_free_list
	list_append( &anim_free_list, instance );
}

// -----------------------------------------------------------------------------
//	anim_release_all_instances()
//
//	Free all anim instances that are on the anim_render_list.
//
//	input:	screen_id	=>		optional parameter that lets you only free a subset
//										of the anim instances.	A screen_id of 0 is the default
//										value, and this is used for animations that always play
//										when they are placed on the aim_render_list.
//
void anim_release_all_instances(int screen_id)
{
	anim_instance* A;
	anim_instance* temp;

	if ( Anim_inited == FALSE )
		return;

	A = GET_FIRST(&anim_render_list);
	while( A !=END_OF_LIST(&anim_render_list) )	{
		temp = GET_NEXT(A);
		if ( A->screen_id == screen_id || screen_id == 0 ) {
			anim_release_render_instance(A);
		}
		A = temp;
	}
}

// -----------------------------------------------------------------------------
//	anim_read_header()
//
// Read the header of a .ani file.  Below is the format of a .ani header
//
//	#bytes	|	description
//	2			|	obsolete, kept for compatibility with old versions
//	2			|	version number
//	2			|	fps
//	1			|	transparent red value
// 	1			|	transparent green value
//	1			|	transparent blue value
//	2			|	width
//	2			|	height
//	2			|	number of frames
//	2			|	packer code
//	763			|	palette
//	2			|	number of key frames
//	2			|	key frame number	}		repeats
//	4			|	key frame offset	}		repeats
//	4			|	compressed data length
//
void anim_read_header(anim *ptr, CFILE *fp)
{
	ptr->width = cfread_short(fp);
	// If first 2 bytes are zero, this means we are using a new format, which includes
	// a version, and fps values.  This is only done since a version number was not included
	// in the original header.

	// default
	Color_xparent.red = 0;
	Color_xparent.green = 255;
	Color_xparent.blue = 0;

	if ( ptr->width == 0 ) {
		ptr->version = cfread_short(fp);
		ptr->fps = cfread_short(fp);

		// version 2 added a custom transparency color
		if ( ptr->version >= 2 ) {
			cfread(&Color_xparent.red, 1, 1, fp);
			cfread(&Color_xparent.green, 1, 1, fp);
			cfread(&Color_xparent.blue, 1, 1, fp);
		}

		ptr->width = cfread_short(fp);
	}
	else {
		ptr->version = 0;
		ptr->fps = 30;
	}
	
	ptr->height = cfread_short(fp);

#ifndef NDEBUG
	// get size of ani compared to power of 2
	int r, floor_pow;
	r = ptr->height;
	floor_pow = 0;
	
	while(r >= 2) {
		r /= 2;
		floor_pow++;
	}

	int floor_size = (int) pow(2, floor_pow);
	int diff = ptr->height - floor_size;
	float waste = 100.0f * float((floor_size - diff))/(2.0f *(float)floor_size);

	if (diff != 0) {
		if (ptr->height > 16) {
			mprintf(("ANI %s with size %dx%d (%.1f%% wasted)\n", ptr->name, ptr->height, ptr->height, waste));
		}
	}
#endif

	ptr->total_frames = cfread_short(fp);
	ptr->packer_code = cfread_ubyte(fp);
	cfread(&ptr->palette, 256, 3, fp);
	ptr->num_keys = cfread_short(fp);

	// store xparent colors
	ptr->xparent_r = Color_xparent.red;
	ptr->xparent_g = Color_xparent.green;
	ptr->xparent_b = Color_xparent.blue;

	if(ptr->total_frames == ptr->num_keys){
		ptr->flags |= ANF_ALL_KEYFRAMES;
	}
}

// -----------------------------------------------------------------------------
//	anim_load()
//
// Load an animation.  This stores the compressed data, which instances
// of the animation can reference.  Must be free'ed later with anim_free()
//
// input:	name				=>		filename of animation
//				file_mapped		=>		boolean, whether to use memory-mapped file or not.
//											Memory-mapped files will page in the animation from disk
//											as it is needed, but performance is not as good
//
//	returns:	pointer to anim that is loaded	=> sucess
//				NULL										=>	failure
//
anim *anim_load(const char *real_filename, int file_mapped)
{
	anim			*ptr;
	CFILE			*fp;
	int			count,idx;
	char name[_MAX_PATH];

//	file_mapped = 0;

	Assert ( real_filename != NULL );

	strcpy( name, real_filename );
	char *p = strchr( name, '.' );
	if ( p ) {
		*p = 0;
	}
	strcat( name, ".ani" );

	ptr = first_anim;
	while (ptr) {
		if (!stricmp(name, ptr->name))
			break;

		ptr = ptr->next;
	}

	if (!ptr) {
		fp = cfopen(name, "rb");
		if ( !fp )
			return NULL;

		ptr = (anim *) malloc(sizeof(anim));
		Assert(ptr);

		ptr->flags = 0;
		ptr->next = first_anim;
		first_anim = ptr;
		Assert(strlen(name) < _MAX_PATH - 1);
		strcpy(ptr->name, name);
		ptr->instance_count = 0;
		ptr->width = 0;
		ptr->height = 0;
		ptr->total_frames = 0;
		ptr->keys = NULL;
		ptr->ref_count=0;

		anim_read_header(ptr, fp);

		if(ptr->num_keys > 0){
			ptr->keys = (key_frame*)malloc(sizeof(key_frame) * ptr->num_keys);
			Assert(ptr->keys != NULL);
		} 			

		// store how long the anim should take on playback (in seconds)
		ptr->time = i2fl(ptr->total_frames)/ptr->fps;

		for(idx=0;idx<ptr->num_keys;idx++){
			ptr->keys[idx].frame_num = 0;
			cfread(&ptr->keys[idx].frame_num, 2, 1, fp);
			cfread(&ptr->keys[idx].offset, 4, 1, fp);
            ptr->keys[idx].frame_num = INTEL_INT( ptr->keys[idx].frame_num );
            ptr->keys[idx].offset = INTEL_INT( ptr->keys[idx].offset );
		}

		/*prev_keyp = &ptr->keys;
		count = ptr->num_keys;
		while (count--) {
			keyp = (key_frame *) malloc(sizeof(key_frame));
			keyp->next = *prev_keyp;
			*prev_keyp = keyp;
			prev_keyp = &keyp->next;

			keyp->frame_num = 0;
			cfread(&keyp->frame_num, 2, 1, fp);
			cfread(&keyp->offset, 4, 1, fp);
		}*/
		cfread(&count, 4, 1, fp);	// size of compressed data
        count = INTEL_INT( count );

		ptr->cfile_ptr = NULL;

		if ( file_mapped ) {
			// Try mapping the file to memory 
			ptr->flags |= ANF_MEM_MAPPED;
			ptr->cfile_ptr = cfopen(name, "rb", CFILE_MEMORY_MAPPED);
		}

		// couldn't memory-map file... must be in a packfile, so stream manually
		if ( file_mapped && !ptr->cfile_ptr ) {
			ptr->flags &= ~ANF_MEM_MAPPED;
			ptr->flags |= ANF_STREAMED;
			ptr->cfile_ptr = cfopen(name, "rb");
		}

		ptr->cache = NULL;

		// If it opened properly as mem-mapped (or streamed)
		if (ptr->cfile_ptr != NULL)	{
			// VERY IMPORTANT STEP
			// Set the data pointer to the compressed data (which is not at the start of the
			// file).  Use ftell() to find out how far we've already parsed into the file
			//
			int offset;
			offset = cftell(fp);
			ptr->file_offset = offset;
			if ( ptr->flags & ANF_STREAMED ) {
				ptr->data = NULL;
				ptr->cache_file_offset = ptr->file_offset;
				ptr->cache = (ubyte*)malloc(ANI_STREAM_CACHE_SIZE+2);
				Assert(ptr->cache);
				cfseek(ptr->cfile_ptr, offset, CF_SEEK_SET);
				cfread(ptr->cache, ANI_STREAM_CACHE_SIZE, 1, ptr->cfile_ptr);
			} else {
				ptr->data = (ubyte*)cf_returndata(ptr->cfile_ptr) + offset;
			}
		} else {
			// Not a memory mapped file (or streamed)
			ptr->flags &= ~ANF_MEM_MAPPED;
			ptr->flags &= ~ANF_STREAMED;
			ptr->data = (ubyte *) malloc(count);
			ptr->file_offset = -1;
			cfread(ptr->data, count, 1, fp);
		}

		cfclose(fp);

		// store screen signature, so we can tell if palette changes
		ptr->screen_sig = gr_screen.signature;

		anim_set_palette(ptr);
	}

	ptr->ref_count++;
	return ptr;
}

// ---------------------------------------------------
//	anim_free()
//
// Free an animation that was loaded with anim_load().  All instances
// referencing this animation must be free'ed or get an assert.
//
int anim_free(anim *ptr)
{
	Assert ( ptr != NULL );
	anim *list, **prev_anim;

	list = first_anim;
	prev_anim = &first_anim;
	while (list && (list != ptr)) {
		prev_anim = &list->next;
		list = list->next;
	}

	if ( !list )
		return -2;

	// only free when ref_count is 0
	ptr->ref_count--;
	if ( ptr->ref_count > 0 ) 
		return -1;

	// only free if there are no playing instances
	if ( ptr->instance_count > 0 )
		return -1;

	if(ptr->keys != NULL){
		free(ptr->keys);
		ptr->keys = NULL;
	}

	if ( ptr->flags & (ANF_MEM_MAPPED|ANF_STREAMED) ) {
		cfclose(ptr->cfile_ptr);
		if ( ptr->cache ) {
			free(ptr->cache);
		}
	}
	else {
		Assert(ptr->data);
		free(ptr->data);
	}

	*prev_anim = ptr->next;
	free(ptr);
	return 0;
}


// ---------------------------------------------------------------------
// anim_playing()
//
// Return if an anim is playing or not.  
//
int anim_playing(anim_instance *ai)
{
	Assert(ai != NULL);
	if ( ai->frame == NULL )
		return 0;
	else 
		return 1;
}


// ---------------------------------------------------------------------
// anim_level_init()
//
// Called at the beginning of a mission to initialize any mission dependent
// anim data.
//
void anim_level_init()
{
}

// ---------------------------------------------------------------------
// anim_level_close()
//
// Called after the end of a mission to clean up any mission dependent 
// anim data. 
//
void anim_level_close()
{
	anim_release_all_instances();
}

// ---------------------------------------------------
//	anim_write_frames_out()
//
//	Write the frames of a .ani file out to disk as .pcx files.
// Use naming convention: filename0000.pcx, filename0001.pcx etc.
//
// return:		0	=>		success
//					-1	=>		failed
//
int anim_write_frames_out(const char *filename)
{
	anim				*source_anim;
	anim_instance	*ai;
	char				root_name[256], pcxname[256];
	char				buf[64];
	int				i,j;
	ubyte				**row_data;

	strcpy(root_name, filename);
	root_name[strlen(filename)-4] = 0;

	source_anim = anim_load(filename);
	if ( source_anim == NULL ) 
		return -1;

	ai = init_anim_instance(source_anim, 16);

	row_data = (ubyte**)malloc((source_anim->height+1) * 4);

	for ( i = 0; i < source_anim->total_frames; i++ ) {
		anim_get_next_raw_buffer(ai, 0, 0, 16);
		strcpy(pcxname, root_name);
		sprintf(buf,"%04d",i);
		strcat(pcxname, buf);

		for ( j = 0; j < source_anim->height; j++ ) {
			row_data[j] = &ai->frame[j*source_anim->width];
		}


		pcx_write_bitmap( pcxname,
								source_anim->width,
								source_anim->height,
								row_data,
								source_anim->palette);

		printf(".");

	}
	printf("\n");
	free(row_data);
	return 0;
}

// ---------------------------------------------------
//	anim_display_info()
//
//	Display information and statistics about a .ani file.
//	This is called when -i switch is on when running ac.exe
//
void anim_display_info(const char *real_filename)
{
	CFILE				*fp;
	anim				A;
	float				percent;
	int				i, uncompressed, compressed, *key_frame_nums=NULL, tmp;
	char filename[MAX_FILENAME_LEN];

	strcpy( filename, real_filename );
	char *p = strchr( filename, '.' );
	if ( p ) {
		*p = 0;
	}
	strcat( filename, ".ani" );

	fp = cfopen(filename, "rb");
	if ( !fp ) {
		printf("Fatal error opening %s", filename);
		return;
	}

	anim_read_header(&A, fp);
	// read the keyframe frame nums and offsets
	key_frame_nums = (int*)malloc(sizeof(int)*A.num_keys);
	Assert(key_frame_nums != NULL);
	for ( i = 0; i < A.num_keys; i++ ) {
		key_frame_nums[i] = 0;
		cfread(&key_frame_nums[i], 2, 1, fp);
		cfread(&tmp, 4, 1, fp);
//printf("key frame num: %d,%d\n", key_frame_nums[i],tmp);
	}

	cfread(&compressed, 4, 1, fp);

	uncompressed = A.width * A.height * A.total_frames;	// 8 bits per pixel
	percent = i2fl(compressed) / uncompressed * 100.0f;

	printf("%% of uncompressed size:    %.0f%%\n", percent);
	printf("Width:                     %d\n", A.width);
	printf("Height:                    %d\n", A.height);
	printf("Total Frames:              %d\n", A.total_frames);

#ifndef NDEBUG
	printf("Key Frames:                %d\n", A.num_keys);
	if ( A.num_keys > 1 && (A.total_frames != A.num_keys) ) {
		printf("key list: (");
		for ( i = 0; i < A.num_keys; i++ ) {
			if ( i < A.num_keys-1 ) 
				printf("%d, ", key_frame_nums[i]);
			else
				printf("%d)\n", key_frame_nums[i]);
		}
	}
#endif

	printf("FPS:                       %d\n", A.fps);

#ifndef NDEBUG
	printf("Transparent RGB:           (%d,%d,%d)\n", A.xparent_r, A.xparent_g, A.xparent_b); 
#endif

	printf("ac version:                %d\n", A.version);

	if ( key_frame_nums != NULL ) {
		free(key_frame_nums);
	}

	cfclose(fp);
}

void anim_reverse_direction(anim_instance *ai)
{
	int temp;

	if(!(ai->parent->flags & ANF_ALL_KEYFRAMES)){
		 // you're not allowed to call anim_reverse_direction(...) unless every frame is a keyframe!!!!
		 // The God of Delta-RLE demands it be thus.
		Int3();
	}
		
	// flip the animation direction
	if(ai->direction == ANIM_DIRECT_FORWARD){
		ai->direction = ANIM_DIRECT_REVERSE;				
	} else if(ai->direction == ANIM_DIRECT_REVERSE){
		ai->direction = ANIM_DIRECT_FORWARD;
	}

	// flip frame_num and last_frame_num
	temp = ai->frame_num;
	ai->frame_num = ai->last_frame_num;
	ai->last_frame_num = temp;

	// flip the start and stop at frames
	temp = ai->stop_at;
	ai->stop_at = ai->start_at;
	ai->start_at = temp;

	// make sure to sync up the time correctly 
	if(ai->direction == ANIM_DIRECT_FORWARD){
		ai->time_elapsed = ((float)ai->frame_num - (float)ai->start_at - 1.0f) / (float)ai->parent->fps;	
	} else if(ai->direction == ANIM_DIRECT_REVERSE) {
		ai->time_elapsed = ((float)ai->start_at - (float)ai->frame_num - 1.0f) / (float)ai->parent->fps;	
	}	
}

void anim_pause(anim_instance *ai)
{
	ai->paused = 1;
}

void anim_unpause(anim_instance *ai)
{
	ai->paused = 0;
}

void anim_ignore_next_frametime()
{
	Anim_ignore_frametime=1;
}

int anim_instance_is_streamed(anim_instance *ai)
{
	Assert(ai);
	return ( ai->parent->flags & ANF_STREAMED );
}

unsigned char anim_instance_get_byte(anim_instance *ai, int offset)
{
	int absolute_offset;
	anim *parent;
	
	Assert(ai);
	Assert(ai->parent->cfile_ptr);
	Assert(ai->parent->flags & ANF_STREAMED);

	parent = ai->parent;
	absolute_offset = ai->file_offset + offset;

	// maybe in cache?
	int cache_offset;
	cache_offset = absolute_offset - parent->cache_file_offset;
	if ( (cache_offset >= 0) && (cache_offset < ANI_STREAM_CACHE_SIZE) ) {
		return parent->cache[cache_offset];
	} else {
		// fill cache
		cfseek(parent->cfile_ptr, absolute_offset, CF_SEEK_SET);
		cfread(parent->cache, ANI_STREAM_CACHE_SIZE, 1, parent->cfile_ptr);
		parent->cache_file_offset = absolute_offset;
		return parent->cache[0];
	}
}

