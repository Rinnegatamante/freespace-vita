/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MissionUI/RedAlert.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Module for Red Alert mission interface and code
 *
 * $Log$
 * Revision 1.8  2005/03/29 02:18:47  taylor
 * Various 64-bit platform fixes
 * Fix compiler errors with MAKE_FS1 and fix gr_set_bitmap() too
 * Make sure that turrets can fire at asteroids for FS1 (needed for a couple missions)
 * Streaming audio support (big thanks to Pierre Willenbrock!!)
 * Removed dependance on strings.tbl for FS1 since we don't actually need it now
 *
 * Revision 1.7  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.6  2003/08/03 16:10:29  taylor
 * cleanup; compile warning fixes
 *
 * Revision 1.5  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.4  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.3  2002/05/26 22:06:17  relnev
 * makefile: disable stand_gui for now.
 *
 * rest: staticize some globals
 *
 * Revision 1.2  2002/05/03 13:34:33  theoddone33
 * More stuff compiles
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 16    9/11/99 12:31a Mikek
 * Bumped up max red alert status ships and status subsystems.
 * 
 * 15    9/06/99 6:38p Dave
 * Improved CD detection code.
 * 
 * 14    8/24/99 5:27p Andsager
 * Make subsystems with zero strength before mission blown off.  Protect
 * red alert pilot against dying between orders and jump.
 * 
 * 13    8/11/99 2:17p Jefff
 * changed a string
 * 
 * 12    7/19/99 8:56p Andsager
 * Added Ship_exited red_alert_carry flag and hull strength for RED ALERT
 * carry over of Exited_ships
 * 
 * 11    7/19/99 2:13p Dave
 * Added some new strings for Heiko.
 * 
 * 10    7/16/99 12:22p Jefff
 * Added sound FX to red alert popup
 * 
 * 9     7/09/99 10:32p Dave
 * Command brief and red alert screens.
 * 
 * 8     5/07/99 10:34a Andsager
 * Make red alert work in FS2
 * 
 * 7     2/16/99 5:07p Neilk
 * Added hires support
 * 
 * 6     1/30/99 5:08p Dave
 * More new hi-res stuff.Support for nice D3D textures.
 * 
 * 5     1/27/99 9:56a Dave
 * Temporary checkin of beam weapons for Dan to make cool sounds.
 * 
 * 4     11/18/98 11:15a Andsager
 * Removed old version or red_alert_read_wingman_status
 * 
 * 3     10/13/98 9:29a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 21    5/14/98 11:26a Lawrance
 * ESC will return to the main hall
 * 
 * 20    5/13/98 5:14p Allender
 * red alert support to go back to previous mission
 * 
 * 19    5/05/98 6:19p Lawrance
 * Fix problems with "retry mission" for red alerts
 * 
 * 18    5/05/98 3:14p Comet
 * AL: Fix bug with restoring secondary weapons in red alert mode
 * 
 * 17    5/04/98 10:49p Lawrance
 * Make sure old pilot files don't cause problems with the new RedAlert
 * data format
 * 
 * 16    5/04/98 6:06p Lawrance
 * Make red alert mode work!
 * 
 * 15    5/02/98 4:10p Lawrance
 * Only use first stage of briefing for red alerts
 * 
 * 14    5/01/98 9:18p Ed
 * mark all goals and events as failed when red alert moves ahead to next
 * mission
 * 
 * 13    4/22/98 10:13a John
 * added assert to trap a bug
 * 
 * 12    4/21/98 12:08a Allender
 * only make red alert move to next mission in campaign mode
 * 
 * 11    4/03/98 10:31a John
 * Made briefing and debriefing arrays be malloc'd
 * 
 * 10    3/28/98 2:53p Allender
 * added hud gauge when entering a red alert mission
 * 
 * 9     3/12/98 10:28p Lawrance
 * Deal with situation where wingman status may not have been properly
 * saved
 * 
 * 8     3/12/98 9:44p Allender
 * go to debrief in red alert when not in campaign mode
 * 
 * 6     3/09/98 9:55p Lawrance
 * improve some comments
 * 
 * 5     3/09/98 4:41p Lawrance
 * Fix up some merge problems.
 * 
 * 4     3/09/98 4:30p Allender
 * multiplayer secondary weapon changes.  red-alert and cargo-known-delay
 * sexpressions.  Add time cargo revealed to ship structure
 * 
 * 3     3/09/98 4:23p Lawrance
 * Replay mission, full save/restore of wingman status
 * 
 * 2     3/09/98 12:13a Lawrance
 * Add support for Red Alert missions
 * 
 * 1     3/08/98 4:54p Lawrance
 *
 * $NoKeywords: $
 */

#include "ui.h"
#include "gamesnd.h"
#include "gamesequence.h"
#include "missionscreencommon.h"
#include "key.h"
#include "bmpman.h"
#include "font.h"
#include "redalert.h"
#include "hud.h"
#include "missionbriefcommon.h"
#include "timer.h"
#include "missioncampaign.h"
#include "missiongoals.h"
#include "linklist.h"
#include "hudwingmanstatus.h"
#include "audiostr.h"
#include "freespace.h"
#include "alphacolors.h"

/////////////////////////////////////////////////////////////////////////////
// Red Alert Mission-Level
/////////////////////////////////////////////////////////////////////////////

static int Red_alert_status;
static int Red_alert_new_mission_timestamp;		// timestamp used to give user a little warning for red alerts
static int Red_alert_num_slots_used = 0;
static int Red_alert_voice_started;

#define RED_ALERT_WARN_TIME		4000				// time to warn user that new orders are coming

#define RED_ALERT_NONE				0
#define RED_ALERT_MISSION			1

#define MAX_RED_ALERT_SLOTS				32
#define MAX_RED_ALERT_SUBSYSTEMS		64
#define RED_ALERT_EXITED_SHIP_CLASS		-1

typedef struct red_alert_ship_status
{
	char	name[NAME_LENGTH];
	float	hull;
	int	ship_class;
	float	subsys_current_hits[MAX_RED_ALERT_SUBSYSTEMS];
	float subsys_aggregate_current_hits[SUBSYSTEM_MAX];
	int	wep[MAX_WL_WEAPONS];
	int	wep_count[MAX_WL_WEAPONS];
} red_alert_ship_status;

static red_alert_ship_status Red_alert_wingman_status[MAX_RED_ALERT_SLOTS];
static char	Red_alert_precursor_mission[MAX_FILENAME_LEN];

/////////////////////////////////////////////////////////////////////////////
// Red Alert Interface
/////////////////////////////////////////////////////////////////////////////

const char *Red_alert_fname[GR_NUM_RESOLUTIONS] = {
	"RedAlert",
	"2_RedAlert"
};

const char *Red_alert_mask[GR_NUM_RESOLUTIONS] = {
	"RedAlert-m",
	"2_RedAlert-m"
};

// font to use for "incoming transmission"
int Ra_flash_font[GR_NUM_RESOLUTIONS] = {
	FONT2,
	FONT2
};

int Ra_flash_y[GR_NUM_RESOLUTIONS] = {
	140,
	200
};

#ifdef MAKE_FS1
static int Ra_flash_coords[GR_NUM_RESOLUTIONS][2] = {
	{
		61, 108			// GR_640
	},
	{
		61, 108			// GR_1024
	}
};
#endif

#define NUM_BUTTONS						2

#define RA_REPLAY_MISSION				0
#define RA_CONTINUE						1

static ui_button_info Buttons[GR_NUM_RESOLUTIONS][NUM_BUTTONS] = {
	{	// GR_640
#ifdef MAKE_FS1
		ui_button_info("RAB_05",	0,		435,	-1,	-1,	5),		// Replay
		ui_button_info("RAB_04",	553,	411,	-1,	-1,	4),		// Exit
#else
		ui_button_info("RAB_00",	2,		445,	-1,	-1, 0),
		ui_button_info("RAB_01",	575,	432,	-1,	-1, 1),
#endif
	},	
	{	// GR_1024
		ui_button_info("2_RAB_00",	4,		712,	-1,	-1, 0),
		ui_button_info("2_RAB_01",	920,	691,	-1,	-1, 1),
	}
};

#ifndef MAKE_FS1
#define RED_ALERT_NUM_TEXT		3

UI_XSTR Red_alert_text[GR_NUM_RESOLUTIONS][RED_ALERT_NUM_TEXT] = {
	{ // GR_640
		{ "Replay",		1405,	46,	451,	UI_XSTR_COLOR_PINK,	-1, &Buttons[0][RA_REPLAY_MISSION].button },
		{ "Previous Mission",	1452,	46,	462,	UI_XSTR_COLOR_PINK,	-1, &Buttons[0][RA_REPLAY_MISSION].button },
		{ "Continue",	1069,	564,	413,	UI_XSTR_COLOR_PINK,	-1, &Buttons[0][RA_CONTINUE].button },
	},
	{ // GR_1024
		{ "Replay",		1405,	75,	722,	UI_XSTR_COLOR_PINK,	-1, &Buttons[1][RA_REPLAY_MISSION].button },
		{ "Previous Mission",	1452,	75,	733,	UI_XSTR_COLOR_PINK,	-1, &Buttons[1][RA_REPLAY_MISSION].button },
		{ "Continue",	1069,	902,	661,	UI_XSTR_COLOR_PINK,	-1, &Buttons[1][RA_CONTINUE].button },
	}
};
#endif

// indicies for coordinates
#define RA_X_COORD 0
#define RA_Y_COORD 1
#define RA_W_COORD 2
#define RA_H_COORD 3


static int Text_delay;

int Ra_brief_text_wnd_coords[GR_NUM_RESOLUTIONS][4] = {
	{
#ifdef MAKE_FS1
		117, 169, 406, 283
#else
		14, 151, 522, 289
#endif
	},
	{
		52, 241, 785, 463
	}
};

static UI_WINDOW Ui_window;
#ifdef MAKE_FS1
static hud_anim Flash_anim;
#endif
static int Background_bitmap;
static int Red_alert_inited = 0;

static int Red_alert_voice;

// open and pre-load the stream buffers for the different voice streams
void red_alert_voice_load()
{
	Assert( Briefing != NULL );
	if ( strnicmp(Briefing->stages[0].voice, NOX("none"), 4) && (strlen(Briefing->stages[0].voice) > 0) ) {
		Red_alert_voice = audiostream_open( Briefing->stages[0].voice, ASF_VOICE );
	}
}

// close all the briefing voice streams
void red_alert_voice_unload()
{
	if ( Red_alert_voice != -1 ) {
		audiostream_close_file(Red_alert_voice, 0);
		Red_alert_voice = -1;
	}
}

// start playback of the red alert voice
void red_alert_voice_play()
{
	if ( Red_alert_voice == -1 ){
		return;	// voice file doesn't exist
	}

	if ( !Briefing_voice_enabled ) {
		return;
	}

	if ( audiostream_is_playing(Red_alert_voice) ){
		return;
	}

	audiostream_play(Red_alert_voice, Master_voice_volume, 0);
	Red_alert_voice_started = 1;
}

// stop playback of the red alert voice
void red_alert_voice_stop()
{
	if ( Red_alert_voice == -1 )
		return;

	audiostream_stop(Red_alert_voice);	// stream is automatically rewound
}

// a button was pressed, deal with it
void red_alert_button_pressed(int n)
{
	switch (n) {
	case RA_CONTINUE:		
		if(game_do_cd_mission_check(Game_current_mission_filename)){
			gameseq_post_event(GS_EVENT_ENTER_GAME);
		} else {
			gameseq_post_event(GS_EVENT_MAIN_MENU);
		}
		break;

	case RA_REPLAY_MISSION:
		if ( Game_mode & GM_CAMPAIGN_MODE ) {
			// TODO: make call to campaign code to set correct mission for loading
			// mission_campaign_play_previous_mission(Red_alert_precursor_mission);
			if ( !mission_campaign_previous_mission() ) {
				gamesnd_play_iface(SND_GENERAL_FAIL);
				break;
			}

			// CD CHECK
			if(game_do_cd_mission_check(Game_current_mission_filename)){
				gameseq_post_event(GS_EVENT_START_GAME);
			} else {
				gameseq_post_event(GS_EVENT_MAIN_MENU);
			}
		} else {
			gamesnd_play_iface(SND_GENERAL_FAIL);
		}
		break;
	}
}

// blit "incoming transmission"
#define RA_FLASH_CYCLE			0.25f
float Ra_flash_time = 0.0f;
int Ra_flash_up = 0;
void red_alert_blit_title()
{
	const char *str = XSTR("Incoming Transmission", 1406);
	int w, h;

	// get the string size	
	gr_set_font(Ra_flash_font[gr_screen.res]);
	gr_get_string_size(&w, &h, str);

	// set alpha color
	color flash_color;
	if(Ra_flash_up){
		gr_init_alphacolor(&flash_color, (int)(255.0f * (Ra_flash_time / RA_FLASH_CYCLE)), 0, 0, 255, AC_TYPE_HUD);
	} else {
		gr_init_alphacolor(&flash_color, (int)(255.0f * (1.0f - (Ra_flash_time / RA_FLASH_CYCLE))), 0, 0, 255, AC_TYPE_HUD);
	}

	// draw
#ifndef MAKE_FS1
	gr_set_color_fast(&flash_color);
	gr_string(Ra_brief_text_wnd_coords[gr_screen.res][0] + ((Ra_brief_text_wnd_coords[gr_screen.res][2] - w) / 2), Ra_flash_y[gr_screen.res] - h - 5, str);
	gr_set_color_fast(&Color_normal);	
#endif

	// increment flash time
	Ra_flash_time += flFrametime;
	if(Ra_flash_time >= RA_FLASH_CYCLE){
		Ra_flash_time = 0.0f;
		Ra_flash_up = !Ra_flash_up;
	}

	// back to the original font
	gr_set_font(FONT1);
}

// Called once when red alert interface is started
void red_alert_init()
{
	int i;
	ui_button_info *b;

	if ( Red_alert_inited ) {
		return;
	}

#ifdef MAKE_FS1
	common_set_interface_palette("ControlConfigPalette");  // set the interface palette
#endif
	Ui_window.create(0, 0, gr_screen.max_w, gr_screen.max_h, 0);
	Ui_window.set_mask_bmap(Red_alert_mask[gr_screen.res]);

	for (i=0; i<NUM_BUTTONS; i++) {
		b = &Buttons[gr_screen.res][i];

		b->button.create(&Ui_window, "", b->x, b->y, 60, 30, 0, 1);
		// set up callback for when a mouse first goes over a button
		b->button.set_highlight_action(common_play_highlight_sound);
		b->button.set_bmaps(b->filename);
		b->button.link_hotspot(b->hotspot);
	}

#ifndef MAKE_FS1
	// all text
	for(i=0; i<RED_ALERT_NUM_TEXT; i++){
		Ui_window.add_XSTR(&Red_alert_text[gr_screen.res][i]);
	}
#endif

	// set up red alert hotkeys
	Buttons[gr_screen.res][RA_CONTINUE].button.set_hotkey(KEY_CTRLED | KEY_ENTER);

	// load in background image and flashing red alert animation
	Background_bitmap = bm_load(Red_alert_fname[gr_screen.res]);

#ifdef MAKE_FS1
	hud_anim_init(&Flash_anim, Ra_flash_coords[gr_screen.res][RA_X_COORD], Ra_flash_coords[gr_screen.res][RA_Y_COORD], NOX("AlertFlash"));
	hud_anim_load(&Flash_anim);
#endif

	Red_alert_voice = -1;

	if ( !Briefing ) {
		Briefing = &Briefings[0];			
	}

	if ( Briefing->num_stages > 0 ) {
		Assert(Briefing->stages[0].new_text);
		brief_color_text_init(Briefing->stages[0].new_text, Ra_brief_text_wnd_coords[gr_screen.res][RA_W_COORD], 0);
	}

	red_alert_voice_load();

	Text_delay = timestamp(200);

	Red_alert_voice_started = 0;
	Red_alert_inited = 1;
}

// Called once when the red alert interface is exited
void red_alert_close()
{
	if (Red_alert_inited) {

		red_alert_voice_stop();
		red_alert_voice_unload();

		if (Background_bitmap >= 0) {
			bm_unload(Background_bitmap);
		}
		
		Ui_window.destroy();
#ifdef MAKE_FS1
		hud_anim_release(&Flash_anim);
#endif
		common_free_interface_palette();		// restore game palette
		game_flush();
	}

	Red_alert_inited = 0;
}

// called once per frame when game state is GS_STATE_RED_ALERT
void red_alert_do_frame(float frametime)
{
	int i, k;	

	// ensure that the red alert interface has been initialized
	if (!Red_alert_inited) {
		Int3();
		return;
	}

	k = Ui_window.process() & ~KEY_DEBUGGED;
	switch (k) {
		case KEY_ESC:
//			gameseq_post_event(GS_EVENT_ENTER_GAME);
			gameseq_post_event(GS_EVENT_MAIN_MENU);
			break;
	}	// end switch

	for (i=0; i<NUM_BUTTONS; i++){
		if (Buttons[gr_screen.res][i].button.pressed()){
			red_alert_button_pressed(i);
		}
	}

	GR_MAYBE_CLEAR_RES(Background_bitmap);
	if (Background_bitmap >= 0) {
		gr_set_bitmap(Background_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0, 0);
	} 

	Ui_window.draw();
#ifdef MAKE_FS1
	hud_anim_render(&Flash_anim, frametime);
#endif

	gr_set_font(FONT1);

	if ( timestamp_elapsed(Text_delay) ) {
		int finished_wipe = 0;
		if ( Briefing->num_stages > 0 ) {
			finished_wipe = brief_render_text(0, Ra_brief_text_wnd_coords[gr_screen.res][RA_X_COORD], Ra_brief_text_wnd_coords[gr_screen.res][RA_Y_COORD], Ra_brief_text_wnd_coords[gr_screen.res][RA_H_COORD], frametime, 0);
		}

		if (finished_wipe) {
			red_alert_voice_play();
		}
	}

	// blit incoming transmission
	red_alert_blit_title();

	gr_flip();
}

// set the red alert status for the current mission
void red_alert_set_status(int status)
{
	Red_alert_status = status;
	Red_alert_new_mission_timestamp = timestamp(-1);		// make invalid
}

// Store a ships weapons into a wingman status structure
void red_alert_store_weapons(red_alert_ship_status *ras, ship_weapon *swp)
{
	int i, sidx;

	if (swp == NULL) {
		return;
	}

	for ( i = 0; i < MAX_WL_PRIMARY; i++ ) {
		ras->wep[i] = swp->primary_bank_weapons[i];
		if ( ras->wep[i] >= 0 ) {
			ras->wep_count[i] = 1;
		} else {
			ras->wep_count[i] = -1;
		}
	}

	for ( i = 0; i < MAX_WL_SECONDARY; i++ ) {
		sidx = i+MAX_WL_PRIMARY;
		ras->wep[sidx] = swp->secondary_bank_weapons[i];
		if ( ras->wep[sidx] >= 0 ) {
			ras->wep_count[sidx] = swp->secondary_bank_ammo[i];
		} else {
			ras->wep_count[sidx] = -1;
		}
	}
}

// Take the weapons stored in a wingman_status struct, and bash them into the ship weapons struct
void red_alert_bash_weapons(red_alert_ship_status *ras, ship_weapon *swp)
{
	int i, j, sidx;

	// restore from ship_exited
	if (ras->ship_class == RED_ALERT_EXITED_SHIP_CLASS) {
		return;
	}

	j = 0;
	for ( i = 0; i < MAX_WL_PRIMARY; i++ ) {
		if ( (ras->wep_count[i] > 0) && (ras->wep[i] >= 0) ) {
			swp->primary_bank_weapons[j] = ras->wep[i];			
			j++;
		}
	}
	swp->num_primary_banks = j;

	j = 0;
	for ( i = 0; i < MAX_WL_SECONDARY; i++ ) {
		sidx = i+MAX_WL_PRIMARY;
		if ( ras->wep[sidx] >= 0 ) {
			swp->secondary_bank_weapons[j] = ras->wep[sidx];
			swp->secondary_bank_ammo[j] = ras->wep_count[sidx];
			j++;
		}
	}
	swp->num_secondary_banks = j;
}

void red_alert_bash_subsys_status(red_alert_ship_status *ras, ship *shipp)
{
	ship_subsys *ss;
	int			count = 0;

	// restore from ship_exited
	if (ras->ship_class == RED_ALERT_EXITED_SHIP_CLASS) {
		return;
	}

	ss = GET_FIRST(&shipp->subsys_list);
	while ( ss != END_OF_LIST( &shipp->subsys_list ) ) {

		if ( count >= MAX_RED_ALERT_SUBSYSTEMS ) {
			Int3();	// ran out of subsystems
			break;
		}

		ss->current_hits = ras->subsys_current_hits[count];
		if (ss->current_hits <= 0) {
			ss->submodel_info_1.blown_off = 1;
		}

		ss = GET_NEXT( ss );
		count++;
	}

	int i;

	for ( i = 0; i < SUBSYSTEM_MAX; i++ ) {
		shipp->subsys_info[i].current_hits = ras->subsys_aggregate_current_hits[i];
	}
}


void red_alert_store_subsys_status(red_alert_ship_status *ras, ship *shipp)
{
	ship_subsys *ss;
	int			count = 0;

	if (shipp == NULL) {
		return;
	}

	ss = GET_FIRST(&shipp->subsys_list);
	while ( ss != END_OF_LIST( &shipp->subsys_list ) ) {

		if ( count >= MAX_RED_ALERT_SUBSYSTEMS ) {
			Int3();	// ran out of subsystems
			break;
		}

		ras->subsys_current_hits[count] = ss->current_hits;

		ss = GET_NEXT( ss );
		count++;
	}

	int i;

	for ( i = 0; i < SUBSYSTEM_MAX; i++ ) {
		ras->subsys_aggregate_current_hits[i] = shipp->subsys_info[i].current_hits;
	}
}


// Record the current state of the players wingman
void red_alert_store_wingman_status()
{
	ship				*shipp;
	red_alert_ship_status	*ras;
	ship_obj			*so;
	object			*ship_objp;

	Red_alert_num_slots_used = 0;

	// store the mission filename for the red alert precursor mission
	strcpy(Red_alert_precursor_mission, Game_current_mission_filename);;

	// store status for all existing ships
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		ship_objp = &Objects[so->objnum];
		Assert(ship_objp->type == OBJ_SHIP);
		shipp = &Ships[ship_objp->instance];

		if ( shipp->flags & SF_DYING ) {
			continue;
		}

		if ( Red_alert_num_slots_used >= MAX_RED_ALERT_SLOTS ) {
			Int3();	// ran out of red alert slots
			continue;
		}

		if ( !(shipp->flags & SF_FROM_PLAYER_WING) && !(shipp->flags & SF_RED_ALERT_STORE_STATUS) ) {
			continue;
		}

		ras = &Red_alert_wingman_status[Red_alert_num_slots_used];
		Red_alert_num_slots_used++;

		strcpy(ras->name, shipp->ship_name);
		ras->hull = Objects[shipp->objnum].hull_strength;
		ras->ship_class = shipp->ship_info_index;
		red_alert_store_weapons(ras, &shipp->weapons);
		red_alert_store_subsys_status(ras, shipp);
	}

	// store exited ships that did not die
	for (int idx=0; idx<MAX_EXITED_SHIPS; idx++) {

		if ( Red_alert_num_slots_used >= MAX_RED_ALERT_SLOTS ) {
			Int3();	// ran out of red alert slots
			continue;
		}

		if (Ships_exited[idx].flags & SEF_RED_ALERT_CARRY) {
			ras = &Red_alert_wingman_status[Red_alert_num_slots_used];
			Red_alert_num_slots_used++;

			strcpy(ras->name, Ships_exited[idx].ship_name);
			ras->hull = float(Ships_exited[idx].hull_strength);
			ras->ship_class = RED_ALERT_EXITED_SHIP_CLASS; //shipp->ship_info_index;
			red_alert_store_weapons(ras, NULL);
			red_alert_store_subsys_status(ras, NULL);
		}
	}

	Assert(Red_alert_num_slots_used > 0);
}

// Delete a ship in a red alert mission (since it must have died/departed in the previous mission)
void red_alert_delete_ship(ship *shipp)
{
	if ( (shipp->wing_status_wing_index >= 0) && (shipp->wing_status_wing_pos >= 0) ) {
		hud_set_wingman_status_dead(shipp->wing_status_wing_index, shipp->wing_status_wing_pos);
	}

	ship_add_exited_ship( shipp, SEF_PLAYER_DELETED );
	obj_delete(shipp->objnum);
	if ( shipp->wingnum >= 0 ) {
		ship_wing_cleanup( shipp-Ships, &Wings[shipp->wingnum] );
	}
}

// Take the stored wingman status information, and adjust the player wing ships accordingly
void red_alert_bash_wingman_status()
{
	int				i;
	ship				*shipp;
	red_alert_ship_status	*ras;
	ship_obj			*so;
	object			*ship_objp;

	if ( !(Game_mode & GM_CAMPAIGN_MODE) ) {
		return;
	}

	if ( Red_alert_num_slots_used <= 0 ) {
		return;
	}

	// go through all ships in the game, and see if there is red alert status data for any

	int remove_list[MAX_RED_ALERT_SLOTS];
	int remove_count = 0;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		ship_objp = &Objects[so->objnum];
		Assert(ship_objp->type == OBJ_SHIP);
		shipp = &Ships[ship_objp->instance];

		if ( !(shipp->flags & SF_FROM_PLAYER_WING) && !(shipp->flags & SF_RED_ALERT_STORE_STATUS) ) {
			continue;
		}

		int found_match = 0;

		for ( i = 0; i < Red_alert_num_slots_used; i++ ) {
			ras = &Red_alert_wingman_status[i];

			if ( !stricmp(ras->name, shipp->ship_name) ) {
				found_match = 1;
				if ( ras->ship_class == RED_ALERT_EXITED_SHIP_CLASS) {
					// if exited ship, we can only restore hull strength
					ship_objp->hull_strength = ras->hull;
				} else {
					// if necessary, restore correct ship class
					if ( ras->ship_class != shipp->ship_info_index ) {
						change_ship_type(SHIP_INDEX(shipp), ras->ship_class);
					}
					// restore hull and weapons
					ship_objp->hull_strength = ras->hull;
					red_alert_bash_weapons(ras, &shipp->weapons);
					red_alert_bash_subsys_status(ras, shipp);
				}
			}
		}

		if ( !found_match ) {
			remove_list[remove_count++] = SHIP_INDEX(shipp);
		}
	}

	// remove ships
	for ( i = 0; i < remove_count; i++ ) {
		// remove ship
		red_alert_delete_ship(&Ships[remove_list[i]]);
	}
}

// write wingman status out to the specified file
void red_alert_write_wingman_status(CFILE *fp)
{
	int				i, j;
	red_alert_ship_status *ras;

	cfwrite_int(Red_alert_num_slots_used, fp);

	if ( Red_alert_num_slots_used <= 0 ) {
		return;
	}

	Assert(strlen(Red_alert_precursor_mission) > 0 );
	cfwrite_string(Red_alert_precursor_mission, fp);

	for ( i = 0; i < Red_alert_num_slots_used; i++ ) {
		ras = &Red_alert_wingman_status[i];
		cfwrite_string(ras->name, fp);
		cfwrite_float(ras->hull, fp);
		cfwrite_int(ras->ship_class, fp);

		for ( j = 0; j < MAX_RED_ALERT_SUBSYSTEMS; j++ ) {
			cfwrite_float(ras->subsys_current_hits[j], fp);
		}

		for ( j = 0; j < SUBSYSTEM_MAX; j++ ) {
			cfwrite_float(ras->subsys_aggregate_current_hits[j], fp);
		}

		for ( j = 0; j < MAX_WL_WEAPONS; j++ ) {
			cfwrite_int( ras->wep[j], fp ) ;
			cfwrite_int( ras->wep_count[j], fp );
		}
	}
}

// red wingman status out of the specified file
void red_alert_read_wingman_status(CFILE *fp, int version)
{
	int				i, j;
	red_alert_ship_status *ras;

	Red_alert_num_slots_used = cfread_int(fp);

	if ( Red_alert_num_slots_used <= 0 ) {
		return;
	}

	cfread_string(Red_alert_precursor_mission, MAX_FILENAME_LEN, fp);

	for ( i = 0; i < Red_alert_num_slots_used; i++ ) {
		ras = &Red_alert_wingman_status[i];
		cfread_string(ras->name, NAME_LENGTH, fp);
		ras->hull = cfread_float(fp);
		ras->ship_class = cfread_int(fp);

		for ( j = 0; j < MAX_RED_ALERT_SUBSYSTEMS; j++ ) {
			ras->subsys_current_hits[j] = cfread_float(fp);
		}

		for ( j = 0; j < SUBSYSTEM_MAX; j++ ) {
			ras->subsys_aggregate_current_hits[j] = cfread_float(fp);
		}

		for ( j = 0; j < MAX_WL_WEAPONS; j++ ) {
			ras->wep[j] = cfread_int(fp) ;
			ras->wep_count[j] = cfread_int(fp);
		}
	}
}

// return !0 if this is a red alert mission, otherwise return 0
int red_alert_mission()
{
	if ( Red_alert_status == RED_ALERT_MISSION ) {
		return 1;
	}

	return 0;
}

// called from sexpression code to start a red alert mission
void red_alert_start_mission()
{
	// if we are not in campaign mode, go to debriefing
//	if ( !(Game_mode & GM_CAMPAIGN_MODE) ) {
//		gameseq_post_event( GS_EVENT_DEBRIEF );	// proceed to debriefing
//		return;
//	}

	// check player health here.  
	// if we're dead (or about to die), don't start red alert mission.
	if (Player_obj->type == OBJ_SHIP) {
		if (Player_obj->hull_strength > 0) {
			// make sure we don't die
			Player_obj->flags |= OF_GUARDIAN;

			// do normal red alert stuff
			Red_alert_new_mission_timestamp = timestamp(RED_ALERT_WARN_TIME);

			// throw down a sound here to make the warning seem ultra-important
			// gamesnd_play_iface(SND_USER_SELECT);
			snd_play(&(Snds[SND_DIRECTIVE_COMPLETE]));
		}
	}
}

// called from main game loop to check to see if we should move to a red alert mission
int red_alert_check_status()
{
	// if the timestamp is invalid, do nothing.
	if ( !timestamp_valid(Red_alert_new_mission_timestamp) )
		return 0;

	// return if the timestamp hasn't elapsed yet
	if ( timestamp_elapsed(Red_alert_new_mission_timestamp) ) {

		// basic premise here is to stop the current mission, and then set the next mission in the campaign
		// which better be a red alert mission
		if ( Game_mode & GM_CAMPAIGN_MODE ) {
			red_alert_store_wingman_status();
			mission_goal_fail_incomplete();
			mission_campaign_store_goals_and_events();
			scoring_level_close();
			mission_campaign_eval_next_mission();
			mission_campaign_mission_over();

			// CD CHECK
			gameseq_post_event(GS_EVENT_START_GAME);
		} else {
			gameseq_post_event(GS_EVENT_END_GAME);
		}
	}

	return 1;
}

