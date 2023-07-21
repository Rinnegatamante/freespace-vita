/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MenuUI/TechMenu.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for code that controls the Tech Room menu
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 5     8/23/99 11:20a Jefff
 * Increased TECH_INTEL_DESC_LEN
 * 
 * 4     8/10/99 3:45p Jefff
 * Put the smack down on the tech room.  Its all new, but tastefully done.
 * 
 * 3     10/13/98 2:47p Andsager
 * Remove reference to Tech_shivan_species_avail
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 11    5/05/98 1:49a Lawrance
 * Add in missing help overlays
 * 
 * 10    4/23/98 10:42p Hoffoss
 * Added species section to techroom.  Still missing description text,
 * because this hasn't been created yet.
 * 
 * 9     4/14/98 10:24p Hoffoss
 * Started on new tech room.
 * 
 * 8     4/02/98 5:40p Hoffoss
 * Added the Load Mission screen to FreeSpace.
 * 
 * 7     8/31/97 6:38p Lawrance
 * pass in frametime to do_frame loop
 * 
 * 6     11/21/96 7:14p Lawrance
 * converted menu code to use a file (menu.tbl) to get the data for the
 * menu
 * 
 * 5     11/13/96 4:02p Lawrance
 * complete over-haul of the menu system and the states associated with
 * them
 * 
 * 4     11/13/96 8:32a Lawrance
 * streamlined menu code
 * 
 * 3     11/06/96 8:54a Lawrance
 * added revision templates, made more efficient
 *
 * $NoKeywords: $
 *
*/


#ifndef _TECHMENU_H
#define _TECHMENU_H

#define MAX_INTEL_ENTRIES			10
#define TECH_INTEL_DESC_LEN		5120

typedef struct {
	char name[32];
	char desc[TECH_INTEL_DESC_LEN];
	char anim_filename[32];
	int  in_tech_db;							// determines if visible in tech db or not
} intel_data;


extern intel_data Intel_info[MAX_INTEL_ENTRIES];
extern int Intel_info_size;


// function prototypes
void techroom_init();
void techroom_close();
void techroom_do_frame(float frametime);
int techroom_on_ships_tab();
void techroom_intel_init();			// called on startup so campaigns can manipulate tech room visibility

#endif

