/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Ship/AiCode.cpp $
 * $Revision: 310 $
 * $Date: 2010-02-08 09:09:25 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 * 
 * AI code that does interesting stuff
 *
 * $Log$
 * Revision 1.11  2006/04/26 19:45:22  taylor
 * fix a FS1 docked speed issue, should keep it in line with the original now
 *
 * Revision 1.10  2005/03/29 02:18:47  taylor
 * Various 64-bit platform fixes
 * Fix compiler errors with MAKE_FS1 and fix gr_set_bitmap() too
 * Make sure that turrets can fire at asteroids for FS1 (needed for a couple missions)
 * Streaming audio support (big thanks to Pierre Willenbrock!!)
 * Removed dependance on strings.tbl for FS1 since we don't actually need it now
 *
 * Revision 1.9  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.8  2003/08/03 16:10:30  taylor
 * cleanup; compile warning fixes
 *
 * Revision 1.7  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.6  2002/07/13 19:47:02  theoddone33
 * Fix some more warnings
 *
 * Change demo building, edit Makefile if you want the demo.
 *
 * Revision 1.5  2002/06/17 06:33:10  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.4  2002/06/09 04:41:26  relnev
 * added copyright header
 *
 * Revision 1.3  2002/06/01 07:12:34  relnev
 * a few NDEBUG updates.
 *
 * removed a few warnings.
 *
 * Revision 1.2  2002/05/03 13:34:33  theoddone33
 * More stuff compiles
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 107   9/15/99 4:42a Mikek
 * Make any big ship attacking Colossus, or Colossus attacking any large
 * ship not use big cruiser movement code.
 * 
 * 106   9/15/99 3:28a Jimb
 * Make all big ships in sm3-08 not do cruiser chase code when attacking
 * Colossus.  Added so Beast doesn't swerve away from Colossus.
 * 
 * 105   9/14/99 4:18p Andsager
 * hack for mission sm3-08 to abort cruiser_chase as sathanas is about to
 * begin circling colossus.
 * 
 * 104   9/08/99 10:44p Andsager
 * Make HUGE ships not die when warping out, after warp effect started.
 * 
 * 103   9/03/99 11:40p Mikek
 * Comment out an annoying nprintf().
 * 
 * 102   9/01/99 11:26p Dave
 * Fixed release build warnings.
 * 
 * 101   9/01/99 9:12p Mikek
 * Make it a boatload harder to become a traitor from hitting a large
 * ship.
 * 
 * 100   9/01/99 4:01p Andsager
 * Make sure BIG|HUGE ships do not respond to shockwaves
 * 
 * 99    9/01/99 10:09a Dave
 * Pirate bob.
 * 
 * 98    8/31/99 4:24p Andsager
 * Reduce collisions when attacking big ships.
 * 
 * 97    8/31/99 7:33a Mikek
 * Improvements in formation flying, less silly behavior, especially when
 * leader is moving very slowly.
 * 
 * 96    8/31/99 5:48a Mikek
 * Making ships not overshoot so much in formation flying.  Intermediate
 * checkin.
 * 
 * 95    8/30/99 12:03a Mikek
 * Make guard behavior much less annoying.  Guarders don't get quite so
 * close and they try to avoid striking the target they are guarding.
 * 
 * 94    8/29/99 4:18p Andsager
 * New "burst" limit for friendly damage.  Also credit more damage done
 * against large friendly ships.
 * 
 * 93    8/28/99 7:29p Dave
 * Fixed wingmen persona messaging. Make sure locked turrets don't count
 * towards the # attacking a player.
 * 
 * 92    8/26/99 10:46p Andsager
 * Apply shockwave damage to lethality.
 * 
 * 91    8/26/99 8:52p Dave
 * Gave multiplayer TvT messaging a heavy dose of sanity. Cheat codes.
 * 
 * 90    8/26/99 5:14p Andsager
 * 
 * 89    8/24/99 8:55p Dave
 * Make sure nondimming pixels work properly in tech menu.
 * 
 * 88    8/23/99 6:21p Jefff
 * added "no traitor" option to missions (and fred)
 * 
 * 87    8/20/99 3:36p Andsager
 * Make sure we don;t miss stealth sweep points.
 * 
 * 86    8/16/99 8:21a Andsager
 * fix link error
 * 
 * 85    8/16/99 8:19a Andsager
 * Add project_point_onto_bbox() to fvi and include in aicode
 * 
 * 84    8/15/99 1:30p Dave
 * Removed some bounding box code because of link errors. Assuming needed
 * function just needs to get checked in by DaveA.
 * 
 * 83    8/15/99 11:59a Andsager
 * For targing big/huge ships, find nearest distance to bbox, not center.
 * 
 * 82    8/13/99 2:20p Andsager
 * Add speed modification to chances turret will find stealth ship
 * 
 * 81    8/13/99 10:49a Andsager
 * Knossos and HUGE ship warp out.  HUGE ship warp in.  Stealth search
 * modes dont collide big ships.
 * 
 * 80    8/10/99 5:02p Andsager
 * Fix bug where AI gets stuck in SM_EVADE_WEAPON with no target.
 * 
 * 79    8/10/99 11:58a Andsager
 * Allow turrets to sometimes see stealth.
 * 
 * 78    7/31/99 2:57p Dave
 * Scaled flak aim and jitter by weapon subsystem strength.
 * 
 * 77    7/27/99 10:33p Andsager
 * improve ai for attacking stealth.  reduced jitter in aim.  reduced
 * error in position when avoiding.  skill level support for attacking
 * stealth.  Made target error same for team vs. team.
 * 
 * 76    7/27/99 10:49a Andsager
 * Make turret fire rate independent of team for HUGE turrets, and also
 * for mult team vs. team.
 * 
 * 75    7/26/99 12:14p Andsager
 * Apply cap to how much slower a transport flies with cargo.  Remove
 * limit on waypoint speed for training.  Enemy ai get stealth exact pos
 * when stealth fires
 * 
 * 74    7/20/99 1:49p Dave
 * Peter Drake build. Fixed some release build warnings.
 * 
 * 73    7/19/99 2:13p Dave
 * Added some new strings for Heiko.
 * 
 * 72    7/19/99 12:02p Andsager
 * Allow AWACS on any ship subsystem. Fix sexp_set_subsystem_strength to
 * only blow up subsystem if its strength is > 0
 * 
 * 71    7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 70    7/14/99 1:44p Andsager
 * modify ai_guard for BIG ships to circle around the long axis
 * 
 * 69    7/09/99 5:54p Dave
 * Seperated cruiser types into individual types. Added tons of new
 * briefing icons. Campaign screen.
 * 
 * 68    7/08/99 4:32p Andsager
 * fix bug with turret-tagged-only
 * 
 * 67    7/08/99 12:06p Andsager
 * Add turret-tagged-only and turret-tagged-clear sexp.
 * 
 * 66    7/02/99 3:49p Andsager
 * Remove debug code.  Allow targeting of stealth from any weapon it
 * fires.
 * 
 * 65    7/02/99 2:01p Andsager
 * Fix bug where big ship tries to evade dumpfire weapon.
 * 
 * 64    7/02/99 10:58a Andsager
 * Put in big ship - big ship attack mode.  Modify stealth sweep ai.
 * 
 * 63    6/30/99 5:53p Dave
 * Put in new anti-camper code.
 * 
 * 62    6/28/99 3:22p Anoop
 * Fix turret optimization, where ship may not have any valid subsystems
 * (all blown off).
 * 
 * 61    6/25/99 5:56p Andsager
 * First real pass on stealth ai.
 * 
 * 60    6/25/99 3:08p Dave
 * Multiple flyby sounds.
 * 
 * 59    6/25/99 1:12p Danw
 * DKA:  Make sure big ship has subsystems before trying to target them.
 * 
 * 58    6/25/99 10:56a Johnson
 * Fixed dumb ai code.
 * 
 * 57    6/24/99 5:15p Dave
 * Make sure stride is always at least one for checking turret subsystem
 * targets.
 * 
 * 56    6/24/99 4:59p Dave
 * Significant speedups to turret firing.
 * 
 * 55    6/23/99 5:51p Andsager
 * Add waypoint-cap-speed.  Checkin stealth ai - inactive.
 * 
 * 54    6/16/99 10:21a Dave
 * Added send-message-list sexpression.
 * 
 * 53    6/15/99 9:25a Andsager
 * Make guard and dynamic chase (who hit you) work with stealth
 * 
 * 52    6/14/99 3:21p Andsager
 * Allow collisions between ship and its debris.  Fix up collision pairs
 * when large ship is warping out.
 * 
 * 51    6/14/99 10:45a Dave
 * Made beam weapons specify accuracy by skill level in the weapons.tbl
 * 
 * 50    6/03/99 8:11a Andsager
 * 
 * 49    6/02/99 5:41p Andsager
 * Reduce range of secondary weapons not fired from turrets in nebula.
 * Reduce range of beams fired from turrrets in nebula
 * 
 * 48    6/02/99 3:23p Andsager
 * Make AI aware of team visibility.  Allow player targeting with team
 * visibility info.  Make stealth ships not targetable by AI in nebula
 * unless tagged.
 * 
 * 47    6/02/99 12:52p Andsager
 * Added team-wide ship visibility.  Implemented for player.
 * 
 * 46    6/01/99 8:35p Dave
 * Finished lockarm weapons. Added proper supercap weapons/damage. Added
 * awacs-set-radius sexpression.
 * 
 * 45    5/28/99 5:35p Andsager
 * Make ai nebula aware
 * 
 * 44    5/24/99 9:55a Dave
 * Fixed stream weapon ai firing problem. ick.
 * 
 * 43    5/20/99 7:00p Dave
 * Added alternate type names for ships. Changed swarm missile table
 * entries.
 * 
 * 42    5/18/99 1:30p Dave
 * Added muzzle flash table stuff.
 * 
 * 41    5/12/99 2:55p Andsager
 * Implemented level 2 tag as priority in turret object selection
 * 
 * 40    5/12/99 10:42a Andsager
 * Fix turret bug allowing HUGE turrets to fire at fighters
 * 
 * 39    5/06/99 11:46a Andsager
 * Bug fixes.  Don't get into illegal strafe submode.  Don't choose turret
 * enemy objnum for beam protected.
 * 
 * 38    5/03/99 10:50p Andsager
 * Make Asteroid_obj_list.  Change get_nearest_turret_objnum() to use
 * Asteroid_obj_list, Ship_obj_list and Missile_obj_list vs.
 * obj_used_list.
 * 
 * 37    4/29/99 2:29p Dave
 * Made flak work much better in multiplayer.
 * 
 * 36    4/28/99 11:36p Dave
 * Tweaked up subspace missile strike a bit,
 * 
 * 35    4/28/99 3:11p Andsager
 * Stagger turret weapon fire times.  Make turrets smarter when target is
 * protected or beam protected.  Add weaopn range to weapon info struct.
 * 
 * 34    4/26/99 10:58a Andsager
 * Add OF_BEAM_PROTECTED flag to keep object from being targeted for zing.
 * 
 * 33    4/23/99 12:12p Andsager
 * Modify wing positions when player is wing leader to prevent some
 * collisions.
 * 
 * 32    4/23/99 12:01p Johnson
 * Added SIF_HUGE_SHIP
 * 
 * 31    4/22/99 11:06p Dave
 * Final pass at beam weapons. Solidified a lot of stuff. All that remains
 * now is to tweak and fix bugs as they come up. No new beam weapon
 * features.
 * 
 * 30    4/20/99 6:39p Dave
 * Almost done with artillery targeting. Added support for downloading
 * images on the PXO screen.
 * 
 * 29    4/20/99 3:40p Andsager
 * Changes to big ship ai.  Uses bounding box as limit where to fly to
 * when flying away.
 * 
 * 28    4/16/99 5:54p Dave
 * Support for on/off style "stream" weapons. Real early support for
 * target-painting lasers.
 * 
 * 27    4/02/99 9:55a Dave
 * Added a few more options in the weapons.tbl for beam weapons. Attempt
 * at putting "pain" packets into multiplayer.
 * 
 * 26    3/28/99 5:58p Dave
 * Added early demo code. Make objects move. Nice and framerate
 * independant, but not much else. Don't use yet unless you're me :)
 * 
 * 25    3/19/99 9:51a Dave
 * Checkin to repair massive source safe crash. Also added support for
 * pof-style nebulae, and some new weapons code.
 * 
 * 24    3/08/99 7:03p Dave
 * First run of new object update system. Looks very promising.
 * 
 * 23    3/05/99 3:55p Anoop
 * Handle some asserts properly.
 * 
 * 22    3/04/99 6:09p Dave
 * Added in sexpressions for firing beams and checking for if a ship is
 * tagged.
 * 
 * 21    3/02/99 9:25p Dave
 * Added a bunch of model rendering debug code. Started work on fixing
 * beam weapon wacky firing.
 * 
 * 20    2/25/99 2:32p Anoop
 * (Alan). Fixed ai path following code for AI_BAY_EMERGE. Put in sanity
 * check so that when the last point on the path is reached, it finishes.
 * 
 * 19    2/19/99 2:11p Anoop
 * Put in some nice handling code for wacky support ship problems (like no
 * docking paths)
 * 
 * 18    2/17/99 2:11p Dave
 * First full run of squad war. All freespace and tracker side stuff
 * works.
 * 
 * 17    2/11/99 5:22p Andsager
 * Fixed bugs, generalized block Sexp_variables
 * 
 * 16    1/29/99 5:07p Dave
 * Fixed multiplayer stuff. Put in multiplayer support for rapid fire
 * missiles.
 * 
 * 15    1/29/99 2:25p Andsager
 * Added turret_swarm_missiles
 * 
 * 14    1/27/99 9:56a Dave
 * Temporary checkin of beam weapons for Dan to make cool sounds.
 * 
 * 13    1/24/99 11:37p Dave
 * First full rev of beam weapons. Very customizable. Removed some bogus
 * Int3()'s in low level net code.
 * 
 * 12    1/21/99 10:44a Dave
 * More beam weapon stuff. Put in warmdown time.
 * 
 * 11    1/12/99 5:45p Dave
 * Moved weapon pipeline in multiplayer to almost exclusively client side.
 * Very good results. Bandwidth goes down, playability goes up for crappy
 * connections. Fixed object update problem for ship subsystems.
 * 
 * 10    1/08/99 2:08p Dave
 * Fixed software rendering for pofview. Super early support for AWACS and
 * beam weapons.
 * 
 * 9     12/23/98 2:53p Andsager
 * Added ship activation and gas collection subsystems, removed bridge
 * 
 * 8     11/12/98 12:13a Dave
 * Tidied code up for multiplayer test. Put in network support for flak
 * guns.
 * 
 * 7     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 6     10/26/98 9:42a Dave
 * Early flak gun support.
 * 
 * 5     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
 * 
 * 4     10/20/98 1:39p Andsager
 * Make so sparks follow animated ship submodels.  Modify
 * ship_weapon_do_hit_stuff() and ship_apply_local_damage() to add
 * submodel_num.  Add submodel_num to multiplayer hit packet.
 * 
 * 3     10/13/98 9:29a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 
 * $NoKeywords: $
 */

// This module contains the actual AI code that does interesting stuff
// to objects.   The code in Ai.cpp is just for bookeeping, allocating
// ai slots and linking them to ships.

#include "pstypes.h"
#include "fix.h"
#include "linklist.h"
#include "object.h"
#include "physics.h"
#include "vecmat.h"
#include "ship.h"
#include "model.h"
#include "2d.h"
#include "3d.h"
#include "ai.h"
#include "floating.h"
#include "player.h"
#include "freespace.h"
#include "weapon.h"
#include "missiongoals.h"
#include "missionlog.h"
#include "timer.h"
#include "sound.h"
#include "aigoals.h"
#include "gamesnd.h"
#include "hudmessage.h"
#include "missionmessage.h"
#include "cmeasure.h"
#include "staticrand.h"
#include "multimsgs.h"
#include "afterburner.h"
#include "hudets.h"
#include "shipfx.h"
#include "shiphit.h"
#include "aibig.h"
#include "multiutil.h"
#include "hud.h"
#include "objcollide.h"
#include "asteroid.h"
#include "hudlock.h"
#include "missiontraining.h"
#include "gamesequence.h"
#include "joy_ff.h"
#include "localize.h"
#include "flak.h"
#include "beam.h"
#include "multi.h"
#include "swarm.h"
#include "multi_team.h"
#include "awacs.h"
#include "fvi.h"

#ifndef PLAT_UNIX
#pragma optimize("", off)
#pragma auto_inline(off)
#endif

#define	UNINITIALIZED_VALUE	-99999.9f

#define INSTRUCTOR_SHIP_NAME NOX("instructor")

#define AICODE_SMALL_MAGNITUDE	0.001f		// cosider a vector NULL if mag is less than this

#define NEXT_REARM_TIMESTAMP (60*1000)			//	Ships will re-request rearm, typically, after this long.

#define BEAM_NEBULA_RANGE_REDUCE_FACTOR		0.8

// AIM_CHASE submode defines
// SM_STEALTH_FIND
#define	SM_SF_AHEAD		0
#define	SM_SF_BEHIND	1
#define	SM_SF_BAIL		2

// SM_STEALTH_SWEEP
#define	SM_SS_SET_GOAL	-1
#define	SM_SS_BOX0		0
#define	SM_SS_LR			1
#define	SM_SS_UL			2
#define	SM_SS_BOX1		3
#define	SM_SS_UR			4
#define	SM_SS_LL			5
#define	SM_SS_BOX2		6
#define	SM_SS_DONE		7

//XSTR:OFF

const char *Mode_text[MAX_AI_BEHAVIORS] = {
	"CHASE",
	"EVADE",
	"GET_BEHIND",
	"CHASE_LONG",
	"SQUIGGLE",
	"GUARD",
	"AVOID",
	"WAYPOINTS",
	"DOCK",
	"NONE",
	"BIGSHIP",
	"PATH",
	"BE_REARMED",
	"SAFETY",
	"EV_WEAPON",
	"STRAFE",
	"PLAY_DEAD",
	"BAY_EMERGE",
	"BAY_DEPART",
	"SENTRYGUN",
	"WARP_OUT",
};

//	Submode text is only valid for CHASE mode.
const char *Submode_text[] = {
"undefined",
"CONT_TURN",
"ATTACK   ",
"E_SQUIG  ",
"E_BRAKE  ",
"EVADE    ",
"SUP_ATTAK",
"AVOID    ",
"BEHIND   ",
"GET_AWAY ",
"E_WEAPON ",
"FLY_AWAY ",
"ATK_4EVER",
"STLTH_FND",
"STLTH_SWP",
"BIG_APPR",
"BIG_CIRC",
"BIG_PARL"
};

const char *Strafe_submode_text[5] = {
"ATTACK",
"AVOID",
"RETREAT1",
"RETREAT2",
"POSITION"
};
//XSTR:ON

/*
//	Used for global ignore of objects.  If an object appears in the Ignore_objects array,
//	no one will attack it.
#define	MAX_IGNORE_OBJECTS	16
typedef struct {
	int	objnum;
	int	signature;
} ignore_object;

ignore_object	Ignore_objects[MAX_IGNORE_OBJECTS];
*/

typedef struct eval_enemy_obj_struct {
	int			turret_parent_objnum;			// parent of turret
	float			weapon_travel_dist;				// max targeting range of turret weapon
	int			enemy_team_mask;
	int			weapon_system_ok;					// is the weapon subsystem of turret ship ok
	int			big_only_flag;						// turret fires only at big and huge ships
	vector		*tpos;
	vector		*tvec;
	ship_subsys *turret_subsys;
	int			current_enemy;


	float			nearest_attacker_dist;			// nearest ship	
	int			nearest_attacker_objnum;

	float			nearest_homing_bomb_dist;		// nearest homing bomb
	int			nearest_homing_bomb_objnum;

	float			nearest_bomb_dist;				// nearest non-homing bomb
	int			nearest_bomb_objnum;

	float			nearest_dist;						// nearest ship attacking this turret
	int			nearest_objnum;
}	eval_enemy_obj_struct;


control_info	AI_ci;

object *Pl_objp;
object *En_objp;

waypoint_list Waypoint_lists[MAX_WAYPOINT_LISTS];

// How close a turret has to be point at its target before it
// can fire.  If the dot of the gun normal and the vector from gun
// to target is greater than this, the turret fires.  The smaller
// the sloppier the shooting.
#define AICODE_TURRET_DUMBFIRE_ANGLE		(0.8f)	
#define AICODE_TURRET_HEATSEEK_ANGLE		(0.7f)	
#define AICODE_TURRET_MAX_TIME_IN_RANGE	(5.0f)

#define	REARM_SOUND_DELAY		(3*F1_0)		//	Amount of time to delay rearm/repair after mode start
#define	REARM_BREAKOFF_DELAY	(3*F1_0)		//	Amount of time to wait after fully rearmed to breakoff.

#define	MIN_DIST_TO_WAYPOINT_GOAL	5.0f
#define	MAX_GUARD_DIST					250.0f
#define	BIG_GUARD_RADIUS				500.0f

#define	MAX_EVADE_TIME			(15 * 1000)	//	Max time to evade a weapon.

// defines for repair ship stuff.
#define	MAX_REPAIR_SPEED			25.0f
#define	MAX_UNDOCK_ABORT_SPEED	2.0f

// defines for EMP effect stuff
#define	MAX_EMP_INACCURACY		50.0f

// defines for stealth
#define	MAX_STEALTH_INACCURACY	50.0f		// at max view dist
#define	STEALTH_MAX_VIEW_DIST	400		// dist at which 1) stealth no longer visible 2) firing inaccuracy is greatest
#define	STEALTH_VIEW_CONE_DOT	0.707		// (half angle of 45 degrees)


ai_class	Ai_classes[MAX_AI_CLASSES];
int	Ai_firing_enabled = 1;
int	Num_ai_classes;

int	AI_FrameCount = 0;
int	Ship_info_inited = 0;
int	AI_watch_object = 0; // Debugging, object to spew debug info for.
int	Num_waypoint_lists = 0;
int	Mission_all_attack = 0;					//	!0 means all teams attack all teams.

const char *Skill_level_names(int level, int translate)
{
	const char *str = NULL;

	#if NUM_SKILL_LEVELS != 5
	#error Number of skill levels is wrong!
	#endif

	if(translate){
		switch( level )	{
		case 0:
			str = XSTR("Very Easy", 469);
			break;
		case 1:
			str = XSTR("Easy", 470);
			break;
		case 2:
			str = XSTR("Medium", 471);
			break;
		case 3:
			str = XSTR("Hard", 472);
			break;
		case 4:
			str = XSTR("Insane", 473);
			break;
		default:	
			Int3();
		}
	} else {
		switch( level )	{
		case 0:
			str = NOX("Very Easy");
			break;
		case 1:
			str = NOX("Easy");
			break;
		case 2:
			str = NOX("Medium");
			break;
		case 3:
			str = NOX("Hard");
			break;
		case 4:
			str = NOX("Insane");
			break;
		default:	
			Int3();
		}
	}

	return str;
}

#define	DELAY_TARGET_TIME	(12*1000)		//	time in milliseconds until a ship can target a new enemy after an order.

//	Make enemy ships turn more slowly at lower skill levels.
float	Turn_time_skill_level_scale[NUM_SKILL_LEVELS] = {3.0f, 2.2f, 1.6f, 1.3f, 1.0f};

//	Maximum number of simultaneous homing weapons on player based on skill level.
int	Max_allowed_player_homers[NUM_SKILL_LEVELS] = {2, 3, 4, 7, 99};

//	Number of ships that can attack another ship at a given skill level.
int	Skill_level_max_attackers[NUM_SKILL_LEVELS] = {2, 3, 4, 5, 99};

//	How long until next predict position.
fix Skill_level_delay[NUM_SKILL_LEVELS] = {2*F1_0, 3*F1_0/2, 4*F1_0/3, F1_0/2, 0};

//	AI ships link primary weapons if energy levels greater than the following amounts:
float	Link_energy_levels_always[NUM_SKILL_LEVELS] = {100.0f, 80.0f, 60.0f, 40.0f, 20.0f};	//	always link
float	Link_energy_levels_maybe[NUM_SKILL_LEVELS] = {90.0f, 60.0f, 40.0f, 20.0f, 10.0f};	//	link if hull strength low

//	Seconds to add to time it takes to get enemy in range.  Only for player's enemies.
float	In_range_time[NUM_SKILL_LEVELS] = {2.0f, 1.4f, 0.75f, 0.0f, -1.0f};

//	No matter what, a random unit vector gets scaled by this amount in firing at an enemy.
//	Note that for shorter in-range times, these values get scaled, so a value of 0.5f is meaningful.
float	Aiming_error[NUM_SKILL_LEVELS] = {3.0f, 2.2f, 1.3f, 0.7f, 0.2f};

//	Chance a countermeasure will be fired based on skill level.
float Cmeasure_fire_chance[NUM_SKILL_LEVELS] = {0.2f, 0.3f, 0.5f, 0.9f, 1.1f};	//	Note, this gets scaled by ai_class

float Shield_manage_delays[NUM_SKILL_LEVELS] = {5.0f, 4.0f, 2.5f, 1.2f, 0.1f};

// accuracy we feed into the beam weapons based upon skill system
// float Beam_accuracy[NUM_SKILL_LEVELS] = {2.0f, 1.5f, 1.0f, 0.7f, 0.4f};

extern float Ship_fire_delay_scale_hostile[NUM_SKILL_LEVELS];
extern float Ship_fire_delay_scale_friendly[NUM_SKILL_LEVELS];

pnode		Path_points[MAX_PATH_POINTS];
pnode		*Ppfp;			//	Free pointer in path points.

float	AI_frametime;

char *Ai_class_names[MAX_AI_CLASSES];

// global for rearm status for teams
int Ai_friendly_rearm_timestamp, Ai_hostile_rearm_timestamp, Ai_neutral_rearm_timestamp, Ai_traitor_rearm_timestamp, Ai_unknown_rearm_timestamp;

// globals for dealing with when to fire huge secondary weapons
#define MAX_HUGE_SECONDARY_INFO	10

typedef struct {
	int team;
	int weapon_index;
	int max_fire_count;
	char	*shipname;
} huge_fire_info;

huge_fire_info Ai_huge_fire_info[MAX_HUGE_SECONDARY_INFO];

int Ai_last_arrive_path;	// index of ship_bay path used by last arrival from a fighter bay

// forward declarations
int	ai_return_path_num_from_dockbay(object *dockee_objp, int dockbay_index);
void	create_model_exit_path(object *pl_objp, object *mobjp, int path_num, int count=1);
void	copy_xlate_model_path_points(object *objp, model_path *mp, int dir, int count, int path_num, pnode *pnp, int randomize_pnt=-1);

// ai_set_rearm_status takes a team (friendly, hostile, neutral) and a time.  This function
// sets the timestamp used to tell is it is a good time for this team to rearm.  Once the timestamp
// is no longer valid, then rearming is not a "good time"
// not safe.  Called from sexpression code.
void ai_set_rearm_status( int team, int time )
{
	Assert( time >= 0 );

	switch (team) {
	case TEAM_FRIENDLY:
		Ai_friendly_rearm_timestamp = timestamp( time * 1000 );
		break;
	case TEAM_HOSTILE:
		Ai_hostile_rearm_timestamp = timestamp( time * 1000 );
		break;
	case TEAM_NEUTRAL:
		Ai_neutral_rearm_timestamp = timestamp( time * 1000 );
		break;
	case TEAM_TRAITOR:
		Ai_traitor_rearm_timestamp = timestamp( time * 1000 );
		break;
	case TEAM_UNKNOWN:
		Ai_traitor_rearm_timestamp = timestamp( time * 1000 );
		break;
	default:
		Int3();
		break;
	}
}

// int ai_good_time_to_rearm returns true(1) or false(0) if it is "safe" for the given
// object to rearm.  "safe" is currently defined by the mission designer using the good/bad
// time to rearm sexpressions.  This status is currently team based.  This function could
// be easily expended to further the definition of "safe"
int ai_good_time_to_rearm( object *objp )
{
	int team, status;

	Assert(objp->type == OBJ_SHIP);
	team = Ships[objp->instance].team;
	status = 0;

	switch(team) {
	case TEAM_FRIENDLY:
		status = timestamp_valid(Ai_friendly_rearm_timestamp);
		break;
	case TEAM_HOSTILE:
		status = timestamp_valid(Ai_hostile_rearm_timestamp);
		break;
	case TEAM_NEUTRAL:
		status = timestamp_valid(Ai_neutral_rearm_timestamp);
		break;
	case TEAM_TRAITOR:
		status = timestamp_valid(Ai_traitor_rearm_timestamp);
		break;
	case TEAM_UNKNOWN:
		status = timestamp_valid(Ai_unknown_rearm_timestamp);
		break;
	default:
		Int3();
		break;
	}

	return status;
}

// functions to deal with letting the ai know about good times to fire powerful secondary
// weapons.

// this function is entry point from sexpression code to set internal data for use by ai code.
void ai_good_secondary_time( int team, int weapon_index, int max_fire_count, char *shipname )
{
	int i, index;

	// find an open slot to put this data
	for ( i = 0; i < MAX_HUGE_SECONDARY_INFO; i++ ) {
		if ( Ai_huge_fire_info[i].weapon_index == -1 )
			break;
	}

	Assert( i < MAX_HUGE_SECONDARY_INFO );			// we've run out of room

	Ai_huge_fire_info[i].weapon_index = weapon_index;
	Ai_huge_fire_info[i].team = team;
	Ai_huge_fire_info[i].max_fire_count = max_fire_count;

	Ai_huge_fire_info[i].shipname = ai_get_goal_ship_name( shipname, &index );
}

// function called internally to the ai code to tell whether or not weapon_num can be fired
// from firer_objp at target_objp.  This function will resolve the team for the firer.
// returns:
//		-1  -- when conditions don't allow firer to fire weapon_num on target_objp
//		>=0 -- when conditions allow firer to fire.  Return value is max number of weapon_nums
//           which can be fired on target_objp
int is_preferred_weapon(int weapon_num, object *firer_objp, object *target_objp)
{
	int i, firer_team, target_signature;
	ship *firer_ship;
	huge_fire_info *hfi = NULL;

	Assert( firer_objp->type == OBJ_SHIP );
	firer_ship = &Ships[firer_objp->instance];
	firer_team = firer_ship->team;

	// get target object's signature and try to find it in the list.
	target_signature = target_objp->signature;
	for ( i = 0; i < MAX_HUGE_SECONDARY_INFO; i++ ) {
		int ship_index, signature;

		hfi = &Ai_huge_fire_info[i];
		if ( hfi->weapon_index == -1 )
			continue;

		ship_index = ship_name_lookup( hfi->shipname );
		if ( ship_index == -1 )
			continue;

		signature = Objects[Ships[ship_index].objnum].signature;

		// sigatures, weapon_index, and team must match
		if ( (signature == target_signature) && (hfi->weapon_index == weapon_num) && (hfi->team == firer_team) )
			break;
	}

	// return -1 if not found
	if ( i == MAX_HUGE_SECONDARY_INFO )
		return -1;

	// otherwise, we can return the max number of weapons we can fire against target_objps

	return hfi->max_fire_count;
}

// function to clear out secondary firing infomration between levels
void ai_init_secondary_info()
{
	int i;

	// clear out the data for dealing with when ai ships can fire huge secondary weapons
	for (i = 0; i < MAX_HUGE_SECONDARY_INFO; i++ ) {
		Ai_huge_fire_info[i].weapon_index = -1;
		Ai_huge_fire_info[i].team = -1;
		Ai_huge_fire_info[i].max_fire_count = -1;
		Ai_huge_fire_info[i].shipname = NULL;
	}
}


//	Garbage collect the Path_points buffer.
//	Scans all objects, looking for used Path_points records.
//	Compresses Path_points buffer, updating aip->path_start and aip->path_cur indices.
//	Updates Ppfp to point to first free record.
//	This function is fairly fast.  Its worst-case running time is proportional to
//	3*MAX_PATH_POINTS + MAX_OBJECTS
//	Things to do to optimize this function:
//		1. if (t != 0) xlt++; can be replaced by xlt += t; assuming t can only be 0 or 1.
//		2. When pp_xlate is getting stuffed the first time, note highest index and use that 
//			instead of MAX_PATH_POINTS in following two for loops.
void garbage_collect_path_points()
{
	int	i;
	int	pp_xlate[MAX_PATH_POINTS];
	object	*A;
	ship_obj	*so;

	//	Scan all objects and create Path_points xlate table.
	for (i=0; i<MAX_PATH_POINTS; i++)
		pp_xlate[i] = 0;

	//	in pp_xlate, mark all used Path_point records
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		A = &Objects[so->objnum];
		ship	*shipp = &Ships[A->instance];
		if (shipp->ai_index != -1) {
			ai_info	*aip = &Ai_info[shipp->ai_index];

			if ((aip->path_length > 0) && (aip->path_start > -1)) {

				for (int i=aip->path_start; i<aip->path_start + aip->path_length; i++) {
					Assert(pp_xlate[i] == 0);	//	If this is not 0, then two paths use this point!
					pp_xlate[i] = 1;
				}
			}
		}
	}

	//	Now, stuff xlate index in pp_xlate.  This is the number to translate any path_start
	//	or path_cur index to.
	int	xlt = 0;
	for (i=0; i<MAX_PATH_POINTS; i++) {
		int	t = pp_xlate[i];

		pp_xlate[i] = xlt;
		if (t != 0)
			xlt++;
	}
	
	//	Update global Path_points free pointer.
	Ppfp = &Path_points[xlt];

	//	Now, using pp_xlate, fixup all aip->path_cur and aip->path_start indices
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		A = &Objects[so->objnum];
		ship	*shipp = &Ships[A->instance];
		if (shipp->ai_index != -1) {
			ai_info	*aip = &Ai_info[shipp->ai_index];

			if ((aip->path_length > 0) && (aip->path_start > -1)) {
				Assert(aip->path_start < MAX_PATH_POINTS);
				aip->path_start = pp_xlate[aip->path_start];

				Assert((aip->path_cur >= 0) && (aip->path_cur < MAX_PATH_POINTS));
				aip->path_cur = pp_xlate[aip->path_cur];
			}
		}
	}

	//	Now, compress the buffer.
	for (i=0; i<MAX_PATH_POINTS; i++)
		if (i != pp_xlate[i])
			Path_points[pp_xlate[i]] = Path_points[i];

}

//	Hash two values together, return result.
//	Hash function: curval shifted right circular by one, newval xored in.
int hash(unsigned int curval, int newval)
{
	int	addval = curval & 1;

	curval >>= 1;
	if (addval)
		curval |= 0x80000000;
	curval ^= newval;

	return curval;
}

//	Hash some information in an object together.
//	On 2/20/97, the information is position and orientation.
int create_object_hash(object *objp)
{
	int	*ip;
	unsigned int	hashval = 0;
	int	i;

	ip = (int *) &objp->orient;

	for (i=0; i<9; i++) {
		hashval = hash(hashval, *ip);
		ip++;
	}

	ip = (int *) &objp->pos;

	for (i=0; i<3; i++) {
		hashval = hash(hashval, *ip);
		ip++;
	}

	return hashval;
}

//	Stuff a list of NUM_SKILL_LEVELS floats at *plist.
void parse_float_list(float *plist)
{
	int	i;

	for (i=0; i<NUM_SKILL_LEVELS; i++) {
		stuff_float(&plist[i]);
	}
}

void parse_ai_class()
{
	ai_class	*aicp = &Ai_classes[Num_ai_classes];

	required_string("$Name:");
	stuff_string(aicp->name, F_NAME, NULL);

	Ai_class_names[Num_ai_classes] = aicp->name;

	required_string("$accuracy:");
	parse_float_list(aicp->ai_accuracy);

	required_string("$evasion:");
	parse_float_list(aicp->ai_evasion);

	required_string("$courage:");
	parse_float_list(aicp->ai_courage);

	required_string("$patience:");
	parse_float_list(aicp->ai_patience);
}

void parse_aitbl()
{
	// open localization
	lcl_ext_open();

	read_file_text("ai.tbl");

	reset_parse();

	Num_ai_classes = 0;

	required_string("#AI Classes");

	while (required_string_either("#End", "$Name:")) {
		Assert( Num_ai_classes < MAX_AI_CLASSES);

		parse_ai_class();

		Num_ai_classes++;
	}

	// close localization
	lcl_ext_close();
}

LOCAL int ai_inited = 0;

//========================= BOOK-KEEPING FUNCTIONS =======================

// Called once at game start-up
void ai_init()
{
	if ( !ai_inited )	{
		// Do the first time initialization stuff here
		int	rval;

		if ((rval = setjmp(parse_abort)) != 0) {
			Error(LOCATION, "Error parsing 'ai.tbl'\r\nError code = %i.\r\n", rval);
		} else {			
			parse_aitbl();			
		}

		ai_inited = 1;
	}

	init_semirand();
	
	ai_level_init();
}

// this inits the ai.  You should be able to call this between
// levels to reset everything.
void ai_level_init()
{
	int i;
 
	// Do the stuff to reset all ai stuff here
	for (i=0; i<MAX_AI_INFO ; i++) {
		Ai_info[i].shipnum = -1;
	}
	Ai_goal_signature = 0;
	Ai_friendly_rearm_timestamp = timestamp(-1);
	Ai_hostile_rearm_timestamp = timestamp(-1);
	Ai_neutral_rearm_timestamp = timestamp(-1);
	Ai_traitor_rearm_timestamp = timestamp(-1);

	// clear out the stuff needed for AI firing powerful secondary weapons
	ai_init_secondary_info();

	Ai_last_arrive_path=0;
}

// BEGIN STEALTH
// -----------------------------------------------------------------------------
// Check if object is a stealth ship
int is_object_stealth_ship(object* objp)
{
	if (objp->type == OBJ_SHIP) {
		if (Ship_info[Ships[objp->instance].ship_info_index].flags & SIF_STEALTH) {
			return 1;
		}
	}

	// not stealth ship
	return 0;
}

// -----------------------------------------------------------------------------
// Init necessary ai info for new stealth target
void init_ai_stealth_info(ai_info *aip, object *stealth_objp)
{
	Assert(is_object_stealth_ship(stealth_objp));

	// set necessary ai info for new stealth target
	aip->stealth_last_pos = stealth_objp->pos;
	aip->stealth_velocity = stealth_objp->phys_info.vel;
	aip->stealth_last_visible_stamp = timestamp();
}

// -----------------------------------------------------------------------------
// Check whether Pl_objp can see a stealth ship object
#define STEALTH_INVISIBLE			0
#define STEALTH_VISIBLE				1
#define STEALTH_FULLY_TARGETABLE	2

float get_skill_stealth_dist_scaler()
{
	// return dist scaler based on skill level
	switch (Game_skill_level) {
	case 0: // very easy
		return 0.65f;

	case 1:	// easy
		return 0.9f;

	case 2:	// medium
		return 1.0f;

	case 3:	// hard
		return 1.1f;

	case 4:	// insane
		return 1.3f;

	default:
		Int3();
	}

	return 1.0f;
}

float get_skill_stealth_dot_scaler()
{
	// return multiplier on dot based on skill level
	switch (Game_skill_level) {
	case 0: // very easy
		return 1.3f;

	case 1:	// easy
		return 1.1f;

	case 2:	// medium
		return 1.0f;

	case 3:	// hard
		return 0.9f;

	case 4:	// insane
		return 0.7f;

	default:
		Int3();
	}

	return 1.0f;
}

int ai_is_stealth_visible(object *viewer_objp, object *stealth_objp)
{
	ship *shipp;
	vector vec_to_stealth;
	float dot_to_stealth, dist_to_stealth, max_stealth_dist;

	Assert(stealth_objp->type == OBJ_SHIP);
	shipp = &Ships[stealth_objp->instance];
	Assert(viewer_objp->type == OBJ_SHIP);

	// check if stealth ship
	Assert(Ship_info[shipp->ship_info_index].flags & SIF_STEALTH);

	// check if in neb and below awac level for visible
	if ( !ship_is_visible_by_team(stealth_objp->instance, Ships[viewer_objp->instance].team) ) {
		vm_vec_sub(&vec_to_stealth, &stealth_objp->pos, &viewer_objp->pos);
		dist_to_stealth = vm_vec_mag_quick(&vec_to_stealth);
		dot_to_stealth = vm_vec_dotprod(&viewer_objp->orient.v.fvec, &vec_to_stealth) / dist_to_stealth;

		// get max dist at which stealth is visible
		max_stealth_dist = get_skill_stealth_dist_scaler() * STEALTH_MAX_VIEW_DIST;

		// now check if within view frustrum
		float needed_dot_to_stealth;
		if (dist_to_stealth < 100) {
			needed_dot_to_stealth = 0.0f;
		} else {
			needed_dot_to_stealth = get_skill_stealth_dot_scaler() * float(STEALTH_VIEW_CONE_DOT) * (dist_to_stealth / max_stealth_dist);
		}
		if (dot_to_stealth > needed_dot_to_stealth) {
			if (dist_to_stealth < max_stealth_dist) {
				return STEALTH_VISIBLE;
			}
		}

		// not within frustrum
		return STEALTH_INVISIBLE;
	}

	// visible by awacs level
	return STEALTH_FULLY_TARGETABLE;
}

// END STEALTH

//	Compute dot product of direction vector and forward vector.
//	Direction vector is vector from one object to other object.
//	Forward vector is the forward vector of the ship.
//	If from_dot == NULL, don't fill it in.
float compute_dots(object *objp, object *other_objp, float *to_dot, float *from_dot)
{
	vector	v2o;
	float		dist;

	dist = vm_vec_normalized_dir(&v2o, &other_objp->pos, &objp->pos);

	*to_dot = vm_vec_dot(&objp->orient.v.fvec, &v2o);

	if (from_dot != NULL)
		*from_dot = - vm_vec_dot(&other_objp->orient.v.fvec, &v2o);

	return dist;
}

// -----------------------------------------------------------------------------
// update estimated stealth info
// this is a "cheat" update
// error increases with time not seen, true distance away, dot to enemey
// this is done only if we can not see the stealth target
// need to infer its position either by weapon fire pos or last know pos
void update_ai_stealth_info_with_error(ai_info *aip/*, int no_error*/)
{
	object *ship;
	object *stealth_objp;
	/*
	float error_time_mult, error_dist_mult, error_dot_mult, error_mult;
	float pos_error, vel_error;
	vector error_vec, vec_to_stealth;
	float dist_to_stealth, dot_to_stealth;
	float delta_time, delta_capped;
	*/

	// make sure I am targeting a stealth ship
	Assert( is_object_stealth_ship(&Objects[aip->target_objnum]) );
	stealth_objp = &Objects[aip->target_objnum];

	// my_ship
	ship = &Objects[Ships[aip->shipnum].objnum];

	// if update is due to weapon fire, get exact stealth position
//	if (no_error) {
	aip->stealth_last_pos = stealth_objp->pos;
	aip->stealth_velocity = stealth_objp->phys_info.vel;
	aip->stealth_last_visible_stamp = timestamp();
//		return;
//	}
/*
	// get time since last seen
	delta_time = 0.001f * (timestamp() - aip->stealth_last_visible_stamp);

	// we don't want our "cheat" guess to more off than what we would get from extrapolating from last visible
	// only update if stealth info is "old"
	if ( (delta_time) < 0.5 ) {
		return;
	}

	// find vec_to_stealth and dist
	vm_vec_sub(&vec_to_stealth, &stealth_objp->pos, &ship->pos);
	dist_to_stealth = vm_vec_normalize_quick(&vec_to_stealth);
	dot_to_stealth = vm_vec_dotprod(&vec_to_stealth, &ship->orient.v.fvec);

	// put cap on time
	delta_capped = delta_time;
	if (delta_time > 5.0) {
		delta_capped = 5.0f;
	}

	// erorr_time_mult (for 0-5) -> (1-6)
	error_time_mult = (1.0f + delta_capped);

	// error_dot_mult (-1 to 1) -> (1-3)
	error_dot_mult = (2 - dot_to_stealth);

	// error_dist_mult (0-1000+) -> (1-4)
	error_dist_mult = dist_to_stealth * 4.0f * 0.001f;
	if (error_dist_mult < 1) {
		error_dist_mult = 1.0f;
	} else if (error_dist_mult > 4) {
		error_dist_mult = 4.0f;
	}

	// multiply error out
	error_mult = error_time_mult * error_dot_mult * error_dist_mult;

	float base_pos_error = 10;
	float base_vel_error = 2;

	// find the position and velocity error magnitude;
	pos_error = base_pos_error * error_mult;
	vel_error = base_vel_error * error_mult;

	// get an error that changes slowly over time
	static_randvec( ((int)aip ^ (Missiontime >> 18)) & 7, &error_vec);
	vm_vec_zero(&error_vec);

	// update pos and vel with error
	vm_vec_scale_add(&aip->stealth_velocity, &stealth_objp->phys_info.vel, &error_vec, vel_error);

	// revise last "known" position to arrive at last pos with given error
	vm_vec_scale_add(&aip->stealth_last_pos, &stealth_objp->pos, &error_vec, pos_error);
	vm_vec_scale_add2(&aip->stealth_last_pos, &aip->stealth_velocity, -(0.001f * delta_time));
	*/
}

//	Update danger_weapon_objnum and signature in ai_info to say this weapon is to be avoided.
void ai_update_danger_weapon(int attacked_objnum, int weapon_objnum)
{
	object	*objp, *weapon_objp;
	ai_info	*aip;
	float		old_dist, new_dist;
	float		old_dot, new_dot;
	object	*old_weapon_objp = NULL;

	if ((attacked_objnum == -1) || (weapon_objnum == -1)) {
		return;
	}

	objp = &Objects[attacked_objnum];

	// AL 2-24-98: If this isn't a ship, we don't need to worry about updating weapon_objnum (ie it would be
	//					an asteroid or bomb).
	if ( objp->type != OBJ_SHIP ) {
		return;
	}

	weapon_objp = &Objects[weapon_objnum];

	aip = &Ai_info[Ships[objp->instance].ai_index];

	// if my taraget is a stealth ship and is not visible
	if (aip->target_objnum >= 0) {
		if ( is_object_stealth_ship(&Objects[aip->target_objnum]) ) {
			if ( ai_is_stealth_visible(objp, &Objects[aip->target_objnum]) == STEALTH_INVISIBLE ) {
				// and the weapon is coming from that stealth ship
				if (weapon_objp->parent == aip->target_objnum) {
					// update my position estimate for stealth ship
					update_ai_stealth_info_with_error(aip/*, 1*/);
				}
			}
		}
	}

	if (aip->danger_weapon_objnum != -1) {
		old_weapon_objp = &Objects[aip->danger_weapon_objnum];
		if ((old_weapon_objp->type == OBJ_WEAPON) && (old_weapon_objp->signature == aip->danger_weapon_signature)) {
			;
		} else {
			aip->danger_weapon_objnum = -1;
		}
	}

	new_dist = compute_dots(weapon_objp, objp, &new_dot, NULL);

	if (aip->danger_weapon_objnum == -1) {
		if (new_dist < 1500.0f) {
			if (new_dot > 0.5f) {
				aip->danger_weapon_objnum = weapon_objnum;
				aip->danger_weapon_signature = weapon_objp->signature;
			}
		}
	} else {
		Assert(old_weapon_objp != NULL);
		old_dist = compute_dots(old_weapon_objp, objp, &old_dot, NULL);
	
		if (old_dot < 0.5f) {
			aip->danger_weapon_objnum = -1;
			old_dist = 9999.9f;
		}

		if ((new_dot > 0.5f) && (new_dot > old_dot-0.01f)) {
			if (new_dist < old_dist) {
				aip->danger_weapon_objnum = weapon_objnum;
				aip->danger_weapon_signature = weapon_objp->signature;
			}
		}
	}
}

//	If rvec != NULL, use it to match bank by calling vm_matrix_interpolate.
//	(rvec defaults to NULL)
void ai_turn_towards_vector(vector *dest, object *objp, 
									 float frametime, float turn_time, vector *slide_vec, vector *rel_pos, float bank_override, int flags, vector *rvec)
{
	//matrix	goal_orient;
	matrix	curr_orient;
	vector	vel_in, vel_out, desired_fvec, src;
	float		delta_time;
	physics_info	*pip;
	vector	vel_limit, acc_limit;
	float		delta_bank;

	//	Don't allow a ship to turn if it has no engine strength.
	// AL 3-12-98: objp may not always be a ship!
	if ( objp->type == OBJ_SHIP ) {
		if (ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_ENGINE) <= 0.0f)
			return;
	}
			
	//nprintf(("AI", "Ship %s turning towards point %7.3f %7.3f %7.3f\n", Ships[objp->instance].ship_name, dest->x, dest->y, dest->z));
	pip = &objp->phys_info;

	vel_in = pip->rotvel;
	curr_orient = objp->orient;
	delta_time = flFrametime;

	Assert(turn_time > 0.0f);
	
	//	Scale turn_time based on skill level and team.
	if (!(flags & AITTV_FAST)){
		if (objp->type == OBJ_SHIP){
			if (Ships[objp->instance].team != Ships[Player_obj->instance].team){
				turn_time *= Turn_time_skill_level_scale[Game_skill_level];
			}
		}
	}

	//	Set max turn rate.
	vel_limit.xyz.x = 2*PI/turn_time;
	vel_limit.xyz.y = 2*PI/turn_time;
	vel_limit.xyz.z = 2*PI/turn_time;

	//	Set rate at which ship can accelerate to its rotational velocity.
	//	For now, weapons just go much faster.
	acc_limit = vel_limit;
	if (objp->type == OBJ_WEAPON)
		vm_vec_scale(&acc_limit, 8.0f);

	src = objp->pos;

	if (rel_pos != NULL) {
		vector	gun_point;
		vm_vec_unrotate(&gun_point, rel_pos, &objp->orient);
		vm_vec_add2(&src, &gun_point);
	}

	vm_vec_normalized_dir(&desired_fvec, dest, &src);

	//	Since ship isn't necessarily moving in the direction it's pointing, sometimes it's better
	//	to be moving towards goal rather than just pointing.  So, if slide_vec is !NULL, try to
	//	make ship move towards goal, not point at goal.
	if (slide_vec != NULL) {
		vm_vec_add2(&desired_fvec, slide_vec);
		vm_vec_normalize(&desired_fvec);
	}

	//	Should be more general case here.  Currently, anything that is not a weapon will bank when it turns.
	if (objp->type == OBJ_WEAPON)
		delta_bank = 0.0f;
	else if ((bank_override) && (Ships[objp->instance].team & opposing_team_mask(Player_ship->team))) {	//	Theoretically, this will only happen for Shivans.
		delta_bank = bank_override;
		//nprintf(("AI", "%i: %7.3f\n", Framecount, bank_override));
	} else {
		delta_bank = vm_vec_dot(&curr_orient.v.rvec, &objp->last_orient.v.rvec);
		delta_bank = 100.0f * (1.0f - delta_bank);
		if (vm_vec_dot(&objp->last_orient.v.fvec, &objp->orient.v.rvec) < 0.0f)
			delta_bank = -delta_bank;

		//nprintf(("AI", "%s: Frame %i: delta bank = %7.3f\n", Ships[objp->instance].ship_name, Framecount, delta_bank));
	}

	//	Dave Andsager: The non-indented lines here are debug code to help you track down the problem in the physics
	//	that is causing ships to inexplicably rotate very far.  If you hit the Int3(), set the next statement to be
	//	the one marked "HERE".  (Do this clicking the cursor there, then right clicking.  Choose the right option.)
	//	This will allow you to rerun vm_forward_interpolate() with the values that caused the error.
	//	Note, you'll need to enable the Int3() about ten lines below.
#ifndef NDEBUG
vector tvec = objp->orient.v.fvec;
vector	vel_in_copy;
matrix	objp_orient_copy;

vel_in_copy = vel_in;
objp_orient_copy = objp->orient;

vel_in = vel_in_copy;	//	HERE
objp->orient = objp_orient_copy;
#endif
	if (rvec != NULL) {
		matrix	out_orient, goal_orient;

		vm_vector_2_matrix(&goal_orient, &desired_fvec, NULL, rvec);
		vm_matrix_interpolate(&goal_orient, &curr_orient, &vel_in, delta_time, &out_orient, &vel_out, &vel_limit, &acc_limit);
		objp->orient = out_orient;
	} else {
		vm_forward_interpolate(&desired_fvec, &curr_orient, &vel_in, delta_time, delta_bank, &objp->orient, &vel_out, &vel_limit, &acc_limit);
	}
#ifndef NDEBUG
if (!((objp->type == OBJ_WEAPON) && (Weapon_info[Weapons[objp->instance].weapon_info_index].subtype == WP_MISSILE))) {
	if (delta_time < 0.25f && vm_vec_dot(&objp->orient.v.fvec, &tvec) < 0.1f)
		Int3();	//	Get Andsager.  A ship has turned too far in one frame.
}
#endif

	pip->rotvel = vel_out;
}

void init_ship_info()
{
	int	i;

	if (Ship_info_inited)
		return;

	for (i=0; i<MAX_SHIP_TYPES; i++) {
		Ship_info[i].min_speed = - Ship_info[i].max_rear_vel;
		Ship_info[i].max_accel = Ship_info[i].max_vel.xyz.z;
	}

	Ship_info_inited = 1;

}

//	Set aip->target_objnum to objnum
//	Update aip->previous_target_objnum.
//	If new target (objnum) is different than old target, reset target_time.
int set_target_objnum(ai_info *aip, int objnum)
{
/*
	char	old_name[32], new_name[32];

	if (!timestamp_elapsed(aip->ok_to_target_timestamp))
		return aip->target_objnum;

	if (Player_ship && (Ships[aip->shipnum].team == Player_ship->team)) {
		if (aip->target_objnum == -1)
			strcpy(old_name, "none");
		else
			strcpy(old_name, Ships[Objects[aip->target_objnum].instance].ship_name);

		if (objnum == -1)
			strcpy(new_name, "none");
		else
			strcpy(new_name, Ships[Objects[objnum].instance].ship_name);

		nprintf(("AI", "Ship %s changing target from %s to %s\n", Ships[aip->shipnum].ship_name, old_name, new_name));
	}
*/

	// AL 2-25-97: Ensure that a protected ship isn't being set as a target (for non-players only)
	/*
	if ( objnum >= 0 ) {
		if ( !(Objects[Ships[aip->shipnum].objnum].flags & OF_PLAYER_SHIP) ) {
			if ( Objects[objnum].flags & OF_PROTECTED ) {
				// AL 2-26-97: removing Int3() until issue with setting OF_PROTECTED in ai_set_attack_subsystem()
				//Int3();								// this should not happen
				return aip->target_objnum;		// don't change targets
			}
		}
	}
	*/

	if ((aip != Player_ai) && (!timestamp_elapsed(aip->ok_to_target_timestamp))) {
		return aip->target_objnum;
	}

	if (aip->target_objnum == objnum) {
		aip->previous_target_objnum = aip->target_objnum;
	} else {
		aip->previous_target_objnum = aip->target_objnum;

		// ignore this assert if a multiplayer observer
		if((Game_mode & GM_MULTIPLAYER) && (aip == Player_ai) && (Player_obj->type == OBJ_OBSERVER)){
		} else {
			Assert(objnum != Ships[aip->shipnum].objnum);	//	make sure not targeting self
		}

		// if stealth target, init ai_info for stealth
		if ( (objnum > 0) && is_object_stealth_ship(&Objects[objnum]) ) {
			init_ai_stealth_info(aip, &Objects[objnum]);
		}

		aip->target_objnum = objnum;
		aip->target_time = 0.0f;
		aip->target_signature = Objects[objnum].signature;
		// clear targeted subsystem
		set_targeted_subsys(aip, NULL, -1);
	}
	
	return aip->target_objnum;
}

int ai_select_primary_weapon(object *objp, object *other_objp, int flags);

//	Make new_subsys the targeted subsystem of ship *aip.
ship_subsys *set_targeted_subsys(ai_info *aip, ship_subsys *new_subsys, int parent_objnum)
{
	Assert(aip != NULL);

	aip->last_subsys_target = aip->targeted_subsys;
	aip->targeted_subsys = new_subsys;
	aip->targeted_subsys_parent = parent_objnum;

	if ( new_subsys ) {
		// Make new_subsys target
		if (new_subsys->system_info->type == SUBSYSTEM_ENGINE) {
			if ( aip != Player_ai ) {
				ai_select_primary_weapon(&Objects[Ships[aip->shipnum].objnum], &Objects[parent_objnum], WIF_PUNCTURE);
				ship_primary_changed(&Ships[aip->shipnum]);	// AL: maybe send multiplayer information when AI ship changes primaries
			}
		}

		if ( aip == Player_ai ) {
			hud_lock_reset(0.5f);
		}

	} else {
		// Cleanup any subsys path information if it exists
		ai_big_subsys_path_cleanup(aip);
	}
	
	return aip->targeted_subsys;
}											  

// called to init the data for single ai object.  At this point,
// the ship and the object and the ai_info are are correctly
// linked together. Ai_info[ai_index].shipnum is the only valid field 
// in ai_info.
//	This is called right when the object is parsed, so you can't assume much
//	has been initialized.  For example, wings, waypoints, goals are probably
//	not yet loaded. --MK, 10/8/96
void ai_object_init(object * obj, int ai_index)
{
	ai_info	*aip;
	Assert(ai_index >= 0 && ai_index < MAX_AI_INFO);

	aip = &Ai_info[ai_index];

	aip->type = 0;		//	0 means not in use.
	aip->wing = -1;		//	Member of what wing? -1 means none.
	aip->ai_class = Ship_info[Ships[obj->instance].ship_info_index].ai_class;
	aip->behavior = AIM_NONE;
}

//	If *aip is docked, set max acceleration to A->mass/(A->mass + B->mass) where A is *aip and B is dock object
void adjust_accel_for_docking(ai_info *aip)
{
	if (aip->dock_objnum != -1) {
		object	*obj2p = &Objects[aip->dock_objnum];
		object	*obj1p;

		obj1p = &Objects[Ships[aip->shipnum].objnum];

		if (obj2p->signature == aip->dock_signature) {
			float	ratio;

			ratio = obj1p->phys_info.mass / (obj1p->phys_info.mass + obj2p->phys_info.mass);

			// put cap on how much ship can slow down
#ifdef MAKE_FS1
			// FS1 can go slower, perhaps down to 0, but I'll cap it at .25 just in case
			if (ratio < 0.25f) {
				ratio = 0.25f;
			}
#else
			if (ratio < 0.8) {
				ratio = 0.8f;
			}
#endif

			if (AI_ci.forward > ratio) {
				AI_ci.forward = ratio;
			}
		}
	}
}

// -------------------------------------------------------------------
void accelerate_ship(ai_info *aip, float accel)
{
	aip->prev_accel = accel;
	AI_ci.forward = accel;
	adjust_accel_for_docking(aip);
}

//	--------------------------------------------------------------------------
void change_acceleration(ai_info *aip, float delta_accel)
{
	float	new_accel;

	if (delta_accel < 0.0f) {
		if (aip->prev_accel > 0.0f)
			aip->prev_accel = 0.0f;
	} else if (aip->prev_accel < 0.0f)
		aip->prev_accel = 0.0f;

	new_accel = aip->prev_accel + delta_accel * flFrametime;

	if (new_accel > 1.0f)
		new_accel = 1.0f;
	else if (new_accel < -1.0f)
		new_accel = -1.0f;
	
	aip->prev_accel = new_accel;

	AI_ci.forward = new_accel;
	adjust_accel_for_docking(aip);
}

void set_accel_for_target_speed(object *objp, float tspeed)
{
	float	max_speed;
	ai_info	*aip;

	aip = &Ai_info[Ships[objp->instance].ai_index];

	max_speed = Ships[objp->instance].current_max_speed;

	AI_ci.forward = tspeed/max_speed;
	aip->prev_accel = AI_ci.forward;

	adjust_accel_for_docking(aip);
}

//	Stuff perim_point with a point on the perimeter of the sphere defined by object *objp
//	on the vector from the center of *objp through the point *vp.
void project_point_to_perimeter(vector *perim_point, vector *pos, float radius, vector *vp)
{
	vector	v1;
	float		mag;

	vm_vec_sub(&v1, vp, pos);
	mag = vm_vec_mag(&v1);

	if (mag == 0.0f) {
		Warning(LOCATION, "projectable point is at center of sphere.");
		(void) vm_vec_make(&v1, 0.0f, radius, 0.0f);
	} else {
		vm_vec_normalize(&v1);
		vm_vec_scale(&v1, 1.1f * radius + 10.0f);
	}

	vm_vec_add2(&v1, pos);
	*perim_point = v1;
}

//	Stuff tan1 with tangent point on sphere.  tan1 is point nearer to *p1
//	*p0 is point through which tangents pass.
//	*centerp is center of sphere.
//	*p1 is another point in space to define the plane in which tan1, tan2 reside.
//	radius is the radius of the sphere.
//	Note, this is a very approximate function just for AI.
//	Note also: On 12/26/96, p1 is used to define the plane perpendicular to that which
//	contains the tangent point.
void get_tangent_point(vector *tan1, vector *p0, vector *centerp, vector *p1, float radius)
{
	vector	dest_vec, v2c, perp_vec, temp_vec, v2;
	float		dist, ratio;

	//	Detect condition of point inside sphere.
	if (vm_vec_dist(p0, centerp) < radius)
		project_point_to_perimeter(tan1, centerp, radius, p0);
	else {
		vm_vec_normalized_dir(&v2c, centerp, p0);

		//	Compute perpendicular vector using p0, centerp, p1
		vm_vec_normal(&temp_vec, p0, centerp, p1);
		vm_vec_sub(&v2, centerp, p0);
		vm_vec_cross(&perp_vec, &temp_vec, &v2);

		vm_vec_normalize(&perp_vec);

		dist = vm_vec_dist_quick(p0, centerp);
		ratio = dist / radius;

		if (ratio < 2.0f)
			vm_vec_scale_add(&dest_vec, &perp_vec, &v2c, ratio-1.0f);
		else
			vm_vec_scale_add(&dest_vec, &v2c, &perp_vec, (1.0f + 1.0f/ratio));

		vm_vec_scale_add(tan1, p0, &dest_vec, dist + radius);
	}
}

//	--------------------------------------------------------------------------
//	Given an object and a point, turn towards the point, resulting in
// approach behavior.
void turn_towards_point(object *objp, vector *point, vector *slide_vec, float bank_override)
{
	ai_info	*aip;
	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];
	
	// check if in formation and if not leader, don't change rotvel.xyz.z (bank to match leader elsewhere)
	if (aip->ai_flags & AIF_FORMATION) {
		if (&Objects[aip->goal_objnum] != objp) {
			float rotvel_z = objp->phys_info.rotvel.xyz.z;
			ai_turn_towards_vector(point, objp, flFrametime, Ship_info[Ships[objp->instance].ship_info_index].srotation_time, slide_vec, NULL, bank_override, 0);
			objp->phys_info.rotvel.xyz.z = rotvel_z;
		}
	} else {
		// normal turn
		ai_turn_towards_vector(point, objp, flFrametime, Ship_info[Ships[objp->instance].ship_info_index].srotation_time, slide_vec, NULL, bank_override, 0);
	}
}

//	--------------------------------------------------------------------------
//	Given an object and a point, turn away from the point, resulting in avoidance behavior.
//	Note: Turn away at full speed, not scaled down by skill level.
void turn_away_from_point(object *objp, vector *point, float bank_override)
{
	vector	opposite_point;

	vm_vec_sub(&opposite_point, &objp->pos, point);
	vm_vec_add2(&opposite_point, &objp->pos);

	ai_turn_towards_vector(&opposite_point, objp, flFrametime, Ship_info[Ships[objp->instance].ship_info_index].srotation_time, NULL, NULL, bank_override, AITTV_FAST);
}


//	--------------------------------------------------------------------------
//	Given an object and a point, turn tangent to the point, resulting in
// a circling behavior.
//	Make object *objp turn around the point *point with a radius of radius.
//	Note that this isn't the same as following a circle of radius radius with
//	center *point, but it should be adequate.
//	Note that if you want to circle an object without hitting it, you should use
//	about twice that object's radius for radius, else you'll certainly bump into it.
//	Return dot product to goal point.
float turn_towards_tangent(object *objp, vector *point, float radius)
{
	vector	vec_to_point;
	vector	goal_point;
	vector	perp_point;				//	point radius away from *point on vector to objp->pos
	vector	up_vec, perp_vec;

	vm_vec_normalized_dir(&vec_to_point, point, &objp->pos);
	vm_vec_crossprod(&up_vec, &vec_to_point, &objp->orient.v.fvec);
	vm_vec_crossprod(&perp_vec, &vec_to_point, &up_vec);

	vm_vec_scale_add(&perp_point, point, &vec_to_point, -radius);
	if (vm_vec_dot(&objp->orient.v.fvec, &perp_vec) > 0.0f) {
		vm_vec_scale_add(&goal_point, &perp_point, &perp_vec, radius);
	} else {
		vm_vec_scale_add(&goal_point, &perp_point, &perp_vec, -radius);
	}

//	Ai_info[Ships[objp->instance].ai_index].goal_point = goal_point;
	turn_towards_point(objp, &goal_point, NULL, 0.0f);

	vector	v2g;

	vm_vec_normalized_dir(&v2g, &goal_point, &objp->pos);
	return vm_vec_dot(&objp->orient.v.fvec, &v2g);
}

float turn_toward_tangent_with_axis(object *objp, object *center_objp, float radius)
{
	vector r_vec, theta_vec;
	vector center_vec, vec_on_cylinder, sph_r_vec;
	float center_obj_z;

	// find closest z of center objp
	vm_vec_sub(&sph_r_vec, &objp->pos, &center_objp->pos);
	center_obj_z = vm_vec_dotprod(&sph_r_vec, &center_objp->orient.v.fvec);

	// find pt on axis with closest z
	vm_vec_scale_add(&center_vec, &center_objp->pos, &center_objp->orient.v.fvec, center_obj_z);

	// get r_vec
	vm_vec_sub(&r_vec, &objp->pos, &center_vec);
//	float r_mag = vm_vec_normalize_quick(&r_vec);
//	mprintf(("cur_r: %.1f, desired_r: %.1f\n", r_mag, radius));
	Assert( (vm_vec_dotprod(&r_vec, &center_objp->orient.v.fvec) < 0.0001));

	// get theta vec - perp to r_vec and z_vec
	vm_vec_crossprod(&theta_vec, &center_objp->orient.v.fvec, &r_vec);

#ifndef NDEBUG
	float mag = vm_vec_normalize(&theta_vec);
	Assert(mag > 0.9999 && mag < 1.0001);
#endif

	vector temp;
	vm_vec_crossprod(&temp, &r_vec, &theta_vec);

#ifndef NDEBUG
	float dot = vm_vec_dotprod(&temp, &center_objp->orient.v.fvec);
	Assert( dot >0.9999 && dot < 1.0001);
#endif

	// find pt on clylinder with closest z
	vm_vec_scale_add(&vec_on_cylinder, &center_vec, &r_vec, radius);

	vector goal_pt, v2g;
	vm_vec_scale_add(&goal_pt, &vec_on_cylinder, &theta_vec, radius);

//	Ai_info[Ships[objp->instance].ai_index].goal_point = goal_pt;
	turn_towards_point(objp, &goal_pt, NULL, 0.0f);

	vm_vec_normalized_dir(&v2g, &goal_pt, &objp->pos);
	return vm_vec_dot(&objp->orient.v.fvec, &v2g);
}

//	Returns a point radius units away from *point that *objp should turn towards to orbit *point
void get_tangent_point(vector *goal_point, object *objp, vector *point, float radius)
{
	vector	vec_to_point;
	vector	perp_point;				//	point radius away from *point on vector to objp->pos
	vector	up_vec, perp_vec;

	vm_vec_normalized_dir(&vec_to_point, point, &objp->pos);
	vm_vec_crossprod(&up_vec, &vec_to_point, &objp->orient.v.fvec);
	vm_vec_crossprod(&perp_vec, &vec_to_point, &up_vec);
	vm_vec_normalize(&perp_vec);

	vm_vec_scale_add(&perp_point, point, &vec_to_point, -radius);

	if (vm_vec_dot(&objp->orient.v.fvec, &perp_vec) > 0.0f) {
		vm_vec_scale_add(goal_point, &perp_point, &perp_vec, radius);
	} else {
		vm_vec_scale_add(goal_point, &perp_point, &perp_vec, -radius);
	}
}

int	Player_attacking_enabled = 1;

// -----------------------------------------------------------------------------
// Determine whether an object is targetable within a nebula
int object_is_targetable(object *target, ship *viewer)
{
	int stealth_ship = 0;

	// if target is ship, check if visible by team
	if (target->type == OBJ_SHIP) {
		stealth_ship = (Ship_info[Ships[target->instance].ship_info_index].flags & SIF_STEALTH);
		if ( ship_is_visible_by_team(target->instance, viewer->team) == 1) {
			return 1;
		}
	}

	// for AI partially targetable works as fully targetable, except for stealth ship
	if (stealth_ship) {
		// if not team targetable, check if within frustrum
		if ( ai_is_stealth_visible(&Objects[viewer->objnum], target) == STEALTH_VISIBLE ) {
			return 1;
		} else {
			return 0;
		}
	}

	// if not fully targetable by team, check awacs level with viewer
	// allow targeting even if only only partially targetable to player
	float radar_return = awacs_get_level(target, viewer);
	if ( radar_return > 0.4 ) {
		return 1;
	} else {
		return 0;
	}
}

//	Return number of enemies attacking object objnum
//
// AL 10.26.97: Also include turrets on large ships when couting enemies attacking
int num_enemies_attacking(int objnum)
{
	object		*objp;
	ship			*sp;
	ship_subsys	*ssp;
	ship_obj		*so;
	int			count;

	count = 0;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		Assert(objp->instance != -1);
		sp = &Ships[objp->instance];

		if (Ai_info[sp->ai_index].target_objnum == objnum)
			count++;

		// consider turrets that may be attacking objnum (but only turrets on SIF_BIG_SHIP ships)
		if ( Ship_info[sp->ship_info_index].flags & SIF_BIG_SHIP ) {

			// loop through all the subsystems, check if turret has objnum as a target
			ssp = GET_FIRST(&sp->subsys_list);
			while ( ssp != END_OF_LIST( &sp->subsys_list ) ) {

				if ( ssp->system_info->type == SUBSYSTEM_TURRET ) {
					if ( (ssp->turret_enemy_objnum == objnum) && (ssp->current_hits > 0) ) {
						count++;
					}
				}
				ssp = GET_NEXT( ssp );
			} // end while
		}
	}

	return count;
}

//	Get the team to fire on given an object.
int get_enemy_team_mask(int objnum)
{
	int	my_team, enemy_team_mask;

	my_team = Ships[Objects[objnum].instance].team;

	if (Mission_all_attack) {
		//	All teams attack all teams.
		switch (my_team) {
		case TEAM_FRIENDLY:
			enemy_team_mask = TEAM_HOSTILE | TEAM_NEUTRAL | TEAM_TRAITOR;
			break;
		case TEAM_HOSTILE:
			enemy_team_mask = TEAM_FRIENDLY | TEAM_NEUTRAL | TEAM_TRAITOR;
			break;
		case TEAM_NEUTRAL:
			enemy_team_mask = TEAM_FRIENDLY | TEAM_HOSTILE | TEAM_TRAITOR;
			break;
		case TEAM_UNKNOWN:
			enemy_team_mask = TEAM_HOSTILE;
			break;
		case TEAM_TRAITOR:
			enemy_team_mask = TEAM_FRIENDLY | TEAM_NEUTRAL | TEAM_HOSTILE | TEAM_TRAITOR;
			break;
		default:
			enemy_team_mask = TEAM_HOSTILE;
			Int3();			//	Illegal value for team!
			break;
		}
	} else {
		switch (my_team) {
		case TEAM_FRIENDLY:
			enemy_team_mask = TEAM_HOSTILE | TEAM_NEUTRAL | TEAM_TRAITOR;
			break;
		case TEAM_HOSTILE:
			enemy_team_mask = TEAM_FRIENDLY | TEAM_NEUTRAL | TEAM_TRAITOR;
			break;
		case TEAM_NEUTRAL:
			enemy_team_mask = TEAM_FRIENDLY | TEAM_TRAITOR;
			break;
		case TEAM_UNKNOWN:
			enemy_team_mask = TEAM_HOSTILE;
			break;
		case TEAM_TRAITOR:
			enemy_team_mask = TEAM_FRIENDLY | TEAM_NEUTRAL | TEAM_HOSTILE | TEAM_TRAITOR;
			break;
		default:
			enemy_team_mask = TEAM_HOSTILE;
			Int3();			//	Illegal value for team!
			break;
		}
	}

	return enemy_team_mask;
}

//	Scan all the ships in *objp's wing.
//	Return the lowest maximum speed of a ship in the wing.
//	Current maximum speed (based on energy settings) is shipp->current_max_speed
float get_wing_lowest_max_speed(object *objp)
{
	ship		*shipp;
	ai_info	*aip;
	float		lowest_max_speed;
	int		wingnum;
	object	*o;
	ship_obj	*so;

	Assert(objp->type == OBJ_SHIP);
	Assert((objp->instance >= 0) && (objp->instance < MAX_OBJECTS));
	shipp = &Ships[objp->instance];
	Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));
	aip = &Ai_info[shipp->ai_index];

	wingnum = aip->wing;

	lowest_max_speed = shipp->current_max_speed;

	if ( wingnum == -1 )
		return lowest_max_speed;

	Assert(wingnum >= 0);

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		o = &Objects[so->objnum];
		ship	*oshipp = &Ships[o->instance];
		ai_info	*oaip = &Ai_info[oshipp->ai_index];

		if ((oaip->mode == AIM_WAYPOINTS) && (oaip->wing == wingnum)) {
			//	Note: If a ship in the wing has a super low max speed, probably its engines are disabled.  So, fly along and
			//	ignore the poor guy.
			float	cur_max = oshipp->current_max_speed;

			if (oaip->ai_flags & AIF_DOCKED) {
				if (oaip->dock_objnum > -1)
					if (Objects[oaip->dock_objnum].type == OBJ_SHIP) 
						cur_max *= o->phys_info.mass/(o->phys_info.mass + Objects[oaip->dock_objnum].phys_info.mass);
			}
							
			if ((oshipp->current_max_speed > 5.0f) && (cur_max < lowest_max_speed)) {
				lowest_max_speed = cur_max;
			}
		}
	}

	return lowest_max_speed;
}

/*
//	Tell everyone to ignore object objnum.
void set_global_ignore_object(int objnum)
{
	int	i;

	Assert(Objects[objnum].type == OBJ_SHIP);

	nprintf(("AI", "Telling everyone to ignore object %s\n", Ships[Objects[objnum].instance].ship_name));

	for (i=0; i<MAX_IGNORE_OBJECTS; i++) {
		if (Ignore_objects[i].objnum == -1) {
			Ignore_objects[i].objnum = objnum;
			Ignore_objects[i].signature = Objects[objnum].signature;
			break;
		}
	}

	if (i == MAX_IGNORE_OBJECTS) {
		//	Couldn't find a free slot, but maybe one of these objects has died.
		for (i=0; i<MAX_IGNORE_OBJECTS; i++) {
			int	o = Ignore_objects[i].objnum;
			if (Objects[o].type != OBJ_SHIP)
				break;		//	Not a ship, so use this slot.
			if (Objects[o].signature != Ignore_objects[i].signature)
				break;		//	Signatures don't match, so use this slot.
		}

		if (i != MAX_IGNORE_OBJECTS) {
			Ignore_objects[i].objnum = objnum;
			Ignore_objects[i].signature = Objects[objnum].signature;
		} else {
			nprintf(("Warning", "Ignore_objects buffer full.  Stealing a slot to ignore object #%i\n"));
			Int3();

			int	r;

			r = objnum % MAX_IGNORE_OBJECTS;

			Ignore_objects[r].objnum = objnum;
			Ignore_objects[r].signature = Objects[objnum].signature;
		}
	}
}

*/

//	Determine if object objnum is supposed to be ignored by object with ai_info *aip.
//	Return:
//		TRUE	if objnum is aip->ignore_objnum (and signatures match)
//				or objnum is in ignore wing
//		FALSE	otherwise
int is_ignore_object(ai_info *aip, int objnum)
{

/*	//	First, scan all objects in global array of objects to be ignored.
	for (int i=0; i<MAX_IGNORE_OBJECTS; i++)
		if (Ignore_objects[i].objnum != -1)
			if (objnum == Ignore_objects[i].objnum)
				if (Objects[Ignore_objects[i].objnum].signature == Ignore_objects[i].signature)
					return 1;
*/

	//	Didn't find in global list.  Now check 
	if (aip->ignore_objnum == UNUSED_OBJNUM)
		return 0;									//	Not ignoring anything.
	else if (aip->ignore_objnum >= 0) {		//	This means it's ignoring an object, not a wing.
		if (aip->ignore_objnum == objnum) {
			if (Objects[aip->ignore_objnum].signature == aip->ignore_signature) {
				return 1;
			} else {
				aip->ignore_objnum = UNUSED_OBJNUM;
				return 0;
			}
		} else {
			return 0;
		}
	} else {											//	Ignoring a wing.
		Int3(); // Should never happen.  I thought I removed this behavior! -- MK, 5/17/98
		return 0;
/*		int	ignore_wingnum = -(aip->ignore_objnum + 1);

		Assert(ignore_wingnum < MAX_WINGS);
		Assert(aip->shipnum >= 0);
		return (Ships[Objects[objnum].instance].wingnum == ignore_wingnum);
*/	}
}

// -----------------------------------------------------------------------------

// given a ship with bounding box and a point, find the closest point on the bbox
int get_nearest_bbox_point(object *ship_obj, vector *start, vector *box_pt)
{
	vector temp, rf_start;
	polymodel *pm;
	pm = model_get(Ship_info[Ships[ship_obj->instance].ship_info_index].modelnum);

	// get start in ship rf
	vm_vec_sub(&temp, start, &ship_obj->pos);
	vm_vec_rotate(&rf_start, &temp, &ship_obj->orient);

	// find box_pt
	int inside = project_point_onto_bbox(&pm->mins, &pm->maxs, &rf_start, &temp);

	// get box_pt in world rf
	vm_vec_unrotate(box_pt, &temp, &ship_obj->orient);
	vm_vec_add2(box_pt, &ship_obj->pos);

	return inside;
}


typedef struct eval_nearest_objnum {
	int	objnum;
	object *trial_objp;
	int	enemy_team_mask;
	int	enemy_wing;
	float	range;
	int	max_attackers;
	int	nearest_objnum;
	float	nearest_dist;
	int	check_danger_weapon_objnum;
} eval_nearest_objnum;


void evaluate_object_as_nearest_objnum(eval_nearest_objnum *eno)
{
	ai_info	*aip;
	ship_subsys	*attacking_subsystem;

	aip = &Ai_info[Ships[Objects[eno->objnum].instance].ai_index];

	attacking_subsystem = aip->targeted_subsys;

	if ((attacking_subsystem != NULL) || !(eno->trial_objp->flags & OF_PROTECTED)) {
		if ( OBJ_INDEX(eno->trial_objp) != eno->objnum ) {
#ifndef NDEBUG
			if (!Player_attacking_enabled && (eno->trial_objp == Player_obj))
				return;
#endif
			//	If only supposed to attack ship in a specific wing, don't attack other ships.
			if ((eno->enemy_wing != -1) && (Ships[eno->trial_objp->instance].wingnum != eno->enemy_wing))
				return;

			//	Don't keep firing at a ship that is in its death throes.
			if (Ships[eno->trial_objp->instance].flags & SF_DYING)
				return;

			if (is_ignore_object(aip, ((eno->trial_objp)-Objects)))
				return;

			if (eno->trial_objp->flags & OF_PROTECTED)
				return;

			if (Ships[eno->trial_objp->instance].flags & SF_ARRIVING)
				return;

			ship_info *sip = &Ship_info[Ships[eno->trial_objp->instance].ship_info_index];

			if (sip->flags & (SIF_NO_SHIP_TYPE | SIF_NAVBUOY))
				return;

			if (Ships[eno->trial_objp->instance].team & eno->enemy_team_mask) {
				float	dist;
				int	num_attacking;

				// Allow targeting of stealth in nebula by his firing at me
				// This is done for a specific ship, not generally.
				if ( !eno->check_danger_weapon_objnum ) {
					// check if can be targeted if inside nebula
					if ( !object_is_targetable(eno->trial_objp, &Ships[Objects[eno->objnum].instance]) ) {
						// check if stealth ship is visible, but not "targetable"
						if ( !((sip->flags & SIF_STEALTH) && ai_is_stealth_visible(&Objects[eno->objnum], eno->trial_objp)) ) {
							return;
						}
					}
				}

				// if objnum is BIG or HUGE, find distance to bbox
				if (sip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) {
					vector box_pt;
					// check if inside bbox
					int inside = get_nearest_bbox_point(eno->trial_objp, &Objects[eno->objnum].pos, &box_pt);
					if (inside) {
						dist = 10.0f;
						// on the box
					} else {
						dist = vm_vec_dist_quick(&Objects[eno->objnum].pos, &box_pt);
					}
				} else {
					dist = vm_vec_dist_quick(&Objects[eno->objnum].pos, &eno->trial_objp->pos);
				}
				
				//	Make it more likely that fighters (or bombers) will be picked as an enemy by scaling up distance for other types.
				if ((Ship_info[Ships[eno->trial_objp->instance].ship_info_index].flags & (SIF_FIGHTER | SIF_BOMBER))) {
					dist = dist * 0.5f;
				}

				num_attacking = num_enemies_attacking(eno->trial_objp-Objects);
				if ((sip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) || (num_attacking < eno->max_attackers)) {
					if (!(sip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))){
						dist *= (float) (num_attacking+2)/2.0f;				//	prevents lots of ships from attacking same target
					}

					if (eno->trial_objp->flags & OF_PLAYER_SHIP){
						dist *= 1.0f + (NUM_SKILL_LEVELS - Game_skill_level - 1)/NUM_SKILL_LEVELS;	//	Favor attacking non-players based on skill level.
					}

					if (dist < eno->nearest_dist) {
						eno->nearest_dist = dist;
						eno->nearest_objnum = eno->trial_objp-Objects;
					}
				}
			}
		}
	}

}


//	Given an object and an enemy team, return the index of the nearest enemy object.
//	Unless aip->targeted_subsys != NULL, don't allow to attack objects
//	with OF_PROTECTED bit set.
//	Ship must be within range "range".
//	Don't attack a ship that already has at least max_attackers attacking it.
int get_nearest_objnum(int objnum, int enemy_team_mask, int enemy_wing, float range, int max_attackers)
{
	object	*danger_weapon_objp;
	ai_info	*aip;
	ship_obj	*so;

	// initialize eno struct
	eval_nearest_objnum eno;
	eno.enemy_team_mask = enemy_team_mask;
	eno.enemy_wing = enemy_wing;
	eno.max_attackers = max_attackers;
	eno.objnum = objnum;
	eno.range = range;
	eno.nearest_dist = range;
	eno.nearest_objnum = -1;
	eno.check_danger_weapon_objnum = 0;

	// go through the list of all ships and evaluate as potential targets
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		eno.trial_objp = &Objects[so->objnum];
		evaluate_object_as_nearest_objnum(&eno);

	}

	// check if danger_weapon_objnum has will show a stealth ship
	aip = &Ai_info[Ships[Objects[objnum].instance].ai_index];
	if (aip->danger_weapon_objnum >= 0) {
		danger_weapon_objp = &Objects[aip->danger_weapon_objnum];
		// validate weapon
		if (danger_weapon_objp->signature == aip->danger_weapon_signature) {
			Assert(danger_weapon_objp->type == OBJ_WEAPON);
			// check if parent is a ship
			if (danger_weapon_objp->parent >= 0) {
				if ( is_object_stealth_ship(&Objects[danger_weapon_objp->parent]) ) {
					// check if stealthy
					if ( ai_is_stealth_visible(&Objects[objnum], &Objects[danger_weapon_objp->parent]) != STEALTH_FULLY_TARGETABLE ) {
						// check if weapon is laser
						if (Weapon_info[Weapons[danger_weapon_objp->instance].weapon_info_index].subtype == WP_LASER) {
							// check stealth ship by its laser fire
							eno.check_danger_weapon_objnum = 1;
							eno.trial_objp = &Objects[danger_weapon_objp->parent];
							evaluate_object_as_nearest_objnum(&eno);
						}
					}
				}
			}
		}
	}

	//	If only looking for target in certain wing and couldn't find anything in
	//	that wing, look for any object.
	if ((eno.nearest_objnum == -1) && (enemy_wing != -1)) {
		return get_nearest_objnum(objnum, enemy_team_mask, -1, range, max_attackers);
	}

	return eno.nearest_objnum;
}

//	Given an object and an enemy team, return the index of the nearest enemy object.
//	Unlike find_enemy or find_nearest_objnum, this doesn't care about things like the protected flag or number
//	of enemies attacking.
//	It is used to find the nearest enemy to determine things like whether to rearm.
int find_nearby_hostile(int objnum, int enemy_team_mask, float range, int *count)
{
	int		nearest_objnum;
	float		nearest_dist;
	object	*objp;
	ai_info	*aip;
	ship_obj	*so;

	nearest_objnum = -1;
	nearest_dist = range;

	aip = &Ai_info[Ships[Objects[objnum].instance].ai_index];

	*count = 0;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];

		if ( OBJ_INDEX(objp) != objnum ) {
			if (Ships[objp->instance].flags & SF_DYING)
				continue;

			if (Ship_info[Ships[objp->instance].ship_info_index].flags & (SIF_NO_SHIP_TYPE | SIF_NAVBUOY))
				continue;

			if (Ships[objp->instance].team & enemy_team_mask) {
				float	dist;

				dist = vm_vec_dist_quick(&Objects[objnum].pos, &objp->pos) - objp->radius*0.75f;
				
				if (dist < range) {
					(*count)++;

					if (dist < nearest_dist) {
						nearest_dist = dist;
						nearest_objnum = objp-Objects;
					}
				}
			}
		}
	}

	return nearest_objnum;
}

// return !0 if objp can be considered for a turret target, 0 otherwise
// input:	objp				=>	object that turret is considering as an enemy
//				turret_parent	=>	object index for ship that turret sits on
int valid_turret_enemy(object *objp, object *turret_parent)
{
	if ( objp == turret_parent ) {
		return 0;
	}

	if ( objp->type == OBJ_ASTEROID ) {
		return 1;
	}

	if ( (objp->type == OBJ_SHIP) ) {
		ship *shipp;
		shipp = &Ships[objp->instance];

		// don't fire at ships with protected bit set!!!
		if ( objp->flags & OF_PROTECTED ) {
			return 0;
		}

		if ( !(Ship_info[shipp->ship_info_index].flags & SIF_DO_COLLISION_CHECK)) {
			return 0;
		}

		if (shipp->flags & SF_ARRIVING) {
			return 0;
		}

		return 1;
	}

	if ( objp->type == OBJ_WEAPON ) {
		if ( Weapon_info[Weapons[objp->instance].weapon_info_index].wi_flags & WIF_BOMB ) {
			if ( obj_team(turret_parent) != Weapons[objp->instance].team ) {
				return 1;
			}
		}
	}

	return 0;
}

// return 1 if objp is in fov of the specified turret, tp.  Otherwise return 0.
//	dist = distance from turret to center point of object
int object_in_turret_fov(object *objp, model_subsystem *tp, vector *tvec, vector *tpos, float dist)
{
	vector	v2e;
	float		dot;
	vm_vec_normalized_dir(&v2e, &objp->pos, tpos);
	dot = vm_vec_dot(&v2e, tvec);

	dot += objp->radius / (dist + objp->radius);

	if ( dot >= tp->turret_fov ) {
		return 1;
	}

	return 0;
}

// return 1 if bomb_objp is headed towards ship_objp
int bomb_headed_towards_ship(object *bomb_objp, object *ship_objp)
{
	float		dot;
	vector	bomb_to_ship_vector;

	vm_vec_normalized_dir(&bomb_to_ship_vector, &ship_objp->pos, &bomb_objp->pos);
	dot = vm_vec_dot(&bomb_objp->orient.v.fvec, &bomb_to_ship_vector);

	if ( dot > 0 ) {
		return 1;
	}

	return 0;
}

// nubmer of live turrets with target_objnum 
int num_turrets_attacking(object *turret_parent, int target_objnum) 
{
	ship_subsys *ss;
	ship *shipp;
	int count = 0;
	shipp = &Ships[turret_parent->instance];

	Assert(turret_parent->type == OBJ_SHIP);
	Assert(Objects[target_objnum].type == OBJ_SHIP);

	for (ss=GET_FIRST(&shipp->subsys_list); ss!=END_OF_LIST(&shipp->subsys_list); ss=GET_NEXT(ss)) {
		// check if subsys is alive
		if (ss->current_hits <= 0.0f) {
			continue;
		}

		// check if it's a turret
		if (ss->system_info->type != SUBSYSTEM_TURRET) {
			continue;
		}

		// if the turret is locked
		if(ss->weapons.flags & SW_FLAG_TURRET_LOCK){
			continue;
		}		

		// check if turret is targeting target_objnum
		if (ss->turret_enemy_objnum == target_objnum) {
			count++;
		}
	}

	return count;
}

float Lethality_range_const = 2.0f;
DCF(lethality_range, "N for modifying range: 1 / (1+N) at 100")
{
	dc_get_arg(ARG_FLOAT);
	Lethality_range_const = Dc_arg_float;
}

float Player_lethality_bump[NUM_SKILL_LEVELS] = {
	// 0.0f, 5.0f, 10.0f, 25.0f, 40.0f
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

// evaluate obj as posssible target for turret
void evaluate_obj_as_target(object *objp, eval_enemy_obj_struct *eeo)
{
	object	*turret_parent_obj = &Objects[eeo->turret_parent_objnum];
	ship		*shipp;
	model_subsystem *tp = eeo->turret_subsys->system_info;
	float dist;

	// Don't look for bombs when weapon system is not ok
	if (objp->type == OBJ_WEAPON && !eeo->weapon_system_ok) {
		return;
	}

	if ( !valid_turret_enemy(objp, turret_parent_obj) ) {
		return;
	}

#ifndef NDEBUG
	if (!Player_attacking_enabled && (objp == Player_obj)) {
		return;
	}
#endif

	if ( objp->type == OBJ_SHIP ) {
		shipp = &Ships[objp->instance];

		// check on enemy team
		if ( !(shipp->team & eeo->enemy_team_mask) ) {
			return;
		}

		// check if protected
		if (objp->flags & OF_PROTECTED) {
			return;
		}

		// check if beam protected
		if (Weapon_info[tp->turret_weapon_type].wi_flags & WIF_BEAM) {
			if (objp->flags & OF_BEAM_PROTECTED) {
				return;
			}
		}

		if (eeo->big_only_flag) {
			if (!(Ship_info[shipp->ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
				return;
			}
		}

		// check if	turret flagged to only target tagged ships
		if ( (eeo->turret_subsys->weapons.flags & SW_FLAG_TAGGED_ONLY) && !ship_is_tagged(objp) ) {
			return;
		}

		// check if valid target in nebula
		if ( !object_is_targetable(objp, &Ships[Objects[eeo->turret_parent_objnum].instance]) ) {
			// BYPASS ocassionally for stealth
			int try_anyway = FALSE;
			if ( is_object_stealth_ship(objp) ) {
				float turret_stealth_find_chance = 0.5f;
				float speed_mod = -0.1f + vm_vec_mag_quick(&objp->phys_info.vel) / 70.0f;
				if (frand() > (turret_stealth_find_chance + speed_mod)) {
					try_anyway = TRUE;
				}
			}

			if (!try_anyway) {
				return;
			}
		}

	} else {
		shipp = NULL;
	}

	// modify dist for BIG|HUGE, getting closest point on bbox, if not inside
	dist = vm_vec_dist_quick(eeo->tpos, &objp->pos) - objp->radius;
	if (dist < 0.0f) {
		dist = 0.0f;
	}

	// check if object is a bomb attacking the turret parent
	// check if bomb is homing on the turret parent ship
	if (objp->type == OBJ_WEAPON) {
		if ( Weapons[objp->instance].homing_object == &Objects[eeo->turret_parent_objnum] ) {
			if ( dist < eeo->nearest_homing_bomb_dist ) {
				if ( (eeo->current_enemy == -1) || object_in_turret_fov(objp, tp, eeo->tvec, eeo->tpos, dist + objp->radius) ) {
					eeo->nearest_homing_bomb_dist = dist;
					eeo->nearest_homing_bomb_objnum = OBJ_INDEX(objp);
				}
			}
		// if not homing, check if bomb is flying towards ship
		} else if ( bomb_headed_towards_ship(objp, &Objects[eeo->turret_parent_objnum]) ) {
			if ( dist < eeo->nearest_bomb_dist ) {
				if ( (eeo->current_enemy == -1) || object_in_turret_fov(objp, tp, eeo->tvec, eeo->tpos, dist + objp->radius) ) {
					eeo->nearest_bomb_dist = dist;
					eeo->nearest_bomb_objnum = OBJ_INDEX(objp);
				}
			}
		}
	} // end weapon section

	// maybe recalculate dist for big or huge ship
//	if (shipp && (Ship_info[shipp->ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
//		fvi_ray_boundingbox(min, max, start, direction, hit);
//		dist = vm_vec_dist_quick(hit, tvec);
//	}

	// check for nearest attcker
	if ( (shipp) && (dist < eeo->weapon_travel_dist) ) {
		ai_info *aip = &Ai_info[shipp->ai_index];

		// modify distance based on number of turrets from my ship attacking enemy (add 10% per turret)
		// dist *= (num_enemies_attacking(OBJ_INDEX(objp))+2)/2;	//	prevents lots of ships from attacking same target
		int num_att_turrets = num_turrets_attacking(turret_parent_obj, OBJ_INDEX(objp));
		dist *= (1.0f + 0.1f*num_att_turrets);

		// return if we're over the cap
		int max_turrets = 3 + Game_skill_level * Game_skill_level;
		if (num_att_turrets > max_turrets) {
			return;
		}

		// modify distance based on lethality of objp to my ship
		float active_lethality = aip->lethality;
		if (objp->flags & OF_PLAYER_SHIP) {
			active_lethality += Player_lethality_bump[Game_skill_level];
		}

		dist /= (1.0f + 0.01f*Lethality_range_const*active_lethality);

		// Make level 2 tagged ships more likely to be targeted
		if (shipp->level2_tag_left > 0.0f) {
			dist *= 0.3f;
		}

		// check if objp is targeting the turret's ship, or if objp has just hit the turret's ship
		if ( aip->target_objnum == eeo->turret_parent_objnum || aip->last_objsig_hit == Objects[eeo->turret_parent_objnum].signature ) {
			// A turret will always target a ship that is attacking itself... self-preservation!
			if ( aip->targeted_subsys == eeo->turret_subsys ) {
				dist *= 0.5f;	// highest priority
			}
		}

		// maybe update nearest attacker
		if ( dist < eeo->nearest_attacker_dist ) {
			if ( (eeo->current_enemy == -1) || object_in_turret_fov(objp, tp, eeo->tvec, eeo->tpos, dist + objp->radius) ) {
				// nprintf(("AI", "Nearest enemy = %s, dist = %7.3f, dot = %6.3f, fov = %6.3f\n", Ships[objp->instance].ship_name, dist, vm_vec_dot(&v2e, tvec), tp->turret_fov));
				eeo->nearest_attacker_dist = dist;
				eeo->nearest_attacker_objnum = OBJ_INDEX(objp);
			}
		}
	} // end ship section

#ifdef MAKE_FS1
	// check if object is an asteroid attacking the turret parent - taylor
	if (objp->type == OBJ_ASTEROID) {
		if ( eeo->turret_parent_objnum == asteroid_collide_objnum(objp) ) {
			// give priority to the closest asteroid *impact* (ms intervals)
			dist *= 0.9f + (0.01f * asteroid_time_to_impact(objp));

			if (dist < eeo->nearest_dist ) {
				if ( (eeo->current_enemy == -1) || object_in_turret_fov(objp, tp, eeo->tvec, eeo->tpos, dist + objp->radius) ) {
					eeo->nearest_dist = dist;
					eeo->nearest_objnum = OBJ_INDEX(objp);
				}
			}
		}
	} // end asteroid selection
#endif
}

// return 0 only if objnum is beam protected and turret is beam turret
int is_target_beam_valid(ship_subsys *turret_subsys, int objnum)
{
	// check if turret has beam weapon
	model_subsystem *tp = turret_subsys->system_info;

	if (Weapon_info[tp->turret_weapon_type].wi_flags & WIF_BEAM) {
		if (Objects[objnum].flags & OF_BEAM_PROTECTED) {
			return 0;
		}

		if (Weapon_info[tp->turret_weapon_type].wi_flags & WIF_HUGE) {
			if (Objects[objnum].type == OBJ_SHIP && !(Ship_info[Ships[Objects[objnum].instance].ship_info_index].flags & (SIF_BIG_SHIP|SIF_HUGE_SHIP)) ) {
				return 0;
			}
		}
	}

	return 1;
}


//	Given an object and an enemy team, return the index of the nearest enemy object.
//
// input:
//				turret_parent_objnum	=> parent objnum for the turret
//				turret_subsys			=> pointer to system_info for the turret subsystem
//				enemy_team_mask		=> OR'ed TEAM_ flags for the enemy of the turret parent ship
//				tpos						=> position of turret (world coords)
//				tvec						=> forward vector of turret (world coords)
//				current_enemy			=>	objnum of current turret target
int get_nearest_turret_objnum(int turret_parent_objnum, ship_subsys *turret_subsys, int enemy_team_mask, vector *tpos, vector *tvec, int current_enemy, int big_only_flag)
{
	float					weapon_travel_dist;
	int					weapon_system_ok;
	object				*objp;
	model_subsystem	*tp;
	eval_enemy_obj_struct eeo;

	// list of stuff to go thru
	ship_obj		*so;
	missile_obj *mo;

	tp = turret_subsys->system_info;
	weapon_travel_dist = min(Weapon_info[tp->turret_weapon_type].lifetime * Weapon_info[tp->turret_weapon_type].max_speed, Weapon_info[tp->turret_weapon_type].weapon_range);

	// Set flag based on strength of weapons subsystem.  If weapons subsystem is destroyed, don't let turrets fire at bombs
	weapon_system_ok = 0;
	if ( ship_get_subsystem_strength( &Ships[Objects[turret_parent_objnum].instance], SUBSYSTEM_WEAPONS ) > 0 ) {
		weapon_system_ok = 1;
	}

	// Initialize eeo struct.
	eeo.turret_parent_objnum = turret_parent_objnum;
	eeo.weapon_system_ok = weapon_system_ok;
	eeo.weapon_travel_dist = weapon_travel_dist;
	eeo.big_only_flag = big_only_flag;
	eeo.enemy_team_mask = enemy_team_mask;
	eeo.current_enemy = current_enemy;
	eeo.tpos = tpos;
	eeo.tvec = tvec;
	eeo.turret_subsys = turret_subsys;

	eeo.nearest_attacker_dist = 99999.0f;
	eeo.nearest_attacker_objnum = -1;

	eeo.nearest_homing_bomb_dist = 99999.0f;
	eeo.nearest_homing_bomb_objnum = -1;

	eeo.nearest_bomb_dist = 99999.0f;
	eeo.nearest_bomb_objnum = -1;

	eeo.nearest_dist = 99999.0f;
	eeo.nearest_objnum = -1;


	// Missile_obj_list
	for( mo = GET_FIRST(&Missile_obj_list); mo != END_OF_LIST(&Missile_obj_list); mo = GET_NEXT(mo) ) {
		objp = &Objects[mo->objnum];
		evaluate_obj_as_target(objp, &eeo);
	}
	// highest priority
	if ( eeo.nearest_homing_bomb_objnum != -1 ) {					// highest priority is an incoming homing bomb
		return eeo.nearest_homing_bomb_objnum;
	} else if ( eeo.nearest_bomb_objnum != -1 ) {					// next highest priority is an incoming dumbfire bomb
		return eeo.nearest_bomb_objnum;
	}


	// Ship_used_list
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		evaluate_obj_as_target(objp, &eeo);
	}

	Assert(eeo.nearest_attacker_objnum < 0 || is_target_beam_valid(turret_subsys, eeo.nearest_attacker_objnum));
		// next highest priority is attacking ship
	if ( eeo.nearest_attacker_objnum != -1 ) {			// next highest priority is an attacking ship
		return eeo.nearest_attacker_objnum;
	 }


#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
		asteroid_obj *ao;
	// Asteroid_obj_list
	for( ao = GET_FIRST(&Asteroid_obj_list); ao != END_OF_LIST(&Asteroid_obj_list); ao = GET_NEXT(ao) ) {
		objp = &Objects[ao->objnum];
		evaluate_obj_as_target(objp, &eeo);
	}
#endif

	return eeo.nearest_objnum;										// lowest priority is the closest enemy objnum
}

//	Return timestamp until a ship can find an enemy.
//	Yes, no parameters.  Based solely on skill level.
int get_enemy_timestamp()
{
	return (NUM_SKILL_LEVELS - Game_skill_level) * ( (myrand() % 500) + 500);
}

// -------------------------------------------------------------------
//	Return objnum if enemy found, else return -1;
//	Don't attack a ship that already has at least max_attackers attacking it.
int find_enemy(int objnum, float range, int max_attackers)
{
	int	enemy_team_mask;

	enemy_team_mask = get_enemy_team_mask(objnum);

	//	if target_objnum != -1, use that as goal.
	ai_info	*aip = &Ai_info[Ships[Objects[objnum].instance].ai_index];
	if (timestamp_elapsed(aip->choose_enemy_timestamp)) {
		aip->choose_enemy_timestamp = timestamp(get_enemy_timestamp());
		if (aip->target_objnum != -1) {
			int	target_objnum = aip->target_objnum;

			// DKA don't undo object as target in nebula missions.
			// This could cause attack on ship on fringe on nebula to stop if attackee moves our of nebula range.  (BAD)
			if ( (Objects[target_objnum].signature == aip->target_signature) ) {
				if (Ships[Objects[target_objnum].instance].team & enemy_team_mask) {
					if (!(Objects[target_objnum].flags & OF_PROTECTED)) {
						// nprintf(("AI", "Frame %i: Object %i resuming goal of object %i\n", AI_FrameCount, objnum, target_objnum));
						return target_objnum;
					}
				}
			} else {
				aip->target_objnum = -1;
				aip->target_signature = -1;
			}
		}
		return get_nearest_objnum(objnum, enemy_team_mask, aip->enemy_wing, range, max_attackers);
	} else {
		aip->target_objnum = -1;
		aip->target_signature = -1;
		return -1;
	}

}

int Use_parent_target = 0;
DCF_BOOL(use_parent_target, Use_parent_target)

// -------------------------------------------------------------------
//	Return objnum if enemy found, else return -1;
//
// input:
//				turret_subsys	=> pointer to turret subsystem
//				objnum			=> parent objnum for the turret
//				tpos				=> position of turret (world coords)
//				tvec				=> forward vector of turret (world coords)
//				current_enemy	=>	objnum of current turret target
int find_turret_enemy(ship_subsys *turret_subsys, int objnum, vector *tpos, vector *tvec, int current_enemy, float fov, int big_only_flag = 0)
{
	int					enemy_team_mask, enemy_objnum;
	model_subsystem	*tp;
	ship_info			*sip;

	tp = turret_subsys->system_info;
	enemy_team_mask = get_enemy_team_mask(objnum);

	//	If a small ship and target_objnum != -1, use that as goal.
	ai_info	*aip = &Ai_info[Ships[Objects[objnum].instance].ai_index];
	sip = &Ship_info[Ships[Objects[objnum].instance].ship_info_index];

	if ((sip->flags & SIF_SMALL_SHIP) && (aip->target_objnum != -1)) {
		int target_objnum = aip->target_objnum;

		if (Objects[target_objnum].signature == aip->target_signature) {
			if (Ships[Objects[target_objnum].instance].team & enemy_team_mask) {
				if ( !(Objects[target_objnum].flags & OF_PROTECTED) ) {		// check this flag as well.
					// nprintf(("AI", "Frame %i: Object %i resuming goal of object %i\n", AI_FrameCount, objnum, target_objnum));
					return target_objnum;
				}
			}
		} else {
			aip->target_objnum = -1;
			aip->target_signature = -1;
		}
	// Not small or small with target objnum
	} else {
		// maybe use aip->target_objnum as next target
		if ((frand() < 0.8f) && (aip->target_objnum != -1) && Use_parent_target) {

			//check if aip->target_objnum is valid target
			int target_flags = Objects[aip->target_objnum].flags;
			if ( target_flags & OF_PROTECTED ) {
				// AL 2-27-98: why is a protected ship being targeted?
				set_target_objnum(aip, -1);
				return -1;
			}

			// maybe use ship target_objnum if valid for turret
			// check for beam weapon and beam protected
			if ( !((target_flags & OF_BEAM_PROTECTED) && (Weapon_info[tp->turret_weapon_type].wi_flags & WIF_BEAM)) ) {
				if ( Objects[aip->target_objnum].type == OBJ_SHIP ) {
					// check for huge weapon and huge ship
					if ( !big_only_flag || (Ship_info[Ships[Objects[aip->target_objnum].instance].ship_info_index].flags & (SIF_BIG_SHIP|SIF_HUGE_SHIP)) ) {
						// check for tagged only and tagged ship
						if ( (turret_subsys->weapons.flags & SW_FLAG_TAGGED_ONLY) && ship_is_tagged(&Objects[aip->target_objnum]) ) {
							// select new target if aip->target_objnum is out of field of view
							vector v2e;
							float dot, dist;
							dist = vm_vec_normalized_dir(&v2e, &Objects[aip->target_objnum].pos, tpos);
							dot = vm_vec_dot(&v2e, tvec);
							//	MODIFY FOR ATTACKING BIG SHIP
							// dot += (0.5f * Objects[aip->target_objnum].radius / dist);
							if (dot > fov) {
								return aip->target_objnum;
							}
						}
					}
				}
			}
		}
	}

	enemy_objnum = get_nearest_turret_objnum(objnum, turret_subsys, enemy_team_mask, tpos, tvec, current_enemy, big_only_flag);
	if ( enemy_objnum >= 0 ) {
		Assert( !((Objects[enemy_objnum].flags & OF_BEAM_PROTECTED) && (Weapon_info[tp->turret_weapon_type].wi_flags & WIF_BEAM)) );
		if ( Objects[enemy_objnum].flags & OF_PROTECTED ) {
			Int3();
			enemy_objnum = aip->target_objnum;
		}
	}

	return enemy_objnum;
}

//	If issued an order to a ship that's awaiting repair, abort that process.
//	However, do not abort process for an object that is currently being repaired -- let it finish.
void ai_set_goal_maybe_abort_dock(object *objp, ai_info *aip)
{
	if (aip->ai_flags & AIF_AWAITING_REPAIR) {
		object	*repair_obj;

		if (aip->dock_objnum == -1) {
			repair_obj = NULL;
		} else {
			repair_obj = &Objects[aip->dock_objnum];
		}
		ai_do_objects_repairing_stuff( objp, repair_obj, REPAIR_INFO_ABORT );
	}
	aip->next_rearm_request_timestamp = timestamp(NEXT_REARM_TIMESTAMP);	//	Might request again after 30 seconds.
}

void force_avoid_player_check(object *objp, ai_info *aip)
{
	if (Ships[objp->instance].team == Player_ship->team){
		aip->avoid_check_timestamp = timestamp(0);		//	Force a check for collision next frame.
	}
}

//	--------------------------------------------------------------------------
//	Set *attacked as object to attack for object *attacker
//	If attacked == NULL, then attack any enemy object.
//	Attack point *rel_pos on object.  This is for supporting attacking subsystems.
void ai_attack_object(object *attacker, object *attacked, int priority, ship_subsys *ssp)
{
	ai_info	*aip;

	Assert(attacker != NULL);
	Assert(attacker->instance != -1);
	Assert(Ships[attacker->instance].ai_index != -1);

	aip = &Ai_info[Ships[attacker->instance].ai_index];
	force_avoid_player_check(attacker, aip);

	aip->ok_to_target_timestamp = timestamp(0);		//	Guarantee we can target.

//	if (!strnicmp(Ships[attacker->instance].ship_name, NOX("Kami"), 4)) {
//		aip->ai_flags |= AIF_KAMIKAZE;
//		aip->ai_flags |= AIF_NO_DYNAMIC;
//	}

	if (attacker == attacked) {
		Int3();		//	Bogus!  Who tried to get me to attack myself!  Trace out and fix!
		return;
	}

	//	Only set to chase if a fighter or bomber, otherwise just return.
	if (!(Ship_info[Ships[attacker->instance].ship_info_index].flags & SIF_SMALL_SHIP)) {
//		nprintf(("AI","Note: AI ship %s refusing to set AI mode to AIM_CHASE\n", Ships[attacker->instance].ship_name));
//		return;
		nprintf(("AI", "AI ship %s is large ship ordered to attack %s\n", Ships[attacker->instance].ship_name, Ships[attacked->instance].ship_name));
	}

	//	This is how "engage enemy" gets processed
	if (attacked == NULL) {
		aip->choose_enemy_timestamp = timestamp(0);
		// nebula safe
		set_target_objnum(aip, find_enemy(attacker-Objects, 99999.9f, 4));
	} else {
		// check if we can see atacked in nebula
		if (aip->target_objnum != attacked - Objects) {
			aip->aspect_locked_time = 0.0f;
		}
		set_target_objnum(aip, attacked - Objects);
	}

	ai_set_goal_maybe_abort_dock(attacker, aip);
	aip->ok_to_target_timestamp = timestamp(DELAY_TARGET_TIME);	//	No dynamic targeting for 7 seconds.

	if (is_ignore_object(aip, aip->target_objnum)) {
		aip->ignore_objnum = UNUSED_OBJNUM;
	}

	aip->mode = AIM_CHASE;
	aip->submode = SM_ATTACK;	// AL 12-15-97: need to set submode?  I got an assert() where submode was bogus
										//					 for AIM_CHASE... it may have been not set correctly here
	if (ssp == NULL) {
		set_targeted_subsys(aip, NULL, -1);
		if (aip->target_objnum != -1) {
			//nprintf(("AI", "Unprotecting ship %s\n", Ships[Objects[aip->target_objnum].instance].ship_name));
			Objects[aip->target_objnum].flags &= ~OF_PROTECTED;	//	If ship had been protected, unprotect it.
		}
	} else {
		Int3();	//	Not supported yet!
	}
}

//	--------------------------------------------------------------------------
//	Set *attacked as object to attack for object *attacker
//	Attack point *rel_pos on object.  This is for supporting attacking subsystems.
void ai_attack_wing(object *attacker, int wingnum, int priority)
{
	ai_info	*aip;

	Assert(attacker != NULL);
	Assert(attacker->instance != -1);
	Assert(Ships[attacker->instance].ai_index != -1);

	aip = &Ai_info[Ships[attacker->instance].ai_index];

	aip->enemy_wing = wingnum;
	aip->mode = AIM_CHASE;
	aip->submode = SM_ATTACK;	// AL 12-15-97: need to set submode?  I got an assert() where submode was bogus
										//					 for AIM_CHASE... it may have been not set correctly here

	aip->ok_to_target_timestamp = timestamp(0);		//	Guarantee we can target.

	int count = Wings[wingnum].current_count;
	if (count > 0) {
		int	index;

		index = (int) (frand() * count);

		if (index >= count)
			index = 0;

		set_target_objnum(aip, Ships[Wings[wingnum].ship_index[index]].objnum);

		ai_set_goal_maybe_abort_dock(attacker, aip);
		aip->ok_to_target_timestamp = timestamp(DELAY_TARGET_TIME);	//	No dynamic targeting for 7 seconds.
	}
}

//	--------------------------------------------------------------------------
//	Set *evaded as object for *evader to evade.
void ai_evade_object(object *evader, object *evaded, int priority)
{
	ai_info	*aip;

	Assert(evader != NULL);
	Assert(evaded != NULL);
	Assert(evader->instance != -1);
	Assert(Ships[evader->instance].ai_index != -1);

	if (evaded == evader) {
		Int3();	//	Bogus!  Who tried to get me to evade myself!  Trace out and fix!
		return;
	}

	aip = &Ai_info[Ships[evader->instance].ai_index];

	set_target_objnum(aip, evaded - Objects);
	aip->mode = AIM_EVADE;

}

//	Ignore some object without changing mode.
void ai_ignore_object(object *ignorer, object *ignored, int priority)
{
	ai_info	*aip;

	Assert(ignorer != NULL);
	Assert(ignored != NULL);
	Assert(ignorer->instance != -1);
	Assert(Ships[ignorer->instance].ai_index != -1);
	Assert(ignorer != ignored);

	aip = &Ai_info[Ships[ignorer->instance].ai_index];

	//	MK, 5/17/98, removing ignoring of wings.
	//	It's too confusing.  It often causes mysterious behavior in which fighters unexpectedly refuse to attack anything.
/*	if (Ships[ignored->instance].wingnum > -1) {
		int wingnum, i;

		wingnum = Ships[ignored->instance].wingnum;
		aip->ignore_objnum = -(wingnum+1);
		// set protected bit for each ship in a wing
		//	MK, 4/23/98: Only set for fighters if they are the original "ignored" object
		for (i = 0; i < Wings[wingnum].current_count; i++ ) {
			object	*objp;

			objp = &Objects[Ships[Wings[wingnum].ship_index[i]].objnum];
			if (objp != ignored) {
				if (Ship_info[Ships[objp->instance].ship_info_index].flags & (SIF_FIGHTER | SIF_BOMBER))
					continue;
			}

			Objects[Ships[Wings[wingnum].ship_index[i]].objnum].flags |= OF_PROTECTED;
		}

	} else {
	*/ {
		aip->ignore_objnum = ignored - Objects;
		aip->ignore_signature = ignored->signature;
		aip->ai_flags &= ~AIF_TEMPORARY_IGNORE;
		ignored->flags |= OF_PROTECTED;					// set protected bit of ignored ship.
	}

}

//	Ignore some object without changing mode.
void ai_ignore_wing(object *ignorer, int wingnum, int priority)
{
	ai_info	*aip;

	Assert(ignorer != NULL);
	Assert(ignorer->instance != -1);
	Assert(Ships[ignorer->instance].ai_index != -1);
	Assert((wingnum >= 0) && (wingnum < MAX_WINGS));

	aip = &Ai_info[Ships[ignorer->instance].ai_index];

	aip->ignore_objnum = -(wingnum +1);
	aip->ai_flags &= ~AIF_TEMPORARY_IGNORE;
}


//	Add a path point in the global buffer Path_points.
//	modify_index = index in Path_points at which to store path point.
//	If modify_index == -1, then create a new point.
//	If a new point is created (ie, modify_index == -1), then Ppfp is updated.
void add_path_point(vector *pos, int path_num, int path_index, int modify_index)
{
	pnode	*pnp;

	if (modify_index == -1) {
		Assert(Ppfp-Path_points < MAX_PATH_POINTS-1);
		pnp = Ppfp;
		Ppfp++;
	} else {
		Assert((modify_index >= 0) && (modify_index < MAX_PATH_POINTS-1));
		pnp = &Path_points[modify_index];
	}

	pnp->pos = *pos;
	pnp->path_num = path_num;
	pnp->path_index = path_index;
}

//	Given two points on a sphere, the center of the sphere and the radius, return a
//	point on the vector through the midpoint of the chord on the sphere.
void bisect_chord(vector *p0, vector *p1, vector *centerp, float radius)
{
	vector	tvec;
	vector	new_pnt;

	vm_vec_add(&tvec, p0, p1);
	vm_vec_sub2(&tvec, centerp);
	vm_vec_sub2(&tvec, centerp);
	if (vm_vec_mag_quick(&tvec) < 0.1f) {
		vm_vec_sub(&tvec, p0, p1);
		if (fl_abs(tvec.xyz.x) <= fl_abs(tvec.xyz.z)){
			tvec.xyz.x = -tvec.xyz.z;
		} else {
			tvec.xyz.y = -tvec.xyz.x;
		}
	}

	vm_vec_normalize(&tvec);
	vm_vec_scale(&tvec, radius);
	vm_vec_add(&new_pnt, centerp, &tvec);

	add_path_point(&new_pnt, -1, -1, -1);
}
			
//	Create a path from the current position to a goal position.
//	The current position is in the current object and the goal position is
//	in the goal object.
//	It is ok to intersect the current object, but not the goal object.
//	This function is useful for creating a path to an initial point near a large
//	object.
//
// input:	subsys_path:	optional param (default 0), indicates this is a path to a subsystem
void create_path_to_point(vector *curpos, vector *goalpos, object *curobjp, object *goalobjp, int subsys_path)
{
	//	If can't cast vector to goalpos, then create an intermediate point.
	if (pp_collide(curpos, goalpos, goalobjp, curobjp->radius)) {
		vector	tan1;
		float		radius;

		// If this is a path to a subsystem, use SUBSYS_PATH_DIST as the radius for the object you are
		// trying to avoid.  This is needed since subsystem paths extend out to SUBSYS_PATH_DIST, and we
		// want ships to reach their path destination without flying to points that sit on the radius of
		// a small ship
		radius = goalobjp->radius;
		if (subsys_path) {
			if ( SUBSYS_PATH_DIST > goalobjp->radius ) {
				radius = SUBSYS_PATH_DIST;
			}
		}

		//	The intermediate point is at the intersection of:
		//		tangent to *goalobjp sphere at point *goalpos
		//		tangent to *goalobjp sphere through point *curpos in plane defined by *curpos, *goalpos, goalobjp->pos
		//	Note, there are two tangents through *curpos, unless *curpos is on the
		//	sphere.  The tangent that causes the nearer intersection (to *goalpos) is chosen.
		get_tangent_point(&tan1, curpos, &goalobjp->pos, goalpos, radius);

		//	If we can't reach tan1 from curpos, insert a new point.
		if (pp_collide(&tan1, curpos, goalobjp, curobjp->radius))
			bisect_chord(curpos, &tan1, &goalobjp->pos, radius);

		add_path_point(&tan1, -1, -1, -1);

		//	If we can't reach goalpos from tan1, insert a new point.
		if (pp_collide(goalpos, &tan1, goalobjp, curobjp->radius))
			bisect_chord(goalpos, &tan1, &goalobjp->pos, radius);
	}

}

//	Given an object and a model path, globalize the points on the model
//	and copy into the global path list.
//	If pnp != NULL, then modify, in place, the path points.  This is used to create new
//	globalized points when the base object has moved.
// input:	randomize_pnt	=> optional parameter (default value -1), add random vector in sphere to this path point
void copy_xlate_model_path_points(object *objp, model_path *mp, int dir, int count, int path_num, pnode *pnp, int randomize_pnt)
{
	matrix	m;
	int		i;
	vector	v1;
	int		pp_index;		//	index in Path_points at which to store point, if this is a modify-in-place (pnp ! NULL)
	int		start_index, finish_index;
	
	// nprintf(("AI", "Creating path for object %s in frame #%i\n", Ships[objp->instance].ship_name, AI_FrameCount));
	
	//	Initialize pp_index.
	//	If pnp == NULL, that means we're creating new points.  If not NULL, then modify in place.
	if (pnp == NULL)
		pp_index = -1;			//	This tells add_path_point to create a new point.
	else
		pp_index = 0;			//	pp_index will get assigned to index in Path_points to reuse.

	vm_copy_transpose_matrix(&m, &objp->orient);

	if (dir == 1) {
		start_index = 0;
		finish_index = min(count, mp->nverts);
	} else {
		Assert(dir == -1);	//	direction must be up by 1 or down by 1 and it's neither!
		start_index = mp->nverts-1;
		finish_index = max(-1, mp->nverts-1-count);
	}

	int offset = 0;
	for (i=start_index; i != finish_index; i += dir) {
		//	Globalize the point.
		vm_vec_rotate(&v1, &mp->verts[i].pos, &m);
		vm_vec_add2(&v1, &objp->pos);

		if ( randomize_pnt == i ) {
			vector v_rand;
			static_randvec(OBJ_INDEX(objp), &v_rand);
			vm_vec_scale(&v_rand, 30.0f);
			vm_vec_add2(&v1, &v_rand);
		}

		if (pp_index != -1)
			pp_index = pnp-Path_points + offset;

		add_path_point(&v1, path_num, i, pp_index);
		offset++;
	}
}


//	For pl_objp, create a path along path path_num into mobjp.
//	The tricky part of this problem is creating the entry to the first point on the
//	predefined path.  The points on this entry path are based on the location of Pl_objp
//	relative to the start of the path.
//
// input:
//				subsys_path:	optional param (default 0), indicating this is a path to a subsystem
void create_model_path(object *pl_objp, object *mobjp, int path_num, int subsys_path)
{	
	ship			*shipp = &Ships[pl_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];

	ship_info	*osip = &Ship_info[Ships[mobjp->instance].ship_info_index];
	polymodel	*pm = model_get(osip->modelnum);
	int			num_points;
	model_path	*mp;
	pnode			*ppfp_start = Ppfp;
	matrix		m;
	vector		gp0;

	Assert(path_num >= 0);

	//	Do garbage collection if necessary.
	if (Ppfp-Path_points + 64 > MAX_PATH_POINTS) {
		garbage_collect_path_points();
		ppfp_start = Ppfp;
	}

	aip->path_start = Ppfp - Path_points;
	Assert(path_num < pm->n_paths);
	
	mp = &pm->paths[path_num];
	num_points = mp->nverts;

	Assert(Ppfp-Path_points + num_points + 4 < MAX_PATH_POINTS);

	vm_copy_transpose_matrix(&m, &mobjp->orient);
	vm_vec_rotate(&gp0, &mp->verts[0].pos, &m);
	vm_vec_add2(&gp0, &mobjp->pos);

	if (pp_collide(&pl_objp->pos, &gp0, mobjp, pl_objp->radius)) {
		vector	perim_point1;
		vector	perim_point2;

		perim_point2 = pl_objp->pos;
		
		//	If object that wants to dock is inside bounding sphere of object it wants to dock with, make it fly out.
		//	Assume it can fly "straight" out to the bounding sphere.
		if (vm_vec_dist_quick(&pl_objp->pos, &mobjp->pos) < mobjp->radius) {
			project_point_to_perimeter(&perim_point2, &mobjp->pos, mobjp->radius, &pl_objp->pos);
			add_path_point(&perim_point2, path_num, -1, -1);
		}

		//	If last point on pre-defined path is inside bounding sphere, create a new point on the surface of the sphere.
		if (vm_vec_dist_quick(&mobjp->pos, &gp0) < mobjp->radius) {
			project_point_to_perimeter(&perim_point1, &mobjp->pos, mobjp->radius, &gp0);
			create_path_to_point(&perim_point2, &perim_point1, pl_objp, mobjp, subsys_path);
			add_path_point(&perim_point1, path_num, -1, -1);
		} else {		//	The predefined path extends outside the sphere.  Create path to that point.
			create_path_to_point(&perim_point2, &gp0, pl_objp, mobjp, subsys_path);
		}
	}

	// AL 12-31-97: If following a subsystem path, add random vector to second last path point
	if ( subsys_path ) {
		copy_xlate_model_path_points(mobjp, mp, 1, mp->nverts, path_num, NULL, mp->nverts-2);
	} else {
		copy_xlate_model_path_points(mobjp, mp, 1, mp->nverts, path_num, NULL);
	}

	aip->path_cur = aip->path_start;
	aip->path_dir = PD_FORWARD;
	aip->path_objnum = mobjp-Objects;
	aip->mp_index = path_num;
	aip->path_length = Ppfp - ppfp_start;
	aip->path_next_check_time = timestamp(1);

	aip->path_goal_obj_hash = create_object_hash(&Objects[aip->path_objnum]);

	aip->path_next_create_time = timestamp(1000);	//	OK to try to create one second later
	aip->path_create_pos = pl_objp->pos;
	aip->path_create_orient = pl_objp->orient;

	aip->ai_flags &= ~AIF_USE_EXIT_PATH;			// ensure this flag is cleared
}

//	For pl_objp, create a path along path path_num into mobjp.
//	The tricky part of this problem is creating the entry to the first point on the
//	predefined path.  The points on this entry path are based on the location of pl_objp
//	relative to the start of the path.
void create_model_exit_path(object *pl_objp, object *mobjp, int path_num, int count)
{	
	ship			*shipp = &Ships[pl_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];

	ship_info	*osip = &Ship_info[Ships[mobjp->instance].ship_info_index];
	polymodel	*pm = model_get(osip->modelnum);
	int			num_points;
	model_path	*mp;
	pnode			*ppfp_start = Ppfp;

	aip->path_start = Ppfp - Path_points;
	Assert(path_num < pm->n_paths);
	
	mp = &pm->paths[path_num];
	num_points = mp->nverts;

	Assert(Ppfp-Path_points + num_points + 4 < MAX_PATH_POINTS);

	copy_xlate_model_path_points(mobjp, mp, -1, count, path_num, NULL);

	aip->path_cur = aip->path_start;
	aip->path_dir = PD_FORWARD;
	aip->path_objnum = mobjp-Objects;
	aip->mp_index = path_num;
	aip->path_length = Ppfp - ppfp_start;
	aip->path_next_check_time = timestamp(1);

	aip->ai_flags |= AIF_USE_EXIT_PATH;		// mark as exit path, referenced in maybe
}

//	Return true if the vector from curpos to goalpos intersects with any ship other than the ignore objects.
//	Calls pp_collide
int pp_collide_any(vector *curpos, vector *goalpos, float radius, object *ignore_objp1, object *ignore_objp2, int big_only_flag)
{
	ship_obj	*so;	

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		object *objp = &Objects[so->objnum];

		if (big_only_flag) {
			if (!(Ship_info[Ships[objp->instance].ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)))
				continue;
		}

		if ((objp != ignore_objp1) && (objp != ignore_objp2)) {
			if (pp_collide(curpos, goalpos, objp, radius))
				return OBJ_INDEX(objp);
		}
	}

	return -1;
}

//	Used to create docking paths and other pre-defined paths through ships.
//	Creates a path in absolute space.
//	Create a path into the object objnum.
//
// input:
//	pl_objp:			object that will use the path
//	objnum:			Object to find path to.
//	path_num:		model path index to use
//	exit_flag:		true means this is an exit path in the model
// subsys_path:	optional param (default 0) that indicates this is a path to a subsystem
//	Exit:
//	ai_info struct in Pl_objp gets stuffed with information to enable Pl_objp to fly the path.
void ai_find_path(object *pl_objp, int objnum, int path_num, int exit_flag, int subsys_path)
{
	ai_info	*aip = &Ai_info[Ships[pl_objp->instance].ai_index];

	Assert(path_num >= 0);

	//	This is test code, find an object with paths.
	if (objnum != -1) {
		object	*objp = &Objects[objnum];

		if (objp->type == OBJ_SHIP) {
			polymodel *pm;

			ship	*shipp = &Ships[objp->instance];
			pm = model_get( shipp->modelnum );
			Assert(pm->n_paths > path_num);
			aip->goal_objnum = objp-Objects;
			aip->goal_signature = objp->signature;
			if (exit_flag)
				create_model_exit_path(pl_objp, objp, path_num);
			else
				create_model_path(pl_objp, objp, path_num, subsys_path);
			return;
		}

	}
}

extern int vector_object_collision(vector *start_pos, vector *end_pos, object *objp, float radius_scale);

//	Maybe make *objp avoid a player object.
//	For now, 4/6/98, only check Player_obj.
//	If player collision would occur, set AIF_AVOIDING_SMALL_SHIP bit in ai_flags.
//	Set aip->avoid_goal_point
int maybe_avoid_player(object *objp, vector *goal_pos)
{
	ai_info	*aip;
	vector	cur_pos, new_goal_pos;
	object	*player_objp;
	vector	n_vec_to_goal, n_vec_to_player;

	aip = &Ai_info[Ships[objp->instance].ai_index];

	if (!timestamp_elapsed(aip->avoid_check_timestamp))
		return 0;

	player_objp = Player_obj;

	float	speed_time;

	//	How far two ships could be apart and still collide within one second.
	speed_time = player_objp->phys_info.speed + objp->phys_info.speed;

	float	obj_obj_dist;

	obj_obj_dist = vm_vec_dist_quick(&player_objp->pos, &objp->pos);

	if (obj_obj_dist > speed_time*2.0f)
		return 0;

	cur_pos = objp->pos;

	new_goal_pos = *goal_pos;

	float dist = vm_vec_normalized_dir(&n_vec_to_goal, goal_pos, &objp->pos);
	vm_vec_normalized_dir(&n_vec_to_player, &player_objp->pos, &objp->pos);

	if (dist > speed_time*2.0f) {
		vm_vec_scale_add(&new_goal_pos, &objp->pos, &n_vec_to_goal, 200.0f);
	}

	if (vector_object_collision(&objp->pos, &new_goal_pos, player_objp, 1.5f)) {
		aip->ai_flags |= AIF_AVOIDING_SMALL_SHIP;

		vector	avoid_vec;

		vm_vec_sub(&avoid_vec, &n_vec_to_goal, &n_vec_to_player);
		if (vm_vec_mag_quick(&avoid_vec) < 0.01f) {
			vm_vec_copy_scale(&avoid_vec, &objp->orient.v.rvec, frand()-0.5f);
			vm_vec_scale_add2(&avoid_vec, &objp->orient.v.uvec, frand()-0.5f);
			vm_vec_normalize(&avoid_vec);
		} else {
			vector	tvec1;
			vm_vec_normalize(&avoid_vec);
			vm_vec_crossprod(&tvec1, &n_vec_to_goal, &avoid_vec);
			vm_vec_crossprod(&avoid_vec, &tvec1, &n_vec_to_player);
		}

		//	Now, avoid_vec is a vector perpendicular to the vector to the player and the direction *objp
		//	should fly in to avoid the player while still approaching its goal.
		vm_vec_scale_add(&aip->avoid_goal_point, &player_objp->pos, &avoid_vec, 400.0f);

		aip->avoid_check_timestamp = timestamp(1000);

		return 1;
	} else {
		aip->ai_flags &= ~AIF_AVOIDING_SMALL_SHIP;
		aip->avoid_check_timestamp = timestamp((int) (obj_obj_dist/200.0f) + 500);

		return 0;
	}
}

//	Make object *still_objp enter AIM_STILL mode.
//	Make it point at view_pos.
void ai_stay_still(object *still_objp, vector *view_pos)
{
	ship	*shipp;
	ai_info	*aip;

	Assert(still_objp->type == OBJ_SHIP);
	Assert((still_objp->instance >= 0) && (still_objp->instance < MAX_OBJECTS));

	shipp = &Ships[still_objp->instance];
	Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

	aip = &Ai_info[shipp->ai_index];

	aip->mode = AIM_STILL;

	//	If view_pos not NULL, point at that point.  Else, point at a point directly in front of ship.  Ie, don't turn.
	if (view_pos != NULL)
		aip->goal_point = *view_pos;
	else
		vm_vec_scale_add(&aip->goal_point, &still_objp->pos, &still_objp->orient.v.fvec, 100.0f);
}

// code which is called from ai_dock_with_object and ai_dock to set flags and apprioriate variable
// when two objects have completed docking.  used because we can dock object initially at misison load
// time (meaning that ai_dock() might never get called).  docker has docked with dockee (i.e. docker
// would be a freighter and dockee would be a cargo).
void ai_do_objects_docked_stuff(object *docker, object *dockee)
{
	ai_info *aip, *other_aip;

	aip = &Ai_info[Ships[docker->instance].ai_index];
	other_aip = &Ai_info[Ships[dockee->instance].ai_index];

	// set the flags and dock_objnum for both objects
	aip->ai_flags |= AIF_DOCKED;
	aip->dock_objnum = OBJ_INDEX(dockee);
	other_aip->ai_flags |= AIF_DOCKED;
	other_aip->dock_objnum = OBJ_INDEX(docker);
	aip->dock_signature = dockee->signature;
	other_aip->dock_signature = docker->signature;

	// add multiplayer hook here to deal with docked objects.  We need to only send information
	// about the object that is docking.  Both flags will get updated.
	if ( MULTIPLAYER_MASTER )
		send_ai_info_update_packet( docker, AI_UPDATE_DOCK );

}

// code which is called when objects become undocked. Equivalent of above function.
// dockee might not be valid since this code can get called to cleanup after a ship
// has blown up!
void ai_do_objects_undocked_stuff( object *docker, object *dockee )
{
	ai_info *aip, *other_aip;

	// add multiplayer hook here to deal with undocked objects.  Do it before we
	// do anything else.  We don't need to send info for both objects, since we can find
	// it be dock_objnum
	if ( MULTIPLAYER_MASTER )
		send_ai_info_update_packet( docker, AI_UPDATE_UNDOCK );

	aip = &Ai_info[Ships[docker->instance].ai_index];

	// set the flags and dock_objnum for both objects
	aip->ai_flags &= ~(AIF_DOCKED | AIF_BEING_REPAIRED);
	aip->dock_objnum = -1;
	
	if ( dockee != NULL ) {
		other_aip = &Ai_info[Ships[dockee->instance].ai_index];
		other_aip->ai_flags &= ~(AIF_DOCKED | AIF_BEING_REPAIRED);
		other_aip->dock_objnum = -1;
	}

}


//	--------------------------------------------------------------------------
//	Interface from goals code to AI.
//	Cause *docker to dock with *dockee.
//	priority is priority of goal from goals code.
//	dock_type is:
//		AIDO_DOCK		set goal of docking
//		AIDO_DOCK_NOW	immediately dock, used for ships that need to be docked at mission start
//		AIDO_UNDOCK		set goal of undocking
void ai_dock_with_object(object *docker, object *dockee, int priority, int dock_type, int docker_index, int dockee_index)
{
	ai_info		*aip;
	polymodel	*pm;
	ai_info		*dockee_aip;

	Assert(docker != NULL);
	Assert(dockee != NULL);
	Assert(docker->instance != -1);
	Assert(Ships[docker->instance].ai_index != -1);
	Assert(Ships[dockee->instance].ai_index != -1);
	Assert( docker_index != -1 );
	Assert( dockee_index != -1 );

	aip = &Ai_info[Ships[docker->instance].ai_index];

	if ((aip->ai_flags & AIF_DOCKED) && (dock_type == AIDO_DOCK)) {
		object	*dockee2;
		int		docker_index2, dockee_index2;

		Assert(aip->dock_objnum > -1);
		dockee2 = &Objects[aip->dock_objnum];
		docker_index2 = aip->dock_index;
		dockee_index2 = aip->dockee_index;
		// MWA -- 2/9/98.  use the goal code to undock the ships since goals might need to get removed
		// and that code will do it properly.  I'd actually be surprised if we got into this code anymore
		// since the outer layer goal code should deal with this issue....but who knows...
		ai_add_goal_ship_internal( aip, AI_GOAL_UNDOCK, NULL, -1, -1, 0 );

		// old code below
		//ai_dock_with_object(docker, dockee2, priority, AIDO_UNDOCK, docker_index2, dockee_index2);
		nprintf(("AI", "Ship %s told to dock with %s, but it was already docked with %s.\n", Ships[docker->instance].ship_name, Ships[dockee->instance].ship_name, Ships[Objects[aip->dock_objnum].instance].ship_name));
		nprintf(("AI", "...so ship %s will now undock.\n", Ships[docker->instance].ship_name));
		return;
	}

	dockee_aip = &Ai_info[Ships[dockee->instance].ai_index];

	aip->goal_objnum = dockee - Objects;
	aip->goal_signature = dockee->signature;

	aip->mode = AIM_DOCK;

	switch (dock_type) {
	case AIDO_DOCK:
		aip->submode = AIS_DOCK_0;
		break;
	case AIDO_DOCK_NOW:
		aip->submode = AIS_DOCK_3A;
		break;
	case AIDO_UNDOCK:
		aip->submode = AIS_UNDOCK_0;
		break;
	default:
		Int3();		//	Bogus dock_type.
	}

	aip->submode_start_time = Missiontime;
	aip->dock_index = docker_index;
	aip->dockee_index = dockee_index;

	dockee_aip->dock_index = dockee_index;
	dockee_aip->dockee_index = docker_index;

	// get the path number to the docking point on the dockee.  Each docking point contains a list
	// of paths that the point can be reached by.  Pick the first path in the path list for now.
	// We only want to do this stuff if we are docking!!!  Be sure to set the path index
	if ((dock_type == AIDO_DOCK) || (dock_type == AIDO_DOCK_NOW)) {
		pm = model_get( Ships[dockee->instance].modelnum );
		Assert( pm->docking_bays[dockee_index].num_spline_paths > 0 );

		// only set the dock path index if we are docking.  undocking will assume that dock_path_index
		// already set from some other docking command
		aip->dock_path_index = dockee_index;
		dockee_aip->dock_path_index = docker_index;
	}

	if (dock_type != AIDO_DOCK_NOW) {
		int path_num;
		//	Note: Second parameter is dock path index.  This should be specified as an
		//	_input_ to this function and passed through.  The path index should be already
		// set for the undock function
		path_num = ai_return_path_num_from_dockbay(dockee, dockee_index);
		ai_find_path(docker, dockee-Objects, path_num, 0);
//		ai_find_path(dockee-Objects, dockee_index, 0);
	} else {
		dock_orient_and_approach(docker, dockee, DOA_DOCK_STAY);
		//aip->dock_objnum = OBJ_INDEX(dockee);
		ai_do_objects_docked_stuff( docker, dockee );
	}

}

//	Cause a ship to fly its waypoints.
//	flags tells:
//		WPF_REPEAT	Set -> repeat waypoints.
void ai_start_waypoints(object *objp, int waypoint_list_index, int wp_flags)
{
	ai_info	*aip;

	Assert(waypoint_list_index < Num_waypoint_lists);

	//nprintf(("AI", "Frame %i: Ship %s instructed to fly waypoint list #%i\n", AI_FrameCount, Ships[objp->instance].ship_name, waypoint_list_index));
	aip = &Ai_info[Ships[objp->instance].ai_index];

	if ( (aip->mode == AIM_WAYPOINTS) && (aip->wp_index == waypoint_list_index) )
		return;

	aip->ai_flags |= AIF_FORMATION_WING;
	aip->ai_flags &= ~AIF_FORMATION_OBJECT;
	aip->wp_list = waypoint_list_index;
	aip->wp_index = 0;
	aip->wp_flags = wp_flags;
	aip->mode = AIM_WAYPOINTS;

	Assert(aip->active_goal != AI_ACTIVE_GOAL_DYNAMIC);
}

//	Make *objp stay within dist units of *other_objp
void ai_do_stay_near(object *objp, object *other_objp, float dist)
{
	ai_info	*aip;

	Assert(objp != other_objp);		//	Bogus!  Told to stay near self.
	Assert(objp->type == OBJ_SHIP);
	Assert((objp->instance >= 0) && (objp->instance < MAX_SHIPS));

	aip = &Ai_info[Ships[objp->instance].ai_index];

	aip->mode = AIM_STAY_NEAR;
	aip->submode = -1;
	aip->stay_near_distance = dist;
	aip->goal_objnum = other_objp-Objects;
	aip->goal_signature = other_objp->signature;

}

//	Make object *objp form on wing of object *goal_objp
void ai_form_on_wing(object *objp, object *goal_objp)
{
	ai_info	*aip;
	ship			*shipp;
	ship_info	*sip;

	// objp == goal_objp sometimes in multiplayer when someone leaves a game -- make a simple
	// out for this case.
	if ( Game_mode & GM_MULTIPLAYER ) {
		if ( objp == goal_objp ) {
			return;
		}
	}

	Assert(objp != goal_objp);		//	Bogus!  Told to form on own's wing!

	shipp = &Ships[objp->instance];
	sip = &Ship_info[shipp->ship_info_index];

	//	Only fighters or bombers allowed to form on wing.
	if (!(sip->flags & (SIF_FIGHTER | SIF_BOMBER))) {
		nprintf(("AI", "Warning: Ship %s tried to form on player's wing, but not fighter or bomber.\n", shipp->ship_name));
		return;
	}

	aip = &Ai_info[Ships[objp->instance].ai_index];

	aip->ai_flags &= ~AIF_FORMATION_WING;
	aip->ai_flags |= AIF_FORMATION_OBJECT;

	aip->goal_objnum = goal_objp-Objects;
	ai_set_goal_maybe_abort_dock(objp, aip);
	aip->ok_to_target_timestamp = timestamp(DELAY_TARGET_TIME*4);		//	Super extra long time until can target another ship.

}

//	Given an object and an object on whose wing to form, return slot to use.
//	Optimize:
//		This function is called per object in formation per frame.  Should store slot in ai_info struct.
int ai_formation_object_get_slotnum(int objnum, object *objp)
{
	int	slotnum = 1;			//	Note: Slot #0 means leader, which isn't someone who was told to form-on-wing.
	object *o;

	for ( o = GET_FIRST(&obj_used_list); o != END_OF_LIST(&obj_used_list); o = GET_NEXT(o) ) {
		if (objp == o)
			break;
		else if (o->type == OBJ_SHIP)
			if (Ai_info[Ships[o->instance].ai_index].ai_flags & AIF_FORMATION_OBJECT)
				if (Ai_info[Ships[o->instance].ai_index].goal_objnum == objnum)
					slotnum++;
	}

	Assert(o != END_OF_LIST(&obj_used_list));	//	Didn't find objp in list of used ships.  Impossible!

	return slotnum;
}

#define	BIGNUM	100000.0f

int Debug_k = 0;

//	Given an attacker's position and a target's position and velocity, compute the time of
//	intersection of a weapon fired by the attacker with speed weapon_speed.
//	Return this value.  Return value of 0.0f means no collision is possible.
float compute_collision_time(vector *targpos, vector *targvel, vector *attackpos, float weapon_speed)
{
	vector	vec_to_target;
	float		pos_dot_vel;
	float		vel_sqr;
	float		discrim;

	vm_vec_sub(&vec_to_target, targpos, attackpos);
	pos_dot_vel = vm_vec_dot(&vec_to_target, targvel);
	vel_sqr = vm_vec_dot(targvel, targvel) - weapon_speed*weapon_speed;
	discrim = pos_dot_vel*pos_dot_vel - vel_sqr*vm_vec_dot(&vec_to_target, &vec_to_target);

	if (discrim > 0.0f) {
		float	t1, t2, t_solve;

		t1 = (-pos_dot_vel + fl_sqrt(discrim)) / vel_sqr;
		t2 = (-pos_dot_vel - fl_sqrt(discrim)) / vel_sqr;

		t_solve = BIGNUM;

		if (t1 > 0.0f)
			t_solve = t1;
		if ((t2 > 0.0f) && (t2 < t_solve))
			t_solve = t2;

		if (t_solve < BIGNUM-1.0f) {
			return t_solve + Debug_k * flFrametime;
		}
	}

	return 0.0f;
}


//	--------------------------------------------------------------------------
//	If far away, use player's speed.
//	If in between, lerp between player and laser speed
//	If close, use laser speed.
// Want to know how much time it will take to get to the enemy.
// This function doesn't account for the fact that by the time the player
// (or his laser) gets to the current enemy position, the enemy will have moved.
// This is dealt with in polish_predicted_enemy_pos.
float compute_time_to_enemy(float dist_to_enemy, object *pobjp, object *eobjp)
{
	float	time_to_enemy;
	float	pl_speed = pobjp->phys_info.speed;
	float	max_laser_distance, max_laser_speed;
	int	bank_num, weapon_num;
	ship	*shipp = &Ships[pobjp->instance];

	bank_num = shipp->weapons.current_primary_bank;
	weapon_num = shipp->weapons.primary_bank_weapons[bank_num];
	max_laser_speed = Weapon_info[weapon_num].max_speed;
	max_laser_distance = max_laser_speed * Weapon_info[weapon_num].lifetime;

	//	If pretty far away, use player's speed to predict position, else
	//	use laser's speed because when close, we care more about hitting
	//	with a laser than about causing ship:ship rendezvous.
	if (dist_to_enemy > 1.5 * max_laser_distance) {
		if (pl_speed > 0.0f)
			time_to_enemy = dist_to_enemy/pl_speed;
		else
			time_to_enemy = 1.0f;
	} else if (dist_to_enemy > 1.1*max_laser_distance) {
		if (pl_speed > 0.1f) {
			float	scale;

			scale = (float) ((dist_to_enemy - max_laser_distance) / max_laser_distance);
		
			time_to_enemy = (float) (dist_to_enemy/(pl_speed * scale + max_laser_speed * (1.0f - scale)));
		} else
			time_to_enemy = 2.0f;
	} else
		time_to_enemy = (float) (dist_to_enemy/max_laser_speed);

	// return time_to_enemy * (1.0f + Ai_info[Ships[pobjp->instance].ai_index].lead_scale);
	return time_to_enemy + flFrametime;
}

//	Stuff *dot and *tts.
//	*dot is always computed.  If dot is less than zero, the magnitude is
//	incorrect, not having been divided by distance.
//	If *dot is > 0.0f, then tts is computed.  This is the time it will take object
//	*objp to get to *pos, assuming it moves right at it.
void fds_aux(float *dot, float *tts, vector *pos, float dtime, object *objp)
{
	vector	v2s;

	vm_vec_sub(&v2s, pos, &objp->pos);
	*dot = vm_vec_dot(&v2s, &objp->orient.v.fvec);

	if (*dot > 0.0f) {
		float	dist;

		dist = vm_vec_dist(&objp->pos, pos);

		if (dist > 0.1f)
			*dot /= dist;
		else
			*dot = 1.0f;

		if (objp->phys_info.speed > 0.1f)
			*tts = dist / objp->phys_info.speed;
		else
			*tts = dist * 100.0f;
	}
}

/*
//	Return index of weapon that could hit object *sobjp within dtime seconds.
//	Actual time until impact returned in *atime.
int find_danger_weapon(object *sobjp, float dtime, float *atime, float dot_threshhold)
{
	object	*objp, *best_objp = NULL;
	float		best_tts = 1000.0f;

	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if ((objp->type == OBJ_WEAPON) && (sobjp-Objects != objp->parent)) {
			float		dot, tts;
			// vector	psp;		//	Predicted ship position.

			//	Get dot and time to current ship position.
			fds_aux(&dot, &tts, &sobjp->pos, dtime, objp);

			//	If dot and tts are in plausible range, do more expensive stuff.
			if (dot > 0.98f) {
//				float	dot_from_sobjp;
				vector	v2e;

				vm_vec_normalized_dir(&v2e, &objp->pos, &sobjp->pos);
//				dot_from_sobjp = vm_vec_dot(&sobjp->orient.v.fvec, &v2e);
//				if (dot_from_sobjp >= dot_threshhold)
					if (tts < dtime) {
						if (tts < best_tts) {
							best_tts = tts;
							best_objp = objp;
						}
					}
			}
		}
	}

	*atime = best_tts;

	if (best_objp != NULL)
		return best_objp-Objects;
	else
		return -1;
}
*/

//	--------------------------------------------------------------------------
void ai_set_positions(object *pl_objp, object *en_objp, ai_info *aip, vector *player_pos, vector *enemy_pos)
{
	*player_pos = pl_objp->pos;

	if (aip->next_predict_pos_time > Missiontime) {
		*enemy_pos = aip->last_predicted_enemy_pos;
	} else {
		*enemy_pos = en_objp->pos;

		aip->next_predict_pos_time = Missiontime + Skill_level_delay[Game_skill_level];
		aip->last_predicted_enemy_pos = *enemy_pos;
	}


}

//	--------------------------------------------------------------------------
int find_nearest_waypoint(object *objp)
{
	int	i;
	float	dist, min_dist, dot;
	int	min_ind;
	ship	*shipp;
	int	wp_listnum;
	waypoint_list	*wpl;

	shipp = &Ships[objp->instance];
	wp_listnum = Ai_info[Ships[objp->instance].ai_index].wp_list;
	Assert(wp_listnum > 0);
	wpl = &Waypoint_lists[wp_listnum];

	min_dist = 999999.0f;
	min_ind = -1;

	for (i=0; i<wpl->count; i++) {
		dist = vm_vec_dist_quick(&objp->pos, &wpl->waypoints[i]);
		dot = vm_vec_dot_to_point(&objp->orient.v.fvec, &objp->pos, &wpl->waypoints[i]);
		dist = (float) (dist * (1.25 - dot));
		if (dist < min_dist) {
			min_dist = dist;
			min_ind = i;
		}
	}

	Assert(min_ind != -1);

	return min_ind;
}

//	Given an ai_info struct, by reading current goal and path information,
//	extract base path information and return in pmp and pmpv.
//	Return true if found, else return false.
//	false means the current point is not on the original path.
int get_base_path_info(int path_cur, int goal_objnum, model_path **pmp, mp_vert **pmpv)
{
	pnode			*pn = &Path_points[path_cur];
	ship_info	*sip = &Ship_info[Ships[Objects[goal_objnum].instance].ship_info_index];
	polymodel	*pm = model_get(sip->modelnum);
	//static	int	debug_last_index = -1;	// no longer used
	*pmpv = NULL;
	*pmp = NULL;

	if (pn->path_num != -1) {
		*pmp = &pm->paths[pn->path_num];
		if (pn->path_index != -1)
			*pmpv = &(*pmp)->verts[pn->path_index];
		else
			return 0;
	} else
		return 0;

/*	if (debug_last_index != *pmpv-(*pmp)->verts) {
		debug_last_index = *pmpv-(*pmp)->verts;
		nprintf(("AI", "Point %i has %i turrets: ", *pmpv-(*pmp)->verts, (*pmpv)->nturrets));
		for (int i=0; i<(*pmpv)->nturrets; i++) {
			nprintf(("AI", "%i ", (*pmpv)->turret_ids[i]));
		}
		nprintf(("AI", "\n"));
	}
*/
	return 1;
}

//	Modify, in place, the points in a global model path.
//	Only modify those points that are defined in the model path.  Don't modify the
//	leadin points, such as those that are necessary to get the model on the path.
void modify_model_path_points(object *objp)
{	
	ai_info		*aip = &Ai_info[Ships[objp->instance].ai_index];
	object		*mobjp = &Objects[aip->path_objnum];
	ship_info	*osip = &Ship_info[Ships[mobjp->instance].ship_info_index];
	polymodel	*pm = model_get(osip->modelnum);
	pnode			*pnp;
	int			path_num, dir;

	Assert((aip->path_start >= 0) && (aip->path_start < MAX_PATH_POINTS));

	pnp = &Path_points[aip->path_start];
	while ((pnp->path_index == -1) && (pnp-Path_points - aip->path_start < aip->path_length))
		pnp++;

	path_num = pnp->path_num;
	Assert((path_num >= 0) && (path_num < pm->n_paths));
	
	Assert(pnp->path_index != -1);	//	If this is -1, that means we never found the model path points

	dir = 1;
	if ( aip->ai_flags & AIF_USE_EXIT_PATH ) {
		dir = -1;
	}

	copy_xlate_model_path_points(mobjp, &pm->paths[path_num], dir, pm->paths[path_num].nverts, path_num, pnp);
}

//	Return an indication of the distance between two matrices.
//	This is the sum of the distances of their dot products from 1.0f.
float ai_matrix_dist(matrix *mat1, matrix *mat2)
{
	float	t;

	t =  1.0f - vm_vec_dot(&mat1->v.fvec, &mat2->v.fvec);
	t += 1.0f - vm_vec_dot(&mat1->v.uvec, &mat2->v.uvec);
	t += 1.0f - vm_vec_dot(&mat1->v.rvec, &mat2->v.rvec);

	return t;
}


//	Paths are created in absolute space, so a moving object needs to have model paths within it recreated.
//	This uses the hash functions which means the slightest movement will cause a recreate, though the timestamp
//	prevents this from happening too often.
//	force_recreate_flag TRUE means to recreate regardless of timestamp.
//	Returns TRUE if path recreated.
float maybe_recreate_path(object *objp, ai_info *aip, int force_recreate_flag)
{
	int	hashval;

	Assert(&Ai_info[Ships[objp->instance].ai_index] == aip);

	if ((aip->mode == AIM_BAY_EMERGE) || (aip->mode == AIM_BAY_DEPART))
		if ((OBJ_INDEX(objp) % 4) == (Framecount % 4))
			force_recreate_flag = 1;

	//	If no path, that means we don't need one.
	if (aip->path_start == -1)
		return 0.0f;

	// AL 11-12-97: If AIF_USE_STATIC_PATH is set, don't try to recreate.  This is needed when ships
	//				    emerge from fighter bays.  We don't need to recreate the path.. and in case the 
	//              parent ship dies, we still want to be able to continue on the path
	if ( aip->ai_flags & AIF_USE_STATIC_PATH ) 
		return 0.0f;

	if (force_recreate_flag || timestamp_elapsed(aip->path_next_create_time)) {
		object	*path_objp;

		path_objp = &Objects[aip->path_objnum];

		if ((hashval = create_object_hash(path_objp)) != aip->path_goal_obj_hash) {
			float dist;
			
			dist = vm_vec_dist_quick(&path_objp->pos, &aip->path_create_pos);
			dist += ai_matrix_dist(&path_objp->orient, &aip->path_create_orient) * 25.0f;

			if (force_recreate_flag || (dist > 2.0f)) {
				aip->path_next_create_time = timestamp(1000);	//	Update again in as little as 1000 milliseconds, ie 1 second.
				aip->path_goal_obj_hash = hashval;
				modify_model_path_points(objp);

				aip->path_create_pos = path_objp->pos;
				aip->path_create_orient = path_objp->orient;
				
				return dist;
			}
		}
	}

	return 0.0f;
}

//	Set acceleration for ai_dock().
void set_accel_for_docking(object *objp, ai_info *aip, float dot, float dot_to_next, float dist_to_next, float dist_to_goal, ship_info *sip)
{
	float prev_dot_to_goal = aip->prev_dot_to_goal;
	
	aip->prev_dot_to_goal = dot;

	if (objp->phys_info.speed < 0.0f) {
		accelerate_ship(aip, 1.0f/32.0f);
	} else if ((prev_dot_to_goal-dot) > 0.01) {
		if (prev_dot_to_goal > dot + 0.05f) {
			accelerate_ship(aip, 0.0f);
		} else {
			change_acceleration(aip, -1.0f);	//	-1.0f means subtract off flFrametime from acceleration value in 0.0..1.0
		}
	} else {
		if ((aip->mode == AIM_DOCK) && (dist_to_next < 150.0f) && (aip->path_start + aip->path_length - 2 == aip->path_cur)) {
			set_accel_for_target_speed(objp, sip->max_speed * max(dist_to_next/500.0f, 1.0f));
			//mprintf(("dist = %7.3f, speed = %7.3f\n", dist_to_next, objp->phys_info.speed));
		} else if ((dot_to_next >= dot * .9) || (dist_to_next > 100.0f)) {
			if (dist_to_goal > 200.0f)
				set_accel_for_target_speed(objp, sip->max_speed * (dot + 1.0f) / 2.0f);
			else {
				float	xdot;

				xdot = (dot_to_next + dot)/2.0f;
				if (xdot < 0.0f)
					xdot = 0.0f;

				// AL: if following a path not in dock mode, move full speed
				if (( aip->mode != AIM_DOCK ) && (dot > 0.9f)) {
					set_accel_for_target_speed(objp, sip->max_speed*dot*dot*dot);
				} else {
					if ((aip->path_cur - aip->path_start < aip->path_length-2) && (dist_to_goal < 2*objp->radius)) {
						//nprintf(("AI", "Target speed = %7.3f\n", dist_to_goal/8.0f));
						set_accel_for_target_speed(objp, dist_to_goal/8.0f + 2.0f);
					} else {
						set_accel_for_target_speed(objp, sip->max_speed * (2*xdot + 0.25f)/4.0f);
					}
				}
			}
		} else {
			float	xdot;

			xdot = max(dot_to_next, 0.1f);
			if ( aip->mode != AIM_DOCK ) {
				set_accel_for_target_speed(objp, sip->max_speed);
			} else {
				float	speed;
				if ((aip->path_cur - aip->path_start < aip->path_length-2) && (dist_to_goal < 2*objp->radius)) {
					speed = dist_to_goal/8.0f + 2.0f;
				} else if (dist_to_goal < 4*objp->radius + 50.0f) {
					speed = dist_to_goal/4.0f + 4.0f;
				} else {
					speed = sip->max_speed * (3*xdot + 1.0f)/4.0f;
				}
				if (aip->mode == AIM_DOCK) {
					speed = speed * 2.0f + 1.0f;
					if (aip->goal_objnum != -1) {
						speed += Objects[aip->goal_objnum].phys_info.speed;
					}
				}

				set_accel_for_target_speed(objp, speed);
			}
		}
	}
}

//	--------------------------------------------------------------------------
//	Follow a path associated with a large object, such as a capital ship.
//	The points defined on the path are in the object's reference frame.
//	The object of interest is goal_objnum.
//	The paths are defined in the model.  The path of interest is wp_list.
//	The next goal point in the path is wp_index.
//	wp_flags contain special information specific to the path.

// The path vertices are defined by model_path structs:
//		typedef struct model_path {
//			char		name[MAX_NAME_LEN];					// name of the subsystem.  Probably displayed on HUD
//			int		nverts;
//			vector	*verts;
//		} model_path;

//	The polymodel struct for the object contains the following:
//		int			n_paths;
//		model_path	*paths;

//	Returns distance to goal point.
float ai_path()
{
	polymodel	*pm;
	int		num_paths, num_points;
	float		dot, dist_to_goal, dist_to_next, speed, dot_to_next;
	ship		*shipp = &Ships[Pl_objp->instance];
	ship_info	*sip = &Ship_info[shipp->ship_info_index];
	ai_info	*aip;
	vector	nvel_vec;
	float		mag, prev_dot_to_goal;
	vector	temp_vec, *slop_vec;
	object	*gobjp;
	ship		*gshipp;
	vector	*cvp, *nvp, next_vec, gcvp, gnvp;		//	current and next vertices in global coordinates.

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	Assert(aip->goal_objnum != -1);
	Assert(Objects[aip->goal_objnum].type == OBJ_SHIP);

	gobjp = &Objects[aip->goal_objnum];
	gshipp = &Ships[gobjp->instance];

	pm = model_get( gshipp->modelnum );
	num_paths = pm->n_paths;
	Assert(num_paths > 0);

	if (aip->path_start == -1) {
		int path_num;
		path_num = ai_return_path_num_from_dockbay(&Objects[aip->goal_objnum], aip->dockee_index);
		Assert(aip->goal_objnum >= 0 && aip->goal_objnum < MAX_OBJECTS);
		ai_find_path(Pl_objp, aip->goal_objnum, path_num, 0);
	}

	// nprintf(("AI", "Frame: %i, Path index = %i/%i\n", AI_FrameCount, aip->path_cur-aip->path_start, aip->path_length));

	maybe_recreate_path(Pl_objp, aip, 0);

	num_points = aip->path_length;

	//	Set cvp and nvp as pointers to current and next vertices of interest on path.
	cvp = &Path_points[aip->path_cur].pos;
	if ((aip->path_cur + aip->path_dir - aip->path_start < num_points) || (aip->path_cur + aip->path_dir < aip->path_start))
		nvp = &Path_points[aip->path_cur + aip->path_dir].pos;
	else {
		//	If this is 0, then path length must be 1 which means we have no direction!
		Assert((aip->path_cur - aip->path_dir >= aip->path_start) && (aip->path_cur - aip->path_dir - aip->path_start < num_points));
		//	Cleanup for above Assert() which we hit too near release. -- MK, 5/24/98.
		if (aip->path_cur - aip->path_dir - aip->path_start >= num_points) {
			if (aip->path_dir == 1)
				aip->path_cur = aip->path_start;
			else
				aip->path_cur = aip->path_start + num_points - 1;
		}

		vector	delvec;
		vm_vec_sub(&delvec, cvp, &Path_points[aip->path_cur - aip->path_dir].pos);
		vm_vec_normalize(&delvec);
		vm_vec_scale_add(&next_vec, cvp, &delvec, 10.0f);
		nvp = &next_vec;
	}

	//	Interrupt if can't get to current goal point.  Debug only.
/*	if (pp_collide(&Pl_objp->pos, cvp, gobjp, Pl_objp->radius)) {
		Int3();
	}
*/
	//	See if can reach next point (as opposed to current point)
	//	However, don't do this if docking and next point is last point.
	//	That is, we don't want to pursue the last point under control of the
	//	path code.  In docking, this is a special hack.
	if ((aip->mode != AIM_DOCK) || ((aip->path_cur-aip->path_start) < num_points - 2)) {
		if ((aip->path_cur + aip->path_dir > aip->path_start) && (aip->path_cur + aip->path_dir < aip->path_start + num_points-2)) {
			if ( timestamp_elapsed(aip->path_next_check_time)) {
				aip->path_next_check_time = timestamp( 3000 );
				if (!pp_collide(&Pl_objp->pos, nvp, gobjp, 1.1f * Pl_objp->radius)) {
					cvp = nvp;
					aip->path_cur += aip->path_dir;
					nvp = &Path_points[aip->path_cur].pos;
					//nprintf(("AI", "Reach: Advancing from point %i to %i of %i points.\n", aip->path_cur-aip->path_dir, aip->path_cur, num_points));
				}
			}
		}
	}

	gcvp = *cvp;
	gnvp = *nvp;

	speed = Pl_objp->phys_info.speed;

	dist_to_goal = vm_vec_dist_quick(&Pl_objp->pos, &gcvp);
	dist_to_next = vm_vec_dist_quick(&Pl_objp->pos, &gnvp);
	//	Can't use fvec, need to use velocity vector because we aren't necessarily
	//	moving in the direction we're facing.

//	if (IS_VEC_NULL(&Pl_objp->phys_info.vel)) {
	if ( vm_vec_mag_quick(&Pl_objp->phys_info.vel) < AICODE_SMALL_MAGNITUDE ) {
		mag = 0.0f;
		vm_vec_zero(&nvel_vec);
	} else
		mag = vm_vec_copy_normalize(&nvel_vec, &Pl_objp->phys_info.vel);

	//	If moving not-very-slowly and sliding, then try to slide at goal, rather than
	//	point at goal.
	slop_vec = NULL;
	if (mag < 1.0f)
		nvel_vec = Pl_objp->orient.v.fvec;
	else if (mag > 5.0f) {
		float	nv_dot;
		nv_dot = vm_vec_dot(&Pl_objp->orient.v.fvec, &nvel_vec);
		if ((nv_dot > 0.5f) && (nv_dot < 0.97f)) {
			slop_vec = &temp_vec;
			vm_vec_sub(slop_vec, &nvel_vec, &Pl_objp->orient.v.fvec);
		}
	}

	if (dist_to_goal > 0.1f)
		ai_turn_towards_vector(&gcvp, Pl_objp, flFrametime, sip->srotation_time, slop_vec, NULL, 0.0f, 0);

	//	Code to control speed is MUCH less forgiving in path following than in waypoint
	//	following.  Must be very close to path or might hit objects.
	prev_dot_to_goal = aip->prev_dot_to_goal;
	dot = vm_vec_dot_to_point(&nvel_vec, &Pl_objp->pos, &gcvp);
	dot_to_next = vm_vec_dot_to_point(&nvel_vec, &Pl_objp->pos, &gnvp);

	set_accel_for_docking(Pl_objp, aip, dot, dot_to_next, dist_to_next, dist_to_goal, sip);
	aip->prev_dot_to_goal = dot;

//mprintf(("Goal index = %i, dist = %7.3f, dot = %7.3f\n", wp_index, dist_to_goal, dot));

	//	If moving at a non-tiny velocity, detect attaining path point by its being close to
	//	line between previous and current object location.
	if ((dist_to_goal < MIN_DIST_TO_WAYPOINT_GOAL) || (vm_vec_dist_quick(&Pl_objp->last_pos, &Pl_objp->pos) > 0.1f)) {
		vector	nearest_point;
		float		r, min_dist_to_goal;

		r = find_nearest_point_on_line(&nearest_point, &Pl_objp->last_pos, &Pl_objp->pos, &gcvp);

		//	Set min_dist_to_goal = how close must be to waypoint to pick next one.
		//	If docking and this is the second last waypoint, must be very close.
		if ((aip->mode == AIM_DOCK) && (aip->path_cur >= aip->path_length-2))
			min_dist_to_goal = MIN_DIST_TO_WAYPOINT_GOAL;
		else
			min_dist_to_goal = MIN_DIST_TO_WAYPOINT_GOAL + Pl_objp->radius;

		if ( (vm_vec_dist_quick(&Pl_objp->pos, &gcvp) < min_dist_to_goal) ||
			(((r >= 0.0f) && (r <= 1.0f)) && (vm_vec_dist_quick(&nearest_point, &gcvp) < (MIN_DIST_TO_WAYPOINT_GOAL + Pl_objp->radius)))) {
			aip->path_cur += aip->path_dir;
			//nprintf(("AI", " Near: Advancing from point %i to %i of %i points.\n", aip->path_cur-aip->path_dir, aip->path_cur, num_points));
			if (((aip->path_cur - aip->path_start) > (num_points+1)) || (aip->path_cur < aip->path_start)) {
				Assert(aip->mode != AIM_DOCK);		//	If docking, should never get this far, getting to last point handled outside ai_path()
				aip->path_dir = -aip->path_dir;
//				aip->path_cur += aip->path_dir;
			}
		}
	}

	return dist_to_goal;
}

void update_min_max(float val, float *min, float *max)
{
	if (val < *min)
		*min = val;
	else if (val > *max)
		*max = val;
}

//	Stuff bounding box of all enemy objects within "range" units of object *my_objp.
//	Stuff ni min_vec and max_vec.
//	Return value: Number of enemy objects in bounding box.
int get_enemy_team_range(object *my_objp, float range, int enemy_team_mask, vector *min_vec, vector *max_vec)
{
	object	*objp;
	ship_obj	*so;
	int		count = 0;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		if (Ships[objp->instance].team & enemy_team_mask) {
			if (Ship_info[Ships[objp->instance].ship_info_index].flags & (SIF_FIGHTER | SIF_BOMBER | SIF_CRUISER | SIF_CAPITAL | SIF_SUPERCAP | SIF_DRYDOCK | SIF_CORVETTE | SIF_AWACS | SIF_GAS_MINER))
				if (vm_vec_dist_quick(&my_objp->pos, &objp->pos) < range) {
					if (count == 0) {
						*min_vec = objp->pos;
						*max_vec = objp->pos;
						count++;
					} else {
						update_min_max(objp->pos.xyz.x, &min_vec->xyz.x, &max_vec->xyz.x);
						update_min_max(objp->pos.xyz.y, &min_vec->xyz.y, &max_vec->xyz.y);
						update_min_max(objp->pos.xyz.z, &min_vec->xyz.z, &max_vec->xyz.z);
					}
				}

		}
	}

	return count;
}

//	Pick a relatively safe spot for objp to fly to.
//	Problem:
//		Finds a spot away from any enemy within a bounding box.
//		Doesn't verify that "safe spot" is not near some other enemy.
void ai_safety_pick_spot(object *objp)
{
	int		objnum;
	int		enemy_team_mask;
	vector	min_vec, max_vec;
	vector	vec_to_center, center;
	vector	goal_pos;

	objnum = OBJ_INDEX(objp);

	enemy_team_mask = get_enemy_team_mask(objnum);

	if (get_enemy_team_range(objp, 1000.0f, enemy_team_mask, &min_vec, &max_vec)) {
		vm_vec_avg(&center, &min_vec, &max_vec);
		vm_vec_normalized_dir(&vec_to_center, &center, &objp->pos);

		vm_vec_scale_add(&goal_pos, &center, &vec_to_center, 2000.0f);
	} else
		vm_vec_scale_add(&goal_pos, &objp->pos, &objp->orient.v.fvec, 100.0f);

	Ai_info[Ships[objp->instance].ai_index].goal_point = goal_pos;
}

//	Fly to desired safe point.
// Returns distance to that point.
float ai_safety_goto_spot(object *objp)
{
	float	dot, dist;
	ai_info	*aip;
	vector	vec_to_goal;
	ship_info	*sip;
	float	dot_val;

	sip = &Ship_info[Ships[objp->instance].ship_info_index];

	aip = &Ai_info[Ships[objp->instance].ai_index];
	dist = vm_vec_normalized_dir(&vec_to_goal, &aip->goal_point, &objp->pos);
	dot = vm_vec_dot(&vec_to_goal, &objp->orient.v.fvec);

	dot_val = (1.1f + dot) / 2.0f;
	if (dist > 200.0f) {
		set_accel_for_target_speed(objp, sip->max_speed * dot_val);
	} else
		set_accel_for_target_speed(objp, sip->max_speed * dot_val * (dist/200.0f + 0.2f));

	return dist;
}

void ai_safety_circle_spot(object *objp)
{
	vector	goal_point;
	ship_info	*sip;
	float		dot;

	sip = &Ship_info[Ships[objp->instance].ship_info_index];

	goal_point = Ai_info[Ships[objp->instance].ai_index].goal_point;
	dot = turn_towards_tangent(objp, &goal_point, 250.0f);	//	Increased from 50 to 250 to make circling not look so wacky.

	set_accel_for_target_speed(objp, 0.5f * (1.0f + dot) * sip->max_speed/4.0f);

//	float dist = vm_vec_dist_quick(&goal_point, &Pl_objp->pos);
//	nprintf(("AI", "Ship %s circling %7.3f %7.3f %7.3f.  Distance = %7.3f\n", Ships[Pl_objp->instance].ship_name, goal_point.xyz.x, goal_point.xyz.y, goal_point.xyz.z, dist));

}

//	--------------------------------------------------------------------------
void ai_safety()
{
	ai_info	*aip;

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	switch (aip->submode) {
	case AISS_1:
		ai_safety_pick_spot(Pl_objp);
		aip->submode = AISS_2;
		aip->submode_start_time = Missiontime;
		break;
	case AISS_1a:	//	Pick a safe point because we just got whacked!
		Int3();
		break;
	case AISS_2:
		if (ai_safety_goto_spot(Pl_objp) < 25.0f) {
			aip->submode = AISS_3;
			aip->submode_start_time = Missiontime;
		}
		break;
	case AISS_3:
		ai_safety_circle_spot(Pl_objp);
		break;
	default:
		Int3();		//	Illegal submode for ai_safety();
		break;
	}
}

//	--------------------------------------------------------------------------
//	make Pl_objp fly waypoints.
void ai_waypoints()
{
	int		wp_index;
	vector	*wp_cur, *wp_next;
	float		dot, dist_to_goal, dist_to_next, speed, dot_to_next;
	ship		*shipp = &Ships[Pl_objp->instance];
	ship_info	*sip = &Ship_info[shipp->ship_info_index];
	waypoint_list	*wpl;
	ai_info	*aip;
	vector	nvel_vec;
	float		mag;
	float		prev_dot_to_goal;
	vector	temp_vec;
	vector	*slop_vec;

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	wp_index = aip->wp_index;

	if (wp_index == -1) {
		ai_start_waypoints(Pl_objp, 0, WPF_REPEAT);
		wp_index = aip->wp_index;
		aip->wp_dir = 1;
	}

	wpl = &Waypoint_lists[Ai_info[Ships[Pl_objp->instance].ai_index].wp_list];

	Assert(wpl->count);	// What? Is this zero? Probably wp_index never got initialized!

	wp_cur = &wpl->waypoints[wp_index];
	wp_next = &wpl->waypoints[(wp_index+1) % wpl->count];
	speed = Pl_objp->phys_info.speed;

	dist_to_goal = vm_vec_dist_quick(&Pl_objp->pos, wp_cur);
	dist_to_next = vm_vec_dist_quick(&Pl_objp->pos, wp_next);

	//	Can't use fvec, need to use velocity vector because we aren't necessarily
	//	moving in the direction we're facing.
	// AL 23-3-98: Account for very small velocities by checking result of vm_vec_mag().
	//					If we don't vm_vec_copy_normalize() will think it is normalizing a null vector.
//	if (IS_VEC_NULL(&Pl_objp->phys_info.vel)) {
	if ( vm_vec_mag_quick(&Pl_objp->phys_info.vel) < AICODE_SMALL_MAGNITUDE ) {
		mag = 0.0f;
		vm_vec_zero(&nvel_vec);
	} else {
		mag = vm_vec_copy_normalize(&nvel_vec, &Pl_objp->phys_info.vel);
	}

	//	If moving not-very-slowly and sliding, then try to slide at goal, rather than
	//	point at goal.
	slop_vec = NULL;
	if (mag < 1.0f) {
		nvel_vec = Pl_objp->orient.v.fvec;
	} else if (mag > 5.0f) {
		float	nv_dot;
		nv_dot = vm_vec_dot(&Pl_objp->orient.v.fvec, &nvel_vec);
		if ((nv_dot > 0.5f) && (nv_dot < 0.97f)) {
			slop_vec = &temp_vec;
			vm_vec_sub(slop_vec, &nvel_vec, &Pl_objp->orient.v.fvec);
		}
	}

	//	If a wing leader, take turns more slowly, based on size of wing.
	int	scale;

	if (Ai_info[Ships[Pl_objp->instance].ai_index].wing >= 0) {
		scale = Wings[Ai_info[Ships[Pl_objp->instance].ai_index].wing].current_count;
		scale = (int) ((scale+1)/2);
	} else {
		scale = 1;
	}

	if (dist_to_goal > 0.1f) {
		ai_turn_towards_vector(wp_cur, Pl_objp, flFrametime, sip->srotation_time*3.0f*scale, slop_vec, NULL, 0.0f, 0);
	}

	prev_dot_to_goal = aip->prev_dot_to_goal;
	dot = vm_vec_dot_to_point(&nvel_vec, &Pl_objp->pos, wp_cur);
	dot_to_next = vm_vec_dot_to_point(&nvel_vec, &Pl_objp->pos, wp_next);
	aip->prev_dot_to_goal = dot;

	//	If there is no next point on the path, don't care about dot to next.
	if (wp_index + 1 >= wpl->count) {
		dot_to_next = dot;
	}

	// nprintf(("AI", "Wp #%i, dot = %6.3f, next dot = %6.3f, dist = %7.2f\n", wp_index, dot, dot_to_next, dist_to_goal));

	if (Pl_objp->phys_info.speed < 0.0f) {
		accelerate_ship(aip, 1.0f/32);
	} else if (prev_dot_to_goal > dot+0.01f) {
		//	We are further from pointing at our goal this frame than last frame, so slow down.
		set_accel_for_target_speed(Pl_objp, Pl_objp->phys_info.speed * 0.95f);
	} else if (dist_to_goal < 100.0f) {
		float slew_dot = vm_vec_dot(&Pl_objp->orient.v.fvec, &nvel_vec);
		if (fl_abs(slew_dot) < 0.9f) {
			accelerate_ship(aip, 0.0f);
		} else if (dot < 0.88f + 0.1f*(100.0f - dist_to_goal)/100.0f) {
			accelerate_ship(aip, 0.0f);
		} else {
			accelerate_ship(aip, 0.5f * dot * dot);
		}
	} else {
		float	dot1;
		if (dist_to_goal < 250.0f) {
			dot1 = dot*dot*dot;				//	Very important to be pointing towards goal when nearby.  Note, cubing preserves sign.
		} else {
			if (dot > 0.0f) {
				dot1 = dot*dot;
			} else {
				dot1 = dot;
			}
		}

		if (dist_to_goal > 100.0f + Pl_objp->radius * 2) {
			if (dot < 0.2f) {
				dot1 = 0.2f;
			}
		}

		if (sip->flags & SIF_SMALL_SHIP) {
			set_accel_for_target_speed(Pl_objp, dot1 * dist_to_goal/5.0f);
		} else {
			set_accel_for_target_speed(Pl_objp, dot1 * dist_to_goal/10.0f);
		}
	}

	//	Make sure not travelling too fast for someone to keep up.
	float	max_allowed_speed = 9999.9f;

	if (shipp->wingnum != -1) {
		max_allowed_speed = 0.9f * get_wing_lowest_max_speed(Pl_objp);
	}

	// check if waypoint speed cap is set and adjust max speed
	if (aip->waypoint_speed_cap > 0) {
		max_allowed_speed = (float) aip->waypoint_speed_cap;
	}

	if (aip->prev_accel * shipp->current_max_speed > max_allowed_speed) {
		accelerate_ship(aip, max_allowed_speed / shipp->current_max_speed);
	}

	if (vm_vec_dist_quick(&Pl_objp->last_pos, &Pl_objp->pos) > 0.1f) {
		vector	nearest_point;
		float		r;

		r = find_nearest_point_on_line(&nearest_point, &Pl_objp->last_pos, &Pl_objp->pos, wp_cur);

		if ( (vm_vec_dist_quick(&Pl_objp->pos, wp_cur) < (MIN_DIST_TO_WAYPOINT_GOAL + fl_sqrt(Pl_objp->radius) + vm_vec_dist_quick(&Pl_objp->pos, &Pl_objp->last_pos))) ||
			(((r >= 0.0f) && (r <= 1.0f)) && (vm_vec_dist_quick(&nearest_point, wp_cur) < (MIN_DIST_TO_WAYPOINT_GOAL + fl_sqrt(Pl_objp->radius))))) {
			wp_index++;
			if (wp_index >= wpl->count) {
				if (aip->wp_flags & WPF_REPEAT) {
					wp_index = 0;
				} else {
					int treat_as_ship;

					// when not repeating waypoints -- mark the goal as done and put and entry into the mission log
					// we must be careful when dealing with wings.  A ship in a wing might be completing
					// a waypoint for for the entire wing, or it might be completing a goal for itself.  If
					// for itself and in a wing, treat the completion as we would a ship
					treat_as_ship = 1;
					if ( Ships[Pl_objp->instance].wingnum != -1 ) {
						int type;

						// I don't think that you can fly waypoints as dynamic goals!!!
						// -- This is legal, just stupid. -- Assert( (aip->active_goal != AI_GOAL_NONE) && (aip->active_goal != AI_ACTIVE_GOAL_DYNAMIC) );
						
						//	Clean up from above Assert, just in case we ship without fixing it.  (Encountered by JimB on 2/9/98)
						if ( (aip->active_goal == AI_GOAL_NONE) || (aip->active_goal == AI_ACTIVE_GOAL_DYNAMIC) ) {
							aip->mode = AIM_NONE;
							Int3();	//	Look at the ship, find out of it's supposed to be flying waypoints. -- MK.
						}

						type = aip->goals[aip->active_goal].type;
						if ( (type == AIG_TYPE_EVENT_WING) || (type == AIG_TYPE_PLAYER_WING) ) {
							treat_as_ship = 0;
						} else {
							treat_as_ship = 1;
						}
					}

					// if the ship is not in a wing, remove the goal and continue on
					if ( treat_as_ship ) {
						ai_mission_goal_complete( aip );					// this call should reset the AI mode
						mission_log_add_entry(LOG_WAYPOINTS_DONE, Ships[Pl_objp->instance].ship_name, wpl->name, -1 );
					} else {
						// this ship is in a wing.  We must mark the goal as being completed for all ships
						// in the wing.  We will also mark an entry in the log that the wing completed the goal
						// not the individual ship.
						ai_mission_wing_goal_complete( Ships[Pl_objp->instance].wingnum, &(aip->goals[aip->active_goal]) );
						mission_log_add_entry( LOG_WAYPOINTS_DONE, Wings[Ships[Pl_objp->instance].wingnum].name, wpl->name, -1 );
					}
					//wp_index = wpl->count-1;
				}
			}

			aip->wp_index = wp_index;
		}
	}
}

//	Make Pl_objp avoid En_objp
//	Not like evading.  This is for avoiding a collision!
//	Note, use sliding if available.
void avoid_ship()
{
	//	To avoid an object, turn towards right or left vector until facing away from object.
	//	To choose right vs. left, pick one that is further from center of avoid object.
	//	Keep turning away from until pointing away from ship.
	//	Stay in avoid mode until at least 3 enemy ship radii away.

	//	Speed setting:
	//	If inside sphere, zero speed and turn towards outside.
	//	If outside sphere, inside 2x sphere, set speed percent of max to:
	//		max(away_dot, (dist-rad)/rad)
	//	where away_dot is dot(Pl_objp->v.fvec, vec_En_objp_to_Pl_objp)

	vector	vec_to_enemy;
	float		away_dot;
	float		dist;
	ship		*shipp = &Ships[Pl_objp->instance];
	ship_info	*sip = &Ship_info[shipp->ship_info_index];
	ai_info	*aip = &Ai_info[shipp->ai_index];
	vector	player_pos, enemy_pos;

	// if we're avoiding a stealth ship, then we know where he is, update with no error
	if ( is_object_stealth_ship(En_objp) ) {
		update_ai_stealth_info_with_error(aip/*, 1*/);
	}

	ai_set_positions(Pl_objp, En_objp, aip, &player_pos, &enemy_pos);
	vm_vec_sub(&vec_to_enemy, &enemy_pos, &Pl_objp->pos);

	dist = vm_vec_normalize(&vec_to_enemy);
	away_dot = -vm_vec_dot(&Pl_objp->orient.v.fvec, &vec_to_enemy);
	
	if ((sip->max_vel.xyz.x > 0.0f) || (sip->max_vel.xyz.y > 0.0f)) {
		if (vm_vec_dot(&Pl_objp->orient.v.rvec, &vec_to_enemy) > 0.0f) {
			AI_ci.sideways = -1.0f;
		} else {
			AI_ci.sideways = 1.0f;
		}
		if (vm_vec_dot(&Pl_objp->orient.v.uvec, &vec_to_enemy) > 0.0f) {
			AI_ci.vertical = -1.0f;
		} else {
			AI_ci.vertical = 1.0f;
		}
	}		

	//nprintf(("AI", "Frame %i: Sliding: %s %s\n", Framecount, AI_ci.sideways < 0 ? "left" : "right", AI_ci.vertical < 0 ? "down" : "up" ));
	// nprintf(("AI", "away_dot = %6.3f, dist = %7.2f, dist/radsum = %6.3f\n", away_dot, dist, dist/(Pl_objp->radius + En_objp->radius)));

	//	If in front of enemy, turn away from it.
	//	If behind enemy, try to get fully behind it.
	if (away_dot < 0.0f) {
		turn_away_from_point(Pl_objp, &enemy_pos, Pl_objp->phys_info.speed);
	} else {
		vector	goal_pos;

		vm_vec_scale_add(&goal_pos, &En_objp->pos, &En_objp->orient.v.fvec, -100.0f);
		turn_towards_point(Pl_objp, &goal_pos, NULL, Pl_objp->phys_info.speed);
	}

	//	Set speed.
	float	radsum = Pl_objp->radius + En_objp->radius;

	if (dist < radsum)
		accelerate_ship(aip, max(away_dot, 0.2f));
	else if (dist < 2*radsum)
		accelerate_ship(aip, max(away_dot, (dist - radsum) / radsum)+0.2f);
	else
		accelerate_ship(aip, 1.0f);

}

//	Maybe it's time to resume the previous AI mode in aip->previous_mode.
//	Each type of previous_mode has its own criteria on when to resume.
//	Return true if previous mode was resumed.
int maybe_resume_previous_mode(object *objp, ai_info *aip)
{
	//	Only (maybe) resume previous goal if current goal is dynamic.
	if (aip->active_goal != AI_ACTIVE_GOAL_DYNAMIC)
		return 0;

	if (aip->mode == AIM_EVADE_WEAPON) {
		if (timestamp_elapsed(aip->mode_time) || (((aip->nearest_locked_object == -1) || (Objects[aip->nearest_locked_object].type != OBJ_WEAPON)) && (aip->danger_weapon_objnum == -1))) {
			Assert(aip->previous_mode != AIM_EVADE_WEAPON);
			aip->mode = aip->previous_mode;
			aip->submode = aip->previous_submode;
			aip->submode_start_time = Missiontime;
			aip->active_goal = AI_GOAL_NONE;
			aip->mode_time = -1;			//	Means do forever.
			return 1;
		}
	} else if ( aip->previous_mode == AIM_GUARD) {
		if ((aip->guard_objnum != -1) && (aip->guard_signature == Objects[aip->guard_objnum].signature)) {
			object	*guard_objp;
			float	dist;

			guard_objp = &Objects[aip->guard_objnum];
			dist = vm_vec_dist_quick(&guard_objp->pos, &objp->pos);

			//	If guarding ship is far away from guardee and enemy is far away from guardee,
			//	then stop chasing and resume guarding.
			if (dist > (MAX_GUARD_DIST + guard_objp->radius) * 6) {
				if ((En_objp != NULL) && (En_objp->type == OBJ_SHIP)) {
					if (vm_vec_dist_quick(&guard_objp->pos, &En_objp->pos) > (MAX_GUARD_DIST + guard_objp->radius) * 6) {
						Assert(aip->previous_mode == AIM_GUARD);
						aip->mode = aip->previous_mode;
						aip->submode = AIS_GUARD_PATROL;
						aip->active_goal = AI_GOAL_NONE;
						return 1;
					}
				}
			}
		}
	}

	return 0;

}

//	Call this function if you want something to happen on average every N quarters of a second.
//	The truth value returned by this function will be the same for any given quarter second interval.
//	The value "num" is only passed in to get asynchronous behavior for different objects.
//	modulus == 1 will always return true.
//	modulus == 2 will return true half the time.
//	modulus == 16 will return true for one quarter second interval every four seconds.
int static_rand_timed(int num, int modulus)
{
	if (modulus < 2)
		return 1;
	else {
		int	t;

		t = Missiontime >> 18;		//	Get time in quarters of a second
		t += num;

		return !(t % modulus);
	}
}

//	Maybe fire afterburner based on AI class
int ai_maybe_fire_afterburner(object *objp, ai_info *aip)
{
	if (aip->ai_class == 0)
		return 0;		//	Lowest level never aburners away
	else  {
		//	Maybe don't afterburner because of a potential collision with the player.
		//	If not multiplayer, near player and player in front, probably don't afterburner.
		if (!(Game_mode & GM_MULTIPLAYER)) {
			if (Ships[objp->instance].team == Player_ship->team) {
				float	dist;

				dist = vm_vec_dist_quick(&objp->pos, &Player_obj->pos) - Player_obj->radius - objp->radius;
				if (dist < 150.0f) {
					vector	v2p;
					float		dot;

					vm_vec_normalized_dir(&v2p, &Player_obj->pos, &objp->pos);
					dot = vm_vec_dot(&v2p, &objp->orient.v.fvec);

					if (dot > 0.0f) {
						if (dot * dist > 50.0f)
							return 0;
					}
				}
			}
		}

		if (aip->ai_class >= Num_ai_classes-2)
			return 1;		//	Highest two levels always aburner away.
		else {
			return static_rand_timed(objp-Objects, Num_ai_classes - aip->ai_class);
		}
	}
}

//	Maybe engage afterburner after being hit by an object.
void maybe_afterburner_after_ship_hit(object *objp, ai_info *aip, object *en_objp)
{
	//	Only do if facing a little away.
	if (en_objp != NULL) {
		vector	v2e;

		vm_vec_normalized_dir(&v2e, &en_objp->pos, &objp->pos);
		if (vm_vec_dot(&v2e, &objp->orient.v.fvec) > -0.5f)
			return;
	}

	if (!( objp->phys_info.flags & PF_AFTERBURNER_ON )) {
		if (ai_maybe_fire_afterburner(objp, aip)) {
			afterburners_start(objp);
			aip->afterburner_stop_time = Missiontime + F1_0/2;
		}
	}
}

//	Return true if object *objp is an instructor.
//	Is an instructor if name begins INSTRUCTOR_SHIP_NAME else not.
int is_instructor(object *objp)
{
	return !strnicmp(Ships[objp->instance].ship_name, INSTRUCTOR_SHIP_NAME, strlen(INSTRUCTOR_SHIP_NAME));
}

//	Evade the weapon aip->danger_weapon_objnum
//	If it's not valid, do a quick out.
//	Evade by accelerating hard.
//	If necessary, turn hard left or hard right.
void evade_weapon()
{
	object	*weapon_objp = NULL;
	object	*unlocked_weapon_objp = NULL, *locked_weapon_objp = NULL;
	vector	weapon_pos, player_pos, goal_point;
	vector	vec_from_enemy;
	float		dot_from_enemy, dot_to_enemy;
	float		dist;
	ship		*shipp = &Ships[Pl_objp->instance];
	ai_info	*aip = &Ai_info[shipp->ai_index];

	if (is_instructor(Pl_objp))
		return;

	//	Make sure we're actually being attacked.
	//	Favor locked objects.
	if (aip->nearest_locked_object != -1) {
		if (Objects[aip->nearest_locked_object].type == OBJ_WEAPON)
			locked_weapon_objp = &Objects[aip->nearest_locked_object];
	}
	
	if (aip->danger_weapon_objnum != -1) {
		if (Objects[aip->danger_weapon_objnum].signature == aip->danger_weapon_signature) {
			unlocked_weapon_objp = &Objects[aip->danger_weapon_objnum];
		} else {
			aip->danger_weapon_objnum = -1;		//	Signatures don't match, so no longer endangered.
		}
	}

	if (locked_weapon_objp != NULL) {
		if (unlocked_weapon_objp != NULL) {
			if (vm_vec_dist_quick(&locked_weapon_objp->pos, &Pl_objp->pos) < 1.5f * vm_vec_dist_quick(&unlocked_weapon_objp->pos, &Pl_objp->pos))
				weapon_objp = locked_weapon_objp;
			else
				weapon_objp = unlocked_weapon_objp;
		} else
			weapon_objp = locked_weapon_objp;
	} else if (unlocked_weapon_objp != NULL)
		weapon_objp = unlocked_weapon_objp;
	else {
		if (aip->mode == AIM_EVADE_WEAPON)
			maybe_resume_previous_mode(Pl_objp, aip);
		return;
	}

	Assert(weapon_objp != NULL);

	if (weapon_objp->type != OBJ_WEAPON) {
		if (aip->mode == AIM_EVADE_WEAPON)
			maybe_resume_previous_mode(Pl_objp, aip);
		return;
	}
	
	weapon_pos = weapon_objp->pos;
	player_pos = Pl_objp->pos;

	//	Make speed based on skill level, varying at highest skill level, which is harder to hit.
	accelerate_ship(aip, 1.0f);

	dist = vm_vec_normalized_dir(&vec_from_enemy, &player_pos, &weapon_pos);

	dot_to_enemy = -vm_vec_dot(&Pl_objp->orient.v.fvec, &vec_from_enemy);
	dot_from_enemy = vm_vec_dot(&weapon_objp->orient.v.fvec, &vec_from_enemy);
	//nprintf(("AI", "dot from enemy = %7.3f\n", dot_from_enemy));

	//	If shot is incoming...
	if (dot_from_enemy < 0.3f) {
		if (weapon_objp == unlocked_weapon_objp)
			aip->danger_weapon_objnum = -1;
		return;
	} else if (dot_from_enemy > 0.7f) {
		if (dist < 200.0f) {
			if (!( Pl_objp->phys_info.flags & PF_AFTERBURNER_ON )) {
				if (ai_maybe_fire_afterburner(Pl_objp, aip)) {
					//nprintf(("AI", "Frame %i, turning on afterburner.\n", AI_FrameCount));
					afterburners_start(Pl_objp);
					aip->afterburner_stop_time = Missiontime + F1_0/2;
				}
			}
		}

		//	If we're sort of pointing towards it...
		if ((dot_to_enemy < -0.5f) || (dot_to_enemy > 0.5f)) {
			float	rdot;

			//	Turn hard left or right, depending on which gets out of way quicker.
			rdot = vm_vec_dot(&Pl_objp->orient.v.rvec, &vec_from_enemy);

			if ((rdot < -0.5f) || (rdot > 0.5f))
				vm_vec_scale_add(&goal_point, &Pl_objp->pos, &Pl_objp->orient.v.rvec, -200.0f);
			else
				vm_vec_scale_add(&goal_point, &Pl_objp->pos, &Pl_objp->orient.v.rvec, 200.0f);

			turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
		}
	}

}

//	Use sliding and backwards moving to face enemy.
//	(Coded 2/20/98.  Works fine, but it's hard to see how to integrate it into the AI system.
//	 Typically ships are moving so fast that a little sliding isn't enough to gain an advantage.
//	 It's currently used to avoid collisions and could be used to evade weapon fire, but the latter
//	 would be frustrating, I think.
//	 This function is currently not called.)
void slide_face_ship()
{
	ship_info	*sip;

	sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];

	//	If can't slide, return.
	if ((sip->max_vel.xyz.x == 0.0f) && (sip->max_vel.xyz.y == 0.0f))
		return;

	vector	goal_pos;
	float		dot_from_enemy, dot_to_enemy;
	vector	vec_from_enemy, vec_to_goal;
	float		dist;
	float		up, right;
	ai_info		*aip;

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	dist = vm_vec_normalized_dir(&vec_from_enemy, &Pl_objp->pos, &En_objp->pos);

	ai_turn_towards_vector(&En_objp->pos, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);

	dot_from_enemy = vm_vec_dot(&vec_from_enemy, &En_objp->orient.v.fvec);
	dot_to_enemy = -vm_vec_dot(&vec_from_enemy, &Pl_objp->orient.v.fvec);

	if (vm_vec_dot(&vec_from_enemy, &En_objp->orient.v.rvec) > 0.0f)
		right = 1.0f;
	else
		right = -1.0f;

	if (vm_vec_dot(&vec_from_enemy, &En_objp->orient.v.uvec) > 0.0f)
		up = 1.0f;
	else
		up = -1.0f;

	vm_vec_scale_add(&goal_pos, &En_objp->pos, &En_objp->orient.v.rvec, right * 200.0f);
	vm_vec_scale_add(&goal_pos, &En_objp->pos, &En_objp->orient.v.uvec, up * 200.0f);

	vm_vec_normalized_dir(&vec_to_goal, &goal_pos, &Pl_objp->pos);

	if (vm_vec_dot(&vec_to_goal, &Pl_objp->orient.v.rvec) > 0.0f)
		AI_ci.sideways = 1.0f;
	else
		AI_ci.sideways = -1.0f;

	if (vm_vec_dot(&vec_to_goal, &Pl_objp->orient.v.uvec) > 0.0f)
		AI_ci.vertical = 1.0f;
	else
		AI_ci.vertical = -1.0f;

	if (dist < 200.0f) {
		if (dot_from_enemy < 0.7f)
			accelerate_ship(aip, -1.0f);
		else
			accelerate_ship(aip, dot_from_enemy + 0.5f);
	} else {
		if (dot_from_enemy < 0.7f) {
			accelerate_ship(aip, 0.2f);
		} else {
			accelerate_ship(aip, 1.0f);
		}
	}
}

//	General code for handling one ship evading another.
//	Problem: This code is also used for avoiding an impending collision.
//	In such a case, it is not good to go to max speed, which is often good
//	for a certain kind of evasion.
void evade_ship()
{
	vector	player_pos, enemy_pos, goal_point;
	vector	vec_from_enemy;
	float		dot_from_enemy;
	float		dist;
	ship		*shipp = &Ships[Pl_objp->instance];
	ship_info	*sip = &Ship_info[shipp->ship_info_index];
	ai_info	*aip = &Ai_info[shipp->ai_index];
	float		bank_override = 0.0f;

	ai_set_positions(Pl_objp, En_objp, aip, &player_pos, &enemy_pos);

	//	Make speed based on skill level, varying at highest skill level, which is harder to hit.
	if (Game_skill_level == NUM_SKILL_LEVELS-1) {
		int	rand_int;
		float	accel_val;

		rand_int = static_rand(Pl_objp-Objects);
		accel_val = (float) (((Missiontime^rand_int) >> 14) & 0x0f)/32.0f + 0.5f;
		accelerate_ship(aip, accel_val);
		//nprintf(("AI", "Accel value = %7.3f\n", accel_val));
	} else
		accelerate_ship(aip, (float) (Game_skill_level+2) / (NUM_SKILL_LEVELS+1));

	if ((Missiontime - aip->submode_start_time > F1_0/2) && (sip->afterburner_fuel_capacity > 0.0f)) {
		float percent_left = 100.0f * shipp->afterburner_fuel / sip->afterburner_fuel_capacity;
		if (percent_left > 30.0f + ((Pl_objp-Objects) & 0x0f)) {
			afterburners_start(Pl_objp);
			aip->afterburner_stop_time = Missiontime + F1_0 + static_rand(Pl_objp-Objects)/4;
		}
	}

	vm_vec_sub(&vec_from_enemy, &player_pos, &enemy_pos);

	dist = vm_vec_normalize(&vec_from_enemy);
	dot_from_enemy = vm_vec_dot(&En_objp->orient.v.fvec, &vec_from_enemy);

	if (dist > 250.0f) {
		vector	gp1, gp2;
		//	If far away from enemy, circle, going to nearer of point far off left or right wing
		vm_vec_scale_add(&gp1, &enemy_pos, &En_objp->orient.v.rvec, 250.0f);
		vm_vec_scale_add(&gp2, &enemy_pos, &En_objp->orient.v.rvec, -250.0f);
		if (vm_vec_dist_quick(&gp1, &Pl_objp->pos) < vm_vec_dist_quick(&gp2, &Pl_objp->pos))
			goal_point = gp1;
		else
			goal_point = gp2;
	} else if (dot_from_enemy < 0.1f) {
		//	If already close to behind, goal is to get completely behind.
		vm_vec_scale_add(&goal_point, &enemy_pos, &En_objp->orient.v.fvec, -1000.0f);
	} else if (dot_from_enemy > 0.9f) {
		//	If enemy pointing almost right at self, and self pointing close to enemy, turn away from
		vector	vec_to_enemy;
		float		dot_to_enemy;

		vm_vec_sub(&vec_to_enemy, &enemy_pos, &player_pos);

		vm_vec_normalize(&vec_to_enemy);
		dot_to_enemy = vm_vec_dot(&Pl_objp->orient.v.fvec, &vec_to_enemy);
		if (dot_to_enemy > 0.75f) {
			//	Used to go to En_objp's right vector, but due to banking while turning, that
			//	caused flying in an odd spiral.
			vm_vec_scale_add(&goal_point, &enemy_pos, &Pl_objp->orient.v.rvec, 1000.0f);
			if (dist < 100.0f)
				bank_override = Pl_objp->phys_info.speed; 
		} else {
			bank_override = Pl_objp->phys_info.speed;			//	In enemy's sights, not pointing at him, twirl away.
			// nprintf(("Mike", " Do sumpin' else."));
			goto evade_ship_l1;
		}
	} else {
evade_ship_l1: ;
		if (aip->ai_evasion > myrand()*100.0f/32767.0f) {
			int	temp;
			float	scale;
			float	psrandval;	//	some value close to zero to choose whether to turn right or left.

			psrandval = (float) (((Missiontime >> 14) & 0x0f) - 8);	//	Value between -8 and 7
			psrandval = psrandval/16.0f;							//	Value between -1/2 and 1/2 (approx)

			//	If not close to behind, turn towards his right or left vector, whichever won't cross his path.
			if (vm_vec_dot(&vec_from_enemy, &En_objp->orient.v.rvec) > psrandval) {
				scale = 1000.0f;
			} else {
				scale = -1000.0f;
			}

			vm_vec_scale_add(&goal_point, &enemy_pos, &En_objp->orient.v.rvec, scale);

			temp = ((Missiontime >> 16) & 0x07);
			temp = ((temp * (temp+1)) % 16)/2 - 4;
			if ((psrandval == 0) && (temp == 0))
				temp = 3;

			scale = 200.0f * temp;

			vm_vec_scale_add2(&goal_point, &En_objp->orient.v.uvec, scale);
		} else {
			//	No evasion this frame, but continue with previous turn.
			//	Reason: If you don't, you lose rotational momentum.  Turning every other frame,
			//	and not in between results in a very slow turn because of loss of momentum.
			if ((aip->prev_goal_point.xyz.x != 0.0f) || (aip->prev_goal_point.xyz.y != 0.0f) || (aip->prev_goal_point.xyz.z != 0.0f))
				goal_point = aip->prev_goal_point;
			else
				vm_vec_scale_add(&goal_point, &enemy_pos, &En_objp->orient.v.rvec, 100.0f);
		}
	}

	// nprintf(("Mike", "Goal point = %7.1f %7.1f %7.1f\n", goal_point.xyz.x, goal_point.xyz.y, goal_point.xyz.z));
	turn_towards_point(Pl_objp, &goal_point, NULL, bank_override);

	aip->prev_goal_point = goal_point;
}

//	--------------------------------------------------------------------------
//	Fly in a manner making it difficult for opponent to attack.
void ai_evade()
{
	evade_ship();
}

/*
// -------------------------------------------------------------------
//	Refine predicted enemy position because enemy will move while we move
//	towards predicted enemy position.
//	last_delta_vec is stuffed with size of polishing in last step.  This small amount
//	can be used to perturb the predicted position to make firing not be exact.
//	This function will almost always undershoot actual position, assuming both ships
//	are moving at constant speed.  But with even one polishing step, the error should
//	be under 1%. The number of polishing steps is specified in the parameter num_polish_steps.
void polish_predicted_enemy_pos(vector *predicted_enemy_pos, object *pobjp, object *eobjp, float dist_to_enemy, vector *last_delta_vec, int num_polish_steps) // Not used:, float time_to_enemy)
{
	int	iteration;
	vector	player_pos = pobjp->pos;
	vector	enemy_pos = *predicted_enemy_pos;
	physics_info	*en_physp = &eobjp->phys_info;
	float		time_to_enemy;
	vector	last_predicted_enemy_pos = *predicted_enemy_pos;
	
	vm_vec_zero(last_delta_vec);

	for (iteration=0; iteration < num_polish_steps; iteration++) {
		dist_to_enemy = vm_vec_dist_quick(predicted_enemy_pos, &player_pos);
		time_to_enemy = compute_time_to_enemy(dist_to_enemy, pobjp, eobjp);
		vm_vec_scale_add(predicted_enemy_pos, &eobjp->pos, &eobjp->phys_info.vel, time_to_enemy);
		vm_vec_sub(last_delta_vec, predicted_enemy_pos, &last_predicted_enemy_pos);
		last_predicted_enemy_pos= *predicted_enemy_pos;
	}
}
*/

/*
Relevant variables are:
	best_dot_to_enemy		best dot product to enemy in last BEST_DOT_TIME seconds
	best_dot_to_time		time at which best dot occurred
	best_dot_from_enemy	best dot product for enemy to player in last BEST_DOT_TIME seconds
	best_dot_from_time	time at which best dot occurred
	submode_start_time	time at which we entered the current submode
	previous_submode		previous submode, get it?
Legal submodes are:
	CONTINUOUS_TURN	vector_id {0..3 = right, -right, up, -up}
	ATTACK
	EVADE_SQUIGGLE
	EVADE_BRAKE
*/

float	G_collision_time;
vector	G_predicted_pos, G_fire_pos;

/*
void show_firing_diag()
{
	float		dot;
	vector	v2t;
	vector	pos1, pos2;
	float		dist;

	if (G_collision_time == 0.0f)
		return;

	mprintf(("Fired from %5.1f, %5.1f %5.1f at time = %5.1f, predict collision in %5.2f seconds at %5.1f %5.1f %5.1f\n",
		Pl_objp->pos.xyz.x, Pl_objp->pos.xyz.y, Pl_objp->pos.xyz.z, (float) Missiontime/1000.0f, G_collision_time, G_predicted_pos.xyz.x, G_predicted_pos.xyz.y, G_predicted_pos.xyz.z));
	vm_vec_normalized_dir(&v2t, &G_predicted_pos, &G_fire_pos);
	dot = vm_vec_dot(&v2t, &Pl_objp->orient.v.fvec);
	mprintf(("Dot of v.fvec and vector to predicted position = %10.7f (%7.3f degrees)\n", dot, acos(dot)*180.0f/3.141592654f));

	vm_vec_scale_add(&pos1, &En_objp->pos, &En_objp->phys_info.vel, G_collision_time);
	vm_vec_scale_add(&pos2, &G_fire_pos, &Pl_objp->orient.v.fvec, G_collision_time*300.0f);
	dist = vm_vec_dist(&pos1, &pos2);

	mprintf(("Enemy, laser pos, distance: [%5.1f %5.1f %5.1f]  [%5.1f %5.1f %5.1f]  %6.2f\n", pos1.xyz.x, pos1.xyz.y, pos1.xyz.z, pos2.xyz.x, pos2.xyz.y, pos2.xyz.z, dist));
}
*/

//	If:
//		flags & WIF_PUNCTURE
//	Then Select a Puncture weapon.
//	Else
//		Select Any ol' weapon.
//	Returns primary_bank index.
int ai_select_primary_weapon(object *objp, object *other_objp, int flags)
{
	ship	*shipp = &Ships[objp->instance];
	ship_weapon *swp = &shipp->weapons;
	ship_info *sip;

	//Assert( other_objp != NULL );
	Assert( shipp->ship_info_index >= 0 && shipp->ship_info_index < MAX_SHIP_TYPES);

	sip = &Ship_info[shipp->ship_info_index];

	if (flags & WIF_PUNCTURE) {
		if (swp->current_primary_bank >= 0) {
			int	bank_index;

			bank_index = swp->current_primary_bank;

			if (Weapon_info[swp->primary_bank_weapons[bank_index]].wi_flags & WIF_PUNCTURE) {
				//nprintf(("AI", "%i: Ship %s selecting weapon %s\n", Framecount, Ships[objp->instance].ship_name, Weapon_info[swp->primary_bank_weapons[bank_index]].name));
				return swp->current_primary_bank;
			}
		}
		for (int i=0; i<swp->num_primary_banks; i++) {
			int	weapon_info_index;

			weapon_info_index = swp->primary_bank_weapons[i];

			if (weapon_info_index > -1){
				if (Weapon_info[weapon_info_index].wi_flags & WIF_PUNCTURE) {
					swp->current_primary_bank = i;
					//nprintf(("AI", "%i: Ship %s selecting weapon %s\n", Framecount, Ships[objp->instance].ship_name, Weapon_info[swp->primary_bank_weapons[i]].name));
					return i;
				}
			}
		}
		
		// AL 26-3-98: If we couldn't find a puncture weapon, pick first available weapon if one isn't active
		if ( swp->current_primary_bank < 0 ) {
			if ( swp->num_primary_banks > 0 ) {
				swp->current_primary_bank = 0;
			}
		}

	} else {		//	Don't need to be using a puncture weapon.
		if (swp->current_primary_bank >= 0) {
			if (!(Weapon_info[swp->primary_bank_weapons[swp->current_primary_bank]].wi_flags & WIF_PUNCTURE)){
				return swp->current_primary_bank;
			}
		}
		for (int i=0; i<swp->num_primary_banks; i++) {
			if (swp->primary_bank_weapons[i] > -1) {
				if (!(Weapon_info[swp->primary_bank_weapons[i]].wi_flags & WIF_PUNCTURE)) {
					swp->current_primary_bank = i;
					nprintf(("AI", "%i: Ship %s selecting weapon %s\n", Framecount, Ships[objp->instance].ship_name, Weapon_info[swp->primary_bank_weapons[i]].name));
					return i;
				}
			}
		}
		//	Wasn't able to find a non-puncture weapon.  Stick with what we have.
	}

	Assert( swp->current_primary_bank != -1 );		// get Alan or Allender

	return swp->current_primary_bank;
}

//	--------------------------------------------------------------------------
//	Maybe link primary weapons.
void set_primary_weapon_linkage(object *objp)
{
	ship		*shipp;
	ai_info	*aip;

	shipp = &Ships[objp->instance];
	aip	= &Ai_info[shipp->ai_index];

	shipp->flags &= ~SF_PRIMARY_LINKED;

	if (Num_weapons > (int) (MAX_WEAPONS * 0.75f)) {
		if (shipp->flags & SF_PRIMARY_LINKED)
			nprintf(("AI", "Frame %i, ship %s: Unlinking primaries.\n", Framecount, shipp->ship_name));
		shipp->flags &= ~SF_PRIMARY_LINKED;
		return;		//	If low on slots, don't link.
	}

	shipp->flags &= ~SF_PRIMARY_LINKED;

	// AL: ensure target is a ship!
	if ( Objects[aip->target_objnum].type == OBJ_SHIP ) {
		// If trying to destroy a big ship (i.e., not disable/disarm), always unleash all weapons
		if ( ship_get_SIF(&Ships[Objects[aip->target_objnum].instance]) & SIF_BIG_SHIP) {
			if ( aip->targeted_subsys == NULL ) {
				shipp->flags |= SF_PRIMARY_LINKED;
				shipp->flags |= SF_SECONDARY_DUAL_FIRE;
				return;
			}
		}
	}

	// AL 2-11-98: If ship has a disarm or disable goal, don't link unless both weapons are
	//					puncture weapons
	if ( (aip->active_goal != AI_GOAL_NONE) && (aip->active_goal != AI_ACTIVE_GOAL_DYNAMIC) ) {
		if ( aip->goals[aip->active_goal].ai_mode & (AI_GOAL_DISABLE_SHIP|AI_GOAL_DISARM_SHIP) ) {
			ship_weapon	*swp;
			swp = &shipp->weapons;
			// only continue if both primaries are puncture weapons
			if ( swp->num_primary_banks == 2 ) {
				if ( !(Weapon_info[swp->primary_bank_weapons[0]].wi_flags & WIF_PUNCTURE) ) 
					return;
				if ( !(Weapon_info[swp->primary_bank_weapons[1]].wi_flags & WIF_PUNCTURE) ) 
					return;
			}
		}
	}

	//	Don't want all ships always linking weapons at start, so asynchronize.
	if (Missiontime < i2f(30))
		return;
	else if (Missiontime < i2f(120)) {
		int r = static_rand((Missiontime >> 17) ^ OBJ_INDEX(objp));
		if ( (r&3) != 0)
			return;
	}

	if (shipp->weapon_energy > Link_energy_levels_always[Game_skill_level]) {
		shipp->flags |= SF_PRIMARY_LINKED;
	} else if (shipp->weapon_energy > Link_energy_levels_maybe[Game_skill_level]) {
		if (objp->hull_strength < Ship_info[shipp->ship_info_index].initial_hull_strength/3.0f)
			shipp->flags |= SF_PRIMARY_LINKED;
	}
}

//	--------------------------------------------------------------------------
//	Fire the current primary weapon.
//	*objp is the object to fire from.
void ai_fire_primary_weapon(object *objp)
{
	ship			*shipp = &Ships[objp->instance];
	ship_weapon	*swp = &shipp->weapons;
	ship_info	*sip;
	ai_info		*aip;
	object		*enemy_objp;

	Assert( shipp->ship_info_index >= 0 && shipp->ship_info_index < MAX_SHIP_TYPES);
	sip = &Ship_info[shipp->ship_info_index];

	aip = &Ai_info[shipp->ai_index];

	//	If low on slots, fire a little less often.
	if (Num_weapons > (int) (0.9f * MAX_WEAPONS)) {
		if (frand() > 0.5f) {
			nprintf(("AI", "Frame %i, %s not fire.\n", Framecount, shipp->ship_name));
			return;
		}
	}

	if (!Ai_firing_enabled){
		return;
	}

	if (aip->target_objnum != -1){
		enemy_objp = &Objects[aip->target_objnum];
	} else {
		enemy_objp = NULL;
	}

	if ( (swp->current_primary_bank < 0) || (swp->current_primary_bank >= swp->num_primary_banks) || timestamp_elapsed(aip->primary_select_timestamp)) {
		int	flags = 0;
		// AL 2-11-98: If attacking any subsystem (not just engines), use disrupter weapon
//		if ((aip->targeted_subsys != NULL) && (aip->targeted_subsys->system_info->type == SUBSYSTEM_ENGINE)) {
		if ( aip->targeted_subsys != NULL ) {
			flags = WIF_PUNCTURE;
		}
		ai_select_primary_weapon(objp, enemy_objp, flags);
		ship_primary_changed(shipp);	// AL: maybe send multiplayer information when AI ship changes primaries
		aip->primary_select_timestamp = timestamp(5 * 1000);	//	Maybe change primary weapon five seconds from now.
	}

	//	If pointing nearly at predicted collision point of target, bash orientation to be perfectly pointing.
	float	dot;
	vector	v2t;

//	if (!IS_VEC_NULL(&G_predicted_pos)) {
	if (!( vm_vec_mag_quick(&G_predicted_pos) < AICODE_SMALL_MAGNITUDE )) {
		if ( !vm_vec_cmp(&G_predicted_pos, &G_fire_pos) ) {
			nprintf(("Warning", "Avoid NULL vector assert.. why are G_predicted_pos and G_fire_pos the same?\n"));
		} else {
			vm_vec_normalized_dir(&v2t, &G_predicted_pos, &G_fire_pos);
			dot = vm_vec_dot(&v2t, &objp->orient.v.fvec);
			if (dot > .998629534f){	//	if within 3.0 degrees of desired heading, bash
				vm_vector_2_matrix(&objp->orient, &v2t, &objp->orient.v.uvec, NULL);
			}
		}
	}

	//	Make sure not firing at a protected ship unless firing at a live subsystem.
	//	Note: This happens every time the ship tries to fire, perhaps every frame.
	//	Should be wrapped in a timestamp, same one that enables it to fire, but that is complicated
	//	by multiple banks it can fire from.
	if (aip->target_objnum != -1) {
		object	*tobjp = &Objects[aip->target_objnum];
		if (tobjp->flags & OF_PROTECTED) {
			if (aip->targeted_subsys != NULL) {
				int	type;

				type = aip->targeted_subsys->system_info->type;
				if (ship_get_subsystem_strength(&Ships[tobjp->instance], type) == 0.0f) {
					aip->target_objnum = -1;
					return;
				}
			} else {
				aip->target_objnum = -1;
				return;
			}
		}
	}

	//	If enemy is protected, not firing a puncture weapon and enemy's hull is low, don't fire.
	if ((enemy_objp != NULL) && (enemy_objp->flags & OF_PROTECTED)) {
		// AL: 3-6-98: Check if current_primary_bank is valid
		if ((enemy_objp->hull_strength < 750.0f) && 
			((aip->targeted_subsys == NULL) || (enemy_objp->hull_strength < aip->targeted_subsys->current_hits + 50.0f)) &&
			(swp->current_primary_bank >= 0) ) {
			if (!(Weapon_info[swp->primary_bank_weapons[swp->current_primary_bank]].wi_flags & WIF_PUNCTURE)) {
				//nprintf(("AI", "Ship %s not firing at protected ship %s because not using disruptor.\n", Ships[objp->instance].ship_name, Ships[enemy_objp->instance].ship_name));
				swp->next_primary_fire_stamp[swp->current_primary_bank] = timestamp(1000);
				return;
			}

			/*
			int	num_attacking;
			num_attacking = num_enemies_attacking(enemy_objp-Objects);
			if (enemy_objp->hull_strength / num_attacking < 200.0f) {
				if (frand() < 0.75f) {
					nprintf(("AI", "Ship %s not firing at protected ship %s because too many attacking.\n", Ships[objp->instance].ship_name, Ships[enemy_objp->instance].ship_name));
					swp->next_primary_fire_stamp[swp->current_primary_bank] = timestamp(500);
					return;
				}
			}
			*/
		}
	}

	set_primary_weapon_linkage(objp);
	
	// I think this will properly solve the problem
	// fire non-streaming weapons
	ship_fire_primary(objp, 0);
	
	// fire streaming weapons
	shipp->flags |= SF_TRIGGER_DOWN;
	ship_fire_primary(objp, 1);
	shipp->flags &= ~SF_TRIGGER_DOWN;
}

//	--------------------------------------------------------------------------
//	Return number of nearby enemy fighters.
//	threshold is the distance within which a ship is considered near.
//
// input:	enemy_team_mask	=>	teams that are considered as an enemy
//				pos					=>	world position to measure ship distances from
//				threshold			=>	max distance from pos to be considered "near"
//
// exit:		number of ships within threshold units of pos
int num_nearby_fighters(int enemy_team_mask, vector *pos, float threshold)
{
	ship_obj	*so;
	object	*ship_objp;
	int		count = 0;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {

		ship_objp = &Objects[so->objnum];

		if (Ships[ship_objp->instance].team & enemy_team_mask) {
			if (Ship_info[Ships[ship_objp->instance].ship_info_index].flags & (SIF_FIGHTER | SIF_BOMBER)) {
				if (vm_vec_dist_quick(pos, &ship_objp->pos) < threshold)
					count++;
			}
		}
	}

	return count;
}

//	--------------------------------------------------------------------------
//	Select secondary weapon to fire.
//	Currently, 1/16/98:
//		If 0 secondary weapons available, return -1
//		If 1 available, use it.
//		If 2 or more, if the current weapon is one of them, stick with it, otherwise choose a random one.
//	priority1 and priority2 are Weapon_info[] bitmasks such as WIF_HOMING_ASPECT.  If any weapon has any bit in priority1
//	set, that weapon will be selected.  If not, apply to priority2.  If neither, return -1, meaning no weapon selected.
//	Note, priorityX have default values of -1, meaning if not set, they will match any weapon.
//	Return value:
//		bank index
//	Should do this:
//		Favor aspect seekers when attacking small ships faraway.
//		Favor rapid fire dumbfire when attacking a large ship.
//		Ignore heat seekers because we're not sure how they'll work.
void ai_select_secondary_weapon(object *objp, ship_weapon *swp, int priority1 = -1, int priority2 = -1)
{
	int	num_weapon_types;
	int	weapon_id_list[MAX_WEAPON_TYPES], weapon_bank_list[MAX_WEAPON_TYPES];
	int	i;
	int	ignore_mask;
	int	initial_bank;

	initial_bank = swp->current_secondary_bank;

	//	Ignore bombs unless one of the priorities asks for them to be selected.
	if (WIF_HUGE & (priority1 | priority2))
		ignore_mask = 0;
	else
		ignore_mask = WIF_HUGE;

	if (!(WIF_BOMBER_PLUS & (priority1 | priority2)))
		ignore_mask |= WIF_BOMBER_PLUS;

#ifndef NDEBUG
	for (i=0; i<MAX_WEAPON_TYPES; i++) {
		weapon_id_list[i] = -1;
		weapon_bank_list[i] = -1;
	}
#endif

	//	Stuff weapon_bank_list with bank index of available weapons.
	num_weapon_types = get_available_secondary_weapons(objp, weapon_id_list, weapon_bank_list);

	int	priority2_index = -1;

	for (i=0; i<num_weapon_types; i++) {
		int	wi_flags;

		wi_flags = Weapon_info[swp->secondary_bank_weapons[weapon_bank_list[i]]].wi_flags;
		if (!(wi_flags & ignore_mask)) {					//	Maybe bombs are illegal.
			if (wi_flags & priority1) {
				swp->current_secondary_bank = weapon_bank_list[i];				//	Found first priority, return it.
				break;
			} else if (wi_flags & priority2)
				priority2_index = weapon_bank_list[i];	//	Found second priority, but might still find first priority.
		}
	}

	//	If didn't find anything above, then pick any secondary weapon.
	if (i == num_weapon_types) {
		swp->current_secondary_bank = priority2_index;	//	Assume we won't find anything.
		if (priority2_index == -1) {
			for (i=0; i<num_weapon_types; i++) {
				int	wi_flags;

				wi_flags = Weapon_info[swp->secondary_bank_weapons[weapon_bank_list[i]]].wi_flags;
				if (!(wi_flags & ignore_mask)) {					//	Maybe bombs are illegal.
					if (swp->secondary_bank_ammo[i] > 0) {
						swp->current_secondary_bank = i;
						break;
					}
				}
			}
		}
	}

	//	If switched banks, force reacquisition of aspect lock.
	if (swp->current_secondary_bank != initial_bank) {
		ai_info	*aip = &Ai_info[Ships[objp->instance].ai_index];
		
		aip->aspect_locked_time = 0.0f;
		aip->current_target_is_locked = 0;
	}


	ship_secondary_changed(&Ships[objp->instance]);	// AL: let multiplayer know if secondary bank has changed
	// nprintf(("AI", "Ship %s selected weapon %s\n", Ships[objp->instance].ship_name, Weapon_info[swp->secondary_bank_weapons[swp->current_secondary_bank]].name));
}

//	Return number of objects homing on object *target_objp
int compute_num_homing_objects(object *target_objp)
{
	object	*objp;
	int		count = 0;

	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if (objp->type == OBJ_WEAPON) {
			if (Weapon_info[Weapons[objp->instance].weapon_info_index].wi_flags & WIF_HOMING) {
				if (Weapons[objp->instance].homing_object == target_objp) {
					count++;
				}
			}
		}
	}

	return count;
}

//	Object *firing_objp just fired weapon weapon_index (index in Weapon_info).
//	If it's a shockwave weapon, tell your team about it!
void ai_maybe_announce_shockwave_weapon(object *firing_objp, int weapon_index)
{
	if ((firing_objp->type == OBJ_SHIP) && (Weapon_info[weapon_index].shockwave_speed > 0.0f)) {
		ship_obj	*so;
		int		firing_ship_team;

		firing_ship_team = Ships[firing_objp->instance].team;

		for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
			object	*A = &Objects[so->objnum];
			Assert(A->type == OBJ_SHIP);

			if (Ships[A->instance].team == firing_ship_team) {
				ai_info	*aip = &Ai_info[Ships[A->instance].ai_index];
				// AL 1-5-98: only avoid shockwave if not docked or repairing
				if ( !(aip->ai_flags & (AIF_DOCKED|AIF_BEING_REPAIRED)) ) {
					aip->ai_flags |= AIF_AVOID_SHOCKWAVE_WEAPON;
				}
			}
		}
	}
}

//	Return total payload of all incoming missiles.
float compute_incoming_payload(object *target_objp)
{
	missile_obj	*mo;
	float			payload = 0.0f;

	for ( mo = GET_NEXT(&Missile_obj_list); mo != END_OF_LIST(&Missile_obj_list); mo = GET_NEXT(mo) ) {
		object	*objp;

		objp = &Objects[mo->objnum];
		Assert(objp->type == OBJ_WEAPON);
		if (Weapons[objp->instance].homing_object == target_objp) {
			payload += Weapon_info[Weapons[objp->instance].weapon_info_index].damage;
		}
	}

	return payload;
}

//	--------------------------------------------------------------------------
//	Return true if OK for *aip to fire its current weapon at its current target.
//	Only reason this function returns false is:
//		weapon is a homer
//		targeted at player
//			OR:	player has too many homers targeted at him
//					Missiontime in that dead zone in which can't fire at this player
//	Note: If player is attacking a ship, that ship is allowed to fire at player.  Otherwise, we get in a situation in which
//	player is attacking a large ship, but that large ship is not defending itself with missiles.
int check_ok_to_fire(int objnum, int target_objnum, weapon_info *wip)
{
	int	num_homers = 0;
	object	*tobjp = &Objects[target_objnum];

	if (target_objnum > -1) {
		// AL 3-4-98: Ensure objp target is a ship first 
		if ( tobjp->type == OBJ_SHIP ) {

			// should not get this far. check if ship is protected from beam and weapon is type beam
			if ( (wip->wi_flags & WIF_BEAM) && (tobjp->flags & OF_BEAM_PROTECTED) ) {
				Int3();
				return 0;
			}
			if (Ship_info[Ships[tobjp->instance].ship_info_index].flags & SIF_SMALL_SHIP) {
				num_homers = compute_num_homing_objects(&Objects[target_objnum]);
			}
		}

		//	If player, maybe fire based on Skill_level and number of incoming weapons.
		//	If non-player, maybe fire based on payload of incoming weapons.
		if (wip->wi_flags & WIF_HOMING) {
			if ((target_objnum > -1) && (tobjp->flags & OF_PLAYER_SHIP)) {
				if (Ai_info[Ships[tobjp->instance].ai_index].target_objnum != objnum) {
					//	Don't allow AI ships to fire at player for fixed periods of time based on skill level.
					//	With 5 skill levels, at Very Easy, they fire in 1/7 of every 10 second interval.
					//	At Easy, 2/7...at Expert, 5/7
					int t = ((Missiontime /(65536*10)) ^ target_objnum ^ 0x01) % (NUM_SKILL_LEVELS+2);
					if (t > Game_skill_level) {
						//nprintf(("AI", "Not OK to fire homer at time thing %i\n", t));
						return 0;
					}
				}
				//nprintf(("AI", " IS OK to fire homer at time thing %i ***\n", t));
				int	swarmers = 0;
				if (wip->wi_flags & WIF_SWARM)
					swarmers = 2;	//	Note, always want to be able to fire swarmers if no currently incident homers.
				if (Max_allowed_player_homers[Game_skill_level] < num_homers + swarmers) {
					return 0;
				}
			} else if (num_homers > 3) {
				float	incoming_payload;

				incoming_payload = compute_incoming_payload(&Objects[target_objnum]);

				if (incoming_payload > tobjp->hull_strength) {
					return 0;
				}
			}
		}
	}

	return 1;
}

//	--------------------------------------------------------------------------
//	Fire a secondary weapon.
//	Maybe choose to fire a different one.
//	priority1 and priority2 are optional parameters with defaults = -1
int ai_fire_secondary_weapon(object *objp, int priority1, int priority2)
{
	ship_weapon *swp;
	ship	*shipp;
	ship_info *sip;
	int		current_bank;
	int		rval = 0;

#ifndef NDEBUG
	if (!Ai_firing_enabled)
		return rval;
#endif

	Assert( objp != NULL );
	Assert(objp->type == OBJ_SHIP);
	shipp = &Ships[objp->instance];
	swp = &shipp->weapons;

	Assert( shipp->ship_info_index >= 0 && shipp->ship_info_index < MAX_SHIP_TYPES);
	sip = &Ship_info[shipp->ship_info_index];

	//	Select secondary weapon.
	current_bank = swp->current_secondary_bank; //ai_select_secondary_weapon(objp, swp, priority1, priority2);

	//nprintf(("AI", "Frame %i: Current bank = %i, ammo remaining = %i\n", Framecount, current_bank, swp->secondary_bank_ammo[current_bank]));
	if (current_bank == -1) {
		swp->next_secondary_fire_stamp[current_bank] = timestamp(500);
		return rval;
	}

	Assert(current_bank < shipp->weapons.num_secondary_banks);

	weapon_info	*wip = &Weapon_info[shipp->weapons.secondary_bank_weapons[current_bank]];

	if ((wip->wi_flags & WIF_HOMING_ASPECT) && (!Ai_info[shipp->ai_index].current_target_is_locked)) {
		//nprintf(("AI", "Not firing secondary weapon because not aspect locked.\n"));
		swp->next_secondary_fire_stamp[current_bank] = timestamp(250);
	} else if ((wip->wi_flags & WIF_BOMB) || (vm_vec_dist_quick(&objp->pos, &En_objp->pos) > 50.0f)) {
		//	This might look dumb, firing a bomb even if closer than 50 meters, but the reason is, if you're carrying
		//	bombs, delivering them is probably more important than surviving.
		ai_info	*aip;

		aip = &Ai_info[shipp->ai_index];
		
		//	Note, maybe don't fire if firing at player and any homers yet fired.
		//	Decreasing chance to fire the more homers are incoming on player.
		if (check_ok_to_fire(OBJ_INDEX(objp), aip->target_objnum, wip)) {
			if (ship_fire_secondary(objp)) {
				rval = 1;
				swp->next_secondary_fire_stamp[current_bank] = timestamp(500);
				//nprintf(("AI", "Frane %i: Ship %s fired secondary %s\n", Framecount, Ships[objp->instance].ship_name, Weapon_info[swp->secondary_bank_weapons[current_bank]].name));
			}

		} else {
			swp->next_secondary_fire_stamp[current_bank] = timestamp(500);
		}
	}

	return rval;
}

//	Return true if it looks like obj1, if continuing to move along current vector, will
//	collide with obj2.
int might_collide_with_ship(object *obj1, object *obj2, float dot_to_enemy, float dist_to_enemy, float duration)
{
	if (obj1->phys_info.speed * duration + 2*(obj1->radius + obj2->radius) > dist_to_enemy)
		if (dot_to_enemy > 0.8f - 2*(obj1->radius + obj2->radius)/dist_to_enemy)
			return objects_will_collide(obj1, obj2, duration, 2.0f);

//		BABY - 
//		CONDITION 1, dist_to_enemy < o1_rad + o2_rad + (obj1.speed + obj2.speed) * time + 50
	
	return 0;

}

//	--------------------------------------------------------------------------
//	Return true if ship *objp firing a laser believes it will hit a teammate.
int might_hit_teammate(object *firing_objp)
{
	int		team;
	object	*objp;
	ship_obj	*so;

	team = Ships[firing_objp->instance].team;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		if (Ships[objp->instance].team == team) {
			float		dist, dot;
			vector	vec_to_objp;

			vm_vec_sub(&vec_to_objp, &firing_objp->pos, &objp->pos);
			dist = vm_vec_mag_quick(&vec_to_objp);
			dot = vm_vec_dot(&firing_objp->orient.v.fvec, &vec_to_objp)/dist;
			if (might_collide_with_ship(firing_objp, objp, dot, dist, 2.0f))
				return 1;
		}
	}

	return 0;

}

//int	Team_not_fire_count=0, Team_hit_count = 0;

void render_all_ship_bay_paths(object *objp)
{
	int		i,j,color;
	ship		*sp = &Ships[objp->instance];
	polymodel	*pm;
	model_path	*mp;

	pm = model_get(sp->modelnum);
	vector	global_path_point;
	vertex	v, prev_vertex;

	if ( pm->ship_bay == NULL )
		return;

	for ( i = 0; i < pm->ship_bay->num_paths; i++ ) {
		mp = &pm->paths[pm->ship_bay->paths[i]];

		for ( j = 0; j < mp->nverts; j++ ) {
			vm_vec_unrotate(&global_path_point, &mp->verts[j].pos, &objp->orient);
			vm_vec_add2(&global_path_point, &objp->pos);
			g3_rotate_vertex(&v, &global_path_point);
			color = 255 - j*50;
			if ( color < 50 ) 
				color = 100;
			gr_set_color(0, color, 0);

			if ( j == mp->nverts-1 ) {
				gr_set_color(255, 0, 0);
			}

			g3_draw_sphere( &v, 1.5f);

			if ( j > 0 )
				g3_draw_line(&v, &prev_vertex);

			prev_vertex = v;
	
		}
	}
}

// debug function to show all path points associated with an object
void render_all_subsys_paths(object *objp)
{
	int		i,j,color;
	ship		*sp = &Ships[objp->instance];
	polymodel	*pm;
	model_path	*mp;

	pm = model_get(sp->modelnum);
	vector	global_path_point;
	vertex	v, prev_vertex;

	if ( pm->ship_bay == NULL )
		return;

	for ( i = 0; i < pm->n_paths; i++ ) {
		mp = &pm->paths[i];
		for ( j = 0; j < mp->nverts; j++ ) {
			vm_vec_unrotate(&global_path_point, &mp->verts[j].pos, &objp->orient);
			vm_vec_add2(&global_path_point, &objp->pos);
			g3_rotate_vertex(&v, &global_path_point);
			color = 255 - j*50;
			if ( color < 50 ) 
				color = 100;
			gr_set_color(0, color, 0);

			if ( j == mp->nverts-1 ) {
				gr_set_color(255, 0, 0);
			}

			g3_draw_sphere( &v, 1.5f);

			if ( j > 0 )
				g3_draw_line(&v, &prev_vertex);

			prev_vertex = v;
		}
	}
}

void render_path_points(object *objp)
{
	ship		*shipp = &Ships[objp->instance];
	ai_info	*aip = &Ai_info[shipp->ai_index];
	object	*dobjp;
	polymodel	*pm;

	render_all_subsys_paths(objp);
	render_all_ship_bay_paths(objp);

	if (aip->goal_objnum < 0)
		return;

	dobjp = &Objects[aip->goal_objnum];
	pm = model_get(Ships[dobjp->instance].modelnum);
	vector	dock_point, global_dock_point;
	vertex	v;

	ship_model_start(&Objects[aip->goal_objnum]);
	if (pm->n_docks) {
		dock_point = pm->docking_bays[0].pnt[0];
		model_find_world_point(&global_dock_point, &dock_point, Ships[dobjp->instance].modelnum, 0, &dobjp->orient, &dobjp->pos );
		g3_rotate_vertex(&v, &global_dock_point);
		gr_set_color(255, 255, 255);
		g3_draw_sphere( &v, 1.5f);
	}

	if (aip->path_start != -1) {
		vertex		prev_vertex;
		pnode			*pp = &Path_points[aip->path_start];
		int			num_points = aip->path_length;
		int			i;

		for (i=0; i<num_points; i++) {
			vertex	v0;

			g3_rotate_vertex( &v0, &pp->pos );

			gr_set_color(0, 128, 96);
			if (i != 0)
				g3_draw_line(&v0, &prev_vertex);

			if (pp-Path_points == aip->path_cur)
				gr_set_color(255,255,0);
			
			g3_draw_sphere( &v0, 4.5f);

			//	Connect all the turrets that can fire upon this point to this point.
/*			if (0) { //pp->path_index != -1) {
				model_path	*pmp;
				mp_vert		*pmpv;

				get_base_path_info(pp->path_index, aip->goal_objnum, &pmp, &pmpv);

				if (pmpv->nturrets) {
					for (int j = 0; j<pmpv->nturrets; j++) {
						vertex	v1;
						vector	turret_pos;
						ship_subsys	*ssp;

						ssp = ship_get_indexed_subsys(&Ships[Objects[aip->goal_objnum].instance], pmpv->turret_ids[j]);

model_find_world_point(&turret_pos, &ssp->system_info->pnt, Ships[dobjp->instance].modelnum, 0, &dobjp->orient, &dobjp->pos );
	
						g3_rotate_vertex(&v1, &turret_pos);
						gr_set_color(255, 255, 0);
						g3_draw_line(&v0, &v1);
						g3_draw_sphere( &v1, 1.5f);
					}
				}
			} */

			prev_vertex = v0;

			pp++;
		}
	}

	ship_model_stop(&Objects[aip->goal_objnum]);
}

// Return the distance that the current AI weapon will travel
float ai_get_weapon_dist(ship_weapon *swp)
{
	int	bank_num, weapon_num;

	bank_num = swp->current_primary_bank;
	weapon_num = swp->primary_bank_weapons[bank_num];

	//	If weapon_num is illegal, return a reasonable value.  A valid weapon
	//	will get selected when this ship tries to fire.
	if (weapon_num == -1) {
		// Int3();
		return 1000.0f;
	}

	return Weapon_info[weapon_num].max_speed * Weapon_info[weapon_num].lifetime;
}

float ai_get_weapon_speed(ship_weapon *swp)
{
	int	bank_num, weapon_num;

	bank_num = swp->current_primary_bank;
	if (bank_num < 0)
		return 100.0f;

	weapon_num = swp->primary_bank_weapons[bank_num];

	if (weapon_num == -1) {
		//Int3();
		return 100.0f;
	}

	return Weapon_info[weapon_num].max_speed;
}

//	Compute the predicted position of a ship to be fired upon from a turret.
//	This is based on position of firing gun, enemy object, weapon speed and skill level constraints.
//	Return value in *predicted_enemy_pos.
//	Also, stuff globals G_predicted_pos, G_collision_time and G_fire_pos.
//	*pobjp		object firing the weapon
//	*eobjp		object being fired upon
void set_predicted_enemy_pos_turret(vector *predicted_enemy_pos, vector *gun_pos, object *pobjp, vector *enemy_pos, vector *enemy_vel, float weapon_speed, float time_enemy_in_range)
{
	ship	*shipp = &Ships[pobjp->instance];
	float	range_time;

	//weapon_speed = ai_get_weapon_speed(&shipp->weapons);

	if (weapon_speed < 1.0f)
		weapon_speed = 1.0f;

	range_time = 2.0f;

	//	Make it take longer for enemies to get player's allies in range based on skill level.
	if (Ships[pobjp->instance].team != Ships[Player_obj->instance].team)
		range_time += In_range_time[Game_skill_level];

	//nprintf(("AI", "time enemy in range = %7.3f\n", aip->time_enemy_in_range));

	if (time_enemy_in_range < range_time) {
		float	dist;

		dist = vm_vec_dist_quick(&pobjp->pos, enemy_pos);
		vm_vec_scale_add(predicted_enemy_pos, enemy_pos, enemy_vel, time_enemy_in_range * dist/weapon_speed);
	} else {
		float	collision_time, scale;
		vector	rand_vec;
		ai_info	*aip = &Ai_info[shipp->ai_index];

		collision_time = compute_collision_time(enemy_pos, enemy_vel, gun_pos, weapon_speed);

		if (collision_time == 0.0f){
			collision_time = 100.0f;
		}

		vm_vec_scale_add(predicted_enemy_pos, enemy_pos, enemy_vel, collision_time);
		if (time_enemy_in_range > 2*range_time){
			scale = (1.0f - aip->ai_accuracy) * 4.0f;
		} else {
			scale = (1.0f - aip->ai_accuracy) * 4.0f * (1.0f + 4.0f * (1.0f - time_enemy_in_range/(2*range_time)));
		}		

		static_randvec(((pobjp-Objects) ^ (Missiontime >> 16)) & 7, &rand_vec);

		vm_vec_scale_add2(predicted_enemy_pos, &rand_vec, scale);
		G_collision_time = collision_time;
		G_fire_pos = *gun_pos;
	}

	G_predicted_pos = *predicted_enemy_pos;
}

//	Compute the predicted position of a ship to be fired upon.
//	This is based on current position of firing object, enemy object, relative position of gun on firing object,
//	weapon speed and skill level constraints.
//	Return value in *predicted_enemy_pos.
//	Also, stuff globals G_predicted_pos, G_collision_time and G_fire_pos.
void set_predicted_enemy_pos(vector *predicted_enemy_pos, object *pobjp, object *eobjp, ai_info *aip)
{
	float	weapon_speed, range_time;
	ship	*shipp = &Ships[pobjp->instance];

	weapon_speed = ai_get_weapon_speed(&shipp->weapons);
	weapon_speed = max(weapon_speed, 1.0f);		// set not less than 1

	range_time = 2.0f;

	//	Make it take longer for enemies to get player's allies in range based on skill level.
	// but don't bias team v. team missions
	if ( !((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_TEAM)) ) {
		if (Ships[pobjp->instance].team != Ships[Player_obj->instance].team) {
			range_time += In_range_time[Game_skill_level];
		}
	}
	//nprintf(("AI", "time enemy in range = %7.3f\n", aip->time_enemy_in_range));

	if (aip->time_enemy_in_range < range_time) {
		float	dist;

		dist = vm_vec_dist_quick(&pobjp->pos, &eobjp->pos);
		vm_vec_scale_add(predicted_enemy_pos, &eobjp->pos, &eobjp->phys_info.vel, aip->time_enemy_in_range * dist/weapon_speed);
	} else {
		float	collision_time;
		vector	gun_pos, pnt;
		polymodel *po = model_get( Ship_info[shipp->ship_info_index].modelnum );

		//	Compute position of gun in absolute space and use that as fire position.
		if(po->gun_banks != NULL){
			pnt = po->gun_banks[0].pnt[0];
		} else {
			pnt = Objects[shipp->objnum].pos;
		}
		vm_vec_unrotate(&gun_pos, &pnt, &pobjp->orient);
		vm_vec_add2(&gun_pos, &pobjp->pos);

		collision_time = compute_collision_time(&eobjp->pos, &eobjp->phys_info.vel, &gun_pos, weapon_speed);

		if (collision_time == 0.0f) {
			collision_time = 100.0f;
		}

		vm_vec_scale_add(predicted_enemy_pos, &eobjp->pos, &eobjp->phys_info.vel, collision_time);

		// set globals
		G_collision_time = collision_time;
		G_fire_pos = gun_pos;
	}

	// Now add error terms (1) regular aim (2) EMP (3) stealth
	float scale = 0.0f;
	vector rand_vec;

	// regular skill level error in aim
	if (aip->time_enemy_in_range > 2*range_time) {
		scale = (1.0f - aip->ai_accuracy) * 4.0f;
	} else {
		scale = (1.0f - aip->ai_accuracy) * 4.0f * (1.0f + 4.0f * (1.0f - aip->time_enemy_in_range/(2*range_time)));
	}

	// if this ship is under the effect of an EMP blast, throw his aim off a bit
	if (shipp->emp_intensity > 0.0f) {
		// never go lower than 1/2 of the EMP effect max, otherwise things aren't noticeable
		scale += (MAX_EMP_INACCURACY * (shipp->emp_intensity < 0.5f ? 0.5f : shipp->emp_intensity));
		mprintf(("AI miss scale factor (EMP) %f\n",scale));
	}

	// if stealthy ship, throw his aim off, more when farther away and when dot is small
	if ( aip->ai_flags & AIF_STEALTH_PURSIUT ) {
		float dist = vm_vec_dist_quick(&pobjp->pos, &eobjp->pos);
		vector temp;
		vm_vec_sub(&temp, &eobjp->pos, &pobjp->pos);
		vm_vec_normalize_quick(&temp);
		float dot = vm_vec_dotprod(&temp, &pobjp->orient.v.fvec);
		float st_err = 3.0f * (1.4f - dot) * (1.0f + dist / (get_skill_stealth_dist_scaler() * STEALTH_MAX_VIEW_DIST)) * (1 - aip->ai_accuracy);
		scale += st_err;
		// mprintf(("error term: %.1f, total %.1f, dot %.3f\n", st_err, scale, dot));
	}

	// get a random vector that changes slowly over time (1x / sec)
	static_randvec(((pobjp-Objects) ^ (Missiontime >> 16)) & 7, &rand_vec);

	vm_vec_scale_add2(predicted_enemy_pos, &rand_vec, scale);

	// set global
	G_predicted_pos = *predicted_enemy_pos;
}

//	Handler of submode for Chase.  Go into a continuous turn for awhile.
void ai_chase_ct()
{
	vector		tvec;
	ship_info	*sip;
	ai_info		*aip;

	Assert(Ships[Pl_objp->instance].ship_info_index >= 0);
	sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];
	Assert(Ships[Pl_objp->instance].ai_index >= 0);
	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	//	Make a continuous turn towards any combination of possibly negated
	// up and right vectors.
	tvec = Pl_objp->pos;

	if (aip->submode_parm0 & 0x01)
		vm_vec_add2(&tvec, &Pl_objp->orient.v.rvec);
	if (aip->submode_parm0 & 0x02)
		vm_vec_sub2(&tvec, &Pl_objp->orient.v.rvec);
	if (aip->submode_parm0 & 0x04)
		vm_vec_add2(&tvec, &Pl_objp->orient.v.uvec);
	if (aip->submode_parm0 & 0x08)
		vm_vec_sub2(&tvec, &Pl_objp->orient.v.uvec);

	//	Detect degenerate cases that cause tvec to be same as player pos.
	if (vm_vec_dist_quick(&tvec, &Pl_objp->pos) < 0.1f) {
		aip->submode_parm0 &= 0x05;
		if (aip->submode_parm0 == 0)
			aip->submode_parm0 = 1;
		vm_vec_add2(&tvec, &Pl_objp->orient.v.rvec);
	}

	ai_turn_towards_vector(&tvec, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);
	accelerate_ship(aip, 1.0f);
}

//	ATTACK submode handler for chase mode.
void ai_chase_eb(ai_info *aip, ship_info *sip, vector *predicted_enemy_pos, float dist_to_enemy)
{
	vector	_pep;
	float		dot_to_enemy, dot_from_enemy;

	compute_dots(Pl_objp, En_objp, &dot_to_enemy, &dot_from_enemy);

	//	If we're trying to slow down to get behind, then point to turn towards is different.
	_pep = *predicted_enemy_pos;
	if ((dot_to_enemy > dot_from_enemy + 0.1f) || (dot_to_enemy > 0.9f))
		vm_vec_scale_add(&_pep, &Pl_objp->pos, &En_objp->orient.v.fvec, 100.0f);

	ai_turn_towards_vector(&_pep, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);

	accelerate_ship(aip, 0.0f);
}

//	Return time until weapon_objp might hit ship_objp.
//	Assumes ship_objp is not moving.
//	Returns negative time if not going to hit.
//	This is a very approximate function, but is pretty fast.
float ai_endangered_time(object *ship_objp, object *weapon_objp)
{
	float		to_dot, from_dot, dist;

	dist = compute_dots(ship_objp, weapon_objp, &to_dot, &from_dot);

	//	Note, this is bogus.  It assumes only the weapon is moving.
	//	Only proceed if weapon sort of pointing at object and object pointing towards or away from weapon
	//	(Ie, if object moving at right angle to weapon, just continue for now...)
	if (weapon_objp->phys_info.speed < 1.0f)
		return dist + 1.0f;
	else if ((from_dot > 0.1f) && (dist/(from_dot*from_dot) < 48*ship_objp->radius)) //: don't require them to see it, they have instruments!: && (fl_abs(to_dot) > 0.5f))
		return dist / weapon_objp->phys_info.speed;
	else
		return -1.0f;
}

//	Return time until danger weapon could hit this ai object.
//	Return negative time if not endangered.
float ai_endangered_by_weapon(ai_info *aip)
{
	object	*weapon_objp;

	if (aip->danger_weapon_objnum == -1) {
		return -1.0f;
	}

	weapon_objp = &Objects[aip->danger_weapon_objnum];

	if (weapon_objp->signature != aip->danger_weapon_signature) {
		aip->danger_weapon_objnum = -1;
		return -1.0f;
	}

	return ai_endangered_time(&Objects[Ships[aip->shipnum].objnum], weapon_objp);
}

//	Return true if this ship is near full strength.
int ai_near_full_strength(object *objp, ship_info *sip)
{
	return (objp->hull_strength/sip->initial_hull_strength > 0.9f) || (get_shield_strength(objp)/sip->shields > 0.8f);
}
				
//	Set acceleration while in attack mode.
void attack_set_accel(ai_info *aip, float dist_to_enemy, float dot_to_enemy, float dot_from_enemy)
{
	float	speed_ratio;

	if (En_objp->phys_info.speed > 1.0f)
		speed_ratio = Pl_objp->phys_info.speed/En_objp->phys_info.speed;
	else
		speed_ratio = 5.0f;

	//	Sometimes, told to attack slowly.  Allows to get in more hits.
	if (aip->ai_flags & AIF_ATTACK_SLOWLY) {
		if ((dist_to_enemy > 200.0f) && (dist_to_enemy < 800.0f)) {
			if ((dot_from_enemy < 0.9f) || ai_near_full_strength(Pl_objp, &Ship_info[Ships[Pl_objp->instance].ship_info_index])) {
				//nprintf(("AI", " slowly "));
				accelerate_ship(aip, max(1.0f - (dist_to_enemy-200.0f)/600.0f, 0.1f));
				return;
			}
		} else
			aip->ai_flags &= ~AIF_ATTACK_SLOWLY;
	}

	if (dist_to_enemy > 200.0f + vm_vec_mag_quick(&En_objp->phys_info.vel) * dot_from_enemy + Pl_objp->phys_info.speed * speed_ratio) {
		//nprintf(("AI", "1"));
		if (ai_maybe_fire_afterburner(Pl_objp, aip)) {
			if (dist_to_enemy > 800.0f) {
				if (!( Pl_objp->phys_info.flags & PF_AFTERBURNER_ON )) {
					float percent_left;
					ship	*shipp;
					ship_info *sip;

					shipp = &Ships[Pl_objp->instance];
					sip = &Ship_info[shipp->ship_info_index];

					if (sip->afterburner_fuel_capacity > 0.0f) {
						percent_left = 100.0f * shipp->afterburner_fuel / sip->afterburner_fuel_capacity;
						if (percent_left > 30.0f + ((Pl_objp-Objects) & 0x0f)) {
							afterburners_start(Pl_objp);
							aip->afterburner_stop_time = Missiontime + F1_0 + static_rand(Pl_objp-Objects)/4;
						}
					}
				}
			}
		}

		accelerate_ship(aip, 1.0f);
	} else if ((Missiontime - aip->last_hit_time > F1_0*7)
		&& (En_objp->phys_info.speed < 10.0f) 
		&& (dist_to_enemy > 25.0f) 
		&& (dot_to_enemy > 0.8f)
		&& (dot_from_enemy < 0.8f)) {
		accelerate_ship(aip, 0.0f);		//	No one attacking us, so don't need to move.
	} else if ((dot_from_enemy < 0.25f) && (dot_to_enemy > 0.5f)) {
		set_accel_for_target_speed(Pl_objp, En_objp->phys_info.speed);
	} else if (Pl_objp->phys_info.speed < 15.0f) {
		accelerate_ship(aip, 1.0f);
	} else if (Pl_objp->phys_info.speed > En_objp->phys_info.speed - 1.0f) {
		if (dot_from_enemy > 0.75f)
			accelerate_ship(aip, 1.0f);
		else
			set_accel_for_target_speed(Pl_objp, En_objp->phys_info.speed*0.75f + 3.0f);
	} else {
		change_acceleration(aip, 0.5f);
	}
}

//	Pl_objp (aip) tries to get behind En_objp.
//	New on 2/21/98: If this ship can move backwards and slide, maybe do that to get behind.
void get_behind_ship(ai_info *aip, ship_info *sip, float dist_to_enemy)
{
	vector	new_pos;
	float		dot;
	vector	vec_from_enemy;
	float		dist;

	dist = vm_vec_normalized_dir(&vec_from_enemy, &Pl_objp->pos, &En_objp->pos);

	vm_vec_scale_add(&new_pos, &En_objp->pos, &En_objp->orient.v.fvec, -100.0f);		//	Pick point 100 units behind.
	ai_turn_towards_vector(&new_pos, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);

	dot = vm_vec_dot(&vec_from_enemy, &En_objp->orient.v.fvec);

	if (dot > 0.25f) {
		accelerate_ship(aip, 1.0f);
	} else {
		accelerate_ship(aip, (dot + 1.0f)/2.0f);
	}
}

int avoid_player(object *objp, vector *goal_pos)
{
	maybe_avoid_player(Pl_objp, goal_pos);
	ai_info	*aip = &Ai_info[Ships[objp->instance].ai_index];

	if (aip->ai_flags & AIF_AVOIDING_SMALL_SHIP) {
		ship_info *sip = &Ship_info[Ships[objp->instance].ship_info_index];

		if (aip->ai_flags & AIF_AVOIDING_SMALL_SHIP) {
			ai_turn_towards_vector(&aip->avoid_goal_point, objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);
			accelerate_ship(aip, 0.5f);
			return 1;
		}
	}

	return 0;
}

//	Determine if a cylinder of width radius from p0 to p1 will collide with big_objp.
//	If so, stuff *collision_point.
int will_collide_pp(vector *p0, vector *p1, float radius, object *big_objp, vector *collision_point)
{
	mc_info	mc;

	mc.model_num = Ships[big_objp->instance].modelnum;		// Fill in the model to check
	mc.orient = &big_objp->orient;			// The object's orient
	mc.pos = &big_objp->pos;					// The object's position
	mc.p0 = p0;										// Point 1 of ray to check
	mc.p1 = p1;
	mc.flags = MC_CHECK_MODEL | MC_CHECK_SPHERELINE | MC_SUBMODEL;					// flags

	mc.radius = radius;

	// Only check the 2nd lowest hull object
	polymodel *pm = model_get(Ships[big_objp->instance].modelnum);
	mc.submodel_num = pm->detail[0]; //pm->submodel->num_details-2];
	model_collide(&mc);

	if (mc.num_hits)
		*collision_point = mc.hit_point_world;

	return mc.num_hits;
}

//	Return true/false if *objp will collide with *big_objp
//	Stuff distance in *distance to collision point if *objp will collide with *big_objp within delta_time seconds.
//	Global collision point stuffed in *collision_point
int will_collide_with_big_ship(object *objp, vector *goal_point, object *big_objp, vector *collision_point, float delta_time)
{
	float		radius;
	vector	end_pos;

	radius = big_objp->radius + delta_time * objp->phys_info.speed;

	if (vm_vec_dist_quick(&big_objp->pos, &objp->pos) > radius) {
		return 0;
	}

	if (goal_point == NULL) {
		vm_vec_scale_add(&end_pos, &objp->pos, &objp->phys_info.vel, delta_time);					// Point 2 of ray to check
	} else {
		end_pos = *goal_point;
	}

	return will_collide_pp(&objp->pos, &end_pos, objp->radius, big_objp, collision_point);
}

//	Return true if *objp is expected to collide with a large ship.
//	Stuff global collision point in *collision_point.
//	If *goal_point is not NULL, use that as the point towards which *objp will be flying.  Don't use *objp velocity
//	*ignore_objp will typically be the target this ship is pursuing, either to attack or guard.  We don't want to avoid it.
int will_collide_with_big_ship_all(object *objp, object *ignore_objp, vector *goal_point, vector *collision_point, float *distance, float delta_time)
{
	ship_obj	*so;
	object	*big_objp;
	int		collision_obj_index = -1;
	float		min_dist = 999999.9f;
	float		collision_time = -1.0f;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		float	time = 0.0f;
		big_objp = &Objects[so->objnum];

		if (big_objp == ignore_objp)
			continue;

		if (Ship_info[Ships[big_objp->instance].ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) {
			vector	cur_collision_point;
			float		cur_dist;

			if (will_collide_with_big_ship(objp, goal_point, big_objp, &cur_collision_point, delta_time)) {

				cur_dist = vm_vec_dist(&cur_collision_point, &objp->pos);

				if (cur_dist < min_dist) {
					min_dist = cur_dist;
					*collision_point = cur_collision_point;
					collision_time = time;
					collision_obj_index = OBJ_INDEX(big_objp);
				}
			}
		}
	}

	*distance = min_dist;
	return collision_obj_index;

}

typedef struct {
	float		dist;
	int		collide;
	vector	pos;
} sgoal;

//int will_collide_pp(vector *p0, vector *p1, float radius, object *big_objp, vector *collision_point)
//	Pick a point for *objp to fly towards to avoid a collision with *big_objp at *collision_point
//	Return result in *avoid_pos
void mabs_pick_goal_point(object *objp, object *big_objp, vector *collision_point, vector *avoid_pos)
{
	matrix	mat1;
	sgoal		goals[4];
	vector	v2b;

	vm_vec_normalized_dir(&v2b, collision_point, &objp->pos);
	vm_vector_2_matrix(&mat1, &v2b, NULL, NULL);

	int	found = 0;

	//	Try various scales, in 0.5f, 0.75f, 1.0f, 1.25f.
	//	First try 0.5f to see if we can find a point that near the center of the target ship, which presumably
	//	means less of a turn.
	//	Try going as far as 1.25f * radius.
	float	s;
	for (s=0.5f; s<1.3f; s += 0.25f) {
		int	i;
		for (i=0; i<4; i++) {
			vector p = big_objp->pos;
			float ku = big_objp->radius*s + objp->radius * (OBJ_INDEX(objp) % 4)/4;		//	This objp->radius stuff to prevent ships from glomming together at one point
			float kr = big_objp->radius*s + objp->radius * ((OBJ_INDEX(objp) % 4) ^ 2)/4;
			if (i&1)
				ku = -ku;
			if (i&2)
				kr = -kr;
			vm_vec_scale_add2(&p, &mat1.v.uvec, ku);
			vm_vec_scale_add2(&p, &mat1.v.rvec, kr);
			goals[i].pos = p;
			goals[i].dist = vm_vec_dist_quick(&objp->pos, &p);
			goals[i].collide = will_collide_pp(&objp->pos, &p, objp->radius, big_objp, collision_point);
			if (!goals[i].collide)
				found = 1;
		}

		//	If we found a point that doesn't collide, find the nearest one and make that the *avoid_pos.
		if (found) {
			float	min_dist = 9999999.9f;
			int	min_index = -1;

			for (i=0; i<4; i++) {
				if (!goals[i].collide && (goals[i].dist < min_dist)) {
					min_dist = goals[i].dist;
					min_index = i;
				}
			}

			Assert(i != -1);
			if (i != -1) {
				*avoid_pos = goals[min_index].pos;
				return;
			}
		}
	}

	//	Drat.  We tried and tried and could not find a point that did not cause a collision.
	//	Get this dump pilot far away from the problem ship.
	vector	away_vec;
	vm_vec_normalized_dir(&away_vec, &objp->pos, collision_point);
	vm_vec_scale_add(avoid_pos, &objp->pos, &away_vec, big_objp->radius*1.5f);

}

//	Return true if a large ship is being ignored.
int maybe_avoid_big_ship(object *objp, object *ignore_objp, ai_info *aip, vector *goal_point, float delta_time)
{
	if (timestamp_elapsed(aip->avoid_check_timestamp)) {
		float		distance;
		vector	collision_point;
		int		ship_num;
		if ((ship_num = will_collide_with_big_ship_all(Pl_objp, ignore_objp, goal_point, &collision_point, &distance, delta_time)) != -1) {
			aip->ai_flags |= AIF_AVOIDING_BIG_SHIP;
			mabs_pick_goal_point(objp, &Objects[ship_num], &collision_point, &aip->avoid_goal_point);
			float dist = vm_vec_dist_quick(&aip->avoid_goal_point, &objp->pos);
			aip->avoid_check_timestamp = timestamp(2000 + min(1000, (int) (dist * 2.0f)));	//	Delay until check again is based on distance to avoid point.
			aip->avoid_ship_num = ship_num;
		} else {
			aip->ai_flags &= ~AIF_AVOIDING_BIG_SHIP;
			aip->ai_flags &= ~AIF_AVOIDING_SMALL_SHIP;
			aip->avoid_ship_num = -1;
			aip->avoid_check_timestamp = timestamp(1500);
		}
	}
	
	if (aip->ai_flags & AIF_AVOIDING_BIG_SHIP) {
		ship_info *sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];

		vector	v2g;

		ai_turn_towards_vector(&aip->avoid_goal_point, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);
		vm_vec_normalized_dir(&v2g, &aip->avoid_goal_point, &Pl_objp->pos);
		float dot = vm_vec_dot(&objp->orient.v.fvec, &v2g);
		float d2 = (1.0f + dot) * (1.0f + dot);
		accelerate_ship(aip, d2/4.0f);
		return 1;
	}

	return 0;
}

//	Set desired right vector for ships flying towards another ship.
//	Since this is governed only by vector to target, it causes ships to align bank and look less chaotic.
void compute_desired_rvec(vector *rvec, vector *goal_pos, vector *cur_pos)
{
	vector	v2e;

	vm_vec_normalized_dir(&v2e, goal_pos, cur_pos);
	rvec->xyz.x = v2e.xyz.z;
	rvec->xyz.y = 0.0f;
	rvec->xyz.z = -v2e.xyz.x;
	if (vm_vec_mag_squared(rvec) < 0.001f)
		rvec->xyz.y = 1.0f;
}

// Handler for stealth find submode of Chase.
void ai_stealth_find()
{
	ai_info		*aip;
	ship_info	*sip;

	vector new_pos, vec_to_enemy;
	float dist_to_enemy, dot_to_enemy, dot_from_enemy;

	Assert(Ships[Pl_objp->instance].ship_info_index >= 0);
	sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];
	Assert(Ships[Pl_objp->instance].ai_index >= 0);
	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	// get time since last seen
	int delta_time = (timestamp() - aip->stealth_last_visible_stamp);

	// if delta_time is really big, i'm real confused, start sweep
	if (delta_time > 10000) {
		aip->submode_parm0 = SM_SF_BAIL;
	}

	// guestimate new position
	vm_vec_scale_add(&new_pos, &aip->stealth_last_pos, &aip->stealth_velocity, (delta_time * 0.001f));

	// if I think he's behind me, go to the goal point
	if ( aip->submode_parm0 == SM_SF_BEHIND ) {
		new_pos = aip->goal_point;
	}

	// check for collision with big ships
	if (maybe_avoid_big_ship(Pl_objp, En_objp, aip, &new_pos, 10.0f)) {
		// reset ai submode to chase
		return;
	}

	// if dist is near max and dot is close to 1, accel, afterburn
	vm_vec_sub(&vec_to_enemy, &new_pos, &Pl_objp->pos);
	dist_to_enemy = vm_vec_normalize_quick(&vec_to_enemy);
	dot_to_enemy = vm_vec_dotprod(&vec_to_enemy, &Pl_objp->orient.v.fvec);

	// if i think i should see him ahead and i don't, set goal pos and turn around, but only if I haven't seen him for a while
	if ( (delta_time > 800) && (aip->submode_parm0 == SM_SF_AHEAD) && (dot_to_enemy > .94) && (dist_to_enemy < get_skill_stealth_dist_scaler()*STEALTH_MAX_VIEW_DIST + 50) ) {
		// do turn around)
		vm_vec_scale_add(&aip->goal_point, &Pl_objp->pos, &Pl_objp->orient.v.fvec, -300.0f);
		aip->submode_parm0 = SM_SF_BEHIND;
		vm_vec_sub(&vec_to_enemy, &new_pos, &Pl_objp->pos);
		dist_to_enemy = vm_vec_normalize_quick(&vec_to_enemy);
		dot_to_enemy = vm_vec_dotprod(&vec_to_enemy, &Pl_objp->orient.v.fvec);
	}

	if ( (dist_to_enemy > get_skill_stealth_dist_scaler()*STEALTH_MAX_VIEW_DIST) && (dot_to_enemy > 0.94f) ) {		// 20 degree half angle
		// accelerate ship
		accelerate_ship(aip, 1.0f);

		// engage afterburner
		if (!( Pl_objp->phys_info.flags & PF_AFTERBURNER_ON )) {
			if (ai_maybe_fire_afterburner(Pl_objp, aip)) {
				afterburners_start(Pl_objp);
				aip->afterburner_stop_time = Missiontime + 3*F1_0/2;
			}
		}

		ai_turn_towards_vector(&new_pos, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);
		return;
	}

	//	If enemy more than 500 meters away, all ships flying there will tend to match bank.
	//	They do this by using their vector to their target to compute their right vector and causing ai_turn_towards_vector
	//	to interpolate a matrix rather than just a vector.
	if (dist_to_enemy > 500.0f) {
		vector	rvec;
		compute_desired_rvec(&rvec, &new_pos, &Pl_objp->pos);
		ai_turn_towards_vector(&new_pos, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0, &rvec);
	} else {
		ai_turn_towards_vector(&new_pos, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);
	}

	dot_from_enemy = -vm_vec_dotprod(&vec_to_enemy, &En_objp->orient.v.fvec);

	attack_set_accel(aip, dist_to_enemy, dot_to_enemy, dot_from_enemy);
}

// -----------------------------------------------------------------------------
// try to find stealth ship by sweeping an area
void ai_stealth_sweep()
{
	ai_info		*aip;
	ship_info	*sip;

	Assert(Ships[Pl_objp->instance].ship_info_index >= 0);
	sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];
	Assert(Ships[Pl_objp->instance].ai_index >= 0);
	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	vector goal_pt;
	vector forward, right, up;
	int lost_time;

	// time since stealth last seen
	lost_time = (timestamp() - aip->stealth_last_visible_stamp);

	// determine which pt to fly to in sweep by keeping track of parm0
	if (aip->submode_parm0 == SM_SS_SET_GOAL) {

		// don't make goal pt more than 2k from current pos
		vm_vec_scale_add(&goal_pt, &aip->stealth_last_pos, &aip->stealth_velocity, (0.001f * lost_time));

		// make box size based on speed of stealth and expected time to intercept (keep box in range 200-500)
		float box_size = vm_vec_mag_quick(&aip->stealth_velocity) * (0.001f * lost_time);
		box_size = min(200.0f, box_size);
		box_size = max(500.0f, box_size);
		aip->stealth_sweep_box_size = box_size;

		aip->goal_point = goal_pt;
		aip->submode_parm0 = SM_SS_BOX0;
	}

	// GET UP, RIGHT, FORWARD FOR BOX based on stealth ship's velocity
	// if velocity changes in stealth mode, then ship is *seen*, and falls out of sweep mode
	// if stealth has no velocity make a velocity
	if ( vm_vec_mag_quick(&aip->stealth_velocity) < 1 ) {
		vm_vec_rand_vec_quick(&aip->stealth_velocity);
	}

	// get "right" vector for box
	vm_vec_crossprod(&right, &aip->stealth_velocity, &vmd_y_vector);

	if ( vm_vec_mag_quick(&right) < 0.01 ) {
		vm_vec_crossprod(&right, &aip->stealth_velocity, &vmd_z_vector);
	}

	vm_vec_normalize_quick(&right);

	// get forward for box
	vm_vec_copy_normalize_quick(&forward, &aip->stealth_velocity);

	// get "up" for box
	vm_vec_crossprod(&up, &forward, &right);
	
	// lost far away ahead (do box)
	switch(aip->submode_parm0) {
	case SM_SS_BOX0:
		goal_pt = aip->goal_point;
		break;

	// pt1 -U +R
	case SM_SS_LR:
		vm_vec_scale_add(&goal_pt, &aip->goal_point, &up, -aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &right, aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &forward, 0.5f*aip->stealth_sweep_box_size);
		break;

	// pt2 +U -R
	case SM_SS_UL:
		vm_vec_scale_add(&goal_pt, &aip->goal_point, &up, aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &right, -aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &forward, 0.5f*aip->stealth_sweep_box_size);
		break;

	// pt3 back
	case SM_SS_BOX1:
		goal_pt = aip->goal_point;
		break;

	// pt4 +U +R
	case SM_SS_UR:
		vm_vec_scale_add(&goal_pt, &aip->goal_point, &up, aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &right, aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &forward, 0.5f*aip->stealth_sweep_box_size);
		break;

	// pt5 -U -R
	case SM_SS_LL:
		vm_vec_scale_add(&goal_pt, &aip->goal_point, &up, -aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &right, -aip->stealth_sweep_box_size);
		vm_vec_scale_add2(&goal_pt, &forward, 0.5f*aip->stealth_sweep_box_size);
		break;

	// pt6 back
	case SM_SS_BOX2:
		goal_pt = aip->goal_point;
		break;

	default:
		Int3();

	}

	// when close to goal_pt, update next goal pt
	float dist_to_goal = vm_vec_dist(&goal_pt, &Pl_objp->pos);
	if (dist_to_goal < 15) {
		aip->submode_parm0++;
	}

	// check for collision with big ship
	if (maybe_avoid_big_ship(Pl_objp, En_objp, aip, &goal_pt, 10.0f)) {
		// skip to the next pt on box
		aip->submode_parm0++;
		return;
	}

	ai_turn_towards_vector(&goal_pt, Pl_objp, flFrametime, sip->srotation_time, NULL, NULL, 0.0f, 0);

	float dot = 1.0f;
	if (dist_to_goal < 100) {
		vector vec_to_goal;
		vm_vec_normalized_dir(&vec_to_goal, &goal_pt, &Pl_objp->pos);
		dot = vm_vec_dotprod(&vec_to_goal, &Pl_objp->orient.v.fvec);
	}

	accelerate_ship(aip, 0.8f*dot);
}

//	ATTACK submode handler for chase mode.
void ai_chase_attack(ai_info *aip, ship_info *sip, vector *predicted_enemy_pos, float dist_to_enemy)
{
	int		start_bank;
	float		dot_to_enemy, dot_from_enemy; //, time_to_hit;
	float		bank_override = 0.0f;

	if (avoid_player(Pl_objp, predicted_enemy_pos))
		return;

	compute_dots(Pl_objp, En_objp, &dot_to_enemy, &dot_from_enemy);

	polymodel *po = model_get( sip->modelnum );

	vector	*rel_pos;
	float		scale;
	vector	randvec;
	vector	new_pos;

	start_bank = Ships[aip->shipnum].weapons.current_primary_bank;
	if (po->n_guns && start_bank != -1 ) {
		rel_pos = &po->gun_banks[start_bank].pnt[0];
	} else
		rel_pos = NULL;

	//	If ship moving slowly relative to its size, then don't attack its center point.
	//	How far from center we attack is based on speed, size and distance to enemy
	if (En_objp->radius > En_objp->phys_info.speed) {
		static_randvec(Pl_objp-Objects, &randvec);
		scale = dist_to_enemy/(dist_to_enemy + En_objp->radius) * En_objp->radius;
		scale *= 0.5f * En_objp->radius/(En_objp->phys_info.speed + En_objp->radius);	// scale downward by 1/2 to 1/4
		vm_vec_scale_add(&new_pos, predicted_enemy_pos, &randvec, scale);
	} else
		new_pos = *predicted_enemy_pos;

	if (dist_to_enemy < 250.0f) {
		if (dot_from_enemy > 0.7f) {
			bank_override = Pl_objp->phys_info.speed;
		}
	}

	//	If enemy more than 500 meters away, all ships flying there will tend to match bank.
	//	They do this by using their vector to their target to compute their right vector and causing ai_turn_towards_vector
	//	to interpolate a matrix rather than just a vector.
	if (dist_to_enemy > 500.0f) {
		vector	rvec;
		compute_desired_rvec(&rvec, predicted_enemy_pos, &Pl_objp->pos);
		ai_turn_towards_vector(&new_pos, Pl_objp, flFrametime, sip->srotation_time, NULL, rel_pos, bank_override, 0, &rvec);
	} else {
		ai_turn_towards_vector(&new_pos, Pl_objp, flFrametime, sip->srotation_time, NULL, rel_pos, bank_override, 0);
	}

	attack_set_accel(aip, dist_to_enemy, dot_to_enemy, dot_from_enemy);
}

//	EVADE_SQUIGGLE submode handler for chase mode.
//	Changed by MK on 5/5/97.
//	Used to evade towards a point off the right or up vector.
//	Now, evade straight away to try to get far away.
//	The squiggling should protect against laser fire.
void ai_chase_es(ai_info *aip, ship_info *sip)
{
	vector	tvec;
	fix		timeslice;
	fix		scale;
	float		bank_override = 0.0f;

	tvec = Pl_objp->pos;

	timeslice = (Missiontime >> 16) & 0x0f;
	scale = ((Missiontime >> 16) & 0x0f) << 14;

	if (timeslice & 0x01)
		vm_vec_scale_add2(&tvec, &Pl_objp->orient.v.rvec, f2fl(scale ^ 0x10000));
	if (timeslice & 0x02)
		vm_vec_scale_sub2(&tvec, &Pl_objp->orient.v.rvec, f2fl(scale));
	if (timeslice & 0x04)
		vm_vec_scale_add2(&tvec, &Pl_objp->orient.v.uvec, f2fl(scale ^ 0x10000));
	if (timeslice & 0x08)
		vm_vec_scale_sub2(&tvec, &Pl_objp->orient.v.uvec, f2fl(scale));

	while (vm_vec_dist_quick(&tvec, &Pl_objp->pos) < 0.1f) {
		tvec.xyz.x += frand();
		tvec.xyz.y += frand();
	}

	bank_override = Pl_objp->phys_info.speed;

	ai_turn_towards_vector(&tvec, Pl_objp, flFrametime/2, sip->srotation_time, NULL, NULL, bank_override, 0);
	accelerate_ship(aip, 1.0f);
}

//	Trying to get away from opponent.
void ai_chase_ga(ai_info *aip, ship_info *sip)
{
	//	If not near end of this submode, evade squiggly.  If near end, just fly straight for a bit
	vector	tvec;
	float		bank_override;
	vector	vec_from_enemy;

	if (En_objp != NULL) {
		vm_vec_normalized_dir(&vec_from_enemy, &Pl_objp->pos, &En_objp->pos);
	} else
		vec_from_enemy = Pl_objp->orient.v.fvec;

	static_randvec(Missiontime >> 15, &tvec);
	vm_vec_scale(&tvec, 100.0f);
	vm_vec_scale_add2(&tvec, &vec_from_enemy, 300.0f);
	vm_vec_add2(&tvec, &Pl_objp->pos);

	bank_override = Pl_objp->phys_info.speed;

	ai_turn_towards_vector(&tvec, Pl_objp, flFrametime/2, sip->srotation_time, NULL, NULL, bank_override, 0);

	accelerate_ship(aip, 2.0f);

	if (ai_maybe_fire_afterburner(Pl_objp, aip)) {
		if (!(Pl_objp->phys_info.flags & PF_AFTERBURNER_ON )) {
			float percent_left = 100.0f * Ships[Pl_objp->instance].afterburner_fuel / sip->afterburner_fuel_capacity;
			if (percent_left > 30.0f + ((Pl_objp-Objects) & 0x0f)) {
				afterburners_start(Pl_objp);
				aip->afterburner_stop_time = Missiontime + 3*F1_0/2;
			}
			afterburners_start(Pl_objp);
			aip->afterburner_stop_time = Missiontime + 3*F1_0/2;
		}
	}

}

//	Make object *objp attack subsystem with ID = subnum.
//	Return true if found a subsystem to attack, else return false.
//	Note, can fail if subsystem exists, but has no hits.
int ai_set_attack_subsystem(object *objp, int subnum)
{
	ship			*shipp, *attacker_shipp;
	ai_info		*aip;
	ship_subsys	*ssp;
	object		*attacked_objp;

	Assert(objp->type == OBJ_SHIP);
	Assert(objp->instance >= 0);

	attacker_shipp = &Ships[objp->instance];
	Assert(attacker_shipp->ai_index >= 0);

	aip = &Ai_info[attacker_shipp->ai_index];

	// MWA -- 2/27/98.  Due to AL's changes, target_objnum is now not always valid (at least sometimes
	// in terms of goals).  So, bail if we don't have a valid target.
	if ( aip->target_objnum == -1 )
		return 0;

	attacked_objp = &Objects[aip->target_objnum];
	shipp = &Ships[attacked_objp->instance];		//  need to get our target's ship pointer!!!

	ssp = ship_get_indexed_subsys(shipp, subnum, &objp->pos);
	if (ssp == NULL)
		return 0;

	set_targeted_subsys(aip, ssp, aip->target_objnum);
	
	if (aip->ignore_objnum == aip->target_objnum)
		aip->ignore_objnum = UNUSED_OBJNUM;

	// -- Done at caller in ai_process_mission_orders -- attacked_objp->flags |= OF_PROTECTED;

	ai_set_goal_maybe_abort_dock(objp, aip);
	aip->ok_to_target_timestamp = timestamp(DELAY_TARGET_TIME);

	return 1;
}

void ai_set_guard_vec(object *objp, object *guard_objp)
{
	ai_info *aip;
	float	radius;

	aip = &Ai_info[Ships[objp->instance].ai_index];

	//	Handle case of bogus call in which ship is told to guard self.
	Assert(objp != guard_objp);
	if (objp == guard_objp) {
		vm_vec_rand_vec_quick(&aip->guard_vec);
		vm_vec_scale(&aip->guard_vec, 100.0f);
		return;
	}

	// check if guard_objp is BIG
	radius = 5.0f * (objp->radius + guard_objp->radius) + 50.0f;
	if (radius > 300.0f) {
		radius = guard_objp->radius * 1.25f;
	}

	vm_vec_sub(&aip->guard_vec, &objp->pos, &guard_objp->pos);

	if (vm_vec_mag(&aip->guard_vec) > 3.0f*radius) {
		//	Far away, don't just use vector to object, causes clustering of guard ships.
		vector	tvec, rvec;
		float	mag;
		mag = vm_vec_copy_normalize(&tvec, &aip->guard_vec);
		vm_vec_rand_vec_quick(&rvec);			
		vm_vec_scale_add2(&tvec, &rvec, 0.5f);
		vm_vec_copy_scale(&aip->guard_vec, &tvec, mag);
	}

	vm_vec_normalize_quick(&aip->guard_vec);
	vm_vec_scale(&aip->guard_vec, radius);
}

//	Make object *objp guard object *other_objp.
//	To be called from the goals code.
void ai_set_guard_wing(object *objp, int wingnum)
{
	ship		*shipp;
	ai_info	*aip;
	int		leader_objnum, leader_shipnum;

	Assert(wingnum >= 0);

	Assert(objp->type == OBJ_SHIP);
	Assert(objp->instance >= 0);

	// shouldn't set the ai mode for the player
	if ( objp == Player_obj ) {
		return;
	}

	shipp = &Ships[objp->instance];

	Assert(shipp->ai_index >= 0);

	aip = &Ai_info[shipp->ai_index];
	force_avoid_player_check(objp, aip);

	ai_set_goal_maybe_abort_dock(objp, aip);
	aip->ok_to_target_timestamp = timestamp(DELAY_TARGET_TIME);

	//	This function is called whenever a guarded ship is destroyed, so this code
	//	prevents a ship from trying to guard a non-existent wing.
	if (Wings[wingnum].current_count < 1) {
		aip->guard_objnum = -1;
		aip->guard_wingnum = -1;
		aip->mode = AIM_NONE;
	} else {
		leader_shipnum = Wings[wingnum].ship_index[0];
		leader_objnum = Ships[leader_shipnum].objnum;

		Assert((leader_objnum >= 0) && (leader_objnum < MAX_OBJECTS));
		//Assert(leader_objnum != objp-Objects);	//	Don't allow ships to guard themselves.
		if (leader_objnum == OBJ_INDEX(objp)) {
			//Int3();	//	Seems illegal, but let's clean up.  Get MikeK.
			return;
		}

		aip->guard_wingnum = wingnum;
		aip->guard_objnum = leader_objnum;
		aip->guard_signature = Objects[leader_objnum].signature;
		aip->mode = AIM_GUARD;
		aip->submode = AIS_GUARD_STATIC;

		ai_set_guard_vec(objp, &Objects[leader_objnum]);
	}
}

//	Make object *objp guard object *other_objp.
//	To be called from the goals code.
void ai_set_evade_object(object *objp, object *other_objp)
{
	ship		*shipp;
	ai_info	*aip;
	int		other_objnum;

	Assert(objp->type == OBJ_SHIP);
	Assert(objp->instance >= 0);

	shipp = &Ships[objp->instance];

	Assert(shipp->ai_index >= 0);

	aip = &Ai_info[shipp->ai_index];

	other_objnum = OBJ_INDEX(other_objp);
	Assert(other_objnum >= 0);

	Assert(other_objnum != Ships[aip->shipnum].objnum);	//	make sure not targeting self
	aip->target_objnum = other_objnum;

	aip->mode = AIM_EVADE;
}

//	Make objp guard other_objp
//	If other_objp is a member of a wing, objp will guard that whole wing
//	UNLESS objp is also a member of the wing!
void ai_set_guard_object(object *objp, object *other_objp)
{
	ship		*shipp;
	ai_info	*aip;
	int		other_objnum;

	Assert(objp->type == OBJ_SHIP);
	Assert(objp->instance >= 0);
	Assert(objp != other_objp);

	shipp = &Ships[objp->instance];

	Assert(shipp->ai_index >= 0);

	aip = &Ai_info[shipp->ai_index];
	aip->avoid_check_timestamp = timestamp(1);

	//	If ship to guard is in a wing, guard that whole wing.
	ai_info	*other_aip = &Ai_info[Ships[other_objp->instance].ai_index];
	if ((other_aip->wing != -1) && (other_aip->wing != aip->wing)) {
		ai_set_guard_wing(objp, Ai_info[Ships[other_objp->instance].ai_index].wing);
	} else {

		other_objnum = other_objp-Objects;

		aip->guard_objnum = other_objnum;
		aip->guard_signature = other_objp->signature;
		aip->guard_wingnum = -1;

		aip->mode = AIM_GUARD;
		aip->submode = AIS_GUARD_STATIC;

		Assert(other_objnum >= 0);	//	Hmm, bogus object and we need its position for guard_vec.

		// vm_vec_sub(&aip->guard_vec, &objp->pos, &Objects[other_objnum].pos);
		ai_set_guard_vec(objp, &Objects[other_objnum]);

		ai_set_goal_maybe_abort_dock(objp, aip);
		aip->ok_to_target_timestamp = timestamp(DELAY_TARGET_TIME);
	}
}

//	Update the aspect_locked_time field based on whether enemy is in view cone.
//	Also set/clear AIF_SEEK_LOCK.
void update_aspect_lock_information(ai_info *aip, vector *vec_to_enemy, float dist_to_enemy, float enemy_radius)
{
	float	dot_to_enemy;
	int	num_weapon_types;
	int	weapon_id_list[MAX_WEAPON_TYPES], weapon_bank_list[MAX_WEAPON_TYPES];
	ship	*shipp;
	ship_weapon	*swp;
	weapon_info	*wip;

	shipp = &Ships[aip->shipnum];
	swp = &shipp->weapons;

	// AL 3-7-98: This probably should never happen, but check to ensure that current_secondary_bank is valid
	if ( (swp->current_secondary_bank < 0) || (swp->current_secondary_bank > swp->num_secondary_banks) ) {
		return;
	}

	num_weapon_types = get_available_secondary_weapons(Pl_objp, weapon_id_list, weapon_bank_list);

	wip = &Weapon_info[swp->secondary_bank_weapons[swp->current_secondary_bank]];

	if (num_weapon_types && (wip->wi_flags & WIF_HOMING_ASPECT)) {
		if (dist_to_enemy > 300.0f - min(enemy_radius, 100.0f))
			aip->ai_flags |= AIF_SEEK_LOCK;
		else
			aip->ai_flags &= ~AIF_SEEK_LOCK;

		//	Update locking information for aspect seeking missiles.
		aip->current_target_is_locked = 0;
		dot_to_enemy = vm_vec_dot(vec_to_enemy, &Pl_objp->orient.v.fvec);

		float	needed_dot = 0.9f - 0.5f * enemy_radius/(dist_to_enemy + enemy_radius);	//	Replaced MIN_TRACKABLE_DOT with 0.9f
		if (dot_to_enemy > needed_dot) {
			aip->aspect_locked_time += flFrametime;
			// nprintf(("AI", "+ Lock time = %7.3f\n", aip->aspect_locked_time));
			if (aip->aspect_locked_time >= wip->min_lock_time) {
				aip->aspect_locked_time = wip->min_lock_time;
				aip->current_target_is_locked = 1;
			}
		} else {
			aip->aspect_locked_time -= flFrametime*2;
			// nprintf(("AI", "- Lock time = %7.3f\n", aip->aspect_locked_time));
			if (aip->aspect_locked_time < 0.0f)
				aip->aspect_locked_time = 0.0f;
		}
		//nprintf(("AI", "dot = %7.3f, time = %7.3f\n", dot_to_enemy, aip->aspect_locked_time));
	
	} else {
		aip->current_target_is_locked = 0;
		aip->aspect_locked_time = 0.0f; // Used to be this, why?: wip->min_lock_time;
		aip->ai_flags &= ~AIF_SEEK_LOCK;
	}

}

//	We're in chase mode and we've recently collided with our target.
//	Fly away from it!
void ai_chase_fly_away(object *objp, ai_info *aip)
{
	int	abort_flag = 0;

	if (aip->ai_flags & AIF_TARGET_COLLISION) {
		aip->ai_flags &= ~AIF_TARGET_COLLISION;	//	Don't process this hit again next frame.
		aip->submode = SM_FLY_AWAY;					//	Focus on avoiding target
		aip->submode_start_time = Missiontime;
	}

	if ((aip->target_objnum == -1) || (Objects[aip->target_objnum].signature != aip->target_signature)) {
		abort_flag = 1;
	}

	if (abort_flag || (Missiontime > aip->submode_start_time + F1_0)) {
		aip->last_attack_time = Missiontime;
		aip->submode = SM_ATTACK;
		aip->submode_start_time = Missiontime;
	} else {
		vector	v2e;
		float		dot;

		vm_vec_normalized_dir(&v2e, &Objects[aip->target_objnum].pos, &objp->pos);

		dot = vm_vec_dot(&objp->orient.v.fvec, &v2e);
		if (dot < 0.0f)
			accelerate_ship(aip, 1.0f);
		else
			accelerate_ship(aip, 1.0f - dot);
		turn_away_from_point(objp, &Objects[aip->target_objnum].pos, 0.0f);
	}
}

//	Return bank index of favored secondary weapon.
//	Return -1 if nothing favored.
//	"favored" means SEXPs have specified the weapon as being good to fire at en_objp.
int has_preferred_secondary(object *objp, object *en_objp, ship_weapon *swp)
{
// int is_preferred_weapon(int weapon_num, object *firer_objp, object *target_objp)
	int	i;

	for (i=0; i<swp->num_secondary_banks; i++) {
		if (swp->secondary_bank_capacity[i] > 0) {
			if (swp->secondary_bank_ammo[i] > 0) {
				if (is_preferred_weapon(swp->secondary_bank_weapons[i], objp, en_objp) != -1){
					return i;
				}
			}
		}
	}

	return -1;
}

//	Choose which secondary weapon to fire.
//	Note, this is not like ai_select_secondary_weapon().  "choose" means make a choice.
//	"select" means execute an order.  Get it?
//	This function calls ai_select_secondary_weapon() with the characteristics it should search for.
void ai_choose_secondary_weapon(object *objp, ai_info *aip, object *en_objp)
{
	float			subsystem_strength = 0.0f;
	int			is_big_ship, priority1, priority2;
	ship_weapon	*swp;
	ship_info	*esip;

	if ( en_objp->type == OBJ_SHIP ) {
		esip = &Ship_info[Ships[en_objp->instance].ship_info_index];
	} else {
		esip = NULL;
	}

	swp = &Ships[objp->instance].weapons;

	// AL 3-5-98: do a quick out if the ship has no secondaries
	if ( swp->num_secondary_banks <= 0 ) {
		swp->current_secondary_bank = -1;
		return;
	}

	int preferred_secondary = has_preferred_secondary(objp, en_objp, swp);

	if (preferred_secondary != -1) {
		if (swp->current_secondary_bank != preferred_secondary) {
			aip->current_target_is_locked = 0;
			aip->aspect_locked_time = 0.0f;
			swp->current_secondary_bank = preferred_secondary;
		}
		//nprintf(("AI", "Favored secondary = %s\n", Weapon_info[swp->secondary_bank_weapons[swp->current_secondary_bank]].name));
		aip->ai_flags |= AIF_UNLOAD_SECONDARIES;
	} else {
		aip->ai_flags &= ~AIF_UNLOAD_SECONDARIES;
		if (aip->targeted_subsys) {
			subsystem_strength = aip->targeted_subsys->current_hits;
		}

		if ( esip ) {
			is_big_ship = esip->flags & (SIF_HUGE_SHIP | SIF_BIG_SHIP);
		} else {
			is_big_ship=0;
		}

		if (is_big_ship) {
			priority1 = WIF_HUGE;
			priority2 = WIF_HOMING;
		} else if ( (esip != NULL) && (esip->flags & SIF_BOMBER) ) {
			priority1 = WIF_BOMBER_PLUS;
			priority2 = WIF_HOMING;
		} else if (subsystem_strength > 100.0f) {
			priority1 = WIF_PUNCTURE;
			priority2 = WIF_HOMING;
		} else {
			priority1 = WIF_HOMING;
			priority2 = 0;
		}
		
		ai_select_secondary_weapon(objp, swp, priority1, priority2);
	}

	// nprintf(("AI", "Frame %i: Chose secondary %s\n", Framecount, Weapon_info[swp->secondary_bank_weapons[swp->current_secondary_bank]].name));
}

//	Return time, in seconds, at which this ship can next fire its current secondary weapon.
float set_secondary_fire_delay(ai_info *aip, ship *shipp, weapon_info *swip)
{
	float t = swip->fire_wait;		//	Base delay for this weapon.
	if (shipp->team == Player_ship->team) {
		//	On player's team, _lower_ skill level = faster firing
		t = t * (Game_skill_level+2) / (NUM_SKILL_LEVELS);
	} else {		//	Not on player's team, higher skill level = faster firing
		t = t * (NUM_SKILL_LEVELS - Game_skill_level+2) / (NUM_SKILL_LEVELS);
	}

	t += (Num_ai_classes - aip->ai_class + 1) * 0.5f;
	t *= frand_range(0.8f, 1.2f);

	//	For the missiles that fire fairly quickly, occasionally add an additional substantial delay.
	if (t < 5.0f)
		if (frand() < 0.5f)
			t = t * 2.0f + 2.0f;

	return t;
}


void ai_chase_big_approach_set_goal(vector *goal_pos, object *attack_objp, object *target_objp, float *accel)
{
	float dist_to_goal;

	// head straight toward him and maybe circle later
	vm_vec_avg(goal_pos, &attack_objp->pos, &target_objp->pos);

	// get distance to goal
	dist_to_goal = vm_vec_dist(goal_pos, &attack_objp->pos);
	
	// set accel
	if (dist_to_goal > 400.0f) {
		*accel = 1.0f;
	} else {
		*accel = dist_to_goal/400.0f;
	}
}

void ai_chase_big_circle_set_goal(vector *goal_pos, object *attack_objp, object *target_objp, float *accel)
{
	get_tangent_point(goal_pos, attack_objp, &target_objp->pos, attack_objp->radius + target_objp->radius + 100.0f);

	*accel = 1.0f;
}

// get the current and desired horizontal separations between target
void ai_chase_big_get_separations(object *attack_objp, object *target_objp, vector *horz_vec_to_target, float *desired_separation, float *cur_separation)
{
	float temp, r_target, r_attacker, h_attacker, h_target;
	float perp_dist;
	vector vec_to_target;
	polymodel *pm;

	// get parameters of ships (as cylinders - radius and height)
	// get radius of attacker (for rotations about forward)
	pm = model_get(Ships[attack_objp->instance].modelnum);
	temp = max(pm->maxs.xyz.x, pm->maxs.xyz.y);
	r_attacker = max(-pm->mins.xyz.x, -pm->mins.xyz.y);
	r_attacker = max(temp, r_attacker);
	h_attacker = max(-pm->mins.xyz.z, pm->maxs.xyz.z);

	// get radius of target (for rotations about forward)
	pm = model_get(Ships[attack_objp->instance].modelnum);
	temp = max(pm->maxs.xyz.x, pm->maxs.xyz.y);
	r_target = max(-pm->mins.xyz.x, -pm->mins.xyz.y);
	r_target = max(temp, r_target);
	h_target = max(-pm->mins.xyz.z, pm->maxs.xyz.z);

	// find separation between cylinders [if parallel]
	vm_vec_sub(&vec_to_target, &attack_objp->pos, &target_objp->pos);

	// find the distance between centers along forward direction of ships
	perp_dist = vm_vec_dotprod(&vec_to_target, &target_objp->orient.v.fvec);

	// subtract off perp component to get "horizontal" separation vector between cylinders [ASSUMING parallel]
	vm_vec_scale_add(horz_vec_to_target, &vec_to_target, &target_objp->orient.v.fvec, -perp_dist);
	*cur_separation = vm_vec_mag_quick(horz_vec_to_target);

	// choose "optimal" separation of 1000 + r_target + r_attacker
	*desired_separation = 1000 + r_target + r_attacker;
}

void ai_chase_big_parallel_set_goal(vector *goal_pos, object *attack_objp, object *target_objp, float *accel)
{
	int opposing;
	float temp, r_target, r_attacker, h_attacker, h_target;
	float separation, optimal_separation;
	vector  horz_vec_to_target;
	polymodel *pm;

	// get parameters of ships (as cylinders - radius and height)
	// get radius of attacker (for rotations about forward)
	pm = model_get(Ships[attack_objp->instance].modelnum);
	temp = max(pm->maxs.xyz.x, pm->maxs.xyz.y);
	r_attacker = max(-pm->mins.xyz.x, -pm->mins.xyz.y);
	r_attacker = max(temp, r_attacker);
	h_attacker = max(-pm->mins.xyz.z, pm->maxs.xyz.z);

	// get radius of target (for rotations about forward)
	pm = model_get(Ships[attack_objp->instance].modelnum);
	temp = max(pm->maxs.xyz.x, pm->maxs.xyz.y);
	r_target = max(-pm->mins.xyz.x, -pm->mins.xyz.y);
	r_target = max(temp, r_target);
	h_target = max(-pm->mins.xyz.z, pm->maxs.xyz.z);

	// are we opposing (only when other ship is not moving)
	opposing = ( vm_vec_dotprod(&attack_objp->orient.v.fvec, &target_objp->orient.v.fvec) < 0 );

	ai_chase_big_get_separations(attack_objp, target_objp, &horz_vec_to_target, &optimal_separation, &separation);

	// choose dist (2000) so that we don't bash
	float dist = 2000;
	if (opposing) {
		dist = - dist;
	}

	// set the goal pos as dist forward from target along target forward
	vm_vec_scale_add(goal_pos, &target_objp->pos, &target_objp->orient.v.fvec, dist);
	// then add horizontal separation
	vm_vec_scale_add2(goal_pos, &horz_vec_to_target, optimal_separation/separation);

	// find the distance between centers along forward direction of ships
	vector vec_to_target;
	vm_vec_sub(&vec_to_target, &target_objp->pos, &attack_objp->pos);
	float perp_dist = vm_vec_dotprod(&vec_to_target, &target_objp->orient.v.fvec);

	float match_accel = target_objp->phys_info.vel.xyz.z / Ship_info[Ships[attack_objp->instance].ship_info_index].max_vel.xyz.z;
	float length_scale = attack_objp->radius;

	// if we're heading toward enemy ship, we want to keep going if we're ahead
	if (opposing) {
		perp_dist = -perp_dist;
	}

	if (perp_dist > 0) {
		// falling behind, so speed up
		*accel = match_accel + (1.0f - match_accel) / length_scale * (perp_dist);
	} else {
		// up in front, so slow down
		*accel = match_accel  - match_accel / length_scale * -perp_dist;
		*accel = max(0.0f, *accel);
	}

}


//	Return *goal_pos for one cruiser to attack another (big ship).
//	Choose point fairly nearby that is not occupied by another cruiser.
void ai_cruiser_chase_set_goal_pos(vector *goal_pos, object *pl_objp, object *en_objp)
{
	ai_info *aip;

	aip = &Ai_info[Ships[pl_objp->instance].ai_index];
	float accel;

	switch (aip->submode) {
	case SM_BIG_APPROACH:
		// do approach stuff;
		ai_chase_big_approach_set_goal(goal_pos, pl_objp, en_objp, &accel);
		break;

	case SM_BIG_CIRCLE:
		// do circle stuff
		ai_chase_big_circle_set_goal(goal_pos, pl_objp, en_objp, &accel);
		break;

	case SM_BIG_PARALLEL:
		// do parallel stuff
		ai_chase_big_parallel_set_goal(goal_pos, pl_objp, en_objp, &accel);
		break;
	}
}

int maybe_hack_cruiser_chase_abort()
{
	ship			*shipp = &Ships[Pl_objp->instance];	
	ship			*eshipp = &Ships[En_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];

	// mission sm3-08, sathanos chasing collosus
	if ( stricmp(Mission_filename, "sm3-08.fs2") == 0 ) {
		if (( stricmp(eshipp->ship_name, "colossus") == 0 ) || ( stricmp(shipp->ship_name, "colossus") == 0 )) {
			// Changed so all big ships attacking the Colossus will not do the chase code.
			// Did this so Beast wouldn't swerve away from Colossus. -- MK, 9/14/99
			//if ( stricmp(shipp->ship_name, "Sathanas") == 0 ) {
				// do cool hack stuff here
				ai_clear_ship_goals( aip );
				aip->mode = AIM_NONE;
				return 1;
			//}
		}
	}

	return 0;
}

//	Make a big ship pursue another big ship.
//	(Note, called "ai_cruiser_chase" because we already have ai_chase_big() which means fighter chases big ship.
void ai_cruiser_chase()
{
	ship_info	*sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];
	ship			*shipp = &Ships[Pl_objp->instance];	
	ai_info		*aip = &Ai_info[shipp->ai_index];

	if (!(sip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
		Int3();	//	Hmm, not a very big ship, how did we get in this function?
		aip->mode = AIM_NONE;
		return;
	}

	if (En_objp->type != OBJ_SHIP) {
		Int3();
		return;
	}

	if (En_objp->instance < 0) {
		Int3();
		return;
	}

	ship			*eshipp;
	ship_info	*esip;

	eshipp = &Ships[En_objp->instance];
	esip = &Ship_info[eshipp->ship_info_index];

	if (!(esip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
		// Int3();	//	Hmm, we're big and we're pursuing something other than a big ship?
		aip->mode = AIM_NONE;
		return;
	}

	vector	goal_pos;
	float turn_time = Ship_info[Ships[Pl_objp->instance].ship_info_index].srotation_time;

	// kamikaze - ram and explode
	if (aip->ai_flags & AIF_KAMIKAZE) {
		ai_turn_towards_vector(&En_objp->pos, Pl_objp, flFrametime, turn_time, NULL, NULL, 0.0f, 0);
		accelerate_ship(aip, 1.0f);
	} 
	
	// really track down and chase
	else {
		// check valid submode
		Assert( (aip->submode == SM_ATTACK) || (aip->submode == SM_BIG_APPROACH) || (aip->submode == SM_BIG_CIRCLE) || (aip->submode == SM_BIG_PARALLEL) );

		// just entering, approach enemy ship
		if (aip->submode == SM_ATTACK) {
			aip->submode = SM_BIG_APPROACH;
		}

		// desired accel
		float accel = 0.0f;
		vector *rvecp = NULL;

		switch (aip->submode) {
		case SM_BIG_APPROACH:
			// do approach stuff;
			ai_chase_big_approach_set_goal(&goal_pos, Pl_objp, En_objp, &accel);
			// maybe set rvec
			break;

		case SM_BIG_CIRCLE:
			// do circle stuff
			ai_chase_big_circle_set_goal(&goal_pos, Pl_objp, En_objp, &accel);
			// maybe set rvec
			break;

		case SM_BIG_PARALLEL:
			// do parallel stuff
			ai_chase_big_parallel_set_goal(&goal_pos, Pl_objp, En_objp, &accel);
			//maybe set rvec
			break;
		}


		// now move as desired
		ai_turn_towards_vector(&goal_pos, Pl_objp, flFrametime, turn_time, NULL, NULL, 0.0f, 0, rvecp);
		accelerate_ship(aip, accel);


		// maybe switch to new mode
		vector vec_to_enemy;
		float dist_to_enemy;
		int moving = (En_objp->phys_info.vel.xyz.z > 0.5f);
		vm_vec_sub(&vec_to_enemy, &En_objp->pos, &Pl_objp->pos);
		dist_to_enemy = vm_vec_mag_quick(&vec_to_enemy);

		switch (aip->submode) {
		case SM_BIG_APPROACH:
			if ( dist_to_enemy < (Pl_objp->radius + En_objp->radius)*1.25f + 200.0f ) {
				// moving
				if (moving) {
					// if within 90 degrees of en forward, go into parallel, otherwise circle
					if ( vm_vec_dotprod(&En_objp->orient.v.fvec, &Pl_objp->orient.v.fvec) > 0 ) {
						aip->submode = SM_BIG_PARALLEL;
					}
				}

				// otherwise cirle
				if ( !maybe_hack_cruiser_chase_abort() ) {
					aip->submode = SM_BIG_CIRCLE;
				}
			}
			break;

		case SM_BIG_CIRCLE:
			// moving
			if (moving) {
				vector temp;
				float desired_sep, cur_sep;
				// we're behind the enemy ship
				if (vm_vec_dotprod(&vec_to_enemy, &En_objp->orient.v.fvec) > 0) {
					// and we're turning toward the enemy
					if (vm_vec_dotprod(&En_objp->orient.v.fvec, &Pl_objp->orient.v.fvec) > 0) {
						// get separation
						ai_chase_big_get_separations(Pl_objp, En_objp, &temp, &desired_sep, &cur_sep);
						// and the separation is > 0.9 desired
						if (cur_sep > 0.9 * desired_sep) {
							aip->submode = SM_BIG_PARALLEL;
						}
					}
				}
			} else {
				// still
				vector temp;
				float desired_sep, cur_sep;
				// we're behind the enemy ship
				if (vm_vec_dotprod(&vec_to_enemy, &En_objp->orient.v.fvec) > 0) {
					// and we're turning toward the enemy
					if (vm_vec_dotprod(&En_objp->orient.v.fvec, &Pl_objp->orient.v.fvec) > 0) {
						// get separation
						ai_chase_big_get_separations(Pl_objp, En_objp, &temp, &desired_sep, &cur_sep);
						//and the separation is [0.9 to 1.1] desired
						if ( (cur_sep > 0.9f * desired_sep) ) {
							aip->submode = SM_BIG_PARALLEL;
						}
					}
				}
				// in front of ship
				else {
					// and we're turning toward the enemy
					if (vm_vec_dotprod(&En_objp->orient.v.fvec, &Pl_objp->orient.v.fvec) < 0) {
						// get separation
						ai_chase_big_get_separations(Pl_objp, En_objp, &temp, &desired_sep, &cur_sep);
						//and the separation is [0.9 to 1.1] desired
						if ( (cur_sep > 0.9f * desired_sep) ) {
							aip->submode = SM_BIG_PARALLEL;
						}
					}
				}
			}
			break;

		case SM_BIG_PARALLEL:
			// we're opposing
			if ( vm_vec_dotprod(&Pl_objp->orient.v.fvec, &En_objp->orient.v.fvec) < 0 ) {
				// and the other ship is moving
				if (moving) {
					// and we no longer overlap
					if ( dist_to_enemy > (0.75 * (En_objp->radius + Pl_objp->radius)) ) {
						aip->submode = SM_BIG_APPROACH;
					}
				}
			}
			break;
		}
	}
}

// --------------------------------------------------------------------------
// Make object Pl_objp chase object En_objp
void ai_chase()
{
	float			dist_to_enemy, time_to_enemy;
	float			dot_to_enemy, dot_from_enemy, real_dot_to_enemy;
	vector		player_pos, enemy_pos, predicted_enemy_pos, real_vec_to_enemy, predicted_vec_to_enemy;
	ship_info	*sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];
	ship			*shipp = &Ships[Pl_objp->instance];
	ship_weapon	*swp = &shipp->weapons;
	ai_info		*aip = &Ai_info[shipp->ai_index];
	int			enemy_sip_flags;

	if (aip->mode != AIM_CHASE) {
		Int3();
	}

	if (sip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) {
		ai_cruiser_chase();
		return;
	}

	if (!(sip->flags & (SIF_FIGHTER | SIF_BOMBER | SIF_ESCAPEPOD))) {
		Warning(LOCATION, "Ship %s is not 'small', but is in chase mode.\nSwitching to AI=none.\n", shipp->ship_name);
		aip->mode = AIM_NONE;
		return;
	}

	//nprintf(("AI", "%7s ", Submode_text[aip->submode]));

	if ( En_objp->type == OBJ_SHIP ) {
		enemy_sip_flags = Ship_info[Ships[En_objp->instance].ship_info_index].flags;
	} else {
		enemy_sip_flags = 0;
	}

	if ( enemy_sip_flags > 0 ) {
		if (enemy_sip_flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) {
			ai_big_chase();
			return;
		}
	}

	//	If collided with target_objnum last frame, avoid that ship.
	//	This should prevent the embarrassing behavior of ships getting stuck on each other
	//	as if they were magnetically attracted. -- MK, 11/13/97.
	if ((aip->ai_flags & AIF_TARGET_COLLISION) || (aip->submode == SM_FLY_AWAY)) {
		ai_chase_fly_away(Pl_objp, aip);
		return;
	}

	ai_set_positions(Pl_objp, En_objp, aip, &player_pos, &enemy_pos);
	dist_to_enemy = vm_vec_dist_quick(&player_pos, &enemy_pos);
	time_to_enemy = compute_time_to_enemy(dist_to_enemy, Pl_objp, En_objp);
	vm_vec_sub(&real_vec_to_enemy, &enemy_pos, &player_pos);

	vm_vec_normalize(&real_vec_to_enemy);

	real_dot_to_enemy = vm_vec_dot(&real_vec_to_enemy, &Pl_objp->orient.v.fvec);

	int is_stealthy_ship = 0;
	if ( (enemy_sip_flags > 0) && (enemy_sip_flags & SIF_STEALTH) ) {
		if ( ai_is_stealth_visible(Pl_objp, En_objp) != STEALTH_FULLY_TARGETABLE ) {
			is_stealthy_ship = 1;
		}
	}

	// Can only acquire lock on a target that isn't hidden from sensors
	if ( !(Ships[En_objp->instance].flags & SF_HIDDEN_FROM_SENSORS) && !is_stealthy_ship ) {
		update_aspect_lock_information(aip, &real_vec_to_enemy, dist_to_enemy, En_objp->radius);
	} else {
		aip->current_target_is_locked = 0;
		aip->ai_flags &= ~AIF_SEEK_LOCK;
	}

	//	If seeking lock, try to point directly at ship, else predict position so lasers can hit it.
	//	If just acquired target, or target is not in reasonable cone, don't refine believed enemy position.
	if ((real_dot_to_enemy < 0.25f) || (aip->target_time < 1.0f) || (aip->ai_flags & AIF_SEEK_LOCK)) {
		predicted_enemy_pos = enemy_pos;
	} else {
		//	Set predicted_enemy_pos.
		//	See if attacking a subsystem.
		if (aip->targeted_subsys != NULL) {
			Assert(En_objp->type == OBJ_SHIP);
			ship_info	*esip = &Ship_info[Ships[En_objp->instance].ship_info_index];
			if (get_shield_strength(En_objp)/esip->shields < HULL_DAMAGE_THRESHOLD_PERCENT) {
				//int	rval;

				if (aip->targeted_subsys != NULL) {
					get_subsystem_pos(&enemy_pos, En_objp, aip->targeted_subsys);
					predicted_enemy_pos = enemy_pos;
					predicted_vec_to_enemy = real_vec_to_enemy;
				} else {
					set_predicted_enemy_pos(&predicted_enemy_pos, Pl_objp, En_objp, aip);
					set_target_objnum(aip, -1);
				}
				// nprintf(("AI", "Attacking subsystem: rval = %i, pos = %7.3f %7.3f %7.3f\n", rval, predicted_enemy_pos.xyz.x, predicted_enemy_pos.xyz.y, predicted_enemy_pos.xyz.z));

			} else {
				set_predicted_enemy_pos(&predicted_enemy_pos, Pl_objp, En_objp, aip);
				// nprintf(("AI", "Attacking subsystem: pos = %7.3f %7.3f %7.3f\n", predicted_enemy_pos.xyz.x, predicted_enemy_pos.xyz.y, predicted_enemy_pos.xyz.z));
			}
		} else {
			set_predicted_enemy_pos(&predicted_enemy_pos, Pl_objp, En_objp, aip);
		}
	}

	vm_vec_sub(&predicted_vec_to_enemy, &predicted_enemy_pos, &player_pos);

	vm_vec_normalize(&predicted_vec_to_enemy);

	dot_to_enemy = vm_vec_dot(&Pl_objp->orient.v.fvec, &predicted_vec_to_enemy);
	dot_from_enemy= - vm_vec_dot(&En_objp->orient.v.fvec, &real_vec_to_enemy);

	//
	//	Set turn and acceleration based on submode.
	//
	switch (aip->submode) {
	case SM_CONTINUOUS_TURN:
		ai_chase_ct();
		break;

	case SM_STEALTH_FIND:
		ai_stealth_find();
		break;

	case SM_STEALTH_SWEEP:
		ai_stealth_sweep();
		break;

	case SM_ATTACK:
	case SM_SUPER_ATTACK:
	case SM_ATTACK_FOREVER:
		if (vm_vec_dist_quick(&Pl_objp->pos, &predicted_enemy_pos) > 100.0f + En_objp->radius * 2.0f) {
			if (maybe_avoid_big_ship(Pl_objp, En_objp, aip, &predicted_enemy_pos, 10.0f))
				return;
		}

		ai_chase_attack(aip, sip, &predicted_enemy_pos, dist_to_enemy);
		break;

	case SM_EVADE_SQUIGGLE:
		ai_chase_es(aip, sip);
		break;

	case SM_EVADE_BRAKE:
		ai_chase_eb(aip, sip, &predicted_enemy_pos, dist_to_enemy);
		break;

	case SM_EVADE:
		evade_ship();
		break;

	case SM_AVOID:
		avoid_ship();
		break;

	case SM_GET_BEHIND:
		get_behind_ship(aip, sip, dist_to_enemy);
		break;

	case SM_GET_AWAY:		//	Used to get away from opponent to prevent endless circling.
		ai_chase_ga(aip, sip);
		break;

	case SM_EVADE_WEAPON:
		evade_weapon();
		break;

	default:
		// Int3();
		aip->last_attack_time = Missiontime;
		aip->submode = SM_ATTACK;
		aip->submode_start_time = Missiontime;
	}

	//
	//	Maybe choose a new submode.
	//
	if ( (aip->submode != SM_AVOID) && (aip->submode != SM_ATTACK_FOREVER) ) {
		//	If a very long time since attacked, attack no matter what!
		if ( (aip->submode != SM_SUPER_ATTACK) && (aip->submode != SM_GET_AWAY) && !(aip->ai_flags & AIF_STEALTH_PURSIUT) ) {
			if (Missiontime - aip->last_attack_time > i2f(6)) {
				aip->submode = SM_SUPER_ATTACK;
				aip->submode_start_time = Missiontime;
				aip->last_attack_time = Missiontime;
			}
		}

		//	If a collision is expected, pull out!
		//	If enemy is pointing away and moving a bit, don't worry about collision detection.
		if ((dot_from_enemy > 0.5f) || (En_objp->phys_info.speed < 10.0f)) {
			if (might_collide_with_ship(Pl_objp, En_objp, dot_to_enemy, dist_to_enemy, 4.0f)) {
				if ((Missiontime - aip->last_hit_time > F1_0*4) && (dist_to_enemy < Pl_objp->radius*2 + En_objp->radius*2)) {
					accelerate_ship(aip, -1.0f);
				} else {
					aip->submode = SM_AVOID;
					aip->submode_start_time = Missiontime;
				}
			}
		}
	}

	switch (aip->submode) {
	case SM_CONTINUOUS_TURN:
		if (Missiontime - aip->submode_start_time > i2f(3)) {
			aip->last_attack_time = Missiontime;
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
		}
		break;

	case SM_ATTACK:
		// if taraget is stealth and stealth not visible, then enter stealth find mode
		if ( (aip->ai_flags & AIF_STEALTH_PURSIUT) && (ai_is_stealth_visible(Pl_objp, En_objp) == STEALTH_INVISIBLE) ) {
			aip->submode = SM_STEALTH_FIND;
			aip->submode_start_time = Missiontime;
			aip->submode_parm0 = SM_SF_AHEAD;
		} else if (ai_near_full_strength(Pl_objp, sip) && (Missiontime - aip->last_hit_target_time > i2f(3)) && (dist_to_enemy < 500.0f) && (dot_to_enemy < 0.5f)) {
			aip->submode = SM_SUPER_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
		} else if ((Missiontime - aip->last_hit_target_time > i2f(6)) &&
			(dist_to_enemy < 500.0f) && (dot_to_enemy < 0.2f) &&
			(frand() < (float) Game_skill_level/NUM_SKILL_LEVELS)) {
			aip->submode = SM_GET_AWAY;
			aip->submode_start_time = Missiontime;
			aip->last_hit_target_time = Missiontime;
		} else if ((enemy_sip_flags & SIF_SMALL_SHIP)
			&& (dot_to_enemy < dot_from_enemy)
			&& (En_objp->phys_info.speed > 15.0f) 
			&& (dist_to_enemy < 200.0f) 
			&& (dist_to_enemy > 50.0f)
			&& (dot_to_enemy < 0.1f)
			&& (Missiontime - aip->submode_start_time > i2f(2))) {
			aip->submode = SM_EVADE_BRAKE;
			aip->submode_start_time = Missiontime;
		} else if ((dot_to_enemy > 0.2f) && (dot_from_enemy > -0.2f) && (dot_from_enemy < 0.1f)) {
			aip->submode = SM_GET_BEHIND;
			aip->submode_start_time = Missiontime;
		} else if ((enemy_sip_flags & SIF_SMALL_SHIP) && (dist_to_enemy < 150.0f) && (dot_from_enemy > dot_to_enemy + 0.5f + aip->ai_courage*.002)) {
			if ((Missiontime - aip->last_hit_target_time > i2f(5)) && (frand() < (float) (aip->ai_class + Game_skill_level)/(Num_ai_classes + NUM_SKILL_LEVELS))) {				aip->submode = SM_GET_AWAY;
				aip->submode_start_time = Missiontime;
				aip->last_hit_target_time = Missiontime;
			} else {
				aip->submode = SM_EVADE_SQUIGGLE;
				aip->submode_start_time = Missiontime;
			}
		} else if ((enemy_sip_flags & SIF_SMALL_SHIP) && (Missiontime - aip->submode_start_time > F1_0*2)) {
			if ((dot_to_enemy < 0.8f) && (dot_from_enemy > dot_to_enemy)) {
				if (frand() > 0.5f) {
					aip->submode = SM_CONTINUOUS_TURN;
					aip->submode_parm0 = myrand() & 0x0f;
					aip->submode_start_time = Missiontime;
				} else {
					aip->submode = SM_EVADE;
					aip->submode_start_time = Missiontime;
				}
			} else {
				aip->submode_start_time = Missiontime;
			}
		}

		aip->last_attack_time = Missiontime;

		break;
		
	case SM_EVADE_SQUIGGLE:
		if ((Missiontime - aip->submode_start_time > i2f(5)) || (dist_to_enemy > 300.0f)) {
			if ((dist_to_enemy < 100.0f) && (dot_to_enemy < 0.0f) && (dot_from_enemy > 0.5f)) {
				aip->submode = SM_EVADE_BRAKE;
				aip->submode_start_time = Missiontime;
			} else {
				aip->last_attack_time = Missiontime;
				aip->submode = SM_ATTACK;
				aip->submode_start_time = Missiontime;
			}
		}
		break;
	
	case SM_EVADE_BRAKE:
		if ((dist_to_enemy < 15.0f) || (En_objp->phys_info.speed < 10.0f)) {
			aip->submode = SM_AVOID;
			aip->submode_start_time = Missiontime;
		} else if ((dot_to_enemy > 0.9f) || ((dot_from_enemy > 0.9f) && (Missiontime - aip->submode_start_time > i2f(1)))) {
			aip->last_attack_time = Missiontime;
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
		} else if (Missiontime - aip->submode_start_time > i2f(4)) {
			aip->last_attack_time = Missiontime;
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
		}
		break;

	case SM_EVADE:
		//	Modified by MK on 5/5/97 to keep trying to regain attack mode.  It's what a human would do.
		if ((dot_to_enemy < 0.2f) && (dot_from_enemy < 0.8f) && (dist_to_enemy < 100.0f) && (En_objp->phys_info.speed > 15.0f)) {
			aip->last_attack_time = Missiontime;
			aip->submode = SM_EVADE_BRAKE;
			aip->submode_start_time = Missiontime;
		} else if (((dot_to_enemy > dot_from_enemy - 0.1f)
			&& (Missiontime > aip->submode_start_time + i2f(1)))
			|| (dist_to_enemy > 150.0f + 2*(Pl_objp->radius + En_objp->radius))) {
			aip->last_attack_time = Missiontime;
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
		} else if (Missiontime - aip->submode_start_time > i2f(2))
			if (dot_from_enemy > 0.8f) {
				aip->submode = SM_EVADE_SQUIGGLE;
				aip->submode_start_time = Missiontime;
			}

		break;

	case SM_SUPER_ATTACK:
		// if stealth and invisible, enter stealth find mode
		if ( (aip->ai_flags & AIF_STEALTH_PURSIUT) && (ai_is_stealth_visible(Pl_objp, En_objp) == STEALTH_INVISIBLE) ) {
			aip->submode = SM_STEALTH_FIND;
			aip->submode_start_time = Missiontime;
			aip->submode_parm0 = SM_SF_AHEAD;
		} else if ((dist_to_enemy < 100.0f) && (dot_to_enemy < 0.8f) && (enemy_sip_flags & SIF_SMALL_SHIP) && (Missiontime - aip->submode_start_time > i2f(5) )) {
			aip->ai_flags &= ~AIF_ATTACK_SLOWLY;	//	Just in case, clear here.

			switch (myrand() % 5) {
			case 0:
				aip->submode = SM_CONTINUOUS_TURN;
				aip->submode_start_time = Missiontime;
				break;
			case 1:
				aip->submode_start_time = Missiontime;	//	Stay in super attack mode
				break;
			case 2:
			case 3:
				if (frand() < (float) 0.5f * (aip->ai_class + Game_skill_level)/(Num_ai_classes + NUM_SKILL_LEVELS)) {
					aip->submode = SM_GET_AWAY;
					aip->submode_start_time = Missiontime;
				} else {
					aip->submode = SM_EVADE;
					aip->submode_start_time = Missiontime;
				}
				break;
			case 4:
				if (dot_from_enemy + (NUM_SKILL_LEVELS - Game_skill_level) * 0.1f > dot_to_enemy) {	//	Less likely to GET_AWAY at lower skill levels.
					aip->submode = SM_EVADE;
					aip->submode_start_time = Missiontime;
				} else {
					aip->submode = SM_GET_AWAY;
					aip->submode_start_time = Missiontime;
				}
				break;
			default:
				Int3();	//	Impossible!
			}
		}

		aip->last_attack_time = Missiontime;

		break;

	case SM_AVOID:
		if ((dot_to_enemy > -0.2f) && (dist_to_enemy / (dot_to_enemy + 0.3f) < 100.0f)) {
			aip->submode_start_time = Missiontime;
		} else if (Missiontime - aip->submode_start_time > i2f(1)/2) {
			if (might_collide_with_ship(Pl_objp, En_objp, dot_to_enemy, dist_to_enemy, 3.0f)) {
				aip->submode_start_time = Missiontime;
			} else {
				aip->submode = SM_GET_BEHIND;
				aip->submode_start_time = Missiontime;
			}
		}

		break;

	case SM_GET_BEHIND:
		if ((dot_from_enemy < -0.7f) || (Missiontime - aip->submode_start_time > i2f(2))) {
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
		}
		break;

	case SM_GET_AWAY:
		if (Missiontime - aip->submode_start_time > i2f(2)) {
			float	rand_dist;

			rand_dist = ((Missiontime >> 17) & 0x03) * 100.0f + 200.0f;	//	Some value in 200..500
			if ((Missiontime - aip->submode_start_time > i2f(5)) || (dist_to_enemy > rand_dist) || (dot_from_enemy < 0.4f)) {
				aip->ai_flags |= AIF_ATTACK_SLOWLY;
				aip->submode = SM_ATTACK;
				aip->time_enemy_in_range = 2.0f;		//	Cheat.  Presumably if they were running away from you, they were monitoring you!
				aip->submode_start_time = Missiontime;
				aip->last_attack_time = Missiontime;
			}
		}
		break;

	case SM_EVADE_WEAPON:
		if (aip->danger_weapon_objnum == -1) {
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
		}
		break;

	// Either change to SM_ATTACK or AIM_FIND_STEALTH
	case SM_STEALTH_FIND:
		// if time > 5 sec change mode to sweep
		if ( !(aip->ai_flags & AIF_STEALTH_PURSIUT) || (ai_is_stealth_visible(Pl_objp, En_objp) == STEALTH_VISIBLE) ) {
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
			// sweep if I can't find in 5 sec or bail from find
		} else if ( ((Missiontime - aip->submode_start_time) > i2f(5)) || (aip->submode_parm0 == SM_SF_BAIL) ) {
			// begin sweep mode
			aip->submode = SM_STEALTH_SWEEP;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
			aip->submode_parm0 = SM_SS_SET_GOAL;
		}
		break;

	case SM_STEALTH_SWEEP:
		if ( !(aip->ai_flags & AIF_STEALTH_PURSIUT) || (ai_is_stealth_visible(Pl_objp, En_objp) == STEALTH_VISIBLE) ) {
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
		} else if ( (timestamp() - aip->stealth_last_visible_stamp) < 5000 ) {
			// go back to find mode
			aip->submode = SM_STEALTH_FIND;
			aip->submode_start_time = Missiontime;
			aip->submode_parm0 = SM_SF_AHEAD;
		} else if ( /*(Missiontime - aip->submode_start_time) > i2f(30) || */(aip->submode_parm0 == SM_SS_DONE) ) {
			// set target objnum = -1
			set_target_objnum(aip, -1);

			// set submode to attack
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->last_attack_time = Missiontime;
		}
		break;

	case SM_ATTACK_FOREVER:	//	Engines blown, just attack.
		break;

	default:
		//Int3();
		aip->submode = SM_ATTACK;
		aip->last_attack_time = Missiontime;

		aip->submode_start_time = Missiontime;
	}

	//
	//	Maybe fire primary weapon and update time_enemy_in_range
	//
	//nprintf(("AI", "time_enemy_in_range = %7.3f, dot = %7.3f\n", aip->time_enemy_in_range, dot_to_enemy));

	if (aip->mode != AIM_EVADE) {
		if (dot_to_enemy > 0.95f - 0.5f * En_objp->radius/max(1.0f, En_objp->radius + dist_to_enemy)) {
			aip->time_enemy_in_range += flFrametime;
			
			//	Chance of hitting ship is based on dot product of firing ship's forward vector with vector to ship
			//	and also the size of the target relative to distance to target.
			if (dot_to_enemy > max(0.5f, 0.90f + aip->ai_accuracy/10.0f - En_objp->radius/max(1.0f,dist_to_enemy))) {

				ship *temp_shipp;
				ship_weapon *tswp;

				temp_shipp = &Ships[Pl_objp->instance];
				tswp = &temp_shipp->weapons;
				if ( tswp->num_primary_banks > 0 ) {
					float	scale;
					Assert(tswp->current_primary_bank < tswp->num_primary_banks);
					weapon_info	*pwip = &Weapon_info[tswp->primary_bank_weapons[tswp->current_primary_bank]];

					//	Less likely to fire if far away and moving.
					scale = pwip->max_speed/(En_objp->phys_info.speed + pwip->max_speed);
					if (scale > 0.6f)
						scale = (scale - 0.6f) * 1.5f;
					else
						scale = 0.0f;
					if (dist_to_enemy < pwip->max_speed * (1.0f + scale)) {
						ai_fire_primary_weapon(Pl_objp);
					}

					//	Don't fire secondaries at a protected ship.
					if (!(En_objp->flags & OF_PROTECTED)) {
						ai_choose_secondary_weapon(Pl_objp, aip, En_objp);
						int current_bank = tswp->current_secondary_bank;
						weapon_info	*swip = &Weapon_info[tswp->secondary_bank_weapons[tswp->current_secondary_bank]];

						if (current_bank > -1) {
							if (aip->ai_flags & AIF_UNLOAD_SECONDARIES) {
								if (timestamp_until(swp->next_secondary_fire_stamp[current_bank]) > swip->fire_wait*1000.0f) {
									swp->next_secondary_fire_stamp[current_bank] = timestamp((int) (swip->fire_wait*1000.0f));
								}
							}

							if (timestamp_elapsed(swp->next_secondary_fire_stamp[current_bank])) {
								if (tswp->current_secondary_bank >= 0) {
									weapon_info	*swip = &Weapon_info[tswp->secondary_bank_weapons[tswp->current_secondary_bank]];
									float firing_range;
									
									if (swip->wi_flags & WIF_BOMB)
										firing_range = swip->max_speed * swip->lifetime * 0.75f;
									else
										firing_range = swip->max_speed * swip->lifetime * (Game_skill_level + 1 + aip->ai_class/2)/NUM_SKILL_LEVELS;

									// reduce firing range in nebula
									extern int Nebula_sec_range;
									if ((The_mission.flags & MISSION_FLAG_FULLNEB) && Nebula_sec_range) {
										firing_range *= 0.8f;
									}

									//	If firing a spawn weapon, distance doesn't matter.
									int	spawn_fire = 0;

									if (swip->wi_flags & WIF_SPAWN) {
										int	count;

										count = num_nearby_fighters(get_enemy_team_mask(OBJ_INDEX(Pl_objp)), &Pl_objp->pos, 1000.0f);

										if (count > 3)
											spawn_fire = 1;
										else if (count >= 1) {
											float hull_percent = Pl_objp->hull_strength/sip->initial_hull_strength;

											if (hull_percent < 0.01f)
												hull_percent = 0.01f;

											if (frand() < 0.25f/(30.0f*hull_percent) * count)	//	With timestamp below, this means could fire in 30 seconds if one enemy.
												spawn_fire = 1;
										}
									}

									if (spawn_fire || (dist_to_enemy < firing_range)) {
										if (ai_fire_secondary_weapon(Pl_objp)) {
											//	Only if weapon was fired do we specify time until next fire.  If not fired, done in ai_fire_secondary...
											float t;
											
											if (aip->ai_flags & AIF_UNLOAD_SECONDARIES) {
												t = swip->fire_wait;
											} else {
												t = set_secondary_fire_delay(aip, temp_shipp, swip);
											}
											//nprintf(("AI", "Next secondary to be fired in %7.3f seconds.\n", t));
											swp->next_secondary_fire_stamp[current_bank] = timestamp((int) (t*1000.0f));
										}
									} else {
										swp->next_secondary_fire_stamp[current_bank] = timestamp(250);
									}
								}
							}
						}
					}
				}
			}
		} else {
			aip->time_enemy_in_range *= (1.0f - flFrametime);
		}
	} else
		aip->time_enemy_in_range *= (1.0f - flFrametime);

}

//	Make the object *objp move so that the point *dp on the object moves towards the point *vp
//	Return distance.
void dock_move_towards_point(object *objp, vector *dp, vector *vp, float speed_scale, float other_obj_speed = 0.0f)
{
	physics_info	*pi = &objp->phys_info;
	float				dist;			//	dist to goal
	vector			v2g;			//	vector to goal
	vector			abs_pnt;		//	location of dock point, ie objp->pos + db

	if (dp == NULL)
		abs_pnt = objp->pos;
	else
		vm_vec_add(&abs_pnt, &objp->pos, dp);

	dist = vm_vec_dist_quick(vp, &abs_pnt);
	if (dist > 0.0f) {
		float	speed;

		dist = vm_vec_normalized_dir(&v2g, vp, &abs_pnt);
		speed = fl_sqrt(dist) * speed_scale;
		if (other_obj_speed < MAX_REPAIR_SPEED*0.75f)
			speed += other_obj_speed;
		else
			speed += MAX_REPAIR_SPEED*0.75f;

		vm_vec_copy_scale(&pi->desired_vel, &v2g, speed);
	} else
		vm_vec_zero(&pi->desired_vel);
}

//	Set the orientation in the global reference frame for an object to attain
//	to dock with another object.
//	*dom		resultant global matrix
//	*db_dest	pointer to destination docking bay information
//	*db_src	pointer to source docking bay information
//	*dorient	pointer to global orientation of docking bay (ie, the dockee object's orient)
//	*sorient	pointer to global orientation of docker
void set_goal_dock_orient(matrix *dom, dock_bay *db_dest, dock_bay *db_src, matrix *dorient, matrix *sorient)
{
	vector	fvec, uvec;
	matrix	m1, m2, m3;

	//	Compute the global orientation of the docker's (dest) docking bay.
	fvec = db_dest->norm[0];
	vm_vec_negate(&fvec);

	vm_vec_normalized_dir(&uvec, &db_dest->pnt[1], &db_dest->pnt[0]);
	vm_vector_2_matrix(&m1, &fvec, &uvec, NULL);

	vm_matrix_x_matrix(&m3, dorient, &m1);

	//	Compute the matrix given by the source docking bay.
	//	Pre-multiply the orientation of the source object (sorient) by the transpose
	//	of the docking bay's orientation, ie unrotate the source object's matrix.
	fvec = db_src->norm[0];
	vm_vec_normalized_dir(&uvec, &db_src->pnt[1], &db_src->pnt[0]);
	vm_vector_2_matrix(&m2, &fvec, &uvec, NULL);
	vm_transpose(&m2);

	vm_matrix_x_matrix(dom, &m3, &m2);
}

#define	DOCK_BACKUP_RETURN_VAL	99999.9f

//	Make objp dock with dobjp
//	Returns distance to goal, defined as distance between corresponding dock points, plus 10.0f * rotational velocity vector (DOA_DOCK only)
//	DOA_APPROACH	means	approach point aip->path_cur
//	DOA_DOCK			means dock
//	DOA_UNDOCK_1	means undock, moving to point nearest dock bay
//	DOA_UNDOCK_2	means undock, moving to point nearest dock bay and facing away from ship
//	DOA_DOCK_STAY	means rigidly maintain position in dock bay.
float dock_orient_and_approach(object *objp, object *dobjp, int dock_mode)
{
	ship_info	*sip0, *sip1;
	polymodel	*pm0, *pm1;
	ai_info		*aip;
	matrix		dom, nm;
	vector		goal_point, docker_point;
	float			fdist = UNINITIALIZED_VALUE;
	int			docker_index, dockee_index;		// index into docking_bays[] array for objects docking
																// docker is Pl_objp -- dockee is dobjp
	aip = &Ai_info[Ships[objp->instance].ai_index];

	//	If dockee has moved much, then path will be recreated.
	//	Might need to change state if moved too far.
	if ((dock_mode != DOA_DOCK_STAY) && (dock_mode != DOA_DOCK)) {
		if (maybe_recreate_path(objp, &Ai_info[Ships[objp->instance].ai_index], 0) > 5.0f) {
/*			if (dock_mode == DOA_APPROACH) {
				return DOCK_BACKUP_RETURN_VAL;
			} else if (dock_mode == DOA_DOCK) {
				return DOCK_BACKUP_RETURN_VAL;		
			}
*/		}
	}

	objp->phys_info.forward_thrust = 0.0f;		//	Kill thrust so we don't have a sputtering thruster.

	sip0 = &Ship_info[Ships[objp->instance].ship_info_index];
	sip1 = &Ship_info[Ships[dobjp->instance].ship_info_index];
	pm0 = model_get( sip0->modelnum );
	pm1 = model_get( sip1->modelnum );

	docker_index = aip->dock_index;
	dockee_index = aip->dockee_index;

	Assert( docker_index >= 0 );
	Assert( dockee_index >= 0 );

	Assert(pm0->docking_bays[docker_index].num_slots == 2);
	Assert(pm1->docking_bays[dockee_index].num_slots == 2);

	float speed_scale = 1.0f;
	if (sip0->flags & SIF_SUPPORT) {
		speed_scale = 3.0f;
	}

	switch (dock_mode) {
	case DOA_APPROACH:
		{
		if (ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_ENGINE) <= 0.0f) {
			return 9999.9f;
		}
		
		//	Compute the desired global orientation matrix for the docker's station.
		//	That is, the normal vector of the docking station must be the same as the
		//	forward vector and the vector between its two points must be the uvec.
		set_goal_dock_orient(&dom, &pm1->docking_bays[dockee_index], &pm0->docking_bays[docker_index], &dobjp->orient, &objp->orient);

		//	Compute new orientation matrix and update rotational velocity.
		vector	w_in, w_out, vel_limit, acc_limit;
		float		tdist, mdist, ss1;

		w_in = objp->phys_info.rotvel;
		vel_limit = objp->phys_info.max_rotvel;
		vm_vec_copy_scale(&acc_limit, &vel_limit, 0.3f);
		
		if (sip0->flags & SIF_SUPPORT)
			vm_vec_scale(&acc_limit, 2.0f);

		// 1 at end of line prevent overshoot
		vm_matrix_interpolate(&dom, &objp->orient, &w_in, flFrametime, &nm, &w_out, &vel_limit, &acc_limit, 1);
		objp->phys_info.rotvel = w_out;
		objp->orient = nm;

		//	Translate towards goal and note distance to goal.
		goal_point = Path_points[aip->path_cur].pos;
		mdist = ai_matrix_dist(&objp->orient, &dom);
		tdist = vm_vec_dist_quick(&objp->pos, &goal_point);

		//	If translation is badly lagging rotation, speed up translation.
		if (mdist > 0.1f) {
			ss1 = tdist/(10.0f * mdist);
			if (ss1 > 2.0f)
				ss1 = 2.0f;
		} else
			ss1 = 2.0f;

		// nprintf(("AI", "speed scale = %7.3f\n", ss1));
		speed_scale *= 1.0f + ss1;

		dock_move_towards_point(objp, NULL, &goal_point, speed_scale, dobjp->phys_info.speed);

		fdist = vm_vec_dist_quick(&objp->pos, &goal_point);

		//	Note, we're interested in distance from goal, so if we're still turning, bash that into return value.
		// nprintf(("AI", "matrix dist = %7.3f, threshold = %7.3f\n", mdist, 2*flFrametime));
		fdist += 2.0f * mdist;

		break;
	}
	case DOA_DOCK:
		if (ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_ENGINE) <= 0.0f) {
			return 9999.9f;
		}
	case DOA_DOCK_STAY:
		//	Compute the desired global orientation matrix for the docker's station.
		//	That is, the normal vector of the docking station must be the same as the
		//	forward vector and the vector between its two points must be the uvec.
		set_goal_dock_orient(&dom, &pm1->docking_bays[dockee_index], &pm0->docking_bays[docker_index], &dobjp->orient, &objp->orient);

		//	Compute distance between dock bay points.
		vector	db0, db1, db2, db3;

		vm_vec_unrotate(&db0, &pm0->docking_bays[docker_index].pnt[0], &objp->orient);
		vm_vec_add2(&db0, &objp->pos);

		vm_vec_unrotate(&db1, &pm0->docking_bays[docker_index].pnt[1], &objp->orient);
		vm_vec_add2(&db1, &objp->pos);

		vm_vec_unrotate(&db2, &pm1->docking_bays[dockee_index].pnt[0], &dobjp->orient);
		vm_vec_add2(&db2, &dobjp->pos);

		vm_vec_unrotate(&db3, &pm1->docking_bays[dockee_index].pnt[1], &dobjp->orient);
		vm_vec_add2(&db3, &dobjp->pos);

		vm_vec_avg(&goal_point, &db2, &db3);

		vm_vec_avg(&docker_point, &db0, &db1);
		vm_vec_sub2(&docker_point, &objp->pos);

		if (dock_mode == DOA_DOCK) {
			vector	t1, t2;
			vector	w_in, w_out, vel_limit, acc_limit;

			fdist = vm_vec_dist_quick(vm_vec_avg(&t1, &db0, &db1), vm_vec_avg(&t2, &db2, &db3));

			//	Compute new orientation matrix and update rotational velocity.
			w_in = objp->phys_info.rotvel;
			vel_limit = objp->phys_info.max_rotvel;
			vm_vec_copy_scale(&acc_limit, &vel_limit, 0.3f);

			if (sip0->flags & SIF_SUPPORT)
				vm_vec_scale(&acc_limit, 2.0f);

			vm_matrix_interpolate(&dom, &objp->orient, &w_in, flFrametime, &nm, &w_out, &vel_limit, &acc_limit);
			objp->phys_info.rotvel = w_out;
			objp->orient = nm;

			//	Note, we're interested in distance from goal, so if we're still turning, bash that into return value.
			fdist += 10.0f * vm_vec_mag_quick(&w_out);

			dock_move_towards_point(objp, &docker_point, &goal_point, speed_scale, dobjp->phys_info.speed);
		} else {
			Assert(dock_mode == DOA_DOCK_STAY);
			objp->orient = dom;
			vector	temp;
			vm_vec_sub(&temp, &goal_point, &docker_point);
			vm_vec_sub(&objp->pos, &goal_point, &docker_point);
		}

		break;
	case DOA_UNDOCK_1: {
		if (ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_ENGINE) <= 0.0f) {
			return 9999.9f;
		}

		//	Undocking.
		//	Move to point on dock path nearest to dock station.
		Assert(aip->path_length >= 2);
		goal_point = Path_points[aip->path_start + aip->path_length-2].pos;

		vm_vec_zero(&docker_point);
		fdist = vm_vec_dist_quick(&objp->pos, &goal_point);

		dock_move_towards_point(objp, &docker_point, &goal_point, speed_scale);

		break;
			  }

	case DOA_UNDOCK_2: {
		//	Undocking.
		//	Move to point on dock path nearest to dock station and orient away from big ship.
		int		desired_index;

		if (ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_ENGINE) <= 0.0f) {
			return 9999.9f;
		}

		Assert(aip->path_length >= 2);
//		if (aip->path_length >= 3)
//			desired_index = aip->path_length-3;
//		else
			desired_index = aip->path_length-2;

		goal_point = Path_points[aip->path_start + desired_index].pos;

		dock_move_towards_point(objp, NULL, &goal_point, speed_scale);

		fdist = vm_vec_dist_quick(&objp->pos, &goal_point);
		break;
			  }
	case DOA_UNDOCK_3: {
		float		dist, goal_dist;
		vector	away_vec;

		goal_dist = objp->radius + dobjp->radius + 25.0f;

		dist = vm_vec_normalized_dir(&away_vec, &objp->pos, &dobjp->pos);
		vm_vec_scale_add(&goal_point, &dobjp->pos, &away_vec, goal_dist);
		if (vm_vec_dist_quick(&goal_point, &dobjp->pos) < vm_vec_dist_quick(&objp->pos, &dobjp->pos))
			fdist = 0.0f;
		else {
			float	dot, accel;
			float turn_time = Ship_info[Ships[objp->instance].ship_info_index].srotation_time;
			ai_turn_towards_vector(&goal_point, objp, flFrametime, turn_time, NULL, NULL, 0.0f, 0);

			dot = vm_vec_dot(&objp->orient.v.fvec, &away_vec);
			accel = 0.1f;
			if (dot > accel)
				accel = dot;
			if (dist > goal_dist/2)
				accel *= 1.2f - 0.5f*goal_dist/dist;

			accelerate_ship(aip, accel);
			fdist = vm_vec_dist_quick(&objp->pos, &goal_point);
		}

		break;
							 }
	}

#ifndef NDEBUG
	//	For debug purposes, compute global orientation of both dock vectors and show
	//	how close they are.
	vector	d0, d1;

	vm_vec_unrotate(&d0, &pm0->docking_bays[docker_index].norm[0], &objp->orient);
	vm_vec_unrotate(&d1, &pm1->docking_bays[dockee_index].norm[0], &dobjp->orient);

	//nprintf(("AI", "or/app: dist = %7.3f/%7.3f, dot = %7.3f, global dot = %7.3f\n", 
	//	vm_vec_dist_quick(&goal_point, &objp->pos), fdist,
	//	vm_vec_dot(&objp->orient.v.fvec, &dom.v.fvec), 
	//	vm_vec_dot(&d0, &d1)));
#endif

	// -- Note, A lot of callers don't care about fdist, so OK to return ERROR value: Assert(fdist != UNINITIALIZED_VALUE);
	return fdist;

}

void debug_find_guard_object()
{
	ship			*shipp = &Ships[Pl_objp->instance];	
	object		*objp;

	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if ((Pl_objp != objp) && (objp->type == OBJ_SHIP)) {
			if (objp->instance != -1) {
				if (Ships[objp->instance].team == shipp->team)	{
					// nprintf(("AI", "Setting guard object for %s to %s\n", shipp->ship_name, Ships[objp->instance].ship_name));
					ai_set_guard_object(Pl_objp, objp);
				}
			}
		}
	}

}

//	Given an object number, return the number of ships attacking it.
int num_ships_attacking(int objnum)
{
	object	*objp;
	ship_obj	*so;
	int		count = 0;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		if (objp->instance != -1) {
			ai_info	*aip;
			aip = &Ai_info[Ships[objp->instance].ai_index];

			if ((aip->mode == AIM_CHASE) && (aip->target_objnum == objnum))
				if (Ships[objp->instance].team != Ships[Objects[objnum].instance].team)
					count++;
		}
	}

	return count;
}

//	For all objects attacking object #objnum, remove the one that is farthest away.
//	Do this by resuming previous behavior, if any.  If not, set target_objnum to -1.
void remove_farthest_attacker(int objnum)
{
	object	*objp, *objp2, *farthest_objp;
	ship_obj	*so;
	float		farthest_dist;

	objp2 = &Objects[objnum];

	farthest_dist = 9999999.9f;
	farthest_objp = NULL;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		if ( !(objp->flags & OF_PLAYER_SHIP)) {
			if (objp->instance != -1) {
				ai_info	*aip2;

				aip2 = &Ai_info[Ships[objp->instance].ai_index];

				if ((aip2->mode == AIM_CHASE) && (aip2->target_objnum == objnum)) {
					if (Ships[objp->instance].team != Ships[Objects[objnum].instance].team) {
						float	dist;

						dist = vm_vec_dist_quick(&objp->pos, &objp2->pos);
						if (dist < farthest_dist) {
							farthest_dist = dist;
							farthest_objp = objp;
						}
					}
				}
			}
		}
	}

	if (farthest_objp != NULL) {
		ai_info	*aip;
		Assert(farthest_objp->type == OBJ_SHIP);
		Assert((farthest_objp->instance > -1) && (farthest_objp->instance < MAX_SHIPS));
		Assert(Ships[farthest_objp->instance].ai_index > -1);

		aip = &Ai_info[Ships[farthest_objp->instance].ai_index];

		if (!maybe_resume_previous_mode(Pl_objp, aip)) {
			//	If already ignoring something under player's orders, don't ignore current target.
			if ((aip->ignore_objnum == UNUSED_OBJNUM) || (aip->ai_flags & AIF_TEMPORARY_IGNORE)) {
				aip->ignore_objnum = aip->target_objnum;
				aip->ignore_signature = Objects[aip->target_objnum].signature;
				aip->ai_flags |= AIF_TEMPORARY_IGNORE;
				aip->ignore_expire_timestamp = timestamp(((myrand() % 10) + 20) * 1000);	//	OK to attack again in 20 to 24 seconds.
			}
			aip->target_objnum = -1;
			ai_do_default_behavior(farthest_objp);
		}
	}
}

// Maybe limit the number of attackers on attack_objnum.  For now, only limit attackers
// in attacked_objnum is the player
// input:	attacked_objnum	=>		object index for ship we want to limit attacks on
//
//	exit:			1	=>	num attackers exceeds maximum, abort
//					0	=>	removed the farthest attacker
//					-1	=>	nothing was done
int ai_maybe_limit_attackers(int attacked_objnum)
{
	int rval=-1;

	// limit the number of ships attacking the _player_ only
//	if ( attacked_objnum == OBJ_INDEX(Player_obj) ) {
	if ( Objects[attacked_objnum].flags & OF_PLAYER_SHIP) {
		int num_attacking;
		num_attacking = num_ships_attacking(attacked_objnum);

		if (num_attacking == Skill_level_max_attackers[Game_skill_level]) {
			remove_farthest_attacker(attacked_objnum);
			rval=0;
		} else if (num_attacking > Skill_level_max_attackers[Game_skill_level]) {
			rval=1;
		}
		//nprintf(("AI", "Num attacking player = %i\n", num_attacking));
	}

	return rval;
}

//	Object being guarded by object *guard_objp was hit by object *hitter_objp
void guard_object_was_hit(object *guard_objp, object *hitter_objp)
{
	int		hitter_objnum;
	ai_info	*aip;

	aip = &Ai_info[Ships[guard_objp->instance].ai_index];

	if (guard_objp == hitter_objp) {
		// Int3();	//	Bogus!  Who tried to get me to attack myself!  Trace out and fix!
		return;
	}

	if (guard_objp->type == OBJ_GHOST || hitter_objp->type == OBJ_GHOST)
		return;

	if (aip->ai_flags & AIF_NO_DYNAMIC)	//	Not allowed to pursue dynamic goals.  So, why are we guarding?
		return;

	Assert( (hitter_objp->type == OBJ_SHIP) || (hitter_objp->type == OBJ_ASTEROID) || (hitter_objp->type == OBJ_WEAPON) );

	hitter_objnum = OBJ_INDEX(hitter_objp);

	if ( hitter_objp->type == OBJ_SHIP ) {
		//	If the hitter object is the ignore object, don't attack it.
		if (is_ignore_object(aip, hitter_objp-Objects))
			return;

		//	If hitter is on same team as me, don't attack him.
		if (Ships[guard_objp->instance].team == Ships[hitter_objp->instance].team)
			return;

		// limit the number of ships attacking hitter_objnum (for now, only if hitter_objnum is player)
		if ( ai_maybe_limit_attackers(hitter_objnum) == 1 ) {
			return;
		}

		// dont attack if you can't see him
		if ( awacs_get_level(hitter_objp, &Ships[aip->shipnum], 1) < 1 ) {
			// if he's a stealth and visible, but not targetable, ok to attack.
			if ( is_object_stealth_ship(hitter_objp) ) {
				if ( ai_is_stealth_visible(guard_objp, hitter_objp) != STEALTH_VISIBLE ) {
					return;
				}
			}
		}
	}

	if (aip->target_objnum == -1) {
		aip->ok_to_target_timestamp = timestamp(0);
	}

	if ((aip->submode == AIS_GUARD_PATROL) || (aip->submode == AIS_GUARD_STATIC)) {

		if ( hitter_objp->type == OBJ_SHIP ) {
			if (!(Ship_info[Ships[guard_objp->instance].ship_info_index].flags & SIF_SMALL_SHIP)) {
				return;
			}

			// limit the number of ships attacking hitter_objnum (for now, only if hitter_objnum is player)
			if ( ai_maybe_limit_attackers(hitter_objnum) == 1 ) {
				return;
			}
		}

		if (aip->target_objnum != hitter_objnum) {
			aip->aspect_locked_time = 0.0f;
		}

		aip->ok_to_target_timestamp = timestamp(0);

		set_target_objnum(aip, hitter_objnum);
		//if (aip->target_objnum == -1) nprintf(("AI", "Frame %i: Attacking NONE\n",Framecount)); else nprintf(("AI", "Frame %i: Attacking %s\n", Framecount, Ships[Objects[aip->target_objnum].instance].ship_name));
		aip->previous_mode = AIM_GUARD;
		aip->previous_submode = aip->submode;
		aip->mode = AIM_CHASE;
		aip->submode = SM_ATTACK;
		aip->submode_start_time = Missiontime;
		aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;
	} else if (aip->previous_mode == AIM_GUARD) {
		if (aip->target_objnum == -1) {

			if ( hitter_objp->type == OBJ_SHIP ) {
				// limit the number of ships attacking hitter_objnum (for now, only if hitter_objnum is player)
				if ( ai_maybe_limit_attackers(hitter_objnum) == 1 ) {
					return;
				}
			}

			set_target_objnum(aip, hitter_objnum);
		//if (aip->target_objnum == -1) nprintf(("AI", "Frame %i: Attacking NONE\n",Framecount)); else nprintf(("AI", "Frame %i: Attacking %s\n", Framecount, Ships[Objects[aip->target_objnum].instance].ship_name));
			aip->mode = AIM_CHASE;
			aip->submode = SM_ATTACK;
			aip->submode_start_time = Missiontime;
			aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;
		} else {
			int	num_attacking_cur, num_attacking_new;

			num_attacking_cur = num_ships_attacking(aip->target_objnum);
			if (num_attacking_cur > 1) {
				num_attacking_new = num_ships_attacking(hitter_objnum);

				if (num_attacking_new < num_attacking_cur) {

					if ( hitter_objp->type == OBJ_SHIP ) {
						// limit the number of ships attacking hitter_objnum (for now, only if hitter_objnum is player)
						if ( ai_maybe_limit_attackers(hitter_objnum) == 1 ) {
							return;
						}
					}
					set_target_objnum(aip, hitter_objp-Objects);
		//if (aip->target_objnum == -1) nprintf(("AI", "Frame %i: Attacking NONE\n",Framecount)); else nprintf(("AI", "Frame %i: Attacking %s\n", Framecount, Ships[Objects[aip->target_objnum].instance].ship_name));
					aip->mode = AIM_CHASE;
					aip->submode = SM_ATTACK;
					aip->submode_start_time = Missiontime;
					aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;
				}
			}
		}
	}
}

//	Ship object *hit_objp was hit by ship object *hitter_objp.
//	See if anyone is guarding hit_objp and, if so, do something useful.
void maybe_update_guard_object(object *hit_objp, object *hitter_objp)
{
	object	*objp;
	ship_obj	*so;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		if (objp->instance != -1) {
			ai_info	*aip;
			aip = &Ai_info[Ships[objp->instance].ai_index];

			if ((aip->mode == AIM_GUARD) || (aip->active_goal == AI_ACTIVE_GOAL_DYNAMIC)) {
				if (aip->guard_objnum == hit_objp-Objects) {
					guard_object_was_hit(objp, hitter_objp);
				} else if ((aip->guard_wingnum != -1) && (aip->guard_wingnum == Ai_info[Ships[hit_objp->instance].ai_index].wing)) {
					guard_object_was_hit(objp, hitter_objp);
				}
			}
		}
	}
}

// Scan missile list looking for bombs homing on guarded_objp
// return 1 if bomb is found (and targeted by guarding_objp), otherwise return 0
int ai_guard_find_nearby_bomb(object *guarding_objp, object *guarded_objp)
{	
	missile_obj	*mo;
	object		*bomb_objp, *closest_bomb_objp=NULL;
	float			dist, dist_to_guarding_obj,closest_dist_to_guarding_obj=999999.0f;
	weapon		*wp;
	weapon_info	*wip;

	for ( mo = GET_NEXT(&Missile_obj_list); mo != END_OF_LIST(&Missile_obj_list); mo = GET_NEXT(mo) ) {
		Assert(mo->objnum >= 0 && mo->objnum < MAX_OBJECTS);
		bomb_objp = &Objects[mo->objnum];

		wp = &Weapons[bomb_objp->instance];
		wip = &Weapon_info[wp->weapon_info_index];

		if ( !(wip->wi_flags & WIF_BOMB) ) {
			continue;
		}

		if ( wp->homing_object != guarded_objp ) {
			continue;
		}

		dist = vm_vec_dist_quick(&bomb_objp->pos, &guarded_objp->pos);

		if (dist < (MAX_GUARD_DIST + guarded_objp->radius)*3) {
			dist_to_guarding_obj = vm_vec_dist_quick(&bomb_objp->pos, &guarding_objp->pos);
			if ( dist_to_guarding_obj < closest_dist_to_guarding_obj ) {
				closest_dist_to_guarding_obj = dist_to_guarding_obj;
				closest_bomb_objp = bomb_objp;
			}
		}
	}

	if ( closest_bomb_objp ) {
		guard_object_was_hit(guarding_objp, closest_bomb_objp);
		return 1;
	}

	return 0;
}

//	Scan enemy ships and see if one is near enough to guard object to be pursued.
void ai_guard_find_nearby_ship(object *guarding_objp, object *guarded_objp)
{
	ship		*guarding_shipp = &Ships[guarding_objp->instance];
	ai_info	*guarding_aip = &Ai_info[guarding_shipp->ai_index];
	ship_obj	*so;
	object	*enemy_objp;
	float		dist;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		enemy_objp = &Objects[so->objnum];

		if (enemy_objp->instance < 0) {
			continue;
		}

		ship	*eshipp = &Ships[enemy_objp->instance];

		//	Don't attack a cargo container or other harmless ships
		if (!(Ship_info[eshipp->ship_info_index].flags & SIF_HARMLESS)) {
			if (guarding_shipp->team != eshipp->team)	{
				dist = vm_vec_dist_quick(&enemy_objp->pos, &guarded_objp->pos);
				if (dist < (MAX_GUARD_DIST + guarded_objp->radius)*3) {
					guard_object_was_hit(guarding_objp, enemy_objp);
				} else if ((dist < 3000.0f) && (Ai_info[eshipp->ai_index].target_objnum == guarding_aip->guard_objnum)) {
					//nprintf(("AI", "%i: Enemy %s targeting guard object (%s), %s will attack!!\n", Framecount, eshipp->ship_name, Ships[Objects[guarding_aip->guard_objnum].instance].ship_name, guarding_shipp->ship_name));
					guard_object_was_hit(guarding_objp, enemy_objp);
				}
			}
		}
	}
}

// Scan for nearby asteroids.  Favor asteroids which have their collide_objnum set to that of the
// guarded ship.  Also, favor asteroids that are closer to the guarding ship, since it looks cooler
// when a ship blows up an asteroid then goes after the pieces that break off.
void ai_guard_find_nearby_asteroid(object *guarding_objp, object *guarded_objp)
{	
	float		dist;

	object	*closest_asteroid_objp=NULL, *danger_asteroid_objp=NULL, *asteroid_objp;
	float		dist_to_self, closest_danger_asteroid_dist=999999.0f, closest_asteroid_dist=999999.0f;

	for ( asteroid_objp = GET_FIRST(&obj_used_list); asteroid_objp != END_OF_LIST(&obj_used_list); asteroid_objp = GET_NEXT(asteroid_objp) ) {
		if ( asteroid_objp->type == OBJ_ASTEROID ) {
			// Attack asteroid if near guarded ship
			dist = vm_vec_dist_quick(&asteroid_objp->pos, &guarded_objp->pos);
			if ( dist < (MAX_GUARD_DIST + guarded_objp->radius)*2) {
				dist_to_self = vm_vec_dist_quick(&asteroid_objp->pos, &guarding_objp->pos);
				if ( OBJ_INDEX(guarded_objp) == asteroid_collide_objnum(asteroid_objp) ) {
					if( dist_to_self < closest_danger_asteroid_dist ) {
						danger_asteroid_objp=asteroid_objp;
						closest_danger_asteroid_dist=dist_to_self;
					}
				} 
				if ( dist_to_self < closest_asteroid_dist ) {
					// only attack if moving slower than own max speed
					if ( vm_vec_mag_quick(&asteroid_objp->phys_info.vel) < guarding_objp->phys_info.max_vel.xyz.z ) {
						closest_asteroid_dist = dist_to_self;
						closest_asteroid_objp = asteroid_objp;
					}
				}
			}
		}
	}

	if ( danger_asteroid_objp ) {
		guard_object_was_hit(guarding_objp, danger_asteroid_objp);
	} else if ( closest_asteroid_objp ) {
		guard_object_was_hit(guarding_objp, closest_asteroid_objp);
	}
}

//	Scan potential harmful objects and see if one is near enough to guard object to be pursued.
void ai_guard_find_nearby_object()
{
	ship			*shipp = &Ships[Pl_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];
	object		*guardobjp;
	int			bomb_found=0;

	guardobjp = &Objects[aip->guard_objnum];
	
	// highest priority is a bomb fired on guarded ship
	bomb_found = ai_guard_find_nearby_bomb(Pl_objp, guardobjp);

	if ( !bomb_found ) {
		// check for ships if there are no bombs fired at guarded ship
		ai_guard_find_nearby_ship(Pl_objp, guardobjp);

		// if not attacking anything, go for asteroid close to guarded ship
		if ( (aip->target_objnum == -1) && asteroid_count() ) {
			ai_guard_find_nearby_asteroid(Pl_objp, guardobjp);
		}
	}
}

// gets closest point on extended axis of cylinder, r_vec, and radius of cylinder
// returns z of axis_point in cyl_objp reference frame
float get_cylinder_points(object *other_objp, object *cyl_objp, vector *axis_pt, vector *r_vec, float *radius)
{
	Assert(other_objp->type == OBJ_SHIP);
	Assert(cyl_objp->type == OBJ_SHIP);

	// get radius of cylinder
	polymodel *pm = model_get(Ships[cyl_objp->instance].modelnum);
	float tempx, tempy;
	tempx = max(-pm->mins.xyz.x, pm->maxs.xyz.x);
	tempy = max(-pm->mins.xyz.y, pm->maxs.xyz.y);
	*radius = max(tempx, tempy);

	// get vec from cylinder to other_obj
	vector r_sph;
	vm_vec_sub(&r_sph, &other_objp->pos, &cyl_objp->pos);

	// get point on axis and on cylinder
	// extended_cylinder_z is along extended cylinder
	// cylinder_z is capped within cylinder
	float extended_cylinder_z = vm_vec_dotprod(&r_sph, &cyl_objp->orient.v.fvec);

	// get pt on axis of extended cylinder
	vm_vec_scale_add(axis_pt, &cyl_objp->pos, &cyl_objp->orient.v.fvec, extended_cylinder_z);

	// get r_vec (pos - axis_pt) normalized
	vm_vec_normalized_dir(r_vec, &other_objp->pos, axis_pt);

	return extended_cylinder_z;
}

// handler for guard behavior when guarding BIG ships
//	When someone has attacked guarded ship, then attack that ship.
// To attack another ship, switch out of guard mode into chase mode.
void ai_big_guard()
{
	
	ship			*shipp = &Ships[Pl_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];
	object		*guard_objp;

	// sanity checks already done in ai_guard()
	guard_objp = &Objects[aip->guard_objnum];

	switch (aip->submode) {
	case AIS_GUARD_STATIC:
	case AIS_GUARD_PATROL:
		{
		vector axis_pt, r_vec, theta_vec;
		float radius, extended_z;

		// get random [0 to 1] based on OBJNUM
		float objval = static_randf(Pl_objp-Objects);

		// get position relative to cylinder of guard_objp		
		extended_z = get_cylinder_points(Pl_objp, guard_objp, &axis_pt, &r_vec, &radius);
		vm_vec_crossprod(&theta_vec, &guard_objp->orient.v.fvec, &r_vec);

		// half ships circle each way
		if (objval > 0.5f) {
			vm_vec_negate(&theta_vec);
		}

		float min_guard_dist = radius + Pl_objp->radius + 50.0f;
		float desired_guard_dist = min_guard_dist + 0.5f * ((1.0f + objval) * MAX_GUARD_DIST);
		float max_guard_dist =     min_guard_dist + 1.0f * ((1.0f + objval) * MAX_GUARD_DIST);

		// get z extents
		float min_z, max_z, length;
		polymodel *pm = model_get(Ships[guard_objp->instance].modelnum);
		min_z = pm->mins.xyz.z;
		max_z = pm->maxs.xyz.z;
		length = max_z - min_z;

		// get desired z
		// how often to choose new desired_z
		// 1*(64) sec < 2000, 2*(64) < 2-4000 3*(64) > 4-8000, etc (Missiontime >> 22 is 64 sec intervals)
		int time_choose = int(floor(log(length * 0.001) / log(2)));
		float desired_z = min_z + length * static_randf( (Pl_objp-Objects) ^ (Missiontime >> (22 + time_choose)) );

		// get r from guard_ship
		float cur_guard_rad = vm_vec_dist(&Pl_objp->pos, &axis_pt);

		// is ship within extents of cylinder of ship it is guarding
		int inside = (extended_z > min_z) && (extended_z < min_z + length);

		vector goal_pt;
		// maybe go into orbit mode
		if (cur_guard_rad < max_guard_dist) {
			if ( cur_guard_rad > min_guard_dist ) {
				if (inside) {
					// orbit
					vm_vec_scale_add(&goal_pt, &axis_pt, &r_vec, desired_guard_dist);
					vm_vec_scale_add2(&goal_pt, &theta_vec, desired_guard_dist);
				} else {
					// move to where I can orbit
					if (extended_z < min_z) {
						vm_vec_scale_add(&goal_pt, &guard_objp->pos, &guard_objp->orient.v.fvec, min_z);
					} else {
						vm_vec_scale_add(&goal_pt, &guard_objp->pos, &guard_objp->orient.v.fvec, max_z);
					}
					vm_vec_scale_add2(&goal_pt, &r_vec, desired_guard_dist);
					vm_vec_scale_add2(&goal_pt, &theta_vec, desired_guard_dist);
				}
			} else {
				// too close for orbit mode
				if (inside) {
					// inside (fly straight out and return circle)
					vm_vec_scale_add(&goal_pt, &axis_pt, &r_vec, max_guard_dist);
				} else {
					// outside (fly to edge and circle)
					if (extended_z < min_z) {
						vm_vec_scale_add(&goal_pt, &guard_objp->pos, &guard_objp->orient.v.fvec, min_z);
					} else {
						vm_vec_scale_add(&goal_pt, &guard_objp->pos, &guard_objp->orient.v.fvec, max_z);
					}
					vm_vec_scale_add2(&goal_pt, &r_vec, max_guard_dist);
					vm_vec_scale_add2(&goal_pt, &theta_vec, desired_guard_dist);
				}
			}

			if (Pl_objp->phys_info.fspeed > 0) {
				// modify goal_pt to take account moving guard objp
				float dist = vm_vec_dist_quick(&Pl_objp->pos, &goal_pt);
				float time = dist / Pl_objp->phys_info.fspeed;
				vm_vec_scale_add2(&goal_pt, &guard_objp->phys_info.vel, time);

				// now modify to move to desired z (at a max of 20 m/s)
				float delta_z = desired_z - extended_z;
				float v_z = delta_z * 0.2f;
				if (v_z < -20) {
					v_z = -20.0f;
				} else if (v_z > 20) {
					v_z = 20.0f;
				}

				vm_vec_scale_add2(&goal_pt, &guard_objp->orient.v.fvec, v_z*time);
			}

		} else {
			// cast vector to center of guard_ship adjusted by desired_z
			float delta_z = desired_z - extended_z;
			vm_vec_scale_add(&goal_pt, &guard_objp->pos, &guard_objp->orient.v.fvec, delta_z);
		}

		// try not to bump into things along the way
		if ( (cur_guard_rad > max_guard_dist) || (extended_z < min_z) || (extended_z > max_z) ) {
			if (maybe_avoid_big_ship(Pl_objp, guard_objp, aip, &goal_pt, 5.0f)) {
				return;
			}

			if (avoid_player(Pl_objp, &goal_pt)) {
				return;
			}
		} else {
			if (maybe_avoid_big_ship(Pl_objp, guard_objp, aip, &goal_pt, 5.0f)) {
				return;
			}
		}

		// got the point, now let's go there
		ai_turn_towards_vector(&goal_pt, Pl_objp, flFrametime, Ship_info[Ships[Pl_objp->instance].ship_info_index].srotation_time, NULL, NULL, 0.0f, 0);
//		aip->goal_point = goal_pt;
		accelerate_ship(aip, 1.0f);

		//	Periodically, scan for a nearby ship to attack.
		if (((AI_FrameCount ^ (Pl_objp-Objects)) & 0x07) == 0) {
			ai_guard_find_nearby_object();
		}
		}
		break;

	case AIS_GUARD_ATTACK:
		//	The guarded ship has been attacked.  Do something useful!
		ai_chase();
		break;

	default:
		//Int3();	//	Illegal submode for Guard mode.
		// AL 06/03/97 comment out Int3() to allow milestone to get out the door
		aip->submode = AIS_GUARD_PATROL;
		break;
	}
}

//	Main handler for guard behavior.
//	When someone has attacked guarded ship, then attack that ship.
// To attack another ship, switch out of guard mode into chase mode.
void ai_guard()
{
	ship			*shipp = &Ships[Pl_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];
	object		*guard_objp;	
	ship			*gshipp;
	float			dist_to_guardobj, dot_to_guardobj;
	vector		vec_to_guardobj;

	/*	//	Debug code, find an object to guard.
	int finding_guard_objnum = 0;	//	Debug code, to see if body of "if" below gets executed. 
	if (aip->guard_objnum == -1) {
		finding_guard_objnum = 1;
		debug_find_guard_object();
		if (aip->guard_objnum == -1)
			return;
	}
*/
	if (aip->guard_objnum == -1) {
		aip->mode = AIM_NONE;
		return;
	}

	Assert(aip->guard_objnum != -1);

	guard_objp = &Objects[aip->guard_objnum];

	if (guard_objp == Pl_objp) {
		Int3();		//	This seems illegal.  Why is a ship guarding itself?
		aip->guard_objnum = -1;
		return;
	}

	// check that I have someone to guard
	if (guard_objp->instance == -1) {
		return;
	}

	//	Not sure whether this should be impossible, or a reasonable cleanup condition.
	//	For now (3/31/97), it's getting trapped by an Assert, so clean it up.
	if (guard_objp->type != OBJ_SHIP) {
		aip->guard_objnum = -1;
		return;
	}

	// handler for gurad object with BIG radius
	if (guard_objp->radius > BIG_GUARD_RADIUS) {
		ai_big_guard();
		return;
	}

	gshipp = &Ships[guard_objp->instance];

	float			objval;
	vector		goal_point;
	vector		rel_vec;
	float			dist_to_goal_point, dot_to_goal_point, accel_scale;
	vector		v2g, rvec;

	// get random [0 to 1] based on OBJNUM
	objval = static_randf(Pl_objp-Objects);

	switch (aip->submode) {
	case AIS_GUARD_STATIC:
	case AIS_GUARD_PATROL:
		//	Stay near ship
		dist_to_guardobj = vm_vec_normalized_dir(&vec_to_guardobj, &guard_objp->pos, &Pl_objp->pos);
		dot_to_guardobj = vm_vec_dot(&Pl_objp->orient.v.fvec, &vec_to_guardobj);

		rel_vec = aip->guard_vec;
		vm_vec_add(&goal_point, &guard_objp->pos, &rel_vec);

		vm_vec_normalized_dir(&v2g, &goal_point, &Pl_objp->pos);
		dist_to_goal_point = vm_vec_dist_quick(&goal_point, &Pl_objp->pos);
		dot_to_goal_point = vm_vec_dot(&v2g, &Pl_objp->orient.v.fvec);
		accel_scale = (1.0f + dot_to_goal_point)/2.0f;

		//	If far away, get closer
		if (dist_to_goal_point > MAX_GUARD_DIST + 1.5 * (Pl_objp->radius + guard_objp->radius)) {
			if (maybe_avoid_big_ship(Pl_objp, guard_objp, aip, &goal_point, 5.0f)) {
				return;
			}

			if (avoid_player(Pl_objp, &goal_point)) {
				return;
			}

			// quite far away, so try to go straight to 
			compute_desired_rvec(&rvec, &goal_point, &Pl_objp->pos);
			ai_turn_towards_vector(&goal_point, Pl_objp, flFrametime, Ship_info[shipp->ship_info_index].srotation_time, NULL, NULL, 0.0f, 0, &rvec);

			accelerate_ship(aip, accel_scale * (0.25f + dist_to_goal_point/700.0f));
		} else {
			if (maybe_avoid_big_ship(Pl_objp, guard_objp, aip, &goal_point, 2.0f)) {
				return;
			}

			// get max of guard_objp (1) normal speed (2) dock speed
			float speed = guard_objp->phys_info.speed;

			if (guard_objp->type == OBJ_SHIP) {
				ai_info	*guard_aip = &Ai_info[Ships[guard_objp->instance].ai_index];

				if (guard_aip->dock_objnum != -1) {
					speed = max(speed, Objects[guard_aip->dock_objnum].phys_info.speed);
				}
			}
			
			//	Deal with guarding a small object.
			//	If going to guard_vec might cause a collision with guarded object, pick a new guard point.
			if (vm_vec_dot(&v2g, &vec_to_guardobj) > 0.8f) {
				if (dist_to_guardobj < dist_to_goal_point) {
					ai_set_guard_vec(Pl_objp, guard_objp);	//	OK to return here.
					return;
				}
			} 

			if (speed > 10.0f) {
				//	If goal ship is moving more than a tiny bit, don't orbit it, get near it.
				if (vm_vec_dist_quick(&goal_point, &Pl_objp->pos) > 40.0f) {
					if (vm_vec_dot(&Pl_objp->orient.v.fvec, &v2g) < 0.0f) {
						//	Just slow down, don't turn.
						set_accel_for_target_speed(Pl_objp, guard_objp->phys_info.speed - dist_to_goal_point/10.0f);
					} else {
						//	Goal point is in front.

						//	If close to goal point, don't change direction, just change speed.
						if (dist_to_goal_point > Pl_objp->radius + 10.0f) {
							turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
						}
						
						set_accel_for_target_speed(Pl_objp, guard_objp->phys_info.speed + (dist_to_goal_point-40.0f)/20.0f);
					}
				} else {
					if (dot_to_goal_point > 0.8f) {
						turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
						set_accel_for_target_speed(Pl_objp, guard_objp->phys_info.speed + dist_to_goal_point*0.1f);
					} else {
						set_accel_for_target_speed(Pl_objp, guard_objp->phys_info.speed - dist_to_goal_point*0.1f - 1.0f);
					}
				}
			// consider guard object STILL
			} else if (guard_objp->radius < 50.0f) {
				if (dist_to_goal_point > 15.0f) {
					turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
					set_accel_for_target_speed(Pl_objp, (dist_to_goal_point-10.0f)/2.0f);
				} else if (Pl_objp->phys_info.speed < 1.0f) {
					turn_away_from_point(Pl_objp, &guard_objp->pos, 0.0f);
				}
				//	It's a big ship
			} else if (dist_to_guardobj > MAX_GUARD_DIST + Pl_objp->radius + guard_objp->radius) {
				//	Orbiting ship, too far away
				float dot = turn_towards_tangent(Pl_objp, &guard_objp->pos, (1.0f + objval/2) * guard_objp->radius);
				accelerate_ship(aip, (1.0f + dot)/2.0f);
			} else if (dist_to_guardobj < Pl_objp->radius + guard_objp->radius) {
				//	Orbiting ship, got too close
				turn_away_from_point(Pl_objp, &guard_objp->pos, 0.0f);
				if ((dist_to_guardobj > guard_objp->radius + Pl_objp->radius + 50.0f) && (guard_objp->phys_info.speed > Pl_objp->phys_info.speed - 1.0f))
					change_acceleration(aip, 0.25f);
				else
					accelerate_ship(aip, 0.5f + objval/4.0f);
			} else {
				//	Orbiting ship, about the right distance away.
				float dot = turn_towards_tangent(Pl_objp, &guard_objp->pos, (1.5f + objval/2.0f)*guard_objp->radius);
				if ((dist_to_guardobj > guard_objp->radius + Pl_objp->radius + 50.0f) && (guard_objp->phys_info.speed > Pl_objp->phys_info.speed - 1.0f))
					set_accel_for_target_speed(Pl_objp, (0.5f * (1.0f + dot)) * (guard_objp->phys_info.speed + (dist_to_guardobj - guard_objp->radius - Pl_objp->radius)/10.0f));
				else
					accelerate_ship(aip, 0.5f * (1.0f + dot) * (0.3f + objval/3.0f));
			}
		}

		//	Periodically, scan for a nearby ship to attack.
		if (((AI_FrameCount ^ (Pl_objp-Objects)) & 0x07) == 0) {
			ai_guard_find_nearby_object();
		}
		break;

	case AIS_GUARD_ATTACK:
		//	The guarded ship has been attacked.  Do something useful!
		ai_chase();

		break;
	default:
		//Int3();	//	Illegal submode for Guard mode.
		// AL 06/03/97 comment out Int3() to allow milestone to get out the door
		aip->submode = AIS_GUARD_PATROL;
		break;
	}

}

// Return the object of the ship that the given object is docked
// with.  Currently, we know a ship is docked when his ai_mode is AIM_DOCK,
// and his submode is AIS_DOCK_3.  I suppose that this is likely to change though.
// Also, the objnum that was is passed in may not be the object that actually
// performed the docking maneuver.  This code will account for that case.
object *ai_find_docked_object( object *docker )
{
	ai_info	*aip;

	// we are trying to find the dockee of docker.  (Note that that these terms
	// are totally relative to what is passed in as a parameter.)

	// first thing to attempt is to check and see if this object is docked with something.
	Assert( docker->type == OBJ_SHIP );		// this had probably better be a ship!!!
	aip = &Ai_info[Ships[docker->instance].ai_index];
	if ( !(aip->ai_flags & AIF_DOCKED) )		// flag not set if not docked with anything
		return NULL;

	if ( aip->dock_objnum == -1 ) {
		Int3();											// mwa says this is wrong wrong wrong
		ai_do_objects_undocked_stuff( docker, NULL );
		return NULL;
	}

	return &Objects[aip->dock_objnum];

}


// define for the points subtracted from score for a rearm started on a player.
#define REPAIR_PENALTY		50


// function to clean up ai flags, variables, and other interesting information
// for a ship that was getting repaired.  The how parameter is useful for multiplayer
// only in that it tells us why the repaired ship is being cleaned up.
void ai_do_objects_repairing_stuff( object *repaired_objp, object *repair_objp, int how )
{
	ai_info *aip, *repair_aip;
	int		stamp = -1;

	Assert( repaired_objp->type == OBJ_SHIP);
	aip = &Ai_info[Ships[repaired_objp->instance].ai_index];

	// multiplayer
	int p_index;
	p_index = -1;
	if(Game_mode & GM_MULTIPLAYER){
		p_index = multi_find_player_by_object(repaired_objp);		
	}		
	else {		
		if(repaired_objp == Player_obj){
			p_index = Player_num;
		}
	}

	switch( how ) {
	case REPAIR_INFO_BEGIN:
		aip->ai_flags |= AIF_BEING_REPAIRED;
		aip->ai_flags &= ~AIF_AWAITING_REPAIR;
		stamp = timestamp(-1);

		// if this is a player ship, then subtract the repair penalty from this player's score
		if ( repaired_objp->flags & OF_PLAYER_SHIP ) {
			if ( !(Game_mode & GM_MULTIPLAYER) ) {
				Player->stats.m_score -= (int)(REPAIR_PENALTY * scoring_get_scale_factor());			// subtract the penalty
			} else {
				/*
				int pnum;

				// multiplayer game -- find the player, then subtract the score
				pnum = multi_find_player_by_object( repaired_objp );
				if ( pnum != -1 ) {
					Net_players[pnum].player->stats.m_score -= (int)(REPAIR_PENALTY * scoring_get_scale_factor());

					// squad war
					multi_team_maybe_add_score(-(int)(REPAIR_PENALTY * scoring_get_scale_factor()), Net_players[pnum].p_info.team);
				} else {
					nprintf(("Network", "Couldn't find player for ship %s for repair penalty\n", Ships[repaired_objp->instance].ship_name));
				}
				*/
			}
		}
		break;

	case REPAIR_INFO_BROKEN:
		aip->ai_flags &= ~AIF_BEING_REPAIRED;
		aip->ai_flags |= AIF_AWAITING_REPAIR;
		stamp = timestamp((int) ((30 + 10*frand()) * 1000));
		break;

	case REPAIR_INFO_END:
		// when only awaiting repair, and the repair is ended, then set dock_objnum to -1.
		if ( aip->ai_flags & AIF_AWAITING_REPAIR ){
			aip->dock_objnum = -1;
		}
		aip->ai_flags &= ~(AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED );
		stamp = timestamp((int) ((30 + 10*frand()) * 1000));
		break;

	case REPAIR_INFO_QUEUE:
		aip->ai_flags |= AIF_AWAITING_REPAIR;
		if ( aip == Player_ai ){
			hud_support_view_start();
		}
		stamp = timestamp(-1);
		break;

	case REPAIR_INFO_ABORT:
	case REPAIR_INFO_KILLED:
		// 5/4/98 -- MWA -- Need to set dock objnum to -1 to let code know this guy who was getting
		// repaired (or queued for repair), isn't really going to be docked with anyone anymore.
		aip->dock_objnum = -1;
		aip->ai_flags &= ~AIF_DOCKED;
		aip->ai_flags &= ~(AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED );
		if (repair_objp != NULL) {
			repair_aip = &Ai_info[Ships[repair_objp->instance].ai_index];
			repair_aip->ai_flags &= ~(AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED );
		}		

		if ( p_index >= 0 ) {
			hud_support_view_abort();

			// send appropriate message to player here
			if ( how == REPAIR_INFO_KILLED ){
				message_send_builtin_to_player( MESSAGE_SUPPORT_KILLED, NULL, MESSAGE_PRIORITY_HIGH, MESSAGE_TIME_SOON, 0, 0, p_index, -1 );
			} else {
				if ( repair_objp ){
					message_send_builtin_to_player( MESSAGE_REPAIR_ABORTED, &Ships[repair_objp->instance], MESSAGE_PRIORITY_NORMAL, MESSAGE_TIME_SOON, 0, 0, p_index, -1 );
				}
			}
		}

		// add log entry if this is a player
		if ( repaired_objp->flags & OF_PLAYER_SHIP ){
			mission_log_add_entry(LOG_PLAYER_REARM_ABORT, Ships[repaired_objp->instance].ship_name, NULL);
		}

		stamp = timestamp((int) ((30 + 10*frand()) * 1000));
		break;

	case REPAIR_INFO_COMPLETE:
		// clear the being repaired flag -- and 
		if ( p_index >= 0 ) {
			Assert( repair_objp );
			
			hud_support_view_stop();			

			message_send_builtin_to_player(MESSAGE_REPAIR_DONE, &Ships[repair_objp->instance], MESSAGE_PRIORITY_LOW, MESSAGE_TIME_SOON, 0, 0, p_index, -1);
		}
		stamp = timestamp((int) ((30 + 10*frand()) * 1000));
		break;

	case REPAIR_INFO_ONWAY:
		// need to set the dock_signature so that clients in multiplayer games rearm correctly
		Assert( repair_objp );
		aip->dock_signature = repair_objp->signature; 
		aip->dock_objnum = OBJ_INDEX(repair_objp);
		stamp = timestamp(-1);
		break;

	default:
		Int3();			// bogus type of repair info
	}

	if (repair_objp){
		Ai_info[Ships[repair_objp->instance].ai_index].warp_out_timestamp = stamp;
	}

	// repair_objp might be NULL is we are cleaning up this mode because of the support ship
	// getting killed.
	if ( repair_objp ) {
		aip = &Ai_info[Ships[repair_objp->instance].ai_index];
		switch ( how ) {
		case REPAIR_INFO_ONWAY:
			Assert( repaired_objp != NULL );
			aip->goal_objnum = OBJ_INDEX(repaired_objp);
			aip->ai_flags |= AIF_REPAIRING;
			break;

		case REPAIR_INFO_BROKEN:
			break;

		case REPAIR_INFO_END:
		case REPAIR_INFO_ABORT:
		case REPAIR_INFO_KILLED:
			if ( how == REPAIR_INFO_ABORT )
				aip->goal_objnum = -1;

			aip->ai_flags &= ~AIF_REPAIRING;
			break;
			
		case REPAIR_INFO_QUEUE:
			ai_add_rearm_goal( repaired_objp, repair_objp );
			break;

		case REPAIR_INFO_BEGIN:
		case REPAIR_INFO_COMPLETE:
			break;

		default:
			Int3();		// bogus type of repair info
		}
	}

	multi_maybe_send_repair_info( repaired_objp, repair_objp, how );
}

//	Cleanup AI stuff for when a ship was supposed to dock with another, but the ship
//	it was supposed to dock with is no longer valid.
void ai_cleanup_dock_mode(ai_info *aip, ship *shipp)
{
	object *objp;

	objp = &Objects[shipp->objnum];
	aip->mode = AIM_NONE;

	if (aip->ai_flags & AIF_REPAIRING) {
		Assert( aip->goal_objnum != -1 );
		ai_do_objects_repairing_stuff( &Objects[aip->goal_objnum], &Objects[shipp->objnum], REPAIR_INFO_KILLED );
	} else if ( aip->ai_flags & AIF_BEING_REPAIRED ) {
		// MWA -- note that we have to use dock_objnum here instead of goal_objnum.
		Assert( aip->dock_objnum != -1 );
		ai_do_objects_repairing_stuff( &Objects[shipp->objnum], &Objects[aip->dock_objnum], REPAIR_INFO_KILLED );
	} else if ( aip->ai_flags & AIF_AWAITING_REPAIR ) {
		// need to find the support ship that has me as a goal_objnum
		// MWA -- note that we have to use dock_objnum here instead of goal_objnum.
		// MWA -- 3/38/98  Check to see if this guy is queued for a support ship, or there is already
		// one in the mission
		if ( mission_is_repair_scheduled(objp) ) {
			mission_remove_scheduled_repair( objp );			// this function will notify multiplayer clients.
		} else {
			if ( aip->dock_objnum != -1 )
				ai_do_objects_repairing_stuff( objp, &Objects[aip->dock_objnum], REPAIR_INFO_ABORT );
			else
				ai_do_objects_repairing_stuff( objp, NULL, REPAIR_INFO_ABORT );
		}
	}

	if ( aip->ai_flags & AIF_DOCKED ) {
		ai_info *other_aip;

		Assert( aip->dock_objnum != -1 );

		// if docked, and the dock_objnum is not undocking, force them to near last stage
		other_aip = &Ai_info[Ships[Objects[aip->dock_objnum].instance].ai_index];
		if ( (other_aip->mode == AIM_DOCK) && (other_aip->submode < AIS_UNDOCK_3) )
			other_aip->submode = AIS_UNDOCK_3;
		ai_do_objects_undocked_stuff( objp, &Objects[aip->dock_objnum] );
	}
}

/*
//	Make dockee_objp shake a bit due to docking.
void ai_dock_shake(object *docker_objp, object *dockee_objp)
{
	vector	tangles;
	matrix	rotmat, tmp;
	float		scale;
	angles	*ap;

	scale = 0.25f;		//	Compute this based on mass and speed at time of docking.

	vm_vec_rand_vec_quick(&tangles);
	vm_vec_scale(&tangles, scale);

	ap = (angles *) &tangles;

	vm_angles_2_matrix(&rotmat, ap);
	vm_matrix_x_matrix( &tmp, &dockee_objp->orient, &rotmat );
	dockee_objp->orient = tmp;

	vm_orthogonalize_matrix(&dockee_objp->orient);

	dock_orient_and_approach(docker_objp, dockee_objp, DOA_DOCK_STAY);

}
*/

//	Make Pl_objp point at aip->goal_point.
void ai_still()
{
	ship	*shipp;
	ai_info	*aip;

	Assert(Pl_objp->type == OBJ_SHIP);
	Assert((Pl_objp->instance >= 0) && (Pl_objp->instance < MAX_OBJECTS));

	shipp = &Ships[Pl_objp->instance];
	Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

	aip = &Ai_info[shipp->ai_index];

	turn_towards_point(Pl_objp, &aip->goal_point, NULL, 0.0f);
}

//	Make *Pl_objp stay near another ship.
void ai_stay_near()
{
	ai_info	*aip;
	int		goal_objnum;

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	goal_objnum = aip->goal_objnum;

	if ((goal_objnum < 0) || (Objects[goal_objnum].type != OBJ_SHIP) || (Objects[goal_objnum].signature != aip->goal_signature)) {
		aip->mode = AIM_NONE;
	} else {
		float		dist, max_dist, scale;
		vector	rand_vec, goal_pos, vec_to_goal;
		object	*goal_objp;

		goal_objp = &Objects[goal_objnum];

		//	Make not all ships pursue same point.
		static_randvec(Pl_objp-Objects, &rand_vec);

		//	Make sure point is in front hemisphere (relative to Pl_objp's position.
		vm_vec_sub(&vec_to_goal, &goal_objp->pos, &Pl_objp->pos);
		if (vm_vec_dot(&rand_vec, &vec_to_goal) > 1.0f) {
			vm_vec_negate(&rand_vec);
		}

		//	Scale the random vector by an amount proportional to the distance from Pl_objp to the true goal.
		dist = vm_vec_dist_quick(&goal_objp->pos, &Pl_objp->pos);
		max_dist = aip->stay_near_distance;
		scale = dist - max_dist/2;
		if (scale < 0.0f)
			scale = 0.0f;

		vm_vec_scale_add(&goal_pos, &goal_objp->pos, &rand_vec, scale);

		if (max_dist < Pl_objp->radius + goal_objp->radius + 25.0f)
			max_dist = Pl_objp->radius + goal_objp->radius + 25.0f;

		if (dist > max_dist) {
			turn_towards_point(Pl_objp, &goal_pos, NULL, 0.0f);
			accelerate_ship(aip, dist / max_dist - 0.8f);
		}
	
	}

}

//	Warn player if dock path is obstructed.
int maybe_dock_obstructed(object *cur_objp, object *goal_objp, int big_only_flag)
{
	vector	*goalpos, *curpos;
	float		radius;
	ai_info	*aip;
	int		collide_objnum;

	aip = &Ai_info[Ships[cur_objp->instance].ai_index];

	Ai_info[Ships[goal_objp->instance].ai_index].ai_flags &= ~AIF_REPAIR_OBSTRUCTED;

	if (goal_objp != Player_obj)
		return -1;

	curpos = &cur_objp->pos;
	radius = cur_objp->radius;
	goalpos = &Path_points[aip->path_cur].pos;
	collide_objnum = pp_collide_any(curpos, goalpos, radius, cur_objp, goal_objp, big_only_flag);

	if (collide_objnum != -1)
		Ai_info[Ships[goal_objp->instance].ai_index].ai_flags |= AIF_REPAIR_OBSTRUCTED;

	return collide_objnum;
}


int Dock_path_warning_given = 0;

//	Docking behavior.
//	Approach a ship, follow path to docking platform, approach platform, after awhile,
//	undock.
void ai_dock()
{
	ship			*shipp = &Ships[Pl_objp->instance];
	ai_info		*aip = &Ai_info[shipp->ai_index];
	object		*goal_objp;
	ship_info	*sip = &Ship_info[shipp->ship_info_index];

	//	Make sure object we're supposed to dock with still exists.
	if ((aip->goal_objnum == -1) || (Objects[aip->goal_objnum].signature != aip->goal_signature)) {
		ai_cleanup_dock_mode(aip, shipp);
		return;
	}

	goal_objp = &Objects[aip->goal_objnum];

	//	For docking submodes (ie, not undocking), follow path.  Once at second last
	//	point on path (point just before point on dock platform), orient into position.
	// For undocking, first mode pushes docked ship straight back from docking point
	// second mode turns ship and moves to point on docking radius
	switch (aip->submode) {

		//	This mode means to find the path to the docking point.
	case AIS_DOCK_0:
		//aip->path_start = -1;
		//nprintf(("AI", "Time = %7.3f, submode = %i\n", f2fl(Missiontime), aip->submode));
		ai_path();
		if (!Dock_path_warning_given && (aip->path_length < 4)) {
			Warning( LOCATION, "Ship '%s' has only %i points on dock path.  Docking will look strange.  Contact Adam.", shipp->ship_name, aip->path_length );
			Dock_path_warning_given = 1;		//	This is on a mission-wide basis, but it's just a hack for now...
		}

		aip->submode = AIS_DOCK_1;
		aip->path_start = -1;
		aip->submode_start_time = Missiontime;
		break;

		//	This mode means to follow the path until just before the end.
	case AIS_DOCK_1: {
		float	dist;
		int	r;

		if ((r = maybe_dock_obstructed(Pl_objp, goal_objp, 1)) != -1) {
			int	r1;
			if ((r1 = maybe_avoid_big_ship(Pl_objp, goal_objp, aip, &goal_objp->pos, 7.0f)) != 0) {
				nprintf(("AI", "Support ship %s avoiding large ship %s\n", Ships[Pl_objp->instance].ship_name, Ships[Objects[r1].instance].ship_name));
				break;
			} /*else {
				nprintf(("AI", "Dock 1: Obstructed by %s\n", Ships[Objects[r].instance].ship_name));
				accelerate_ship(aip, 0.0f);
				aip->submode = AIS_DOCK_0;
			} */
		} //else {
		{
			dist = ai_path();
			//nprintf(("AI", "Time = %7.3f, submode = %i\n", f2fl(Missiontime), aip->submode));
			//nprintf(("AI", "Dock 1: Frame: %i, goal point = %i, dist = %7.3f\n", Framecount, aip->path_cur-aip->path_start, dist));

			if (aip->path_cur-aip->path_start >= aip->path_length-1) {		//	If got this far, advance no matter what.
				aip->submode = AIS_DOCK_2;
				aip->submode_start_time = Missiontime;
				aip->path_cur--;
				Assert(aip->path_cur-aip->path_start >= 0);
			} else if (aip->path_cur-aip->path_start >= aip->path_length-2) {
				if (Pl_objp->phys_info.speed > goal_objp->phys_info.speed + 1.5f) {
					set_accel_for_target_speed(Pl_objp, goal_objp->phys_info.speed);
				} else {
					aip->submode = AIS_DOCK_2;
					aip->submode_start_time = Missiontime;
				}
			}
		}
		break;
					  }
	//	This mode means to drag oneself right to the second last point on the path.
	//	Path code allows it to overshoot.
	case AIS_DOCK_2: {
		float		dist;
		int	r;

		if ((r = maybe_dock_obstructed(Pl_objp, goal_objp,0)) != -1) {
			nprintf(("AI", "Dock 2: Obstructed by %s\n", Ships[Objects[r].instance].ship_name));
			accelerate_ship(aip, 0.0f);
			aip->submode = AIS_DOCK_1;
		} else {
			//nprintf(("AI", "Time = %7.3f, submode = %i\n", f2fl(Missiontime), aip->submode));
			dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_APPROACH);
			Assert(dist != UNINITIALIZED_VALUE);

			if (dist == DOCK_BACKUP_RETURN_VAL) {
				int path_num;
				aip->submode = AIS_DOCK_1;
				path_num = ai_return_path_num_from_dockbay(&Objects[aip->goal_objnum], aip->dockee_index);
				Assert(aip->goal_objnum >= 0 && aip->goal_objnum < MAX_OBJECTS);
				ai_find_path(Pl_objp, aip->goal_objnum, path_num, 0);
				break;
			}

			//nprintf(("AI", "Dock 2: dist = %7.3f\n", vm_vec_dist_quick(&Pl_objp->pos, &goal_point)));
			float	tolerance;
			if (Objects[aip->goal_objnum].flags & OF_PLAYER_SHIP)
				tolerance = 6*flFrametime + 1.0f;
			else
				tolerance = 4*flFrametime + 0.5f;

			if ( dist < tolerance) {
				aip->submode = AIS_DOCK_3;
				aip->submode_start_time = Missiontime;
				aip->path_cur++;
			}
		}
		break;
						  }

	case AIS_DOCK_3:
	case AIS_DOCK_3A:
		{
		Assert(aip->goal_objnum != -1);
		int	r;

		if ((r = maybe_dock_obstructed(Pl_objp, goal_objp,0)) != -1) {
			nprintf(("AI", "Dock 1: Obstructed by %s\n", Ships[Objects[r].instance].ship_name));
			accelerate_ship(aip, 0.0f);
			aip->submode = AIS_DOCK_2;
		} else {

			//nprintf(("AI", "Time = %7.3f, submode = %i\n", f2fl(Missiontime), aip->submode));
			float dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_DOCK);
			Assert(dist != UNINITIALIZED_VALUE);

			if (dist == DOCK_BACKUP_RETURN_VAL) {
				aip->submode = AIS_DOCK_2;
				break;
			}

			//nprintf(("AI", "Dock 3: dist = %7.3f\n", dist));

			if (dist < 2*flFrametime * (1.0f + fl_sqrt(goal_objp->phys_info.speed))) {
				// - Removed by MK on 11/7/97, causes errors for ships docked at mission start: maybe_recreate_path(Pl_objp, aip, 1);
				dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_DOCK);
				Assert(dist != UNINITIALIZED_VALUE);

				physics_ship_init(Pl_objp);

				ai_do_objects_docked_stuff( Pl_objp, goal_objp );

				if (aip->submode == AIS_DOCK_3) {
					snd_play_3d( &Snds[SND_DOCK_ATTACH], &Pl_objp->pos, &View_position );
					hud_maybe_flash_docking_text(Pl_objp);
					// ai_dock_shake(Pl_objp, goal_objp);

					if ((Pl_objp == Player_obj) || (goal_objp == Player_obj))
						joy_ff_docked();  // shake player's joystick a little
				}

				//	If this ship is repairing another ship...
				if (aip->ai_flags & AIF_REPAIRING) {
					aip->submode = AIS_DOCK_4;			//	Special rearming only dock mode.
					aip->submode_start_time = Missiontime;
				} else {
					aip->submode = AIS_DOCK_4A;
					aip->submode_start_time = Missiontime;
				}
			}
		}
		break;
		}

		//	Yes, we just sit here.  We wait for further orders.  No, it's not a bug.
	case AIS_DOCK_4A:
		//nprintf(("AI", "Time = %7.3f, submode = %i\n", f2fl(Missiontime), aip->submode));
		//nprintf(("AI", "."));
		if (aip->active_goal >= 0) {
			mission_log_add_entry(LOG_SHIP_DOCK, Ships[Pl_objp->instance].ship_name, Ships[goal_objp->instance].ship_name);

			if (aip->goals[aip->active_goal].ai_mode == AI_GOAL_DOCK) {
				ai_mission_goal_complete( aip );					// Note, this calls ai_set_default_behavior().
			} 
		} else {	//	Can happen for initially docked ships.
			ai_do_default_behavior( &Objects[Ships[aip->shipnum].objnum] );		// do the default behavior
		}
		
		break;

	case AIS_DOCK_4: {
		//	This mode is only for rearming/repairing.
		//	The ship that is performing the rearm enters this mode after it docks.
		Assert((aip->goal_objnum >= -1) && (aip->goal_objnum < MAX_OBJECTS));

		//nprintf(("AI", "Time = %7.3f, submode = %i\n", f2fl(Missiontime), aip->submode));
		float dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_DOCK);
		Assert(dist != UNINITIALIZED_VALUE);

		object	*goal_objp = &Objects[aip->goal_objnum];
		Assert(goal_objp->type == OBJ_SHIP);
		ship			*goal_shipp = &Ships[goal_objp->instance];		
		ai_info		*goal_aip = &Ai_info[goal_shipp->ai_index];

		//nprintf(("AI", "Dock 4: dist = %7.3f\n", dist));

		//	Make sure repair has not broken off.
		if (dist > 5.0f) {	//	Oops, too far away!
			if ( goal_aip->ai_flags & AIF_BEING_REPAIRED )
				ai_do_objects_repairing_stuff( goal_objp, Pl_objp, REPAIR_INFO_BROKEN);

			if (dist > Pl_objp->radius*2 + goal_objp->radius*2) {
				//	Got real far away from goal, so move back a couple modes and try again.
				aip->submode = AIS_DOCK_2;
				aip->submode_start_time = Missiontime;
			}
		} else {
			if ( goal_aip->ai_flags & AIF_AWAITING_REPAIR )
				ai_do_objects_repairing_stuff( goal_objp, Pl_objp, REPAIR_INFO_BEGIN );
		}

		break;
						  }

	case AIS_UNDOCK_0: {
		int path_num;
		//	First stage of undocking.

		//nprintf(("AI", "Undock 0:\n"));

		aip->submode = AIS_UNDOCK_1;
		aip->submode_start_time = Missiontime;
		if (aip->dock_objnum == -1) {
			aip->submode = AIS_UNDOCK_3;
		} else {

			// set up the path points for the undocking procedure.  dock_path_index member should
			// have gotten set in the docking code.
			Assert( aip->dock_path_index != -1 );
			path_num = ai_return_path_num_from_dockbay(goal_objp, aip->dock_path_index);
			ai_find_path(Pl_objp, goal_objp-Objects, path_num, 0);

			// Play a ship docking detach sound
			snd_play_3d( &Snds[SND_DOCK_DETACH], &Pl_objp->pos, &View_position );
		}
		break;
							 }
	case AIS_UNDOCK_1: {
		//	Using thrusters, exit from dock station to nearest next dock path point.
		float	dist;
		
		//nprintf(("AI", "Undock 1: time in this mode = %7.3f\n", f2fl(Missiontime - aip->submode_start_time)));

		if (Missiontime - aip->submode_start_time < REARM_BREAKOFF_DELAY) {
			break;		//	Waiting for one second to elapse to let detach sound effect play out.
		}
		else {	// AL - added 05/16/97.  Hack to play depart sound.  Will probably take out.
					// Assumes that the submode_start_time is not used for AIS_UNDOCK_1 anymore
			if ( aip->submode_start_time != 0 )
				snd_play_3d( &Snds[SND_DOCK_DEPART], &Pl_objp->pos, &View_position );
			aip->submode_start_time = 0;
		}

		dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_UNDOCK_1);
		Assert(dist != UNINITIALIZED_VALUE);

		float dist_to_dock_obj = vm_vec_dist_quick(&Pl_objp->pos, &Objects[aip->goal_objnum].pos);

		//	Move to within 0.1 units of second last point on path before orienting, or just plain far away from docked-to ship.
		//	This allows undock to complete if first ship flies away.
		if ((dist < 2*flFrametime) || (dist_to_dock_obj > 2*Pl_objp->radius)) {
			aip->submode = AIS_UNDOCK_2;
			aip->submode_start_time = Missiontime;
		}
		break;
							 }
	case AIS_UNDOCK_2: {
		float dist;
		ai_info *other_aip;

		// get pointer to docked object's aip to reset flags, etc
		Assert( aip->dock_objnum != -1 );
		other_aip = &Ai_info[Ships[Objects[aip->dock_objnum].instance].ai_index];

		//	Second stage of undocking.
		dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_UNDOCK_2);
		Assert(dist != UNINITIALIZED_VALUE);


		//nprintf(("AI", "Undock 2: dist = %7.3f\n", dist));
		
		//	If at goal point, or quite far away from dock object
		if ((dist < 2.0f) || (vm_vec_dist_quick(&Pl_objp->pos, &goal_objp->pos) > (Pl_objp->radius + goal_objp->radius)*2) || (goal_objp->phys_info.speed > MAX_UNDOCK_ABORT_SPEED) ) {
			// reset the dock flags.  If rearm/repair, reset rearm repair flags for those ships as well.
			if ( sip->flags & SIF_SUPPORT ) {
				ai_do_objects_repairing_stuff( &Objects[aip->dock_objnum], Pl_objp, REPAIR_INFO_END );
			}

			// clear out flags for AIF_DOCKED for both objects.
			ai_do_objects_undocked_stuff( Pl_objp, goal_objp );
			physics_ship_init(Pl_objp);
			aip->submode = AIS_UNDOCK_3;				//	The do-nothing mode, until another order is issued

			//aip->ai_flags &= ~AIF_DOCKED;		//	@MK, 9/18/97
			//other_aip->ai_flags &= ~AIF_DOCKED;
			//aip->dock_objnum = -1;					// invalidate who obj is docked with
			//other_aip->dock_objnum = -1;			// MWA 10/07/97 invalide docked objects dock_objnum value as well

			// don't add undock log entries for support ships.
			if ( !(sip->flags & SIF_SUPPORT) )
				mission_log_add_entry(LOG_SHIP_UNDOCK, Ships[Pl_objp->instance].ship_name, Ships[goal_objp->instance].ship_name);

		}
		break;
		}
	case AIS_UNDOCK_3: {
		float dist = dock_orient_and_approach(Pl_objp, &Objects[aip->goal_objnum], DOA_UNDOCK_3);
		Assert(dist != UNINITIALIZED_VALUE);

		if (dist < Pl_objp->radius/2 + 5.0f) {
			aip->submode = AIS_UNDOCK_4;
		}

		// possible that this flag hasn't been cleared yet.  When aborting a rearm, this submode might
		// be entered directly.
		if ( (sip->flags & SIF_SUPPORT) && (aip->ai_flags & AIF_REPAIRING) ) {
			ai_do_objects_repairing_stuff( &Objects[aip->goal_objnum], Pl_objp, REPAIR_INFO_ABORT );
		}

		break;
						 }
	case AIS_UNDOCK_4: {
		ai_info *other_aip;

		// MWA 10/07/97  I'm slightly confused by the dual use of goal_objnum and dock_objnum.  Seems to me
		// that goal_objnum and dock_objnum are the same through this whole docking/undocking process, although
		// I could be wrong.  dock_objnum was reset in undock_2 submode so try to use goal_objnum here to
		// get other ships ai_info pointer
		Assert( aip->goal_objnum != -1 );
		other_aip = &Ai_info[Ships[Objects[aip->goal_objnum].instance].ai_index];

		aip->mode = AIM_NONE;
		aip->dock_path_index = -1;		// invalidate the docking path index

		// these flags should have been cleared long ago!
		// Get Allender if you hit one of these!!!!!
		// removed by allender on 2/16 since a ship may be docked with some other ship, but still be the
		// goal_objnum of this ship ending it's undocking mode.
		//Assert( !(aip->ai_flags & AIF_DOCKED) );
		//Assert( !(other_aip->ai_flags & AIF_DOCKED) );
		//Assert( !(aip->ai_flags & AIF_REPAIRING) );
		//Assert( !(other_aip->ai_flags & AIF_BEING_REPAIRED) );
		//Assert( !(other_aip->ai_flags & AIF_AWAITING_REPAIR) );

		// only call mission goal complete if this was indeed an undock goal
		if ( aip->active_goal > -1 ) {
			if ( aip->goals[aip->active_goal].ai_mode == AI_GOAL_UNDOCK )
				ai_mission_goal_complete( aip );			// this call should reset the AI mode
			//else
			//	aip->active_goal = -1;						// this ensures that this ship might get new goal
		}

		break;
							 }
	default:
		Int3();	//	Error, bogus submode
	}

}

// TURRET BEGIN

//	Given an object and a turret on that object, return the global position and forward vector
//	of the turret.   The gun normal is the unrotated gun normal, (the center of the FOV cone), not
// the actual gun normal given using the current turret heading.  But it _is_ rotated into the model's orientation
//	in global space.
void ship_get_global_turret_info(object *objp, model_subsystem *tp, vector *gpos, vector *gvec)
{
	matrix	m;
	vm_copy_transpose_matrix(&m, &objp->orient);
//	vm_vec_rotate(gpos, &tp->turret_avg_firing_point, &m);
	vm_vec_rotate(gpos, &tp->pnt, &m);
	vm_vec_add2(gpos, &objp->pos);
	vm_vec_rotate(gvec, &tp->turret_norm, &m);	
}

// Given an object and a turret on that object, return the actual firing point of the gun
// and its normal.   This uses the current turret angles.  We are keeping track of which
// gun to fire next in the ship specific info for this turret subobject.  Use this info
// to determine which position to fire from next.
//	Stuffs:
//		*gpos: absolute position of gun firing point
//		*gvec: vector fro *gpos to *targetp
void ship_get_global_turret_gun_info(object *objp, ship_subsys *ssp, vector *gpos, vector *gvec, int use_angles, vector *targetp)
{
	vector * gun_pos;
	model_subsystem *tp = ssp->system_info;

	ship_model_start(objp);

	gun_pos = &tp->turret_firing_point[ssp->turret_next_fire_pos % tp->turret_num_firing_points];

	model_find_world_point(gpos, gun_pos, tp->model_num, tp->turret_gun_sobj, &objp->orient, &objp->pos );

	if (use_angles)
		model_find_world_dir(gvec, &tp->turret_norm, tp->model_num, tp->turret_gun_sobj, &objp->orient, &objp->pos );
	else {
		//vector	gun_pos2;
		//vm_vec_add(&gun_pos2, gpos, gun_pos);
		vm_vec_normalized_dir(gvec, targetp, gpos);
	}

	ship_model_stop(objp);	
}

//	Rotate a turret towards an enemy.
//	Return TRUE if caller should use angles in subsequent rotations.
//	Some obscure model thing only John Slagel knows about.
//	Sets predicted enemy position.
//	If the turret (*ss) has a subsystem targeted, the subsystem is used as the predicted point.
int aifft_rotate_turret(ship *shipp, int parent_objnum, ship_subsys *ss, object *objp, object *lep, vector *predicted_enemy_pos, vector *gvec)
{
	if (ss->turret_enemy_objnum != -1)	{
		model_subsystem *tp = ss->system_info;
		vector	gun_pos, gun_vec;
		float		weapon_speed;
		float		weapon_system_strength;

		//	weapon_system_strength scales time enemy in range in 0..1.  So, the lower this is, the worse the aiming will be.
		weapon_system_strength = ship_get_subsystem_strength(shipp, SUBSYSTEM_WEAPONS);

		ship_get_global_turret_info(&Objects[parent_objnum], tp, &gun_pos, &gun_vec);

		weapon_speed = Weapon_info[tp->turret_weapon_type].max_speed;
		float weapon_travel_dist = weapon_speed * Weapon_info[tp->turret_weapon_type].lifetime;

		vector	enemy_point;
		if (ss->targeted_subsys != NULL) {
			if (ss->turret_enemy_objnum != -1) {
				vm_vec_unrotate(&enemy_point, &ss->targeted_subsys->system_info->pnt, &Objects[ss->turret_enemy_objnum].orient);
				vm_vec_add2(&enemy_point, &Objects[ss->turret_enemy_objnum].pos);
			}
		} else {
			if ((lep->type == OBJ_SHIP) && (Ship_info[Ships[lep->instance].ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
				ai_big_pick_attack_point_turret(lep, ss, &gun_pos, &gun_vec, &enemy_point, tp->turret_fov, min(weapon_travel_dist, Weapon_info[tp->turret_weapon_type].weapon_range));
			} else {
				enemy_point = lep->pos;
			}
		}

		set_predicted_enemy_pos_turret(predicted_enemy_pos, &gun_pos, objp, &enemy_point, &lep->phys_info.vel, weapon_speed, ss->turret_time_enemy_in_range * (weapon_system_strength + 1.0f)/2.0f);

		if (weapon_system_strength < 0.7f) {
			vector	rand_vec;

			static_randvec(Missiontime >> 18, &rand_vec);	//	Return same random number for two seconds.
			//	Add to predicted_enemy_pos value in .45 to 1.5x radius of enemy ship, so will often miss, but not by a huge amount.
			vm_vec_scale_add2(predicted_enemy_pos, &rand_vec, (1.0f - weapon_system_strength)*1.5f * lep->radius);
		}

		vector	v2e;
		vm_vec_normalized_dir(&v2e, predicted_enemy_pos, &gun_pos);
		if (vm_vec_dot(&v2e, gvec) > tp->turret_fov) {
			int	rval;

			rval = model_rotate_gun(shipp->modelnum, ss->system_info, &Objects[parent_objnum].orient, 
				&ss->submodel_info_1.angs, &ss->submodel_info_2.angs,
				&Objects[parent_objnum].pos, predicted_enemy_pos);
		}
	}

	return 0;
}

//	Determine if subsystem *enemy_subsysp is hittable from objp.
//	If so, return dot product of vector from point abs_gunposp to *enemy_subsysp
float	aifft_compute_turret_dot(object *objp, object *enemy_objp, vector *abs_gunposp, ship_subsys *turret_subsysp, ship_subsys *enemy_subsysp)
{
	float	dot_out;
	vector	subobj_pos, vector_out;

	vm_vec_unrotate(&subobj_pos, &enemy_subsysp->system_info->pnt, &enemy_objp->orient);
	vm_vec_add2(&subobj_pos, &enemy_objp->pos);

	if (ship_subsystem_in_sight(enemy_objp, enemy_subsysp, abs_gunposp, &subobj_pos, 1, &dot_out, &vector_out)) {
		vector	turret_norm;

		vm_vec_rotate(&turret_norm, &turret_subsysp->system_info->turret_norm, &objp->orient);
		return vm_vec_dot(&turret_norm, &vector_out);
	} else
		return -1.0f;

}

#define MAX_AIFFT_TURRETS			60
ship_subsys *aifft_list[MAX_AIFFT_TURRETS];
float aifft_rank[MAX_AIFFT_TURRETS];
int aifft_list_size = 0;
int aifft_max_checks = 5;
DCF(mf, "")
{
	dc_get_arg(ARG_INT);
	aifft_max_checks = Dc_arg_int;
}


//	Pick a subsystem to attack on enemy_objp.
//	Only pick one if enemy_objp is a big ship or a capital ship.
//	Returns dot product from turret to subsystem in *dot_out
ship_subsys *aifft_find_turret_subsys(object *objp, ship_subsys *ssp, object *enemy_objp, float *dot_out)
{
	ship	*eshipp, *shipp;
	ship_info	*esip;
	ship_subsys	*best_subsysp = NULL;
	float dot;

	Assert(enemy_objp->type == OBJ_SHIP);

	eshipp = &Ships[enemy_objp->instance];
	esip = &Ship_info[eshipp->ship_info_index];

	shipp = &Ships[objp->instance];

	float	best_dot = 0.0f;
	*dot_out = best_dot;

	//	Compute absolute gun position.
	vector	abs_gun_pos;
	vm_vec_unrotate(&abs_gun_pos, &ssp->system_info->pnt, &objp->orient);
	vm_vec_add2(&abs_gun_pos, &objp->pos);

	//	Only pick a turret to attack on large ships.
	if (!(esip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)))
		return best_subsysp;

	// Make sure big or huge ship *actually* has subsystems  (ie, knossos)
	if (esip->n_subsystems == 0) {
		return best_subsysp;
	}

	// first build up a list subsystems to traverse
	ship_subsys	*pss;
	aifft_list_size = 0;
	for ( pss = GET_FIRST(&eshipp->subsys_list); pss !=END_OF_LIST(&eshipp->subsys_list); pss = GET_NEXT(pss) ) {
		model_subsystem *psub = pss->system_info;

		// if we've reached max turrets bail
		if(aifft_list_size >= MAX_AIFFT_TURRETS){
			break;
		}

		// Don't process destroyed objects
		if ( pss->current_hits <= 0.0f ){
			continue;
		}
		
		switch (psub->type) {
		case SUBSYSTEM_WEAPONS:
			aifft_list[aifft_list_size] = pss;
			aifft_rank[aifft_list_size++] = 1.4f;
			break;

		case SUBSYSTEM_TURRET:
			aifft_list[aifft_list_size] = pss;
			aifft_rank[aifft_list_size++] = 1.2f;
			break;

		case SUBSYSTEM_SENSORS:
		case SUBSYSTEM_ENGINE:
			aifft_list[aifft_list_size] = pss;
			aifft_rank[aifft_list_size++] = 1.0f;
			break;
		}
	}

	// DKA:  6/28/99 all subsystems can be destroyed.
	//Assert(aifft_list_size > 0);
	if (aifft_list_size == 0) {
		return best_subsysp;
	}

	// determine a stride value so we're not checking too many turrets
	int stride = aifft_list_size > aifft_max_checks ? aifft_list_size / aifft_max_checks : 1;
	if(stride <= 0){
		stride = 1;
	}
	int offset = (int)frand_range(0.0f, (float)(aifft_list_size % stride));
	int idx;
	for(idx=offset; idx<aifft_list_size; idx+=stride){
		dot = aifft_compute_turret_dot(objp, enemy_objp, &abs_gun_pos, ssp, aifft_list[idx]);			

		if (dot* aifft_rank[idx] > best_dot) {
			best_dot = dot*aifft_rank[idx];
			best_subsysp = aifft_list[idx];
		}
	}

	Assert(best_subsysp != &eshipp->subsys_list);

	*dot_out = best_dot;
	return best_subsysp;
}

// Set active weapon for turret
void ai_turret_select_default_weapon(ship_subsys *turret)
{
	ship_weapon *twp;

	twp = &turret->weapons;

	// If a primary weapon is available, select it
	if ( twp->num_primary_banks > 0 ) {
		turret->system_info->turret_weapon_type = twp->primary_bank_weapons[0];
	} else if ( twp->num_secondary_banks > 0 ) {
		turret->system_info->turret_weapon_type = twp->secondary_bank_weapons[0];
	}
}

// return !0 if the specified target should scan for a new target, otherwise return 0
int turret_should_pick_new_target(ship_subsys *turret)
{
//	int target_type;

	if ( timestamp_elapsed(turret->turret_next_enemy_check_stamp) ) {
		return 1;
	}

	return 0;

/*
	if ( turret->turret_enemy_objnum == -1 ) {
		return 1;
	}
		
	target_type = Objects[turret->turret_enemy_objnum].type;
	if ( (target_type != OBJ_SHIP) && (target_type != OBJ_ASTEROID) ) {
		return 1;
	}

	return 0;
*/
}

// Set the next fire timestamp for a turret, based on weapon type and ai class
void turret_set_next_fire_timestamp(ship_subsys *turret, ai_info *aip)
{
	float	wait;
	int	weapon_id;

	weapon_id = turret->system_info->turret_weapon_type;

	wait = Weapon_info[weapon_id].fire_wait * 1000.0f;

	// make side even for team vs. team
	if ((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_TEAM)) {
		// flak guns need to fire more rapidly
		if (Weapon_info[weapon_id].wi_flags & WIF_FLAK) {
			wait *= Ship_fire_delay_scale_friendly[Game_skill_level] * 0.5f;
			wait += (Num_ai_classes - aip->ai_class - 1) * 40.0f;
		} else {
			wait *= Ship_fire_delay_scale_friendly[Game_skill_level];
			wait += (Num_ai_classes - aip->ai_class - 1) * 100.0f;
		}
	} else {
		// flak guns need to fire more rapidly
		if (Weapon_info[weapon_id].wi_flags & WIF_FLAK) {
			if (Ships[aip->shipnum].team == TEAM_FRIENDLY) {
				wait *= Ship_fire_delay_scale_friendly[Game_skill_level] * 0.5f;
			} else {
				wait *= Ship_fire_delay_scale_hostile[Game_skill_level] * 0.5f;
			}	
			wait += (Num_ai_classes - aip->ai_class - 1) * 40.0f;

		} else if (Weapon_info[weapon_id].wi_flags & WIF_HUGE) {
			// make huge weapons fire independently of team
			wait *= Ship_fire_delay_scale_friendly[Game_skill_level];
			wait += (Num_ai_classes - aip->ai_class - 1) * 100.0f;
		} else {
			// give team friendly an advantage
			if (Ships[aip->shipnum].team == TEAM_FRIENDLY) {
				wait *= Ship_fire_delay_scale_friendly[Game_skill_level];
			} else {
				wait *= Ship_fire_delay_scale_hostile[Game_skill_level];
			}	
			wait += (Num_ai_classes - aip->ai_class - 1) * 100.0f;
		}
	}

	// vary wait time +/- 10%
	wait *= frand_range(0.9f, 1.1f);
	turret->turret_next_fire_stamp = timestamp((int) wait);
}

// Decide  if a turret should launch an aspect seeking missile
int turret_should_fire_aspect(ship_subsys *turret, float dot, int weapon_class)
{
	weapon_info *wip;

	wip = &Weapon_info[weapon_class];

	if ( (dot > AICODE_TURRET_DUMBFIRE_ANGLE) && (turret->turret_time_enemy_in_range >= min(wip->min_lock_time,AICODE_TURRET_MAX_TIME_IN_RANGE)) ) {
		return 1;
	}

	return 0;
}

// Update how long current target has been in this turrets range
void turret_update_enemy_in_range(ship_subsys *turret, float seconds)
{
	turret->turret_time_enemy_in_range += seconds;

	if ( turret->turret_time_enemy_in_range < 0.0f ) {
		turret->turret_time_enemy_in_range = 0.0f;
	}

	if ( turret->turret_time_enemy_in_range > AICODE_TURRET_MAX_TIME_IN_RANGE ) {
		turret->turret_time_enemy_in_range = AICODE_TURRET_MAX_TIME_IN_RANGE;
	}
}



// Fire a weapon from a turret
void turret_fire_weapon(ship_subsys *turret, int parent_objnum, vector *turret_pos, vector *turret_fvec, vector *predicted_pos = NULL)
{
	matrix	turret_orient;
	int		turret_weapon_class, weapon_objnum;
	ai_info	*parent_aip;
	ship		*parent_ship;
	beam_fire_info fire_info;
	float flak_range = 0.0f;

	parent_aip = &Ai_info[Ships[Objects[parent_objnum].instance].ai_index];
	parent_ship = &Ships[Objects[parent_objnum].instance];
	turret_weapon_class = turret->system_info->turret_weapon_type;

	if (check_ok_to_fire(parent_objnum, turret->turret_enemy_objnum, &Weapon_info[turret_weapon_class])) {
		vm_vector_2_matrix(&turret_orient, turret_fvec, NULL, NULL);
		turret->turret_last_fire_direction = *turret_fvec;

		// set next fire timestamp for the turret
		turret_set_next_fire_timestamp(turret, parent_aip);

		// if this weapon is a beam weapon, handle it specially
		if(Weapon_info[turret_weapon_class].wi_flags & WIF_BEAM){
			// if this beam isn't free to fire
			if (!(turret->weapons.flags & SW_FLAG_BEAM_FREE)) {
				Int3();	// should never get this far
				return;
			}

			// stuff beam firing info
			memset(&fire_info, 0, sizeof(beam_fire_info));
			fire_info.accuracy = 1.0f;
			fire_info.beam_info_index = turret_weapon_class;
			fire_info.beam_info_override = NULL;
			fire_info.shooter = &Objects[parent_objnum];
			fire_info.target = &Objects[turret->turret_enemy_objnum];
			fire_info.target_subsys = NULL;
			fire_info.turret = turret;

			// fire a beam weapon
			beam_fire(&fire_info);
		} else {

			// don't fire swarm, but set up swarm info
			if (Weapon_info[turret_weapon_class].wi_flags & WIF_SWARM) {
				turret_swarm_set_up_info(parent_objnum, turret, turret_weapon_class);
				return;
			} else {
				weapon_objnum = weapon_create( turret_pos, &turret_orient, turret_weapon_class, parent_objnum, 0, -1, 1);
				weapon_set_tracking_info(weapon_objnum, parent_objnum, turret->turret_enemy_objnum, 1, turret->targeted_subsys);		
			}

			//nprintf(("AI", "Turret_time_enemy_in_range = %7.3f\n", ss->turret_time_enemy_in_range));		
			if (weapon_objnum != -1) {
				Weapons[Objects[weapon_objnum].instance].target_num = turret->turret_enemy_objnum;
				// AL 1-6-97: Store pointer to turret subsystem
				Weapons[Objects[weapon_objnum].instance].turret_subsys = turret;

				if ( Weapon_info[turret_weapon_class].launch_snd != -1 ) {
					// Don't play turret firing sound if turret sits on player ship... it gets annoying.
					if ( parent_objnum != OBJ_INDEX(Player_obj) ) {						
						snd_play_3d( &Snds[Weapon_info[turret_weapon_class].launch_snd], turret_pos, &View_position );						
					}
				}		

				// if the gun is a flak gun
				if(Weapon_info[turret_weapon_class].wi_flags & WIF_FLAK){			
					// show a muzzle flash
					flak_muzzle_flash(turret_pos, turret_fvec, turret_weapon_class);

					// pick a firing range so that it detonates properly			
					flak_pick_range(&Objects[weapon_objnum], predicted_pos, ship_get_subsystem_strength(parent_ship, SUBSYSTEM_WEAPONS));

					// determine what that range was
					flak_range = flak_get_range(&Objects[weapon_objnum]);
				}

				// in multiplayer (and the master), then send a turret fired packet.
				if ( MULTIPLAYER_MASTER && (weapon_objnum != -1) ) {
					int subsys_index;

					subsys_index = ship_get_index_from_subsys(turret, parent_objnum );
					Assert( subsys_index != -1 );
					if(Weapon_info[turret_weapon_class].wi_flags & WIF_FLAK){			
						send_flak_fired_packet( parent_objnum, subsys_index, weapon_objnum, flak_range );
					} else {
						send_turret_fired_packet( parent_objnum, subsys_index, weapon_objnum );
					}
				}
			}
		}
	} else {
		float wait = 1000.0f * frand_range(0.9f, 1.1f);
		turret->turret_next_fire_stamp = timestamp((int) wait);
	}
}

void turret_swarm_fire_from_turret(ship_subsys *turret, int parent_objnum, int target_objnum, ship_subsys *target_subsys)
{
	int turret_weapon_class, weapon_objnum;
	matrix turret_orient;
	vector turret_pos, turret_fvec;

	// parent not alive, quick out.
	if (Objects[parent_objnum].type != OBJ_SHIP) {
		return;
	}

	//	change firing point
	ship_get_global_turret_gun_info(&Objects[parent_objnum], turret, &turret_pos, &turret_fvec, 1, NULL);
	turret->turret_next_fire_pos++;

	// get class [index into Weapon_info array
	turret_weapon_class = turret->system_info->turret_weapon_type;
	Assert(Weapon_info[turret_weapon_class].wi_flags & WIF_SWARM);

	// make turret_orient from turret_fvec -- turret->turret_last_fire_direction
	vm_vector_2_matrix(&turret_orient, &turret_fvec, NULL, NULL);

	// create weapon and homing info
	weapon_objnum = weapon_create(&turret_pos, &turret_orient, turret_weapon_class, parent_objnum, 0, -1, 1);
	weapon_set_tracking_info(weapon_objnum, parent_objnum, target_objnum, 1, target_subsys);

	// do other cool stuff if weapon is created.
	if (weapon_objnum > -1) {
		Weapons[Objects[weapon_objnum].instance].turret_subsys = turret;
		Weapons[Objects[weapon_objnum].instance].target_num = turret->turret_enemy_objnum;

		// maybe sound
		if ( Weapon_info[turret_weapon_class].launch_snd != -1 ) {
			// Don't play turret firing sound if turret sits on player ship... it gets annoying.
			if ( parent_objnum != OBJ_INDEX(Player_obj) ) {
				snd_play_3d( &Snds[Weapon_info[turret_weapon_class].launch_snd], &turret_pos, &View_position );
			}
		}
		
		// in multiplayer (and the master), then send a turret fired packet.
		if ( MULTIPLAYER_MASTER && (weapon_objnum != -1) ) {
			int subsys_index;

			subsys_index = ship_get_index_from_subsys(turret, parent_objnum );
			Assert( subsys_index != -1 );
			send_turret_fired_packet( parent_objnum, subsys_index, weapon_objnum );
		}
	}
}

int Num_ai_firing = 0;
int Num_find_turret_enemy = 0;
int Num_turrets_fired = 0;
//	Given a turret tp and its parent parent_objnum, fire from the turret at its enemy.
void ai_fire_from_turret(ship *shipp, ship_subsys *ss, int parent_objnum)
{
	float		weapon_firing_range;
	vector	v2e;
	object	*lep;		//	Last enemy pointer
	model_subsystem	*tp = ss->system_info;
	int		use_angles, turret_weapon_class;
	vector	predicted_enemy_pos;
	object	*objp;
	ai_info	*aip;

	if (!Ai_firing_enabled) {
		return;
	}

	if (ss->current_hits < 0.0f) {
		return;
	}

	if ( ship_subsys_disrupted(ss) ){		// AL 1/19/98: Make sure turret isn't suffering disruption effects
		return;
	}

	// Check turret free
	if (ss->weapons.flags & SW_FLAG_TURRET_LOCK) {
		return;
	}

	// If beam weapon, check beam free
	if ( (Weapon_info[tp->turret_weapon_type].wi_flags & WIF_BEAM) && !(ss->weapons.flags & SW_FLAG_BEAM_FREE) ) {
		return;
	}

	Assert( shipp->objnum == parent_objnum );

	if ( tp->turret_weapon_type < 0 ){
		return;
	}

	// Monitor number of calls to ai_fire_from_turret
	Num_ai_firing++;

	turret_weapon_class = tp->turret_weapon_type;

	// AL 09/14/97: ensure ss->turret_enemy_objnum != -1 before setting lep
	if ( (ss->turret_enemy_objnum >= 0 && ss->turret_enemy_objnum < MAX_OBJECTS) && (ss->turret_enemy_sig == Objects[ss->turret_enemy_objnum].signature)) {
		lep = &Objects[ss->turret_enemy_objnum];

		// MK -- here is where turret is targeting a bomb.  I simply return for now.  We should force
		// a target change -- or better yet, never pick a weapon when this turret has a "huge" weapon
		// loaded.

		// we only care about targets which are ships.
		//if ( lep->type != OBJ_SHIP )
		//	return;

		//	If targeted a small ship and have a huge weapon, don't fire.  But this shouldn't happen, as a small ship should not get selected.
		if ( Weapon_info[turret_weapon_class].wi_flags & WIF_HUGE ) {
			if ( lep->type != OBJ_SHIP ) {
				return;
			}
			if ( !(Ship_info[Ships[lep->instance].ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) ) {
				return;
			}
		}

		// If targeting protected or beam protected ship, don't fire.  Reset enemy objnum
		if (lep->type == OBJ_SHIP) {
			// Check if we're targeting a protected ship
			if (lep->flags & OF_PROTECTED) {
				ss->turret_enemy_objnum = -1;
				ss->turret_time_enemy_in_range = 0.0f;
				return;
			}

			// Check if we're targeting a beam protected ship with a beam weapon
			if ( (lep->flags & OF_BEAM_PROTECTED) && (Weapon_info[turret_weapon_class].wi_flags & WIF_BEAM) ) {
				ss->turret_enemy_objnum = -1;
				ss->turret_time_enemy_in_range = 0.0f;
				return;
			}
		}
	} else {
		ss->turret_enemy_objnum = -1;
		lep = NULL;
	}
	
	Assert((parent_objnum >= 0) && (parent_objnum < MAX_OBJECTS));
	objp = &Objects[parent_objnum];
	Assert(objp->type == OBJ_SHIP);
	aip = &Ai_info[Ships[objp->instance].ai_index];

	// Use the turret info for all guns, not one gun in particular.
	vector	 gvec, gpos;
	ship_get_global_turret_info(&Objects[parent_objnum], tp, &gpos, &gvec);

	// Rotate the turret even if time hasn't elapsed, since it needs to turn to face its target.
	use_angles = aifft_rotate_turret(shipp, parent_objnum, ss, objp, lep, &predicted_enemy_pos, &gvec);

	if ( !timestamp_elapsed(ss->turret_next_fire_stamp)){
		return;
	}

	// Don't try to fire beyond weapon_limit_range
	weapon_firing_range = min(Weapon_info[tp->turret_weapon_type].lifetime * Weapon_info[tp->turret_weapon_type].max_speed, Weapon_info[tp->turret_weapon_type].weapon_range);

	// if beam weapon in nebula and target not tagged, decrase firing range
	extern int Nebula_sec_range;
	if (Weapon_info[turret_weapon_class].wi_flags & WIF_BEAM) {
		if ( !((shipp->tag_left > 0) || (shipp->level2_tag_left > 0)) ) {
			if (Nebula_sec_range) {
				weapon_firing_range *= float(BEAM_NEBULA_RANGE_REDUCE_FACTOR);
			}
		}
	}

	if (ss->turret_enemy_objnum != -1) {
		float dist_to_enemy = vm_vec_normalized_dir(&v2e, &predicted_enemy_pos, &gpos) - lep->radius;
		if (dist_to_enemy > weapon_firing_range) {
			ss->turret_enemy_objnum = -1;		//	Force picking of new enemy.
		}
	}

	// Turret spawn weapons are a special case.  They fire if there are enough enemies in the 
	// immediate area (not necessarily in the turret fov).
	if ( Weapon_info[turret_weapon_class].wi_flags & WIF_SPAWN ) {
		int num_ships_nearby;
		num_ships_nearby = num_nearby_fighters(get_enemy_team_mask(parent_objnum), &gpos, 1500.0f);
		if (( num_ships_nearby >= 3 ) || ((num_ships_nearby >= 2) && (frand() < 0.1f))) {
			turret_fire_weapon(ss, parent_objnum, &gpos, &ss->turret_last_fire_direction);
		} else {
			ss->turret_next_fire_stamp = timestamp(1000);	//	Regardless of firing rate, don't check whether should fire for awhile.
		}
		return;
	}

	//	Maybe pick a new enemy.
	if ( turret_should_pick_new_target(ss) ) {
		Num_find_turret_enemy++;
		int objnum = find_turret_enemy(ss, parent_objnum, &gpos, &gvec, ss->turret_enemy_objnum, tp->turret_fov, Weapon_info[turret_weapon_class].wi_flags & WIF_HUGE);
		Assert(objnum < 0 || is_target_beam_valid(ss, objnum));

		if (objnum != -1) {
			if (ss->turret_enemy_objnum == -1) {
				ss->turret_enemy_objnum = objnum;
				ss->turret_enemy_sig = Objects[objnum].signature;
				// why return?
				return;
			} else {
				ss->turret_enemy_objnum = objnum;
				ss->turret_enemy_sig = Objects[objnum].signature;
			}
		} else {
			ss->turret_enemy_objnum = -1;
		}

		if (ss->turret_enemy_objnum != -1) {
			float	dot = 1.0f;
			lep = &Objects[ss->turret_enemy_objnum];
			if ( lep->type == OBJ_SHIP ) {
				ss->targeted_subsys = aifft_find_turret_subsys(objp, ss, lep, &dot);				
			}
			ss->turret_next_enemy_check_stamp = timestamp((int) (max(dot, 0.5f)*2000.0f) + 1000);
		} else {
			ss->turret_next_enemy_check_stamp = timestamp((int) (2000.0f * frand_range(0.9f, 1.1f)));	//	Check every two seconds
		}
	}

	//	If still don't have an enemy, return.  Or, if enemy is protected, return.
	if (ss->turret_enemy_objnum != -1) {
		//	Don't shoot at ship we're going to dock with.
		if (ss->turret_enemy_objnum == aip->dock_objnum) {
			ss->turret_enemy_objnum = -1;
			return;
		}

		if (Objects[ss->turret_enemy_objnum].flags & OF_PROTECTED) {
			//	This can happen if the enemy was selected before it became protected.
			ss->turret_enemy_objnum = -1;
			return;
		}
		lep = &Objects[ss->turret_enemy_objnum];
	} else {
		if (timestamp_until(ss->turret_next_fire_stamp) < 500) {
			ss->turret_next_fire_stamp = timestamp(500);
		}
		return;
	}

	if ( lep == NULL ){
		return;
	}

	Assert(ss->turret_enemy_objnum != -1);

	float dot = vm_vec_dot(&v2e, &gvec);

	if (dot > tp->turret_fov ) {
		// Ok, the turret is lined up... now line up a particular gun.
		int ok_to_fire = 0;
		float dist_to_enemy;

		// We're ready to fire... now get down to specifics, like where is the
		// actual gun point and normal, not just the one for whole turret.
		ship_get_global_turret_gun_info(&Objects[parent_objnum], ss, &gpos, &gvec, use_angles, &predicted_enemy_pos);
		ss->turret_next_fire_pos++;

		// Fire in the direction the turret is facing, not right at the target regardless of turret dir.
		vm_vec_sub(&v2e, &predicted_enemy_pos, &gpos);
		dist_to_enemy = vm_vec_normalize(&v2e);
		dot = vm_vec_dot(&v2e, &gvec);

		// if the weapon is a flak gun, add some jitter to its aim so it fires in a "cone" to make a cool visual effect
		// and make them less lethal
		if(Weapon_info[turret_weapon_class].wi_flags & WIF_FLAK){
			flak_jitter_aim(&v2e, dist_to_enemy, ship_get_subsystem_strength(shipp, SUBSYSTEM_WEAPONS));
		}

		// Fire if:
		//		dumbfire and nearly pointing at target.
		//		heat seeking and target in a fairly wide cone.
		//		aspect seeking and target is locked.
		turret_weapon_class = tp->turret_weapon_type;

		// if dumbfire (lasers and non-homing missiles)
		if ( !(Weapon_info[turret_weapon_class].wi_flags & WIF_HOMING) ) {
			if ((dist_to_enemy < 75.0f) || (dot > AICODE_TURRET_DUMBFIRE_ANGLE )) {
				turret_update_enemy_in_range(ss, 2*Weapon_info[turret_weapon_class].fire_wait);
				ok_to_fire = 1;
			}
		} else if ( Weapon_info[turret_weapon_class].wi_flags & WIF_HOMING_HEAT ) {	// if heat seekers
			if ((dist_to_enemy < 50.0f) || (dot > AICODE_TURRET_HEATSEEK_ANGLE )) {
				turret_update_enemy_in_range(ss, 2*Weapon_info[turret_weapon_class].fire_wait);
				ok_to_fire = 1;
			}
		} else if ( Weapon_info[turret_weapon_class].wi_flags & WIF_HOMING_ASPECT ) {	// if aspect seeker
			if ((dist_to_enemy < 50.0f) || (dot > AICODE_TURRET_DUMBFIRE_ANGLE )) {
				turret_update_enemy_in_range(ss, 2*Weapon_info[turret_weapon_class].fire_wait);
			}
			if ( turret_should_fire_aspect(ss, dot, turret_weapon_class) ) {
				ok_to_fire = 1;
			}
		}

		if ( ok_to_fire ) {
			Num_turrets_fired++;
			
			turret_fire_weapon(ss, parent_objnum, &gpos, &v2e, &predicted_enemy_pos);						
		} else {
			turret_update_enemy_in_range(ss, -4*Weapon_info[tp->turret_weapon_type].fire_wait);
			ss->turret_next_fire_stamp = timestamp(500);
		}
	} else {
		// Lost him!
		ss->turret_enemy_objnum = -1;		//	Reset enemy objnum, find a new one next frame.
		ss->turret_time_enemy_in_range = 0.0f;
	}
}

// TURRET END

#ifndef NDEBUG
#define	MAX_AI_DEBUG_RENDER_STUFF	100
typedef struct ai_render_stuff {
	ship_subsys	*ss;
	int			parent_objnum;
} ai_render_stuff;

ai_render_stuff AI_debug_render_stuff[MAX_AI_DEBUG_RENDER_STUFF];

int	Num_AI_debug_render_stuff = 0;

void ai_debug_render_stuff()
{
	vertex	vert1, vert2;
	vector	gpos2;
	int		i;

	for (i=0; i<Num_AI_debug_render_stuff; i++) {
		ship_subsys	*ss;
		int	parent_objnum;
		vector	gpos, gvec;
		model_subsystem	*tp;

		ss = AI_debug_render_stuff[i].ss;
		tp = ss->system_info;

		parent_objnum = AI_debug_render_stuff[i].parent_objnum;

		ship_get_global_turret_info(&Objects[parent_objnum], tp, &gpos, &gvec);
		g3_rotate_vertex(&vert1, &gpos);
		vm_vec_scale_add(&gpos2, &gpos, &gvec, 20.0f);
		g3_rotate_vertex(&vert2, &gpos2);
		gr_set_color(0, 0, 255);
		g3_draw_sphere(&vert1, 2.0f);
		gr_set_color(255, 0, 255);
		g3_draw_sphere(&vert2, 2.0f);
		g3_draw_line(&vert1, &vert2);
	}

	// draw from beta to its goal point
/*	for (i=0; i<6; i++) {
		ai_info *aip = &Ai_info[i];
		gr_set_color(0, 0, 255);
		g3_rotate_vertex(&vert1, &Objects[i].pos);
		g3_rotate_vertex(&vert2, &aip->goal_point);
		g3_draw_line(&vert1, &vert2);
	} */
	

	Num_AI_debug_render_stuff = 0;
}

#endif

#ifndef NDEBUG
int	Msg_count_4996 = 0;
#endif

//	--------------------------------------------------------------------------
// Process subobjects of object objnum.
//	Deal with engines disabled.
void process_subobjects(int objnum)
{
	model_subsystem	*psub;
	ship_subsys	*pss;
	object	*objp = &Objects[objnum];
	ship		*shipp = &Ships[objp->instance];
	ai_info	*aip = &Ai_info[shipp->ai_index];
	ship_info	*sip = &Ship_info[shipp->ship_info_index];

	for ( pss = GET_FIRST(&shipp->subsys_list); pss !=END_OF_LIST(&shipp->subsys_list); pss = GET_NEXT(pss) ) {
		psub = pss->system_info;

		// Don't process destroyed objects
		if ( pss->current_hits <= 0.0f ) 
			continue;

		switch (psub->type) {
		case SUBSYSTEM_TURRET:
			if ( psub->turret_num_firing_points > 0 )	{
				ai_fire_from_turret(shipp, pss, objnum);
			} else {
#ifndef NDEBUG
				if (!Msg_count_4996) {
					Warning( LOCATION, "Ship '%s' has turrets with no guns!\nProbably a model problem, so get an artist!", shipp->ship_name );
					Msg_count_4996++;
				}
#endif
				}
			break;

		case SUBSYSTEM_ENGINE:
		case SUBSYSTEM_NAVIGATION:
		case SUBSYSTEM_COMMUNICATION:
		case SUBSYSTEM_WEAPONS:
		case SUBSYSTEM_SENSORS:
		case SUBSYSTEM_UNKNOWN:
			break;

		// next set of subsystems may rotation
		case SUBSYSTEM_RADAR:
		case SUBSYSTEM_SOLAR:
		case SUBSYSTEM_GAS_COLLECT:
		case SUBSYSTEM_ACTIVATION:
			break;
		default:
			Error(LOCATION, "Illegal subsystem type.\n");
		}

		// do solar/radar/gas/activator rotation here
		if ( psub->flags & MSS_FLAG_ROTATES )	{
			if (psub->flags & MSS_FLAG_STEPPED_ROTATE	) {
				submodel_stepped_rotate(psub, &pss->submodel_info_1);
			} else {
				submodel_rotate(psub, &pss->submodel_info_1 );
			}
		}

	}

	//	Deal with a ship with blown out engines.
	if (ship_get_subsystem_strength(shipp, SUBSYSTEM_ENGINE) == 0.0f) {
		if (sip->flags & (SIF_FIGHTER | SIF_BOMBER)) {
			// AL: Only attack forever if not trying to depart to a docking bay.  Need to have this in, since
			//     a ship may get repaired... and it should still try to depart.  Since docking bay departures
			//     are not handled as goals, we don't want to leave the AIM_BAY_DEPART mode.
			if ( aip->mode != AIM_BAY_DEPART ) {
				ai_attack_object(objp, NULL, 99, NULL);		//	Regardless of current mode, enter attack mode.
				aip->submode = SM_ATTACK_FOREVER;				//	Never leave attack submode, don't avoid, evade, etc.
			}
		}
	}


}

//	Given an object and the wing it's in, return its index in the wing list.
//	This defines its location in the wing formation.
//	If the object can't be found in the wing, return -1.
//	*objp		object of interest
//	wingnum	the wing *objp is in
int get_wing_index(object *objp, int wingnum)
{
	wing	*wingp;
	int	i;

	Assert((wingnum >= 0) && (wingnum < MAX_WINGS));

	wingp = &Wings[wingnum];

	for (i=wingp->current_count-1; i>=0; i--)
		if ( objp->instance == wingp->ship_index[i] )
			break;

	return i;		//	Note, returns -1 if string not found.
}

//	Given a wing, return a pointer to the object of its leader.
//	Asserts if object not found.
//	Currently, the wing leader is defined as the first object in the wing.
//	wingnum		Wing number in Wings array.
//	If wing leader is disabled, swap it with another ship.
object * get_wing_leader(int wingnum)
{
	wing		*wingp;
	int		ship_num;

	Assert((wingnum >= 0) && (wingnum < MAX_WINGS));

	wingp = &Wings[wingnum];

	Assert(wingp->current_count != 0);			//	Make sure there is a leader

	ship_num = wingp->ship_index[0];

	//	If this ship is disabled, try another ship in the wing.
	int n = 0;
	while (ship_get_subsystem_strength(&Ships[ship_num], SUBSYSTEM_ENGINE) == 0.0f) {
		n++;
		if (n >= wingp->current_count)
			break;	
		ship_num = wingp->ship_index[n];
	}

	if (( n != 0) && (n != wingp->current_count)) {
		int t = wingp->ship_index[0];
		wingp->ship_index[0] = wingp->ship_index[n];
		wingp->ship_index[n] = t;
	}

	return &Objects[Ships[ship_num].objnum];
}

#define	DEFAULT_WING_X_DELTA		1.0f
#define	DEFAULT_WING_Y_DELTA		0.25f
#define	DEFAULT_WING_Z_DELTA		0.75f
#define	DEFAULT_WING_MAG		(fl_sqrt(DEFAULT_WING_X_DELTA*DEFAULT_WING_X_DELTA + DEFAULT_WING_Y_DELTA*DEFAULT_WING_Y_DELTA + DEFAULT_WING_Z_DELTA*DEFAULT_WING_Z_DELTA))
// next constant is higher that MAX_SHIPS_IN_WINGS to deal with forming on player's wing
#define	MAX_FORMATION_ROWS		4

//	Given a position in a wing, return the desired location of the ship relative to the leader
//	*_delta_vec		OUTPUT.  delta vector based on wing_index
//	wing_index		position in wing.
void get_wing_delta(vector *_delta_vec, int wing_index)
{
	int	wi0;

	Assert(wing_index >= 0);

	int	k, row, column;

	int bank = wing_index / (MAX_FORMATION_ROWS*(MAX_FORMATION_ROWS+1)/2);
	wi0 = wing_index % (MAX_FORMATION_ROWS * (MAX_FORMATION_ROWS+1)/2);

	k = 0;
	for (row=1; row<MAX_FORMATION_ROWS+1; row++) {
		k += row;
		if (wi0 < k)
			break;
	}

	row--;
	column = wi0 - k + row + 1;

	_delta_vec->xyz.x = ((float) column - (float) row/2.0f) * DEFAULT_WING_X_DELTA/DEFAULT_WING_MAG;
	_delta_vec->xyz.y = ((float)row + (float)bank*2.25f) * DEFAULT_WING_Y_DELTA/DEFAULT_WING_MAG;
	_delta_vec->xyz.z = - ((float)row + 0.5f * (float) bank) * DEFAULT_WING_Z_DELTA/DEFAULT_WING_MAG;
}

//	Compute the largest radius of a ship in a *objp's wing.
float gwlr_1(object *objp, ai_info *aip)
{
	int		wingnum = aip->wing;
	float		max_radius;
	object	*o;
	ship_obj	*so;

	Assert(wingnum >= 0);

	max_radius = objp->radius;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		o = &Objects[so->objnum];
		if (Ai_info[Ships[o->instance].ai_index].wing == wingnum)
			if (o->radius > max_radius)
				max_radius = o->radius;
	}

	return max_radius;
}

//	Compute the largest radius of a ship forming on *objp's wing.
float gwlr_object_1(object *objp, ai_info *aip)
{
	float		max_radius;
	object	*o;
	ship_obj	*so;

	max_radius = objp->radius;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		o = &Objects[so->objnum];
		if (Ai_info[Ships[o->instance].ai_index].goal_objnum == OBJ_INDEX(objp))
			if (o->radius > max_radius)
				max_radius = o->radius;
	}

	return max_radius;
}

//	For the wing that *objp is part of, return the largest ship radius in that wing.
float get_wing_largest_radius(object *objp, int formation_object_flag)
{
	ship		*shipp;
	ai_info	*aip;

	Assert(objp->type == OBJ_SHIP);
	Assert((objp->instance >= 0) && (objp->instance < MAX_OBJECTS));
	shipp = &Ships[objp->instance];
	Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));
	aip = &Ai_info[shipp->ai_index];

	if (formation_object_flag) {
		return gwlr_object_1(objp, aip);
	} else {
		return gwlr_1(objp, aip);
	}

}

float Wing_y_scale = 2.0f;
float Wing_scale = 1.0f;
DCF(wing_y_scale, "")
{
	dc_get_arg(ARG_FLOAT);
	Wing_y_scale = Dc_arg_float;
}

DCF(wing_scale, "")
{
	dc_get_arg(ARG_FLOAT);
	Wing_scale = Dc_arg_float;
}

// Given a wing leader and a position in the wing formation, return the desired absolute location to fly to.
//	Returns result in *result_pos.
void get_absolute_wing_pos(vector *result_pos, object *leader_objp, int wing_index, int formation_object_flag)
{
	vector	wing_delta, rotated_wing_delta;
	float		wing_spread_size;

	get_wing_delta(&wing_delta, wing_index);		//	Desired location in leader's reference frame

	wing_spread_size = max(50.0f, 3.0f * get_wing_largest_radius(leader_objp, formation_object_flag) + 15.0f);

	// for player obj (1) move ships up 20% (2) scale formation up 20%
	if (leader_objp->flags & OF_PLAYER_SHIP) {
		wing_delta.xyz.y *= Wing_y_scale;
		wing_spread_size *= Wing_scale;
	}

	vm_vec_scale(&wing_delta, wing_spread_size * (1.0f + leader_objp->phys_info.speed/70.0f));

	vm_vec_unrotate(&rotated_wing_delta, &wing_delta, &leader_objp->orient);	//	Rotate into leader's reference.

	vm_vec_add(result_pos, &leader_objp->pos, &rotated_wing_delta);	//	goal_point is absolute 3-space point.
}

#ifndef NDEBUG
int Debug_render_wing_phantoms;

void render_wing_phantoms(object *objp)
{
	int		i;
	ship		*shipp;
	ai_info	*aip;
	int		wingnum;
	int		wing_index;		//	Index in wing struct, defines 3-space location in wing.
	vector	goal_point;
	
	Assert(objp->type == OBJ_SHIP);
	Assert((objp->instance >= 0) && (objp->instance < MAX_SHIPS));

	shipp = &Ships[objp->instance];
	Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

	aip = &Ai_info[shipp->ai_index];

	wingnum = aip->wing;

	if (wingnum == -1)
		return;

	wing_index = get_wing_index(objp, wingnum);

	//	If this ship is NOT the leader, abort.
	if (wing_index != 0)
		return;

	for (i=0; i<32; i++)
		if (Debug_render_wing_phantoms & (1 << i)) {
			get_absolute_wing_pos(&goal_point, objp, i, 0);
	
			vertex	vert;
			gr_set_color(255, 0, 128);
			g3_rotate_vertex(&vert, &goal_point);
			g3_draw_sphere(&vert, 2.0f);
		}

	Debug_render_wing_phantoms = 0;

}

void render_wing_phantoms_all()
{
	object	*objp;
	ship_obj	*so;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		ship		*shipp;
		ai_info	*aip;
		int		wingnum;
		int		wing_index;		//	Index in wing struct, defines 3-space location in wing.

		objp = &Objects[so->objnum];
		
		Assert((objp->instance >= 0) && (objp->instance < MAX_SHIPS));
		shipp = &Ships[objp->instance];
		Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

		aip = &Ai_info[shipp->ai_index];

		wingnum = aip->wing;

		if (wingnum == -1)
			continue;

		wing_index = get_wing_index(objp, wingnum);

		//	If this ship is NOT the leader, abort.
		if (wing_index != 0)
			continue;
		
		render_wing_phantoms(objp);

		return;
	}
}

#endif

//	Hook from goals code to AI.
//	Force a wing to fly in formation.
//	Sets AIF_FORMATION bit in ai_flags.
//	wingnum		Wing to force to fly in formation
void ai_fly_in_formation(int wingnum)
{
	object	*objp;
	ship		*shipp;
	ship_obj	*so;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		Assert((objp->instance >= 0) && (objp->instance < MAX_SHIPS));

		shipp = &Ships[objp->instance];
		Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

		if (Ai_info[shipp->ai_index].wing == wingnum) {
			Ai_info[shipp->ai_index].ai_flags |= AIF_FORMATION_WING;
			Ai_info[shipp->ai_index].ai_flags &= ~AIF_FORMATION_OBJECT;
		}
	}
}

//	Hook from goals code to AI.
//	Force a wing to abandon formation flying.
//	Clears AIF_FORMATION bit in ai_flags.
//	wingnum		Wing to force to fly in formation
void ai_disband_formation(int wingnum)
{
	object	*objp;
	ship		*shipp;
	ship_obj	*so;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];
		Assert((objp->instance >= 0) && (objp->instance < MAX_SHIPS));

		shipp = &Ships[objp->instance];
		Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

		if (Ai_info[shipp->ai_index].wing == wingnum) {
			Ai_info[shipp->ai_index].ai_flags &= ~AIF_FORMATION_WING;
		}
	}
}

float	Leader_chaos = 0.0f;
int Chaos_frame = -1;

//	Return true if objp is flying in an erratic manner
//	Only true if objp is a player
int formation_is_leader_chaotic(object *objp)
{
	if (Game_mode & GM_MULTIPLAYER)
		return 0;

	if (objp != Player_obj)
		return 0;

	if (Framecount != Chaos_frame) {
		float	speed_scale;
		float	fdot, udot;

		speed_scale = 3.0f + objp->phys_info.speed * 0.1f;

		fdot = 5.0f * (1.0f - vm_vec_dot(&objp->orient.v.fvec, &objp->last_orient.v.fvec)) * flFrametime;
		udot = 8.0f * (1.0f - vm_vec_dot(&objp->orient.v.uvec, &objp->last_orient.v.uvec)) * flFrametime;

		Leader_chaos += fdot * speed_scale + udot * speed_scale;

		Leader_chaos *= (1.0f - flFrametime*0.2f);

		if (Leader_chaos < 0.0f)
			Leader_chaos = 0.0f;
		else if (Leader_chaos > 1.7f)
			Leader_chaos = 1.7f;

		//nprintf(("AI", "Frame %i: chaos = %7.4f\n", Framecount, Leader_chaos));

		Chaos_frame = Framecount;
	}

	return (Leader_chaos > 1.0f);
}

// Fly in formation.
//	Make Pl_objp assume its proper place in formation.
//	If the leader of the wing is doing something stupid, like fighting a battle,
//	then the poor sap wingmates will be in for a "world of hurt"
//	Return TRUE if we need to process this object's normal mode
int ai_formation()
{
	object	*leader_objp;
	ship		*shipp;
	ai_info	*aip, *laip;
	int		wingnum;
	int		wing_index;		//	Index in wing struct, defines 3-space location in wing.
	int		player_wing;	// index of the players wingnum
	vector	goal_point, future_goal_point_5, future_goal_point_2, future_goal_point_x, future_goal_point_1000x, vec_to_goal, dir_to_goal;
	float		dot_to_goal, dist_to_goal, leader_speed;

	Assert(Pl_objp->type == OBJ_SHIP);
	Assert((Pl_objp->instance >= 0) && (Pl_objp->instance < MAX_SHIPS));

	shipp = &Ships[Pl_objp->instance];

	Assert((shipp->ai_index >= 0) && (shipp->ai_index < MAX_AI_INFO));

	aip = &Ai_info[shipp->ai_index];

	Assert((aip->ai_flags & AIF_FORMATION) != AIF_FORMATION);	//	Make sure not both types of formation flying in effect.

	//	Determine which kind of formation flying.
	//	If tracking an object, not in waypoint mode:
	if (aip->ai_flags & AIF_FORMATION_OBJECT) {
		if ((aip->goal_objnum < 0) || (aip->goal_objnum >= MAX_OBJECTS)) {
			aip->ai_flags &= ~AIF_FORMATION_OBJECT;
			return 1;
		}
		
		wing_index = ai_formation_object_get_slotnum(aip->goal_objnum, Pl_objp);
		leader_objp = &Objects[aip->goal_objnum];
	} else {	//	Formation flying in waypoint mode.
		Assert(aip->ai_flags & AIF_FORMATION_WING);
		if (aip->mode != AIM_WAYPOINTS) {
			aip->ai_flags &= ~AIF_FORMATION_WING;
			return 1;
		}

		wingnum = aip->wing;

		if (wingnum == -1)
			return 1;

		// disable formation flying for any ship in the players wing
		player_wing = Ships[Player_obj->instance].wingnum;
		if ( (player_wing != -1) && (wingnum == player_wing) )
			return 1;

		wing_index = get_wing_index(Pl_objp, wingnum);

		leader_objp = get_wing_leader(wingnum);

	}

	//	If docked with a ship in this wing, only the more massive one actually flies in formation.
	if (aip->dock_objnum != -1) {
		object	*other_objp = &Objects[aip->dock_objnum];
		ai_info	*other_aip = &Ai_info[Ships[other_objp->instance].ai_index];

		if (aip->wing == other_aip->wing) {
			if (Pl_objp->phys_info.mass < other_objp->phys_info.mass)
				return 0;
			else if (Pl_objp->phys_info.mass == other_objp->phys_info.mass) {
				if (Pl_objp->signature < other_objp->signature)
					return 0;
			}
		}
	}

	Assert(leader_objp != NULL);
	laip = &Ai_info[Ships[leader_objp->instance].ai_index];

	//	Make sure we're really in this wing.
	if (wing_index == -1)
		return 1;

	//	If this ship is the leader, abort, as he doesn't have to follow anyone.
	if (wing_index == 0) {
		// nprintf(("AI", "Hmm, wing leader %s in ai_formation for no good reason.\n", shipp->ship_name));
		return 1;
	}

	if (aip->mode == AIM_WAYPOINTS) {
		aip->wp_list = laip->wp_list;
		if (laip->wp_index < Waypoint_lists[laip->wp_list].count)
			aip->wp_index = laip->wp_index;
		else
			aip->wp_index = Waypoint_lists[laip->wp_list].count - 1;
		aip->wp_flags = laip->wp_flags;
		aip->wp_dir = laip->wp_dir;
	}

	#ifndef NDEBUG
	Debug_render_wing_phantoms |= (1 << wing_index);
	#endif

	leader_speed = leader_objp->phys_info.speed;
	vector leader_vec = leader_objp->phys_info.vel;

	get_absolute_wing_pos(&goal_point, leader_objp, wing_index, aip->ai_flags & AIF_FORMATION_OBJECT);
	vm_vec_scale_add(&future_goal_point_5, &goal_point, &leader_vec, 10.0f);
	vm_vec_scale_add(&future_goal_point_2, &goal_point, &leader_vec, 5.0f);
	vm_vec_scale_add(&future_goal_point_x, &goal_point, &leader_objp->orient.v.fvec, 10.0f);	//	used when very close to destination
	vm_vec_scale_add(&future_goal_point_1000x, &goal_point, &leader_objp->orient.v.fvec, 1000.0f);	//	used when very close to destination

	//	Now, get information telling this object how to turn and accelerate to get to its
	//	desired location.
	vm_vec_sub(&vec_to_goal, &goal_point, &Pl_objp->pos);
	if ( vm_vec_mag_quick(&vec_to_goal) < AICODE_SMALL_MAGNITUDE )
		vec_to_goal.xyz.x += 0.1f;

	vm_vec_copy_normalize(&dir_to_goal, &vec_to_goal);
	//dot_to_goal = vm_vec_dot(&dir_to_goal, &leader_objp->orient.v.fvec);
	dot_to_goal = vm_vec_dot(&dir_to_goal, &Pl_objp->orient.v.fvec);
	dist_to_goal = vm_vec_dist_quick(&Pl_objp->pos, &goal_point);
	float	dist_to_goal_2 = vm_vec_dist_quick(&Pl_objp->pos, &future_goal_point_2);

	// nprintf(("AI", "dot = %7.3f, dist = %8.3f, speed = %7.3f, leader speed = %7.3f\n", dot_to_goal, dist_to_goal, Pl_objp->phys_info.speed, leader_objp->phys_info.speed));

	int	chaotic_leader = 0;

	chaotic_leader = formation_is_leader_chaotic(leader_objp);	//	Set to 1 if leader is player and flying erratically.  Causes ships to not aggressively pursue formation location.

	if (dist_to_goal > 500.0f) {
		turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
		accelerate_ship(aip, 1.0f);
	} else if (dist_to_goal > 200.0f) {
		if (dot_to_goal > -0.5f) {
			turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
			float range_speed = shipp->current_max_speed - leader_speed;
			if (range_speed > 0.0f)
				set_accel_for_target_speed(Pl_objp, leader_speed + range_speed * (dist_to_goal+100.0f)/500.0f);
			else
				set_accel_for_target_speed(Pl_objp, shipp->current_max_speed);
		} else {
			turn_towards_point(Pl_objp, &future_goal_point_5, NULL, 0.0f);
			if (leader_speed > 10.0f)
				set_accel_for_target_speed(Pl_objp, leader_speed *(1.0f + dot_to_goal));
			else
				set_accel_for_target_speed(Pl_objp, 10.0f);
		}
	} else {
		vector	v2f2;
		float	dot_to_f2;
		float	dist_to_f2;

		dist_to_f2 = vm_vec_normalized_dir(&v2f2, &future_goal_point_2, &Pl_objp->pos);
		dot_to_f2 = vm_vec_dot(&v2f2, &Pl_objp->orient.v.fvec);

		//	Leader flying like a maniac.  Don't try hard to form on wing.
		if (chaotic_leader) {
			turn_towards_point(Pl_objp, &future_goal_point_2, NULL, 0.0f);
			set_accel_for_target_speed(Pl_objp, min(leader_speed*0.8f, 20.0f));
		} else if (dist_to_goal > 75.0f) {
			turn_towards_point(Pl_objp, &future_goal_point_2, NULL, 0.0f);
			float	delta_speed;
			float range_speed = shipp->current_max_speed - leader_speed;
			if (range_speed > 0.0f)
				delta_speed = dist_to_goal_2/500.0f * range_speed;
			else
				delta_speed = shipp->current_max_speed - leader_speed;
			if (dot_to_goal < 0.0f) {
				delta_speed = -delta_speed;
				if (-delta_speed > leader_speed/2)
					delta_speed = -leader_speed/2;
			}

			if (leader_speed < 5.0f)
				if (delta_speed < 5.0f)
					delta_speed = 5.0f;

			float scale = dot_to_f2;
			if (scale < 0.1f)
				scale = 0.0f;
			else
				scale *= scale;

			set_accel_for_target_speed(Pl_objp, scale * (leader_speed + delta_speed));
		} else {
			//nprintf(("AI", "Dot = %7.3f\n", dot_to_goal));

			if (leader_speed < 5.0f) {
				//	Leader very slow.  If not close to goal point, get very close.  Note, keep trying to get close unless
				//	moving very slowly, else momentum can carry far away from goal.

				if ((dist_to_goal > 10.0f) || ((Pl_objp->phys_info.speed > leader_speed + 2.5f) && (dot_to_goal > 0.5f))) {
					//nprintf(("MK", "(1) "));
					turn_towards_point(Pl_objp, &goal_point, NULL, 0.0f);
					set_accel_for_target_speed(Pl_objp, leader_speed + dist_to_goal/10.0f);
				} else {
					if (Pl_objp->phys_info.speed < 0.5f) {
						//nprintf(("MK", "(2) "));
						turn_towards_point(Pl_objp, &future_goal_point_1000x, NULL, 0.0f);
					} else {
						//nprintf(("MK", "(3) "));
					}
					set_accel_for_target_speed(Pl_objp, leader_speed);
				}
				//nprintf(("MK", "dist: %7.3f, dot: %6.3f, speed: %7.3f\n", dist_to_goal, dot_to_goal, Pl_objp->phys_info.speed));
			} else if (dist_to_goal > 10.0f) {
				float	dv;

				//future_goal_point_2;

				turn_towards_point(Pl_objp, &future_goal_point_2, NULL, 0.0f);

				if (dist_to_goal > 25.0f) {
					if (dot_to_goal < 0.3f)
						dv = -0.1f;
					else
						dv = dot_to_goal - 0.2f;

					set_accel_for_target_speed(Pl_objp, leader_speed + dist_to_goal/5.0f * dv);
				} else {
					set_accel_for_target_speed(Pl_objp, leader_speed + 1.5f * dot_to_goal - 1.0f);
				}
			} else {
				if (Pl_objp->phys_info.speed < 0.1f)
					turn_towards_point(Pl_objp, &future_goal_point_1000x, NULL, 0.0f);
				else
					turn_towards_point(Pl_objp, &future_goal_point_x, NULL, 0.0f);
				set_accel_for_target_speed(Pl_objp, 0.0f);
			}
		}

	}

	//	See how different this ship's bank is relative to wing leader
	float	up_dot = vm_vec_dot(&leader_objp->orient.v.uvec, &Pl_objp->orient.v.uvec);
	if (up_dot < 0.996f) {
		vector	w_out;
		matrix	new_orient;
		vector	angular_accel;

		vm_vec_copy_scale(&angular_accel, &Pl_objp->phys_info.max_rotvel, 0.2f);
		vm_matrix_interpolate(&leader_objp->orient, &Pl_objp->orient, &Pl_objp->phys_info.rotvel, flFrametime, &new_orient, &w_out, &Pl_objp->phys_info.max_rotvel, &angular_accel, 1);

	//	nprintf(("AI", "Frame %d Bashing formation orient.  Dot was %6.3f, becomes %6.3f\n", Framecount, up_dot, vm_vec_dot(&leader_objp->orient.v.uvec, &new_orient.v.uvec)));
		Pl_objp->orient = new_orient;
		Pl_objp->phys_info.rotvel = w_out;
	//	Pl_objp->phys_info.desired_rotvel = w_out;
	} else {
		Pl_objp->phys_info.rotvel.xyz.z = 0.0f;
	}

	return 0;
}

//	Return index of object repairing object objnum.
int find_repairing_objnum(int objnum)
{
	object		*objp;
	ship			*shipp;
	ship_info	*sip;
	ship_obj		*so;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		objp = &Objects[so->objnum];

		Assert((objp->instance >= 0) && (objp->instance < MAX_SHIPS));

		shipp = &Ships[objp->instance];
		sip = &Ship_info[shipp->ship_info_index];

		if (sip->flags & SIF_SUPPORT) {
			ai_info	*aip;

			aip = &Ai_info[shipp->ai_index];

			if (aip->goal_objnum == objnum) {
				return objp-Objects;
			}
		}
	}

	return -1;
}

//	If object *objp is being repaired, deal with it!
void ai_do_repair_frame(object *objp, ai_info *aip, float frametime)
{
	if (Ships[objp->instance].team == TEAM_TRAITOR) {
		ai_abort_rearm_request(objp);
		return;
	}

	if (aip->ai_flags & (AIF_BEING_REPAIRED | AIF_AWAITING_REPAIR)) {
		int	dock_objnum;
		ai_info	*repair_aip;

		dock_objnum = aip->dock_objnum; // find_repairing_objnum(objp-Objects);
		//Assert(dock_objnum != -1);
		if (dock_objnum == -1)
			return;
		if (Objects[dock_objnum].signature != aip->dock_signature) {
			Int3();		//	Curious -- object numbers match, but signatures do not.
							//	Must mean original repair ship died and was replaced by current ship.
			return;
		}
	
		repair_aip = &Ai_info[Ships[Objects[dock_objnum].instance].ai_index];
		//Assert(repair_aip->mode == AIM_DOCK);

		if (aip->ai_flags & AIF_BEING_REPAIRED) {
			// Assert(repair_aip->submode == AIS_DOCK_4);

			//	Wait awhile into the mode to synchronize with sound effect.
			if (Missiontime - repair_aip->submode_start_time > REARM_SOUND_DELAY) {
				int repaired;

				repaired = ship_do_rearm_frame( objp, frametime );		// hook to do missile rearming

				//	See if fully repaired.  If so, cause process to stop.
				if ( repaired && (repair_aip->submode == AIS_DOCK_4)) {

					repair_aip->submode = AIS_UNDOCK_0;
					repair_aip->submode_start_time = Missiontime;

					// if repairing player object -- tell him done with repair
					if ( !MULTIPLAYER_CLIENT ){
						ai_do_objects_repairing_stuff( objp, &Objects[dock_objnum], REPAIR_INFO_COMPLETE );
					}
				}
			}
		} else if (aip->ai_flags & AIF_AWAITING_REPAIR) {
			//	If this ship has been awaiting repair for 90+ seconds, abort.
			if ( !MULTIPLAYER_CLIENT ) {
				if ((Game_mode & GM_MULTIPLAYER) || (objp != Player_obj)) {
					if ((repair_aip->goal_objnum == OBJ_INDEX(objp)) && (timestamp_elapsed(aip->abort_rearm_timestamp))) {
						ai_abort_rearm_request(objp);
						aip->next_rearm_request_timestamp = timestamp(NEXT_REARM_TIMESTAMP);
					}
				}
			}
		}
	} else {
		// AL 11-24-97: If this is the player ship, ensure the repair sound isn't playing.  We need to
		//              do this check, since this is a looping sound, and may continue on if rearm/repair
		//              finishes abnormally once sound begins looping.
		if ( objp == Player_obj ) {
			player_stop_repair_sound();
		}
	}
}

//	Shell around dock_orient_and_approach to detect whether dock process should be aborted.
//	obj1 is the ship performing the repair.
//	obj2 is the ship being repaired.
void call_doa(object *obj1, object *obj2, ship_info *sip1)
{
	if (sip1->flags & SIF_SUPPORT) {
		if (obj2->phys_info.speed > MAX_REPAIR_SPEED) {

			// call the ai_abort rearm request code
			ai_abort_rearm_request( obj2 );
		} else
			dock_orient_and_approach(obj1, obj2, DOA_DOCK_STAY);
	} else {
		if (Ship_info[Ships[obj1->instance].ship_info_index].flags & SIF_CARGO)
			dock_orient_and_approach(obj1, obj2, DOA_DOCK_STAY);
		else if (Ship_info[Ships[obj2->instance].ship_info_index].flags & SIF_CARGO)
			dock_orient_and_approach(obj2, obj1, DOA_DOCK_STAY);
		else {
			//mprintf(("Warning: Not sure, but making %s [%s] move to stay docked with %s [%s]\n",
			//	Ships[obj1->instance].ship_name, Ship_info[Ships[obj1->instance].ship_info_index].name, Ships[obj2->instance].ship_name, Ship_info[Ships[obj2->instance].ship_info_index].name));
			dock_orient_and_approach(obj1, obj2, DOA_DOCK_STAY);

		}
	}

}

//	Maybe launch a countermeasure.
//	Also, detect a supposed homing missile that no longer exists.
void ai_maybe_launch_cmeasure(object *objp, ai_info *aip)
{
	float			dist;
	ship_info	*sip;
	ship			*shipp;

	shipp = &Ships[objp->instance];
	sip = &Ship_info[shipp->ship_info_index];

	if (!(sip->flags & (SIF_SMALL_SHIP | SIF_TRANSPORT)))
		return;

	if (!shipp->cmeasure_count)
		return;

	if ( !timestamp_elapsed(shipp->cmeasure_fire_stamp) )
		return;

	//	If not on player's team and Skill_level + ai_class is low, never fire a countermeasure.  The ship is too dumb.
	if (shipp->team != Player_ship->team) {
		if (Game_skill_level + aip->ai_class < 4){
			return;
		}
	}

	if ((aip->nearest_locked_object != -1) && (Objects[aip->nearest_locked_object].type == OBJ_WEAPON)) {
		object	*weapon_objp;
		weapon	*weaponp;
		weapon_info	*wip;

		weapon_objp = &Objects[aip->nearest_locked_object];
		weaponp = &Weapons[weapon_objp->instance];
		wip = &Weapon_info[weaponp->weapon_info_index];

		if ((dist = vm_vec_dist_quick(&objp->pos, &weapon_objp->pos)) < weapon_objp->phys_info.speed*2.0f) {
	
			aip->nearest_locked_distance = dist;
			//	Verify that this object is really homing on us.
			object	*weapon_objp;

			weapon_objp = &Objects[aip->nearest_locked_object];

			float	fire_chance;

			//	For ships on player's team, have constant, average chance to fire.
			//	For enemies, increasing chance with higher skill level.
			if (shipp->team == Player_ship->team)
				fire_chance = Cmeasure_fire_chance[NUM_SKILL_LEVELS/2];
			else
				fire_chance = Cmeasure_fire_chance[Game_skill_level];

			//	Decrease chance to fire at lower ai class.
			fire_chance *= (float) aip->ai_class/Num_ai_classes;

			float r = frand();
			if (fire_chance < r) {
				//nprintf(("AI", "Not firing countermeasure due to skill level: %7.3f < %7.3f\n", fire_chance, r));
				shipp->cmeasure_fire_stamp = timestamp(CMEASURE_WAIT + (int) (fire_chance*2000));		//	Wait 1/2 second (CMEASURE_WAIT) + additional delay to decrease chance of firing very soon.
				return;
			}

			if (weapon_objp->type == OBJ_WEAPON) {
				if (weapon_objp->instance >= 0) {
					//nprintf(("AI", "Firing countermeasure at time t=%7.3f\n", f2fl(Missiontime)));
					ship_launch_countermeasure(objp);
					shipp->cmeasure_fire_stamp = timestamp(2*CMEASURE_WAIT);
					return;
				}
			}
	
		}
	}

	return;
}

//	--------------------------------------------------------------------------
void ai_preprocess_ignore_objnum(object *objp, ai_info *aip)
{
//	if (aip->ignore_objnum == UNUSED_OBJNUM)
//		return;

	if (aip->ai_flags & AIF_TEMPORARY_IGNORE) {
		if (timestamp_elapsed(aip->ignore_expire_timestamp)) {
			aip->ignore_objnum = UNUSED_OBJNUM;
		}
	}

	if (is_ignore_object(aip, aip->goal_objnum)) {
		aip->goal_objnum = -1;
		// AL 12-11-97: If in STRAFE mode, we need to ensure that target_objnum is also
		//              set to -1
		if ( aip->mode == AIM_STRAFE ) {
			aip->target_objnum = -1;
		}
	}

	if (is_ignore_object(aip, aip->target_objnum))
		aip->target_objnum = -1;
}

/*
void ai_safety_circle_spot()
{
	vector	goal_point;
	ship_info	*sip;

	sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];

	goal_point = Ai_info[Ships[Pl_objp->instance].ai_index].goal_point;
	turn_towards_tangent(Pl_objp, &goal_point, 50.0f);

	set_accel_for_target_speed(Pl_objp, sip->max_speed/4.0f);

//	float dist = vm_vec_dist_quick(&goal_point, &Pl_objp->pos);
//	nprintf(("AI", "Ship %s circling %7.3f %7.3f %7.3f.  Distance = %7.3f\n", Ships[Pl_objp->instance].ship_name, goal_point.xyz.x, goal_point.xyz.y, goal_point.xyz.z, dist));

}
*/

#define	CHASE_CIRCLE_DIST		100.0f

void ai_chase_circle(object *objp)
{
	float		dist_to_goal;
	float		target_speed;
	vector	goal_point;
	ship_info	*sip;
	ai_info		*aip;

	sip = &Ship_info[Ships[Pl_objp->instance].ship_info_index];

	target_speed = sip->max_speed/4.0f;
	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	Assert(vm_vec_mag(&aip->goal_point) >= 0.0f);		//	Supposedly detects bogus vector

	goal_point = aip->goal_point;

	if (aip->ignore_objnum == UNUSED_OBJNUM) {
		dist_to_goal = vm_vec_dist_quick(&aip->goal_point, &objp->pos);

		if (dist_to_goal > 2*CHASE_CIRCLE_DIST) {
			vector	vec_to_goal;
			//	Too far from circle goal, create a new goal point.
			vm_vec_normalized_dir(&vec_to_goal, &aip->goal_point, &objp->pos);
			vm_vec_scale_add(&aip->goal_point, &objp->pos, &vec_to_goal, CHASE_CIRCLE_DIST);
		}

		goal_point = aip->goal_point;
	} else if (is_ignore_object(aip, aip->ignore_objnum)) {
		object	*ignore_objp = &Objects[aip->ignore_objnum];

		vector	tvec1;
		float		dist;

		dist = vm_vec_normalized_dir(&tvec1, &Pl_objp->pos, &ignore_objp->pos);

		if (dist < ignore_objp->radius*2 + 1500.0f) {
			vm_vec_scale_add(&goal_point, &Pl_objp->pos, &tvec1, ignore_objp->radius*2 + 1400.0f);
			if (dist < ignore_objp->radius*2 + 1300.0f)
				target_speed = sip->max_speed * (1.25f - dist/(ignore_objp->radius*2 + 1500.0f));
		}
	}

	Assert(vm_vec_mag(&aip->goal_point) >= 0.0f);		//	Supposedly detects bogus vector

	turn_towards_tangent(Pl_objp, &goal_point, 10*objp->radius + 200.0f);

	set_accel_for_target_speed(Pl_objp, target_speed);

}

#define	SHIELD_BALANCE_RATE	0.2f		//	0.1f -> takes 10 seconds to equalize shield.

//	Transfer shield energy to most recently hit section from others.
void ai_transfer_shield(object *objp, int quadrant_num)
{
	int	i;
	float	transfer_amount;
	float	transfer_delta;
	ship_info	*sip;
	float	max_quadrant_strength;

	sip = &Ship_info[Ships[objp->instance].ship_info_index];
	max_quadrant_strength = sip->shields/MAX_SHIELD_SECTIONS;

	transfer_amount = 0.0f;
	transfer_delta = (SHIELD_BALANCE_RATE/2) * max_quadrant_strength;

	if (objp->shields[quadrant_num] + (MAX_SHIELD_SECTIONS-1)*transfer_delta > max_quadrant_strength)
		transfer_delta = (max_quadrant_strength - objp->shields[quadrant_num])/(MAX_SHIELD_SECTIONS-1);

	for (i=0; i<MAX_SHIELD_SECTIONS; i++)
		if (i != quadrant_num) {
			if (objp->shields[i] >= transfer_delta) {
				objp->shields[i] -= transfer_delta;
				transfer_amount += transfer_delta;
			} else {
				transfer_amount += objp->shields[i];
				objp->shields[i] = 0.0f;
			}
		}

	objp->shields[quadrant_num] += transfer_amount;
}

void ai_balance_shield(object *objp)
{
	int	i;
	float	shield_strength_avg;
	float	delta;


	shield_strength_avg = get_shield_strength(objp)/MAX_SHIELD_SECTIONS;

	delta = SHIELD_BALANCE_RATE * shield_strength_avg;

	for (i=0; i<MAX_SHIELD_SECTIONS; i++)
		if (objp->shields[i] < shield_strength_avg) {
			add_shield_strength(objp, delta);
			if (objp->shields[i] > shield_strength_avg)
				objp->shields[i] = shield_strength_avg;
		} else {
			add_shield_strength(objp, -delta);
			if (objp->shields[i] < shield_strength_avg)
				objp->shields[i] = shield_strength_avg;
		}
}

//	Manage the shield for this ship.
//	Try to max out the side that was most recently hit.
void ai_manage_shield(object *objp, ai_info *aip)
{
	ship_info *sip;

	sip = &Ship_info[Ships[objp->instance].ship_info_index];

	if (timestamp_elapsed(aip->shield_manage_timestamp)) {
		float		delay;

		//	Scale time until next manage shield based on Skill_level.
		//	Ships on player's team are treated as if Skill_level is average.
		if (Ships[objp->instance].team != Player_ship->team){
			delay = Shield_manage_delays[Game_skill_level];
		} else {
			delay = Shield_manage_delays[NUM_SKILL_LEVELS/2];
		}

		//	Scale between 1x and 3x based on ai_class
		delay = delay + delay * (float) (3*(Num_ai_classes - aip->ai_class - 1) / (Num_ai_classes - 1));
		aip->shield_manage_timestamp = timestamp((int) (delay * 1000.0f));

		if (sip->flags & SIF_SMALL_SHIP) {
			if (Missiontime - aip->last_hit_time < F1_0*10)
				ai_transfer_shield(objp, aip->last_hit_quadrant);
			else
				ai_balance_shield(objp);
		}

		// nprintf(("AI", "Time: %7.3f Next: %7.3f, Shields: %7.3f %7.3f %7.3f %7.3f\n", f2fl(Missiontime), f2fl(Missiontime) + delay, objp->shields[0], objp->shields[1], objp->shields[2], objp->shields[3]));
	}
}

//	See if object *objp should evade an incoming missile.
//	*aip is the ai_info pointer within *objp.
void ai_maybe_evade_locked_missile(object *objp, ai_info *aip)
{
	ship			*shipp;
	ship_info	*sip;

	shipp = &Ships[objp->instance];
	sip = &Ship_info[shipp->ship_info_index];

	//	Only small ships evade an incoming missile.  Why would a capital ship try to swerve?
	if (!(Ship_info[Ships[objp->instance].ship_info_index].flags & SIF_SMALL_SHIP)) {
		return;
	}

	if (aip->ai_flags & (AIF_NO_DYNAMIC | AIF_KAMIKAZE)) {	//	If not allowed to pursue dynamic objectives, don't evade.  Dumb?  Maybe change. -- MK, 3/15/98
		return;
	}

	if (aip->nearest_locked_object != -1) {
		object	*missile_objp;

		missile_objp = &Objects[aip->nearest_locked_object];

		if (Weapons[missile_objp->instance].homing_object != objp) {
			//nprintf(("AI", "\nMissile lost home!"));
			aip->nearest_locked_object = -1;
			return;
		}

		if ((missile_objp->type == OBJ_WEAPON) && (Weapon_info[Weapons[missile_objp->instance].weapon_info_index].wi_flags & WIF_HOMING)) {
			float dist = vm_vec_dist_quick(&missile_objp->pos, &objp->pos);
			float dist2 = 4.0f  * vm_vec_mag_quick(&missile_objp->phys_info.vel);			
			if (dist < dist2) {
				switch (aip->mode) {
				//	If in AIM_STRAFE mode, don't evade if parent of weapon is targeted ship.
				case AIM_STRAFE:
					if ((missile_objp->parent != -1) && (missile_objp->parent == aip->target_objnum)) {
						;
					} else {
						;		//	Alan -- If you want to handle incoming weapons from someone other than the ship
								//	the strafing ship is attacking, do it here.
					}
					break;
				case AIM_CHASE:
					//	Don't always go into evade weapon mode.  Usually, a countermeasure gets launched.
					// If low on countermeasures, more likely to try to evade.  If 8+, never evade due to low cmeasures.
					if (((((Missiontime >> 18) ^ OBJ_INDEX(objp)) & 3) == 0) || 
						(objp->phys_info.speed < 40.0f) ||
						(frand() < 1.0f - (float) shipp->cmeasure_count/8.0f)) {
						if (aip->submode != SM_ATTACK_FOREVER) {	//	SM_ATTACK_FOREVER means engines blown.
							aip->submode = SM_EVADE_WEAPON;
							aip->submode_start_time = Missiontime;
						}
					}
					break;
				case AIM_DOCK:	//	Ships in dock mode can evade iif they are not currently repairing or docked.
					if (aip->ai_flags & (AIF_REPAIRING | AIF_DOCKED))
						break;
				case AIM_GUARD:
					//	If in guard mode and far away from guard object, don't pursue guy that hit me.
					if ((aip->guard_objnum != -1) && (aip->guard_signature == Objects[aip->guard_objnum].signature)) {
						if (vm_vec_dist_quick(&objp->pos, &Objects[aip->guard_objnum].pos) > 500.0f) {
							return;
						}
					}
				case AIM_EVADE:
				case AIM_GET_BEHIND:
				case AIM_STAY_NEAR:
				case AIM_STILL:
 				case AIM_AVOID:
				case AIM_WAYPOINTS:
				case AIM_NONE:
				case AIM_BIGSHIP:
				case AIM_PATH:
				case AIM_BE_REARMED:
				case AIM_SAFETY:
				case AIM_BAY_EMERGE:
					aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;
					aip->previous_mode = aip->mode;
					aip->previous_submode = aip->submode;
					aip->mode = AIM_EVADE_WEAPON;
					aip->submode = -1;
					aip->submode_start_time = Missiontime;
					aip->mode_time = timestamp(MAX_EVADE_TIME);	//	Max time to evade.
					//nprintf(("AI", "%s Evade weapon in frame #%i\n", Ships[objp->instance].ship_name, AI_FrameCount));
					break;
				case AIM_EVADE_WEAPON:		//	Note: We don't want to change mode on another evasion, or previous_mode will get bashed.
				case AIM_PLAY_DEAD:
				case AIM_BAY_DEPART:
				case AIM_SENTRYGUN:
					break;
				case AIM_WARP_OUT:
					break;
				default:
					Int3();			//	Hey, what mode is it?
					break;
				}
			}
		} else {
			aip->nearest_locked_object = -1;
		}
	}
}

//	Maybe evade a dumbfire weapon that was fired when Pl_objp was targeted.
//	Have an 80% chance of evading in a second
void maybe_evade_dumbfire_weapon(ai_info *aip)
{
	//	Only small ships evade an incoming missile.  Why would a capital ship try to swerve?
	if (!(Ship_info[Ships[Pl_objp->instance].ship_info_index].flags & SIF_SMALL_SHIP)) {
		return;
	}

	//	Make sure in a mode in which we evade dumbfire weapons.
	switch (aip->mode) {
	case AIM_CHASE:
		if (aip->submode == SM_ATTACK_FOREVER) {
			return;
		}
	case AIM_GUARD:
		//	If in guard mode and far away from guard object, don't pursue guy that hit me.
		if ((aip->guard_objnum != -1) && (aip->guard_signature == Objects[aip->guard_objnum].signature)) {
			if (vm_vec_dist_quick(&Objects[Ships[aip->shipnum].objnum].pos, &Objects[aip->guard_objnum].pos) > 500.0f) {
				return;
			}
		}
 	case AIM_STILL:
	case AIM_STAY_NEAR:
	case AIM_EVADE:
	case AIM_GET_BEHIND:
	case AIM_AVOID:
	case AIM_PATH:
	case AIM_NONE:
	case AIM_WAYPOINTS:
	case AIM_SAFETY:
		break;
	case AIM_STRAFE:
	case AIM_BIGSHIP:
	case AIM_DOCK:
	case AIM_PLAY_DEAD:
	case AIM_EVADE_WEAPON:
	case AIM_BAY_EMERGE:
	case AIM_BAY_DEPART:
	case AIM_SENTRYGUN:
	case AIM_WARP_OUT:
		return;
	default:
		Int3();	//	Bogus mode!
		return;
	}

	if (is_instructor(&Objects[Ships[aip->shipnum].objnum]))
		return;	//	Instructor doesn't evade.

	float t = ai_endangered_by_weapon(aip);
	if ((t > 0.0f) && (t < 1.0f)) {
	// Check if this weapon is from a large ship Pl_objp is attacking... if so, enter strafe mode
		if ( ai_big_maybe_enter_strafe_mode(Pl_objp, aip->danger_weapon_objnum) ) {
			return;
		}

		switch (aip->mode) {
		case AIM_CHASE:
			switch (aip->submode) {
			case SM_EVADE:
			case SM_ATTACK_FOREVER:
			case SM_AVOID:
			case SM_GET_AWAY:
			case SM_EVADE_WEAPON:
				break;
			default:
				if (ai_near_full_strength(Pl_objp, &Ship_info[Ships[Pl_objp->instance].ship_info_index])) {
					//mprintf(("Ship %s entered super mode at %7.3f\n", Ships[Pl_objp->instance].ship_name, 1.0f * Missiontime / (1<<16)));
					aip->submode = SM_SUPER_ATTACK;
					aip->submode_start_time = Missiontime;
					aip->last_attack_time = Missiontime;
				} else {
					//mprintf(("Ship %s entered dumbfire evade mode at %7.3f\n", Ships[Pl_objp->instance].ship_name, 1.0f * Missiontime / (1<<16)));
					aip->submode = SM_EVADE_WEAPON;
					aip->submode_start_time = Missiontime;
				}
				break;
			}
			break;
		case AIM_GUARD:
		case AIM_STILL:
		case AIM_STAY_NEAR:
		case AIM_EVADE:
		case AIM_GET_BEHIND:
		case AIM_AVOID:
		case AIM_PATH:
		case AIM_NONE:
		case AIM_WAYPOINTS:
		case AIM_SAFETY:
			if (!(aip->ai_flags & (AIF_NO_DYNAMIC | AIF_KAMIKAZE)) && (Ship_info[Ships[aip->shipnum].ship_info_index].flags & SIF_SMALL_SHIP)) {
				aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;
				aip->previous_mode = aip->mode;
				aip->previous_submode = aip->submode;
				aip->mode = AIM_EVADE_WEAPON;
				aip->submode = -1;
				aip->submode_start_time = Missiontime;
				aip->mode_time = timestamp(MAX_EVADE_TIME);	//	Evade for up to five seconds.
			}
			break;
		case AIM_STRAFE:
		case AIM_BIGSHIP:
		case AIM_DOCK:
		case AIM_PLAY_DEAD:
		case AIM_EVADE_WEAPON:
		case AIM_BAY_EMERGE:
		case AIM_BAY_DEPART:
		case AIM_SENTRYGUN:
			break;
		default:
			Int3();	//	Bogus mode!
		}
	}
}

// determine what path to use when emerging from a fighter bay
// input:	pl_objp	=>	pointer to object for ship that is arriving
//				pos		=>	output parameter, it is the starting world pos for path choosen
//				v.fvec		=>	output parameter, this is the forward vector that ship has when arriving
//
// exit:		-1		=>	path could not be located
//				 0		=> success
int ai_acquire_emerge_path(object *pl_objp, int parent_objnum, vector *pos, vector *fvec)
{
	int			path_index, sb_path_index;
	ship			*parent_sp = NULL;
	polymodel	*pm;
	ai_info		*aip;
	ship_bay		*sb;
	pnode			*pnp;
	vector		*next_point;

	aip = &Ai_info[Ships[pl_objp->instance].ai_index];

	if ( parent_objnum == -1 ) {
		Int3();
		return -1;
	}

	parent_sp = &Ships[Objects[parent_objnum].instance];

	Assert(parent_sp != NULL);
	pm = model_get( parent_sp->modelnum );
	sb = pm->ship_bay;

	if ( sb == NULL ) 
		return -1;

	if ( sb->num_paths <= 0 ) 
		return -1;

	// try to find a bay path that is not taken
	path_index = -1;
	sb_path_index = Ai_last_arrive_path++;

	if ( sb_path_index >= sb->num_paths ) {
		sb_path_index=0;
		Ai_last_arrive_path=0;
	}

	path_index = sb->paths[sb_path_index];
	if ( path_index == -1 ) 
		return -1;

	// create the path for pl_objp to follow
	create_model_exit_path(pl_objp, &Objects[parent_objnum], path_index, pm->paths[path_index].nverts);
	
	// Set this flag, so we don't bother recreating the path... we won't need to update the path
	// that has just been created.
//	aip->ai_flags |= AIF_USE_STATIC_PATH;

	// now return to the caller what the starting world pos and starting fvec for the ship will be
	Assert((aip->path_start >= 0) && (aip->path_start < MAX_PATH_POINTS));
	pnp = &Path_points[aip->path_start];
	*pos = pnp->pos;

	// calc the forward vector using the starting two points of the path
	pnp = &Path_points[aip->path_start+1];
	next_point = &pnp->pos;
	vm_vec_normalized_dir(fvec, next_point, pos);

	// record the parent objnum, since we'll need it once we're done with following the path
	aip->goal_objnum = parent_objnum;
	aip->goal_signature = Objects[parent_objnum].signature;
	aip->mode = AIM_BAY_EMERGE;
	aip->submode_start_time = Missiontime;

	// set up starting vel
	vector vel;
	float speed;
	speed = Ship_info[Ships[pl_objp->instance].ship_info_index].max_speed;
	vel = *fvec;
	vm_vec_scale( &vel, speed );
	pl_objp->phys_info.vel = vel;
	pl_objp->phys_info.desired_vel = vel;
	pl_objp->phys_info.prev_ramp_vel.xyz.x = 0.0f;
	pl_objp->phys_info.prev_ramp_vel.xyz.y = 0.0f;
	pl_objp->phys_info.prev_ramp_vel.xyz.z = speed;
	pl_objp->phys_info.forward_thrust = 0.0f;		// How much the forward thruster is applied.  0-1.

	return 0;	
}

// clean up path data used for emerging from a fighter bay
void ai_emerge_bay_path_cleanup(ai_info *aip)
{
	aip->path_start = -1;
	aip->path_cur = -1;
	aip->path_length = 0;
	aip->mode = AIM_NONE;
}

// handler for AIM_BAY_EMERGE
void ai_bay_emerge()
{
	ai_info	*aip;
	int		parent_died=0;

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	// if no path to follow, leave this mode
	if ( aip->path_start < 0 ) {
		aip->mode = AIM_NONE;
		return;
	}

	// ensure parent ship is still alive
	if ( aip->goal_objnum < 0 ) {
		parent_died=1;
	} 
	if ( !parent_died ) {
		if ( Objects[aip->goal_objnum].signature != aip->goal_signature ) {
			parent_died=1;
		}
	}

	if ( !parent_died ) {
		Assert(Objects[aip->goal_objnum].type == OBJ_SHIP);
		if ( Ships[Objects[aip->goal_objnum].instance].flags & SF_DYING ) {
			parent_died = 1;
		}
	}

	if ( parent_died ) {
		ai_emerge_bay_path_cleanup(aip);
		return;
	}

	// follow the path to the final point
	ai_path();

	// New test: must have been in AI_EMERGE mode for at least 10 seconds, and be a minimum distance from the start point
	if ( ( (Missiontime - aip->submode_start_time) > 10*F1_0 ) && (vm_vec_dist_quick(&Pl_objp->pos, &Objects[aip->goal_objnum].pos) > 0.75f * Objects[aip->goal_objnum].radius)) {
		// erase path
		ai_emerge_bay_path_cleanup(aip);
	}

	// 2-25-99: Need this check to fix an assert for supercap ships... maybe we'll only do this check for supercaps	
	if (aip->path_cur > (aip->path_start+aip->path_length-1)) {
		ai_emerge_bay_path_cleanup(aip);
	}	
}

// Select the closest depart path
//
//	input:	aip	=>		ai info pointer to ship seeking to depart
//				pm		=>		pointer to polymodel for the ship contining the ship bay/depart paths
//
// exit:		>=0	=>		ship bay path index for depart path (ie index into sb->paths[])
//				-1		=>		no path could be found
//
// NOTE: this function should only be used for calculating closest depart paths for ai mode
//			AI_BAY_DEPART.  It tries to find the closest path that isn't already in use
int ai_find_closest_depart_path(ai_info *aip, polymodel *pm)
{
	int			i, j, best_path, best_free_path;
	float			dist, min_dist, min_free_dist;
	vector		*source;
	model_path	*mp;
	ship_bay		*sb;

	sb = pm->ship_bay;

	best_free_path = best_path = -1;
	min_free_dist = min_dist = 1e20f;
	Assert(aip->shipnum >= 0);
	source = &Objects[Ships[aip->shipnum].objnum].pos;

	for ( i = 0; i < sb->num_paths; i++ ) {


		mp = &pm->paths[sb->paths[i]];
		for ( j = 0; j < mp->nverts; j++ ) {
			dist = vm_vec_dist_squared(source, &mp->verts[j].pos);

			if ( dist < min_dist ) {
				min_dist = dist;
				best_path = i;
			}

			// If this is a free path
			if ( !(sb->depart_flags & (1<<i)) ) {
				if ( dist < min_free_dist ) {
					min_free_dist = dist;
					best_free_path = i;
				}
			}
		}
	}

	if ( best_free_path >= 0 ) {
		return best_free_path;		
	}

	return best_path;
}

// determine what path to use when trying to depart to a fighter bay
// NOTE: this should be called when AIM_BAY_DEPART mode is set
//
// input:	pl_objp	=>	pointer to object for ship that is departing
//
// exit:		-1	=>	could not find depart path
//				0	=> found depart path
int ai_acquire_depart_path(object *pl_objp, int parent_objnum)
{
	int			objnum, path_index;
	polymodel	*pm;
	ai_info		*aip;
	ship			*sp;
	ship_bay		*sb;

	aip = &Ai_info[Ships[pl_objp->instance].ai_index];

	if ( parent_objnum == -1 ) {
		ship_obj	*so;

		// for now just locate a captial ship on the same team:
		so = GET_FIRST(&Ship_obj_list);
		objnum = -1;
		while(so != END_OF_LIST(&Ship_obj_list)){
			sp = &Ships[Objects[so->objnum].instance];
			if ( (Ship_info[sp->ship_info_index].flags & (SIF_HUGE_SHIP)) && (sp->team == Ships[pl_objp->instance].team) ) {
				objnum = so->objnum;
				break;
			}
			so = GET_NEXT(so);
		} 
	} else {
		objnum = parent_objnum;
	}

	aip->path_start = -1;

	if ( objnum == -1 )
		return -1;

	pm = model_get( Ships[Objects[objnum].instance].modelnum );
	sb = pm->ship_bay;

	if ( sb == NULL ) 
		return -1;
	if ( sb->num_paths <= 0 ) 
		return -1;

/*
	
	path_index = -1;
	for ( i = 0; i < sb->num_paths; i++ ) {
		if ( !(sb->depart_flags & (1<<i)) ) {
			sb->depart_flags |= (1<<i);
			path_index = sb->paths[i];
			aip->submode_parm0 = i;			// use mode-specific parameter to record ship bay path index
			break;
		}
	}
*/
	
	// take the closest path we can find
	int ship_bay_path;
	ship_bay_path = ai_find_closest_depart_path(aip, pm);
	path_index = sb->paths[ship_bay_path];
	aip->submode_parm0 = ship_bay_path;
	sb->depart_flags |= (1<<ship_bay_path);

	if ( path_index == -1 ) {
		return -1;
	}

	Assert(pm->n_paths > path_index);
	ai_find_path(pl_objp, objnum, path_index, 0);

	// Set this flag, so we don't bother recreating the path... we won't need to update the path
	// that has just been created.
	aip->ai_flags &= ~AIF_USE_STATIC_PATH;

	aip->goal_objnum = objnum;
	aip->goal_signature = Objects[objnum].signature;
	aip->mode = AIM_BAY_DEPART;

	Ships[pl_objp->instance].flags |= SF_DEPART_DOCKBAY;
	return 0;
}

// handler for AIM_BAY_DEPART
void ai_bay_depart()
{
	ai_info	*aip;

	aip = &Ai_info[Ships[Pl_objp->instance].ai_index];

	// if no path to follow, leave this mode
	if ( aip->path_start < 0 ) {
		aip->mode = AIM_NONE;
		return;
	}

	// check if parent ship still exists, if not abort depart 
	if ( aip->goal_signature != Objects[aip->goal_objnum].signature ) {
		aip->mode = AIM_NONE;
		return;
	}

	// follow the path to the final point
	ai_path();

	// if the final point is reached, let default AI take over
	if ( aip->path_cur >= (aip->path_start+aip->path_length) ) {
		polymodel	*pm;
		ship_bay		*sb;

		pm = model_get( Ships[Objects[aip->goal_objnum].instance].modelnum );
		sb = pm->ship_bay;
		if ( sb != NULL ) {
			sb->depart_flags &= ~(1<<aip->submode_parm0);
		}

		// make ship disappear
		Pl_objp->flags |= OF_SHOULD_BE_DEAD;
		ship_departed( Pl_objp->instance );

		// clean up path stuff
		aip->path_start = -1;
		aip->path_cur = -1;
		aip->path_length = 0;
		aip->mode = AIM_NONE;
	}
}

// Handler for AIM_SENTRYGUN.  This AI mode is for sentry guns only (ie floating turrets).
void ai_sentrygun()
{
	// Nothing to do here.  Turret firing is handled via process_subobjects().
	// If you want the sentry guns to do anything beyond firing their turrets at enemies, add it here!
}

//	--------------------------------------------------------------------------
//	Execute behavior given by aip->mode.
void ai_execute_behavior(ai_info *aip)
{
	switch (aip->mode) {
	case AIM_CHASE:
		if (En_objp) {
			ai_chase();
		} else if (aip->submode == SM_EVADE_WEAPON) {
			evade_weapon();
			// maybe reset submode
			if (aip->danger_weapon_objnum == -1) {
				aip->submode = SM_ATTACK;
				aip->submode_start_time = Missiontime;
				aip->last_attack_time = Missiontime;
			}
		} else {
			//	Don't circle if this is the instructor.
			ship	*shipp = &Ships[aip->shipnum];
			ship_info	*sip = &Ship_info[shipp->ship_info_index];

			if (strnicmp(shipp->ship_name, INSTRUCTOR_SHIP_NAME, strlen(INSTRUCTOR_SHIP_NAME))) {
				if (sip->flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) {
					aip->mode = AIM_NONE;
				} else {
					ai_chase_circle(Pl_objp);
				}
			}
		}
		break;
	case AIM_EVADE:
		if (En_objp) {
			ai_evade();
		} else {
			vector	tvec;
			vm_vec_scale_add(&tvec, &Pl_objp->pos, &Pl_objp->orient.v.rvec, 100.0f);
			turn_towards_point(Pl_objp, &tvec, NULL, 0.0f);
			accelerate_ship(aip, 0.5f);
		}
		break;
	case AIM_STILL:
		ai_still();
		break;
	case AIM_STAY_NEAR:
		ai_stay_near();
		break;
	case AIM_GUARD:
		ai_guard();
		break;
	case AIM_WAYPOINTS:
		ai_waypoints();
		break;
	case AIM_DOCK:
		ai_dock();
		break;
	case AIM_NONE:
		// ai_formation();
		break;
	case AIM_BIGSHIP:
		ai_big_ship(Pl_objp);
		break;
	case AIM_PATH: {
		int path_num;
		path_num = ai_return_path_num_from_dockbay(&Objects[aip->goal_objnum], 0);
		ai_find_path(Pl_objp, aip->goal_objnum, path_num, 0);
		ai_path();
		break;
	}
	case AIM_SAFETY:
		ai_safety();
		break;
	case AIM_EVADE_WEAPON:
		evade_weapon();
		break;
	case AIM_STRAFE:
		if (En_objp) {
			Assert(En_objp->type == OBJ_SHIP);
			ai_big_strafe();	// strafe a big ship
		} else {
			aip->mode = AIM_NONE;
		}
		break;
	case AIM_BAY_EMERGE:
		ai_bay_emerge();
		break;
	case AIM_BAY_DEPART:
		ai_bay_depart();
		break;
	case AIM_SENTRYGUN:
		ai_sentrygun();
		break;
	case AIM_WARP_OUT:
		break;		//	Note, handled directly from ai_frame().
	default:
		Int3();		//	This should never happen -- MK, 5/12/97	
		break;
	}

	if ( !(ship_get_SIF(aip->shipnum) & SIF_NOT_FLYABLE) ) {
		maybe_evade_dumbfire_weapon(aip);
	}
}

//	Auxiliary function for maybe_request_support.
//	Return 1 if subsystem "type" is worthy of repair, else return 0.
//	Since subsystems cannot be repaired if they are at 0 strength, don't return 1 if subsystem is dead.
int mrs_subsystem(ship *shipp, int type)
{
	float	t;

	t = ship_get_subsystem_strength(shipp, type);

	if (t > 0.0f) {
		return (int) ((1.0f - t) * 3);
	} else {
		return 3;
	}
}

//	Return number of ships on *objp's team that are currently rearming.
int num_allies_rearming(object *objp)
{
	ship_obj	*so;
	int		team;
	int		count = 0;

	team = Ships[objp->instance].team;

	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		object	*A;
		
		Assert (so->objnum != -1);
		A = &Objects[so->objnum];

		if (Ships[A->instance].team == team) {
			if (Ai_info[Ships[A->instance].ai_index].ai_flags & (AIF_REPAIRING | AIF_AWAITING_REPAIR)) {
				count++;
			}
		}
	}

	return count;
}


//	Maybe ship *objp should request support (rearm/repair).
//	If it does, return TRUE, else return FALSE.
int maybe_request_support(object *objp)
{
	ship_info	*sip;
	ship			*shipp;
	ai_info		*aip;
	int			desire;

	Assert(objp->type == OBJ_SHIP);
	shipp = &Ships[objp->instance];
	aip = &Ai_info[shipp->ai_index];
	sip = &Ship_info[shipp->ship_info_index];

	if (!timestamp_elapsed(aip->next_rearm_request_timestamp))
		return 0;

	//	Only fighters and bombers request support.
	if (!(sip->flags & (SIF_FIGHTER | SIF_BOMBER)))
		return 0;

	//	A ship that is currently awaiting does not need support!
	if (aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED))
		return 0;

	if (!is_support_allowed(objp))
		return 0;

	//if (shipp->team != TEAM_FRIENDLY)
	//	return 0;

	//	Compute a desire value.
	//	Desire of 0 means no reason to request support.
	//	1 is slight, 2 more, etc.  Maximum is around 20.  Anything larger than 3 is pretty strong.
	desire = 0;

	//	Set desire based on hull strength.
	//	No: We no longer repair hull, so this would cause repeated repair requests.
	//desire += 6 - (int) ((objp->hull_strength/sip->initial_hull_strength) * 6.0f);

	//	Set desire based on key subsystems.
	desire += 2*mrs_subsystem(shipp, SUBSYSTEM_ENGINE);	//	Note, disabled engine forces repair request, regardless of nearby enemies.
	desire += mrs_subsystem(shipp, SUBSYSTEM_COMMUNICATION);
	desire += mrs_subsystem(shipp, SUBSYSTEM_WEAPONS);
	desire += mrs_subsystem(shipp, SUBSYSTEM_SENSORS);

	//	Set desire based on percentage of secondary weapons.
	ship_weapon *swp = &shipp->weapons;

	for ( int i = 0; i < swp->num_secondary_banks; i++ ) {
		if (swp->secondary_bank_start_ammo[i] > 0) {
//			float r = (float) swp->secondary_bank_ammo[i]*Weapon_info[swp->secondary_bank_weapons[i]].cargo_size/swp->secondary_bank_capacity[i];
			float r = (float) swp->secondary_bank_ammo[i]/swp->secondary_bank_start_ammo[i];
			desire += (int) ((1.0f - r) * 3.0f);
		}
	}

	//	If no reason to repair, don't bother to see if it's safe to repair.
	if (desire == 0){
		return 0;
	}

	//	Compute danger threshold.
	//	Balance this with desire and maybe request support.
	if (ai_good_time_to_rearm( objp )) {
		ai_issue_rearm_request(objp);
		return 1;
	} else if (num_allies_rearming(objp) < 2) {
		if (desire >= 8) {	//	guarantees disabled will cause repair request
			ai_issue_rearm_request(objp);
		} else if (desire >= 3) {		//	>= 3 means having a single subsystem fully blown will cause repair.
			int	count;
			int objnum = find_nearby_hostile(OBJ_INDEX(objp), get_enemy_team_mask(OBJ_INDEX(objp)), 2000.0f, &count);

			if ((objnum == -1) || (count < 2) || (vm_vec_dist_quick(&objp->pos, &Objects[objnum].pos) > 3000.0f*count/desire)) {
				ai_issue_rearm_request(objp);
				return 1;
			} else {
				//nprintf(("AI", "Would like to rearm, but enemy only %7.3f units away.\n", vm_vec_dist_quick(&objp->pos, &Objects[objnum].pos)));
			}
		}
	}

	return 0;

}

void ai_set_mode_warp_out(object *objp, ai_info *aip)
{
	ai_abort_rearm_request(objp);
	if (aip->mode != AIM_WARP_OUT) {
		aip->mode = AIM_WARP_OUT;
		aip->submode = AIS_WARP_1;
	}
}

//	Maybe warp ship out.
//	Shivan and HoL fighter/bomber warp out if their weapons subsystems have been destroyed.
void ai_maybe_warp_out(object *objp)
{
	ship	*shipp;

	// don't do anything if in a training mission.
	if ( The_mission.game_type & MISSION_TYPE_TRAINING )
		return;

	Assert(objp->type == OBJ_SHIP);

	shipp = &Ships[objp->instance];
	ai_info	*aip = &Ai_info[shipp->ai_index];

	if (aip->mode == AIM_WARP_OUT)
		return;

	//	If a support ship with no goals and low hull, warp out.  Be sure that there are no pending goals
	// in the support ships ai_goal array.  Just process this ships goals.
	ship_info	*sip = &Ship_info[shipp->ship_info_index];
	if (sip->flags & SIF_SUPPORT) {
		if ( timestamp_elapsed(aip->warp_out_timestamp) ) {
			ai_process_mission_orders( OBJ_INDEX(objp), aip );
			if ( (aip->dock_objnum == -1) && (objp->hull_strength/sip->initial_hull_strength < 0.25f) ) {
				ai_set_mode_warp_out(objp, aip);
			}
		}
	}

	//	Friendly don't warp out, they'll eventually request support.
	if (shipp->team == TEAM_FRIENDLY)
		return;

	if (!(shipp->flags & SF_DEPARTING)) {
		ship_info	*sip;

		sip = &Ship_info[shipp->ship_info_index];
		if (sip->flags & (SIF_FIGHTER | SIF_BOMBER)) {
			if (aip->warp_out_timestamp == 0) {
				//if (ship_get_subsystem_strength(shipp, SUBSYSTEM_WEAPONS) == 0.0f) {
				//	aip->warp_out_timestamp = timestamp(((myrand() % 10) + 10) * 1000);
				//}
			} else if (timestamp_elapsed(aip->warp_out_timestamp)) {
				ai_set_mode_warp_out(objp, aip);
			}
		}
	}
}

//	Warp this ship out.
void ai_warp_out(object *objp)
{
	// if dying, don't warp out.
	if (Ships[objp->instance].flags & SF_DYING) {
		return;
	}

	ai_info	*aip;

	aip = &Ai_info[Ships[objp->instance].ai_index];

	switch (aip->submode) {
	case AIS_WARP_1:
		aip->force_warp_time = timestamp(10*1000);	//	Try to avoid a collision for up to ten seconds.
		aip->submode = AIS_WARP_2;
		break;
	case AIS_WARP_2:			//	Make sure won't collide with any object.
		if (timestamp_elapsed(aip->force_warp_time) || !collide_predict_large_ship(objp, objp->radius*2.0f + 100.0f)) {
			aip->submode = AIS_WARP_3;

			// maybe recalculate collision pairs.
			if (ship_get_warp_speed(objp) > ship_get_max_speed(&Ships[objp->instance])) {
				// recalculate collision pairs
				OBJ_RECALC_PAIRS(objp);	
			}

			aip->force_warp_time = timestamp(4*1000);		//	Try to attain target speed for up to 4 seconds.
		} else {
			vector	goal_point;
			vm_vec_scale_add(&goal_point, &objp->pos, &objp->orient.v.uvec, 100.0f);
			turn_towards_point(objp, &goal_point, NULL, 0.0f);
			accelerate_ship(aip, 0.0f);
		}
		break;
	case AIS_WARP_3:
		//	Rampup desired_vel in here from current to desired velocity and set PF_USE_VEL. (not sure this is the right flag)
		//	desired velocity is computed in shipfx_calculate_warp_time().  See shipfx#572 for sample code.
		float	speed, goal_speed;
		float shipfx_calculate_warp_speed(object*);
		goal_speed = shipfx_calculate_warp_speed(objp);

		// HUGE ships go immediately to AIS_WARP_4
		if (Ship_info[Ships[objp->instance].ship_info_index].flags & SIF_HUGE_SHIP) {
			aip->submode = AIS_WARP_4;
			break;
		}
		//compute_warpout_stuff(objp, &goal_speed, &warp_time, &warp_pos);
		//goal_speed = 80.0f;
		//set_accel_for_target_speed(objp, 40.0f);
		// DKA 8/11/99 objp->phys_info.flags |= PF_USE_VEL;	This flag is set in object code if warping out and AIS_WARP >= 3, properly accounting for blown engines
		speed = goal_speed * flFrametime + objp->phys_info.speed * (1.0f - flFrametime);
		vm_vec_copy_scale(&objp->phys_info.vel, &objp->orient.v.fvec, speed);
		objp->phys_info.desired_vel = objp->phys_info.vel;
		// nprintf(("AI", "Frame %i, speed = %7.3f, goal = %7.3f\n", Framecount, vm_vec_mag_quick(&objp->phys_info.vel), goal_speed));
		if (timestamp_elapsed(aip->force_warp_time) || (fl_abs(objp->phys_info.speed - goal_speed) < 2.0f))
			aip->submode = AIS_WARP_4;
		break;
	case AIS_WARP_4: {
		shipfx_warpout_start(objp);
		aip->submode = AIS_WARP_5;
		break;
	}
	case AIS_WARP_5:
		break;
	default:
		Int3();		//	Illegal submode for warping out.
	}
}

//	Return object index of weapon that could produce a shockwave that should be known about to *objp.
//	Return nearest one.
int ai_find_shockwave_weapon(object *objp, ai_info *aip)
{
	missile_obj	*mo;
	float	nearest_dist = 999999.9f;
	int	nearest_index = -1;

	for ( mo = GET_NEXT(&Missile_obj_list); mo != END_OF_LIST(&Missile_obj_list); mo = GET_NEXT(mo) ) {
		object		*A;
		weapon		*wp;
		weapon_info	*wip;
	
		Assert(mo->objnum >= 0 && mo->objnum < MAX_OBJECTS);
		A = &Objects[mo->objnum];

		Assert(A->type == OBJ_WEAPON);
		Assert((A->instance >= 0) && (A->instance < MAX_WEAPONS));
		wp = &Weapons[A->instance];
		wip = &Weapon_info[wp->weapon_info_index];
		Assert( wip->subtype == WP_MISSILE );

		if (wip->shockwave_speed > 0.0f) {
			float	dist;

			dist = vm_vec_dist_quick(&objp->pos, &A->pos);
			if (dist < nearest_dist) {
				nearest_dist = dist;
				nearest_index = mo->objnum;
			}
		}
	}

	return nearest_index;

}

#define	EVADE_SHOCKWAVE_DAMAGE_THRESHOLD		100.0f

//	Tell all ships to avoid a big ship that is blowing up.
//	Only avoid if shockwave is fairly large.
//	OK to tell everyone to avoid.  If they're too far away, that gets cleaned up in the frame interval.
void ai_announce_ship_dying(object *dying_objp)
{
	float damage = ship_get_exp_damage(dying_objp);
	if (damage >= EVADE_SHOCKWAVE_DAMAGE_THRESHOLD) {
		ship_obj	*so;

		for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
			if (Ship_info[Ships[Objects[so->objnum].instance].ship_info_index].flags & (SIF_SMALL_SHIP | SIF_FREIGHTER)) {
				ai_info	*aip;

				aip = &Ai_info[Ships[Objects[so->objnum].instance].ai_index];

				if ( !(aip->ai_flags & (AIF_DOCKED|AIF_BEING_REPAIRED)) ) {
					aip->ai_flags |= AIF_AVOID_SHOCKWAVE_SHIP;
				}
			}
		}
	}
}


//	Return object index of weapon that could produce a shockwave that should be known about to *objp.
//	Return nearest one.
int ai_find_shockwave_ship(object *objp, ai_info *aip)
{
	ship_obj	*so;
	float	nearest_dist = 999999.9f;
	int	nearest_index = -1;

	for ( so = GET_NEXT(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		object		*A;
		ship			*shipp;
	
		Assert(so->objnum >= 0 && so->objnum < MAX_OBJECTS);
		A = &Objects[so->objnum];

		Assert(A->type == OBJ_SHIP);
		Assert((A->instance >= 0) && (A->instance < MAX_SHIPS));
		shipp = &Ships[A->instance];
		//	Only look at objects in the process of dying.
		if (shipp->flags & SF_DYING) {
			float damage = ship_get_exp_damage(objp);

			if (damage >= EVADE_SHOCKWAVE_DAMAGE_THRESHOLD) {		//	Only evade quite large blasts
				float	dist;

				dist = vm_vec_dist_quick(&objp->pos, &A->pos);
				if (dist < nearest_dist) {
					nearest_dist = dist;
					nearest_index = so->objnum;
				}
			}
		}
	}

	return nearest_index;

}

int aas_1(object *objp, ai_info *aip, vector *safe_pos)
{
	// MAKE SURE safe_pos DOES NOT TAKE US TOWARD THE A SHIP WE'RE ATTACKING.
	if (aip->ai_flags & AIF_AVOID_SHOCKWAVE_WEAPON) {
		//	If we don't currently know of a weapon to avoid, try to find one.
		//	If we can't find one, then clear the bit so we don't keep coming here.
		if (aip->shockwave_object == -1) {
			int shockwave_weapon = ai_find_shockwave_weapon(objp, aip);
			if (shockwave_weapon == -1) {
				aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_WEAPON;
				return 0;
			} else {
				aip->shockwave_object = shockwave_weapon;
			}
		}

		//	OK, we have reason to believe we should avoid aip->shockwave_object.
		Assert(aip->shockwave_object > -1);
		object	*weapon_objp = &Objects[aip->shockwave_object];
		if (weapon_objp->type != OBJ_WEAPON) {
			aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_WEAPON;
			aip->shockwave_object = -1;
			return 0;
		}

		weapon	*weaponp = &Weapons[weapon_objp->instance];
		weapon_info	*wip = &Weapon_info[weaponp->weapon_info_index];
		object *target_ship_obj = NULL;

		if (wip->shockwave_speed == 0.0f) {
			aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_WEAPON;
			aip->shockwave_object = -1;
			return 0;
		}

		float	danger_dist;
		vector	expected_pos;		//	Position at which we expect the weapon to detonate.
		int		pos_set = 0;

		danger_dist = wip->outer_radius;
		//	Set predicted position of detonation.
		//	If an aspect locked missile, assume it will detonate at the homing position.
		//	If not, which is not possible in a default FreeSpace weapon, then predict it will detonate at some
		//	time in the future, this time based on max lifetime and life left.
		if (wip->wi_flags & WIF_HOMING_ASPECT) {
			expected_pos = weaponp->homing_pos;
			if (weaponp->homing_object && weaponp->homing_object->type == OBJ_SHIP) {
				target_ship_obj = weaponp->homing_object;
			}
			pos_set = 1;
			if (IS_VEC_NULL(&weaponp->homing_pos)) {
				pos_set = 0;
				if (weaponp->target_num != -1) {
					if (Objects[weaponp->target_num].type == OBJ_SHIP) {
						target_ship_obj = &Objects[weaponp->target_num];
						expected_pos = target_ship_obj->pos;
						pos_set = 1;
					}
				}
			}
		}

		if (!pos_set) {
			float	time_scale;

			if (wip->lifetime - weaponp->lifeleft > 5.0f) {
				time_scale = 1.0f;
			} else {
				time_scale = weaponp->lifeleft/2.0f;
			}

			vm_vec_scale_add(&expected_pos, &weapon_objp->pos, &weapon_objp->orient.v.fvec, time_scale);
		}

		//	See if too far away to care about shockwave.
		if (vm_vec_dist_quick(&objp->pos, &expected_pos) > danger_dist*2.0f) {
			//aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_WEAPON;
			return 0;
		} else {
			// try to find a safe position
			vector vec_from_exp;
			float dir = 1.0f;
			vm_vec_sub(&vec_from_exp, &objp->pos, &expected_pos);
			float dot = vm_vec_dotprod(&vec_from_exp, &weapon_objp->orient.v.fvec);
			if (dot > -30) {
				// if we're already on the other side of the explosion, don't try to fly behind it
				dir = -1.0f;
			}

			//	Fly towards a point behind the weapon.
			vm_vec_scale_add(safe_pos, &weapon_objp->pos, &weapon_objp->orient.v.fvec, -50000.0f*dir);

			// verify safe_pos will not make us collide with our target objnum, else try 2 other vecs
			// don't bang your head, else go
//			int go_safe = FALSE;
			int go_safe = TRUE;
/*			if (target_ship_obj) {
				if (pp_collide(&objp->pos, safe_pos, target_ship_obj, objp->radius)) {
					// try up to 2 other random directions
					vector dir_vec, rand_vec;
					int idx;
					for (idx=0; idx<2; idx++) {
						vm_vec_rand_vec_quick(&rand_vec);
						vm_vec_scale_add(&dir_vec, &weapon_objp->orient.v.fvec, &rand_vec, 0.5f);
						vm_vec_scale_add(safe_pos, &weapon_objp->pos, &dir_vec, -50000.0f*dir);
						if ( !pp_collide(&objp->pos, safe_pos, target_ship_obj, objp->radius) ) {
							go_safe = TRUE;
							break;
						}
					}
				} else { // direct path is safe
					go_safe = TRUE;
				}
			} else { // no target_obj_ship
				go_safe = TRUE;
			} */

			if (go_safe) {
				return 1;
			} else {
				// can't figure out a good way to go
				return 0;
			}
		}
	} else if (aip->ai_flags & AIF_AVOID_SHOCKWAVE_SHIP) {
		if (aip->shockwave_object == -1) {
			int shockwave_ship = ai_find_shockwave_ship(objp, aip);
			if (shockwave_ship == -1) {
				aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_SHIP;
				return 0;
			} else {
				aip->shockwave_object = shockwave_ship;
			}
		}

		Assert(aip->shockwave_object > -1);
		object	*ship_objp = &Objects[aip->shockwave_object];
		if (ship_objp == objp) {
			aip->shockwave_object = -1;
			return 0;
		}

		if (ship_objp->type != OBJ_SHIP) {
			aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_SHIP;
			return 0;
		}

		//	Optimize note! Don't really have to normalize.  We only need a point away from the blowing-up ship.
		vector safe_vec;

		vm_vec_normalized_dir(&safe_vec, &objp->pos, &ship_objp->pos);
		vm_vec_scale_add(safe_pos, &ship_objp->pos, &safe_vec, 50000.0f);	//	Fly away from the ship.

		float outer_rad = ship_get_exp_outer_rad(ship_objp);

		if (vm_vec_dist_quick(&objp->pos, &ship_objp->pos) > outer_rad*1.5f) {
			aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_WEAPON;
			return 0;
		}

		return 1;

	} else {
		Int3();	//	Illegal -- supposedly avoiding a shockwave, but neither ship nor weapon.  What is it!?
	}

	return 0;
}

/*
int rct_done = 0;

void rand_chance_test()
{
	int	i;
	float	frametime;

	if (rct_done)
		return;

	rct_done = 1;

	for (frametime=0.02f; frametime<0.25f; frametime *= 1.25f) {
		float	chance;

		nprintf(("AI", "%6.4f: ", frametime));
		for (chance=0.25f; chance<2.5f; chance += 0.25f) {
			int count = 0;

			for (i=0; i<100.0f/frametime; i++) {
				if (rand_chance(frametime, chance))
					count++;
			}
			nprintf(("AI", "%3i ", count));
		}
		nprintf(("AI", "\n"));
	}
}
*/

//	--------------------------------------------------------------------------
//	Make object *objp avoid the nearest dangerous shockwave-producing weapon.
//	If it looks like there is no valid shockwave-producing weapon then clear the AIF_AVOID_SHOCKWAVE_WEAPON bit in ai_flags and return.
//	Return 1 if avoiding a shockwave, else return 0.
int ai_avoid_shockwave(object *objp, ai_info *aip)
{
	vector	safe_pos;

	//rand_chance_test();
	// BIG|HUGE do not respond to shockwaves
	if (Ship_info[Ships[objp->instance].ship_info_index].flags & (SIF_BIG_SHIP|SIF_HUGE_SHIP)) {
		// don't come here again
		aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE;
		return 0;
	}

	//	Don't all react right away.
	if (!(aip->ai_flags & AIF_AVOID_SHOCKWAVE_STARTED))
		if (!rand_chance(flFrametime, (float) aip->ai_class/4.0f + 0.25f))	//	Chance to avoid in 1 second is 0.25 + ai_class/4
			return 0;

	if (!aas_1(objp, aip, &safe_pos)) {
		aip->ai_flags |= AIF_AVOID_SHOCKWAVE_STARTED;
		return 0;
	}

	aip->ai_flags |= AIF_AVOID_SHOCKWAVE_STARTED;

	//	OK, evade the shockwave!
	turn_towards_point(objp, &safe_pos, NULL, 0.0f);
	vector	vec_to_safe_pos;
	float		dot_to_goal;

	vm_vec_normalized_dir(&vec_to_safe_pos, &safe_pos, &objp->pos);

	dot_to_goal = vm_vec_dot(&objp->orient.v.fvec, &vec_to_safe_pos);
	if (dot_to_goal < -0.5f)
		accelerate_ship(aip, 0.3f);
	else {
		accelerate_ship(aip, 1.0f + dot_to_goal);
		if (dot_to_goal > 0.2f) {
			if (!(objp->phys_info.flags & PF_AFTERBURNER_ON )) {
				afterburners_start(objp);
				aip->afterburner_stop_time = Missiontime + 2*F1_0;
			}
		}
	}

	return 1;
}

//	Awaiting repair.  Be useful.
//	Probably fly towards incoming repair ship.
//	Return true if this ship is close to being repaired, else return false.
int ai_await_repair_frame(object *objp, ai_info *aip)
{
	if (!(aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED)))
		return 0;

	if (aip->dock_objnum == -1)
		return 0;

	ship	*shipp;
	ship_info	*sip;

	shipp = &Ships[Objects[aip->dock_objnum].instance];
	sip = &Ship_info[shipp->ship_info_index];

	aip->ai_flags &= ~AIF_FORMATION_OBJECT;	//	Prevents endless rotation.

	if (!(sip->flags & SIF_SUPPORT))
		return 0;

	vector	goal_point;
	object	*repair_objp;

	repair_objp = &Objects[aip->dock_objnum];

	if (Ships[repair_objp->instance].team == TEAM_TRAITOR) {
		ai_abort_rearm_request(repair_objp);
		return 0;
	}

	vm_vec_scale_add(&goal_point, &repair_objp->pos, &repair_objp->orient.v.uvec, -50.0f);	//	Fly towards point below repair ship.

	vector	vtr;
	float dist = vm_vec_normalized_dir(&vtr, &goal_point, &objp->pos);
	float dot = vm_vec_dot(&vtr, &objp->orient.v.fvec);

	if (dist > 200.0f) {
		//nprintf(("AI", "%s flying towards %s for repair, dist = %7.3f\n", Ships[objp->instance].ship_name, &Ships[repair_objp->instance].ship_name, dist));
		accelerate_ship(aip, (0.9f + dot) * dist/1500.0f);
		turn_towards_point(objp, &goal_point, NULL, 0.0f);
	} else {
		accelerate_ship(aip, 0.0f);
		//nprintf(("AI", "%s sitting still awaiting repair from %s, dist = %7.3f\n", Ships[objp->instance].ship_name, &Ships[repair_objp->instance].ship_name, dist));
	}

	return 1;
}

//	Maybe cause this ship to self-destruct.
//	Currently, any small ship (SIF_SMALL_SHIP) that has been disabled will self-destruct after awhile.
//	Maybe should only do this if they are preventing their wing from re-entering.
void ai_maybe_self_destruct(object *objp, ai_info *aip)
{
	//	Friendly ships can be repaired, so no self-destruct.
	//	In multiplayer, just don't self-destruct.  I figured there would be a problem. -- MK, 3/19/98.
	if ((Ships[objp->instance].team == TEAM_FRIENDLY) || (Game_mode & GM_MULTIPLAYER))
		return;

	//	Small ships in a wing blow themselves up after awhile if engine or weapons system has been destroyed.
	//	Reason: Don't want them to prevent a re-emergence of the wing.
	//	Note: Don't blow up if not in a wing for two reasons: One, won't affect re-emergence of waves and (1) disable the Dragon
	//	mission would be broken.
	if ((Ship_info[Ships[objp->instance].ship_info_index].flags & SIF_SMALL_SHIP) && (Ships[objp->instance].wingnum != -1)) {
		if ((ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_ENGINE) <= 0.0f) ||
			(ship_get_subsystem_strength(&Ships[objp->instance], SUBSYSTEM_WEAPONS) <= 0.0f)) {
			if (aip->self_destruct_timestamp < 0)
				aip->self_destruct_timestamp = timestamp(90 * 1000);	//	seconds until self-destruct
		} else {
			aip->self_destruct_timestamp = -1;
		}

		if (aip->self_destruct_timestamp < 0) {
			return;
		}

		if (timestamp_elapsed(aip->self_destruct_timestamp)) {
			ship_apply_local_damage( objp, objp, &objp->pos, objp->hull_strength*flFrametime + 1.0f, MISS_SHIELDS);
		}
	}
}

// Determine if pl_objp needs a new target, called from ai_frame()
int ai_need_new_target(object *pl_objp, int target_objnum)
{
	object *objp;

	if ( target_objnum < 0 ) {
		return 1;
	}

	objp = &Objects[target_objnum];

	if ( (objp->type != OBJ_SHIP) && (objp->type != OBJ_ASTEROID) && (objp->type != OBJ_WEAPON) ) {
		return 1;
	}

	if ( objp->type == OBJ_SHIP ) {
		if ( Ships[objp->instance].flags & SF_DYING ) {
			return 1;
		} else if (Ships[objp->instance].team == Ships[pl_objp->instance].team)
			return 1;
	}

	return 0;
}

//	If *objp is recovering from a collision with a big ship, handle it.
//	Return true if recovering.
int maybe_big_ship_collide_recover_frame(object *objp, ai_info *aip)
{
	float	dot, dist;
	vector	v2g;
	
	if (aip->ai_flags & AIF_BIG_SHIP_COLLIDE_RECOVER_1) {
		ai_turn_towards_vector(&aip->big_recover_pos_1, objp, flFrametime, Ship_info[Ships[objp->instance].ship_info_index].srotation_time, NULL, NULL, 0.0f, 0, NULL);
		dist = vm_vec_normalized_dir(&v2g, &aip->big_recover_pos_1, &objp->pos);
		dot = vm_vec_dot(&objp->orient.v.fvec, &v2g);
		accelerate_ship(aip, dot);

		//	If close to desired point, or 15+ seconds since entered this mode, continue to next mode.
		if ((timestamp_until(aip->big_recover_timestamp) < -15*1000) || (dist < (0.5f + flFrametime) * objp->phys_info.speed)) {
			aip->ai_flags &= ~AIF_BIG_SHIP_COLLIDE_RECOVER_1;
			aip->ai_flags |= AIF_BIG_SHIP_COLLIDE_RECOVER_2;
		}

		return 1;

	} else if (aip->ai_flags & AIF_BIG_SHIP_COLLIDE_RECOVER_2) {
		ai_turn_towards_vector(&aip->big_recover_pos_2, objp, flFrametime, Ship_info[Ships[objp->instance].ship_info_index].srotation_time, NULL, NULL, 0.0f, 0, NULL);
		dist = vm_vec_normalized_dir(&v2g, &aip->big_recover_pos_2, &objp->pos);
		dot = vm_vec_dot(&objp->orient.v.fvec, &v2g);
		accelerate_ship(aip, dot);

		//	If close to desired point, or 30+ seconds since started avoiding collision, done avoiding.
		if ((timestamp_until(aip->big_recover_timestamp) < -30*1000) || (dist < (0.5f + flFrametime) * objp->phys_info.speed)) {
			aip->ai_flags &= ~AIF_BIG_SHIP_COLLIDE_RECOVER_2;
			aip->ai_flags &= ~AIF_TARGET_COLLISION;
		}

		return 1;
	}

	if (aip->ai_flags & AIF_TARGET_COLLISION) {
		aip->ai_flags &= ~AIF_TARGET_COLLISION;
	}
	return 0;
}

void validate_mode_submode(ai_info *aip)
{
	switch (aip->mode) {
	case AIM_CHASE:
		// check valid submode
		switch (aip->submode) {
		case SM_CONTINUOUS_TURN:
		case SM_ATTACK:
		case SM_EVADE_SQUIGGLE:
		case SM_EVADE_BRAKE:	
		case SM_EVADE:		
		case SM_SUPER_ATTACK:
		case SM_AVOID:	
		case SM_GET_BEHIND:
		case SM_GET_AWAY:		
		case SM_EVADE_WEAPON:
		case SM_FLY_AWAY:	
		case SM_ATTACK_FOREVER:
			break;
		default:
			Int3();
		}
		break;

	case AIM_STRAFE:
		// check valid submode
		switch(aip->submode) {
		case AIS_STRAFE_ATTACK:
		case AIS_STRAFE_AVOID:
		case AIS_STRAFE_RETREAT1:
		case AIS_STRAFE_RETREAT2:
		case AIS_STRAFE_POSITION:
			break;
		default:
			Int3();
		}
		break;
	}
}

//	--------------------------------------------------------------------------
// Process AI object "objnum".
void ai_frame(int objnum)
{
	ship		*shipp = &Ships[Objects[objnum].instance];
	ai_info	*aip = &Ai_info[shipp->ai_index];
	int		target_objnum;

//	validate_mode_submode(aip);

	Assert((aip->mode != AIM_WAYPOINTS) || (aip->active_goal != AI_ACTIVE_GOAL_DYNAMIC));

	// Set globals defining the current object and its enemy object.
	Pl_objp = &Objects[objnum];

	if (aip->mode == AIM_WARP_OUT) {
		ai_warp_out(Pl_objp);
		return;
	}

/*	//	HACK! TEST! REMOVE ME!
	if (Ship_info[shipp->ship_info_index].flags & SIF_BIG_SHIP)
		if (shipp->team == Player_ship->team)
			aip->mode = AIM_CHASE;
*/

//	if (!strnicmp(Ships[Pl_objp->instance].ship_name, "cancer", 6))
//		nprintf(("AI", "Ship %s: mode = %s, submode = %i\n", Ships[Pl_objp->instance].ship_name, Mode_text[aip->mode], aip->submode));

	ai_maybe_self_destruct(Pl_objp, aip);

//	if ( timestamp_elapsed(aip->goal_check_time) ) {
		ai_process_mission_orders( objnum, aip );
//		aip->goal_check_time = timestamp_rand(1000,2000);
//	}

	//	Avoid a shockwave, if necessary.  If a shockwave and rearming, stop rearming.
	if (aip->ai_flags & AIF_AVOID_SHOCKWAVE) {
		if (ai_avoid_shockwave(Pl_objp, aip)) {
			aip->ai_flags &= ~(AIF_BIG_SHIP_COLLIDE_RECOVER_1 | AIF_BIG_SHIP_COLLIDE_RECOVER_2);
			if (aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED))
				ai_abort_rearm_request(Pl_objp);
			return;
		}
	} else {
		aip->ai_flags &= ~AIF_AVOID_SHOCKWAVE_STARTED;
	}

	// moved call to ai_do_repair frame here from below because of the subsequent if statment returning
	// if the ship is getting repaired
	//	If waiting to be repaired, just stop and sit.
	ai_do_repair_frame(Pl_objp, aip, flFrametime);
	if ((aip->ai_flags & AIF_AWAITING_REPAIR) || (aip->ai_flags & AIF_BEING_REPAIRED)) {
		if (ai_await_repair_frame(Pl_objp, aip))
			return;
	}

	if (aip->mode == AIM_PLAY_DEAD)
		return;

	//	If recovering from a collision with a big ship, don't continue.
	if (maybe_big_ship_collide_recover_frame(Pl_objp, aip))
		return;

	ai_preprocess_ignore_objnum(Pl_objp, aip);
	target_objnum = set_target_objnum(aip, aip->target_objnum);

	// nprintf(("AI", "Frame %i: Coords = %7.3f %7.3f %7.3f\n", AI_FrameCount, Pl_objp->pos.xyz.x, Pl_objp->pos.xyz.y, Pl_objp->pos.xyz.z));

	Assert(objnum != target_objnum);

	ai_manage_shield(Pl_objp, aip);
	
	if ( maybe_request_support(Pl_objp) ) {
		if ( Ships[Pl_objp->instance].flags & SF_FROM_PLAYER_WING ) {
			ship_maybe_tell_about_rearm(shipp);
		}
	}

	ai_maybe_warp_out(Pl_objp);

/*
	//	If this ship is attacking an object's subsystems and someone else destroyed
	//	the subsystem, it could continue attacking the ship.  Need to invalidate the objnum.
	if (target_objnum >= 0)
		if (Objects[target_objnum].flags & OF_PROTECTED) {
			// if (aip->targeted_subsys != NULL)
			//	; //nprintf(("AI", "subsys hits = %7.3f\n", aip->targeted_subsys->current_hits));

			if ((aip->targeted_subsys == NULL) || (aip->targeted_subsys->current_hits <= 0.0f)) {
				target_objnum = -1;
				aip->target_objnum = -1;
			}
		}
*/


	//	Find an enemy if don't already have one.
	En_objp = NULL;
	if ( ai_need_new_target(Pl_objp, target_objnum) ) {
		if ((aip->mode != AIM_EVADE_WEAPON) && (aip->active_goal == AI_ACTIVE_GOAL_DYNAMIC)) {
			aip->resume_goal_time = -1;
			aip->active_goal = AI_GOAL_NONE;
		} else if (aip->resume_goal_time == -1) {
			// AL 12-9-97: Don't allow cargo and navbuoys to set their aip->target_objnum
			if ( !(Ship_info[shipp->ship_info_index].flags & SIF_HARMLESS) ) {
				target_objnum = find_enemy(objnum, MAX_ENEMY_DISTANCE, Skill_level_max_attackers[Game_skill_level]);		//	Attack up to 25K units away.
				if (target_objnum != -1) {
					if (aip->target_objnum != target_objnum)
						aip->aspect_locked_time = 0.0f;
					set_target_objnum(aip, target_objnum);
					En_objp = &Objects[target_objnum];
				}
			}
		}
	} else if (target_objnum >= 0) {
		En_objp = &Objects[target_objnum];
	}

	// set base stealth info each frame
	aip->ai_flags &= ~AIF_STEALTH_PURSIUT;
	if (En_objp && En_objp->type == OBJ_SHIP) {
		if (Ship_info[Ships[En_objp->instance].ship_info_index].flags & SIF_STEALTH) {
			int stealth_state = ai_is_stealth_visible(Pl_objp, En_objp);
			float dist = vm_vec_dist_quick(&En_objp->pos, &Pl_objp->pos);

			if (stealth_state != STEALTH_FULLY_TARGETABLE) {
				aip->ai_flags |= AIF_STEALTH_PURSIUT;
			}

			if ( (stealth_state == STEALTH_FULLY_TARGETABLE) || (stealth_state == STEALTH_VISIBLE) ) {
				aip->stealth_last_visible_stamp = timestamp();
				aip->stealth_last_cheat_visible_stamp = aip->stealth_last_visible_stamp;
				aip->stealth_last_pos = En_objp->pos;
				aip->stealth_velocity = En_objp->phys_info.vel;
			} else if (dist < 100) {
				// get cheat timestamp
				aip->stealth_last_cheat_visible_stamp = timestamp();

				// set approximate pos and vel, with increasing error as time from last_visible_stamp increases
				update_ai_stealth_info_with_error(aip/*, 0*/);
			}
		}
	}

	/*	if ((Pl_objp != NULL) && (En_objp != NULL)) {
		slide_face_ship();
		return;
	}
*/
	// AL 12-10-97: ensure that cargo and navbuoys aip->target_objnum is always -1.
	if ( Ship_info[shipp->ship_info_index].flags & SIF_HARMLESS ) {
		aip->target_objnum = -1;
	}

	if ((En_objp != NULL) && (En_objp->pos.xyz.x == Pl_objp->pos.xyz.x) && (En_objp->pos.xyz.y == Pl_objp->pos.xyz.y) && (En_objp->pos.xyz.z == Pl_objp->pos.xyz.z)) {
		mprintf(("Warning: Object and its enemy have same position.  Object #%i\n", Pl_objp-Objects));
		En_objp = NULL;
	}

	if (aip->mode == AIM_CHASE) {
		if (En_objp == NULL) {
			aip->active_goal = -1;
		}
	}

	//	If there is a goal to resume and enough time has elapsed, resume the goal.
	if ((aip->resume_goal_time > 0) && (aip->resume_goal_time < Missiontime)) {
		aip->active_goal = AI_GOAL_NONE;
		aip->resume_goal_time = -1;
		target_objnum = find_enemy(objnum, 2000.0f, Skill_level_max_attackers[Game_skill_level]);
		if (target_objnum != -1) {
			if (aip->target_objnum != target_objnum) {
				aip->aspect_locked_time = 0.0f;
			}
			set_target_objnum(aip, target_objnum);
		}
	}

	// check if targeted subsystem has been destroyed, if so, move onto another subsystem
	// if trying to disable or disarm the target
	if ((En_objp != NULL) && ( aip->targeted_subsys != NULL )) {
		Assert(En_objp->type == OBJ_SHIP);
		if ( aip->targeted_subsys->current_hits <= 0.0f ) {
			int subsys_type;

			if ( aip->goals[0].ai_mode == AI_GOAL_DISABLE_SHIP ) {
				subsys_type = SUBSYSTEM_ENGINE;
			} else if ( aip->goals[0].ai_mode == AI_GOAL_DISARM_SHIP ) {
				subsys_type = SUBSYSTEM_TURRET;
			} else {
				subsys_type = -1;
			}

			if ( subsys_type != -1 ) {
				ship_subsys *new_subsys;
				new_subsys = ship_return_next_subsys(&Ships[En_objp->instance], subsys_type, &Pl_objp->pos);
				if ( new_subsys != NULL ) {
					set_targeted_subsys(aip, new_subsys, aip->target_objnum);
				} else {
					// AL 12-16-97: no more subsystems to attack... reset targeting info
					aip->target_objnum = -1;
					set_targeted_subsys(aip, NULL, -1);
				}
			} else {
				// targeted subsys is destroyed, so stop attacking it
				set_targeted_subsys(aip, NULL, -1);
			}
		}
	}

	ai_maybe_launch_cmeasure(Pl_objp, aip);
	ai_maybe_evade_locked_missile(Pl_objp, aip);

	aip->target_time += flFrametime;

	int in_formation = 0;
	if (aip->ai_flags & AIF_FORMATION) {
		in_formation = !ai_formation();
	}

	if ( !in_formation ) {
		ai_execute_behavior(aip);
	}

	process_subobjects(objnum);
	maybe_resume_previous_mode(Pl_objp, aip);
	
	if (Pl_objp->phys_info.flags & PF_AFTERBURNER_ON ) {
		if (Missiontime > aip->afterburner_stop_time) {
			//nprintf(("AI", "Frame %i, turning off afterburner.\n", AI_FrameCount));
			afterburners_stop(Pl_objp);
		}
	}
//	validate_mode_submode(aip);
}

int Waypoints_created = 0;

//	Find the ship with the name *name in the Ship_info array.
int find_ship_name(char *name)
{
	int	i;

	for (i=0; i<Num_ship_types; i++)
		if (!strcmp(Ship_info[i].name, name))
			return i;

	return -1;
}

void create_waypoints()
{
	int	i, j, z;

	// Waypoints_created = 1;

	if (Waypoints_created)
		return;

	for (j=0; j<Num_waypoint_lists; j++)
		for (i=0; i<Waypoint_lists[j].count; i++) {
			z = obj_create(OBJ_WAYPOINT, 0, j * 65536 + i, NULL,
				&Waypoint_lists[j].waypoints[i], 0.0f, OF_RENDERS);
		}

	Waypoints_created = 1;
}

int Last_ai_obj = -1;

void ai_process( object * obj, int ai_index, float frametime )
{
//	if (Ships[obj->instance].flags & SF_DYING)
//		nprintf(("AI", "Frame: %i Ship %s is dying!\n", Framecount, Ships[obj->instance].ship_name));

	if (obj->flags & OF_SHOULD_BE_DEAD)
		return;

	// return if ship is dead, unless it's a big ship...then its turrets still fire, like I was quoted in a magazine.  -- MK, 5/15/98.
	if ((Ships[obj->instance].flags & SF_DYING ) && !(Ship_info[Ships[obj->instance].ship_info_index].flags & (SIF_HUGE_SHIP | SIF_BIG_SHIP))){
		return;
	}

	int rfc = 1;		//	Assume will be Reading Flying Controls.

	Assert( obj->type == OBJ_SHIP );
	Assert( ai_index >= 0 );

	init_ship_info();

	create_waypoints();

	AI_frametime = frametime;
	if (obj-Objects <= Last_ai_obj) {
		AI_FrameCount++;
	}

	memset( &AI_ci, 0, sizeof(AI_ci) );

	ai_frame(obj-Objects);

	AI_ci.pitch = 0.0f;
	AI_ci.bank = 0.0f;
	AI_ci.heading = 0.0f;

	// the ships maximum velocity now depends on the energy flowing to engines
	obj->phys_info.max_vel.xyz.z = Ships[obj->instance].current_max_speed;
	ai_info	*aip = &Ai_info[Ships[obj->instance].ai_index];

	//	In certain circumstances, the AI says don't fly in the normal way.
	//	One circumstance is in docking and undocking, when the ship is moving
	//	under thruster control.
	switch (aip->mode) {
	case AIM_DOCK:
		if ((aip->submode >= AIS_DOCK_2) && (aip->submode != AIS_UNDOCK_3))
			rfc = 0;
		break;
	case AIM_WARP_OUT:
		if (aip->submode >= AIS_WARP_3)
			rfc = 0;
		break;
//	case AIM_NONE:
//		if (aip->submode == AIS_NONE_FORMATION)
//			rfc = 0;
//		break;
	default:
		break;
	}

	if (rfc == 1) {
		vector copy_desired_rotvel = obj->phys_info.rotvel;
		physics_read_flying_controls( &obj->orient, &obj->phys_info, &AI_ci, frametime);
		// if obj is in formation and not flight leader, don't update rotvel
		if (aip->ai_flags & AIF_FORMATION) {
			if (&Objects[aip->goal_objnum] != obj) {
				obj->phys_info.desired_rotvel = copy_desired_rotvel;
				obj->phys_info.rotvel = copy_desired_rotvel;
			}
		}
	}

	Last_ai_obj = obj-Objects;
}

//	Initialize ai_info struct of object objnum.
void init_ai_object(int objnum)
{
	int	ship_index, ai_index;
	ai_info	*aip;
	int ship_type;
	object	*objp;
	vector	near_vec;			//	A vector nearby and mainly in front of this object.

	objp = &Objects[objnum];
	ship_index = objp->instance;
	ai_index = Ships[ship_index].ai_index;
	Assert((ai_index >= 0) && (ai_index < MAX_AI_INFO));

	aip = &Ai_info[ai_index];

	ship_type = Ships[ship_index].ship_info_index;

	vm_vec_scale_add(&near_vec, &objp->pos, &objp->orient.v.fvec, 100.0f);
	vm_vec_scale_add2(&near_vec, &objp->orient.v.rvec, 10.0f);

	// Things that shouldn't have to get initialized, but initialize them just in case!
	aip->ai_flags = 0;
	aip->previous_mode = AIM_NONE;
	aip->mode_time = -1;
	aip->target_objnum = -1;
	aip->target_signature = -1;
	aip->previous_target_objnum = -1;
	aip->target_time = 0.0f;
	aip->enemy_wing = -1;
	aip->attacker_objnum = -1;
	aip->goal_objnum = -1;
	aip->goal_signature = -1;
	aip->guard_objnum = -1;
	aip->guard_signature = -1;
	aip->guard_wingnum = -1;
	aip->dock_signature = -1;
	aip->submode = 0;
	aip->previous_submode = 0;
	aip->best_dot_to_enemy = -1.0f;
	aip->best_dot_from_enemy = -1.0f;
	aip->best_dot_to_time = 0;
	aip->best_dot_from_time = 0;
	aip->submode_start_time = 0;
	aip->submode_parm0 = 0;
	aip->active_goal = -1;
	aip->goal_check_time = timestamp(0);
	aip->last_predicted_enemy_pos = near_vec;
	aip->prev_goal_point = near_vec;
	aip->goal_point = near_vec;
	aip->time_enemy_in_range = 0.0f;
	aip->last_attack_time = 0;
	aip->last_hit_time = 0;
	aip->last_hit_quadrant = 0;
	aip->hitter_objnum = -1;
	aip->hitter_signature = -1;
	aip->resume_goal_time = -1;
	aip->prev_accel = 0.0f;
	aip->prev_dot_to_goal = 0.0f;

	aip->ignore_objnum = UNUSED_OBJNUM;
	aip->ignore_signature = -1;

	// aip->mode = AIM_NONE;

	// End of Things that shouldn't have to get initialized, but initialize them just in case!

	aip->ai_courage = Ai_classes[Ship_info[ship_type].ai_class].ai_courage[Game_skill_level];
	aip->ai_patience = Ai_classes[Ship_info[ship_type].ai_class].ai_patience[Game_skill_level];
	aip->ai_evasion = Ai_classes[Ship_info[ship_type].ai_class].ai_evasion[Game_skill_level];
	aip->ai_accuracy = Ai_classes[Ship_info[ship_type].ai_class].ai_accuracy[Game_skill_level];

	if (Num_waypoint_lists > 0) {
		aip->wp_index = -1;
		aip->wp_list = -1;
	} else {
		aip->wp_index = -1;
		aip->wp_list = -1;
	}

	aip->attacker_objnum = -1;
	aip->goal_signature = -1;

	Objects[objnum].phys_info.prev_fvec = Objects[objnum].orient.v.fvec;

	aip->last_predicted_enemy_pos.xyz.x = 0.0f;	//	Says this value needs to be recomputed!
	aip->time_enemy_in_range = 0.0f;

	aip->resume_goal_time = -1;					//	Say there is no goal to resume.

	aip->active_goal = -1;
	aip->path_start = -1;
	aip->path_goal_dist = -1;
	aip->path_length = 0;
	aip->path_subsystem_next_check = 1;
	aip->dock_path_index = -1;
	aip->dock_index = -1;
	aip->dock_objnum = -1;

	aip->danger_weapon_objnum = -1;
	aip->danger_weapon_signature = -1;

	aip->lead_scale = 0.0f;
	aip->last_hit_target_time = Missiontime;

	aip->nearest_locked_object = -1;
	aip->nearest_locked_distance = 99999.0f;

	aip->targeted_subsys = NULL;
	aip->last_subsys_target = NULL;
	aip->targeted_subsys_parent = -1;

	// The next two fields are used to time the rearming to allow useful sound effects for missile rearming
	aip->rearm_first_missile = TRUE;		//	flag to indicate that next missile to load is the first missile
	aip->rearm_release_delay = 0;			//	timestamp to delay the separation of docked ships after rearm

	aip->next_predict_pos_time = 0;

	aip->afterburner_stop_time = 0;
	aip->last_objsig_hit = -1;				// object signature of the ship most recently hit by aip

	aip->path_next_create_time = timestamp(1);
	aip->path_create_pos = Objects[objnum].pos;
	aip->path_create_orient = Objects[objnum].orient;

	aip->ignore_expire_timestamp = timestamp(1);
	aip->warp_out_timestamp = 0;
	aip->next_rearm_request_timestamp = timestamp(1);
	aip->primary_select_timestamp = timestamp(1);
	aip->secondary_select_timestamp = timestamp(1);
	aip->scan_for_enemy_timestamp = timestamp(1);

	aip->choose_enemy_timestamp = timestamp(3*(NUM_SKILL_LEVELS-Game_skill_level) * ((rand_alt() % 500) + 500));

	aip->shockwave_object = -1;
	aip->shield_manage_timestamp = timestamp(1);
	aip->self_destruct_timestamp = -1;	//	This is a flag that we have not yet set this.
	aip->ok_to_target_timestamp = timestamp(1);
	aip->pick_big_attack_point_timestamp = timestamp(1);
	vm_vec_zero(&aip->big_attack_point);

	aip->avoid_check_timestamp = timestamp(1);

	aip->abort_rearm_timestamp = -1;

	// artillery stuff
	aip->artillery_objnum = -1;
	aip->artillery_sig = -1;	

	// waypoint speed cap
	aip->waypoint_speed_cap = -1;

	// set lethality to enemy team
	aip->lethality = 0.0f;
}

void init_ai_objects()
{
	int	i;

	for (i=0; i<num_objects; i++){
		if (Objects[i].type == OBJ_SHIP){
			init_ai_object(i);
		}
	}
}

void init_ai_system()
{
	// MWA -- removed next line of code on 11/12/97.  When a ship is created
	// it calls init_ai_object() on it's objnum.  Doing this init at the point where
	// this function gets called messes things up.
	//init_ai_objects();

	Ppfp = Path_points;
	Waypoints_created = 0;

	Dock_path_warning_given = 0;

/*	for (int i=0; i<MAX_IGNORE_OBJECTS; i++) {
		Ignore_objects[i].objnum = -1;
		Ignore_objects[i].signature = -1;
	}
*/

}

void ai_set_default_behavior(object *obj, int classnum)
{
	ai_info	*aip;

	Assert(obj != NULL);
	Assert(obj->instance != -1);
	Assert(Ships[obj->instance].ai_index != -1);

	aip = &Ai_info[Ships[obj->instance].ai_index];

	aip->behavior = classnum;

}

void ai_do_default_behavior(object *obj)
{
	ai_info	*aip;
	int		ship_flags;

	Assert(obj != NULL);
	Assert(obj->instance != -1);
	Assert(Ships[obj->instance].ai_index != -1);

	aip = &Ai_info[Ships[obj->instance].ai_index];

	ship_flags = Ship_info[Ships[obj->instance].ship_info_index].flags;
	if (!is_instructor(obj) && (ship_flags & (SIF_FIGHTER | SIF_BOMBER))) {
		int enemy_objnum = find_enemy(OBJ_INDEX(obj), 1000.0f, Skill_level_max_attackers[Game_skill_level]);
		set_target_objnum(aip, enemy_objnum);
		aip->mode = AIM_CHASE;
		aip->submode = SM_ATTACK;
	} else if (ship_flags & (SIF_SUPPORT)) {
		aip->mode = AIM_SAFETY;
		aip->submode = AISS_1;
		aip->ai_flags &= ~(AIF_REPAIRING);
	} else if ( ship_flags & SIF_SENTRYGUN ) {
		aip->mode = AIM_SENTRYGUN;
	} else {
		aip->mode = AIM_NONE;
	}
	
	aip->submode_start_time = Missiontime;
	aip->active_goal = AI_GOAL_NONE;
}

#define	FRIENDLY_DAMAGE_THRESHOLD	50.0f		//	Display a message at this threshold.  Note, this gets scaled by Skill_level

// send the given message from objp.  called from the maybe_process_friendly_hit
// code below when a message must get send to the player when he fires on friendlies
void process_friendly_hit_message( int message, object *objp )
{
	int index;

	// no traitor in multiplayer
	if(Game_mode & GM_MULTIPLAYER){
		return;
	}

	// don't send this message if a player ship was hit.
	if ( objp->flags & OF_PLAYER_SHIP ){
		return;
	}

	// check if objp is a cargo contianer -- if so, then find a new ship to send the message
	index = objp->instance;
	if ( !(Ship_info[Ships[objp->instance].ship_info_index].flags & (SIF_FIGHTER|SIF_BOMBER)) ){
		index = -1;
	}

	// if the message is "oops" (the don't hit me message), always make come from Terran command
	if ( message == MESSAGE_OOPS ){
		index = -1;
	}

	if ( index >= 0){
		message_send_builtin_to_player( message, &Ships[index], MESSAGE_PRIORITY_HIGH, MESSAGE_TIME_ANYTIME, 0, 0, -1, -1 );
	} else {
		message_send_builtin_to_player( message, NULL, MESSAGE_PRIORITY_HIGH, MESSAGE_TIME_ANYTIME, 0, 0, -1, -1 );
	}
}

extern	void ship_set_subsystem_strength( ship *shipp, int type, float strength );

//	Object *objp_weapon, fired by *objp_hitter, hit object *objp_ship.
void maybe_process_friendly_hit(object *objp_hitter, object *objp_hit, object *objp_weapon)
{
	// no turning traitor in multiplayer
	if ( Game_mode & GM_MULTIPLAYER ) {
		return;
	}

	// ditto if mission says no traitors allowed
	if (The_mission.flags & MISSION_FLAG_NO_TRAITOR) {
		return;
	}

	if ((objp_hitter == Player_obj) && (Player_ship->team == TEAM_FRIENDLY)) {

		// AL 12-4-97: It is possible the Player is a OBJ_GHOST at this point.  If so, bail out.
		if ( objp_hitter->type != OBJ_SHIP ) {
			return;
		}

		Assert(objp_hitter->type == OBJ_SHIP);
		Assert(objp_hit->type == OBJ_SHIP);
		Assert(objp_weapon->type == OBJ_WEAPON);

		ship	*shipp_hitter = &Ships[objp_hitter->instance];
		ship	*shipp_hit = &Ships[objp_hit->instance];

		if (shipp_hitter->team != shipp_hit->team) {
			return;
		}

		// get the player
		player *pp = &Players[Player_num];

		// wacky stuff here
		if (pp->friendly_hits != 0) {
			float	time_since_last_hit = f2fl(Missiontime - pp->friendly_last_hit_time);
			if ((time_since_last_hit >= 0.0f) && (time_since_last_hit < 10000.0f)) {
				if (time_since_last_hit > 60.0f) {
					pp->friendly_hits = 0;
					pp->friendly_damage = 0.0f;
				} else if (time_since_last_hit > 2.0f) {
					pp->friendly_hits -= (int) time_since_last_hit/2;
					pp->friendly_damage -= time_since_last_hit;
				}

				if (pp->friendly_damage < 0.0f) {
					pp->friendly_damage = 0.0f;
				}

				if (pp->friendly_hits < 0) {
					pp->friendly_hits = 0;
				}
			}
		}

		float	damage;		//	Damage done by weapon.  Gets scaled down based on size of ship.

		damage = Weapon_info[Weapons[objp_weapon->instance].weapon_info_index].damage;
		
		// wacky stuff here
		ship_info *sip = &Ship_info[Ships[objp_hit->instance].ship_info_index];
		if (sip->initial_hull_strength > 1000.0f) {
			float factor = sip->initial_hull_strength / 1000.0f;
			factor = min(100.0f, factor);
			damage /= factor;
		}

		//	Don't penalize much at all for hitting cargo
		if (sip->flags & (SIF_CARGO | SIF_SENTRYGUN)) {
			damage /= 10.0f;
		}

		//	Hit ship, but not targeting it, so it's not so heinous, maybe an accident.
		if (Ai_info[shipp_hitter->ai_index].target_objnum != OBJ_INDEX(objp_hit)) {
			damage /= 5.0f;
		}

		pp->friendly_last_hit_time = Missiontime;
		pp->friendly_hits++;

		// cap damage and number of hits done this frame
		float accredited_damage = min(MAX_BURST_DAMAGE, pp->damage_this_burst + damage) - pp->damage_this_burst;
		pp->friendly_damage += accredited_damage;
		pp->damage_this_burst += accredited_damage;

		// Done with adjustments to damage.  Evaluate based on current friendly_damage
		nprintf(("AI", "Friendly damage: %.1f, threshold: %.1f, inc damage: %.1f, max burst: %d\n", pp->friendly_damage, FRIENDLY_DAMAGE_THRESHOLD * (1.0f + (float) (NUM_SKILL_LEVELS + 1 - Game_skill_level)/3.0f), pp->damage_this_burst, MAX_BURST_DAMAGE ));
		
		if (is_instructor(objp_hit)) {
			// it's not nice to hit your instructor
			if (pp->friendly_damage > FRIENDLY_DAMAGE_THRESHOLD) {
				message_send_builtin_to_player( MESSAGE_INSTRUCTOR_ATTACK, NULL, MESSAGE_PRIORITY_HIGH, MESSAGE_TIME_IMMEDIATE, 0, 0, -1, -1);
				pp->last_warning_message_time = Missiontime;
				ship_set_subsystem_strength( Player_ship, SUBSYSTEM_WEAPONS, 0.0f);

				training_fail();

				//	Instructor warp out.
				ai_set_mode_warp_out(objp_hit, &Ai_info[Ships[objp_hit->instance].ai_index]);
				gameseq_post_event( GS_EVENT_PLAYER_WARPOUT_START_FORCED );	//	Force player to warp out.

				//ship_apply_global_damage( objp_hitter, objp_hit, NULL, 2*(get_shield_strength(objp_hitter) + Ship_info[shipp_hitter->ship_info_index].initial_hull_strength) );
				//ship_apply_global_damage( objp_hitter, objp_hit, NULL, 1.0f );
			} else if (Missiontime - pp->last_warning_message_time > F1_0*4) {
				// warning every 4 sec
				// use NULL as the message sender here since it is the Terran Command persona
				message_send_builtin_to_player( MESSAGE_INSTRUCTOR_HIT, NULL, MESSAGE_PRIORITY_HIGH, MESSAGE_TIME_IMMEDIATE, 0, 0, -1, -1);
				pp->last_warning_message_time = Missiontime;
			}

		// not nice to hit your friends
		} else if (pp->friendly_damage > FRIENDLY_DAMAGE_THRESHOLD * (1.0f + (float) (NUM_SKILL_LEVELS + 1 - Game_skill_level)/3.0f)) {
			process_friendly_hit_message( MESSAGE_HAMMER_SWINE, objp_hit );
			mission_goal_fail_all();
			ai_abort_rearm_request( Player_obj );

			Player_ship->team = TEAM_TRAITOR;

		} else if ((damage > frand()) && (Missiontime - pp->last_warning_message_time > F1_0*4) && (pp->friendly_damage > FRIENDLY_DAMAGE_THRESHOLD)) {
			// no closer than 4 sec intervals
			//	Note: (damage > frand()) added on 12/9/97 by MK.  Since damage is now scaled down for big ships, we could get too
			//	many warnings.  Kind of tedious.  frand() returns a value in 0..1, so this won't affect legit hits.
			process_friendly_hit_message( MESSAGE_OOPS, objp_hit );
			pp->last_warning_message_time = Missiontime;
		}
	}
}

//	Maybe make ship with ai_info *aip attack hitter_objnum as a dynamic goal
void maybe_set_dynamic_chase(ai_info *aip, int hitter_objnum)
{
	Assert(Ship_info[Ships[aip->shipnum].ship_info_index].flags & (SIF_FIGHTER | SIF_BOMBER));

	// limit the number of ships attacking hitter_objnum (for now, only if hitter_objnum is player)
	if ( ai_maybe_limit_attackers(hitter_objnum) == 1 ) {
		return;
	}

	// only set as target if can be targeted.
	if (awacs_get_level(&Objects[hitter_objnum], &Ships[aip->shipnum], 1) < 1) {
		return;
	}

	if (aip->target_objnum != hitter_objnum)
		aip->aspect_locked_time = 0.0f;
	set_target_objnum(aip, hitter_objnum);
	aip->resume_goal_time = Missiontime + i2f(20);	//	Only chase up to 20 seconds.
	aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;

	set_targeted_subsys(aip, NULL, -1);		//	Say not attacking any particular subsystem.

	aip->previous_submode = aip->mode;
	aip->mode = AIM_CHASE;
	aip->submode = SM_ATTACK;
}


//	Return true if *objp has armed an aspect seeking bomb.
//	This function written so a ship with an important bomb to fire will willingly take hits in the face to fire its bomb.
int firing_aspect_seeking_bomb(object *objp)
{
	ship	*shipp;
	int	bank_index;
	ship_weapon	*swp;

	shipp = &Ships[objp->instance];

	swp = &shipp->weapons;

	bank_index = swp->current_secondary_bank;

	if (bank_index != -1)
		if (swp->secondary_bank_ammo[bank_index] > 0) {
			if (Weapon_info[swp->secondary_bank_weapons[bank_index]].wi_flags & WIF_BOMB) {
				if (Weapon_info[swp->secondary_bank_weapons[bank_index]].wi_flags & WIF_HOMING_ASPECT) {
					return 1;
				}
			}
		}

	return 0;
}

//	*objp collided with big ship *big_objp at global point *collide_pos
//	Make it fly away from the collision point.
// collision_normal is NULL, when a collision is imminent and we just want to bug out.
void big_ship_collide_recover_start(object *objp, object *big_objp, vector *collide_pos, vector *collision_normal)
{
	ai_info	*aip;

	Assert(objp->type == OBJ_SHIP);

	aip = &Ai_info[Ships[objp->instance].ai_index];

	if (!timestamp_elapsed(aip->big_recover_timestamp) && (aip->ai_flags & AIF_BIG_SHIP_COLLIDE_RECOVER_1))
		return;

	//nprintf(("AI", "Ship %s beginning to avoid ship %s at time %7.3f", Ships[objp->instance].ship_name, Ships[big_objp->instance].ship_name, f2fl(Missiontime)));
	if (collision_normal) {
		aip->big_recover_timestamp = timestamp(2000);
		aip->big_collision_normal = *collision_normal;
	//	nprintf(("AI", " normal\n"));
	} else {
		aip->big_recover_timestamp = timestamp(500);
	//	nprintf(("AI", " no normal\n"));
	}


	aip->ai_flags &= ~AIF_BIG_SHIP_COLLIDE_RECOVER_2;
	aip->ai_flags |= AIF_BIG_SHIP_COLLIDE_RECOVER_1;

//	vector	out_vec;
//	vm_vec_normalized_dir(&out_vec, &objp->pos, collide_pos);

	// big_recover_pos_1 is 100 m out along normal
	vector direction;
	if (collision_normal) {
		direction = *collision_normal;
	} else {
		vm_vec_copy_scale(&direction, &objp->orient.v.fvec, -1.0f);
	}
	vm_vec_scale_add(&aip->big_recover_pos_1, &objp->pos, &direction, 100.0f);

	// go out 200 m from box closest box point
	get_world_closest_box_point_with_delta(&aip->big_recover_pos_2, big_objp, &aip->big_recover_pos_1, NULL, 300.0f);

	accelerate_ship(aip, 0.0f);
/*
	if (vm_vec_dot(collision_normal, &objp->orient.v.fvec) > 0.5f) {
//		vm_vec_scale_add(&aip->big_recover_pos_1, &objp->pos, &out_vec, big_objp->radius/2.0f);
//		vm_vec_scale_add(&aip->big_recover_pos_2, &aip->big_recover_pos_1, &objp->orient.v.uvec, big_objp->radius/2.0f);
//		vm_vec_scale_add(&aip->big_recover_pos_2, &objp->pos, &out_vec, big_objp->radius*2.0f);
		accelerate_ship(aip, 2.0f);
	} else {
//		vm_vec_scale_add(&aip->big_recover_pos_1, &objp->pos, &out_vec, big_objp->radius/2.0f);
//		vm_vec_scale_add(&aip->big_recover_pos_2, &aip->big_recover_pos_1, &objp->orient.v.uvec, big_objp->radius/2.0f);
		accelerate_ship(aip, 0.0f);
	} */
}

float max_lethality = 0.0f;

void ai_update_lethality(object *ship_obj, object *other_obj, float damage)
{
	Assert(ship_obj->type == OBJ_SHIP);
	Assert(other_obj->type == OBJ_WEAPON || other_obj->type == OBJ_SHOCKWAVE);
	int dont_count = FALSE;

	int parent = other_obj->parent;
	if (Objects[parent].type == OBJ_SHIP) {
		if (Objects[parent].signature == other_obj->parent_sig) {

			// check damage done to enemy team
			if (Ships[ship_obj->instance].team != Ships[Objects[parent].instance].team) {

				// other is weapon
				if (other_obj->type == OBJ_WEAPON) {
					weapon *wp = &Weapons[other_obj->instance];
					weapon_info *wif = &Weapon_info[wp->weapon_info_index];

					// if parent is BIG|HUGE, don't count beam
					if (Ship_info[Ships[Objects[parent].instance].ship_info_index].flags & (SIF_BIG_SHIP|SIF_HUGE_SHIP)) {
						if (wif->wi_flags & WIF_BEAM) {
							dont_count = TRUE;
						}
					}
				}

				if (!dont_count) {
					float lethality = 0.025f * damage;	// 2 cyclops (@2000) put you at 100 lethality

					// increase lethality weapon's parent ship
					ai_info *aip = &Ai_info[Ships[Objects[parent].instance].ai_index];
					aip->lethality += lethality;
					aip->lethality = min(110.0f, aip->lethality);
					// if you hit, don;t be less than 0
					aip->lethality = max(0.0f, aip->lethality);

//					if (aip->lethality > max_lethality) {
//						max_lethality = aip->lethality;
//						mprintf(("new lethalilty high: %.1f\n", max_lethality));
//					}

					// if parent is player, show his lethality
//					if (Objects[parent].flags & OF_PLAYER_SHIP) {
//						mprintf(("Player lethality: %.1f\n", aip->lethality));
//					}
				}
			}
		}
	}
}


//	Object *objp_ship was hit by either weapon *objp_weapon or collided into by ship hit_objp at point *hitpos.
void ai_ship_hit(object *objp_ship, object *hit_objp, vector *hitpos, int shield_quadrant, vector *hit_normal)
{
	int		hitter_objnum = -2;
	object	*objp_hitter = NULL;
	ship		*shipp;
	ai_info	*aip, *hitter_aip;

	shipp = &Ships[objp_ship->instance];
	aip = &Ai_info[shipp->ai_index];

	if (objp_ship->flags & OF_PLAYER_SHIP)
		return;

	if ((aip->mode == AIM_WARP_OUT) || (aip->mode == AIM_PLAY_DEAD))
		return;

	if (hit_objp->type == OBJ_SHIP) {
		//	If the object that this ship collided with is a big ship
		if (Ship_info[Ships[hit_objp->instance].ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP)) {
			//	And the current object is _not_ a big ship
			if (!(Ship_info[Ships[objp_ship->instance].ship_info_index].flags & (SIF_BIG_SHIP | SIF_HUGE_SHIP))) {
				//	Recover from hitting a big ship.  Note, if two big ships collide, they just pound away at each other.  Oh well.  Recovery looks dumb and it's very late.
				big_ship_collide_recover_start(objp_ship, hit_objp, hitpos, hit_normal);
			}
		}
	}

	if (hit_objp->type == OBJ_WEAPON) {
		//	Make sure the object that fired this weapon is still alive.  If not, abort.
		// Assert(hit_objp->parent >= 0);
		if(hit_objp->parent < 0){
			return;
		}
		if ( hit_objp->parent_sig != Objects[hit_objp->parent].signature ){
			return;
		}

		//	Hit by a protected ship, don't attack it.
		if (Objects[hit_objp->parent].flags & OF_PROTECTED) {
			if ((Ship_info[shipp->ship_info_index].flags & (SIF_FIGHTER | SIF_BOMBER)) && (aip->target_objnum == -1)) {
				if (aip->mode == AIM_CHASE) {
					if (aip->submode != SM_EVADE_WEAPON) {
						aip->mode = AIM_CHASE;
						aip->submode = SM_EVADE_WEAPON;
						aip->submode_start_time = Missiontime;
					}
				} else if (aip->mode != AIM_EVADE_WEAPON) {
					aip->active_goal = AI_ACTIVE_GOAL_DYNAMIC;
					aip->previous_mode = aip->mode;
					aip->previous_submode = aip->submode;
					aip->mode = AIM_EVADE_WEAPON;
					aip->submode = -1;
					aip->submode_start_time = Missiontime;
					aip->mode_time = timestamp(MAX_EVADE_TIME);	//	Evade for up to five seconds.
				}

			}
			return;
		}

		hitter_objnum = hit_objp->parent;
		Assert((hitter_objnum >= 0) && (hitter_objnum < MAX_OBJECTS));
		objp_hitter = &Objects[hitter_objnum];
		maybe_process_friendly_hit(objp_hitter, objp_ship, hit_objp);		//	Deal with player's friendly fire.

		if ( (shipp->team & TEAM_FRIENDLY) && !(Game_mode & GM_MULTIPLAYER) ) {
			ship_maybe_ask_for_help(shipp);
		}
	} else if (hit_objp->type == OBJ_SHIP) {
		if (shipp->team == Ships[hit_objp->instance].team)		//	Don't have AI react to collisions between teammates.
			return;
		objp_hitter = hit_objp;
		hitter_objnum = hit_objp-Objects;
	} else {
		Int3();	//	Hmm, what kind of object hit this if not weapon or ship?  Get MikeK.
		return;
	}

	//	Collided into a protected ship, don't attack it.
	if (hit_objp->flags & OF_PROTECTED)
		return;

	Assert(objp_hitter != NULL);
	hitter_aip = &Ai_info[Ships[objp_hitter->instance].ai_index];
	hitter_aip->last_hit_target_time = Missiontime;
	
	// store the object signature of objp_ship into ai_info, since we want to track the last ship hit by 'hitter_objnum'
	hitter_aip->last_objsig_hit = objp_ship->signature; 

	aip->last_hit_time = Missiontime;

	if (aip->ai_flags & (AIF_NO_DYNAMIC | AIF_KAMIKAZE))	//	If not allowed to pursue dynamic objectives, don't evade.  Dumb?  Maybe change. -- MK, 3/15/98
		return;

	//	If this ship is awaiting repair, abort!
	if (aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED)) {
		ship_info	*sip = &Ship_info[shipp->ship_info_index];

		if (objp_ship->hull_strength/sip->initial_hull_strength < 0.3f) {
			//	No, only abort if hull below a certain level.
			aip->next_rearm_request_timestamp = timestamp(NEXT_REARM_TIMESTAMP/2);	//	Might request again after 15 seconds.
			if ( !(objp_ship->flags & OF_PLAYER_SHIP) )						// mwa -- don't abort rearm for a player
				ai_abort_rearm_request(objp_ship);
		}
	}

	//	If firing a bomb, ignore enemy fire so we can gain lock drop the bomb.
	//	Only ignore fire if aspect_locked_time > 0.5f, as this means we're in range.
	if (firing_aspect_seeking_bomb(objp_ship)) {
		if ((aip->ai_flags & AIF_SEEK_LOCK) && (aip->aspect_locked_time > 0.1f))
			return;
	}

	//	If in AIM_STRAFE mode and got hit by target, maybe attack turret if appropriate
	if (aip->mode == AIM_STRAFE) {
		Assert(hitter_objnum != -2);
		if (aip->target_objnum == hitter_objnum) {
			if ( hit_objp->type == OBJ_WEAPON ) {
				ai_big_strafe_maybe_attack_turret(objp_ship, hit_objp);
			}
			return;
		}
		else {
				// AL 11-10-97:
			;	// do nothing here, we'll attack this hitter if it is a fighter or bomber (this is handled
				// in code later in this function
		}
	}

	if (objp_ship == Player_obj)
		return;		//	We don't do AI for the player.

	maybe_update_guard_object(objp_ship, objp_hitter);

	//	Big ships don't go any further.
	if (!(Ship_info[shipp->ship_info_index].flags & SIF_SMALL_SHIP))
		return;

	//	If the hitter object is the ignore object, don't attack it.
	ship_info	*sip = &Ship_info[shipp->ship_info_index];
	if ((is_ignore_object(aip, objp_hitter-Objects)) && (sip->flags & (SIF_BOMBER | SIF_FIGHTER))) {
		if (aip->mode == AIM_NONE) {
			aip->mode = AIM_CHASE;	//	This will cause the ship to move, if not attack.
			aip->submode = SM_EVADE;
		}
		return;
	}

	//	Maybe abort based on mode.
	switch (aip->mode) {
	case AIM_CHASE:
		if (aip->submode == SM_ATTACK_FOREVER)
			return;

		if ( hit_objp->type == OBJ_WEAPON ) {
			if ( ai_big_maybe_enter_strafe_mode(objp_ship, OBJ_INDEX(hit_objp), 1) )
				return;
		}

	case AIM_GUARD:
		//	If in guard mode and far away from guard object, don't pursue guy that hit me.
			if ((aip->guard_objnum != -1) && (aip->guard_signature == Objects[aip->guard_objnum].signature)) {
				if (vm_vec_dist_quick(&objp_ship->pos, &Objects[aip->guard_objnum].pos) > 500.0f) {
					return;
				}
			}
	case AIM_STILL:
	case AIM_STAY_NEAR:
		// Note: Dealt with above, at very top.  case AIM_PLAY_DEAD:
	case AIM_STRAFE:
		break;
	case AIM_EVADE_WEAPON:
	case AIM_EVADE:
	case AIM_GET_BEHIND:
	case AIM_AVOID:
	case AIM_DOCK:
	case AIM_BIGSHIP:
	case AIM_PATH:
	case AIM_NONE:
	case AIM_BAY_DEPART:
	case AIM_SENTRYGUN:
		return;
	case AIM_BAY_EMERGE:
		// If just leaving the docking bay, don't react to enemy fire... just keep flying away from docking bay
		if ( (Missiontime - aip->submode_start_time) < 5*F1_0 ) {
			return;
		}
		break;
	case AIM_WAYPOINTS:
		if (sip->flags & (SIF_FIGHTER | SIF_BOMBER))
			break;
		else
			return;
		break;
	case AIM_SAFETY:
		if ((aip->submode != AISS_1) || (Missiontime - aip->submode_start_time > i2f(1))) {
			aip->submode = AISS_1;
			aip->submode_start_time = Missiontime;
		}
		return;
		break;
	case AIM_WARP_OUT:
		return;
		break;
	default:
		Int3();	//	Bogus mode!
	}

	if (timestamp_elapsed(aip->ok_to_target_timestamp))
		aip->ai_flags &= ~AIF_FORMATION;			//	If flying in formation, bug out!

	aip->hitter_objnum = hitter_objnum;
	aip->hitter_signature = Objects[hitter_objnum].signature;

	//	If the hitter is not on the same team as the hittee, do some stuff.
	if (shipp->team != Ships[objp_hitter->instance].team) {
		//nprintf(("AI", "Object %i attacking %i, who just hit him!\n", objp_ship-Objects, hitter_objnum));

		if ((hitter_objnum != aip->target_objnum) && (sip->flags & (SIF_FIGHTER | SIF_BOMBER))) {
			maybe_set_dynamic_chase(aip, hitter_objnum);
			maybe_afterburner_after_ship_hit(objp_ship, aip, &Objects[hitter_objnum]);
		} else {
			if ((aip->mode == AIM_CHASE) && ((objp_ship->hull_strength/sip->initial_hull_strength > 0.9f) || (get_shield_strength(objp_ship)/sip->shields > 0.8f))) {
				switch (aip->submode) {
				case SM_ATTACK:
				case SM_SUPER_ATTACK:
				case SM_GET_AWAY:
					break;
				default:
					if (sip->flags & (SIF_FIGHTER | SIF_BOMBER)) {
						maybe_set_dynamic_chase(aip, hitter_objnum);
					}
					maybe_afterburner_after_ship_hit(objp_ship, aip, &Objects[hitter_objnum]);
					break;
				}
			} else if (aip->mode == AIM_CHASE) {
				switch (aip->submode) {
				case SM_ATTACK:
					aip->submode = SM_EVADE;
					aip->submode_start_time = Missiontime;
					break;
				case SM_SUPER_ATTACK:
					if (Missiontime - aip->submode_start_time > i2f(1)) {
						aip->submode = SM_EVADE;
						aip->submode_start_time = Missiontime;
					}
					break;
				case SM_EVADE_BRAKE:
					break;
				case SM_EVADE_SQUIGGLE:
					aip->submode = SM_EVADE;
					aip->submode_start_time = Missiontime;
					break;
				default:
					if (sip->flags & (SIF_BOMBER | SIF_FIGHTER)) {
						maybe_set_dynamic_chase(aip, hitter_objnum);
						maybe_afterburner_after_ship_hit(objp_ship, aip, &Objects[hitter_objnum]);
					}

					break;
				}
			} else {
				// AL 3-15-98: Prevent escape pods from entering chase mode
				if ( (sip->flags & (SIF_BOMBER | SIF_FIGHTER)) ) {
					maybe_set_dynamic_chase(aip, hitter_objnum);
				}
				maybe_afterburner_after_ship_hit(objp_ship, aip, &Objects[hitter_objnum]);
			}
		}
	}
}

//	Ship shipnum has been destroyed.
//	Cleanup.
// the parameter 'method' is used to tell is this ship was destroyed or it departed normally.
// This function will get called in either case, and there are things that should be done if
// the ship actually gets destroyed which shouldn't get done if it departed.
void ai_ship_destroy(int shipnum, int method)
{
	int		objnum;
	object	*other_objp;
	ship		*shipp;
	ship_obj	*so;
	ai_info	*dead_aip;

	Assert((shipnum >= 0) && (shipnum < MAX_SHIPS));
	objnum = Ships[shipnum].objnum;
	dead_aip = &Ai_info[Ships[shipnum].ai_index];

	// if I was getting repaired, or awaiting repair, then cleanup the repair mode.  When awaiting repair, the dock_objnum
	// is -1.  When the support ship is on the way, the dock_objnum >= 0 (points to support ship).
	if ( dead_aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED) ) {
		if ( dead_aip->dock_objnum >= 0 )
			ai_do_objects_repairing_stuff( &Objects[objnum], &Objects[dead_aip->dock_objnum], REPAIR_INFO_END);
		else
			ai_do_objects_repairing_stuff( &Objects[objnum], NULL, REPAIR_INFO_END );
	}

	//	For all objects that had this ship as a target, wipe it out, forcing find of a new enemy.
	for ( so = GET_FIRST(&Ship_obj_list); so != END_OF_LIST(&Ship_obj_list); so = GET_NEXT(so) ) {
		other_objp = &Objects[so->objnum];
		Assert(other_objp->instance != -1);

		shipp = &Ships[other_objp->instance];
		Assert(shipp->ai_index != -1);

		ai_info	*aip = &Ai_info[shipp->ai_index];

		// MWA 2/11/98
		// code commented out below is taken care of in ai_cleanup_dock_mode when gets called when the
		// support ship starts it's death roll.

		//	If the destroyed ship was on its way to repair the current ship
		if (aip->dock_objnum == objnum) {

			// clean up the flags for any kind of docking mode.  If aip was part of a goal of dock/undock
			// then it will get cleaned up by the goal code.
			ai_do_objects_undocked_stuff( other_objp, NULL );

			if ( aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED) ) {
				int abort_reason;
				if ( method == SEF_DEPARTED ) {
					abort_reason = REPAIR_INFO_ABORT;
				} else {
					abort_reason = REPAIR_INFO_KILLED;
				}
				ai_do_objects_repairing_stuff( other_objp, NULL, abort_reason );
			}
		}

		if (aip->target_objnum == objnum) {
			set_target_objnum(aip, -1);
			//	If this ship had a dynamic goal of chasing the dead ship, clear the dynamic goal.
			if (aip->resume_goal_time != -1)
				aip->active_goal = AI_GOAL_NONE;
		}

		if (aip->goal_objnum == objnum) {
			aip->goal_objnum = -1;
			aip->goal_signature = -1;
		}

		if (aip->guard_objnum == objnum) {
			aip->guard_objnum = -1;
			aip->guard_signature = -1;
		}

		if ((aip->guard_wingnum != -1) && (aip->guard_wingnum == Ai_info[Ships[Objects[objnum].instance].ai_index].wing)) {
			if (aip->guard_wingnum != aip->wing)
				ai_set_guard_wing(other_objp, aip->guard_wingnum);
		}

		if (aip->hitter_objnum == objnum)
			aip->hitter_objnum = -1;

	}

}

/*
//	Interface function to goals code.
//	Make object *objp fly to point *vp and warp out.
void ai_warp_out(object *objp, vector *vp)
{
	ai_info	*aip;

	aip = &Ai_info[Ships[objp->instance].ai_index];

	if (aip->mode != AIM_WARP_OUT) {
		ai_set_mode_warp_out(objp, aip);
	}
	float	dist;
	float	dot;
	vector	v2v;
	ai_info	*aip;

	dist = vm_vec_normalized_dir(&v2v, vp, &objp->pos);

	if (dist < objp->radius + 5.0f) {

		// Start the warp out effect 
		shipfx_warpout_start(objp);

	} else {
		dot = vm_vec_dot(&objp->orient.v.fvec, &v2v);

		aip = &Ai_info[Ships[objp->instance].ai_index];

		if (dist > 500.0f)
			accelerate_ship(aip, 1.0f);
		else
			accelerate_ship(aip, (3*dot + 1.0f)/4.0f);

		turn_towards_point(objp, vp, NULL, 0.0f);
	}
}
*/


//	Do stuff at start of deathroll.
void ai_deathroll_start(object *ship_obj)
{
	ai_info	*aip;
	ship		*shipp, *other_ship;

	shipp = &Ships[ship_obj->instance];
	aip = &Ai_info[shipp->ai_index];

	// mark object we are docked with so we can do damage and separate during deathroll
	// keep dock_objnum_when_dead from being changed if already set (only allow to be set when -1)
	if (Ships[ship_obj->instance].dock_objnum_when_dead == -1) {
		Ships[ship_obj->instance].dock_objnum_when_dead = aip->dock_objnum;
		// set other_ship dock_objnum_when_dead, if other_ship exits.
		if (Ships[ship_obj->instance].dock_objnum_when_dead != -1) {
			other_ship = &Ships[Objects[aip->dock_objnum].instance];
			other_ship->dock_objnum_when_dead = shipp->objnum;
		}
	}

	ai_cleanup_dock_mode(aip, shipp);

	aip->mode = AIM_NONE;
}

//	Object *requester_objp tells rearm ship to abort rearm.
//	Returns true if it succeeded, else false.
//	To succeed means you were previously rearming.
int ai_abort_rearm_request(object *requester_objp)
{
	ship		*requester_shipp;
	ai_info	*requester_aip;

	Assert(requester_objp->type == OBJ_SHIP);
	if(requester_objp->type != OBJ_SHIP){
		return 0;
	}
	Assert((requester_objp->instance >= 0) && (requester_objp->instance < MAX_SHIPS));	
	if((requester_objp->instance < 0) || (requester_objp->instance >= MAX_SHIPS)){
		return 0;
	}
	requester_shipp = &Ships[requester_objp->instance];
	Assert((requester_shipp->ai_index >= 0) && (requester_shipp->ai_index < MAX_AI_INFO));		
	if((requester_shipp->ai_index < 0) || (requester_shipp->ai_index >= MAX_AI_INFO)){
		return 0;
	}	
	requester_aip = &Ai_info[requester_shipp->ai_index];
	
	if (requester_aip->ai_flags & (AIF_AWAITING_REPAIR | AIF_BEING_REPAIRED)){

		// dock_objnum is always valid once a rearm repair has been requested.  It points to the
		// ship that is coming to repair me.
		if (requester_aip->dock_objnum != -1) {
			object	*repair_objp;
			ai_info	*repair_aip;

			repair_objp = &Objects[requester_aip->dock_objnum];
			repair_aip = &Ai_info[Ships[repair_objp->instance].ai_index];

			//	Make sure signatures match.  This prevents nasty bugs in which an object
			//	that was repairing another is destroyed and is replaced by another ship
			//	before this code comes around.
			if (repair_objp->signature == requester_aip->dock_signature) {

				Assert( repair_objp->type == OBJ_SHIP );

				// if support ship is in the process of undocking, don't do anything.
				if ( repair_aip->submode < AIS_UNDOCK_0 ) {
					ai_do_objects_repairing_stuff( requester_objp, repair_objp, REPAIR_INFO_ABORT );

					if ( repair_aip->submode == AIS_DOCK_4 )
						repair_aip->submode = AIS_UNDOCK_0;
					else
						repair_aip->submode = AIS_UNDOCK_3;

					repair_aip->submode_start_time = Missiontime;
				} else {
					nprintf(("AI", "Not aborting rearm since already undocking\n"));
				}
			}
		} else {
			// setting these flags is the safe things to do.  There may not be a corresponding repair
			// ship for this guys since a repair ship may be currently repairing someone else.
			ai_do_objects_repairing_stuff( requester_objp, NULL, REPAIR_INFO_ABORT );

			// try and remove this guy from an arriving support ship.
			mission_remove_scheduled_repair(requester_objp);
		}

		return 1;
	} else if ( requester_aip->ai_flags & AIF_REPAIRING ) {
		// a support ship can request to abort when he is told to do something else (like warp out).
		// see if this support ships goal_objnum is valid.  If so, then issue this ai_abort comment
		// for the ship that he is enroute to repair
		if ( requester_aip->goal_objnum != -1 ) {
			int val;

			val = ai_abort_rearm_request( &Objects[requester_aip->goal_objnum] );
			return val;
		}
	}

	return 0;
}

// function which gets called from ai-issue_rearm_request and from code in missionparse.cpp
// to actually issue the rearm goal (support_obj to rearm requester_obj);
void ai_add_rearm_goal( object *requester_objp, object *support_objp )
{
	ship *support_shipp, *requester_shipp;
	ai_info *support_aip, *requester_aip;

	support_shipp = &Ships[support_objp->instance];
	requester_shipp = &Ships[requester_objp->instance];
	requester_aip = &Ai_info[requester_shipp->ai_index];

	Assert( support_shipp->ai_index != -1 );
	support_aip = &Ai_info[support_shipp->ai_index];

	// if the requester is a player object, issue the order as the squadmate messaging code does.  Doing so
	// ensures that the player get a higher priority!
	requester_aip->ai_flags |= AIF_AWAITING_REPAIR;	//	Tell that I'm awaiting repair.
	if ( requester_objp->flags & OF_PLAYER_SHIP )
		ai_add_ship_goal_player( AIG_TYPE_PLAYER_SHIP, AI_GOAL_REARM_REPAIR, -1, requester_shipp->ship_name, support_aip );
	else
		ai_add_goal_ship_internal( support_aip, AI_GOAL_REARM_REPAIR, requester_shipp->ship_name, -1, -1 );

}

//	Object *requester_objp requests rearming.
//	Returns objnum of ship coming to repair requester on success
//	Success means you found someone to rearm you and you weren't previously rearming.
int ai_issue_rearm_request(object *requester_objp)
{
	object	*objp;
	ship		*requester_shipp;
	ai_info	*requester_aip;

	Assert(requester_objp->type == OBJ_SHIP);
	Assert((requester_objp->instance >= 0) && (requester_objp->instance < MAX_SHIPS));
	requester_shipp = &Ships[requester_objp->instance];
	Assert((requester_shipp->ai_index >= 0) && (requester_shipp->ai_index < MAX_AI_INFO));
	requester_aip = &Ai_info[requester_shipp->ai_index];
	
	//	Make sure not already awaiting repair.
	if (requester_aip->ai_flags & AIF_AWAITING_REPAIR) {
		nprintf(("AI", "Ship %s already awaiting rearm by ship %s.\n", requester_shipp->ship_name, &Ships[Objects[requester_aip->dock_objnum].instance].ship_name));	
		return -1;
	}

	if ( !is_support_allowed(requester_objp) )
		return -1;

	//nprintf(("AI", "Ship %s requesting rearming.\n", requester_shipp->ship_name));
	requester_aip->next_rearm_request_timestamp = timestamp(NEXT_REARM_TIMESTAMP);	//	Might request again after this much time.

	// call ship_find_repair_ship to get a support ship.  If none is found, then we will warp one in.  This
	// function will return the next available ship which can repair requester
	objp = ship_find_repair_ship( requester_objp );
	ai_do_objects_repairing_stuff( requester_objp, objp, REPAIR_INFO_QUEUE );
	if ( objp ) {

		// MWA 5/14/98 -- moved next item into the ai_do_objects_repairing_stuff function so that clients
		// would properly update their hud support view
		//ai_add_rearm_goal( requester_objp, objp );
		return OBJ_INDEX(objp);

	} else {
		// call to warp in repair ship!!!!  for now, warp in any number of ships needed.  Should cap it to
		// some reasonable max (or let support ships warp out).  We should assume here that ship_find_repair_ship()
		// would have returned a valid object if there are too many support ships already in the mission
		mission_warp_in_support_ship( requester_objp );

		return -1;
	}

}

// make objp rearm and repair goal_objp
void ai_rearm_repair( object *objp, object  *goal_objp, int priority, int docker_index, int dockee_index )
{
	ai_info *aip, *goal_aip;

	aip = &Ai_info[Ships[objp->instance].ai_index];
	aip->goal_objnum = goal_objp-Objects;

	// nprintf(("AI", "Ship %s preparing to rearm ship %s.\n", shipp->ship_name, requester_shipp->ship_name));

	ai_dock_with_object(objp, goal_objp, priority, AIDO_DOCK, docker_index, dockee_index);
	aip->ai_flags |= AIF_REPAIRING;						//	Tell that repair guy is busy trying to repair someone.

	goal_aip = &Ai_info[Ships[goal_objp->instance].ai_index];
	goal_aip->dock_objnum = objp-Objects;		//	Tell which object is coming to repair.
	goal_aip->dock_signature = objp->signature;

	ai_do_objects_repairing_stuff( goal_objp, objp, REPAIR_INFO_ONWAY );

	goal_aip->abort_rearm_timestamp = timestamp(NEXT_REARM_TIMESTAMP*3/2);
}

// Given a dockee object and the index of the dockbay for that object (ie the dockbay index
// into polymodel->dockbays[] for the model associated with the object), return the index
// of a path_num associated with than dockbay (this is an index into polymodel->paths[])
int ai_return_path_num_from_dockbay(object *dockee_objp, int dockbay_index)
{
	if ( dockbay_index < 0 || dockee_objp == NULL ) {
		Int3();		// should never happen
		return -1;
	}

	if ( dockee_objp->type == OBJ_SHIP ) {
		int			path_num;
		polymodel	*pm;

		pm = model_get( Ships[dockee_objp->instance].modelnum );

		// sanity checks
		Assert(pm->n_docks > dockbay_index);
		Assert(pm->docking_bays[dockbay_index].num_spline_paths > 0);
		Assert(pm->docking_bays[dockbay_index].splines != NULL);
		if(pm->n_docks <= dockbay_index){
			return -1;
		}
		if(pm->docking_bays[dockbay_index].num_spline_paths <= 0){
			return -1;
		}
		if(pm->docking_bays[dockbay_index].splines == NULL){
			return -1;
		}

		// We only need to return one path for the dockbay, so return the first
		path_num = pm->docking_bays[dockbay_index].splines[0];
		return path_num;
	} else {
		return -1;
	}
}

//	Actually go ahead and fire the synaptics.
void cheat_fire_synaptic(object *objp, ship *shipp, ai_info *aip)
{
	ship_weapon	*swp;
	swp = &shipp->weapons;
	int	current_bank = swp->current_secondary_bank;

	ai_select_secondary_weapon(objp, swp, WIF_SPAWN, 0);
	if (timestamp_elapsed(swp->next_secondary_fire_stamp[current_bank])) {
		if (ship_fire_secondary(objp)) {
			nprintf(("AI", "ship %s cheat fired synaptic!\n", shipp->ship_name));
			swp->next_secondary_fire_stamp[current_bank] = timestamp(2500);
		}
	}
}

//	For the subspace mission (sm3-09a)
//		for delta wing
//			if they're sufficiently far into the mission
//				if they're near one or more enemies
//					every so often
//						fire a synaptic if they have one.
void maybe_cheat_fire_synaptic(object *objp, ai_info *aip)
{
	//	Only do in subspace missions.
	if ( The_mission.flags & MISSION_FLAG_SUBSPACE )	{
		ship	*shipp;
		int	num, time;

		shipp = &Ships[objp->instance];

		if (!(strnicmp(shipp->ship_name, NOX("delta"), 5))) {
			num = shipp->ship_name[6] - '1';

			if ((num >= 0) && (num <= 3)) {
				time = Missiontime >> 16;	//	Convert to seconds.

				time -= 2*60;	//	Subtract off two minutes.

				if (time > 0) {
					int modulus = 17 + num*3;

					if ((time % modulus) < 2) {
						int count = num_nearby_fighters(get_enemy_team_mask(OBJ_INDEX(objp)), &objp->pos, 1500.0f);

						if (count > 0) {
							cheat_fire_synaptic(objp, shipp, aip);
						}
					}
				}
			}
		}
	}

}

