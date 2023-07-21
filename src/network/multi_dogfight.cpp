/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_dogfight.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 * 
 * $Log$
 * Revision 1.4  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.3  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:47  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 11    8/18/99 11:31a Jefff
 * mission title on kill matrix
 * 
 * 10    8/17/99 9:55a Dave
 * Fixed dogfight scoring problem.
 * 
 * 9     8/06/99 12:29a Dave
 * Multiple bug fixes.
 * 
 * 8     5/03/99 8:32p Dave
 * New version of multi host options screen.
 * 
 * 7     4/12/99 10:07p Dave
 * Made network startup more forgiving. Added checkmarks to dogfight
 * screen for players who hit commit.
 * 
 * 6     4/12/99 2:22p Dave
 * More checks for dogfight stats.
 * 
 * 5     2/24/99 3:40p Dave
 * Fixed problem where ships were becoming traitor all the time.
 * 
 * 4     2/24/99 2:25p Dave
 * Fixed up chatbox bugs. Made squad war reporting better. Fixed a respawn
 * bug for dogfight more.
 * 
 * 3     2/23/99 8:11p Dave
 * Tidied up dogfight mode. Fixed TvT ship type problems for alpha wing.
 * Small pass over todolist items.
 * 
 * 2     2/23/99 2:29p Dave
 * First run of oldschool dogfight mode. 
 *   
 * $NoKeywords: $
 */

#include "multi.h"
#include "multiutil.h"
#include "multi_log.h"
#include "bmpman.h"
#include "object.h"
#include "systemvars.h"
#include "freespace.h"
#include "key.h"
#include "missionscreencommon.h"
#include "eventmusic.h"
#include "gamesnd.h"
#include "multiui.h"
#include "chatbox.h"
#include "ui.h"
#include "font.h"
#include "multi_dogfight.h"
#include "alphacolors.h"

// ----------------------------------------------------------------------------------------------------
// MULTI DOGFIGHT DEFINES/VARS
//

// interface stuff
UI_WINDOW Multi_df_window;

#define NUM_MULTI_DF_BUTTONS			1
#define ACCEPT_BUTTON					0

ui_button_info Multi_df_buttons[GR_NUM_RESOLUTIONS][NUM_MULTI_DF_BUTTONS] = {
	{ // GR_640
		// accept
		ui_button_info("CB_05a",	571,	425,	578,	413,	5),
	}, 	
	{ // GR_1024
		// accept
		ui_button_info("2_CB_05a",	914,	681,	914,	660,	5),
	}
};

int Multi_df_background_bitmap = -1;
const char *Multi_df_background_fname[GR_NUM_RESOLUTIONS] = {
	"KillMatrix",
	"2_KillMatrix"
};
const char *Multi_df_mask_fname[GR_NUM_RESOLUTIONS] = {
	"KillMatrix-m",
	"2_KillMatrix-m"
};

// coord 3 is max width
static int Kill_matrix_title_coords[GR_NUM_RESOLUTIONS][3] = {
	{	// GR_640
		19, 118, 172
	},
	{	// GR_1024
		33, 194, 272
	}
};

// display area coords
int Multi_df_display_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
		43, 133, 569, 269
	},
	{ // GR_1024
		60, 213, 919, 429
	}
};

#define MULTI_DF_TOTAL_ADJUST				5

// "check" icon coords
int Multi_df_check_coords[GR_NUM_RESOLUTIONS] = {
	// GR_640
	28,	

	// GR_1024
	45	
};

// players when the screen started - we need to store this explicity so that even after players leave, we can display the full kill matrix
typedef struct multi_df_score {
	char callsign[CALLSIGN_LEN+1];		// callsign for this guy	
	scoring_struct stats;					// stats for the guy	
	int np_index;								// absolute index into the netplayers array
} multi_df_score;
multi_df_score Multi_df_score[MAX_PLAYERS];
int Multi_df_score_count = 0;


// ----------------------------------------------------------------------------------------------------
// MULTI DOGFIGHT FORWARD DECLARATIONS
//

// process button presses
void multi_df_process_buttons();

// button was pressed
void multi_df_button_pressed(int button);

// setup kill matrix data
void multi_df_setup_kill_matrix();

// blit the kill matrix
void multi_df_blit_kill_matrix();

// stuff a string representing the # of kills, player X had on player Y (where X and Y are indices into Multi_df_score)
// returns the # of kills
int multi_df_stuff_kills(char *kills, int player_x, int player_y);


// ----------------------------------------------------------------------------------------------------
// MULTI DOGFIGHT FUNCTIONS
//

// call once per level just before entering the mission
void multi_df_level_pre_enter()
{			
	int idx;

	// if we're not in dogfight mode, do nothing
	if(!(Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return;
	}

	// go through all player ships and make them hostile
	for(idx=0; idx<MAX_PLAYERS; idx++){
		if(MULTI_CONNECTED(Net_players[idx]) && !MULTI_STANDALONE(Net_players[idx]) && !MULTI_OBSERVER(Net_players[idx]) && (Net_players[idx].player != NULL) && (Net_players[idx].player->objnum >= 0) && (Objects[Net_players[idx].player->objnum].type == OBJ_SHIP)){
			Ships[Objects[Net_players[idx].player->objnum].instance].team = TEAM_TRAITOR;
		}
	}

	// 
}

// evaluate a kill in dogfight by a netplayer
void multi_df_eval_kill(net_player *killer, object *dead_obj)
{
	int dead_index = -1;
	
	// if we're not in dogfight mode, do nothing
	if(!(Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return;
	}

	// sanity checks
	if((killer == NULL) || (dead_obj == NULL) || (killer->player == NULL)){
		return;
	}
	
	// try and find the dead player
	dead_index = multi_find_player_by_object(dead_obj);
	if(dead_index < 0){
		return;
	}
	Assert(dead_index < MAX_PLAYERS);
	if(dead_index == NET_PLAYER_INDEX(killer)){
		return;
	}

	// update his kills
	killer->player->stats.m_dogfight_kills[dead_index]++;
}

// debrief
void multi_df_debrief_init()
{
	// no longer is mission
	Game_mode &= ~(GM_IN_MISSION);	
	game_flush();

	// call scoring level close for my stats.  Needed for award_init.  The stats will
	// be backed out if used chooses to replace them.
	scoring_level_close();

	// multiplayer debriefing stuff
	multi_debrief_init();

	// close down any old instances of the chatbox
	chatbox_close();

	// create the new one
	chatbox_create();

	// always play success music
	common_music_init(SCORE_DEBRIEF_SUCCESS);

	// setup kill matrix
	multi_df_setup_kill_matrix();

	UI_WINDOW *w;
	ui_button_info *b;
	int idx;

	// load background bitmap
	Multi_df_background_bitmap = bm_load(Multi_df_background_fname[gr_screen.res]);
	Assert(Multi_df_background_bitmap);

	// create the UI window
	Multi_df_window.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);
	Multi_df_window.set_mask_bmap(Multi_df_mask_fname[gr_screen.res]);
	
	// initialize the control buttons
	for (idx=0; idx<NUM_MULTI_DF_BUTTONS; idx++) {
		b = &Multi_df_buttons[gr_screen.res][idx];

		// create the button
		b->button.create(&Multi_df_window, NULL, b->x, b->y, 60, 30, 1, 1);
		
		// set its highlight action
		b->button.set_highlight_action(common_play_highlight_sound);

		// set its animation bitmaps
		b->button.set_bmaps(b->filename);

		// link the mask hotspot
		b->button.link_hotspot(b->hotspot);
	}		

	// add some text
	w = &Multi_df_window;	
	w->add_XSTR("Accept", 1035, Multi_df_buttons[gr_screen.res][ACCEPT_BUTTON].xt, Multi_df_buttons[gr_screen.res][ACCEPT_BUTTON].yt, &Multi_df_buttons[gr_screen.res][ACCEPT_BUTTON].button, UI_XSTR_COLOR_PINK);
}

// do frame
void multi_df_debrief_do()
{
	int k, new_k;
	char buf[256];
	
	k = chatbox_process();	
	new_k = Multi_df_window.process(k, 0);	

	// process keypresses
	switch(new_k){
	case KEY_ESC:
		multi_debrief_esc_hit();
		break;
	}

	// process buttons	
	multi_df_process_buttons();

	// music stuff
	common_music_do();

	// process debriefing details
	multi_debrief_do_frame();

	// draw the background
	GR_MAYBE_CLEAR_RES(Multi_df_background_bitmap);
	if (Multi_df_background_bitmap >= 0) {
		gr_set_bitmap(Multi_df_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	} 

	// draw the window
	Multi_df_window.draw();	

	// kill matrix
	multi_df_blit_kill_matrix();

	// render the chatbox
	chatbox_render();

	// draw the mission title
	strcpy(buf, The_mission.name);
	gr_force_fit_string(buf, 255, Kill_matrix_title_coords[gr_screen.res][2]);
	gr_set_color_fast(&Color_bright_white);
	gr_string(Kill_matrix_title_coords[gr_screen.res][0], Kill_matrix_title_coords[gr_screen.res][1], buf);

	// flip
	gr_flip();
}

// close
void multi_df_debrief_close()
{
	int idx;

	// shutdown the chatbox
	chatbox_close();

	// if stats weren't accepted, backout my own stats
	if (multi_debrief_stats_accept_code() != 1) {		
		// if stats weren't accepted, backout my own stats
		if (multi_debrief_stats_accept_code() != 1) {
			if(MULTIPLAYER_MASTER){
				for(idx=0; idx<MAX_PLAYERS; idx++){
					if(MULTI_CONNECTED(Net_players[idx]) && !MULTI_STANDALONE(Net_players[idx]) && !MULTI_PERM_OBSERVER(Net_players[idx]) && (Net_players[idx].player != NULL)){
						scoring_backout_accept(&Net_players[idx].player->stats);
					}
				}
			} else {
				scoring_backout_accept( &Player->stats );
			}
		}
	}

	// music stuff
	common_music_close();
}


// ----------------------------------------------------------------------------------------------------
// MULTI DOGFIGHT FORWARD DEFINITIONS
//

// process button presses
void multi_df_process_buttons()
{
	int idx;

	for(idx=0; idx<NUM_MULTI_DF_BUTTONS; idx++){
		if(Multi_df_buttons[gr_screen.res][idx].button.pressed()){
			multi_df_button_pressed(idx);
			break;
		}
	}
}

// button was pressed
void multi_df_button_pressed(int button)
{
	switch(button){
	case ACCEPT_BUTTON:
		multi_debrief_accept_hit();
		break;
	}
}

// setup kill matrix data
void multi_df_setup_kill_matrix()
{
	int idx, s_idx;
	multi_df_score *s;

	Multi_df_score_count = 0;

	// add players as necessary
	for(idx=0; idx<MAX_PLAYERS; idx++){
		if(MULTI_CONNECTED(Net_players[idx]) && !MULTI_STANDALONE(Net_players[idx]) && !MULTI_PERM_OBSERVER(Net_players[idx]) && (Net_players[idx].player != NULL)){
			// stuff data for this guy
			s = &Multi_df_score[Multi_df_score_count++];

			ml_printf("Dogfight debrief stats for %s", Net_players[idx].player->callsign);
			for(s_idx=0; s_idx<MAX_PLAYERS; s_idx++){
				ml_printf("%d", Net_players[idx].player->stats.m_dogfight_kills[s_idx]);
			}

			s->stats = Net_players[idx].player->stats;
			strcpy(s->callsign, Net_players[idx].player->callsign);			
			s->np_index = idx;
		}
	}
}

// blit the kill matrix
void multi_df_blit_kill_matrix()
{
	int idx, s_idx, str_len;
	int cx, cy;
	char squashed_string[CALLSIGN_LEN+1] = "";

	// max width of an individual item, and the text that can be in that item
	float max_item_width = ((float)Multi_df_display_coords[gr_screen.res][2] - 40.0f) / (float)(Multi_df_score_count + 1);
	float max_text_width = max_item_width * 0.8f;

	// start x for the top bar (one item to the right)
	int top_x_start = Multi_df_display_coords[gr_screen.res][0] + (int)max_item_width;
	int top_y_start = Multi_df_display_coords[gr_screen.res][1];	

	// start x for the side bar
	int side_x_start = Multi_df_display_coords[gr_screen.res][0];
	int side_y_start = Multi_df_display_coords[gr_screen.res][1] + 10;

	// draw the top bar
	cx = top_x_start;
	cy = top_y_start;
	for(idx=0; idx<Multi_df_score_count; idx++){		
		// force the string to fit nicely
		strcpy(squashed_string, Multi_df_score[idx].callsign);
		gr_force_fit_string(squashed_string, CALLSIGN_LEN, (int)max_text_width);
		gr_get_string_size(&str_len, NULL, squashed_string);

		// set color and blit the string		
		Assert(Multi_df_score[idx].np_index >= 0);
		if(Multi_df_score[idx].np_index >= 0){
			gr_set_color_fast(Color_netplayer[Multi_df_score[idx].np_index]);
		}
		gr_string(cx + (int)((max_item_width - (float)str_len)/2.0f), cy, squashed_string);

		// next spot
		cx += (int)max_item_width;
	}

	// draw the rest of the scoreboard	
	cx = side_x_start;
	cy = side_y_start;
	int row_total;
	for(idx=0; idx<Multi_df_score_count; idx++){		
		// draw a check if necessary
		if(!MULTI_CONNECTED(Net_players[Multi_df_score[idx].np_index]) || (Net_players[Multi_df_score[idx].np_index].state == NETPLAYER_STATE_DEBRIEF_ACCEPT) || (Net_players[Multi_df_score[idx].np_index].state == NETPLAYER_STATE_DEBRIEF_REPLAY)){
			if(Multi_common_icons[MICON_VALID] != -1){
				gr_set_bitmap(Multi_common_icons[MICON_VALID], GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(Multi_df_check_coords[gr_screen.res], cy);
			}
		}

		// draw the name
		cx = Multi_df_display_coords[gr_screen.res][0];
		strcpy(squashed_string, Multi_df_score[idx].callsign);
		gr_force_fit_string(squashed_string, CALLSIGN_LEN, (int)max_text_width);
		gr_get_string_size(&str_len, NULL, squashed_string);		
		Assert(Multi_df_score[idx].np_index >= 0);
		if(Multi_df_score[idx].np_index >= 0){
			gr_set_color_fast(Color_netplayer[Multi_df_score[idx].np_index]);
		}
		gr_string(cx, cy, squashed_string);

		cx = top_x_start;
		row_total = 0;
		for(s_idx=0; s_idx<Multi_df_score_count; s_idx++){
			// stuff the string to be displayed and select the proper display color
			if(s_idx == idx){
				strcpy(squashed_string, "-");
				gr_set_color_fast(&Color_grey);
			} else {
				row_total += multi_df_stuff_kills(squashed_string, idx, s_idx);
				Assert(Multi_df_score[idx].np_index >= 0);
				if(Multi_df_score[idx].np_index >= 0){
					gr_set_color_fast(Color_netplayer[Multi_df_score[idx].np_index]);
				}				
			}						

			// draw the string
			gr_force_fit_string(squashed_string, CALLSIGN_LEN, (int)max_text_width);
			gr_get_string_size(&str_len, NULL, squashed_string);
			gr_string(cx + (int)((max_item_width - (float)str_len)/2.0f), cy, squashed_string);

			// next spot
			cx += (int)max_item_width;
		}

		// draw the row total
		gr_set_color_fast(Color_netplayer[Multi_df_score[idx].np_index]);
		sprintf(squashed_string, "(%d)", row_total);
		gr_get_string_size(&str_len, NULL, squashed_string);
		gr_string(Multi_df_display_coords[gr_screen.res][0] + Multi_df_display_coords[gr_screen.res][2] - (MULTI_DF_TOTAL_ADJUST + str_len), cy, squashed_string);

		cy += 10;
	}

	/*
	// blit totals
	int column_total;
	cx = top_x_start;
	cy += 3;
	for(idx=0; idx<Multi_df_score_count; idx++){
		// add up this column
		column_total = 0;
		for(s_idx=0; s_idx<Multi_df_score_count; s_idx++){
			// add			
			column_total += Multi_df_score[idx].stats.m_dogfight_kills[s_idx];
		}

		// draw
		sprintf(squashed_string, "%d", column_total);
		gr_set_color_fast(Color_netplayer[Multi_df_score[idx].np_index]);
		gr_string(cx, cy, squashed_string);

		// next item
		cx += (int)max_item_width;
	}
	*/
}

// stuff a string representing the # of kills, player X had on player Y (where X and Y are indices into Multi_df_score)
// returns the # of kills
int multi_df_stuff_kills(char *kills, int player_x, int player_y)
{
	multi_df_score *s = &Multi_df_score[player_x];
	strcpy(kills, "");
	
	sprintf(kills, "%d", s->stats.m_dogfight_kills[Multi_df_score[player_y].np_index]);
	return s->stats.m_dogfight_kills[Multi_df_score[player_y].np_index];
}
