/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Io/Mouse.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Include file for mouse reading routines
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
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
 * 17    5/11/98 5:29p Hoffoss
 * Added mouse button mapped to joystick button support.
 * 
 * 16    5/07/98 6:58p Hoffoss
 * Made changes to mouse code to fix a number of problems.
 * 
 * 15    5/05/98 8:38p Hoffoss
 * Added sensitivity adjustment to options menu and made it save to pilot
 * file.
 * 
 * 14    5/01/98 5:45p Hoffoss
 * Made further improvements to the mouse code.
 * 
 * 13    5/01/98 1:14p Hoffoss
 * Changed mouse usage so directInput is only used for release version.
 * 
 * 12    4/30/98 5:40p Hoffoss
 * Added mouse as a supported control to fly the ship.
 * 
 * 11    4/29/98 12:13a Lawrance
 * Add function to check down count of mouse button without reseting the
 * internal count.  Added hook to reset demo trailer timer when a button
 * is pressed.
 * 
 * 10    4/02/98 5:26p John
 * 
 * 9     12/04/97 3:47p John
 * Made joystick move mouse cursor
 * 
 * 8     11/20/97 5:36p Dave
 * Hooked in a bunch of main hall changes (including sound). Made it
 * possible to reposition (rewind/ffwd) 
 * sound buffer pointers. Fixed animation direction change framerate
 * problem.
 * 
 * 7     4/22/97 12:32p John
 * added mouse_init function
 * 
 * 6     3/26/97 10:52a Lawrance
 * mouse always on in menus, disappears in gameplay after 1 second
 * 
 * 5     3/11/97 1:37p Lawrance
 * added mouse_up_count(), changed mouse_mark() to mouse_mark_button() &
 * mouse_mark_move()
 * 
 * 4     2/17/97 5:18p John
 * Added a bunch of RCS headers to a bunch of old files that don't have
 * them.
 *
 * $NoKeywords: $
 */

#ifndef _MOUSE_H
#define _MOUSE_H

#include "pstypes.h"

extern int Mouse_sensitivity;
extern int Use_mouse_to_fly;
extern int Mouse_hidden;
extern int Keep_mouse_centered;

// call once to init the mouse
void mouse_init();

extern void mouse_mark_button( uint flags, int set );

// Fills in xpos & ypos if not NULL.
// Returns Button states
// Always returns coordinates clipped to screen coordinates.
extern int mouse_get_pos( int *xpos, int *ypos );

// get_real_pos could be negative.
extern void mouse_get_real_pos(int *mx, int *my);

extern void mouse_set_pos(int xpos,int ypos);

#define MOUSE_LEFT_BUTTON		(1<<0)
#define MOUSE_RIGHT_BUTTON		(1<<1)
#define MOUSE_MIDDLE_BUTTON	(1<<2)

#define MOUSE_NUM_BUTTONS		3

// keep the following two #defines up to date with the #defines above
#define LOWEST_MOUSE_BUTTON	(1<<0)
#define HIGHEST_MOUSE_BUTTON	(1<<2)

// Returns the number of times button n went from up to down since last call
int mouse_down_count(int n, int reset_count = 1);
// Returns the number of times button n went from down to up since last call
int mouse_up_count(int n);

extern void mouse_flush();

int mouse_down(int btn);			// returns 1 if mouse button btn is down, 0 otherwise
float mouse_down_time(int btn);	// returns the fraction of time btn has been down since last call
int mouse_is_visible();				// returns 1 if mouse is visible, 0 otherwise

void mouse_eval_deltas();
void mouse_get_delta(int *dx = NULL, int *dy = NULL, int *dz = NULL);

#endif

