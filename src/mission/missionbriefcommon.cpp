/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Mission/MissionBriefCommon.cpp $
 * $Revision: 311 $
 * $Date: 2010-02-08 09:09:26 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module for briefing code common to FreeSpace and FRED
 *
 * $Log$
 * Revision 1.12  2005/10/01 22:04:58  taylor
 * fix FS1 (de)briefing voices, the directory names are different in FS1
 * hard code the table values so that the fs1.vp file isn't needed
 * hard code a mission fix for sm2-08a since a have no idea how to fix it otherwise
 * generally cleanup some FS1 code
 * fix volume sliders in the options screen that never went all the way up
 *
 * Revision 1.11  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.10  2004/07/04 11:39:06  taylor
 * fix missing debrief text, crash on exit, path separator's, warning fixes, no GR_SOFT
 *
 * Revision 1.9  2003/06/11 18:30:32  taylor
 * plug memory leaks
 *
 * Revision 1.8  2003/06/03 04:00:40  taylor
 * Polish language support (Janusz Dziemidowicz)
 *
 * Revision 1.7  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.6  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.5  2002/06/09 04:41:22  relnev
 * added copyright header
 *
 * Revision 1.4  2002/06/02 04:26:34  relnev
 * warning cleanup
 *
 * Revision 1.3  2002/06/01 07:12:33  relnev
 * a few NDEBUG updates.
 *
 * removed a few warnings.
 *
 * Revision 1.2  2002/05/07 03:16:46  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 19    11/02/99 3:23p Jefff
 * translate briefing icon names
 * 
 * 18    9/09/99 9:44a Jefff
 * doh, fixed reversed brief text color thingy.  i am stoopid.
 * 
 * 17    9/08/99 11:14a Jefff
 * toned down hostile/friendly colors in briefing text
 * 
 * 16    9/07/99 12:20p Mikeb
 * return pos of briefing icon even it does not fit on screen.
 * 
 * 15    9/03/99 1:32a Dave
 * CD checking by act. Added support to play 2 cutscenes in a row
 * seamlessly. Fixed super low level cfile bug related to files in the
 * root directory of a CD. Added cheat code to set campaign mission # in
 * main hall.
 * 
 * 14    8/10/99 7:28p Jefff
 * shuffled some text around
 * 
 * 13    7/30/99 3:05p Jefff
 * Fixed briefing icon fades -- in and out were reversed.
 * 
 * 12    7/26/99 1:52p Mikeb
 * Fixed strange briefing bug where a NULL wasn't being checked for when
 * copying briefing stage text. Odd.
 * 
 * 11    7/24/99 6:15p Jefff
 * moved "stage x of y" text in multiplayer mode so its not covered by the
 * chatbox
 * 
 * 10    7/20/99 7:09p Jefff
 * briefing text occupies full window in 1024x768
 * 
 * 9     7/09/99 5:54p Dave
 * Seperated cruiser types into individual types. Added tons of new
 * briefing icons. Campaign screen.
 * 
 * 8     6/29/99 7:39p Dave
 * Lots of small bug fixes.
 * 
 * 7     2/05/99 7:19p Neilk
 * Removed black part from mission screen, fixed info text coords
 * 
 * 6     1/29/99 4:17p Dave
 * New interface screens.
 * 
 * 5     12/18/98 1:13a Dave
 * Rough 1024x768 support for Direct3D. Proper detection and usage through
 * the launcher.
 * 
 * 4     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
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
 * 122   6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 121   6/05/98 9:54a Lawrance
 * OEM changes
 * 
 * 120   6/01/98 11:43a John
 * JAS & MK:  Classified all strings for localization.
 * 
 * 119   5/23/98 10:38p Lawrance
 * Avoid doing a cfile refresh when running debug
 * 
 * 118   5/23/98 6:49p Lawrance
 * Fix problems with refreshing the file list when a CD is inserted
 * 
 * 117   5/21/98 6:57p Lawrance
 * Don't prompt for the CD if voice not found
 * 
 * 116   5/21/98 12:35a Lawrance
 * Tweak how CD is checked for
 * 
 * 115   5/12/98 11:46a John
 * Changed the way the "glowing movement" type text draw.   Use Hoffoss's
 * gr_get_string_size optional length parameter to determine length of
 * string which accounts for kerning on the last character and then I only
 * draw each character only once.
 * 
 * 114   5/08/98 5:32p Lawrance
 * prompt for CD if can't load animations or voice
 * 
 * 113   5/06/98 5:30p John
 * Removed unused cfilearchiver.  Removed/replaced some unused/little used
 * graphics functions, namely gradient_h and _v and pixel_sp.   Put in new
 * DirectX header files and libs that fixed the Direct3D alpha blending
 * problems.
 * 
 * 112   4/27/98 9:08p Allender
 * fix the debriefing stage problems when clients get to screen long after
 * server
 * 
 * 111   4/25/98 3:49p Lawrance
 * Save briefing auto-advance pref
 * 
 * 110   4/20/98 3:53p Lawrance
 * Fix various bugs with auto-advancing through briefings.
 * 
 * $NoKeywords: $
 */

#include "freespace.h"
#include "ship.h"
#include "key.h"
#include "2d.h"
#include "3d.h"
#include "line.h"
#include "timer.h"
#include "math.h"
#include "linklist.h"
#include "mouse.h"
#include "hud.h"
#include "osapi.h"
#include "object.h"
#include "multi.h"
#include "bmpman.h"
#include "missionbrief.h"
#include "missiongrid.h"
#include "missionbriefcommon.h"
#include "animplay.h"
#include "fvi.h"
#include "float.h"
#include "gamesnd.h"
#include "cmdline.h"
#include "parselo.h"
#include "audiostr.h"
#include "missioncmdbrief.h"
#include "missiondebrief.h"
#include "alphacolors.h"
#include "localize.h"
#include "vecmat.h"

// --------------------------------------------------------------------------------------
// briefing icons
// --------------------------------------------------------------------------------------
hud_frames	Icon_bitmaps[MAX_BRIEF_ICONS][MAX_SPECIES_NAMES];
hud_anim		Icon_highlight_anims[MAX_BRIEF_ICONS][MAX_SPECIES_NAMES];
hud_anim		Icon_fade_anims[MAX_BRIEF_ICONS][MAX_SPECIES_NAMES];


// --------------------------------------------------------------------------------------
// briefing screen
// --------------------------------------------------------------------------------------

brief_screen bscreen;

// briefing screen sections
#define BRIEF_CUP_X1			400
#define BRIEF_CUP_Y1			70
#define BRIEF_CUP_X2			639
#define BRIEF_CUP_Y2			245
#define BRIEF_CUPINFO_X1	445
#define BRIEF_CUPINFO_Y1	247
#define BRIEF_CUPINFO_X2	639
#define BRIEF_CUPINFO_Y2	438

const char *Brief_static_name[GR_NUM_RESOLUTIONS] = {
	"BriefMap",
	"2_BriefMap"
};

int Brief_static_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		42, 122
#else
		10, 130
#endif
	},
	{ // GR_1024
		15, 208
	}
};

int Brief_bmap_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		42, 122
#else
		0, 115
#endif
	},
	{ // GR_1024
		0, 184
	}
};

int Brief_grid_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		54, 133, 530, 239
#else
		19, 147, 555, 232
#endif
	},
	{ // GR_1024
		30, 235, 888, 371
	}
};

int Brief_text_coords[GR_NUM_RESOLUTIONS][4] = {
	{ // GR_640
#ifdef MAKE_FS1
		49, 415, 374, 56
#else
		28, 399, 395, 74
#endif
	},
	{ // GR_1024
		46, 637, 630, 120
	}
};

int Brief_stage_text_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		61, 387
#else
		138, 117
#endif
	},
	{ // GR_1024
		227, 194
	}
};

int Brief_stage_text_coords_multi[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		61, 387
#else
		479, 385
#endif
	},
	{ // GR_1024
		821, 616
	}
};

int Brief_text_max_lines[GR_NUM_RESOLUTIONS] = {
	6, 6
};

#define LOOKAT_DIST	500.0f

// --------------------------------------------------------------------------------------
// Game-wide global data
// --------------------------------------------------------------------------------------
briefing		Briefings[MAX_TEAMS];			// there is exactly one briefing per mission
debriefing	Debriefings[MAX_TEAMS];			// there can be multiple debriefings per mission
briefing		*Briefing;							// pointer used in code -- points to correct briefing
debriefing	*Debriefing;						// pointer to correct debriefing

int			Briefing_voice_enabled=1;		// flag which turn on/off voice playback of briefings/debriefings

// --------------------------------------------------------------------------------------
// Module global data
// --------------------------------------------------------------------------------------

static int Last_new_stage;
int	Cur_brief_id;

const char BRIEF_META_CHAR = '$';

// static int Brief_voice_ask_for_cd;

// camera related
static vector	Current_cam_pos;		// current camera position
static vector	Target_cam_pos;		// desired camera position
static matrix	Current_cam_orient;	// current camera orientation
static matrix	Target_cam_orient;	// desired camera orientation
static matrix	Start_cam_orient;		// start camera orientation
static vector	Start_cam_pos;			// position of camera at the start of a translation
static vector	Cam_vel;					//	camera velocity
static vector	Current_lookat_pos;	// lookat point
static vector	Target_lookat_pos;	// lookat point
static vector	Start_lookat_pos;
static vector	Lookat_vel;				//	lookat point velocity

static float	Start_cam_move;		// time at which camera started moving (seconds)
static float	Total_move_time;		// time in which camera should move from current pos to target pos (seconds)
static float	Elapsed_time;

static float	Start_dist;
static float	End_dist;
static float	Dist_change_rate;

static vector	Acc_limit;
static vector	Vel_limit;

static float	Total_dist;
static float	Peak_speed;
static float	Cam_accel;
static float	Last_dist;
static vector	W_init;

// flag to indicate that the sound for a spinning highlight animation has played
static int Brief_stage_highlight_sound_handle = -1;

// used for scrolling briefing text ( if necessary )
int		Num_brief_text_lines[MAX_TEXT_STREAMS];
int		Top_brief_text_line;
static char		Brief_text[MAX_BRIEF_LINES][MAX_BRIEF_LINE_LEN];

// Used to support drawing colored text for the briefing.  Gets complicates since we
// need to be able to draw one character at a time as well when the briefing text
// first appears.
typedef struct colored_char
{
	char	letter;
	ubyte	color;		// index into Brief_text_colors[]
} colored_char;

static colored_char Colored_text[MAX_TEXT_STREAMS][MAX_BRIEF_LINES][MAX_BRIEF_LINE_LEN];
static int Colored_text_len[MAX_TEXT_STREAMS][MAX_BRIEF_LINES];

#define MAX_BRIEF_TEXT_COLORS			9
#define BRIEF_TEXT_WHITE				0
#define BRIEF_TEXT_BRIGHT_WHITE		1
#define BRIEF_TEXT_RED					2
#define BRIEF_TEXT_GREEN				3
#define BRIEF_TEXT_YELLOW				4
#define BRIEF_TEXT_BLUE					5
#define BRIEF_IFF_FRIENDLY				6
#define BRIEF_IFF_HOSTILE				7
#define BRIEF_IFF_NEUTRAL				8

color Brief_color_red, Brief_color_green;

color *Brief_text_colors[MAX_BRIEF_TEXT_COLORS] = 
{
	&Color_white,
	&Color_bright_white,
	&Color_red,
	&Color_green,
	&Color_yellow,
	&Color_blue,
	&Brief_color_green,
	&Brief_color_red,
	&IFF_colors[IFF_COLOR_NEUTRAL][0],
};

#define BRIGHTEN_LEAD	2

float Brief_text_wipe_time_elapsed;
static int Max_briefing_line_len;

static int Brief_voice_ended;
static int Brief_textdraw_finished;
static int Brief_voice_started;
static int Brief_stage_time;

const float		BRIEF_TEXT_WIPE_TIME	= 1.5f;		// time in seconds for wipe to occur
static int		Brief_text_wipe_snd;					// sound handle of sound effect for text wipe
static int		Play_brief_voice;

// animation stuff
static int		Play_highlight_flag;
static int		Cam_target_reached;
static int		Cam_movement_done;

// moving icons
typedef struct icon_move_info
{
	icon_move_info	*next, *prev;
	int				used;
	int				id;
	vector			start;
	vector			finish;
	vector			current;

	// used to move icons smoothly
	vector			direction;
	float				total_dist;
	float				accel;
	float				total_move_time;
	float				peak_speed;
	int				reached_dest;
	float				last_dist;
} icon_move_info;

#define MAX_MOVE_ICONS	10
icon_move_info	Icon_movers[MAX_MOVE_ICONS];
icon_move_info	Icon_move_list;	// head of linked list

// fading out icons
typedef struct fade_icon
{
	hud_anim	fade_anim;		// anim info
	vector	pos;
	int		team;
} fade_icon;

#define		MAX_FADE_ICONS	30
fade_icon	Fading_icons[MAX_FADE_ICONS];
int			Num_fade_icons;

// voice id's for briefing text
int Brief_voices[MAX_BRIEF_STAGES];

cmd_brief *Cur_cmd_brief;
cmd_brief Cmd_briefs[MAX_TEAMS];

#ifdef MAKE_FS1
static const char *fs1_icon_tbl[MAX_BRIEF_ICONS][3] = {
	{ "iconfighter", "FadeFighter", "iconhighlight04" },
	{ "iconfighterW", "FadeFighterW", "iconhighlight02" },
	{ "iconcargo", "FadeCargo", "iconhighlight04" },
	{ "iconcargoW", "FadeCargoW", "iconhighlight02" },
	{ "iconbigship", "FadeBigShip", "iconhighlight07" },
	{ "iconbigshipW", "FadeBigShipW", "iconhighlight05" },
	{ "iconcapital", "FadeCapital", "iconhighlight06" },
	{ "iconplanet", "FadePlanet", "iconhighlight03" },
	{ "iconasteroid", "FadeAsteroid", "iconhighlight03" },
	{ "iconwaypoint", "FadeWaypoint", "iconhighlight04" },
	{ "iconsupport", "FadeSupport", "iconhighlight04" },
	{ "iconfreighter", "FadeFreighter", "iconhighlight01" },
	{ "iconfreighterc", "FadeFreighterC", "iconhighlight01" },
	{ "iconfreighterwing", "FadeFreighterW", "iconhighlight05" },
	{ "iconfreightercwing", "FadeFreighterCW", "iconhighlight05" },
	{ "iconInstallation", "FadeInstallation", "iconhighlight03" },
	{ "iconbomber", "FadeBomber", "iconhighlight07" },
	{ "iconbomberW", "FadeBomberW", "iconhighlight05" },
	{ "iconcruiser", "FadeCruiser", "iconhighlight01" },
	{ "iconcruiserW", "FadeCruiserW", "iconhighlight05" },
	{ "iconunknown", "FadeUnknown", "iconhighlight04" },
	{ "iconunknownW", "FadeUnknownW", "iconhighlight01" },
	{ "iconfighterP", "FadeFighterP", "iconhighlight04" },
	{ "iconfighterWP", "FadeFighterWP", "iconhighlight02" },
	{ "iconbomberP", "FadeBomberP", "iconhighlight07" },
	{ "iconbomberWP", "FadeBomberWP", "iconhighlight05" },
	{ "iconwaypoint", "FadeWaypoint", "iconhighlight04" }
};
#endif

// --------------------------------------------------------------------------------------
// forward declarations
// --------------------------------------------------------------------------------------
void	brief_render_elements(vector *pos, grid *gridp);
void	brief_render_icons(int stage_num, float frametime);
void 	brief_parse_icon_tbl();
void	brief_grid_read_camera_controls( control_info * ci, float frametime );
void	brief_maybe_create_new_grid(grid *gridp, vector *pos, matrix *orient, int force = 0);
grid	*brief_create_grid(grid *gridp, vector *forward, vector *right, vector *center, int nrows, int ncols, float square_size);
grid	*brief_create_default_grid(void);
void	brief_render_grid(grid *gridp);
void	brief_modify_grid(grid *gridp);
void	brief_rpd_line(vector *v0, vector *v1);
void	brief_set_text_color(int color_index);
extern void get_camera_limits(matrix *start_camera, matrix *end_camera, float time, vector *acc_max, vector *w_max);
int brief_text_wipe_finished();

void brief_set_icon_color(int team)
{
	switch (team) { 
	case TEAM_FRIENDLY:	SET_COLOR_FRIENDLY;	break;
	case TEAM_HOSTILE:	SET_COLOR_HOSTILE;	break;
	case TEAM_NEUTRAL:	SET_COLOR_NEUTRAL;	break;
	case TEAM_TRAITOR:	SET_COLOR_HOSTILE;	break;
	default:
		SET_COLOR_HOSTILE;	break;
	} // end switch
}

// --------------------------------------------------------------------------------------
//	brief_move_icon_reset()
//
//
void brief_move_icon_reset()
{
	int i;

	list_init(&Icon_move_list);
	for ( i = 0; i < MAX_MOVE_ICONS; i++ )
		Icon_movers[i].used = 0;
}


// --------------------------------------------------------------------------------------
// Does one time initialization of the briefing and debriefing structures.
// Namely setting all malloc'ble pointers to NULL.  Called once at game startup.
void mission_brief_common_init()
{
	int i,j;

	// setup brief text colors
	gr_init_alphacolor( &Brief_color_green, 50, 100, 50, 255, AC_TYPE_HUD);
	gr_init_alphacolor( &Brief_color_red, 140, 20, 20, 255, AC_TYPE_HUD);

	// extra catch to reset anything that's already loaded (ie. mission restart)
	mission_brief_common_reset();
	mission_debrief_common_reset();

	if ( Fred_running )	{
		// If Fred is running malloc out max space
		for (i=0; i<MAX_TEAMS; i++ )	{
			for (j=0; j<MAX_BRIEF_STAGES; j++ )	{
				Briefings[i].stages[j].new_text = (char *)malloc(MAX_BRIEF_LEN);
				Assert(Briefings[i].stages[j].new_text!=NULL);
				Briefings[i].stages[j].icons = (brief_icon *)malloc(sizeof(brief_icon)*MAX_STAGE_ICONS);
				Assert(Briefings[i].stages[j].icons!=NULL);
				Briefings[i].stages[j].lines = (brief_line *)malloc(sizeof(brief_line)*MAX_BRIEF_STAGE_LINES);
				Assert(Briefings[i].stages[j].lines!=NULL);
				Briefings[i].stages[j].num_icons = 0;
				Briefings[i].stages[j].num_lines = 0;
			}
		}

		for (i=0; i<MAX_TEAMS; i++ )	{
			for (j=0; j<MAX_DEBRIEF_STAGES; j++ )	{
				Debriefings[i].stages[j].new_text = (char *)malloc(MAX_DEBRIEF_LEN);
				Assert(Debriefings[i].stages[j].new_text!=NULL);
				Debriefings[i].stages[j].new_recommendation_text = (char *)malloc(MAX_RECOMMENDATION_LEN);
				Assert(Debriefings[i].stages[j].new_recommendation_text!=NULL);
			}
		}

	} else {
		// If game is running don't malloc anything
		for (i=0; i<MAX_TEAMS; i++ )	{
			for (j=0; j<MAX_BRIEF_STAGES; j++ )	{
				Briefings[i].stages[j].new_text = NULL;
				Briefings[i].stages[j].num_icons = 0;
				Briefings[i].stages[j].icons = NULL;
				Briefings[i].stages[j].num_lines = 0;
				Briefings[i].stages[j].lines = NULL;
			}
		}

		for (i=0; i<MAX_TEAMS; i++ )	{
			for (j=0; j<MAX_DEBRIEF_STAGES; j++ )	{
				Debriefings[i].stages[j].new_text = NULL;
				Debriefings[i].stages[j].new_recommendation_text = NULL;
			}
		}

	}

		
}

//--------------------------------------------------------------------------------------
// Frees all the memory allocated in the briefing and debriefing structures
// and sets all pointers to NULL.
void mission_brief_common_reset()
{
	int i,j;

	if ( Fred_running )	{
		return;						// Don't free these under Fred.
	}

	for (i=0; i<MAX_TEAMS; i++ )	{
		for (j=0; j<MAX_BRIEF_STAGES; j++ )	{
			if ( Briefings[i].stages[j].new_text )	{
				free(Briefings[i].stages[j].new_text);
				Briefings[i].stages[j].new_text = NULL;			
			}
	
			if ( Briefings[i].stages[j].icons )	{
				free(Briefings[i].stages[j].icons);
				Briefings[i].stages[j].icons = NULL;
			}

			if ( Briefings[i].stages[j].lines )	{
				free(Briefings[i].stages[j].lines);
				Briefings[i].stages[j].lines = NULL;
			}
		}
	}
}

// split from above since we need to clear them separately
void mission_debrief_common_reset()
{
	int i, j;

	if ( Fred_running ) {
		return;						// Don't free these under Fred.
	}

	for (i=0; i<MAX_TEAMS; i++ )	{
		for (j=0; j<MAX_DEBRIEF_STAGES; j++ )	{
			if ( Debriefings[i].stages[j].new_text )	{
				free(Debriefings[i].stages[j].new_text);
				Debriefings[i].stages[j].new_text = NULL;
			}

			if ( Debriefings[i].stages[j].new_recommendation_text )	{
				free(Debriefings[i].stages[j].new_recommendation_text);
				Debriefings[i].stages[j].new_recommendation_text = NULL;
			}
		}
	}		
}




// --------------------------------------------------------------------------------------
//	brief_reset()
//
//
void brief_reset()
{
	int i;

	Briefing = NULL;
	for ( i = 0; i < MAX_TEAMS; i++ ) 
		Briefings[i].num_stages = 0;
	Cur_brief_id = 1;
}

// --------------------------------------------------------------------------------------
//	debrief_reset()
//
//
void debrief_reset()
{
	int i,j;

	Debriefing = NULL;
	for ( i = 0; i < MAX_TEAMS; i++ ) {
		Debriefings[i].num_stages = 0;
		for (j=0; j<MAX_DEBRIEF_STAGES; j++ )	{
			if ( Debriefings[i].stages[j].new_recommendation_text )	{
				Debriefings[i].stages[j].new_recommendation_text[0] = 0;
			}
		}
	}

	// MWA 4/27/98 -- must initialize this variable here since we cannot do it as debrief
	// init time because race conditions between all players in the game make that type of
	// initialization unsafe.
	Debrief_multi_stages_loaded = 0;
}

// --------------------------------------------------------------------------------------
//	brief_init_screen()
//
//	Set up the screen regions.  A mulitplayer briefing will look different than a single player
// briefing.
//
void brief_init_screen(int multiplayer_flag)
{
	bscreen.map_x1			= Brief_grid_coords[gr_screen.res][0];
	bscreen.map_x2			= Brief_grid_coords[gr_screen.res][0] + Brief_grid_coords[gr_screen.res][2];
	bscreen.map_y1			= Brief_grid_coords[gr_screen.res][1];
	bscreen.map_y2			= Brief_grid_coords[gr_screen.res][1] + Brief_grid_coords[gr_screen.res][3];
	/*
	bscreen.map_x1			= BRIEF_GRID3_X1;
	bscreen.map_x2			= BRIEF_GRID0_X2;
	bscreen.map_y1			= BRIEF_GRID3_Y1;
	bscreen.map_y2			= BRIEF_GRID0_Y2+4;
	bscreen.btext_x1		= BRIEF_TEXT_X1;
	bscreen.btext_x2		= BRIEF_TEXT_X2;
	bscreen.btext_y1		= BRIEF_TEXT_Y1;
	bscreen.btext_y2		= BRIEF_TEXT_Y2;
	bscreen.cup_x1			= BRIEF_CUP_X1;
	bscreen.cup_y1			= BRIEF_CUP_Y1;
	bscreen.cup_x2			= BRIEF_CUP_X2;
	bscreen.cup_y2			= BRIEF_CUP_Y2;
	bscreen.cupinfo_x1	= BRIEF_CUPINFO_X1;
	bscreen.cupinfo_y1	= BRIEF_CUPINFO_Y1;
	bscreen.cupinfo_x2	= BRIEF_CUPINFO_X2;
	bscreen.cupinfo_y2	= BRIEF_CUPINFO_Y2;
	*/
}

// --------------------------------------------------------------------------------------
//	brief_init_icons()
//
//
void brief_init_icons()
{
	if ( Fred_running ) {
		gr_init_alphacolor( &IFF_colors[IFF_COLOR_HOSTILE][0],  0xff, 0x00, 0x00, 15*16, AC_TYPE_HUD);
		gr_init_alphacolor( &IFF_colors[IFF_COLOR_FRIENDLY][0], 0x00, 0xff, 0x00, 15*16, AC_TYPE_HUD);
	}

	int idx;
	int i;
	
	for (i=0; i<MAX_BRIEF_ICONS; i++) {
		for (idx=0; idx<MAX_SPECIES_NAMES; idx++) {
			Icon_bitmaps[i][idx].first_frame = -1;
			Icon_bitmaps[i][idx].num_frames = 0;
		}
	}

	// Load in the bitmaps for the icons from icons.tbl
	brief_parse_icon_tbl();
}

// Reset the highlight and fade anims... call before brief_parse_icon_tbl();
void brief_init_anims()
{
	int idx;
	int i;

	for (i=0; i<MAX_BRIEF_ICONS; i++) {
		for (idx=0; idx<MAX_SPECIES_NAMES; idx++) {
			Icon_highlight_anims[i][idx].first_frame = -1;
			Icon_highlight_anims[i][idx].num_frames = 0;
			Icon_fade_anims[i][idx].first_frame = -1;
			Icon_fade_anims[i][idx].num_frames = 0;
		}
	}
}

// ------------------------------------------------------------------------
//	brief_unload_icons() 
//
//
void brief_unload_icons()
{
	hud_frames		*ib;
	int				i, j, idx;

	for ( i = 0; i < MAX_BRIEF_ICONS; i++ ) {
		for(idx=0; idx<MAX_SPECIES_NAMES; idx++){
			ib = &Icon_bitmaps[i][idx];

			if (ib->first_frame < 0)
				continue;

			for ( j=0; j<ib->num_frames; j++ ) {
				bm_unload(ib->first_frame+j);
			}
		}
	}
}

// determine if icon type is used in the current briefing
int brief_icon_used_in_briefing(int icon_type)
{
	int num_icons, num_stages, i, j;

	num_stages = Briefing->num_stages;

	for ( i = 0; i < num_stages; i++ ) {
		num_icons = Briefing->stages[i].num_icons;
		for ( j = 0; j < num_icons; j++ ) {
			if ( Briefing->stages[i].icons[j].type == icon_type ) {
				return 1;
			}
		}
	}

	return 0;
}

// --------------------------------------------------------------------------------------
//	brief_parse_icon_tbl()
//
//
void brief_parse_icon_tbl()
{
#ifndef MAKE_FS1
	int			num_icons, rval;
	char			name[NAME_LENGTH];
	hud_frames	*hf;
	hud_anim		*ha;
	int idx;

	// open localization
	lcl_ext_open();

	if ((rval = setjmp(parse_abort)) != 0) {
		Error(LOCATION, "Unable to parse icons.tbl!  Code = %i.\n", rval);
	}
	else {
		read_file_text("icons.tbl");
		reset_parse();		
	}

	num_icons = 0;
	required_string("#Start");


	int load_this_icon = 0;

	while (required_string_either("#End","$Name:")) {
		for(idx=0; idx<MAX_SPECIES_NAMES; idx++){
			Assert( num_icons < MAX_BRIEF_ICONS);
			hf = &Icon_bitmaps[num_icons][idx];

			// load in regular frames
			required_string("$Name:");
			stuff_string(name, F_NAME, NULL);

			if ( Fred_running ) {
				load_this_icon = 1;
			} else {
				load_this_icon = brief_icon_used_in_briefing(num_icons);
			}

			if ( load_this_icon ) {
				hf->first_frame = bm_load_animation(name, &hf->num_frames);
				if ( hf->first_frame == -1 ) {
					Int3();	// missing briefing icon
				}
			}

			// load in fade frames
			required_string("$Name:");
			stuff_string(name, F_NAME, NULL);
			ha = &Icon_fade_anims[num_icons][idx];
			hud_anim_init(ha, 0, 0, name);

			// load in highlighting frames
			required_string("$Name:");
			stuff_string(name, F_NAME, NULL);
			ha = &Icon_highlight_anims[num_icons][idx];
			hud_anim_init(ha, 0, 0, name);
		}

		// next icon _type_
		num_icons++;
	}
	required_string("#End");

	// close localization
	lcl_ext_close();
#else
	// load up hard coded values for FS1
	char name[NAME_LENGTH];
	hud_frames *hf;
	hud_anim *ha;
	int idx;
	int load_this_icon = 0;

	for (idx = 0; idx < MAX_BRIEF_ICONS; idx++) {
		hf = &Icon_bitmaps[idx][0];

		// load in regular frames
		strncpy(name, fs1_icon_tbl[idx][0], NAME_LENGTH);

		if ( Fred_running ) {
			load_this_icon = 1;
		} else {
			load_this_icon = brief_icon_used_in_briefing(idx);
		}
		
		if ( load_this_icon ) {
			hf->first_frame = bm_load_animation(name, &hf->num_frames);
			if ( hf->first_frame == -1 ) {
				Int3();	// missing briefing icon
			}
		}

		// load in fade frames
		strncpy(name, fs1_icon_tbl[idx][1], NAME_LENGTH);
		ha = &Icon_fade_anims[idx][0];
		hud_anim_init(ha, 0, 0, name);

		// load in highlighting frames
		strncpy(name, fs1_icon_tbl[idx][2], NAME_LENGTH);
		ha = &Icon_highlight_anims[idx][0];
		hud_anim_init(ha, 0, 0, name);
	}
#endif
}

// --------------------------------------------------------------------------------------
//	brief_close_map()
//
//
void brief_close_map()
{
	brief_unload_icons();
}

void brief_preload_highlight_anim(brief_icon *bi)
{
	hud_anim *ha;
	int species = ship_get_species_by_type(bi->ship_class);

#ifdef MAKE_FS1
	species = 0;
#endif

	if(species < 0){
		return;
	}

	ha = &Icon_highlight_anims[bi->type][species];
	if ( !stricmp(NOX("none"), ha->name) ) {
		return;
	}

	// force read of data from disk, so we don't glitch on initial playback
	if ( ha->first_frame == -1 ) {
		hud_anim_load(ha);
		Assert(ha->first_frame >= 0);
	}

	bi->highlight_anim = *ha;

	gr_set_bitmap(ha->first_frame, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	gr_aabitmap(0, 0);
}

void brief_preload_fade_anim(brief_icon *bi)
{
	hud_anim *ha;
	int species = ship_get_species_by_type(bi->ship_class);

#ifdef MAKE_FS1
	species = 0;
#endif

	if(species < 0){
		return;
	}

	ha = &Icon_fade_anims[bi->type][species];
	if ( !stricmp(NOX("none"), ha->name) ) {
		return;
	}

	// force read of data from disk, so we don't glitch on initial playback
	if ( ha->first_frame == -1 ) {
		hud_anim_load(ha);
		Assert(ha->first_frame >= 0);
	}

	gr_set_bitmap(ha->first_frame, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	gr_aabitmap(0, 0);
}

// preload highlight, fadein and fadeout animations that are used in this stage
void brief_preload_anims()
{
	int			num_icons, num_stages, i, j;
	brief_icon	*bi;

	num_stages = Briefing->num_stages;

	for ( i = 0; i < num_stages; i++ ) {
		num_icons = Briefing->stages[i].num_icons;
		for ( j = 0; j < num_icons; j++ ) {
			bi = &Briefing->stages[i].icons[j];
			if ( bi->flags & BI_HIGHLIGHT ) {
				brief_preload_highlight_anim(bi);
			}
			brief_preload_fade_anim(bi);
		}
	}
}

// --------------------------------------------------------------------------------------
//	brief_init_map()
//
//
void brief_init_map()
{
	vector *pos;
	matrix *orient;

	Assert( Briefing != NULL );

	pos = &Briefing->stages[0].camera_pos;
	orient = &Briefing->stages[0].camera_orient;
	vm_vec_zero(&Current_lookat_pos);
	vm_vec_zero(&Target_lookat_pos);
	Elapsed_time = 0.0f;
	Total_move_time = 0.0f;

	The_grid = brief_create_default_grid();
	brief_maybe_create_new_grid(The_grid, pos, orient, 1);

	brief_init_anims();
	brief_init_icons();
	brief_move_icon_reset();
	brief_preload_anims();

	Brief_text_wipe_snd = -1;
	Last_new_stage = -1;
	Num_fade_icons=0;
}

#ifndef PLAT_UNIX
#pragma optimize("", off)
#endif

// render fade-out anim frame

void brief_render_fade_outs(float frametime)
{
	int			i,bx,by,w,h;
	float			bxf,byf;
	vertex		tv;			// temp vertex used to find screen position for text
	fade_icon	*fi;
	

	for (i=0; i<Num_fade_icons; i++) {
		fi = &Fading_icons[i];

		g3_rotate_vertex(&tv, &fi->pos);

		if (!(tv.flags & PF_PROJECTED))
			g3_project_vertex(&tv);

		if (!(tv.flags & PF_OVERFLOW) ) {  // make sure point projected before drawing text

			brief_set_icon_color(fi->team);

			if ( fi->fade_anim.first_frame < 0 ) {
				continue;
			}

			bm_get_info( fi->fade_anim.first_frame, &w, &h, NULL);
			
			bxf = tv.sx - w / 2.0f + 0.5f;
			byf = tv.sy - h / 2.0f + 0.5f;
			bx = fl2i(bxf);
			by = fl2i(byf);

			if ( fi->fade_anim.first_frame >= 0 ) {
				fi->fade_anim.sx = bx;
				fi->fade_anim.sy = by;
#ifdef MAKE_FS1
				// FS1 has the anis reversed from FS2 so play them backwards
				hud_anim_render(&fi->fade_anim, frametime, 1, 0, 0, 1);
#else			
				hud_anim_render(&fi->fade_anim, frametime, 1, 0, 0, 0);
#endif
			}
		}
	}
}

// figure out how far an icon should move based on the elapsed time
float brief_icon_get_dist_moved(icon_move_info *mi, float elapsed_time)
{
	float time, dist_moved=0.0f;
	
	// first half of movement
	if ( elapsed_time < mi->total_move_time/2.0f ) {
		dist_moved=0.5f*mi->accel*elapsed_time*elapsed_time;	// d = 1/2at^2
		return dist_moved;
	}

	// second half of movement
	time=elapsed_time - mi->total_move_time/2.0f;
	dist_moved=(mi->total_dist/2.0f)+(mi->peak_speed*time) - 0.5f*mi->accel*time*time;
	return dist_moved;
}

// Draw a line between two icons on the briefing screen
void brief_render_icon_line(int stage_num, int line_num)
{
	brief_line	*bl;
	brief_icon	*icon[2];
	vertex		icon_vertex[2];
	int			icon_status[2] = {0,0};
	int			icon_w, icon_h, i;
	float			icon_x[2], icon_y[2];

	bl = &Briefing->stages[stage_num].lines[line_num];
	icon[0] = &Briefing->stages[stage_num].icons[bl->start_icon];
	icon[1] = &Briefing->stages[stage_num].icons[bl->end_icon];

	// project icons
	for (i=0; i<2; i++) {
		g3_rotate_vertex(&icon_vertex[i],&icon[i]->pos);
		if (!(icon_vertex[i].flags&PF_PROJECTED))
			g3_project_vertex(&icon_vertex[i]);

		if (!(icon_vertex[i].flags & PF_OVERFLOW) ) {  // make sure point projected before drawing text
			icon_status[i]=1;
		}
	}

	if ( !icon_status[0] || !icon_status[1] ) {
		return;
	}

	// get screen (x,y) for icons
	for (i=0; i<2; i++) {
		brief_common_get_icon_dimensions(&icon_w, &icon_h, icon[i]->type, icon[i]->ship_class);
		icon_x[i] = icon_vertex[i].sx;
		icon_y[i] = icon_vertex[i].sy;
	}

	brief_set_icon_color(icon[0]->team);

	gr_line(fl2i(icon_x[0]), fl2i(icon_y[0]), fl2i(icon_x[1]), fl2i(icon_y[1]));
}

// -------------------------------------------------------------------------------------
// Draw a briefing icon
//
// parameters:		stage_num		=>		briefing stage number (start at 0)
//						icon_num			=>		icon number in stage
//						frametime		=>		time elapsed in seconds
//						selected			=>		FRED only (will be 0 or non-zero)
//						w_scale_factor	=>		scale icon in width by this amount (default 1.0f)
//						h_scale_factor	=>		scale icon in height by this amount (default 1.0f)
void brief_render_icon(int stage_num, int icon_num, float frametime, int selected, float w_scale_factor, float h_scale_factor)
{
	brief_icon	*bi, *closeup_icon;
	hud_frames	*ib;
	vertex		tv;	// temp vertex used to find screen position for text
	vector		*pos = NULL;
	int			bx,by,bc,w,h,icon_w,icon_h,icon_bitmap=-1;
	float			bxf, byf, dist=0.0f;

	Assert( Briefing != NULL );
	
	bi = &Briefing->stages[stage_num].icons[icon_num];

	icon_move_info *mi, *next;
	int interp_pos_found = 0;
	
	mi = GET_FIRST(&Icon_move_list);
	if (mi)
		while ( mi != &Icon_move_list ) {
			next = mi->next;
			if ( ( mi->id != 0 ) && ( mi->id == bi->id ) ) {

				if ( !mi->reached_dest ) {
					dist = brief_icon_get_dist_moved(mi, Elapsed_time);
					if ( dist < mi->last_dist ) {
						mi->reached_dest=1;
						mi->last_dist=0.0f;
					}
					mi->last_dist=dist;
				}

				if ( !mi->reached_dest ) {
					vector dist_moved;
					vm_vec_copy_scale(&dist_moved, &mi->direction, dist);
					vm_vec_add(&mi->current, &mi->start, &dist_moved);
				} else {
					mi->current = mi->finish;
				}

				pos = &mi->current;
				interp_pos_found = 1;
				break;
			}
			mi = next;
		}
	
	if ( !interp_pos_found )
		pos = &bi->pos;
		
	brief_render_elements(pos, The_grid);
	g3_rotate_vertex(&tv,pos);

	if (!(tv.flags&PF_PROJECTED))
		g3_project_vertex(&tv);

	if (!(tv.flags & PF_OVERFLOW) ) {  // make sure point projected before drawing text

		brief_set_icon_color(bi->team);

		int species = ship_get_species_by_type(bi->ship_class);

#ifdef MAKE_FS1
		species = 0;
#endif

		if(species < 0){
			return;
		}

		ib = &Icon_bitmaps[bi->type][species];
		if ( ib->first_frame < 0 ) {
			Int3();
			return;
		}

		brief_common_get_icon_dimensions(&icon_w, &icon_h, bi->type, bi->ship_class);

		closeup_icon = brief_get_closeup_icon();

		if ( bi == closeup_icon || selected ) {
			icon_bitmap=ib->first_frame+1;
//			gr_set_bitmap(ib->first_frame+1);
		}
		else {
			icon_bitmap=ib->first_frame;
//			gr_set_bitmap(ib->first_frame);
		}
	
		// draw icon centered at draw position
//		bx = fl2i(tv.sx - ib->icon_w/2.0f);
//		by = fl2i(tv.sy - ib->icon_h/2.0f);
//		bc = bx + fl2i(ib->icon_w/2.0f);
		//gr_aabitmap(bx, by);

		float scaled_w, scaled_h;
		
		scaled_w = icon_w * w_scale_factor;
		scaled_h = icon_h * h_scale_factor;
		bxf = tv.sx - scaled_w / 2.0f + 0.5f;
		byf = tv.sy - scaled_h / 2.0f + 0.5f;
		bx = fl2i(bxf);
		by = fl2i(byf);
		bc = fl2i(tv.sx);

		if ( !Fred_running && ((bx < 0) || (bx > gr_screen.max_w) || (by < 0) || (by > gr_screen.max_h)) ) {
			bi->x = bx;
			bi->y = by;
			return;
		}

		vertex va, vb;
		va.sx = bxf;
		va.sy = byf;
		va.u = 0.0f;
		va.v = 0.0f;

		vb.sx = bxf+scaled_w-1;
		vb.sy = byf+scaled_h-1;
		vb.u = 1.0f;
		vb.v = 1.0f;

		// render highlight anim frame
		if ( (bi->flags&BI_SHOWHIGHLIGHT) && (bi->flags&BI_HIGHLIGHT) ) {
			hud_anim *ha = &bi->highlight_anim;
			if ( ha->first_frame >= 0 ) {
				ha->sx = bi->hold_x;
				if ( strlen(bi->label) > 0 ) {
					ha->sy = bi->hold_y - fl2i(gr_get_font_height()/2.0f +0.5) - 2;
				} else {
					ha->sy = bi->hold_y;
				}

				//hud_set_iff_color(bi->team);
				brief_set_icon_color(bi->team);

				hud_anim_render(ha, frametime, 1, 0, 1);

				if ( Brief_stage_highlight_sound_handle < 0 ) {
					if ( !Fred_running) {
						Brief_stage_highlight_sound_handle = snd_play(&Snds_iface[SND_ICON_HIGHLIGHT]);					
					}
				}
			}
		}

		// render fade-in anim frame
		if ( bi->flags & BI_FADEIN ) {
			hud_anim *ha = &bi->fadein_anim;
			if ( ha->first_frame >= 0 ) {
				ha->sx = bx;
				ha->sy = by;
//				hud_set_iff_color(bi->team);
				brief_set_icon_color(bi->team);

#ifdef MAKE_FS1
				// FS1 has the anims backwards from FS2 so play in reverse
				if ( hud_anim_render(ha, frametime, 1, 0, 0, 0) == 0 ) {
#else
				if ( hud_anim_render(ha, frametime, 1, 0, 0, 1) == 0 ) {
#endif
					bi->flags &= ~BI_FADEIN;
				}
			} else {
				bi->flags &= ~BI_FADEIN;
			}
		}		

		if ( !(bi->flags & BI_FADEIN) ) {
			gr_set_bitmap(icon_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);

			if ( Fred_running )	{
				gr_aascaler(&va, &vb);
			} else {
				// Don't bother scaling for the game
				gr_aabitmap(bx, by);
			}

			// draw text centered over the icon (make text darker)
			if ( bi->type == ICON_FIGHTER_PLAYER || bi->type == ICON_BOMBER_PLAYER ) {
				gr_get_string_size(&w,&h,Players[Player_num].callsign);
				gr_printf(bc - fl2i(w/2.0f), by - h, Players[Player_num].callsign);
			}
			else {
				if (Lcl_gr) {
					char buf[128];
					strcpy(buf, bi->label);
					lcl_translate_brief_icon_name(buf);
					gr_get_string_size(&w, &h, buf);
					gr_printf(bc - fl2i(w/2.0f), by - h, buf);
				} else if(Lcl_pl) {
					char buf[128];
					strcpy(buf, bi->label);
					lcl_translate_brief_icon_name_pl(buf);
					gr_get_string_size(&w, &h, buf);
					gr_printf(bc - fl2i(w/2.0f), by - h, buf);
				} else {
					gr_get_string_size(&w,&h,bi->label);
					gr_printf(bc - fl2i(w/2.0f), by - h, bi->label);
				}
			}

			// show icon as selected (FRED only)
			if ( selected ) {
				gr_get_string_size(&w,&h,NOX("(S)"));
				gr_printf(bc - fl2i(w/2.0f), by - h*2, NOX("(S)"));
			}
		}

		// store screen x,y,w,h
		bi->x = bx;
		bi->y = by;
		bi->w = fl2i(scaled_w);
		bi->h = fl2i(scaled_h);

	}  // end if vertex is projected
}

#ifndef PLAT_UNIX
#pragma optimize("", on)
#endif

// -------------------------------------------------------------------------------------
// brief_render_icons()
//
void brief_render_icons(int stage_num, float frametime)
{
	int i, num_icons, num_lines;

	Assert( Briefing != NULL );
	
	num_icons = Briefing->stages[stage_num].num_icons;
	num_lines = Briefing->stages[stage_num].num_lines;

	if ( Cam_target_reached ) {
		for ( i = 0; i < num_lines; i++ ) {
			brief_render_icon_line(stage_num, i);
		}
	}

	for ( i = 0; i < num_icons; i++ ) {
		brief_render_icon(stage_num, i, frametime, 0);
	}
}

// ------------------------------------------------------------------------------------
// brief_start_highlight_anims()
//
//	see if there are any highlight animations to play
//
void brief_start_highlight_anims(int stage_num)
{
	brief_stage		*bs;
	brief_icon		*bi;
	int				x,y,i,anim_w,anim_h;

	Assert( Briefing != NULL );
	bs = &Briefing->stages[stage_num];
	
	for ( i = 0; i < bs->num_icons; i++ ) {
		bi = &bs->icons[i];
		if ( bi->flags & BI_HIGHLIGHT ) {
			bi->flags &= ~BI_SHOWHIGHLIGHT;
			if ( bi->highlight_anim.first_frame < 0 ) {
				continue;
			}

			bi->highlight_anim.time_elapsed=0.0f;

			bm_get_info( bi->highlight_anim.first_frame, &anim_w, &anim_h, NULL);
			x = fl2i( i2fl(bi->x) + bi->w/2.0f - anim_w/2.0f );
			y = fl2i( i2fl(bi->y) + bi->h/2.0f - anim_h/2.0f );
			bi->hold_x = x;
			bi->hold_y = y;
			bi->flags |= BI_SHOWHIGHLIGHT;
			bi->highlight_anim.time_elapsed=0.0f;
		}
	}
}

// -------------------------------------------------------------------------------------
// brief_render_map()
//
//
void brief_render_map(int stage_num, float frametime)
{
	brief_stage *bs;

	gr_set_clip(bscreen.map_x1 + 1, bscreen.map_y1 + 1, bscreen.map_x2 - bscreen.map_x1 - 1, bscreen.map_y2 - bscreen.map_y1 - 2);
	
	// REMOVED by neilk: removed gr_clear for FS2 because interface no longer calls for black background on grid
#ifdef MAKE_FS1
	gr_clear();
#endif

  if (stage_num >= Briefing->num_stages) {
		gr_reset_clip();
		return;
	}

	Assert(Briefing);
	bs = &Briefing->stages[stage_num];

	g3_start_frame(0);
	g3_set_view_matrix(&Current_cam_pos, &Current_cam_orient, 0.5f);

	brief_maybe_create_new_grid(The_grid, &Current_cam_pos, &Current_cam_orient);
	brief_render_grid(The_grid);

	brief_render_fade_outs(frametime);

	// go ahead and render everything that is in the active objects list
	brief_render_icons(stage_num, frametime);

	if ( Cam_target_reached && brief_text_wipe_finished() ) {

		if ( Brief_textdraw_finished == 0 ) {
			Brief_textdraw_finished = 1;
			Brief_stage_time = 0;
		}

		if ( Play_highlight_flag ) {
			brief_start_highlight_anims(stage_num);
			Play_highlight_flag = 0;
		}
	}

	anim_render_all(ON_BRIEFING_SELECT, frametime);

	gr_reset_clip();
	g3_end_frame();
}


// -------------------------------------------------------------------------------------
// brief_text_set_color()
//
void brief_text_set_color(char c) {
	switch ( c ) {
		case 'f':
			SET_COLOR_FRIENDLY;
			break;
		case 'h':
			SET_COLOR_HOSTILE;
			break;
		case 'n':
			SET_COLOR_NEUTRAL;
			break;
		case 'r':
			gr_set_color_fast(&Color_red);
			break;
		case 'g':
			gr_set_color_fast(&Color_green);
			break;
		case 'b':
			gr_set_color_fast(&Color_blue);
			break;
		default:	
			Int3();	// unsupported meta-code
			break;
	} // end switch
}

// Display what stage of the briefing is active
void brief_blit_stage_num(int stage_num, int stage_max)
{
	char buf[64];
	// int w;

	Assert( Briefing != NULL );
#ifdef MAKE_FS1
	gr_set_color_fast(&Color_bright_blue);
#else
	gr_set_color_fast(&Color_text_heading);
#endif
	sprintf(buf, XSTR( "Stage %d of %d", 394), stage_num + 1, stage_max);
	if (Game_mode & GM_MULTIPLAYER) {
		gr_printf(Brief_stage_text_coords_multi[gr_screen.res][0], Brief_stage_text_coords_multi[gr_screen.res][1], buf);
	} else {
		gr_printf(Brief_stage_text_coords[gr_screen.res][0], Brief_stage_text_coords[gr_screen.res][1], buf);
	}

	// draw the title of the mission	
	// if this goes above briefing text, it will need to be raised 10 pixels in multiplayer to make
	// room for stage num, which makes toom for chat box
	/*
	if (Game_mode & GM_MULTIPLAYER) {
		gr_get_string_size(&w,NULL,The_mission.name);
		gr_string(bscreen.map_x2 - w, bscreen.map_y2 + 5, The_mission.name);		
	} else {
		gr_get_string_size(&w,NULL,The_mission.name);
		gr_string(bscreen.map_x2 - w, bscreen.map_y2 + 5, The_mission.name);		
	}
	*/
}

// Render a line of text for the briefings.  Lines are drawn in as a wipe, with leading bright
// white characters.  Have to jump through some hoops since we support colored words.  This means
// that we need to process the line one character at a time.
void brief_render_line(int line_num, int x, int y, int instance)
{
	int len, count, next, truncate_len, last_color, offset, w, h, bright_len, i;
	colored_char *src;
	char line[MAX_BRIEF_LINE_LEN];

	src = &Colored_text[instance][line_num][0];
	len = Colored_text_len[instance][line_num];

	if (len <= 0){
		return;
	}

	truncate_len = fl2i(Brief_text_wipe_time_elapsed / BRIEF_TEXT_WIPE_TIME * Max_briefing_line_len);
	if (truncate_len > len){
		truncate_len = len;
	}

	bright_len = 0;
	if (truncate_len < len) {
		if (truncate_len <= BRIGHTEN_LEAD) {
			bright_len = truncate_len;
			truncate_len = 0;

		} else {
			bright_len = BRIGHTEN_LEAD;
			truncate_len -= BRIGHTEN_LEAD; 
		}
	}

	offset = 0;
	count  = 0;
	next	 = 0;

	gr_set_color_fast(&Color_white);
	last_color = BRIEF_TEXT_WHITE;
	for (i=0; i<truncate_len; i++) {
		if (count >= truncate_len){
			break;
		}

		line[next] = src[count].letter;

		if (is_white_space(line[next])) {
			// end of word reached, blit it
			line[next + 1] = 0;
			gr_string(x + offset, y, line);
			gr_get_string_size(&w, &h, line);
			offset += w;
			next = 0;

			// reset color
			if (last_color != BRIEF_TEXT_WHITE) {
				brief_set_text_color(BRIEF_TEXT_WHITE);
				last_color = BRIEF_TEXT_WHITE;
			}

			count++;
			continue;
		}

		if (src[count].color != last_color) {
			brief_set_text_color(src[count].color);
			last_color = src[count].color;
		}

		count++;
		next++;
	}	// end for

	line[next] = 0;
	gr_string(x + offset, y, line);


	// draw leading portion of the line bright white
	if (bright_len) {

		gr_set_color_fast(&Color_bright_white);
		for (i=0; i<truncate_len+bright_len; i++) {
			line[i] = src[i].letter;
		}

		line[i] = 0;


		if ( truncate_len > 0 )	{
			int width_dim, height_dim;
			gr_get_string_size(&width_dim, &height_dim, line, truncate_len );
			gr_string(x+width_dim, y, &line[truncate_len]);
		} else {
			gr_string(x, y, line);
		}

		// JAS: Not needed?
		//		// now erase the part we don't want to be bright white
		//		gr_set_color_fast(&Color_black);
		//		if (i > BRIGHTEN_LEAD) {
		//			line[i - BRIGHTEN_LEAD] = 0;
		//			gr_get_string_size(&w, &h, line);
		//			gr_set_clip(x, y, w, gr_get_font_height());
		//			gr_clear();
		//			gr_reset_clip();
		//		}
	}
}

int brief_text_wipe_finished()
{
	if ( Brief_text_wipe_time_elapsed > (BRIEF_TEXT_WIPE_TIME+0.5f) ) {
		return 1;
	}

	return 0;
}

// -------------------------------------------------------------------------------------
// brief_render_text()
//
// input:	frametime	=>	Time in seconds of previous frame
//				instance		=>	Optional parameter.  Used to indicate which text stream is used.
//									This value is 0 unless multiple text streams are required
int brief_render_text(int line_offset, int x, int y, int h, float frametime, int instance, int line_spacing)
{
	int fh, line, yy;

	fh = gr_get_font_height();
	if (Brief_text_wipe_time_elapsed == 0) {
		if (snd_is_playing(Brief_text_wipe_snd)) {
			snd_stop(Brief_text_wipe_snd);
		}
		gamesnd_play_iface(SND_BRIEF_TEXT_WIPE);
		Play_brief_voice = 1;
	}

	Brief_text_wipe_time_elapsed += frametime;

	line = line_offset;
	yy = 0;
	while (yy + fh <= h) {
		if (line >= Num_brief_text_lines[instance])
			break;

		brief_render_line(line, x, y + yy, instance);

		line++;
		yy += fh + line_spacing;
	}

	if ( brief_text_wipe_finished() && (Play_brief_voice) ) {
		Play_brief_voice = 0;
		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------
// brief_render_elements()
//
// Draw the lines that show objects positions on the grid
//
void brief_render_elements(vector *pos, grid* gridp)
{
	vector	gpos;	//	Location of point on grid.
//	vector	tpos;
	float		dxz;
	plane		tplane;
	vector	*gv;
	
	if ( pos->xyz.y < 1 && pos->xyz.y > -1 )
		return;

	tplane.A = gridp->gmatrix.v.uvec.xyz.x;
	tplane.B = gridp->gmatrix.v.uvec.xyz.y;
	tplane.C = gridp->gmatrix.v.uvec.xyz.z;
	tplane.D = gridp->planeD;

	compute_point_on_plane(&gpos, &tplane, pos);

	dxz = vm_vec_dist(pos, &gpos)/8.0f;

	gv = &gridp->gmatrix.v.uvec;
	if (gv->xyz.x * pos->xyz.x + gv->xyz.y * pos->xyz.y + gv->xyz.z * pos->xyz.z < -gridp->planeD)
		gr_set_color(127, 127, 127);
	else
		gr_set_color(255, 255, 255);   // white

// AL 11-20-97: don't draw elevation lines.. they are confusing
/*
	brief_rpd_line(&gpos, pos);	//	Line from grid to object center.

	tpos = gpos;

	vm_vec_scale_add2(&gpos, &gridp->gmatrix.v.rvec, -dxz/2);
	vm_vec_scale_add2(&gpos, &gridp->gmatrix.v.fvec, -dxz/2);
	
	vm_vec_scale_add2(&tpos, &gridp->gmatrix.v.rvec, dxz/2);
	vm_vec_scale_add2(&tpos, &gridp->gmatrix.v.fvec, dxz/2);
	
	brief_rpd_line(&gpos, &tpos);

	vm_vec_scale_add2(&gpos, &gridp->gmatrix.v.rvec, dxz);
	vm_vec_scale_add2(&tpos, &gridp->gmatrix.v.rvec, -dxz);

	brief_rpd_line(&gpos, &tpos);
*/
}


// ------------------------------------------------------------------------------------
// brief_reset_icons()
//
void brief_reset_icons(int stage_num)
{
	brief_stage		*bs;
	brief_icon		*bi;
	int				i;

	Assert( Briefing != NULL );
	bs = &Briefing->stages[stage_num];

	for ( i = 0; i < bs->num_icons; i++ ) {
		bi = &bs->icons[i];
		bi->flags &= ~BI_SHOWHIGHLIGHT;
	}
}

// ------------------------------------------------------------------------------------
// brief_set_camera_target()
//
//	input:	pos		=>		target position for the camera
//				orient	=>		target orientation for the camera
//				time		=>		time in ms to reach target
//
void brief_set_camera_target(vector *pos, matrix *orient, int time)
{
	float time_in_seconds;
	
	time_in_seconds = time / 1000.0f;

	if ( time == 0 ) {
		Current_cam_pos = *pos;
		Current_cam_orient = *orient;
	}

	Target_cam_pos = *pos;
	Target_cam_orient = *orient;
	Start_cam_orient = Current_cam_orient;
	Start_cam_pos = Current_cam_pos;								// we need this when checking if camera movement complete
	Start_cam_move = timer_get_milliseconds()*1000.0f;		// start time, convert to seconds
	Total_move_time = time_in_seconds;
	Elapsed_time = 0.0f;

	vm_vec_scale_add(&Start_lookat_pos, &Start_cam_pos, &Start_cam_orient.v.fvec, LOOKAT_DIST);
	vm_vec_scale_add(&Target_lookat_pos, &Target_cam_pos, &Target_cam_orient.v.fvec, LOOKAT_DIST);

	Play_highlight_flag = 1;								// once target reached, play highlight anims
	Cam_target_reached = 0;
	Cam_movement_done=0;
	anim_release_all_instances(ON_BRIEFING_SELECT);	// stop any briefing-specific anims
	
	// calculate camera velocity
	vm_vec_sub(&Cam_vel, pos, &Current_cam_pos);
//	vm_vec_scale(&Cam_vel, 1.0f/time_in_seconds);
	if ( !IS_VEC_NULL(&Cam_vel) ) {
		vm_vec_normalize(&Cam_vel);
	}

	// calculate lookat point velocity
	vm_vec_sub(&Lookat_vel, &Target_lookat_pos, &Current_lookat_pos);
	vm_vec_scale(&Lookat_vel, 1.0f/time_in_seconds);

	Start_dist = vm_vec_dist(&Start_cam_pos, &Start_lookat_pos);
	End_dist = vm_vec_dist(&Target_cam_pos, &Target_lookat_pos);
	Dist_change_rate = (End_dist - Start_dist) / time_in_seconds;

	Total_dist=vm_vec_dist(&Start_cam_pos, &Target_cam_pos);

//	Peak_speed=Total_dist/Total_move_time*1.5f;
//	Cam_accel = Peak_speed/Total_move_time*3.0f;

	Peak_speed=Total_dist/Total_move_time*2.0f;
	Cam_accel = 4*Total_dist/(Total_move_time*Total_move_time);
	Last_dist=0.0f;

	vm_vec_zero(&W_init);

	get_camera_limits(&Start_cam_orient, &Target_cam_orient, Total_move_time, &Acc_limit, &Vel_limit);
}


ubyte brief_return_color_index(char c)
{
	switch (c) {
		case 'f':
			return BRIEF_IFF_FRIENDLY;

		case 'h':
			return BRIEF_IFF_HOSTILE;

		case 'n':
			return BRIEF_IFF_NEUTRAL;

		case 'r':
			return BRIEF_TEXT_RED;

		case 'g':
			return BRIEF_TEXT_GREEN;

		case 'b':
			return BRIEF_TEXT_BLUE;

		default:	
			Int3();	// unsupported meta-code
			break;
	} // end switch

	return BRIEF_TEXT_WHITE;
}

void brief_set_text_color(int color_index)
{
	Assert(color_index < MAX_BRIEF_TEXT_COLORS);
	gr_set_color_fast(Brief_text_colors[color_index]);
}

// Set up the Colored_text array.
// input:		index		=>		Index into Brief_text[] for source text.
//					instance	=>		Which instance of Colored_text[] to use.  
//										Value is 0 unless multiple text streams are required.
int brief_text_colorize(int index, int instance)
{
	char *src;
	int len, i, skip_to_next_word, dest_len;
	colored_char *dest;
	ubyte active_color_index;

	src = Brief_text[index];
	dest = &Colored_text[instance][index][0];
	len = strlen(src);

	skip_to_next_word = 0;
	dest_len = 0;
	active_color_index = BRIEF_TEXT_WHITE;
	for (i=0; i<len; i++) {
		if (skip_to_next_word) {
			if (is_white_space(src[i])) {
				skip_to_next_word = 0;
			}

			continue;
		}

		if ( src[i] == BRIEF_META_CHAR && is_white_space(src[i + 2]) ) {
			active_color_index = brief_return_color_index(src[i + 1]);
			skip_to_next_word = 1;
			continue;
		}

		if (is_white_space(src[i])) {
			active_color_index = BRIEF_TEXT_WHITE;
		}

		dest[dest_len].letter = src[i];
		dest[dest_len].color  = active_color_index;
		dest_len++;
	} // end for

	dest[dest_len].letter = 0;
	Colored_text_len[instance][index] = dest_len;
	return len;
}

// ------------------------------------------------------------------------------------
// brief_color_text_init()
//
//	input:	src		=>		paragraph of text to process
//				w			=>		max width of line in pixels
//				instance	=>		optional parameter, used when multiple text streams are required
//									(default value is 0)
int brief_color_text_init(char *src, int w, int instance)
{
	int i, n_lines, len;
	int n_chars[MAX_BRIEF_LINES];
	char *p_str[MAX_BRIEF_LINES];
	
	Assert(src);
	n_lines = split_str(src, w, n_chars, p_str, MAX_BRIEF_LINES, BRIEF_META_CHAR);
	Assert(n_lines >= 0);

	Max_briefing_line_len = 1;
	for (i=0; i<n_lines; i++) {
		Assert(n_chars[i] < MAX_BRIEF_LINE_LEN);
		strncpy(Brief_text[i], p_str[i], n_chars[i]);
		Brief_text[i][n_chars[i]] = 0;
		drop_leading_white_space(Brief_text[i]);
		len = brief_text_colorize(i, instance);
		if (len > Max_briefing_line_len)
			Max_briefing_line_len = len;
	}

	Brief_text_wipe_time_elapsed = 0.0f;
	Play_brief_voice = 0;

	Num_brief_text_lines[instance] = n_lines;
	return n_lines;
}

// ------------------------------------------------------------------------------------
// brief_get_free_move_icon()
//
//	returns:		failure	=>		-1
//					success	=>		handle to a free move icon struct
//
int brief_get_free_move_icon()
{
	int i;

	for ( i = 0; i < MAX_MOVE_ICONS; i++ ) {
		if ( Icon_movers[i].used == 0 )
			break;
	}
	
	if ( i == MAX_MOVE_ICONS ) 
		return -1;

	Icon_movers[i].used = 1;
	return i;
}


// ------------------------------------------------------------------------------------
// brief_set_move_list()
//
//	input:	new_stage		=>		new stage number that briefing is now moving to
//				current_stage	=>		current stage that the briefing is on
//				time				=>		time in seconds
//
int brief_set_move_list(int new_stage, int current_stage, float time)
{
	brief_stage		*newb, *cb;	
	icon_move_info	*imi;	
	int				i,j,k,num_movers,is_gone=0;
	vector			zero_v = ZERO_VECTOR;

	Assert(new_stage != current_stage);
	
	Assert( Briefing != NULL );
	newb = &Briefing->stages[new_stage];
	cb = &Briefing->stages[current_stage];
	num_movers = 0;
	
	for ( i = 0; i < cb->num_icons; i++ ) {
		is_gone=1;
		for ( j = 0; j < newb->num_icons; j++ ) {
			if ( ( cb->icons[i].id != 0 ) && ( cb->icons[i].id == newb->icons[j].id ) ) {
				is_gone=0;
				if ( vm_vec_cmp(&cb->icons[i].pos, &newb->icons[j].pos) ) {
					//nprintf(("Alan","We found a match in icon %s\n", cb->icons[i].label));
					k = brief_get_free_move_icon();				
					if ( k == -1 ) {
						Int3();	// should never happen, get Alan
						return 0;
					}
					imi = &Icon_movers[k];
					imi->id = cb->icons[i].id;
					imi->start = cb->icons[i].pos;
					imi->finish = newb->icons[j].pos;
					imi->current = imi->start;
					list_append(&Icon_move_list, imi);

					imi->total_dist = vm_vec_dist(&imi->start, &imi->finish);
					imi->total_move_time = time;
					imi->peak_speed = imi->total_dist/imi->total_move_time*2.0f;
					imi->accel = 4*imi->total_dist/(time*time);
					imi->last_dist=0.0f;
					imi->reached_dest=0;
					imi->direction = zero_v;

					vm_vec_sub(&imi->direction, &imi->finish, &imi->start);
					if ( !IS_VEC_NULL(&imi->direction) ) {
						vm_vec_normalize(&imi->direction);
					}

					num_movers++;
				}
			}
		}

		// Set up fading icon (to fade out)
		if (is_gone == 1) {
			if ( Num_fade_icons >= MAX_FADE_ICONS ) {
				Int3();
				Num_fade_icons=0;
			}

			int species = ship_get_species_by_type(cb->icons[i].ship_class);

#ifdef MAKE_FS1
			species = 0;
#endif

			if(species < 0) {
				return 0;
			}

			Fading_icons[Num_fade_icons].fade_anim = Icon_fade_anims[cb->icons[i].type][species];
			Fading_icons[Num_fade_icons].pos = cb->icons[i].pos;
			Fading_icons[Num_fade_icons].team = cb->icons[i].team;
			Num_fade_icons++;
		}
	}

	// flag new icons for fading in
	for ( i=0; i<newb->num_icons; i++ ) {
		int is_new = 1;
		newb->icons[i].flags &= ~BI_FADEIN;
		for ( j=0; j<cb->num_icons; j++ ) {
			if ( ( cb->icons[j].id != 0 ) && ( cb->icons[j].id == newb->icons[i].id ) ) {
				is_new=0;
			}
		}
		if ( is_new ) {
			int species = ship_get_species_by_type(newb->icons[i].ship_class);

#ifdef MAKE_FS1
			species = 0;
#endif

			if(species < 0) {
				return 0;
			}

			newb->icons[i].flags |= BI_FADEIN;
			newb->icons[i].fadein_anim = Icon_fade_anims[newb->icons[i].type][species];
			newb->icons[i].fadein_anim.time_elapsed = 0.0f;
		}
	}

	return num_movers;
}

void brief_clear_fade_out_icons()
{
	Num_fade_icons = 0;
}


// ------------------------------------------------------------------------------------
// brief_set_new_stage()
//
//	input:	pos			=>		target position for the camera
//				orient		=>		target orientation for the camera
//				time			=>		time in ms to reach target
//				stage_num	=>		stage number of briefing (start numbering at 0)
//

void brief_set_new_stage(vector *pos, matrix *orient, int time, int stage_num)
{
	char msg[MAX_BRIEF_LEN];
	int num_movers, new_time, not_objv = 1;

	Assert( Briefing != NULL );
	new_time = time;

	if (stage_num >= Briefing->num_stages) {
		not_objv = 0;  // turns out this is an objectives stage
		new_time = 0;
	}

	if ( stage_num == Last_new_stage ) {
		return;
	}

	num_movers = 0;
	brief_move_icon_reset();
	brief_clear_fade_out_icons();
	if ( (Last_new_stage != -1) && not_objv ) {
		num_movers = brief_set_move_list(stage_num, Last_new_stage, new_time / 1000.0f);
	}

	if ( (Last_new_stage != -1) && (num_movers == 0) && not_objv ) {
		if ( !vm_vec_cmp( &Briefing->stages[stage_num].camera_pos, &Briefing->stages[Last_new_stage].camera_pos) ) {
			if ( !vm_vec_cmp( &Briefing->stages[stage_num].camera_orient.v.fvec, &Briefing->stages[Last_new_stage].camera_orient.v.fvec) ){
				new_time = 0;
			}
		}
	}

	if (not_objv) {
		if(Briefing->stages[stage_num].new_text == NULL){
			strcpy(msg, "");
		} else {
			strcpy(msg, Briefing->stages[stage_num].new_text);
		}
	} else {
		strcpy(msg, XSTR( "Please review your objectives for this mission.", 395));
	}

	if (gr_screen.res == GR_640) {
		// GR_640
		Num_brief_text_lines[0] = brief_color_text_init(msg, MAX_BRIEF_LINE_W_640);
	} else {
		// GR_1024
		Num_brief_text_lines[0] = brief_color_text_init(msg, MAX_BRIEF_LINE_W_1024);		
	}
	Top_brief_text_line = 0;

	if (not_objv){
		brief_set_camera_target(pos, orient, new_time);
	}

	if ( snd_is_playing(Brief_stage_highlight_sound_handle) ) {
		snd_stop(Brief_stage_highlight_sound_handle);
	}

	Brief_voice_ended = 0;
	Brief_textdraw_finished = 0;
	Brief_voice_started = 0;
	Brief_stage_time = 0;


	Brief_stage_highlight_sound_handle = -1;
	Last_new_stage = stage_num;
}

// ------------------------------------------------------------------------------------
// camera_pos_past_target()
//
//
int camera_pos_past_target(vector *start, vector *current, vector *dest)
{
	vector num, den;
	float ratio;

	vm_vec_sub(&num, current, start);
	vm_vec_sub(&den, start, dest);

	ratio = vm_vec_mag_quick(&num) / vm_vec_mag_quick(&den);
	if (ratio >= 1.0f)
		return TRUE;
	
	return FALSE;
}

// ------------------------------------------------------------------------------------
// Interpolate between matrices.
// elapsed_time/total_time gives percentage of interpolation between cur
// and goal.
void interpolate_matrix(matrix *result, matrix *goal, matrix *start, float elapsed_time, float total_time)
{
	vector fvec, rvec;
	float	time0, time1;
	
	if ( !vm_matrix_cmp( goal, start ) ) {
		return;
	}	

	time0 = elapsed_time / total_time;
	time1 = (total_time - elapsed_time) / total_time;

	vm_vec_copy_scale(&fvec, &start->v.fvec, time1);
	vm_vec_scale_add2(&fvec, &goal->v.fvec, time0);

	vm_vec_copy_scale(&rvec, &start->v.rvec, time1);
	vm_vec_scale_add2(&rvec, &goal->v.rvec, time0);

	vm_vector_2_matrix(result, &fvec, NULL, &rvec);
 }

// calculate how far the camera should have moved
float brief_camera_get_dist_moved(float elapsed_time)
{
	float time, dist_moved=0.0f;
	
	// first half of movement
	if ( elapsed_time < Total_move_time/2.0f ) {
		dist_moved=0.5f*Cam_accel*elapsed_time*elapsed_time;	// d = 1/2at^2
		return dist_moved;
	}

	// second half of movement
	time=elapsed_time - Total_move_time/2.0f;
	dist_moved=(Total_dist/2.0f)+(Peak_speed*time) - 0.5f*Cam_accel*time*time;
	return dist_moved;

}

// ------------------------------------------------------------------------------------
// Update the camera position
void brief_camera_move(float frametime, int stage_num)
{
	vector	dist_moved;
	float		dist;
	vector	w_out;
	matrix	result;

	Elapsed_time += frametime;

	if ( Cam_target_reached ) { 
//		Current_cam_pos = Target_cam_pos;
//		Current_lookat_pos = Target_lookat_pos;
//		Current_cam_orient = Target_cam_orient;
		return;
	}

	// Update orientation
	if ( (Elapsed_time < Total_move_time) ) {
//		interpolate_matrix(&Current_cam_orient, &Target_cam_orient, &Start_cam_orient, Elapsed_time, Total_move_time );
		vm_matrix_interpolate(&Target_cam_orient, &Current_cam_orient, &W_init, frametime, &result, &w_out, &Vel_limit, &Acc_limit);
		Current_cam_orient = result;
		W_init = w_out;
	}

	/*
	// interpolate lookat position
	if ( vm_vec_cmp( &Current_lookat_pos, &Target_lookat_pos ) ) {
		vm_vec_copy_scale(&dist_moved, &Lookat_vel, Elapsed_time);
		vm_vec_add(&Current_lookat_pos, &Start_lookat_pos, &dist_moved);

		if ( camera_pos_past_target(&Start_lookat_pos, &Current_lookat_pos, &Target_lookat_pos) ) {
			Current_lookat_pos = Target_lookat_pos;
		}
	}

	cur_dist = Start_dist + Dist_change_rate * Elapsed_time;
	vm_vec_copy_scale(&dist_moved, &Current_cam_orient.v.fvec, -cur_dist);
	vm_vec_add(&Current_cam_pos, &Current_lookat_pos, &dist_moved);
	*/

	// use absolute pos to update position
	if ( vm_vec_cmp( &Current_cam_pos, &Target_cam_pos ) ) {
		dist = brief_camera_get_dist_moved(Elapsed_time);
		if ( dist < Last_dist ) {
			Cam_movement_done=1;
			Last_dist=0.0f;
		}
		Last_dist=dist;

		if ( Cam_movement_done == 0 ) {
			vm_vec_copy_scale(&dist_moved, &Cam_vel, dist);
			vm_vec_add(&Current_cam_pos, &Start_cam_pos, &dist_moved);
		} else {
			Current_cam_pos=Target_cam_pos;
		}
	}
	else {
		Cam_movement_done=1;
		Current_cam_pos=Target_cam_pos;
	}

	if ( Cam_movement_done && (Elapsed_time >= Total_move_time) ) {
		Cam_target_reached=1;
	}
}

//	Project the viewer's position onto the grid plane.  If more than threshold distance
//	from grid center, move grid center.
void brief_maybe_create_new_grid(grid* gridp, vector *pos, matrix *orient, int force)
{
	int roundoff;
	plane	tplane;
	vector	gpos, tmp, c;
	float	dist_to_plane;
	float	square_size, ux, uy, uz;

	ux = tplane.A = gridp->gmatrix.v.uvec.xyz.x;
	uy = tplane.B = gridp->gmatrix.v.uvec.xyz.y;
	uz = tplane.C = gridp->gmatrix.v.uvec.xyz.z;
	tplane.D = gridp->planeD;

	compute_point_on_plane(&c, &tplane, pos);
	dist_to_plane = fl_abs(vm_dist_to_plane(pos, &gridp->gmatrix.v.uvec, &c));
	square_size = 1.0f;

	while (dist_to_plane >= 25.0f)
	{
		square_size *= 10.0f;
		dist_to_plane /= 10.0f;
	}
	
	if (fvi_ray_plane(&gpos, &gridp->center, &gridp->gmatrix.v.uvec, pos, &orient->v.fvec, 0.0f)<0.0f)	{
		vector p;
		vm_vec_scale_add(&p,pos,&orient->v.fvec, 100.0f );
		compute_point_on_plane(&gpos, &tplane, &p );
	}

	if (vm_vec_dist(&gpos, &c) > 50.0f * square_size)
	{
		vm_vec_sub(&tmp, &gpos, &c);
		vm_vec_normalize(&tmp);
		vm_vec_scale_add(&gpos, &c, &tmp, 50.0f * square_size);
	}

	roundoff = (int) square_size * 10;
	if (!ux)
		gpos.xyz.x = fl_roundoff(gpos.xyz.x, roundoff);
	if (!uy)
		gpos.xyz.y = fl_roundoff(gpos.xyz.y, roundoff);
	if (!uz)
		gpos.xyz.z = fl_roundoff(gpos.xyz.z, roundoff);

	if ((square_size != gridp->square_size) ||
		(gpos.xyz.x != gridp->center.xyz.x) ||
		(gpos.xyz.y != gridp->center.xyz.y) ||
		(gpos.xyz.z != gridp->center.xyz.z) || force)
	{
		gridp->square_size = square_size;
		gridp->center = gpos;
		brief_modify_grid(gridp);
	}
}

//	Create a grid
//	*forward is vector pointing forward
//	*right is vector pointing right
//	*center is center point of grid
//	length is length of grid
//	width is width of grid
//	square_size is size of a grid square
//	For example:
//		*forward = (0.0, 0.0, 1.0)
//		*right   = (1.0, 0.0, 0.0)
//		*center = (0.0, 0.0, 0.0)
//		nrows = 10
//		ncols =  50.0
//		square_size = 10.0
//	will generate a grid of squares 10 long by 5 wide.
//	Each grid square will be 10.0 x 10.0 units.
//	The center of the grid will be at the global origin.
//	The grid will be parallel to the xz plane (because the normal is 0,1,0).
//	(In fact, it will be the xz plane because it is centered on the origin.)
//
//	Stuffs grid in *gridp.  If gridp == NULL, mallocs and returns a grid.
grid *brief_create_grid(grid *gridp, vector *forward, vector *right, vector *center, int nrows, int ncols, float square_size)
{
	int	i, ncols2, nrows2, d = 1;
	vector	dfvec, drvec, cur, cur2, tvec, uvec, save, save2;

	Assert(square_size > 0.0);
	if (double_fine_gridlines)
		d = 2;

	if (gridp == NULL)
		gridp = (grid *) malloc(sizeof(grid));

	Assert(gridp);

	gridp->center = *center;
	gridp->square_size = square_size;

	//	Create the plane equation.
	Assert(!IS_VEC_NULL(forward));
	Assert(!IS_VEC_NULL(right));

	vm_vec_copy_normalize(&dfvec, forward);
	vm_vec_copy_normalize(&drvec, right);

	vm_vec_cross(&uvec, &dfvec, &drvec);
	
	Assert(!IS_VEC_NULL(&uvec));

	gridp->gmatrix.v.uvec = uvec;

	gridp->planeD = -(center->xyz.x * uvec.xyz.x + center->xyz.y * uvec.xyz.y + center->xyz.z * uvec.xyz.z);
	Assert(!_isnan(gridp->planeD));

	gridp->gmatrix.v.fvec = dfvec;
	gridp->gmatrix.v.rvec = drvec;

	vm_vec_scale(&dfvec, square_size);
	vm_vec_scale(&drvec, square_size);

	vm_vec_scale_add(&cur, center, &dfvec, (float) -nrows * d / 2);
	vm_vec_scale_add2(&cur, &drvec, (float) -ncols * d / 2);
	vm_vec_scale_add(&cur2, center, &dfvec, (float) -nrows * 5 / 2);
	vm_vec_scale_add2(&cur2, &drvec, (float) -ncols * 5 / 2);
	save = cur;
	save2 = cur2;

	gridp->ncols = ncols;
	gridp->nrows = nrows;
	ncols2 = ncols / 2;
	nrows2 = nrows / 2;
	Assert(ncols < MAX_GRIDLINE_POINTS && nrows < MAX_GRIDLINE_POINTS);

	// Create the points along the edges of the grid, so we can just draw lines
	// between them to form the grid.  
	for (i=0; i<=ncols*d; i++) {
		gridp->gpoints1[i] = cur;  // small, dark gridline points
		vm_vec_scale_add(&tvec, &cur, &dfvec, (float) nrows * d);
		gridp->gpoints2[i] = tvec;
		vm_vec_add2(&cur, &drvec);
	}

	for (i=0; i<=ncols2; i++) {
		gridp->gpoints5[i] = cur2;  // large, brighter gridline points
		vm_vec_scale_add(&tvec, &cur2, &dfvec, (float) nrows2 * 10);
		gridp->gpoints6[i] = tvec;
		vm_vec_scale_add2(&cur2, &drvec, 10.0f);
	}

	cur = save;
	cur2 = save2;
	for (i=0; i<=nrows*d; i++) {
		gridp->gpoints3[i] = cur;  // small, dark gridline points
		vm_vec_scale_add(&tvec, &cur, &drvec, (float) ncols * d);
		gridp->gpoints4[i] = tvec;
		vm_vec_add2(&cur, &dfvec);
	}

	for (i=0; i<=nrows2; i++) {
		gridp->gpoints7[i] = cur2;  // large, brighter gridline points
		vm_vec_scale_add(&tvec, &cur2, &drvec, (float) ncols2 * 10);
		gridp->gpoints8[i] = tvec;
		vm_vec_scale_add2(&cur2, &dfvec, 10.0f);
	}

	return gridp;
}

//	Create a nice grid -- centered at origin, 10x10, 10.0 size squares, in xz plane.
grid *brief_create_default_grid(void)
{
	grid	*rgrid;
	vector	fvec, rvec, cvec;

	rgrid = brief_create_grid(&Global_grid, vm_vec_make(&fvec, 0.0f, 0.0f, 1.0f),
		vm_vec_make(&rvec, 1.0f, 0.0f, 0.0f),
		vm_vec_make(&cvec, 0.0f, -10.0f, 0.0f), 100, 100, 5.0f);

	physics_init(&rgrid->physics);
	rgrid->physics.flags |= (PF_ACCELERATES | PF_SLIDE_ENABLED);
	return rgrid;
}

//	Rotate and project points and draw a line.
void brief_rpd_line(vector *v0, vector *v1)
{
	vertex	tv0, tv1;
	g3_rotate_vertex(&tv0, v0);
	g3_rotate_vertex(&tv1, v1);

/*
	g3_project_vertex(&tv0);	
	g3_project_vertex(&tv1);

	if ( (tv0.flags & PF_OVERFLOW) || (tv1.flags & PF_OVERFLOW) )
		return;
*/

	gr_set_color_fast(&Color_grey);
	g3_draw_line(&tv0, &tv1);
}

//	Renders a grid defined in a grid struct
void brief_render_grid(grid *gridp)
{
	int	i, ncols, nrows;

	ncols = gridp->ncols;
	nrows = gridp->nrows;
	if (double_fine_gridlines)
	{
		ncols *= 2;
		nrows *= 2;
	}

	gr_set_color(30,30,30);
//	SET_DARK;

	//	Draw the column lines.
	for (i=0; i<=ncols; i++)
		brief_rpd_line(&gridp->gpoints1[i], &gridp->gpoints2[i]);

	//	Draw the row lines.
	for (i=0; i<=nrows; i++)
		brief_rpd_line(&gridp->gpoints3[i], &gridp->gpoints4[i]);

	ncols = gridp->ncols / 2;
	nrows = gridp->nrows / 2;

	// now draw the larger, brighter gridlines that is x10 the scale of smaller one.
//	SET_MEDIUM;
/*
	for (i=0; i<=ncols; i++)
		brief_rpd_line(&gridp->gpoints5[i], &gridp->gpoints6[i]);

	for (i=0; i<=nrows; i++)
		brief_rpd_line(&gridp->gpoints7[i], &gridp->gpoints8[i]);
*/
}

void brief_modify_grid(grid *gridp)
{
	brief_create_grid(gridp, &gridp->gmatrix.v.fvec, &gridp->gmatrix.v.rvec, &gridp->center,
		gridp->nrows, gridp->ncols, gridp->square_size);
}

void brief_unload_anims()
{
	int idx;
	int i;
	
	for (i=0; i<MAX_BRIEF_ICONS; i++) {
		for(idx=0; idx<MAX_SPECIES_NAMES; idx++){
			hud_anim_release(&Icon_highlight_anims[i][idx]);
			hud_anim_release(&Icon_fade_anims[i][idx]);
		}
	}
}

void brief_common_close()
{
	brief_close_map();
	brief_unload_anims();
}


void brief_restart_text_wipe()
{
	Brief_stage_time = 0;
	Brief_voice_ended = 0;
	Brief_voice_started = 0;
	Brief_text_wipe_time_elapsed = 0.0f;
}

// initialize the array of handles to the different voice streams
void brief_voice_init()
{
	int i;
	for ( i = 0; i < MAX_BRIEF_STAGES; i++ ) {
		Brief_voices[i] = -1;
	}
}

void brief_load_voice_file(int voice_num, char *name)
{
	int load_attempts = 0;
	while(1) {

		if ( load_attempts++ > 5 ) {
			break;
		}

		Brief_voices[voice_num] = audiostream_open( name, ASF_VOICE );
		if ( Brief_voices[voice_num] >= 0 ) {
			break;
		}

		// Don't bother to ask for the CD in multiplayer
		if ( Game_mode & GM_MULTIPLAYER ) {
			break;
		}

		// couldn't load animation, ask user to insert CD (if necessary)
		// if ( Brief_voice_ask_for_cd ) {
			// if ( game_do_cd_check() == 0 ) {
				// Brief_voice_ask_for_cd = 0;
				// break;
			// }
		// }
	}
}

// open and pre-load the stream buffers for the different voice streams
void brief_voice_load_all()
{
	int			i;
	brief_stage	*bs;

	// Brief_voice_ask_for_cd = 1;

	Assert( Briefing != NULL );
	for ( i = 0; i < Briefing->num_stages; i++ ) {
		bs = &Briefing->stages[i];
		if ( strnicmp(bs->voice, NOX("none"), 4) ) {
			brief_load_voice_file(i, bs->voice);
//			Brief_voices[i] = audiostream_open( bs->voice, ASF_VOICE );
		}
	}
}

// close all the briefing voice streams
void brief_voice_unload_all()
{
	int i;

	for ( i = 0; i < MAX_BRIEF_STAGES; i++ ) {
		if ( Brief_voices[i] != -1 ) {
			audiostream_close_file(Brief_voices[i], 0);
			Brief_voices[i] = -1;
		}
	}
}

// start playback of the voice for a particular briefing stage
void brief_voice_play(int stage_num)
{
	if ( Brief_voices[stage_num] == -1 )
		return;	// voice file doesn't exist

	if ( !Briefing_voice_enabled ) {
		return;
	}

	if ( audiostream_is_playing( Brief_voices[stage_num]) )
		return;

	audiostream_play(Brief_voices[stage_num], Master_voice_volume, 0);
	Brief_voice_started = 1;
}

// stop playback of the voice for a particular briefing stage
void brief_voice_stop(int stage_num)
{
	if ( Brief_voices[stage_num] == -1 )
		return;

	audiostream_stop(Brief_voices[stage_num]);	// stream is automatically rewound
}

// pause playback of the voice for a particular briefing stage, to resume just
// call brief_voice_unpause() again
void brief_voice_pause(int stage_num)
{
	if ( Brief_voices[stage_num] == -1 )
		return;

	audiostream_pause(Brief_voices[stage_num]);
}

void brief_voice_unpause(int stage_num)
{
	if ( Brief_voices[stage_num] == -1 )
		return;

	audiostream_unpause(Brief_voices[stage_num]);
}

void brief_reset_last_new_stage()
{
	Last_new_stage = -1;
}

// get the dimensions for a briefing icon
void brief_common_get_icon_dimensions(int *w, int *h, int type, int ship_class)
{
	Assert(type >= 0 && type < MAX_BRIEF_ICONS);

	// in case anything goes wrong
	*w=0;
	*h=0;

	int species = ship_get_species_by_type(ship_class);

#ifdef MAKE_FS1
	species = 0;
#endif

	if(species < 0){
		return;
	}

	if ( Icon_bitmaps[type][species].first_frame >= 0 ) {
		bm_get_info( Icon_bitmaps[type][species].first_frame, w, h, NULL);
	} else {
		*w=0;
		*h=0;
	}
}

void cmd_brief_reset()
{
	int i, j;
	static int inited = 0;

	if (inited) {
		for (i=0; i<MAX_TEAMS; i++) {
			for (j=0; j<Cmd_briefs[i].num_stages; j++) {
				if (Cmd_briefs[i].stage[j].text)
					free(Cmd_briefs[i].stage[j].text);
			}
		}
	}

	inited = 1;
	for (i=0; i<MAX_TEAMS; i++)
		Cmd_briefs[i].num_stages = 0;
}

#define STAGE_ADVANCE_DELAY	1000		// time in ms to wait after voice stops before advancing stage

// should briefing advance to the next stage?
int brief_time_to_advance(int stage_num, float frametime)
{
	int voice_active, advance = 0;
	brief_icon *closeup_icon;

	closeup_icon = brief_get_closeup_icon();

	if ( closeup_icon ) {
		return 0;
	}

	if ( !Player->auto_advance ) {
		return 0;
	}

	Brief_stage_time += fl2i(frametime*1000 + 0.5f);

	if ( (Brief_voices[stage_num] >= 0) && Briefing_voice_enabled ) {
		voice_active = 1;
	} else {
		voice_active = 0;
	}

	if ( voice_active && (Brief_voice_ended == 0) && Brief_voice_started) {
		if ( !audiostream_is_playing( Brief_voices[stage_num]) ) {
			Brief_voice_ended = 1;
			Brief_stage_time = 0;
		}
	}
	
	if ( Brief_voice_ended ) {
		if ( Brief_stage_time > STAGE_ADVANCE_DELAY ) {
			advance = 1;
		}
	}

	if ( !voice_active && (Brief_textdraw_finished > 0) ) {
		if ( Brief_stage_time > max(5000, Num_brief_text_lines[0] * 3500) ) {
			advance = 1;
		}
	}

	return advance;
}
