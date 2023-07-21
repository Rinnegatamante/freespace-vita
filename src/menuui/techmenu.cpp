/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MenuUI/TechMenu.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module that contains functions to drive the Tech Menu user interface
 *
 * $Log$
 * Revision 1.10  2006/04/26 19:42:25  taylor
 * fix techroom list selection so that it matches the original FS1 behavior
 * correct a stupid bitwise bug
 *
 * Revision 1.9  2005/03/29 02:18:47  taylor
 * Various 64-bit platform fixes
 * Fix compiler errors with MAKE_FS1 and fix gr_set_bitmap() too
 * Make sure that turrets can fire at asteroids for FS1 (needed for a couple missions)
 * Streaming audio support (big thanks to Pierre Willenbrock!!)
 * Removed dependance on strings.tbl for FS1 since we don't actually need it now
 *
 * Revision 1.8  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.7  2003/08/03 16:10:29  taylor
 * cleanup; compile warning fixes
 *
 * Revision 1.6  2003/05/25 02:30:42  taylor
 * Freespace 1 support
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
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 43    11/02/99 3:22p Jefff
 * translate ship names in tech room
 * 
 * 42    10/25/99 5:47p Jefff
 * reassigned some xstr ids
 * 
 * 41    10/12/99 4:50p Jefff
 * 
 * 40    9/08/99 11:09a Dave
 * Use stills for intel and weapon stuff in the techroom if animations
 * aren't avaliable.
 * 
 * 39    9/05/99 11:19p Dave
 * Made d3d texture cache much more safe. Fixed training scoring bug where
 * it would backout scores without ever having applied them in the first
 * place.
 * 
 * 38    9/03/99 11:19a Jefff
 * fixed double render of intel descriptions
 * 
 * 37    9/03/99 1:32a Dave
 * CD checking by act. Added support to play 2 cutscenes in a row
 * seamlessly. Fixed super low level cfile bug related to files in the
 * root directory of a CD. Added cheat code to set campaign mission # in
 * main hall.
 * 
 * 36    9/01/99 5:12p Jefff
 * Fixed bad scroll bug
 * 
 * 35    8/25/99 3:03p Jefff
 * 'more' indicator, lotsa coord changes for new background,  disabling of
 * buttons in trackball mode, list follows selected item when prev/next
 * buttons used.
 * 
 * 34    8/24/99 8:55p Dave
 * Make sure nondimming pixels work properly in tech menu.
 * 
 * 33    8/24/99 10:47a Jefff
 * tech room weapon anims.  added tech anim field to weapons.tbl
 * 
 * 32    8/10/99 5:29p Jefff
 * use new tech_title field in weapons array
 * 
 * 31    8/10/99 3:45p Jefff
 * Put the smack down on the tech room.  Its all new, but tastefully done.
 * 
 * 30    8/09/99 5:53p Jefff
 * 
 * 29    8/02/99 10:27a Jefff
 * removed a warning for the time being
 * 
 * 28    8/02/99 10:13a Jefff
 * started scrubbing, waiting till after demo to continue
 * 
 * 27    7/28/99 1:02p Jefff
 * Changed "Species" to "Intelligence" -- also changed in strings.tbl.
 * 
 * 26    7/26/99 4:33p Jordonr
 * Don't need techroom palette
 * 
 * 25    7/20/99 1:49p Dave
 * Peter Drake build. Fixed some release build warnings.
 * 
 * 24    7/19/99 2:13p Dave
 * Added some new strings for Heiko.
 * 
 * 23    7/14/99 9:42a Dave
 * Put in clear_color debug function. Put in base for 3dnow stuff / P3
 * stuff
 * 
 * 22    6/16/99 4:06p Dave
 * New pilot info popup. Added new draw-bitmap-as-poly function.
 * 
 * 21    5/09/99 8:57p Dave
 * Final E3 build preparations.
 * 
 * 20    5/09/99 6:00p Dave
 * Lots of cool new effects. E3 build tweaks.
 * 
 * 19    4/29/99 2:16p Neilk
 * added a 2nd callback for slider so its model is not loaded on mouselock
 * until mouse button is released
 * 
 * 18    4/26/99 5:05p Neilk
 * updated to new artwork, added slider support
 * 
 * 17    4/25/99 3:02p Dave
 * Build defines for the E3 build.
 * 
 * 16    4/23/99 12:01p Johnson
 * Added SIF_HUGE_SHIP
 * 
 * 15    4/12/99 10:07p Dave
 * Made network startup more forgiving. Added checkmarks to dogfight
 * screen for players who hit commit.
 * 
 * 14    4/08/99 2:10a Dave
 * Numerous bug fixes for the beta. Added builtin mission info for the
 * beta.
 * 
 * 13    2/19/99 11:42a Dave
 * Put in model rendering autocentering.
 * 
 * 12    2/01/99 5:55p Dave
 * Removed the idea of explicit bitmaps for buttons. Fixed text
 * highlighting for disabled gadgets.
 * 
 * 11    1/30/99 5:08p Dave
 * More new hi-res stuff.Support for nice D3D textures.
 * 
 * 10    1/29/99 3:56p Neilk
 * Converted tech_infos screens to multiresolution
 * 
 * 9     1/29/99 3:54p Neilk
 * 
 * 8     1/29/99 12:47a Dave
 * Put in sounds for beam weapon. A bunch of interface screens (tech
 * database stuff).
 * 
 * 7     11/30/98 1:07p Dave
 * 16 bit conversion, first run.
 * 
 * 6     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
 * 
 * 5     10/16/98 9:40a Andsager
 * Remove ".h" files from model.h
 * 
 * 4     10/13/98 2:47p Andsager
 * Remove reference to Tech_shivan_species_avail
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
 * 67    9/21/98 10:02p Dave
 * Last minute changes to techroom weapon/ship/species stuff.
 * 
 * 66    9/07/98 2:49p Dan
 * Removed spurious Assert
 * 
 * 65    7/06/98 2:42p Hoffoss
 * Fixed bug with weapons shown in tech database.
 * 
 * 64    6/10/98 11:48a Lawrance
 * fix bug with trying to free NULL anims
 * 
 * 63    6/05/98 9:54a Lawrance
 * OEM changes
 * 
 * 62    6/01/98 11:43a John
 * JAS & MK:  Classified all strings for localization.
 * 
 * 61    5/26/98 11:10a Lawrance
 * Fix bug where window controls get disabled when F1 pressed twice
 * 
 * 60    5/23/98 10:38p Lawrance
 * Avoid doing a cfile refresh when running debug
 * 
 * 59    5/23/98 6:49p Lawrance
 * Fix problems with refreshing the file list when a CD is inserted
 * 
 * 58    5/22/98 11:15a Lawrance
 * Tweak how CD gets asked for
 * 
 * 57    5/22/98 1:06a Hoffoss
 * Made Fred not use OLE.
 * 
 * 56    5/21/98 6:57p Lawrance
 * Only ask for the CD once
 * 
 * 55    5/21/98 12:46a Hoffoss
 * Made text offset reset when new description is loaded.
 * 
 * 54    5/20/98 9:46p John
 * added code so the places in code that change half the palette don't
 * have to clear the screen.
 * 
 * 53    5/20/98 3:28p Sandeep
 * Fixed bug where in multi only discovered weapons show up, and in single
 * all weapons showed up (backwards).
 * 
 * 52    5/20/98 2:11p Hoffoss
 * Fixed some species description problems.
 * 
 * 51    5/18/98 6:45p Hoffoss
 * Added species descriptions to species database in techroom.
 * 
 * 50    5/17/98 6:13p Hoffoss
 * Fixed scrolling of info text area. 
 *
 * $NoKeywords: $
 *
 */

#include "gamesequence.h"
#include "techmenu.h"
#include "2d.h"
#include "3d.h"
#include "snazzyui.h"
#include "managepilot.h"
#include "key.h"
#include "bmpman.h"
#include "timer.h"
#include "missioncampaign.h"
#include "missionshipchoice.h"
#include "freespace.h"
#include "mainhallmenu.h"
#include "missionscreencommon.h"
#include "gamesnd.h"
#include "font.h"
#include "vecmat.h"
#include "mouse.h"
#include "uidefs.h"
#include "contexthelp.h"
#include "alphacolors.h"
#include "animplay.h"
#include "localize.h"
#include "lighting.h"

#define REVOLUTION_RATE	5.2f

#ifdef MAKE_FS1
	#define NUM_BUTTONS 18
#else
	#define NUM_BUTTONS	16
#endif
#define NUM_TABS		3
#define LIST_BUTTONS_MAX	41

#define SHIPS_DATA_MODE		(1<<0)
#define WEAPONS_DATA_MODE	(1<<1)
#define SPECIES_DATA_MODE	(1<<2)
#define WEAPONS_SPECIES_DATA_MODE	(WEAPONS_DATA_MODE | SPECIES_DATA_MODE)

#define SHIPS_DATA_TAB					0
#define WEAPONS_DATA_TAB				1
#define INTEL_DATA_TAB					2
#define TECH_DATABASE_TAB				3
#define SIMULATOR_TAB					4
#define CUTSCENES_TAB					5
#define CREDITS_TAB						6

#define SCROLL_LIST_UP					7
#define SCROLL_LIST_DOWN				8
#define SCROLL_INFO_UP					9
#define SCROLL_INFO_DOWN				10

#define PREV_ENTRY_BUTTON				11
#define NEXT_ENTRY_BUTTON				12

#define HELP_BUTTON						13
#define OPTIONS_BUTTON					14
#define EXIT_BUTTON						15

#ifdef MAKE_FS1
#define PREV_ENTRY_BUTTON2				16
#define NEXT_ENTRY_BUTTON2				17
#endif


#define REPEAT						(1<<0)
#define NO_MOUSE_OVER_SOUND	(1<<1)

// indicies for coords
#define SHIP_X_COORD 0 
#define SHIP_Y_COORD 1
#define SHIP_W_COORD 2
#define SHIP_H_COORD 3

// background filename for species
// note weapon filename is now same as ship filename
const char *Tech_background_filename[GR_NUM_RESOLUTIONS] = {
	"TechShipData",
	"2_TechShipData"
};
const char *Tech_mask_filename[GR_NUM_RESOLUTIONS] = {
	"TechShipData-M",
	"2_TechShipData-M"
};
#ifndef MAKE_FS1
const char *Tech_slider_filename[GR_NUM_RESOLUTIONS] = {
	"slider",
	"2_slider"
};
#endif

#ifdef MAKE_FS1
// need the second background for FS1
const char *Tech_data_background_filename = {
	"TechData"
};
const char *Tech_data_mask_filename = {
	"TechData-M"
};
#endif

int Tech_list_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		35, 131, 178, 198
#else
		27, 98, 161, 234
#endif
	},
	{ // GR_1024
		43, 157, 253, 374
	}
};

int Tech_ship_display_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		234, 115, 390, 229
#else
		239, 98, 393, 222
#endif
	},
	{ // GR_1024
		382, 158, 629, 355
	}
};

int Tech_desc_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		22, 362, 382, 64
#else
		29, 347, 365, 125
#endif
	},
	{ // GR_1024
		47, 555, 584, 200
	}
};

int Tech_ani_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		424, 140
#else
		196, 115
#endif
	},
	{ // GR_1024
		449, 245
	}
};

#ifdef MAKE_FS1
int Tech_data_desc_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
		24, 139, 376, 281
	},
	{ // GR_1024
		24, 182, 638, 528
	}
};
#endif

#ifndef MAKE_FS1
int Tech_slider_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
		2, 118, 20, 194
	},
	{ // GR_1024
		3, 190, 32, 310
	}
};
#endif

// detail backup
int Tech_detail_backup;
int Tech_texture_backup;

#define MAX_TEXT_LINES		100
#define MAX_TEXT_LINE_LEN	256

struct techroom_buttons {
	const char *filename;
	int x, y, xt, yt;
	int hotspot;
	int tab;
	int flags;
	UI_BUTTON button;  // because we have a class inside this struct, we need the constructor below..

	techroom_buttons(const char *name, int x1, int y1, int xt1, int yt1, int h, int t, int f = 0) : filename(name), x(x1), y(y1), xt(xt1), yt(yt1), hotspot(h), tab(t), flags(f) {}
};

static techroom_buttons Buttons[GR_NUM_RESOLUTIONS][NUM_BUTTONS] = {
	{	// GR_640
#ifdef MAKE_FS1
		techroom_buttons("TDB_04",	20,		429,	-1,	-1,	4,	-1),							// ship data tab
		techroom_buttons("TDB_05",	70,		429,	-1,	-1,	5,	-1),							// weapons data tab
		techroom_buttons("TDB_06",	147,	429,	-1,	-1,	6,	-1),							// species data tab
		techroom_buttons("TDB_00",	0,		0,		-1,	-1,	0,	-1),							// technical database tab
		techroom_buttons("TDB_01",	0,		19,		-1,	-1,	1,	-1),							// mission simulator tab
		techroom_buttons("TDB_02",	0,		35,		-1,	-1,	2,	-1),							// cutscenes tab
		techroom_buttons("TDB_03",	0,		56,		-1,	-1,	3,	-1),							// credits tab
		techroom_buttons("TSB_16",	0,		206,	-1,	-1,	16,	SHIPS_DATA_MODE),				// list up
		techroom_buttons("TSB_17",	0,		254,	-1,	-1,	17,	SHIPS_DATA_MODE),				// list down
		techroom_buttons("TSB_18",	411,	351,	-1,	-1,	18,	SHIPS_DATA_MODE),				// description up
		techroom_buttons("TSB_19",	411,	397,	-1,	-1,	19,	SHIPS_DATA_MODE),				// description down
//		techroom_buttons("TDB_11",	558,	272,	-1,	-1,	11, WEAPONS_SPECIES_DATA_MODE),		// prev data entry
//		techroom_buttons("TDB_12",	606,	272,	-1,	-1,	12, WEAPONS_SPECIES_DATA_MODE),		// next data entry
		techroom_buttons("TSB_22",	477,	338,	-1,	-1,	22, SHIPS_DATA_MODE),				// prev data entry
		techroom_buttons("TSB_23",	524,	338,	-1,	-1,	23, SHIPS_DATA_MODE),				// next data entry
		techroom_buttons("TDB_13",  469,	425,	-1,	-1,	13, -1),							// help
		techroom_buttons("TDB_14",  448,	452,	-1,	-1,	14, -1),							// options
		techroom_buttons("TDB_15",	552,	411,	-1,	-1,	15, -1),							// exit
		techroom_buttons("TDB_11",	477,	291,	-1,	-1,	11, WEAPONS_SPECIES_DATA_MODE),		// prev data entry
		techroom_buttons("TDB_12",	524,	291,	-1,	-1,	12, WEAPONS_SPECIES_DATA_MODE),		// next data entry
#else
		techroom_buttons("TDB_04",	406,	384,	447,	393,	4,	-1),											// ship data tab
		techroom_buttons("TDB_05",	404,	418,	447,	429,	5,	-1),											// weapons data tab
		techroom_buttons("TDB_06",	404,	447,	447,	461,	6,	-1),											// species data tab
		techroom_buttons("TDB_00",	7,		3,		37,	7,		0,	-1),											// technical database tab
		techroom_buttons("TDB_01",	7,		18,	37,	23,	1,	-1),											// mission simulator tab
		techroom_buttons("TDB_02",	7,		34,	37,	38,	2,	-1),											// cutscenes tab
		techroom_buttons("TDB_03",	7,		49,	37,	54,	3,	-1),											// credits tab
		techroom_buttons("TDB_07",	1,		86,	-1,	-1,	7,	SHIPS_DATA_MODE, REPEAT),				// prev data entry
		techroom_buttons("TDB_08",	1,		317,	-1,	-1,	8,	SHIPS_DATA_MODE, REPEAT),				// next data entry
		techroom_buttons("TDB_09",	1,		406,	-1,	-1,	9,	SHIPS_DATA_MODE, REPEAT),				// prev data entry
		techroom_buttons("TDB_10",	1,		447,	-1,	-1,	10,	SHIPS_DATA_MODE, REPEAT),			// next data entry
		//techroom_buttons("TDB_11",	558,	272,	-1,	-1,	11,	WEAPONS_SPECIES_DATA_MODE),		// prev data entry
		//techroom_buttons("TDB_12",	606,	272,	-1,	-1,	12,	WEAPONS_SPECIES_DATA_MODE),		// next data entry
		techroom_buttons("TDB_11a",559,	323,	-1,	-1,	11,	SHIPS_DATA_MODE, REPEAT),			// prev data entry
		techroom_buttons("TDB_12a",609,	323,	-1,	-1,	12,	SHIPS_DATA_MODE, REPEAT),			// next data entry
		techroom_buttons("TDB_13",	533,	425,	500,	440,	13,	-1),										// help
		techroom_buttons("TDB_14",	533,	455,	479,	464,	14,	-1),										// options
		techroom_buttons("TDB_15a",571,	425,	588,	413,	15,	-1),										// exit		
#endif
	}, 
	{	// GR_1024
		techroom_buttons("2_TDB_04",	649,	614,	717,	630,	4,	-1),										// ship data tab
		techroom_buttons("2_TDB_05",	646,	669,	717,	687,	5,	-1),										// weapons data tab
		techroom_buttons("2_TDB_06",	646,	716,	717,	739,	6,	-1),										// species data tab
		techroom_buttons("2_TDB_00",	12,	5,		59,	12,	0,	-1),										// technical database tab
		techroom_buttons("2_TDB_01",	12,	31,	59,	37,	1,	-1),										// mission simulator tab
		techroom_buttons("2_TDB_02",	12,	56,	59,	62,	2,	-1),										// cutscenes tab
		techroom_buttons("2_TDB_03",	12,	81,	59,	88,	3,	-1),										// credits tab
		techroom_buttons("2_TDB_07",	1,		138,	-1,	-1,	7,	SHIPS_DATA_MODE, REPEAT),			// prev data entry
		techroom_buttons("2_TDB_08",	1,		507,	-1,	-1,	8,	SHIPS_DATA_MODE, REPEAT),			// next data entry
		techroom_buttons("2_TDB_09",	1,		649,	-1,	-1,	9,	SHIPS_DATA_MODE, REPEAT),			// prev data entry
		techroom_buttons("2_TDB_10",	1,		716,	-1,	-1,	10,	SHIPS_DATA_MODE, REPEAT),		// next data entry
#ifdef MAKE_FS1
		// filler
		techroom_buttons("2_TDB_11",	893,	436,	-1,	-1,	11,	WEAPONS_SPECIES_DATA_MODE),	// prev data entry
		techroom_buttons("2_TDB_12",	970,	436,	-1,	-1,	12,	WEAPONS_SPECIES_DATA_MODE),	// next data entry
#endif
		techroom_buttons("2_TDB_11a",	895,	518,	-1,	-1,	11,	SHIPS_DATA_MODE, REPEAT),		// prev data entry
		techroom_buttons("2_TDB_12a",	974,	518,	-1,	-1,	12,	SHIPS_DATA_MODE, REPEAT),		// next data entry
		techroom_buttons("2_TDB_13",	854,	681,	800,	704,	13,	-1),									// help
		techroom_buttons("2_TDB_14",	854,	728,	780,	743,	14,	-1),									// options
		techroom_buttons("2_TDB_15a",	914,	681,	930,	660,	15,	-1),									// exit		
	}, 
};

static UI_WINDOW Ui_window;
static UI_BUTTON View_window;
//static int Background_bitmap;
#ifdef MAKE_FS1
static int Tech_data_background_bitmap;
#endif
static int Tech_background_bitmap;
static int Tab = 0;
// static int List_size;
static int List_offset;
static int Select_tease_line;
static int Trackball_mode = 1;
static int Trackball_active = 0;
static matrix Techroom_ship_orient = IDENTITY_MATRIX;
// static int Tech_room_ask_for_cd;

static int Text_size;
static int Text_offset;
static int Text_line_size[MAX_TEXT_LINES];
static char *Text_lines[MAX_TEXT_LINES];

static int Cur_entry;				// this is the current entry selected, using entry indexing
static int Cur_entry_index;		// this is the current entry selected, using master list indexing
static int Techroom_ship_modelnum;
static float Techroom_ship_rot;
static UI_BUTTON List_buttons[LIST_BUTTONS_MAX];  // buttons for each line of text in list
static int Anim_playing_id = -1;
static anim_instance *Cur_anim_instance = NULL;
//static int Palette_bmp;
#ifdef MAKE_FS1
static int ShipWin01;
static int ShipWin02;
static int ShipWin03;
static int ShipWin04;
#endif
static ubyte Palette[768];
//static char Palette_name[128];	// no longer used

static int Ships_loaded = 0;
static int Weapons_loaded = 0;
static int Intel_loaded = 0;

// out entry data struct & vars
typedef struct {
	int	index;		// index into the master table that its in (ie Ship_info[])
	char* name;			// ptr to name string
	char* desc;			// ptr to description string
	anim* animation;	// ptr to the animation
	int	bitmap;		// bitmap handle
	int	has_anim;	// flag to indicate the presence of an animation for this item
} tech_list_entry;

static tech_list_entry Ship_list[MAX_SHIP_TYPES];
static int Ship_list_size = 0;
static tech_list_entry Weapon_list[MAX_WEAPON_TYPES];
static int Weapon_list_size = 0;
static tech_list_entry Intel_list[MAX_INTEL_ENTRIES];
static int Intel_list_size = 0;
static tech_list_entry *Current_list;								// points to currently valid display list
static int Current_list_size = 0;

#ifndef MAKE_FS1
// slider stuff
static UI_SLIDER2 Tech_slider;
#endif

//XSTR:OFF
#ifdef MAKE_FS1
static const char *Intel_anim_filenames[MAX_INTEL_ENTRIES] = {
	"tech_tpilot.ani",
	"tech_vasudan.ani",
	"tech_shivan.ani",
};
#endif
//XSTR:ON

// Intelligence master data structs (these get inited @ game startup from species.tbl)
intel_data Intel_info[MAX_INTEL_ENTRIES];
int Intel_info_size = 0;

// some prototypes to make you happy
int techroom_load_ani(anim **animpp, char *name);
void tech_common_render();
void techroom_start_anim();
void tech_scroll_list_up();
void tech_scroll_list_down();
#ifdef MAKE_FS1
void techroom_data_init();
void techroom_tab_setup(int set_palette);
#endif




////////////////////////////////////////////////////
// like, functions and stuff

void techroom_init_desc(char *src, int w)
{
	Text_size = Text_offset = 0;
	if (!src) {
		return;
	}

	Text_size = split_str(src, w, Text_line_size, Text_lines, MAX_TEXT_LINES);
	Assert(Text_size >= 0 && Text_size < MAX_TEXT_LINES);
}

void techroom_select_new_entry()
{
	Assert(Current_list != NULL);
	if (Current_list == NULL) return;

	Cur_entry_index = Current_list[Cur_entry].index;

	// if we are in the ships tab, load the ship model
	if (Tab == SHIPS_DATA_TAB) {
		ship_info *sip = &Ship_info[Cur_entry_index];

#ifdef MULTIPLAYER_BETA_BUILD
		// don't load supercaps in the beta
		if((sip->flags & SIF_SUPERCAP) || (sip->flags & SIF_DRYDOCK)){
			Techroom_ship_modelnum = -1;
		} else {
			Techroom_ship_modelnum = model_load(sip->pof_file, sip->n_subsystems, &sip->subsystems[0]);
		}

		// page in ship textures properly (takes care of nondimming pixels)
		model_page_in_textures(Techroom_ship_modelnum, Cur_entry_index);
#else
		Techroom_ship_modelnum = model_load(sip->pof_file, sip->n_subsystems, &sip->subsystems[0]);

		// page in ship textures properly (takes care of nondimming pixels)
		model_page_in_textures(Techroom_ship_modelnum, Cur_entry_index);
#endif
	} else {
		Techroom_ship_modelnum = -1;
		Trackball_mode = 0;
	}

//	Techroom_ship_rot = PI;

#ifdef MAKE_FS1
	if (Tab == (WEAPONS_DATA_TAB || INTEL_DATA_TAB)){
		techroom_init_desc(Current_list[Cur_entry].desc, Tech_data_desc_coords[gr_screen.res][SHIP_W_COORD]);
	} else {
		techroom_init_desc(Current_list[Cur_entry].desc, Tech_desc_coords[gr_screen.res][SHIP_W_COORD]);
	}
#else
	techroom_init_desc(Current_list[Cur_entry].desc, Tech_desc_coords[gr_screen.res][SHIP_W_COORD]);
#endif
	techroom_start_anim();
}

// write out the current description in the bottom window
void techroom_render_desc(int xo, int yo, int h)
{
	int y, z, len, font_height;
	char line[MAX_TEXT_LINE_LEN + 1];

	font_height = gr_get_font_height();

	y = 0;
	z = Text_offset;
	while (y + font_height <= h) {
		if (z >= Text_size){
			break;
		}

		len = Text_line_size[z];
		if (len > MAX_TEXT_LINE_LEN){
			len = MAX_TEXT_LINE_LEN;
		}

		strncpy(line, Text_lines[z], len);
		line[len] = 0;
		gr_string(xo, yo + y, line);

		y += font_height;
		z++;
	}

	// maybe output 'more' indicator
#ifndef MAKE_FS1   // not used in original
	if ( z < Text_size ) {
		// can be scrolled down
		int more_txt_x = Tech_desc_coords[gr_screen.res][0] + (Tech_desc_coords[gr_screen.res][2]/2) - 10;	// FIXME should move these to constants since they dont move
		int more_txt_y = Tech_desc_coords[gr_screen.res][1] + Tech_desc_coords[gr_screen.res][3];				// located below brief text, centered
		int w, h;
		gr_get_string_size(&w, &h, XSTR("more", 1469), strlen(XSTR("more", 1469)));
		gr_set_color_fast(&Color_black);
		gr_rect(more_txt_x-2, more_txt_y, w+3, h);
		gr_set_color_fast(&Color_red);
		gr_string(more_txt_x, more_txt_y, XSTR("more", 1469));  // base location on the input x and y?
	}
#endif

}

// new version of weapons
void techroom_weapons_render2(float frametime)
{
	// render common stuff
#ifndef MAKE_FS1
	tech_common_render();
#else
	// render description in its box
	gr_set_color_fast(&Color_text_normal);
	techroom_render_desc(Tech_data_desc_coords[gr_screen.res][SHIP_X_COORD], Tech_data_desc_coords[gr_screen.res][SHIP_Y_COORD], Tech_data_desc_coords[gr_screen.res][SHIP_H_COORD]);
#endif

	// render the animation
	{
		// JAS: This code is hacked to allow the animation to use all 256 colors
		extern int Palman_allow_any_color;
		Palman_allow_any_color = 1;
		anim_render_all(0, frametime);
		Palman_allow_any_color = 0;
	}

	// if our active item has a bitmap instead of an animation, draw it
	if((Cur_entry > 0) && (Current_list[Cur_entry].animation == NULL) && (Current_list[Cur_entry].bitmap >= 0)){
		gr_set_bitmap(Current_list[Cur_entry].bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(Tech_ani_coords[gr_screen.res][0], Tech_ani_coords[gr_screen.res][1]);
	}
}

// renders the stuff common to all 3 tech room tabs
void tech_common_render()
{
	char buf[256];
	int y, z, font_height;

	// render description in its box
	gr_set_color_fast(&Color_text_normal);
	techroom_render_desc(Tech_desc_coords[gr_screen.res][SHIP_X_COORD], Tech_desc_coords[gr_screen.res][SHIP_Y_COORD], Tech_desc_coords[gr_screen.res][SHIP_H_COORD]);

	font_height = gr_get_font_height();

	// draw the list of entries
	y = 0;
	z = List_offset;
	while (y + font_height <= Tech_list_coords[gr_screen.res][SHIP_H_COORD]) {
		if (z >= Current_list_size) {
			break;
		}

		if (z == Cur_entry) {
			gr_set_color_fast(&Color_text_selected);
		} else if (z == Select_tease_line) {
			gr_set_color_fast(&Color_text_subselected);
		} else {
			gr_set_color_fast(&Color_text_normal);
		}

		strcpy(buf, Current_list[z].name);
		if (Lcl_gr) {
			lcl_translate_ship_name(buf);
		}

		gr_force_fit_string(buf, 255, Tech_list_coords[gr_screen.res][SHIP_W_COORD]);
		gr_string(Tech_list_coords[gr_screen.res][SHIP_X_COORD], Tech_list_coords[gr_screen.res][SHIP_Y_COORD] + y, buf);

		List_buttons[z - List_offset].update_dimensions(Tech_list_coords[gr_screen.res][SHIP_X_COORD], Tech_list_coords[gr_screen.res][SHIP_Y_COORD] + y, Tech_list_coords[gr_screen.res][SHIP_W_COORD], font_height);
		List_buttons[z - List_offset].enable(1);

		y += font_height;
		z++;
	}

	// disable the rest of the list buttons
	z -= List_offset;
	while (z < LIST_BUTTONS_MAX) {
		List_buttons[z++].disable();
	}
}

void techroom_ships_render(float frametime)
{
	// render all the common stuff
	tech_common_render();

	// now render the trackball ship, which is unique to the ships tab
	float rev_rate;
	angles rot_angles, view_angles;
	int z;
	ship_info *sip = &Ship_info[Cur_entry_index];

	// get correct revolution rate
	rev_rate = REVOLUTION_RATE;
	z = sip->flags;
	if (z & SIF_BIG_SHIP) {
		rev_rate *= 1.7f;
	}
	if (z & SIF_HUGE_SHIP) {
		rev_rate *= 3.0f;
	}

	// rotate the ship as much as required for this frame
	Techroom_ship_rot += PI2 * frametime / rev_rate;
	while (Techroom_ship_rot > PI2){
		Techroom_ship_rot -= PI2;	
	}

	// turn off fogging
	gr_fog_set(GR_FOGMODE_NONE, 0, 0, 0, -1.0f, -1.0f);

	//	reorient ship
	if (Trackball_active) {
		int dx, dy;
		matrix mat1, mat2;

		if (Trackball_active) {
			mouse_get_delta(&dx, &dy);
			if (dx || dy) {
				vm_trackball(-dx, -dy, &mat1);
				vm_matrix_x_matrix(&mat2, &mat1, &Techroom_ship_orient);
				Techroom_ship_orient = mat2;
			}
		}

	} else {
		// setup stuff needed to render the ship
		view_angles.p = -0.6f;
		view_angles.b = 0.0f;
		view_angles.h = 0.0f;
		vm_angles_2_matrix(&Techroom_ship_orient, &view_angles);

		rot_angles.p = 0.0f;
		rot_angles.b = 0.0f;
		rot_angles.h = Techroom_ship_rot;
		vm_rotate_matrix_by_angles(&Techroom_ship_orient, &rot_angles);
	}

	gr_set_clip(Tech_ship_display_coords[gr_screen.res][SHIP_X_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_Y_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_W_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_H_COORD]);	

	// render the ship
#ifdef MULTIPLAYER_BETA_BUILD
	if((sip->flags & SIF_SUPERCAP) || (sip->flags & SIF_DRYDOCK)) {
		gr_set_color_fast(&Color_bright);
		gr_string(Tech_ship_display_coords[gr_screen.res][SHIP_X_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_Y_COORD] + 50, NOX("No soup for you!"));
	} else {
		g3_start_frame(1);

		g3_set_view_matrix(&sip->closeup_pos, &vmd_identity_matrix, sip->closeup_zoom * 1.3f);

		// lighting for techroom
		light_reset();
		vector light_dir = vmd_zero_vector;
		light_dir.y = 1.0f;	
		light_add_directional(&light_dir, 0.85f, 1.0f, 1.0f, 1.0f);
		// light_filter_reset();
		light_rotate_all();
		// lighting for techroom

		model_clear_instance(Techroom_ship_modelnum);
		model_set_detail_level(0);
		model_render(Techroom_ship_modelnum, &Techroom_ship_orient, &vmd_zero_vector, MR_NO_LIGHTING | MR_LOCK_DETAIL | MR_AUTOCENTER);

		g3_end_frame();
	}
#else
	g3_start_frame(1);

	g3_set_view_matrix(&sip->closeup_pos, &vmd_identity_matrix, sip->closeup_zoom * 1.3f);

	// lighting for techroom
	light_reset();
	vector light_dir = vmd_zero_vector;
	light_dir.xyz.y = 1.0f;
	light_add_directional(&light_dir, 0.85f, 1.0f, 1.0f, 1.0f);
	// light_filter_reset();
	light_rotate_all();
	// lighting for techroom

	model_clear_instance(Techroom_ship_modelnum);
	model_set_detail_level(0);
	model_render(Techroom_ship_modelnum, &Techroom_ship_orient, &vmd_zero_vector, MR_LOCK_DETAIL | MR_AUTOCENTER);

	g3_end_frame();
#endif

	gr_reset_clip();
}

// select previous entry in current list
void tech_prev_entry()
{
	Cur_entry--;
	if (Cur_entry < 0) {
		Cur_entry = Current_list_size - 1;

		// scroll to end of list
		List_offset = Cur_entry - Tech_list_coords[gr_screen.res][SHIP_H_COORD] / gr_get_font_height() + 1;
		if (List_offset < 0) {
			// this happens when there are not enough items to scroll
			List_offset = 0;
		}
#ifndef MAKE_FS1
		Tech_slider.force_currentItem(Tech_slider.get_numberItems());
#endif
	} else {
		// maybe adjust list position by 1
		if (List_offset > Cur_entry) {
			tech_scroll_list_up();
#ifndef MAKE_FS1
			Tech_slider.forceUp();
#endif
		}
	}

	techroom_select_new_entry();
	gamesnd_play_iface(SND_SCROLL);
}

// select next entry in current list
void tech_next_entry()
{
	Cur_entry++;
	if (Cur_entry >= Current_list_size) {
		Cur_entry = 0;

		// scroll to beginning of list
		List_offset = 0;
#ifndef MAKE_FS1
		Tech_slider.force_currentItem(Cur_entry);
#endif
	} else {
		// maybe adjust list position by 1
		if (List_offset + Tech_list_coords[gr_screen.res][SHIP_H_COORD] / gr_get_font_height() <= Cur_entry) {
			tech_scroll_list_down();
#ifndef MAKE_FS1
			Tech_slider.forceDown();
#endif
		}
	}

	techroom_select_new_entry();
	gamesnd_play_iface(SND_SCROLL);
}

void tech_scroll_info_up()
{
	if (Text_offset) {
		Text_offset--;
		gamesnd_play_iface(SND_SCROLL);
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

void tech_scroll_info_down()
{
	int h;

	if (Tab == SHIPS_DATA_TAB){
		h = Tech_desc_coords[gr_screen.res][SHIP_H_COORD];
	} else {
		h = Tech_desc_coords[gr_screen.res][3];
	}

	if (Text_offset + h / gr_get_font_height() < Text_size) {
		Text_offset++;
		gamesnd_play_iface(SND_SCROLL);
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

void tech_scroll_list_up()
{
	//int last;

	if (List_offset > 0) {
		List_offset--;
		//last = List_offset + Tech_list_coords[gr_screen.res][SHIP_H_COORD] / gr_get_font_height() - 1;

#ifdef MAKE_FS1
		if ( (List_offset + Tech_list_coords[gr_screen.res][SHIP_H_COORD] / gr_get_font_height() - 1) < Cur_entry ) {
			Cur_entry--;
			techroom_select_new_entry();
		}
#endif
		gamesnd_play_iface(SND_SCROLL);
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

void tech_scroll_list_down()
{
	if (List_offset + Tech_list_coords[gr_screen.res][SHIP_H_COORD] / gr_get_font_height() < Current_list_size) {
		List_offset++;

#ifdef MAKE_FS1
		if ( (List_offset + Tech_list_coords[gr_screen.res][SHIP_H_COORD] / gr_get_font_height()) > Cur_entry ) {
			Cur_entry++;
			techroom_select_new_entry();
		}
#endif
		gamesnd_play_iface(SND_SCROLL);
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

// this doesnt do a damn thing...
void tech_ship_scroll_capture()
{
	techroom_select_new_entry();
}


// this is obsolete - see techroom_weapons_render2(...)
void techroom_weapons_render(float frametime)
{
	gr_set_color_fast(&Color_text_normal);
	techroom_render_desc(Tech_desc_coords[gr_screen.res][0], Tech_desc_coords[gr_screen.res][1], Tech_desc_coords[gr_screen.res][3]);

	{
		// JAS: This code is hacked to allow the animation to use all 256 colors
		extern int Palman_allow_any_color;
		Palman_allow_any_color = 1;
		anim_render_all(0, frametime);
		Palman_allow_any_color = 0;
	}

	// if our active item has a bitmap instead of an animation, draw it
	if((Cur_entry >= 0) && (Current_list[Cur_entry].animation == NULL) && (Current_list[Cur_entry].bitmap >= 0)){
		gr_set_bitmap(Current_list[Cur_entry].bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(Tech_ani_coords[gr_screen.res][0], Tech_ani_coords[gr_screen.res][1]);
	}
}

void techroom_intel_render(float frametime)
{
#ifndef MAKE_FS1
	tech_common_render();
#else
	// render description in its box
	gr_set_color_fast(&Color_text_normal);
	techroom_render_desc(Tech_data_desc_coords[gr_screen.res][SHIP_X_COORD], Tech_data_desc_coords[gr_screen.res][SHIP_Y_COORD], Tech_data_desc_coords[gr_screen.res][SHIP_H_COORD]);
#endif

	{
		// JAS: This code is hacked to allow the animation to use all 256 colors
		extern int Palman_allow_any_color;
		Palman_allow_any_color = 1;
		anim_render_all(0, frametime);
		Palman_allow_any_color = 0;
	}

	// if our active item has a bitmap instead of an animation, draw it
	if((Cur_entry >= 0) && (Current_list[Cur_entry].animation == NULL) && (Current_list[Cur_entry].bitmap >= 0)){
		gr_set_bitmap(Current_list[Cur_entry].bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(Tech_ani_coords[gr_screen.res][0], Tech_ani_coords[gr_screen.res][1]);
	}
}

void techroom_stop_anim(int id)
{
	if (Cur_anim_instance && (id != Anim_playing_id)) {
		anim_stop_playing(Cur_anim_instance);
		Cur_anim_instance = NULL;
	}
}

void techroom_start_anim()
{
	int id;
	anim *animp;
	anim_play_struct aps;

	if (Cur_entry < 0) {
		techroom_stop_anim(-1);
		Anim_playing_id = -1;
	}

	if (Tab == WEAPONS_DATA_TAB) {
		id = Cur_entry;
	} else {
		id = Cur_entry + 2000;						// this offset is arbitrary?
	}

	techroom_stop_anim(id);

	// if we actually have an animation
	if(Current_list[Cur_entry].animation != NULL){
		animp = Current_list[Cur_entry].animation;	

		if (id != Anim_playing_id) {
			Anim_playing_id = -1;
			if (animp) {
				anim_play_init(&aps, animp, Tech_ani_coords[gr_screen.res][0], Tech_ani_coords[gr_screen.res][1]);
				aps.looped = 1;
				Cur_anim_instance = anim_play(&aps);
				Anim_playing_id = id;
			}

			if (animp) {
				memcpy(Palette, animp->palette, 384);
				gr_set_palette(animp->name, Palette, 1);
			}
		}
	}
}


void techroom_change_tab(int num)
{
	int i, multi = 0, mask, font_height, max_num_entries_viewable;	

	Tab = num;
	// Assert(Current_list_size >= 0);
	List_offset = 0;
	Cur_entry = 0;
	multi = Player->flags & PLAYER_FLAGS_IS_MULTI;

	for (i=0; i<LIST_BUTTONS_MAX; i++){
		List_buttons[i].disable();
	}

	// disable some stuff in multiplayer
	if (Player->flags & PLAYER_FLAGS_IS_MULTI) {
		Buttons[gr_screen.res][SIMULATOR_TAB].button.disable();
		Buttons[gr_screen.res][CUTSCENES_TAB].button.disable();
	}

	switch (Tab) {
		case SHIPS_DATA_TAB:
			mask = multi ? SIF_IN_TECH_DATABASE_M : SIF_IN_TECH_DATABASE;
			
			// load ship info if necessary
			if (Ships_loaded == 0) {
				Ship_list_size = 0;
				for (i=0; i<Num_ship_types; i++) {
#ifndef MAKE_FS1
					if (Ship_info[i].flags & mask) {
#else
					// make sure it has a description before displaying
					if ((Ship_info[i].flags & mask) && (Ship_info[i].tech_desc)) {
#endif
						// this ship should be displayed, fill out the entry struct
						Ship_list[Ship_list_size].bitmap = -1;
						Ship_list[Ship_list_size].index = i;
						Ship_list[Ship_list_size].animation = NULL;			// no anim for ships
						Ship_list[Ship_list_size].has_anim = 0;				// no anim for ships
						Ship_list[Ship_list_size].name = Ship_info[i].name;
						Ship_list[Ship_list_size].desc = Ship_info[i].tech_desc;
						Ship_list_size++;
					}				
				}
				Ships_loaded = 1;
			}

			Current_list = Ship_list;
			Current_list_size = Ship_list_size;

			font_height = gr_get_font_height();
			max_num_entries_viewable = Tech_list_coords[gr_screen.res][SHIP_H_COORD] / font_height;
#ifndef MAKE_FS1
			Tech_slider.set_numberItems(Current_list_size > max_num_entries_viewable ? Current_list_size-max_num_entries_viewable : 0);
#endif

			// no anim to start here
			break;

		case WEAPONS_DATA_TAB:
#ifdef MAKE_FS1
			techroom_data_init();
#endif
			// load weapon info & anims if necessary
			if (Weapons_loaded == 0) {
				Weapon_list_size = 0;
				mask = multi ? WIF_PLAYER_ALLOWED : WIF_IN_TECH_DATABASE;

				for (i=0; i<MAX_WEAPON_TYPES; i++) {
					if (Weapon_info[i].wi_flags & mask) { 
#ifndef MAKE_FS1
						// note: hack here to exclude dogfight weapons -- dont put weapon in if it has same description as pvs weapon
						if ((Weapon_list_size > 0) && (!strcmp(Weapon_info[i].tech_desc, Weapon_list[Weapon_list_size-1].desc))) {
#else
						// exclude entries with no description, basically the same as above but helps get rid of special cases as well
						if ((Weapon_list_size > 0) && (!Weapon_info[i].tech_desc)) {
#endif
							continue;
						}

						// we have a weapon that should be in the tech db, so fill out the entry struct
						Weapon_list[Weapon_list_size].index = i;
						Weapon_list[Weapon_list_size].desc = Weapon_info[i].tech_desc;
						Weapon_list[Weapon_list_size].has_anim = 1;
						Weapon_list[Weapon_list_size].name = Weapon_info[i].tech_title;
						Weapon_list[Weapon_list_size].bitmap = -1;
						if (Weapon_list[Weapon_list_size].name[0] == 0) {
							Weapon_list[Weapon_list_size].name = Weapon_info[i].name;
						}

#ifdef MAKE_FS1
						// figure out the animation based on weapon name
						if (!strcmp(Weapon_info[i].name, "ML-16 Laser")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_ML16.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Disruptor")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Disruptor.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "D-Advanced")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_DAdvanced.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Avenger")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Avenger.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Flail")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Flail.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Prometheus")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Prometheus.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Banshee")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Banshee.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "MX-50")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_MX50.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "D-Missile")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_DisruptorMissile.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Fury")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Fury.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Hornet")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Hornet.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Interceptor")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Interceptor.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Phoenix V")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Phoenix.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Synaptic")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Synaptic.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Stiletto")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Stiletto.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Tsunami")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Tsunami.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Harbinger")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_Harbinger.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Leech Cannon")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_leech.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "EM Pulse")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_empulse.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "S-Breaker")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_sbreaker.ani", NAME_LENGTH);
						} else if (!strcmp(Weapon_info[i].name, "Cluster Bomb")) {
							strncpy(Weapon_info[i].tech_anim_filename, "CB_cluster.ani", NAME_LENGTH);
						}
#endif

						// load the weapon animation
						if(!techroom_load_ani(&Weapon_list[Weapon_list_size].animation, Weapon_info[i].tech_anim_filename)){
							Weapon_list[Weapon_list_size].has_anim = 0;
							Weapon_list[Weapon_list_size].animation = NULL;

							// hmm. try a bitmap instead
							Weapon_list[Weapon_list_size].bitmap = bm_load(Weapon_info[i].tech_anim_filename);
						}						

						Weapon_list_size++;
					}				
				}

				Weapons_loaded = 1;
			}

			Current_list = Weapon_list;
			Current_list_size = Weapon_list_size;

			font_height = gr_get_font_height();
			max_num_entries_viewable = Tech_list_coords[gr_screen.res][SHIP_H_COORD] / font_height;
#ifndef MAKE_FS1
			Tech_slider.set_numberItems(Current_list_size > max_num_entries_viewable ? Current_list_size-max_num_entries_viewable : 0);
#endif

			techroom_start_anim();
			break;

		case INTEL_DATA_TAB:

#ifdef MAKE_FS1
			techroom_data_init();
#endif
			// load intel if necessary
			if ( Intel_loaded == 0 ) {
				// now populate the entry structs
				Intel_list_size = 0;
				for (i=0; i<Intel_info_size; i++) {

					if (Intel_info[i].in_tech_db != 0) {
						// leave option for no animation if string == "none"
						if (!strcmp(Intel_info[i].anim_filename, "none")) {
							Intel_list[Intel_list_size].has_anim = 0;
							Intel_list[Intel_list_size].animation = NULL;
						} else {
							// try and load as an animation
							Intel_list[Intel_list_size].has_anim = 0;
							Intel_list[Intel_list_size].bitmap = -1;
							if(techroom_load_ani(&Intel_list[Intel_list_size].animation, Intel_info[i].anim_filename)){
								Intel_list[Intel_list_size].has_anim = 1;							
							} else {
								Intel_list[Intel_list_size].bitmap = bm_load(Intel_info[i].anim_filename);
							}
						}

						Intel_list[Intel_list_size].desc = Intel_info[i].desc;
						Intel_list[Intel_list_size].index = i;
						Intel_list[Intel_list_size].name = Intel_info[i].name;
						Intel_list_size++;
					}
				}	
				Intel_loaded = 1;
			}

			// index lookup on intel is a pretty pointless, but it keeps everything 
			// consistent and doesnt really hurt anything
			Current_list = Intel_list;
			Current_list_size = Intel_list_size;

			font_height = gr_get_font_height();
			max_num_entries_viewable = Tech_list_coords[gr_screen.res][SHIP_H_COORD] / font_height;
#ifndef MAKE_FS1
			Tech_slider.set_numberItems(Current_list_size > max_num_entries_viewable ? Current_list_size-max_num_entries_viewable : 0);
#endif

			techroom_start_anim();
			break;
	}

	// reset the entry
	Cur_entry = 0;
	techroom_select_new_entry();

#ifdef MAKE_FS1
	techroom_tab_setup(1);
#endif
}

int techroom_button_pressed(int num)
{
	switch (num) {
		case SHIPS_DATA_TAB:
		case WEAPONS_DATA_TAB:
		case INTEL_DATA_TAB:
			techroom_change_tab(num);
			break;

		case SIMULATOR_TAB:
#if !defined(E3_BUILD) && !defined(PD_BUILD)
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_SIMULATOR_ROOM);
			return 1;
#else
			return 0;
#endif

		case CUTSCENES_TAB:
#if !defined(E3_BUILD) && !defined(PD_BUILD)
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_GOTO_VIEW_CUTSCENES_SCREEN);
			return 1;
#else
			return 0;
#endif

		case CREDITS_TAB:
#if !defined(E3_BUILD) && !defined(PD_BUILD)
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_CREDITS);
			return 1;
#else 
			return 0;
#endif

		case PREV_ENTRY_BUTTON:
			tech_prev_entry();
			break;

		case NEXT_ENTRY_BUTTON:
			tech_next_entry();
			break;

#ifdef MAKE_FS1
		case PREV_ENTRY_BUTTON2:
			tech_prev_entry();
			break;

		case NEXT_ENTRY_BUTTON2:
			tech_next_entry();
			break;
#endif

		case SCROLL_LIST_UP:
			tech_scroll_list_up();
#ifndef MAKE_FS1
			Tech_slider.forceUp();
#endif
			break;

		case SCROLL_LIST_DOWN:
			tech_scroll_list_down();
#ifndef MAKE_FS1
			Tech_slider.forceDown();
#endif
			break;

		case SCROLL_INFO_UP:
			tech_scroll_info_up();
			break;

		case SCROLL_INFO_DOWN:
			tech_scroll_info_down();
			break;

		case HELP_BUTTON:
			launch_context_help();
			gamesnd_play_iface(SND_HELP_PRESSED);
			break;

		case OPTIONS_BUTTON:
			gamesnd_play_iface(SND_SWITCH_SCREENS);
			gameseq_post_event(GS_EVENT_OPTIONS_MENU);
			break;

		case EXIT_BUTTON:
			gamesnd_play_iface(SND_COMMIT_PRESSED);
			gameseq_post_event(GS_EVENT_MAIN_MENU);
			break;
	}

	return 0;
}

int techroom_load_ani(anim **animpp, char *name)
{
	int load_attempts = 0;
	char anim_filename[64] = "2_";

	// hi-res support
	// (i dont think there are any hi-res anims for these tho)
	if (gr_screen.res == GR_1024) {
		strcat(anim_filename, name);
	} else {
		strcpy(anim_filename, name);
	}

	while(1) {
		if ( load_attempts++ > 5 ) {
			// Tech_room_ask_for_cd = 0;
			return 0;
		}

		*animpp = anim_load(anim_filename, 1);
		if ( *animpp ) {
			return 1;
		} else if (gr_screen.res == GR_1024) {
			// try to load low-res version if hi-res failed
			*animpp = anim_load(name, 1);
			if (*animpp) {
				return 1;
			}
		}

		// couldn't load animation, ask user to insert CD (if necessary)
		// if ( Tech_room_ask_for_cd ) {
			// if ( game_do_cd_check() == 0 ) {
				// Tech_room_ask_for_cd = 0;
				// break;
			// }
		// }
	}

	// bogus
	return 0;
}


void techroom_intel_init()
{
	int rval;
	static int inited = 0;

	// open localization
	lcl_ext_open();

	if (!inited) {
		if ((rval = setjmp(parse_abort)) != 0) {
			// close localization
			lcl_ext_close();

			return;
		} else {
			read_file_text("species.tbl");
			reset_parse();

			Intel_info_size = 0;

#ifndef MAKE_FS1
			while (optional_string("$Entry:")) {
				Assert(Intel_info_size < MAX_INTEL_ENTRIES);
				if (Intel_info_size >= MAX_INTEL_ENTRIES) break;

				required_string("$Name:");
				stuff_string(Intel_info[Intel_info_size].name, F_NAME, NULL, 32);
				required_string("$Anim:");
				stuff_string(Intel_info[Intel_info_size].anim_filename, F_NAME, NULL, 32);
				required_string("$AlwaysInTechRoom:");
				stuff_int(&Intel_info[Intel_info_size].in_tech_db);
				required_string("$Description:");
				stuff_string(Intel_info[Intel_info_size].desc, F_MULTITEXT, NULL, TECH_INTEL_DESC_LEN);


				Intel_info_size++;
			}
#else
			if (optional_string("$Terran Tech Description:")) {
				stuff_string(Intel_info[Intel_info_size].desc, F_MULTITEXT, NULL, TECH_INTEL_DESC_LEN);
				strcpy(Intel_info[Intel_info_size].name, "Terran");
				strcpy(Intel_info[Intel_info_size].anim_filename, Intel_anim_filenames[0]);
				Intel_info[Intel_info_size].in_tech_db = 1;

				Intel_info_size++;
			}
			
			if (optional_string("$Vasudan Tech Description:")) {

				stuff_string(Intel_info[Intel_info_size].desc, F_MULTITEXT, NULL, TECH_INTEL_DESC_LEN);
				strcpy(Intel_info[Intel_info_size].name, "Vasudan");
				strcpy(Intel_info[Intel_info_size].anim_filename, Intel_anim_filenames[1]);
				Intel_info[Intel_info_size].in_tech_db = 1;

				Intel_info_size++;
			}
			
			if (optional_string("$Shivan Tech Description:")) {

				stuff_string(Intel_info[Intel_info_size].desc, F_MULTITEXT, NULL, TECH_INTEL_DESC_LEN);
				strcpy(Intel_info[Intel_info_size].name, "Shivan");
				strcpy(Intel_info[Intel_info_size].anim_filename, Intel_anim_filenames[2]);
				// FIXME: shouldn't always be in the intel database but no choice at this point
				// there are only about 4 missions before they show up anyway so it may not be worth it
				Intel_info[Intel_info_size].in_tech_db = 1;

				Intel_info_size++;
			}
#endif
			inited = 1;
		}
	}

	// close localization
	lcl_ext_close();
}

void techroom_init()
{
	int i, idx;
	techroom_buttons *b;

	gr_reset_clip();
	gr_clear();
	Mouse_hidden++;
	gr_flip();
	Mouse_hidden--;

	Ships_loaded = 0;
	Weapons_loaded = 0;
	Intel_loaded = 0;

	// Tech_room_ask_for_cd = 1;

	// backup and bash detail level stuff
	Tech_detail_backup = Detail.detail_distance;
	Detail.detail_distance = MAX_DETAIL_LEVEL;
	Tech_texture_backup = Detail.hardware_textures;
	Detail.hardware_textures = MAX_DETAIL_LEVEL;

	/*
	Palette_bmp = bm_load("TechDataPalette");
	Assert(Palette_bmp);
	bm_get_palette(Palette_bmp, Palette, Palette_name);  // get the palette for this bitmap
	gr_set_palette(Palette_name, Palette, 1);
	*/

#ifdef MAKE_FS1
	common_set_interface_palette("TechDataPalette");
#endif

	// unflag fullneb 
	The_mission.flags &= ~MISSION_FLAG_FULLNEB;

	// set up UI stuff
	Ui_window.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);
#ifndef MAKE_FS1  // do this with techroom_tab_setup()
	Ui_window.set_mask_bmap(Tech_mask_filename[gr_screen.res]);
#endif

	Tech_background_bitmap = bm_load(Tech_background_filename[gr_screen.res]);
	if (Tech_background_bitmap < 0) {
		// failed to load bitmap, not a good thing
		Int3();
	}

#ifdef MAKE_FS1 // for weapons/species tab
	Tech_data_background_bitmap = bm_load(Tech_data_background_filename);
	if (Tech_data_background_bitmap < 0) {
		// failed to load bitmap, not a good thing
		Int3();
	}

	techroom_tab_setup(0);

	// don't draw buttons that are only on other tabs
	for (i=0; i<PREV_ENTRY_BUTTON2; i++) {
#else
	for (i=0; i<NUM_BUTTONS; i++) {
#endif
		b = &Buttons[gr_screen.res][i];

		b->button.create(&Ui_window, "", b->x, b->y, 60, 30, b->flags & REPEAT, 1);
		// set up callback for when a mouse first goes over a button
		if (b->filename) {
			b->button.set_bmaps(b->filename);
//			if ( !(b->flags & NO_MOUSE_OVER_SOUND) ) {
				b->button.set_highlight_action(common_play_highlight_sound);
//			}

		} else {
			b->button.hide();
		}

		b->button.link_hotspot(b->hotspot);
	}
	
#ifndef MAKE_FS1
	// common tab button text
	Ui_window.add_XSTR("Technical Database", 1055, Buttons[gr_screen.res][TECH_DATABASE_TAB].xt,  Buttons[gr_screen.res][TECH_DATABASE_TAB].yt, &Buttons[gr_screen.res][TECH_DATABASE_TAB].button, UI_XSTR_COLOR_GREEN);
	Ui_window.add_XSTR("Mission Simulator", 1056, Buttons[gr_screen.res][SIMULATOR_TAB].xt,  Buttons[gr_screen.res][SIMULATOR_TAB].yt, &Buttons[gr_screen.res][SIMULATOR_TAB].button, UI_XSTR_COLOR_GREEN);
	Ui_window.add_XSTR("Cutscenes", 1057, Buttons[gr_screen.res][CUTSCENES_TAB].xt,  Buttons[gr_screen.res][CUTSCENES_TAB].yt, &Buttons[gr_screen.res][CUTSCENES_TAB].button, UI_XSTR_COLOR_GREEN);
	Ui_window.add_XSTR("Credits", 1058, Buttons[gr_screen.res][CREDITS_TAB].xt,  Buttons[gr_screen.res][CREDITS_TAB].yt, &Buttons[gr_screen.res][CREDITS_TAB].button, UI_XSTR_COLOR_GREEN);

	// common ship/weapon/intel text
	Ui_window.add_XSTR("Ships", 293, Buttons[gr_screen.res][SHIPS_DATA_TAB].xt,  Buttons[gr_screen.res][SHIPS_DATA_TAB].yt, &Buttons[gr_screen.res][SHIPS_DATA_TAB].button, UI_XSTR_COLOR_GREEN);
	Ui_window.add_XSTR("Weapons", 1553, Buttons[gr_screen.res][WEAPONS_DATA_TAB].xt,  Buttons[gr_screen.res][WEAPONS_DATA_TAB].yt, &Buttons[gr_screen.res][WEAPONS_DATA_TAB].button, UI_XSTR_COLOR_GREEN);
	Ui_window.add_XSTR("Intelligence", 1066, Buttons[gr_screen.res][INTEL_DATA_TAB].xt,  Buttons[gr_screen.res][INTEL_DATA_TAB].yt, &Buttons[gr_screen.res][INTEL_DATA_TAB].button, UI_XSTR_COLOR_GREEN);

	// common help/options/commit text

	// NK: removed these two text labels on Tech screen update 4/26/99
//	Ui_window.add_XSTR("Help", 928, Buttons[gr_screen.res][HELP_BUTTON].xt,  Buttons[gr_screen.res][HELP_BUTTON].yt, &Buttons[gr_screen.res][HELP_BUTTON].button, UI_XSTR_COLOR_GREEN);
//	Ui_window.add_XSTR("Options", 1036, Buttons[gr_screen.res][OPTIONS_BUTTON].xt,  Buttons[gr_screen.res][OPTIONS_BUTTON].yt, &Buttons[gr_screen.res][OPTIONS_BUTTON].button, UI_XSTR_COLOR_GREEN);		
	Ui_window.add_XSTR("Exit", 1418, Buttons[gr_screen.res][EXIT_BUTTON].xt,  Buttons[gr_screen.res][EXIT_BUTTON].yt, &Buttons[gr_screen.res][EXIT_BUTTON].button, UI_XSTR_COLOR_PINK);		
#endif

	if (Player->flags & PLAYER_FLAGS_IS_MULTI) {
		Buttons[gr_screen.res][SIMULATOR_TAB].button.disable();
		Buttons[gr_screen.res][CUTSCENES_TAB].button.disable();
	}

	// set some hotkeys
	Buttons[gr_screen.res][PREV_ENTRY_BUTTON].button.set_hotkey(KEY_LEFT);
	Buttons[gr_screen.res][NEXT_ENTRY_BUTTON].button.set_hotkey(KEY_RIGHT);
#ifndef MAKE_FS1 // set per tab
	Buttons[gr_screen.res][SCROLL_INFO_UP].button.set_hotkey(KEY_UP);
	Buttons[gr_screen.res][SCROLL_INFO_DOWN].button.set_hotkey(KEY_DOWN);
#endif


	for (i=0; i<LIST_BUTTONS_MAX; i++) {
		List_buttons[i].create(&Ui_window, "", 0, 0, 60, 30, 0, 1);
		List_buttons[i].hide();
		List_buttons[i].disable();
	}

	View_window.create(&Ui_window, "", Tech_ship_display_coords[gr_screen.res][SHIP_X_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_Y_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_W_COORD], Tech_ship_display_coords[gr_screen.res][SHIP_H_COORD], 1, 1);
	View_window.hide();

#ifdef MAKE_FS1
	ShipWin01 = bm_load(NOX("ShipWin01"));
	ShipWin02 = bm_load(NOX("ShipWin02"));
	ShipWin03 = bm_load(NOX("ShipWin03"));
	ShipWin04 = bm_load(NOX("ShipWin04"));
#endif

	Buttons[gr_screen.res][HELP_BUTTON].button.set_hotkey(KEY_F1);
	Buttons[gr_screen.res][EXIT_BUTTON].button.set_hotkey(KEY_CTRLED | KEY_ENTER);
	Buttons[gr_screen.res][SCROLL_LIST_UP].button.set_hotkey(KEY_PAGEUP);
	Buttons[gr_screen.res][SCROLL_LIST_DOWN].button.set_hotkey(KEY_PAGEDOWN);

	// init help overlay states
	help_overlay_set_state(TECH_ROOM_OVERLAY, 0);

	// setup slider
#ifndef MAKE_FS1
	Tech_slider.create(&Ui_window, Tech_slider_coords[gr_screen.res][SHIP_X_COORD], Tech_slider_coords[gr_screen.res][SHIP_Y_COORD], Tech_slider_coords[gr_screen.res][SHIP_W_COORD], Tech_slider_coords[gr_screen.res][SHIP_H_COORD], Num_ship_types, Tech_slider_filename[gr_screen.res], &tech_scroll_list_up, &tech_scroll_list_down, &tech_ship_scroll_capture);
#endif

	Cur_anim_instance = NULL;

	// zero weapon and intel anim/bitmap stuff
	for(idx=0; idx<MAX_WEAPON_TYPES; idx++){
		Weapon_list[idx].animation = NULL;
		Weapon_list[idx].bitmap = -1;
	}
	for(idx=0; idx<MAX_INTEL_ENTRIES; idx++){
		Intel_list[idx].animation = NULL;
		Intel_list[idx].bitmap = -1;
	}

	Anim_playing_id = -1;
	techroom_change_tab(Tab);
}

void techroom_close()
{
	int i;

	techroom_stop_anim(-1);
	for (i=0; i<MAX_WEAPON_TYPES; i++) {
		if ( Weapon_list[i].animation ) {
			anim_free(Weapon_list[i].animation);
			Weapon_list[i].animation = NULL;
		}
		if( Weapon_list[i].bitmap >= 0 ){
			bm_unload(Weapon_list[i].bitmap);
			Weapon_list[i].bitmap = -1;
		}
	}

	for (i=0; i<MAX_INTEL_ENTRIES; i++){
		if (Intel_list[i].animation != NULL) {
			anim_free(Intel_list[i].animation);
			Intel_list[i].animation = NULL;
		}
		if( Intel_list[i].bitmap >= 0 ){
			bm_unload(Intel_list[i].bitmap);
			Intel_list[i].bitmap = -1;
		}
	}

	Ships_loaded = 0;
	Weapons_loaded = 0;
	Intel_loaded = 0;

#ifdef MAKE_FS1
	if (ShipWin01){
		bm_unload(ShipWin01);
	}
	if (ShipWin02){
		bm_unload(ShipWin02);
	}
	if (ShipWin03){
		bm_unload(ShipWin03);
	}
	if (ShipWin04){
		bm_unload(ShipWin04);
	}

	if (Tech_data_background_bitmap) {
		bm_unload(Tech_data_background_bitmap);
	}
#endif

	if (Tech_background_bitmap) {
		bm_unload(Tech_background_bitmap);
	}

	Ui_window.destroy();
	common_free_interface_palette();		// restore game palette
	/*
	if (Palette_bmp){
		bm_unload(Palette_bmp);
	}
	*/

	// restore detail settings
	Detail.detail_distance = Tech_detail_backup;
	Detail.hardware_textures = Tech_texture_backup;
}

#ifdef MAKE_FS1
void techroom_data_init()
{
	int i;
	techroom_buttons *b;

	for (i=PREV_ENTRY_BUTTON2; i<NUM_BUTTONS; i++) {
		b = &Buttons[gr_screen.res][i];

		b->button.create(&Ui_window, "", b->x, b->y, 60, 30, b->flags & REPEAT, 1);
		// set up callback for when a mouse first goes over a button
		if (b->filename) {
			b->button.set_bmaps(b->filename);
			b->button.set_highlight_action(common_play_highlight_sound);
		} else {
			b->button.hide();
		}

		b->button.link_hotspot(b->hotspot);
	}
}

void techroom_tab_setup(int set_palette)
{
	// char *pal;
	int i;
	int flags[256];

	for (i=0; i<256; i++){
		flags[i] = 0;
	}

	// activate, deactivate any necessary controls
	for (i=0; i<NUM_BUTTONS; i++) {
		Buttons[gr_screen.res][i].button.enable();
		if (Buttons[gr_screen.res][i].filename)
			Buttons[gr_screen.res][i].button.unhide();
	}

	// do other special processing
	switch (Tab) {
		case SHIPS_DATA_TAB:
			// set the mask for this tab
			Ui_window.set_mask_bmap(Tech_mask_filename[gr_screen.res]);

			// setup hotkeys to scroll list
			Buttons[gr_screen.res][SCROLL_INFO_UP].button.set_hotkey(KEY_UP);
			Buttons[gr_screen.res][SCROLL_INFO_DOWN].button.set_hotkey(KEY_DOWN);

			// hide extra buttons
			Buttons[gr_screen.res][PREV_ENTRY_BUTTON2].button.hide();
			Buttons[gr_screen.res][NEXT_ENTRY_BUTTON2].button.hide();
			break;

		case WEAPONS_DATA_TAB:
			// set the mask for this tab
			Ui_window.set_mask_bmap(Tech_data_mask_filename);

			// the list isn't supposed to scroll so clear the hotkeys
			Buttons[gr_screen.res][SCROLL_INFO_UP].button.set_hotkey(-1);
			Buttons[gr_screen.res][SCROLL_INFO_DOWN].button.set_hotkey(-1);

			// hide extra buttons
			Buttons[gr_screen.res][SCROLL_LIST_UP].button.hide();
			Buttons[gr_screen.res][SCROLL_LIST_DOWN].button.hide();
			Buttons[gr_screen.res][SCROLL_INFO_UP].button.hide();
			Buttons[gr_screen.res][SCROLL_INFO_DOWN].button.hide();
			Buttons[gr_screen.res][PREV_ENTRY_BUTTON].button.hide();
			Buttons[gr_screen.res][NEXT_ENTRY_BUTTON].button.hide();
			break;

		case INTEL_DATA_TAB:
			// set the mask for this tab
			Ui_window.set_mask_bmap(Tech_data_mask_filename);

			// the list isn't supposed to scroll so clear the hotkeys
			Buttons[gr_screen.res][SCROLL_INFO_UP].button.set_hotkey(-1);
			Buttons[gr_screen.res][SCROLL_INFO_DOWN].button.set_hotkey(-1);

			// hide extra buttons
			Buttons[gr_screen.res][SCROLL_LIST_UP].button.hide();
			Buttons[gr_screen.res][SCROLL_LIST_DOWN].button.hide();
			Buttons[gr_screen.res][SCROLL_INFO_UP].button.hide();
			Buttons[gr_screen.res][SCROLL_INFO_DOWN].button.hide();
			Buttons[gr_screen.res][PREV_ENTRY_BUTTON].button.hide();
			Buttons[gr_screen.res][NEXT_ENTRY_BUTTON].button.hide();
			break;
	}
}
#endif

void techroom_do_frame(float frametime)
{
	int i, k;	

	// turn off controls when overlay is on
	if ( help_overlay_active(TECH_ROOM_OVERLAY) ) {
		Buttons[gr_screen.res][HELP_BUTTON].button.reset_status();
		Ui_window.set_ignore_gadgets(1);
	}

	// turn off controls in trackball mode
	if (Trackball_active) {
		Ui_window.set_ignore_gadgets(1);
	} else {
		Ui_window.set_ignore_gadgets(0);
	}

	k = Ui_window.process() & ~KEY_DEBUGGED;

	if ( (k > 0) || B1_JUST_RELEASED ) {
		if ( help_overlay_active(TECH_ROOM_OVERLAY) ) {
			help_overlay_set_state(TECH_ROOM_OVERLAY, 0);
			Ui_window.set_ignore_gadgets(0);
			k = 0;
		}
	}

	if ( !help_overlay_active(TECH_ROOM_OVERLAY) ) {
		Ui_window.set_ignore_gadgets(0);
	}

	switch (k) {
		case KEY_SHIFTED | KEY_TAB:  // activate previous tab
			i = Tab - 1;
			if (i < 0) {
				i = NUM_TABS - 1;
			}

			techroom_change_tab(i);
			break;

		case KEY_TAB:  // activate next tab
			i = Tab + 1;
			if (i >= NUM_TABS) {
				i = 0;
			}

			techroom_change_tab(i);
			break;

		case KEY_CTRLED | KEY_DOWN:
			if ( !(Player->flags & PLAYER_FLAGS_IS_MULTI) ) {
				techroom_button_pressed(SIMULATOR_TAB);
				break;
			}
			// fall through

		case KEY_CTRLED | KEY_UP:
			techroom_button_pressed(CREDITS_TAB);
			break;
/*
		case KEY_UP:
			tech_prev_entry();
			break;

		case KEY_DOWN:
			tech_next_entry();
			break;
*/
		case KEY_CTRLED | KEY_ENTER:
		case KEY_ESC:
			gameseq_post_event(GS_EVENT_MAIN_MENU);
			break;
	}	

	// check ship model window for activity
	if (View_window.pressed()) {
		Trackball_active = 1;
		Trackball_mode = 1;
	}
	if (B1_RELEASED) {
		Trackball_active = 0;
	}

	// check all da buttons
	for (i=0; i<NUM_BUTTONS; i++) {
		if (Buttons[gr_screen.res][i].button.pressed()) {
			if (techroom_button_pressed(i)) {
				return;
			}
		}
	}

	// check for mouseovers/clicks on the selection list
	Select_tease_line = -1;
	for (i=0; i<LIST_BUTTONS_MAX; i++) {
		if (List_buttons[i].is_mouse_on()) {
			Select_tease_line = i + List_offset;
		}
	
		if (List_buttons[i].pressed()) {
			Cur_entry = i + List_offset;
			gamesnd_play_iface(SND_USER_SELECT);
			techroom_select_new_entry();
		}
	}

#ifndef MAKE_FS1  // do this per tab instead
	// clear & draw bg bitmap
	GR_MAYBE_CLEAR_RES(Tech_background_bitmap);
	if (Tech_background_bitmap >= 0) {
		gr_set_bitmap(Tech_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	}
#endif

	// render
	switch (Tab) {
		case SHIPS_DATA_TAB:
#ifdef MAKE_FS1
			// clear & draw bg bitmap
			GR_MAYBE_CLEAR_RES(Tech_background_bitmap);
			if (Tech_background_bitmap >= 0) {
				gr_set_bitmap(Tech_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(0, 0);
			}
#endif
			techroom_ships_render(frametime);

#ifdef MAKE_FS1
			if (ShipWin01) {
				gr_set_bitmap(ShipWin01, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(223, 104);
			}

			if (ShipWin02) {
				gr_set_bitmap(ShipWin02, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(621, 124);
			}

			if (ShipWin03) {
				gr_set_bitmap(ShipWin03, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(223, 338);
			}

			if (ShipWin04) {
				gr_set_bitmap(ShipWin04, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(218, 124);
			}
#endif

			break;

		case WEAPONS_DATA_TAB:
#ifdef MAKE_FS1
			// clear & draw bg bitmap
			GR_MAYBE_CLEAR_RES(Tech_data_background_bitmap);
			if (Tech_data_background_bitmap >= 0) {
				gr_set_bitmap(Tech_data_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(0, 0);
			}
#endif
			techroom_weapons_render2(frametime);

			break;

		case INTEL_DATA_TAB:
#ifdef MAKE_FS1
			// clear & draw bg bitmap
			GR_MAYBE_CLEAR_RES(Tech_data_background_bitmap);
			if (Tech_data_background_bitmap >= 0) {
				gr_set_bitmap(Tech_data_background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(0, 0);
			}
#endif
			techroom_intel_render(frametime);

			break;
	}

	Ui_window.draw();

	for (i=TECH_DATABASE_TAB; i<=CREDITS_TAB; i++) {
		if (Buttons[gr_screen.res][i].button.button_down()) {
			break;
		}
	}
	if (i > CREDITS_TAB) {
		Buttons[gr_screen.res][TECH_DATABASE_TAB].button.draw_forced(2);
	}

	for (i=0; i<NUM_TABS; i++){
		if (Buttons[gr_screen.res][i].button.button_down()){
			break;
		}
	}
	if (i == NUM_TABS){
		Buttons[gr_screen.res][Tab].button.draw_forced(2);
	}

	// blit help overlay if active
	help_overlay_maybe_blit(TECH_ROOM_OVERLAY);

	gr_flip();
}

