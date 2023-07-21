/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MissionUI/MissionPause.cpp $
 * $Revision: 312 $
 * $Date: 2010-02-08 09:09:27 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 * 
 *
 * $Log$
 * Revision 1.5  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.4  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.3  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:46  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 7     7/29/99 10:48p Dave
 * Multiplayer pause screen.
 * 
 * 6     6/29/99 7:39p Dave
 * Lots of small bug fixes.
 * 
 * 5     6/09/99 2:17p Dave
 * Fixed up pleasewait bitmap rendering.
 * 
 *
 * $NoKeywords: $
 */

#include "missionpause.h"
#include "ui.h"
#include "multi_pause.h"
#include "popup.h"
#include "2d.h"
#include "bmpman.h"
#include "key.h"
#include "audiostr.h"
#include "gamesequence.h"
#include "freespace.h"
#include "hud.h"
#include "object.h"
#include "font.h"
#include "alphacolors.h"
#include "beam.h"

// ----------------------------------------------------------------------------------------------------------------
// PAUSE DEFINES/VARS
//

// pause bitmap name
const char *Pause_bmp_name[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	"Pause",
#else
	"PleaseWait",
#endif
	"2_PleaseWait"
};

// pause bitmap display stuff
int Please_wait_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		96, 205, 447, 70
#else
		152, 217, 316, 26
#endif
	},
	{ // GR_1024
		247, 346, 510, 36
	}	
};

const char *Pause_multi_fname[GR_NUM_RESOLUTIONS] = {
	"MPPause",
	"2_MPPause"
};
const char *Pause_multi_mask[GR_NUM_RESOLUTIONS] = {
	"MPPause-m",
	"2_MPPause-m"
};

// pause window objects
UI_WINDOW Pause_win;
UI_CHECKBOX Pause_single_step;
UI_CHECKBOX Pause_physics;
UI_CHECKBOX Pause_ai;
UI_CHECKBOX Pause_ai_render;
UI_CHECKBOX Pause_firing;
UI_CHECKBOX Pause_external_view_mode_check;
UI_BUTTON Pause_continue;

// if we're already paused
int Paused = 0;

// background screen (for the chatbox)
int Pause_background_bitmap = -1;

// saved background screen
int Pause_saved_screen = -1;

// if we're in external vie wmode
int Pause_external_view_mode = 0;

// externs
extern int Player_attacking_enabled;
extern int Ai_render_debug_flag;
extern int Ai_firing_enabled;
extern int physics_paused;
extern int ai_paused;
extern int last_single_step;
extern int game_single_step;

// ----------------------------------------------------------------------------------------------------------------
// PAUSE FUNCTIONS
//

// initialize the pause screen
void pause_init(int multi)
{
	// if we're already paused. do nothing
	if ( Paused ) {
		return;
	}	

	// pause all beam weapon sounds
	beam_pause_sounds();

	if(!(Game_mode & GM_STANDALONE_SERVER)){
		Pause_saved_screen = gr_save_screen();

		// pause all game music
		audiostream_pause_all();

		//JAS: REMOVED CALL TO SET INTERFACE PALETTE TO GET RID OF SCREEN CLEAR WHEN PAUSING
		//common_set_interface_palette();  // set the interface palette
		Pause_win.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);	

		if (multi) {
			Pause_win.set_mask_bmap(Pause_multi_mask[gr_screen.res]);
			Pause_background_bitmap = bm_load(Pause_multi_fname[gr_screen.res]);

			multi_pause_init(&Pause_win);		
		} else {
			Pause_background_bitmap = bm_load(Pause_bmp_name[gr_screen.res]);
		}
	} else {
		multi_pause_init(NULL);
	}

	Paused = 1;
}

// pause do frame - will handle running multiplayer operations if necessary
void pause_do(int multi)
{
	int k;
#ifndef MAKE_FS1
	const char *pause_str = XSTR("Paused", 767);
	int str_w, str_h;
#endif

	if(Game_mode & GM_STANDALONE_SERVER){
		multi_pause_do();
	} else {		
		//	RENDER A GAME FRAME HERE AS THE BACKGROUND
		gr_restore_screen(Pause_saved_screen);
		if (Pause_background_bitmap >= 0) {
			gr_set_bitmap(Pause_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
			if(multi){
				gr_bitmap(0,0);
			} else {
				// draw the bitmap
				gr_bitmap(Please_wait_coords[gr_screen.res][0], Please_wait_coords[gr_screen.res][1]);

#ifndef MAKE_FS1
				// draw "Paused" on it
				gr_set_color_fast(&Color_normal);
				gr_set_font(FONT2);
				gr_get_string_size(&str_w, &str_h, pause_str);
				gr_string((gr_screen.max_w - str_w) / 2, (gr_screen.max_h - str_h) / 2, pause_str);
				gr_set_font(FONT1);
#endif
			}
		}
	
		// the multi paused screen will do its own window processing
		if (multi) {
			multi_pause_do();
		}
		// otherwise process the ui window here
		else {
			k = Pause_win.process() & ~KEY_DEBUGGED;
			switch (k) {			
			case KEY_ESC:
			case KEY_PAUSE:
				gameseq_post_event(GS_EVENT_PREVIOUS_STATE);		
				break;
			}	// end switch
		}
	
		// draw the background window
		Pause_win.draw();		

		// a very unique case where we shouldn't be doing the page flip because we're inside of popup code
		if(!popup_active()){
			gr_flip();
		} else {
			// this should only be happening in a very unique multiplayer case
			Assert(Game_mode & GM_MULTIPLAYER);
		}
	}
}

// close the pause screen
void pause_close(int multi)
{
	// if we're not paused - do nothing
	if ( !Paused ) {
		return;
	}

	// unpause all beam weapon sounds
	beam_unpause_sounds();

	// deinit stuff
	if(Game_mode & GM_STANDALONE_SERVER){
		multi_pause_close();
	} else {
		gr_free_screen(Pause_saved_screen);	

		if (Pause_background_bitmap){
			bm_unload(Pause_background_bitmap);
		}

		Pause_win.destroy();		
		game_flush();
		if (multi) {
			multi_pause_close();
		}

		// unpause all the music
		audiostream_unpause_all();		
	}

	Paused = 0;
}

// debug pause init
void pause_debug_init()
{
	Pause_win.create( 100,100,400,300, WIN_DIALOG );

	Pause_physics.create( &Pause_win, NOX("Physics Pause <P>"), 200, 150, physics_paused );
	Pause_ai.create( &Pause_win, NOX("AI Pause <A>"), 200, 175, ai_paused );
	#ifndef NDEBUG
	Pause_ai_render.create( &Pause_win, NOX("AI Render Stuff <R>"), 200, 200, Ai_render_debug_flag);
	#endif
	Pause_firing.create( &Pause_win, NOX("AI firing <F>"), 200, 225, Ai_firing_enabled);
	Pause_external_view_mode_check.create( &Pause_win, NOX("External View <E>"), 200, 250, Pause_external_view_mode);
	Pause_single_step.create( &Pause_win, NOX("Single Step <S>"), 200, 290, game_single_step );
	Pause_continue.create( &Pause_win, NOX("Leave Pause"), 200, 350, 200, 40 );

	Pause_single_step.set_hotkey( KEY_S );
	Pause_physics.set_hotkey( KEY_P );
	Pause_ai.set_hotkey( KEY_A );
	Pause_ai_render.set_hotkey( KEY_R );
	Pause_firing.set_hotkey( KEY_F );
	Pause_external_view_mode_check.set_hotkey( KEY_E );
	Pause_continue.set_hotkey( KEY_ESC );

	Pause_continue.set_focus();
}

// debug pause do frame
void pause_debug_do()
{
	int key;

	key = Pause_win.process();
	if ( Pause_single_step.changed())	{
		game_single_step = Pause_single_step.checked();
	}

	if ( Pause_physics.changed())	{
		physics_paused = Pause_physics.checked();
	}

	if ( Pause_ai.changed())	{
		ai_paused = Pause_ai.checked();
		if (ai_paused){
			obj_init_all_ships_physics();
		}
	}

	if ( Pause_ai_render.changed())	{
		Ai_render_debug_flag = Pause_ai_render.checked();
	}

	if ( Pause_firing.changed())	{
		Ai_firing_enabled = Pause_firing.checked();
	}

	if ( Pause_external_view_mode_check.changed())	{
		Pause_external_view_mode = Pause_external_view_mode_check.checked();
		if (Pause_external_view_mode){
			HUD_sourced_printf(HUD_SOURCE_HIDDEN, XSTR( "External view of player ship.", 182));
		} else {
			HUD_sourced_printf(HUD_SOURCE_HIDDEN, XSTR( "View from inside player ship.", 183));
		}
	}

	if ( Pause_continue.pressed() || (key == KEY_PAUSE) )	{	//	Changed, MK, 11/9/97, only Pause break pause.
		gameseq_post_event(GS_EVENT_PREVIOUS_STATE);
	}

	gr_clear();
	Pause_win.draw();

	gr_flip();
}

// debug pause close
void pause_debug_close()
{
	last_single_step = 0;	// Make so single step waits a frame before stepping
	Pause_win.destroy();
	game_flush();
}

