/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Weapon/Trails.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * External defs for missile trail stuff
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     11/14/98 5:33p Dave
 * Lots of nebula work. Put in ship contrails.
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 3     3/23/98 5:00p John
 * Improved missile trails.  Made smooth alpha under hardware.  Made end
 * taper.  Made trail touch weapon.
 * 
 * 2     12/21/97 6:15p John
 * Made a seperate system for missile trails
 * 
 * 1     12/21/97 5:30p John
 * Initial version
 *
 * $NoKeywords: $
 */

#ifndef _TRAILS_H
#define _TRAILS_H

#include "pstypes.h"

#define NUM_TRAIL_SECTIONS 16

// contrail info - similar to that for missile trails
// place this inside of info structures instead of explicit structs (eg. ship_info instead of ship, or weapon_info instead of weapon)
typedef struct trail_info {
	vector pt;				// offset from the object's center
	float w_start;			// starting width
	float w_end;			// ending width
	float a_start;			// starting alpha
	float a_end;			// ending alpha
	float max_life;		// max_life for a section
	int stamp;				// spew timestamp
	int bitmap;				// bitmap to use
} trail_info;

// Call at start of level to reinit all missilie trail stuff
void trail_level_init();

// Needs to be called from somewhere to move the trails each frame
void trail_move_all(float frametime);

// Needs to be called from somewhere to render the trails each frame
void trail_render_all();

// The following functions are what the weapon code calls
// to deal with trails:

// Returns -1 if failed
int trail_create(trail_info info);
void trail_add_segment( int trail_num, vector *pos );
void trail_set_segment( int trail_num, vector *pos );
void trail_object_died( int trail_num );
int trail_stamp_elapsed( int trail_num );
void trail_set_stamp( int trail_num );

#endif //_TRAILS_H

