/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MissionUI/MissionLoopBrief.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Campaign Loop briefing screen
 *
 * $Log$
 * Revision 1.4  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
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
 * 4     9/06/99 9:44p Jefff
 * break out of loop support
 * 
 * 3     9/06/99 6:38p Dave
 * Improved CD detection code.
 * 
 * 2     8/27/99 12:04a Dave
 * Campaign loop screen.
 *  
 *
 * $NoKeywords: $
 */

#include "missionloopbrief.h"
#include "missionscreencommon.h"
#include "2d.h"
#include "bmpman.h"
#include "gamesnd.h"
#include "eventmusic.h"
#include "audiostr.h"
#include "key.h"
#include "gamesequence.h"
#include "missioncampaign.h"
#include "animplay.h"
#include "freespace.h"

// ---------------------------------------------------------------------------------------------------------------------------------------
// MISSION LOOP BRIEF DEFINES/VARS
//

const char *Loop_brief_fname[GR_NUM_RESOLUTIONS] = {
	"LoopBrief",		// GR_640
	"2_LoopBrief",		// GR_1024
};

const char *Loop_brief_mask[GR_NUM_RESOLUTIONS] = {
	"LoopBrief-m",		// GR_640
	"2_Loopbrief-m",	// GR_1024
};

#define NUM_LOOP_BRIEF_BUTTONS				2
#define LOOP_BRIEF_DECLINE						0
#define LOOP_BRIEF_ACCEPT						1
ui_button_info Loop_buttons[GR_NUM_RESOLUTIONS][NUM_LOOP_BRIEF_BUTTONS] = {
	{ // GR_640
		ui_button_info("LBB_00",		529,	437,	-1,	-1,	0),
		ui_button_info("LBB_01",		575,	433,	-1,	-1,	1),
	},
	{ // GR_1024
		ui_button_info("2_LBB_00",		846,	699,	-1,	-1,	0),
		ui_button_info("2_LBB_01",		920,	693,	-1,	-1,	1),
	}
};

#define NUM_LOOP_TEXT							2
UI_XSTR Loop_text[GR_NUM_RESOLUTIONS][NUM_LOOP_TEXT] = {
	{ // GR_640
		{ "Decline",		1467,	514,	413,	UI_XSTR_COLOR_PINK,	-1,	&Loop_buttons[0][LOOP_BRIEF_DECLINE].button },
		{ "Accept",			1035,	573,	413,	UI_XSTR_COLOR_PINK,	-1,	&Loop_buttons[0][LOOP_BRIEF_ACCEPT].button },
	},	
	{ // GR_1024
		{ "Decline",		1467,	855,	670,	UI_XSTR_COLOR_PINK,	-1,	&Loop_buttons[1][LOOP_BRIEF_DECLINE].button },
		{ "Accept",			1035,	928,	670,	UI_XSTR_COLOR_PINK,	-1,	&Loop_buttons[1][LOOP_BRIEF_ACCEPT].button },
	}
};

// loop brief anim
int Loop_brief_anim_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
		24, 267
	},
	{ // GR_640
		167, 491
	}
};

// text window coords
int Loop_brief_text_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
		25,	107,	591,	143
	},
	{ // GR_1024
		40,	171,	945,	229
	}
};

UI_WINDOW Loop_brief_window;
int Loop_brief_bitmap;

anim *Loop_anim;
anim_instance *Loop_anim_instance;

int Loop_sound;

// ---------------------------------------------------------------------------------------------------------------------------------------
// MISSION LOOP BRIEF FUNCTIONS
//

// button press
void loop_brief_button_pressed(int i)
{	
	switch(i){
	case LOOP_BRIEF_DECLINE:		
		// CD CHECK
		if(game_do_cd_mission_check(Game_current_mission_filename)){
			gameseq_post_event(GS_EVENT_START_GAME);
			gamesnd_play_iface(SND_USER_SELECT);
		} else {
			gameseq_post_event(GS_EVENT_MAIN_MENU);
		}		
		break;

	case LOOP_BRIEF_ACCEPT:
		// select the loop mission		
		Campaign.loop_enabled = 1;
		Campaign.loop_reentry = Campaign.next_mission;			// save reentry pt, so we can break out of loop
		Campaign.next_mission = Campaign.loop_mission;		
		
		// CD CHECK
		if(game_do_cd_mission_check(Game_current_mission_filename)){
			gameseq_post_event(GS_EVENT_START_GAME);
			gamesnd_play_iface(SND_USER_SELECT);
		} else {
			gameseq_post_event(GS_EVENT_MAIN_MENU);
		}		
		break;
	}
}

// init
void loop_brief_init()
{
	int idx;
	ui_button_info *b;

	// load the background bitmap
	Loop_brief_bitmap = bm_load(Loop_brief_fname[gr_screen.res]);
	Assert(Loop_brief_bitmap != -1);

	// window
	Loop_brief_window.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);
	Loop_brief_window.set_mask_bmap(Loop_brief_mask[gr_screen.res]);	

	// add the buttons
	for (idx=0; idx<NUM_LOOP_BRIEF_BUTTONS; idx++) {
		b = &Loop_buttons[gr_screen.res][idx];

		b->button.create(&Loop_brief_window, "", b->x, b->y, 60, 30, 0, 1);		
		b->button.set_highlight_action(common_play_highlight_sound);
		b->button.set_bmaps(b->filename);
		b->button.link_hotspot(b->hotspot);
	}

	// add text
	for(idx=0; idx<NUM_LOOP_TEXT; idx++){
		Loop_brief_window.add_XSTR(&Loop_text[gr_screen.res][idx]);
	}

	// load animation if any
	Loop_anim = NULL;
	Loop_anim_instance = NULL;
	if(Campaign.missions[Campaign.current_mission].mission_loop_brief_anim != NULL){
		Loop_anim = anim_load(Campaign.missions[Campaign.current_mission].mission_loop_brief_anim);
	} else {
		Loop_anim = anim_load("CB_default");
	}

	// fire up an anim instance
	if(Loop_anim != NULL){
		anim_play_struct aps;

		anim_play_init(&aps, Loop_anim, Loop_brief_anim_coords[gr_screen.res][0], Loop_brief_anim_coords[gr_screen.res][1]);
		aps.framerate_independent = 1;
		aps.looped = 1;
		aps.screen_id = GS_STATE_LOOP_BRIEF;
		Loop_anim_instance = anim_play(&aps);
	}

	// init brief text
	if(Campaign.missions[Campaign.current_mission].mission_loop_desc != NULL){
		brief_color_text_init(Campaign.missions[Campaign.current_mission].mission_loop_desc, Loop_brief_text_coords[gr_screen.res][2]);
	}

	// open sound
	if(Campaign.missions[Campaign.current_mission].mission_loop_brief_sound != NULL){
		Loop_sound = audiostream_open(Campaign.missions[Campaign.current_mission].mission_loop_brief_sound, ASF_VOICE);

		if(Loop_sound != -1){
			audiostream_play(Loop_sound, Master_voice_volume, 0);
		}
	}

	// music
	common_music_init(SCORE_BRIEFING);
}

// do
void loop_brief_do()
{
	int k;
	int idx;

	// process keys
	k = Loop_brief_window.process();	

	switch (k) {
	case KEY_ESC:		
		gameseq_post_event(GS_EVENT_MAIN_MENU);
		break;
	}	

	// process button presses
	for (idx=0; idx<NUM_LOOP_BRIEF_BUTTONS; idx++){
		if (Loop_buttons[gr_screen.res][idx].button.pressed()){
			loop_brief_button_pressed(idx);
		}
	}
	
	common_music_do();

	// clear
	GR_MAYBE_CLEAR_RES(Loop_brief_bitmap);
	if (Loop_brief_bitmap >= 0) {
		gr_set_bitmap(Loop_brief_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	} 
	
	// draw the window
	Loop_brief_window.draw();		

	// render the briefing text
	brief_render_text(0, Loop_brief_text_coords[gr_screen.res][0], Loop_brief_text_coords[gr_screen.res][1], Loop_brief_text_coords[gr_screen.res][3], flFrametime);

	// render all anims
	anim_render_all(GS_STATE_LOOP_BRIEF, flFrametime);

	gr_flip();
}

// close
void loop_brief_close()
{
	// this makes sure that we're all cool no matter how the user decides to exit
	mission_campaign_mission_over();

	// free the bitmap
	if (Loop_brief_bitmap >= 0){
		bm_unload(Loop_brief_bitmap);
	}		
	Loop_brief_bitmap = -1;

	// destroy the window
	Loop_brief_window.destroy();

	// free up anim stuff
	if(Loop_anim_instance != NULL){
		anim_release_render_instance(Loop_anim_instance);
		Loop_anim_instance = NULL;
	}
	if(Loop_anim != NULL){
		anim_free(Loop_anim);
		Loop_anim = NULL;
	}

	// stop voice
	if(Loop_sound != -1){
		audiostream_stop(Loop_sound);
		audiostream_close_file(Loop_sound);
		Loop_sound = -1;
	}

	// stop music
	common_music_close();
}
