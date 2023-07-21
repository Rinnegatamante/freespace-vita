/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Weapon/Shockwave.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C file for creating and managing shockwaves
 *
 * $Log$
 * Revision 1.4  2004/09/20 01:31:45  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.3  2002/06/09 04:41:29  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:53  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 7     9/01/99 10:15a Dave
 * 
 * 6     7/18/99 12:32p Dave
 * Randomly oriented shockwaves.
 * 
 * 5     3/23/99 2:29p Andsager
 * Fix shockwaves for kamikazi and Fred defined.  Collect together
 * shockwave_create_info struct.
 * 
 * 4     2/26/99 4:14p Dave
 * Put in the ability to have multiple shockwaves for ships.
 * 
 * 3     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 49    5/18/98 3:04p Lawrance
 * Play shockwave impact sound
 * 
 * 48    5/18/98 12:59a Lawrance
 * Replace shockwave impact sound with a new "whoosh" sound that
 * originates from the shockwave center
 * 
 * 47    4/15/98 10:17p Mike
 * Training mission #5.
 * Fix application of subsystem damage.
 * 
 * 46    4/09/98 7:58p John
 * Cleaned up tmapper code a bit.   Put NDEBUG around some ndebug stuff.
 * Took out XPARENT flag settings in all the alpha-blended texture stuff.
 * 
 * 45    3/31/98 5:19p John
 * Removed demo/save/restore.  Made NDEBUG defined compile.  Removed a
 * bunch of debug stuff out of player file.  Made model code be able to
 * unload models and malloc out only however many models are needed.
 *  
 * 
 * 44    3/30/98 4:02p John
 * Made machines with < 32 MB of RAM use every other frame of certain
 * bitmaps.   Put in code to keep track of how much RAM we've malloc'd.
 * 
 * 43    3/26/98 5:26p John
 * added new paging code. nonfunctional.
 * 
 * 42    3/04/98 4:11p Lawrance
 * Have area effects affect asteroids, have asteroids cast an area effect,
 * fix ship shockwaves
 * 
 * 41    2/26/98 10:08p Hoffoss
 * Rewrote state saving and restoring to fix bugs and simplify the code.
 * 
 * 40    2/22/98 12:19p John
 * Externalized some strings
 * 
 * 39    2/20/98 8:32p Lawrance
 * Make shockwaves affect subsystems more realistically.
 * 
 * 38    2/16/98 11:26a Lawrance
 * ensure shockwave lasts full duration
 * 
 * 37    2/16/98 10:04a Lawrance
 * Fix broken shockwave damage.
 * 
 * 36    2/14/98 4:42p Lawrance
 * pass shockwave object (not parent) to ship_apply_global_damage()
 * 
 * 35    2/12/98 11:54p Lawrance
 * restructure rendering code to use an animation
 * 
 * 34    2/11/98 5:38p Dave
 * Put in a global inited flag for shockwaves.
 * 
 * 33    2/02/98 8:47a Andsager
 * Ship death area damage applied as instantaneous damage for small ships
 * and shockwaves for large (>50 radius) ships.
 * 
 * 32    1/26/98 11:54a Lawrance
 * Don't allow Navbuoys to be affected by area-effect damage and blasts.
 * 
 * 31    1/22/98 11:43p Lawrance
 * Play sound effect when player is hit by shockwave.
 * 
 * 30    1/14/98 4:31p Dave
 * Made shockwaves apply damage correctly.
 * 
 * 29    1/14/98 2:59p Allender
 * if shockwave came from a weapon, make the shockwave's parent be the
 * weapons parent.
 * 
 * 28    12/30/97 6:44p John
 * Made g3_Draw_bitmap functions account for aspect of bitmap.
 * 
 * 27    12/02/97 4:00p John
 * Added first rev of thruster glow, along with variable levels of
 * translucency, which retquired some restructing of palman.
 * 
 * 26    11/29/97 2:06p John
 * made g3_draw_bitmap and g3_draw_rotated bitmap take w&h, not w/2 & h/2,
 * like they used to incorrectly assume.   Added code to model to read in
 * thruster radius's.
 * 
 * 25    11/19/97 10:20p Lawrance
 * remove ship_shockwave from ship struct, handled in physics now
 * 
 * 24    11/16/97 8:52p Andsager
 * For shockwaves, update physics damping info in physics code.
 * 
 * 23    9/18/97 10:48p Lawrance
 * comment out unused struct member
 * 
 * 22    9/18/97 4:08p John
 * Cleaned up & restructured ship damage stuff.
 * 
 * 21    9/17/97 5:12p John
 * Restructured collision routines.  Probably broke a lot of stuff.
 * 
 * 20    9/04/97 5:10p Andsager
 * implement physics using moment of inertia and mass (from BSPgen).
 * Added to phys_info struct.  Updated ship_info, polymodel structs.
 * Updated weapon ($Mass and $Force) and ship ($Mass -> $Density) tables
 * 
 * 19    9/03/97 4:33p John
 * changed bmpman to only accept ani and pcx's.  made passing .pcx or .ani
 * to bm_load functions not needed.   Made bmpman keep track of palettes
 * for bitmaps not mapped into game palettes.
 * 
 * 18    8/26/97 3:31p Andsager
 * scaled shockwave shake duration according to damage
 * 
 * 17    8/05/97 1:25a Mike
 * Make ship death roll be shortened by more damage.
 * 
 * 16    7/31/97 5:55p John
 * made so you pass flags to obj_create.
 * Added new collision code that ignores any pairs that will never
 * collide.
 * 
 * 15    7/25/97 4:30p Andsager
 * Save shockwave info
 * 
 * 14    7/25/97 1:04p Andsager
 * Modified physics flag PF_REDUCED_DAMP for damping when object is hit.
 * Flag is now set in physics_apply_whack/shock and turned off in
 * physics_sim_vel.  Weapons should not directly set this flag.
 * 
 * 13    7/22/97 2:40p Andsager
 * shockwaves now cause proper rotation of ships
 * 
 * 12    7/20/97 7:01p Lawrance
 * changed names of anim_ files to be more consistent
 * 
 * 11    7/18/97 10:52a Lawrance
 * let player have some control when shockwave hits
 * 
 * 10    7/17/97 8:02p Lawrance
 * tweaking shockwave effect
 * 
 * 9     7/16/97 5:51p Lawrance
 * make shockwaves translucent
 * 
 * 8     7/16/97 4:00p Lawrance
 * render shockwaves by default
 * 
 * 7     7/16/97 3:50p Lawrance
 * render shockwaves first, to fake transparency
 * 
 * 6     7/16/97 2:52p Lawrance
 * make shockwaves objects
 * 
 * 5     7/15/97 7:26p Lawrance
 * make shockwave blast persist over time
 * 
 * 4     7/09/97 1:56p Lawrance
 * add savegame support for shockwaves
 * 
 * 3     7/09/97 10:33a Lawrance
 * make area-effect spheres translucent
 * 
 * 2     7/08/97 6:00p Lawrance
 * implementing shockwaves
 * 
 * 1     7/08/97 1:30p Lawrance
 *
 * $NoKeywords: $
 */

#include "2d.h"
#include	"3d.h"
#include "weapon.h"
#include "ship.h"
#include "freespace.h"	// for colors
#include "shockwave.h"
#include "timer.h"
#include "animplay.h"
#include "bmpman.h"
#include "linklist.h"
#include "shiphit.h"
#include "gamesnd.h"
#include "asteroid.h"

// -----------------------------------------------------------
// Data structures
// -----------------------------------------------------------

// -----------------------------------------------------------
// Module-wide globals
// -----------------------------------------------------------

static const char *Shockwave_filenames[MAX_SHOCKWAVE_TYPES] = 
{
//XSTR:OFF
	"shockwave01"
//XSTR:ON
};

shockwave			Shockwaves[MAX_SHOCKWAVES];
shockwave_info	Shockwave_info[MAX_SHOCKWAVE_TYPES];

shockwave			Shockwave_list;
int					Shockwave_inited = 0;

// -----------------------------------------------------------
// Function macros
// -----------------------------------------------------------
#define SW_INDEX(sw) (sw-Shockwaves)
	
// -----------------------------------------------------------
// Externals
// -----------------------------------------------------------
extern int Show_area_effect;

// ------------------------------------------------------------------------------------
// shockwave_create()
//
// Call to create a shockwave
//
//	input:	parent_objnum	=> object number of object spawning the shockwave
//				pos				=>	vector specifing global position of shockwave center
//				speed				=>	speed at which shockwave expands (m/s)
//				inner_radius	=>	radius at which damage applied is at maximum
//				outer_radius	=> damage decreases linearly to zero from inner_radius to
//										outer_radius.  Outside outer_radius, damage is 0.
//				damage			=>	the maximum damage (ie within inner_radius)
//				blast				=> the maximux blast (within inner_radius)
//				sw_flag			=> indicates whether shockwave is from weapon or ship explosion
//				delay          => delay in ms before the shockwave actually starts
//
//	return:	success			=>	object number of shockwave
//				failure			=>	-1
//
int shockwave_create(int parent_objnum, vector *pos, shockwave_create_info *sci, int flag, int delay)
{
	int				i, objnum, real_parent;
	shockwave		*sw;
	shockwave_info	*si;
	matrix			orient;

 	for ( i = 0; i < MAX_SHOCKWAVES; i++ ) {
		if ( !(Shockwaves[i].flags & SW_USED) ){
			break;
		}
	}

	if ( i == MAX_SHOCKWAVES ) {
		return -1;
	}

	// real_parent is the guy who caused this shockwave to happen
	if ( Objects[parent_objnum].type == OBJ_WEAPON ){
		real_parent = Objects[parent_objnum].parent;
	} else {
		real_parent = parent_objnum;
	}

	sw = &Shockwaves[i];
	sw->flags = (SW_USED | flag);
	sw->speed = sci->speed;
	sw->inner_radius = sci->inner_rad;
	sw->outer_radius = sci->outer_rad;
	sw->damage = sci->damage;
	sw->blast = sci->blast;
	sw->radius = 1.0f;
	sw->pos = *pos;
	sw->num_objs_hit = 0;
	sw->shockwave_info_index=0;		// only one type for now... type could be passed is as a parameter
	sw->current_bitmap=-1;

	sw->time_elapsed=0.0f;
	sw->delay_stamp = delay;

	sw->rot_angle = sci->rot_angle;

	si = &Shockwave_info[sw->shockwave_info_index];
//	sw->total_time = i2fl(si->num_frames) / si->fps;	// in seconds
	sw->total_time = sw->outer_radius / sw->speed;

	if ( Objects[parent_objnum].type == OBJ_WEAPON ) {		
		sw->weapon_info_index = Weapons[Objects[parent_objnum].instance].weapon_info_index;
	}
	else {		
		sw->weapon_info_index = -1;
	}

	orient = vmd_identity_matrix;

	objnum = obj_create( OBJ_SHOCKWAVE, real_parent, i, &orient, &sw->pos, sw->outer_radius, OF_RENDERS );

	if ( objnum == -1 ){
		Int3();
	}

	sw->objnum = objnum;

	list_append(&Shockwave_list, sw);

	return objnum;
}

// ------------------------------------------------------------------------------------
// shockwave_delete()
//
// Delete a shockwave
//
//	input:	object *objp	=>		pointer to shockwave object
//
void shockwave_delete(object *objp)
{
	Assert(objp->type == OBJ_SHOCKWAVE);
	Assert(objp->instance >= 0 && objp->instance < MAX_SHOCKWAVES);

	Shockwaves[objp->instance].flags = 0;
	Shockwaves[objp->instance].objnum = -1;	
	list_remove(&Shockwave_list, &Shockwaves[objp->instance]);
}

// ------------------------------------------------------------------------------------
// shockwave_delete_all()
//
//
void shockwave_delete_all()
{
	shockwave	*sw, *next;
	
	sw = GET_FIRST(&Shockwave_list);
	while ( sw != &Shockwave_list ) {
		next = sw->next;
		Assert(sw->objnum != -1);
		Objects[sw->objnum].flags |= OF_SHOULD_BE_DEAD;
		sw = next;
	}
}

// Set the correct frame of animation for the shockwave
void shockwave_set_framenum(int index)
{
	int				framenum;
	shockwave		*sw;
	shockwave_info	*si;

	sw = &Shockwaves[index];
	si = &Shockwave_info[sw->shockwave_info_index];

	framenum = fl2i(sw->time_elapsed / sw->total_time * si->num_frames + 0.5);

	// ensure we don't go past the number of frames of animation
	if ( framenum > (si->num_frames-1) ) {
		framenum = (si->num_frames-1);
		Objects[sw->objnum].flags |= OF_SHOULD_BE_DEAD;
	}

	if ( framenum < 0 ) {
		framenum = 0;
	}

	sw->current_bitmap = si->bitmap_id + framenum;
}

// ------------------------------------------------------------------------------------
// shockwave_move()
//
//	Simulate a single shockwave.  If the shockwave radius exceeds outer_radius, then
// delete the shockwave.
//
//	input:		ojbp			=>		object pointer that points to shockwave object
//					frametime	=>		time to simulate shockwave
//
void shockwave_move(object *shockwave_objp, float frametime)
{
	shockwave	*sw;
	object		*objp;
	float			blast,damage;
	int			i;
	
	Assert(shockwave_objp->type == OBJ_SHOCKWAVE);
	Assert(shockwave_objp->instance  >= 0 && shockwave_objp->instance < MAX_SHOCKWAVES);
	sw = &Shockwaves[shockwave_objp->instance];

	// if the shockwave has a delay on it
	if(sw->delay_stamp != -1){
		if(timestamp_elapsed(sw->delay_stamp)){
			sw->delay_stamp = -1;
		} else {
			return;
		}
	}

	sw->time_elapsed += frametime;
/*
	if ( sw->time_elapsed > sw->total_time ) {
		shockwave_objp->flags |= OF_SHOULD_BE_DEAD;
	}
*/

	shockwave_set_framenum(shockwave_objp->instance);
		
	sw->radius += (frametime * sw->speed);
	if ( sw->radius > sw->outer_radius ) {
		sw->radius = sw->outer_radius;
		shockwave_objp->flags |= OF_SHOULD_BE_DEAD;
		return;
	}

	// blast ships and asteroids
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

		// only apply damage to a ship once from a shockwave
		for ( i = 0; i < sw->num_objs_hit; i++ ) {
			if ( objp->signature == sw->obj_sig_hitlist[i] ){
				break;
			}
		}
		if ( i < sw->num_objs_hit ){
			continue;
		}

		if ( weapon_area_calc_damage(objp, &sw->pos, sw->inner_radius, sw->outer_radius, sw->blast, sw->damage, &blast, &damage, sw->radius) == -1 ){
			continue;
		}

		// okay, we have damage applied, record the object signature so we don't repeatedly apply damage
		Assert(sw->num_objs_hit < SW_MAX_OBJS_HIT);
		if ( sw->num_objs_hit >= SW_MAX_OBJS_HIT) {
			sw->num_objs_hit--;
		}

		switch(objp->type) {
		case OBJ_SHIP:
			sw->obj_sig_hitlist[sw->num_objs_hit++] = objp->signature;
			ship_apply_global_damage(objp, shockwave_objp, &sw->pos, damage );
			weapon_area_apply_blast(NULL, objp, &sw->pos, blast, 1);
			break;
		case OBJ_ASTEROID:
			asteroid_hit(objp, NULL, NULL, damage);
			break;
		default:
			Int3();
			break;
		}


		// If this shockwave hit the player, play shockwave impact sound
		if ( objp == Player_obj ) {
			snd_play( &Snds[SND_SHOCKWAVE_IMPACT], 0.0f, max(0.4f, damage/Weapon_info[sw->weapon_info_index].damage) );
		}

	}	// end for
}

// ------------------------------------------------------------------------------------
// shockwave_render()
//
//	Draw the shockwave identified by handle
//
//	input:	objp	=>		pointer to shockwave object
//
void shockwave_render(object *objp)
{
	shockwave		*sw;
	shockwave_info	*si;
	vertex			p;

	Assert(objp->type == OBJ_SHOCKWAVE);
	Assert(objp->instance >= 0 && objp->instance < MAX_SHOCKWAVES);

	sw = &Shockwaves[objp->instance];
	si = &Shockwave_info[sw->shockwave_info_index];

	if( (sw->delay_stamp != -1) && !timestamp_elapsed(sw->delay_stamp)){
		return;
	}

	if ( sw->current_bitmap < 0 ){
		return;
	}

	// turn off fogging
	if(The_mission.flags & MISSION_FLAG_FULLNEB){
		gr_fog_set(GR_FOGMODE_NONE, 0, 0, 0, -1.0f, -1.0f);
	}

	g3_rotate_vertex(&p, &sw->pos );

	gr_set_bitmap(sw->current_bitmap, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, 1.3f, -1, -1);
	g3_draw_rotated_bitmap(&p, fl_radian(sw->rot_angle), sw->radius, TMAP_FLAG_TEXTURED);	
}

// ------------------------------------------------------------------------------------
// shockwave_init()
//
// Call once at the start of each level (mission)
//
void shockwave_level_init()
{
	int i;	
	shockwave_info	*si;

	// load in shockwaves
	for ( i=0; i<MAX_SHOCKWAVE_TYPES; i++ ) {
		si = &Shockwave_info[i];
		si->bitmap_id	= bm_load_animation( Shockwave_filenames[i], &si->num_frames, &si->fps, 1 );
		if ( si->bitmap_id < 0 ) {
			Error(LOCATION, "Could not load %s anim file\n", Shockwave_filenames[i]);
		}
	}
	
	list_init(&Shockwave_list);

	for ( i = 0; i < MAX_SHOCKWAVES; i++ ) {
		Shockwaves[i].flags = 0;
		Shockwaves[i].objnum = -1;
	}

	Shockwave_inited = 1;
}

// ------------------------------------------------------------------------------------
// shockwave_level_close()
//
// Call at the close of each level (mission)
void shockwave_level_close()
{
	if(Shockwave_inited){
		shockwave_delete_all();		
	}
	Shockwave_inited = 0;
}

// ------------------------------------------------------------------------------------
// shockwave_close()
//
//	Called at game-shutdown to 
//
void shockwave_close()
{
}

// ------------------------------------------------------------------------------------
// shockwave_move_all()
//
//	Simulate all shockwaves in Shockwave_list
//
//	input:	frametime	=>		time for last frame in ms
//
void shockwave_move_all(float frametime)
{
	shockwave	*sw, *next;
	
	sw = GET_FIRST(&Shockwave_list);
	while ( sw != &Shockwave_list ) {
		next = sw->next;
		Assert(sw->objnum != -1);
		shockwave_move(&Objects[sw->objnum], frametime);
		sw = next;
	}
}

// ------------------------------------------------------------------------------------
// shockwave_render_all()
//
//
void shockwave_render_all()
{
	shockwave	*sw, *next;
	
	sw = GET_FIRST(&Shockwave_list);
	while ( sw != &Shockwave_list ) {
		next = sw->next;
		Assert(sw->objnum != -1);
		shockwave_render(&Objects[sw->objnum]);
		sw = next;
	}
}

// return the weapon_info_index field for a shockwave
int shockwave_weapon_index(int index)
{
	return Shockwaves[index].weapon_info_index;
}

// return the maximum radius for specified shockwave
float	shockwave_max_radius(int index)
{
	return Shockwaves[index].outer_radius;
}

void shockwave_page_in()
{
	int i;
	shockwave_info	*si;

	// load in shockwaves
	for ( i=0; i<MAX_SHOCKWAVE_TYPES; i++ ) {
		si = &Shockwave_info[i];
		bm_page_in_texture( si->bitmap_id, si->num_frames );
	}

}
