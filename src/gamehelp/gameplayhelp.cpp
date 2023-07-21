/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/GameHelp/GameplayHelp.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Module for displaying in-game help
 *
 * $Log$
 * Revision 1.6  2005/03/29 02:18:47  taylor
 * Various 64-bit platform fixes
 * Fix compiler errors with MAKE_FS1 and fix gr_set_bitmap() too
 * Make sure that turrets can fire at asteroids for FS1 (needed for a couple missions)
 * Streaming audio support (big thanks to Pierre Willenbrock!!)
 * Removed dependance on strings.tbl for FS1 since we don't actually need it now
 *
 * Revision 1.5  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.4  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.3  2002/06/09 04:41:17  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:44  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 8     9/08/99 2:38p Jefff
 * sound pausing going to menu from game
 * 
 * 7     9/01/99 2:56p Jefff
 * a few control key description changes
 * 
 * 6     7/29/99 4:43p Jefff
 * updated for fs2 -- fixed buttons, added "press esc" text, hi-res
 * support, the works.
 * 
 * 5     7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 4     1/30/99 5:08p Dave
 * More new hi-res stuff.Support for nice D3D textures.
 * 
 * 3     10/13/98 9:28a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 21    6/17/98 11:01a Lawrance
 * ensure translated descriptions appear in the online help
 * 
 * 20    6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 19    5/17/98 3:33p Lawrance
 * Add 'target last ship to send transmission' key to online help
 * 
 * 18    5/15/98 8:36p Lawrance
 * Add 'target ship that last sent transmission' target key
 * 
 * 17    5/09/98 4:52p Lawrance
 * Implement padlock view (up/rear/left/right)
 * 
 * 16    4/27/98 10:11a Lawrance
 * Add in disabled beep for missing buttons
 * 
 * 15    4/25/98 1:25p Lawrance
 * Change screen shot key to Print Scrn
 * 
 * 14    4/22/98 2:50p John
 * Made PrintScreen actually be Shift+PrintScreen.
 * 
 * 13    4/18/98 9:21p Lawrance
 * show correct key binding for screen shots
 * 
 * 12    4/11/98 7:59p Lawrance
 * Allow space, tab and enter to cycle through pages
 * 
 * 11    4/05/98 7:44p Lawrance
 * remove obsolete 'show goals' binding
 * 
 * 10    4/02/98 11:40a Lawrance
 * check for #ifdef DEMO instead of #ifdef DEMO_RELEASE
 * 
 * 9     4/01/98 2:22p Lawrance
 * remove hotkey references in on-line help for demo builds
 * 
 * 8     3/31/98 4:57p Lawrance
 * update text for taking a screen shot
 * 
 * 7     3/30/98 1:31p Lawrance
 * Take out debug key help
 * 
 * 6     3/25/98 5:34p Dave
 * First rev of multiplayer online key help.
 * 
 * 5     3/25/98 2:16p Dave
 * Select random default image for newly created pilots. Fixed several
 * multi-pause messaging bugs. Begin work on online help for multiplayer
 * keys.
 * 
 * 4     3/18/98 12:03p John
 * Marked all the new strings as externalized or not.
 * 
 * 3     3/16/98 5:53p Lawrance
 * Fix bug that was causing joystick only bindings to mess up
 * 
 * 2     3/09/98 9:54p Lawrance
 * integrate new art for gameplay help
 * 
 * 1     3/09/98 5:05p Lawrance
 *
 * $NoKeywords: $
 */

#include "contexthelp.h"
#include "gamesequence.h"
#include "freespace.h"
#include "key.h"
#include "bmpman.h"
#include "2d.h"
#include "timer.h"
#include "math.h"
#include "mouse.h"
#include "controlsconfig.h"
#include "ui.h"
#include "gamesnd.h"
#include "missionscreencommon.h"
#include "alphacolors.h"
#include "beam.h"
#include "audiostr.h"

// text positioning constats
#define TITLE_Y			35
#define Y_START			70
#define X_OFFSET_1		70

// pixel offset for description of key from where key binding starts
#define KEY_DESCRIPTION_OFFSET	193	

// different gameplay help pages
#define GP_FIRST_SCREEN									0		// keep up to date

#define GP_HELP_BASIC_KEYS								0
#define GP_HELP_MOVEMENT_KEYS							1
#define GP_HELP_COMMON_TARGET_KEYS					2
#define GP_HELP_ADVANCED_TARGET_KEYS				3
#define GP_HELP_MESSAGING								4
#define GP_HELP_WEAPON_KEYS							5
#define GP_HELP_THROTTLE_AND_ETS_KEYS				6
#define GP_HELP_VIEW_KEYS								7
#define GP_HELP_MISC_KEYS								8
#define GP_HELP_MULTI_KEYS								9

#define GP_LAST_SCREEN_SINGLE							8		// keep up to date
#define GP_LAST_SCREEN_MULTI							9		// keep up to date

// set this to GP_LAST_SCREEN_SINGLE or GP_LAST_SCREEN_MULTI based upon what game mode we're in
int Gp_last_screen;

#define NUM_BUTTONS						3
#define PREVIOUS_PAGE_BUTTON			0
#define NEXT_PAGE_BUTTON				1
#define CONTINUE_BUTTON					2

struct gameplay_help_buttons {
	const char *filename;
	int x, y;
	int hotspot;
	int tab;
	int flags;
	UI_BUTTON button;  // because we have a class inside this struct, we need the constructor below..

	gameplay_help_buttons(const char *name, int x1, int y1, int h) : filename(name), x(x1), y(y1), hotspot(h) {}
};

static gameplay_help_buttons Buttons[GR_NUM_RESOLUTIONS][NUM_BUTTONS] = {
//XSTR:OFF
	{
#ifdef MAKE_FS1
		gameplay_help_buttons("F1B_00",	71,		373,	0),		// previous
		gameplay_help_buttons("F1B_01",	121,	373,	1),		// next
//		gameplay_help_buttons("F1B_02",	447,	452,	2),		// options
		gameplay_help_buttons("F1B_03",	554,	411,	3),		// continue
#else
		gameplay_help_buttons("F1B_00",	15,	389,	0),
		gameplay_help_buttons("F1B_01",	60,	389,	1),
		gameplay_help_buttons("F1B_02",	574,	431,	2)
#endif
	},
	{
		gameplay_help_buttons("2_F1B_00",	24,	622,	0),
		gameplay_help_buttons("2_F1B_01",	96,	622,	1),
		gameplay_help_buttons("2_F1B_02",	919,	689,	2)
	},
//XSTR:ON
};

#ifndef MAKE_FS1
#define GAME_HELP_NUM_TEXT		2

static UI_XSTR Game_help_text[GR_NUM_RESOLUTIONS][GAME_HELP_NUM_TEXT] = {
	{ // GR_640
		{ "Press ESC to return to the game",	1441,	263,	389,	UI_XSTR_COLOR_GREEN, -1, NULL },
		{ "Continue",		1069,		571,	413,	UI_XSTR_COLOR_GREEN, -1, &Buttons[gr_screen.res][CONTINUE_BUTTON].button }
	}, 
	{ // GR_1024
		// not needed for FS1
		{ "Press ESC to return to the game",	1441,	421,	622,	UI_XSTR_COLOR_GREEN, -1, NULL },
		{ "Continue",		1069,		928,	663,	UI_XSTR_COLOR_GREEN, -1, &Buttons[gr_screen.res][CONTINUE_BUTTON].button }
	}
};
#endif

static const char *Game_help_filename[GR_NUM_RESOLUTIONS] = {
	"F1",
	"2_F1"
};

static const char *Game_help_mask_filename[GR_NUM_RESOLUTIONS] = {
	"F1-m",
	"2_F1-m"
};

static UI_WINDOW Ui_window;
static int Background_bitmap;
static int Gameplay_help_inited = 0;

static int Current_help_page;

// generate a line for the on-line help for a control item with specified id
// input:	id		=>	index for control item within Control_config[]
//				buf	=> buffer with enough space to hold ouput string
char *gameplay_help_control_text(int id, char *buf)
{
	int			has_key=0, has_joy=0;
	config_item	*ci;

	ci = &Control_config[id];

	if ( ci->key_id >= 0 ) {
		sprintf(buf, textify_scancode(ci->key_id));
		has_key=1;
	}

	if ( ci->joy_id >= 0 ) {
		if ( has_key ) {
			strcat(buf, XSTR( ", ", 129));
		}
		strcat(buf, Joy_button_text[ci->joy_id]);
		has_joy=1;
	}

	if ( !has_key && !has_joy ) {
		strcpy(buf, XSTR( "no binding", 130));
	}

	strcat(buf, XSTR( " - ", 131));
	strcat(buf, ci->text);

	return buf;
}

void gameplay_help_blit_control_line(int x, int y, int id)
{
	int			has_key=0, has_joy=0;
	char			buf[256];
	config_item	*ci;

	ci = &Control_config[id];

	buf[0] = 0;

	if ( ci->key_id >= 0 ) {
		sprintf(buf, textify_scancode(ci->key_id));
		has_key=1;
	}

	if ( ci->joy_id >= 0 ) {
		if ( has_key ) {
			strcat(buf, XSTR( ", ", 129));
		}
		strcat(buf, Joy_button_text[ci->joy_id]);
		has_joy=1;
	}

	if ( !has_key && !has_joy ) {
		strcpy(buf, XSTR( "no binding", 130));
	}

	gr_string(x,y,buf);

//	gr_string(x+KEY_DESCRIPTION_OFFSET,y,ci->text);
	gr_string(x+KEY_DESCRIPTION_OFFSET, y, XSTR(ci->text, CONTROL_CONFIG_XSTR + id));
}

void gameplay_help_blit_control_line_raw(int x, int y, const char *control_text, const char *control_description)
{
	gr_string(x,y,control_text);
	gr_string(x+KEY_DESCRIPTION_OFFSET,y,control_description);
}

// game_play_help_set_title() will display the title for the help screen and
// set the font for the rest of the screen
void gameplay_help_set_title(const char *title)
{
	int sy=TITLE_Y;
	char buf[128];

	gr_set_color_fast(&Color_bright);
	gr_printf(0x8000,sy,title);
	sprintf(buf, XSTR( "Page %d of %d", 132),  Current_help_page+1, Gp_last_screen+1);
	gr_printf(0x8000,sy+gr_get_font_height()+2,buf);
	gr_set_color_fast(&Color_normal);
}

// called once when the gameplay help state is entered
void gameplay_help_init()
{
	int i;
	gameplay_help_buttons *b;

	if ( Gameplay_help_inited ) {
		return;
	}

	common_set_interface_palette("InterfacePalette");  // set the interface palette
	Ui_window.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);
	Ui_window.set_mask_bmap(Game_help_mask_filename[gr_screen.res]);

	for (i=0; i<NUM_BUTTONS; i++) {
		b = &Buttons[gr_screen.res][i];

		b->button.create(&Ui_window, "", b->x, b->y, 60, 30, 0, 1);
		// set up callback for when a mouse first goes over a button
		b->button.set_highlight_action(common_play_highlight_sound);
		b->button.set_bmaps(b->filename);
		b->button.link_hotspot(b->hotspot);
	}

#ifndef MAKE_FS1
	// add all xstrs
	for (i=0; i<GAME_HELP_NUM_TEXT; i++)
	{
		Ui_window.add_XSTR(&Game_help_text[gr_screen.res][i]);
	}
#endif

	// set the proper last screen # based upon game mode
	if(Game_mode & GM_MULTIPLAYER){
		Gp_last_screen = GP_LAST_SCREEN_MULTI;
	} else {
		Gp_last_screen = GP_LAST_SCREEN_SINGLE;
	}

	// setup hotkeys so lights flash when keys are pressed
	Buttons[gr_screen.res][CONTINUE_BUTTON].button.set_hotkey(KEY_CTRLED | KEY_ENTER);
	Buttons[gr_screen.res][PREVIOUS_PAGE_BUTTON].button.set_hotkey(KEY_LEFT);
	Buttons[gr_screen.res][NEXT_PAGE_BUTTON].button.set_hotkey(KEY_RIGHT);

	Background_bitmap = bm_load(Game_help_filename[gr_screen.res]);

	Current_help_page = GP_HELP_BASIC_KEYS;
	Gameplay_help_inited = 1;
}

// called when moving to the previous help page
void gameplay_help_goto_prev_screen()
{
	Current_help_page--;
	if (Current_help_page < GP_FIRST_SCREEN) {
		Current_help_page = Gp_last_screen;
	}
	gamesnd_play_iface(SND_SWITCH_SCREENS);

}

// called when proceeding to the next help page
void gameplay_help_goto_next_screen()
{
	Current_help_page++;
	if (Current_help_page > Gp_last_screen) {
		Current_help_page = GP_FIRST_SCREEN;
	}
	gamesnd_play_iface(SND_SWITCH_SCREENS);
}

// called when the screen is exited
void gameplay_help_leave()
{
	// unpause all game sounds
	beam_unpause_sounds();
	audiostream_unpause_all();

	gameseq_post_event(GS_EVENT_PREVIOUS_STATE);
	game_flush();
}

// deal with a keypress on the gameplay help screen
void gameplay_help_process_key(int k)
{
	switch ( k ) {
		case KEY_ESC:
			gameplay_help_leave();
			break;

		case KEY_ENTER:
		case KEY_SPACEBAR:
		case KEY_TAB:
			//gameplay_help_goto_next_screen();
			Buttons[gr_screen.res][NEXT_PAGE_BUTTON].button.press_button();
			break;

		case KEY_SHIFTED | KEY_TAB:
			Buttons[gr_screen.res][PREVIOUS_PAGE_BUTTON].button.press_button();
//			gameplay_help_goto_prev_screen();
			break;

		default:
			break;

	} // end switch
}

// deal with buttons being pressed on the gameplay help screen
void gameplay_help_button_pressed(int n)
{
	switch (n) {

	case PREVIOUS_PAGE_BUTTON:
		gameplay_help_goto_prev_screen();
		break;

	case NEXT_PAGE_BUTTON:
		gameplay_help_goto_next_screen();
		break;

	case CONTINUE_BUTTON:
		gameplay_help_leave();
		gamesnd_play_iface(SND_COMMIT_PRESSED);
		break;

	default:
		Int3();
		break;
	}
}

// Draw the help text onto the screen
void gameplay_help_draw_text()
{
	int x_offset, y_offset, separation;

	separation = gr_get_font_height() + 3;

	switch ( Current_help_page ) {

		case GP_HELP_BASIC_KEYS:
			gameplay_help_set_title(XSTR( "Basic Keys", 133));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,END_MISSION);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,FIRE_PRIMARY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MAX_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ZERO_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TOGGLE_AUTO_TARGETING);

			y_offset += separation;
			y_offset += separation;

			gr_set_color_fast(&Color_bright);
			gr_printf(0x8000,y_offset,XSTR( "Function Keys", 134));
			gr_set_color_fast(&Color_normal);

			y_offset += separation;
			y_offset += separation;

			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "F1", 135), XSTR( "context-sensitive help", 136));

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "F2", 137), XSTR( "options screen (available anywhere in game)", 138));

#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "F3", 139), XSTR( "hotkey assignment", 140));
#endif

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "F4", 141), XSTR( "HUD message scroll-back", 142));

#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "F5...F12", 143), XSTR( "hotkeys", 144));
#endif

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "Shift-Esc", 145), XSTR( "quit FreeSpace 2 immediately", 146));

			break;

		case GP_HELP_MOVEMENT_KEYS:
			gameplay_help_set_title(XSTR( "Movement Keys", 147));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,FORWARD_THRUST);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MAX_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ONE_THIRD_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TWO_THIRDS_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MINUS_5_PERCENT_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PLUS_5_PERCENT_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ZERO_THROTTLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,AFTERBURNER);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,REVERSE_THRUST);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PITCH_FORWARD);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PITCH_BACK);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,YAW_LEFT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,YAW_RIGHT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,BANK_LEFT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,BANK_RIGHT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,BANK_WHEN_PRESSED);
			break;

		case GP_HELP_COMMON_TARGET_KEYS:
			gameplay_help_set_title(XSTR( "Basic Targeting Keys", 148));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TOGGLE_AUTO_TARGETING);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_SHIP_IN_RETICLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_CLOSEST_HOSTILE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV_CLOSEST_HOSTILE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_CLOSEST_FRIENDLY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV_CLOSEST_FRIENDLY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_SUBOBJECT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV_SUBOBJECT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_SUBOBJECT_IN_RETICLE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MATCH_TARGET_SPEED);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TOGGLE_AUTO_MATCH_TARGET_SPEED);
			break;

		case GP_HELP_ADVANCED_TARGET_KEYS:
			gameplay_help_set_title(XSTR( "Advanced Targeting Keys", 149));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_CLOSEST_SHIP_ATTACKING_SELF);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_CLOSEST_SHIP_ATTACKING_TARGET);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_LAST_TRANMISSION_SENDER);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_TARGETS_TARGET);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_ESCORT_SHIP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_CLOSEST_REPAIR_SHIP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_UNINSPECTED_CARGO);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV_UNINSPECTED_CARGO);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEWEST_SHIP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_BOMB);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV_BOMB);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,STOP_TARGETING_SHIP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_NEXT_LIVE_TURRET);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TARGET_PREV_LIVE_TURRET);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,STOP_TARGETING_SUBSYSTEM);

#if !(defined(FS2_DEMO) || defined(FS1_DEMO))

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset, y_offset, XSTR( "F5...F12", 143), XSTR( "Select target assigned to that hotkey", 150));

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset, y_offset, XSTR( "Shift-F5...F12", 151), XSTR( "Add/remove target from that hotkey", 152));

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset, y_offset, XSTR( "Alt-Shift-F5...F12", 153), XSTR( "Clear that hotkey", 154));

#endif

			break;

		case GP_HELP_MESSAGING:
			gameplay_help_set_title(XSTR( "Messaging Keys", 155));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,SQUADMSG_MENU);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,REARM_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ATTACK_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,DISARM_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,DISABLE_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,CAPTURE_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ENGAGE_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,FORM_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,IGNORE_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PROTECT_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,COVER_MESSAGE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,WARP_MESSAGE);

#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset, y_offset, XSTR( "F5...F12", 143), XSTR( "send specified order to these target(s)", 156));
#endif
			break;

		case GP_HELP_WEAPON_KEYS:
			gameplay_help_set_title(XSTR( "Weapon Keys", 157));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,FIRE_PRIMARY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,FIRE_SECONDARY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,CYCLE_NEXT_PRIMARY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,CYCLE_PREV_PRIMARY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,CYCLE_SECONDARY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,CYCLE_NUM_MISSLES);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,LAUNCH_COUNTERMEASURE);
			break;

		case GP_HELP_THROTTLE_AND_ETS_KEYS:
			gameplay_help_set_title(XSTR( "Energy Management Keys", 158));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,SHIELD_EQUALIZE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,SHIELD_XFER_TOP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,SHIELD_XFER_BOTTOM);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,SHIELD_XFER_LEFT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,SHIELD_XFER_RIGHT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,INCREASE_WEAPON);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,DECREASE_WEAPON);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,INCREASE_SHIELD);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,DECREASE_SHIELD);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,INCREASE_ENGINE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,DECREASE_ENGINE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ETS_EQUALIZE);
/*
			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,XFER_LASER);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,XFER_SHIELD);
*/
			break;

		case GP_HELP_MISC_KEYS:
			gameplay_help_set_title(XSTR( "Miscellaneous Keys", 159));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			// ending mission
			//
			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,END_MISSION);

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "ESC", 160), XSTR( "invoke abort mission popup", 161));

			// time compression
			//
			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TIME_SPEED_UP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,TIME_SLOW_DOWN);

			// radar keys
			//
			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,RADAR_RANGE_CYCLE);

			// escort view keys
			//
			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ADD_REMOVE_ESCORT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,ESCORT_CLEAR);

			// Pause, Print Screenshot
			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "Pause", 162), XSTR( "pause game", 163));            

			y_offset += separation;
			gameplay_help_blit_control_line_raw(x_offset,y_offset, XSTR( "Print Scrn", 164), XSTR( "take screen shot", 165));            

			break;

		case GP_HELP_VIEW_KEYS:
			gameplay_help_set_title(XSTR( "View Keys", 166));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			gameplay_help_blit_control_line(x_offset, y_offset,PADLOCK_UP);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PADLOCK_DOWN);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PADLOCK_LEFT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,PADLOCK_RIGHT);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_CHASE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_EXTERNAL);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_EXTERNAL_TOGGLE_CAMERA_LOCK);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_SLEW);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_DIST_INCREASE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_DIST_DECREASE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_CENTER);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,VIEW_OTHER_SHIP);
			break;

		case GP_HELP_MULTI_KEYS:
			gameplay_help_set_title(XSTR( "Multiplayer Keys", 167));
			x_offset=X_OFFSET_1;
			y_offset=Y_START;

			// ingame messaging
			gr_set_color_fast(&Color_bright);
			gr_printf(0x8000,y_offset,XSTR( "Ingame messaging keys (tap for text, hold for voice)", 168));
			gr_set_color_fast(&Color_normal);
			
			y_offset += separation;
			
			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MULTI_MESSAGE_ALL);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MULTI_MESSAGE_FRIENDLY);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MULTI_MESSAGE_HOSTILE);

			y_offset += separation;
			gameplay_help_blit_control_line(x_offset, y_offset,MULTI_MESSAGE_TARGET);

			break;

	} //	end switch		
}

// gameplay_help_do_frame() is the function that displays help when acutally playing the game
void gameplay_help_do_frame(float frametime)
{
	int i, k;	

	// ensure the gameplay help interface has been initialized
	if (!Gameplay_help_inited) {
		Int3();
		return;
	}

	// make sure game sounds are paused
	beam_pause_sounds();
	audiostream_pause_all();

	k = Ui_window.process() & ~KEY_DEBUGGED;
	gameplay_help_process_key(k);

	for (i=0; i<NUM_BUTTONS; i++){
		if (Buttons[gr_screen.res][i].button.pressed()){
			gameplay_help_button_pressed(i);
		}
	}
	
	GR_MAYBE_CLEAR_RES(Background_bitmap);
	if (Background_bitmap >= 0) {
		gr_set_bitmap(Background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	}

	Ui_window.draw();

	gameplay_help_draw_text();
	gr_flip();
}


// called once when leaving the gameplay help state
void gameplay_help_close()
{
	if ( Gameplay_help_inited ) {
		if (Background_bitmap >= 0) {
			bm_unload(Background_bitmap);
		}

		Ui_window.destroy();
		common_free_interface_palette();		// restore game palette
		game_flush();
	}

	Gameplay_help_inited = 0;
}

