/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MenuUI/MainHallMenu.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for main-hall menu code
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     8/26/99 9:45a Dave
 * First pass at easter eggs and cheats.
 * 
 * 3     6/03/99 10:15p Dave
 * Put in temporary main hall screen.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 7     4/25/98 2:00p Dave
 * Installed a bunch of multiplayer context help screens. Reworked ingame
 * join ship select screen. Fix places where network timestamps get hosed.
 * 
 * 6     3/31/98 7:09p Dave
 * Changed around main hall code to make way for multiple different halls.
 * 
 * 5     2/05/98 7:13p Dave
 * Put in new misc animation sound triggers.
 * 
 * 4     12/06/97 1:11a Lawrance
 * make a general interface for help overlays
 * 
 * 3     12/05/97 2:39p Lawrance
 * added some different sounds to main hall, add support for looping
 * ambient sounds
 * 
 * 2     12/01/97 4:59p Dave
 * Synchronized multiplayer debris objects. Put in pilot popup in main
 * hall. Optimized simulated multiplayer lag module. Fixed a potential
 * file_xfer bug.
 * 
 * 1     11/19/97 8:30p Dave
 * 
 * $NoKeywords: $
 *
 */

#ifndef _MAIN_HALL_MENU_HEADER_FILE
#define _MAIN_HALL_MENU_HEADER_FILE

// the # of main halls we're supporting
#define NUM_MAIN_HALLS			2

// initialize the main hall proper 
void main_hall_init(int main_hall_num);

// do a frame for the main hall
void main_hall_do(float frametime);

// close the main hall proper
void main_hall_close();

// start the main hall music playing
void main_hall_start_music();

// stop the main hall music
void main_hall_stop_music();

// what main hall we're on (should be 0 or 1)
int main_hall_id();

// start the ambient sounds playing in the main hall
void main_hall_start_ambient();
void main_hall_stop_ambient();
void main_hall_reset_ambient_vol();

void main_hall_do_multi_ready();

// make the vasudan main hall funny
void main_hall_vasudan_funny();

#endif

