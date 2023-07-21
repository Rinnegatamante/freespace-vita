/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/UI/UiDefs.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Include file for stuff used internally by the UI code.
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 8     4/01/98 5:07p Hoffoss
 * Changed mouse button handling for UI in order to track fast events
 * (like button going down and released all between frames).
 * 
 * 7     2/03/98 4:21p Hoffoss
 * Made UI controls draw white text when disabled.
 * 
 * 6     6/12/97 12:39p John
 * made ui use freespace colors
 * 
 * 5     6/11/97 1:13p John
 * Started fixing all the text colors in the game.
 * 
 * 4     12/08/96 1:58a Lawrance
 * added some additional colors 
 * 
 * 3     12/02/96 2:17p John
 * Made right button drag UI gadgets around and
 * Ctrl+Shift+Alt+F12 dumps out where they are.
 * 
 * 2     11/15/96 11:43a John
 * 
 * 1     11/14/96 6:55p John
 *
 * $NoKeywords: $
 */

#ifndef _UIDEFS_H
#define _UIDEFS_H

#include "pstypes.h"
#include "key.h"
#include "mouse.h"
#include "2d.h"
#include "freespace.h"

#define CBLACK Color_black
#define CGREEN Color_green
#define CBRIGHT_GREEN Color_bright_green
#define CGRAY Color_grey
#define CDARK_GRAY Color_bright_white  // since gray doesn't work with our current color system..
#define CWHITE Color_white
#define CBRIGHT Color_bright_white

#define BORDER_WIDTH 8

void ui_hline(int x1, int x2, int y );
void ui_vline(int y1, int y2, int x );
void ui_string_centered( int x, int y, const char * s );
void ui_draw_shad( int x1, int y1, int x2, int y2, int r1, int g1, int b1, int r2, int g2, int b2 );
void ui_draw_frame( int x1, int y1, int x2, int y2 );
void ui_rect( int x1, int y1, int x2, int y2 );
void ui_draw_box_out( int x1, int y1, int x2, int y2 );
void ui_draw_box_in( int x1, int y1, int x2, int y2 );
void ui_draw_line_in( int x1, int y1, int x2, int y2 );
void ui_draw_sunken_border( int x1, int y1, int x2, int y2 );

#define BUTTON_PRESSED          1
#define BUTTON_RELEASED         2
#define BUTTON_JUST_PRESSED     4
#define BUTTON_JUST_RELEASED    8
#define BUTTON_DOUBLE_CLICKED   16

#define B1_PRESSED          (ui_mouse.b1_status & BUTTON_PRESSED)
#define B1_RELEASED         (ui_mouse.b1_status & BUTTON_RELEASED)
#define B1_JUST_PRESSED     (ui_mouse.b1_status & BUTTON_JUST_PRESSED)
#define B1_JUST_RELEASED    (ui_mouse.b1_status & BUTTON_JUST_RELEASED)
#define B1_DOUBLE_CLICKED   (ui_mouse.b1_status & BUTTON_DOUBLE_CLICKED)

#define B2_PRESSED          (ui_mouse.b2_status & BUTTON_PRESSED)
#define B2_RELEASED         (ui_mouse.b2_status & BUTTON_RELEASED)
#define B2_JUST_PRESSED     (ui_mouse.b2_status & BUTTON_JUST_PRESSED)
#define B2_JUST_RELEASED    (ui_mouse.b2_status & BUTTON_JUST_RELEASED)

typedef struct UI_MOUSE	{
	int	x, y;
	int	dx, dy;
	int	b1_status;
	int	b1_last_status;
	int	b1_time_lastpressed;
	int	b2_status;
	int	b2_last_status;
	int	b2_time_lastpressed;
	int	timestamp;
} UI_MOUSE;

extern UI_MOUSE ui_mouse;
extern void ui_mouse_process();


#define Middle(x) ((x)/2)

#endif

