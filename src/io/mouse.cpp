/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Io/Mouse.cpp $
 * $Revision: 193 $
 * $Date: 2003-05-25 04:30:44 +0200 (Sun, 25 May 2003) $
 * $Author: taylor $
 *
 * Routines to read the mouse.
 *
 * $Log$
 * Revision 1.7  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.6  2002/07/13 06:46:48  theoddone33
 * Warning cleanups
 *
 * Revision 1.5  2002/06/09 04:41:22  relnev
 * added copyright header
 *
 * Revision 1.4  2002/06/02 04:26:34  relnev
 * warning cleanup
 *
 * Revision 1.3  2002/05/29 06:25:13  theoddone33
 * Keyboard input, mouse tracking now work
 *
 * Revision 1.2  2002/05/07 03:16:46  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 4     7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 3     6/02/99 6:18p Dave
 * Fixed TNT lockup problems! Wheeeee!
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 29    6/10/98 2:52p Hoffoss
 * Made mouse code use DI by default, but fall back on normal code if that
 * fails.
 * 
 * 28    5/24/98 1:35p Hoffoss
 * Fixed bug where  mouse cursor is always recentering with a
 * mouse_flush() call in debug version.
 * 
 * 27    5/22/98 4:50p Hoffoss
 * Trying to fix mouse acceleration problem..
 * 
 * 26    5/21/98 12:26p Lawrance
 * Fixed mouse jerk at mission start while in debug build only.
 * 
 * 25    5/15/98 2:41p Hoffoss
 * Made mouse default to off (for flying ship) and fixed some new pilot
 * init bugs.
 * 
 * 24    5/08/98 4:13p Hoffoss
 * Fixed problem with mouse pointer centering causing lost keypresses.
 * 
 * 23    5/07/98 6:58p Hoffoss
 * Made changes to mouse code to fix a number of problems.
 * 
 * 22    5/05/98 8:38p Hoffoss
 * Added sensitivity adjustment to options menu and made it save to pilot
 * file.
 * 
 * 21    5/05/98 1:03p Hoffoss
 * Fixed initialization bug.
 * 
 * 20    5/01/98 5:45p Hoffoss
 * Made further improvements to the mouse code.
 * 
 * 19    5/01/98 3:35p Hoffoss
 * Made changes to release version of mouse code.
 * 
 * 18    5/01/98 1:14p Hoffoss
 * Changed mouse usage so directInput is only used for release version.
 * 
 * 17    4/30/98 5:40p Hoffoss
 * Added mouse as a supported control to fly the ship.
 * 
 * 16    4/29/98 12:13a Lawrance
 * Add function to check down count of mouse button without reseting the
 * internal count.  Added hook to reset demo trailer timer when a button
 * is pressed.
 * 
 * 15    4/02/98 5:26p John
 * 
 * 14    1/19/98 6:15p John
 * Fixed all my Optimized Build compiler warnings
 * 
 * 13    12/04/97 3:47p John
 * Made joystick move mouse cursor
 * 
 * 12    11/20/97 5:36p Dave
 * Hooked in a bunch of main hall changes (including sound). Made it
 * possible to reposition (rewind/ffwd) 
 * sound buffer pointers. Fixed animation direction change framerate
 * problem.
 * 
 * 11    5/12/97 11:41a John
 * Added range checking to mouse position
 * 
 * 10    4/22/97 5:55p Lawrance
 * let mouse.cpp decide if mouse has moved
 * 
 * 9     4/22/97 12:29p John
 * Changed mouse code so that you have to call mouse_init for the mouse
 * stuff to work.
 * 
 * 8     4/22/97 10:56a John
 * fixed some resource leaks.
 * 
 * 7     3/26/97 10:52a Lawrance
 * mouse always on in menus, disappears in gameplay after 1 second
 * 
 * 6     3/11/97 1:37p Lawrance
 * added mouse_up_count(), changed mouse_mark() to mouse_mark_button() &
 * mouse_mark_move()
 * 
 * 5     12/09/96 1:29p Lawrance
 * adding 3 button support
 * 
 * 4     12/03/96 4:19p John
 * Added some code so that holding down the mouse buttons between menus
 * doesn't select the next menu.
 *
 * $NoKeywords: $
 */

#ifndef PLAT_UNIX
#include <windows.h>
#include <windowsx.h>
#endif

#include "mouse.h"
#include "2d.h"
#include "osapi.h"

#define MOUSE_MODE_DI	0
#define MOUSE_MODE_WIN	1

#ifdef NDEBUG
LOCAL int Mouse_mode = MOUSE_MODE_DI;
#else
LOCAL int Mouse_mode = MOUSE_MODE_WIN;
#endif

LOCAL int mouse_inited = 0;
#ifndef PLAT_UNIX
LOCAL int Di_mouse_inited = 0;
#endif
LOCAL int Mouse_x;
LOCAL int Mouse_y;

CRITICAL_SECTION mouse_lock;

// #define USE_DIRECTINPUT

int mouse_flags;
int mouse_left_pressed = 0;
int mouse_right_pressed = 0;
int mouse_middle_pressed = 0;
int mouse_left_up = 0;
int mouse_right_up = 0;
int mouse_middle_up = 0;
int Mouse_dx = 0;
int Mouse_dy = 0;
int Mouse_dz = 0;

int Mouse_sensitivity = 9;/*SEB 4*/
int Use_mouse_to_fly = 1;/*SEB 0*/
int Mouse_hidden = 0;
int Keep_mouse_centered = 0;;

int di_init();
void di_cleanup();
void mouse_force_pos(int x, int y);
void mouse_eval_deltas_di();

int mouse_is_visible()
{
	return !Mouse_hidden;
}

void mouse_close()
{
	if (!mouse_inited)
		return;

#ifdef USE_DIRECTINPUT
	di_cleanup();
#endif
	mouse_inited = 0;
#ifdef PLAT_UNIX
	STUB_FUNCTION;
#else
	DeleteCriticalSection( &mouse_lock );
#endif
}

void mouse_init()
{
	// Initialize queue
	if ( mouse_inited ) return;
	mouse_inited = 1;

#ifdef PLAT_UNIX
	STUB_FUNCTION;
#else
	InitializeCriticalSection( &mouse_lock );
#endif

	ENTER_CRITICAL_SECTION(&mouse_lock);

	mouse_flags = 0;
	Mouse_x = gr_screen.max_w / 2;
	Mouse_y = gr_screen.max_h / 2;

#ifdef USE_DIRECTINPUT
	if (!di_init())
		Mouse_mode = MOUSE_MODE_WIN;
#else
	Mouse_mode = MOUSE_MODE_WIN;
#endif

	LEAVE_CRITICAL_SECTION(&mouse_lock);	

	atexit( mouse_close );
}


// ----------------------------------------------------------------------------
// mouse_mark_button() is called asynchronously by the OS when a mouse button
// goes up or down.  The mouse button that is affected is passed via the 
// flags parameter.  
//
// parameters:   flags ==> mouse button pressed/released
//               set   ==> 1 - button is pressed
//                         0 - button is released

void mouse_mark_button( uint flags, int set)
{
	if ( !mouse_inited ) return;

	ENTER_CRITICAL_SECTION(&mouse_lock);

	if ( !(mouse_flags & MOUSE_LEFT_BUTTON) )	{

		if ( (flags & MOUSE_LEFT_BUTTON) && (set == 1) ) {
			mouse_left_pressed++;

////////////////////////////
/// SOMETHING TERRIBLE IS ABOUT TO HAPPEN.  I FEEL THIS IS NECESSARY FOR THE DEMO, SINCE
/// I DON'T WANT TO CALL CRITICAL SECTION CODE EACH FRAME TO CHECK THE LEFT MOUSE BUTTON.
/// PLEASE SEE ALAN FOR MORE INFORMATION.
////////////////////////////
#if defined(FS2_DEMO) || defined(FS1_DEMO)
					{
					extern void demo_reset_trailer_timer();
					demo_reset_trailer_timer();
					}
#endif
////////////////////////////
/// IT'S OVER.  SEE, IT WASN'T SO BAD RIGHT?  IT'S IS VERY UGLY LOOKING, I KNOW.
////////////////////////////

		}
	}
	else {
		if ( (flags & MOUSE_LEFT_BUTTON) && (set == 0) ){
			mouse_left_up++;
		}
	}

	if ( !(mouse_flags & MOUSE_RIGHT_BUTTON) )	{

		if ( (flags & MOUSE_RIGHT_BUTTON) && (set == 1) ){
			mouse_right_pressed++;
		}
	}
	else {
		if ( (flags & MOUSE_RIGHT_BUTTON) && (set == 0) ){
			mouse_right_up++;
		}
	}

	if ( !(mouse_flags & MOUSE_MIDDLE_BUTTON) )	{

		if ( (flags & MOUSE_MIDDLE_BUTTON) && (set == 1) ){
			mouse_middle_pressed++;
		}
	}
	else {
		if ( (flags & MOUSE_MIDDLE_BUTTON) && (set == 0) ){
			mouse_middle_up++;
		}
	}

	if ( set ){
		mouse_flags |= flags;
	} else {
		mouse_flags &= ~flags;
	}

	LEAVE_CRITICAL_SECTION(&mouse_lock);	
}

void mouse_flush()
{
	if (!mouse_inited)
		return;

	mouse_eval_deltas();
	Mouse_dx = Mouse_dy = Mouse_dz = 0;
	ENTER_CRITICAL_SECTION(&mouse_lock);
	mouse_left_pressed = 0;
	mouse_right_pressed = 0;
	mouse_middle_pressed = 0;
	mouse_flags = 0;
	LEAVE_CRITICAL_SECTION(&mouse_lock);	
}

int mouse_down_count(int n, int reset_count)
{
	int tmp = 0;
	if ( !mouse_inited ) return 0;

	if ( (n < LOWEST_MOUSE_BUTTON) || (n > HIGHEST_MOUSE_BUTTON)) return 0;

	ENTER_CRITICAL_SECTION(&mouse_lock);

	switch (n) {
		case MOUSE_LEFT_BUTTON:
			tmp = mouse_left_pressed;
			if ( reset_count ) {
				mouse_left_pressed = 0;
			}
			break;

		case MOUSE_RIGHT_BUTTON:
			tmp = mouse_right_pressed;
			if ( reset_count ) {
				mouse_right_pressed = 0;
			}
			break;

		case MOUSE_MIDDLE_BUTTON:
			tmp = mouse_middle_pressed;
			if ( reset_count ) {
				mouse_middle_pressed = 0;
			}
			break;
	} // end switch

	LEAVE_CRITICAL_SECTION(&mouse_lock);	

	return tmp;
}

// mouse_up_count() returns the number of times button n has gone from down to up
// since the last call
//
// parameters:  n - button of mouse (see #define's in mouse.h)
//
int mouse_up_count(int n)
{
	int tmp = 0;
	if ( !mouse_inited ) return 0;

	if ( (n < LOWEST_MOUSE_BUTTON) || (n > HIGHEST_MOUSE_BUTTON)) return 0;

	ENTER_CRITICAL_SECTION(&mouse_lock);

	switch (n) {
		case MOUSE_LEFT_BUTTON:
			tmp = mouse_left_up;
			mouse_left_up = 0;
			break;

		case MOUSE_RIGHT_BUTTON:
			tmp = mouse_right_up;
			mouse_right_up = 0;
			break;

		case MOUSE_MIDDLE_BUTTON:
			tmp = mouse_middle_up;
			mouse_middle_up = 0;
			break;

		default:
			Assert(0);	// can't happen
			break;
	} // end switch

	LEAVE_CRITICAL_SECTION(&mouse_lock);	

	return tmp;
}

// returns 1 if mouse button btn is down, 0 otherwise

int mouse_down(int btn)
{
	int tmp;
	if ( !mouse_inited ) return 0;

	if ( (btn < LOWEST_MOUSE_BUTTON) || (btn > HIGHEST_MOUSE_BUTTON)) return 0;


	ENTER_CRITICAL_SECTION(&mouse_lock);


	if ( mouse_flags & btn )
		tmp = 1;
	else
		tmp = 0;

	LEAVE_CRITICAL_SECTION(&mouse_lock);	

	return tmp;
}

// returns the fraction of time btn has been down since last call 
// (currently returns 1 if buttons is down, 0 otherwise)
//
float mouse_down_time(int btn)
{
	float tmp;
	if ( !mouse_inited ) return 0.0f;

	if ( (btn < LOWEST_MOUSE_BUTTON) || (btn > HIGHEST_MOUSE_BUTTON)) return 0.0f;

	ENTER_CRITICAL_SECTION(&mouse_lock);

	if ( mouse_flags & btn )
		tmp = 1.0f;
	else
		tmp = 0.0f;

	LEAVE_CRITICAL_SECTION(&mouse_lock);

	return tmp;
}

void mouse_get_delta(int *dx, int *dy, int *dz)
{
	if (dx)
		*dx = Mouse_dx;
	if (dy)
		*dy = Mouse_dy;
	if (dz)
		*dz = Mouse_dz;
}

// Forces the actual windows cursor to be at (x,y).  This may be independent of our tracked (x,y) mouse pos.
void mouse_force_pos(int x, int y)
{
	if (os_foreground()) {  // only mess with windows's mouse if we are in control of it
#ifdef PLAT_UNIX
		SDL_WarpMouse(x, y);
#else
		POINT pnt;

		pnt.x = x;
		pnt.y = y;
		ClientToScreen((HWND) os_get_window(), &pnt);
		SetCursorPos(pnt.x, pnt.y);
#endif
	}
}

#include "gamesequence.h"

// change in mouse position since last call
void mouse_eval_deltas()
{
	static int old_x = 0;
	static int old_y = 0;
	int tmp_x, tmp_y, cx, cy;

	Mouse_dx = Mouse_dy = Mouse_dz = 0;
	if (!mouse_inited)
		return;

	if (Mouse_mode == MOUSE_MODE_DI) {
		mouse_eval_deltas_di();
		return;
	}

	cx = gr_screen.max_w / 2;
	cy = gr_screen.max_h / 2;

	ENTER_CRITICAL_SECTION(&mouse_lock);

#ifdef PLAT_UNIX
	SDL_GetMouseState (&tmp_x, &tmp_y);
#else
	POINT pnt;
	GetCursorPos(&pnt);
	ScreenToClient((HWND)os_get_window(), &pnt);
	tmp_x = pnt.x;
	tmp_y = pnt.y;
#endif

	Mouse_dx = tmp_x - old_x;
	Mouse_dy = tmp_y - old_y;
	Mouse_dz = 0;

	if (Keep_mouse_centered && Mouse_hidden) {
		if (Mouse_dx || Mouse_dy)
			mouse_force_pos(cx, cy);

		old_x = cx;
		old_y = cy;

	} else {
		old_x = tmp_x;
		old_y = tmp_y;
	}

	LEAVE_CRITICAL_SECTION(&mouse_lock);
}

#ifndef PLAT_UNIX
#include "vdinput.h"

static LPDIRECTINPUT			Di_mouse_obj = NULL;
static LPDIRECTINPUTDEVICE	Di_mouse = NULL;
#endif

void mouse_eval_deltas_di()
{
#ifdef PLAT_UNIX
	STUB_FUNCTION;
#else
	int repeat = 1;
	HRESULT hr = 0;
	DIMOUSESTATE mouse_state;

	Mouse_dx = Mouse_dy = Mouse_dz = 0;
	if (!Di_mouse_inited)
		return;

	repeat = 1;
	memset(&mouse_state, 0, sizeof(mouse_state));
	while (repeat) {
		repeat = 0;

		hr = Di_mouse->GetDeviceState(sizeof(mouse_state), &mouse_state);
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
			// DirectInput is telling us that the input stream has
			// been interrupted.  We aren't tracking any state
			// between polls, so we don't have any special reset
			// that needs to be done.  We just re-acquire and
			// try again.
			Sleep(500);		// Pause a half second...
			hr = Di_mouse->Acquire();
			if (SUCCEEDED(hr))
				repeat = 1;
		}
	}

	if (SUCCEEDED(hr)) {
		Mouse_dx = (int) mouse_state.lX;
		Mouse_dy = (int) mouse_state.lY;
		Mouse_dz = (int) mouse_state.lZ;

	} else {
		Mouse_dx = Mouse_dy = Mouse_dz = 0;
	}

	Mouse_x += Mouse_dx;
	Mouse_y += Mouse_dy;

	if (Mouse_x < 0)
		Mouse_x = 0;

	if (Mouse_y < 0)
		Mouse_y = 0;

	if (Mouse_x >= gr_screen.max_w)
		Mouse_x = gr_screen.max_w - 1;

	if (Mouse_y >= gr_screen.max_h)
		Mouse_y = gr_screen.max_h - 1;

	// keep the mouse inside our window so we don't switch applications or anything (debug bug people reported?)
	// JH: Dang!  This makes the mouse readings in DirectInput act screwy!
//	mouse_force_pos(gr_screen.max_w / 2, gr_screen.max_h / 2);
#endif
}

int mouse_get_pos(int *xpos, int *ypos)
{
	int flags;

	if (Mouse_mode == MOUSE_MODE_DI) {
		if (xpos)
			*xpos = Mouse_x;

		if (ypos)
			*ypos = Mouse_y;

		return mouse_flags;
	}

	if (!mouse_inited) {
		*xpos = *ypos = 0;
		return 0;
	}

#ifdef PLAT_UNIX
	flags = SDL_GetMouseState (&Mouse_x, &Mouse_y);
	// DDOI - FIXME?
#else
	POINT pnt;
	GetCursorPos(&pnt);
	ScreenToClient((HWND)os_get_window(), &pnt);

//	EnterCriticalSection(&mouse_lock);

	flags = mouse_flags;
	Mouse_x = pnt.x;
	Mouse_y = pnt.y;
#endif

//	LeaveCriticalSection(&mouse_lock);

	if (Mouse_x < 0){
		Mouse_x = 0;
	}

	if (Mouse_y < 0){
		Mouse_y = 0;
	}

	if (Mouse_x >= gr_screen.max_w){
		Mouse_x = gr_screen.max_w - 1;
	}

	if (Mouse_y >= gr_screen.max_h){
		Mouse_y = gr_screen.max_h - 1;
	}
	
	if (xpos){
		*xpos = Mouse_x;
	}

	if (ypos){
		*ypos = Mouse_y;
	}

	return flags;
}

void mouse_get_real_pos(int *mx, int *my)
{
	if (Mouse_mode == MOUSE_MODE_DI) {
		*mx = Mouse_x;
		*my = Mouse_y;
		return;
	}

#ifdef PLAT_UNIX
	SDL_GetMouseState (mx, my);
#else
	POINT pnt;
	GetCursorPos(&pnt);
	ScreenToClient((HWND)os_get_window(), &pnt);
	
	*mx = pnt.x;
	*my = pnt.y;
#endif
}

void mouse_set_pos(int xpos, int ypos)
{
	if (Mouse_mode == MOUSE_MODE_DI) {
		Mouse_x = xpos;
		Mouse_y = ypos;
		return;
	}

	if ((xpos != Mouse_x) || (ypos != Mouse_y)){
		mouse_force_pos(xpos, ypos);
	}
}

int di_init()
{
#ifdef PLAT_UNIX
	STUB_FUNCTION;
	return 0;
#else
	HRESULT hr;

	if (Mouse_mode == MOUSE_MODE_WIN){
		return 0;
	}

	Di_mouse_inited = 0;
	hr = DirectInputCreate(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &Di_mouse_obj, NULL);
	if (FAILED(hr)) {
		hr = DirectInputCreate(GetModuleHandle(NULL), 0x300, &Di_mouse_obj, NULL);
		if (FAILED(hr)) {
			mprintf(( "DirectInputCreate() failed!\n" ));
			return FALSE;
		}
	}

	hr = Di_mouse_obj->CreateDevice(GUID_SysMouse, &Di_mouse, NULL);
	if (FAILED(hr)) {
		mprintf(( "CreateDevice() failed!\n" ));
		return FALSE;
	}

	hr = Di_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		mprintf(( "SetDataFormat() failed!\n" ));
		return FALSE;
	}

	hr = Di_mouse->SetCooperativeLevel((HWND)os_get_window(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		mprintf(( "SetCooperativeLevel() failed!\n" ));
		return FALSE;
	}
/*
	DIPROPDWORD hdr;

	// Turn on buffering
	hdr.diph.dwSize = sizeof(DIPROPDWORD); 
	hdr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	hdr.diph.dwObj = 0;		
	hdr.diph.dwHow = DIPH_DEVICE;	// Apply to entire device
	hdr.dwData = 16;	//MAX_BUFFERED_KEYBOARD_EVENTS;

	hr = Di_mouse->SetProperty( DIPROP_BUFFERSIZE, &hdr.diph );
	if (FAILED(hr)) {
		mprintf(( "SetProperty DIPROP_BUFFERSIZE failed\n" ));
		return FALSE;
	}

	Di_event = CreateEvent( NULL, FALSE, FALSE, NULL );
	Assert(Di_event != NULL);

	hr = Di_mouse->SetEventNotification(Di_event);
	if (FAILED(hr)) {
		mprintf(( "SetEventNotification failed\n" ));
		return FALSE;
	}
*/
	Di_mouse->Acquire();

	Di_mouse_inited = 1;
	return TRUE;
#endif
}

void di_cleanup()
{
#ifdef PLAT_UNIX
	STUB_FUNCTION;
#else
	// Destroy any lingering IDirectInputDevice object.
	if (Di_mouse) {
		// Unacquire the device one last time just in case we got really confused
		// and tried to exit while the device is still acquired.
		Di_mouse->Unacquire();

		Di_mouse->Release();
		Di_mouse = NULL;
	}

	// Destroy any lingering IDirectInput object.
	if (Di_mouse_obj) {
		Di_mouse_obj->Release();
		Di_mouse_obj = NULL;
	}

	Di_mouse_inited = 0;
#endif
}

