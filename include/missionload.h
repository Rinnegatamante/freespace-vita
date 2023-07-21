/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Mission/MissionLoad.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Mission load header file
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 20    2/23/98 6:55p Lawrance
 * Rip out obsolete code.
 * 
 * 19    6/26/97 5:53p Lawrance
 * save recently played missions, allow player to choose from list
 * 
 * 18    4/25/97 11:31a Allender
 * Campaign state now saved in campaign save file in player directory.
 * Made some global variables follow naming convention.  Solidified
 * continuing campaigns based on new structure
 *
 * $NoKeywords: $
 */

#ifndef _MISSIONLOAD_H
#define _MISSIONLOAD_H

#include "pstypes.h"

// -----------------------------------------------
// For recording most recent missions played
// -----------------------------------------------
#define			MAX_RECENT_MISSIONS	10
extern	char	Recent_missions[MAX_RECENT_MISSIONS][MAX_FILENAME_LEN];
extern	int	Num_recent_missions;

// Mission_load takes no parameters.
// It expects the following variables to be set correctly:
// Game_current_mission_filename

int mission_load();
void mission_init();

// Functions for mission load menu
void mission_load_menu_init();
void mission_load_menu_close();
void mission_load_menu_do();

#endif

