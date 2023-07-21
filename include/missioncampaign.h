/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Mission/MissionCampaign.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * header file for dealing with campaigns
 *
 * $Log$
 * Revision 1.4  2003/06/11 18:30:32  taylor
 * plug memory leaks
 *
 * Revision 1.3  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 16    9/30/99 6:02p Jefff
 * Changes for OEM
 * 
 * 15    9/09/99 11:40p Dave
 * Handle an Assert() in beam code. Added supernova sounds. Play the right
 * 2 end movies properly, based upon what the player did in the mission.
 * 
 * 14    9/07/99 6:55p Jefff
 * functionality to break out of a loop.  hacked functionality to jump to
 * a specific mission in a campaign -- doesnt grant ships/weapons from
 * skipped missions tho.
 * 
 * 13    9/06/99 9:45p Jefff
 * break out of loop and skip mission support
 * 
 * 12    9/06/99 6:38p Dave
 * Improved CD detection code.
 * 
 * 11    8/27/99 12:04a Dave
 * Campaign loop screen.
 * 
 * 10    7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 9     4/25/99 3:02p Dave
 * Build defines for the E3 build.
 * 
 * 8     2/25/99 4:19p Dave
 * Added multiplayer_beta defines. Added cd_check define. Fixed a few
 * release build warnings. Added more data to the squad war request and
 * response packets.
 * 
 * 7     12/12/98 3:17p Andsager
 * Clean up mission eval, goal, event and mission scoring.
 * 
 * 6     12/10/98 9:59a Andsager
 * Fix some bugs with mission loops
 * 
 * 5     12/09/98 1:56p Andsager
 * Initial checkin of mission loop
 * 
 * 4     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 3     10/07/98 6:27p Dave
 * Globalized mission and campaign file extensions. Removed Silent Threat
 * special code. Moved \cache \players and \multidata into the \data
 * directory.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 47    9/10/98 1:17p Dave
 * Put in code to flag missions and campaigns as being MD or not in Fred
 * and Freespace. Put in multiplayer support for filtering out MD
 * missions. Put in multiplayer popups for warning of non-valid missions.
 * 
 * 46    6/05/98 9:54a Lawrance
 * OEM changes
 * 
 * 45    5/21/98 9:25p Allender
 * endgame movie always viewable at end of campaign
 * 
 * 44    5/13/98 5:14p Allender
 * red alert support to go back to previous mission
 * 
 * 43    5/12/98 4:16p Hoffoss
 * Fixed bug where not all missions in all campaigns were being filtered
 * out of stand alone mission listing in simulator room.
 * 
 * 42    4/28/98 5:25p Sandeep
 * 
 * 41    4/27/98 7:30p Hoffoss
 * Added campaign descriptions to campaign files and changed campaign room
 * to display them.
 * 
 * 40    4/25/98 7:40p Allender
 * fixd some small hotkey stuff.  Worked on turret orientation being
 * correct for multiplayer.  new sexpression called end-campaign will will
 * end the main campaign
 * 
 * 39    4/13/98 10:25p Hoffoss
 * Added a flag for subspace missions, and for aboard the Galatea or
 * Bastion.
 * 
 * 38    4/08/98 6:16p Hoffoss
 * Added a description field to campaign structure, since the campaign
 * room has a window for this.  We'll need to add this in sometime.
 * 
 * 37    4/06/98 6:37p Dave
 * Put in max_observers netgame server option. Make sure host is always
 * defaulted to alpha 1 or zeta 1. Changed create game so that MAX_PLAYERS
 * can always join but need to be kicked before commit can happen. Put in
 * support for server ending a game and notifying clients of a special
 * condition.
 * 
 * 36    4/02/98 11:40a Lawrance
 * check for #ifdef DEMO instead of #ifdef DEMO_RELEASE
 * 
 * 35    3/31/98 12:23a Allender
 * changed macro names of campaign types to be more descriptive.  Added
 * "team" to objectives dialog for team v. team missions.  Added two
 * distinct multiplayer campaign types
 * 
 * 34    3/30/98 10:37p Allender
 * added demo.fsc as default campaign for demo.  Delete obsolete pilots
 * (including their campaign save games)
 * 
 * 33    3/26/98 5:24p Allender
 * put in respawn edit box into mission notes dialog.  Made loading of
 * missions/campaign happen when first entering the game setup screen.
 * 
 * 32    3/17/98 4:17p Allender
 * made Fred/FreeSpace use the same campaign loading code
 * 
 * 31    3/02/98 5:22p Hoffoss
 * Removed ready room and added campaign room.
 * 
 * 30    2/26/98 10:07p Hoffoss
 * Rewrote state saving and restoring to fix bugs and simplify the code.
 * 
 * 29    2/23/98 11:08p Dave
 * Finished up multiplayer campaign support. Seems bug-free.
 * 
 * 28    2/10/98 5:51p Hoffoss
 * Changed campaign stuff so new pilots start out with no active campaign
 * (will get set in readyroom before user can move beyond the readyroom).
 * 
 * 27    2/05/98 10:14p Lawrance
 * Implement Save and Quit
 * 
 * 26    12/19/97 2:59p Allender
 * more stuff to get persistent ships/weapons across campaign missions
 * 
 * 25    12/19/97 12:03p Allender
 * Added GM_CAMPAIGN_MODE to indicate when player is in a campaign or not.
 * Started adding FreeSpace support for carrying of ship/weapon types
 * across missions in a campaign.
 * 
 * 24    12/18/97 5:12p Allender
 * initial support for ship/weapon persistence
 * 
 * 23    12/05/97 1:55p Allender
 * delete campaign save files when pilot is deleted
 * 
 * 22    11/20/97 7:19p Hoffoss
 * Renamed mission_campaign_do_new() to mission_campaign_load().  Changed
 * readyroom to not lose campaign savefile until commiting to a different
 * comapaign.
 * 
 * 21    11/20/97 5:36p Dave
 * Hooked in a bunch of main hall changes (including sound). Made it
 * possible to reposition (rewind/ffwd) 
 * sound buffer pointers. Fixed animation direction change framerate
 * problem.
 * 
 * 20    11/16/97 1:11p Hoffoss
 * Coded up readyroom screen, first pass.
 * 
 * 19    11/15/97 2:37p Dave
 * More multiplayer campaign support.
 * 
 * 18    11/13/97 5:03p Sandeep
 * added CSFE support
 * 
 * 17    11/12/97 4:40p Dave
 * Put in multiplayer campaign support parsing, loading and saving. Made
 * command-line variables better named. Changed some things on the initial
 * pilot select screen.
 * 
 * 16    11/11/97 4:57p Dave
 * Put in support for single vs. multiplayer pilots. Began work on
 * multiplayer campaign saving. Put in initial player select screen
 * 
 * 15    11/06/97 4:36p Allender
 * campaign work to add a briefing cutscene to play before the briefing.
 * 
 * 14    10/21/97 4:50p Allender
 * goal/event sexpression support.  Fixed campaign save file, sexpressions
 * handling, sexpression editing, etc
 * 
 * 13    10/20/97 5:13p Allender
 * new subsystem sabotage/repair/set sexpressions.  Added new event/goal
 * status checking sexpressions (not fully implemented yet).  Change
 * campaign save files to save all events as well as goals
 * 
 * 12    9/18/97 10:18p Lawrance
 * add function to extract the mission names from a campaign file
 * 
 * 11    8/21/97 10:58p Hoffoss
 * Fixed bug in mission scanning for campaign editor, and added mission
 * notes to sexp help window in campaign editor when mission is selected.
 * 
 * 10    8/13/97 5:49p Hoffoss
 * Fixed bugs, made additions.
 * 
 * 9     8/08/97 5:18p Jasen
 * Fixed bug with current campaign in pilot file has been deleted.  Now it
 * defaults back to default if current doesn't exist.
 * 
 * 8     5/09/97 9:49a Hoffoss
 * Added campaign loading for Fred and 2 new tokens to campaign file
 * format.
 * 
 * 7     4/25/97 1:55p Allender
 * added close() routine to free memory at game shutdown
 * 
 * 6     4/25/97 11:31a Allender
 * Campaign state now saved in campaign save file in player directory.
 * Made some global variables follow naming convention.  Solidified
 * continuing campaigns based on new structure
 * 
 * 5     4/23/97 3:21p Allender
 * more campaign stuff -- mission branching through campaign file now
 * works!!!!
 * 
 * 4     4/22/97 10:44a Allender
 * more campaign stuff.  Info about multiple campaigns now stored in
 * player file -- not saving some player information in save games.
 * 
 * 3     4/18/97 9:59a Allender
 * more campaign stuff.  All campaign related varaibles now stored in
 * campaign structure
 * 
 * 2     4/17/97 9:02p Allender
 * campaign stuff.  All campaign material stored in external file.
 * Continuing campaign won't work at this point
 * 
 * 1     4/15/97 2:11p Allender
 *
 * $NoKeywords: $
 */

#ifndef _MISSION_CAMPAIGN_H
#define _MISSION_CAMPAIGN_H

#include "parselo.h"
#include "cfile.h"
#include "ship.h"
#include "weapon.h"
#include "scoring.h"

// name of the builtin campaign.
#if defined(FS2_DEMO) || defined(FS1_DEMO)
	#define BUILTIN_CAMPAIGN		"demo"
	#define BUILTIN_CAMPAIGN_NAME	"DEMO Campaign"
#elif defined(OEM_BUILD)
	#define BUILTIN_CAMPAIGN		"FreeSpace2OEM"
	#define BUILTIN_CAMPAIGN_NAME	"OEM Campaign"
#elif defined(E3_BUILD)
	#define BUILTIN_CAMPAIGN		"E3"
	#define BUILTIN_CAMPAIGN_NAME	"E3 Campaign"
#elif defined(MAKE_FS1)
    #define BUILTIN_CAMPAIGN		"FreeSpace"
    #define BUILTIN_CAMPAIGN_NAME	"The Main Freespace Campaign"
#else
	#define BUILTIN_CAMPAIGN		"FreeSpace2"
	#define BUILTIN_CAMPAIGN_NAME	"The Main Freespace2 Campaign"
#endif

#define MAX_CAMPAIGN_MISSIONS	100			// maximum number of missions in a campaign

#define CAMPAIGN_ERROR_CORRUPT			-1
#define CAMPAIGN_ERROR_SEXP_EXHAUSTED	-2

// types of campaigns -- these defines match the string literals listed below which
// are found in the campaign files.  I don't think that we need campaigns for furball
// missions.
#define CAMPAIGN_TYPE_SINGLE			0
#define CAMPAIGN_TYPE_MULTI_COOP		1
#define CAMPAIGN_TYPE_MULTI_TEAMS	2

#define MAX_CAMPAIGN_TYPES				3

// type of movies we may be able to play
#define CAMPAIGN_MOVIE_PRE_MISSION		1
#define CMAPAIGN_MOVIE_POST_MISSION		2

#define CAMPAIGN_SINGLE_PLAYER_SIG     0xddddeeee
#define CAMPAIGN_MULTI_PLAYER_SIG      0xeeeeffff

// defines for possibly persistent information
#define CAMPAIGN_PERSISTENT_SHIP			1
#define CAMPAIGN_PERSISTENT_WEAPON		2

#define CMISSION_FLAG_BASTION	(1<<0)	// set if stationed on Bastion, else Galatea
#define CMISSION_FLAG_SKIPPED	(1<<1)	// set if skipped, else not

#define CAMPAIGN_LOOP_MISSION_UNINITIALIZED	-2

extern const char *campaign_types[MAX_CAMPAIGN_TYPES];

// structure for a campaign definition.  It contains the mission names and other interesting
// information about a campaign and the mission strucuture within.

typedef struct mgoal {
	char	name[NAME_LENGTH];		// name of the goal (same as name in the mission_goal structure
	char	status;						// failed, satisfied, or incomplete (same as goal completion);
} mgoal;

typedef struct mevent {
	char	name[NAME_LENGTH];
	char	status;
} mevent;

typedef struct cmission {
	char				*name;					// name of the mission
	char				*notes;					// mission notes for mission (used by Fred)
	char				briefing_cutscene[NAME_LENGTH];	// name of the cutscene to be played before this mission
	int				formula;					// sexpression used to determine mission branching.
	int				completed;				// has the player completed this mission
	int				num_goals;				// number of goals this mission had
	mgoal				*goals;					// malloced array of mgoals (of num_goals size) which has the goal completion status
	int				num_events;				// number of events this mission had
	mevent			*events;					// malloced array of mevents (of num_events) size) which has event completion status
	int				has_mission_loop;		// whether current mission has side loop
	int				mission_loop_formula;// formula to determine whether to allow a side loop
	char				*mission_loop_desc;	// message in popup
	char				*mission_loop_brief_anim;
	char				*mission_loop_brief_sound;
	int				level;					// what level of the tree it's on (Fred)
	int				pos;						// what x position on level it's on (Fred)
	int				flags;
	scoring_struct	stats;
} cmission;

typedef struct campaign {
	char		name[NAME_LENGTH];						// name of the campaign
	char		filename[MAX_FILENAME_LEN];			// filename the campaign info is in
	char		*desc;										// description of campaign
	int		type;											// type of campaign
	int		num_missions;								// number of missions in the campaign
	int		num_missions_completed;					// number of missions in the campaign that have been flown
	int		current_mission;							// the current mission that the player is playing.  Only valid during the mission
	int		next_mission;								// number of the next mission to fly when comtinuing the campaign.  Always valid
	int		prev_mission;								// mission that we just came from.  Always valid
	int		loop_enabled;								// whether mission loop is chosen - true during a loop, false otherwise
	int		loop_mission;								// mission number of misssion loop (if any)
	int		loop_reentry;								// mission number to return to after loop is finished
	int		realign_required;							// are any missions missing alignment info? (Fred)
	int		num_players;								// valid in multiplayer campaigns -- number of players campaign supports.
	ubyte		ships_allowed[MAX_SHIP_TYPES];		// which ships the player can use
	ubyte		weapons_allowed[MAX_WEAPON_TYPES];	// which weapons the player can use
	cmission	missions[MAX_CAMPAIGN_MISSIONS];		// decription of the missions
} campaign;

extern campaign Campaign;

// campaign wasn't ended
extern int Campaign_ended_in_mission;

// structure for players.  Holds the campaign name, number of missions flown in the campaign, and result
// of the missions flown.  This structure is stored in the player file and thus is persistent across
// games
typedef struct campaign_info
{
	char		filename[NAME_LENGTH];
	int		num_missions_completed;
	ubyte		missions_completed[MAX_CAMPAIGN_MISSIONS];
} campaign_info;

// extern'ed so the mission loading can get a list of campains.  Only use this
// data after mission_campaign_build_list() is called
#define MAX_CAMPAIGNS	128
extern char *Campaign_names[MAX_CAMPAIGNS];
extern char *Campaign_file_names[MAX_CAMPAIGNS];
extern int	Num_campaigns;

// called at game startup time to load the default single player campaign
void mission_campaign_init( void );

// called to reload the default campaign
int mission_campaign_load_by_name( const char *filename );
int mission_campaign_load_by_name_csfe( const char *filename, const char *callsign );


// load up and initialize a new campaign
int mission_campaign_load( const char *filename, int load_savefile = 1 );

// function to save the state of the campaign between missions or to load a campaign save file
extern int mission_campaign_save( void );

// declaration for local campaign save game load function
extern void mission_campaign_savefile_load( const char *cfilename );
extern void mission_campaign_savefile_delete( const char *cfilename, int is_multi = -1 );
extern void mission_campaign_delete_all_savefiles( const char *pilot_name, int is_multi );

// if a given campaign is a multiplayer campaign, we can load and save the multiplayer info portion with these functions
extern int mission_campaign_parse_is_multi(const char *filename, char *name);

// function which sets up internal variable for player to play next mission in the campaign
extern int mission_campaign_next_mission( void );

// function which is called with the current mission in this campaign is over
extern void mission_campaign_mission_over( void );

// frees all memory at game close time
extern void mission_campaign_close( void );		// gets called in more than game close
extern void mission_campaign_shutdown( void );	// only called in game close

// read in a campaign file.  Used by Fred.
int mission_campaign_load_fred(const char *filename, const char *name_verify = NULL);

// used by Fred to get a mission's list of goals.
void read_mission_goal_list(int num);

void mission_campaign_build_list( int multiplayer );

// returns index of mission with passed name
extern int mission_campaign_find_mission( const char *name );

// maybe play a movie.  type indicates before or after mission
extern void mission_campaign_maybe_play_movie(int type);

// save persistent information
extern void mission_campaign_save_persistent( int type, int index );

void mission_campaign_savefile_generate_root(char *filename);

// The following are functions I added to set up the globals and then
// execute the corresponding mission_campaign_savefile functions.

// Saves the campaign camp under the player name pname
int campaign_savefile_save(const char *pname);
// Deletes the campaign save camp under the player name pname
void campaign_delete_save( const char *cfn, const char *pname);
// Loads campaign camp from fname under player name pname
void campaign_savefile_load(const char *fname, const char *pname);

// get name and type of specified campaign file
int mission_campaign_get_info(const char *filename, char *name, int *type, int *max_players, char **desc = NULL);

// get a listing of missions in a campaign
int mission_campaign_get_mission_list(const char *filename, char **list, int max);

// load up a campaign for the current player.
int mission_load_up_campaign();

// stores mission goals and events in Campaign struct
void mission_campaign_store_goals_and_events();

// evaluates next mission and possible loop mission
void mission_campaign_eval_next_mission();

// returns to the beginning of the previous mission
int mission_campaign_previous_mission();

// proceeds to next mission in campaign
void mission_campaign_skip_to_next(int start_game = 1);

// break out of loop
void mission_campaign_exit_loop();

// jump to specified mission
void mission_campaign_jump_to_mission(const char *name);

// stuff for the end of the campaign of the single player game
void mission_campaign_end_init();
void mission_campaign_end_close();
void mission_campaign_end_do();

// End CSFE stuff
#endif

