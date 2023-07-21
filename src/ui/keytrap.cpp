/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/UI/KEYTRAP.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Routines for gadgets that trap keypresses.
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:29  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:53  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 2     1/14/98 6:43p Hoffoss
 * Massive changes to UI code.  A lot cleaner and better now.  Did all
 * this to get the new UI_DOT_SLIDER to work properly, which the old code
 * wasn't flexible enough to handle.
 * 
 * 1     11/14/96 6:55p John
 *
 * $NoKeywords: $
 */

#include "uidefs.h"
#include "ui.h"


void UI_KEYTRAP::create(UI_WINDOW *wnd, int key, void (*_user_function)(void) )
{
	base_create( wnd, UI_KIND_BUTTON, 0, 0, 0, 0 );

	pressed_down = 0;
	set_hotkey(key);
	set_callback(_user_function);

	parent = this;		// Ugly.  This keeps KEYTRAPS from getting keyboard control.
};

void UI_KEYTRAP::draw()
{
}

void UI_KEYTRAP::process(int focus)
{
	pressed_down = 0;

	if (disabled_flag) {
		return;
	}

	if (my_wnd->keypress == hotkey)	{
		pressed_down = 1;
		my_wnd->last_keypress = 0;
	}

	if (pressed_down && user_function)	{
		user_function();
	}
}

int UI_KEYTRAP::pressed()
{
	return pressed_down;
}

