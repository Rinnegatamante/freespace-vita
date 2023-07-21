/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Object/CollideDebrisWeapon.cpp $
 * $Revision: 193 $
 * $Date: 2003-05-25 04:30:44 +0200 (Sun, 25 May 2003) $
 * $Author: taylor $
 *
 * Routines to detect collisions and do physics, damage, etc for weapons and debris
 *
 * $Log$
 * Revision 1.3  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.2  2002/06/09 04:41:24  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 4     7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 3     10/16/98 1:22p Andsager
 * clean up header files
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 7     4/02/98 6:29p Lawrance
 * compile out asteroid references for demo
 * 
 * 6     3/02/98 2:58p Mike
 * Make "asteroids" in debug console turn asteroids on/off.
 * 
 * 5     2/19/98 12:46a Lawrance
 * Further work on asteroids.
 * 
 * 4     2/05/98 12:51a Mike
 * Early asteroid stuff.
 * 
 * 3     1/13/98 8:09p John
 * Removed the old collision system that checked all pairs.   Added code
 * to disable collisions and particles.
 * 
 * 2     9/17/97 5:12p John
 * Restructured collision routines.  Probably broke a lot of stuff.
 * 
 * 1     9/17/97 2:14p John
 * Initial revision
 *
 * $NoKeywords: $
 */

#include "objcollide.h"
#include "asteroid.h"
#include "debris.h"
#include "fvi.h"

// placeholder struct for ship_debris collisions
typedef struct ship_weapon_debris_struct {
	object	*ship_object;
	object	*debris_object;
	vector	ship_collision_cm_pos;
	vector	r_ship;
	vector	collision_normal;
	int		shield_hit_tri;
	vector	shield_hit_tri_point;
	float		impulse;
} ship_weapon_debris_struct;


// Checks debris-weapon collisions.  pair->a is debris and pair->b is weapon.
// Returns 1 if all future collisions between these can be ignored
int collide_debris_weapon( obj_pair * pair )
{
	vector	hitpos;
	int		hit;
	object *pdebris = pair->a;
	object *weapon = pair->b;

	Assert( pdebris->type == OBJ_DEBRIS );
	Assert( weapon->type == OBJ_WEAPON );

	// first check the bounding spheres of the two objects.
	hit = fvi_segment_sphere(&hitpos, &weapon->last_pos, &weapon->pos, &pdebris->pos, pdebris->radius);
	if (hit) {
		hit = debris_check_collision(pdebris, weapon, &hitpos );
		if ( !hit )
			return 0;

		weapon_hit( weapon, pdebris, &hitpos );
		debris_hit( pdebris, weapon, &hitpos, Weapon_info[Weapons[weapon->instance].weapon_info_index].damage );
		return 0;

	} else {
		return weapon_will_never_hit( weapon, pdebris, pair );
	}
}				



// Checks debris-weapon collisions.  pair->a is debris and pair->b is weapon.
// Returns 1 if all future collisions between these can be ignored
int collide_asteroid_weapon( obj_pair * pair )
{
#if !(defined(FS2_DEMO) || defined(FS1_DEMO))

	if (!Asteroids_enabled)
		return 0;

	vector	hitpos;
	int		hit;
	object	*pasteroid = pair->a;
	object	*weapon = pair->b;

	Assert( pasteroid->type == OBJ_ASTEROID);
	Assert( weapon->type == OBJ_WEAPON );

	// first check the bounding spheres of the two objects.
	hit = fvi_segment_sphere(&hitpos, &weapon->last_pos, &weapon->pos, &pasteroid->pos, pasteroid->radius);
	if (hit) {
		hit = asteroid_check_collision(pasteroid, weapon, &hitpos );
		if ( !hit )
			return 0;

		weapon_hit( weapon, pasteroid, &hitpos );
		asteroid_hit( pasteroid, weapon, &hitpos, Weapon_info[Weapons[weapon->instance].weapon_info_index].damage );
		return 0;

	} else {
		return weapon_will_never_hit( weapon, pasteroid, pair );
	}

#else
	return 0;
#endif
}				


