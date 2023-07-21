/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Ship/Afterburner.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for managing the afterburners
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 6     2/26/98 10:07p Hoffoss
 * Rewrote state saving and restoring to fix bugs and simplify the code.
 * 
 * 5     8/05/97 10:48a Lawrance
 * save afterburner data for the player
 * 
 * 4     7/27/97 5:14p Lawrance
 * add afterburners to the player control info
 * 
 * 3     7/16/97 4:42p Mike
 * Make afterburner shake viewer, not ship.
 * Shake for limited time.
 * Add timestamp_until() function to timer library.
 * 
 * 2     7/11/97 8:57a Lawrance
 * make afterburner work same for player and AI ships
 * 
 * 1     7/10/97 2:24p Lawrance
 *
 * $NoKeywords: $
 */

#ifndef __AFTERBURNER_H__
#define __AFTERBURNER_H__

#define	AFTERBURNER_MIN_DELAY	1000
#define	ABURN_DECAY_TIME			1500		// time in ms ship shakes after afterburner pressed

#include "cfile.h"
#include "object.h"

void afterburners_start(object *objp);
void afterburners_stop(object *objp, int key_released = 0);
void afterburner_stop_sounds();
void afterburners_update(object *objp, float fl_frametime);
void afterburner_level_init();

#endif /* __AFTERBURNER_H__ */

