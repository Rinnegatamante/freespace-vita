/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Source$
 * $Revision: 307 $
 * $Author: taylor $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 *
 * main header file for parsing code  
 *
 * $Log$
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
 * 26    8/23/99 6:21p Jefff
 * added "no traitor" option to missions (and fred)
 * 
 * 25    8/23/99 5:04p Jefff
 * Added new mission flag to disable built-in messages from playing.
 * Added fred support as well.
 * 
 * 24    8/16/99 3:53p Andsager
 * Add special warp in interface in Fred and saving / reading.
 * 
 * 23    8/16/99 2:01p Andsager
 * Knossos warp-in warp-out.
 * 
 * 22    7/28/99 1:36p Andsager
 * Modify cargo1 to include flag CARGO_NO_DEPLETE.  Add sexp
 * cargo-no-deplete (only for BIG / HUGE).  Modify ship struct to pack
 * better.
 * 
 * 21    7/26/99 5:50p Dave
 * Revised ingame join. Better? We'll see....
 * 
 * 20    7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 19    7/02/99 4:31p Dave
 * Much more sophisticated lightning support.
 * 
 * 18    7/01/99 11:44a Dave
 * Updated object sound system to allow multiple obj sounds per ship.
 * Added hit-by-beam sound. Added killed by beam sound.
 * 
 * 17    6/28/99 4:51p Andsager
 * Add ship-guardian sexp (does not allow ship to be killed)
 * 
 * 16    5/20/99 7:00p Dave
 * Added alternate type names for ships. Changed swarm missile table
 * entries.
 * 
 * 15    4/26/99 8:49p Dave
 * Made all pof based nebula stuff full customizable through fred.
 * 
 * 14    4/26/99 12:49p Andsager
 * Add protect object from beam support to Fred
 * 
 * 13    3/24/99 4:05p Dave
 * Put in support for assigning the player to a specific squadron with a
 * specific logo. Preliminary work for doing pos/orient checksumming in
 * multiplayer to reduce bandwidth.
 * 
 * 12    3/01/99 7:39p Dave
 * Added prioritizing ship respawns. Also fixed respawns in TvT so teams
 * don't mix respawn points.
 * 
 * 11    2/26/99 6:01p Andsager
 * Add sexp has-been-tagged-delay and cap-subsys-cargo-known-delay
 * 
 * 10    2/23/99 8:11p Dave
 * Tidied up dogfight mode. Fixed TvT ship type problems for alpha wing.
 * Small pass over todolist items.
 * 
 * 9     2/23/99 2:29p Dave
 * First run of oldschool dogfight mode. 
 * 
 * 8     2/11/99 2:15p Andsager
 * Add ship explosion modification to FRED
 * 
 * 7     2/03/99 12:42p Andsager
 * Add escort priority.  Modify ship_flags_dlg to include field.  Save and
 * Load.  Add escort priority field to ship.
 * 
 * 6     11/14/98 5:32p Dave
 * Lots of nebula work. Put in ship contrails.
 * 
 * 5     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 4     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
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
 * 147   8/31/98 2:06p Dave
 * Make cfile sort the ordering or vp files. Added support/checks for
 * recognizing "mission disk" players.
 * 
 * 146   5/11/98 4:33p Allender
 * fixed ingame join problems -- started to work on new object updating
 * code (currently ifdef'ed out)
 * 
 * 145   5/05/98 11:05p Allender
 * ability to flag mission as "no promotion" where promotions and badges
 * are *not* granted even if they should be.  Slight fix to multiplayer
 * problem where locking_subsys is wrong for players current target
 * 
 * 144   5/04/98 6:06p Lawrance
 * Make red alert mode work!
 * 
 * 143   4/20/98 4:56p Allender
 * allow AI ships to respawn as many times as there are respawns in the
 * mission.  
 * 
 * 142   4/14/98 12:08a Allender
 * save wingman status information in parse object and restore from parse
 * object when respawned
 * 
 * 141   4/13/98 10:25p Hoffoss
 * Added a flag for subspace missions, and for aboard the Galatea or
 * Bastion.
 * 
 * 140   4/06/98 10:24p Dave
 * Fixed up Netgame.respawn for the standalone case.
 * 
 * 139   4/03/98 12:17a Allender
 * new sexpression to detect departed or destroyed.  optionally disallow
 * support ships.  Allow docking with escape pods 
 * 
 * 138   4/02/98 6:31p Lawrance
 * reduce MAX_SUBSYS_STATUS to 125 if DEMO defined
 * 
 * 137   3/26/98 5:24p Allender
 * put in respawn edit box into mission notes dialog.  Made loading of
 * missions/campaign happen when first entering the game setup screen.
 * 
 * 136   3/18/98 10:38p Allender
 * added required "num players" for multiplayer missions.  Put in required
 * "num players" for multiplayer campaigns.  Added campaign editor support
 * to determine "num players"
 * 
 * 135   3/16/98 8:27p Allender
 * Fred support for two new AI flags -- kamikaze and no dynamic goals.
 * 
 * 
 */

#ifndef _PARSE_H
#define _PARSE_H

#include <setjmp.h>
#include "parselo.h"
#include "ship.h"
#include "ai.h"
#include "missionbriefcommon.h"
#include "weapon.h"

#define NUM_NEBULAS			3				// how many background nebulas we have altogether
#define NUM_NEBULA_COLORS	9

// arrival anchor types
#define SPECIAL_ARRIVAL_ANCHORS_OFFSET	90000  // offset used to avoid conflicting with ship anchors
#define ANY_FRIENDLY					(SPECIAL_ARRIVAL_ANCHORS_OFFSET + 0)
#define ANY_HOSTILE					(SPECIAL_ARRIVAL_ANCHORS_OFFSET + 1)
#define ANY_NEUTRAL					(SPECIAL_ARRIVAL_ANCHORS_OFFSET + 2)
#define ANY_FRIENDLY_PLAYER		(SPECIAL_ARRIVAL_ANCHORS_OFFSET + 3)
#define ANY_HOSTILE_PLAYER			(SPECIAL_ARRIVAL_ANCHORS_OFFSET + 4)
#define ANY_NEUTRAL_PLAYER			(SPECIAL_ARRIVAL_ANCHORS_OFFSET + 5)

// update version when mission file format changes, and add approprate code
// to check loaded mission version numbers in the parse code.  Also, be sure
// to update both MissionParse and MissionSave (FRED) when changing the
// mission file format!
#define	MISSION_VERSION 0.10f
#define	FRED_MISSION_VERSION 0.10f

// maximum number of starting wings for the player
#define MAX_STARTING_WINGS		3
extern const char *Starting_wing_names[MAX_STARTING_WINGS+1];

#define WING_PLAYER_BASE	0x80000  // used by Fred to tell ship_index in a wing points to a player

// defines used for parse_mission() to tell what kind of information to get from the mission file
#define MISSION_PARSE_MISSION_INFO	1

// bitfield definitions for missions game types
#define OLD_MAX_GAME_TYPES				4					// needed for compatibility
#define OLD_GAME_TYPE_SINGLE_ONLY	0
#define OLD_GAME_TYPE_MULTI_ONLY		1
#define OLD_GAME_TYPE_SINGLE_MULTI	2
#define OLD_GAME_TYPE_TRAINING		3

#define MAX_MISSION_TYPES				5
#define MISSION_TYPE_SINGLE			(1<<0)
#define MISSION_TYPE_MULTI				(1<<1)
#define MISSION_TYPE_TRAINING			(1<<2)
#define MISSION_TYPE_MULTI_COOP		(1<<3)
#define MISSION_TYPE_MULTI_TEAMS		(1<<4)
#define MISSION_TYPE_MULTI_DOGFIGHT	(1<<5)

#define MISSION_FLAG_SUBSPACE				(1<<0)	// mission takes place in subspace
#define MISSION_FLAG_NO_PROMOTION		(1<<1)	// cannot get promoted or badges in this mission
#define MISSION_FLAG_FULLNEB				(1<<2)	// mission is a full nebula mission
#define MISSION_FLAG_NO_BUILTIN_MSGS	(1<<3)	// disables builtin msgs
#define MISSION_FLAG_NO_TRAITOR			(1<<4)	// player cannot become a traitor

// some mice macros for mission type
#define IS_MISSION_MULTI_COOP			(The_mission.game_type & MISSION_TYPE_MULTI_COOP)
#define IS_MISSION_MULTI_TEAMS		(The_mission.game_type & MISSION_TYPE_MULTI_TEAMS)
#define IS_MISSION_MULTI_DOGFIGHT	(The_mission.game_type & MISSION_TYPE_MULTI_DOGFIGHT)

#define MISSION_DESC_LENGTH	512

typedef struct mission {
	char	name[NAME_LENGTH];
	char	author[NAME_LENGTH];
	float	version;
	char	created[DATE_TIME_LENGTH];
	char	modified[DATE_TIME_LENGTH];
	char	notes[NOTES_LENGTH];
	char	mission_desc[MISSION_DESC_LENGTH];
	int	game_type;
	int	flags;
	int	num_players;									// valid in multiplayer missions -- number of players supported
	uint	num_respawns;									// valid in multiplayer missions -- number of respawns allowed
	int	red_alert;
	int	scramble;
	int	disallow_support;								// should support ships be disallowed.
	char	tour_name[NAME_LENGTH];
	char	pre_briefing_cutscene[FILESPEC_LENGTH];
	char	pre_mission_cutscene[FILESPEC_LENGTH];
	char	next_mission_success[NAME_LENGTH];
	char	next_mission_partial[NAME_LENGTH];
	char	next_mission_failure[NAME_LENGTH];
	char	squad_filename[MAX_FILENAME_LEN+1];		// if the player has been reassigned to a squadron, this is the filename of the logo, otherwise empty string
	char	squad_name[NAME_LENGTH+1];				// if the player has been reassigned to a squadron, this is the name of the squadron, otherwise empty string
} mission;

// cargo defines
// NOTE: MAX_CARGO MUST REMAIN <= 64 (CARGO_NO_DEPLETE) for NO_DEPLETE to work.
#define CARGO_INDEX_MASK	0xBF
#define CARGO_NO_DEPLETE	0x40
#define MAX_CARGO				30

extern mission The_mission;
extern char Mission_filename[80];  // filename of mission in The_mission (Fred only)

#define	MAX_IFF					3
#define	MAX_FORMATION_NAMES	3
#define	MAX_STATUS_NAMES		3
#define	MAX_TEAM_NAMES			4

// defines for arrival locations.  These defines should match their counterparts in the arrival location
// array
#define	MAX_ARRIVAL_NAMES				4
#define	ARRIVE_AT_LOCATION			0
#define	ARRIVE_NEAR_SHIP				1
#define	ARRIVE_IN_FRONT_OF_SHIP		2
#define	ARRIVE_FROM_DOCK_BAY			3

// defines for departure locations.  These defines should match their counterparts in the departure location
// array
#define MAX_DEPARTURE_NAMES			2
#define DEPART_AT_LOCATION				0
#define DEPART_AT_DOCK_BAY				1

#define	MAX_GOAL_TYPE_NAMES	3

// alternate ship type names
#define MAX_ALT_TYPE_NAMES				10
extern char Mission_alt_types[MAX_ALT_TYPE_NAMES][NAME_LENGTH];
extern int Mission_alt_type_count;

// These species defines must match the contents of the Species_names[MAX_SPECIES_NAMES] array.
#define	MAX_SPECIES_NAMES		3
#define	SPECIES_TERRAN			0
#define	SPECIES_VASUDAN		1
#define	SPECIES_SHIVAN			2
#define	SPECIES_NONE			3

#if defined(FS2_DEMO) || defined(FS1_DEMO)
	#define MAX_SUBSYS_STATUS		125
#else
	#define MAX_SUBSYS_STATUS		250
#endif

#define MAX_SPECIAL_ARRIVAL_ANCHORS	6
extern const char *Special_arrival_anchor_names[MAX_SPECIAL_ARRIVAL_ANCHORS];

extern char *Ship_class_names[MAX_SHIP_TYPES];
extern const char *Iff_names[MAX_IFF];
extern const char *Ai_behavior_names[MAX_AI_BEHAVIORS];
extern char *Formation_names[MAX_FORMATION_NAMES];
extern const char *Team_names[MAX_TEAM_NAMES];
extern int	Team_names_index_xlate[MAX_TEAM_NAMES_INDEX+1];
extern const char *Status_desc_names[MAX_STATUS_NAMES];
extern const char *Status_type_names[MAX_STATUS_NAMES];
extern const char *Status_target_names[MAX_STATUS_NAMES];
extern const char *Arrival_location_names[MAX_ARRIVAL_NAMES];
extern const char *Departure_location_names[MAX_ARRIVAL_NAMES];
extern const char *Goal_type_names[MAX_GOAL_TYPE_NAMES];
extern const char *Species_names[MAX_SPECIES_NAMES];
extern const char *Reinforcement_type_names[];
extern char *Object_flags[];
extern const char *Parse_object_flags[];
extern const char *Icon_names[];

extern char *Cargo_names[MAX_CARGO];
extern char Cargo_names_buf[MAX_CARGO][NAME_LENGTH];

extern char Mission_parse_storm_name[NAME_LENGTH];

extern int	Num_iff;
extern int	Num_ai_behaviors;
extern int	Num_ai_classes;
extern int	Num_cargo;
extern int	Num_status_names;
extern int	Num_arrival_names;
extern int	Num_formation_names;
extern int	Num_goal_type_names;
extern int	Num_team_names;
extern int	Num_reinforcement_type_names;
extern int	Player_starts;
extern fix	Entry_delay_time;

extern ushort Current_file_checksum;
extern int    Current_file_length;

#define SUBSYS_STATUS_NO_CHANGE	-999

typedef struct subsys_status {
	char	name[NAME_LENGTH];
	float	percent;  // percent damaged
	int	primary_banks[MAX_PRIMARY_BANKS];
	int	secondary_banks[MAX_SECONDARY_BANKS];
	int	secondary_ammo[MAX_SECONDARY_BANKS];
	int	ai_class;
	int	subsys_cargo_name;
} subsys_status;

//	a parse object
//	information from a $OBJECT: definition is read into this struct to
// be copied into the real object, ship, etc. structs
typedef struct p_object {
	struct p_object *next, *prev;
	char	name[NAME_LENGTH];
	vector	pos;
	matrix	orient;
	int	ship_class;
	int	iff;
	int	team;
	int	behavior;							// ai_class;
	int	ai_goals;							// sexp of lists of goals that this ship should try and do
	char	cargo1;
	int	status_count;
	int	status_type[MAX_OBJECT_STATUS];
	int	status[MAX_OBJECT_STATUS];
	int	target[MAX_OBJECT_STATUS];
	int	arrival_location;
	int	arrival_distance;					// used when arrival location is near or in front of some ship
	int	arrival_anchor;					// ship used for anchoring an arrival point
	int	arrival_cue;						//	Index in Sexp_nodes of this sexp.
	int	arrival_delay;
	int	subsys_index;						// index into subsys_status array
	int	subsys_count;						// number of elements used in subsys_status array
	int	initial_velocity;
	int	initial_hull;
	int	initial_shields;

	int	departure_location;
	int	departure_anchor;
	int	departure_cue;						//	Index in Sexp_nodes of this sexp.
	int	departure_delay;

	char	misc[NAME_LENGTH];
	int	determination;
	int	wingnum;								// set to -1 if not in a wing -- Wing array index otherwise
	int	flags;								// mission savable flags
	int	escort_priority;					// priority in escort list
	int	ai_class;
	int	hotkey;								// hotkey number (between 0 and 9) -1 means no hotkey
	int	score;
	int	orders_accepted;					// which orders this ship will accept from the player
	char	docked_with[NAME_LENGTH];
	char	docker_point[NAME_LENGTH];
	char	dockee_point[NAME_LENGTH];
	int	group;								// group object is within or -1 if none.
	int	persona_index;
	float	kamikaze_damage;					// base damage for a kamikaze attack
	int	special_exp_index;
	ushort net_signature;					// network signature this object can have

	char	wing_status_wing_index;			// wing index (0-4) in wingman status gauge
	char	wing_status_wing_pos;			// wing position (0-5) in wingman status gauge

	uint	respawn_count;						// number of respawns for this object.  Applies only to player wing ships in multiplayer
	int	respawn_priority;					// priority this ship has for controlling respawn points

	char	alt_type_index;					// optional alt type index
} p_object;

// defines for flags used for p_objects when they are created.  Used to help create
// special circumstances for those ships.  The order that these bitfields appear
// in this list MUST match the order that they appear in the flags array in MissionParse.cpp!!!!

#define MAX_PARSE_OBJECT_FLAGS	20

#define P_OF_CARGO_KNOWN			(1<<0)
#define P_SF_IGNORE_COUNT			(1<<1)
#define P_OF_PROTECTED				(1<<2)
#define P_SF_REINFORCEMENT			(1<<3)
#define P_OF_NO_SHIELDS				(1<<4)
#define P_SF_ESCORT					(1<<5)
#define P_OF_PLAYER_START			(1<<6)
#define P_SF_NO_ARRIVAL_MUSIC		(1<<7)
#define P_SF_NO_ARRIVAL_WARP		(1<<8)
#define P_SF_NO_DEPARTURE_WARP	(1<<9)
#define P_SF_LOCKED					(1<<10)
#define P_SF_INVULNERABLE			(1<<11)
#define P_SF_HIDDEN_FROM_SENSORS	(1<<12)
#define P_SF_SCANNABLE				(1<<13)	// ship is a "scannable" ship
#define P_AIF_KAMIKAZE				(1<<14)
#define P_AIF_NO_DYNAMIC			(1<<15)
#define P_SF_RED_ALERT_STORE_STATUS	(1<<16)
#define P_OF_BEAM_PROTECTED		(1<<17)
#define P_SF_GUARDIAN				(1<<18)
#define P_KNOSSOS_WARP_IN			(1<<19)


// the following parse object flags are used internally by Freespace
#define P_SF_USE_UNIQUE_ORDERS	(1<<26)	// tells a newly created ship to use the default orders for that ship
#define P_SF_INITIALLY_DOCKED		(1<<27)	// is this parse object initially docked with something else
#define P_SF_CANNOT_ARRIVE			(1<<28)	// used to indicate that this ship's arrival cue will never be true
#define P_SF_WARP_BROKEN			(1<<29)	// warp engine should be broken for this ship
#define P_SF_WARP_NEVER				(1<<30)	// warp drive is destroyed
#define P_SF_PLAYER_START_VALID	(1<<31)	// this is a valid player start object

extern p_object ship_arrival_list;			// used by sexpression parser

#if defined(FS2_DEMO) || defined(FS1_DEMO)
	#define MAX_SHIP_ARRIVALS		45
	#define MAX_WING_ARRIVALS		10
#else 
	#define MAX_SHIP_ARRIVALS		90			// maximum of 90 objects can arrive later
	#define MAX_WING_ARRIVALS		20			// maximum of 20 wings can arrive later
#endif

extern p_object ship_arrivals[MAX_SHIP_ARRIVALS];
extern int num_ship_arrivals;

extern p_object Support_ship_pobj, *Arriving_support_ship;

typedef struct {
	int		default_ship;  // default ship type for player start point (recommended choice)
	int		number_choices; // number of ship choices inside ship_list
	int		ship_list[MAX_SHIP_TYPES];
	int		ship_count[MAX_SHIP_TYPES];
	int		weaponry_pool[MAX_WEAPON_TYPES];
} team_data;

#define MAX_P_WINGS		16
#define MAX_SHIP_LIST	16

#define TOKEN_LENGTH	32

extern team_data Team_data[MAX_TEAMS];
extern subsys_status Subsys_status[MAX_SUBSYS_STATUS];
extern int Subsys_index;

extern vector Parse_viewer_pos;
extern matrix Parse_viewer_orient;

extern int Mission_arrival_timestamp;
extern int Mission_departure_timestamp;
extern fix Mission_end_time;

extern char Parse_names[MAX_SHIPS + MAX_WINGS][NAME_LENGTH];
extern int Num_parse_names;
extern int Num_teams;

extern char			Player_start_shipname[NAME_LENGTH];
extern int			Player_start_shipnum;
extern p_object	Player_start_pobject;

extern int Mission_palette;  // index of palette file to use for mission
extern int Nebula_index;  // index into Nebula_filenames[] of nebula to use in mission.
extern const char *Nebula_filenames[NUM_NEBULAS];
extern const char *Nebula_colors[NUM_NEBULA_COLORS];
extern p_object *Arriving_support_ship;

extern char Neb2_texture_name[MAX_FILENAME_LEN];

int parse_main(const char *mission_name, int flags = 0);
int mission_parse_ship_arrived(const char *shipname);
p_object *mission_parse_get_arrival_ship( const char *name );
p_object *mission_parse_get_arrival_ship( ushort net_signature );
p_object *mission_parse_get_original_ship( ushort net_signature );
int parse_create_object(p_object *objp);

// used in squadmate messaging stuff to create wings from reinforcements.
int parse_wing_create_ships(wing *wingp, int num_to_create, int force = 0, int specific_instance = -1 );

// function for getting basic mission data without loading whole mission
int mission_parse_is_multi(const char *filename, char *mission_name );
int mission_parse_get_multi_mission_info(const char *filename);

// called externally from multiplayer code
void mission_do_departure(object *objp);

// called externally from Freespace.cpp
void mission_parse_fixup_players(void);

// get a index to a perminently kept around name of a ship or wing
int get_parse_name_index(const char *name);

// called from freespace game level loop
void mission_parse_eval_stuff();

// function to set the ramaing time left in the mission
void mission_parse_set_end_time( int seconds );

// code to warp in a repair ship.
void mission_warp_in_support_ship( object *requester_objp );
int mission_is_support_ship_arriving( void );
void mission_add_to_arriving_support( object *requester_objp );
int mission_is_repair_scheduled( object *objp );
int mission_remove_scheduled_repair( object *objp );
void mission_parse_support_arrived( int objnum );

// alternate name stuff
int mission_parse_lookup_alt(char *name);
void mission_parse_lookup_alt_index(int index, char *out);
int mission_parse_add_alt(char *name);
void mission_parse_reset_alt();

// code to save/restore mission parse stuff
int get_mission_info(char *filename, mission *missionp = NULL);

#endif

