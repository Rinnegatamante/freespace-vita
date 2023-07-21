/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Ship/AWACS.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * all sorts of cool stuff about ships
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     6/02/99 12:52p Andsager
 * Added team-wide ship visibility.  Implemented for player.
 * 
 * 3     1/25/99 5:03a Dave
 * First run of stealth, AWACS and TAG missile support. New mission type
 * :)
 * 
 * 2     1/08/99 2:08p Dave
 * Fixed software rendering for pofview. Super early support for AWACS and
 * beam weapons.
 * 
 * 
 * $NoKeywords: $
 */

#ifndef __FS2_AWACS_HEADER_FILE
#define __FS2_AWACS_HEADER_FILE

// ----------------------------------------------------------------------------------------------------
// AWACS DEFINES/VARS
//

// DAVE'S OFFICIAL DEFINITION OF AWACS

// total awacs levels for all teams
extern float Awacs_team[MAX_TEAMS];	// total AWACS capabilities for each team
extern float Awacs_level;				// Awacs_friendly - Awacs_hostile

// ----------------------------------------------------------------------------------------------------
// AWACS FUNCTIONS
//

// call when initializing level, before parsing mission
void awacs_level_init();

// call every frame to process AWACS details
void awacs_process();

// get the total AWACS level for target to viewer
// < 0.0f		: untargetable
// 0.0 - 1.0f	: marginally targetable
// 1.0f			: fully targetable as normal
float awacs_get_level(object *target, ship *viewer, int use_awacs=1);

// Determine if ship is visible by team
// return 1 if ship is fully visible
// return 0 if ship is only partly visible
int ship_is_visible_by_team(int ship_num, int team);


#endif

