/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Hud/HUDreticle.cpp $
 * $Revision: 193 $
 * $Date: 2003-05-25 04:30:44 +0200 (Sun, 25 May 2003) $
 * $Author: taylor $
 *
 * C module to draw and manage the recticle
 *
 * $Log$
 * Revision 1.6  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.5  2003/05/22 15:58:44  taylor
 * fix missed German build option for auto-lang
 *
 * Revision 1.4  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.3  2002/06/09 04:41:21  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:45  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 14    10/27/99 10:04p Jefff
 * changed german match speed indicator
 * 
 * 13    10/14/99 2:50p Jefff
 * localization fixes
 * 
 * 12    8/28/99 4:54p Dave
 * Fixed directives display for multiplayer clients for wings with
 * multiple waves. Fixed hud threat indicator rendering color.
 * 
 * 11    8/09/99 3:14p Dave
 * Make "launch" warning gauge draw in code.
 * 
 * 10    6/10/99 3:43p Dave
 * Do a better job of syncing text colors to HUD gauges.
 * 
 * 9     6/08/99 10:48a Jasenw
 * new coords for new HUD stuff
 * 
 * 8     6/03/99 2:31p Jasenw
 * changed coords and removed weapon indicators.
 * 
 * 7     1/07/99 2:21p Jasen
 * fixed toparc
 * 
 * 6     1/07/99 9:07a Jasen
 * HUD coords
 * 
 * 5     12/28/98 3:17p Dave
 * Support for multiple hud bitmap filenames for hi-res mode.
 * 
 * 4     12/21/98 5:02p Dave
 * Modified all hud elements to be multi-resolution friendly.
 * 
 * 3     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 69    8/28/98 3:28p Dave
 * EMP effect done. AI effects may need some tweaking as required.
 * 
 * 68    8/25/98 1:48p Dave
 * First rev of EMP effect. Player side stuff basically done. Next comes
 * AI code.
 * 
 * 67    6/12/98 4:52p Hoffoss
 * Added support for special characters in in forgeign languages.
 * 
 * 66    6/09/98 5:17p Lawrance
 * French/German localization
 * 
 * 65    6/09/98 10:31a Hoffoss
 * Created index numbers for all xstr() references.  Any new xstr() stuff
 * added from here on out should be added to the end if the list.  The
 * current list count can be found in FreeSpace.cpp (search for
 * XSTR_SIZE).
 * 
 * 64    5/21/98 7:18p Lawrance
 * Don't flash 'launch' for attempted lock
 * 
 * 63    5/13/98 1:53p Lawrance
 * If no secondary banks on a ship, show correct reticle gauge
 * 
 * 62    5/06/98 8:05p Dave
 * Made standalone reset properly under weird conditions. Tweak
 * optionsmulti screen. Upped MAX_WEAPONS to 350. Put in new launch
 * countdown anim. Minro ui fixes/tweaks.
 * 
 * 61    5/05/98 5:51p Lawrance
 * Draw HUD center crosshair with bright alpha index
 * 
 * 60    5/04/98 6:12p Lawrance
 * fix progblems with weapon gauges in situation when weapons are on ship
 * 
 * 59    5/01/98 4:22p Lawrance
 * Use ship->weapons instead of sip when displaying weapon linking gauge
 * 
 * 58    4/13/98 11:19a Lawrance
 * fix launch threat indicator
 * 
 * 57    4/08/98 10:34p Allender
 * make threat indicators work in multiplayer.  Fix socket problem (once
 * and for all???)
 * 
 * 56    3/26/98 5:26p John
 * added new paging code. nonfunctional.
 * 
 * 55    2/23/98 6:49p Lawrance
 * Use gr_aabitmap_ex() instead of clipping regions
 * 
 * 54    2/22/98 4:17p John
 * More string externalization classification... 190 left to go!
 * 
 * 53    2/22/98 12:19p John
 * Externalized some strings
 * 
 * 52    2/12/98 4:58p Lawrance
 * Change to new flashing method.
 * 
 * 51    2/09/98 9:07p Lawrance
 * Ensure 'evaded' popup has precedence over 'launch' popup
 * 
 * 50    1/25/98 10:31p Lawrance
 * Don't draw most hud gauges when viewing from another ship.
 * 
 * 49    1/19/98 11:37p Lawrance
 * Fixing Optimization build warnings
 * 
 * 48    1/14/98 10:21a Lawrance
 * Draw reticle correctly when gauges are disabled.
 * 
 * 47    1/08/98 3:20p Johnson
 * ALAN: Ensure entire throttle gauge flashes
 * 
 * 46    1/05/98 9:38p Lawrance
 * Implement flashing HUD gauges.
 * 
 * 45    1/02/98 9:10p Lawrance
 * Big changes to how colors get set on the HUD.
 * 
 * 44    12/16/97 9:13p Lawrance
 * Integrate new gauges into HUD config.
 * 
 * 43    12/10/97 10:02p Lawrance
 * Change weapon linking to use flags.
 * 
 * 42    12/08/97 1:49p Lawrance
 * only play threat lock sound when a lock is being attempted
 * 
 * 41    11/17/97 6:37p Lawrance
 * new gauges: extended target view, new lock triangles, support ship view
 * 
 * 40    11/11/97 10:25p Lawrance
 * add sound hook for when missile threat flashes
 * 
 * 39    11/11/97 5:05p Lawrance
 * get threat indicator working more reliably
 * 
 * 38    11/05/97 11:20p Lawrance
 * add speed numbers to the reticle
 * 
 * 37    11/04/97 7:49p Lawrance
 * integrating new HUD reticle and shield icons
 * 
 * 36    11/03/97 5:38p Dave
 * Cleaned up more multiplayer sequencing. Added OBJ_OBSERVER module/type.
 * Restructured HUD_config structs/flags.
 * 
 * $NoKeywords: $
 *
*/

#include "2d.h"

#include "hudreticle.h"
#include "hud.h"
#include "pixel.h"
#include "math.h"
#include "player.h"
#include "ship.h"
#include "freespace.h"
#include "ai.h"
#include "bmpman.h"
#include "key.h"
#include "timer.h"
#include "math.h"
#include "gamesnd.h"
#include "hudtargetbox.h"
#include "multi.h"
#include "emp.h"
#include "localize.h"

static int Reticle_inited = 0;

#ifdef MAKE_FS1
	#define NUM_RETICLE_ANIS		11
#else
	#define NUM_RETICLE_ANIS			6		// keep up to date when modifying the number of reticle ani files
#endif

#define RETICLE_TOP_ARC				0
#define RETICLE_LASER_WARN			1
#define RETICLE_LOCK_WARN			2
#define RETICLE_LEFT_ARC			3
#define RETICLE_RIGHT_ARC			4
#ifdef MAKE_FS1
#define RETICLE_ONE_PRIMARY		5
#define RETICLE_TWO_PRIMARY		6
#define RETICLE_ONE_SECONDARY	7
#define RETICLE_TWO_SECONDARY	8
#define RETICLE_THREE_SECONDARY	9
#define RETICLE_CENTER			10
// #define RETICLE_LAUNCH_LABEL	11
#else
#define RETICLE_CENTER			5
#endif

int Hud_throttle_frame_h[GR_NUM_RESOLUTIONS] = {
	85,
	136
};
int Hud_throttle_frame_w[GR_NUM_RESOLUTIONS] = {
	49, 
	78
};
int Hud_throttle_frame_bottom_y[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	330,
#else
	325,
#endif
	520
};
int Hud_throttle_h[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	49,
#else
	50,
#endif
	80
};
int Hud_throttle_bottom_y[GR_NUM_RESOLUTIONS] = {
#ifdef MAKE_FS1
	312,
#else
	307,
#endif
	491
};
int Hud_throttle_aburn_h[GR_NUM_RESOLUTIONS] = {
	17,
	27
};
int Hud_throttle_aburn_button[GR_NUM_RESOLUTIONS] = {
	259,
	414
};

int Outer_circle_radius[GR_NUM_RESOLUTIONS] = {
	104,
	166
};

int Hud_reticle_center[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
		320, 242
	},
	{ // GR_1024
		512, 385
	}
};

char Reticle_frame_names[GR_NUM_RESOLUTIONS][NUM_RETICLE_ANIS][MAX_FILENAME_LEN] = 
{
//XSTR:OFF
	{ // GR_640
#ifdef MAKE_FS1
		"toparc1",
		"toparc2",
		"toparc3",
		"leftarc",
		"rightarc1",
		"rightarc2",
		"rightarc3",
		"rightarc4",
		"rightarc5",
		"rightarc6",
//		"toparc4",
		"reticle1",
#else
		"toparc1",
		"toparc2",
		"toparc3",
		"leftarc",
		"rightarc1",
		"reticle1",
#endif
	}, 
	{ // GR_1024
		"2_toparc1",
		"2_toparc2",
		"2_toparc3",
		"2_leftarc",
		"2_rightarc1",
#ifdef MAKE_FS1
		"2_rightarc2",
		"2_rightarc3",
		"2_rightarc4",
		"2_rightarc5",
		"2_rightarc6",	
		// "2_toparc4",
#endif
		"2_reticle1",	
	}
//XSTR:ON
};

// reticle frame coords
int Reticle_frame_coords[GR_NUM_RESOLUTIONS][NUM_RETICLE_ANIS][2] = {
	{ // GR_640
#ifdef MAKE_FS1
		{241, 139},		// toparc1
		{300, 139},		// toparc2
		{320, 139},		// toparc3
		{220, 246},		// leftarc
		{374, 244},		// rightarc1
		{406, 255},		// rightarc2
		{406, 255},		// rightarc3
		{391, 278},		// rightarc4
		{391, 278},		// rightarc5
		{391, 278},		// rightarc6
		// {297, 162},	// toparc4
		{308, 233}		// reticle1
#else
		{241, 137},		// toparc1
		{400, 245},		// toparc2
		{394, 261},		// toparc3
		{216, 168},		// leftarc
		{359, 168},		// rightarc1
		{308, 235}		// reticle1
#endif
	}, 
	{ // GR_1024
		{386, 219},
		{640, 393},
		{631, 419},
		{346, 269},
		{574, 269},
#ifdef MAKE_FS1
		{649, 401},
		{649, 401},
		{625, 438},
		{625, 438},
		{625, 438},
//		{475, 258},
#endif
		{493, 370}
	}
};

// "launch" gauge coords
int Reticle_launch_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
		297,	161
	},
	{ // GR_1024
		475,	258
	}
};

hud_frames Reticle_gauges[NUM_RETICLE_ANIS];

#define THREAT_DUMBFIRE				(1<<0)
#define THREAT_ATTEMPT_LOCK		(1<<1)
#define THREAT_LOCK					(1<<2)

#define THREAT_UPDATE_DUMBFIRE_TIME		1000		// time between checking for dumbfire threats
#define THREAT_UPDATE_LOCK_TIME			500		// time between checking for lock threats

#define THREAT_DUMBFIRE_FLASH				180
#define THREAT_LOCK_FLASH					180
static int Threat_dumbfire_timer;		// timestamp for when to show next flashing frame for dumbfire threat
static int Threat_lock_timer;				// timestamp for when to show next flashing frame for lock threat

static int Threat_dumbfire_frame;			// frame offset of current dumbfire flashing warning
static int Threat_lock_frame;				// frame offset of current lock flashing warning

// coordinates
static int Max_speed_coords[GR_NUM_RESOLUTIONS][2] = 
{
	{ // GR_640
		236, 254
	}, 
	{ // GR_1024
		377, 406
	}
};
static int Zero_speed_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
		252, 303
	},
	{ // GR_1024
		403, 485
	}
};

// Called at the start of each level.. use Reticle_inited so we only load frames once
void hud_init_reticle()
{
	int			i;
	hud_frames	*hfp;

	Threat_dumbfire_timer = timestamp(0);
	Threat_lock_timer = timestamp(0);
	Threat_dumbfire_frame = 1;
	Threat_lock_frame = 1;
	Player->threat_flags = 0;
	Player->update_dumbfire_time = timestamp(0);		
	Player->update_lock_time = timestamp(0);

	if ( Reticle_inited ) {
		return;
	}

	for ( i = 0; i < NUM_RETICLE_ANIS; i++ ) {
		hfp = &Reticle_gauges[i];
		hfp->first_frame = bm_load_animation(Reticle_frame_names[gr_screen.res][i], &hfp->num_frames);
		if ( hfp->first_frame < 0 ) {
			Warning(LOCATION,"Cannot load hud ani: %s\n", Reticle_frame_names[gr_screen.res][i]);
		}
	}

	Reticle_inited = 1;
}

// called once per frame to update the reticle gauges.  Makes calls to
// ship_dumbfire_threat() and ship_lock_threat() and updates Threat_flags.
void hud_update_reticle( player *pp )
{
	int rval;
	ship *shipp;

	// multiplayer clients won't call this routine
	if ( MULTIPLAYER_CLIENT || MULTI_OBSERVER(Net_players[MY_NET_PLAYER_NUM]))
		return;

	shipp = &Ships[Objects[pp->objnum].instance];

	if ( ship_dumbfire_threat(shipp) ) {
		pp->threat_flags |= THREAT_DUMBFIRE;
		pp->update_dumbfire_time = timestamp(THREAT_UPDATE_DUMBFIRE_TIME);
	}

	if ( timestamp_elapsed(pp->update_dumbfire_time) ) {
		pp->update_dumbfire_time = timestamp(THREAT_UPDATE_DUMBFIRE_TIME);
		pp->threat_flags &= ~THREAT_DUMBFIRE;
	}

	if ( timestamp_elapsed(pp->update_lock_time) ) {
		pp->threat_flags &= ~(THREAT_LOCK | THREAT_ATTEMPT_LOCK);
		pp->update_lock_time = timestamp(THREAT_UPDATE_LOCK_TIME);
		rval = ship_lock_threat(shipp);
		if ( rval == 1 ) {
			pp->threat_flags |= THREAT_ATTEMPT_LOCK;
		} else if ( rval == 2 ) {
			pp->threat_flags |= THREAT_LOCK;
		}
	}
}

// draw left arc (the dark portion of the throttle gauge)
void hud_render_throttle_background(int y_end)
{
	int x,y,w,h;

	hud_set_gauge_color(HUD_THROTTLE_GAUGE);

	x = Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][0];
	y = Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][1];

	bm_get_info( Reticle_gauges[RETICLE_LEFT_ARC].first_frame+1,&w,&h);	

	if ( y_end > y ) {
		GR_AABITMAP_EX(Reticle_gauges[RETICLE_LEFT_ARC].first_frame+1, x, y, w, y_end-y+1, 0, 0);		
	}
}

// draw left arc (the bright portion of the throttle gauge)
void hud_render_throttle_foreground(int y_end)
{
	int x,y,w,h;

	hud_set_gauge_color(HUD_THROTTLE_GAUGE);

	x = Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][0];
	y = Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][1];

	bm_get_info( Reticle_gauges[RETICLE_LEFT_ARC].first_frame+1,&w,&h);

	if ( y_end < Hud_throttle_frame_bottom_y[gr_screen.res] ) {		
		GR_AABITMAP_EX(Reticle_gauges[RETICLE_LEFT_ARC].first_frame+2, x, y_end, w, h-(y_end-y), 0, y_end-y);		
	}
}

// Draw the throttle speed number
void hud_render_throttle_speed(float current_speed, int y_end)
{
	char buf[32];
	int sx, sy, x_pos, y_pos, w, h;

	hud_set_gauge_color(HUD_THROTTLE_GAUGE);

	// y_end is the y-coordinate of the current throttle setting, calc x-coordinate for edge of 
	// circle (x^2 + y^2 = r^2)
	y_pos = Hud_reticle_center[gr_screen.res][1] - y_end;
	x_pos = (int)sqrt(double(Outer_circle_radius[gr_screen.res] * Outer_circle_radius[gr_screen.res] - y_pos * y_pos) );
	x_pos = Hud_reticle_center[gr_screen.res][0] - x_pos;

	// draw current speed at (x_pos, y_end);
	sprintf(buf, "%d", fl2i(current_speed+0.5f));
	hud_num_make_mono(buf);
	gr_get_string_size(&w, &h, buf);
	sx = x_pos - w - 2;
	sy = fl2i(y_end - h/2.0f + 1.5);
	gr_printf(sx, sy, buf);

	if ( Players[Player_num].flags & PLAYER_FLAGS_MATCH_TARGET ) {
		int offset;
		if ( current_speed <= 9.5 ) {
			offset = 0;
		} else {
			offset = 3;
		}
		if (Lcl_gr) {
			// print an m, cuz the voice says its an m.  
			// its a normal m cuz the german font has no special m (its an a)
			gr_string(sx+offset, sy + h, "m");
		} else {
			gr_printf(sx+offset, sy + h, "%c", Lcl_special_chars + 3);
		}
	}
}

// draw the "desired speed" bar on the throttle
void hud_render_throttle_line(int y)
{
	// hud_set_bright_color();
	hud_set_gauge_color(HUD_THROTTLE_GAUGE, HUD_C_BRIGHT);

	GR_AABITMAP_EX(Reticle_gauges[RETICLE_LEFT_ARC].first_frame+3, Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][0], y, Hud_throttle_frame_w[gr_screen.res], 1, 0, y-Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][1]);	
}

// Draw the throttle gauge along the left arc of the reticle
void hud_show_throttle()
{
	float	desired_speed, max_speed, current_speed, percent_max, percent_aburn_max;
	int	desired_y_pos, y_end;

	ship_info	*sip;
	sip = &Ship_info[Player_ship->ship_info_index];

	current_speed = Player_obj->phys_info.fspeed;
	if ( current_speed < 0.0f){
		current_speed = 0.0f;
	}

	max_speed = Ships[Player_obj->instance].current_max_speed;
	if ( max_speed <= 0 ) {
		max_speed = sip->max_vel.xyz.z;
	}

	desired_speed = Player->ci.forward * max_speed;
	if ( desired_speed < 0.0f ){		// so ships that go backwards don't force the indicators below where they can go
		desired_speed = 0.0f;
	}

	desired_y_pos = Hud_throttle_bottom_y[gr_screen.res] - fl2i(Hud_throttle_h[gr_screen.res]*desired_speed/max_speed+0.5f) - 1;

	Assert(max_speed != 0);
	percent_max = current_speed / max_speed;

	percent_aburn_max = 0.0f;
	if ( percent_max > 1 ) {
		percent_max = 1.0f;
		percent_aburn_max = (current_speed - max_speed) / (sip->afterburner_max_vel.xyz.z - max_speed);
		if ( percent_aburn_max > 1.0f ) {
			percent_aburn_max = 1.0f;
		}
		if ( percent_aburn_max < 0 ) {
			percent_aburn_max = 0.0f;
		}
	}

	y_end = Hud_throttle_bottom_y[gr_screen.res] - fl2i(Hud_throttle_h[gr_screen.res]*percent_max+0.5f);
	if ( percent_aburn_max > 0 ) {
		y_end -= fl2i(percent_aburn_max * Hud_throttle_aburn_h[gr_screen.res] + 0.5f);
	}

	if ( Player_obj->phys_info.flags & PF_AFTERBURNER_ON ) {
		desired_y_pos = 240;
	}

	// draw left arc (the dark portion of the throttle gauge)
#ifdef MAKE_FS1
	hud_render_throttle_background(y_end);
#endif

	// draw throttle speed number
	hud_render_throttle_speed(current_speed, y_end);

	// draw the "desired speed" bar on the throttle
	hud_render_throttle_line(desired_y_pos);

	// draw left arc (the bright portion of the throttle gauge)
	hud_render_throttle_foreground(y_end);

	gr_printf(Max_speed_coords[gr_screen.res][0], Max_speed_coords[gr_screen.res][1], "%d",fl2i(max_speed));
	gr_printf(Zero_speed_coords[gr_screen.res][0], Zero_speed_coords[gr_screen.res][1], XSTR( "0", 292));
}

#ifdef MAKE_FS1
// Draw the primary and secondary weapon indicators along the right arc of the reticle
void hud_show_reticle_weapons()
{
	int			gauge_index=0, frame_offset=0;
	ship_weapon	*swp;

	swp = &Player_ship->weapons;

	switch( swp->num_primary_banks ) {
		case 0:
			gauge_index = -1;
			break;

		case 1:
			gauge_index = RETICLE_ONE_PRIMARY;
			if ( Player_ship->weapons.current_primary_bank == -1 ) {
				frame_offset = 0;	
			} else {
				frame_offset = 1;	
			}
			break;

		case 2:
			gauge_index = RETICLE_TWO_PRIMARY;
			if ( swp->current_primary_bank == -1 ) {
				frame_offset = 0;	
			} else {
				if ( Player_ship->flags & SF_PRIMARY_LINKED ) {
					frame_offset = 3;
				} else {
					if ( swp->current_primary_bank == 0 ) {
						frame_offset = 1;
					} else {
						frame_offset = 2;
					}
				}
			}
			break;

		default:
			Int3();	// shouldn't happen (get Alan if it does)
			return;
			break;
	}
	
	if ( gauge_index != -1 ) {
		GR_AABITMAP(Reticle_gauges[gauge_index].first_frame+frame_offset, Reticle_frame_coords[gr_screen.res][gauge_index][0], Reticle_frame_coords[gr_screen.res][gauge_index][1]);		
	}

	int num_banks = swp->num_secondary_banks;
	if ( num_banks <= 0 ) {
		num_banks = Ship_info[Player_ship->ship_info_index].num_secondary_banks;
	}

	switch( num_banks ) {
		case 0:
			Int3();
			gauge_index = -1;
			break;

		case 1:
			gauge_index = RETICLE_ONE_SECONDARY;
			break;

		case 2:
			gauge_index = RETICLE_TWO_SECONDARY;
			break;

		case 3:
			gauge_index = RETICLE_THREE_SECONDARY;
			break;

		default:
			Int3();	// shouldn't happen (get Alan if it does)
			return;
			break;
	}
	
	if ( gauge_index != -1 ) {
		if ( swp->num_secondary_banks <= 0 ) {
			frame_offset = 0;
		} else {
			frame_offset = swp->current_secondary_bank+1;
		}

		GR_AABITMAP(Reticle_gauges[gauge_index].first_frame+frame_offset, Reticle_frame_coords[gr_screen.res][gauge_index][0], Reticle_frame_coords[gr_screen.res][gauge_index][1]);		
	}
}
#endif

// Draw the lock threat gauge on the HUD.  Use Threat_flags to determine if a 
// threat exists, and draw flashing frames.
void hud_show_lock_threat()
{
	int frame_offset;

	if ( Player->threat_flags & (THREAT_LOCK | THREAT_ATTEMPT_LOCK) ) {
		if ( timestamp_elapsed(Threat_lock_timer) ) {
			if ( Player->threat_flags & THREAT_LOCK )  {
				Threat_lock_timer = timestamp(fl2i(THREAT_LOCK_FLASH/2.0f));
			} else {
				Threat_lock_timer = timestamp(THREAT_LOCK_FLASH);
			}
			Threat_lock_frame++;
			if ( Threat_lock_frame > 2 ) {
				Threat_lock_frame = 1;
			}
			if ( (Threat_lock_frame == 2) && (Player->threat_flags & THREAT_ATTEMPT_LOCK ) ) {
				snd_play( &Snds[SND_THREAT_FLASH]);
			}
		}
		frame_offset = Threat_lock_frame;
	} else {
		frame_offset = 0;
	}

	hud_set_gauge_color(HUD_THREAT_GAUGE);

	GR_AABITMAP(Reticle_gauges[RETICLE_LOCK_WARN].first_frame+frame_offset, Reticle_frame_coords[gr_screen.res][RETICLE_LOCK_WARN][0], Reticle_frame_coords[gr_screen.res][RETICLE_LOCK_WARN][1]);

	// "launch" flash
	if ( (frame_offset > 0) && (Player->threat_flags & THREAT_LOCK) ) {
		if ( hud_targetbox_flash_expired(TBOX_FLASH_CMEASURE) ) {
			// hack
			int bright;
			if(frame_offset % 2){
				bright = 1;
			} else {
				bright = 0;
			}
			// GR_AABITMAP(Reticle_gauges[RETICLE_LAUNCH_LABEL].first_frame+frame_offset%2, Reticle_frame_coords[gr_screen.res][RETICLE_LAUNCH_LABEL][0], Reticle_frame_coords[gr_screen.res][RETICLE_LAUNCH_LABEL][1]);

			// use hud text flash gauge code
			hud_show_text_flash_icon(XSTR("Launch", 1507), Reticle_launch_coords[gr_screen.res][1], bright);
		}
	}
}

// Draw the dumbfire threat gauge on the HUD.  Use Threat_flags to determine if a
// threat exists, and draw flashing frames.
void hud_show_dumbfire_threat()
{
	int frame_offset;

	if ( Player->threat_flags & THREAT_DUMBFIRE ) {
		if ( timestamp_elapsed(Threat_dumbfire_timer) ) {
			Threat_dumbfire_timer = timestamp(THREAT_DUMBFIRE_FLASH);
			Threat_dumbfire_frame++;
			if ( Threat_dumbfire_frame > 2 ) {
				Threat_dumbfire_frame = 1;
			}
		}
		frame_offset = Threat_dumbfire_frame;
	} else {
		frame_offset = 0;
	}

	hud_set_gauge_color(HUD_THREAT_GAUGE);

	GR_AABITMAP(Reticle_gauges[RETICLE_LASER_WARN].first_frame + frame_offset, Reticle_frame_coords[gr_screen.res][RETICLE_LASER_WARN][0], Reticle_frame_coords[gr_screen.res][RETICLE_LASER_WARN][1]);	
}

// Draw the center of the reticle
void hud_show_center_reticle()
{
	Assert(Reticle_gauges[RETICLE_CENTER].first_frame != -1 );

//	hud_set_default_color();
	// hud_set_bright_color();
	hud_set_gauge_color(HUD_CENTER_RETICLE, HUD_C_BRIGHT);

	GR_AABITMAP(Reticle_gauges[RETICLE_CENTER].first_frame, Reticle_frame_coords[gr_screen.res][RETICLE_CENTER][0], Reticle_frame_coords[gr_screen.res][RETICLE_CENTER][1]);
}

// Draw top portion of reticle
void hud_show_top_arc()
{
	hud_set_gauge_color(HUD_CENTER_RETICLE);

	// hud_set_default_color();
	if ( hud_gauge_active(HUD_THREAT_GAUGE) ) {	
		// draw top arc
		GR_AABITMAP(Reticle_gauges[RETICLE_TOP_ARC].first_frame+1, Reticle_frame_coords[gr_screen.res][RETICLE_TOP_ARC][0], Reticle_frame_coords[gr_screen.res][RETICLE_TOP_ARC][1]);		

		// draw dumbfire threat
		hud_show_dumbfire_threat();

		// draw lock threat
		hud_show_lock_threat();
	} else {		
		// draw top arc without any holes
		GR_AABITMAP(Reticle_gauges[RETICLE_TOP_ARC].first_frame, Reticle_frame_coords[gr_screen.res][RETICLE_TOP_ARC][0], Reticle_frame_coords[gr_screen.res][RETICLE_TOP_ARC][1]);
	}
}

// Draw right portion of reticle
void hud_show_right_arc()
{	
	hud_set_gauge_color(HUD_CENTER_RETICLE);

#ifndef MAKE_FS1
	GR_AABITMAP(Reticle_gauges[RETICLE_RIGHT_ARC].first_frame+1, Reticle_frame_coords[gr_screen.res][RETICLE_RIGHT_ARC][0], Reticle_frame_coords[gr_screen.res][RETICLE_RIGHT_ARC][1]);		
#else
		
	// draw the weapons indicators in the holes along the right arc
	if ( hud_gauge_active(HUD_WEAPON_LINKING_GAUGE) ) {		
		// draw right arc with holes in it
		GR_AABITMAP(Reticle_gauges[RETICLE_RIGHT_ARC].first_frame+1, Reticle_frame_coords[gr_screen.res][RETICLE_RIGHT_ARC][0], Reticle_frame_coords[gr_screen.res][RETICLE_RIGHT_ARC][1]);		
//		the following line was removed by Jasen to get rid of "undeclared identifier"
//		hehe - DB
		hud_show_reticle_weapons();
	} else {		
		// draw right arc without any holes
		GR_AABITMAP(Reticle_gauges[RETICLE_RIGHT_ARC].first_frame, Reticle_frame_coords[gr_screen.res][RETICLE_RIGHT_ARC][0], Reticle_frame_coords[gr_screen.res][RETICLE_RIGHT_ARC][1]);
	}
#endif
}

// Draw the left portion of the reticle
void hud_show_left_arc()
{			
#ifndef MAKE_FS1  // FS1: is drawn another way
	// draw left arc (the dark portion of the throttle gauge)
	hud_set_gauge_color(HUD_CENTER_RETICLE);	
	GR_AABITMAP(Reticle_gauges[RETICLE_LEFT_ARC].first_frame, Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][0], Reticle_frame_coords[gr_screen.res][RETICLE_LEFT_ARC][1]);			
#endif
	
	// draw the throttle
	if ( hud_gauge_active(HUD_THROTTLE_GAUGE) ) {
		hud_set_gauge_color(HUD_THROTTLE_GAUGE);
		hud_show_throttle();		
	} 
}

// called once per frame from HUD_render_2d() to draw the reticle gauges
void hud_show_reticle()
{
#ifdef __vita__
	if ( !(Viewer_mode & (VM_EXTERNAL | VM_OTHER_SHIP | VM_CHASE | VM_WARP_CHASE | VM_PADLOCK_ANY )) ) {
#else
	if ( !(Viewer_mode & VM_OTHER_SHIP) ) {
#endif
		hud_show_top_arc();
		hud_show_right_arc();
		hud_show_left_arc();
	}

	// draw the center of the reticle
	if ( hud_gauge_active(HUD_CENTER_RETICLE) ) {
		hud_show_center_reticle();
	}
}

void hudreticle_page_in()
{
	hud_frames	*hfp;

	int i;
	for ( i = 0; i < NUM_RETICLE_ANIS; i++ ) {
		hfp = &Reticle_gauges[i];
		bm_page_in_aabitmap( hfp->first_frame, hfp->num_frames);
	}

}
