/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Weapon/Weapons.cpp $
 * $Revision: 311 $
 * $Date: 2010-02-08 09:09:26 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Code to handle the weapon systems
 *
 * $Log$
 * Revision 1.13  2005/10/01 22:04:58  taylor
 * fix FS1 (de)briefing voices, the directory names are different in FS1
 * hard code the table values so that the fs1.vp file isn't needed
 * hard code a mission fix for sm2-08a since a have no idea how to fix it otherwise
 * generally cleanup some FS1 code
 * fix volume sliders in the options screen that never went all the way up
 *
 * Revision 1.12  2005/08/12 08:48:11  taylor
 * don't scale weapon damage for FS1, this way you can still kill a capship with lasers rather than bombs
 *
 * Revision 1.11  2004/09/20 01:31:45  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.10  2003/06/11 18:30:33  taylor
 * plug memory leaks
 *
 * Revision 1.9  2003/05/25 02:30:44  taylor
 * Freespace 1 support
 *
 * Revision 1.8  2002/06/21 03:04:12  relnev
 * nothing important
 *
 * Revision 1.7  2002/06/17 06:33:11  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.6  2002/06/09 04:41:30  relnev
 * added copyright header
 *
 * Revision 1.5  2002/06/05 08:05:29  relnev
 * stub/warning removal.
 *
 * reworked the sound code.
 *
 * Revision 1.4  2002/06/01 03:32:00  relnev
 * fix texture loading mistake.
 *
 * enable some d3d stuff for opengl also
 *
 * Revision 1.3  2002/05/28 08:52:03  relnev
 * implemented two assembly stubs.
 *
 * cleaned up a few warnings.
 *
 * added a little demo hackery to make it progress a little farther.
 *
 * Revision 1.2  2002/05/07 03:16:53  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 69    9/14/99 3:26a Dave
 * Fixed laser fogging problem in nebula (D3D)> Fixed multiplayer
 * respawn-too-early problem. Made a few crash points safe.
 * 
 * 68    9/14/99 1:32a Andsager
 * Better LOD for weapon explosions when behind.  Move point ahead to get
 * vertex and then find size.
 * 
 * 67    9/07/99 1:10p Mikek
 * Fix code I busted due to adding lifeleft to missiles.
 * 
 * 66    9/07/99 12:20a Andsager
 * LOD less agressive at lower hardware detail level
 * 
 * 65    9/06/99 7:21p Dave
 * Commented out bad lifeleft scaling code in weapon_set_tracking_info()
 * 
 * 64    9/06/99 3:23p Andsager
 * Make fireball and weapon expl ani LOD choice look at resolution of the
 * bitmap
 * 
 * 63    9/06/99 12:46a Andsager
 * Add weapon_explosion_ani LOD
 * 
 * 62    9/05/99 11:24p Mikek
 * Fixed problems caused by earlier checkin (that was rolled back).
 * Problem was wp->target_pos was not set for swarmers.
 * 
 * More tweaking of missile behavior.  Also add 20% to lifetime of a
 * locked missile.
 * 
 * [Rolled back -- MK] 63    9/05/99 2:23p Mikek
 * Make aspect seekers a little less likely to miss their target.
 * Mysterious why they do it so often.  Maybe fix for FS3...
 * 
 * [Rolled back -- MK] 62    9/04/99 12:09p Mikek
 * Limit number of spawned weapons that can home on player based on skill
 * level.  Works same as for non-spawned weapons.  Only do in single
 * player.
 * 
 * 61    8/27/99 1:34a Andsager
 * Modify damage by shockwaves for BIG|HUGE ships.  Modify shockwave damge
 * when weapon blows up.
 * 
 * 60    8/24/99 10:47a Jefff
 * tech room weapon anims.  added tech anim field to weapons.tbl
 * 
 * 59    8/16/99 11:58p Andsager
 * Disable collision on proximity for ships with SIF_DONT_COLLIDE_INVIS
 * hulls.
 * 
 * 58    8/10/99 5:30p Jefff
 * Added tech_title string to weapon_info.  Changed parser accordingly.
 * 
 * 57    8/05/99 2:06a Dave
 * Whee.
 * 
 * 56    8/02/99 5:16p Dave
 * Bumped up weapon title string length from 32 to 48
 * 
 * 55    7/29/99 5:41p Jefff
 * Sound hooks for cmeasure success
 * 
 * 54    7/24/99 1:54p Dave
 * Hud text flash gauge. Reworked dead popup to use 4 buttons in red-alert
 * missions.
 * 
 * 53    7/19/99 7:20p Dave
 * Beam tooling. Specialized player-killed-self messages. Fixed d3d nebula
 * pre-rendering.
 * 
 * 52    7/18/99 12:32p Dave
 * Randomly oriented shockwaves.
 * 
 * 51    7/16/99 1:50p Dave
 * 8 bit aabitmaps. yay.
 * 
 * 50    7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 49    7/08/99 10:53a Dave
 * New multiplayer interpolation scheme. Not 100% done yet, but still
 * better than the old way.
 * 
 * 48    7/02/99 4:31p Dave
 * Much more sophisticated lightning support.
 * 
 * 47    7/01/99 5:57p Johnson
 * Oops. Fixed big ship damage.
 * 
 * 46    7/01/99 4:23p Dave
 * Full support for multiple linked ambient engine sounds. Added "big
 * damage" flag.
 * 
 * 45    6/30/99 5:53p Dave
 * Put in new anti-camper code.
 * 
 * 44    6/22/99 3:24p Danw
 * Fixed incorrect weapon hit sound culling.
 * 
 * 43    6/21/99 7:25p Dave
 * netplayer pain packet. Added type E unmoving beams.
 * 
 * 42    6/14/99 10:45a Dave
 * Made beam weapons specify accuracy by skill level in the weapons.tbl
 * 
 * 41    6/11/99 11:13a Dave
 * last minute changes before press tour build.
 * 
 * 40    6/04/99 2:16p Dave
 * Put in shrink effect for beam weapons.
 * 
 * 39    6/01/99 8:35p Dave
 * Finished lockarm weapons. Added proper supercap weapons/damage. Added
 * awacs-set-radius sexpression.
 * 
 * 38    6/01/99 3:52p Dave
 * View footage screen. Fixed xstrings to not display the & symbol. Popup,
 * dead popup, pxo find player popup, pxo private room popup.
 * 
 * 37    5/27/99 6:17p Dave
 * Added in laser glows.
 * 
 * 36    5/20/99 7:00p Dave
 * Added alternate type names for ships. Changed swarm missile table
 * entries.
 * 
 * 35    5/08/99 8:25p Dave
 * Upped object pairs. First run of nebula lightning.
 * 
 * 34    4/28/99 11:13p Dave
 * Temporary checkin of artillery code.
 * 
 * 33    4/28/99 3:11p Andsager
 * Stagger turret weapon fire times.  Make turrets smarter when target is
 * protected or beam protected.  Add weaopn range to weapon info struct.
 * 
 * 32    4/22/99 11:06p Dave
 * Final pass at beam weapons. Solidified a lot of stuff. All that remains
 * now is to tweak and fix bugs as they come up. No new beam weapon
 * features.
 * 
 * 31    4/19/99 11:01p Dave
 * More sophisticated targeting laser support. Temporary checkin.
 * 
 * 30    4/16/99 5:54p Dave
 * Support for on/off style "stream" weapons. Real early support for
 * target-painting lasers.
 * 
 * 29    4/07/99 6:22p Dave
 * Fred and Freespace support for multiple background bitmaps and suns.
 * Fixed link errors on all subprojects. Moved encrypt_init() to
 * cfile_init() and lcl_init(), since its safe to call twice.
 * 
 * 28    4/02/99 1:35p Dave
 * Removed weapon hit packet. No good for causing pain.
 * 
 * 27    4/02/99 9:55a Dave
 * Added a few more options in the weapons.tbl for beam weapons. Attempt
 * at putting "pain" packets into multiplayer.
 * 
 * 26    3/31/99 9:26p Dave
 * Don't load beam textures when in Fred.
 * 
 * 25    3/31/99 8:24p Dave
 * Beefed up all kinds of stuff, incluging beam weapons, nebula effects
 * and background nebulae. Added per-ship non-dimming pixel colors.
 * 
 * 24    3/23/99 2:29p Andsager
 * Fix shockwaves for kamikazi and Fred defined.  Collect together
 * shockwave_create_info struct.
 * 
 * 23    3/23/99 11:03a Dave
 * Added a few new fields and fixed parsing code for new weapon stuff.
 * 
 * 22    3/19/99 9:52a Dave
 * Checkin to repair massive source safe crash. Also added support for
 * pof-style nebulae, and some new weapons code.
 * 
 * 24    3/15/99 6:45p Daveb
 * Put in rough nebula bitmap support.
 * 
 * 23    3/12/99 3:19p Enricco
 * Remove spurious Int3()
 * 
 * 22    3/11/99 5:53p Dave
 * More network optimization. Spliced in Dell OEM planet bitmap crap.
 * 
 * 21    3/10/99 6:51p Dave
 * Changed the way we buffer packets for all clients. Optimized turret
 * fired packets. Did some weapon firing optimizations.
 * 
 * 20    2/24/99 4:02p Dave
 * Fixed weapon locking and homing problems for multiplayer dogfight mode.
 * 
 * 19    2/05/99 12:52p Dave
 * Fixed Glide nondarkening textures.
 * 
 * 18    1/29/99 12:47a Dave
 * Put in sounds for beam weapon. A bunch of interface screens (tech
 * database stuff).
 * 
 * 17    1/27/99 9:56a Dave
 * Temporary checkin of beam weapons for Dan to make cool sounds.
 * 
 * 16    1/25/99 5:03a Dave
 * First run of stealth, AWACS and TAG missile support. New mission type
 * :)
 * 
 * 15    1/24/99 11:37p Dave
 * First full rev of beam weapons. Very customizable. Removed some bogus
 * Int3()'s in low level net code.
 * 
 * 14    1/21/99 10:45a Dave
 * More beam weapon stuff. Put in warmdown time.
 * 
 * 13    1/12/99 5:45p Dave
 * Moved weapon pipeline in multiplayer to almost exclusively client side.
 * Very good results. Bandwidth goes down, playability goes up for crappy
 * connections. Fixed object update problem for ship subsystems.
 * 
 * 12    1/08/99 2:08p Dave
 * Fixed software rendering for pofview. Super early support for AWACS and
 * beam weapons.
 * 
 * 11    1/06/99 2:24p Dave
 * Stubs and release build fixes.
 * 
 * 10    12/01/98 6:12p Johnson
 * Make sure to page in weapon impact animations as xparent textures.
 * 
 * 9     11/20/98 4:08p Dave
 * Fixed flak effect in multiplayer.
 * 
 * 8     11/14/98 5:33p Dave
 * Lots of nebula work. Put in ship contrails.
 * 
 * 7     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 6     10/26/98 9:42a Dave
 * Early flak gun support.
 * 
 * 5     10/23/98 3:51p Dave
 * Full support for tstrings.tbl and foreign languages. All that remains
 * is to make it active in Fred.
 * 
 * 4     10/07/98 6:27p Dave
 * Globalized mission and campaign file extensions. Removed Silent Threat
 * special code. Moved \cache \players and \multidata into the \data
 * directory.
 * 
 * 3     10/07/98 4:49p Andsager
 * don't do weapon swap (was needed for mission disk)
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 314   9/21/98 11:19p Dave
 * Weapon name fix.
 * 
 * 313   9/19/98 4:33p Adam
 * Changed default values for particle spew (used on Leech Cannon)
 * 
 * 312   9/13/98 10:51p Dave
 * Put in newfangled icons for mission simulator room. New mdisk.vp
 * checksum and file length.
 * 
 * 311   9/13/98 4:29p Andsager
 * Maintain Weapon_info compataiblity with mission disk
 * 
 * 310   9/13/98 4:26p Andsager
 * 
 * 309   9/01/98 4:25p Dave
 * Put in total (I think) backwards compatibility between mission disk
 * freespace and non mission disk freespace, including pilot files and
 * campaign savefiles.
 * 
 * 308   8/28/98 3:29p Dave
 * EMP effect done. AI effects may need some tweaking as required.
 * 
 * 307   8/25/98 1:49p Dave
 * First rev of EMP effect. Player side stuff basically done. Next comes
 * AI code.
 * 
 * 306   8/18/98 10:15a Dave
 * Touchups on the corkscrew missiles. Added particle spewing weapons.
 * 
 * 305   8/17/98 5:07p Dave
 * First rev of corkscrewing missiles.
 * 
 * 304   6/30/98 2:23p Dave
 * Revised object update system. Removed updates for all weapons. Put
 * button info back into control info packet.
 * 
 * 303   6/22/98 8:36a Allender
 * revamping of homing weapon system.  don't send as object updates
 * anymore
 * 
 * 302   5/24/98 2:25p Allender
 * be sure that homing missiles die on client when lifeleft gets too
 * negative (lost packets)
 * 
 * 301   5/20/98 5:47p Sandeep
 * 
 * 300   5/18/98 1:58a Mike
 * Make Phoenix not be fired at fighters (but yes bombers).
 * Improve positioning of ships in guard mode.
 * Make turrets on player ship not fire near end of support ship docking.
 * 
 * $NoKeywords: $
 */

#include <stdlib.h>

#include "pstypes.h"
#include "systemvars.h"
#include "vecmat.h"
#include "tmapper.h"
#include "2d.h"
#include "3d.h"
#include "bmpman.h"
#include "model.h"
#include "key.h"
#include "physics.h"
#include "floating.h"
#include "model.h"
#include "lighting.h"
#include "object.h"
#include "weapon.h"
#include "ship.h"
#include "fireballs.h"
#include "player.h"
#include "hudtarget.h"
#include "freespace.h"
#include "radar.h"
#include "ai.h"
#include "sound.h"
#include "multi.h"
#include "multimsgs.h"
#include "linklist.h"
#include "timer.h"
#include "gamesnd.h"
#include "cmeasure.h"
#include "shockwave.h"
#include "model.h"
#include "staticrand.h"
#include "swarm.h"
#include "multiutil.h"
#include "shiphit.h"
#include "trails.h"
#include "hud.h"
#include "objcollide.h"
#include "aibig.h"
#include "particle.h"
#include "asteroid.h"
#include "joy_ff.h"
#include "multi_obj.h"
#include "corkscrew.h"
#include "emp.h"
#include "localize.h"
#include "flak.h"
#include "muzzleflash.h"

#ifndef NDEBUG
int Weapon_flyby_sound_enabled = 1;
DCF_BOOL( weapon_flyby, Weapon_flyby_sound_enabled )
#endif

static int Weapon_flyby_sound_timer;	

weapon Weapons[MAX_WEAPONS];
weapon_info Weapon_info[MAX_WEAPON_TYPES];

#define		MISSILE_OBJ_USED	(1<<0)			// flag used in missile_obj struct
#define		MAX_MISSILE_OBJS	MAX_WEAPONS		// max number of missiles tracked in missile list
missile_obj Missile_objs[MAX_MISSILE_OBJS];	// array used to store missile object indexes
missile_obj Missile_obj_list;						// head of linked list of missile_obj structs


// WEAPON EXPLOSION INFO
#define MAX_weapon_expl_lod						4
#define MAX_Weapon_expl_info					3

typedef struct weapon_expl_lod {
	char	filename[MAX_FILENAME_LEN];
	int	bitmap_id;
	int	num_frames;
	int	fps;
} weapon_expl_lod;

typedef struct weapon_expl_info	{
	int					lod_count;	
	weapon_expl_lod		lod[MAX_weapon_expl_lod];
} weapon_expl_info;

weapon_expl_info Weapon_expl_info[MAX_Weapon_expl_info];

int Num_weapon_expl = 0;

int Num_weapon_types = 0;

int Num_weapons = 0;
int Weapons_inited = 0;

int laser_model_inner = -1;
int laser_model_outer = -1;

int missile_model = -1;

char	*Weapon_names[MAX_WEAPON_TYPES];

int	First_secondary_index = -1;

#define	MAX_SPAWN_WEAPONS	10			//	Up to 10 weapons can spawn weapons.

int	Num_spawn_types;
char	Spawn_names[MAX_SPAWN_WEAPONS][NAME_LENGTH];

int Num_player_weapon_precedence;				// Number of weapon types in Player_weapon_precedence
int Player_weapon_precedence[MAX_WEAPON_TYPES];	// Array of weapon types, precedence list for player weapon selection

// Used to avoid playing too many impact sounds in too short a time interval.
// This will elimate the odd "stereo" effect that occurs when two weapons impact at 
// nearly the same time, like from a double laser (also saves sound channels!)
#define	IMPACT_SOUND_DELTA	50		// in milliseconds
int		Weapon_impact_timer;			// timer, initalized at start of each mission

// energy suck defines
#define ESUCK_DEFAULT_WEAPON_REDUCE				(10.0f)
#define ESUCK_DEFAULT_AFTERBURNER_REDUCE		(10.0f)

// scale factor for supercaps taking damage from weapons which are not "supercap" weapons
#define SUPERCAP_DAMAGE_SCALE			0.25f

// scale factor for big ships getting hit by flak
#define FLAK_DAMAGE_SCALE				0.05f

extern int Max_allowed_player_homers[];
extern int compute_num_homing_objects(object *target_objp);

// 
void parse_weapon_expl_tbl()
{
#ifndef MAKE_FS1
	int	rval, idx;
	char base_filename[256] = "";

	// open localization
	lcl_ext_open();

	if ((rval = setjmp(parse_abort)) != 0) {
		Error(LOCATION, "Unable to parse weapon_expl.tbl!  Code = %i.\n", rval);
	}
	else {
		read_file_text(NOX("weapon_expl.tbl"));
		reset_parse();		
	}

	Num_weapon_expl = 0;
	required_string("#Start");
	while (required_string_either("#End","$Name:")) {
		Assert( Num_weapon_expl < MAX_Weapon_expl_info);

		// base filename
		required_string("$Name:");
		stuff_string(base_filename, F_NAME, NULL);

		// # of lod levels - make sure old fireball.tbl is compatible
		Weapon_expl_info[Num_weapon_expl].lod_count = 1;
		if(optional_string("$LOD:")){
			stuff_int(&Weapon_expl_info[Num_weapon_expl].lod_count);
		}

		// stuff default filename
		strcpy(Weapon_expl_info[Num_weapon_expl].lod[0].filename, base_filename);

		// stuff LOD level filenames
		for(idx=1; idx<Weapon_expl_info[Num_weapon_expl].lod_count; idx++){
			if(idx >= MAX_weapon_expl_lod){
				break;
			}

			sprintf(Weapon_expl_info[Num_weapon_expl].lod[idx].filename, "%s_%d", base_filename, idx);
		}

		Num_weapon_expl++;
	}
	required_string("#End");

	// close localization
	lcl_ext_close();
#else
	// hard coded value for FS1
	Num_weapon_expl = 0;

	Weapon_expl_info[Num_weapon_expl].lod_count = 1;
	strncpy(Weapon_expl_info[Num_weapon_expl].lod[0].filename, "ExpMissileHit1", MAX_FILENAME_LEN);

	Num_weapon_expl++;
#endif
}

int get_weapon_expl_info_index(char *filename)
{
	for (int i=0; i<MAX_Weapon_expl_info; i++) {
		if ( stricmp(Weapon_expl_info[i].lod[0].filename, filename) == 0) {
			return i;
		}
	}

	return -1;
}
// ----------------------------------------------------------------------
// missile_obj_list_init()
//
// Clear out the Missile_obj_list
//
void missile_obj_list_init()
{
	int i;

	list_init(&Missile_obj_list);
	for ( i = 0; i < MAX_MISSILE_OBJS; i++ ) {
		Missile_objs[i].flags = 0;
	}
}

// ---------------------------------------------------
// missile_obj_list_add()
//
// Function to add a node from the Missile_obj_list.  Only
// called from weapon_create()
int missile_obj_list_add(int objnum)
{
	int i;

	for ( i = 0; i < MAX_MISSILE_OBJS; i++ ) {
		if ( !(Missile_objs[i].flags & MISSILE_OBJ_USED) )
			break;
	}
	if ( i == MAX_MISSILE_OBJS ) {
		Error(LOCATION, "Fatal Error: Ran out of missile object nodes\n");
		return -1;
	}
	
	Missile_objs[i].flags = 0;
	Missile_objs[i].objnum = objnum;
	list_append(&Missile_obj_list, &Missile_objs[i]);
	Missile_objs[i].flags |= MISSILE_OBJ_USED;

	return i;
}

// ---------------------------------------------------
// missle_obj_list_remove()
//
// Function to remove a node from the Missile_obj_list.  Only
// called from weapon_delete()
void missle_obj_list_remove(int index)
{
	Assert(index >= 0 && index < MAX_MISSILE_OBJS);
	list_remove(&Missile_obj_list, &Missile_objs[index]);	
	Missile_objs[index].flags = 0;
}

// ---------------------------------------------------
// missile_obj_list_rebuild()
//
// Called by the save/restore code to rebuild Missile_obj_list
//
void missile_obj_list_rebuild()
{
	object *objp;

	missile_obj_list_init();

	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if ( objp->type == OBJ_WEAPON && Weapon_info[Weapons[objp->instance].weapon_info_index].subtype == WP_MISSILE ) {
			Weapons[objp->instance].missile_list_index = missile_obj_list_add(OBJ_INDEX(objp));
		}
	}
}

// If this is a player countermeasure, let the player know he evaded a missile
void weapon_maybe_alert_cmeasure_success(object *objp)
{
	if ( objp->type == OBJ_CMEASURE ) {
		cmeasure *cmp;
		cmp = &Cmeasures[objp->instance];
		if ( cmp->source_objnum == OBJ_INDEX(Player_obj) ) {
			hud_start_text_flash(XSTR("Evaded", 1430), 800);
			snd_play(&Snds[SND_MISSILE_EVADED_POPUP]);
		} else if ( Objects[cmp->source_objnum].flags & OF_PLAYER_SHIP ) {
			send_countermeasure_success_packet( cmp->source_objnum );
		}
	}
}

// ---------------------------------------------------
// missile_obj_return_address()
//
// Called externally to generate an address from an index into
// the Missile_objs[] array
//
missile_obj *missile_obj_return_address(int index)
{
	Assert(index >= 0 && index < MAX_MISSILE_OBJS);
	return &Missile_objs[index];
}

//	Return the index of Weapon_info[].name that is *name.
int weapon_info_lookup(const char *name)
{
	int	i;

#ifdef MAKE_FS1
	// fix the stupid table stuff - it's non-fatal but annoying error messages
	if (!strcmp(name, "Disruptor Missile")) {
		name = "D-Missile";
	// this one fixes the same issue with a SilentThreat mission
	} else if (!strcmp(name, "Shield Breaker")) {
		name = "S-Breaker";
	}
#endif

	for (i=0; i<Num_weapon_types; i++)
		if (!stricmp(name, Weapon_info[i].name))
			return i;

	return -1;
}

#define DEFAULT_WEAPON_SPAWN_COUNT	10

//	Parse the weapon flags.
void parse_wi_flags(weapon_info *weaponp)
{
	char	weapon_strings[MAX_WEAPON_FLAGS][NAME_LENGTH];
	int	num_strings;

	required_string("$Flags:");

	num_strings = stuff_string_list(weapon_strings, MAX_WEAPON_FLAGS);
	
	for (int i=0; i<num_strings; i++) {
		if (!stricmp(NOX("Electronics"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_ELECTRONICS;		
		else if (!strnicmp(NOX("Spawn"), weapon_strings[i], 5)) {
			if (weaponp->spawn_type == -1) {
				int	skip_length, name_length;
				char	*temp_string;

				temp_string = weapon_strings[i];

				weaponp->wi_flags |= WIF_SPAWN;
				weaponp->spawn_type = (short)Num_spawn_types;
				skip_length = strlen(NOX("Spawn")) + strspn(&temp_string[strlen(NOX("Spawn"))], NOX(" \t"));
				char *num_start = strchr(&temp_string[skip_length], ',');
				if (num_start == NULL) {
					weaponp->spawn_count = DEFAULT_WEAPON_SPAWN_COUNT;
					name_length = 999;
				} else {
					weaponp->spawn_count = (short)atoi(num_start+1);
					name_length = num_start - temp_string - skip_length;
				}

				strncpy(Spawn_names[Num_spawn_types++], &(weapon_strings[i][skip_length]), name_length);
				Assert(Num_spawn_types < MAX_SPAWN_WEAPONS);
			} else
				Warning(LOCATION, "Illegal to have two spawn types for one weapon.\n"
										"Ignoring weapon %s", weapon_strings[i]);
		} else if (!stricmp(NOX("Remote Detonate"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_REMOTE;
		else if (!stricmp(NOX("Puncture"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_PUNCTURE;		
		else if (!stricmp(NOX("Big Ship"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_BIG_ONLY;
		else if (!stricmp(NOX("Huge"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_HUGE;
		else if (!stricmp(NOX("Bomber+"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_BOMBER_PLUS;
		else if (!stricmp(NOX("child"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_CHILD;
		else if (!stricmp(NOX("Bomb"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_BOMB;
		else if (!stricmp(NOX("No Dumbfire"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_NO_DUMBFIRE;
		else if (!stricmp(NOX("In tech database"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_IN_TECH_DATABASE;
		else if (!stricmp(NOX("Player allowed"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_PLAYER_ALLOWED;		
		else if (!stricmp(NOX("Particle Spew"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_PARTICLE_SPEW;
		else if (!stricmp(NOX("EMP"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_EMP;
		else if (!stricmp(NOX("Esuck"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_ENERGY_SUCK;
		else if (!stricmp(NOX("Flak"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_FLAK;
		else if (!stricmp(NOX("Corkscrew"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_CORKSCREW;
		else if (!stricmp(NOX("Shudder"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_SHUDDER;		
		else if (!stricmp(NOX("lockarm"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_LOCKARM;		
		else if (!stricmp(NOX("beam"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_BEAM;
		else if (!stricmp(NOX("stream"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_STREAM;
		else if (!stricmp(NOX("supercap"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_SUPERCAP;
#ifdef MAKE_FS1
		else if (!stricmp(NOX("Swarm"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_SWARM;
		else if (!stricmp(NOX("No Ship"), weapon_strings[i]))
			weaponp->wi_flags |= WIF_CHILD;
#endif 
		else
			Warning(LOCATION, "Bogus string in weapon flags: %s\n", weapon_strings[i]);
	}	

	// SWARM, CORKSCREW and FLAK should be mutually exclusive
	if(weaponp->wi_flags & WIF_FLAK){
		Assert(!(weaponp->wi_flags & WIF_CORKSCREW) && !(weaponp->wi_flags & WIF_SWARM));
	}
	if(weaponp->wi_flags & WIF_CORKSCREW){
		Assert(!(weaponp->wi_flags & WIF_FLAK) && !(weaponp->wi_flags & WIF_SWARM));
	}
	if(weaponp->wi_flags & WIF_SWARM){
		Assert(!(weaponp->wi_flags & WIF_CORKSCREW) && !(weaponp->wi_flags & WIF_FLAK));
	}

	// make sure flak guns are only placed on turrets
	if(weaponp->wi_flags & WIF_FLAK){
		Assert(weaponp->wi_flags & WIF_BIG_ONLY);
	}
}

// function to parse the information for a specific weapon type.	
// return 0 if successful, otherwise return -1
#define WEAPONS_MULTITEXT_LENGTH 2048

int parse_weapon()
{
	char buf[WEAPONS_MULTITEXT_LENGTH];
	weapon_info *wip;
	char fname[255] = "";
	int idx;

	wip = &Weapon_info[Num_weapon_types];

	wip->wi_flags = 0;

	required_string("$Name:");
	stuff_string(wip->name, F_NAME, NULL);
	diag_printf ("Weapon name -- %s\n", wip->name);

	// AL 28-3-98: If this is a demo build, we only want to parse weapons that are preceded with
	//             the '@' symbol
	#ifdef DEMO // not needed FS2_DEMO (separate table file)
		if ( wip->name[0] != '@' ) {
			// advance to next weapon, and return -1

			if ( skip_to_start_of_strings("$Name:", "#End") != 1 ) {
				Int3();
			}
			return -1;
		}
	#endif

	if ( wip->name[0] == '@' ) {
		char old_name[NAME_LENGTH];
		strcpy(old_name, wip->name);
		strcpy(wip->name, old_name+1);
	}

	wip->title[0] = 0;
	if (optional_string("+Title:")) {
		stuff_string(wip->title, F_NAME, NULL, WEAPON_TITLE_LEN);
	}

	wip->desc = NULL;
	if (optional_string("+Description:")) {
		stuff_string(buf, F_MULTITEXT, NULL);
		wip->desc = strdup(buf);
	}

	wip->tech_title[0] = 0;
	if (optional_string("+Tech Title:")) {
		stuff_string(wip->tech_title, F_NAME, NULL, NAME_LENGTH);
	}

	wip->tech_anim_filename[0] = 0;
	if (optional_string("+Tech Anim:")) {
		stuff_string(wip->tech_anim_filename, F_NAME, NULL, NAME_LENGTH);
	}

	wip->tech_desc = NULL;
	if (optional_string("+Tech Description:")) {
		stuff_string(buf, F_MULTITEXT, NULL, WEAPONS_MULTITEXT_LENGTH);
		wip->tech_desc = strdup(buf);
	}

	//	Read the model file.  It can be a POF file or none.
	//	If there is no model file (Model file: = "none") then we use our special
	//	laser renderer which requires inner, middle and outer information.
	required_string("$Model file:");
	stuff_string(wip->pofbitmap_name, F_NAME, NULL);
	diag_printf ("Model pof file -- %s\n", wip->pofbitmap_name );
	if ( stricmp(wip->pofbitmap_name, NOX("none")) ) {
		wip->model_num = -1;				
		wip->render_type = WRT_POF;
		wip->laser_bitmap = -1;
	} else {
		//	No POF or AVI file specified, render as special laser type.
		ubyte r,g,b;

		wip->render_type = WRT_LASER;
		wip->model_num = -1;

		// laser bitmap itself
		required_string("@Laser Bitmap:");
		stuff_string(wip->pofbitmap_name, F_NAME, NULL);
		wip->laser_bitmap = -1;
		if(!Fred_running){
			wip->laser_bitmap = bm_load( wip->pofbitmap_name );
		}

		// optional laser glow
		wip->laser_glow_bitmap = -1;
		if(optional_string("@Laser Glow:")){
			stuff_string(fname, F_NAME, NULL);		
			if(!Fred_running){
				wip->laser_glow_bitmap = bm_load( fname );

				// might as well lock it down as an aabitmap now
				if(wip->laser_glow_bitmap >= 0){
					bm_lock(wip->laser_glow_bitmap, 8, BMP_AABITMAP);
					bm_unlock(wip->laser_glow_bitmap);
				}
			}
		}
		
		required_string("@Laser Color:");
		stuff_byte(&r);	stuff_byte(&g);	stuff_byte(&b);
		gr_init_color( &wip->laser_color_1, r, g, b );

		// optional string for cycling laser colors
		gr_init_color(&wip->laser_color_2, 0, 0, 0);
		if(optional_string("@Laser Color2:")){
			stuff_byte(&r);	stuff_byte(&g);	stuff_byte(&b);
			gr_init_color( &wip->laser_color_2, r, g, b );
		}

		required_string("@Laser Length:");
		stuff_float(&wip->laser_length);
		
		required_string("@Laser Head Radius:");
		stuff_float(&wip->laser_head_radius);

		required_string("@Laser Tail Radius:");
		stuff_float(&wip->laser_tail_radius );
	}

	required_string("$Mass:");
	stuff_float( &(wip->mass) );
	diag_printf ("Weapon mass -- %7.3f\n", wip->mass);

	required_string("$Velocity:");
	stuff_float( &(wip->max_speed) );
	diag_printf ("Weapon mass -- %7.3f\n", wip->max_speed);

	required_string("$Fire Wait:");
	stuff_float( &(wip->fire_wait) );
	diag_printf ("Weapon fire wait -- %7.3f\n", wip->fire_wait);

	required_string("$Damage:");
	stuff_float(&wip->damage);

	// secondary weapons require these values
	if (First_secondary_index != -1) {
		required_string("$Blast Force:");
		stuff_float( &(wip->blast_force) );
		diag_printf ("Weapon blast force -- %7.3f\n", wip->blast_force);

		required_string("$Inner Radius:");
		stuff_float( &(wip->inner_radius) );
		if ( wip->inner_radius != 0 ) {
			wip->wi_flags |= WIF_AREA_EFFECT;
		}
		diag_printf ("Weapon inner blast radius -- %7.3f\n", wip->inner_radius);

		required_string("$Outer Radius:");
		stuff_float( &(wip->outer_radius) );
		if ( wip->outer_radius != 0 ) {
			wip->wi_flags |= WIF_AREA_EFFECT;
		}
		diag_printf ("Weapon outer blast radius -- %7.3f\n", wip->outer_radius);

		required_string("$Shockwave Speed:");
		stuff_float( &(wip->shockwave_speed) );
		if ( wip->shockwave_speed != 0 ) {
			wip->wi_flags |= WIF_SHOCKWAVE;
		}
		diag_printf ("Shockwave speed -- %7.3f\n", wip->shockwave_speed);
	} 
	// for primary weapons they're optional
	else {
		if(optional_string("$Blast Force:")){
			stuff_float( &(wip->blast_force) );
			diag_printf ("Weapon blast force -- %7.3f\n", wip->blast_force);
		}

		if(optional_string("$Inner Radius:")){
			stuff_float( &(wip->inner_radius) );
			if ( wip->inner_radius != 0 ) {
				wip->wi_flags |= WIF_AREA_EFFECT;
			}
			diag_printf ("Weapon inner blast radius -- %7.3f\n", wip->inner_radius);
		}

		if(optional_string("$Outer Radius:")){
			stuff_float( &(wip->outer_radius) );
			if ( wip->outer_radius != 0 ) {
				wip->wi_flags |= WIF_AREA_EFFECT;
			}
			diag_printf ("Weapon outer blast radius -- %7.3f\n", wip->outer_radius);
		}

		if(optional_string("$Shockwave Speed:")){
			stuff_float( &(wip->shockwave_speed) );
			if ( wip->shockwave_speed != 0 ) {
				wip->wi_flags |= WIF_SHOCKWAVE;
			}
			diag_printf ("Shockwave speed -- %7.3f\n", wip->shockwave_speed);
		}
	}

	required_string("$Armor Factor:");
	stuff_float(&wip->armor_factor);

	required_string("$Shield Factor:");
	stuff_float(&wip->shield_factor);

	required_string("$Subsystem Factor:");
	stuff_float(&wip->subsystem_factor);

	required_string("$Lifetime:");
	stuff_float(&wip->lifetime);

	required_string("$Energy Consumed:");
	stuff_float(&wip->energy_consumed);

	required_string("$Cargo Size:");
	stuff_float(&wip->cargo_size);

	int is_homing=0;
	required_string("$Homing:");
	stuff_boolean(&is_homing);

	if (is_homing == 1) {
		char	temp_type[128];

		// the following five items only need to be recorded if the weapon is a homing weapon
		required_string("+Type:");
		stuff_string(temp_type, F_NAME, NULL);

		if (!stricmp(temp_type, NOX("HEAT"))) {
			float	view_cone_angle;

			wip->wi_flags |= WIF_HOMING_HEAT | WIF_TURNS;

			required_string("+Turn Time:");			
			stuff_float(&wip->turn_time);

			required_string("+View Cone:");
			stuff_float(&view_cone_angle);

			wip->fov = (float)cos((float)(ANG_TO_RAD(view_cone_angle/2.0f)));

		} else if (!stricmp(temp_type, NOX("ASPECT"))) {
			wip->wi_flags |= WIF_HOMING_ASPECT | WIF_TURNS;

			required_string("+Turn Time:");			
			stuff_float(&wip->turn_time);

			required_string("+Min Lock Time:");			// minimum time (in seconds) to achieve lock
			stuff_float(&wip->min_lock_time);

			required_string("+Lock Pixels/Sec:");		// pixels/sec moved while locking
			stuff_int(&wip->lock_pixels_per_sec);

			required_string("+Catch-up Pixels/Sec:");	// pixels/sec moved while catching-up for a lock
			stuff_int(&wip->catchup_pixels_per_sec);

			required_string("+Catch-up Penalty:");		// number of extra pixels to move while locking as a penalty for catching up for a lock
			stuff_int(&wip->catchup_pixel_penalty);
		} else
			Error(LOCATION, "Illegal homing type = %s.\nMust be HEAT or ASPECT.\n", temp_type);

	}

	// swarm missiles
	int s_count;
	wip->swarm_count = -1;
	if(optional_string("$Swarm:")){
		wip->swarm_count = SWARM_DEFAULT_NUM_MISSILES_FIRED;
		stuff_int(&s_count);
		wip->swarm_count = (short)s_count;

		// flag as being a swarm weapon
		wip->wi_flags |= WIF_SWARM;
	}

	required_string("$LaunchSnd:");
	stuff_int(&wip->launch_snd);

	required_string("$ImpactSnd:");
	stuff_int(&wip->impact_snd);

	if (First_secondary_index != -1) {
		required_string("$FlyBySnd:");
		stuff_int(&wip->flyby_snd);
	}

	//	Secondary weapons are required to have a rearm rate.
	if (First_secondary_index != -1) {
		required_string( "$Rearm Rate:");
		stuff_float( &wip->rearm_rate );
		if (wip->rearm_rate > 0.1f)
			wip->rearm_rate = 1.0f/wip->rearm_rate;
		else
			wip->rearm_rate = 1.0f;
	}

	wip->weapon_range = 999999999.9f;
	if (optional_string("+Weapon Range:")) {
		stuff_float(&wip->weapon_range);
	}

	wip->spawn_type = -1;
	parse_wi_flags(wip);

	char trail_name[MAX_FILENAME_LEN] = "";
	trail_info *ti = &wip->tr_info;
	memset(ti, 0, sizeof(trail_info));
#ifndef MAKE_FS1
	if(optional_string("$Trail:")){	
		wip->wi_flags |= WIF_TRAIL;		// missile leaves a trail

		required_string("+Start Width:");
		stuff_float(&ti->w_start);

		required_string("+End Width:");
		stuff_float(&ti->w_end);

		required_string("+Start Alpha:");
		stuff_float(&ti->a_start);

		required_string("+End Alpha:");
		stuff_float(&ti->a_end);		

		required_string("+Max Life:");
		stuff_float(&ti->max_life);

		ti->stamp = fl2i(1000.0f*ti->max_life)/(NUM_TRAIL_SECTIONS+1);

		required_string("+Bitmap:");
		stuff_string(trail_name, F_NAME, NULL);
		ti->bitmap = bm_load(trail_name);
		// wip->delta_time = fl2i(1000.0f*wip->max_life)/(NUM_TRAIL_SECTIONS+1);		// time between sections.  max_life / num_sections basically.
	}
#else
	// seemed easier to separate this out from above
	int has_trail=0;
	required_string("$Trail:");
	stuff_boolean(&has_trail);

	if (has_trail == 1) {
		wip->wi_flags |= WIF_TRAIL;     // missile leaves a trail

		required_string("+Head Width:");
		stuff_float(&ti->w_start);

		required_string("+Tail Width:");
		stuff_float(&ti->w_end);

		ti->a_start = 1.0;
		ti->a_end = 0.0;

		required_string("+Life:");
		stuff_float(&ti->max_life);

		ti->stamp = fl2i(1000.0f*ti->max_life)/(NUM_TRAIL_SECTIONS+1);

		required_string("+Bitmap:");
		stuff_string(trail_name, F_NAME, NULL);
		ti->bitmap = bm_load(trail_name);
	}
#endif

	// read in filename for icon that is used in weapons selection
	wip->icon_filename[0] = 0;
	if ( optional_string("$Icon:") ) {
		stuff_string(wip->icon_filename, F_NAME, NULL);
	}

	// read in filename for animation that is used in weapons selection
	wip->anim_filename[0] = 0;
	if ( optional_string("$Anim:") ) {
		stuff_string(wip->anim_filename, F_NAME, NULL);
	}

	wip->impact_weapon_expl_index = -1;
	if ( optional_string("$Impact Explosion:") ) {
		char impact_ani_file[FILESPEC_LENGTH];
		stuff_string(impact_ani_file, F_NAME, NULL);
		if ( stricmp(impact_ani_file,NOX("none")))	{
			wip->impact_weapon_expl_index = get_weapon_expl_info_index(impact_ani_file);
			//int num_frames, fps;
			//wip->impact_explosion_ani = bm_load_animation( impact_ani_file, &num_frames, &fps, 1 );

			required_string("$Impact Explosion Radius:");
			stuff_float(&wip->impact_explosion_radius);
		}
	}

	// muzzle flash
	char mflash_string[255] = "";
	wip->muzzle_flash = -1;
	if( optional_string("$Muzzleflash:") ){
		stuff_string(mflash_string, F_NAME, NULL);

		// look it up
		wip->muzzle_flash = mflash_lookup(mflash_string);

		if(wip->muzzle_flash >= 0){			
			wip->wi_flags |= WIF_MFLASH;
		}
	}

	// EMP optional stuff (if WIF_EMP is not set, none of this matters, anyway)
	if( optional_string("$EMP Intensity:") ){
		stuff_float(&wip->emp_intensity);
	} else {
		wip->emp_intensity = EMP_DEFAULT_INTENSITY;
	}
	if( optional_string("$EMP Time:") ){
		stuff_float(&wip->emp_time);
	} else {
		wip->emp_intensity = EMP_DEFAULT_TIME;
	}

	// Energy suck optional stuff (if WIF_ENERGY_SUCK is not set, none of this matters anyway)
	if( optional_string("$Leech Weapon:") ){
		stuff_float(&wip->weapon_reduce);
	} else {
		wip->weapon_reduce = ESUCK_DEFAULT_WEAPON_REDUCE;
	}
	if( optional_string("$Leech Afterburner:") ){
		stuff_float(&wip->afterburner_reduce);
	} else {
		wip->afterburner_reduce = ESUCK_DEFAULT_AFTERBURNER_REDUCE;
	}

	// beam weapon optional stuff
	wip->b_info.beam_type = -1;
	wip->b_info.beam_life = -1.0f;
	wip->b_info.beam_warmup = -1;
	wip->b_info.beam_warmdown = -1;
	wip->b_info.beam_muzzle_radius = 0.0f;
	wip->b_info.beam_particle_count = -1;
	wip->b_info.beam_particle_radius = 0.0f;
	wip->b_info.beam_particle_angle = 0.0f;
	wip->b_info.beam_particle_ani = -1;	
	wip->b_info.beam_loop_sound = -1;
	wip->b_info.beam_warmup_sound = -1;
	wip->b_info.beam_warmdown_sound = -1;
	wip->b_info.beam_num_sections = 0;
	wip->b_info.beam_glow_bitmap = -1;
	wip->b_info.beam_shots = 0;
	wip->b_info.beam_shrink_factor = 0.0f;
	wip->b_info.beam_shrink_pct = 0.0f;
	if( optional_string("$BeamInfo:")){
		// beam type
		required_string("+Type:");
		stuff_int(&wip->b_info.beam_type);

		// how long it lasts
		required_string("+Life:");
		stuff_float(&wip->b_info.beam_life);

		// warmup time
		required_string("+Warmup:");
		stuff_int(&wip->b_info.beam_warmup);

		// warmdowm time
		required_string("+Warmdown:");
		stuff_int(&wip->b_info.beam_warmdown);

		// muzzle glow radius
		required_string("+Radius:");
		stuff_float(&wip->b_info.beam_muzzle_radius);

		// particle spew count
		required_string("+PCount:");
		stuff_int(&wip->b_info.beam_particle_count);

		// particle radius
		required_string("+PRadius:");
		stuff_float(&wip->b_info.beam_particle_radius);

		// angle off turret normal
		required_string("+PAngle:");
		stuff_float(&wip->b_info.beam_particle_angle);

		// particle bitmap/ani		
		required_string("+PAni:");
		stuff_string(fname, F_NAME, NULL);
		if(!Fred_running){
			int num_frames, fps;
			wip->b_info.beam_particle_ani = bm_load_animation(fname, &num_frames, &fps, 1);
		}

		// magic miss #
		required_string("+Miss Factor:");		
		for(idx=0; idx<NUM_SKILL_LEVELS; idx++){
			wip->b_info.beam_miss_factor[idx] = 0.00001f;
			stuff_float(&wip->b_info.beam_miss_factor[idx]);
		}

		// beam fire sound
		required_string("+BeamSound:");
		stuff_int(&wip->b_info.beam_loop_sound);

		// warmup sound
		required_string("+WarmupSound:");
		stuff_int(&wip->b_info.beam_warmup_sound);

		// warmdown sound
		required_string("+WarmdownSound:");
		stuff_int(&wip->b_info.beam_warmdown_sound);

		// glow bitmap
		required_string("+Muzzleglow:");
		stuff_string(fname, F_NAME, NULL);
		if(!Fred_running){
			wip->b_info.beam_glow_bitmap = bm_load(fname);
		}

		// # of shots (only used for type D beams)
		required_string("+Shots:");
		stuff_int(&wip->b_info.beam_shots);

		// shrinkage
		required_string("+ShrinkFactor:");
		stuff_float(&wip->b_info.beam_shrink_factor);
		required_string("+ShrinkPct:");
		stuff_float(&wip->b_info.beam_shrink_pct);

		// beam sections
		while( optional_string("$Section:") ){
			beam_weapon_section_info i;
			char tex_name[255] = "";
			
			// section width
			required_string("+Width:");
			stuff_float(&i.width);

			// texture
			required_string("+Texture:");
			stuff_string(tex_name, F_NAME, NULL);
			i.texture = -1;
			if(!Fred_running){
				i.texture = bm_load(tex_name);
				if(i.texture >= 0){
					bm_lock(i.texture, 16, BMP_TEX_OTHER);
					bm_unlock(i.texture);
				}
			}

			// rgba inner
			required_string("+RGBA Inner:");
			stuff_byte(&i.rgba_inner[0]);
			stuff_byte(&i.rgba_inner[1]);
			stuff_byte(&i.rgba_inner[2]);
			stuff_byte(&i.rgba_inner[3]);

			// rgba outer
			required_string("+RGBA Outer:");
			stuff_byte(&i.rgba_outer[0]);
			stuff_byte(&i.rgba_outer[1]);
			stuff_byte(&i.rgba_outer[2]);
			stuff_byte(&i.rgba_outer[3]);

			// flicker
			required_string("+Flicker:");
			stuff_float(&i.flicker);			

			// zadd
			required_string("+Zadd:");
			stuff_float(&i.z_add);

			// maybe copy it
			if(wip->b_info.beam_num_sections < MAX_BEAM_SECTIONS - 1){
				wip->b_info.sections[wip->b_info.beam_num_sections++] = i;
			}
		}		
	}

	// tag weapon optional stuff
	wip->tag_level = -1;
	wip->tag_time = -1.0f;
	if( optional_string("$Tag:")){
		stuff_int(&wip->tag_level);
		stuff_float(&wip->tag_time);		
		wip->wi_flags |= WIF_TAG;
	}	

	return 0;
}

// function to parse the information for a specific ship type.	
void parse_cmeasure()
{
	cmeasure_info *cmeasurep;

	cmeasurep = &Cmeasure_info[Num_cmeasure_types];

	required_string("$Name:");
	stuff_string(cmeasurep->cmeasure_name, F_NAME, NULL);

/*$Name:					Type One
$Velocity:				20.0				;; speed relative to ship, rear-fired until POF info added, MK, 5/22/97
$Fire Wait:				0.5
$Lifetime Min:			1.0				;; Minimum lifetime
$Lifetime Max:			2.0				;; Maximum lifetime.  Actual lifetime is rand(min..max).
$LaunchSnd:				counter_1.wav,	.8, 10, 300	;; countermeasure 1 fired (sound is 3d)
*/

	required_string("$Velocity:");
	stuff_float( &(cmeasurep->max_speed) );

	required_string("$Fire Wait:");
	stuff_float( &(cmeasurep->fire_wait) );

	required_string("$Lifetime Min:");
	stuff_float(&cmeasurep->life_min);

	required_string("$Lifetime Max:");
	stuff_float(&cmeasurep->life_max);

	required_string("$LaunchSnd:");
	stuff_int(&cmeasurep->launch_sound);

	required_string("$Model:");
	stuff_string(cmeasurep->pof_name, F_NAME, NULL);
	cmeasurep->model_num = -1;		
}


//	For all weapons that spawn weapons, given an index at weaponp->spawn_type,
// convert the strings in Spawn_names to indices in the Weapon_types array.
void translate_spawn_types()
{
	int	i,j;

	for (i=0; i<Num_weapon_types; i++)
		if (Weapon_info[i].spawn_type != -1) {
			int	spawn_type = Weapon_info[i].spawn_type;

			for (j=0; j<Num_weapon_types; j++)
				if (!stricmp(Spawn_names[spawn_type], Weapon_info[j].name)) {
					Weapon_info[i].spawn_type = (short)j;
					if (i == j){
						Warning(LOCATION, "Weapon %s spawns itself.  Infinite recursion?\n", Weapon_info[i].name);
					}
				}
		}
}

void parse_weaponstbl()
{
	// open localization
	lcl_ext_open();

	read_file_text("weapons.tbl");
	reset_parse();

	Num_weapon_types = 0;
	First_secondary_index = -1;
	Num_spawn_types = 0;
	
	required_string("#Primary Weapons");
	while (required_string_either("#End", "$Name:")) {
		Assert( Num_weapon_types < MAX_WEAPON_TYPES );
		// AL 28-3-98: If parse_weapon() fails, try next .tbl weapon
		if ( parse_weapon() ) {
			continue;
		}
		Weapon_info[Num_weapon_types].subtype = WP_LASER;
		Num_weapon_types++;
	}
	required_string("#End");

	required_string("#Secondary Weapons");
	First_secondary_index = Num_weapon_types;
	while (required_string_either("#End", "$Name:")) {
		Assert( Num_weapon_types < MAX_WEAPON_TYPES );
		// AL 28-3-98: If parse_weapon() fails, try next .tbl weapon
		if ( parse_weapon() ) {
			continue;
		}
		Weapon_info[Num_weapon_types].subtype = WP_MISSILE;
		Num_weapon_types++;
	}
	required_string("#End");

	required_string("#Beam Weapons");
	while (required_string_either("#End", "$Name:")) {
		Assert( Num_weapon_types < MAX_WEAPON_TYPES );
		// AL 28-3-98: If parse_weapon() fails, try next .tbl weapon
		if ( parse_weapon() ) {
			continue;
		}
		Weapon_info[Num_weapon_types].subtype = WP_BEAM;
		Num_weapon_types++;
	}
	required_string("#End");

	required_string("#Countermeasures");
	while (required_string_either("#End", "$Name:")) {
		Assert( Num_cmeasure_types < MAX_CMEASURE_TYPES );
		parse_cmeasure();
		Num_cmeasure_types++;
	}

	required_string("#End");

	// Read in a list of weapon_info indicies that are an ordering of the player weapon precedence.
	// This list is used to select an alternate weapon when a particular weapon is not available
	// during weapon selection.
	required_string("$Player Weapon Precedence:");
	Num_player_weapon_precedence = stuff_int_list(Player_weapon_precedence, MAX_WEAPON_TYPES, WEAPON_LIST_TYPE);

	translate_spawn_types();

	// close localization
	lcl_ext_close();
}

void create_weapon_names()
{
	int	i;

	for (i=0; i<Num_weapon_types; i++)
		Weapon_names[i] = Weapon_info[i].name;
}

// This will get called once at game startup
void weapon_init()
{
	int rval;

	if ( !Weapons_inited ) {
#if !(defined(FS2_DEMO) || defined(FS1_DEMO))
		// parse weapon_exp.tbl
		parse_weapon_expl_tbl();
#endif
		// parse weapons.tbl
		if ((rval = setjmp(parse_abort)) != 0) {
			Error(LOCATION, "Error parsing 'weapons.tbl'\r\nError code = %i.\r\n", rval);
		} else {			
			parse_weaponstbl();
			create_weapon_names();
			Weapons_inited = 1;
		}
	}

	weapon_level_init();
}


// This will get called at the start of each level.
void weapon_level_init()
{
	int i;

	// Reset everything between levels
	Num_weapons = 0;
	for (i=0; i<MAX_WEAPONS; i++)	{
		Weapons[i].objnum = -1;
		Weapons[i].weapon_info_index = -1;
	}

	trail_level_init();		// reset all missile trails

	swarm_level_init();
	missile_obj_list_init();
	
	cscrew_level_init();

	// emp effect
	emp_level_init();

	Weapon_flyby_sound_timer = timestamp(0);
	Weapon_impact_timer = 1;	// inited each level, used to reduce impact sounds
}

MONITOR( NumWeaponsRend );	

float weapon_glow_scale_f = 2.3f;
float weapon_glow_scale_r = 2.3f;
float weapon_glow_scale_l = 1.5f;
float weapon_glow_alpha_d3d = 0.85f;
float weapon_glow_alpha_glide = 0.99f;
void weapon_render(object *obj)
{
	int num;
	weapon_info *wip;
	weapon *wp;
	color c;

	MONITOR_INC(NumWeaponsRend, 1);

	Assert(obj->type == OBJ_WEAPON);

	num = obj->instance;
	wp = &Weapons[num];
	wip = &Weapon_info[Weapons[num].weapon_info_index];

	switch (wip->render_type) {
		case WRT_LASER: {
			// turn off fogging for good measure
			gr_fog_set(GR_FOGMODE_NONE, 0, 0, 0, -1.0f, -1.0f);

			if (wip->laser_bitmap >= 0) {					
				gr_set_color_fast(&wip->laser_color_1);
				gr_set_bitmap(wip->laser_bitmap, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, 0.99999f, -1, -1);

				vector headp;
				vm_vec_scale_add(&headp, &obj->pos, &obj->orient.v.fvec, wip->laser_length);
				wp->weapon_flags &= ~WF_CONSIDER_FOR_FLYBY_SOUND;
				if ( g3_draw_laser(&headp, wip->laser_head_radius, &obj->pos, wip->laser_tail_radius) ) {
					wp->weapon_flags |= WF_CONSIDER_FOR_FLYBY_SOUND;
				}
			}			

			// maybe draw laser glow bitmap
			if(wip->laser_glow_bitmap >= 0){
				// get the laser color
				weapon_get_laser_color(&c, obj);

				vector headp2;			
				vm_vec_scale_add(&headp2, &obj->pos, &obj->orient.v.fvec, wip->laser_length * weapon_glow_scale_l);
				gr_set_bitmap(wip->laser_glow_bitmap, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, (gr_screen.mode == GR_DIRECT3D || gr_screen.mode == GR_OPENGL) ? weapon_glow_alpha_d3d : weapon_glow_alpha_glide, -1, -1);
				g3_draw_laser_rgb(&headp2, wip->laser_head_radius * weapon_glow_scale_f, &obj->pos, wip->laser_tail_radius * weapon_glow_scale_r, c.red, c.green, c.blue);
			}						
			break;
		}

		case WRT_POF:	{
				uint render_flags = MR_NORMAL|MR_IS_MISSILE|MR_NO_LIGHTING;

				model_clear_instance(wip->model_num);

				if ( (wip->wi_flags & WIF_THRUSTER) && (wp->thruster_bitmap > -1) ) {
					float	ft;

					//	Add noise to thruster geometry.
					//ft = obj->phys_info.forward_thrust;					
					ft = 1.0f;		// Always use 1.0f for missiles					
					ft *= (1.0f + frand()/5.0f - 1.0f/10.0f);
					if (ft > 1.0f)
						ft = 1.0f;

					model_set_thrust( wip->model_num, ft, wp->thruster_bitmap, wp->thruster_glow_bitmap, wp->thruster_glow_noise);
					render_flags |= MR_SHOW_THRUSTERS;
				}

				model_render(wip->model_num, &obj->orient, &obj->pos, render_flags);

				// render a missile plume as well
				/*
				static int plume = -1;	
				extern float Interp_thrust_twist;
				extern float Interp_thrust_twist2;
				if(plume == -1){
					plume = model_load("plume01.pof", -1, NULL);
				}
				if(plume != -1){
					Interp_thrust_twist = tw;
					Interp_thrust_twist2 = tw2;
					model_set_alpha(plume_alpha);
					model_render(plume, &obj->orient, &obj->pos, MR_ALL_XPARENT);
					Interp_thrust_twist = -1.0f;
					Interp_thrust_twist2 = -1.0f;
				}
				*/
			}
			break;

		default:
			Warning(LOCATION, "Unknown weapon rendering type = %i\n", wip->render_type);
	}
}

void weapon_delete(object *obj)
{
	weapon *wp;
	int num;

	num = obj->instance;

	Assert( Weapons[num].objnum == OBJ_INDEX(obj));
	wp = &Weapons[num];

	Assert(wp->weapon_info_index >= 0);
	wp->weapon_info_index = -1;
	if (wp->swarm_index >= 0) {
		swarm_delete(wp->swarm_index);
		wp->swarm_index = -1;
	}

	if(wp->cscrew_index >= 0) {
		cscrew_delete(wp->cscrew_index);
		wp->cscrew_index = -1;
	}

	if (wp->missile_list_index >= 0) {
		missle_obj_list_remove(wp->missile_list_index);
		wp->missile_list_index = -1;
	}

	if (wp->flak_index >= 0){
		flak_delete(wp->flak_index);
		wp->flak_index = -1;
	}

	if (wp->trail_num > -1) {
		trail_object_died(wp->trail_num);
	}

	wp->objnum = -1;
	Num_weapons--;
	Assert(Num_weapons >= 0);
}

// Check if missile is newly locked onto the Player, maybe play a launch warning
void weapon_maybe_play_warning(weapon *wp)
{
	if ( wp->homing_object == Player_obj ) {
		if ( !(wp->weapon_flags & WF_LOCK_WARNING_PLAYED) ) {
			wp->weapon_flags |= WF_LOCK_WARNING_PLAYED;
			if ( Weapon_info[wp->weapon_info_index].wi_flags & WIF_HOMING_HEAT ) {
				snd_play(&Snds[SND_HEATLOCK_WARN]);
			} else {
				Assert(Weapon_info[wp->weapon_info_index].wi_flags & WIF_HOMING_ASPECT);
				snd_play(&Snds[SND_ASPECTLOCK_WARN]);
			}
		}
	}
}

#define	CMEASURE_DETONATE_DISTANCE		40.0f

//	Detonate all missiles near this countermeasure.
void detonate_nearby_missiles(cmeasure *cmp)
{
	missile_obj	*mop;
	vector		cmeasure_pos;

	cmeasure_pos = Objects[cmp->objnum].pos;

	mop = GET_FIRST(&Missile_obj_list);
	while(mop != END_OF_LIST(&Missile_obj_list)) {
		object	*objp;
		weapon	*wp;

		objp = &Objects[mop->objnum];
		wp = &Weapons[objp->instance];

		if (wp->team != cmp->team) {
			if ( Missiontime - wp->creation_time > F1_0/2) {
				if (vm_vec_dist_quick(&cmeasure_pos, &objp->pos) < CMEASURE_DETONATE_DISTANCE) {
					if (wp->lifeleft > 0.2f) { 
						//nprintf(("Jim", "Frame %i: Cmeasure #%i detonating weapon #%i\n", Framecount, cmp-Cmeasures, wp-Weapons));
						wp->lifeleft = 0.2f;
						// nprintf(("AI", "Frame %i: Flagging weapon %i for detonation.\n", Framecount, wp-Weapons));
					}
				}
			}
		}

		mop = mop->next;
	}
}

//	Find an object for weapon #num (object *weapon_objp) to home on due to heat.
void find_homing_object(object *weapon_objp, int num)
{
	object		*objp, *old_homing_objp;
	weapon_info	*wip;
	weapon		*wp;
	float			best_dist;

	wp = &Weapons[num];

	wip = &Weapon_info[Weapons[num].weapon_info_index];

	best_dist = 99999.9f;

	// save the old homing object so that multiplayer servers can give the right information
	// to clients if the object changes
	old_homing_objp = wp->homing_object;

	wp->homing_object = &obj_used_list;

	//	Scan all objects, find a weapon to home on.
	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if ((objp->type == OBJ_SHIP) || (objp->type == OBJ_CMEASURE)) {
			if (objp->type == OBJ_CMEASURE)
				if (Cmeasures[objp->instance].flags & CMF_DUD_HEAT)
					continue;

			int homing_object_team = obj_team(objp);
			if ( (homing_object_team != wp->team) || (homing_object_team == TEAM_TRAITOR) ) {
				float		dist;
				float		dot;
				vector	vec_to_object;

				// AL 2-17-98: If ship is immune to sensors, can't home on it (Sandeep says so)!
				if ( objp->type == OBJ_SHIP ) {
					if ( Ships[objp->instance].flags & SF_HIDDEN_FROM_SENSORS ) {
						continue;
					}

					//	MK, 9/4/99.
					//	If this is a player object, make sure there aren't already too many homers.
					//	Only in single player.  In multiplayer, we don't want to restrict it in dogfight on team vs. team.
					//	For co-op, it's probably also OK.
					if (!( Game_mode & GM_MULTIPLAYER )) {
						int	num_homers = compute_num_homing_objects(objp);
						if (Max_allowed_player_homers[Game_skill_level] < num_homers)
							continue;
					}
				}

				dist = vm_vec_normalized_dir(&vec_to_object, &objp->pos, &weapon_objp->pos);

				if (objp->type == OBJ_CMEASURE)
					dist *= 0.5f;

				dot = vm_vec_dot(&vec_to_object, &weapon_objp->orient.v.fvec);

				if (dot > wip->fov) {
					if (dist < best_dist) {
						best_dist = dist;
						wp->homing_object = objp;
						wp->target_sig = objp->signature;

						weapon_maybe_alert_cmeasure_success(objp);
					}
				}
			}
		}
	}

//	if (wp->homing_object->type == OBJ_CMEASURE)
//		nprintf(("AI", "Frame %i: Weapon #%i homing on cmeasure #%i\n", Framecount, num, objp-Objects));

	if (wp->homing_object == Player_obj)
		weapon_maybe_play_warning(wp);

	// if the old homing object is different that the new one, send a packet to clients
	if ( MULTIPLAYER_MASTER && (old_homing_objp != wp->homing_object) ) {
		send_homing_weapon_info( num );
	}
}

//	Scan all countermeasures.  Maybe make weapon_objp home on it.
void find_homing_object_cmeasures_1(object *weapon_objp)
{
	object	*objp;
	weapon	*wp;
	weapon_info	*wip;
	float		best_dot, dist, dot;

	wp = &Weapons[weapon_objp->instance];
	wip = &Weapon_info[wp->weapon_info_index];

	best_dot = wip->fov;			//	Note, setting to this avoids comparison below.

	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if (objp->type == OBJ_CMEASURE) {
			vector	vec_to_object;
			dist = vm_vec_normalized_dir(&vec_to_object, &objp->pos, &weapon_objp->pos);

			if (dist < MAX_CMEASURE_TRACK_DIST) {
				float	chance;
				if (wip->wi_flags & WIF_HOMING_ASPECT) {
					chance = 1.0f/2.0f;	//	aspect seeker this likely to chase a countermeasure
				} else {
					chance = 1.0f/1.5f;	//	heat seeker this likely to chase a countermeasure
				}
				if ((objp->signature != wp->cmeasure_ignore_objnum) && (objp->signature != wp->cmeasure_chase_objnum)) {
					if (frand() < chance) {
						wp->cmeasure_ignore_objnum = objp->signature;	//	Don't process this countermeasure again.
						//nprintf(("Jim", "Frame %i: Weapon #%i ignoring cmeasure #%i\n", Framecount, OBJ_INDEX(weapon_objp), objp->signature));
					} else  {
						wp->cmeasure_chase_objnum = objp->signature;	//	Don't process this countermeasure again.
						//nprintf(("Jim", "Frame %i: Weapon #%i CHASING cmeasure #%i\n", Framecount, OBJ_INDEX(weapon_objp), objp->signature));
					}
				}
				
				if (objp->signature != wp->cmeasure_ignore_objnum) {

					dot = vm_vec_dot(&vec_to_object, &weapon_objp->orient.v.fvec);

					if (dot > best_dot) {
						//nprintf(("Jim", "Frame %i: Weapon #%i homing on cmeasure #%i\n", Framecount, weapon_objp-Objects, objp->signature));
						best_dot = dot;
						wp->homing_object = objp;
						weapon_maybe_alert_cmeasure_success(objp);
					}
				}
			}
		}
	}
}


//	Someone launched countermeasures.
//	For all heat-seeking homing objects, see if should favor tracking a countermeasure instead.
void find_homing_object_cmeasures()
{
	object	*weapon_objp;

	// nprintf(("AI", "Scanning for countermeasures in frame %i\n", Framecount));

	if (Cmeasures_homing_check == 0)
		return;

	if (Cmeasures_homing_check <= 0)
		Cmeasures_homing_check = 1;

	Cmeasures_homing_check--;

	for (weapon_objp = GET_FIRST(&obj_used_list); weapon_objp != END_OF_LIST(&obj_used_list); weapon_objp = GET_NEXT(weapon_objp) ) {
		if (weapon_objp->type == OBJ_WEAPON) {
			weapon_info	*wip = &Weapon_info[Weapons[weapon_objp->instance].weapon_info_index];

			if (wip->wi_flags & WIF_HOMING)
				find_homing_object_cmeasures_1(weapon_objp);
		}
	}

}

//	Find object with signature "sig" and make weapon home on it.
void find_homing_object_by_sig(object *weapon_objp, int sig)
{
	ship_obj		*sop;
	weapon		*wp;
	object		*old_homing_objp;

	wp = &Weapons[weapon_objp->instance];

	// save the old object so that multiplayer masters know whether to send a homing update packet
	old_homing_objp = wp->homing_object;

	sop = GET_FIRST(&Ship_obj_list);
	while(sop != END_OF_LIST(&Ship_obj_list)) {
		object	*objp;

		objp = &Objects[sop->objnum];
		if (objp->signature == sig) {
			wp->homing_object = objp;
			wp->target_sig = objp->signature;
			break;
		}

		sop = sop->next;
	}

	// if the old homing object is different that the new one, send a packet to clients
	if ( MULTIPLAYER_MASTER && (old_homing_objp != wp->homing_object) ) {
		send_homing_weapon_info( weapon_objp->instance );
	}

}

//	Make weapon num home.  It's also object *obj.
void weapon_home(object *obj, int num, float frame_time)
{
	weapon		*wp;
	weapon_info	*wip;
	object		*hobjp;

	Assert(obj->type == OBJ_WEAPON);
	Assert(obj->instance == num);
	wp = &Weapons[num];
	wip = &Weapon_info[wp->weapon_info_index];
	hobjp = Weapons[num].homing_object;

	//	If not 1/2 second gone by, don't home yet.
	if ((hobjp == &obj_used_list) || ( f2fl(Missiontime - wp->creation_time) < 0.25f )) {
		//	If this is a heat seeking homing missile and 1/2 second has elapsed since firing
		//	and we don't have a target (else we wouldn't be inside the IF), find a new target.
		if (wip->wi_flags & WIF_HOMING_HEAT)
			if ( f2fl(Missiontime - wp->creation_time) > 0.5f )
				find_homing_object(obj, num);

		if (obj->phys_info.speed > wip->max_speed) {
			obj->phys_info.speed -= frame_time * 4;
			vm_vec_copy_scale( &obj->phys_info.desired_vel, &obj->orient.v.fvec, obj->phys_info.speed);
		} else if ((obj->phys_info.speed < wip->max_speed/4) && (wip->wi_flags & WIF_HOMING_HEAT)) {
			obj->phys_info.speed = wip->max_speed/4;
			vm_vec_copy_scale( &obj->phys_info.desired_vel, &obj->orient.v.fvec, obj->phys_info.speed);
		}

/*	Removed code that makes bombs drop for a bit.  They looked odd and it was confusing.  People wondered where their weapons went.
		//	Make bombs drop down for first second of life.
		if (wip->wi_flags & WIF_BOMB) {
			if (wip->lifetime - wp->lifeleft < 0.5f) {
				float	time_scale = wip->lifetime - wp->lifeleft;
				vm_vec_scale_add2(&obj->phys_info.desired_vel, &obj->orient.uvec, (time_scale - 0.5f) * max(10.0f, obj->phys_info.speed/2.0f));
			}
		}
*/
		return;
	}

	// AL 4-8-98: If orgiginal target for aspect lock missile is lost, stop homing
	if (wip->wi_flags & WIF_HOMING_ASPECT) {
		if ( wp->target_sig > 0 ) {
			if ( wp->homing_object->signature != wp->target_sig ) {
				wp->homing_object = &obj_used_list;
				return;
			}
		}
	}

  	// AL 4-13-98: Stop homing on a subsystem if parent ship has changed
	if (wip->wi_flags & WIF_HOMING_HEAT) {
		if ( wp->target_sig > 0 ) {
			if ( wp->homing_object->signature != wp->target_sig ) {
				wp->homing_subsys = NULL;
			}
		}
	}

/*
	if (hobjp->type == OBJ_NONE) {
		find_homing_object(obj, num);
		return;
	}
*/

	switch (hobjp->type) {
	case OBJ_NONE:
		if (wip->wi_flags & WIF_HOMING_ASPECT)
			find_homing_object_by_sig(obj, wp->target_sig);
		else
			find_homing_object(obj, num);
		return;
		break;
	case OBJ_SHIP:
		if (hobjp->signature != wp->target_sig) {
			if (wip->wi_flags & WIF_HOMING_ASPECT)
				find_homing_object_by_sig(obj, wp->target_sig);
			else
				find_homing_object(obj, num);
			return;
		}
		break;
	case OBJ_WEAPON:
		// only allowed to home on bombs
		Assert(Weapon_info[Weapons[hobjp->instance].weapon_info_index].wi_flags & WIF_BOMB);
		if (wip->wi_flags & WIF_HOMING_ASPECT)
			find_homing_object_by_sig(obj, wp->target_sig);
		else
			find_homing_object(obj, num);
		break;
	case OBJ_CMEASURE:
		break;
	default:
		return;
	}

	//	See if this weapon is the nearest homing object to the object it is homing on.
	//	If so, update some fields in the target object's ai_info.
	if (hobjp != &obj_used_list) {
		float	dist;

		dist = vm_vec_dist_quick(&obj->pos, &hobjp->pos);

		if (hobjp->type == OBJ_SHIP) {
			ai_info	*aip;

			aip = &Ai_info[Ships[hobjp->instance].ai_index];

			if ((aip->nearest_locked_object == -1) || (dist < aip->nearest_locked_distance)) {
				aip->nearest_locked_object = obj-Objects;
				aip->nearest_locked_distance = dist;
			}
		}
	}

	//	If the object it is homing on is still valid, home some more!
	if (hobjp != &obj_used_list) {
		float		old_dot, vel;
		vector	vec_to_goal;
		vector	target_pos;	// position of what the homing missile is seeking

		vm_vec_zero(&target_pos);

		// the homing missile may be seeking a subsystem on a ship.  If so, we need to calculate the
		// world coordinates of that subsystem so the homing missile can seek it out.
		//	For now, March 7, 1997, MK, heat seeking homing missiles will be able to home on
		//	any subsystem.  Probably makes sense for them to only home on certain kinds of subsystems.
		if ( wp->homing_subsys != NULL ) {
			get_subsystem_world_pos(hobjp, Weapons[num].homing_subsys, &target_pos);
			wp->homing_pos = target_pos;	// store the homing position in weapon data
			Assert( !vm_is_vec_nan(&wp->homing_pos) );
		} else {
			float	fov;
			float	dist;

			dist = vm_vec_dist_quick(&obj->pos, &hobjp->pos);
			if (hobjp->type == OBJ_CMEASURE) {
				if (dist < CMEASURE_DETONATE_DISTANCE) {
					cmeasure	*cmp;

					cmp = &Cmeasures[hobjp->instance];

					//	Make this missile detonate soon.  Not right away, not sure why.  Seems better.
					if (cmp->team != wp->team) {
						detonate_nearby_missiles(cmp);
						//nprintf(("AI", "Frame %i: Weapon %i hit cmeasure, will die!\n", Framecount, wp-Weapons));
						return;
					}
				}
			}

			fov = 0.8f;
			if (wip->fov > 0.8f)
				fov = wip->fov;

			int pick_homing_point = 0;
			if ( IS_VEC_NULL(&wp->homing_pos) ) {
				pick_homing_point = 1;
			}

			//	Update homing position if it hasn't been set, you're within 500 meters, or every half second, approximately.
			//	For large objects, don't lead them.
			if (hobjp->radius < 40.0f) {
				target_pos = hobjp->pos;
				wp->homing_pos = target_pos;
			} else if ( pick_homing_point || (dist < 500.0f) || (rand_chance(flFrametime, 2.0f)) ) {

				if (hobjp->type == OBJ_SHIP) {
					if ( !pick_homing_point ) {
						// ensure that current attack point is only updated in world coords (ie not pick a different vertex)
						wp->pick_big_attack_point_timestamp = 0;
					}

					if ( pick_homing_point ) {
						// If *any* player is parent of homing missile, then use position where lock indicator is
						if ( Objects[obj->parent].flags & OF_PLAYER_SHIP ) {
							player *pp;

							// determine the player
							pp = Player;
							if ( Game_mode & GM_MULTIPLAYER ) {
								int pnum;

								pnum = multi_find_player_by_object( &Objects[obj->parent] );
								if ( pnum != -1 ){
									pp = Net_players[pnum].player;
								}
							}

							// If player has apect lock, we don't want to find a homing point on the closest
							// octant... setting the timestamp to 0 ensures this.
							if (wip->wi_flags & WIF_HOMING_ASPECT) {
								wp->pick_big_attack_point_timestamp = 0;
							} else {
								wp->pick_big_attack_point_timestamp = 1;
							}

							if ( pp && pp->locking_subsys ) {
								wp->big_attack_point = pp->locking_subsys->system_info->pnt;
							} else {
								vm_vec_zero(&wp->big_attack_point);
							}
						}
					}

					ai_big_pick_attack_point(hobjp, obj, &target_pos, fov);

				} else {
					target_pos = hobjp->pos;
				}

				wp->homing_pos = target_pos;
				Assert( !vm_is_vec_nan(&wp->homing_pos) );
				// nprintf(("AI", "Attack point = %7.3f %7.3f %7.3f\n", target_pos.xyz.x, target_pos.xyz.y, target_pos.xyz.z));
			} else
				target_pos = wp->homing_pos;
		}

		//	Couldn't find a lock.
		if (IS_VEC_NULL(&target_pos))
			return;

		//	Cause aspect seeking weapon to home at target's predicted position.
		//	But don't use predicted position if dot product small or negative.
		//	If do this, with a ship headed towards missile, could choose a point behind missile.``1
		float	dist_to_target, time_to_target;
		
		dist_to_target = vm_vec_normalized_dir(&vec_to_goal, &target_pos, &obj->pos);
		time_to_target = dist_to_target/wip->max_speed;

		vector	tvec;
		tvec = obj->phys_info.vel;
		vm_vec_normalize(&tvec);

		old_dot = vm_vec_dot(&tvec, &vec_to_goal);

		//	If a weapon has missed its target, detonate it.
		//	This solves the problem of a weapon circling the center of a subsystem that has been blown away.
		//	Problem: It does not do impact damage, just proximity damage.
		if ((dist_to_target < flFrametime * obj->phys_info.speed * 4.0f + 10.0f) && (old_dot < 0.0f)) {
			int kill_missile = TRUE;
			if (wp->homing_object) {
				if (wp->homing_object->type == OBJ_SHIP) {
					ship *shipp = &Ships[wp->homing_object->instance];
					if (Ship_info[shipp->ship_info_index].flags & SIF_DONT_COLLIDE_INVIS) {
						kill_missile = FALSE;
					}
				}
			}
			
			if (kill_missile && (wp->lifeleft > 0.01f)) {
				wp->lifeleft = 0.01f;
			}
		}

		//	Only lead target if more than one second away.  Otherwise can miss target.  I think this
		//	is what's causing Harbingers to miss the super destroyer. -- MK, 4/15/98
		if ((wip->wi_flags & WIF_HOMING_ASPECT) && (old_dot > 0.1f) && (time_to_target > 0.1f))
			vm_vec_scale_add2(&target_pos, &hobjp->phys_info.vel, min(time_to_target, 2.0f));

		//nprintf(("AI", "Dot = %7.3f, dist = %7.3f, time_to = %6.3f, deg/sec = %7.3f\n", old_dot, dist_to_target, time_to_target, angles/flFrametime));

		// nprintf(("AI", "Weapon %i, lifeleft = %7.3f, dist = %7.3f, dot = %6.3f\n", num, Weapons[num].lifeleft, vm_vec_dist_quick(&obj->pos, &Weapons[num].homing_object->pos), old_dot));

		//	If a HEAT seeking (rather than ASPECT seeking) homing missile, verify that target is in viewcone.
		if (wip->wi_flags & WIF_HOMING_HEAT) {
			if ((old_dot < wip->fov) && (dist_to_target > wip->inner_radius*1.1f)) {	//	Delay finding new target one frame to allow detonation.
				find_homing_object(obj, num);
				return;			//	Maybe found a new homing object.  Return, process more next frame.
			} else	//	Subtract out life based on how far from target this missile points.
				if (wip->fov < 0.95f) {
					wp->lifeleft -= flFrametime * (0.95f - old_dot);
					//Should only happen when time is compressed.
					//if (flFrametime * (1.0f - old_dot) > 1.0f)
					//	Int3();
				}
		} else if (wip->wi_flags & WIF_HOMING_ASPECT) {	//	subtract life as if max turn is 90 degrees.
			if (wip->fov < 0.95f)
				wp->lifeleft -= flFrametime * (0.95f - old_dot);
		} else
			Assert(0);	//	Hmm, a homing missile, but not aspect or heat?


		//	Control speed based on dot product to goal.  If close to straight ahead, move
		//	at max speed, else move slower based on how far from ahead.
		if (old_dot < 0.90f) {
			obj->phys_info.speed = max(0.2f, old_dot* (float) fabs(old_dot));
			if (obj->phys_info.speed < wip->max_speed*0.75f)
				obj->phys_info.speed = wip->max_speed*0.75f;
		} else
			obj->phys_info.speed = wip->max_speed;

		//	For first second of weapon's life, it doesn't fly at top speed.  It ramps up.
		if (Missiontime - wp->creation_time < i2f(1)) {
			float	t;

			t = f2fl(Missiontime - wp->creation_time);
			obj->phys_info.speed *= t*t;
		}

		Assert( obj->phys_info.speed > 0.0f );

		vm_vec_copy_scale( &obj->phys_info.desired_vel, &obj->orient.v.fvec, obj->phys_info.speed);

		// turn the missile towards the target only if non-swarm.  Homing swarm missiles choose
		// a different vector to turn towards, this is done in swarm_update_direction().
//		if ( !(wip->wi_flags & WIF_SWARM) ) {
		if ( wp->swarm_index < 0 ) {
			// nprintf(("AI", "Dot, dist = %7.3f, %7.3f, target pos = %7.3f %7.3f %7.3f\n", old_dot, vm_vec_dist_quick(&obj->pos, &target_pos), target_pos.xyz.x, target_pos.xyz.y, target_pos.xyz.z));
			ai_turn_towards_vector(&target_pos, obj, frame_time, wip->turn_time, NULL, NULL, 0.0f, 0, NULL);
			vel = vm_vec_mag(&obj->phys_info.desired_vel);

			vm_vec_copy_scale(&obj->phys_info.desired_vel, &obj->orient.v.fvec, vel);

		}

/*		//	If this weapon shot past its target, make it detonate.
		if ((old_dot < 0.0f) && (dist_to_target < 50.0f)) {
			if (wp->lifeleft > 0.01f)
				wp->lifeleft = 0.01f;
		}
*/	}
}

// as Mike K did with ships -- break weapon into process_pre and process_post for code to execute
// before and after physics movement

void weapon_process_pre( object *obj, float frame_time)
{
	// if the object is a corkscrew style weapon, process it now
	if((obj->type == OBJ_WEAPON) && (Weapons[obj->instance].cscrew_index >= 0)){
		cscrew_process_pre(obj);
	}

	// if the weapon is a flak weapon, maybe detonate it early
	if((obj->type == OBJ_WEAPON) && (Weapon_info[Weapons[obj->instance].weapon_info_index].wi_flags & WIF_FLAK) && (Weapons[obj->instance].flak_index >= 0)){
		flak_maybe_detonate(obj);		
	}
}

int	Homing_hits = 0, Homing_misses = 0;


MONITOR( NumWeapons );	

// maybe play a "whizz sound" if close enough to view position
void weapon_maybe_play_flyby_sound(object *weapon_objp, weapon *wp)
{	
	// do a quick out if not a laser
	if ( Weapon_info[wp->weapon_info_index].subtype != WP_LASER ) {
		return;
	}

	// don't play flyby sounds too close together
	if ( !timestamp_elapsed(Weapon_flyby_sound_timer) ) {
		return;
	}

	if ( !(wp->weapon_flags & WF_PLAYED_FLYBY_SOUND) && (wp->weapon_flags & WF_CONSIDER_FOR_FLYBY_SOUND) ) {
		float		dist, dot, radius;

		dist = vm_vec_dist_quick(&weapon_objp->pos, &Eye_position);

		if ( Viewer_obj ) {
			radius = Viewer_obj->radius;
		} else {
			radius = 0.0f;
		}

		if ( (dist > radius) && (dist < 55) ) {
			vector	vec_to_weapon;

			vm_vec_sub(&vec_to_weapon, &weapon_objp->pos, &Eye_position);
			vm_vec_normalize(&vec_to_weapon);

			// ensure laser is in front of eye
			dot = vm_vec_dot(&vec_to_weapon, &Eye_matrix.v.fvec);
			if ( dot < 0.1 ) {
				return;
			}

			// ensure that laser is moving in similar direction to fvec
			dot = vm_vec_dot(&vec_to_weapon, &weapon_objp->orient.v.fvec);
			
//			nprintf(("Alan", "Weapon dot: %.2f\n", dot));
			if ( (dot < -0.80) && (dot > -0.98) ) {
				snd_play_3d( &Snds[SND_WEAPON_FLYBY], &weapon_objp->pos, &Eye_position );
				Weapon_flyby_sound_timer = timestamp(200);
				wp->weapon_flags |= WF_PLAYED_FLYBY_SOUND;
			}
		}
	}
}

// process a weapon after physics movement.  MWA reorders some of the code on 8/13 for multiplayer.  When
// adding something to this function, decide whether or not a client in a multiplayer game needs to do
// what is normally done in a single player game.  Things like plotting an object on a radar, effect
// for exhaust are things that are done on all machines.  Things which calculate weapon targets, new
// velocities, etc, are server only functions and should go after the if ( !MULTIPLAYER_MASTER ) statement
// See Allender if you cannot decide what to do.
void weapon_process_post(object * obj, float frame_time)
{
	int			num;	
	weapon_info	*wip;
	weapon		*wp;

	MONITOR_INC( NumWeapons, 1 );	
	
	Assert(obj->type == OBJ_WEAPON);

	num = obj->instance;

#ifndef NDEBUG
	int objnum = OBJ_INDEX(obj);
	Assert( Weapons[num].objnum == objnum );
#endif

	wp = &Weapons[num];

	wp->lifeleft -= frame_time;
	wip = &Weapon_info[wp->weapon_info_index];

	// check life left.  Multiplayer client code will go through here as well.  We must be careful in weapon_hit
	// when killing a missile that spawn child weapons!!!!
	if ( wp->lifeleft < 0.0f ) {
		if ( wip->subtype & WP_MISSILE ) {
			if(Game_mode & GM_MULTIPLAYER){				
				if ( !MULTIPLAYER_CLIENT || (MULTIPLAYER_CLIENT && (wp->lifeleft < -2.0f)) || (MULTIPLAYER_CLIENT && (wip->wi_flags & WIF_CHILD))) {					// don't call this function multiplayer client -- host will send this packet to us
					// nprintf(("AI", "Frame %i: Weapon %i detonated, dist = %7.3f!\n", Framecount, obj-Objects));
					weapon_detonate(obj);					
				}
			} else {
				// nprintf(("AI", "Frame %i: Weapon %i detonated, dist = %7.3f!\n", Framecount, obj-Objects));
				weapon_detonate(obj);									
			}
			if (wip->wi_flags & WIF_HOMING) {
				Homing_misses++;
				// nprintf(("AI", "Miss!  Hits = %i/%i\n", Homing_hits, (Homing_hits + Homing_misses)));
			}
		} else {
			obj->flags |= OF_SHOULD_BE_DEAD;
//			demo_do_flag_dead(OBJ_INDEX(obj));
		}

		return;
	}

	// plot homing missiles on the radar
	if (wip->wi_flags & WIF_HOMING) {
		if ( hud_gauge_active(HUD_RADAR) ) {
			radar_plot_object( obj );
		}
	}

	// trail missiles
	if ((wip->wi_flags & WIF_TRAIL) && !(wip->wi_flags & WIF_CORKSCREW)) {
		if ( wp->trail_num > -1 )	{
			if (trail_stamp_elapsed(wp->trail_num)) {

				trail_add_segment( wp->trail_num, &obj->pos );
				
				trail_set_stamp(wp->trail_num);
			} else {
				trail_set_segment( wp->trail_num, &obj->pos );
			}

		}
	}

	if ( wip->wi_flags & WIF_THRUSTER )	{
		ship_do_weapon_thruster_frame( wp, obj, flFrametime );	
	}

	// maybe play a "whizz sound" if close enough to view position
	#ifndef NDEBUG
	if ( Weapon_flyby_sound_enabled ) {
		weapon_maybe_play_flyby_sound(obj, wp);
	}
	#else
		weapon_maybe_play_flyby_sound(obj, wp);
	#endif	
	
	//	If our target is still valid, then update some info.
	if (wp->target_num != -1) {
		if (Objects[wp->target_num].signature == wp->target_sig) {
			float		cur_dist;
			vector	v0;

			vm_vec_avg(&v0, &obj->pos, &obj->last_pos);

			cur_dist = vm_vec_dist_quick(&v0, &Objects[wp->target_num].pos);

			if (cur_dist < wp->nearest_dist) {
				wp->nearest_dist = cur_dist;
			} else if (cur_dist > wp->nearest_dist + 1.0f) {
				float		dot;
				vector	tvec;
				ai_info	*parent_aip;
				float		lead_scale = 0.0f;

				parent_aip = NULL;
				if (obj->parent != Player_obj-Objects) {
					parent_aip = &Ai_info[Ships[Objects[obj->parent].instance].ai_index];
					lead_scale = parent_aip->lead_scale;
				}

				vm_vec_normalized_dir(&tvec, &v0, &Objects[wp->target_num].pos);
				dot = vm_vec_dot(&tvec, &Objects[wp->target_num].orient.v.fvec);
				// nprintf(("AI", "Miss dot = %7.3f, dist = %7.3f, lead_scale = %7.3f\n", dot, cur_dist, lead_scale));
				wp->target_num = -1;

				//	Learn!  If over-shooting or under-shooting, compensate.
				//	Really need to compensate for left/right errors.  This does no good against someone circling
				//	in a plane perpendicular to the attacker's forward vector.
				if (parent_aip != NULL) {
					if (cur_dist > 100.0f)
						parent_aip->lead_scale = 0.0f;

					if (dot < -0.1f){
						parent_aip->lead_scale += cur_dist/2000.0f;
					} else if (dot > 0.1f) {
						parent_aip->lead_scale -= cur_dist/2000.0f;
					}
					
					if (fl_abs(parent_aip->lead_scale) > 1.0f){
						parent_aip->lead_scale *= 0.9f;
					}
				}
			}
		}
	}

	if(wip->wi_flags & WIF_PARTICLE_SPEW){
		weapon_maybe_spew_particle(obj);
	}

	// a single player or multiplayer server function -- it affects actual weapon movement.
	if (wip->wi_flags & WIF_HOMING) {
		weapon_home(obj, num, frame_time);

/*		if (wip->wi_flags & WIF_BOMB) {
			if (wip->lifetime - obj->lifeleft < 1.0f) {
				
			}
		}
*/		
		// If this is a swarm type missile, 
//		if ( wip->wi_flags & WIF_SWARM ) 
		if ( wp->swarm_index >= 0 ) {
			swarm_update_direction(obj, frame_time);
		}

		if( wp->cscrew_index >= 0) {
			cscrew_process_post(obj);			
		}
	}
}

//	Update weapon tracking information.
void weapon_set_tracking_info(int weapon_objnum, int parent_objnum, int target_objnum, int target_is_locked, ship_subsys *target_subsys)
{
	int			ai_index;
	object		*parent_objp;
	weapon		*wp;
	weapon_info	*wip;
	int targeting_same = 0;

	if ( weapon_objnum < 0 ) {
		return;
	}

	Assert(Objects[weapon_objnum].type == OBJ_WEAPON);

	wp = &Weapons[Objects[weapon_objnum].instance];
	wip = &Weapon_info[wp->weapon_info_index];
	parent_objp = &Objects[parent_objnum];

	Assert(parent_objp->type == OBJ_SHIP);
	ai_index = Ships[parent_objp->instance].ai_index;

	if ( ai_index >= 0 ) {
		int target_team = -1;
		if ( target_objnum >= 0 ) {
			int obj_type = Objects[target_objnum].type;
			if ( (obj_type == OBJ_SHIP) || (obj_type == OBJ_WEAPON) ) {
				target_team = obj_team(&Objects[target_objnum]);
			}
		}
	
		// determining if we're targeting the same team
		if(Ships[parent_objp->instance].team == target_team){
			targeting_same = 1;
		} else {
			targeting_same = 0;
		}

		if ((target_objnum != -1) && (!targeting_same || ((Game_mode & GM_MULTIPLAYER) && (Netgame.type_flags & NG_TYPE_DOGFIGHT) && (target_team == TEAM_TRAITOR))) ) {
			wp->target_num = target_objnum;
			wp->target_sig = Objects[target_objnum].signature;
			wp->nearest_dist = 99999.0f;
			if ( (wip->wi_flags & WIF_HOMING_ASPECT) && target_is_locked) {
				wp->homing_object = &Objects[target_objnum];
				wp->homing_subsys = target_subsys;
				weapon_maybe_play_warning(wp);
			} else if ( wip->wi_flags & WIF_HOMING_HEAT ) {
				//	Make a heat seeking missile try to home.  If the target is outside the view cone, it will
				//	immediately drop it and try to find one in its view cone.
				if (target_objnum != -1) {
					wp->homing_object = &Objects[target_objnum];
					weapon_maybe_play_warning(wp);
				} else
					wp->homing_object = &obj_used_list;

				wp->homing_subsys = target_subsys;
			}
		} else {
			wp->target_num = -1;
			wp->target_sig = -1;
		}

		//	If missile is locked on target, increase its lifetime by 20% since missiles can be fired at limit of range
		//	as defined by velocity*lifeleft, but missiles often slow down a bit, plus can be fired at a moving away target.
		//	Confusing to many players when their missiles run out of gas before getting to target.	
		// DB - removed 7:14 pm 9/6/99. was totally messing up lifetimes for all weapons.
		//	MK, 7:11 am, 9/7/99.  Put it back in, but with a proper check here to make sure it's an aspect seeker and
		//	put a sanity check in the color changing laser code that was broken by this code.
		if (target_is_locked && (wp->target_num != -1) && (wip->wi_flags & WIF_HOMING_ASPECT) ) {
			wp->lifeleft *= 1.2f;
		}

		ai_update_danger_weapon(target_objnum, weapon_objnum);		
	}
}


// weapon_create() will create a weapon object
//
// Returns:  index of weapon in the Objects[] array, -1 if the weapon object was not created
int Weapons_created = 0;
int weapon_create( vector * pos, matrix * orient, int weapon_id, int parent_objnum, int secondary_flag, int group_id, int is_locked )
{
	int			n, objnum;
	int num_deleted;
	object		*objp, *parent_objp;
	weapon		*wp;
	weapon_info	*wip;

	Assert(weapon_id >= 0 && weapon_id < Num_weapon_types);

	// beam weapons should never come through here!
	Assert(!(Weapon_info[weapon_id].wi_flags & WIF_BEAM));

	num_deleted = 0;
	if (Num_weapons >= MAX_WEAPONS-5) {

		//No, do remove for AI ships -- MK, 3/12/98  // don't need to try and delete weapons for ai ships
		//if ( !(Objects[parent_objnum].flags & OF_PLAYER_SHIP) )
		//	return -1;

		num_deleted = collide_remove_weapons();
		nprintf(("WARNING", "Deleted %d weapons because of lack of slots\n", num_deleted));
		if (num_deleted == 0){
			return -1;
		}
	}

	for (n=0; n<MAX_WEAPONS; n++ ){
		if (Weapons[n].weapon_info_index < 0){
			break;
		}
	}

	if (n == MAX_WEAPONS) {
		// if we supposedly deleted weapons above, what happened here!!!!
		if (num_deleted){
			Int3();				// get allender -- something funny is going on!!!
		}

		return -1;
	}

	Weapons_created++;
	objnum = obj_create( OBJ_WEAPON, parent_objnum, n, orient, pos, 2.0f, OF_RENDERS | OF_COLLIDES | OF_PHYSICS );
	Assert(objnum >= 0);
	Assert(First_secondary_index != -1);
	objp = &Objects[objnum];

	parent_objp = NULL;
	if(parent_objnum >= 0){
		parent_objp = &Objects[parent_objnum];
	}

	// Create laser n!
	wp = &Weapons[n];
	wip = &Weapon_info[weapon_id];

	// check if laser or dumbfire missile
	// set physics flag to allow optimization
	if ((wip->subtype == WP_LASER) || ((wip->subtype == WP_MISSILE) && !(wip->wi_flags & WIF_HOMING))) {
		// set physics flag
		objp->phys_info.flags |= PF_CONST_VEL;
	}

	wp->weapon_info_index = weapon_id;
	wp->lifeleft = wip->lifetime;

	wp->objnum = objnum;
	wp->homing_object = &obj_used_list;		//	Assume not homing on anything.
	wp->homing_subsys = NULL;
	wp->creation_time = Missiontime;
	wp->group_id = group_id;

	// we don't necessarily need a parent
	if(parent_objp != NULL){
		Assert(parent_objp->type == OBJ_SHIP);	//	Get Mike, a non-ship has fired a weapon!
		Assert((parent_objp->instance >= 0) && (parent_objp->instance < MAX_SHIPS));
		wp->team = Ships[parent_objp->instance].team;
		wp->species = Ship_info[Ships[parent_objp->instance].ship_info_index].species;
	} else {
		wp->team = 0;
		wp->species = 0;
	}
	wp->turret_subsys = NULL;
	vm_vec_zero(&wp->homing_pos);
	wp->weapon_flags = 0;
	wp->target_sig = -1;
	wp->cmeasure_ignore_objnum = -1;
	wp->cmeasure_chase_objnum = -1;

	// Init the thruster info
	wp->thruster_bitmap = -1;
	wp->thruster_frame = 0.0f;
	wp->thruster_glow_bitmap = -1;
	wp->thruster_glow_noise = 1.0f;
	wp->thruster_glow_frame = 0.0f;

	if ( wip->wi_flags & WIF_SWARM ) {
		wp->swarm_index = (short)swarm_create();
	} else {
		wp->swarm_index = -1;
	}		

	// if this is a particle spewing weapon, setup some stuff
	if(wip->wi_flags & WIF_PARTICLE_SPEW){
		wp->particle_spew_time = -1;		
	}

	// assign the network signature.  The starting sig is sent to all clients, so this call should
	// result in the same net signature numbers getting assigned to every player in the game
	if ( Game_mode & GM_MULTIPLAYER ) {
		if(wip->subtype == WP_MISSILE){
			Objects[objnum].net_signature = multi_assign_network_signature( MULTI_SIG_NON_PERMANENT );

			// for weapons that respawn, add the number of respawnable weapons to the net signature pool
			// to reserve N signatures for the spawned weapons
			if ( wip->wi_flags & WIF_SPAWN ){
				multi_set_network_signature( (ushort)(Objects[objnum].net_signature + wip->spawn_count), MULTI_SIG_NON_PERMANENT );
			}
		} else {
			Objects[objnum].net_signature = multi_assign_network_signature( MULTI_SIG_NON_PERMANENT );
		}
		// for multiplayer clients, when creating lasers, add some more life to the lasers.  This helps
		// to overcome some problems associated with lasers dying on client machine before they get message
		// from server saying it hit something.
		// removed 1/13/98 -- MWA if ( MULTIPLAYER_CLIENT && (wip->subtype == WP_LASER) )
		//	removed 1/13/98 -- MWA	wp->lifeleft += 1.5f;
	}

	//	Make remote detonate missiles look like they're getting detonated by firer simply by giving them variable lifetimes.
	if (!(Objects[parent_objnum].flags & OF_PLAYER_SHIP) && (wip->wi_flags & WIF_REMOTE)) {
		float rand_val;

		if ( Game_mode & GM_NORMAL ){
			rand_val = frand();
		} else {
			rand_val = static_randf(Objects[objnum].net_signature);
		}

		wp->lifeleft = wp->lifeleft/2.0f + rand_val * wp->lifeleft/2.0f;
	}

	objp->phys_info.mass = wip->mass;
	objp->phys_info.side_slip_time_const = 0.0f;
	objp->phys_info.rotdamp = 0.0f;
	vm_vec_zero(&objp->phys_info.max_vel);
	objp->phys_info.max_vel.xyz.z = wip->max_speed;
	vm_vec_zero(&objp->phys_info.max_rotvel);
	objp->shields[0] = wip->damage;
	if (wip->wi_flags & WIF_BOMB){
		objp->hull_strength = 50.0f;
	} else {
		objp->hull_strength = 0.0f;
	}

	if ( wip->subtype == WP_MISSILE ){
		objp->radius = model_get_radius(wip->model_num);
	} else if ( wip->subtype == WP_LASER ) {
		objp->radius = wip->laser_head_radius;
	}

	//	Set desired velocity and initial velocity.
	//	For lasers, velocity is always the same.
	//	For missiles, it is a small amount plus the firing ship's velocity.
	//	For missiles, the velocity trends towards some goal.
	//	Note: If you change how speed works here, such as adding in speed of parent ship, you'll need to change the AI code
	//	that predicts collision points.  See Mike Kulas or Dave Andsager.  (Or see ai_get_weapon_speed().)
	if (!(wip->wi_flags & WIF_HOMING)) {
		vm_vec_copy_scale(&objp->phys_info.desired_vel, &objp->orient.v.fvec, objp->phys_info.max_vel.xyz.z );
		objp->phys_info.vel = objp->phys_info.desired_vel;
		objp->phys_info.speed = vm_vec_mag(&objp->phys_info.desired_vel);
	} else {		
		//	For weapons that home, set velocity to sum of forward component of parent's velocity and 1/4 weapon's max speed.
		//	Note that it is important to extract the forward component of the parent's velocity to factor out sliding, else
		//	the missile will not be moving forward.
		if(parent_objp != NULL){
			vm_vec_copy_scale(&objp->phys_info.desired_vel, &objp->orient.v.fvec, vm_vec_dot(&parent_objp->phys_info.vel, &parent_objp->orient.v.fvec) + objp->phys_info.max_vel.xyz.z/4 );
		} else {
			vm_vec_copy_scale(&objp->phys_info.desired_vel, &objp->orient.v.fvec, objp->phys_info.max_vel.xyz.z/4 );
		}
		objp->phys_info.vel = objp->phys_info.desired_vel;
		objp->phys_info.speed = vm_vec_mag(&objp->phys_info.vel);
	}

	// create the corkscrew
	if ( wip->wi_flags & WIF_CORKSCREW ) {
		wp->cscrew_index = (short)cscrew_create(objp);
	} else {
		wp->cscrew_index = -1;
	}

	// if this is a flak weapon shell, make it so
	// NOTE : this function will change some fundamental things about the weapon object
	if ( wip->wi_flags & WIF_FLAK ){
		flak_create(wp);
	} else {
		wp->flak_index = -1;
	}	

	wp->missile_list_index = -1;
	// If this is a missile, then add it to the Missile_obj_list
	if ( wip->subtype == WP_MISSILE ) {
		wp->missile_list_index = missile_obj_list_add(objnum);
	}

	if (wip->wi_flags & WIF_TRAIL /*&& !(wip->wi_flags & WIF_CORKSCREW) */) {
		wp->trail_num = trail_create(wip->tr_info);		

		if ( wp->trail_num > -1 )	{
			// Add two segments.  One to stay at launch pos, one to move.
			trail_add_segment( wp->trail_num, &objp->pos );
			trail_add_segment( wp->trail_num, &objp->pos );
		}
	}

	// Ensure weapon flyby sound doesn't get played for player lasers
	if ( parent_objp == Player_obj ) {
		wp->weapon_flags |= WF_PLAYED_FLYBY_SOUND;
	}

	wp->pick_big_attack_point_timestamp = timestamp(1);

	//	Set detail levels for POF-type weapons.
	if (Weapon_info[wp->weapon_info_index].model_num != -1) {
		polymodel * pm;
		int	i;
		pm = model_get(Weapon_info[wp->weapon_info_index].model_num);

		for (i=0; i<pm->n_detail_levels; i++){
			pm->detail_depth[i] = (objp->radius*20.0f + 20.0f) * i;
		}
	}

		// if the weapon was fired locked
	if(is_locked){
		wp->weapon_flags |= WF_LOCKED_WHEN_FIRED;
	}

	Num_weapons++;
	return objnum;
}

//	Spawn child weapons from object *objp.
void spawn_child_weapons(object *objp)
{
	int	i;
	int	child_id;
	int	parent_num;
	ushort starting_sig;
	weapon	*wp;
	weapon_info	*wip;

	Assert(objp->type == OBJ_WEAPON);
	Assert((objp->instance >= 0) && (objp->instance < MAX_WEAPONS));

	wp = &Weapons[objp->instance];
	Assert((wp->weapon_info_index >= 0) && (wp->weapon_info_index < MAX_WEAPON_TYPES));
	wip = &Weapon_info[wp->weapon_info_index];

	child_id = wip->spawn_type;

	parent_num = objp->parent;

	if ((Objects[parent_num].type != objp->parent_type) || (Objects[parent_num].signature != objp->parent_sig)) {
		mprintf(("Warning: Parent of spawn weapon does not exist.  Not spawning.\n"));
		return;
	}

	starting_sig = 0;
	if ( Game_mode & GM_MULTIPLAYER ) {		
		// get the next network signature and save it.  Set the next usable network signature to be
		// the passed in objects signature + 1.  We "reserved" N of these slots when we created objp
		// for it's spawned children.
		starting_sig = multi_get_next_network_signature( MULTI_SIG_NON_PERMANENT );
		multi_set_network_signature( objp->net_signature, MULTI_SIG_NON_PERMANENT );
	}

	for (i=0; i<wip->spawn_count; i++) {
		int		weapon_objnum;
		vector	tvec, pos;
		matrix	orient;

		// for multiplayer, use the static randvec functions based on the network signatures to provide
		// the randomness so that it is the same on all machines.
		if ( Game_mode & GM_MULTIPLAYER ){
			static_randvec(objp->net_signature + i, &tvec);
		} else {
			vm_vec_rand_vec_quick(&tvec);
		}
		vm_vec_scale_add(&pos, &objp->pos, &tvec, objp->radius);

		vm_vector_2_matrix(&orient, &tvec, NULL, NULL);
		weapon_objnum = weapon_create(&pos, &orient, child_id, parent_num, 1, -1, wp->weapon_flags & WF_LOCKED_WHEN_FIRED);

		//	Assign a little randomness to lifeleft so they don't all disappear at the same time.
		if (weapon_objnum != -1) {
			float rand_val;

			if ( Game_mode & GM_NORMAL ){
				rand_val = frand();
			} else {
				rand_val = static_randf(objp->net_signature + i);
			}

			Weapons[Objects[weapon_objnum].instance].lifeleft *= rand_val*0.4f + 0.8f;
		}

	}

	// in multiplayer, reset the next network signature to the one that was saved.
	if ( Game_mode & GM_MULTIPLAYER ){
		multi_set_network_signature( starting_sig, MULTI_SIG_NON_PERMANENT );
	}
}

// -----------------------------------------------------------------------
// weapon_hit_do_sound()
//
// Play a sound effect when a weapon hits a ship
//
// To elimate the "stereo" effect of two lasers hitting at nearly
// the same time, and to reduce the number of sound channels used,
// only play one impact sound if IMPACT_SOUND_DELTA has elapsed
//
// Note: Uses Weapon_impact_timer global for timer variable
//
void weapon_hit_do_sound(object *hit_obj, weapon_info *wip, vector *hitpos)
{
	int	is_hull_hit;
	float shield_str;

	// If non-missiles (namely lasers) expire without hitting a ship, don't play impact sound
	if	( wip->subtype != WP_MISSILE ) {		
		if ( !hit_obj ) {
			// flak weapons make sounds		
			if(wip->wi_flags & WIF_FLAK){
				snd_play_3d( &Snds[wip->impact_snd], hitpos, &Eye_position );				
			}
			return;
		}

		switch(hit_obj->type) {
		case OBJ_SHIP:
			// do nothing
			break;

		case OBJ_ASTEROID:
			if ( timestamp_elapsed(Weapon_impact_timer) ) {
				snd_play_3d( &Snds[wip->impact_snd], hitpos, &Eye_position );
				Weapon_impact_timer = timestamp(IMPACT_SOUND_DELTA);
			}
			return;
			break;

		default:
			return;
		}
	}

	if ( hit_obj == NULL ) {
		snd_play_3d( &Snds[wip->impact_snd], hitpos, &Eye_position );
		return;
	}

	if ( timestamp_elapsed(Weapon_impact_timer) ) {

		is_hull_hit = 1;
		if ( hit_obj->type == OBJ_SHIP ) {
			shield_str = ship_quadrant_shield_strength(hit_obj, hitpos);
		} else {
			shield_str = 0.0f;
		}

		// play a shield hit if shields are above 10% max in this quadrant
		if ( shield_str > 0.1f ) {
			is_hull_hit = 0;
		}

		if ( !is_hull_hit ) {
			// Play a shield impact sound effect
			if ( hit_obj == Player_obj ) {
				snd_play_3d( &Snds[SND_SHIELD_HIT_YOU], hitpos, &Eye_position );
				// AL 12-15-97: Add missile impact sound even when shield is hit
				if ( wip->subtype == WP_MISSILE ) {
					snd_play_3d( &Snds[SND_PLAYER_HIT_MISSILE], hitpos, &Eye_position);
				}
			} else {
				snd_play_3d( &Snds[SND_SHIELD_HIT], hitpos, &Eye_position );
			}
		} else {
			// Play a hull impact sound effect
			switch ( wip->subtype ) {
				case WP_LASER:
					if ( hit_obj == Player_obj )
						snd_play_3d( &Snds[SND_PLAYER_HIT_LASER], hitpos, &Eye_position );
					else {
						if ( wip->impact_snd != -1 ) {
							snd_play_3d( &Snds[wip->impact_snd], hitpos, &Eye_position );
						}
					}
					break;
				case WP_MISSILE:
					if ( hit_obj == Player_obj ) 
						snd_play_3d( &Snds[SND_PLAYER_HIT_MISSILE], hitpos, &Eye_position);
					else {
						if ( wip->impact_snd != -1 ) {
							snd_play_3d( &Snds[wip->impact_snd], hitpos, &Eye_position );
						}
					}
					break;
				default:	
					nprintf(("Warning","WARNING ==> Cannot determine sound to play for weapon impact\n"));
					break;
			} // end switch
		}

		Weapon_impact_timer = timestamp(IMPACT_SOUND_DELTA);
	}
}

// distrupt any subsystems that fall into damage sphere of this Electronics missile
//
// input:	ship_obj		=>		pointer to ship that holds subsystem
//				blast_pos	=>		world pos of weapon blast
//				wi_index		=>		weapon info index of weapon causing blast
void weapon_do_electronics_affect(object *ship_objp, vector *blast_pos, int wi_index)
{
	weapon_info			*wip;
	ship					*shipp;
	ship_subsys			*ss;
	model_subsystem	*psub;
	vector				subsys_world_pos;
	float					dist;

	shipp = &Ships[ship_objp->instance];
	wip = &Weapon_info[wi_index];

	for ( ss = GET_FIRST(&shipp->subsys_list); ss != END_OF_LIST(&shipp->subsys_list); ss = GET_NEXT(ss) ) {
		psub = ss->system_info;

		// convert subsys point to world coords
		vm_vec_unrotate(&subsys_world_pos, &psub->pnt, &ship_objp->orient);
		vm_vec_add2(&subsys_world_pos, &ship_objp->pos);

		// see if subsys point is within damage sphere
		dist = vm_vec_dist_quick(blast_pos, &subsys_world_pos);	
		if ( dist < wip->outer_radius ) {
			ship_subsys_set_disrupted(ss, fl2i(6000.0f + frand()*4000.0f));
		}
	}
}

//	----------------------------------------------------------------------
//	weapon_area_calc_damage()
//
// Calculate teh damage for an object based on the location of an area-effect
// explosion.
//
// input:		objp			=>		object pointer ship receiving blast effect
//					pos			=>		world pos of blast center
//					inner_rad	=>		smallest radius at which full damage is done
//					outer_rad	=>		radius at which no damage is done
//					max_blast	=>		maximum blast possible from explosion
//					max_damage	=>		maximum damage possible from explosion
//					blast			=>		OUTPUT PARAMETER: receives blast value from explosion
//					damage		=>		OUTPUT PARAMETER: receives damage value from explosion
//					limit			=>		a limit on the area, needed for shockwave damage
//
//	returns:		no damage occurred	=>		-1
//					damage occured			=>		0
//
int weapon_area_calc_damage(object *objp, vector *pos, float inner_rad, float outer_rad, float max_blast, float max_damage, float *blast, float *damage, float limit)
{
	float			dist, max_dist, min_dist;

 	// only blast ships and asteroids
	if ( (objp->type != OBJ_SHIP) && (objp->type != OBJ_ASTEROID)) {
		return -1;
	}

	max_dist = objp->radius + outer_rad;
	dist = vm_vec_dist_quick(&objp->pos, pos);	
	if ( (dist > max_dist) || (dist > (limit+objp->radius)) ) {
		return -1;	// spheres don't intersect at all
	}

	if ( dist < (inner_rad+objp->radius) ) {
		// damage is maximum within inner radius
		*damage = max_damage;
		*blast = max_blast;
	} else {
		float dist_to_outer_rad_squared, total_dist_squared;
		min_dist = dist - objp->radius;
		Assert(min_dist < outer_rad);
		dist_to_outer_rad_squared = (outer_rad-min_dist)*(outer_rad-min_dist);
		total_dist_squared = (inner_rad-outer_rad)*(inner_rad-outer_rad);
		// AL 2-24-98: drop off damage relative to square of distance
		Assert(dist_to_outer_rad_squared <= total_dist_squared);
		*damage = max_damage * dist_to_outer_rad_squared/total_dist_squared;


//		*damage = (min_dist - outer_rad) * max_damage/(inner_rad - outer_rad);
		*blast =  (min_dist - outer_rad) * max_blast /(inner_rad - outer_rad);
	}

	// nprintf(("AI", "Frame %i: Damage = %7.3f, %7.3f meters away.\n", Framecount, *damage, dist));

	return 0;
}

//	----------------------------------------------------------------------
//	weapon_area_apply_blast()
//
// Apply the blast effects of an explosion to a ship
//
// input:	force_apply_pos	=>		world pos of where force is applied to object
//				ship_obj				=>		object pointer of ship receiving the blast
//				blast_pos			=>		world pos of blast center
//				blast					=>		force of blast
//				make_shockwave		=>		boolean, whether to create a shockwave or not
//
void weapon_area_apply_blast(vector *force_apply_pos, object *ship_obj, vector *blast_pos, float blast, int make_shockwave)
{
	#define	SHAKE_CONST 3000
	vector		force, vec_blast_to_ship, vec_ship_to_impact;
	polymodel		*pm;

	// apply blast force based on distance from center of explosion
	vm_vec_sub(&vec_blast_to_ship, &ship_obj->pos, blast_pos);
	vm_vec_normalize_safe(&vec_blast_to_ship);
	vm_vec_copy_scale(&force, &vec_blast_to_ship, blast );


	vm_vec_sub(&vec_ship_to_impact, blast_pos, &ship_obj->pos);

	pm = model_get(Ships[ship_obj->instance].modelnum);
	Assert ( pm != NULL );

	if (make_shockwave) {
		physics_apply_shock (&force, blast, &ship_obj->phys_info, &ship_obj->orient, &pm->mins, &pm->maxs, pm->rad);
		if (ship_obj == Player_obj) {
			joy_ff_play_vector_effect(&vec_blast_to_ship, blast * 2.0f);
		}
	} else {
		ship_apply_whack( &force, &vec_ship_to_impact, ship_obj);
	}
}

//	----------------------------------------------------------------------
//	weapon_do_area_effect()
//
// Do the area effect for a weapon
//
// input:	wobjp			=>		object pointer to weapon causing explosion
//				pos			=>		world pos of explosion center
//				other_obj	=>		object pointer to ship that weapon impacted on (can be NULL)
//
void weapon_do_area_effect(object *wobjp, vector *pos, object *other_obj)
{
	weapon_info	*wip;
	weapon *wp;
	object		*objp;
	float			damage, blast;

	wip = &Weapon_info[Weapons[wobjp->instance].weapon_info_index];	
	wp = &Weapons[wobjp->instance];
	Assert(wip->inner_radius != 0);	

	// only blast ships and asteroids
	for ( objp = GET_FIRST(&obj_used_list); objp !=END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if ( (objp->type != OBJ_SHIP) && (objp->type != OBJ_ASTEROID) ) {
			continue;
		}
	
		if ( objp->type == OBJ_SHIP ) {
			// don't blast navbuoys
			if ( ship_get_SIF(objp->instance) & SIF_NAVBUOY ) {
				continue;
			}
		}

		if ( weapon_area_calc_damage(objp, pos, wip->inner_radius, wip->outer_radius, wip->blast_force, wip->damage, &blast, &damage, wip->outer_radius) == -1 ){
			continue;
		}

		// scale damage
		damage *= weapon_get_damage_scale(wip, wobjp, other_obj);		

		switch ( objp->type ) {
		case OBJ_SHIP:
			ship_apply_global_damage(objp, wobjp, pos, damage);
			weapon_area_apply_blast(NULL, objp, pos, blast, 0);			
			break;
		case OBJ_ASTEROID:
			asteroid_hit(objp, NULL, NULL, damage);
			break;
		default:
			Int3();
			break;
		} 	

	}	// end for

	// if this weapon has the "Electronics" flag set, then disrupt subsystems in sphere
	if ( (other_obj != NULL) && (wip->wi_flags & WIF_ELECTRONICS) ) {
		if ( other_obj->type == OBJ_SHIP ) {
			weapon_do_electronics_affect(other_obj, pos, Weapons[wobjp->instance].weapon_info_index);
		}
	}
}


//	----------------------------------------------------------------------
//	weapon_hit()
//
// This function is called when a weapon hits something (or, in the case of
// missiles explodes for any particular reason)
//
void weapon_hit( object * weapon_obj, object * other_obj, vector * hitpos )
{
	Assert(weapon_obj != NULL);
	if(weapon_obj == NULL){
		return;
	}
	Assert((weapon_obj->type == OBJ_WEAPON) && (weapon_obj->instance >= 0) && (weapon_obj->instance < MAX_WEAPONS));
	if((weapon_obj->type != OBJ_WEAPON) || (weapon_obj->instance < 0) || (weapon_obj->instance >= MAX_WEAPONS)){
		return;
	}

	int			num = weapon_obj->instance;
	int			weapon_type = Weapons[num].weapon_info_index;
	object		*weapon_parent_objp;
	weapon_info	*wip;
	// int np_index;

	Assert((weapon_type >= 0) && (weapon_type < MAX_WEAPONS));
	if((weapon_type < 0) || (weapon_type >= MAX_WEAPONS)){
		return;
	}
	wip = &Weapon_info[weapon_type];
	weapon_parent_objp = &Objects[weapon_obj->parent];

	// if this is the player ship, and is a laser hit, skip it. wait for player "pain" to take care of it
	// if( ((wip->subtype != WP_LASER) || !MULTIPLAYER_CLIENT) && (Player_obj != NULL) && (other_obj == Player_obj) ){
	if((other_obj != Player_obj) || (wip->subtype != WP_LASER) || !MULTIPLAYER_CLIENT){
		weapon_hit_do_sound(other_obj, wip, hitpos);
	}

	if ( wip->impact_weapon_expl_index > -1 )	{
		int expl_ani_handle = weapon_get_expl_handle(wip->impact_weapon_expl_index, hitpos, wip->impact_explosion_radius);
		particle_create( hitpos, &vmd_zero_vector, 0.0f, wip->impact_explosion_radius, PARTICLE_BITMAP_PERSISTENT, expl_ani_handle );
	}

	weapon_obj->flags |= OF_SHOULD_BE_DEAD;

	int sw_flag = SW_WEAPON;

	// check if this is an area effect weapon
	if ( wip->wi_flags & WIF_AREA_EFFECT ) {
	// if ( wip->subtype & WP_MISSILE && wip->wi_flags & WIF_AREA_EFFECT ) {
		if ( wip->wi_flags & WIF_SHOCKWAVE ) {
			float actual_damage = wip->damage;
			// Shockwaves caused by weapons hitting weapons are 1/4 as powerful
			if ( ((other_obj) && (other_obj->type == OBJ_WEAPON)) || (Weapons[num].weapon_flags & WF_DESTROYED_BY_WEAPON)) {
				actual_damage /= 4.0f;
				sw_flag |= SW_WEAPON_KILL;
			}
			shockwave_create_info sci;
			sci.blast = wip->blast_force;
			sci.damage = actual_damage;
			sci.inner_rad = wip->inner_radius;
			sci.outer_rad = wip->outer_radius;
			sci.speed = wip->shockwave_speed;
			sci.rot_angle = 0.0f;

			shockwave_create(OBJ_INDEX(weapon_obj), hitpos, &sci, sw_flag);
//			snd_play_3d( &Snds[SND_SHOCKWAVE_IMPACT], hitpos, &Eye_position );
		}
		else {
			weapon_do_area_effect(weapon_obj, hitpos, other_obj);
		}
	}

	// check if this is an EMP weapon
	if(wip->wi_flags & WIF_EMP){
		emp_apply(&weapon_obj->pos, wip->inner_radius, wip->outer_radius, wip->emp_intensity, wip->emp_time);
	}	

	// spawn weapons - note the change from FS 1 multiplayer.
	if (wip->wi_flags & WIF_SPAWN){
		spawn_child_weapons(weapon_obj);
	}	
}

void weapon_detonate(object *objp)
{
	Assert(objp != NULL);
	if(objp == NULL){
		return;
	}
	Assert((objp->type == OBJ_WEAPON) && (objp->instance >= 0));
	if((objp->type != OBJ_WEAPON) || (objp->instance < 0)){
		return;
	}	

	// send a detonate packet in multiplayer
	if(MULTIPLAYER_MASTER){
		send_weapon_detonate_packet(objp);
	}

	// call weapon hit
	weapon_hit(objp, NULL, &objp->pos);
}

//	Return the Weapon_info[] index of the weapon with name *name.
int weapon_name_lookup(const char *name)
{
	int	i;

	for ( i=0; i < Num_weapon_types; i++) {
		if (!stricmp(name, Weapon_info[i].name)) {
			return i;
		}
	}

	return -1;
}

// Group_id:  If you should quad lasers, they should all have the same group id.  
// This will be used to optimize lighting, since each group only needs to cast one light.
// Call this to get a new group id, then pass it to each weapon_create call for all the
// weapons in the group.   Number will be between 0 and WEAPON_MAX_GROUP_IDS and will
// get reused.
int weapon_create_group_id()
{
	static int current_id = 0;

	int n = current_id;
	
	current_id++;
	if ( current_id >= WEAPON_MAX_GROUP_IDS )	{
		current_id = 0;
	}
	return n;
}


void weapons_page_in()
{
	int i, j, idx;

	// Page in bitmaps for all weapons
	for (i=0; i<Num_weapon_types; i++ )	{
		weapon_info *wip = &Weapon_info[i];

		wip->wi_flags &= (~WIF_THRUSTER);		// Assume no thrusters
		
		switch( wip->render_type )	{
			case WRT_POF:
				{
					wip->model_num = model_load( wip->pofbitmap_name, 0, NULL );

					polymodel *pm = model_get( wip->model_num );

					// If it has a model, and the model pof has thrusters, then set
					// the flags
					if ( pm->n_thrusters > 0 )	{
						//mprintf(( "Weapon %s has thrusters!\n", wip->name ));
						wip->wi_flags |= WIF_THRUSTER;
					}
		
					for (j=0; j<pm->n_textures; j++ )	{
						int bitmap_num = pm->original_textures[j];

						if ( bitmap_num > -1 )	{
							// if we're in Glide (and maybe later with D3D), use nondarkening textures
							if(gr_screen.mode == GR_GLIDE){
								bm_page_in_nondarkening_texture( bitmap_num );
							} else {
								bm_page_in_texture( bitmap_num );
							}
						}
					}
				}
				break;

			case WRT_LASER:
				{
					bm_page_in_texture( wip->laser_bitmap );

					if(wip->laser_glow_bitmap >= 0){
						bm_page_in_texture(wip->laser_glow_bitmap);
					}
				}
				break;

			default:
				Int3();	// Invalid weapon rendering type.
		}

		// If this has an impact vclip page it in.
//		if ( wip->impact_explosion_ani > -1 )	{
//			int nframes, fps;
//			bm_get_info( wip->impact_explosion_ani, NULL, NULL, NULL, &nframes, &fps );
//			bm_page_in_xparent_texture( wip->impact_explosion_ani, nframes );
//		}

		// trail bitmaps
		if ( (wip->wi_flags & WIF_TRAIL) && (wip->tr_info.bitmap > -1) )	{
			bm_page_in_texture( wip->tr_info.bitmap );
		}

		// if this is a beam weapon, page in its stuff
		if(wip->wi_flags & WIF_BEAM){
			// all beam sections
			for(idx=0; idx<wip->b_info.beam_num_sections; idx++){
				if((idx < MAX_BEAM_SECTIONS) && (wip->b_info.sections[idx].texture >= 0)){
					bm_page_in_texture(wip->b_info.sections[idx].texture);
				}
			}

			// muzzle glow
			if(wip->b_info.beam_glow_bitmap >= 0){
				bm_page_in_texture(wip->b_info.beam_glow_bitmap);
			}

			// particle ani
			if(wip->b_info.beam_particle_ani >= 0){
				int nframes, fps;
				bm_get_info( wip->b_info.beam_particle_ani, NULL, NULL, NULL, &nframes, &fps );
				bm_page_in_texture( wip->b_info.beam_particle_ani, nframes );
			}
		}
	}

	// explosion ani's
	for (i=0; i<Num_weapon_expl; i++) {
		int bitmap_handle, nframes, fps;

		for (j=0; j<Weapon_expl_info[i].lod_count; j++) {
			//load ani
			bitmap_handle = bm_load_animation(Weapon_expl_info[i].lod[j].filename, &nframes, &fps, 1);
			Weapon_expl_info[i].lod[j].bitmap_id = bitmap_handle;
			Weapon_expl_info[i].lod[j].fps = fps;
			Weapon_expl_info[i].lod[j].num_frames = nframes;

			// page it in
			bm_page_in_xparent_texture(bitmap_handle, nframes);
		}
	}

	// Counter measures
	for (i=0; i<Num_cmeasure_types; i++ )	{
		cmeasure_info *cmeasurep;

		cmeasurep = &Cmeasure_info[i];
	
		cmeasurep->model_num = model_load( cmeasurep->pof_name, 0, NULL );

		polymodel *pm = model_get( cmeasurep->model_num );

		for (j=0; j<pm->n_textures; j++ )	{
			int bitmap_num = pm->original_textures[j];

			if ( bitmap_num > -1 )	{
				bm_page_in_texture( bitmap_num );
			}
		}
		Assert( cmeasurep->model_num > -1 );
	}

}

// call to get the "color" of the laser at the given moment (since glowing lasers can cycle colors)
void weapon_get_laser_color(color *c, object *objp)
{
	weapon *wep;
	weapon_info *winfo;
	float pct;

	// sanity
	if(c == NULL){
		return;
	}

	// sanity
	Assert(objp->type == OBJ_WEAPON);
	Assert(objp->instance >= 0);
	Assert(Weapons[objp->instance].weapon_info_index >= 0);
	if((objp->type != OBJ_WEAPON) || (objp->instance < 0) || (Weapons[objp->instance].weapon_info_index < 0)){
		return;
	}
	wep = &Weapons[objp->instance];
	winfo = &Weapon_info[wep->weapon_info_index];

	// if we're a one-color laser
	if((winfo->laser_color_2.red == 0) && (winfo->laser_color_2.green == 0) && (winfo->laser_color_2.blue == 0)){
		*c = winfo->laser_color_1;
	}

	// lifetime pct
	pct = 1.0f - (wep->lifeleft / winfo->lifetime);
	if(pct > 0.5f){
		pct = 0.5f;
	} else if (pct < 0.0f)
		pct = 0.0f;

	pct *= 2.0f;
	
	// otherwise interpolate between the colors
	gr_init_color( c, (int)((float)winfo->laser_color_1.red + (((float)winfo->laser_color_2.red - (float)winfo->laser_color_1.red) * pct)), 
							(int)((float)winfo->laser_color_1.green + (((float)winfo->laser_color_2.green - (float)winfo->laser_color_1.green) * pct)), 
							(int)((float)winfo->laser_color_1.blue + (((float)winfo->laser_color_2.blue - (float)winfo->laser_color_1.blue) * pct)) );
}

// default weapon particle spew data
int Weapon_particle_spew_count = 1;
int Weapon_particle_spew_time = 25;
float Weapon_particle_spew_vel = 0.4f;
float Weapon_particle_spew_radius = 2.0f;
float Weapon_particle_spew_lifetime = 0.15f;
float Weapon_particle_spew_scale = 0.8f;

// for weapons flagged as particle spewers, spew particles. wheee
void weapon_maybe_spew_particle(object *obj)
{
	weapon *wp;
	int idx;
	vector direct, direct_temp, particle_pos;
	vector null_vec = ZERO_VECTOR;
	vector vel;
	float ang;

	// check some stuff
	Assert(obj->type == OBJ_WEAPON);
	Assert(obj->instance >= 0);
	Assert(Weapons[obj->instance].weapon_info_index >= 0);
	Assert(Weapon_info[Weapons[obj->instance].weapon_info_index].wi_flags & WIF_PARTICLE_SPEW);
	
	wp = &Weapons[obj->instance];	

	// if the weapon's particle timestamp has elapse`d
	if((wp->particle_spew_time == -1) || timestamp_elapsed(wp->particle_spew_time)){
		// reset the timestamp
		wp->particle_spew_time = timestamp(Weapon_particle_spew_time);

		// spew some particles
		for(idx=0; idx<Weapon_particle_spew_count; idx++){
			// get the backward vector of the weapon
			direct = obj->orient.v.fvec;
			vm_vec_negate(&direct);

			//	randomly perturb x, y and z
			
			// uvec
			ang = fl_radian(frand_range(-90.0f, 90.0f));
			vm_rot_point_around_line(&direct_temp, &direct, ang, &null_vec, &obj->orient.v.fvec);			
			direct = direct_temp;
			vm_vec_scale(&direct, Weapon_particle_spew_scale);

			// rvec
			ang = fl_radian(frand_range(-90.0f, 90.0f));
			vm_rot_point_around_line(&direct_temp, &direct, ang, &null_vec, &obj->orient.v.rvec);			
			direct = direct_temp;
			vm_vec_scale(&direct, Weapon_particle_spew_scale);

			// fvec
			ang = fl_radian(frand_range(-90.0f, 90.0f));
			vm_rot_point_around_line(&direct_temp, &direct, ang, &null_vec, &obj->orient.v.uvec);			
			direct = direct_temp;
			vm_vec_scale(&direct, Weapon_particle_spew_scale);

			// get a velovity vector of some percentage of the weapon's velocity
			vel = obj->phys_info.vel;
			vm_vec_scale(&vel, Weapon_particle_spew_vel);

			// emit the particle
			vm_vec_add(&particle_pos, &obj->pos, &direct);
			particle_create(&particle_pos, &vel, Weapon_particle_spew_lifetime, Weapon_particle_spew_radius, PARTICLE_SMOKE);
		}
	}
}

// debug console functionality
void pspew_display_dcf()
{
	dc_printf("Particle spew settings\n\n");
	dc_printf("Particle spew count (pspew_count) : %d\n", Weapon_particle_spew_count);
	dc_printf("Particle spew time (pspew_time) : %d\n", Weapon_particle_spew_time);
	dc_printf("Particle spew velocity (pspew_vel) : %f\n", Weapon_particle_spew_vel);
	dc_printf("Particle spew size (pspew_size) : %f\n", Weapon_particle_spew_radius);
	dc_printf("Particle spew lifetime (pspew_life) : %f\n", Weapon_particle_spew_lifetime);
	dc_printf("Particle spew scale (psnew_scale) : %f\n", Weapon_particle_spew_scale);
}

DCF(pspew_count, "Number of particles spewed at a time")
{	
	dc_get_arg(ARG_INT);
	if(Dc_arg_type & ARG_INT){
		Weapon_particle_spew_count = Dc_arg_int;
	}

	pspew_display_dcf();
}

DCF(pspew_time, "Time between particle spews")
{	
	dc_get_arg(ARG_INT);
	if(Dc_arg_type & ARG_INT){
		Weapon_particle_spew_time = Dc_arg_int;
	}

	pspew_display_dcf();
}

DCF(pspew_vel, "Relative velocity of particles (0.0 - 1.0)")
{	
	dc_get_arg(ARG_FLOAT);
	if(Dc_arg_type & ARG_FLOAT){
		Weapon_particle_spew_vel = Dc_arg_float;
	}

	pspew_display_dcf();
}

DCF(pspew_size, "Size of spewed particles")
{	
	dc_get_arg(ARG_FLOAT);
	if(Dc_arg_type & ARG_FLOAT){
		Weapon_particle_spew_radius = Dc_arg_float;
	}

	pspew_display_dcf();
}

DCF(pspew_life, "Lifetime of spewed particles")
{	
	dc_get_arg(ARG_FLOAT);
	if(Dc_arg_type & ARG_FLOAT){
		Weapon_particle_spew_lifetime = Dc_arg_float;
	}

	pspew_display_dcf();
}

DCF(pspew_scale, "How far away particles are from the weapon path")
{	
	dc_get_arg(ARG_FLOAT);
	if(Dc_arg_type & ARG_FLOAT){
		Weapon_particle_spew_scale = Dc_arg_float;
	}

	pspew_display_dcf();
}

// return a scale factor for damage which should be applied for 2 collisions
float weapon_get_damage_scale(weapon_info *wip, object *wep, object *target)
{
	weapon *wp;	
	int from_player = 0;
	float total_scale = 1.0f;
#ifndef MAKE_FS1
	float hull_pct;
	int is_big_damage_ship = 0;
#endif

	// sanity
	if((wip == NULL) || (wep == NULL) || (target == NULL)){
		return 1.0f;
	}

	// don't scale any damage if its not a weapon	
	if((wep->type != OBJ_WEAPON) || (wep->instance < 0) || (wep->instance >= MAX_WEAPONS)){
		return 1.0f;
	}
	wp = &Weapons[wep->instance];

	// was the weapon fired by the player
	from_player = 0;
	if((wep->parent >= 0) && (wep->parent < MAX_OBJECTS) && (Objects[wep->parent].flags & OF_PLAYER_SHIP)){
		from_player = 1;
	}
		
	// if this is a lockarm weapon, and it was fired unlocked
	if((wip->wi_flags & WIF_LOCKARM) && !(wp->weapon_flags & WF_LOCKED_WHEN_FIRED)){		
		total_scale *= 0.1f;
	}

#ifndef MAKE_FS1 // don't do special damage scaling for capships in FS1
	// if the hit object was a ship
	if(target->type == OBJ_SHIP){
		ship *shipp;
		ship_info *sip;

		// get some info on the ship
		Assert((target->instance >= 0) && (target->instance < MAX_SHIPS));
		if((target->instance < 0) || (target->instance >= MAX_SHIPS)){
			return total_scale;
		}
		shipp = &Ships[target->instance];
		sip = &Ship_info[Ships[target->instance].ship_info_index];

		// get hull pct of the ship currently
		hull_pct = target->hull_strength / sip->initial_hull_strength;

		// if it has hit a supercap ship and is not a supercap class weapon
		if((sip->flags & SIF_SUPERCAP) && !(wip->wi_flags & WIF_SUPERCAP)){
			// if the supercap is around 3/4 damage, apply nothing
			if(hull_pct <= 0.75f){
				return 0.0f;
			} else {
				total_scale *= SUPERCAP_DAMAGE_SCALE;
			}
		}

		// determine if this is a big damage ship
		is_big_damage_ship = (sip->flags & SIF_BIG_DAMAGE);

		// if this is a large ship, and is being hit by flak
		if(is_big_damage_ship && (wip->wi_flags & WIF_FLAK)){
			total_scale *= FLAK_DAMAGE_SCALE;
		}
		
		// if the player is firing small weapons at a big ship
		if( from_player && is_big_damage_ship && !(wip->wi_flags & (WIF_HURTS_BIG_SHIPS)) ){

			// if its a laser weapon
			if(wip->subtype == WP_LASER){
				total_scale *= 0.01f;
			} else {
				total_scale *= 0.05f;
			}
		}

		// if the weapon is a small weapon being fired at a big ship
		if( is_big_damage_ship && !(wip->wi_flags & (WIF_HURTS_BIG_SHIPS)) ){
			if(hull_pct > 0.1f){
				total_scale *= hull_pct;
			} else {
				return 0.0f;
			}
		}
	}
#endif
	
	return total_scale;
}

int weapon_get_expl_handle(int weapon_expl_index, vector *pos, float size)
{
	weapon_expl_info *wei = &Weapon_expl_info[weapon_expl_index];

	if (wei->lod_count == 1) {
		return wei->lod[0].bitmap_id;
	}

	// now we have to do some work
	vertex v;
	int x, y, w, h, bm_size;
	int must_stop = 0;
	int best_lod = 1;
	int behind = 0;

	// start the frame
	extern float Viewer_zoom;
	extern int G3_count;

	if(!G3_count){
		g3_start_frame(1);
		must_stop = 1;
	}
	g3_set_view_matrix(&Eye_position, &Eye_matrix, Viewer_zoom);

	// get extents of the rotated bitmap
	g3_rotate_vertex(&v, pos);

	// if vertex is behind, find size if in front, then drop down 1 LOD
	if (v.codes & CC_BEHIND) {
		float dist = vm_vec_dist_quick(&Eye_position, pos);
		vector temp;

		behind = 1;
		vm_vec_scale_add(&temp, &Eye_position, &Eye_matrix.v.fvec, dist);
		g3_rotate_vertex(&v, &temp);

		// if still behind, bail and go with default
		if (v.codes & CC_BEHIND) {
			behind = 0;
		}
	}

	if (!g3_get_bitmap_dims(wei->lod[0].bitmap_id, &v, size, &x, &y, &w, &h, &bm_size)) {
		if (Detail.hardware_textures == 4) {
			// straight LOD
			if(w <= bm_size/8){
				best_lod = 3;
			} else if(w <= bm_size/2){
				best_lod = 2;
			} else if(w <= 1.3f*bm_size){
				best_lod = 1;
			} else {
				best_lod = 0;
			}
		} else {
			// less aggressive LOD for lower detail settings
			if(w <= bm_size/8){
				best_lod = 3;
			} else if(w <= bm_size/3){
				best_lod = 2;
			} else if(w <= (1.15f*bm_size)){
				best_lod = 1;
			} else {
				best_lod = 0;
			}		
		}
	}

	// if it's behind, bump up LOD by 1
	if (behind) {
		best_lod++;
	}

	// end the frame
	if(must_stop){
		g3_end_frame();
	}

	best_lod = min(best_lod, wei->lod_count - 1);
	return wei->lod[best_lod].bitmap_id;
}

// -------------------------------------------------------------------------------------------------
// weapon_close()
//
// called in game_shutdown() to free malloced memory
//
// NOTE: do not call this function.  It is only called from game_shutdown()
void weapon_close()
{
	int i;
	
	// free info from parsed table data
	for (i=0; i<MAX_WEAPON_TYPES; i++) {
		if ( Weapon_info[i].desc != NULL ) {
			free(Weapon_info[i].desc);
			Weapon_info[i].desc = NULL;
		}
		if ( Weapon_info[i].tech_desc != NULL ) {
			free(Weapon_info[i].tech_desc);
			Weapon_info[i].tech_desc = NULL;
		}
	}
}
