/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Freespace2/LevelPaging.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Code to page in all the bitmaps at the beginning of a level.
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:17  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:44  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 4     8/19/99 10:12a Alanl
 * preload mission-specific messages on machines greater than 48MB
 * 
 * 3     8/10/99 6:54p Dave
 * Mad optimizations. Added paging to the nebula effect.
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 6     5/23/98 4:14p John
 * Added code to preload textures to video card for AGP.   Added in code
 * to page in some bitmaps that weren't getting paged in at level start.
 * 
 * 5     4/05/98 4:15p Dave
 * Fixed a weapons model paging problem with the standalone server.
 * 
 * 4     4/01/98 5:34p John
 * Made only the used POFs page in for a level.   Reduced some interp
 * arrays.    Made custom detail level work differently.
 * 
 * 3     3/29/98 4:05p John
 * New paging code that loads everything necessary at level startup.
 * 
 * 2     3/26/98 5:26p John
 * added new paging code. nonfunctional.
 * 
 * 1     3/26/98 5:14p John
 *
 * $NoKeywords: $
 */

#include "freespace.h"
#include "bmpman.h"
#include "levelpaging.h"

// All the page in functions
extern void ship_page_in();
extern void debris_page_in();
extern void particle_page_in();
extern void stars_page_in();
extern void hud_page_in();
extern void radar_page_in();
extern void weapons_page_in();
extern void fireballs_page_in();
extern void shockwave_page_in();
extern void shield_hit_page_in();
extern void asteroid_page_in();
extern void training_mission_page_in();
extern void neb2_page_in();
extern void message_pagein_mission_messages();

// Pages in all the texutures for the currently
// loaded mission.  Call game_busy() occasionally...
void level_page_in()
{

	mprintf(( "Beginning level bitmap paging...\n" ));

	if(!(Game_mode & GM_STANDALONE_SERVER)){		
		bm_page_in_start();
	}

	// Most important ones first
	ship_page_in();
	weapons_page_in();
	fireballs_page_in();
	particle_page_in();
	debris_page_in();
	hud_page_in();
	radar_page_in();
	training_mission_page_in();
	stars_page_in();
	shockwave_page_in();
	shield_hit_page_in();
	asteroid_page_in();
	neb2_page_in();

	// preload mission messages if NOT running low-memory (greater than 48MB)
	if (game_using_low_mem() == false) {
		message_pagein_mission_messages();
	}

	if(!(Game_mode & GM_STANDALONE_SERVER)){
		bm_page_in_stop();
	}

	mprintf(( "Ending level bitmap paging...\n" ));

}

