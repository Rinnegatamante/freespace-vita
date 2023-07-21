/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Object/ObjCollide.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for all the Collide????.cpp modules
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 9     4/21/99 6:15p Dave
 * Did some serious housecleaning in the beam code. Made it ready to go
 * for anti-fighter "pulse" weapons. Fixed collision pair creation. Added
 * a handy macro for recalculating collision pairs for a given object.
 * 
 * 8     11/19/98 11:08p Andsager
 * Check in of physics and collision detection of rotating submodels
 * 
 * 7     11/17/98 4:33p Andsager
 * add submodel_rot_hit to collision_info struct
 * 
 * 6     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 5     10/23/98 1:11p Andsager
 * Make ship sparks emit correctly from rotating structures.
 * 
 * 4     10/20/98 1:39p Andsager
 * Make so sparks follow animated ship submodels.  Modify
 * ship_weapon_do_hit_stuff() and ship_apply_local_damage() to add
 * submodel_num.  Add submodel_num to multiplayer hit packet.
 * 
 * 3     10/16/98 1:22p Andsager
 * clean up header files
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 14    5/08/98 5:25p Lawrance
 * Don't allow collision sounds too play over each so much
 * 
 * 13    4/24/98 5:35p Andsager
 * Fix sparks sometimes drawing not on model.  If ship is sphere in
 * collision, don't draw sparks.  Modify ship_apply_local_damage() to take
 * parameter no_spark.
 * 
 * 12    4/01/98 1:48p Allender
 * major changes to ship collision in multiplayer.  Clients now do own
 * ship/ship collisions (with their own ship only)  Modifed the hull
 * update packet to be sent quicker when object is target of player.
 * 
 * 11    2/26/98 10:07p Hoffoss
 * Rewrote state saving and restoring to fix bugs and simplify the code.
 * 
 * 10    2/05/98 12:51a Mike
 * Early asteroid stuff.
 * 
 * 9     2/04/98 6:08p Lawrance
 * Add a light collision sound, overlay a shield collide sound if
 * applicable.
 * 
 * 8     1/13/98 3:11p Allender
 * new code to remove old weapons when no more weapon slots available.
 * Currently not called anywhere pending further testing
 * 
 * 7     1/12/98 9:26p Andsager
 * Implement collisions from rotation.
 * 
 * 6     1/08/98 12:12a Mike
 * Make ships turn before warping out, if necessary, to avoid a collision.
 * Warn player if his warpout will collide.  Abort if in stage1.
 * 
 * 5     1/05/98 9:07p Andsager
 * Changed ship_shipor_debris_hit_info struct to more meaninful names
 * 
 * 4     12/22/97 9:56p Andsager
 * Implement ship:debris collisions.  Generalize and move
 * ship_ship_or_debris_hit struct from CollideShipShip to ObjCollide.h
 * 
 * 3     10/28/97 4:57p John
 * Put Andsager's new sphereline collide code officially into the code
 * base and did a little restructuring.  Fixed a few little bugs with it
 * and added some simple bounding box elimination and did some timings.
 * 
 * 
 * 2     10/25/97 10:13a Andsager
 * Added SPHERE_POLY_CHECK to central location used by ModelCollide.cpp
 * and Collide_ship_ship.cpp
 * 
 * 1     9/17/97 2:14p John
 * Initial revision
 *
 * $NoKeywords: $
 */

#ifndef _COLLIDESTUFF_H
#define _COLLIDESTUFF_H

#include "pstypes.h"

struct object;
struct CFILE;
struct mc_info;

// used for ship:ship and ship:debris
typedef struct collision_info_struct {
	object	*heavy;
	object	*light;
	vector	heavy_collision_cm_pos;	// should be zero
	vector	light_collision_cm_pos;	// relative cm collision pos
	vector	r_heavy;						// relative to A
	vector	r_light;						// relative to B
	vector	hit_pos;					// relative hit position in A's rf (r_heavy)
	vector	collision_normal;		// normal outward from heavy
	float		hit_time;				// time normalized [0,1] when sphere hits model
	float		impulse;					// damage scales according to impulse
	vector	light_rel_vel;			// velocity of light relative to heavy before collison
	int		collide_rotate;		// if collision is detected purely from rotation
	int		submodel_num;			// submodel of heavy object that is hit
	int		edge_hit;				// if edge is hit, need to change collision normal
	int		submodel_rot_hit;		// if collision is against rotating submodel
} collision_info_struct;


//===============================================================================
// GENERAL COLLISION DETECTION HELPER FUNCTIONS 
// These are in CollideGeneral.cpp and are used by one or more of the collision-
// type specific collision modules.
//===============================================================================

// Keeps track of pairs of objects for collision detection
typedef struct obj_pair	{
	object *a;
	object *b;
	int (*check_collision)( obj_pair * pair );
	int	next_check_time;	// a timestamp that when elapsed means to check for a collision
	struct obj_pair *next;
} obj_pair;


#define COLLISION_OF(a,b) (((a)<<8)|(b))

#define COLLISION_TYPE_NONE	0	
#define COLLISION_TYPE_OLD		1	// checks all n objects with each other each frame
#define COLLISION_TYPE_NEW		2	// keeps track of collision pairs.  throws out collisions that won't happen.

extern int collision_type;

#define SUBMODEL_NO_ROT_HIT	0
#define SUBMODEL_ROT_HIT		1
void set_hit_struct_info(collision_info_struct *hit, mc_info *mc, int submodel_rot_hit);

void obj_reset_pairs();
void obj_add_pair( object *A, object *B, int check_time = -1, int add_to_end = 0 );

void obj_check_all_collisions();

// Returns TRUE if the weapon will never hit the other object.
// If it can it predicts how long until these two objects need
// to be checked and fills the time in in current_pair.
// CODE is locatated in CollideGeneral.cpp
int weapon_will_never_hit( object *weapon, object *other, obj_pair * current_pair );


//	See if two lines intersect by doing recursive subdivision.
//	Bails out if larger distance traveled is less than sum of radii + 1.0f.
// CODE is locatated in CollideGeneral.cpp
int collide_subdivide(vector *p0, vector *p1, float prad, vector *q0, vector *q1, float qrad);


//===============================================================================
// SPECIFIC COLLISION DETECTION FUNCTIONS 
//===============================================================================

// Checks weapon-weapon collisions.  pair->a and pair->b are weapons.
// Returns 1 if all future collisions between these can be ignored
// CODE is locatated in CollideWeaponWeapon.cpp
int collide_weapon_weapon( obj_pair * pair );

// Checks ship-weapon collisions.  pair->a is ship and pair->b is weapon.
// Returns 1 if all future collisions between these can be ignored
// CODE is locatated in CollideShipWeapon.cpp
int collide_ship_weapon( obj_pair * pair );
void ship_weapon_do_hit_stuff(object *ship_obj, object *weapon_obj, vector *world_hitpos, vector *hitpos, int quadrant_num, int submodel_num = -1);

// Checks debris-weapon collisions.  pair->a is debris and pair->b is weapon.
// Returns 1 if all future collisions between these can be ignored
// CODE is locatated in CollideDebrisWeapon.cpp
int collide_debris_weapon( obj_pair * pair );

// Checks debris-ship collisions.  pair->a is debris and pair->b is ship.
// Returns 1 if all future collisions between these can be ignored
// CODE is locatated in CollideDebrisShip.cpp
int collide_debris_ship( obj_pair * pair );

int collide_asteroid_ship(obj_pair *pair);
int collide_asteroid_weapon(obj_pair *pair);

// Checks ship-ship collisions.  pair->a and pair->b are ships.
// Returns 1 if all future collisions between these can be ignored
// CODE is locatated in CollideShipShip.cpp
int collide_ship_ship( obj_pair * pair );

//	Predictive functions.
//	Returns true if vector from curpos to goalpos with radius radius will collide with object goalobjp
int pp_collide(vector *curpos, vector *goalpos, object *goalobjp, float radius);

//	Return true if objp will collide with some large ship if it moves distance distance.
int collide_predict_large_ship(object *objp, float distance);

// function to remove old weapons when no more weapon slots available.
int collide_remove_weapons(void);

void collide_ship_ship_do_sound(vector *world_hit_pos, object *A, object *B, int player_involved);
void collide_ship_ship_sounds_init();

int get_ship_quadrant_from_global(vector *global_pos, object *objp);

#endif

