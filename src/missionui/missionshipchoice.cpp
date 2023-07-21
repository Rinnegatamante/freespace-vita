/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MissionUI/MissionShipChoice.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module to allow player ship selection for the mission
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
 * Revision 1.4  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.3  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.2  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 26    11/02/99 3:23p Jefff
 * "x Meters" to "x Meter" in German
 * 
 * 25    8/05/99 3:40p Jefff
 * hi-res text adjustments
 * 
 * 24    8/04/99 11:56a Jefff
 * fixed gamma wing hi-res coordinate error.  fixed
 * ss_load_individual_animation to load anis from a packfile.
 * 
 * 23    7/30/99 4:22p Andsager
 * restored ship and weapon anim sounds for demo.  Added sound for
 * changing ship in weapon loadout screen.
 * 
 * 22    7/28/99 12:23p Jefff
 * updated hi-res wing icon coords
 * 
 * 21    7/21/99 3:24p Andsager
 * Modify demo to use 2 frame ship and weapon select anis and cut sounds
 * associated with ani
 * 
 * 20    7/16/99 2:23p Anoop
 * Fixed build error.
 * 
 * 19    7/16/99 1:49p Dave
 * 8 bit aabitmaps. yay.
 * 
 * 18    7/15/99 6:36p Jamesa
 * Moved default ship name into the ships.tbl
 * 
 * 17    7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 16    6/29/99 7:39p Dave
 * Lots of small bug fixes.
 * 
 * 15    6/04/99 11:32a Dave
 * Added reset text to ship select screen. Fixed minor xstr bug in ui
 * window
 * 
 * 14    3/23/99 9:26p Neilk
 * fixed various multiplayer lock button problems
 * 
 * 13    3/23/99 11:55a Neilk
 * new support for ship anis
 * 
 * 14    3/15/99 6:27p Neilk
 * Added hires support for the ship animations
 * 
 * 13    3/15/99 11:18a Neilk
 * Modified animation code so ship animations loop back to frame 51
 * 
 * 12    3/12/99 12:02p Davidg
 * Modified coordinates for low res ship animations for new artwork
 * 
 * 11    3/10/99 6:21p Neilk
 * Added new artwork for hires
 * 
 * 10    2/21/99 6:01p Dave
 * Fixed standalone WSS packets. 
 * 
 * 9     2/18/99 11:46a Neilk
 * hires interface coord support
 * 
 * 8     2/11/99 3:08p Dave
 * PXO refresh button. Very preliminary squad war support.
 * 
 * 7     2/01/99 5:55p Dave
 * Removed the idea of explicit bitmaps for buttons. Fixed text
 * highlighting for disabled gadgets.
 * 
 * 6     1/29/99 4:17p Dave
 * New interface screens.
 * 
 * 5     12/18/98 1:13a Dave
 * Rough 1024x768 support for Direct3D. Proper detection and usage through
 * the launcher.
 * 
 * 4     11/30/98 1:07p Dave
 * 16 bit conversion, first run.
 * 
 * 3     10/13/98 9:28a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 102   9/11/98 4:14p Dave
 * Fixed file checksumming of < file_size. Put in more verbose kicking and
 * PXO stats store reporting.
 * 
 * 101   6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 100   6/01/98 11:43a John
 * JAS & MK:  Classified all strings for localization.
 * 
 * 99    5/23/98 5:50p Lawrance
 * Don't reset scroll offset when rebuilding the list
 * 
 * 98    5/06/98 11:50p Lawrance
 * Clean up help overlay code for loadout screens
 * 
 * 97    4/30/98 6:03p Lawrance
 * Make drag and drop work better.
 * 
 * 96    4/29/98 3:31p Lawrance
 * draw disabled frames for icons when appropriate
 * 
 * 95    4/28/98 9:35a Dave
 * Remove bogus assert in create_wings() for ships which arrive late and
 * haven't been created yet.
 * 
 * 94    4/27/98 6:02p Dave
 * Modify how missile scoring works. Fixed a team select ui bug. Speed up
 * multi_lag system. Put in new main hall.
 * 
 * 93    4/14/98 12:57a Dave
 * Made weapon select screen show netplayer names above ships. Fixed pilot
 * info popup to show the status of pilot images more correctly.
 * 
 * 92    4/13/98 3:27p Lawrance
 * fix coords for ship selection pool numbers
 * 
 * 91    4/13/98 3:11p Andsager
 * Fixed bug when there is no description for a ship.
 * 
 * 90    4/10/98 4:51p Hoffoss
 * Made several changes related to tooltips.
 * 
 * 89    4/05/98 7:43p Lawrance
 * fix up saving/restoring of link status and auto-target/match-speed.
 * 
 * 88    4/03/98 4:16p Adam
 * changed coord's and skip frame for new SS animations
 * 
 * 87    4/02/98 11:40a Lawrance
 * check for #ifdef DEMO instead of #ifdef DEMO_RELEASE
 * 
 * 86    4/01/98 11:19p Dave
 * Put in auto-loading of xferred pilot pic files. Grey out background
 * behind pinfo popup. Put a chatbox message in when players are kicked.
 * Moved mission title down in briefing. Other ui fixes.
 * 
 * 85    4/01/98 9:21p John
 * Made NDEBUG, optimized build with no warnings or errors.
 * 
 * 84    3/31/98 11:47p Lawrance
 * Fix some bugs related to wingmen selection when doing a quick mission
 * restart.
 * 
 * 83    3/31/98 1:50p Duncan
 * ALAN: fix bugs with selecting alternate weapons 
 * 
 * 82    3/30/98 12:18a Lawrance
 * change some DEMO_RELEASE code to not compile code rather than return
 * early
 * 
 * 81    3/29/98 12:55a Lawrance
 * Get demo build working with limited set of data.
 * 
 * 80    3/26/98 6:01p Dave
 * Put in file checksumming routine in cfile. Made pilot pic xferring more
 * robust. Cut header size of voice data packets in half. Put in
 * restricted game host query system.
 * 
 * 79    3/25/98 8:43p Hoffoss
 * Changed anim_play() to not be so damn complex when you try and call it.
 * 
 * 78    3/12/98 4:03p Lawrance
 * don't press buttons when icon dropped on them
 * 
 * 77    3/09/98 11:13a Lawrance
 * Fix up drop sound effects used in loadout screens.
 * 
 * 76    3/06/98 5:36p Dave
 * Finished up first rev of team vs team. Probably needs to be debugged
 * thoroughly.
 * 
 * 75    3/05/98 6:48p Lawrance
 * reposition commit_pressed() to ensure popup gets called after clear but
 * before flip
 * 
 * 74    3/05/98 5:03p Dave
 * More work on team vs. team support for multiplayer. Need to fix bugs in
 * weapon select.
 * 
 * 73    3/05/98 12:38a Lawrance
 * Fix bug with flashing buttons showing over help overlay
 * 
 * 72    3/02/98 5:42p John
 * Removed WinAVI stuff from Freespace.  Made all HUD gauges wriggle from
 * afterburner.  Made gr_set_clip work good with negative x &y.  Made
 * model_caching be on by default.  Made each cached model have it's own
 * bitmap id.  Made asteroids not rotate when model_caching is on.  
 * 
 * 71    3/01/98 3:26p Dave
 * Fixed a few team select bugs. Put in multiplayer intertface sounds.
 * Corrected how ships are disabled/enabled in team select/weapon select
 * screens.
 * 
 * 70    2/28/98 7:04p Lawrance
 * Don't show reset button in multiplayer
 * 
 * 69    2/26/98 8:21p Allender
 * fix compiler warning
 * 
 * 68    2/26/98 4:59p Allender
 * groundwork for team vs team briefings.  Moved weaponry pool into the
 * Team_data structure.  Added team field into the p_info structure.
 * Allow for mutliple structures in the briefing code.
 * 
 * 67    2/24/98 6:21p Lawrance
 * Integrate new reset button into loadout screens
 * 
 * 66    2/22/98 4:30p John
 * More string externalization classification
 * 
 * 65    2/22/98 4:17p John
 * More string externalization classification... 190 left to go!
 * 
 * 64    2/22/98 12:19p John
 * Externalized some strings
 * 
 * 63    2/19/98 6:26p Dave
 * Fixed a few file xfer bugs. Tweaked mp team select screen. Put in
 * initial support for player data uploading.
 * 
 * 62    2/18/98 3:56p Dave
 * Several bugs fixed for mp team select screen. Put in standalone packet
 * routing for team select.
 * 
 * 61    2/17/98 6:07p Dave
 * Tore out old multiplayer team select screen, installed new one.
 * 
 * 60    2/13/98 3:46p Dave
 * Put in dynamic chatbox sizing. Made multiplayer file lookups use cfile
 * functions.
 * 
 * 59    2/12/98 2:38p Allender
 * fix multiplayer primary/secondary weapon problems when ships are
 * outfitted with less than max number
 * 
 * 58    2/07/98 5:47p Lawrance
 * reset flashing if a button gets highlighted
 * 
 * 57    2/05/98 11:21p Lawrance
 * When flashing buttons, use highlight frame
 * 
 * 56    1/30/98 10:00a Allender
 * made large ships able to attack other ships.  Made goal code recognize
 * when ships removed from wings during ship select
 * 
 * 55    1/22/98 5:26p Dave
 * Modified some pregame sequencing packets. Starting to repair broken
 * standalone stuff.
 * 
 * 54    1/17/98 2:46a Dave
 * Reworked multiplayer join/accept process. Ingame join still needs to be
 * integrated.
 * 
 * 53    1/15/98 4:11p Lawrance
 * Add call to check if slot is player occupied.
 * 
 * 52    1/13/98 4:47p Allender
 * change default terran ship to reflect new ship class name
 * 
 * 51    1/12/98 5:17p Dave
 * Put in a bunch of multiplayer sequencing code. Made weapon/ship select
 * work through the standalone.
 * 
 * 50    1/10/98 12:46a Lawrance
 * Store last_modified time for mission into player loadout struct.
 * 
 * 49    1/09/98 6:06p Dave
 * Put in network sound support for multiplayer ship/weapon select
 * screens. Made clients exit game correctly through warp effect. Fixed
 * main hall menu help overlay bug.
 * 
 * 48    1/08/98 11:38a Lawrance
 * correct typo
 * 
 * 47    1/08/98 11:36a Lawrance
 * Get ship select and weapons loadout icon dropping sound effects working
 * for single and multiplayer
 * 
 * 46    1/02/98 9:10p Lawrance
 * Big changes to how colors get set on the HUD.
 * 
 * 45    12/29/97 4:21p Lawrance
 * Flash buttons on briefing/ship select/weapons loadout when enough time
 * has elapsed without activity.
 * 
 * 44    12/29/97 9:42a Lawrance
 * Ensure that WING_SLOT_IS_PLAYER gets set correctly in multiplayer.
 * 
 * 43    12/24/97 8:54p Lawrance
 * Integrating new popup code
 * 
 * 42    12/24/97 1:19p Lawrance
 * fix some bugs with the multiplayer ship/weapons loadout
 * 
 * 41    12/23/97 5:25p Allender
 * more fixes to multiplayer ship selection.  Fixed strange reentrant
 * problem with cf_callback when loading freespace data
 * 
 * 40    12/23/97 11:59a Allender
 * changes to ship/wespon selection for multplayer.  added sounds to some
 * interface screens.  Update and modiied end-of-briefing packets -- yet
 * to be tested.
 * 
 * 39    12/23/97 11:04a Lawrance
 * fix bug in ss_swap_slot_slot()
 * 
 * 38    12/23/97 10:57a Lawrance
 * move player_set_weapon_prefs() to when commit is pressed in briefing
 * (from entering gameplay)
 * 
 * 37    12/23/97 10:54a Lawrance
 * fix some bugs in multiplayer ship selection
 * 
 * 36    12/22/97 6:18p Lawrance
 * Get save/restore of player loadout working with new code
 * 
 * 35    12/22/97 1:40a Lawrance
 * Re-write ship select/weapons loadout to be multiplayer friendly
 * 
 * 34    12/19/97 1:23p Dave
 * Put in multiplayer groundwork for new weapon/ship select screens.
 * 
 * $NoKeywords: $
 *
*/

#include "missionscreencommon.h"
#include "missionshipchoice.h"
#include "missionparse.h"
#include "missionbrief.h"
#include "freespace.h"
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
#include "weapon.h"
#include "ui.h"
#include "ailocal.h"
#include "player.h"
#include "audiostr.h"
#include "bmpman.h"
#include "palman.h"
#include "snazzyui.h"
#include "animplay.h"
#include "packunpack.h"
#include "missionweaponchoice.h"
#include "contexthelp.h"
#include "gamesnd.h"
#include "sound.h"
#include "missionhotkey.h"
#include "multi.h"
#include "multimsgs.h"
#include "missionload.h"
#include "eventmusic.h"
#include "chatbox.h"
#include "popup.h"
#include "multiui.h"
#include "multiteamselect.h"
#include "multiutil.h"
#include "hudwingmanstatus.h"
#include "alphacolors.h"
#include "localize.h"

//////////////////////////////////////////////////////
// Game-wide Globals
//////////////////////////////////////////////////////
char default_player_ship[255] = NOX("GTF Ulysses");
int Select_default_ship = 0;
int Ship_select_open = 0;	// This game-wide global flag is set to 1 to indicate that the ship
									// select screen has been opened and memory allocated.  This flag
									// is needed so we can know if ship_select_close() needs to called if
									// restoring a game from the Options screen invoked from ship select

int Commit_pressed;	// flag to indicate that the commit button was pressed
							// use a flag, so the ship_create() can be done at the end of the loop

//////////////////////////////////////////////////////
// Module Globals
//////////////////////////////////////////////////////
static int Ship_anim_class = -1;		// ship class that is playing as an animation
static int Ss_delta_x, Ss_delta_y;	// used to offset the carried icon to make it smoothly leave static position

//////////////////////////////////////////////////////
// UI Data structs
//////////////////////////////////////////////////////
typedef struct ss_icon_info
{
	int				icon_bmaps[NUM_ICON_FRAMES];
	int				current_icon_bitmap;
	anim_t				*anim;
	anim_instance_t	*anim_instance;
} ss_icon_info;

typedef struct ss_slot_info
{
	int status;			// slot status (WING_SLOT_DISABLED, etc)
	int sa_index;		// index into ship arrival list, -1 if ship is created
	int original_ship_class;
} ss_slot_info;

typedef struct ss_wing_info
{
	int num_slots;
	int wingnum;
	int is_late;
	ss_slot_info ss_slots[MAX_WING_SLOTS];
} ss_wing_info;

//ss_icon_info	Ss_icons[MAX_SHIP_TYPES];		// holds ui info on different ship icons
//ss_wing_info	Ss_wings[MAX_WING_BLOCKS];		// holds ui info for wings and wing slots

ss_wing_info	Ss_wings_teams[MAX_TEAMS][MAX_WING_BLOCKS];
ss_wing_info	*Ss_wings;

ss_icon_info	Ss_icons_teams[MAX_TEAMS][MAX_SHIP_TYPES];
ss_icon_info	*Ss_icons;

int Ss_mouse_down_on_region = -1;

int Selected_ss_class;	// set to ship class of selected ship, -1 if none selected
int Hot_ss_icon;			// index that icon is over in list (0..4)
int Hot_ss_slot;			// index for slot that mouse is over (0..11)

////////////////////////////////////////////////////////////
// Ship Select UI
////////////////////////////////////////////////////////////
UI_WINDOW	Ship_select_ui_window;	

static int Ship_anim_coords[GR_NUM_RESOLUTIONS][2] = {
	{
#ifdef MAKE_FS1
		10, 77
#else
		257, 84		// GR_640
#endif
	},
	{
		412, 135	// GR_1024
	}
};

#ifndef MAKE_FS1
static int Ship_info_coords[GR_NUM_RESOLUTIONS][2] = {
	{
		28, 78				// GR_640
	},
	{
		45, 125				// GR_1024
	}
};
#endif

// coordinate lookup indicies
#define SHIP_SELECT_X_COORD 0
#define SHIP_SELECT_Y_COORD 1
#define SHIP_SELECT_W_COORD 2
#define SHIP_SELECT_H_COORD 3


// NK: changed from 37 to 51 for new FS2 animations
#if defined(FS2_DEMO) || defined(FS1_DEMO)
#define SHIP_ANIM_LOOP_FRAME	0
#elif MAKE_FS1
#define SHIP_ANIM_LOOP_FRAME	36
#else
#define SHIP_ANIM_LOOP_FRAME	51
#endif

#define MAX_ICONS_ON_SCREEN	4

// (x,y) pairs for ship icon and ship icon number
int Ship_list_coords[GR_NUM_RESOLUTIONS][MAX_ICONS_ON_SCREEN][4] = {
	{
		{23,331,4,341},
		{23,361,4,371},
		{23,391,4,401},
		{23,421,4,431}
	},
	{
		{29,530,10,540},
		{29,578,10,588},
		{29,626,10,636},
		{29,674,10,684}
	}
};

// Store the x locations for the icons in the wing formations
int Wing_icon_coords[GR_NUM_RESOLUTIONS][MAX_WSS_SLOTS][2] = {
	{
		{124,345},
		{100,376},
		{148,376},
		{124,407},

		{222,345},
		{198,376},
		{246,376},
		{222,407},

		{320,345},
		{296,376},
		{344,376},
		{320,407}
	},
	{
		{218,584},
		{194,615},
		{242,615},
		{218,646},

		{373,584},
		{349,615},
		{397,615},
		{373,646},

		{531,584},
		{507,615},
		{555,615},
		{531,646}
	}
};

//////////////////////////////////////////////////////
// Linked List of icons to show on ship selection list
//////////////////////////////////////////////////////
#define SS_ACTIVE_ITEM_USED	(1<<0)
typedef struct ss_active_item
{
	ss_active_item	*prev, *next;
	int				ship_class;
	int				flags;
} ss_active_item;

static ss_active_item	SS_active_head;
static ss_active_item	SS_active_items[MAX_WSS_SLOTS];

static int SS_active_list_start;
static int SS_active_list_size;

//////////////////////////////////////////////////////
// Background bitmaps data for ship_select
//////////////////////////////////////////////////////
static const char* Ship_select_background_fname[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	"Brief",
	"Brief"
#else
	"ShipSelect",
	"2_ShipSelect"
#endif
};

static const char* Ship_select_background_mask_fname[GR_NUM_RESOLUTIONS] = {
	"ShipSelect-m",
	"2_ShipSelect-m"
};

int Ship_select_background_bitmap;

//////////////////////////////////////////////////////
// Ship select specific buttons
//////////////////////////////////////////////////////
#define NUM_SS_BUTTONS				4
#define SS_BUTTON_SCROLL_UP		0
#define SS_BUTTON_SCROLL_DOWN		1
#define SS_BUTTON_RESET				2
#define SS_BUTTON_DUMMY				3	// needed to capture mouse for drag/drop icons

// convenient struct for handling all button controls
struct ss_buttons {
	const char *filename;
	int x, y, xt, yt;
	int hotspot;
	int scrollable;
	UI_BUTTON button;  // because we have a class inside this struct, we need the constructor below..

	ss_buttons(const char *name, int x1, int y1, int xt1, int yt1, int h, int s) : filename(name), x(x1), y(y1), xt(xt1), yt(yt1), hotspot(h), scrollable(s) {}
};

static ss_buttons Ship_select_buttons[GR_NUM_RESOLUTIONS][NUM_SS_BUTTONS] = {
	{	// GR_640
#ifdef MAKE_FS1
		ss_buttons("ssb_08",	0,		301,	-1,	-1,	8,	0),	// SCROLL UP
		ss_buttons("ssb_09",	0,		453,	-1,	-1,	9,	0),	// SCROLL DOWN
		ss_buttons("ssb_39",	566,	317,	-1,	-1,	39,	0),	// RESET
		ss_buttons("ssb_39",	0,		0,		-1,	-1,	99,	0)	// dummy for drag n' drop
#else
		ss_buttons("ssb_08",		5,			303,	-1,	-1,	8,	0),		// SCROLL UP
		ss_buttons("ssb_09",		5,			454,	-1,	-1,	9,	0),		// SCROLL DOWN
		ss_buttons("ssb_39",		571,		347,	-1,	-1,	39,0),		// RESET
		ss_buttons("ssb_39",		0,			0,		-1,	-1,	99,0)			// dummy for drag n' drop
#endif
	},
	{	// GR_1024
		ss_buttons("2_ssb_08",	8,			485,	-1,	-1,	8,	0),		// SCROLL UP
		ss_buttons("2_ssb_09",	8,			727,	-1,	-1,	9,	0),		// SCROLL DOWN
		ss_buttons("2_ssb_39",	913,		556,	-1,	-1,	39,0),		// RESET
		ss_buttons("2_ssb_39",	0,			0,		-1,	-1,	99,0)			// dummy for drag n' drop
	}
};

// ship select text
#ifndef MAKE_FS1
#define SHIP_SELECT_NUM_TEXT			1

UI_XSTR Ship_select_text[GR_NUM_RESOLUTIONS][SHIP_SELECT_NUM_TEXT] = {
	{ // GR_640
		{ "Reset",			1337,		580,	337,	UI_XSTR_COLOR_GREEN, -1, &Ship_select_buttons[0][SS_BUTTON_RESET].button }
	}, 
	{ // GR_1024
		{ "Reset",			1337,		938,	546,	UI_XSTR_COLOR_GREEN, -1, &Ship_select_buttons[1][SS_BUTTON_RESET].button }
	}
};
#endif

// Mask bitmap pointer and Mask bitmap_id
static bitmap*	ShipSelectMaskPtr;		// bitmap pointer to the ship select mask bitmap
static ubyte*	ShipSelectMaskData;		// pointer to actual bitmap data
static int		Shipselect_mask_w, Shipselect_mask_h;
static int		ShipSelectMaskBitmap;	// bitmap id of the ship select mask bitmap

static MENU_REGION	Region[NUM_SHIP_SELECT_REGIONS];
static int				Num_mask_regions;

//////////////////////////////////////////////////////
// Drag and Drop variables
//////////////////////////////////////////////////////
typedef struct ss_carry_icon_info
{
	int from_slot;		// slot index (0..11), -1 if carried from list
	int ship_class;	// ship class of carried icon 
	int from_x, from_y;
} ss_carry_icon_info;

ss_carry_icon_info Carried_ss_icon;

////////////////////////////////////////////////////////////////////
// Internal function prototypes
////////////////////////////////////////////////////////////////////

// render functions
void draw_ship_icons();
void draw_ship_icon_with_number(int screen_offset, int ship_class);
void stop_ship_animation();
void start_ship_animation(int ship_class, int play_sound=0);

// pick-up 
int pick_from_ship_list(int screen_offset, int ship_class);
void pick_from_wing(int wb_num, int ws_num);

// ui related
void ship_select_button_do(int i);
void ship_select_common_init();
void ss_reset_selected_ship();
void ss_restore_loadout();
void maybe_change_selected_wing_ship(int wb_num, int ws_num);

// init functions
void ss_init_pool(team_data *pteam);
int create_wings();

// loading/unloading
void ss_unload_icons();
void ss_init_units();
void unload_ship_anim_instances();
void unload_ship_anims();
anim* ss_load_individual_animation(int ship_class);

// Carry icon functions
int	ss_icon_being_carried();
void	ss_reset_carried_icon();
void	ss_set_carried_icon(int from_slot, int ship_class);

#define SHIP_DESC_X	445
#define SHIP_DESC_Y	273

const char *ss_tooltip_handler(const char *str)
{
	if (Selected_ss_class < 0)
		return NULL;

	if (!stricmp(str, NOX("@ship_name"))) {
		return Ship_info[Selected_ss_class].name;

	} else if (!stricmp(str, NOX("@ship_type"))) {
		return Ship_info[Selected_ss_class].type_str;

	} else if (!stricmp(str, NOX("@ship_maneuverability"))) {
		return Ship_info[Selected_ss_class].maneuverability_str;

	} else if (!stricmp(str, NOX("@ship_armor"))) {
		return Ship_info[Selected_ss_class].armor_str;

	} else if (!stricmp(str, NOX("@ship_manufacturer"))) {
		return Ship_info[Selected_ss_class].manufacturer_str;

	} else if (!stricmp(str, NOX("@ship_desc"))) {
		char *str;
		int x, y, w, h;

		str = Ship_info[Selected_ss_class].desc;
		if (!str)
			return NULL;

		gr_get_string_size(&w, &h, str);
		x = SHIP_DESC_X - w / 2;
		y = SHIP_DESC_Y - h / 2;

		gr_set_color_fast(&Color_black);
		gr_rect(x - 5, y - 5, w + 10, h + 10);

		gr_set_color_fast(&Color_bright_white);
		gr_string(x, y, str);
		return NULL;
	}

	return NULL;
}

// Is an icon being carried?
int ss_icon_being_carried()
{
	if ( Carried_ss_icon.ship_class >= 0 ) {
		return 1;
	}

	return 0;
}

// Clear out carried icon info
void ss_reset_carried_icon()
{
	Carried_ss_icon.from_slot = -1;
	Carried_ss_icon.ship_class = -1;
}

// return !0 if carried icon has moved from where it was picked up
int ss_carried_icon_moved()
{
	int mx, my;

	mouse_get_pos( &mx, &my );
	if ( Carried_ss_icon.from_x != mx || Carried_ss_icon.from_y != my) {
		return 1;
	}

	return 0;
}

// Set carried icon data 
void ss_set_carried_icon(int from_slot, int ship_class)
{
	Carried_ss_icon.from_slot = from_slot;
	Carried_ss_icon.ship_class = ship_class;

	// Set the mouse to captured
	Ship_select_buttons[gr_screen.res][SS_BUTTON_DUMMY].button.capture_mouse();
}

// clear all active list items, and reset the flags inside the SS_active_items[] array
void clear_active_list()
{
	int i;
	for ( i = 0; i < MAX_WSS_SLOTS; i++ ) {
		SS_active_items[i].flags = 0;
		SS_active_items[i].ship_class = -1;
	}
	list_init(&SS_active_head);

	SS_active_list_start = 0;
	SS_active_list_size = 0;
}

// get a free element from SS_active_items[]
ss_active_item *get_free_active_list_node()
{
	int i;
	for ( i = 0; i < MAX_WSS_SLOTS; i++ ) {
		if ( SS_active_items[i].flags == 0 ) {
			SS_active_items[i].flags |= SS_ACTIVE_ITEM_USED;
			return &SS_active_items[i];
		}
	}
	return NULL;
}

// add a ship into the active list
void active_list_add(int ship_class)
{
	ss_active_item *sai;

	sai = get_free_active_list_node();
	Assert(sai != NULL);
	sai->ship_class = ship_class;
	list_append(&SS_active_head, sai);
}

// remove a ship from the active list
void active_list_remove(int ship_class)
{
	ss_active_item *sai, *temp;
	
	// next store players not assigned to wings
	sai = GET_FIRST(&SS_active_head);

	while(sai != END_OF_LIST(&SS_active_head)){
		temp = GET_NEXT(sai);
		if ( sai->ship_class == ship_class ) {
			list_remove(&SS_active_head, sai);
			sai->flags = 0;
		}
		sai = temp;
	}
}
	
// Build up the ship selection active list, which is a list of all ships that the player
// can choose from.
void init_active_list()
{
	int i;
	ss_active_item	*sai;

	clear_active_list();

	// build the active list
	for ( i = 0; i < MAX_SHIP_TYPES; i++ ) {
		if ( Ss_pool[i] > 0 ) {
			sai = get_free_active_list_node();
			if ( sai != NULL ) {
				sai->ship_class = i;
				list_append(&SS_active_head, sai);
				SS_active_list_size++;
			}
		}
	}
}

void ship_select_check_buttons()
{
	int			i;
	ss_buttons	*b;

	for ( i = 0; i < NUM_SS_BUTTONS; i++ ) {
		b = &Ship_select_buttons[gr_screen.res][i];
		if ( b->button.pressed() ) {
			ship_select_button_do(b->hotspot);
		}
	}
}

// reset the ship selection to the mission defaults
void ss_reset_to_default()
{
	if ( Game_mode & GM_MULTIPLAYER ) {
		Int3();
		return;
	}

	stop_ship_animation();

	ss_init_pool(&Team_data[Common_team]);
	ss_init_units();
	init_active_list();
	ss_reset_selected_ship();
	ss_reset_carried_icon();

	// reset weapons 
	wl_reset_to_defaults();

	start_ship_animation(Selected_ss_class, 1);
}

// -------------------------------------------------------------------
// ship_select_redraw_pressed_buttons()
//
// Redraw any ship select buttons that are pressed down.  This function is needed
// since we sometimes need to draw pressed buttons last to ensure the entire
// button gets drawn (and not overlapped by other buttons)
//
void ship_select_redraw_pressed_buttons()
{
	int			i;
	ss_buttons	*b;
	
	common_redraw_pressed_buttons();

	for ( i = 0; i < NUM_SS_BUTTONS; i++ ) {
		b = &Ship_select_buttons[gr_screen.res][i];
		if ( b->button.pressed() ) {
			b->button.draw_forced(2);
		}
	}
}

void ship_select_buttons_init()
{
	ss_buttons	*b;
	int			i;

	for ( i = 0; i < NUM_SS_BUTTONS; i++ ) {
		b = &Ship_select_buttons[gr_screen.res][i];
		b->button.create( &Ship_select_ui_window, "", b->x, b->y, 60, 30, b->scrollable);
		// set up callback for when a mouse first goes over a button
		b->button.set_highlight_action( common_play_highlight_sound );
		b->button.set_bmaps(b->filename);
		b->button.link_hotspot(b->hotspot);
	}

#ifndef MAKE_FS1
	// add all xstrs
	for(i=0; i<SHIP_SELECT_NUM_TEXT; i++){
		Ship_select_ui_window.add_XSTR(&Ship_select_text[gr_screen.res][i]);
	}
#endif

	// We don't want to have the reset button appear in multiplayer
	if ( Game_mode & GM_MULTIPLAYER ) {
		Ship_select_buttons[gr_screen.res][SS_BUTTON_RESET].button.disable();
		Ship_select_buttons[gr_screen.res][SS_BUTTON_RESET].button.hide();
	}

	Ship_select_buttons[gr_screen.res][SS_BUTTON_DUMMY].button.disable();
	Ship_select_buttons[gr_screen.res][SS_BUTTON_DUMMY].button.hide();
}

// -------------------------------------------------------------------------------------
// ship_select_button_do() do the button action for the specified pressed button
//
void ship_select_button_do(int i)
{
	if ( Background_playing )
		return;

	switch ( i ) {
		case SHIP_SELECT_SHIP_SCROLL_UP:
			if ( Current_screen != ON_SHIP_SELECT )
				break;

			if ( common_scroll_down_pressed(&SS_active_list_start, SS_active_list_size, MAX_ICONS_ON_SCREEN) ) {
				gamesnd_play_iface(SND_SCROLL);
			} else {
				gamesnd_play_iface(SND_GENERAL_FAIL);
			}
			break;

		case SHIP_SELECT_SHIP_SCROLL_DOWN:
			if ( Current_screen != ON_SHIP_SELECT )
				break;

			if ( common_scroll_up_pressed(&SS_active_list_start, SS_active_list_size, MAX_ICONS_ON_SCREEN) ) {
				gamesnd_play_iface(SND_SCROLL);
			} else {
				gamesnd_play_iface(SND_GENERAL_FAIL);
			}

			break;

		case SHIP_SELECT_RESET:
			ss_reset_to_default();
			break;
	} // end switch
}

// ---------------------------------------------------------------------
// ship_select_init() is called once when the ship select screen begins
//
//
void ship_select_init()
{
	common_set_interface_palette("ShipPalette");
	common_flash_button_init();

	// if in multiplayer -- set my state to be ship select
	if ( Game_mode & GM_MULTIPLAYER ){		
		// also set the ship which is mine as the default
		maybe_change_selected_wing_ship(Net_player->p_info.ship_index/4,Net_player->p_info.ship_index % 4);
	}

	set_active_ui(&Ship_select_ui_window);
	Current_screen = ON_SHIP_SELECT;

	Ss_mouse_down_on_region = -1;

	help_overlay_set_state(SS_OVERLAY,0);

	if ( Ship_select_open ) {
		start_ship_animation( Selected_ss_class );
		common_buttons_maybe_reload(&Ship_select_ui_window);	// AL 11-21-97: this is necessary since we may returning from the hotkey
																				// screen, which can release common button bitmaps.
		common_reset_buttons();
		nprintf(("Alan","ship_select_init() returning without doing anything\n"));
		return;
	}

	nprintf(("Alan","entering ship_select_init()\n"));
	common_select_init();

	ShipSelectMaskBitmap = bm_load(Ship_select_background_mask_fname[gr_screen.res]);
	if (ShipSelectMaskBitmap < 0) {
		if (gr_screen.res == GR_640) {
			Error(LOCATION,"Could not load in 'shipselect-m'!");
		} else if (gr_screen.res == GR_1024) {
			Error(LOCATION,"Could not load in '2_shipselect-m'!");
		}
	}

	Shipselect_mask_w = -1;
	Shipselect_mask_h = -1;

	// get a pointer to bitmap by using bm_lock()
	ShipSelectMaskPtr = bm_lock(ShipSelectMaskBitmap, 8, BMP_AABITMAP);
	ShipSelectMaskData = (ubyte*)ShipSelectMaskPtr->data;	
	bm_get_info(ShipSelectMaskBitmap, &Shipselect_mask_w, &Shipselect_mask_h);

	help_overlay_load(SS_OVERLAY);

	// Set up the mask regions
   // initialize the different regions of the menu that will react when the mouse moves over it
	Num_mask_regions = 0;
	
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	COMMON_BRIEFING_REGION,				0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	COMMON_SS_REGION,						0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	COMMON_WEAPON_REGION,				0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	COMMON_COMMIT_REGION,				0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	COMMON_HELP_REGION,					0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	COMMON_OPTIONS_REGION,				0);

	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	SHIP_SELECT_SHIP_SCROLL_UP,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	SHIP_SELECT_SHIP_SCROLL_DOWN,		0);

	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	SHIP_SELECT_ICON_0,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	SHIP_SELECT_ICON_1,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	SHIP_SELECT_ICON_2,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	SHIP_SELECT_ICON_3,		0);

	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_0_SHIP_0,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_0_SHIP_1,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_0_SHIP_2,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_0_SHIP_3,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_1_SHIP_0,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_1_SHIP_1,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_1_SHIP_2,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_1_SHIP_3,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_2_SHIP_0,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_2_SHIP_1,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_2_SHIP_2,		0);
	snazzy_menu_add_region(&Region[Num_mask_regions++], "",	WING_2_SHIP_3,		0);

	Ship_select_open = 1;	// This game-wide global flag is set to 1 to indicate that the ship
									// select screen has been opened and memory allocated.  This flag
									// is needed so we can know if ship_select_close() needs to called if
									// restoring a game from the Options screen invoked from ship select

	// init ship selection masks and buttons
	Ship_select_ui_window.create( 0, 0, gr_screen.max_w, gr_screen.max_h, 0 );
	Ship_select_ui_window.set_mask_bmap(Ship_select_background_mask_fname[gr_screen.res]);
	Ship_select_ui_window.tooltip_handler = ss_tooltip_handler;
	common_buttons_init(&Ship_select_ui_window);
	ship_select_buttons_init();
	start_ship_animation( Selected_ss_class );	

	// init ship selection background bitmpa
	Ship_select_background_bitmap = bm_load(Ship_select_background_fname[gr_screen.res]);
}


// Return the ship class for the icon specified by index.  Need to iterate through the active
// list of icons to find out what ship class for this icon
//
// input: index => list index (0..3)
// exit:  ship class, -1 if none
//
int ss_get_ship_class_from_list(int index)
{
	ss_active_item	*sai;
	int				list_entry, i, count;

	i = 0;
	count = 0;
	list_entry = -1;
	for ( sai = GET_FIRST(&SS_active_head); sai != END_OF_LIST(&SS_active_head); sai = GET_NEXT(sai) ) {
		count++;
		if ( count <= SS_active_list_start )
			continue;

		if ( i >= MAX_ICONS_ON_SCREEN )
			break;

		if ( i == index ) {
			list_entry = sai->ship_class;
			break;
		}

		i++;
	}

	return list_entry;
}

// ---------------------------------------------------------------------
// maybe_pick_up_list_icon()
//
void maybe_pick_up_list_icon(int offset)
{
	int ship_class;

	ship_class = ss_get_ship_class_from_list(offset);
	if ( ship_class != -1 ) {
		pick_from_ship_list(offset, ship_class);
	}
}

// ---------------------------------------------------------------------
// maybe_change_selected_ship()
//
void maybe_change_selected_ship(int offset)
{
	int ship_class;

	ship_class = ss_get_ship_class_from_list(offset);
	if ( ship_class == -1 )
		return;

	if ( Ss_mouse_down_on_region != (SHIP_SELECT_ICON_0+offset) ) {
		return;
	}

	if ( Selected_ss_class == -1 ) {
		Selected_ss_class = ship_class;
		start_ship_animation(Selected_ss_class, 1);
	}
	else if ( Selected_ss_class != ship_class ) {
		Selected_ss_class = ship_class;
		start_ship_animation(Selected_ss_class, 1);
	}
	else
		Assert( Selected_ss_class == ship_class );
}

void maybe_change_selected_wing_ship(int wb_num, int ws_num)
{
	ss_slot_info	*ss_slot;

	Assert(wb_num >= 0 && wb_num < MAX_WING_BLOCKS);
	Assert(ws_num >= 0 && ws_num < MAX_WING_SLOTS);	
	
	if ( Ss_wings[wb_num].wingnum < 0 ) {
		return;
	}

	ss_slot = &Ss_wings[wb_num].ss_slots[ws_num];
	switch ( ss_slot->status & ~WING_SLOT_LOCKED ) {

		case WING_SLOT_FILLED:
		case WING_SLOT_FILLED|WING_SLOT_IS_PLAYER:
			if ( Selected_ss_class != -1 && Selected_ss_class != Wss_slots[wb_num*4+ws_num].ship_class ) {
				Selected_ss_class = Wss_slots[wb_num*4+ws_num].ship_class;
				start_ship_animation(Selected_ss_class, 1);
			}
			break;

		default:
			// do nothing
			break;
	} // end switch
}

// ---------------------------------------------------------------------
// do_mouse_over_wing_slot()
//
// returns:	0 => icon wasn't dropped onto slot
//				1 => icon was dropped onto slot
int do_mouse_over_wing_slot(int block, int slot)
{
	Hot_ss_slot = block*4 + slot;

	if ( !mouse_down(MOUSE_LEFT_BUTTON) ) {
		if ( ss_icon_being_carried() ) {

			if ( ss_disabled_slot(block*4+slot) ) {
				gamesnd_play_iface(SND_ICON_DROP);
				return 0;
			}

			if ( !ss_carried_icon_moved() ) {
				ss_reset_carried_icon();
				return 0;
			}

			ss_drop(Carried_ss_icon.from_slot, Carried_ss_icon.ship_class, Hot_ss_slot, -1);
			ss_reset_carried_icon();
		}
	}
	else {
		if ( Ss_mouse_down_on_region == (WING_0_SHIP_0+block*4+slot) ) {
			pick_from_wing(block, slot);
		}
	}

	return 1;
}

void do_mouse_over_list_slot(int index)
{
	Hot_ss_icon = index;

	if ( Ss_mouse_down_on_region != (SHIP_SELECT_ICON_0+index) ){
		return;
	}

	if ( mouse_down(MOUSE_LEFT_BUTTON) )
		maybe_pick_up_list_icon(index);
}

// Icon has been dropped, but not onto a wing slot
void ss_maybe_drop_icon()
{
	if ( Drop_icon_mflag )  {
		if ( ss_icon_being_carried() ) {
			// Add back into the ship entry list
			if ( Carried_ss_icon.from_slot >= 0 ) {
				// return to list
				ss_drop(Carried_ss_icon.from_slot, -1, -1, Carried_ss_icon.ship_class);
			} else {
				if ( ss_carried_icon_moved() ) {
					gamesnd_play_iface(SND_ICON_DROP);
				}
			}
			ss_reset_carried_icon();
		}	
	}
}

void ss_anim_pause()
{
	if ( Selected_ss_class >= 0 && Ss_icons[Selected_ss_class].anim_instance ) {
		anim_pause(Ss_icons[Selected_ss_class].anim_instance);
	}
}

void ss_anim_unpause()
{
	if ( Selected_ss_class >= 0 && Ss_icons[Selected_ss_class].anim_instance ) {
		anim_unpause(Ss_icons[Selected_ss_class].anim_instance);
	}
}

// maybe flash a button if player hasn't done anything for a while
void ss_maybe_flash_button()
{
	if ( common_flash_bright() ) {
		// weapon loadout button
		if ( Common_buttons[Current_screen-1][gr_screen.res][2].button.button_hilighted() ) {
			common_flash_button_init();
		} else {
			Common_buttons[Current_screen-1][gr_screen.res][2].button.draw_forced(1);
		}
	}
}


// -------------------------------------------------------------------------------------
// ship_select_render(float frametime)
//
void ship_select_render(float frametime)
{
	if ( !Background_playing ) {
		gr_set_bitmap(Ship_select_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	}

	anim_render_all(0, frametime);
	anim_render_all(ON_SHIP_SELECT, frametime);
}


// blit any active ship information text
void ship_select_blit_ship_info()
{
#ifndef MAKE_FS1
	int y_start;
	ship_info *sip;
	char str[100];
	color *header = &Color_white;
	color *text = &Color_green;


	// if we don't have a valid ship selected, do nothing
	if(Selected_ss_class == -1){
		return;
	}

	// get the ship class
	sip = &Ship_info[Selected_ss_class];

	// starting line
	y_start = Ship_info_coords[gr_screen.res][SHIP_SELECT_Y_COORD];

	memset(str,0,100);

	// blit the ship class (name)
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Class",739));
	y_start += 10;
	if(strlen(sip->name)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->name);
	}
	y_start += 10;

	// blit the ship type
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Type",740));
	y_start += 10;
	if((sip->type_str != NULL) && strlen(sip->type_str)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->type_str);
	}
	y_start+=10;

	// blit the ship length
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Length",741));
	y_start += 10;
	if((sip->ship_length != NULL) && strlen(sip->ship_length)){
		if (Lcl_gr) {
			// in german, drop the s from Meters and make sure M is caps
			char *sp = strstr(sip->ship_length, "Meters");
			if (sp) {
				sp[5] = ' ';		// make the old s a space now
			}
		}
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start, sip->ship_length);
	}
	y_start += 10;

	// blit the max velocity
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Max Velocity",742));	
	y_start += 10;
	sprintf(str,XSTR("%d m/s",743),(int)sip->max_vel.xyz.z);
	gr_set_color_fast(text);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,str);
	y_start += 10;

	// blit the maneuverability
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Maneuverability",744));
	y_start += 10;
	if((sip->maneuverability_str != NULL) && strlen(sip->maneuverability_str)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->maneuverability_str);
	}
	y_start += 10;

	// blit the armor
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Armor",745));
	y_start += 10;
	if((sip->armor_str != NULL) && strlen(sip->armor_str)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->armor_str);
	}
	y_start += 10;

	// blit the gun mounts 
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Gun Mounts",746));
	y_start += 10;
	if((sip->gun_mounts != NULL) && strlen(sip->gun_mounts)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->gun_mounts);
	}
	y_start += 10;

	// blit the missile banke
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Missile Banks",747));
	y_start += 10;
	if((sip->missile_banks != NULL) && strlen(sip->missile_banks)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->missile_banks);
	}
	y_start += 10;

	// blit the manufacturer
	gr_set_color_fast(header);
	gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start,XSTR("Manufacturer",748));
	y_start += 10;
	if((sip->manufacturer_str != NULL) && strlen(sip->manufacturer_str)){
		gr_set_color_fast(text);
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD]+4, y_start,sip->manufacturer_str);
	}
	y_start += 10;

	// blit the _short_ text description
	/*
	Assert(Multi_ts_ship_info_line_count < 3);
	gr_set_color_fast(&Color_normal);
	for(idx=0;idx<SHIP_SELECT_ship_info_line_count;idx++){
		gr_string(Ship_info_coords[gr_screen.res][SHIP_SELECT_X_COORD], y_start, SHIP_SELECT_ship_info_lines[idx]);
		y_start += 10;
	}
	*/
#endif
}


// ---------------------------------------------------------------------
// ship_select_do() is called once per game frame, and is responsible for
// updating the ship select screen
//
//	frametime is in seconds
void ship_select_do(float frametime)
{
	int k, ship_select_choice, snazzy_action;

	ship_select_choice = snazzy_menu_do(ShipSelectMaskData, Shipselect_mask_w, Shipselect_mask_h, Num_mask_regions, Region, &snazzy_action, 0);

	Hot_ss_icon = -1;
	Hot_ss_slot = -1;

	k = common_select_do(frametime);

	if ( help_overlay_active(SS_OVERLAY) ) {
		ss_anim_pause();
	}
	else {
		ss_anim_unpause();
	}

	// Check common keypresses
	common_check_keys(k);

	if ( Mouse_down_last_frame ) {
		Ss_mouse_down_on_region = ship_select_choice;
	}

	// Check for the mouse over a region (not clicked, just over)
	if ( ship_select_choice > -1 ) {

		switch(ship_select_choice) {
			case SHIP_SELECT_ICON_0:
				do_mouse_over_list_slot(0);
				break;
			case SHIP_SELECT_ICON_1:
				do_mouse_over_list_slot(1);
				break;
			case SHIP_SELECT_ICON_2:
				do_mouse_over_list_slot(2);
				break;
			case SHIP_SELECT_ICON_3:
				do_mouse_over_list_slot(3);
				break;
			case WING_0_SHIP_0:
				if ( do_mouse_over_wing_slot(0,0) )
					ship_select_choice = -1;
				break;
			case WING_0_SHIP_1:
				if ( do_mouse_over_wing_slot(0,1) )
					ship_select_choice = -1;
				break;
			case WING_0_SHIP_2:
				if ( do_mouse_over_wing_slot(0,2) )
					ship_select_choice = -1;
				break;
			case WING_0_SHIP_3:
				if ( do_mouse_over_wing_slot(0,3) )
					ship_select_choice = -1;
				break;
			case WING_1_SHIP_0:
				if ( do_mouse_over_wing_slot(1,0) )
					ship_select_choice = -1;
				break;
			case WING_1_SHIP_1:
				if ( do_mouse_over_wing_slot(1,1) )
					ship_select_choice = -1;
				break;
			case WING_1_SHIP_2:
				if ( do_mouse_over_wing_slot(1,2) )
					ship_select_choice = -1;
				break;
			case WING_1_SHIP_3:
				if ( do_mouse_over_wing_slot(1,3) )
					ship_select_choice = -1;
				break;
			case WING_2_SHIP_0:
				if ( do_mouse_over_wing_slot(2,0) )
					ship_select_choice = -1;
				break;
			case WING_2_SHIP_1:
				if ( do_mouse_over_wing_slot(2,1) )
					ship_select_choice = -1;
				break;
			case WING_2_SHIP_2:
				if ( do_mouse_over_wing_slot(2,2) )
					ship_select_choice = -1;
				break;
			case WING_2_SHIP_3:
				if ( do_mouse_over_wing_slot(2,3) )
					ship_select_choice = -1;
				break;

			default:
				break;
		}	// end switch
	}

	// check buttons
	common_check_buttons();
	ship_select_check_buttons();

	// Check for the mouse clicks over a region
	if ( ship_select_choice > -1 && snazzy_action == SNAZZY_CLICKED ) {
		switch (ship_select_choice) {

			case SHIP_SELECT_ICON_0:
				maybe_change_selected_ship(0);
				break;

			case SHIP_SELECT_ICON_1:
				maybe_change_selected_ship(1);
				break;

			case SHIP_SELECT_ICON_2:
				maybe_change_selected_ship(2);
				break;

			case SHIP_SELECT_ICON_3:
				maybe_change_selected_ship(3);
				break;

			case WING_0_SHIP_0:
				maybe_change_selected_wing_ship(0,0);
				break;

			case WING_0_SHIP_1:
				maybe_change_selected_wing_ship(0,1);
				break;

			case WING_0_SHIP_2:
				maybe_change_selected_wing_ship(0,2);
				break;

			case WING_0_SHIP_3:
				maybe_change_selected_wing_ship(0,3);
				break;

			case WING_1_SHIP_0:
				maybe_change_selected_wing_ship(1,0);
				break;

			case WING_1_SHIP_1:
				maybe_change_selected_wing_ship(1,1);
				break;

			case WING_1_SHIP_2:
				maybe_change_selected_wing_ship(1,2);
				break;

			case WING_1_SHIP_3:
				maybe_change_selected_wing_ship(1,3);
				break;

			case WING_2_SHIP_0:
				maybe_change_selected_wing_ship(2,0);
				break;

			case WING_2_SHIP_1:
				maybe_change_selected_wing_ship(2,1);
				break;

			case WING_2_SHIP_2:
				maybe_change_selected_wing_ship(2,2);
				break;

			case WING_2_SHIP_3:
				maybe_change_selected_wing_ship(2,3);
				break;

			default:
				break;

		}	// end switch
	}

	ss_maybe_drop_icon();

	if ( Ship_anim_class >= 0) {
		Assert(Selected_ss_class >= 0);
		if ( Ss_icons[Selected_ss_class].anim_instance->frame_num == Ss_icons[Selected_ss_class].anim_instance->stop_at ) { 
			nprintf(("anim", "Frame number = %d, Stop at %d\n", Ss_icons[Selected_ss_class].anim_instance->frame_num, Ss_icons[Selected_ss_class].anim_instance->stop_at));
			anim_play_struct aps;
			anim_release_render_instance(Ss_icons[Selected_ss_class].anim_instance);
			anim_play_init(&aps, Ss_icons[Selected_ss_class].anim, Ship_anim_coords[gr_screen.res][0], Ship_anim_coords[gr_screen.res][1]);
			aps.start_at = SHIP_ANIM_LOOP_FRAME;
//			aps.start_at = 0;
			aps.screen_id = ON_SHIP_SELECT;
			aps.framerate_independent = 1;
			aps.skip_frames = 0;
			Ss_icons[Selected_ss_class].anim_instance = anim_play(&aps);
		}
	}

	gr_reset_clip();

	ship_select_render(frametime);
	if ( !Background_playing ) {		
		Ship_select_ui_window.draw();
		ship_select_redraw_pressed_buttons();
		common_render_selected_screen_button();
	}

	// The background transition plays once. Display ship icons after Background done playing
	if ( !Background_playing ) {
		draw_ship_icons();
		for ( int i = 0; i < MAX_WING_BLOCKS; i++ ) {
			draw_wing_block(i, Hot_ss_slot, -1, Selected_ss_class);
		}		
	}
	
	if ( ss_icon_being_carried() ) {
		int mouse_x, mouse_y;
		mouse_get_pos( &mouse_x, &mouse_y );
		gr_set_bitmap(Ss_icons[Carried_ss_icon.ship_class].icon_bmaps[ICON_FRAME_SELECTED], GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(mouse_x + Ss_delta_x , mouse_y + Ss_delta_y);
	}

	// draw out ship information
	ship_select_blit_ship_info();


	ss_maybe_flash_button();

	// blit help overlay if active
	help_overlay_maybe_blit(SS_OVERLAY);

	// If the commit button was pressed, do the commit button actions.  Done at the end of the
	// loop so there isn't a skip in the animation (since ship_create() can take a long time if
	// the ship model is not in memory
	if ( Commit_pressed ) {		
		commit_pressed();		
		Commit_pressed = 0;
	}

	gr_flip();

	if ( Game_mode & GM_MULTIPLAYER ) {
		if ( Selected_ss_class >= 0 )
			Net_player->p_info.ship_class = Selected_ss_class;
	}	 

	if(!Background_playing){
		// should render this as close to last as possible so it overlaps all controls
		// chatbox_render();		
	}

	// If the commit button was pressed, do the commit button actions.  Done at the end of the
	// loop so there isn't a skip in the animation (since ship_create() can take a long time if
	// the ship model is not in memory
	if ( Commit_pressed ) {		
		commit_pressed();		
		Commit_pressed = 0;
	}
}


// ------------------------------------------------------------------------
//	ship_select_close() is called once when the ship select screen is exited
//
//
void ship_select_close()
{
	key_flush();

	if ( !Ship_select_open ) {
		nprintf(("Alan","ship_select_close() returning without doing anything\n"));
		return;
	}

	nprintf(("Alan", "Entering ship_select_close()\n"));

	// done with the bitmaps, so unlock it
	bm_unlock(ShipSelectMaskBitmap);

	// unload the bitmaps
	bm_unload(ShipSelectMaskBitmap);
	help_overlay_unload(SS_OVERLAY);

	// release the bitmpas that were previously extracted from anim files
	ss_unload_icons();

	// Release any active ship anim instances
	unload_ship_anim_instances();

	// unload ship animations if they were loaded
	unload_ship_anims();

	Ship_select_ui_window.destroy();

#ifdef MAKE_FS1
	common_free_interface_palette();
#endif

	Ship_anim_class = -1;
	Ship_select_open = 0;	// This game-wide global flag is set to 0 to indicate that the ship
									// select screen has been closed and memory freed.  This flag
									// is needed so we can know if ship_select_close() needs to called if
									// restoring a game from the Options screen invoked from ship select
}

//	ss_unload_icons() frees the bitmaps used for ship icons 
void ss_unload_icons()
{
	int					i,j;
	ss_icon_info		*icon;

	for ( i = 0; i < MAX_SHIP_TYPES; i++ ) {
		icon = &Ss_icons[i];

		for ( j = 0; j < NUM_ICON_FRAMES; j++ ) {
			if ( icon->icon_bmaps[j] >= 0 ) {
				bm_release(icon->icon_bmaps[j]);
				icon->icon_bmaps[j] = -1;
			}
		}
	}
}

// ------------------------------------------------------------------------
//	draw_ship_icons() will request which icons to draw on screen.
void draw_ship_icons()
{
	int i;
	int count=0;

	ss_active_item	*sai;
	i = 0;
	for ( sai = GET_FIRST(&SS_active_head); sai != END_OF_LIST(&SS_active_head); sai = GET_NEXT(sai) ) {
		count++;
		if ( count <= SS_active_list_start )
			continue;

		if ( i >= MAX_ICONS_ON_SCREEN )
			break;

		draw_ship_icon_with_number(i, sai->ship_class);
		i++;
	}
}

// ------------------------------------------------------------------------
//	draw_ship_icon_with_number() will draw a ship icon on screen with the
// number of available ships to the left.
//
//
void draw_ship_icon_with_number(int screen_offset, int ship_class)
{
	char	buf[32];
	int	num_x,num_y;
	ss_icon_info *ss_icon;


	Assert( screen_offset >= 0 && screen_offset <= 3 );
	Assert( ship_class >= 0 );
	ss_icon = &Ss_icons[ship_class];

	num_x = Ship_list_coords[gr_screen.res][screen_offset][2];
	num_y = Ship_list_coords[gr_screen.res][screen_offset][3];
	
	// assume default bitmap is to be used
	ss_icon->current_icon_bitmap = ss_icon->icon_bmaps[ICON_FRAME_NORMAL];

	// next check if ship has mouse over it
	if ( Hot_ss_icon > -1 ) {
		Assert(Hot_ss_icon <= 3);
		if ( Hot_ss_icon == screen_offset )
			ss_icon->current_icon_bitmap = ss_icon->icon_bmaps[ICON_FRAME_HOT];
	}

	// highest precedence is if the ship is selected
	if ( Selected_ss_class > -1 ) {
		if ( Selected_ss_class == ship_class )
			ss_icon->current_icon_bitmap = ss_icon->icon_bmaps[ICON_FRAME_SELECTED];
	}

	if ( Ss_pool[ship_class] <= 0 ) {
		return;
	}

	// blit the icon
	gr_set_bitmap(ss_icon->current_icon_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	gr_bitmap(Ship_list_coords[gr_screen.res][screen_offset][0], Ship_list_coords[gr_screen.res][screen_offset][1]);

	// blit the number
	sprintf(buf, "%d", Ss_pool[ship_class] );
	gr_set_color_fast(&Color_white);
	gr_string(num_x, num_y, buf);
}

// ------------------------------------------------------------------------
//	stop_ship_animation() will halt the currently playing ship animation.  The
// instance will be freed, (but the compressed data is not freed).  The animation
// will not display after this function is called (even on this frame), since
// the instance is removed from the anim_render_list.
void stop_ship_animation()
{
	ss_icon_info	*ss_icon;

	if ( Ship_anim_class == -1 ) 
		return;

	ss_icon = &Ss_icons[Ship_anim_class];

	anim_release_render_instance(ss_icon->anim_instance);
	ss_icon->anim_instance = NULL;

	Ship_anim_class = -1;
}


// ------------------------------------------------------------------------
// this loads an individual animation file
// it attempts to load a hires version (ie, it attaches a "2_" in front of the
// filename. if no hires version is available, it defaults to the lowres

anim* ss_load_individual_animation(int ship_class)
{
	anim *p_anim;
	char animation_filename[CF_MAX_FILENAME_LENGTH+4];
	
	// 1024x768 SUPPORT
	// If we are in 1024x768, we first want to append "2_" in front of the filename
	if (gr_screen.res == GR_1024) {
		Assert(strlen(Ship_info[ship_class].anim_filename) <= 30);
		strcpy(animation_filename, "2_");
		strcat(animation_filename, Ship_info[ship_class].anim_filename);
		// now check if file exists
		// GRR must add a .ANI at the end for detection
		strcat(animation_filename, ".ani");
		
		p_anim = anim_load(animation_filename, 1);
		if (p_anim == NULL) {
			// failed loading hi-res, revert to low res
			strcpy(animation_filename, Ship_info[ship_class].anim_filename);
			p_anim = anim_load(animation_filename, 1);
			mprintf(("Ship ANI: Can not find %s, using lowres version instead.\n", animation_filename)); 
		} else {
			mprintf(("SHIP ANI: Found hires version of %s\n",animation_filename));
		}
		/*
		// this is lame and doesnt work cuz cf_exist() doesnt search the packfiles
		if (!cf_exist(animation_filename, CF_TYPE_INTERFACE)) {
			// file does not exist, use original low res version
			strcpy(animation_filename, Ship_info[ship_class].anim_filename);
			mprintf(("Ship ANI: Can not find %s, using lowres version instead.\n", animation_filename)); 
		} else {
			animation_filename[strlen(animation_filename) - 4] = '\0';
			mprintf(("SHIP ANI: Found hires version of %s\n",animation_filename));
		}
		*/
	} else {
		strcpy(animation_filename, Ship_info[ship_class].anim_filename);
		p_anim = anim_load(animation_filename, 1);
	}
	
	return p_anim;
}

// ------------------------------------------------------------------------
//	start_ship_animation() will start a ship animation playing, and will 
// load the compressed anim from disk if required.
void start_ship_animation(int ship_class, int play_sound)
{
	ss_icon_info	*ss_icon;
	Assert( ship_class >= 0 );

	if ( Ship_anim_class == ship_class ) 
		return;

	if ( Ship_anim_class >= 0 ) {
		stop_ship_animation();
	}

	ss_icon = &Ss_icons[ship_class];

	// see if we need to load in the animation from disk
	if ( ss_icon->anim == NULL ) {
		ss_icon->anim = ss_load_individual_animation(ship_class);
	}

	// see if we need to get an instance
	if ( ss_icon->anim_instance == NULL ) {
		anim_play_struct aps;

		anim_play_init(&aps, ss_icon->anim, Ship_anim_coords[gr_screen.res][0], Ship_anim_coords[gr_screen.res][1]);
		aps.screen_id = ON_SHIP_SELECT;
		aps.framerate_independent = 1;
		aps.skip_frames = 0;
		ss_icon->anim_instance = anim_play(&aps);
	}

	Ship_anim_class = ship_class;

//	if ( play_sound ) {
		gamesnd_play_iface(SND_SHIP_ICON_CHANGE);
//	}
}

// ------------------------------------------------------------------------
//	unload_ship_anims() will free all compressed anims from memory that were
// loaded for the ship animations.
//
//
void unload_ship_anims()
{
	for ( int i = 0; i < MAX_SHIP_TYPES; i++ ) {
		if ( Ss_icons[i].anim ) {
			anim_free(Ss_icons[i].anim);
			Ss_icons[i].anim = NULL;
		}
	}
}

// ------------------------------------------------------------------------
//	unload_ship_anim_instances() will free any active ship animation instances.
//
//
void unload_ship_anim_instances()
{
	for ( int i = 0; i < MAX_SHIP_TYPES; i++ ) {
		if ( Ss_icons[i].anim_instance ) {
			anim_release_render_instance(Ss_icons[i].anim_instance);
			Ss_icons[i].anim_instance = NULL;
		}
	}
}

// ------------------------------------------------------------------------
// commit_pressed() is called when the commit button from any of the briefing/ship select/ weapon
// select screens is pressed.  The ship selected is created, and the interface music is stopped.
void commit_pressed()
{
	int player_ship_info_index;
	
	if ( Wss_num_wings > 0 ) {
		if(!(Game_mode & GM_MULTIPLAYER)){
			int rc;
			rc = create_wings();
			if (rc != 0) {
				gamesnd_play_iface(SND_GENERAL_FAIL);
				return;
			}
		}
	}
	else {

		if ( Selected_ss_class == -1 ) {
			player_ship_info_index = Team_data[Common_team].default_ship;

		} else {
			Assert(Selected_ss_class >= 0 );
			player_ship_info_index = Selected_ss_class;
		}

		update_player_ship( player_ship_info_index );
		if ( wl_update_ship_weapons(Ships[Player_obj->instance].objnum, &Wss_slots[0]) == -1 ) {
			popup(PF_USE_AFFIRMATIVE_ICON, 1, POPUP_OK, XSTR( "Player ship has no weapons", 461));
			return;
		}
	}

	// Check to ensure that the hotkeys are still pointing to valid objects.  It is possible
	// for the player to assign a ship to a hotkey, then go and delete that ship in the 
	// ship selection, and then try to start the mission.  This function will detect those objects,
	// and remove them from the hotkey linked lists.
	mission_hotkey_validate();

	gamesnd_play_iface(SND_COMMIT_PRESSED);

	// save the player loadout
	if ( !(Game_mode & GM_MULTIPLAYER) ) {
		strcpy(Player_loadout.filename, Game_current_mission_filename);
		strcpy(Player_loadout.last_modified, The_mission.modified);
		wss_save_loadout();
	}

	// move to the next stage
	// in multiplayer this is the final mission sync
	if(Game_mode & GM_MULTIPLAYER){		
		Multi_sync_mode = MULTI_SYNC_POST_BRIEFING;
		gameseq_post_event(GS_EVENT_MULTI_MISSION_SYNC);	
		
		// otherwise tell the standalone to move everyone into this state and continue
		if((Net_player->flags & NETINFO_FLAG_GAME_HOST) && !(Net_player->flags & NETINFO_FLAG_AM_MASTER)){
			send_mission_sync_packet(MULTI_SYNC_POST_BRIEFING);
		}
	}
	// in single player we jump directly into the mission
	else {
		gameseq_post_event(GS_EVENT_ENTER_GAME);
	}
}

// ------------------------------------------------------------------------
// pick_from_ship_list() will determine if an icon from the ship selection
// list can be picked up (for drag and drop).  It calculates the difference
// in x & y between the icon and the mouse, so we can move the icon with the
// mouse in a realistic way
int pick_from_ship_list(int screen_offset, int ship_class)
{
	int rval = -1;
	Assert(ship_class >= 0);

	if ( Wss_num_wings == 0 )
		return rval;

	// If carrying an icon, then do nothing
	if ( ss_icon_being_carried() )
		return rval;

	if ( Ss_pool[ship_class] > 0 ) {
		int mouse_x, mouse_y;

		ss_set_carried_icon(-1, ship_class);
		mouse_get_pos( &mouse_x, &mouse_y );
		Ss_delta_x = Ship_list_coords[gr_screen.res][screen_offset][0] - mouse_x;
		Ss_delta_y = Ship_list_coords[gr_screen.res][screen_offset][1] - mouse_y;
		Assert( Ss_pool[ship_class] >= 0 );
		rval = 0;
	}

	common_flash_button_init();
	return rval;
}

// ------------------------------------------------------------------------
// pick_from_wing() will determine if an icon from the wing formation (wb_num)
// and slot number (ws_num) can be picked up (for drag and drop).  It calculates
// the difference in x & y between the icon and the mouse, so we can move the icon with the
// mouse in a realistic way
void pick_from_wing(int wb_num, int ws_num)
{
	int slot_index;
	Assert(wb_num >= 0 && wb_num < MAX_WING_BLOCKS);
	Assert(ws_num >= 0 && ws_num < MAX_WING_SLOTS);
	
	ss_wing_info *wb;
	ss_slot_info *ws;
	wb = &Ss_wings[wb_num];
	ws = &wb->ss_slots[ws_num];
	slot_index = wb_num*4+ws_num;

	if ( wb->wingnum < 0 )
		return;

	// Take care of case where the mouse button goes from up to down in one frame while
	// carrying an icon
	if ( Drop_on_wing_mflag && ss_icon_being_carried() ) {
		if ( !ss_disabled_slot(slot_index) ) {
			ss_drop(Carried_ss_icon.from_slot, Carried_ss_icon.ship_class, slot_index, -1);
			ss_reset_carried_icon();
			gamesnd_play_iface(SND_ICON_DROP_ON_WING);
		}
	}

	if ( ss_icon_being_carried() )
		return;

	if ( ss_disabled_slot(slot_index) ) {
		return;
	}

	switch ( ws->status ) {
		case WING_SLOT_DISABLED:
		case WING_SLOT_IGNORE:
			return;
			break;

		case WING_SLOT_EMPTY:
		case WING_SLOT_EMPTY|WING_SLOT_IS_PLAYER:
			// TODO: add fail sound
			return;
			break;

		case WING_SLOT_FILLED|WING_SLOT_IS_PLAYER:
		case WING_SLOT_FILLED:
			{
			int mouse_x, mouse_y;
			Assert(Wss_slots[slot_index].ship_class >= 0);
			ss_set_carried_icon(slot_index, Wss_slots[slot_index].ship_class);

			mouse_get_pos( &mouse_x, &mouse_y );
			Ss_delta_x = Wing_icon_coords[gr_screen.res][slot_index][0] - mouse_x;
			Ss_delta_y = Wing_icon_coords[gr_screen.res][slot_index][1] - mouse_y;
			Carried_ss_icon.from_x = mouse_x;
			Carried_ss_icon.from_y = mouse_y;
			}
			break;
	
		default:
			Int3();
			break;

	} // end switch

	common_flash_button_init();
}

// ------------------------------------------------------------------------
// draw_wing_block() will draw the wing icons for the wing formation number
// passed in as a parameter.  
//
// input:	wb_num	=>		wing block number (numbering starts at 0)
//				hot_slot	=>		index of slot that mouse is over
//				selected_slot	=>	index of slot that is selected
//				class_select	=>	all ships of this class are drawn selected (send -1 to not use)
void draw_wing_block(int wb_num, int hot_slot, int selected_slot, int class_select)
{
	ss_wing_info	*wb;
	ss_slot_info	*ws;
	ss_icon_info	*icon;
	wing				*wp;
	int				i, bitmap_to_draw, w, h, sx, sy, slot_index;

	Assert(wb_num >= 0 && wb_num < MAX_WING_BLOCKS);		
	wb = &Ss_wings[wb_num];
	
	if ( wb->wingnum == -1 )
		return;	
	
	// print the wing name under the wing
	wp = &Wings[wb->wingnum];
	gr_get_string_size(&w, &h, wp->name);
	sx = Wing_icon_coords[gr_screen.res][wb_num*4][0] + 16 - w/2;
	sy = Wing_icon_coords[gr_screen.res][wb_num*4 + 3][1] + 32 + h;
	gr_set_color_fast(&Color_normal);
	gr_string(sx, sy, wp->name);

	for ( i = 0; i < MAX_WING_SLOTS; i++ ) {
		bitmap_to_draw = -1;
		ws = &wb->ss_slots[i];
		slot_index = wb_num*4 + i;

		if ( Wss_slots[slot_index].ship_class >= 0 ) {
			icon = &Ss_icons[Wss_slots[slot_index].ship_class];
		} else {
			icon = NULL;
		}

		switch(ws->status & ~WING_SLOT_LOCKED ) {
			case WING_SLOT_FILLED:
			case WING_SLOT_FILLED|WING_SLOT_IS_PLAYER:

				Assert(icon);

				if ( class_select >= 0 ) {	// only ship select
					if ( Carried_ss_icon.from_slot == slot_index ) {
						if ( ss_carried_icon_moved() ) {
							bitmap_to_draw = Wing_slot_empty_bitmap;
						} else {
							bitmap_to_draw = -1;
						}
						break;
					}
				}

				if ( ws->status & WING_SLOT_LOCKED ) {					
					bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_DISABLED];

					// in multiplayer, determine if this it the special case where the slot is disabled, and 
					// it is also _my_ slot (ie, team capatains/host have not locked players yet)
					if((Game_mode & GM_MULTIPLAYER) && multi_ts_disabled_high_slot(slot_index)){
						bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_DISABLED_HIGH];
					}

					break;
				}

				bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_NORMAL];
				if ( selected_slot == slot_index || class_select == Wss_slots[slot_index].ship_class) {
					bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_SELECTED];
				} else if ( hot_slot == slot_index ) {
					if ( mouse_down(MOUSE_LEFT_BUTTON) ){
						bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_SELECTED];
					} else {
						bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_HOT];
					}
				}

				if ( ws->status & WING_SLOT_IS_PLAYER && (selected_slot != slot_index) ) {
					bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_PLAYER];
				}
				break;

			case WING_SLOT_EMPTY:
			case WING_SLOT_EMPTY|WING_SLOT_IS_PLAYER:
				bitmap_to_draw = Wing_slot_empty_bitmap;
				break;

			case WING_SLOT_DISABLED:
			case WING_SLOT_IGNORE:
				if ( icon ) {
					bitmap_to_draw = icon->icon_bmaps[ICON_FRAME_DISABLED];
				} else {
					bitmap_to_draw = Wing_slot_disabled_bitmap;
				}
				break;

			default:
				Int3();
				break;

		}	// end switch

		
		if ( bitmap_to_draw != -1 ) {
			gr_set_bitmap(bitmap_to_draw, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
			gr_bitmap(Wing_icon_coords[gr_screen.res][slot_index][0], Wing_icon_coords[gr_screen.res][slot_index][1]);
		}
	}
}

// called by multiplayer team select to set the slot based flags
void ss_make_slot_empty(int slot_index)
{
	int wing_num,slot_num;
	ss_wing_info	*wb;
	ss_slot_info	*ws;

	// calculate the wing #
	wing_num = slot_index / 4;
	slot_num = slot_index % 4;

	// get the wing and slot entries
	wb = &Ss_wings[wing_num];
	ws = &wb->ss_slots[slot_num];

	// set the flags
	ws->status &= ~(WING_SLOT_FILLED | WING_SLOT_DISABLED);
	ws->status |= WING_SLOT_EMPTY;
}

// called by multiplayer team select to set the slot based flags
void ss_make_slot_full(int slot_index)
{
	int wing_num,slot_num;
	ss_wing_info	*wb;
	ss_slot_info	*ws;

	// calculate the wing #
	wing_num = slot_index / 4;
	slot_num = slot_index % 4;

	// get the wing and slot entries
	wb = &Ss_wings[wing_num];
	ws = &wb->ss_slots[slot_num];

	// set the flags
	ws->status &= ~(WING_SLOT_EMPTY | WING_SLOT_DISABLED);
	ws->status |= WING_SLOT_FILLED;
}

void ss_blit_ship_icon(int x,int y,int ship_class,int bmap_num)
{
	// blit the bitmap in the correct location
	if(ship_class == -1){
		gr_set_bitmap(Wing_slot_empty_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	} else {
		ss_icon_info *icon = &Ss_icons[ship_class];
		Assert(icon->icon_bmaps[bmap_num] != -1);	
		gr_set_bitmap(icon->icon_bmaps[bmap_num], GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	}
	gr_bitmap(x,y);	
}

// ------------------------------------------------------------------------
//	unload_ship_icons() frees the memory that was used to hold the bitmaps
// for ship icons 
//
void unload_wing_icons()
{
	if ( Wing_slot_empty_bitmap != -1 ) {
		bm_release(Wing_slot_empty_bitmap);
		Wing_slot_empty_bitmap = -1;
	}

	if ( Wing_slot_disabled_bitmap != -1 ) {
		bm_release(Wing_slot_disabled_bitmap);
		Wing_slot_disabled_bitmap = -1;
	}
}

// ------------------------------------------------------------------------
//	create_wings() will ensure the correct ships are in the player wings
// for the game.  It works by calling change_ship_type() on the wing ships
// so they match what the player selected.   ship_create() is called for the
// player ship (and current_count, ship_index[] is updated), since it is not yet
// part of the wing structure.
//
// returns:   0 ==> success
//           !0 ==> failure
int create_wings()
{
	ss_wing_info		*wb;
	ss_slot_info		*ws;
	wing					*wp;
	p_object				*p_objp;

	int shipnum, objnum, slot_index;
	int cleanup_ship_index[MAX_WING_SLOTS];
	int i,j,k;
	int found_pobj;

	for ( i = 0; i < MAX_WING_BLOCKS; i++ ) {
		
		wb = &Ss_wings[i];

		if ( wb->wingnum ==  -1 )
			continue;

		wp = &Wings[wb->wingnum];		
		
		for ( j = 0; j < MAX_WING_SLOTS; j++ ) {
			slot_index = i*4+j;
			ws = &wb->ss_slots[j];
			switch ( ws->status ) {

				case WING_SLOT_FILLED:
				case WING_SLOT_FILLED|WING_SLOT_IS_PLAYER:
				case WING_SLOT_FILLED|WING_SLOT_LOCKED:
				case WING_SLOT_FILLED|WING_SLOT_IS_PLAYER|WING_SLOT_LOCKED:
					if ( wp->ship_index[j] >= 0 ) {
						Assert(Ships[wp->ship_index[j]].objnum >= 0);
					}

					if ( ws->status & WING_SLOT_IS_PLAYER ) {
						update_player_ship(Wss_slots[slot_index].ship_class);

						if ( wl_update_ship_weapons(Ships[Player_obj->instance].objnum, &Wss_slots[i*4+j]) == -1 ) {
							popup(PF_USE_AFFIRMATIVE_ICON, 1, POPUP_OK, XSTR( "Player ship has no weapons", 461));
							return -1;
						}

						objnum = OBJ_INDEX(Player_obj);
						shipnum = Objects[objnum].instance;
					} else {
						if ( wb->is_late) {
							found_pobj = 0;
							for ( p_objp = GET_FIRST(&ship_arrival_list); p_objp != END_OF_LIST(&ship_arrival_list); p_objp = GET_NEXT(p_objp) ) {
								if ( p_objp->wingnum == WING_INDEX(wp) ) {
									if ( ws->sa_index == (p_objp-ship_arrivals) ) {
										p_objp->ship_class = Wss_slots[slot_index].ship_class;
										wl_update_parse_object_weapons(p_objp, &Wss_slots[i*4+j]);
										found_pobj = 1;
										break;
									}
								}
							}
							Assert(found_pobj);
						}
						else {
							// AL 10/04/97
							// Change the ship type of the ship if different than current.
							// NOTE: This will reset the weapons for this ship.  I think this is
							//       the right thing to do, since the ships may have different numbers
							//			of weapons and may not have the same allowed weapon types
							if ( Ships[wp->ship_index[j]].ship_info_index != Wss_slots[slot_index].ship_class )
								change_ship_type(wp->ship_index[j], Wss_slots[slot_index].ship_class);
							wl_update_ship_weapons(Ships[wp->ship_index[j]].objnum, &Wss_slots[i*4+j]);
						}
					}

					break;

				case WING_SLOT_EMPTY:
				case WING_SLOT_EMPTY|WING_SLOT_IS_PLAYER:
					if ( ws->status & WING_SLOT_IS_PLAYER ) {						
						popup(PF_USE_AFFIRMATIVE_ICON, 1, POPUP_OK, XSTR( "Player %s must select a place in player wing", 462), Player->callsign);
						return -1;
					}
					break;

				default:
					break;
			}
		}	// end for (wing slot)	
	}	// end for (wing block)

	for ( i = 0; i < MAX_WING_BLOCKS; i++ ) {
		wb = &Ss_wings[i];
		wp = &Wings[wb->wingnum];		

		if ( wb->wingnum == -1 )
			continue;

		for ( k = 0; k < MAX_WING_SLOTS; k++ ) {
			cleanup_ship_index[k] = -1;
		}

		for ( j = 0; j < MAX_WING_SLOTS; j++ ) {
			ws = &wb->ss_slots[j];
			switch( ws->status ) {
				case WING_SLOT_EMPTY:	
					// delete ship that is not going to be used by the wing
					if ( wb->is_late ) {
						list_remove( &ship_arrival_list, &ship_arrivals[ws->sa_index]);
						wp->wave_count--;
						Assert(wp->wave_count >= 0);
					}
					else {
						shipnum = wp->ship_index[j];
						Assert( shipnum >= 0 && shipnum < MAX_SHIPS );
						cleanup_ship_index[j] = shipnum;
						ship_add_exited_ship( &Ships[shipnum], SEF_PLAYER_DELETED );
						obj_delete(Ships[shipnum].objnum);
						hud_set_wingman_status_none( Ships[shipnum].wing_status_wing_index, Ships[shipnum].wing_status_wing_pos);
					}
					break;

				default:
					break;

			} // end switch

		}	// end for (wing slot)	

		for ( k = 0; k < MAX_WING_SLOTS; k++ ) {
			if ( cleanup_ship_index[k] != -1 ) {
				ship_wing_cleanup( cleanup_ship_index[k], wp );
			}
		}

	}	// end for (wing block)
	
	return 0;
}

void ship_stop_animation()
{
	if ( Ship_anim_class >= 0  )
		stop_ship_animation();
}

// ----------------------------------------------------------------------------
// update_player_ship()
//
// Updates the ship class of the player ship
//
//	parameters:	si_index  => ship info index of ship class to change to
//
//
void update_player_ship(int si_index)
{
	Assert( si_index >= 0 );
	Assert( Player_obj != NULL);

	// AL 10/04/97
	// Change the ship type of the player ship if different than current.
	// NOTE: This will reset the weapons for this ship.  I think this is
	//       the right thing to do, since the ships may have different numbers
	//			of weapons and may not have the same allowed weapon types
	if ( Player_ship->ship_info_index != si_index ) 
		change_ship_type(Player_obj->instance, si_index);

	Player->last_ship_flown_si_index = si_index;
}

// ----------------------------------------------------------------------------
// create a default player ship
//
//	parameters:		use_last_flown	=> select ship that was last flown on a mission
//						(this is a default parameter which is set to 1)
//
// returns:			0 => success
//               !0 => failure
//
int create_default_player_ship(int use_last_flown)
{
	int	player_ship_class=-1, i;

	// find the ship that matches the string stored in default_player_ship

	if ( use_last_flown ) {
		player_ship_class = Players[Player_num].last_ship_flown_si_index;
	}
	else {
		for (i = 0; i < Num_ship_types; i++) {
			if ( !stricmp(Ship_info[i].name, default_player_ship) ) {
				player_ship_class = i;
				Players[Player_num].last_ship_flown_si_index = player_ship_class;
				break;
			}
		}

		if (i == Num_ship_types)
			return 1;
	}

	update_player_ship(player_ship_class);

	// debug code to keep using descent style physics if the player starts a new game
#ifndef NDEBUG
	if ( use_descent ) {
		use_descent = 0;
		toggle_player_object();
	}
#endif

	return 0;
}

// return the original ship class for the specified slot
int ss_return_original_ship_class(int slot_num)
{
	int wnum, snum;

	wnum = slot_num/4;
	snum = slot_num%4;

	return Ss_wings[wnum].ss_slots[snum].original_ship_class;
}

// return the ship arrival index for the slot (-1 means no ship arrival index)
int ss_return_saindex(int slot_num)
{
	int wnum, snum;

	wnum = slot_num/4;
	snum = slot_num%4;

	return Ss_wings[wnum].ss_slots[snum].sa_index;
}

// ----------------------------------------------------------------------------
// ss_return_ship()
//
// For a given wing slot, return the ship index if the ship has been created.  
// Otherwise, find the index into ship_arrivals[] for the ship
//
//	input:	wing_block	=>		wing block of ship to find
//				wing_slot	=>		wing slot of ship to find
//				ship_index	=>		OUTPUT parameter: the Ships[] index of the ship in the wing slot
//										This value will be -1 if there is no ship created yet
//				ppobjp		=>		OUTPUT parameter: returns a pointer to a parse object for
//										the ship that hasn't been created yet.  Set to NULL if the
//										ship has already been created
//
// returns:	the original ship class of the ship, or -1 if the ship doesn't exist
//
// NOTE: For the player wing, the player is not yet in the wp->ship_index[].. so
// that is why there is an offset of 1 when getting ship indicies from the player
// wing.  The player is special cased by looking at the status of the wing slot
//
int ss_return_ship(int wing_block, int wing_slot, int *ship_index, p_object **ppobjp)
{
	*ship_index = -1;
	*ppobjp = NULL;

	ss_slot_info	*ws;

	if (!Wss_num_wings) {
		*ppobjp = NULL;
		*ship_index = Player_obj->instance;
		return Player_ship->ship_info_index;
	}

	if ( Ss_wings[wing_block].wingnum < 0 ) {
		return -1;
	}

	ws = &Ss_wings[wing_block].ss_slots[wing_slot];

	// Check to see if ship is on the ship_arrivals[] list
	if ( ws->sa_index != -1 ) {
		*ship_index = -1;
		*ppobjp = &ship_arrivals[ws->sa_index];
	} else {
		*ship_index = Wings[Ss_wings[wing_block].wingnum].ship_index[wing_slot];
		Assert(*ship_index != -1);		
	}

	return ws->original_ship_class;
}

// return the name of the ship in the specified wing position... if the ship is the
// player ship, return the player callsign
//
// input: ensure at least NAME_LENGTH bytes allocated for name buffer
void ss_return_name(int wing_block, int wing_slot, char *name)
{
	ss_slot_info	*ws;
	wing				*wp;

	ws = &Ss_wings[wing_block].ss_slots[wing_slot];
	wp = &Wings[Ss_wings[wing_block].wingnum];		

	if (!Wss_num_wings) {
		strcpy(name, Player->callsign);
		return;
	}

	// Check to see if ship is on the ship_arrivals[] list
	if ( ws->sa_index != -1 ) {
		strcpy(name, ship_arrivals[ws->sa_index].name);
	} else {
		ship *sp;
		sp = &Ships[wp->ship_index[wing_slot]];

		// in multiplayer, return the callsigns of the players who are in the ships
		if(Game_mode & GM_MULTIPLAYER){
			int player_index = multi_find_player_by_object(&Objects[sp->objnum]);
			if(player_index != -1){
				strcpy(name,Net_players[player_index].player->callsign);
			} else {
				strcpy(name,sp->ship_name);
			}
		} else {		
			strcpy(name, sp->ship_name);
		}
	}
}

int ss_get_selected_ship()
{
	return Selected_ss_class;
}

// Set selected ship to the first occupied wing slot, or first ship in pool if no slots are occupied
void ss_reset_selected_ship()
{
	int i;

	Selected_ss_class = -1;

	if ( Wss_num_wings <= 0 ) {
		Selected_ss_class = Team_data[Common_team].default_ship;
		return;
	}

	// get first ship class found on slots
	for ( i = 0; i < MAX_WSS_SLOTS; i++ ) {
		if ( Wss_slots[i].ship_class >= 0 ) {
			Selected_ss_class = Wss_slots[i].ship_class;
			break;
		}
	}

	if ( Selected_ss_class == -1 ) {
		Int3();
		for ( i = 0; i < MAX_SHIP_TYPES; i++ ) {
			if ( Ss_pool[i] > 0 ) {
				Selected_ss_class = i;
			}
		}
	}

	if ( Selected_ss_class == -1 ) {
		Int3();
		return;
	}
}

// There may be ships that are in wings but not in Team_data[0].  Since we still want to show those
// icons in the ship selection list, the code below checks for these cases.  If a ship is found in
// a wing, and is not in Team_data[0], it is appended to the end of the ship_count[] and ship_list[] arrays
// that are in Team_data[0]
//
// exit: number of distinct ship classes available to choose from
int ss_fixup_team_data(team_data *tdata)
{
	int i, j, k, ship_in_parse_player, list_size;
	p_object		*p_objp;
	team_data	*p_team_data;

	p_team_data = tdata;
	ship_in_parse_player = 0;
	list_size = p_team_data->number_choices;

	for ( i = 0; i < MAX_PLAYER_WINGS; i++ ) {
		wing *wp;
		if ( Starting_wings[i] == -1 )
			continue;
		wp = &Wings[Starting_wings[i]];
		for ( j = 0; j < wp->current_count; j++ ) {
			ship_in_parse_player = 0;
			
			for ( k = 0; k < p_team_data->number_choices; k++ ) {
				Assert( p_team_data->ship_count[k] >= 0 );
				if ( p_team_data->ship_list[k] == Ships[wp->ship_index[j]].ship_info_index ) {
					ship_in_parse_player = 1;
					break;
				}
			}	// end for, go to next item in parse player

			if ( !ship_in_parse_player ) {
				p_team_data->ship_count[list_size] = 0;
				p_team_data->ship_list[list_size] = Ships[wp->ship_index[j]].ship_info_index;
				p_team_data->number_choices++;
				list_size++;
			}
		}	// end for, go get next ship in wing

		if ( wp->current_count == 0 ) {

			for ( p_objp = GET_FIRST(&ship_arrival_list); p_objp != END_OF_LIST(&ship_arrival_list); p_objp = GET_NEXT(p_objp) ) {
				if ( p_objp->wingnum == WING_INDEX(wp) ) {
					ship_in_parse_player = 0;
			
					for ( k = 0; k < p_team_data->number_choices; k++ ) {
						Assert( p_team_data->ship_count[k] >= 0 );
						if ( p_team_data->ship_list[k] == p_objp->ship_class ) {
							ship_in_parse_player = 1;
							break;
						}
					}	// end for, go to next item in parse player

					if ( !ship_in_parse_player ) {
						p_team_data->ship_count[list_size] = 0;
						p_team_data->ship_list[list_size] = p_objp->ship_class;
						p_team_data->number_choices++;
						list_size++;
					}
				}
			}
		}
	}	// end for, go to next wing

	if ( list_size == 0 ) {
		// ensure that the default player ship is in the ship_list too
		ship_in_parse_player = 0;
		for ( k = 0; k < p_team_data->number_choices; k++ ) {
			Assert( p_team_data->ship_count[k] >= 0 );
			if ( p_team_data->ship_list[k] == p_team_data->default_ship ) {
				ship_in_parse_player = 1;
				break;
			}
		}
		if ( !ship_in_parse_player ) {
			p_team_data->ship_count[list_size] = 0;
			p_team_data->ship_list[list_size] = p_team_data->default_ship;
			p_team_data->number_choices++;
			list_size++;
		}
	}

	return list_size;
}

// set numbers of ships in pool to default values
void ss_init_pool(team_data *pteam)
{
	int i;

	for ( i = 0; i < MAX_SHIP_TYPES; i++ ) {
		Ss_pool[i] = -1;
	}

	// set number of available ships based on counts in team_data
	for ( i = 0; i < pteam->number_choices; i++ ) {
		Ss_pool[pteam->ship_list[i]] = pteam->ship_count[i];
	}
}

// load the icons for a specific ship class
void ss_load_icons(int ship_class)
{
	ss_icon_info	*icon;
	int				first_frame, num_frames, i;

	icon = &Ss_icons[ship_class];

	first_frame = bm_load_animation(Ship_info[ship_class].icon_filename, &num_frames);
	if ( first_frame == -1 ) {
		Int3();	// Could not load in icon frames.. get Alan
		return;
	}

	for ( i = 0; i < num_frames; i++ ) {
		icon->icon_bmaps[i] = first_frame+i;
	}

	// set the current bitmap for the ship icon
	icon->current_icon_bitmap = icon->icon_bmaps[ICON_FRAME_NORMAL];
}

// load all the icons for ships in the pool
void ss_load_all_icons()
{
	#ifndef DEMO // not for FS2_DEMO

	int i, j;

	for ( i = 0; i < MAX_SHIP_TYPES; i++ ) {
		// clear out data
		Ss_icons[i].current_icon_bitmap = -1;
		Ss_icons[i].anim = NULL;
		Ss_icons[i].anim_instance = NULL;
		for ( j = 0; j < NUM_ICON_FRAMES; j++ ) {
			Ss_icons[i].icon_bmaps[j] = -1;
		}

		if ( Ss_pool[i] >= 0 ) {
			ss_load_icons(i);
		}
	}

	#endif
}

// Load in a specific ship animation.  The data is loaded as a memory-mapped file since these animations
// are awfully big.
void ss_load_anim(int ship_class)
{
	ss_icon_info	*icon;

	icon = &Ss_icons[ship_class];

	// load the compressed ship animation into memory 
	// NOTE: if last parm of load_anim is 1, the anim file is mapped to memory 
	Assert( icon->anim == NULL );
	icon->anim = ss_load_individual_animation(ship_class);
	if ( icon->anim == NULL ) {
		Int3();		// couldn't load anim filename.. get Alan
	}
}

// Load in any ship animations.  This function assumes that Ss_pool has been inited.
void ss_load_all_anims()
{
	#ifndef DEMO // not for FS2_DEMO

	int i;

	for ( i = 0; i < MAX_SHIP_TYPES; i++ ) {
		if ( Ss_pool[i] > 0 ) {
			ss_load_anim(i);
		}
	}

	#endif
}

// determine if the slot is disabled
int ss_disabled_slot(int slot_num)
{
	if ( Wss_num_wings <= 0 ){
		return 0;
	}

	// HACK HACK HACK - call the team select function in multiplayer
	if(Game_mode & GM_MULTIPLAYER) {
		return multi_ts_disabled_slot(slot_num);
	} 
	return ( Ss_wings[slot_num/4].ss_slots[slot_num%4].status & WING_SLOT_IGNORE );
}

// reset the slot data
void ss_clear_slots()
{
	int				i,j;
	ss_slot_info	*slot;

	for ( i = 0; i < MAX_WSS_SLOTS; i++ ) {
		Wss_slots[i].ship_class = -1;
	}

	for ( i = 0; i < 3; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			slot = &Ss_wings[i].ss_slots[j];
			slot->status = WING_SLOT_DISABLED;
			slot->sa_index = -1;
			slot->original_ship_class = -1;
		}
	}
}

// initialize all wing struct stuff
void ss_clear_wings()
{
	int idx;

	for(idx=0;idx<MAX_PLAYER_WINGS;idx++){
		Ss_wings[idx].wingnum = -1;
		Ss_wings[idx].num_slots = 0;
		Ss_wings[idx].is_late = 0;
	}
}

// set up Wss_num_wings and Wss_wings[] based on Starting_wings[] info
void ss_init_wing_info(int wing_num,int starting_wing_num)
{
	wing				*wp;
	ss_wing_info	*ss_wing;
	ss_slot_info	*slot;
		
	ss_wing = &Ss_wings[wing_num];	

	if ( Starting_wings[starting_wing_num] < 0 ) {
		return;
	}

	ss_wing->wingnum = Starting_wings[starting_wing_num];
	Wss_num_wings++;

	wp = &Wings[Ss_wings[wing_num].wingnum];
	ss_wing->num_slots = wp->current_count;

	if ( wp->current_count == 0 || wp->ship_index[0] == -1 ) {
		p_object *p_objp;
		// Temporarily fill in the current count and initialize the ship list in the wing
		// This gets cleaned up before the mission is started
		for ( p_objp = GET_FIRST(&ship_arrival_list); p_objp != END_OF_LIST(&ship_arrival_list); p_objp = GET_NEXT(p_objp) ) {
			if ( p_objp->wingnum == WING_INDEX(wp) ) {
				slot = &ss_wing->ss_slots[ss_wing->num_slots++];
				slot->sa_index = p_objp-ship_arrivals;
				slot->original_ship_class = p_objp->ship_class;
			}
			ss_wing->is_late = 1;
		}
	}	
}

// Determine if a ship is actually a console player ship
int ss_wing_slot_is_console_player(int index)
{
	int wingnum, slotnum;
	
	wingnum=index/4;
	slotnum=index%4;

	if ( wingnum >= Wss_num_wings ) {
		return 0;
	}

	if ( Ss_wings[wingnum].ss_slots[slotnum].status & WING_SLOT_IS_PLAYER ) {
		return 1;
	}

	return 0;
}

// init the ship selection portion of the units, and set up the ui data
void ss_init_units()
{
	int				i,j;
	wing				*wp;
	ss_slot_info	*ss_slot;
	ss_wing_info	*ss_wing;	

	for ( i = 0; i < Wss_num_wings; i++ ) {

		ss_wing = &Ss_wings[i];

		if ( ss_wing->wingnum < 0 ) {
			Int3();
			continue;
		}

		wp = &Wings[ss_wing->wingnum];

		for ( j = 0; j < ss_wing->num_slots; j++ ) {
				
			ss_slot = &ss_wing->ss_slots[j];

			if ( ss_slot->sa_index == -1 ) {
				ss_slot->original_ship_class = Ships[wp->ship_index[j]].ship_info_index;
			}
	
			// Set the type of slot.  Check if the slot is marked as locked, if so then the player is not
			// going to be able to modify that ship.
			if ( ss_slot->sa_index == -1 ) {
				int objnum;
				if ( Ships[wp->ship_index[j]].flags & SF_LOCKED ) {
					ss_slot->status = WING_SLOT_DISABLED;
					ss_slot->status |= WING_SLOT_LOCKED;
				} else {
					ss_slot->status = WING_SLOT_FILLED;
				}

				objnum = Ships[wp->ship_index[j]].objnum;
				if ( Objects[objnum].flags & OF_PLAYER_SHIP ) {
					if ( ss_slot->status & WING_SLOT_LOCKED ) {
						// Int3();	// Get Alan
						
						// just unflag it
						ss_slot->status &= ~(WING_SLOT_LOCKED);
					}
					ss_slot->status = WING_SLOT_FILLED;
					if ( objnum == OBJ_INDEX(Player_obj) ) {
						ss_slot->status |= WING_SLOT_IS_PLAYER;
					}
				}
			} else {
				if ( ship_arrivals[ss_slot->sa_index].flags & P_SF_LOCKED ) {
					ss_slot->status = WING_SLOT_DISABLED;
					ss_slot->status |= WING_SLOT_LOCKED;
				} else {
					ss_slot->status = WING_SLOT_FILLED;
				}
				if ( ship_arrivals[ss_slot->sa_index].flags & P_OF_PLAYER_START ) {
					if ( ss_slot->status & WING_SLOT_LOCKED ) {
						// Int3();	// Get Alan

						// just unflag it
						ss_slot->status &= ~(WING_SLOT_LOCKED);
					}
					ss_slot->status = WING_SLOT_FILLED;
					ss_slot->status |= WING_SLOT_IS_PLAYER;
				}
			}

			// Assign the ship class to the unit
			Wss_slots[i*4+j].ship_class = ss_slot->original_ship_class;

		}	// end for
	}	// end for

	// lock/unlock any necessary slots for multiplayer
	if(Game_mode & GM_MULTIPLAYER){
		ss_recalc_multiplayer_slots();
	}
}

// set the necessary pointers
void ss_set_team_pointers(int team)
{
	Ss_wings = Ss_wings_teams[team];
	Ss_icons = Ss_icons_teams[team];
	Ss_pool = Ss_pool_teams[team];
	Wl_pool = Wl_pool_teams[team];
	Wss_slots = Wss_slots_teams[team];
}

// initialize team specific stuff
void ship_select_init_team_data(int team_num)
{			
	int idx;

	// set up the pointers to initialize the data structures.
	Ss_wings = Ss_wings_teams[team_num];
	Ss_icons = Ss_icons_teams[team_num];
	Ss_pool = Ss_pool_teams[team_num];
	Wl_pool = Wl_pool_teams[team_num];
	Wss_slots = Wss_slots_teams[team_num];
	
	ss_fixup_team_data(&Team_data[team_num]);
	ss_init_pool(&Team_data[team_num]);
	
	ss_clear_slots();		// reset data for slots	
	ss_clear_wings();

	// determine how many wings we should be checking for
	Wss_num_wings = 0;
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_TEAM)){
		// now setup wings for easy reference		
		ss_init_wing_info(0,team_num);			
	} else {			
		// now setup wings for easy reference
		for(idx=0;idx<MAX_PLAYER_WINGS;idx++){
			ss_init_wing_info(idx,idx);	
		}
	}
	

	// if there are no wings, don't call the init_units() function
	if ( Wss_num_wings <= 0 ) {
		Wss_slots[0].ship_class = Team_data[team_num].default_ship;
		return;
	}

	ss_init_units();	
}

// called when the briefing is entered
void ship_select_common_init()
{		
	int idx;

	// initialize team critical data for all teams
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_TEAM)){		
		// initialize for all teams in the game
		for(idx=0;idx<MULTI_TS_MAX_TEAMS;idx++){	
			ship_select_init_team_data(idx);
		}		

		// finally, intialize team data for myself
		ship_select_init_team_data(Common_team);
	} else {			
		ship_select_init_team_data(Common_team);
	}
	
	init_active_list();

	// load the necessary icons/animations
	ss_load_all_icons();
	ss_load_all_anims();

	ss_reset_selected_ship();
	ss_reset_carried_icon();
}

// change any interface data based on updated Wss_slots[] and Ss_pool[]
void ss_synch_interface()
{
	int				i;
	ss_slot_info	*slot;

	int old_list_start = SS_active_list_start;

	init_active_list();	// build the list of pool ships

	if ( old_list_start < SS_active_list_size ) {
		SS_active_list_start = old_list_start;
	}

	for ( i = 0; i < MAX_WSS_SLOTS; i++ ) {
		slot = &Ss_wings[i/4].ss_slots[i%4];

		if ( Wss_slots[i].ship_class == -1 ) {
			if ( slot->status & WING_SLOT_FILLED ) {
				slot->status &= ~WING_SLOT_FILLED;
				slot->status |= WING_SLOT_EMPTY;
			}
		} else {
			if ( slot->status & WING_SLOT_EMPTY ) {
				slot->status &= ~WING_SLOT_EMPTY;
				slot->status |= WING_SLOT_FILLED;
			}
		}
	}
}

// exit: data changed flag
int ss_swap_slot_slot(int from_slot, int to_slot, int *sound)
{
	int i, tmp, fwnum, fsnum, twnum, tsnum;

	if ( from_slot == to_slot ) {
		*sound=SND_ICON_DROP_ON_WING;
		return 0;
	}

	// ensure from_slot has a ship to pick up
	if ( Wss_slots[from_slot].ship_class < 0 ) {
		*sound=SND_ICON_DROP;
		return 0;
	}

	fwnum = from_slot/4;
	fsnum = from_slot%4;

	twnum = to_slot/4;
	tsnum = to_slot%4;

	// swap ship class
	tmp = Wss_slots[from_slot].ship_class;
	Wss_slots[from_slot].ship_class = Wss_slots[to_slot].ship_class;
	Wss_slots[to_slot].ship_class = tmp;

	// swap weapons
	for ( i = 0; i < MAX_WL_WEAPONS; i++ ) {
		tmp = Wss_slots[from_slot].wep[i];
		Wss_slots[from_slot].wep[i] = Wss_slots[to_slot].wep[i];
		Wss_slots[to_slot].wep[i] = tmp;

		tmp = Wss_slots[from_slot].wep_count[i];
		Wss_slots[from_slot].wep_count[i] = Wss_slots[to_slot].wep_count[i];
		Wss_slots[to_slot].wep_count[i] = tmp;
	}

	*sound=SND_ICON_DROP_ON_WING;
	return 1;
}

// exit: data changed flag
int ss_dump_to_list(int from_slot, int to_list, int *sound)
{
	int i, fwnum, fsnum;
	wss_unit	*slot;

	slot = &Wss_slots[from_slot];

	// ensure from_slot has a ship to pick up
	if ( slot->ship_class < 0 ) {
		*sound=SND_ICON_DROP;
		return 0;
	}

	fwnum = from_slot/4;
	fsnum = from_slot%4;

	// put ship back in list
	Ss_pool[to_list]++;		// return to list
	slot->ship_class = -1;	// remove from slot

	// put weapons back in list
	for ( i = 0; i < MAX_WL_WEAPONS; i++ ) {
		if ( (slot->wep[i] >= 0) && (slot->wep_count[i] > 0) ) {
			Wl_pool[slot->wep[i]] += slot->wep_count[i];
			slot->wep[i] = -1;
			slot->wep_count[i] = 0;
		}
	}

	*sound=SND_ICON_DROP;
	return 1;
}

// exit: data changed flag
int ss_grab_from_list(int from_list, int to_slot, int *sound)
{
	wss_unit	*slot;
	int i, wep[MAX_WL_WEAPONS], wep_count[MAX_WL_WEAPONS];

	slot = &Wss_slots[to_slot];

	// ensure that pool has ship
	if ( Ss_pool[from_list] <= 0 ) {
		*sound=SND_ICON_DROP;
		return 0;
	}

	Assert(slot->ship_class < 0 );	// slot should be empty

	// take ship from list->slot
	Ss_pool[from_list]--;
	slot->ship_class = from_list;

	// take weapons from list->slot
	wl_get_default_weapons(from_list, to_slot, wep, wep_count);
	wl_remove_weps_from_pool(wep, wep_count, slot->ship_class);
	for ( i = 0; i < MAX_WL_WEAPONS; i++ ) {
		slot->wep[i] = wep[i];
		slot->wep_count[i] = wep_count[i];
	}

	*sound=SND_ICON_DROP_ON_WING;
	return 1;
}

// exit: data changed flag
int ss_swap_list_slot(int from_list, int to_slot, int *sound)
{
	int i, wep[MAX_WL_WEAPONS], wep_count[MAX_WL_WEAPONS];
	wss_unit	*slot;

	// ensure that pool has ship
	if ( Ss_pool[from_list] <= 0 ) {
		*sound=SND_ICON_DROP;
		return 0;
	}

	slot = &Wss_slots[to_slot];
	Assert(slot->ship_class >= 0 );	// slot should be filled

	// put ship from slot->list
	Ss_pool[Wss_slots[to_slot].ship_class]++;

	// put weapons from slot->list
	for ( i = 0; i < MAX_WL_WEAPONS; i++ ) {
		if ( (slot->wep[i] >= 0) && (slot->wep_count[i] > 0) ) {
			Wl_pool[slot->wep[i]] += slot->wep_count[i];
			slot->wep[i] = -1;
			slot->wep_count[i] = 0;
		}
	}

	// take ship from list->slot
	Ss_pool[from_list]--;
	slot->ship_class = from_list;

	// take weapons from list->slot
	wl_get_default_weapons(from_list, to_slot, wep, wep_count);
	wl_remove_weps_from_pool(wep, wep_count, slot->ship_class);
	for ( i = 0; i < MAX_WL_WEAPONS; i++ ) {
		slot->wep[i] = wep[i];
		slot->wep_count[i] = wep_count[i];
	}

	*sound=SND_ICON_DROP_ON_WING;
	return 1;
}

void ss_apply(int mode, int from_slot, int from_list, int to_slot, int to_list,int player_index)
{
	int update=0;
	int sound=-1;

	switch(mode){
	case WSS_SWAP_SLOT_SLOT:
		update = ss_swap_slot_slot(from_slot, to_slot, &sound);
		break;
	case WSS_DUMP_TO_LIST:
		update = ss_dump_to_list(from_slot, to_list, &sound);
		break;
	case WSS_GRAB_FROM_LIST:
		update = ss_grab_from_list(from_list, to_slot, &sound);
		break;
	case WSS_SWAP_LIST_SLOT:
		update = ss_swap_list_slot(from_list, to_slot, &sound);
		break;
	}

	// only play this sound if the move was done locally (by the host in other words)
	if ( (sound >= 0) && (player_index == -1) ) {
		gamesnd_play_iface(sound);		
	}

	if ( update ) {
		// NO LONGER USED - THERE IS A MULTIPLAYER VERSION OF THIS SCREEN NOW
		/*
		if ( MULTIPLAYER_HOST ) {
			int size;
			ubyte wss_data[MAX_PACKET_SIZE-20];
			size = store_wss_data(wss_data, MAX_PACKET_SIZE-20, sound);
			send_wss_update_packet(wss_data, size, player_index);
		}
		*/

		ss_synch_interface();
	}
}

void ss_drop(int from_slot,int from_list,int to_slot,int to_list,int player_index)
{
	int mode;
	common_flash_button_init();
	
	mode = wss_get_mode(from_slot, from_list, to_slot, to_list, -1);
	if ( mode >= 0 ) {
		ss_apply(mode, from_slot, from_list, to_slot, to_list,player_index);
	}	
}

// lock/unlock any necessary slots for multiplayer
void ss_recalc_multiplayer_slots()
{
	int				i,j,objnum;
	wing				*wp;
	ss_slot_info	*ss_slot;
	ss_wing_info	*ss_wing;
	
	// no wings
	if ( Wss_num_wings <= 0 ) {
		Wss_slots[0].ship_class = Team_data[Common_team].default_ship;;
		return;
	}

	for ( i = 0; i < Wss_num_wings; i++ ) {
		ss_wing = &Ss_wings[i];
		if ( ss_wing->wingnum < 0 ) {
			Int3();
			continue;
		}

		// NOTE : the method below will eventually have to change to account for all possible netgame options
		
		// get the wing pointer
		wp = &Wings[ss_wing->wingnum];		
		for ( j = 0; j < ss_wing->num_slots; j++ ) {				
			// get the objnum of the ship in this slot
			objnum = Ships[wp->ship_index[j]].objnum;

			// get the slot pointer
			ss_slot = &ss_wing->ss_slots[j];			
			
			if (ss_slot->sa_index == -1) {					
				// lock all slots by default
				ss_slot->status |= WING_SLOT_LOCKED;
				
				// if this is my slot, then unlock it
				if(!multi_ts_disabled_slot((i*4)+j)){				
					ss_slot->status &= ~WING_SLOT_LOCKED;
				}
			}
		}
	}
} 

