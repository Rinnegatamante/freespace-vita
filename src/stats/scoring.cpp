/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Stats/Scoring.cpp $
 * $Revision: 198 $
 * $Date: 2003-06-11 20:30:33 +0200 (Wed, 11 Jun 2003) $
 * $Author: taylor $
 *
 * Scoring system code, medals, rank, etc.
 *
 * $Log$
 * Revision 1.5  2003/06/11 18:30:33  taylor
 * plug memory leaks
 *
 * Revision 1.4  2003/05/25 02:30:44  taylor
 * Freespace 1 support
 *
 * Revision 1.3  2002/06/09 04:41:27  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:52  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 23    10/25/99 5:51p Jefff
 * commented some scoring debug code
 * 
 * 22    10/15/99 3:01p Jefff
 * scoring change in coop and tvt.  for big ships, killer gets 100% of
 * score, but teammates also get 50%.
 * 
 * 21    9/12/99 9:56p Jefff
 * fixed another cause of the
 * medals-in-training-missions-not-getting-recorded bug
 * 
 * 20    9/10/99 9:44p Dave
 * Bumped version # up. Make server reliable connects not have such a huge
 * timeout. 
 * 
 * 19    9/05/99 11:19p Dave
 * Made d3d texture cache much more safe. Fixed training scoring bug where
 * it would backout scores without ever having applied them in the first
 * place.
 * 
 * 18    8/26/99 8:49p Jefff
 * Updated medals screen and about everything that ever touches medals in
 * one way or another.  Sheesh.
 * 
 * 17    8/22/99 5:53p Dave
 * Scoring fixes. Added self destruct key. Put callsigns in the logfile
 * instead of ship designations for multiplayer players.
 * 
 * 16    8/22/99 1:19p Dave
 * Fixed up http proxy code. Cleaned up scoring code. Reverse the order in
 * which d3d cards are detected.
 * 
 * 15    8/17/99 1:12p Dave
 * Send TvT update when a client has finished joining so he stays nice and
 * synched.
 * 
 * 14    8/06/99 9:46p Dave
 * Hopefully final changes for the demo.
 * 
 * 13    7/30/99 7:01p Dave
 * Dogfight escort gauge. Fixed up laser rendering in Glide.
 * 
 * 12    4/30/99 12:18p Dave
 * Several minor bug fixes.
 * 
 * 11    4/28/99 11:13p Dave
 * Temporary checkin of artillery code.
 * 
 * 10    3/01/99 10:00a Dave
 * Fxied several dogfight related stats bugs.
 * 
 * 9     2/26/99 4:14p Dave
 * Put in the ability to have multiple shockwaves for ships.
 * 
 * 8     2/23/99 2:29p Dave
 * First run of oldschool dogfight mode. 
 * 
 * 7     2/17/99 2:11p Dave
 * First full run of squad war. All freespace and tracker side stuff
 * works.
 * 
 * 6     1/29/99 2:08a Dave
 * Fixed beam weapon collisions with players. Reduced size of scoring
 * struct for multiplayer. Disabled PXO.
 * 
 * 5     1/12/99 5:45p Dave
 * Moved weapon pipeline in multiplayer to almost exclusively client side.
 * Very good results. Bandwidth goes down, playability goes up for crappy
 * connections. Fixed object update problem for ship subsystems.
 * 
 * 4     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
 * 
 * 3     10/13/98 9:29a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 84    9/15/98 11:44a Dave
 * Renamed builtin ships and wepaons appropriately in FRED. Put in scoring
 * scale factors. Fixed standalone filtering of MD missions to non-MD
 * hosts.
 * 
 * 83    9/10/98 6:14p Dave
 * Removed bogus assert.
 * 
 * 82    7/14/98 2:37p Allender
 * fixed bug where two scoring variables were not getting properly reset
 * between levels in multiplayer game
 * 
 * 81    6/01/98 11:43a John
 * JAS & MK:  Classified all strings for localization.
 * 
 * 80    5/23/98 3:16p Allender
 * work on object update packet optimizations (a new updating system).
 * Don't allow medals/promotions/badges when playing singple player
 * missions through the simulator
 * 
 * 79    5/18/98 9:14p Dave
 * Put in network config files support.
 * 
 * 78    5/16/98 9:14p Allender
 * fix scoring ckise fir training missions to actually count medals, but
 * nothing else.  Code used to Assert when wings were granted then taken
 * away because they were actually never granted in scoring structure
 * 
 * 77    5/15/98 9:52p Dave
 * Added new stats for freespace. Put in artwork for viewing stats on PXO.
 * 
 * 76    5/15/98 4:12p Allender
 * removed redbook code.  Put back in ingame join timer.  Major fixups for
 * stats in multiplayer.  Pass correct score, medals, etc when leaving
 * game.  Be sure clients display medals, badges, etc.
 * 
 * 75    5/13/98 5:13p Allender
 * red alert support to go back to previous mission
 * 
 * 74    5/07/98 12:56a Dave
 * Fixed incorrect calls to free() from stats code. Put in new artwork for
 * debrief and host options screens. Another modification to scoring
 * system for secondary weapons.
 * 
 * 73    5/06/98 3:15p Allender
 * fixed some ranking problems.  Made vasudan support ship available in
 * multiplayer mode.
 * 
 * 72    5/04/98 1:43p Dave
 * Fixed up a standalone resetting problem. Fixed multiplayer stats
 * collection for clients. Make sure all multiplayer ui screens have the
 * correct palette at all times.
 * 
 * 71    5/03/98 2:52p Dave
 * Removed multiplayer furball mode.
 * 
 * 70    5/01/98 12:34p John
 * Added code to force FreeSpace to run in the same dir as exe and made
 * all the parse error messages a little nicer.
 * 
 * 69    4/28/98 5:27p Hoffoss
 * Added kills by type stats to barracks, and fixed some bugs this turned
 * up but no one knew about yet apparently.
 * 
 * 68    4/27/98 6:01p Dave
 * Modify how missile scoring works. Fixed a team select ui bug. Speed up
 * multi_lag system. Put in new main hall.
 * 
 * 67    4/25/98 3:54p Dave
 * Fixed a scoring bug where hidden ships had improper return values from
 * scoring_eval_kill(...)
 * 
 * 66    4/21/98 11:55p Dave
 * Put in player deaths statskeeping. Use arrow keys in the ingame join
 * ship select screen. Don't quit the game if in the debriefing and server
 * leaves.
 * 
 * 65    4/21/98 4:44p Dave
 * Implement Vasudan ships in multiplayer. Added a debug function to bash
 * player rank. Fixed a few rtvoice buffer overrun problems. Fixed ui
 * problem in options screen.  
 *
 * $NoKeywords: $
 */

#include "freespace.h"
#include "pstypes.h"
#include "object.h"
#include "ship.h"
#include "multi.h"
#include "multiutil.h"
#include "scoring.h"
#include "player.h"
#include "parselo.h"
#include "multimsgs.h"
#include "medals.h"
#include "localize.h"
#include "multi_team.h"
#include "multi_dogfight.h"
#include "multi_pmsg.h"

// what percent of points of total damage to a ship a player has to have done to get an assist (or a kill) when it is killed
#define ASSIST_PERCENTAGE				(0.15f)
#define KILL_PERCENTAGE					(0.30f)

// these tables are overwritten with the values from rank.tbl
rank_stuff Ranks[NUM_RANKS];

// scoring scale factors by skill level
float Scoring_scale_factors[NUM_SKILL_LEVELS] = {
	0.2f,					// very easy
	0.4f,					// easy
	0.7f,					// medium
	1.0f,					// hard
	1.25f					// insane
};

void parse_rank_tbl()
{
	char buf[MULTITEXT_LENGTH];
	int rval, idx;

	if ((rval = setjmp(parse_abort)) != 0) {
		Error(LOCATION, "Error parsing 'rank.tbl'\r\nError code = %i.\r\n", rval);
	} 

	// open localization
	lcl_ext_open();

	read_file_text("rank.tbl");
	reset_parse();

	// parse in all the rank names
	idx = 0;
	skip_to_string("[RANK NAMES]");
	ignore_white_space();
	while ( required_string_either("#End", "$Name:") ) {
		Assert ( idx < NUM_RANKS );
		required_string("$Name:");
		stuff_string( Ranks[idx].name, F_NAME, NULL );
		required_string("$Points:");
		stuff_int( &Ranks[idx].points );
		required_string("$Bitmap:");
		stuff_string( Ranks[idx].bitmap, F_NAME, NULL );
		required_string("$Promotion Voice Base:");
		stuff_string( Ranks[idx].promotion_voice_base, F_NAME, NULL, MAX_FILENAME_LEN - 2 );
		required_string("$Promotion Text:");
		stuff_string(buf, F_MULTITEXT, NULL);
		drop_white_space(buf);
		compact_multitext_string(buf);
		Ranks[idx].promotion_text = strdup(buf);
		idx++;
	}

	required_string("#End");

	// be sure that all rank points are in order
#ifndef NDEBUG
	for ( idx = 0; idx < NUM_RANKS-1; idx++ ) {
		if ( Ranks[idx].points >= Ranks[idx+1].points )
			Int3();
	}
#endif

	// close localization
	lcl_ext_close();
}

// free memory from table parsing
void scoring_tbl_close()
{
	int i;
	
	for (i=0; i<NUM_RANKS; i++) {
		if (Ranks[i].promotion_text) {
			free(Ranks[i].promotion_text);
			Ranks[i].promotion_text = NULL;
		}
	}
}

// initialize a nice blank scoring element
void init_scoring_element(scoring_struct *s)
{
	int i;

	if (s == NULL) {
		Int3();	//	DaveB -- Fix this!
		// read_pilot_file(char* callsign);
		return;
	}

	memset(s, 0, sizeof(scoring_struct));
	s->score = 0;
	s->rank = RANK_ENSIGN;
	s->assists = 0;
	s->kill_count = 0;
	s->kill_count_ok = 0;

	for (i=0; i<NUM_MEDALS; i++){
		s->medals[i] = 0;
	}

	for (i=0; i<MAX_SHIP_TYPES; i++){
		s->kills[i] = 0;
		s->m_kills[i] = 0;
	}

	s->m_kill_count		= 0;
	s->m_kill_count_ok	= 0;

	s->m_score = 0;
	s->m_assists = 0;
   s->p_bonehead_hits=0; s->mp_bonehead_hits=0;
	s->s_bonehead_hits=0; s->ms_bonehead_hits=0;
	s->m_bonehead_kills=0;
	
	s->bonehead_kills=0;   
   
	s->p_shots_fired=0; s->p_shots_hit=0;
   s->s_shots_fired=0; s->s_shots_hit=0;

   s->mp_shots_fired=0; s->mp_shots_hit=0;
   s->ms_shots_fired=0; s->ms_shots_hit=0;

	s->m_player_deaths = 0;

   s->flags = 0;	

	s->missions_flown = 0;
	s->flight_time = 0;
	s->last_flown = 0;
	s->last_backup = 0;

	for(i=0; i<MAX_PLAYERS; i++){
		s->m_dogfight_kills[i] = 0;
	}
}

#ifndef NDEBUG
//XSTR:OFF
void scoring_eval_harbison( ship *shipp )
{
	FILE *fp;

	if ( !stricmp(shipp->ship_name, "alpha 2") && (!stricmp(Game_current_mission_filename, "demo01") || !stricmp(Game_current_mission_filename, "sm1-01")) ) {
		int death_count;

		fp = fopen("i:\\volition\\cww\\harbison.txt", "r+t");
		if ( !fp )
			return;
		fscanf(fp, "%d", &death_count );
		death_count++;
		fseek(fp, 0, SEEK_SET);
		fprintf(fp, "%d\n", death_count);
		fclose(fp);
	}
}
//XSTR:ON
#endif

// initialize the Player's mission-based stats before he goes into a mission
void scoring_level_init( scoring_struct *scp )
{
	int i;

	scp->m_medal_earned = -1;		// hasn't earned a medal yet
	scp->m_promotion_earned = -1;
	scp->m_badge_earned = -1;
   scp->m_score = 0;
	scp->m_assists = 0;
	scp->mp_shots_fired=0;
	scp->mp_shots_hit = 0;
	scp->ms_shots_fired = 0;
	scp->ms_shots_hit = 0;

	scp->mp_bonehead_hits=0;
	scp->ms_bonehead_hits=0;
	scp->m_bonehead_kills=0;

   for (i=0; i<MAX_SHIP_TYPES; i++){
		scp->m_kills[i] = 0;
		scp->m_okKills[i]=0;
	}

	scp->m_kill_count = 0;
	scp->m_kill_count_ok = 0;
	
	scp->m_player_deaths =0;

	for(i=0; i<MAX_PLAYERS; i++){
		scp->m_dogfight_kills[i] = 0;
	}
}

void scoring_eval_rank( scoring_struct *sc )
{
	int i, score, new_rank, old_rank;

	old_rank = sc->rank;
	new_rank = old_rank;

	// first check to see if the promotion flag is set -- if so, return the new rank
	if ( Player->flags & PLAYER_FLAGS_PROMOTED ) {
	
		new_rank = sc->rank;

		// if the player does indeed get promoted, we should change his mission score
		// to reflect the differce between all time and new rank score
#ifdef MAKE_FS1
		if ( sc->rank < MAX_FREESPACE1_RANK ) {
#else
		if ( sc->rank < MAX_FREESPACE2_RANK ) {
#endif
			new_rank = sc->rank + 1;
			if ( (sc->m_score + sc->score) < Ranks[new_rank].points )
				sc->m_score = (Ranks[new_rank].points - sc->score);
		}
	} else {
		// we get here only if player wasn't promoted automatically.
		score = sc->m_score + sc->score;
		for (i=0; i<NUM_RANKS; i++) {
			if ( score >= Ranks[i].points )
				new_rank = i;
		}
	}

	// if the ranks do not match, then "grant" the new rank
	if ( old_rank != new_rank ) {
		Assert( new_rank >= 0 );
		sc->m_promotion_earned = new_rank;
		sc->rank = new_rank;
	}
}

// function to evaluate whether or not a new badge is going to be awarded.  This function returns
// which medal is awarded.
void scoring_eval_badges(scoring_struct *sc)
{
	int i, total_kills, badge;

	// to determine badges, we count kills based on fighter/bomber types.  We must count kills in
	// all time stats + current mission stats.  And, only for enemy fighters/bombers
	total_kills = 0;
	for (i = 0; i < MAX_SHIP_TYPES; i++ ) {
		if ( (Ship_info[i].flags & SIF_FIGHTER) || (Ship_info[i].flags & SIF_BOMBER) ) {
			total_kills += sc->m_okKills[i];
			total_kills += sc->kills[i];
		}
	}

	// total_kills should now reflect the number of kills on hostile fighters/bombers.  Check this number
	// against badge kill numbers, and return the badge index if we would get a new one.
	badge = -1;
	for (i = 0; i < MAX_BADGES; i++ ) {
		if ( total_kills >= Medals[Badge_index[i]].kills_needed ){
			badge = i;
		}
	}

	// if player could have a badge based on kills, and doesn't currently have this badge, then
	// return the badge id.
	if ( (badge != -1 ) && (sc->medals[Badge_index[badge]] < 1) ) {
		sc->medals[Badge_index[badge]] = 1;
		sc->m_badge_earned = badge;
	}
}

// central point for dealing with accepting the score for a misison.
void scoring_do_accept(scoring_struct *score)
{
	int idx;

	// do rank, badges, and medals first since they require the alltime stuff
	// to not be updated yet.	

	// do medal stuff
	if ( score->m_medal_earned != -1 ){
		score->medals[score->m_medal_earned]++;
	}

	// return when in training mission.  We can grant a medal in training, but don't
	// want to calculate any other statistics.
	if (The_mission.game_type == MISSION_TYPE_TRAINING){
		return;
	}	

	scoring_eval_rank(score);
	scoring_eval_badges(score);

	score->kill_count += score->m_kill_count;
	score->kill_count_ok += score->m_kill_count_ok;

	score->score += score->m_score;
	score->assists += score->m_assists;
	score->p_shots_fired += score->mp_shots_fired;
	score->s_shots_fired += score->ms_shots_fired;

	score->p_shots_hit += score->mp_shots_hit;
	score->s_shots_hit += score->ms_shots_hit;

	score->p_bonehead_hits += score->mp_bonehead_hits;
	score->s_bonehead_hits += score->ms_bonehead_hits;
	score->bonehead_kills += score->m_bonehead_kills;

	for(idx=0;idx<MAX_SHIP_TYPES;idx++){
		score->kills[idx] = (unsigned short)(score->kills[idx] + score->m_okKills[idx]);
	}

	// add in mission time
	score->flight_time += (unsigned int)f2fl(Missiontime);
	score->last_backup = score->last_flown;
	score->last_flown = time(NULL);
	score->missions_flown++;
}

// backout the score for a mission.  This function gets called when the player chooses to refly a misison
// after debriefing
void scoring_backout_accept( scoring_struct *score )
{
	int idx;

	// if a badge was earned, take it back
	if ( score->m_badge_earned != -1){
		score->medals[Badge_index[score->m_badge_earned]] = 0;
	}

	// return when in training mission.  We can grant a medal in training, but don't
	// want to calculate any other statistics.
	if (The_mission.game_type == MISSION_TYPE_TRAINING){
		return;
	}

	score->kill_count -= score->m_kill_count;
	score->kill_count_ok -= score->m_kill_count_ok;

	score->score -= score->m_score;
	score->assists -= score->m_assists;
	score->p_shots_fired -= score->mp_shots_fired;
	score->s_shots_fired -= score->ms_shots_fired;

	score->p_shots_hit -= score->mp_shots_hit;
	score->s_shots_hit -= score->ms_shots_hit;

	score->p_bonehead_hits -= score->mp_bonehead_hits;
	score->s_bonehead_hits -= score->ms_bonehead_hits;
	score->bonehead_kills -= score->m_bonehead_kills;

	for(idx=0;idx<MAX_SHIP_TYPES;idx++){
		score->kills[idx] = (unsigned short)(score->kills[idx] - score->m_okKills[idx]);
	}

	// if the player was given a medal, take it back
	if ( score->m_medal_earned != -1 ) {
		score->medals[score->m_medal_earned]--;
		Assert( score->medals[score->m_medal_earned] >= 0 );
	}

	// if the player was promoted, take it back
	if ( score->m_promotion_earned != -1) {
		score->rank--;
		Assert( score->rank >= 0 );
	}	

	score->flight_time -= (unsigned int)f2fl(Missiontime);
	score->last_flown = score->last_backup;	
	score->missions_flown--;
}

// merge any mission stats accumulated into the alltime stats (as well as updating per campaign stats)
void scoring_level_close(int accepted)
{
	int idx;
	scoring_struct *sc;

	// want to calculate any other statistics.
	if (The_mission.game_type == MISSION_TYPE_TRAINING){
		// call scoring_do_accept
		// this will grant any potential medals and then early bail, and
		// then we will early bail
		scoring_do_accept(&Player->stats);
		return;
	}

	if(accepted){
		// apply mission stats for all players in the game
		if(Game_mode & GM_MULTIPLAYER){
			nprintf(("Network","Storing stats for all players now\n"));
			for(idx=0;idx<MAX_PLAYERS;idx++){
				if(MULTI_CONNECTED(Net_players[idx]) && !MULTI_STANDALONE(Net_players[idx])){
					// get the scoring struct
					sc = &Net_players[idx].player->stats;
					scoring_do_accept( sc );
				}
			}
		} else {
			nprintf(("General","Storing stats now\n"));
			scoring_do_accept( &Player->stats );
		}

		// If this mission doesn't allow promotion or badges
		// then be sure that these don't get done.  Don't allow promotions or badges when
		// playing normally and not in a campaign.
		if ( (The_mission.flags & MISSION_FLAG_NO_PROMOTION) || ((Game_mode & GM_NORMAL) && !(Game_mode & GM_CAMPAIGN_MODE)) ) {
			if ( Player->stats.m_promotion_earned != -1) {
				Player->stats.rank--;
				Player->stats.m_promotion_earned = -1;
			}

			// if a badge was earned, take it back
			if ( Player->stats.m_badge_earned != -1){
				Player->stats.medals[Badge_index[Player->stats.m_badge_earned]] = -1;
				Player->stats.m_badge_earned = -1;
			}
		}

	} 	
}

// STATS damage, assists recording stuff
void scoring_add_damage(object *ship_obj,object *other_obj,float damage)
{
	int found_slot, signature;
	int lowest_index,idx;
	object *use_obj;
	ship *sp;

	// multiplayer clients bail here
	if(MULTIPLAYER_CLIENT){
		return;
	}

	// if we have no other object, bail
	if(other_obj == NULL){
		return;
	}	

	// for player kill/assist evaluation, we have to know exactly how much damage really mattered. For example, if
	// a ship had 1 hit point left, and the player hit it with a nuke, it doesn't matter that it did 10,000,000 
	// points of damage, only that 1 point would count
	float actual_damage = 0.0f;
	
	// other_obj might not always be the parent of other_obj (in the case of debug code for sure).  See
	// if the other_obj has a parent, and if so, use the parent.  If no parent, see if other_obj is a ship
	// and if so, use that ship.
	if ( other_obj->parent != -1 ){		
		use_obj = &Objects[other_obj->parent];
		signature = use_obj->signature;
	} else {
		signature = other_obj->signature;
		use_obj = other_obj;
	}
	
	// don't count damage done to a ship by himself
	if(use_obj == ship_obj){
		return;
	}

	// get a pointer to the ship and add the actual amount of damage done to it
	// get the ship object, and determine the _actual_ amount of damage done
	sp = &Ships[ship_obj->instance];
	// see comments at beginning of function
	if(ship_obj->hull_strength < 0.0f){
		actual_damage = damage + ship_obj->hull_strength;
	} else {
		actual_damage = damage;
	}
	if(actual_damage < 0.0f){
		actual_damage = 0.0f;
	}
	sp->total_damage_received += actual_damage;

	// go through and clear out all old damagers
	for(idx=0; idx<MAX_DAMAGE_SLOTS; idx++){
		if((sp->damage_ship_id[idx] >= 0) && (ship_get_by_signature(sp->damage_ship_id[idx]) < 0)){
			sp->damage_ship_id[idx] = -1;
			sp->damage_ship[idx] = 0;
		}
	}

	// only evaluate possible kill/assist numbers if the hitting object (use_obj) is a piloted ship (ie, ignore asteroids, etc)
	// don't store damage a ship may do to himself
	if((ship_obj->type == OBJ_SHIP) && (use_obj->type == OBJ_SHIP)){
		found_slot = 0;
		// try and find an open slot
		for(idx=0;idx<MAX_DAMAGE_SLOTS;idx++){
			// if this ship object doesn't exist anymore, use the slot
			if((sp->damage_ship_id[idx] == -1) || (ship_get_by_signature(sp->damage_ship_id[idx]) < 0) || (sp->damage_ship_id[idx] == signature) ){
				found_slot = 1;
				break;
			}
		}

		// if not found (implying all slots are taken), then find the slot with the lowest damage % and use that
		if(!found_slot){
			lowest_index = 0;
			for(idx=0;idx<MAX_DAMAGE_SLOTS;idx++){
				if(sp->damage_ship[idx] < sp->damage_ship[lowest_index]){
				   lowest_index = idx;
				}
			}
		} else {
			lowest_index = idx;
		}

		// fill in the slot damage and damager-index
		if(found_slot){
			sp->damage_ship[lowest_index] += actual_damage;								
		} else {
			sp->damage_ship[lowest_index] = actual_damage;
		}
		sp->damage_ship_id[lowest_index] = signature;
	}	
}

char Scoring_debug_text[4096];

// evaluate a kill on a ship
void scoring_eval_kill(object *ship_obj)
{		
	float max_damage_pct;		// the pct% of total damage the max damage object did
	int max_damage_index;		// the index into the dying ship's damage_ship[] array corresponding the greatest amount of damage
	int killer_sig;				// signature of the guy getting credit for the kill (or -1 if none)
	int idx,net_player_num;
	player *plr;					// pointer to a player struct if it was a player who got the kill
	net_player *net_plr = NULL;
	ship *dead_ship;				// the ship which was killed
	net_player *dead_plr = NULL;
	int i;

	// multiplayer clients bail here
	if(MULTIPLAYER_CLIENT){
		return;
	}

	// we don't evaluate kills on anything except ships
	if(ship_obj->type != OBJ_SHIP){
		return;	
	}
	if((ship_obj->instance < 0) || (ship_obj->instance >= MAX_SHIPS)){
		return;
	}

	// assign the dead ship
	dead_ship = &Ships[ship_obj->instance];

	// evaluate player deaths
	if(Game_mode & GM_MULTIPLAYER){
		net_player_num = multi_find_player_by_object(ship_obj);
		if(net_player_num != -1){
			Net_players[net_player_num].player->stats.m_player_deaths++;
			nprintf(("Network","Setting player %s deaths to %d\n",Net_players[net_player_num].player->callsign,Net_players[net_player_num].player->stats.m_player_deaths));
			dead_plr = &Net_players[net_player_num];
		}
	} else {
		if(ship_obj == Player_obj){
			Player->stats.m_player_deaths++;
		}
	}

	// if this ship doesn't show up on player sensors, then don't eval a kill
	if ( dead_ship->flags & SF_HIDDEN_FROM_SENSORS ){
		// make sure to set invalid killer id numbers
		dead_ship->damage_ship_id[0] = -1;
		dead_ship->damage_ship[0] = -1.0f;
		return;
	}

#ifndef NDEBUG
	scoring_eval_harbison( dead_ship );
#endif

	net_player_num = -1;

	// clear out invalid damager ships
	for(idx=0; idx<MAX_DAMAGE_SLOTS; idx++){
		if((dead_ship->damage_ship_id[idx] >= 0) && (ship_get_by_signature(dead_ship->damage_ship_id[idx]) < 0)){
			dead_ship->damage_ship[idx] = 0.0f;
			dead_ship->damage_ship_id[idx] = -1;
		}
	}
			
	// determine which object did the most damage to the dying object, and how much damage that was
	max_damage_index = -1;
	for(idx=0;idx<MAX_DAMAGE_SLOTS;idx++){
		// bogus ship
		if(dead_ship->damage_ship_id[idx] < 0){
			continue;
		}

		// if this slot did more damage then the next highest slot
		if((max_damage_index == -1) || (dead_ship->damage_ship[idx] > dead_ship->damage_ship[max_damage_index])){
			max_damage_index = idx;
		}			
	}
	
	// doh
	if((max_damage_index < 0) || (max_damage_index >= MAX_DAMAGE_SLOTS)){
		return;
	}

	// the pct of total damage applied to this ship
	max_damage_pct = dead_ship->damage_ship[max_damage_index] / dead_ship->total_damage_received;
	if(max_damage_pct < 0.0f){
		max_damage_pct = 0.0f;
	} 
	if(max_damage_pct > 1.0f){
		max_damage_pct = 1.0f;
	}

	// only evaluate if the max damage % is high enough to record a kill and it was done by a valid object
	if((max_damage_pct >= KILL_PERCENTAGE) && (dead_ship->damage_ship_id[max_damage_index] >= 0)){
		// set killer_sig for this ship to the signature of the guy who gets credit for the kill
		killer_sig = dead_ship->damage_ship_id[max_damage_index];

		// null this out for now
		plr = NULL;
		net_plr = NULL;

		// get the player (whether single or multiplayer)
		net_player_num = -1;
		if(Game_mode & GM_MULTIPLAYER){
			net_player_num = multi_find_player_by_signature(killer_sig);
			if(net_player_num != -1){
				plr = Net_players[net_player_num].player;
				net_plr = &Net_players[net_player_num];
			}
		} else {
			if(Objects[Player->objnum].signature == killer_sig){
				plr = Player;
			}
		}		

		// if we found a valid player, evaluate some kill details
		if(plr != NULL){
			int si_index;

			// bogus
			if((plr->objnum < 0) || (plr->objnum >= MAX_OBJECTS)){
				return;
			}			

			// get the ship info index of the ship type of this kill.  we need to take ship
			// copies into account here.
			si_index = dead_ship->ship_info_index;
			if ( Ship_info[si_index].flags & SIF_SHIP_COPY ){
				si_index = ship_info_base_lookup( si_index );
			}

			// if you hit this next Assert, find allender.  If not here, don't worry about it, you may safely
			// continue
			Assert( !(Ship_info[si_index].flags & SIF_SHIP_COPY) );

			// if he killed a guy on his own team increment his bonehead kills
			if((Ships[Objects[plr->objnum].instance].team == dead_ship->team) && !((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT))){
				plr->stats.m_bonehead_kills++;
				plr->stats.m_score -= (int)(dead_ship->score * scoring_get_scale_factor());

				// squad war
				if(net_plr != NULL){
					multi_team_maybe_add_score(-(int)(dead_ship->score * scoring_get_scale_factor()), net_plr->p_info.team);
				}
			} 
			// otherwise increment his valid kill count and score
			else {
				// dogfight mode
				if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT) && (multi_find_player_by_object(ship_obj) < 0)){
					// don't add a kill for dogfight kills on non-players
				} else {
					plr->stats.m_okKills[si_index]++;		
					plr->stats.m_kill_count_ok++;
					plr->stats.m_score += (int)(dead_ship->score * scoring_get_scale_factor());
					hud_gauge_popup_start(HUD_KILLS_GAUGE);

					// multiplayer
					if(net_plr != NULL){
						multi_team_maybe_add_score((int)(dead_ship->score * scoring_get_scale_factor()), net_plr->p_info.team);

						// award teammates 50% of score value for big ship kills
						// not in dogfight tho
						if (!(Netgame.type_flags & NG_TYPE_DOGFIGHT) && (Ship_info[dead_ship->ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
							for (i=0; i<MAX_PLAYERS; i++) {
								if (MULTI_CONNECTED(Net_players[i]) && (Net_players[i].p_info.team == net_plr->p_info.team) && (&Net_players[i] != net_plr)) {
									Net_players[i].player->stats.m_score += (int)(dead_ship->score * scoring_get_scale_factor() * 0.5f);
/*
#if !defined(RELEASE_REAL)
									// DEBUG CODE TO TEST NEW SCORING
									char score_text[1024] = "";
									sprintf(score_text, "You get %d pts for the helping kill the big ship", (int)(dead_ship->score * scoring_get_scale_factor() * 0.5f));							
									if (Net_players[i].player != Net_player->player) {			// check if its me
										send_game_chat_packet(Net_player, score_text, MULTI_MSG_TARGET, &Net_players[i], NULL, 2);								
									} else {
										HUD_printf(score_text);
									}
#endif
*/
								}
							}
						}

						// death message
						if((Net_player != NULL) && (Net_player->flags & NETINFO_FLAG_AM_MASTER) && (net_plr != NULL) && (dead_plr != NULL) && (net_plr->player != NULL) && (dead_plr->player != NULL)){
							char dead_text[1024] = "";

							sprintf(dead_text, "%s gets the kill for %s", net_plr->player->callsign, dead_plr->player->callsign);							
							send_game_chat_packet(Net_player, dead_text, MULTI_MSG_ALL, NULL, NULL, 2);
							HUD_printf(dead_text);
						}
					}
				}
			}
				
			// increment his all-encompassing kills
			plr->stats.m_kills[si_index]++;
			plr->stats.m_kill_count++;			
			
			// update everyone on this guy's kills if this is multiplayer
			if(MULTIPLAYER_MASTER && (net_player_num != -1)){
				// send appropriate stats
				if(Netgame.type_flags & NG_TYPE_DOGFIGHT){
					// evaluate dogfight kills
					multi_df_eval_kill(&Net_players[net_player_num], ship_obj);

					// update stats
					send_player_stats_block_packet(&Net_players[net_player_num], STATS_DOGFIGHT_KILLS);
				} else {
					send_player_stats_block_packet(&Net_players[net_player_num], STATS_MISSION_KILLS);
				}				
			}
		}
	} else {
		// set killer_sig for this ship to -1, indicating no one got the kill for it
		killer_sig = -1;
	}		
		
	// pass in the guy who got the credit for the kill (if any), so that he doesn't also
	// get credit for an assist
	scoring_eval_assists(dead_ship,killer_sig);	

	// bash damage_ship_id[0] with the signature of the guy who is getting credit for the kill
	dead_ship->damage_ship_id[0] = killer_sig;
	dead_ship->damage_ship[0] = max_damage_pct;

/*
	// debug code
#if !defined(RELEASE_REAL)
	if (Game_mode & GM_MULTIPLAYER) {
		char buf[256];
		sprintf(Scoring_debug_text, "%s killed.\nDamage by ship:\n\n", Ship_info[dead_ship->ship_info_index].name);

		// show damage done by player
		for (int i=0; i<MAX_DAMAGE_SLOTS; i++) {
			int net_player_num = multi_find_player_by_signature(dead_ship->damage_ship_id[i]);
			if (net_player_num != -1) {
				plr = Net_players[net_player_num].player;
				sprintf(buf, "%s: %f", plr->callsign, dead_ship->damage_ship[i]);

				if (dead_ship->damage_ship_id[i] == killer_sig ) {
					strcat(buf, "  KILLER\n");
				} else {
					strcat(buf, "\n");
				}

				strcat(Scoring_debug_text, buf);	
			}

		}
	}
#endif
*/
}

// kill_id is the object signature of the guy who got the credit for the kill (may be -1, if no one got it)
// this is to insure that you don't also get an assist if you get the kill.
void scoring_eval_assists(ship *sp,int killer_sig)
{
	int idx;
	player *plr;

	// multiplayer clients bail here
	if(MULTIPLAYER_CLIENT){
		return;
	}
		
	// evaluate each damage slot to see if it did enough to give the assis
	for(idx=0;idx<MAX_DAMAGE_SLOTS;idx++){
		// if this slot did enough damage to get an assist
		if((sp->damage_ship[idx]/sp->total_damage_received) >= ASSIST_PERCENTAGE){
			// get the player which did this damage (if any)
			plr = NULL;
			
			// multiplayer
			if(Game_mode & GM_MULTIPLAYER){
				int net_player_num = multi_find_player_by_signature(sp->damage_ship_id[idx]);
				if(net_player_num != -1){
					plr = Net_players[net_player_num].player;
				}
			}
			// single player
			else {
				if(Objects[Player->objnum].signature == sp->damage_ship_id[idx]){
					plr = Player;
				}
			}

			// if we found a player, give him the assist if it wasn't on his own team
			if((plr != NULL) && (sp->team != Ships[Objects[plr->objnum].instance].team) && (killer_sig != Objects[plr->objnum].signature)){
				plr->stats.m_assists++;

				nprintf(("Network","-==============GAVE PLAYER %s AN ASSIST=====================-\n",plr->callsign));
				break;
			}
		}
	}
}

// eval a hit on an object (for primary and secondary hit purposes)
void scoring_eval_hit(object *hit_obj, object *other_obj,int from_blast)
{	
	// multiplayer clients bail here
	if(MULTIPLAYER_CLIENT){
		return;
	}

	// only evaluate hits on ships and asteroids
	if((hit_obj->type != OBJ_SHIP) && (hit_obj->type != OBJ_ASTEROID)){
		return;
	}

	// if the other_obj == NULL, we can't evaluate where it came from, so bail here
	if(other_obj == NULL){
		return;
	}

	// other bogus situtations
	if(other_obj->instance < 0){
		return;
	}
	
	if((other_obj->type == OBJ_WEAPON) && !(Weapons[other_obj->instance].weapon_flags & WF_ALREADY_APPLIED_STATS)){		
		// bogus weapon
		if(other_obj->instance >= MAX_WEAPONS){
			return;
		}

		// bogus parent
		if(other_obj->parent < 0){
			return;
		}
		if(other_obj->parent >= MAX_SHIPS){
			return;
		}
		if(Objects[other_obj->parent].type != OBJ_SHIP){
			return;
		}
		if((Objects[other_obj->parent].instance < 0) || (Objects[other_obj->parent].instance >= MAX_SHIPS)){
			return;
		}		

		int is_bonehead = 0;
		int sub_type = Weapon_info[Weapons[other_obj->instance].weapon_info_index].subtype;

		// determine if this was a bonehead hit or not
		if(hit_obj->type == OBJ_SHIP){
		   is_bonehead = Ships[hit_obj->instance].team==Ships[Objects[other_obj->parent].instance].team ? 1 : 0;
		}
		// can't have a bonehead hit on an asteroid
		else {
			is_bonehead = 0;
		}

		// set the flag indicating that we've already applied a "stats" hit for this weapon
		// Weapons[other_obj->instance].weapon_flags |= WF_ALREADY_APPLIED_STATS;

		// in multiplayer -- only the server records the stats
		if( Game_mode & GM_MULTIPLAYER ) {
			if ( Net_player->flags & NETINFO_FLAG_AM_MASTER ) {
				int player_num;

				// get the player num of the parent object.  A player_num of -1 means that the
				// parent of this object was not a player
				player_num = multi_find_player_by_object( &Objects[other_obj->parent] );
				if ( player_num != -1 ) {
					switch(sub_type) {
					case WP_LASER : 
						if(is_bonehead){
							Net_players[player_num].player->stats.mp_bonehead_hits++;
						} else {
							Net_players[player_num].player->stats.mp_shots_hit++; 
						}

						// Assert( Net_players[player_num].player->stats.mp_shots_hit <= Net_players[player_num].player->stats.mp_shots_fired );
						break;
					case WP_MISSILE :
						// friendly hit, once it hits a friendly, its done
						if(is_bonehead){					
							if(!from_blast){
								Net_players[player_num].player->stats.ms_bonehead_hits++;
							}					
						}
						// hostile hit
						else {
							// if its a bomb, count every bit of damage it does
							if(Weapons[other_obj->instance].weapon_flags & WIF_BOMB){
								// once we get impact damage, stop keeping track of it
								Net_players[player_num].player->stats.ms_shots_hit++;
							}
							// if its not a bomb, only count impact damage
							else {
								if(!from_blast){
									Net_players[player_num].player->stats.ms_shots_hit++;
								}	
							}				
						}
					default : 
						break;
					}
				}
			}
		} else if(Player_obj == &(Objects[other_obj->parent])){
			switch(sub_type){
			case WP_LASER : 
				if(is_bonehead){
					Player->stats.mp_bonehead_hits++;
				} else {
					Player->stats.mp_shots_hit++; 
				}
				break;
			case WP_MISSILE :
				// friendly hit, once it hits a friendly, its done
				if(is_bonehead){					
					if(!from_blast){
						Player->stats.ms_bonehead_hits++;
					}					
				}
				// hostile hit
				else {
					// if its a bomb, count every bit of damage it does
					if(Weapons[other_obj->instance].weapon_flags & WIF_BOMB){
						// once we get impact damage, stop keeping track of it
						Player->stats.ms_shots_hit++;
					}
					// if its not a bomb, only count impact damage
					else {
						if(!from_blast){
							Player->stats.ms_shots_hit++;
						}
					}
				}				
				break;
			default : 
				break;
			}
		}
	}
}

// get a scaling factor for adding/subtracting from mission score
float scoring_get_scale_factor()
{
	// multiplayer dogfight. don't scale anything
	if((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT)){
		return 1.0f;
	}

	// check for bogus Skill_level values
	Assert((Game_skill_level >= 0) && (Game_skill_level < NUM_SKILL_LEVELS));
	if((Game_skill_level < 0) || (Game_skill_level > NUM_SKILL_LEVELS-1)){
		return Scoring_scale_factors[0];
	}

	// return the correct scale value
	return Scoring_scale_factors[Game_skill_level];
}


// ----------------------------------------------------------------------------------------
// DCF functions
//

// bash the passed player to the specified rank
void scoring_bash_rank(player *pl,int rank)
{	
	// if this is an invalid rank, do nothing
	if((rank < RANK_ENSIGN) || (rank > RANK_ADMIRAL)){
		nprintf(("General","Could not bash player rank - invalid value!!!\n"));
		return;
	}

	// set the player's score and rank
	pl->stats.score = Ranks[rank].points + 1;
	pl->stats.rank = rank;
}

DCF(rank, "changes scoring vars")
{
	if(Dc_command){		
		dc_get_arg(ARG_INT);		
		
		// parse the argument and change things around accordingly		
		if((Dc_arg_type & ARG_INT) && (Player != NULL)){							
			scoring_bash_rank(Player,Dc_arg_int);
		}		
	}
	dc_printf("Usage\n0 : Ensign\n1 : Lieutenant Junior Grade\n");
	dc_printf("2 : Lietenant\n3 : Lieutenant Commander\n");
	dc_printf("4 : Commander\n5 : Captain\n6 : Commodore\n");
	dc_printf("7 : Rear Admiral\n8 : Vice Admiral\n9 : Admiral");
}
