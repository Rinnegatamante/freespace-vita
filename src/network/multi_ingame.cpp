/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_ingame.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * $Log$
 * Revision 1.7  2005/10/02 09:30:10  taylor
 * sync up rest of big-endian network changes.  it should at least be as good as what's in FS2_Open now, only better :)
 *
 * Revision 1.6  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.5  2004/06/11 01:17:50  tigital
 * byte-swapping changes for bigendian systems
 *
 * Revision 1.4  2003/08/03 16:10:29  taylor
 * cleanup; compile warning fixes
 *
 * Revision 1.3  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.2  2002/06/02 04:26:34  relnev
 * warning cleanup
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 20    8/03/99 11:02p Dave
 * Maybe fixed sync problems in multiplayer.
 * 
 * 19    7/30/99 7:01p Dave
 * Dogfight escort gauge. Fixed up laser rendering in Glide.
 * 
 * 18    7/28/99 5:33p Dave
 * Nailed the missing stats bug to the wall. Problem was optimized build
 * and using GET_DATA() with array elements. BLECH.
 * 
 * 17    7/26/99 6:07p Dave
 * Removed some warnings.
 * 
 * 16    7/26/99 5:50p Dave
 * Revised ingame join. Better? We'll see....
 * 
 * 15    7/24/99 5:48p Jefff
 * converted to new UI stuff -- added 1024 support
 * 
 * 14    7/08/99 10:53a Dave
 * New multiplayer interpolation scheme. Not 100% done yet, but still
 * better than the old way.
 * 
 * 13    4/21/99 6:15p Dave
 * Did some serious housecleaning in the beam code. Made it ready to go
 * for anti-fighter "pulse" weapons. Fixed collision pair creation. Added
 * a handy macro for recalculating collision pairs for a given object.
 * 
 * 12    3/10/99 6:50p Dave
 * Changed the way we buffer packets for all clients. Optimized turret
 * fired packets. Did some weapon firing optimizations.
 * 
 * 11    3/09/99 6:24p Dave
 * More work on object update revamping. Identified several sources of
 * unnecessary bandwidth.
 * 
 * 10    3/08/99 7:03p Dave
 * First run of new object update system. Looks very promising.
 * 
 * 9     3/01/99 7:39p Dave
 * Added prioritizing ship respawns. Also fixed respawns in TvT so teams
 * don't mix respawn points.
 * 
 * 8     1/30/99 5:08p Dave
 * More new hi-res stuff.Support for nice D3D textures.
 * 
 * 7     1/27/99 9:56a Dave
 * Temporary checkin of beam weapons for Dan to make cool sounds.
 * 
 * 6     11/19/98 8:03a Dave
 * Full support for D3-style reliable sockets. Revamped packet lag/loss
 * system, made it receiver side and at the lowest possible level.
 * 
 * 5     11/17/98 11:12a Dave
 * Removed player identification by address. Now assign explicit id #'s.
 * 
 * 4     11/05/98 5:55p Dave
 * Big pass at reducing #includes
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
 * 83    9/18/98 4:54p Dave
 * Fixed ingame ship select icon problem.
 * 
 * 82    9/18/98 4:23p Dave
 * Reversed the logic on a bogus assert.
 * 
 * 81    9/18/98 3:13p Allender
 * actually delete ships from wings when ingame joining.  Shoudl get over
 * problem were too many ships are presend in the mission since we don't
 * delete unused ships until after ingame join is completed
 * 
 * 80    9/15/98 1:59p Dave
 * Fixed bonehead mistake calling multi_endgame_ending()
 * 
 * 79    9/11/98 4:14p Dave
 * Fixed file checksumming of < file_size. Put in more verbose kicking and
 * PXO stats store reporting.
 * 
 * 78    9/11/98 2:10p Allender
 * base temporary parse_object's ai_goals to -1 so that we don't try and
 * free sepxressions that shouldn't be freed
 * 
 * 77    8/07/98 10:16a Allender
 * use obj_set_flags for the COULD_BE_PLAYER flag
 * 
 * 76    7/24/98 9:27a Dave
 * Tidied up endgame sequencing by removing several old flags and
 * standardizing _all_ endgame stuff with a single function call.
 * 
 * 75    6/30/98 2:17p Dave
 * Revised object update system. Removed updates for all weapons. Put
 * button info back into control info packet.
 * 
 * 74    6/13/98 6:01p Hoffoss
 * Externalized all new (or forgot to be added) strings to all the code.
 * 
 * 73    6/13/98 3:18p Hoffoss
 * NOX()ed out a bunch of strings that shouldn't be translated.
 * 
 * 72    6/10/98 2:56p Dave
 * Substantial changes to reduce bandwidth and latency problems.
 * 
 * 71    6/04/98 11:46a Dave
 * Drastically reduce size/rate of client control info update packets. Put
 * in rate limiting for object updating from server.
 * 
 * 70    5/26/98 11:54a Allender
 * fix multiplayer problems and sexpression crash
 * 
 * 69    5/25/98 1:33a Allender
 * fixed timestamp problem for client update packets, small fix for ingame
 * validation code
 * 
 * 68    5/24/98 9:17p Allender
 * commented out test code to help rectify bogus player objects when
 * ingame joining
 * 
 * 67    5/23/98 3:16p Allender
 * work on object update packet optimizations (a new updating system).
 * Don't allow medals/promotions/badges when playing singple player
 * missions through the simulator
 * 
 * 66    5/22/98 9:35p Dave
 * Put in channel based support for PXO. Put in "shutdown" button for
 * standalone. UI tweaks for TvT
 * 
 * 65    5/21/98 10:03p Allender
 * add secondary ammo counts to ingame join packets
 * 
 * 64    5/21/98 1:52a Dave
 * Remove obsolete command line functions. Reduce shield explosion packets
 * drastically. Tweak PXO screen even more. Fix file xfer system so that
 * we can guarantee file uniqueness.
 * 
 * 63    5/21/98 12:14a Allender
 * fix ingame join problems
 * 
 * 62    5/20/98 3:25p Allender
 * ingame join changes (which probably won't make the final version).
 * Added RAS code into psnet
 * 
 * 61    5/19/98 11:36p Allender
 * fixed very nasty mask problem with ingame joiner marking player objects
 * incorrectly.  Named ingame joiner ship and observer ship unique names
 * 
 * 60    5/19/98 8:35p Dave
 * Revamp PXO channel listing system. Send campaign goals/events to
 * clients for evaluation. Made lock button pressable on all screens. 
 * 
 * 59    5/18/98 12:41a Allender
 * fixed subsystem problems on clients (i.e. not reporting properly on
 * damage indicator).  Fixed ingame join problem with respawns.  minor
 * comm menu stuff
 * 
 * 58    5/15/98 4:12p Allender
 * removed redbook code.  Put back in ingame join timer.  Major fixups for
 * stats in multiplayer.  Pass correct score, medals, etc when leaving
 * game.  Be sure clients display medals, badges, etc.
 * 
 * 57    5/15/98 1:44p Allender
 * initialize hotkeys when entering the mission ingame joining
 * 
 * 56    5/11/98 4:33p Allender
 * fixed ingame join problems -- started to work on new object updating
 * code (currently ifdef'ed out)
 * 
 * 55    5/09/98 4:31p Chad
 * Fixed weapon recharge rate problem and fixed weapon link status problem
 * for ingame joiners.
 * 
 * 54    5/08/98 11:21a Allender
 * fix ingame join trouble.  Small messaging fix.  Enable collisions for
 * friendlies again
 * 
 * 53    5/03/98 7:04p Dave
 * Make team vs. team work mores smoothly with standalone. Change how host
 * interacts with standalone for picking missions. Put in a time limit for
 * ingame join ship select. Fix ingame join ship select screen for Vasudan
 * ship icons.
 * 
 * 52    5/03/98 2:52p Dave
 * Removed multiplayer furball mode.
 * 
 * 51    5/02/98 1:47a Dave
 * Make sure ingame joiners know how many respawns they have left. Tidy up
 * some multiui stuff.
 * 
 * 50    5/01/98 4:11p Comet
 * Fixed ship_ets bug. I think.
 * 
 * 49    4/30/98 12:49a Allender
 * change ship type and weapons of any player wing ship, not just ones
 * that players currently occupy
 * 
 * 48    4/29/98 9:36p Allender
 * ingame join tweaks.  added network message for countermeasures
 * 
 * 47    4/29/98 12:28p Chad
 * set packet size when selecting a player's ingame choice
 * 
 * 46    4/25/98 2:02p Dave
 * Put in multiplayer context help screens. Reworked ingame join ship
 * select screen. Fixed places where network timestamps get hosed.
 * 
 * 45    4/23/98 11:52p Allender
 * make homing weapons send their homing object.  Fixed ingame joiners so
 * they bash ship types and weapons correctly when joining
 * 
 * 44    4/23/98 1:49a Allender
 * major rearm/repair fixes for multiplayer.  Fixed respawning of AI ships
 * to not respawn until 5 seconds after they die.  Send escort information
 * to ingame joiners
 * 
 * 43    4/22/98 5:53p Dave
 * Large reworking of endgame sequencing. Updated multi host options
 * screen for new artwork. Put in checks for host or team captains leaving
 * midgame.
 * 
 * 42    4/22/98 4:59p Allender
 * new multiplayer dead popup.  big changes to the comm menu system for
 * team vs. team.  Start of debriefing stuff for team vs. team  Make form
 * on my wing work with individual ships who have high priority orders
 * 
 * 41    4/21/98 11:56p Dave
 * Put in player deaths statskeeping. Use arrow keys in the ingame join
 * ship select screen. Don't quit the game if in the debriefing and server
 * leaves.
 * 
 * 40    4/20/98 12:40a Allender
 * fixed nasty problem where network read code was not reentrant.  minor
 * UI tweaks.  ingame joiners now get netgame info correctly.
 * 
 * 39    4/07/98 5:42p Dave
 * Put in support for ui display of voice system status (recording,
 * playing back, etc). Make sure main hall music is stopped before
 * entering a multiplayer game via ingame join.
 * 
 * 38    4/06/98 6:47p Allender
 * be sure that ingame joiner has reasonable current primary and secondary
 * banks
 * 
 * 37    4/06/98 12:33a Allender
 * ingame joiners need to unmark all player ships as could be players
 * before getting ship information from host.  Don't send count with
 * countermeasure fired packet
 * 
 * 36    4/04/98 4:22p Dave
 * First rev of UDP reliable sockets is done. Seems to work well if not
 * overly burdened.
 * 
 * 35    4/04/98 3:55p Allender
 * made ingame join send packet to all other clients in the game when in
 * game joiner selects his ship
 * 
 * 34    4/03/98 1:03a Dave
 * First pass at unreliable guaranteed delivery packets.
 * 
 * 33    4/02/98 5:36p John
 * Made the level paging occur at the same time as the level is read in.
 * 
 * 32    3/31/98 4:03p Allender
 * some ingame join fixed -- make sure ingame joiners have parse objects
 * for all other players
 * 
 * 31    3/24/98 5:12p Allender
 * ingame join packet sequencing
 *
 * $NoKeywords: $
 */

#include <limits.h>

#include "ai.h"
#include "object.h"
#include "ship.h"
#include "multi.h"
#include "multiutil.h"
#include "multimsgs.h"
#include "multiui.h"
#include "missionparse.h"
#include "freespace.h"
#include "gamesequence.h"
#include "2d.h"
#include "ui.h"
#include "key.h"
#include "gamesnd.h"
#include "linklist.h"
#include "multi_ingame.h"
#include "missionscreencommon.h"
#include "popup.h"
#include "bmpman.h"
#include "mouse.h"
#include "multi_observer.h"
#include "multi_xfer.h"
#include "multi_kick.h"
#include "missiongoals.h"
#include "mainhallmenu.h"
#include "stats.h"
#include "multiteamselect.h"
#include "missionweaponchoice.h"
#include "multi_endgame.h"
#include "hudescort.h"
#include "hudshield.h"
#include "objcollide.h"
#include "missionhotkey.h"
#include "multi_campaign.h"
#include "multi_obj.h"
#include "alphacolors.h"
#include "timer.h"

extern void get_vector_data(ubyte *data, int *size, vector vec);
extern void add_vector_data(ubyte *data, int *size, vector vec);
// --------------------------------------------------------------------------------------------------
// DAVE's BIGASS INGAME JOIN WARNING/DISCLAIMER
//
// Ingame joining is another delicate system. Although not as delicate as server transfer, it will
// help to take as many precautions as possible when handling ingame joins. Please be sure to follow
// all the same rules as explained in multi_strans.h
//
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
// INGAME JOIN DESCRIPTION
//
// 1.) Joiner sends a JOIN packet to the server
// 2.) If the server accepts him, he receives an ACCEPT packet in return
// 3.) The client then moves into the INGAME_SYNC state to begin receiving data from the server
// 4.) The first thing he does on this screen is send his filesig packet to the server. At which 
//     point the server will either let him in or deny him. There are no file transfers ingame.
// 5.) The server calls multi_handle_ingame_joiners() once per frame, through multi_do_frame()
// 6.) After verifiying or kicking the player because of his file signature, the server tells the
//     player to load the mission
// 7.) When the mission is loaded, the server, sends a netgame update to the client
// 8.) Without waiting, the server then begins sending data ship packets to the player
// 9.) Upon confirmation of receiving these packets, the server sends wing data packets
// 10.) Upon completion of this, the server sends respawn point packets
// 11.) Upon completion of this, the server sends a post briefing data block packet containing ship class and 
//      weapon information
// 12.) After this, the server sends a player settings packet (to all players for good measure)
// 13.) At this point, the server sends a jump into mission packet
// 14.) Upon receipt of this packet, the client moves into the ingame ship select state
// 15.) The first thing the client does in this state is load the mission data (textures, etc)
// 16.) The player is presented with a list of ships he can choose from. He selects one and sends
//      an INGAME_SHIP_REQUEST to the server. 
// 17.) The server checks to see if this request is acceptable and sends an INGAME_SHIP_REQUEST back
//      with the appropriate data.
// 18.) If the client received an affirmative, he selects the ship and jumps into the mission, otherwise
//      he removes it from the list and tries for another ship
// --------------------------------------------------------------------------------------------------


LOCAL	int	Ingame_ships_deleted = 0;
//LOCAL	int	Ingame_ships_to_delete[MAX_SHIPS];	// no longer used


// --------------------------------------------------------------------------------------------------
// INGAME JOIN FORWARD DECLARATIONS
//

void multi_ingame_send_ship_update(net_player *p);

void multi_ingame_join_check_buttons();
void multi_ingame_join_button_pressed(int n);



// --------------------------------------------------------------------------------------------------
// INGAME JOIN COMMON DEFINITIONS
//


// --------------------------------------------------------------------------------------------------
// INGAME JOIN SERVER FUNCTIONS
//

// called on the server to process ingame joiners and move them through the motions of ingame joining
void multi_handle_ingame_joiners()
{
	int idx;

	Assert( MULTIPLAYER_MASTER );

	// if my ingame joining flag isn't set, then don't do anything.
	if ( !(Netgame.flags & NG_FLAG_INGAME_JOINING) ){
		return;
	}

	// traverse through all the players
	for(idx = 0; idx<MAX_PLAYERS; idx++){
		// only process ingame joiners
		if( !(Net_players[idx].flags & NETINFO_FLAG_RELIABLE_CONNECTED) || !(Net_players[idx].flags & NETINFO_FLAG_INGAME_JOIN)){
			continue;
		}
		
		// if we're waiting for players to receive files, then check on their status
		if(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_FILE_XFER){						
			switch(multi_xfer_get_status(Net_players[idx].s_info.xfer_handle)){
			// if it has successfully completed, set his ok flag
			case MULTI_XFER_SUCCESS :
				// set his ok flag
				Net_players[idx].flags |= NETINFO_FLAG_MISSION_OK;

				// release the xfer instance handle
				multi_xfer_release_handle(Net_players[idx].s_info.xfer_handle);
				Net_players[idx].s_info.xfer_handle = -1;
				break;
			// if it has failed or timed-out, kick the player
			case MULTI_XFER_TIMEDOUT:
			case MULTI_XFER_FAIL:
				// release the xfer handle
				multi_xfer_release_handle(Net_players[idx].s_info.xfer_handle);
				Net_players[idx].s_info.xfer_handle = -1;
						
				// kick the loser
				multi_kick_player(idx, 0, KICK_REASON_BAD_XFER);
				break;
			}						
		}		
		
		// if the player has verified his file signature then send him the packet to load the mission and mark this down
		if((Net_players[idx].flags & NETINFO_FLAG_MISSION_OK) && !(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_LOADING_MISSION)){
			// send the netgame update here as well
			send_netgame_update_packet(&Net_players[idx]);
			send_netplayer_update_packet(&Net_players[idx]);

			// send the packet and mark it down
			send_netplayer_load_packet(&Net_players[idx]);			
			Net_players[idx].s_info.ingame_join_flags |= INGAME_JOIN_FLAG_LOADING_MISSION;						
		}

		// once he has finished loading the mission, start sending him ship data packets and mark this down
		if((Net_players[idx].state == NETPLAYER_STATE_MISSION_LOADED) && !(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_SENDING_SHIPS)){
			int i;

			// send the packet and mark it down
			for (i = 0; i < Num_teams; i++ ) {
				if(Game_mode & GM_STANDALONE_SERVER){
					send_wss_slots_data_packet(i, 1, &Net_players[idx], 0);
				} else {
					send_wss_slots_data_packet(i, 1, &Net_players[idx]);
				}
			}

			// mark the netgame as a critical stage in ingame joining so that I don't evaluate mission event/
			// goals, ship arrivals, etc.
			Netgame.flags |= NG_FLAG_INGAME_JOINING_CRITICAL;

			// send the packet and mark it down
			send_ingame_ships_packet(&Net_players[idx]);
			Net_players[idx].s_info.ingame_join_flags |= INGAME_JOIN_FLAG_SENDING_SHIPS;
		}

		// once he has finished receiving the ship data, start sending him wing data and mark this down
		/*
		if((Net_players[idx].state == NETPLAYER_STATE_INGAME_SHIPS) && !(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_SENDING_WINGS)){
			// setup the list of wings to send
			Net_players[idx].s_info.wing_index = 0;
			Net_players[idx].s_info.wing_index_backup = 0;

			// send the packet and mark it down
			send_ingame_wings_packet(&Net_players[idx]);
			Net_players[idx].s_info.ingame_join_flags |= INGAME_JOIN_FLAG_SENDING_WINGS;
		}
		*/

		// once he has received the respawn packet, send him the player settings for all the players in the game and mark this down
		if((Net_players[idx].state == NETPLAYER_STATE_INGAME_SHIPS) && !(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_SENDING_POST)){
			// reset the critical ingame joining flag so that I as server, will start evaluating mission
			// things again
			Netgame.flags &= ~NG_FLAG_INGAME_JOINING_CRITICAL;

			// send the packet and mark it down
			if(Game_mode & GM_STANDALONE_SERVER){
				send_post_sync_data_packet(&Net_players[idx],0);
			} else {
				send_post_sync_data_packet(&Net_players[idx]);
			}
			Net_players[idx].s_info.ingame_join_flags |= INGAME_JOIN_FLAG_SENDING_POST;
		}				

		// once the settings have been received, send him the jump into mission packet and mark this down. now the joiner
		// moves into the ship select state (ingame)
		if((Net_players[idx].state == NETPLAYER_STATE_POST_DATA_ACK) && !(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_PICK_SHIP)){			
			// if this guy is an obsever, create his observer object and be done!
			if(Net_players[idx].flags & NETINFO_FLAG_OBSERVER){
				multi_obs_create_observer(&Net_players[idx]);
				Net_players[idx].flags &= ~(NETINFO_FLAG_INGAME_JOIN);
				Netgame.flags &= ~(NG_FLAG_INGAME_JOINING_CRITICAL | NG_FLAG_INGAME_JOINING);
			}
		
			// send the packet and mark it down
			send_jump_into_mission_packet(&Net_players[idx]);
			Net_players[idx].s_info.ingame_join_flags |= INGAME_JOIN_FLAG_PICK_SHIP;
		}

		// check to see if his timestamp for ship update (hull, shields, etc) has popped. If so, send some info and reset
		if(timestamp_elapsed(Net_players[idx].s_info.last_full_update_time)){
			// send the ships
			multi_ingame_send_ship_update(&Net_players[idx]);

			// reset the timestamp
			Net_players[idx].s_info.last_full_update_time = timestamp(INGAME_SHIP_UPDATE_TIME);
		}

		// once he has received the weapon state packet, send him the player settings for all the players in the game and mark this down
		if(!(Net_players[idx].s_info.ingame_join_flags & INGAME_JOIN_FLAG_SENDING_SETS)){			
			// send the packet and mark it down
			// this will update _ALL_ players in the game which is important
			send_player_settings_packet();
			send_player_settings_packet( &Net_players[idx] );		// send directly so he gets the packet
		
			Net_players[idx].s_info.ingame_join_flags |= INGAME_JOIN_FLAG_SENDING_SETS;
		}		
	}
}

// the final step for an ingame joining observer - create my observer object, unflag myself as joining and jump into mission
void multi_ingame_observer_finish()
{
	// create my local observer object
	multi_obs_create_observer_client();

	// unflag myself as being an ingame joiner
	Net_player->flags &= ~(NETINFO_FLAG_INGAME_JOIN);

	// set my state to be in-mission
	Net_player->state = NETPLAYER_STATE_IN_MISSION;
	send_netplayer_update_packet();

	// jump into the game
	gameseq_post_event(GS_EVENT_ENTER_GAME);
}

// --------------------------------------------------------------------------------------------------
// INGAME DATA SYNC SCREEN 
//

// mission sync screen init function for ingame joining
void multi_ingame_sync_init()
{	
	// if we couldn't get the file signature correctly. send some bogus values
	multi_get_mission_checksum(Game_current_mission_filename);
	
	// everyone should re-initialize these 
	init_multiplayer_stats();

	// reset all sequencing info
	multi_oo_reset_sequencing();

	// send the file signature to the host for possible mission file transfer
	strcpy(Netgame.mission_name,Game_current_mission_filename);
	send_file_sig_packet(Multi_current_file_checksum,Multi_current_file_length);
	
	Ingame_ships_deleted = 0;
}

// mission sync screen do function for ingame joining
void multi_ingame_sync_do()
{	
}

// mission sync screen do function for ingame joining
void multi_ingame_sync_close()
{
}


// --------------------------------------------------------------------------------------------------
// INGAME SHIP SELECT SCREEN 
//
static const char *Multi_ingame_join_bitmap_fname[GR_NUM_RESOLUTIONS] = {
	"MultiIngame",				// GR_640
	"2_MultiIngame"			// GR_1024
};

static const char *Multi_ingame_join_bitmap_mask_fname[GR_NUM_RESOLUTIONS] = {
	"MultiIngame-M",			// GR_640
	"2_MultiIngame-M"			// GR_1024
};


// button defs
#define MULTI_INGAME_JOIN_NUM_BUTTONS       2
#define MIJ_CANCEL		0
#define MIJ_JOIN			1

ui_button_info Multi_ingame_join_buttons[GR_NUM_RESOLUTIONS][MULTI_INGAME_JOIN_NUM_BUTTONS] = {
	{ // GR_640
		ui_button_info( "MIB_00",	532,	434,	510,	413,	0 ),						// cancel
		ui_button_info( "MIB_01",	572,	428,	585,	413,	1 ),						// join
	},
	{ // GR_1024
		ui_button_info( "2_MIB_00",	851,	695,	916,	685,	0 ),						// cancel
		ui_button_info( "2_MIB_01",	916,	685,	950,	665,	1 ),						// join
	}
};

#define MULTI_INGAME_JOIN_NUM_TEXT			8

UI_XSTR Multi_ingame_join_text[GR_NUM_RESOLUTIONS][MULTI_INGAME_JOIN_NUM_TEXT] = {
	{ // GR_640		
		{"Cancel",							387,	510,	413,	UI_XSTR_COLOR_PINK, -1, &Multi_ingame_join_buttons[GR_640][MIJ_CANCEL].button},	
		{"Join",								1303,	585,	413,	UI_XSTR_COLOR_PINK, -1, &Multi_ingame_join_buttons[GR_640][MIJ_JOIN].button},
		{"Select Ship",					317,	39,	6,		UI_XSTR_COLOR_PINK, -1, NULL},
		{"name",								1423,	39,	28,	UI_XSTR_COLOR_GREEN, -1, NULL},
		{"class",							1424,	145,	28,	UI_XSTR_COLOR_GREEN,	-1, NULL},
		{"status",							1425,	214,	28,	UI_XSTR_COLOR_GREEN,	-1, NULL},
		{"primary",							1426,	295,	28,	UI_XSTR_COLOR_GREEN, -1, NULL},
		{"secondary",						1427,	440,	28,	UI_XSTR_COLOR_GREEN, -1, NULL}
	},
	{ // GR_1024		
		{"Cancel",							387,	843,	665,	UI_XSTR_COLOR_PINK, -1, &Multi_ingame_join_buttons[GR_1024][MIJ_CANCEL].button},	
		{"Join",								1303,	950,	665,	UI_XSTR_COLOR_PINK, -1, &Multi_ingame_join_buttons[GR_1024][MIJ_JOIN].button},
		{"Select Ship",					317,	63,	14,	UI_XSTR_COLOR_PINK, -1, NULL},
		{"name",								1423,	63,	45,	UI_XSTR_COLOR_GREEN, -1, NULL},
		{"class",							1424,	233,	45,	UI_XSTR_COLOR_GREEN,	-1, NULL},
		{"status",							1425,	343,	45,	UI_XSTR_COLOR_GREEN,	-1, NULL},
		{"primary",							1426,	473,	45,	UI_XSTR_COLOR_GREEN, -1, NULL},
		{"secondary",						1427,	704,	45,	UI_XSTR_COLOR_GREEN, -1, NULL}
	}
};

#define MI_FIELD_X		0
#define MI_FIELD_Y		1
#define MI_FIELD_W		2
#define MI_FIELD_H		3

static int Mi_width[GR_NUM_RESOLUTIONS] = { 
	569,		// GR_640
	910		// GR_1024
};

static int Mi_height[GR_NUM_RESOLUTIONS] = {
	339,		// GR_640
	542		// GR_1024
};

static int Mi_spacing[GR_NUM_RESOLUTIONS] = {
	30,
	48
};

static int Mi_name_field[GR_NUM_RESOLUTIONS][4] = {
	// GR_640
	{
		33,			// x
		49,			// y
		100,			// width
		339			// height
	},
	// GR_1024
	{
		53,			// x
		78,			// y
		160,			// width
		542			// height
	}
};

static int Mi_class_field[GR_NUM_RESOLUTIONS][4] = {
	// GR_640
	{
		140,			// x
		49,			// y
		59,			// width
		339			// height
	},
	// GR_1024
	{
		224,			// x
		78,			// y
		94,			// width
		542			// height
	}
};

static int Mi_status_field[GR_NUM_RESOLUTIONS][4] = {
	// GR_640
	{
		209,			// x
		49,			// y
		69,			// width
		339			// height
	},
	// GR_1024
	{
		334,			// x
		78,			// y
		110,			// width
		542			// height
	}
};

static int Mi_primary_field[GR_NUM_RESOLUTIONS][4] = {
	// GR_640
	{
		287,			// x
		49,			// y
		145,			// width
		339			// height
	},
	// GR_1024
	{
		459,			// x
		78,			// y
		232,			// width
		542			// height
	}
};

static int Mi_secondary_field[GR_NUM_RESOLUTIONS][4] = {
	// GR_640
	{
		441,			// x
		49,			// y
		145,			// width
		339			// height
	},
	// GR_1024
	{
		706,			// x
		78,			// y
		232,			// width
		542			// height
	}
};

// for timing a player out
static int Multi_ingame_timer_coords[GR_NUM_RESOLUTIONS][2] = {
	{
		// GR_640
		26,
		411
	},
	{
		// GR_1024
		42,
		658
	}
};

//#define MULTI_INGAME_TIME_LEFT_X			26
//#define MULTI_INGAME_TIME_LEFT_Y			411

#define MULTI_INGAME_TIME_SECONDS		(1000 * 15)
LOCAL int Ingame_time_left;

// uses MULTI_JOIN_REFRESH_TIME as its timestamp
UI_WINDOW Multi_ingame_window;											// the window object for the join screen
UI_BUTTON Multi_ingame_select_button;									// for selecting list items
int Multi_ingame_bitmap;													// the background bitmap

// ship class icons
#define MULTI_INGAME_MAX_SHIP_ICONS			40
typedef struct is_icon {
	int bmaps[NUM_ICON_FRAMES];
	int ship_class;
} is_icon;
is_icon Multi_ingame_ship_icon[MULTI_INGAME_MAX_SHIP_ICONS];
int Multi_ingame_num_ship_icons;

// # of available ships (also == the # currently being displayed)
int Multi_ingame_num_avail;

// signatures for each of the available ships
ushort Multi_ingame_ship_sigs[MAX_PLAYERS];

// net signature of the ship we've requested to grab as an ingame joiner (waiting for server response if >= 0)
ushort Multi_ingame_join_sig;

// the index into the list of the ship currently selected
int Multi_ingame_ship_selected;

// temporary stuff - used only until we come up with a more permanent interface for this screen
#define MAX_INGAME_SHIPS 50
#define INGAME_FINAL_TIMEOUT 4000

ushort Ingame_ship_signatures[MAX_INGAME_SHIPS];

//XSTR:ON

// local variables to hold ship/obj/ai information for the joiner.  We need to
// create a bogus ship so that packets that the joiner receives during his join
// have valid Player_ship, Player_obj, and Player_ai to work with
int Ingame_shipnum;

// display the available ships (OF_COULD_BE_PLAYER flagged)
void multi_ingame_join_display_avail();

// try and scroll the selected ship up
void multi_ingame_scroll_select_up();

// try and scroll the selected ship down
void multi_ingame_scroll_select_down();

// handle all timeout details
void multi_ingame_handle_timeout();

int multi_ingame_get_ship_class_icon(int ship_class)
{
	int idx;

	// lookup through all available ship icons
	for(idx=0;idx<Multi_ingame_num_ship_icons;idx++){
		if(Multi_ingame_ship_icon[idx].ship_class == ship_class){
			return idx;
		}
	}

	// couldn't find it 
	return -1;
}

void multi_ingame_load_icons()
{
	int first_frame, num_frames, idx, s_idx;

	// zero out the icon handles	
	for(idx=0;idx<MULTI_INGAME_MAX_SHIP_ICONS;idx++){
		Multi_ingame_ship_icon[idx].ship_class = -1;		
		for(s_idx=0;s_idx<NUM_ICON_FRAMES;s_idx++){
			Multi_ingame_ship_icon[idx].bmaps[s_idx] = -1;		
		}
	}		
	Multi_ingame_num_ship_icons = 0;

	// traverse through all ship types
	for(idx=0;idx<MAX_SHIP_TYPES;idx++){
		// if there is a valid icon for this ship
		if((strlen(Ship_info[idx].icon_filename) > 0) && (Multi_ingame_num_ship_icons < MULTI_INGAME_MAX_SHIP_ICONS)){
			// set the ship class
			Multi_ingame_ship_icon[Multi_ingame_num_ship_icons].ship_class = idx;

			// load in the animation frames for the icon	
			first_frame = bm_load_animation(Ship_info[idx].icon_filename, &num_frames);
			if ( first_frame == -1 ) {
				Int3();	// Could not load in icon frames.. get Dave
			}	
			for ( s_idx = 0; s_idx < num_frames; s_idx++ ) {
				Multi_ingame_ship_icon[Multi_ingame_num_ship_icons].bmaps[s_idx] = first_frame+s_idx;
			}

			Multi_ingame_num_ship_icons++;
		}
	}
}

void multi_ingame_unload_icons()
{
	int idx,s_idx;

	// unload all the bitmaps
	for(idx=0;idx<Multi_ingame_num_ship_icons;idx++){
		for(s_idx=0;s_idx<NUM_ICON_FRAMES;s_idx++){
			if(Multi_ingame_ship_icon[idx].bmaps[s_idx] != -1){
				bm_release(Multi_ingame_ship_icon[idx].bmaps[s_idx]);
				Multi_ingame_ship_icon[idx].bmaps[s_idx] = -1;
			}
		}
	}
}

// ingame join ship selection screen init
void multi_ingame_select_init()
{
	/// int objnum, wingnum_save,idx, goals_save;
	// ushort net_signature;
	int idx;

	Multi_ingame_join_sig = 0;
	Net_player->player->objnum = -1;

	// create a ship, then find a ship to copy crucial information from.  Save and restore the wing
	// number to be safe.
	/*
	wingnum_save = Player_start_pobject.wingnum;
	net_signature = Player_start_pobject.net_signature;
	goals_save = Player_start_pobject.ai_goals;
	Player_start_pobject.wingnum = -1;
	Player_start_pobject.net_signature = 0;
	Player_start_pobject.ai_goals = -1;
	objnum = parse_create_object( &Player_start_pobject );
	Player_start_pobject.wingnum = wingnum_save;
	Player_start_pobject.net_signature = net_signature;
	Player_start_pobject.ai_goals = goals_save;

	if ( objnum == -1 ) {
		nprintf(("Network", "Bailing ingame join because unable to create parse object player ship\n"));
		multi_quit_game(PROMPT_NONE, MULTI_END_NOTIFY_NONE, MULTI_END_ERROR_INGAME_SHIP);
		return;
	}

	// make it invalid
	Player_obj = &Objects[objnum];
	Player_obj->net_signature = 0;						
	Player_ship = &Ships[Player_obj->instance];
	strcpy(Player_ship->ship_name, NOX("JIP Ship"));
	Player_ai = &Ai_info[Player_ship->ai_index];
	*/

	// load the temp ship icons
	multi_ingame_load_icons();

	// blast all the ingame ship signatures
	memset(Multi_ingame_ship_sigs,0,sizeof(ushort) * MAX_PLAYERS);

	// the index into the list of the ship currently selected
	Multi_ingame_ship_selected = -1;

	// initialize the time he has left to select a ship
	Ingame_time_left = timestamp(MULTI_INGAME_TIME_SECONDS);

	// initialize GUI data	

	// create the interface window
	Multi_ingame_window.create(0,0,gr_screen.max_w,gr_screen.max_h,0);
	Multi_ingame_window.set_mask_bmap(Multi_ingame_join_bitmap_mask_fname[gr_screen.res]);

	// load the background bitmap
	Multi_ingame_bitmap = bm_load(Multi_ingame_join_bitmap_fname[gr_screen.res]);
	if(Multi_ingame_bitmap < 0)
		Error(LOCATION, "Couldn't load background bitmap for ingame join");	
	
	// create the interface buttons
	for(idx=0; idx<MULTI_INGAME_JOIN_NUM_BUTTONS; idx++) {
		// create the object
		Multi_ingame_join_buttons[gr_screen.res][idx].button.create(&Multi_ingame_window, "", Multi_ingame_join_buttons[gr_screen.res][idx].x, Multi_ingame_join_buttons[gr_screen.res][idx].y, 1, 1, 0, 1);

		// set the sound to play when highlighted
		Multi_ingame_join_buttons[gr_screen.res][idx].button.set_highlight_action(common_play_highlight_sound);

		// set the ani for the button
		Multi_ingame_join_buttons[gr_screen.res][idx].button.set_bmaps(Multi_ingame_join_buttons[gr_screen.res][idx].filename);

		// set the hotspot
		Multi_ingame_join_buttons[gr_screen.res][idx].button.link_hotspot(Multi_ingame_join_buttons[gr_screen.res][idx].hotspot);
	}	
	
	// create all xstrs
	for(idx=0; idx<MULTI_INGAME_JOIN_NUM_TEXT; idx++) {
		Multi_ingame_window.add_XSTR(&Multi_ingame_join_text[gr_screen.res][idx]);
	}

	// create the list item select button
	Multi_ingame_select_button.create(&Multi_ingame_window, "", Mi_name_field[gr_screen.res][MI_FIELD_X], Mi_name_field[gr_screen.res][MI_FIELD_Y], Mi_width[gr_screen.res], Mi_height[gr_screen.res], 0, 1);
	Multi_ingame_select_button.hide();			

	// load freespace stuff
	// JAS: Code to do paging used to be here.
}

// process all ship list related details
void multi_ingame_ship_list_process()
{
	int select_index,y;

	// if we currently don't have any ships selected, but we've got items on the list, select the first one
	if((Multi_ingame_ship_selected == -1) && (Multi_ingame_num_avail > 0)){
		gamesnd_play_iface(SND_USER_SELECT);
		Multi_ingame_ship_selected = 0;
	}

	// if we currently have a ship selected, but it disappears, select the next ship (is possible0
	if((Multi_ingame_ship_selected >= 0) && (Multi_ingame_ship_selected >= Multi_ingame_num_avail)){
		gamesnd_play_iface(SND_USER_SELECT);
		Multi_ingame_ship_selected = Multi_ingame_num_avail-1;
	}
	
	// if the player clicked on the select button, see if the selection has changed
	if(Multi_ingame_select_button.pressed()){
		Multi_ingame_select_button.get_mouse_pos(NULL,&y);
		select_index = y / Mi_spacing[gr_screen.res];

		// if we've selected a valid item
		if((select_index >= 0) && (select_index < Multi_ingame_num_avail)){
			// if we're not selected the same item, play a sound
			if(Multi_ingame_ship_selected != select_index){
				gamesnd_play_iface(SND_USER_SELECT);
			}

			// select the item
			Multi_ingame_ship_selected = select_index;
		}
	}
}


// determines if a button was pressed, and acts accordingly
void multi_ingame_join_check_buttons()
{
	int idx;
	for(idx=0; idx<MULTI_INGAME_JOIN_NUM_BUTTONS; idx++) {
		// we only really need to check for one button pressed at a time,
		// so we can break after finding one.
		if(Multi_ingame_join_buttons[gr_screen.res][idx].button.pressed()) {
			multi_ingame_join_button_pressed(idx);
			break;
		}
	}
}

// a button was pressed, so make it do its thing
// this is the "acting accordingly" part
void multi_ingame_join_button_pressed(int n)
{
	switch(n) {
	case MIJ_CANCEL:
		multi_quit_game(PROMPT_CLIENT);
		break;
	case MIJ_JOIN:
		// don't do further processing if the game is paused
		if ( Netgame.game_state == NETGAME_STATE_PAUSED )
			return;

		if(Multi_ingame_join_sig == 0) {
			// if he has a valid ship selected
			if(Multi_ingame_ship_selected >= 0) {
				gamesnd_play_iface(SND_USER_SELECT);
			
				// select the sig of this ship and send a request for it
				Multi_ingame_join_sig = Multi_ingame_ship_sigs[Multi_ingame_ship_selected];
				
				// send a request to the
				send_ingame_ship_request_packet(INGAME_SR_REQUEST,Multi_ingame_join_sig);
			} else {
				gamesnd_play_iface(SND_GENERAL_FAIL);
			}
		} else {
			gamesnd_play_iface(SND_GENERAL_FAIL);
		}

		break;
	default:
		// how the hell did this happen?
		Int3();
	}
}


// ingame join ship selection screen do
void multi_ingame_select_do()
{	
	int k = Multi_ingame_window.process();

	// process any keypresses
	switch(k){
	case KEY_ESC :
		multi_quit_game(PROMPT_CLIENT);		
		break;

	case KEY_UP:
		multi_ingame_scroll_select_up();
		break;

	case KEY_DOWN:
		multi_ingame_scroll_select_down();
		break;
	}	

	// process button presses
	// multi_ingame_process_buttons();
	multi_ingame_join_check_buttons();
	
	// process any ship list related events
	multi_ingame_ship_list_process();	
	
	// draw the background, etc
	gr_reset_clip();	
	GR_MAYBE_CLEAR_RES(Multi_ingame_bitmap);
	if(Multi_ingame_bitmap != -1){
		gr_set_bitmap(Multi_ingame_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(0,0);
	}
	Multi_ingame_window.draw();

	// handle all timeout details. blitting, etc
	multi_ingame_handle_timeout();

	// display the available ships
	multi_ingame_join_display_avail();		

	// flip the buffer
	gr_flip();	
}

// ingame join ship select close
void multi_ingame_select_close()
{	
	// unload any bitmaps
	if(!bm_unload(Multi_ingame_bitmap)){
		nprintf(("General","WARNING : could not unload background bitmap %s\n",Multi_ingame_join_bitmap_fname[gr_screen.res]));
	}	

	// unload all the ship class icons
	multi_ingame_unload_icons();
	
	// destroy the UI_WINDOW
	Multi_ingame_window.destroy();	

	// stop main hall music
	main_hall_stop_music();	
}

// display an individual ships information, starting at the indicated y pixel value
void multi_ingame_join_display_ship(object *objp,int y_start)
{
	int icon_num,idx;
	ship_info *sip;
	int y_spacing;
	ship_weapon *wp;

	sip = &Ship_info[Ships[objp->instance].ship_info_index];
	
	// blit the ship name itself
	gr_set_color_fast(&Color_normal);
	gr_string(Mi_name_field[gr_screen.res][MI_FIELD_X],y_start+10, Ships[objp->instance].ship_name);
	
	// blit the ship class icon
	icon_num = multi_ingame_get_ship_class_icon(Ships[objp->instance].ship_info_index);
	if(icon_num != -1){
		gr_set_bitmap(Multi_ingame_ship_icon[icon_num].bmaps[0], GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap(Mi_class_field[gr_screen.res][MI_FIELD_X] + 15, y_start);
	}
	
	gr_set_color_fast(&Color_bright);
	wp = &Ships[objp->instance].weapons;
	
	// blit the ship's primary weapons	
	y_spacing = (Mi_spacing[gr_screen.res] - (wp->num_primary_banks * 10)) / 2;
	for(idx=0;idx<wp->num_primary_banks;idx++){
		gr_string(Mi_primary_field[gr_screen.res][MI_FIELD_X], y_start + y_spacing + (idx * 10), Weapon_info[wp->primary_bank_weapons[idx]].name);
	}

	// blit the ship's secondary weapons	
	y_spacing = (Mi_spacing[gr_screen.res] - (wp->num_secondary_banks * 10)) / 2;
	for(idx=0;idx<wp->num_secondary_banks;idx++){
		gr_string(Mi_secondary_field[gr_screen.res][MI_FIELD_X], y_start + y_spacing + (idx * 10), Weapon_info[wp->secondary_bank_weapons[idx]].name);
	}	

	// blit the shield/hull integrity
	hud_shield_show_mini(objp, Mi_status_field[gr_screen.res][MI_FIELD_X] + 15, y_start + 3,5,7);
}

// display the available ships (OF_COULD_BE_PLAYER flagged)
void multi_ingame_join_display_avail()
{		
	ship_obj *moveup;	

	// recalculate this # every frame
	Multi_ingame_num_avail = 0;	

	// display a background highlight rectangle for any selected lines
	if(Multi_ingame_ship_selected != -1){		
		int y_start = (Mi_name_field[gr_screen.res][MI_FIELD_Y] + (Multi_ingame_ship_selected * Mi_spacing[gr_screen.res]));		

		// draw the border
		gr_set_color_fast(&Color_bright_blue);
		gr_line(Mi_name_field[gr_screen.res][MI_FIELD_X]-1,y_start-1, (Mi_name_field[gr_screen.res][MI_FIELD_X]-1) + (Mi_width[gr_screen.res]+2),y_start-1);
		gr_line(Mi_name_field[gr_screen.res][MI_FIELD_X]-1,y_start + Mi_spacing[gr_screen.res] - 2, (Mi_name_field[gr_screen.res][MI_FIELD_X]-1) + (Mi_width[gr_screen.res]+2),y_start + Mi_spacing[gr_screen.res] - 2);
		gr_line(Mi_name_field[gr_screen.res][MI_FIELD_X]-1,y_start, Mi_name_field[gr_screen.res][MI_FIELD_X]-1, y_start + Mi_spacing[gr_screen.res] - 2);
		gr_line((Mi_name_field[gr_screen.res][MI_FIELD_X]-1) + (Mi_width[gr_screen.res]+2), y_start,(Mi_name_field[gr_screen.res][MI_FIELD_X]-1) + (Mi_width[gr_screen.res]+2),y_start + Mi_spacing[gr_screen.res] - 2);
	}

	moveup = GET_FIRST(&Ship_obj_list);	
	while(moveup != END_OF_LIST(&Ship_obj_list)){
		if( !(Ships[Objects[moveup->objnum].instance].flags & (SF_DYING|SF_DEPARTING)) && (Objects[moveup->objnum].flags & OF_COULD_BE_PLAYER) ) {
			// display the ship
			multi_ingame_join_display_ship(&Objects[moveup->objnum],Mi_name_field[gr_screen.res][MI_FIELD_Y] + (Multi_ingame_num_avail * Mi_spacing[gr_screen.res]));

			// set the ship signature
			Multi_ingame_ship_sigs[Multi_ingame_num_avail] = Objects[moveup->objnum].net_signature;
			
			// inc the # available
			Multi_ingame_num_avail++;
		}
		moveup = GET_NEXT(moveup);
	}		
}

// try and scroll the selected ship up
void multi_ingame_scroll_select_up()
{
	if(Multi_ingame_ship_selected > 0){
		gamesnd_play_iface(SND_USER_SELECT);
		Multi_ingame_ship_selected--;
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}	
}

// try and scroll the selected ship down
void multi_ingame_scroll_select_down()
{
	if(Multi_ingame_ship_selected < (Multi_ingame_num_avail - 1)){
		gamesnd_play_iface(SND_USER_SELECT);
		Multi_ingame_ship_selected++;
	} else {
		gamesnd_play_iface(SND_GENERAL_FAIL);
	}
}

// handle all timeout details
void multi_ingame_handle_timeout()
{
	/*
	// uncomment this block to disable the timer
	gr_set_color_fast(&Color_bright_red);
	gr_string(Multi_ingame_timer_coords[gr_screen.res][0], Multi_ingame_timer_coords[gr_screen.res][1], "Timer disabled!!");
	return;
	*/

	// if we've timed out, leave the game
	if( timestamp_elapsed(Ingame_time_left) ) {
		multi_quit_game(PROMPT_NONE, MULTI_END_NOTIFY_INGAME_TIMEOUT, MULTI_END_ERROR_NONE);
		return;
	}

	// otherwise, blit how much time we have left
	int time_left = timestamp_until(Ingame_time_left) / 1000;
	char tl_string[100];
	gr_set_color_fast(&Color_bright);
	memset(tl_string,0,100);
	sprintf(tl_string,XSTR("Time remaining : %d s\n",682),time_left);	
	gr_string(Multi_ingame_timer_coords[gr_screen.res][0], Multi_ingame_timer_coords[gr_screen.res][1], tl_string);
}


// --------------------------------------------------------------------------------------------------
// PACKET HANDLER functions
// these are also defined in multimsgs.h, but the implementations are in the module for the sake of convenience
//

#define INGAME_PACKET_SLOP		75				// slop value used for packets to ingame joiner

void process_ingame_ships_packet( ubyte *data, header *hinfo )
{
	int offset, sflags, oflags, team, j;
	ubyte p_type;
	ushort net_signature;	
	short wing_data;	
	int team_val, slot_index, idx;
	char ship_name[255] = "";
	object *objp;
	int net_sig_modify;

	// go through the ship obj list and delete everything. YEAH
	if(!Ingame_ships_deleted){
		int idx;

		// no player object
		Player_obj = NULL;
		Player_ship = NULL;
		Player_ai = NULL;

		// delete all ships
		for(idx=0; idx<MAX_SHIPS; idx++){
			if((Ships[idx].objnum >= 0) && (Ships[idx].objnum < MAX_OBJECTS)){
				obj_delete(Ships[idx].objnum);
			}
		}

		Ingame_ships_deleted = 1;
	}

	offset = HEADER_LENGTH;

	// go
	GET_DATA( p_type );	
	while ( p_type == INGAME_SHIP_NEXT ) {
		p_object *p_objp;
		int ship_num, objnum;

		GET_STRING( ship_name );
		GET_USHORT( net_signature );
		GET_INT( sflags );
		GET_INT( oflags );
		GET_INT( team );		
		GET_SHORT( wing_data );
		net_sig_modify = 0;
		if(wing_data >= 0){
			GET_UINT(Wings[wing_data].current_wave);			
			net_sig_modify = Wings[wing_data].current_wave - 1;
		}

		// lookup ship in the original ships array
		p_objp = mission_parse_get_original_ship(net_signature);
		if(p_objp == NULL){
			// if this ship is part of wing not on its current wave, look for its "original" by subtracting out wave #
			p_objp = mission_parse_get_arrival_ship((ushort)(net_signature - (ushort)net_sig_modify));
		}
		if(p_objp == NULL){
			Int3();
			nprintf(("Network", "Couldn't find ship %s in either arrival list or in mission"));
			multi_quit_game(PROMPT_NONE, MULTI_END_NOTIFY_NONE, MULTI_END_ERROR_INGAME_BOGUS);
			return;
		}

		// go ahead and create the parse object.  Set the network signature of this guy before
		// creation
		// multi_set_network_signature( net_signature, MULTI_SIG_SHIP );
		objnum = parse_create_object( p_objp );
		ship_num = Objects[objnum].instance;
		Objects[objnum].flags = oflags;
		Objects[objnum].net_signature = net_signature;

		// assign any common data
		strcpy(Ships[ship_num].ship_name, ship_name);
		Ships[ship_num].flags = sflags;
		Ships[ship_num].team = team;
		Ships[ship_num].wingnum = (int)wing_data;				

		GET_DATA( p_type );
	}

	PACKET_SET_SIZE();

	// if we have reached the end of the list and change our network state
	if ( p_type == INGAME_SHIP_LIST_EOL ) {		
		// merge all created list
		obj_merge_created_list();

		// fixup player ship stuff
		for(idx=0; idx<MAX_SHIPS; idx++){
			if(Ships[idx].objnum < 0){	
				continue;
			}

			// get the team and slot.  Team will be -1 when it isn't a part of player wing.  So, if
			// not -1, then be sure we have a valid slot, then change the ship type, etc.
			objp = &Objects[Ships[idx].objnum];		
			multi_ts_get_team_and_slot(Ships[idx].ship_name, &team_val, &slot_index);
			if ( team_val != -1 ) {
				Assert( slot_index != -1 );

				// change the ship type and the weapons
				change_ship_type(objp->instance, Wss_slots_teams[team_val][slot_index].ship_class);
				wl_bash_ship_weapons(&Ships[idx].weapons, &Wss_slots_teams[team_val][slot_index]);
	
				// Be sure to mark this ship as as a could_be_player
				obj_set_flags( objp, objp->flags | OF_COULD_BE_PLAYER );
				objp->flags &= ~OF_PLAYER_SHIP;
			}

			// if this is a player ship, make sure we find out who's it is and set their objnum accordingly
			if(team_val != -1){
				for( j = 0; j < MAX_PLAYERS; j++){
					if(MULTI_CONNECTED(Net_players[j]) && (Net_players[j].player->objnum == Objects[Ships[idx].objnum].net_signature)) {						
						// nprintf(("Network", "Making %s ship for %s\n", Ships[shipnum].ship_name, Net_players[j].player->callsign));
						multi_assign_player_ship( j, objp, Ships[idx].ship_info_index );
						objp->flags |= OF_PLAYER_SHIP;
						objp->flags &= ~OF_COULD_BE_PLAYER;
						break;
					}
				}
			}
		}

		// notify the server that we're all good.
		Net_player->state = NETPLAYER_STATE_INGAME_SHIPS;
		send_netplayer_update_packet();

		// add some mission sync text
		multi_common_add_text(XSTR("Ships packet ack (ingame)\n",683));
	}
}

void send_ingame_ships_packet(net_player *player)
{
	ubyte data[MAX_PACKET_SIZE];
	ubyte p_type;
	ship_obj *so;
	int packet_size;
	short wing_data;

	BUILD_HEADER( SHIPS_INGAME_PACKET );

	// essentially, we are going to send a list of ship names to the joiner for ships that are not
	// in wings.  The joiner will take the list, create any ships which should be created, and delete all
	// other ships after the list is sent.
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		ship *shipp;

		shipp = &Ships[Objects[so->objnum].instance];

		// skip all wings.
		// if ( shipp->wingnum != -1 ){
			// continue;
		// }

		if ( Objects[so->objnum].net_signature == STANDALONE_SHIP_SIG ){
			continue;
		}

		//  add the ship name and other information such as net signature, ship and object(?) flags.
		p_type = INGAME_SHIP_NEXT;
		ADD_DATA( p_type );
		ADD_STRING( shipp->ship_name );
		ADD_USHORT( Objects[so->objnum].net_signature );
		ADD_UINT( shipp->flags );
		ADD_UINT( Objects[so->objnum].flags );
		ADD_INT( shipp->team );
		wing_data = (short)shipp->wingnum;
		ADD_SHORT(wing_data);
		if(wing_data >= 0){
			ADD_UINT(Wings[wing_data].current_wave);
		}

		// don't send anymore data if we are getting close to the maximum size of this packet.  Send it off and
		// keep going
		if ( packet_size > (MAX_PACKET_SIZE - INGAME_PACKET_SLOP) ) {
			p_type = INGAME_SHIP_LIST_EOP;
			ADD_DATA( p_type );			
			multi_io_send_reliable(player, data, packet_size);
			BUILD_HEADER( SHIPS_INGAME_PACKET );
		}
	}

	// end of the ship list!!!
	p_type = INGAME_SHIP_LIST_EOL;
	ADD_DATA( p_type );	
	multi_io_send_reliable(player, data, packet_size);
}

void process_ingame_wings_packet( ubyte *data, header *hinfo )
{
	Int3();
}
/*
// code to process the wing data from a server.
void process_ingame_wings_packet( ubyte *data, header *hinfo )
{
	int offset, wingnum;
	ubyte p_type, what;

	offset = HEADER_LENGTH;

	GET_DATA( p_type );

	// p_type tells us whether to stop or not
	while ( p_type == INGAME_WING_NEXT ) {
		wing *wingp;

		// get the wingnum and a pointer to it.  The game stores data for all wings always, so this
		// is perfectly valid
		GET_DATA( wingnum );
		wingp = &Wings[wingnum];

		GET_DATA( what );
		if ( what == INGAME_WING_NOT_ARRIVED ) {
			Assert( wingp->total_arrived_count == 0 );			// this had better be true!!!
		} else if ( what == INGAME_WING_DEPARTED ) {
			// mark the wing as gone.  if it isn't, it soon will be.  Maybe we should send more information
			// about these wings later (like total_arrived_count, etc), but we will see.
			wingp->flags |= WF_WING_GONE;
		} else {
			int total_arrived_count, current_count, current_wave, i, j;
			ushort signature;
			int shipnum;

			// the wing is present in the mission on the server.  Get the crucial information about the
			// wing.  Then get the ships for this wing in order on the client machine
			GET_DATA( total_arrived_count );
			GET_DATA( current_count );
			GET_DATA( current_wave );

			Assert( current_wave > 0 );
			Assert( total_arrived_count > 0 );

			// for this wing, strip it down to nothing.  Let the parse object ocde recreate the
			// wing from the parse objects, then bash any weapons, etc for player wings.  We need
			// to do this because we might actually wind up with > MAX_SHIPS_PER_WING if we
			// don't delete them all first, and have a > 0 threshold, and are on something other
			// than the first wave.  Only do this for non-player wings.

			nprintf(("Network", "Clearing %s -- %d ships\n", wingp->name, wingp->current_count));
			for ( i = 0; i < wingp->current_count; i++ ) {
				int index, objnum;

				index = wingp->ship_index[i];
				Assert( index != -1 );
				objnum = Ships[index].objnum;
				Assert( objnum != -1 );

				// delete the object since we are filling the wing again anyway.
				obj_delete( objnum );
				Objects[objnum].net_signature = 0;				// makes this object "invalid" until dead.
				if ( Objects[objnum].type == OBJ_GHOST ) {
					nprintf(("Network", "Marking ghost objnum %d as dead\n", objnum));
					Objects[objnum].flags |= OF_SHOULD_BE_DEAD;
				}
				Ingame_ships_to_delete[index] = 0;		// be sure that this guy doesn't get deleted, since we already deleted it
				wingp->ship_index[i] = -1;
			}
			wingp->current_count = 0;
			wingp->total_arrived_count = 0;

			// now, recreate all the ships needed
			for (i = 0; i < current_count; i++ ) {
				int which_one, team, slot_index, specific_instance;;
				ship *shipp;
				object *objp;

				GET_DATA( signature );

				// assign which_one to be the given signature - wing's base signature.  This let's us
				// know which ship to create (i.e. the total_arrivel_count);
				which_one = signature - wingp->net_signature;
				Assert( (which_one >= 0) && (which_one < (wingp->net_signature + (wingp->wave_count*wingp->num_waves))) );
				wingp->total_arrived_count = (ushort)which_one;

				// determine which ship in the ahip arrival list this guy is.  It is a 0 based index
				specific_instance = which_one % wingp->wave_count;

				// call parse_wing_create_ships making sure that we only ever create 1 ship at a time.  We don't
				// want parse_wing_create_ships() to assign network signature either.  We will directly
				// assign it here.

				wingp->current_wave = 0;						// make it the first wave.  Ensures that ships don't get removed off the list
				parse_wing_create_ships( wingp, 1, 1, specific_instance );
				shipnum = wingp->ship_index[wingp->current_count-1];
				Ingame_ships_to_delete[shipnum] = 0;			// "unmark" this ship so it doesn't get deleted.

				// kind of stupid, but bash the name since it won't get recreated properly from
				// the parse_wing_create_ships call.
				shipp = &Ships[shipnum];
				sprintf(shipp->ship_name, NOX("%s %d"), wingp->name, which_one + 1);
				nprintf(("Network", "Created %s\n", shipp->ship_name));

				objp = &Objects[shipp->objnum];
				objp->net_signature = (ushort)(wingp->net_signature + which_one);

				// get the team and slot.  Team will be -1 when it isn't a part of player wing.  So, if
				// not -1, then be sure we have a valid slot, then change the ship type, etc.
				multi_ts_get_team_and_slot(shipp->ship_name, &team, &slot_index);
				if ( team != -1 ) {
					Assert( slot_index != -1 );

					// change the ship type and the weapons
					change_ship_type(objp->instance, Wss_slots_teams[team][slot_index].ship_class);
					wl_bash_ship_weapons(&shipp->weapons,&Wss_slots_teams[team][slot_index]);

					// Be sure to mark this ship as as a could_be_player
					obj_set_flags( objp, objp->flags | OF_COULD_BE_PLAYER );
					objp->flags &= ~OF_PLAYER_SHIP;
				}

				// if this is a player ship, make sure we find out who's it is and set their objnum accordingly
				for( j = 0; j < MAX_PLAYERS; j++){
					if(MULTI_CONNECTED(Net_players[j]) && (Net_players[j].player->objnum == signature)) {
						Assert( team != -1 );		// to help trap errors!!!
						nprintf(("Network", "Making %s ship for %s\n", Ships[shipnum].ship_name, Net_players[j].player->callsign));
						multi_assign_player_ship( j, objp, Ships[shipnum].ship_info_index );
						objp->flags |= OF_PLAYER_SHIP;
						objp->flags &= ~OF_COULD_BE_PLAYER;
						break;
					}
				}
			}


			// we will have no ships in any wings at this point (we didn't create any when we loaded the
			// mission).  Set the current wave of this wing to be 1 less than was passed in since this value
			// will get incremented in parse_wing_create_ships;
			wingp->current_wave = current_wave;
			wingp->total_arrived_count = total_arrived_count;
		}

		GET_DATA( p_type );
	}

   PACKET_SET_SIZE();

	// if we have reached the end of the list change our network state
	if ( p_type == INGAME_WING_LIST_EOL ) {
		Net_player->state = NETPLAYER_STATE_INGAME_WINGS;
		send_netplayer_update_packet();

		// add some mission sync text
		multi_common_add_text(XSTR("Wings packet (ingame)\n",684));
	}
	
}

// function to send information about wings.  We need to send enough information to let the client
// construct or reconstruct any wings in the mission.  We will rely on the fact that the host wing array
// will exactly match the client wing array (in terms of number, and wing names)
void send_ingame_wings_packet( net_player *player )
{
	ubyte data[MAX_PACKET_SIZE];
	ubyte p_type;
	int packet_size, i;
	ubyte what;

	BUILD_HEADER( WINGS_INGAME_PACKET );

	// iterate through the wings list
	for ( i = 0; i < num_wings; i++ ) {
		wing *wingp;

		wingp = &Wings[i];

		p_type = INGAME_WING_NEXT;
		ADD_DATA( p_type );

		ADD_DATA( i );

		// add wing data that the client needs.  There are several conditions to send to clients:
		//
		// 1. wing hasn't arrived -- total_arrived_count will be 0
		// 2. wing is done (or currently departing)
		// 3. wing is present (any wave, any number of ships).
		//
		// 1 and 2 are easy to handle.  (3) is the hardest.
		if ( wingp->total_arrived_count == 0 ) {
			what = INGAME_WING_NOT_ARRIVED;
			ADD_DATA( what );
		} else if ( wingp->flags & (WF_WING_GONE | WF_WING_DEPARTING) ) {
			what = INGAME_WING_DEPARTED;
			ADD_DATA( what );
		} else {
			int j;

			// include to code to possibly send more wing data here in this part of the if/else
			// chain.  We can do this because MAX_WINGS * 8 (8 being the number of byte for a minimum
			// description of a wing) is always less than MAX_PACKET_SIZE.  Checking here ensures that
			// we have enough space for *this* wing in the packet, and not the largest wing.  The
			// formula below looks at number of ships in the wing, the name length, length of the signature,
			// and the size of the bytes added before the ship names.  32 accounts for a little slop
			if ( packet_size > (MAX_PACKET_SIZE - (wingp->current_count * (NAME_LENGTH+2) + 32)) ) {
				p_type = INGAME_WING_LIST_EOP;
				ADD_DATA( p_type );				
				multi_io_send_reliable(player, data, packet_size);
				BUILD_HEADER( WINGS_INGAME_PACKET );
			}
			what = INGAME_WING_PRESENT;
			ADD_DATA( what );
			ADD_DATA( wingp->total_arrived_count );
			ADD_DATA( wingp->current_count );
			ADD_DATA( wingp->current_wave );

			// add the ship name and net signature of all ships currently in the wing.
			for ( j = 0; j < wingp->current_count; j++ ) {
				ship *shipp;

				shipp = &Ships[wingp->ship_index[j]];
				//ADD_STRING( shipp->ship_name );
				ADD_DATA( Objects[shipp->objnum].net_signature );
			}
		}

	}

	p_type = INGAME_WING_LIST_EOL;
	ADD_DATA( p_type );
	
	multi_io_send_reliable(player, data, packet_size);
}
*/

// send a request or a reply regarding ingame join ship choice
void send_ingame_ship_request_packet(int code,int rdata,net_player *pl)
{
	ubyte data[MAX_PACKET_SIZE],val;
	ship *shipp;
	int i, packet_size = 0;
	ushort signature;
	p_object *pobj;

	// add the data
	BUILD_HEADER(INGAME_SHIP_REQUEST);

	// add the code
	ADD_INT(code);
	
	// add any code specific data
	switch(code){
	case INGAME_SR_REQUEST:
		// add the net signature of the ship we're requesting
		signature = (ushort)rdata;
		ADD_USHORT( signature );
		break;
	case INGAME_SR_CONFIRM:
		// get a pointer to the ship
		shipp = &Ships[Objects[rdata].instance];

		// add the most recent position and orientation for the requested ship
		//ADD_DATA(Objects[rdata].pos);
        add_vector_data( data, &packet_size, Objects[rdata].pos );
		ADD_ORIENT(Objects[rdata].orient);
		ADD_INT( Missiontime );

		// add the # of respawns this ship has left
		pobj = mission_parse_get_arrival_ship( Objects[rdata].net_signature );
		Assert(pobj != NULL);
		ADD_UINT(pobj->respawn_count);

		// add the ships ets settings
		val = (ubyte)shipp->weapon_recharge_index;
		ADD_DATA(val);
		val = (ubyte)shipp->shield_recharge_index;
		ADD_DATA(val);
		val = (ubyte)shipp->engine_recharge_index;
		ADD_DATA(val);

		// add current primary and secondary banks, and add link status
		val = (ubyte)shipp->weapons.current_primary_bank;
		ADD_DATA(val);
		val = (ubyte)shipp->weapons.current_secondary_bank;
		ADD_DATA(val);		

		// add the current ammo count for secondary banks;
		val = (ubyte)shipp->weapons.num_secondary_banks;		// for sanity checking
		ADD_DATA(val);
		for ( i = 0; i < shipp->weapons.num_secondary_banks; i++ ) {
			Assert( shipp->weapons.secondary_bank_ammo[i] < UCHAR_MAX );
			val = (ubyte)shipp->weapons.secondary_bank_ammo[i];
			ADD_DATA(val);
		}

		// add the link status of weapons
		// primary link status	
		val = 0;
		if(shipp->flags & SF_PRIMARY_LINKED){
			val |= (1<<0);
		}
		if(shipp->flags & SF_SECONDARY_DUAL_FIRE){
			val |= (1<<1);
		}
		ADD_DATA(val);		
		break;
	}

	// send the packet
	if(Net_player->flags & NETINFO_FLAG_AM_MASTER){
		Assert(pl != NULL);		
		multi_io_send_reliable(pl, data, packet_size);
	} else {		
		multi_io_send_reliable(Net_player, data, packet_size);
	}

	// if this is a confirm to a player -- send data to the other players in the game telling them
	if ( (code == INGAME_SR_CONFIRM) && (Net_player->flags & NETINFO_FLAG_AM_MASTER) ) {
		int i, player_num;

		player_num = NET_PLAYER_NUM(pl);
		code = INGAME_PLAYER_CHOICE;
		BUILD_HEADER(INGAME_SHIP_REQUEST);
		ADD_INT(code);
		ADD_INT(player_num);
		ADD_USHORT(Objects[rdata].net_signature);
		for (i = 0; i < MAX_PLAYERS; i++ ) {
			if(MULTI_CONNECTED(Net_players[i]) && (&Net_players[i] != Net_player) && (i != player_num) ) {				
				multi_io_send_reliable(&Net_players[i], data, packet_size);
			}
		}
	}
}

// function to validate all players in the game according to their team select index.  If discrepancies
// are found, this function should be able to fix them up.
void multi_ingame_validate_players()
{
	int i;

	for ( i = 0; i < MAX_PLAYERS; i++ ) {
		if( MULTI_CONNECTED(Net_players[i]) && (Net_player != &Net_players[i]) && !MULTI_STANDALONE(Net_players[i]) ) {
			const char *ship_name;
			int shipnum, objnum, player_objnum;

			player_objnum = Net_players[i].player->objnum;
			if ( (Objects[player_objnum].type != OBJ_SHIP) || (Objects[player_objnum].type != OBJ_GHOST) ) {
				Int3();
			}

			ship_name = multi_ts_get_shipname( Net_players[i].p_info.team, Net_players[i].p_info.ship_index );
			Assert( ship_name != NULL );
			shipnum = ship_name_lookup( ship_name );
			if ( shipnum == -1 ) {
				// ship could be respawning
				continue;
			}
			objnum = Ships[shipnum].objnum;
			Assert( objnum != -1 );

			// if this guy's objnum isn't a ship, then it should proably be a ghost!!
			if ( Objects[objnum].type == OBJ_SHIP ) {
				if ( objnum != Net_players[i].player->objnum ) {
					Int3();
					Net_players[i].player->objnum = objnum;
				}
			} else {
				Assert( Objects[objnum].type == OBJ_GHOST );
			}
		}
	}
}

// process an ingame ship request packet
void process_ingame_ship_request_packet(ubyte *data, header *hinfo)
{
	int code;
	object *objp;
	int offset = HEADER_LENGTH;
	int team, slot_index, i;
	uint respawn_count;
	ubyte val, num_secondary_banks;
	p_object *pobj;

	// get the code
	GET_INT(code);

	switch(code){
	// a request for a ship from an ingame joiner
	case INGAME_SR_REQUEST:			
		int player_num;		
		ushort sig_request;

		// lookup the player and make sure he doesn't already have an objnum (along with possible error conditions)
		GET_USHORT(sig_request);
		PACKET_SET_SIZE();
			
		player_num = find_player_id(hinfo->id);	
		if(player_num == -1){
			nprintf(("Network","Received ingame ship request packet from unknown player!!\n"));		
			break;
		}
		
		// make sure this player doesn't already have an object
		Assert(MULTI_CONNECTED(Net_players[player_num]));
		if(Net_players[player_num].player->objnum != -1){
			send_ingame_ship_request_packet(INGAME_SR_DENY,0,&Net_players[player_num]);
			break;
		}
		
		// try and find the object
		objp = NULL;
		objp = multi_get_network_object(sig_request);
		if(objp == NULL || !(objp->flags & OF_COULD_BE_PLAYER)){
			send_ingame_ship_request_packet(INGAME_SR_DENY,0,&Net_players[player_num]);
			break;
		}		

		// Assign the player this objnum and ack him
		Net_players[player_num].player->objnum = OBJ_INDEX(objp);
		Net_players[player_num].state = NETPLAYER_STATE_IN_MISSION;                   // since he'll do this anyway...
		Net_players[player_num].flags &= ~(NETINFO_FLAG_INGAME_JOIN);
		multi_assign_player_ship( player_num, objp, Ships[objp->instance].ship_info_index );

		// update his ets and link status stuff
		multi_server_update_player_weapons(&Net_players[player_num],&Ships[objp->instance]);

		objp->flags &= ~(OF_COULD_BE_PLAYER);
		objp->flags |= OF_PLAYER_SHIP;

		// send a player settings packet to update all other players of this guy's final choices
		send_player_settings_packet();

		// initialize datarate limiting for this guy
		multi_oo_rate_init(&Net_players[player_num]);
		
		// ack him
		send_ingame_ship_request_packet(INGAME_SR_CONFIRM,OBJ_INDEX(objp),&Net_players[player_num]);

		// clear my ingame join flag so that others may join
		Netgame.flags &= ~NG_FLAG_INGAME_JOINING;

		// clear his net stats
		scoring_level_init( &(Net_players[player_num].player->stats) );
		break;

		// a denial for the ship we requested from the server
	case INGAME_SR_DENY :
		PACKET_SET_SIZE();

		// set this to -1 so we can pick again
		Multi_ingame_join_sig = 0;

		// display a popup
		popup(PF_BODY_BIG | PF_USE_AFFIRMATIVE_ICON,1,POPUP_OK,XSTR("You have been denied the requested ship",686));
		break;

	// a confirmation that we can use the selected ship
	case INGAME_SR_CONFIRM:
		// object *temp_objp;

		// delete the ship this ingame joiner was using.  Unassign Player_obj so that this object
		// doesn't become a ghost.
		// temp_objp = Player_obj;
		// Player_obj = NULL;
		// obj_delete( OBJ_INDEX(temp_objp) );

		// get the object itself
		objp = multi_get_network_object(Multi_ingame_join_sig);
		Assert(objp != NULL);

		// get its most recent position and orientation
		//GET_DATA(objp->pos);
        get_vector_data( data, &offset, objp->pos );
		GET_ORIENT(objp->orient);
		GET_INT( Missiontime );
		GET_UINT( respawn_count );
				
		// tell the server I'm in the mission
		Net_player->state = NETPLAYER_STATE_IN_MISSION;
		send_netplayer_update_packet();

		// setup our object				
		Net_player->player->objnum = OBJ_INDEX(objp);			
		Player_obj = objp;
		Player_obj->flags &= ~(OF_COULD_BE_PLAYER);
		Player_obj->flags |= OF_PLAYER_SHIP;
		multi_assign_player_ship( MY_NET_PLAYER_NUM, objp, Ships[objp->instance].ship_info_index );

		// must change the ship type and weapons.  An ingame joiner know about the default class
		// and weapons for a ship, but these could have changed.
		multi_ts_get_team_and_slot(Player_ship->ship_name, &team, &slot_index);
		Assert( team != -1 );
		Assert( slot_index != -1 );
		change_ship_type(objp->instance, Wss_slots_teams[team][slot_index].ship_class);
		wl_bash_ship_weapons(&Player_ship->weapons,&Wss_slots_teams[team][slot_index]);

		// get the parse object for it and assign the respawn count
		pobj = mission_parse_get_arrival_ship( objp->net_signature );
		Assert(pobj != NULL);
		pobj->respawn_count = respawn_count;

		// get the ships ets settings
		GET_DATA(val);
		Player_ship->weapon_recharge_index = val;
		GET_DATA(val);
		Player_ship->shield_recharge_index = val;
		GET_DATA(val);
		Player_ship->engine_recharge_index = val;		

		// get current primary and secondary banks, and add link status
		GET_DATA(val);
		Player_ship->weapons.current_primary_bank = val;
		GET_DATA(val);
		Player_ship->weapons.current_secondary_bank = val;				

		// secondary bank ammo data
		GET_DATA( num_secondary_banks );
		Assert( num_secondary_banks == Player_ship->weapons.num_secondary_banks );
		for ( i = 0; i < Player_ship->weapons.num_secondary_banks; i++ ) {
			GET_DATA(val);
			Player_ship->weapons.secondary_bank_ammo[i] = val;
		}


		// get the link status of weapons
		GET_DATA(val);
		if(val & (1<<0)){
			Player_ship->flags |= SF_PRIMARY_LINKED;
		}
		if(val & (1<<1)){
			Player_ship->flags |= SF_SECONDARY_DUAL_FIRE;
		}		
		PACKET_SET_SIZE();					

		// be sure that this ships current primary/secondary weapons are valid.  Easiest is to just
		// bash the values to 0!
		/*
		if ( Player_ship->weapons.current_primary_bank == -1 )
			Player_ship->weapons.current_primary_bank = 0;
		if ( Player_ship->weapons.current_secondary_bank == -1 )
			Player_ship->weapons.current_secondary_bank = 0;
		*/
		
		Net_player->flags &= ~(NETINFO_FLAG_INGAME_JOIN);

		// clear all object collision pairs, then add myself to the list
		extern void obj_reset_all_collisions();
		obj_reset_all_collisions();
		// obj_reset_pairs();
		// obj_add_pairs( OBJ_INDEX(Player_obj) );

		mission_hotkey_set_defaults();

		//multi_ingame_validate_players();

		// jump into the mission 
		// NOTE : we check this flag because its possible that the player could have received an endgame packet in the same
		//        frame as getting this confirmation. In that case, he should be quitting to the main menu. We must not make
		//        him continue on into the mission
		if(!multi_endgame_ending()){
			gameseq_post_event(GS_EVENT_ENTER_GAME);								
		} 
		break;

	case INGAME_PLAYER_CHOICE: {
		int player_num;
		ushort net_signature;
		object *objp;

		// get the player number of this guy, and the net signature of the ship he has chosen
		GET_INT(player_num);
		GET_USHORT(net_signature);
		PACKET_SET_SIZE();

		objp = multi_get_network_object(net_signature);
		if ( objp == NULL ) {
			// bogus!!!  couldn't find the object -- we cannot connect his -- this is really bad!!!
			nprintf(("Network", "Couldn't find ship for ingame joiner %s\n", Net_players[player_num].player->callsign));
			break;
		}
		objp->flags |= OF_PLAYER_SHIP;
		objp->flags &= ~OF_COULD_BE_PLAYER;

		multi_assign_player_ship( player_num, objp, Ships[objp->instance].ship_info_index );

		break;
		}
	}
}


// --------------------------------------------------------------------------------------------------
// INGAME JOIN FORWARD DEFINITIONS
//

void multi_ingame_send_ship_update(net_player *p)
{
	ship_obj *moveup;
	
	// get the first object on the list
	moveup = GET_FIRST(&Ship_obj_list);
	
	// go through the list and send all ships which are mark as OF_COULD_BE_PLAYER
	while(moveup!=END_OF_LIST(&Ship_obj_list)){
		if(Objects[moveup->objnum].flags & OF_COULD_BE_PLAYER){
			// send the update
			send_ingame_ship_update_packet(p,&Ships[Objects[moveup->objnum].instance]);
		}

		// move to the next item
		moveup = GET_NEXT(moveup);
	}
}

// for now, I guess we'll just send hull and shield % values
void send_ingame_ship_update_packet(net_player *p,ship *sp)
{
	ubyte data[MAX_PACKET_SIZE];
	object *objp;
	int idx;
	int packet_size = 0;
	float f_tmp;

	BUILD_HEADER(INGAME_SHIP_UPDATE);
	
	// just send net signature, shield and hull percentages
	objp = &Objects[sp->objnum];
	ADD_USHORT(objp->net_signature);
	ADD_UINT(objp->flags);
	ADD_FLOAT(objp->hull_strength);
	
	// shield percentages
	for(idx=0; idx<MAX_SHIELD_SECTIONS; idx++){
		f_tmp = objp->shields[idx];
		ADD_FLOAT(f_tmp);
	}
	
	multi_io_send_reliable(p, data, packet_size);
}

void process_ingame_ship_update_packet(ubyte *data, header *hinfo)
{
	int offset;
	float garbage;
	int flags;
	int idx;
	ushort net_sig;
	object *lookup;
	float f_tmp;
	
	offset = HEADER_LENGTH;
	// get the net sig for the ship and do a lookup
	GET_USHORT(net_sig);
	GET_INT(flags);
   
	// get the object
	lookup = multi_get_network_object(net_sig);
	if(lookup == NULL){
		// read in garbage values if we can't find the ship
		nprintf(("Network","Got ingame ship update for unknown object\n"));
		GET_FLOAT(garbage);
		for(idx=0;idx<MAX_SHIELD_SECTIONS;idx++){
			GET_FLOAT(garbage);
		}

		PACKET_SET_SIZE();
		return;
	}
	// otherwise read in the ship values
	lookup->flags = flags;
 	GET_FLOAT(lookup->hull_strength);
	for(idx=0;idx<MAX_SHIELD_SECTIONS;idx++){
		GET_FLOAT(f_tmp);
		lookup->shields[idx] = f_tmp;
	}

	PACKET_SET_SIZE();
}

