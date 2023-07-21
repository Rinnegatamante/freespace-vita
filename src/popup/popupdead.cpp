/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Popup/PopupDead.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module for the death popup
 *
 * $Log$
 * Revision 1.4  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.3  2002/06/09 04:41:25  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:51  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 14    9/14/99 4:35a Dave
 * Argh. Added all kinds of code to handle potential crashes in debriefing
 * code.
 * 
 * 13    9/14/99 12:51a Jefff
 * sm3-09 no skip hack.
 * 
 * 12    9/11/99 1:09a Mikek
 * Removed semi-bogus Int3() in popupdead code which seemed to get tripped
 * by the supernova.
 * 
 * 11    9/10/99 6:49p Jefff
 * changed skip popup to normal type popup
 * 
 * 10    9/09/99 7:17p Jefff
 * 
 * 9     9/07/99 4:44p Jefff
 * fixed double counting of mission failures
 * 
 * 8     9/07/99 1:54p Jefff
 * skip mission cleanup
 * 
 * 7     9/06/99 9:46p Jefff
 * skip mission support
 * 
 * 6     8/11/99 5:47p Jefff
 * fixed button bitmap loading
 * 
 * 5     7/24/99 1:54p Dave
 * Hud text flash gauge. Reworked dead popup to use 4 buttons in red-alert
 * missions.
 * 
 * 4     6/01/99 3:52p Dave
 * View footage screen. Fixed xstrings to not display the & symbol. Popup,
 * dead popup, pxo find player popup, pxo private room popup.
 * 
 * 3     10/13/98 9:29a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 11    6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 10    5/17/98 1:43a Dave
 * Eradicated chatbox problems. Remove speed match for observers. Put in
 * help screens for PXO. Fix messaging and end mission privelges. Fixed
 * team select screen bugs. Misc UI fixes.
 * 
 * 9     5/01/98 4:24p Lawrance
 * Change "Main Hall" to "Flight Deck"
 * 
 * 8     4/22/98 4:59p Allender
 * new multiplayer dead popup.  big changes to the comm menu system for
 * team vs. team.  Start of debriefing stuff for team vs. team  Make form
 * on my wing work with individual ships who have high priority orders
 * 
 * 7     3/12/98 4:02p Lawrance
 * Cleanup how pause works, fix couple pause-related bugs.
 * 
 * 6     3/05/98 12:35p Lawrance
 * Fix bug where two options could be highlighted at once.
 * 
 * 5     2/22/98 4:30p John
 * More string externalization classification
 * 
 * 4     2/22/98 2:48p John
 * More String Externalization Classification
 * 
 * 3     2/22/98 12:19p John
 * Externalized some strings
 * 
 * 2     2/10/98 11:20p Lawrance
 * Implement separate dead popup system
 * 
 * 1     2/10/98 6:02p Lawrance
 *
 * $NoKeywords: $
 */


#define POPUPDEAD_NUM_CHOICES				3		// normal
#ifdef MAKE_FS1
#define POPUPDEAD_NUM_CHOICES_RA			3		// Theres on 3 possible in FS1
#else
#define POPUPDEAD_NUM_CHOICES_RA			4		// red alert
#endif
#define POPUPDEAD_NUM_CHOICES_SKIP		3		// skip mission menu

#ifdef MAKE_FS1
#define POPUPDEAD_NUM_CHOICES_MAX		3
#else
#define POPUPDEAD_NUM_CHOICES_MAX		4
#endif

#include "ui.h"
#include "key.h"
#include "mouse.h"
#include "freespace.h"
#include "gamesnd.h"
#include "keycontrol.h"
#include "player.h"
#include "multi.h"
#include "multiutil.h"
#include "popupdead.h"
#include "alphacolors.h"
#include "gamesequence.h"
#include "popup.h"

UI_WINDOW	Popupdead_window;
UI_BUTTON	Popupdead_buttons[POPUPDEAD_NUM_CHOICES_MAX];				// actual lit buttons
UI_BUTTON	Popupdead_button_regions[POPUPDEAD_NUM_CHOICES_MAX];	// fake buttons used for mouse detection over text

int Popupdead_region_coords[GR_NUM_RESOLUTIONS][POPUPDEAD_NUM_CHOICES_MAX][4] =
{	
	{	// GR_640
#ifdef MAKE_FS1
		{464, 402, 505, 421},		// upper right pixel of text, lower right pixel of button (for tiny popup)
		{464, 422, 505, 442},
		{464, 442, 505, 462},
#else
		{464, 389, 497, 403},		// upper right pixel of text, lower right pixel of button (for tiny popup)
		{464, 413, 497, 427},
		{464, 435, 497, 446},		
		{464, 457, 497, 466},
#endif
	}, 
	{	// GR_1024
		{745, 627, 809, 664},
		{745, 663, 809, 700},		// upper right pixel of text, lower right pixel of button (for tiny popup)
		{745, 699, 809, 736},		
#ifndef MAKE_FS1
		{745, 735, 809, 772},
#endif
	}, 
};

int Popupdead_button_coords[GR_NUM_RESOLUTIONS][POPUPDEAD_NUM_CHOICES_MAX][2] =
{
	{	// GR_640
#ifdef MAKE_FS1
		{474, 394},		// upper left pixel (tiny popup)
		{474, 415},
		{474, 435},
#else
		{478, 387},						// upper left pixel (tiny popup)
		{478, 410},
		{478, 432},
		{478, 455},
#endif
	},
	{	// GR_1024
		{760, 620},						// upper left pixel (tiny popup)
		{760, 656},
		{760, 692},
#ifndef MAKE_FS1
		{760, 728},
#endif
	}
};

const char *Popupdead_background_filename[GR_NUM_RESOLUTIONS] = {
	"PopDeath",		// GR_640
	"2_PopDeath"	// GR-1024
};

int Popupdead_background_coords[GR_NUM_RESOLUTIONS][2] = 
{
	{ // GR_640
		131, 363
	},
	{ // GR_1024
		205, 581
	}
};

const char *Popupdead_button_filenames[GR_NUM_RESOLUTIONS][POPUPDEAD_NUM_CHOICES_MAX] = 
{
	{	// GR_640
		"PopD_00",				// first choice
		"PopD_01",				// second choice
		"PopD_02",				// third choice
#ifndef MAKE_FS1
		"PopD_03",				// fourth choice
#endif
	},
	{	// GR_1024
		"2_PopD_00",			// first choice
		"2_PopD_01",			// second choice
		"2_PopD_02",			// third choice
#ifndef MAKE_FS1
		"2_PopD_03",			// fourth choice
#endif
	}
};

int Popupdead_skip_message_y[GR_NUM_RESOLUTIONS] = { 
		96,	// GR_640
		160
};


static const char *Popupdead_button_text[POPUPDEAD_NUM_CHOICES_MAX];

// multiplayer specifics to help with return values since they can vary
#define POPUPDEAD_OBS_ONLY			1
#define POPUPDEAD_OBS_QUIT			2
#define POPUPDEAD_RESPAWN_ONLY	3
#define POPUPDEAD_RESPAWN_QUIT	4

int Popupdead_default_choice;		// What the default choice is (ie activated when Enter pressed)
int Popupdead_active	=	0;			// A dead popup is active
int Popupdead_choice;				// Index for choice picked (-1 if none picked)
int Popupdead_num_choices;			// number of buttons
int Popupdead_multi_type;			// what kind of popup is active for muliplayer
int Popupdead_skip_active = 0;	// The skip-misison popup is active
int Popupdead_skip_already_shown = 0;

// Initialize the dead popup data
void popupdead_start()
{
	int			i;
	UI_BUTTON	*b;

	if ( Popupdead_active ) {
		// Int3();
		return;
	}

	// increment number of deaths
	Player->failures_this_session++;


	// create base window
	Popupdead_window.create(Popupdead_background_coords[gr_screen.res][0], Popupdead_background_coords[gr_screen.res][1], 1, 1, 0);
	Popupdead_window.set_foreground_bmap(Popupdead_background_filename[gr_screen.res]);

	Popupdead_num_choices = 0;
	Popupdead_multi_type = -1;

	if ( Game_mode & GM_NORMAL ) {
		// also do a campaign check here?
		if (0) { //((Player->show_skip_popup) && (!Popupdead_skip_already_shown) && (Game_mode & GM_CAMPAIGN_MODE) && (Game_mode & GM_NORMAL) && (Player->failures_this_session >= PLAYER_MISSION_FAILURE_LIMIT)) {
			// init the special preliminary death popup that gives the skip option
			Popupdead_button_text[0] = XSTR( "Do Not Skip This Mission", 1473);
			Popupdead_button_text[1] = XSTR( "Advance To The Next Mission", 1474);
			Popupdead_button_text[2] = XSTR( "Don't Show Me This Again", 1475);
			Popupdead_num_choices = POPUPDEAD_NUM_CHOICES_SKIP;
			Popupdead_skip_active = 1;
		} else if(The_mission.red_alert) {
			// We can't staticly declare these because they are externalized
			Popupdead_button_text[0] = XSTR( "Quick Start Mission", 105);
			Popupdead_button_text[1] = XSTR( "Return To Flight Deck", 106);
			Popupdead_button_text[2] = XSTR( "Return To Briefing", 107);
#ifndef MAKE_FS1
			Popupdead_button_text[3] = XSTR( "Replay previous mission", 1432);
#endif
			Popupdead_num_choices = POPUPDEAD_NUM_CHOICES_RA;
		} else {
			Popupdead_button_text[0] = XSTR( "Quick Start Mission", 105);
			Popupdead_button_text[1] = XSTR( "Return To Flight Deck", 106);
			Popupdead_button_text[2] = XSTR( "Return To Briefing", 107);
			Popupdead_num_choices = POPUPDEAD_NUM_CHOICES;
		}
	} else {
		// in multiplayer, we have different choices depending on respawn mode, etc.

		// if the player has run out of respawns and must either quit and become an observer
		if(Net_player->flags & NETINFO_FLAG_LIMBO){

			// the master should not be able to quit the game
			if( ((Net_player->flags & NETINFO_FLAG_AM_MASTER) && (multi_num_players() > 1)) || (Net_player->flags & NETINFO_FLAG_TEAM_CAPTAIN) ) {
				Popupdead_button_text[0] = XSTR( "Observer Mode", 108);
				Popupdead_num_choices = 1;
				Popupdead_multi_type = POPUPDEAD_OBS_ONLY;
			} else {
				Popupdead_button_text[0] = XSTR( "Observer Mode", 108);
				Popupdead_button_text[1] = XSTR( "Return To Flight Deck", 106);
				Popupdead_num_choices = 2;
				Popupdead_multi_type = POPUPDEAD_OBS_QUIT;
			}
		} else {
			// the master of the game should not be allowed to quit
			if ( ((Net_player->flags & NETINFO_FLAG_AM_MASTER) && (multi_num_players() > 1)) || (Net_player->flags & NETINFO_FLAG_TEAM_CAPTAIN) ) {
				Popupdead_button_text[0] = XSTR( "Respawn", 109);
				Popupdead_num_choices = 1;
				Popupdead_multi_type = POPUPDEAD_RESPAWN_ONLY;
			} else {
				Popupdead_button_text[0] = XSTR( "Respawn", 109);
				Popupdead_button_text[1] = XSTR( "Return To Flight Deck", 106);
				Popupdead_num_choices = 2;
				Popupdead_multi_type = POPUPDEAD_RESPAWN_QUIT;
			}
		}
	}

	// create buttons
	for (i=0; i < Popupdead_num_choices; i++) {
		b = &Popupdead_buttons[i];
		b->create(&Popupdead_window, "", Popupdead_button_coords[gr_screen.res][i][0], Popupdead_button_coords[gr_screen.res][i][1], 30, 20, 0, 1);
		b->set_bmaps(Popupdead_button_filenames[gr_screen.res][i], 3, 0);
		b->set_highlight_action(common_play_highlight_sound);

		// create invisible buttons to detect mouse presses... can't use mask since button region is dynamically sized
		int lx, w, h;
		gr_get_string_size(&w, &h, Popupdead_button_text[i]);
		lx = Popupdead_region_coords[gr_screen.res][i][0] - w;
		b = &Popupdead_button_regions[i];	
		b->create(&Popupdead_window, "", lx, Popupdead_region_coords[gr_screen.res][i][1], Popupdead_region_coords[gr_screen.res][i][2]-lx, Popupdead_region_coords[gr_screen.res][i][3]-Popupdead_region_coords[gr_screen.res][i][1], 0, 1);
		b->hide();
	}
	
	Popupdead_default_choice = 0;
	Popupdead_choice = -1;
	Popupdead_active = 1;
}

// maybe play a sound when key up/down is pressed to switch default choice
void popupdead_play_default_change_sound()
{
	int i, mouse_over=0;
	UI_BUTTON *br, *b;

	// only play if mouse not currently highlighting a choice
	for ( i = 0; i < Popupdead_num_choices; i++ ) {
		br = &Popupdead_button_regions[i];
		b = &Popupdead_buttons[i];
		if ( br->button_down() ) {
			mouse_over=1;
			break;
		}

		if ( br->button_hilighted() && !b->button_down() ) {
			mouse_over=1;
			break;
		}

		if ( b->button_hilighted() ) {
			mouse_over=1;
		}
	}

	if (!mouse_over) {
		gamesnd_play_iface(SND_USER_SELECT);
	}
}

// do any key processing here
// exit:	-1		=>	nothing was done
//			>=0	=> a choice was selected
int popupdead_process_keys(int k)
{
	int masked_k;

	if ( k <= 0 ) {
		return -1;
	}
	
	switch(k) {

	case KEY_ENTER:
		return Popupdead_default_choice;	// select the current default choice
		break;

	case KEY_ESC:
		if (Popupdead_skip_active) {
			return 0;								// 0 mimics a "do not skip"
		} else {
			return 1;								// do nothing here for now - 1 mimics a "return to flight deck"
		}
		break;

	case KEY_DOWN:
	case KEY_PAD2:
	case KEY_TAB:
		popupdead_play_default_change_sound();
		Popupdead_default_choice++;
		if ( Popupdead_default_choice >= Popupdead_num_choices ) {
			Popupdead_default_choice=0;
		}
		break;

	case KEY_UP:
	case KEY_PAD8:
	case KEY_SHIFTED+KEY_TAB:
		popupdead_play_default_change_sound();
		Popupdead_default_choice--;
		if ( Popupdead_default_choice < 0 ) {
			Popupdead_default_choice=Popupdead_num_choices-1;
		}
		break;

	case KEY_PAUSE:
		game_process_pause_key();
		break;

	default:
		break;
	} // end switch

	// read the dead key set
	masked_k = k & ~KEY_CTRLED;	// take out CTRL modifier only
	process_set_of_keys(masked_k, Dead_key_set_size, Dead_key_set);
	button_info_do(&Player->bi);	// call functions based on status of button_info bit vectors

	return -1;
}


// see if any popup buttons have been pressed
// exit: -1						=> no buttons pressed
//			>=0					=>	button index that was pressed
int popupdead_check_buttons()
{
	int			i;
	UI_BUTTON	*b;

	for ( i = 0; i < Popupdead_num_choices; i++ ) {
		b = &Popupdead_button_regions[i];
		if ( b->pressed() ) {
			return i;
		}

		b = &Popupdead_buttons[i];
		if ( b->pressed() ) {
			return i;
		}
	}

	return -1;
}

// See if any of the button should change appearance based on mouse position
void popupdead_force_draw_buttons()
{
	int i,mouse_is_highlighting=0;
	UI_BUTTON *br, *b;

	for ( i = 0; i < Popupdead_num_choices; i++ ) {
		br = &Popupdead_button_regions[i];
		b = &Popupdead_buttons[i];
		if ( br->button_down() ) {
			b->draw_forced(2);
			mouse_is_highlighting=1;
			continue;
		}

		if ( (b->button_hilighted()) || (br->button_hilighted() && !b->button_down()) ) {
			Popupdead_default_choice=i;
			mouse_is_highlighting=1;
			b->draw_forced(1);
		}
	}

	// Only if mouse is not highlighting an option, let the default choice be drawn highlighted
	if ( (!mouse_is_highlighting) && (Popupdead_num_choices>1) ) {
		for ( i = 0; i < Popupdead_num_choices; i++ ) {
			b = &Popupdead_buttons[i];
			// highlight the default choice
			if ( i == Popupdead_default_choice ) {
				b->draw_forced(1);
			}
		}
	}
}

// Draw the button text nicely formatted in the popup
void popupdead_draw_button_text()
{
	int w,h,i,sx,sy;

	gr_set_color_fast(&Color_bright_blue);

	for ( i=0; i < Popupdead_num_choices; i++ ) {
		gr_get_string_size(&w, &h, Popupdead_button_text[i]);
		sx = Popupdead_region_coords[gr_screen.res][i][0]-w;
		sy = Popupdead_region_coords[gr_screen.res][i][1]+4;
		gr_string(sx, sy, Popupdead_button_text[i]);
	}
}

// reinits the death popup
// used for skip popup to death popup transition
void popupdead_restart() {
	Popupdead_skip_already_shown = 1;
	Popupdead_skip_active = 0;
	Popupdead_active = 0;
	Player->failures_this_session--;		// hacky correction for a double count when calling popupdead_start() twice
	popupdead_start();
}

// do the "skip mission" version of the death popup
int popupdead_skip_do_frame()
{
	int k = Popupdead_window.process();

	// check for input
	int choice = popupdead_process_keys(k);
	if (choice < 0) {
		choice = popupdead_check_buttons();
	}

	// take appropriate options
	switch (choice) {
	case 0:
		// try this mission again, so proceed to normal death popup
		gamesnd_play_iface(SND_USER_SELECT);
		popupdead_restart();
		break;
	case 1:
		// skip this mission
		mission_campaign_skip_to_next();

		gamesnd_play_iface(SND_USER_SELECT);
		break;
	case 2:
		// dont show this again
		Player->show_skip_popup = 0;
		
		gamesnd_play_iface(SND_USER_SELECT);
		popupdead_restart();
		break;
	}

	// render
	Popupdead_window.draw();
	popupdead_force_draw_buttons();
	popupdead_draw_button_text();

	// render skip mission message
	gr_set_color_fast(&Color_bright_white);
	gr_string(0x8000, Popupdead_skip_message_y[gr_screen.res], XSTR("You have failed this mission five times.", 1470));
	gr_string(0x8000, Popupdead_skip_message_y[gr_screen.res] + gr_get_font_height(), XSTR("If you like, you may advance to the next mission.", 1471));

	return choice;
}


// Called once per frame to run the dead popup
int popupdead_do_frame(float frametime)
{
	int k, choice;

/*
	if (Popupdead_skip_active) {
		// do the skip mission popup, and thats all
		if (popupdead_skip_do_frame() == 1) {
			return 2;	// fake the "go to briefing" choice
		} else {
			return -1;	// fake no choice
		}
	}
*/
	if ( !Popupdead_active ) {
		return -1;
	}

	// dont let dude skip 3-09.  hack.
	if(Game_mode & GM_CAMPAIGN_MODE){
		if((Campaign.current_mission >= 0) && (Campaign.current_mission < MAX_CAMPAIGN_MISSIONS)){
			if ((Campaign.missions[Campaign.current_mission].name != NULL) && !stricmp(Campaign.missions[Campaign.current_mission].name, "sm3-09.fs2")) {
				Popupdead_skip_already_shown = 1;
			}
		}
	}

	// maybe show skip mission popup
	if ((!Popupdead_skip_already_shown) && (Player->show_skip_popup) && (Game_mode & GM_NORMAL) && (Game_mode & GM_CAMPAIGN_MODE) && (Player->failures_this_session >= PLAYER_MISSION_FAILURE_LIMIT)) {
		int popup_choice = popup(0, 3, XSTR("Do Not Skip This Mission", 1473),
												 XSTR("Advance To The Next Mission", 1474),
												 XSTR("Don't Show Me This Again", 1475),
												 XSTR("You have failed this mission five times.  If you like, you may advance to the next mission.", 1472) );
		switch (popup_choice) {
		case 0:
			// stay on this mission, so proceed to normal death popup
			// in other words, do nothing.
			break;
		case 1:
			// skip this mission
			Popupdead_active = 0;
			mission_campaign_skip_to_next();
			gameseq_post_event(GS_EVENT_START_GAME);
			return -1;
		case 2:
			// dont show this again
			Player->show_skip_popup = 0;
			break;
		}

		Popupdead_skip_already_shown = 1;
	}

	
	k = Popupdead_window.process();

	choice = popupdead_process_keys(k);
	if ( choice >= 0 ) {
		// do something different for single/multiplayer
		if ( Game_mode & GM_NORMAL ) {
			Popupdead_choice=choice;
		} else {
			Assert( Popupdead_multi_type != -1 );
			switch ( Popupdead_multi_type ) {
				
			case POPUPDEAD_OBS_ONLY:
			case POPUPDEAD_OBS_QUIT:
				Popupdead_choice = POPUPDEAD_DO_OBSERVER;
				if ( (Popupdead_multi_type == POPUPDEAD_OBS_QUIT) && (choice == 1) )
					Popupdead_choice = POPUPDEAD_DO_MAIN_HALL;
				break;

			case POPUPDEAD_RESPAWN_ONLY:
			case POPUPDEAD_RESPAWN_QUIT:
				Popupdead_choice = POPUPDEAD_DO_RESPAWN;
				if ( (Popupdead_multi_type == POPUPDEAD_RESPAWN_QUIT) && (choice == 1) )
					Popupdead_choice = POPUPDEAD_DO_MAIN_HALL;
				break;

			default:
				Int3();
				break;
			}
		}
	}

	choice = popupdead_check_buttons();
	if ( choice >= 0 ) {
		// do something different for single/multiplayer
		if ( Game_mode & GM_NORMAL ) {
			Popupdead_choice=choice;
		} else {
			Assert( Popupdead_multi_type != -1 );
			switch ( Popupdead_multi_type ) {
				
			case POPUPDEAD_OBS_ONLY:
			case POPUPDEAD_OBS_QUIT:
				Popupdead_choice = POPUPDEAD_DO_OBSERVER;
				if ( (Popupdead_multi_type == POPUPDEAD_OBS_QUIT) && (choice == 1) )
					Popupdead_choice = POPUPDEAD_DO_MAIN_HALL;
				break;

			case POPUPDEAD_RESPAWN_ONLY:
			case POPUPDEAD_RESPAWN_QUIT:
				Popupdead_choice = POPUPDEAD_DO_RESPAWN;
				if ( (Popupdead_multi_type == POPUPDEAD_RESPAWN_QUIT) && (choice == 1) )
					Popupdead_choice = POPUPDEAD_DO_MAIN_HALL;
				break;

			default:
				Int3();
				break;
			}
		}
	}

	Popupdead_window.draw();
	popupdead_force_draw_buttons();
	popupdead_draw_button_text();

	return Popupdead_choice;
}

// Close down the dead popup
void popupdead_close()
{
	if ( !Popupdead_active ) {
		return;
	}

	gamesnd_play_iface(SND_POPUP_DISAPPEAR);
	Popupdead_window.destroy();
	game_flush();

	Popupdead_active = 0;
	Popupdead_skip_active = 0;
	Popupdead_skip_already_shown = 0;
}

// Is there a dead popup active?
int popupdead_is_active()
{
	return Popupdead_active;
} 

