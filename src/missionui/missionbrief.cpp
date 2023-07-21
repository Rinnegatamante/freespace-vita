/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MissionUI/MissionBrief.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module that contains code to display the mission briefing to the player
 *
 * $Log$
 * Revision 1.13  2005/10/03 11:25:42  taylor
 * be sure to properly clean out old briefing entries when compacting
 *
 * Revision 1.12  2005/10/01 22:04:58  taylor
 * fix FS1 (de)briefing voices, the directory names are different in FS1
 * hard code the table values so that the fs1.vp file isn't needed
 * hard code a mission fix for sm2-08a since a have no idea how to fix it otherwise
 * generally cleanup some FS1 code
 * fix volume sliders in the options screen that never went all the way up
 *
 * Revision 1.11  2005/08/12 08:58:41  taylor
 * fix the strange mouse click issues on briefing, ship select and weapon select screens
 *
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
 * Revision 1.8  2004/07/04 11:31:43  taylor
 * amd64 support, compiler warning fixes, don't use software rendering
 *
 * Revision 1.7  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.6  2003/01/30 19:55:33  relnev
 * remove second bmpman.h include (Taylor Richards)
 *
 * Revision 1.5  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.4  2002/06/09 04:41:22  relnev
 * added copyright header
 *
 * Revision 1.3  2002/06/02 04:26:34  relnev
 * warning cleanup
 *
 * Revision 1.2  2002/05/07 03:16:46  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 48    10/14/99 2:51p Jefff
 * localiztion fixes
 * 
 * 47    10/13/99 3:23p Jefff
 * fixed unnumbered XSTRs
 * 
 * 46    9/12/99 8:09p Dave
 * Fixed problem where skip-training button would cause mission messages
 * not to get paged out for the current mission.
 * 
 * 45    9/09/99 3:40p Jefff
 * no exit loop outisde of campaign mode
 * 
 * 44    9/07/99 6:53p Jefff
 * functionality to break out of a loop
 * 
 * 43    9/06/99 6:38p Dave
 * Improved CD detection code.
 * 
 * 42    9/03/99 10:57a Jefff
 * numbered an xstr
 * 
 * 41    8/30/99 5:30p Jefff
 * fixed "more" being cut off in d3d
 * 
 * 40    8/30/99 10:29a Jefff
 * added autocenter flag to closup rendering
 * 
 * 39    8/19/99 11:33a Dave
 * Fixed debug build.
 * 
 * 38    8/19/99 10:59a Dave
 * Packet loss detection.
 * 
 * 37    8/16/99 4:05p Dave
 * Big honking checkin.
 * 
 * 36    8/16/99 9:48a Jefff
 * all clicks on briefing icons should now register
 * 
 * 35    8/10/99 7:29p Jefff
 * added mission title to briefing screen
 * 
 * 34    8/03/99 1:34a Andsager
 * fix skip training mission (again)
 * 
 * 33    8/02/99 12:01p Jefff
 * fixed "skip training" text pos
 * 
 * 32    7/30/99 5:42p Jasenw
 * Fixed coords for skip training button.
 * 
 * 31    7/29/99 10:50p Dave
 * Oops. Accidentally blew away some interface text changes.
 * 
 * 30    7/29/99 10:48p Dave
 * 
 * 29    7/29/99 3:05p Andsager
 * Add skip training text. and properly skip.
 * 
 * 28    7/24/99 6:07p Jefff
 * Added "lock" text to multiplayer lock button
 * 
 * 27    7/21/99 10:51a Jefff
 * added "more" indicator to briefing text box
 * 
 * 26    7/19/99 3:01p Dave
 * Fixed icons. Added single transport icon.
 * 
 * 25    7/18/99 5:20p Dave
 * Jump node icon. Fixed debris fogging. Framerate warning stuff.
 * 
 * 24    7/16/99 1:49p Dave
 * 8 bit aabitmaps. yay.
 * 
 * 23    7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 22    7/09/99 5:54p Dave
 * Seperated cruiser types into individual types. Added tons of new
 * briefing icons. Campaign screen.
 * 
 * 21    6/29/99 7:39p Dave
 * Lots of small bug fixes.
 * 
 * 20    6/11/99 11:13a Dave
 * last minute changes before press tour build.
 * 
 * 19    6/04/99 1:18p Dave
 * Fixed briefing model rendering problems. Made show background option in
 * fred toggle nebula rendering.
 * 
 * 18    5/07/99 10:34a Andsager
 * Make red alert work in FS2
 * 
 * 17    4/23/99 12:01p Johnson
 * Added SIF_HUGE_SHIP
 * 
 * 16    3/25/99 2:45p Neilk
 * Fixed lock button
 * 
 * 15    2/11/99 3:08p Dave
 * PXO refresh button. Very preliminary squad war support.
 * 
 * 14    2/05/99 7:19p Neilk
 * Removed black part from mission screen, fixed info text coords
 * 
 * 13    2/02/99 4:35p Neilk
 * fixed coordinate problem where primary goals was on top of interface in
 * mission briefing
 * 
 * 12    2/01/99 5:55p Dave
 * Removed the idea of explicit bitmaps for buttons. Fixed text
 * highlighting for disabled gadgets.
 * 
 * 11    1/30/99 7:32p Neilk
 * Fixed coords problems for mission briefing screens
 * 
 * 10    1/30/99 5:08p Dave
 * More new hi-res stuff.Support for nice D3D textures.
 * 
 * 9     1/30/99 1:29a Dave
 * Fixed nebula thumbnail problem. Full support for 1024x768 choose pilot
 * screen.  Fixed beam weapon death messages.
 * 
 * 8     1/29/99 4:17p Dave
 * New interface screens.
 * 
 * 7     1/13/99 7:19p Neilk
 * Converted Mission Brief, Barracks, Synch to high res support
 * 
 * 6     12/18/98 1:13a Dave
 * Rough 1024x768 support for Direct3D. Proper detection and usage through
 * the launcher.
 * 
 * 5     11/30/98 1:07p Dave
 * 16 bit conversion, first run.
 * 
 * 4     11/20/98 4:08p Dave
 * Fixed flak effect in multiplayer.
 * 
 * 3     10/13/98 9:28a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 114   9/17/98 3:08p Dave
 * PXO to non-pxo game warning popup. Player icon stuff in create and join
 * game screens. Upped server count refresh time in PXO to 35 secs (from
 * 20).
 * 
 * 113   6/19/98 3:52p Lawrance
 * Don't use large text in popup, since it is missing large foreign chars
 * 
 * 112   6/09/98 5:17p Lawrance
 * French/German localization
 * 
 * 111   6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 110   6/01/98 11:43a John
 * JAS & MK:  Classified all strings for localization.
 * 
 * 109   5/22/98 10:44a Lawrance
 * put in case label to suppress warning
 * 
 * 108   5/19/98 11:35p Lawrance
 * Play sound for briefing control button presses
 * 
 * 107   5/19/98 8:47p Lawrance
 * Set proper distance for jump nodes
 * 
 * 106   5/19/98 8:35p Dave
 * Revamp PXO channel listing system. Send campaign goals/events to
 * clients for evaluation. Made lock button pressable on all screens. 
 * 
 * 105   5/12/98 2:16p Hoffoss
 * Added debug code to switch models of closeup icon to more easily check
 * positions and zooms.
 * 
 * 104   5/06/98 11:50p Lawrance
 * Clean up help overlay code for loadout screens
 * 
 * 103   5/06/98 8:03p Allender
 * AL: only free pointers in brief_compact_stages if non-null
 * 
 * 102   5/05/98 4:48p Lawrance
 * Fix bug with launching closeup icon when pause button is pressed
 * 
 * 101   4/29/98 1:45p Lawrance
 * Print out mission filename if debug build, or if INTERPLAY QA defined
 * 
 * 100   4/29/98 12:15a Lawrance
 * reset demo trailer timer when briefing moves to a new stage
 * 
 * 98    4/25/98 11:55p Lawrance
 * fix bug with the auto-advance button
 * 
 * 97    4/25/98 3:49p Lawrance
 * Save briefing auto-advance pref
 * 
 * 96    4/25/98 12:00p Lawrance
 * disable advance keypress for 1/2 second after autoadvance
 * 
 * 95    4/22/98 7:24p Dave
 * Made sure the "player/ships" locked button for multiplayer appears on
 * all briefing screens.
 * 
 * 94    4/20/98 3:53p Lawrance
 * Fix various bugs with auto-advancing through briefings.
 * 
 * 93    4/19/98 12:11p Lawrance
 * add new art for pause button in the briefing
 * 
 * 92    4/17/98 10:39p Andsager
 * AL: Fix bug with auto-advance of briefing stages.
 * 
 * 91    4/16/98 8:05p Lawrance
 * Don't show closeup icon for planets
 * 
 * 90    4/15/98 5:17p Lawrance
 * fix bug with skip training 
 *
 * $NoKeywords: $
 *
*/

#include "freespace.h"
#include "missionload.h"
#include "missionscreencommon.h"
#include "missionshipchoice.h"
#include "missionparse.h"
#include "missiongoals.h"
#include "gamesequence.h"
#include "ship.h"
#include "key.h"
#include "2d.h"
#include "line.h"
#include "3d.h"
#include "model.h"
#include "timer.h"
#include "math.h"
#include "linklist.h"
#include "mouse.h"
#include "hud.h"
#include "ui.h"
#include "osapi.h"
#include "audiostr.h"
#include "gamesnd.h"
#include "eventmusic.h"
#include "missioncampaign.h"
#include "object.h"
#include "multi.h"
#include "snazzyui.h"
#include "bmpman.h"
#include "missionbrief.h"
#include "missionbriefcommon.h"
#include "missiongrid.h"
#include "multimsgs.h"
#include "cmdline.h"
#include "contexthelp.h"
#include "chatbox.h"
#include "multiteamselect.h"
#include "multiui.h"
#include "asteroid.h"
#include "popup.h"
#include "sexp.h"
#include "alphacolors.h"
#include "font.h"
#include "missionmessage.h"

/*
#define OBJECTIVES_X	65
#define OBJECTIVES_Y	137
#define OBJECTIVES_W	508
#define OBJECTIVES_H	233
*/

static int Brief_goals_coords[GR_NUM_RESOLUTIONS][4] = {
	{
#ifdef MAKE_FS1
		68, 138, 504, 208
#else
		65,152,508,211		// GR_640
#endif
	},
	{
		104,243,813,332		// GR_1024
	}
};

static int	Current_brief_stage;	// what stage of the briefing we're on
static int	Last_brief_stage;
static int	Num_brief_stages;
static int	Brief_multiplayer = FALSE;

static int	Brief_last_auto_advance = 0;	// timestamp of last auto-advance

// for managing the scene cut transition
static int	Quick_transition_stage;
static int	Start_fade_up_anim, Start_fade_down_anim;
static int	Brief_playing_fade_sound;
hud_anim		Fade_anim;

int	Briefing_music_handle = -1;
int	Briefing_music_begin_timestamp = 0;

// --------------------------------------------------------------------------------------
// Module scope globals
// --------------------------------------------------------------------------------------

static MENU_REGION	Briefing_select_region[NUM_BREIFING_REGIONS];
static int				Num_briefing_regions;

// For closeup display 
#define					ONE_REV_TIME		6		// time (sec) for one revolution
#define					MAX_ANG_CHG			0.15f

static int Closeup_coords[GR_NUM_RESOLUTIONS][4] = {
	{
		203, 151, 200, 213	// GR_640
	},
	{
		325, 241, 200, 213	// GR_1024
	}
};

static int Brief_infobox_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		0, 397
#else
		0, 391
#endif
	},
	{ // GR_1024
		0, 627
	}
};

static const char *Brief_infobox_filename[GR_NUM_RESOLUTIONS] = {
	"InfoBox",
	"2_Infobox"
};

static const char *Brief_filename[GR_NUM_RESOLUTIONS] = {
	"Brief",
	"2_Brief"
};

static const char *Brief_multi_filename[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	"Brief",	// use the standard bitmap and slap the chatbox on later
#else
	"BriefMulti",
#endif
	"2_BriefMulti"
};

static const char *Brief_mask_filename[GR_NUM_RESOLUTIONS] = {
	"Brief-m",
	"2_Brief-m"
};

static const char *Brief_multi_mask_filename[GR_NUM_RESOLUTIONS] = {
	"BriefMulti-m",
	"2_BriefMulti-m"
};


static const char *Brief_win_filename[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	"mapwin",
	"mapwin"
#else
	"Briefwin",
	"2_Briefwin"
#endif
};

// coordinate inidices
#define BRIEF_X_COORD 0
#define BRIEF_Y_COORD 1
#define BRIEF_W_COORD 2
#define BRIEF_H_COORD 3

//static int Closeup_region[4] = {220,132,420,269};
int Closeup_region[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
		211, 158, 215, 157
	}, 
	{ // GR_1024
		337, 253, 345, 252
	}, 
};

const char *Closeup_background_filename[GR_NUM_RESOLUTIONS] = {
	NOX("BriefPop"),	// GR_640
	NOX("2_BriefPop")	// GR_1024
};

const char *Closeup_button_filename[GR_NUM_RESOLUTIONS] = {
	NOX("BPB_00"),		// GR_640
	NOX("2_BPB_00"),		// GR_1024
};

int Closeup_button_hotspot = 14;

//static int			Closeup_button_coords[2] = {CLOSEUP_X+164,CLOSEUP_Y+227};
int Closeup_button_coords[GR_NUM_RESOLUTIONS][2] = {	
#ifdef MAKE_FS1
	{ 369, 306 },
#else
	{ 374, 316 },		// GR_640	
#endif
	{ 599, 506 }		// GR_1024	
};

UI_BUTTON	Closeup_close_button;
int Closeup_bitmap=-1;
int Closeup_one_revolution_time=ONE_REV_TIME;

brief_icon *Closeup_icon;
angles Closeup_angles;
matrix Closeup_orient;
vector Closeup_pos;
int Closeup_font_height;
int Closeup_x1, Closeup_y1;

// used for the 3d view of a closeup ship
float Closeup_zoom;
vector Closeup_cam_pos;

// Mask bitmap pointer and Mask bitmap_id
bitmap* BriefingMaskPtr;		// bitmap pointer to the briefing select mask bitmap
ubyte* BriefingMaskData;		// pointer to actual bitmap data
int Briefing_mask_w, Briefing_mask_h;
int BriefingMaskBitmap;	// bitmap id of the briefing mask bitmap
int Brief_inited = FALSE;

// --------------------------------------------------------------------------------------
// Briefing specific UI
// --------------------------------------------------------------------------------------
#define	BRIEF_LAST_STAGE_MASK			7
#define	BRIEF_NEXT_STAGE_MASK			8
#define	BRIEF_PREV_STAGE_MASK			9
#define	BRIEF_FIRST_STAGE_MASK			10
#define	BRIEF_TEXT_SCROLL_UP_MASK		11
#define	BRIEF_TEXT_SCROLL_DOWN_MASK	12
#define	BRIEF_SKIP_TRAINING_MASK		15
#define	BRIEF_PAUSE_MASK					16

//XSTR:OFF
static const char *Brief_mask_single[GR_NUM_RESOLUTIONS] = {
	"brief-m",		// GR_640
	"2_brief-m"		// GR_1024
};

static const char *Brief_mask_multi[GR_NUM_RESOLUTIONS] = {
	"briefmulti-m",		// GR_640
	"2_briefmulti-m"			// GR_1024
};
//XSTR:ON

struct brief_buttons {	
	const char *filename;
	int x, y;
	int xt, yt;
	int hotspot;
	int repeat;
	UI_BUTTON button;  // because we have a class inside this struct, we need the constructor below..

	brief_buttons(const char *name, int x1, int y1, int xt1, int yt1, int h, int r = 0) : filename(name), x(x1), y(y1), xt(xt1), yt(yt1), hotspot(h), repeat(r) {}
};

int	Brief_grid_bitmap = -1;
int	Brief_text_bitmap = -1;

int	Brief_multitext_bitmap = -1;
int	Brief_background_bitmap =-1;

#ifdef MAKE_FS1
static int MapWin01 = -1;
static int MapWin02 = -1;
static int MapWin03 = -1;
static int MapWin04 = -1;
#endif

UI_WINDOW Brief_ui_window;

// Briefing specific buttons
#define NUM_BRIEF_BUTTONS 10

brief_buttons	Brief_buttons[GR_NUM_RESOLUTIONS][NUM_BRIEF_BUTTONS] = {
	{ // GR_640
#ifdef MAKE_FS1
		brief_buttons("BRB_08",		356,	351,	-1,	-1,	8),		// vcr - fastforward
		brief_buttons("BRB_09",		331,	351,	-1,	-1,	9),		// vcr - play forwards
		brief_buttons("BRB_10",		277,	351,	-1,	-1,	10),	// vcr - play backwards
		brief_buttons("BRB_11",		253,	351,	-1,	-1,	11),	// vcr - reverse
		brief_buttons("BRB_12",		0,		400,	-1,	-1,	12),	// scroll up
		brief_buttons("BRB_13",		0,		442,	-1,	-1,	13),	// scroll down
		brief_buttons("BRB_15",		562,	0,		-1,	-1,	15),	// skip training
		brief_buttons("BRB_16",		304,	351,	-1,	-1,	16),	// vcr - pause
		brief_buttons("TSB_34",		601,	344,	-1,	-1,	50),	// multi lock
		brief_buttons("BRB_15",		562,	0,		-1,	-1,	15)		// exit loop
#else
		brief_buttons("BRB_08",		110,	116,	117,	157,	8),
		brief_buttons("BRB_09",		84,	116,	117,	157,	9),
		brief_buttons("BRB_10",		29,	116,	117,	157,	10),
		brief_buttons("BRB_11",		4,		116,	117,	157,	11),
		brief_buttons("BRB_12",		0,		405,	117,	157,	12),
		brief_buttons("BRB_13",		0,		447,	117,	157,	13),			
		brief_buttons("BRB_15",		562,	0,		117,	157,	15),			// skip training
		brief_buttons("BRB_16",		56,	116,	117,	157,	16),
		brief_buttons("TSB_34",		603,	374,	117,	157,	34),	
		brief_buttons("BRB_15",		562,	0,		117,	157,	15)			// exit loop	
#endif
	}, 
	{ // GR_1024
		brief_buttons("2_BRB_08",		175,	187,	117,	157,	8),
		brief_buttons("2_BRB_09",		135,	187,	117,	157,	9),
		brief_buttons("2_BRB_10",		47,	187,	117,	157,	10),
		brief_buttons("2_BRB_11",		8,		187,	117,	157,	11),
		brief_buttons("2_BRB_12",		0,		649,	117,	157,	12),
		brief_buttons("2_BRB_13",		0,		716,	117,	157,	13),			
		brief_buttons("2_BRB_15",		900,	0,		117,	157,	15),		// skip training
		brief_buttons("2_BRB_16",		91,	187,	117,	157,	16),
		brief_buttons("2_TSB_34",		966,	599,	117,	157,	34),			
		brief_buttons("2_BRB_15",		900,	0,		117,	157,	15)			// exit loop	
	}, 	
};

// briefing UI
#ifndef MAKE_FS1
#define BRIEF_SELECT_NUM_TEXT			3

UI_XSTR Brief_select_text[GR_NUM_RESOLUTIONS][BRIEF_SELECT_NUM_TEXT] = {
	{ // GR_640
		{ "Lock",				1270,	602,	364,	UI_XSTR_COLOR_GREEN, -1, &Brief_buttons[0][BRIEF_BUTTON_MULTI_LOCK].button },
		{ "Skip Training",	1442,	467,	7,		UI_XSTR_COLOR_GREEN, -1, &Brief_buttons[0][BRIEF_BUTTON_SKIP_TRAINING].button },
		{ "Exit Loop",			1477,	490,	7,		UI_XSTR_COLOR_GREEN, -1, &Brief_buttons[0][BRIEF_BUTTON_EXIT_LOOP].button }
	}, 
	{ // GR_1024
		{ "Lock",				1270,	964,	584,	UI_XSTR_COLOR_GREEN, -1, &Brief_buttons[1][BRIEF_BUTTON_MULTI_LOCK].button },
		{ "Skip Training",	1442,	805,	12,	UI_XSTR_COLOR_GREEN, -1, &Brief_buttons[1][BRIEF_BUTTON_SKIP_TRAINING].button },
		{ "Exit Loop",			1477,	830,	12,	UI_XSTR_COLOR_GREEN, -1, &Brief_buttons[1][BRIEF_BUTTON_EXIT_LOOP].button }
	}
};
#endif

// coordinates for briefing title -- the x value is for the RIGHT side of the text
static int Title_coords[GR_NUM_RESOLUTIONS][2] = {
#ifdef MAKE_FS1
	{577, 387},
#else
	{575, 117},		// GR_640
#endif
	{918, 194}		// GR_1024
};

// coordinates for briefing title in multiplayer briefings -- the x value is for the LEFT side of the text
// third coord is max width of area for it to fit into (it is force fit there)
static int Title_coords_multi[GR_NUM_RESOLUTIONS][3] = {
#ifdef MAKE_FS1
	{577, 387, 190},
#else
	{1, 105, 190},		// GR_640
#endif
	{1, 174, 304}		// GR_1024
};

// briefing line widths
int Brief_max_line_width[GR_NUM_RESOLUTIONS] = {
	MAX_BRIEF_LINE_W_640, MAX_BRIEF_LINE_W_1024
};

// --------------------------------------------------------------------------------------
// Forward declarations
// --------------------------------------------------------------------------------------
int brief_setup_closeup(brief_icon *bi);
void brief_maybe_blit_scene_cut(float frametime);
void brief_transition_reset();

const char *brief_tooltip_handler(const char *str)
{
	if (!stricmp(str, NOX("@close"))) {
		if (Closeup_icon)
			return XSTR( "Close", 428);
	}

	return NULL;
}

// brief_skip_training_pressed()
//
// called when the skip training button on the briefing screen is hit.  When this happens,
// do a popup, then move to the next mission in the campaign.
void brief_skip_training_pressed()
{
	int val;

	val = popup(PF_USE_NEGATIVE_ICON | PF_USE_AFFIRMATIVE_ICON,2,POPUP_NO,POPUP_YES,XSTR( "Skip Training\n\n\n\nAre you sure you want to skip this training mission?", 429));

	// val is 0 when we hit no (first on the list)
	// AL: also, -1 is returned when ESC is hit
	if ( val <= 0 ){
		return;
	}

	// page out mission messages
	message_mission_shutdown();

	if ( !(Game_mode & GM_CAMPAIGN_MODE) ){
		gameseq_post_event( GS_EVENT_MAIN_MENU );
	}

	// tricky part.  Need to move to the next mission in the campaign.
	mission_goal_mark_objectives_complete();
	mission_goal_fail_incomplete();
	mission_campaign_store_goals_and_events();

	mission_campaign_eval_next_mission();
	mission_campaign_mission_over();	

	// CD CHECK
	if(game_do_cd_mission_check(Game_current_mission_filename)){
		gameseq_post_event( GS_EVENT_START_GAME );
	} else {
		gameseq_post_event( GS_EVENT_MAIN_MENU );
	}
}

#if defined(FS2_DEMO) || defined(FS1_DEMO)
	extern void demo_reset_trailer_timer();
#endif
// --------------------------------------------------------------------------------------
//	brief_do_next_pressed()
//
//
void brief_do_next_pressed(int play_sound)
{
	int now;
	now = timer_get_milliseconds();

	if ( (now - Brief_last_auto_advance) < 500 ) {
		return;
	}

#if defined(FS2_DEMO) || defined(FS1_DEMO)
	demo_reset_trailer_timer();
#endif

	Current_brief_stage++;
	if ( Current_brief_stage >= Num_brief_stages ) {
		Current_brief_stage = Num_brief_stages - 1;
		gamesnd_play_iface(SND_GENERAL_FAIL);
		if ( Quick_transition_stage != -1 )
			brief_transition_reset();
	} else {
		if ( play_sound ) {
			gamesnd_play_iface(SND_BRIEF_STAGE_CHG);
		}
	}

	Assert(Current_brief_stage >= 0);
}

// --------------------------------------------------------------------------------------
//	brief_do_prev_pressed()
//
//
void brief_do_prev_pressed()
{
	Current_brief_stage--;
	if ( Current_brief_stage < 0 ) {
		Current_brief_stage = 0;
		gamesnd_play_iface(SND_GENERAL_FAIL);
		if ( Quick_transition_stage != -1 )
			brief_transition_reset();
	} else {
		gamesnd_play_iface(SND_BRIEF_STAGE_CHG);
	}
	Assert(Current_brief_stage >= 0);
}


// --------------------------------------------------------------------------------------
//	brief_do_start_pressed()
//
//
void brief_do_start_pressed()
{
	if ( Current_brief_stage != 0 ) {
		gamesnd_play_iface(SND_BRIEF_STAGE_CHG);
		Current_brief_stage = 0;
		if ( Quick_transition_stage != -1 )
			brief_transition_reset();
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
	Assert(Current_brief_stage >= 0);
}

// --------------------------------------------------------------------------------------
//	brief_do_end_pressed()
//
//
void brief_do_end_pressed()
{
	if ( Current_brief_stage != Num_brief_stages - 1 ) {
		gamesnd_play_iface(SND_BRIEF_STAGE_CHG);
		Current_brief_stage = Num_brief_stages - 1;
		if ( Quick_transition_stage != -1 )
			brief_transition_reset();

	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
	Assert(Current_brief_stage >= 0);
}


void brief_scroll_up_text()
{
	Top_brief_text_line--;
	if ( Top_brief_text_line < 0 ) {
		Top_brief_text_line = 0;
		gamesnd_play_iface(SND_GENERAL_FAIL);
	} else {
		gamesnd_play_iface(SND_SCROLL);
	}
}

void brief_scroll_down_text()
{
	Top_brief_text_line++;
	if ( (Num_brief_text_lines[0] - Top_brief_text_line) < Brief_text_max_lines[gr_screen.res]) {
		Top_brief_text_line--;
		gamesnd_play_iface(SND_GENERAL_FAIL);
	} else {
		gamesnd_play_iface(SND_SCROLL);
	}
}


// handles the exit loop option
void brief_exit_loop_pressed()
{
	int val = popup(PF_USE_NEGATIVE_ICON | PF_USE_AFFIRMATIVE_ICON, 2, POPUP_NO, POPUP_YES, XSTR( "Exit Loop\n\n\n\nAre you sure you want to leave the mission loop?", 1489));

	// bail if esc hit or no clicked
	if (val <= 0) {
		return;
	}

	// handle the details
	// this also posts the start game event
	mission_campaign_exit_loop();
}


// -------------------------------------------------------------------------------------
// brief_select_button_do() do the button action for the specified pressed button
//
void brief_button_do(int i)
{
	switch ( i ) {
		case BRIEF_BUTTON_LAST_STAGE:
			brief_do_end_pressed();
			break;

		case BRIEF_BUTTON_NEXT_STAGE:
			brief_do_next_pressed(1);
			break;

		case BRIEF_BUTTON_PREV_STAGE:
			brief_do_prev_pressed();
			break;

		case BRIEF_BUTTON_FIRST_STAGE:
			brief_do_start_pressed();
			break;

		case BRIEF_BUTTON_SCROLL_UP:
			brief_scroll_up_text();
			break;

		case BRIEF_BUTTON_SCROLL_DOWN:
			brief_scroll_down_text();
			break;

		case BRIEF_BUTTON_PAUSE:
			gamesnd_play_iface(SND_USER_SELECT);
			Player->auto_advance ^= 1;
			break;

		case BRIEF_BUTTON_SKIP_TRAINING:
			brief_skip_training_pressed();
			break;

		case BRIEF_BUTTON_EXIT_LOOP:
			brief_exit_loop_pressed();
			break;

		case BRIEF_BUTTON_MULTI_LOCK:
			Assert(Game_mode & GM_MULTIPLAYER);			
			// the "lock" button has been pressed
			multi_ts_lock_pressed();

			// disable the button if it is now locked
			if(multi_ts_is_locked()){
				Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.disable();
			}
			break;
	} // end switch
}

// -------------------------------------------------------------------
// brief_check_buttons()
//
// Iterate through the briefing buttons, checking if they are pressed
//
void brief_check_buttons()
{
	int			i;
	UI_BUTTON	*b;

	for (i=0; i<NUM_BRIEF_BUTTONS; i++) {
		b = &Brief_buttons[gr_screen.res][i].button;
		if ( b->pressed() ) {
			common_flash_button_init();
			brief_button_do(i);
		}
	}

	if (Closeup_close_button.pressed()) {
		brief_turn_off_closeup_icon();
	}
}

// -------------------------------------------------------------------
// brief_redraw_pressed_buttons()
//
// Redraw any briefing buttons that are pressed down.  This function is needed
// since we sometimes need to draw pressed buttons last to ensure the entire
// button gets drawn (and not overlapped by other buttons)
//
void brief_redraw_pressed_buttons()
{
	int			i;
	UI_BUTTON	*b;
	
	common_redraw_pressed_buttons();

	for ( i = 0; i < NUM_BRIEF_BUTTONS; i++ ) {
		b = &Brief_buttons[gr_screen.res][i].button;
		if ( b->button_down() ) {
			b->draw_forced(2);
		}
	}

	if ( !Player->auto_advance ) {
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_PAUSE].button.draw_forced(2);
	}
}

void brief_buttons_init()
{
	UI_BUTTON	*b;
	int			i;

	//if ( Briefing->num_stages <= 0 )
	//	return;

	for ( i = 0; i < NUM_BRIEF_BUTTONS; i++ ) {
		b = &Brief_buttons[gr_screen.res][i].button;
		b->create( &Brief_ui_window, "", Brief_buttons[gr_screen.res][i].x, Brief_buttons[gr_screen.res][i].y, 60, 30, 0, 1 );
		// set up callback for when a mouse first goes over a button
		b->set_highlight_action( common_play_highlight_sound );

		if ((i == BRIEF_BUTTON_SKIP_TRAINING) || (i == BRIEF_BUTTON_EXIT_LOOP)) {
			b->set_bmaps(Brief_buttons[gr_screen.res][i].filename, 3, 0);
		} else {
			b->set_bmaps(Brief_buttons[gr_screen.res][i].filename);
		}
		b->link_hotspot(Brief_buttons[gr_screen.res][i].hotspot);
	}

#ifndef MAKE_FS1
	// add all xstrs
	for(i=0; i<BRIEF_SELECT_NUM_TEXT; i++) {
		Brief_ui_window.add_XSTR(&Brief_select_text[gr_screen.res][i]);
	}
#endif

	// Hide the 'skip training' button by default.  Only enable and unhide if we are playing a training
	// mission
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_SKIP_TRAINING].button.disable();
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_SKIP_TRAINING].button.hide();
	if ( (Game_mode & GM_NORMAL) && (The_mission.game_type & MISSION_TYPE_TRAINING) ) {
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_SKIP_TRAINING].button.enable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_SKIP_TRAINING].button.unhide();
	}

	// Hide the 'exit loop' button by default.  Only enable and unhide if we are playing a loop
	// mission
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_EXIT_LOOP].button.disable();
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_EXIT_LOOP].button.hide();
	if ( (Game_mode & GM_NORMAL) && (Campaign.loop_enabled) && (Game_mode & GM_CAMPAIGN_MODE) ) {
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_EXIT_LOOP].button.enable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_EXIT_LOOP].button.unhide();
	}

	// maybe disable the multi-lock button
	if(!(Game_mode & GM_MULTIPLAYER)){
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.disable();
	} else {
		// if we're not the host of the game (or a tema captain in team vs. team mode), disable the lock button
		if(Netgame.type_flags & NG_TYPE_TEAM){
			if(!(Net_player->flags & NETINFO_FLAG_TEAM_CAPTAIN)){
				Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.disable();
			}
		} else {
			if(!(Net_player->flags & NETINFO_FLAG_GAME_HOST)){
				Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.disable();
			}
		}
	}

	// create close button for closeup popup
	Closeup_close_button.create( &Brief_ui_window, "", Closeup_button_coords[gr_screen.res][BRIEF_X_COORD], Closeup_button_coords[gr_screen.res][BRIEF_Y_COORD], 60, 30, 0, 1 );
	Closeup_close_button.set_highlight_action( common_play_highlight_sound );
	Closeup_close_button.set_bmaps(Closeup_button_filename[gr_screen.res]);
	Closeup_close_button.link_hotspot(Closeup_button_hotspot);

	// set up hotkeys for buttons so we draw the correct animation frame when a key is pressed
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_LAST_STAGE].button.set_hotkey(KEY_SHIFTED|KEY_RIGHT);
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_NEXT_STAGE].button.set_hotkey(KEY_RIGHT);
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_PREV_STAGE].button.set_hotkey(KEY_LEFT);
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_FIRST_STAGE].button.set_hotkey(KEY_SHIFTED|KEY_LEFT);
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_SCROLL_UP].button.set_hotkey(KEY_UP);
	Brief_buttons[gr_screen.res][BRIEF_BUTTON_SCROLL_DOWN].button.set_hotkey(KEY_DOWN);

	Closeup_close_button.disable();
	Closeup_close_button.hide();

	// if we have no briefing stages, hide and disable briefing buttons
	if(Num_brief_stages <= 0){
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_LAST_STAGE].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_LAST_STAGE].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_NEXT_STAGE].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_NEXT_STAGE].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_PREV_STAGE].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_PREV_STAGE].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_FIRST_STAGE].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_FIRST_STAGE].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_SCROLL_UP].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_SCROLL_UP].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_SCROLL_DOWN].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_SCROLL_DOWN].button.hide();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_PAUSE].button.disable();
		Brief_buttons[gr_screen.res][BRIEF_BUTTON_PAUSE].button.hide();		
	}
}

// --------------------------------------------------------------------------------------
//	brief_get_closeup_icon()
//
//
brief_icon *brief_get_closeup_icon()
{
	return Closeup_icon;
}

// stop showing the closeup view of an icon
void brief_turn_off_closeup_icon()
{
	// turn off closup
	if ( Closeup_icon != NULL ) {
		gamesnd_play_iface(SND_BRIEF_ICON_SELECT);
		Closeup_icon = NULL;
		Closeup_close_button.disable();
		Closeup_close_button.hide();
	}
}

// --------------------------------------------------------------------------------------
//	brief_load_bitmaps()
//
//
void brief_load_bitmaps()
{
	Brief_text_bitmap = bm_load(Brief_infobox_filename[gr_screen.res]);
	Brief_grid_bitmap = bm_load(Brief_win_filename[gr_screen.res]);
	
	if ( Closeup_bitmap == -1 ) {
		Closeup_bitmap = bm_load(Closeup_background_filename[gr_screen.res]);
	}

#ifdef MAKE_FS1
	MapWin01 = bm_load(NOX("MapWin01"));
	MapWin02 = bm_load(NOX("MapWin02"));
	MapWin03 = bm_load(NOX("MapWin03"));
	MapWin04 = bm_load(NOX("MapWin04"));
#endif
}

// --------------------------------------------------------------------------------------
//	brief_ui_init()
//
//
void brief_ui_init()
{
	if(Game_mode & GM_MULTIPLAYER) {
		Brief_background_bitmap = bm_load(Brief_multi_filename[gr_screen.res]);
	} else {
		Brief_background_bitmap = bm_load(Brief_filename[gr_screen.res]);	
	}

	if ( Num_brief_stages <= 0 ){
		return;
	}

	brief_load_bitmaps();
}


// --------------------------------------------------------------------------------------
//	brief_set_default_closeup()
//
//
void brief_set_default_closeup()
{
	brief_stage		*bs;
	int				i;

	bs = &Briefing->stages[0];

	if ( Briefing->num_stages <= 0 ) {
		Closeup_icon = NULL;
		return;
	}

	if ( bs->num_icons <= 0 ) {
		Closeup_icon = NULL;
		return;
	}

	// check for the first highlighted icons to have as the default closeup
	for ( i = 0; i < bs->num_icons; i++ ) {
		if ( bs->icons[i].flags & BI_HIGHLIGHT )
			break;
	}
	
	if ( i == bs->num_icons ) {
		brief_setup_closeup(&bs->icons[0]);
	}
	else {
		brief_setup_closeup(&bs->icons[i]);
	}
}

//  funciton to evaluate the sexpressions of the briefing stages eliminating those stages
// which shouldn't get shown
void brief_compact_stages()
{
	int num, before, result, i;

	/*
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.campaign_mode == MP_CAMPAIGN) && !(Net_player->flags & NETINFO_FLAG_AM_MASTER)){
		Game_mode |= GM_CAMPAIGN_MODE;
	}
	*/

	before = Briefing->num_stages;

	num = 0;
	while ( num < Briefing->num_stages ) {
		result = eval_sexp( Briefing->stages[num].formula );
		if ( !result ) {
			if ( Briefing->stages[num].new_text ) {
				free( Briefing->stages[num].new_text );
				Briefing->stages[num].new_text = NULL;
			}

			if ( Briefing->stages[num].icons ) {
				free( Briefing->stages[num].icons );
				Briefing->stages[num].icons = NULL;
			}


			if ( Briefing->stages[num].lines ) {
				free( Briefing->stages[num].lines );
				Briefing->stages[num].lines = NULL;
			}

			Briefing->stages[num].num_icons = 0;
			for ( i = num+1; i < Briefing->num_stages; i++ ) {
				Briefing->stages[i-1] = Briefing->stages[i];
			}
			Briefing->num_stages--;
			continue;
		}
		num++;
	}

	// completely clear out the old entries (if any) so we don't access them by mistake - taylor
	if (before > Briefing->num_stages) {
		for (i = Briefing->num_stages; i < before; i++) {
			memset( &Briefing->stages[i], 0, sizeof(brief_stage) );
		}
	}

	/*
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.campaign_mode == MP_CAMPAIGN) && !(Net_player->flags & NETINFO_FLAG_AM_MASTER)){
		Game_mode &= ~(GM_CAMPAIGN_MODE);
	}
	*/
}


// --------------------------------------------------------------------------------------
// brief_init() 
//
	int red_alert_mission(void);
//
void brief_init()
{
	// Since first stage of briefing can take some time to arrive and play, 
	// reset the trailer timer on briefing init.
#if defined(FS2_DEMO) || defined(FS1_DEMO)
	demo_reset_trailer_timer();
#endif

	// for multiplayer, change the state in my netplayer structure
	// and initialize the briefing chat area thingy
	if ( Game_mode & GM_MULTIPLAYER ){
		Net_player->state = NETPLAYER_STATE_BRIEFING;
	}

	// Non standard briefing in red alert mission
	if ( red_alert_mission() ) {
		gameseq_post_event(GS_EVENT_RED_ALERT);
		return;
	}

	// get a pointer to the appropriate briefing structure
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_TEAM)){
		Briefing = &Briefings[Net_player->p_info.team];
	} else {
		Briefing = &Briefings[0];			
	}

	Brief_last_auto_advance = 0;

	brief_compact_stages();			// compact the briefing array to eliminate unused stages

	common_set_interface_palette("BriefingPalette");

	ship_stop_animation();
	set_active_ui(&Brief_ui_window);
	Current_screen = ON_BRIEFING_SELECT;
	brief_restart_text_wipe();
	common_flash_button_init();
	common_music_init(SCORE_BRIEFING);


	help_overlay_set_state(BR_OVERLAY,0);

	if ( Brief_inited == TRUE ) {
		common_buttons_maybe_reload(&Brief_ui_window);	// AL 11-21-97: this is necessary since we may returning from the hotkey
																		// screen, which can release common button bitmaps.
		common_reset_buttons();
		nprintf(("Alan","brief_init() returning without doing anything\n"));
		return;
	}

	if (The_mission.game_type & MISSION_TYPE_TRAINING)
		Num_brief_stages = Briefing->num_stages;
	else
		Num_brief_stages = Briefing->num_stages + 1;

	Current_brief_stage = 0;
	Last_brief_stage = 0;

	// init the scene-cut data
	brief_transition_reset();

#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
	hud_anim_init(&Fade_anim, Brief_static_coords[gr_screen.res][0], Brief_static_coords[gr_screen.res][1], Brief_static_name[gr_screen.res]);
	hud_anim_load(&Fade_anim);
#endif

	nprintf(("Alan","Entering brief_init()\n"));
	common_select_init();

	if(Game_mode & GM_MULTIPLAYER) {
		BriefingMaskBitmap = bm_load(Brief_multi_mask_filename[gr_screen.res]);
	} else {
		BriefingMaskBitmap = bm_load(Brief_mask_filename[gr_screen.res]);
	}

	if (BriefingMaskBitmap < 0) {
		Error(LOCATION,"Could not load in 'brief-m'!");
	}

	Briefing_mask_w = -1;
	Briefing_mask_h = -1;

	// get a pointer to bitmap by using bm_lock()
	BriefingMaskPtr = bm_lock(BriefingMaskBitmap, 8, BMP_AABITMAP);
	BriefingMaskData = (ubyte*)BriefingMaskPtr->data;
	bm_get_info(BriefingMaskBitmap, &Briefing_mask_w, &Briefing_mask_h);

	help_overlay_load(BR_OVERLAY);

	// Set up the mask regions
   // initialize the different regions of the menu that will react when the mouse moves over it
	Num_briefing_regions = 0;

	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	COMMON_BRIEFING_REGION,				0);
#ifndef FS1_DEMO  // not available in demo
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	COMMON_SS_REGION,						0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	COMMON_WEAPON_REGION,				0);
#endif
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	COMMON_COMMIT_REGION,				0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	COMMON_HELP_REGION,					0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	COMMON_OPTIONS_REGION,				0);

	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	BRIEF_LAST_STAGE_MASK,			0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	BRIEF_NEXT_STAGE_MASK,			0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	BRIEF_PREV_STAGE_MASK,			0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	BRIEF_FIRST_STAGE_MASK,			0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	BRIEF_TEXT_SCROLL_UP_MASK,		0);
	snazzy_menu_add_region(&Briefing_select_region[Num_briefing_regions++], "",	BRIEF_TEXT_SCROLL_DOWN_MASK,	0);

	// init common UI
	Brief_ui_window.create( 0, 0, gr_screen.max_w, gr_screen.max_h, 0 );

	if(Game_mode & GM_MULTIPLAYER){
		Brief_ui_window.set_mask_bmap(Brief_mask_multi[gr_screen.res]);
	} else {
		Brief_ui_window.set_mask_bmap(Brief_mask_single[gr_screen.res]);
	}

	Brief_ui_window.tooltip_handler = brief_tooltip_handler;
	common_buttons_init(&Brief_ui_window);
	brief_buttons_init();

	// if multiplayer, initialize a few other systems
	if(Game_mode & GM_MULTIPLAYER){		
		// again, should not be necessary, but we'll leave it for now
		chatbox_create();

		// force the chatbox to be small
		chatbox_force_small();
	}

	// set up the screen regions
	brief_init_screen(Brief_multiplayer);

	// init briefing specific UI
	brief_ui_init();

	// init the briefing map
	brief_init_map();

	// init the briefing voice playback
	brief_voice_init();
	brief_voice_load_all();

	// init objectives display stuff
	ML_objectives_init(Brief_goals_coords[gr_screen.res][BRIEF_X_COORD], Brief_goals_coords[gr_screen.res][BRIEF_Y_COORD], Brief_goals_coords[gr_screen.res][BRIEF_W_COORD], Brief_goals_coords[gr_screen.res][BRIEF_H_COORD]);

	// set the camera target
	if ( Briefing->num_stages > 0 ) {
		brief_set_new_stage(&Briefing->stages[0].camera_pos, &Briefing->stages[0].camera_orient, 0, Current_brief_stage);
		brief_reset_icons(Current_brief_stage);
	}

	Brief_playing_fade_sound = 0;
	Brief_mouse_up_flag	= 0;
	Closeup_font_height = gr_get_font_height();
	Closeup_icon = NULL;
   Brief_inited = TRUE;
}

// -------------------------------------------------------------------------------------
// brief_render_closeup_text()
//
//
#define CLOSEUP_TEXT_OFFSET	10
void brief_render_closeup_text()
{
/*
	brief_icon	*bi;
	char			line[MAX_ICON_TEXT_LINE_LEN];
	int			n_lines, i, render_x, render_y;
	int			n_chars[MAX_ICON_TEXT_LINES];
	char			*p_str[MAX_ICON_TEXT_LINES];

	if ( Closeup_icon == NULL ) {
		Int3();
		return;
	}

	bi = Closeup_icon;

	render_x = Closeup_region[0];
	render_y = Closeup_region[1] + CLOSEUP_IMG_H;
	
	gr_set_clip(render_x+CLOSEUP_TEXT_OFFSET, render_y, CLOSEUP_W,CLOSEUP_TEXT_H);
	gr_set_color_fast(&Color_white);

//	n_lines = split_str(bi->text, CLOSEUP_W - 2*CLOSEUP_TEXT_OFFSET, n_chars, p_str, MAX_ICON_TEXT_LINES);
	Assert(n_lines != -1);

	for ( i = 0; i < n_lines; i++ ) {
		Assert(n_chars[i] < MAX_ICON_TEXT_LINE_LEN);
		strncpy(line, p_str[i], n_chars[i]);
		line[n_chars[i]] = 0;
		gr_printf(0,0+i*Closeup_font_height,line);
	}
*/
}

// -------------------------------------------------------------------------------------
// brief_render_closeup()
//
//
void brief_render_closeup(int ship_class, float frametime)
{
	matrix	view_orient = IDENTITY_MATRIX;
	matrix	temp_matrix;
	float		ang;
	int		w,h;

	if (ship_class < 0)
		return;

	if (Closeup_bitmap < 0)
		return;

	ang = PI2 * frametime/Closeup_one_revolution_time;
	if ( ang > MAX_ANG_CHG )
		ang = MAX_ANG_CHG;

	Closeup_angles.h += ang;
	if ( Closeup_angles.h > PI2 )
		Closeup_angles.h -= PI2;
	vm_angles_2_matrix(&temp_matrix, &Closeup_angles );
	Closeup_orient = temp_matrix;

	w = Closeup_region[gr_screen.res][2];
	h = Closeup_region[gr_screen.res][3];
	gr_set_clip(Closeup_region[gr_screen.res][0], Closeup_region[gr_screen.res][1], w, h);

	g3_start_frame(1);

	g3_set_view_matrix(&Closeup_cam_pos, &view_orient, Closeup_zoom);
	model_clear_instance( Closeup_icon->modelnum );
	model_set_detail_level(0);

	int is_neb = The_mission.flags & MISSION_FLAG_FULLNEB;

	// maybe switch off nebula rendering
	if(is_neb){
		The_mission.flags &= ~MISSION_FLAG_FULLNEB;
	}

	int model_render_flags;
	if ( Closeup_icon->type == ICON_JUMP_NODE ) {
		model_set_outline_color(HUD_color_red, HUD_color_green, HUD_color_blue);		
		model_render_flags = MR_NO_LIGHTING | MR_LOCK_DETAIL | MR_AUTOCENTER | MR_NO_POLYS | MR_SHOW_OUTLINE;
	} else {
		model_render_flags = MR_NO_LIGHTING | MR_LOCK_DETAIL | MR_AUTOCENTER;
	}

	model_render( Closeup_icon->modelnum, &Closeup_orient, &Closeup_pos, model_render_flags );

	if (is_neb) {
		The_mission.flags |= MISSION_FLAG_FULLNEB;
	}

	g3_end_frame();

	gr_set_color_fast(&Color_bright_white);

	gr_printf(0x8000,2,Closeup_icon->closeup_label);
//	brief_render_closeup_text();

	Closeup_close_button.enable();
	Closeup_close_button.unhide();

	gr_reset_clip();
}

// -------------------------------------------------------------------------------------
// brief_render()
//
//	frametime is in seconds
void brief_render(float frametime)
{
	int z;
	int w;

#ifndef NDEBUG
	int h;
#endif

	if ( Num_brief_stages <= 0 ) {
		gr_set_color_fast(&Color_white);
		Assert( Game_current_mission_filename != NULL );
		gr_printf(0x8000,200,XSTR( "No Briefing exists for mission: %s", 430), Game_current_mission_filename);

		#ifndef NDEBUG
		gr_get_string_size(&w, &h, The_mission.name);
		gr_set_color_fast(&Color_normal);
		gr_printf(0x8000, 230, NOX("[filename: %s, last mod: %s]"), Mission_filename, The_mission.modified);
		#endif

		return;
	}

	gr_set_bitmap(Brief_grid_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	gr_bitmap(Brief_bmap_coords[gr_screen.res][0], Brief_bmap_coords[gr_screen.res][1]);

	brief_render_map(Current_brief_stage, frametime);

	// draw the frame bitmaps
	gr_set_bitmap(Brief_text_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	gr_bitmap(Brief_infobox_coords[gr_screen.res][0], Brief_infobox_coords[gr_screen.res][1]);
	brief_blit_stage_num(Current_brief_stage, Num_brief_stages);

	z = brief_render_text(Top_brief_text_line, Brief_text_coords[gr_screen.res][0], Brief_text_coords[gr_screen.res][1], Brief_text_coords[gr_screen.res][3], frametime);
	if (z) {
		brief_voice_play(Current_brief_stage);
	}

	// maybe output the "more" indicator
	if ( (Brief_text_max_lines[gr_screen.res] + Top_brief_text_line + 2) < Num_brief_text_lines[0] ) {
		// can be scrolled down
		int more_txt_x = Brief_text_coords[gr_screen.res][0] + (Brief_max_line_width[gr_screen.res]/2) - 10;
		int more_txt_y = Brief_text_coords[gr_screen.res][1] + Brief_text_coords[gr_screen.res][3] - 2;				// located below brief text, centered
		int w, h;
		gr_get_string_size(&w, &h, XSTR("more", 1469), strlen(XSTR("more", 1469)));
		gr_set_color_fast(&Color_black);
		gr_rect(more_txt_x-2, more_txt_y, w+3, h);
		gr_set_color_fast(&Color_red);
		gr_string(more_txt_x, more_txt_y, XSTR("more", 1469));  // base location on the input x and y?
	}

	brief_maybe_blit_scene_cut(frametime);	

#ifdef MAKE_FS1
	if (MapWin01 != -1) {
		gr_set_bitmap(MapWin01, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(63, 122);
	}

	if (MapWin02 != -1) {
		gr_set_bitmap(MapWin02, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(575, 122);
	}

	if (MapWin03 != -1) {
		gr_set_bitmap(MapWin03, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(63, 350);
	}

	if (MapWin04 != -1) {
		gr_set_bitmap(MapWin04, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(42, 122);
	}
#endif

#if !defined(NDEBUG) || defined(INTERPLAYQA)
	gr_set_color_fast(&Color_normal);
	int title_y_offset = (Game_mode & GM_MULTIPLAYER) ? 20 : 10;
	gr_printf(Brief_bmap_coords[gr_screen.res][0], Brief_bmap_coords[gr_screen.res][1]-title_y_offset, NOX("[name: %s, mod: %s]"), Mission_filename, The_mission.modified);
#endif

	// output mission title
#ifndef MAKE_FS1
	gr_set_color_fast(&Color_bright_white);
#else
	gr_set_color_fast(&Color_bright_blue);
#endif
	if (Game_mode & GM_MULTIPLAYER) {
		char buf[256];
		strncpy(buf, The_mission.name, 256);
		gr_force_fit_string(buf, 255, Title_coords_multi[gr_screen.res][2]);
#ifdef MAKE_FS1
		// align from the end of the string instead of the beginning
		gr_get_string_size(&w, NULL, buf);
		gr_string(Title_coords_multi[gr_screen.res][0] - w, Title_coords_multi[gr_screen.res][1], buf);
#else
		gr_string(Title_coords_multi[gr_screen.res][0], Title_coords_multi[gr_screen.res][1], buf);
#endif
	} else {
		gr_get_string_size(&w, NULL, The_mission.name);
		gr_string(Title_coords[gr_screen.res][0] - w, Title_coords[gr_screen.res][1], The_mission.name);
	}

	// maybe do objectives
	if (Current_brief_stage == Briefing->num_stages) {
		ML_objectives_do_frame(0);
	}	
}

// -------------------------------------------------------------------------------------
// brief_set_closeup_pos()
//
//
#define CLOSEUP_OFFSET 20
void brief_set_closeup_pos(brief_icon *bi)
{
	Closeup_y1 = 10;
	Closeup_x1 = fl2i(320 - Closeup_coords[gr_screen.res][BRIEF_W_COORD]/2.0f + 0.5f);
}

void brief_get_closeup_ship_modelnum(brief_icon *ci)
{
	object	*objp;
	ship		*sp;

	// find the model number for the ship to display
	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {

		if ( objp == &obj_used_list || objp->type != OBJ_SHIP ) {
			continue;
		}
		
		sp = &Ships[objp->instance];
		if ( sp->ship_info_index == ci->ship_class ) {
			ci->ship_class = sp->ship_info_index;
			ci->modelnum = sp->modelnum;
			ci->radius = objp->radius;
			break;
		}
	}
}

// cut any text off after (and including) '#' char
void brief_truncate_label(char *src)
{
	char *pointer_to_last_char;

	pointer_to_last_char = strstr(src, NOX("#"));

	if ( pointer_to_last_char ) {
		*pointer_to_last_char = 0;
	}
}

// -------------------------------------------------------------------------------------
// brief_setup_closeup()
//
// exit: 0	=>		set-up icon sucessfully
//			-1	=>		could not setup closeup icon
int brief_setup_closeup(brief_icon *bi)
{
	char				pof_filename[NAME_LENGTH];
	ship_info		*sip=NULL;
	vector			tvec;

	Closeup_icon = bi;
	Closeup_icon->ship_class = bi->ship_class;
	Closeup_icon->modelnum = -1;

	Closeup_one_revolution_time = ONE_REV_TIME;

	switch(Closeup_icon->type) {
	case ICON_PLANET:
		Closeup_icon = NULL;
		return -1;
		/*
		strcpy(pof_filename, NOX("planet.pof"));
		strcpy(Closeup_icon->closeup_label, XSTR("planet",-1));
		vm_vec_make(&Closeup_cam_pos, 0.0f, 0.0f, -8300.0f);
		Closeup_zoom = 0.5f;
		Closeup_one_revolution_time = ONE_REV_TIME * 3;
		*/
		break;
	case ICON_ASTEROID_FIELD:
#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
		strcpy(pof_filename, Asteroid_info[ASTEROID_TYPE_BIG].pof_files[0]);
		strcpy(Closeup_icon->closeup_label, XSTR( "asteroid", 431));
		(void) vm_vec_make(&Closeup_cam_pos, 0.0f, 0.0f, -334.0f);
		Closeup_zoom = 0.5f;
#endif
		break;
	case ICON_JUMP_NODE:
		strcpy(pof_filename, NOX("subspacenode.pof"));
		strcpy(Closeup_icon->closeup_label, XSTR( "jump node", 432));
		(void) vm_vec_make(&Closeup_cam_pos, 0.0f, 0.0f, -2700.0f);
		Closeup_zoom = 0.5f;
		Closeup_one_revolution_time = ONE_REV_TIME * 3;
		break;
	case ICON_UNKNOWN:
	case ICON_UNKNOWN_WING:
		strcpy(pof_filename, NOX("unknownship.pof"));
		strcpy(Closeup_icon->closeup_label, XSTR( "unknown", 433));
		(void) vm_vec_make(&Closeup_cam_pos, 0.0f, 0.0f, -22.0f);
		Closeup_zoom = 0.5f;
		break;
	default:
		brief_get_closeup_ship_modelnum(Closeup_icon);
		Assert( Closeup_icon->ship_class != -1 );
		sip = &Ship_info[Closeup_icon->ship_class];

		strcpy(Closeup_icon->closeup_label,sip->name);

		// cut any text off after (and including) '#' char
		brief_truncate_label(Closeup_icon->closeup_label);

		if ( sip->flags & (SIF_SMALL_SHIP|SIF_BIG_SHIP|SIF_HUGE_SHIP|SIF_SENTRYGUN) ) {
			strcat(Closeup_icon->closeup_label, XSTR( " class", 434));
		}
		break;
	}
	
	if ( Closeup_icon->modelnum == -1 ) {
		if ( sip == NULL ) {
			Closeup_icon->modelnum = model_load(pof_filename, 0, NULL);
		} else {
			Closeup_icon->modelnum = model_load(sip->pof_file, sip->n_subsystems, &sip->subsystems[0]);
		}
		Closeup_icon->radius = model_get_radius(Closeup_icon->modelnum);
	}

	vm_set_identity(&Closeup_orient);
	(void) vm_vec_make(&tvec, 0.0f, 0.0f, -1.0f);
	Closeup_orient.v.fvec = tvec;
	vm_vec_zero(&Closeup_pos);
	Closeup_angles.p  = 0.0f;
	Closeup_angles.b  = 0.0f;
	Closeup_angles.h  = PI;

	brief_set_closeup_pos(bi);

	if ( sip ) {
		Closeup_cam_pos = sip->closeup_pos;
		Closeup_zoom = sip->closeup_zoom;
	}

	return 0;
}

// -------------------------------------------------------------------------------------
// brief_update_closeup_icon()
//
//	input:	mode	=>		how to update the closeup view
//								0 -> disable
//
void brief_update_closeup_icon(int mode)
{
	brief_stage		*bs;
	brief_icon		*bi;
	int				i, closeup_index;
	

	if ( mode == 0 ) {
		// mode 0 means disable the closeup icon
		if ( Closeup_icon != NULL ) {
			brief_turn_off_closeup_icon();
		}
		return;
	}

	if ( Closeup_icon == NULL )
		return;

	bs = &Briefing->stages[Current_brief_stage];

	closeup_index = -1;
	// see if any icons are being highlighted this stage
	for ( i = 0; i < bs->num_icons; i++ ) {
		bi = &bs->icons[i];
		if ( bi->flags & BI_HIGHLIGHT ) {
			closeup_index = i;
			break;
		}
	}

	if ( closeup_index != -1 ) {
		bi = &bs->icons[closeup_index];
		brief_setup_closeup(bi);
	}
	else {
		Closeup_icon = NULL;
	}
}


// -------------------------------------------------------------------------------------
// brief_check_for_anim()
//
//
void brief_check_for_anim()
{
	int				mx, my, i, iw, ih, x, y;
	brief_stage		*bs;
	brief_icon		*bi = NULL;

	bs = &Briefing->stages[Current_brief_stage];
	mouse_get_pos( &mx, &my );

	// if mouse click is over the VCR controls, don't launch an icon
	// FIXME - should prolly push these into defines instead of hardcoding this
//	if ( mx >= 0 && mx <= 115 && my >= 136 && my <= 148 ) {
//		return;
//	}

	// same as above but without the hardcoded values, which were wrong anyway.  don't know
	// how this will work longterm but will hopefully keep things working well - taylor
	for (i = 0; i <= BRIEF_BUTTON_FIRST_STAGE; i++) {
		Brief_buttons[gr_screen.res][i].button.get_dimensions(&x, &y, &iw, &ih);

		if (mx >= x && mx <= (x+iw) && my >= y && my <= (y+ih)) {
			return;
		}
 	}

	// if mouse coords are outside the briefing screen, then go away
	my -= bscreen.map_y1;
	mx -= bscreen.map_x1;
	if ( my < 0 || mx < 0 || mx > (bscreen.map_x2-bscreen.map_x1+1) || my > (bscreen.map_y2-bscreen.map_y1+1) )
		return;

	for ( i = 0; i < bs->num_icons; i++ ) {
		bi = &bs->icons[i];
		brief_common_get_icon_dimensions(&iw, &ih, bi->type, bi->ship_class);
		if ( mx < bi->x ) continue;
		if ( mx > (bi->x + iw) ) continue;
		if ( my < bi->y ) continue;
		if ( my > (bi->y + ih) ) continue;
		// if we've got here, must be a hit
		break;
	}

	if ( i == bs->num_icons ) {
		brief_turn_off_closeup_icon();
		return;
	}

	if ( brief_setup_closeup(bi) == 0 ) {
		gamesnd_play_iface(SND_BRIEF_ICON_SELECT);
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

// maybe flash a button if player hasn't done anything for a while
void brief_maybe_flash_button()
{
	UI_BUTTON *b;

	if ( Num_brief_stages <= 0 ) 
		return;

	if ( Closeup_icon != NULL ) {
		common_flash_button_init();
		return;
	}

	if ( common_flash_bright() ) {
		if ( Current_brief_stage == (Num_brief_stages-1) ) {

			// AL 4-4-98: Don't flash ship selection button on briefing in demo build
#if defined(FS2_DEMO) || defined(FS1_DEMO)
				return;
#else
			// AL 30-3-98: Don't flash ship selection button if in a training mission, 
			if ( brief_only_allow_briefing() ) {
				return;
			}

			b = &Common_buttons[Current_screen-1][gr_screen.res][1].button;		// ship select button
#endif
		} else {
			b = &Brief_buttons[gr_screen.res][1].button;		// next stage button
		}

		if ( b->button_hilighted() ) {
			common_flash_button_init();
		} else {
			b->draw_forced(1);
		}
	}
}

// -------------------------------------------------------------------------------------
// brief_do_frame()
//
// frametime is in seconds
//
void brief_do_frame(float frametime)
{
	int k, brief_choice;

	if ( red_alert_mission() ) {
		return;
	}


	if ( !Brief_inited ){
		brief_init();
	}

	int snazzy_action = -1;
	brief_choice = snazzy_menu_do(BriefingMaskData, Briefing_mask_w, Briefing_mask_h, Num_briefing_regions, Briefing_select_region, &snazzy_action, 0);

	k = common_select_do(frametime);

	if ( Closeup_icon ) {
		Brief_mouse_up_flag = 0;
	}

	if ( help_overlay_active(BR_OVERLAY) ) {
		common_flash_button_init();
		brief_turn_off_closeup_icon();
	}

	// Check common keypresses
	common_check_keys(k);

#ifndef NDEBUG
	int cam_change = 0;
#endif

	if ( Briefing->num_stages > 0 ) {

		// check for special keys
		switch(k) {

#ifndef NDEBUG			
			case KEY_CTRLED | KEY_PAGEUP: {
				if (Closeup_icon->ship_class) {
					Closeup_icon->ship_class--;

					ship_info *sip = &Ship_info[Closeup_icon->ship_class];
					if (sip->modelnum < 0)
						sip->modelnum = model_load(sip->pof_file, 0, NULL);

					mprintf(("Shiptype = %d (%s)\n", Closeup_icon->ship_class, sip->name));
					mprintf(("Modelnum = %d (%s)\n", sip->modelnum, sip->pof_file));
					brief_setup_closeup(Closeup_icon);
				}

				break;
			}

			case KEY_CTRLED | KEY_PAGEDOWN: {
				if (Closeup_icon->ship_class < Num_ship_types - 1) {
					Closeup_icon->ship_class++;

					ship_info *sip = &Ship_info[Closeup_icon->ship_class];
					if (sip->modelnum < 0)
						sip->modelnum = model_load(sip->pof_file, 0, NULL);

					mprintf(("Shiptype = %d (%s)\n", Closeup_icon->ship_class, sip->name));
					mprintf(("Modelnum = %d (%s)\n", sip->modelnum, sip->pof_file));
					brief_setup_closeup(Closeup_icon);
				}

				break;
			}

			case KEY_A:
				Closeup_cam_pos.xyz.z += 1;
				cam_change = 1;
				break;

			case KEY_A + KEY_SHIFTED:
				Closeup_cam_pos.xyz.z += 10;
				cam_change = 1;
				break;

			case KEY_Z:
				Closeup_cam_pos.xyz.z -= 1;
				cam_change = 1;
				break;

			case KEY_Z + KEY_SHIFTED:
				Closeup_cam_pos.xyz.z -= 10;
				cam_change = 1;
				break;
			
			case KEY_Y:
				Closeup_cam_pos.xyz.y += 1;
				cam_change = 1;
				break;

			case KEY_Y + KEY_SHIFTED:
				Closeup_cam_pos.xyz.y += 10;
				cam_change = 1;
				break;

			case KEY_H:
				Closeup_cam_pos.xyz.y -= 1;
				cam_change = 1;
				break;

			case KEY_H + KEY_SHIFTED:
				Closeup_cam_pos.xyz.y -= 10;
				cam_change = 1;
				break;

			case KEY_COMMA:
				Closeup_zoom -= 0.1f;
				if ( Closeup_zoom < 0.1 ) 
					Closeup_zoom = 0.1f;
				cam_change = 1;
				break;

			case KEY_COMMA+KEY_SHIFTED:
				Closeup_zoom -= 0.5f;
				if ( Closeup_zoom < 0.1 ) 
					Closeup_zoom = 0.1f;
				cam_change = 1;
				break;

			case KEY_PERIOD:
				Closeup_zoom += 0.1f;
				cam_change = 1;
				break;

			case KEY_PERIOD+KEY_SHIFTED:
				Closeup_zoom += 0.5f;
				cam_change = 1;
				break;
#endif
			case 1000:		// need this to avoid warning about no case
				break;

			default:
				break;
		} // end switch
	}

#ifndef NDEBUG
	if ( cam_change ) {
		nprintf(("General","Camera pos: %.2f, %.2f %.2f // ", Closeup_cam_pos.xyz.x, Closeup_cam_pos.xyz.y, Closeup_cam_pos.xyz.z));
		nprintf(("General","Camera zoom: %.2f\n", Closeup_zoom));
	}
#endif

	if ( brief_choice > -1 && snazzy_action == SNAZZY_OVER ) {
		Brief_mouse_up_flag = 0;
		brief_choice = -1;
	}


	common_check_buttons();
	// if ( Briefing->num_stages > 0 )
	brief_check_buttons();

	if ( brief_choice != -1 ) {
		Brief_mouse_up_flag = 0;
	}

	gr_reset_clip();

	common_render(frametime);

	if ( Current_brief_stage < (Num_brief_stages-1) ) {
		if ( !help_overlay_active(BR_OVERLAY) && brief_time_to_advance(Current_brief_stage, frametime) ) {
			brief_do_next_pressed(0);
			common_flash_button_init();
			Brief_last_auto_advance = timer_get_milliseconds();
		}
	}

	if ( !Background_playing ) {
		int time = -1;
		int check_jump_flag = 1;
		if ( Current_brief_stage != Last_brief_stage ) {

			// Check if we have a quick transition pending
			if ( Quick_transition_stage != -1 ) {
					Quick_transition_stage = -1;
					brief_reset_last_new_stage();
					time = 0;
					check_jump_flag = 0;
			}

			if ( check_jump_flag ) {
				if ( abs(Current_brief_stage - Last_brief_stage) > 1 ) {
					Quick_transition_stage = Current_brief_stage;
					Current_brief_stage = Last_brief_stage;
					Assert(Current_brief_stage >= 0);
					Start_fade_up_anim = 1;
					goto Transition_done;
				}
			}

			if ( time != 0 ) {
				if ( Current_brief_stage > Last_brief_stage ) {
					if ( Briefing->stages[Last_brief_stage].flags & BS_FORWARD_CUT ) {
						Quick_transition_stage = Current_brief_stage;
						Current_brief_stage = Last_brief_stage;
						Assert(Current_brief_stage >= 0);
						Start_fade_up_anim = 1;
						goto Transition_done;
					} else {
						time = Briefing->stages[Current_brief_stage].camera_time;
					}
				}
				else {
					if ( Briefing->stages[Last_brief_stage].flags & BS_BACKWARD_CUT ) { 
						Quick_transition_stage = Current_brief_stage;
						Current_brief_stage = Last_brief_stage;
						Assert(Current_brief_stage >= 0);
						Start_fade_up_anim = 1;
						goto Transition_done;
					} else {
						time = Briefing->stages[Last_brief_stage].camera_time;
					}
				}
			}

			brief_voice_stop(Last_brief_stage);

			if ( Current_brief_stage < 0 ) {
				Int3();
				Current_brief_stage=0;
			}

			// set the camera target
			brief_set_new_stage(&Briefing->stages[Current_brief_stage].camera_pos,
									  &Briefing->stages[Current_brief_stage].camera_orient,
									  time, Current_brief_stage);

			Brief_playing_fade_sound = 0;
			Last_brief_stage = Current_brief_stage;
			brief_reset_icons(Current_brief_stage);
			brief_update_closeup_icon(0);
		}

		Transition_done:

		if ( Brief_mouse_up_flag && !Closeup_icon) {
			brief_check_for_anim();
		}

		brief_render(frametime);
		brief_camera_move(frametime, Current_brief_stage);

		if (Closeup_icon && (Closeup_bitmap >= 0)) {
			// blit closeup background
			gr_set_bitmap(Closeup_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
			gr_bitmap(Closeup_coords[gr_screen.res][BRIEF_X_COORD], Closeup_coords[gr_screen.res][BRIEF_Y_COORD]);
		}

		Brief_ui_window.draw();
		brief_redraw_pressed_buttons();
		common_render_selected_screen_button();

		if (Closeup_icon) {
			brief_render_closeup(Closeup_icon->ship_class, frametime);
		}

		// render some extra stuff in multiplayer
		if (Game_mode & GM_MULTIPLAYER) {
			// should render this last so that it overlaps all controls
			chatbox_render();

			// render the status indicator for the voice system
			multi_common_voice_display_status();

			// blit the "ships/players" locked button
			// multi_ts_blit_locked_button();

			// maybe blit the multiplayer "locked" button	
			// if its locked, everyone blits it as such
			if(multi_ts_is_locked()){
				Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.draw_forced(2);
			} 
			// anyone who can't hit the button sees it off, otherwise
			else {
				if( ((Netgame.type_flags & NG_TYPE_TEAM) && !(Net_player->flags & NETINFO_FLAG_TEAM_CAPTAIN)) ||
					 ((Netgame.type_flags & NG_TYPE_TEAM) && !(Net_player->flags & NETINFO_FLAG_GAME_HOST)) ){
					Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.draw_forced(0);
				} else {
					Brief_buttons[gr_screen.res][BRIEF_BUTTON_MULTI_LOCK].button.draw();
				}
			}
		}
	}		

	// maybe flash a button if player hasn't done anything for a while
	brief_maybe_flash_button();

	// blit help overlay if active
	help_overlay_maybe_blit(BR_OVERLAY);	

	gr_flip();	

	// If the commit button was pressed, do the commit button actions.  Done at the end of the
	// loop so there isn't a skip in the animation (since ship_create() can take a long time if
	// the ship model is not in memory
	if (Commit_pressed) {
		if (Game_mode & GM_MULTIPLAYER) {
			multi_ts_commit_pressed();
		} else {
			commit_pressed();
		}

		Commit_pressed = 0;
	}
}

// --------------------------------------------------------------------------------------
//	brief_unload_bitmaps()
//
//
void brief_unload_bitmaps()
{	
	if ( BriefingMaskBitmap != -1 ) {
		bm_unload(BriefingMaskBitmap);
		BriefingMaskBitmap = -1;
	}

	if ( Brief_text_bitmap != -1 ) {
		bm_unload(Brief_text_bitmap);
		Brief_text_bitmap = -1;
	}

	if(Brief_grid_bitmap != -1){
		bm_unload(Brief_grid_bitmap);
		Brief_grid_bitmap = -1;
	}

	if ( Brief_multitext_bitmap != -1 ) {
		bm_unload(Brief_multitext_bitmap);
		Brief_multitext_bitmap = -1;
	}

	if ( Brief_background_bitmap != -1 ) {
		bm_unload(Brief_background_bitmap);
		Brief_background_bitmap = -1;
	}

#ifdef MAKE_FS1
	if (MapWin01 != -1){
		bm_unload(MapWin01);
		MapWin01 = -1;
	}

	if (MapWin02 != -1){
		bm_unload(MapWin02);
		MapWin02 = -1;
	}

	if (MapWin03 != -1){
		bm_unload(MapWin03);
		MapWin03 = -1;
	}

	if (MapWin04 != -1){
		bm_unload(MapWin04);
		MapWin04 = -1;
	}
#endif

	help_overlay_unload(BR_OVERLAY);
}

// ------------------------------------------------------------------------------------
// brief_close()
//
//
void brief_close()
{
	if ( Brief_inited == FALSE ) {
		nprintf(("Warning","brief_close() returning without doing anything\n"));
		return;
	}

	nprintf(("Alan", "Entering brief_close()\n"));

	ML_objectives_close();

	// unload the audio streams used for voice playback
	brief_voice_unload_all();

#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
	hud_anim_release(&Fade_anim);
#endif

	// done mask bitmap, so unlock it
	bm_unlock(BriefingMaskBitmap);

	Brief_ui_window.destroy();

#ifdef MAKE_FS1
	// restore palette
	common_free_interface_palette();
#endif

	// unload the bitmaps
	brief_unload_bitmaps();

	brief_common_close();

	Brief_inited = FALSE;
}

void briefing_stop_music()
{
	if ( Briefing_music_handle != -1 ) {
		audiostream_close_file(Briefing_music_handle);
		Briefing_music_handle = -1;
	}
}

void briefing_load_music(const char* fname)
{
	if ( Cmdline_freespace_no_music ) {
		return;
	}

	if ( Briefing_music_handle != -1 )
		return;

	if ( fname )
		Briefing_music_handle = audiostream_open( fname, ASF_EVENTMUSIC );
}

void briefing_start_music()
{
	if ( Briefing_music_handle != -1 ) {
		if ( !audiostream_is_playing(Briefing_music_handle) )
			audiostream_play(Briefing_music_handle, Master_event_music_volume);
	}
	else {
		nprintf(("Warning", "No music file exists to play music at this briefing!\n"));
	}
}

void brief_stop_voices()
{
	brief_voice_stop(Current_brief_stage);
}

void brief_maybe_blit_scene_cut(float frametime)
{
	if ( Start_fade_up_anim ) {

#if defined(FS2_DEMO) || defined(FS1_DEMO)
		Fade_anim.time_elapsed = 0.0f;
		Start_fade_up_anim = 0;
		Start_fade_down_anim = 1;
		Current_brief_stage = Quick_transition_stage;

		if ( Current_brief_stage < 0 ) {
			brief_transition_reset();
			Current_brief_stage = Last_brief_stage;
		}
		goto Fade_down_anim_start;
#else
		int framenum;

		Fade_anim.time_elapsed += frametime;

		if ( !Brief_playing_fade_sound ) {
			gamesnd_play_iface(SND_BRIEFING_STATIC);					
			Brief_playing_fade_sound = 1;
		}

		if ( Fade_anim.time_elapsed > Fade_anim.total_time ) {
			Fade_anim.time_elapsed = 0.0f;
			Start_fade_up_anim = 0;
			Start_fade_down_anim = 1;
			Current_brief_stage = Quick_transition_stage;
		
			if ( Current_brief_stage < 0 ) {
				brief_transition_reset();
				Current_brief_stage = Last_brief_stage;
			}

			Assert(Current_brief_stage >= 0);			
			goto Fade_down_anim_start;
		}

		// draw the correct frame of animation
		framenum = fl2i( (Fade_anim.time_elapsed * Fade_anim.num_frames) / Fade_anim.total_time );
		if ( framenum < 0 )
			framenum = 0;
		if ( framenum >= Fade_anim.num_frames )
			framenum = Fade_anim.num_frames-1;

		// Blit the bitmap for this frame
		gr_set_bitmap(Fade_anim.first_frame + framenum, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(Fade_anim.sx, Fade_anim.sy);
#endif
	}


	Fade_down_anim_start:
	if ( Start_fade_down_anim ) {

#if defined(FS2_DEMO) || defined(FS1_DEMO)
		Fade_anim.time_elapsed = 0.0f;
		Start_fade_up_anim = 0;
		Start_fade_down_anim = 0;
		return;
#else

		int framenum;

		Fade_anim.time_elapsed += frametime;

		if ( Fade_anim.time_elapsed > Fade_anim.total_time ) {
			Fade_anim.time_elapsed = 0.0f;
			Start_fade_up_anim = 0;
			Start_fade_down_anim = 0;
			return;
		}

		// draw the correct frame of animation
		framenum = fl2i( (Fade_anim.time_elapsed * Fade_anim.num_frames) / Fade_anim.total_time );
		if ( framenum < 0 )
			framenum = 0;
		if ( framenum >= Fade_anim.num_frames )
			framenum = Fade_anim.num_frames-1;

		// Blit the bitmap for this frame
		gr_set_bitmap(Fade_anim.first_frame + (Fade_anim.num_frames-1) - framenum, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(Fade_anim.sx, Fade_anim.sy);

#endif
	}
}

void brief_transition_reset()
{
	Quick_transition_stage = -1;
	Start_fade_up_anim = 0;
	Start_fade_down_anim = 0;
	Fade_anim.time_elapsed = 0.0f;
}

// return 1 if this mission only allow players to use the briefing (and not ship or 
// weapon loadout).  Otherwise return 0.
int brief_only_allow_briefing()
{
	if ( The_mission.game_type & MISSION_TYPE_TRAINING ) {
		return 1;
	}

	if ( The_mission.scramble || The_mission.red_alert) {
		return 1;
	}

	return 0;
}
