/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Hud/HUDescort.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module for managing and displaying ships that are in an escort
 *
 * $Log$
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
 * 22    8/04/99 2:24a Dave
 * Fixed escort gauge ordering for dogfight.
 * 
 * 21    7/30/99 7:01p Dave
 * Dogfight escort gauge. Fixed up laser rendering in Glide.
 * 
 * 20    7/29/99 1:57p Jefff
 * Fixed location of "D" for disabled ships in hi-res
 * 
 * 19    7/22/99 7:49p Dave
 * Show enemy ships on the escort list in red.
 * 
 * 18    6/23/99 10:13p Andsager
 * Fix Baranec hud escort default color bug!
 * 
 * 17    6/10/99 3:43p Dave
 * Do a better job of syncing text colors to HUD gauges.
 * 
 * 16    5/24/99 11:28a Dave
 * Sexpression for adding/removing ships from the hud escort list.
 * 
 * 15    3/30/99 5:40p Dave
 * Fixed reinforcements for TvT in multiplayer.
 * 
 * 14    3/19/99 9:51a Dave
 * Checkin to repair massive source safe crash. Also added support for
 * pof-style nebulae, and some new weapons code.
 * 
 * 14    3/12/99 4:24p Andsager
 * Fix bug where two or more ships are culled from escort list same frame
 * 
 * 13    3/04/99 9:22a Andsager
 * Make escort list work with ship-is-visible.  When not visible, dump,
 * when becoming visible, maybe add.
 * 
 * 12    3/02/99 12:27p Andsager
 * Fixed bug in hud_add_ship_to_escort -- multiple entries and
 * uninitialized entries.
 * 
 * 11    3/02/99 11:45a Anoop
 * put in assert for escort list
 * 
 * 10    3/01/99 12:00p Andsager
 * Fix escort priority to look at newly added ships.
 * 
 * 9     2/03/99 6:13p Andsager
 * Added priorities to escort list.  Folded escort hit info into
 * escort_info struct.
 * 
 * 8     2/01/99 9:47a Jasen
 * Tweaked coords for the monitoring window in hi res. 
 * 
 * 7     1/30/99 7:48p Jasen
 * 
 * 6     1/07/99 9:07a Jasen
 * HUD coords
 * 
 * 5     12/28/98 3:17p Dave
 * Support for multiple hud bitmap filenames for hi-res mode.
 * 
 *
 * $NoKeywords: $
 */


#include "object.h"
#include "ship.h"
#include "linklist.h"
#include "2d.h"
#include "hud.h"
#include "hudtarget.h"
#include "hudtargetbox.h"
#include "gamesnd.h"
#include "freespace.h"
#include "bmpman.h"
#include "font.h"
#include "hudshield.h"
#include "timer.h"
#include "hudescort.h"
#include "emp.h"
#include "alphacolors.h"
#include "multi.h"
#include "multiutil.h"

int Show_escort_view;

typedef struct escort_info
{
	int					objnum;
	int					obj_signature;	// so we are sure we have a valid objnum
	int					priority;		// higher priority is higher in the list
	short					np_id;			// netplayer id (for multiplayer dogfight mode)
	shield_hit_info	hit_info;
} escort_info;

escort_info		Escort_ships[MAX_ESCORT_SHIPS];
int				Num_escort_ships;


// size of complete escort list, including all those wanting to get onto list but without space
#define MAX_COMPLETE_ESCORT_LIST	20


#define NUM_ESCORT_FRAMES 3
hud_frames Escort_gauges[NUM_ESCORT_FRAMES];
int Escort_gauges_loaded = 0;

int Escort_gauge_y[MAX_ESCORT_SHIPS] = { 219, 230, 241 };


int Escort_gauge_text_coords[GR_NUM_RESOLUTIONS][MAX_ESCORT_SHIPS][4][2] =
{
	{ // GR_640
		{
			{489,219}, 
			{599,212}, 
			{604,219}, 
			{474,219}
		},
		{
			{489,230}, 
			{599,223},
			{604,230},
			{474,230}
		},
		{
			{489,241}, 
			{599,234}, 
			{604,241},
			{474,241} 
		},
	}, 
	{ // GR_1024
		{
			{869,343},
			{973,338}, 
			{981,343}, 
			{854,343} 
		},
		{
			{869,354}, 
			{973,349},
			{981,354},
			{854,354}
		},
		{
			{869,365}, 
			{973,360},
			{981,365},
			{854,365}
		},
	}
};

// escort gauge coords
int Escort_coords[GR_NUM_RESOLUTIONS][4][2] = {
	{ // GR_640
		{486, 206},
		{486, 219},
		{486, 230},
		{486, 241}
	},
	{ // GR_1024
		{865, 330},
		{865, 343},
		{865, 354},
		{865, 365}
	}
};

// monitoring text coords
int Monitoring_coords[GR_NUM_RESOLUTIONS][2] = {
	{ // GR_640
		489, 208
	},
	{ // GR_1024
		869, 331
	}
};
	
const char *Escort_gauge_filenames[GR_NUM_RESOLUTIONS][MAX_ESCORT_SHIPS] = 
{
//XSTR:OFF
	{ // GR_640
		"escort1",
		"escort2",
		"escort3"
	}, 
	{ // GR_1024
		"escort1",
		"escort2",
		"escort3"
	}
//XSTR:ON
};

static int Last_target_index;	// index into Escort_gauges for last targeted via 'Next Escort Target' key

// data needed from HUDshield.cpp
extern hud_frames Shield_mini_gauge;

// called from HUD init, loads the bitmap data in once, and resets any data for each level
void hud_escort_init()
{
	int i;

	if ( !Escort_gauges_loaded ) {
		for ( i = 0; i < MAX_ESCORT_SHIPS; i++ ) {
			Escort_gauges[i].first_frame = bm_load_animation(Escort_gauge_filenames[gr_screen.res][i], &Escort_gauges[i].num_frames);
			if ( Escort_gauges[i].first_frame == -1 ) {
				Warning(LOCATION, "Could not load in ani: %s\n", Escort_gauge_filenames[gr_screen.res][i]);
				return;
			}
		}
		Escort_gauges_loaded = 1;
	}

	Last_target_index = -1;
}

// ----------------------------------------------------------------------
// hud_escort_clear_all()
//
void hud_escort_clear_all()
{
	int i;

	Num_escort_ships = 0;
	for ( i = 0; i < MAX_ESCORT_SHIPS; i++ ) {
		Escort_ships[i].obj_signature = -99;
		Escort_ships[i].np_id = -1;
		shield_info_reset(&Escort_ships[i].hit_info);
	}
}

// internal helper function for sort.
// sorts first by priority number and then alphabetically
int escort_compare_func(const void *e1, const void *e2)
{
	escort_info *escort1, *escort2;
	int diff;
	int ret;

	escort1 = (escort_info*) e1;
	escort2 = (escort_info*) e2;

	// multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		int n1, n2;

		n1 = find_player_id(escort1->np_id);
		n2 = find_player_id(escort2->np_id);
		if((n1 < 0) || (n2 < 0) || (Net_players[n1].player == NULL) || (Net_players[n2].player == NULL)){
			ret = 0;
		} else {
			// player 1 is higher than player 2
			if(Net_players[n1].player->stats.m_kill_count_ok >= Net_players[n2].player->stats.m_kill_count_ok){
				ret = -1;
			} else {
				ret = 1;
			}
		}
	} else {
		diff = escort2->priority - escort1->priority;

		if (diff != 0) {
			ret = diff;
		} else {
			char *name1, *name2;
			name1 = Ships[Objects[escort1->objnum].instance].ship_name;
			name2 = Ships[Objects[escort2->objnum].instance].ship_name;

			ret = stricmp(name1, name2);
		}
	}

	return ret;
}

// create complete priority sorted escort list for all active ships
// escorts - array of escort info
// num_escorts - number of escorts requests in field of active ships
//	  This will be culled to MAX_ESCORTS, selecting the top set from escorts
void hud_create_complete_escort_list(escort_info *escorts, int *num_escorts)
{
	ship_obj *so;
	object *objp;	
	int idx;

	// start with none on list
	*num_escorts = 0;

	// multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		for(idx=0; idx<MAX_PLAYERS; idx++){
			// break out of the loop when we have reached our max
			if ( *num_escorts == MAX_COMPLETE_ESCORT_LIST ) {
				mprintf(("exceeded max ships in big escort list"));
				break;
			}		

			// is this a valid player			
			if(MULTI_CONNECTED(Net_players[idx]) && !MULTI_OBSERVER(Net_players[idx]) && !MULTI_STANDALONE(Net_players[idx])){
				// add the ship
				escorts[*num_escorts].objnum = -1;
				escorts[*num_escorts].obj_signature = -1;
				escorts[*num_escorts].priority = -1;
				escorts[*num_escorts].np_id = Net_players[idx].player_id;
				(*num_escorts)++;
			}
		}
	}
	// all others 
	else {
		for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
			Assert( so->objnum >= 0 && so->objnum < MAX_OBJECTS);
			if((so->objnum < 0) || (so->objnum >= MAX_OBJECTS)){
				continue;
			}
			objp = &Objects[so->objnum];
			Assert( objp->type == OBJ_SHIP );
			if(objp->type != OBJ_SHIP){
				continue;
			}

			// break out of the loop when we have reached our max
			if ( *num_escorts == MAX_COMPLETE_ESCORT_LIST ) {
				mprintf(("exceeded max ships in big escort list"));
				break;
			}		
			
			// only process ships that might be on the list
			if ( !(Ships[objp->instance].flags & SF_ESCORT) ){
				continue;
			}

			// only process ships that can be seen by sensors
			if ( (Ships[objp->instance].flags & SF_HIDDEN_FROM_SENSORS) ){
				continue;
			}

			// don't process ships that are dying, or objects that should be dead
			if ( (Ships[objp->instance].flags & (SF_DYING|SF_DEPARTING)) || (objp->flags & OF_SHOULD_BE_DEAD) ){
				continue;
			}

			// add the ship
			escorts[*num_escorts].objnum = so->objnum;
			escorts[*num_escorts].obj_signature = objp->signature;
			escorts[*num_escorts].priority = Ships[objp->instance].escort_priority;
			escorts[*num_escorts].np_id = -1;
			(*num_escorts)++;			
		}
	}
}


// ----------------------------------------------------------------------
// hud_init_escort_info()
//
// Set up the escort list
//
void hud_setup_escort_list(int level)
{
	int num_escorts, num_complete_escorts;
	escort_info complete_escorts[MAX_COMPLETE_ESCORT_LIST];

	hud_escort_clear_all();

	// get complete escort list
	hud_create_complete_escort_list(complete_escorts, &num_complete_escorts);

	// sort escort list by priority
	qsort(complete_escorts, num_complete_escorts, sizeof(escort_info), escort_compare_func);

	// set number in escort list
	num_escorts = num_complete_escorts;
	if (num_escorts > MAX_ESCORT_SHIPS) {
		num_escorts = MAX_ESCORT_SHIPS;
	}

	// add ships to escort list
	for (Num_escort_ships=0; Num_escort_ships<num_escorts; Num_escort_ships++) {
		Escort_ships[Num_escort_ships].obj_signature = complete_escorts[Num_escort_ships].obj_signature;
		Escort_ships[Num_escort_ships].priority = complete_escorts[Num_escort_ships].priority;
		Escort_ships[Num_escort_ships].objnum = complete_escorts[Num_escort_ships].objnum;
		Escort_ships[Num_escort_ships].np_id = complete_escorts[Num_escort_ships].np_id;
	}

	if(level){
		Show_escort_view = 1;
	}
}


// combine complete escort list with Escort_ships, keeping valid hit info
void merge_escort_lists(escort_info *complete_escorts, int num_complete_escorts)
{
	int i, j, top_complete_escorts;
	int valid_hit_info[MAX_ESCORT_SHIPS];

	// may be > 1 ship change to list (ie, 2 or 3 culled during same frame)
	// set Num_escort_ships and cap
	Num_escort_ships = num_complete_escorts;
	if (Num_escort_ships > MAX_ESCORT_SHIPS) {
		Num_escort_ships = MAX_ESCORT_SHIPS;
	}

	// nothing to do
	if (Num_escort_ships == 0) {
		return;
	}

	// check used as a flag whether top slots in complete_escorts were copied
	// this is important re. hit info
	for (i=0; i<MAX_ESCORT_SHIPS; i++) {
		valid_hit_info[i] = 0;
	}

	// get the top slots in complete escort list that will be copied onto Escort_ships
	top_complete_escorts = num_complete_escorts;
	if (top_complete_escorts > MAX_ESCORT_SHIPS) {
		top_complete_escorts = MAX_ESCORT_SHIPS;
	}

	// copy for Escort_ships to complete_escorts to retain hit_info
	if(!((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT))){
		for (i=0; i<top_complete_escorts; i++) {
			for (j=0; j<Num_escort_ships; j++) {
				if (Escort_ships[j].obj_signature == complete_escorts[i].obj_signature) {
					complete_escorts[i] = Escort_ships[j];
					valid_hit_info[i] = 1;
					break;
				}
			}
		}

		// copy top slots to Escort_ships
		for (i=0; i<top_complete_escorts; i++) {
			Escort_ships[i] = complete_escorts[i];
			// check all ships are valid
			int objnum = Escort_ships[i].objnum;
			Assert( objnum >=0 && objnum < MAX_OBJECTS );
			if((objnum < 0) || (objnum >= MAX_OBJECTS)){
				continue;
			}
			if ( !valid_hit_info[i] ) {
				shield_info_reset(&Escort_ships[i].hit_info);
			}	
		}
	}

	// reset Num_escort_ships
	Num_escort_ships = top_complete_escorts;
}


// ----------------------------------------------------------------------
// hud_remove_ship_from_escort_index()
//
// Take a ship out of the escort list
void hud_remove_ship_from_escort_index(int dead_index, int objnum)
{
	int			i, count, num_complete_escorts;
	escort_info bakup_arr[MAX_ESCORT_SHIPS], complete_escorts[MAX_COMPLETE_ESCORT_LIST];

	// remove him from escort list
	if((objnum >= 0) && (Objects[objnum].type == OBJ_SHIP) && (Objects[objnum].instance >= 0)){
		Ships[Objects[objnum].instance].flags &= ~SF_ESCORT;
	}

	count = 0;
	for ( i = 0; i < Num_escort_ships; i++ ) {
		if ( i != dead_index ) {
			bakup_arr[count++] = Escort_ships[i];
		}
	}

	for ( i = 0; i < count; i++ ) {
		Escort_ships[i] = bakup_arr[i];
	}

	Num_escort_ships--;
	Assert(Num_escort_ships >= 0);	

	// get complete escort list
	hud_create_complete_escort_list(complete_escorts, &num_complete_escorts);

	// sort escort list by priority
	qsort(complete_escorts, num_complete_escorts, sizeof(escort_info), escort_compare_func);

	// merge list
	merge_escort_lists(complete_escorts, num_complete_escorts);

	hud_gauge_popup_start(HUD_ESCORT_VIEW);

}

// called once per frame to remove dead or departed ships from the escort list
void hud_escort_cull_list()
{
	int i, objnum, np_index;

	// multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		for ( i = 0; i < Num_escort_ships; i++ ) {
			np_index = find_player_id(Escort_ships[i].np_id);
			
			// maybe remove him if he left
			if ( np_index < 0 ) {
				hud_remove_ship_from_escort_index(i, -1);
				i--;
			}
		}
	} 
	// everything else
	else {
		for ( i = 0; i < Num_escort_ships; i++ ) {
			objnum = Escort_ships[i].objnum;
			Assert( objnum >=0 && objnum < MAX_OBJECTS );
			if ( Objects[objnum].flags & OF_SHOULD_BE_DEAD || Ships[Objects[objnum].instance].flags & SF_HIDDEN_FROM_SENSORS ) {
				hud_remove_ship_from_escort_index(i, objnum);
				i--;
			}
		}
	}
}

// Set the color for the text to be displayed
int hud_escort_set_gauge_color(int index, int friendly)
{
	int is_flashing=0;
	shield_hit_info	*shi;

	shi = &Escort_ships[index].hit_info;

	// multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		hud_set_gauge_color(HUD_ESCORT_VIEW);
		return 0;
	}
	
	if(friendly){
		hud_set_gauge_color(HUD_ESCORT_VIEW, HUD_C_DIM);
	} else {
		gr_set_color_fast(&Color_red);
	}

	// set flashing color
	if ( !timestamp_elapsed(shi->shield_hit_timers[HULL_HIT_OFFSET]) ) {
		if ( timestamp_elapsed(shi->shield_hit_next_flash[HULL_HIT_OFFSET]) ) {
			shi->shield_hit_next_flash[HULL_HIT_OFFSET] = timestamp(SHIELD_FLASH_INTERVAL);
			shi->shield_show_bright ^= (1<<HULL_HIT_OFFSET);	// toggle between default and bright frames
		}

		is_flashing=1;
		if ( shi->shield_show_bright & (1<<HULL_HIT_OFFSET) ) {
			if(friendly){
				hud_set_gauge_color(HUD_ESCORT_VIEW, HUD_C_BRIGHT);
			} else {
				gr_set_color_fast(&Color_bright_red);
			}
		} else {			
			if(friendly){
				hud_set_gauge_color(HUD_ESCORT_VIEW, HUD_C_DIM);
			} else {
				gr_set_color_fast(&Color_dim_red);
			}			
		}
	}

	return is_flashing;
}

// draw the shield icon and integrity for the escort ship
void hud_escort_show_icon(int index, object *objp)
{
	float			shields, integrity;
	int			screen_integrity, offset;
	char			buf[255];
	ship			*sp;
	ship_info	*sip;
	shield_hit_info	*shi;	

	// multiplayer dogfight code should never get into here
	Assert(!((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)));
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return;
	}

	sp = &Ships[objp->instance];
	sip = &Ship_info[sp->ship_info_index];
	shi = &Escort_ships[index].hit_info;

	// determine if its "friendly" or not	
	if(Player_ship != NULL){
		hud_escort_set_gauge_color(index, (sp->team == Player_ship->team) ? 1 : 0);
	} else {
		hud_escort_set_gauge_color(index, 1);
	}

	// draw a 'D' if a ship is disabled
	if ( (sp->flags & SF_DISABLED) || (ship_subsys_disrupted(sp, SUBSYSTEM_ENGINE)) ) {		
		emp_hud_string( Escort_gauge_text_coords[gr_screen.res][index][3][0], Escort_gauge_text_coords[gr_screen.res][index][3][1], EG_NULL, XSTR( "D", 284));				
	}

	// print out ship name
	strcpy(buf, sp->ship_name);
	gr_force_fit_string(buf, 255, 100);	
	
	emp_hud_string( Escort_gauge_text_coords[gr_screen.res][index][0][0], Escort_gauge_text_coords[gr_screen.res][index][0][1], EG_ESCORT1 + index, buf);	

	// show ship integrity
	hud_get_target_strength(objp, &shields, &integrity);
	screen_integrity = fl2i(integrity*100 + 0.5f);
	offset = 0;
	if ( screen_integrity < 100 ) {
		offset = 2;
		if ( screen_integrity == 0 ) {
			if ( integrity > 0 ) {
				screen_integrity = 1;
			}
		}
	}
	emp_hud_printf( Escort_gauge_text_coords[gr_screen.res][index][2][0]+offset, Escort_gauge_text_coords[gr_screen.res][index][2][1], EG_NULL, "%d", screen_integrity);	
}

// multiplayer dogfight
void hud_escort_show_icon_dogfight(int index)
{
	int			hull_integrity = 100;
	char			buf[255];	
	int			np_index;
	object		*objp;
	ship_info	*sip;

	int stat_shift = 40;

	// always use the standard color to avoid confusion
	hud_set_gauge_color(HUD_ESCORT_VIEW);	

	// netplayer index
	np_index = find_player_id(Escort_ships[index].np_id);
	if((np_index < 0) || (np_index >= MAX_PLAYERS) || (Net_players[np_index].player == NULL)){
		return;
	}
	
	// print out player name
	strcpy(buf, Net_players[np_index].player->callsign);
	gr_force_fit_string(buf, 255, 100 - stat_shift);
	emp_hud_string( Escort_gauge_text_coords[gr_screen.res][index][0][0], Escort_gauge_text_coords[gr_screen.res][index][0][1], EG_ESCORT1 + index, buf);	

	// can we get the player object?
	objp = NULL;
	if((Net_players[np_index].player->objnum >= 0) && (Net_players[np_index].player->objnum < MAX_OBJECTS) && (Objects[Net_players[np_index].player->objnum].type == OBJ_SHIP)){
		objp = &Objects[Net_players[np_index].player->objnum];
		if((objp->instance >= 0) && (objp->instance < MAX_SHIPS) && (Ships[objp->instance].ship_info_index >= 0) && (Ships[objp->instance].ship_info_index < MAX_SHIPS)){
			sip = &Ship_info[Ships[objp->instance].ship_info_index];
		} else {
			return;
		}

		hull_integrity = (int)(((float)objp->hull_strength / (float)sip->initial_hull_strength) * 100.0f);
		if(hull_integrity < 0){
			hull_integrity = 0;
		}
	}

	// show ship integrity
	if(objp == NULL){	
		emp_hud_printf( Escort_gauge_text_coords[gr_screen.res][index][2][0] - stat_shift, Escort_gauge_text_coords[gr_screen.res][index][2][1], EG_NULL, "%d", Net_players[np_index].player->stats.m_kill_count_ok);	
	} else {
		emp_hud_printf( Escort_gauge_text_coords[gr_screen.res][index][2][0] - stat_shift, Escort_gauge_text_coords[gr_screen.res][index][2][1], EG_NULL, "(%d%%) %d", hull_integrity, Net_players[np_index].player->stats.m_kill_count_ok);	
	}
}


// ----------------------------------------------------------------------
// hud_display_escort()
//
// Display the data on ships in the escort list
void hud_display_escort()
{
	int			i;
	object		*objp;	

	if ( !Show_escort_view ) {
		return;
	}

	if ( !Num_escort_ships ) {
		return;
	}

	// hud_set_default_color();
	hud_set_gauge_color(HUD_ESCORT_VIEW);

	// draw the top of the escort view
	GR_AABITMAP(Escort_gauges[0].first_frame, Escort_coords[gr_screen.res][0][0], Escort_coords[gr_screen.res][0][1]);	
	gr_string(Monitoring_coords[gr_screen.res][0], Monitoring_coords[gr_screen.res][1], XSTR( "monitoring", 285));

	if ( Num_escort_ships >= 2 ) {
		GR_AABITMAP(Escort_gauges[1].first_frame, Escort_coords[gr_screen.res][1][0], Escort_coords[gr_screen.res][1][1]);		
	}

	if ( Num_escort_ships >= 3 ) {
		GR_AABITMAP(Escort_gauges[1].first_frame, Escort_coords[gr_screen.res][2][0], Escort_coords[gr_screen.res][2][1]);		
	}
	
	// draw bottom of box
	GR_AABITMAP(Escort_gauges[2].first_frame, Escort_coords[gr_screen.res][3][0], Escort_coords[gr_screen.res][Num_escort_ships][1]);	

	// multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		// draw the escort ship data
		for ( i = 0; i < Num_escort_ships; i++ ) {
			// oops. only show top 3
			if(i > 2){
				break;
			}

			// draw
			hud_escort_show_icon_dogfight(i);
		}
	}
	// everything else
	else {
		// draw the escort ship data
		for ( i = 0; i < Num_escort_ships; i++ ) {
			objp = &Objects[Escort_ships[i].objnum];
			hud_escort_show_icon(i, objp);
		}
	}
}

// ----------------------------------------------------------------------
// hud_escort_view_toggle()
//
void hud_escort_view_toggle()
{
	Show_escort_view ^= 1;
	if ( Show_escort_view ) {
		HUD_sourced_printf(HUD_SOURCE_HIDDEN, XSTR( "Escort view enabled", 286));
	} else {
		HUD_sourced_printf(HUD_SOURCE_HIDDEN, XSTR( "Escort view disabled", 287));
	}
}

// try to add a ship to the escort list, if slot available
void hud_add_ship_to_escort(int objnum, int supress_feedback)
{
	escort_info complete_escorts[MAX_COMPLETE_ESCORT_LIST];
	int num_complete_escorts, idx, found;

	// get complete escort list
	hud_create_complete_escort_list(complete_escorts, &num_complete_escorts);

	// check if ship is already on complete escort list
	found = 0;
	for (idx=0; idx<num_complete_escorts; idx++) {
		if (complete_escorts[idx].obj_signature == Objects[objnum].signature) {
			found = 1;
			break;
		}
	}

	// add new ship into complete list
	if ( !found ) {
		complete_escorts[num_complete_escorts].objnum = objnum;
		complete_escorts[num_complete_escorts].obj_signature = Objects[objnum].signature;
		complete_escorts[num_complete_escorts].priority = Ships[Objects[objnum].instance].escort_priority;

		// remove him from escort list
		Ships[Objects[objnum].instance].flags |= SF_ESCORT;

		num_complete_escorts++;
	}

	// sort escort list by priority
	qsort(complete_escorts, num_complete_escorts, sizeof(escort_info), escort_compare_func);

	// merge list
	merge_escort_lists(complete_escorts, num_complete_escorts);

	// maybe do feedback
	if ( (Num_escort_ships == MAX_ESCORT_SHIPS) && !supress_feedback) {
		found = 0;
		// search thru list for objnum
		for (idx=0; idx<Num_escort_ships; idx++) {
			if (Escort_ships[idx].objnum == objnum) {
				found = 1;
				break;
			}
		}

		if (!found) {
			HUD_sourced_printf(HUD_SOURCE_HIDDEN, XSTR( "Escort list is full with %d ships", 288), Num_escort_ships);
			snd_play( &Snds[SND_TARGET_FAIL]);
		}
	}

	hud_gauge_popup_start(HUD_ESCORT_VIEW);
}


// ----------------------------------------------------------------------
// hud_add_remove_ship_escort()
//
void hud_add_remove_ship_escort(int objnum, int supress_feedback)
{
	int in_escort, i;

	// no ships on the escort list in multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return;
	}

	if ( objnum < 0 ) {
		Int3();
		return;
	}

	if ( Objects[objnum].type != OBJ_SHIP ) {
		if ( !supress_feedback ) {
			snd_play( &Snds[SND_TARGET_FAIL]);
		}
		return;
	}

	in_escort = 0;
	for ( i = 0; i < Num_escort_ships; i++ ) {
		if ( Escort_ships[i].obj_signature == Objects[objnum].signature ) {
			in_escort = 1;
			break;
		}
	}

	if ( in_escort ) {				
		hud_remove_ship_from_escort_index(i, objnum);
		return;
	}

	hud_add_ship_to_escort(objnum, supress_feedback);
}

void hud_remove_ship_from_escort(int objnum)
{
	int in_escort, i;

	// no ships on the escort list in multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return;
	}

	if ( objnum < 0 ) {
		Int3();
		return;
	}	

	in_escort = 0;
	for ( i = 0; i < Num_escort_ships; i++ ) {
		if ( Escort_ships[i].obj_signature == Objects[objnum].signature ) {
			in_escort = 1;
			break;
		}
	}

	if ( in_escort ) {
		hud_remove_ship_from_escort_index(i, objnum);
		return;
	}	
}

// Called whenever a ship is hit to determine if that ship is in the escort list.  If it
// is, then start timers to flash the name hull/shield icon for that ship.
void hud_escort_ship_hit(object *objp, int quadrant)
{
	int					num, i;
	shield_hit_info	*shi;

	// no ships on the escort list in multiplayer dogfight
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return;
	}

	for ( i = 0; i < Num_escort_ships; i++ ) {
		if ( Escort_ships[i].objnum == OBJ_INDEX(objp) ) {
			shi = &Escort_ships[i].hit_info;
			num = Quadrant_xlate[quadrant];
			hud_gauge_popup_start(HUD_ESCORT_VIEW);
			if ( quadrant >= 0 ) {
				shi->shield_hit_timers[num] = timestamp(SHIELD_HIT_DURATION);
			} else {
				shi->shield_hit_timers[HULL_HIT_OFFSET] = timestamp(SHIELD_HIT_DURATION);
			}
		}
	}
}

// target the next ship in the escort list
void hud_escort_target_next()
{
	int objnum;

	if ( Num_escort_ships == 0 ) {
		snd_play( &Snds[SND_TARGET_FAIL], 0.0f );
		return;
	}

	Last_target_index++;
	if ( Last_target_index >= Num_escort_ships ) {
		Last_target_index = 0;
	}

	objnum = Escort_ships[Last_target_index].objnum;
	set_target_objnum( Player_ai,  objnum);
	hud_restore_subsystem_target(&Ships[Objects[objnum].instance]);
}

// return the number of ships currently on the escort list
int hud_escort_num_ships_on_list()
{
	return Num_escort_ships;
}

// Return the object number for the ship at index position in the escort list
int hud_escort_return_objnum(int index)
{
	int escort_objnum, escort_sig;
	if ( index >= Num_escort_ships ) {
		return -1;
	}

	escort_objnum = Escort_ships[index].objnum;
	escort_sig = Escort_ships[index].obj_signature;

	if ( escort_objnum < 0 ) {
		return -1;
	}

	// ensure this is still a valid index
	if ( Objects[escort_objnum].signature != escort_sig ) {
		return -1;
	}

	return Escort_ships[index].objnum;
}


void hudescort_page_in()
{
	int i;

	for ( i = 0; i < MAX_ESCORT_SHIPS; i++ ) {
		bm_page_in_aabitmap( Escort_gauges[i].first_frame, Escort_gauges[i].num_frames);
	}
}

void hud_escort_add_player(short id)
{
	Assert(Game_mode & GM_MULTIPLAYER);
	if(!(Game_mode & GM_MULTIPLAYER)){
		return;
	}	

	int idx;

	// just go through and add as long as its not a duplicate
	for(idx=0; idx<Num_escort_ships; idx++){
		if(Escort_ships[idx].np_id == id){
			return;
		}
	}

	// re-setup the escort list
	hud_setup_escort_list(0);
}

void hud_escort_remove_player(short id)
{	
	Assert(Game_mode & GM_MULTIPLAYER);
	if(!(Game_mode & GM_MULTIPLAYER)){
		return;
	}

	int idx;

	// find the instance and remove it if possible
	for(idx=0; idx<Num_escort_ships; idx++){
		if(Escort_ships[idx].np_id == id){
			hud_remove_ship_from_escort_index(idx, -1);
			return;
		}
	}
}
