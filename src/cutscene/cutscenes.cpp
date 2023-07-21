/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Cutscene/Cutscenes.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Code for the cutscenes viewer screen
 *
 * $Log$
 * Revision 1.10  2005/03/29 02:18:47  taylor
 * Various 64-bit platform fixes
 * Fix compiler errors with MAKE_FS1 and fix gr_set_bitmap() too
 * Make sure that turrets can fire at asteroids for FS1 (needed for a couple missions)
 * Streaming audio support (big thanks to Pierre Willenbrock!!)
 * Removed dependance on strings.tbl for FS1 since we don't actually need it now
 *
 * Revision 1.9  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.8  2003/06/11 18:30:32  taylor
 * plug memory leaks
 *
 * Revision 1.7  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.6  2002/07/24 00:20:41  relnev
 * nothing interesting
 *
 * Revision 1.5  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.4  2002/06/02 04:26:34  relnev
 * warning cleanup
 *
 * Revision 1.3  2002/05/26 22:06:17  relnev
 * makefile: disable stand_gui for now.
 *
 * rest: staticize some globals
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 15    6/16/00 3:15p Jefff
 * sim of the year dvd version changes, a few german soty localization
 * fixes
 * 
 * 14    10/13/99 10:20a Jefff
 * fixed bug where intro cutscene showed 2x after getting bad ending
 * 
 * 13    10/06/99 10:30a Jefff
 * OEM updates
 * 
 * 12    9/30/99 6:01p Jefff
 * OEM updates
 * 
 * 11    9/07/99 1:10p Mikek
 * Make movie check not hang in ~RELEASE_REAL build.
 * 
 * 10    9/06/99 6:38p Dave
 * Improved CD detection code.
 * 
 * 9     9/06/99 1:16a Dave
 * Make sure the user sees the intro movie.
 * 
 * 8     9/03/99 1:31a Dave
 * CD checking by act. Added support to play 2 cutscenes in a row
 * seamlessly. Fixed super low level cfile bug related to files in the
 * root directory of a CD. Added cheat code to set campaign mission # in
 * main hall.
 * 
 * 7     7/19/99 2:13p Dave
 * Added some new strings for Heiko.
 * 
 * 6     6/01/99 3:52p Dave
 * View footage screen. Fixed xstrings to not display the & symbol. Popup,
 * dead popup, pxo find player popup, pxo private room popup.
 * 
 * 5     1/30/99 5:08p Dave
 * More new hi-res stuff.Support for nice D3D textures.
 * 
 * 4     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
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
 * 18    6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 17    6/01/98 11:43a John
 * JAS & MK:  Classified all strings for localization.
 * 
 * 16    5/24/98 9:01p Lawrance
 * Add commit sounds when accept is pressed
 * 
 * 15    5/21/98 8:05p Allender
 * fix possible bug with number of cutscenes shown in the list
 * 
 * 14    5/21/98 12:35a Lawrance
 * Tweak how CD is checked for
 * 
 * 13    5/20/98 1:34p Hoffoss
 * Added cutscene description rendering.
 * 
 * 12    5/19/98 12:19p Mike
 * Cheat codes!
 * 
 * 11    5/12/98 4:17p Hoffoss
 * Make ctrl-arrows (up/down) switch between tech room screens.
 * 
 * 10    5/11/98 8:04p Hoffoss
 * Fixed minor bugs.
 * 
 * 9     5/10/98 10:05p Allender
 * only show cutscenes which have been seen before.  Made Fred able to
 * write missions anywhere, defaulting to player misison folder, not data
 * mission folder.  Fix FreeSpace code to properly read missions from
 * correct locations
 * 
 * 8     5/08/98 5:30p Lawrance
 * add CD checks for movie playing
 * 
 * 7     5/08/98 4:07p Allender
 * more cutscene stuff
 * 
 * 6     5/07/98 2:33p Hoffoss
 * Removed help and options buttons.
 * 
 * 5     4/30/98 4:53p John
 * Restructured and cleaned up cfile code.  Added capability to read off
 * of CD-ROM drive and out of multiple pack files.
 * 
 * 4     4/23/98 8:27p Allender
 * basic support for cutscene playback.  Into movie code in place.  Tech
 * room can view cutscenes stored in CDROM_dir variable
 * 
 * 3     4/21/98 7:07p Hoffoss
 * Fixed problem where when switching screens flashes old tab hilight once
 * before switching to new state.
 * 
 * 2     4/17/98 6:33p Hoffoss
 * Made changes to the tech room group of screens.  Cutscenes screen is
 * now in a new file.
 *
 * $NoKeywords: $
 */

#include "cutscenes.h"
#include "ui.h"
#include "cfile.h"
#include "gamesnd.h"
#include "gamesequence.h"
#include "freespace.h"
#include "key.h"
#include "bmpman.h"
#include "movie.h"
#include "popup.h"
#include "mainhallmenu.h"
#include "alphacolors.h"
#include "localize.h"

const char *Cutscene_bitmap_name[GR_NUM_RESOLUTIONS] = {
	"ViewFootage",
	"2_ViewFootage"
};
const char *Cutscene_mask_name[GR_NUM_RESOLUTIONS] = {
	"ViewFootage-m",
	"2_ViewFootage-m"
};

int Num_cutscenes;
int Cutscenes_viewable;
int Description_index;
cutscene_info Cutscenes[MAX_CUTSCENES];

extern int All_movies_enabled;		//	If set, all movies may be viewed.  Keyed off cheat code.

// initialization stuff for cutscenes
void cutscene_init()
{
#ifndef FS1_DEMO  // no cuscenes in FS1 demo
	char buf[MULTITEXT_LENGTH];
	int rval;

	if ((rval = setjmp(parse_abort)) != 0) {
		Error(LOCATION, "Error parsing 'rank.tbl'\r\nError code = %i.\r\n", rval);
	} 

	// open localization
	lcl_ext_open();

	read_file_text("cutscenes.tbl");
	reset_parse();

	// parse in all the rank names
	Num_cutscenes = 0;
	skip_to_string("#Cutscenes");
	ignore_white_space();
	while ( required_string_either("#End", "$Filename:") ) {
		Assert ( Num_cutscenes < MAX_CUTSCENES );
		required_string("$Filename:");
		stuff_string( Cutscenes[Num_cutscenes].filename, F_PATHNAME, NULL );
		required_string("$Name:");
		stuff_string( Cutscenes[Num_cutscenes].name, F_NAME, NULL );
		required_string("$Description:");
		stuff_string(buf, F_MULTITEXT, NULL);
		drop_white_space(buf);
		compact_multitext_string(buf);
		Cutscenes[Num_cutscenes].description = strdup(buf);
		required_string("$cd:");
		stuff_int( &Cutscenes[Num_cutscenes].cd );

		Num_cutscenes++;
	}

	required_string("#End");

	Cutscenes_viewable = INTRO_CUTSCENE_FLAG;

	// close localization
	lcl_ext_close();
#endif  // FS1_DEMO
}

// free up memory from table parsing
void cutscene_tbl_close()
{
	int i;
	
	for (i=0; i<MAX_CUTSCENES; i++) {
		if (Cutscenes[i].description) {
			free(Cutscenes[i].description);
			Cutscenes[i].description = NULL;
		}
	}
}

// function to return 0 based index of which CD a particular movie is on
// returns -1 on failure.
int cutscenes_get_cd_num( const char *filename )
{
#if defined(OEM_BUILD)
	return 0;				// only 1 cd for OEM
#else
	int i;

	for (i = 0; i < Num_cutscenes; i++ ) {
		if ( !stricmp(Cutscenes[i].filename, filename) ) {
			return (Cutscenes[i].cd - 1);
		}
	}

	return -1;
#endif // defined(OEM_BUILD)
}

// marks a cutscene as viewable
void cutscene_mark_viewable(const char *filename)
{
	int i;

	for (i = 0; i < Num_cutscenes; i++ ) {
		if ( !stricmp(Cutscenes[i].filename, filename) ) {
			Cutscenes_viewable |= (1<<i);
			return;
		}
	}
}

#define NUM_BUTTONS				8

#define TECH_DATABASE_BUTTON	0
#define SIMULATOR_BUTTON		1
#define CUTSCENES_BUTTON		2
#define CREDITS_BUTTON			3

#define SCROLL_UP_BUTTON		4
#define SCROLL_DOWN_BUTTON		5
#define PLAY_BUTTON				6
#define EXIT_BUTTON				7

static int Num_files;
static int Cutscene_list[MAX_CUTSCENES];
static int Selected_line = 0;  // line that is currently selected for binding
static int Scroll_offset;
static int Background_bitmap;
static UI_BUTTON List_region;
static UI_WINDOW Ui_window;

static ui_button_info Buttons[GR_NUM_RESOLUTIONS][NUM_BUTTONS] = {
	{ // GR_640
#ifdef MAKE_FS1
		ui_button_info("TDB_00",	0,		0,		-1,	-1,	0),		// technical database tab
		ui_button_info("TDB_01",	0,		19,		-1,	-1,	1),		// mission simulator tab
		ui_button_info("TDB_02",	0,		35,		-1,	-1,	2),		// cutscenes tab
		ui_button_info("TDB_03",	0,		56,		-1,	-1,	3),		// credits tab

		ui_button_info("VFB_04",	570,	304,	-1,	-1,	4),		// scroll up
		ui_button_info("VFB_05",	600,	304,	-1,	-1,	5),		// scroll down
		ui_button_info("VFB_06",	573,	347,	-1,	-1,	6),		// play
		ui_button_info("VFB_07",	566,	411,	-1,	-1,	7),		// exit
#else
		ui_button_info("TDB_00",	7,		5,		37,	7,		0),			// tech database 1
		ui_button_info("TDB_01",	7,		19,	37,	23,	1),			// tech database 2
		ui_button_info("TDB_02",	7,		34,	37,	38,	2),			// tech database 3
		ui_button_info("TDB_03",	7,		49,	37,	54,	3),			// tech database 4

		ui_button_info("VFB_04",	6,		318,	-1,	-1,	4),			// scroll up
		ui_button_info("VFB_05",	36,	318,	-1,	-1,	5),			// scroll down
		ui_button_info("VFB_06",	578,	319,	587,	366,	6),			// play
		ui_button_info("VFB_07",	574,	431,	587,	413,	7),			// exit
#endif
	},
	{ // GR_1024
		ui_button_info("2_TDB_00",	12,	7,		59,	12,	0),			// tech database 1
		ui_button_info("2_TDB_01",	12,	31,	59,	37,	1),			// tech database 2
		ui_button_info("2_TDB_02",	12,	56,	59,	62,	2),			// tech database 3
		ui_button_info("2_TDB_03",	12,	81,	59,	88,	3),			// tech database 4

		ui_button_info("2_VFB_04",	9,		509,	-1,	-1,	4),			// scroll up
		ui_button_info("2_VFB_05",	58,	509,	-1,	-1,	5),			// scroll down
		ui_button_info("2_VFB_06",	925,	511,	940,	586,	6),			// play
		ui_button_info("2_VFB_07",	918,	689,	940,	661,	7),			// exit
	}
};

// text
#ifndef MAKE_FS1
#define NUM_CUTSCENE_TEXT			6

UI_XSTR Cutscene_text[GR_NUM_RESOLUTIONS][NUM_CUTSCENE_TEXT] = {
	{ // GR_640
		{"Technical Database",		1055,		37,	7,		UI_XSTR_COLOR_GREEN, -1, &Buttons[0][TECH_DATABASE_BUTTON].button },
		{"Mission Simulator",		1056,		37,	23,	UI_XSTR_COLOR_GREEN, -1, &Buttons[0][SIMULATOR_BUTTON].button },
		{"Cutscenes",					1057,		37,	38,	UI_XSTR_COLOR_GREEN, -1, &Buttons[0][CUTSCENES_BUTTON].button },
		{"Credits",						1058,		37,	54,	UI_XSTR_COLOR_GREEN, -1, &Buttons[0][CREDITS_BUTTON].button },
		
		{"Play",							1335,		587,	366,	UI_XSTR_COLOR_GREEN, -1, &Buttons[0][PLAY_BUTTON].button },
		{"Exit",							1419,		587,	413,	UI_XSTR_COLOR_PINK, -1, &Buttons[0][EXIT_BUTTON].button },			
	},
	{ // GR_1024
		{"Technical Database",		1055,		59,	12,	UI_XSTR_COLOR_GREEN, -1, &Buttons[1][TECH_DATABASE_BUTTON].button },
		{"Mission Simulator",		1056,		59,	37,	UI_XSTR_COLOR_GREEN, -1, &Buttons[1][SIMULATOR_BUTTON].button },
		{"Cutscenes",					1057,		59,	62,	UI_XSTR_COLOR_GREEN, -1, &Buttons[1][CUTSCENES_BUTTON].button },
		{"Credits",						1058,		59,	88,	UI_XSTR_COLOR_GREEN, -1, &Buttons[1][CREDITS_BUTTON].button },
		
		{"Play",							1335,		940,	586,	UI_XSTR_COLOR_GREEN, -1, &Buttons[1][PLAY_BUTTON].button },
		{"Exit",							1419,		940,	661,	UI_XSTR_COLOR_PINK, -1, &Buttons[1][EXIT_BUTTON].button },			
	}
};
#endif

int Cutscene_list_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		25, 119, 542, 193
#else
		9,	117,	621,	198
#endif
	},
	{ // GR_1024
		14,	188,	994,	316
	}
};

int Cutscene_desc_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		19, 356, 465, 40
#else
		9,	378, 484, 73
#endif
	},
	{ // GR_1024
		14, 605, 775, 117
	}
};

#define MAX_TEXT_LINES		20
int Cutscene_max_text_lines[GR_NUM_RESOLUTIONS] = {
	10,
	MAX_TEXT_LINES
};
#define MAX_TEXT_LINE_LEN	256

static int Text_size;
static int Text_offset = 0;
static int Text_line_size[MAX_TEXT_LINES];
static char *Text_lines[MAX_TEXT_LINES];


int cutscenes_validate_cd(const char *mve_name, int prompt_for_cd)
{
	int cd_present = 0;
	int cd_drive_num;
	int cd_mve_is_on;
	char volume_name[128];

#ifdef RELEASE_REAL
	int num_attempts = 0;
#endif

	while(1) {
		int path_set_ok;

		cd_mve_is_on = cutscenes_get_cd_num(mve_name);
		if ((cd_mve_is_on != 0) && (cd_mve_is_on != 1) && (cd_mve_is_on != 2)) {
			cd_present = 0;
			break;
		}

#if defined(OEM_BUILD)
		sprintf(volume_name, NOX("FS2_OEM"));
#else
		sprintf(volume_name, NOX("FREESPACE2_%c"), '1' + cd_mve_is_on);
#endif


		cd_drive_num = find_freespace_cd(volume_name);
		path_set_ok = set_cdrom_path(cd_drive_num);

		if ( path_set_ok ) {
			cd_present = 1;
			break;
		}

#ifdef RELEASE_REAL
		if ( !prompt_for_cd ) {
			cd_present = 0;
			break;
		}

		// no CD found, so prompt user
		char popup_msg[256];
		int popup_rval;

#if defined(DVD_MESSAGE_HACK)
		sprintf(popup_msg, XSTR( "Movie not found\n\nInsert FreeSpace DVD to continue", 203));
#else 
		sprintf(popup_msg, XSTR( "Movie not found\n\nInsert FreeSpace CD #%d to continue", 203), cd_mve_is_on+1);
#endif

		popup_rval = popup(PF_BODY_BIG, 2, POPUP_CANCEL, POPUP_OK, popup_msg);
		if ( popup_rval != 1 ) {
			cd_present = 0;
			break;
		}

		if ( num_attempts++ > 5 ) {
			cd_present = 0;
			break;
		}
#else
		cd_present = 0;
		break;
#endif

	}

	return cd_present;
}

void cutscenes_screen_play()
{
	char name[MAX_FILENAME_LEN], *full_name;
	int which_cutscene;

	Assert( (Selected_line >= 0) && (Selected_line < Num_files) );
	which_cutscene = Cutscene_list[Selected_line];

	strcpy(name, Cutscenes[which_cutscene].filename );
	full_name = cf_add_ext(name, NOX(".mve"));

	int rval = movie_play(full_name);
	if ( !rval ) {
		char str[256];

		sprintf(str, XSTR( "Unable to play movie %s.", 204), Cutscenes[which_cutscene].name );
		popup(0, 1, POPUP_OK, str );
	}
}

void cutscenes_screen_scroll_line_up()
{
	if (Selected_line) {
		Selected_line--;
		gamesnd_play_iface(SND_SCROLL);

	} else
		gamesnd_play_iface(SND_GENERAL_FAIL);
	
	if (Selected_line < Scroll_offset)
		Scroll_offset = Selected_line;
}

void cutscenes_screen_scroll_line_down()
{
	int h;

	if (Selected_line < Num_files - 1) {
		Selected_line++;
		gamesnd_play_iface(SND_SCROLL);

	} else
		gamesnd_play_iface(SND_GENERAL_FAIL);
	
	h = Cutscene_list_coords[gr_screen.res][3] / gr_get_font_height();
	if (Selected_line >= Scroll_offset + h){
		Scroll_offset++;
	}
}

void cutscenes_screen_scroll_screen_up()
{
	int h;

	if (Scroll_offset) {
		Scroll_offset--;
		Assert(Selected_line > Scroll_offset);
		h = Cutscene_list_coords[gr_screen.res][3] / gr_get_font_height();
		while (Selected_line >= Scroll_offset + h){
			Selected_line--;
		}

		gamesnd_play_iface(SND_SCROLL);

	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

void cutscenes_screen_scroll_screen_down()
{
	int h;

	h = Cutscene_list_coords[gr_screen.res][3] / gr_get_font_height();
	if (Scroll_offset + h < Num_files) {
		Scroll_offset++;
		if (Selected_line < Scroll_offset){
			Selected_line = Scroll_offset;
		}

		gamesnd_play_iface(SND_SCROLL);
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

int cutscenes_screen_button_pressed(int n)
{
	switch (n) {
		case TECH_DATABASE_BUTTON:
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_TECH_MENU);
			return 1;

		case SIMULATOR_BUTTON:
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_SIMULATOR_ROOM);
			return 1;

		case CREDITS_BUTTON:
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_CREDITS);
			return 1;

		case SCROLL_UP_BUTTON:
			cutscenes_screen_scroll_screen_up();
			break;

		case SCROLL_DOWN_BUTTON:
			cutscenes_screen_scroll_screen_down();
			break;

		case PLAY_BUTTON:
			cutscenes_screen_play();
			break;

		case EXIT_BUTTON:
			gamesnd_play_iface(SND_COMMIT_PRESSED);
			gameseq_post_event(GS_EVENT_MAIN_MENU);
			game_flush();
			break;
	}

	return 0;
}

void cutscenes_screen_init()
{
	int i;
	ui_button_info *b;

	Ui_window.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);
	Ui_window.set_mask_bmap(Cutscene_mask_name[gr_screen.res]);

	for (i=0; i<NUM_BUTTONS; i++) {
		b = &Buttons[gr_screen.res][i];

		b->button.create(&Ui_window, "", b->x, b->y, 60, 30, (i < 2), 1);
		// set up callback for when a mouse first goes over a button
		b->button.set_highlight_action(common_play_highlight_sound);
		b->button.set_bmaps(b->filename);
		b->button.link_hotspot(b->hotspot);
	}

#ifndef MAKE_FS1
	// add xstrs
	for(i=0; i<NUM_CUTSCENE_TEXT; i++){
		Ui_window.add_XSTR(&Cutscene_text[gr_screen.res][i]);
	}
#endif

	Buttons[gr_screen.res][EXIT_BUTTON].button.set_hotkey(KEY_CTRLED | KEY_ENTER);
	Buttons[gr_screen.res][SCROLL_UP_BUTTON].button.set_hotkey(KEY_PAGEUP);
	Buttons[gr_screen.res][SCROLL_DOWN_BUTTON].button.set_hotkey(KEY_PAGEDOWN);	

	List_region.create(&Ui_window, "", Cutscene_list_coords[gr_screen.res][0], Cutscene_list_coords[gr_screen.res][1], Cutscene_list_coords[gr_screen.res][2], Cutscene_list_coords[gr_screen.res][3], 0, 1);
	List_region.hide();

	// set up hotkeys for buttons so we draw the correct animation frame when a key is pressed
	Buttons[gr_screen.res][SCROLL_UP_BUTTON].button.set_hotkey(KEY_PAGEUP);
	Buttons[gr_screen.res][SCROLL_DOWN_BUTTON].button.set_hotkey(KEY_PAGEDOWN);

	Background_bitmap = bm_load(Cutscene_bitmap_name[gr_screen.res]);
	Scroll_offset = Selected_line = 0;
	Description_index = -1;

	// when doing a debug version, just put all of the movie files here.
#ifndef NDEBUG
	//Cutscenes_viewable = 0xffffffff;			// makes all cutscenes viewble.
#endif

	if (All_movies_enabled)
		Cutscenes_viewable = 0xffffffff;		//	Cheat code enables all movies.

	Num_files = 0;
	for ( i = 0; i < Num_cutscenes; i++ ) {
		if ( Cutscenes_viewable & (1<<i) ) {
			Cutscene_list[Num_files] = i;
			Num_files++;
		}
	}
}

void cutscenes_screen_close()
{
	if (Background_bitmap)
		bm_unload(Background_bitmap);

	Ui_window.destroy();
}

void cutscenes_screen_do_frame()
{
	int i, k, y, z;
	int font_height = gr_get_font_height();
	int select_tease_line = -1;

	k = Ui_window.process();
	switch (k) {
		case KEY_DOWN:  // select next line
			cutscenes_screen_scroll_line_down();
			break;

		case KEY_UP:  // select previous line
			cutscenes_screen_scroll_line_up();
			break;

		case KEY_TAB:
		case KEY_CTRLED | KEY_DOWN:
			cutscenes_screen_button_pressed(CREDITS_BUTTON);
			break;

		case KEY_SHIFTED | KEY_TAB:
		case KEY_CTRLED | KEY_UP:
			cutscenes_screen_button_pressed(SIMULATOR_BUTTON);
			break;

		case KEY_ENTER:
			cutscenes_screen_play();
			break;

		case KEY_ESC:  // cancel
			gameseq_post_event(GS_EVENT_MAIN_MENU);
			game_flush();
			break;

		case KEY_F1:  // show help overlay
			break;

		case KEY_F2:  // goto options screen
			gameseq_post_event(GS_EVENT_OPTIONS_MENU);
			break;
	}	// end switch

	for (i=0; i<NUM_BUTTONS; i++){
		if (Buttons[gr_screen.res][i].button.pressed()){
			if (cutscenes_screen_button_pressed(i)){
				return;
			}
		}
	}

	if (List_region.button_down()) {
		List_region.get_mouse_pos(NULL, &y);
		z = Scroll_offset + y / font_height;
		if ((z >= 0) && (z < Num_files))
			select_tease_line = z;
	}
	
	if (List_region.pressed()) {
		List_region.get_mouse_pos(NULL, &y);
		z = Scroll_offset + y / font_height;
		if ((z >= 0) && (z < Num_files))
			Selected_line = z;
	}

	GR_MAYBE_CLEAR_RES(Background_bitmap);
	if (Background_bitmap >= 0) {
		gr_set_bitmap(Background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	} 

	Ui_window.draw();

	for (i=TECH_DATABASE_BUTTON; i<=CREDITS_BUTTON; i++){
		if (Buttons[gr_screen.res][i].button.button_down()){
			break;
		}
	}

	if (i > CREDITS_BUTTON){
		Buttons[gr_screen.res][CUTSCENES_BUTTON].button.draw_forced(2);
	}

	y = 0;
	z = Scroll_offset;
	while (y + font_height <= Cutscene_list_coords[gr_screen.res][3]) {
		if (z >= Num_files){
			break;
		}

		if (z == Selected_line){
			gr_set_color_fast(&Color_text_selected);
		} else if (z == select_tease_line) {
			gr_set_color_fast(&Color_text_subselected);
		} else {
			gr_set_color_fast(&Color_text_normal);
		}

		gr_printf(Cutscene_list_coords[gr_screen.res][0], Cutscene_list_coords[gr_screen.res][1] + y, Cutscenes[Cutscene_list[z]].name);

		y += font_height;
		z++;
	}

	if (Description_index != Selected_line) {
		char *src;

		Description_index = Selected_line;
		Text_size = 0;
		src = Cutscenes[Cutscene_list[Description_index]].description;
		if (src) {
			Text_size = split_str(src, Cutscene_desc_coords[gr_screen.res][2], Text_line_size, Text_lines, Cutscene_max_text_lines[gr_screen.res]);
			Assert(Text_size >= 0 && Text_size < Cutscene_max_text_lines[gr_screen.res]);
		}
	}

	if (Description_index >= 0) {
		int len;
		char line[MAX_TEXT_LINE_LEN + 1];

		gr_set_color_fast(&Color_text_normal);

		y = 0;
		z = Text_offset;
		while (y + font_height <= Cutscene_desc_coords[gr_screen.res][3]) {
			if (z >= Text_size)
				break;

			len = Text_line_size[z];
			if (len > MAX_TEXT_LINE_LEN)
				len = MAX_TEXT_LINE_LEN;

			strncpy(line, Text_lines[z], len);
			line[len] = 0;
			gr_string(Cutscene_desc_coords[gr_screen.res][0], Cutscene_desc_coords[gr_screen.res][1] + y, line);

			y += font_height;
			z++;
		}
	}

	gr_flip();
}

