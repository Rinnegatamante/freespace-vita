/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Io/Key.cpp $
 * $Revision: 310 $
 * $Date: 2010-02-08 09:09:25 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * <insert description of file here>
 *
 * $Log$
 * Revision 1.12  2004/06/11 01:14:12  tigital
 * OSX: switched to __APPLE__
 *
 * Revision 1.11  2003/05/18 03:57:08  taylor
 * do not swap German z and y keys if they are already swapped
 *
 * Revision 1.9  2003/01/30 19:55:01  relnev
 * add German keys (this is mostly a patch already sent in by someone else that hasn't made it into cvs yet) (Taylor Richards)
 *
 * Revision 1.8  2002/06/17 23:11:39  relnev
 * enable sdl key repeating.
 *
 * swap '/` keys.
 *
 * Revision 1.7  2002/06/09 04:41:21  relnev
 * added copyright header
 *
 * Revision 1.6  2002/06/05 04:03:32  relnev
 * finished cfilesystem.
 *
 * removed some old code.
 *
 * fixed mouse save off-by-one.
 *
 * sound cleanups.
 *
 * Revision 1.5  2002/05/31 03:34:02  theoddone33
 * Fix Keyboard
 * Add titlebar
 *
 * Revision 1.4  2002/05/30 23:46:29  theoddone33
 * some minor key changes (not necessarily fixes)
 *
 * Revision 1.3  2002/05/30 16:50:24  theoddone33
 * Keyboard partially fixed
 *
 * Revision 1.2  2002/05/29 23:17:50  theoddone33
 * Non working text code and fixed keys
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 6     10/29/99 6:10p Jefff
 * squashed the damned y/z german issues once and for all
 * 
 * 5     6/07/99 1:21p Dave
 * Fixed debug console scrolling problem. Thread related.
 * 
 * 4     6/02/99 6:18p Dave
 * Fixed TNT lockup problems! Wheeeee!
 * 
 * 3     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 37    6/19/98 3:50p Lawrance
 * change GERMAN to GR_BUILD
 * 
 * 36    6/17/98 11:05a Lawrance
 * translate french and german keys
 * 
 * 35    6/12/98 4:49p Hoffoss
 * Added code to remap scancodes for german and french keyboards.
 * 
 * 34    5/20/98 12:10a Mike
 * Remove mprintfs.
 * 
 * 33    5/19/98 12:19p Mike
 * Cheat codes!
 * 
 * 32    5/19/98 12:28a Mike
 * Cheat stuff.
 * 
 * 31    5/18/98 11:01p Mike
 * Adding support for cheat system.
 * 
 * 30    5/11/98 12:09a Lawrance
 * Put in code to turn on/off NumLock key when running under 95
 * 
 * 29    5/01/98 4:23p Lawrance
 * Remap the scancode for the UK "\" key
 * 
 * 28    4/18/98 12:42p John
 * Added code to use DirectInput to read keyboard. Took out because it
 * didn't differentiate btwn Pause and Numlock and sometimes Ctrl.
 * 
 * 27    4/13/98 10:16a John
 * Switched gettime back to timer_get_milliseconds, which is now thread
 * safe.
 * 
 * 26    4/12/98 11:08p Lawrance
 * switch back to using gettime() in separate threads
 * 
 * 25    4/12/98 5:31p Lawrance
 * use timer_get_milliseconds() instead of gettime()
 * 
 * 24    3/25/98 8:08p John
 * Restructured software rendering into two modules; One for windowed
 * debug mode and one for DirectX fullscreen.   
 * 
 * 23    1/23/98 3:49p Mike
 * Fix bug in negative time-down due to latency.
 * 
 * 22    1/07/98 6:41p Lawrance
 * Pass message latency to the keyboard lib.
 * 
 * 21    11/17/97 10:42a John
 * On Debug+Backsp, cleared out keys so that it looks like nothing ever
 * happened, so they're not stuck down.
 * 
 * 20    11/14/97 4:33p Mike
 * Change Debug key to backquote (from F11).
 * Balance a ton of subsystems in ships.tbl.
 * Change "Heavy Laser" to "Disruptor".
 * 
 * 19    9/13/97 9:30a Lawrance
 * added ability to block certain keys from the keyboard
 * 
 * 18    9/10/97 6:02p Hoffoss
 * Added code to check for key-pressed sexp operator in FreeSpace as part
 * of training mission stuff.
 * 
 * 17    9/09/97 11:08a Sandeep
 * fixed warning level 4
 * 
 * 16    7/29/97 5:30p Lawrance
 * move gettime() to timer module
 * 
 * 15    4/22/97 10:56a John
 * fixed some resource leaks.
 * 
 * 14    2/03/97 4:23p Allender
 * use F11 as debug key now
 * 
 * 13    1/10/97 5:15p Mike
 * Moved ship-specific parameters from obj_subsystem to ship_subsys.
 * 
 * Added turret code to AI system.
 *
 * $NoKeywords: $
 */

//#define USE_DIRECTINPUT

#ifndef PLAT_UNIX
#include <windows.h>
#include <windowsx.h>
#endif

#include <ctype.h>	// for toupper
#include "pstypes.h"
#include "key.h"
#include "fix.h"
#include "timer.h"
#include "osapi.h"
#include "localize.h"
#ifdef PANDORA
#include "mouse.h"
#endif

#define KEY_BUFFER_SIZE 16

//-------- Variable accessed by outside functions ---------
ubyte				keyd_buffer_type;		// 0=No buffer, 1=buffer ASCII, 2=buffer scans
ubyte				keyd_repeat;
uint				keyd_last_pressed;
uint				keyd_last_released;
ubyte				keyd_pressed[NUM_KEYS];
int				keyd_time_when_last_pressed;

typedef struct keyboard	{
	ushort			keybuffer[KEY_BUFFER_SIZE];
	uint				time_pressed[KEY_BUFFER_SIZE];
	uint				TimeKeyWentDown[NUM_KEYS];
	uint				TimeKeyHeldDown[NUM_KEYS];
	uint				TimeKeyDownChecked[NUM_KEYS];
	uint				NumDowns[NUM_KEYS];
	uint				NumUps[NUM_KEYS];
	int				down_check[NUM_KEYS];  // nonzero if has been pressed yet this mission
	uint				keyhead, keytail;
} keyboard;

keyboard key_data;

int key_inited = 0;

CRITICAL_SECTION key_lock;

//int Backspace_debug=1;	// global flag that will enable/disable the backspace key from stopping execution
								// This flag was created since the backspace key is also used to correct mistakes
								// when typing in your pilots callsign.  This global flag is checked before execution
								// is stopped.

#ifdef PLAT_UNIX
int SDLtoFS2[SDLK_LAST];
#endif

int ascii_table[128] = 
{ 255, 255, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',255,255,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 255, 255,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 39, '`',
  255, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 255,'*',
  255, ' ', 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255,255,
  255, 255, 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255 };

int shifted_ascii_table[128] = 
{ 255, 255, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',255,255,
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 255, 255,
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
  255, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 255,255,
  255, ' ', 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255,255,
  255, 255, 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255 };

// used to limit the keypresses that are accepted from the keyboard
#define MAX_FILTER_KEYS 64
int Num_filter_keys;
int Key_filter[MAX_FILTER_KEYS];

static int Key_numlock_was_on = 0;	// Flag to indicate whether NumLock is on at start
static int Key_running_NT = 0;		// NT is the OS

int Cheats_enabled = 0;
int Key_normal_game = 0;

#ifdef PLAT_UNIX
void FillSDLArray ()
{
	SDLtoFS2[SDLK_0] = KEY_0;
	SDLtoFS2[SDLK_1] = KEY_1;
	SDLtoFS2[SDLK_2] = KEY_2;
	SDLtoFS2[SDLK_3] = KEY_3;
	SDLtoFS2[SDLK_4] = KEY_4;
	SDLtoFS2[SDLK_5] = KEY_5;
	SDLtoFS2[SDLK_6] = KEY_6;
	SDLtoFS2[SDLK_7] = KEY_7;
	SDLtoFS2[SDLK_8] = KEY_8;
	SDLtoFS2[SDLK_9] = KEY_9;

	SDLtoFS2[SDLK_a] = KEY_A;
	SDLtoFS2[SDLK_b] = KEY_B;
	SDLtoFS2[SDLK_c] = KEY_C;
	SDLtoFS2[SDLK_d] = KEY_D;
	SDLtoFS2[SDLK_e] = KEY_E;
	SDLtoFS2[SDLK_f] = KEY_F;
	SDLtoFS2[SDLK_g] = KEY_G;
	SDLtoFS2[SDLK_h] = KEY_H;
	SDLtoFS2[SDLK_i] = KEY_I;
	SDLtoFS2[SDLK_j] = KEY_J;
	SDLtoFS2[SDLK_k] = KEY_K;
	SDLtoFS2[SDLK_l] = KEY_L;
	SDLtoFS2[SDLK_m] = KEY_M;
	SDLtoFS2[SDLK_n] = KEY_N;
	SDLtoFS2[SDLK_o] = KEY_O;
	SDLtoFS2[SDLK_p] = KEY_P;
	SDLtoFS2[SDLK_q] = KEY_Q;
	SDLtoFS2[SDLK_r] = KEY_R;
	SDLtoFS2[SDLK_s] = KEY_S;
	SDLtoFS2[SDLK_t] = KEY_T;
	SDLtoFS2[SDLK_u] = KEY_U;
	SDLtoFS2[SDLK_v] = KEY_V;
	SDLtoFS2[SDLK_w] = KEY_W;
	SDLtoFS2[SDLK_x] = KEY_X;
	SDLtoFS2[SDLK_y] = KEY_Y;
	SDLtoFS2[SDLK_z] = KEY_Z;
#ifndef PANDORA
	if (Lcl_gr) {
		SDLtoFS2[SDLK_WORLD_63] = KEY_MINUS;
		SDLtoFS2[SDLK_WORLD_20] = KEY_EQUAL;
		SDLtoFS2[SDLK_MINUS] = KEY_DIVIDE;
		SDLtoFS2[SDLK_HASH] = KEY_SLASH;
		SDLtoFS2[SDLK_COMMA] = KEY_COMMA;
		SDLtoFS2[SDLK_PERIOD] = KEY_PERIOD;
		SDLtoFS2[SDLK_WORLD_86] = KEY_SEMICOL;

		SDLtoFS2[SDLK_WORLD_92] = KEY_LBRACKET;
		SDLtoFS2[SDLK_PLUS] = KEY_RBRACKET;

		SDLtoFS2[SDLK_CARET] = KEY_LAPOSTRO;
		SDLtoFS2[SDLK_WORLD_68] = KEY_RAPOSTRO;
	} else 
#endif
	{
		SDLtoFS2[SDLK_MINUS] = KEY_MINUS;
		SDLtoFS2[SDLK_EQUALS] = KEY_EQUAL;
		SDLtoFS2[SDLK_SLASH] = KEY_DIVIDE; // No idea - DDOI
		SDLtoFS2[SDLK_BACKSLASH] = KEY_SLASH;
		//SDLtoFS2[SDLK_BACKSLASH] = KEY_SLASH_UK; // ?
		SDLtoFS2[SDLK_COMMA] = KEY_COMMA;
		SDLtoFS2[SDLK_PERIOD] = KEY_PERIOD;
		SDLtoFS2[SDLK_SEMICOLON] = KEY_SEMICOL;

		SDLtoFS2[SDLK_LEFTBRACKET] = KEY_LBRACKET;
		SDLtoFS2[SDLK_RIGHTBRACKET] = KEY_RBRACKET;

		SDLtoFS2[SDLK_BACKQUOTE] = KEY_LAPOSTRO;
		SDLtoFS2[SDLK_QUOTE] = KEY_RAPOSTRO;
	}

	SDLtoFS2[SDLK_ESCAPE] = KEY_ESC;
	SDLtoFS2[SDLK_RETURN] = KEY_ENTER;
	SDLtoFS2[SDLK_BACKSPACE] = KEY_BACKSP;
	SDLtoFS2[SDLK_TAB] = KEY_TAB;
	SDLtoFS2[SDLK_SPACE] = KEY_SPACEBAR;

	SDLtoFS2[SDLK_NUMLOCK] = KEY_NUMLOCK;
	SDLtoFS2[SDLK_SCROLLOCK] = KEY_SCROLLOCK;
	SDLtoFS2[SDLK_CAPSLOCK] = KEY_CAPSLOCK;

	SDLtoFS2[SDLK_LSHIFT] = KEY_LSHIFT;
	SDLtoFS2[SDLK_RSHIFT] = KEY_RSHIFT;

	SDLtoFS2[SDLK_LALT] = KEY_LALT;
	SDLtoFS2[SDLK_RALT] = KEY_RALT;

	SDLtoFS2[SDLK_LCTRL] = KEY_LCTRL;
	SDLtoFS2[SDLK_RCTRL] = KEY_RCTRL;

	SDLtoFS2[SDLK_F1] = KEY_F1;
	SDLtoFS2[SDLK_F2] = KEY_F2;
	SDLtoFS2[SDLK_F3] = KEY_F3;
	SDLtoFS2[SDLK_F4] = KEY_F4;
	SDLtoFS2[SDLK_F5] = KEY_F5;
	SDLtoFS2[SDLK_F6] = KEY_F6;
	SDLtoFS2[SDLK_F7] = KEY_F7;
	SDLtoFS2[SDLK_F8] = KEY_F8;
	SDLtoFS2[SDLK_F9] = KEY_F9;
	SDLtoFS2[SDLK_F10] = KEY_F10;
	SDLtoFS2[SDLK_F11] = KEY_F11;
	SDLtoFS2[SDLK_F12] = KEY_F12;

	SDLtoFS2[SDLK_KP0] = KEY_PAD0;
	SDLtoFS2[SDLK_KP1] = KEY_PAD1;
	SDLtoFS2[SDLK_KP2] = KEY_PAD2;
	SDLtoFS2[SDLK_KP3] = KEY_PAD3;
	SDLtoFS2[SDLK_KP4] = KEY_PAD4;
	SDLtoFS2[SDLK_KP5] = KEY_PAD5;
	SDLtoFS2[SDLK_KP6] = KEY_PAD6;
	SDLtoFS2[SDLK_KP7] = KEY_PAD7;
	SDLtoFS2[SDLK_KP8] = KEY_PAD8;
	SDLtoFS2[SDLK_KP9] = KEY_PAD9;
	SDLtoFS2[SDLK_KP_MINUS] = KEY_PADMINUS;
	SDLtoFS2[SDLK_KP_PLUS] = KEY_PADPLUS;
	SDLtoFS2[SDLK_KP_PERIOD] = KEY_PADPERIOD;
	SDLtoFS2[SDLK_KP_DIVIDE] = KEY_PADDIVIDE;
	SDLtoFS2[SDLK_KP_MULTIPLY] = KEY_PADMULTIPLY;
	SDLtoFS2[SDLK_KP_ENTER] = KEY_PADENTER;

	SDLtoFS2[SDLK_INSERT] = KEY_INSERT;
	SDLtoFS2[SDLK_HOME] = KEY_HOME;
	SDLtoFS2[SDLK_PAGEUP] = KEY_PAGEUP;
	SDLtoFS2[SDLK_DELETE] = KEY_DELETE;
	SDLtoFS2[SDLK_END] = KEY_END;
	SDLtoFS2[SDLK_PAGEDOWN] = KEY_PAGEDOWN;
	SDLtoFS2[SDLK_UP] = KEY_UP;
	SDLtoFS2[SDLK_DOWN] = KEY_DOWN;
	SDLtoFS2[SDLK_LEFT] = KEY_LEFT;
	SDLtoFS2[SDLK_RIGHT] = KEY_RIGHT;

	SDLtoFS2[SDLK_PRINT] = KEY_PRINT_SCRN;
	SDLtoFS2[SDLK_PAUSE] = KEY_PAUSE;
	SDLtoFS2[SDLK_BREAK] = KEY_BREAK;
}
#endif

int key_numlock_is_on()
{
#ifdef PLAT_UNIX
	int keys[SDLK_LAST];
	SDL_GetKeyState(keys);
	if ( keys[SDLK_NUMLOCK] ) {
		return 1;
	}
#else
	unsigned char keys[256];
	GetKeyboardState(keys);
	if ( keys[VK_NUMLOCK]  ) {
		return 1;
	}
#endif
	return 0;
}

void key_turn_off_numlock()
{
#ifdef PLAT_UNIX
//	STUB_FUNCTION; /* sdl doesn't support this */
#else
	unsigned char keys[256];
	GetKeyboardState(keys);
	keys[VK_NUMLOCK] = 0;
	SetKeyboardState(keys);
#endif
}

void key_turn_on_numlock()
{
#ifdef PLAT_UNIX
//	STUB_FUNCTION; /* sdl doesn't support this */
#else
	unsigned char keys[256];
	GetKeyboardState(keys);
	keys[VK_NUMLOCK] = 1;
	SetKeyboardState(keys);
#endif
}

//	Convert a BIOS scancode to ASCII.
//	If scancode >= 127, returns 255, meaning there is no corresponding ASCII code.
//	Uses ascii_table and shifted_ascii_table to translate scancode to ASCII.
int key_to_ascii(int keycode )
{
	int shifted;

	if ( !key_inited ) return 255;

	shifted = keycode & KEY_SHIFTED;
	keycode &= KEY_MASK;

	if ( keycode>=127 )
		return 255;

	if (shifted)
		return shifted_ascii_table[keycode];
	else
		return ascii_table[keycode];
}

//	Flush the keyboard buffer.
//	Clear the keyboard array (keyd_pressed).
void key_flush()
{
	int i;
	uint CurTime;

	if ( !key_inited ) return;

	ENTER_CRITICAL_SECTION(&key_lock);	

	key_data.keyhead = key_data.keytail = 0;

	//Clear the keyboard buffer
	for (i=0; i<KEY_BUFFER_SIZE; i++ )	{
		key_data.keybuffer[i] = 0;
		key_data.time_pressed[i] = 0;
	}
	
	//Clear the keyboard array

	CurTime = timer_get_milliseconds();


	for (i=0; i<NUM_KEYS; i++ )	{
		keyd_pressed[i] = 0;
		key_data.TimeKeyDownChecked[i] = CurTime;
		key_data.TimeKeyWentDown[i] = CurTime;
		key_data.TimeKeyHeldDown[i] = 0;
		key_data.NumDowns[i]=0;
		key_data.NumUps[i]=0;
	}

	LEAVE_CRITICAL_SECTION(&key_lock);	
}

//	A nifty function which performs the function:
//		n = (n+1) % KEY_BUFFER_SIZE
//	(assuming positive values of n).
int add_one( int n )
{
	n++;
	if ( n >= KEY_BUFFER_SIZE ) n=0;
	return n;
}

// Returns 1 if character waiting... 0 otherwise
int key_checkch()
{
	int is_one_waiting = 0;

	if ( !key_inited ) return 0;

	ENTER_CRITICAL_SECTION(&key_lock);	

	if (key_data.keytail != key_data.keyhead){
		is_one_waiting = 1;
	}

	LEAVE_CRITICAL_SECTION(&key_lock);		

	return is_one_waiting;
}

//	Return key scancode if a key has been pressed,
//	else return 0.
//	Reads keys out of the key buffer and updates keyhead.
int key_inkey()
{
	int key = 0;

	if ( !key_inited ) return 0;

	ENTER_CRITICAL_SECTION(&key_lock);	

	if (key_data.keytail!=key_data.keyhead)	{
		key = key_data.keybuffer[key_data.keyhead];
		key_data.keyhead = add_one(key_data.keyhead);
	}

	LEAVE_CRITICAL_SECTION(&key_lock);	

	return key;
}

//	Unget a key.  Puts it back in the input queue.
void key_outkey(int key)
{
	int	bufp;

	if ( !key_inited ) return;

	ENTER_CRITICAL_SECTION(&key_lock);		

	bufp = key_data.keytail+1;

	if (bufp >= KEY_BUFFER_SIZE){
		bufp = 0;
	}

	key_data.keybuffer[key_data.keytail] = (unsigned short)key;

	key_data.keytail = bufp;

	LEAVE_CRITICAL_SECTION(&key_lock);		
}



//	Return amount of time last key was held down.
//	This is currently (July 17, 1996) bogus because our timing is
//	not accurate.
int key_inkey_time(uint * time)
{
	int key = 0;

	if ( !key_inited ) {
		*time = 0;
		return 0;
	}
	
	ENTER_CRITICAL_SECTION(&key_lock);		

	if (key_data.keytail!=key_data.keyhead)	{
		key = key_data.keybuffer[key_data.keyhead];
		*time = key_data.time_pressed[key_data.keyhead];
		key_data.keyhead = add_one(key_data.keyhead);
	}

	LEAVE_CRITICAL_SECTION(&key_lock);		

	return key;
}


//	Returns scancode of last key pressed, if any (returns 0 if no key pressed)
//	but does not update keyhead pointer.
int key_peekkey()
{
	int key = 0;

	if ( !key_inited ) return 0;

	ENTER_CRITICAL_SECTION(&key_lock);		

	if (key_data.keytail!=key_data.keyhead)	{
		key = key_data.keybuffer[key_data.keyhead];
	}
	LEAVE_CRITICAL_SECTION(&key_lock);		

	return key;
}

// If not installed, uses BIOS and returns getch();
//	Else returns pending key (or waits for one if none waiting).
int key_getch()
{
	int dummy=0;
	int in;

	if ( !key_inited ) return 0;
	
	while (!key_checkch()){
		os_poll();

		dummy++;
	}
	in = key_inkey();

	return in;
}

//	Set global shift_status with modifier results (shift, ctrl, alt).
uint key_get_shift_status()
{
	unsigned int shift_status = 0;

	if ( !key_inited ) return 0;

	ENTER_CRITICAL_SECTION(&key_lock);		

	if ( keyd_pressed[KEY_LSHIFT] || keyd_pressed[KEY_RSHIFT] )
		shift_status |= KEY_SHIFTED;

	if ( keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT] )
		shift_status |= KEY_ALTED;

	if ( keyd_pressed[KEY_LCTRL] || keyd_pressed[KEY_RCTRL] )
		shift_status |= KEY_CTRLED;

#ifndef NDEBUG
	if (keyd_pressed[KEY_DEBUG_KEY])
		shift_status |= KEY_DEBUGGED;
#else
	if (keyd_pressed[KEY_DEBUG_KEY]) {
		mprintf(("Cheats_enabled = %i, Key_normal_game = %i\n", Cheats_enabled, Key_normal_game));
		if ((Cheats_enabled) && Key_normal_game) {
			mprintf(("Debug key\n"));
			shift_status |= KEY_DEBUGGED1;
		}
	}
#endif
	LEAVE_CRITICAL_SECTION(&key_lock);		

	return shift_status;
}

//	Returns amount of time key (specified by "code") has been down since last call.
//	Returns float, unlike key_down_time() which returns a fix.
float key_down_timef(uint scancode)	
{
	uint time_down, time;
	uint delta_time;

	if ( !key_inited ) return 0.0f;

	if (scancode >= NUM_KEYS) return 0.0f;

	ENTER_CRITICAL_SECTION(&key_lock);		

	time = timer_get_milliseconds();
	delta_time = time - key_data.TimeKeyDownChecked[scancode];
	key_data.TimeKeyDownChecked[scancode] = time;

	if ( delta_time <= 1 ) {
		key_data.TimeKeyWentDown[scancode] = time;
		if (keyd_pressed[scancode])	{
			LEAVE_CRITICAL_SECTION(&key_lock);		
			return 1.0f;
		} else	{
			LEAVE_CRITICAL_SECTION(&key_lock);		
			return 0.0f;
		}
	}

	if ( !keyd_pressed[scancode] )	{
		time_down = key_data.TimeKeyHeldDown[scancode];
		key_data.TimeKeyHeldDown[scancode] = 0;
	} else	{
		time_down =  time - key_data.TimeKeyWentDown[scancode];
		key_data.TimeKeyWentDown[scancode] = time;
	}

	LEAVE_CRITICAL_SECTION(&key_lock);		

	return i2fl(time_down) / i2fl(delta_time);
}

/*
//	Returns amount of time key (specified by "code") has been down since last call.
//	Returns float, unlike key_down_time() which returns a fix.
fix key_down_time( uint code )
{
	uint time_down, time;
	uint delta_time;

	if ( !key_inited ) return 0.0f;

	if ((scancode<0)|| (scancode>=NUM_KEYS)) return 0.0f;

	EnterCriticalSection( &key_lock );

	time = timer_get_milliseconds();
	delta_time = time - TimeKeyDownChecked[scancode];
	TimeKeyDownChecked[scancode] = time;

	if ( delta_time <= 1 ) {
		LeaveCriticalSection( &key_lock );
		if (keyd_pressed[scancode])
			return F1_0;
		else
			return 0;
	}

	if ( !keyd_pressed[scancode] )	{
		time_down = key_data.TimeKeyHeldDown[scancode];
		key_data.TimeKeyHeldDown[scancode] = 0;
	} else	{
		time_down =  time - key_data.TimeKeyWentDown[scancode];
		key_data.TimeKeyWentDown[scancode] = time;
	}

	LeaveCriticalSection( &key_lock );

	return fixmuldiv( time_down, F1_0, delta_time );
}
*/


// Returns number of times key has went from up to down since last call.
int key_down_count(int scancode)	
{
	int n;

	if ( !key_inited ) return 0;

	if ((scancode<0)|| (scancode>=NUM_KEYS)) return 0;

	ENTER_CRITICAL_SECTION(&key_lock);		

	n = key_data.NumDowns[scancode];
	key_data.NumDowns[scancode] = 0;

	LEAVE_CRITICAL_SECTION(&key_lock);		

	return n;
}


// Returns number of times key has went from down to up since last call.
int key_up_count(int scancode)	
{
	int n;

	if ( !key_inited ) return 0;
	if ((scancode<0)|| (scancode>=NUM_KEYS)) return 0;

	ENTER_CRITICAL_SECTION(&key_lock);		

	n = key_data.NumUps[scancode];
	key_data.NumUps[scancode] = 0;

	LEAVE_CRITICAL_SECTION(&key_lock);		

	return n;
}

int key_check(int key)
{
	return key_data.down_check[key];
}

//	Add a key up or down code to the key buffer.  state=1 -> down, state=0 -> up
// latency => time difference in ms between when key was actually pressed and now
//void key_mark( uint code, int state )
void key_mark( uint code, int state, uint latency )
{
	uint scancode, breakbit, temp, event_time;
	ushort keycode;	

	if ( !key_inited ) return;

	ENTER_CRITICAL_SECTION(&key_lock);		

	// If running in the UK, need to translate their wacky slash scancode to ours
	if ( code == KEY_SLASH_UK ) {
		code = KEY_SLASH;
	}

#ifndef PANDORA
	if(Lcl_fr){
		switch (code) {
		case KEY_A:
			code = KEY_Q;
			break;

		case KEY_M:
			code = KEY_COMMA;
			break;

		case KEY_Q:
			code = KEY_A;
			break;

		case KEY_W:
			code = KEY_Z;
			break;

		case KEY_Z:
			code = KEY_W;
			break;

		case KEY_SEMICOL:
			code = KEY_M;
			break;

		case KEY_COMMA:
			code = KEY_SEMICOL;
			break;
		}

#if !defined(PLAT_UNIX) || defined(__APPLE__)
	} else if(Lcl_gr){
		switch (code) {
		case KEY_Y:
			code = KEY_Z;
			break;

		case KEY_Z:
			code = KEY_Y;
			break;
		}
#endif
	}
#else
extern int Cmdline_noshouldermb;
	if (!Cmdline_noshouldermb) {
		if (code == KEY_RCTRL) {
			// Left click button...
			mouse_mark_button(MOUSE_LEFT_BUTTON, state);
			LEAVE_CRITICAL_SECTION(&key_lock);
			return;
		}
		if (code == KEY_RSHIFT) {
			// Right click button...
			mouse_mark_button(MOUSE_RIGHT_BUTTON, state);
			LEAVE_CRITICAL_SECTION(&key_lock);
			return;
		}
	}
#endif
	if ( (code == 0xc5) && !Key_running_NT ) {
		key_turn_off_numlock();
	}

	Assert( code < NUM_KEYS );	

	event_time = timer_get_milliseconds() - latency;
	// event_time = timeGetTime() - latency;

	// Read in scancode
	scancode = code & (NUM_KEYS-1);
	breakbit = !state;
	
	if (breakbit)	{
		// Key going up
		keyd_last_released = scancode;
		keyd_pressed[scancode] = 0;
		key_data.NumUps[scancode]++;

		//	What is the point of this code?  "temp" is never used!
		temp = 0;
		temp |= keyd_pressed[KEY_LSHIFT] || keyd_pressed[KEY_RSHIFT];
		temp |= keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT];
		temp |= keyd_pressed[KEY_LCTRL] || keyd_pressed[KEY_RCTRL];
//#ifndef NDEBUG
		temp |= keyd_pressed[KEY_DEBUG_KEY];
//#endif	
		if (event_time < key_data.TimeKeyWentDown[scancode])
			key_data.TimeKeyHeldDown[scancode] = 0;
		else
			key_data.TimeKeyHeldDown[scancode] += event_time - key_data.TimeKeyWentDown[scancode];
	} else {
		// Key going down
		keyd_last_pressed = scancode;
		keyd_time_when_last_pressed = event_time;
		if (!keyd_pressed[scancode])	{
			// First time down
			key_data.TimeKeyWentDown[scancode] = event_time;
			keyd_pressed[scancode] = 1;
			key_data.NumDowns[scancode]++;
			key_data.down_check[scancode]++;

//			mprintf(( "Scancode = %x\n", scancode ));

//			if ( scancode == KEY_BREAK )
//				Int3();


		} else if (!keyd_repeat) {
			// Don't buffer repeating key if repeat mode is off
			scancode = 0xAA;		
		} 

		if ( scancode!=0xAA ) {
			keycode = (unsigned short)scancode;

			if ( keyd_pressed[KEY_LSHIFT] || keyd_pressed[KEY_RSHIFT] )
				keycode |= KEY_SHIFTED;

			if ( keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT] )
				keycode |= KEY_ALTED;

			if ( keyd_pressed[KEY_LCTRL] || keyd_pressed[KEY_RCTRL] )
				keycode |= KEY_CTRLED;

#ifndef NDEBUG
			if ( keyd_pressed[KEY_DEBUG_KEY] )
				keycode |= KEY_DEBUGGED;
//			if ( keycode == (KEY_BACKSP + KEY_DEBUGGED) )	{
//				keycode = 0;
//				keyd_pressed[KEY_DEBUG_KEY] = 0;
//				keyd_pressed[KEY_BACKSP] = 0;
//				Int3();
//			}
#else
			if ( keyd_pressed[KEY_DEBUG_KEY] ) {
				mprintf(("Cheats_enabled = %i, Key_normal_game = %i\n", Cheats_enabled, Key_normal_game));
				if (Cheats_enabled && Key_normal_game) {
					keycode |= KEY_DEBUGGED1;
				}
			}

#endif

			if ( keycode )	{
				temp = key_data.keytail+1;
				if ( temp >= KEY_BUFFER_SIZE ) temp=0;

				if (temp!=key_data.keyhead)	{
					int i, accept_key = 1;
					// Num_filter_keys will only be non-zero when a key filter has
					// been explicity set up via key_set_filter()
					for ( i = 0; i < Num_filter_keys; i++ ) {
						accept_key = 0;
						if ( Key_filter[i] == keycode ) {
							accept_key = 1;
							break;
						}
					}

					if ( accept_key ) {
						key_data.keybuffer[key_data.keytail] = keycode;
						key_data.time_pressed[key_data.keytail] = keyd_time_when_last_pressed;
						key_data.keytail = temp;
					}
				}
			}
		}
	}

	LEAVE_CRITICAL_SECTION(&key_lock);		
}

#ifdef USE_DIRECTINPUT
void di_cleanup();
int di_init();
#endif


void key_close()
{
	if ( !key_inited ) return;

	#ifdef USE_DIRECTINPUT
		di_cleanup();
	#endif

	if ( Key_numlock_was_on ) {
		key_turn_on_numlock();
		Key_numlock_was_on = 0;
	}

	key_inited = 0;
#ifdef PLAT_UNIX
//	STUB_FUNCTION; /* don't need this? */
#else
	DeleteCriticalSection( &key_lock );
#endif
}

void key_init()
{
	// Initialize queue
	if ( key_inited ) return;
	key_inited = 1;

#ifdef PLAT_UNIX
	FillSDLArray ();
//	STUB_FUNCTION; /* don't need this */
#else
	InitializeCriticalSection( &key_lock );

	ENTER_CRITICAL_SECTION(&key_lock);		
#endif

	keyd_time_when_last_pressed = timer_get_milliseconds();
	keyd_buffer_type = 1;
	keyd_repeat = 1;

	// Clear the keyboard array
	key_flush();

	// Clear key filter
	key_clear_filter();

#ifdef PLAT_UNIX
//	STUB_FUNCTION; /* don't need this */
#else
	LEAVE_CRITICAL_SECTION(&key_lock);		

	#ifdef USE_DIRECTINPUT
		di_init();
	#endif

	OSVERSIONINFO ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ver);
	if ( ver.dwPlatformId == VER_PLATFORM_WIN32_NT ) {
		Key_running_NT = 1;
	} else {
		Key_running_NT = 0;
		if ( key_numlock_is_on() ) {
			Key_numlock_was_on = 1;
			key_turn_off_numlock();
		}
	}
#endif

	atexit(key_close);
}

void key_level_init()
{
	int i;

	for (i=0; i<NUM_KEYS; i++)
		key_data.down_check[i] = 0;
}

void key_lost_focus()
{
	if ( !key_inited ) return;

	key_flush();	
}

void key_got_focus()
{
	if ( !key_inited ) return;
	
	key_flush();	
}

// Restricts the keys that are accepted from the keyboard
//
//	filter_array	=>		array of keys to act as a filter
//	num				=>		number of keys in filter_array
//
void key_set_filter(int *filter_array, int num)
{
	int i;

	if ( num >= MAX_FILTER_KEYS ) {
		Int3();
		num = MAX_FILTER_KEYS;
	}

	Num_filter_keys = num;

	for ( i = 0; i < num; i++ ) {
		Key_filter[i] = filter_array[i];
	}
}

// Clear the key filter, so all keypresses are accepted from keyboard 
//
void key_clear_filter()
{
	int i;

	Num_filter_keys = 0;
	for ( i = 0; i < MAX_FILTER_KEYS; i++ ) {
		Key_filter[i] = -1;
	}
}


#ifdef USE_DIRECTINPUT

// JAS - April 18, 1998
// Not using because DI has the following problems:  (Everything else works ok)
// Under NT, Pause and Numlock report as identical keys.
// Under 95, Pause is the same as pressing Ctrl then Numlock.  So the game fires each
// time you hit it.
// 

//============================================================================
// Direct Input code
// For the keyboard, this basically replaces our old functionallity of:
// WM_KEYDOWN:
//    key_mark(...);
// WM_KEYUP:
//    key_mark(...);
//============================================================================


#include "vdinput.h"

#define MAX_BUFFERED_KEYBOARD_EVENTS 10

static LPDIRECTINPUT			Di_object = NULL;
static LPDIRECTINPUTDEVICE	Di_keyboard = NULL;
static HANDLE					Di_thread = NULL;
static DWORD					Di_thread_id = NULL;
static HANDLE					Di_event = NULL;

DWORD di_process(DWORD lparam)
{
	while (1) {
		if ( WaitForSingleObject( Di_event, INFINITE )==WAIT_OBJECT_0 )	{

			//mprintf(( "Got event!\n" ));

			HRESULT hr;

			DIDEVICEOBJECTDATA rgdod[10]; 
			DWORD dwItems = MAX_BUFFERED_KEYBOARD_EVENTS; 

again:;
			hr = Di_keyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), rgdod,  &dwItems, 0); 

			if (hr == DIERR_INPUTLOST) {
				/*
				*  DirectInput is telling us that the input stream has
				*  been interrupted.  We aren't tracking any state
				*  between polls, so we don't have any special reset
				*  that needs to be done.  We just re-acquire and
				*  try again.
				*/
				Sleep(1000);		// Pause a second...
				hr = Di_keyboard->Acquire();
				if (SUCCEEDED(hr)) {
					goto again;
				}
			}

			if (SUCCEEDED(hr)) { 
				 // dwItems = number of elements read (could be zero)
				 if (hr == DI_BUFFEROVERFLOW) { 
					// Buffer had overflowed. 
					mprintf(( "Buffer overflowed!\n" ));
				 } 
					int i;

					//mprintf(( "Got %d events\n", dwItems ));

					for (i=0; i<(int)dwItems; i++ )	{
						int key = rgdod[i].dwOfs;
						int state = rgdod[i].dwData;
						int stamp = rgdod[i].dwTimeStamp;

						int latency;
						latency = timeGetTime() - stamp;
						if ( latency < 0 )
							latency=0;

//						if ( key == KEY_PRINT_SCRN )	{
//							key_mark( key, 1, latency );
//						}
//						key_mark( key, (state&0x80?1:0), latency );
						mprintf(( "Key=%x, State=%x, Time=%d, Latency=%d\n", key, state, stamp, latency ));
					}

			} 
		} 

	}

	return 0;
}


int di_init()
{
    HRESULT hr;

	 return 0;


    /*
     *  Register with the DirectInput subsystem and get a pointer
     *  to a IDirectInput interface we can use.
     *
     *  Parameters:
     *
     *      g_hinst
     *
     *          Instance handle to our application or DLL.
     *
     *      DIRECTINPUT_VERSION
     *
     *          The version of DirectInput we were designed for.
     *          We take the value from the <dinput.h> header file.
     *
     *      &g_pdi
     *
     *          Receives pointer to the IDirectInput interface
     *          that was created.
     *
     *      NULL
     *
     *          We do not use OLE aggregation, so this parameter
     *          must be NULL.
     *
     */
    hr = DirectInputCreate(GetModuleHandle(NULL), 0x300, &Di_object, NULL);

    if (FAILED(hr)) {
        mprintf(( "DirectInputCreate failed!\n" ));
        return FALSE;
    }

    /*
     *  Obtain an interface to the system keyboard device.
     *
     *  Parameters:
     *
     *      GUID_SysKeyboard
     *
     *          The instance GUID for the device we wish to access.
     *          GUID_SysKeyboard is a predefined instance GUID that
     *          always refers to the system keyboard device.
     *
     *      &g_pKeyboard
     *
     *          Receives pointer to the IDirectInputDevice interface
     *          that was created.
     *
     *      NULL
     *
     *          We do not use OLE aggregation, so this parameter
     *          must be NULL.
     *
     */
    hr = Di_object->CreateDevice(GUID_SysKeyboard, &Di_keyboard, NULL);

    if (FAILED(hr)) {
        mprintf(( "CreateDevice failed!\n" ));
        return FALSE;
    }

    /*
     *  Set the data format to "keyboard format".
     *
     *  A data format specifies which controls on a device we
     *  are interested in, and how they should be reported.
     *
     *  This tells DirectInput that we will be passing an array
     *  of 256 bytes to IDirectInputDevice::GetDeviceState.
     *
     *  Parameters:
     *
     *      c_dfDIKeyboard
     *
     *          Predefined data format which describes
     *          an array of 256 bytes, one per scancode.
     */
    hr = Di_keyboard->SetDataFormat(&c_dfDIKeyboard);

    if (FAILED(hr)) {
        mprintf(( "SetDataFormat failed!\n" ));
        return FALSE;
    }


    /*
     *  Set the cooperativity level to let DirectInput know how
     *  this device should interact with the system and with other
     *  DirectInput applications.
     *
     *  Parameters:
     *
     *      DISCL_NONEXCLUSIVE
     *
     *          Retrieve keyboard data when acquired, not interfering
     *          with any other applications which are reading keyboard
     *          data.
     *
     *      DISCL_FOREGROUND
     *
     *          If the user switches away from our application,
     *          automatically release the keyboard back to the system.
     *
     */
	hr = Di_keyboard->SetCooperativeLevel((HWND)os_get_window(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	if (FAILED(hr)) {
		mprintf(( "SetCooperativeLevel failed!\n" ));
		return FALSE;
	}

	DIPROPDWORD hdr;

	// Turn on buffering
	hdr.diph.dwSize = sizeof(DIPROPDWORD); 
	hdr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	hdr.diph.dwObj = 0;		
	hdr.diph.dwHow = DIPH_DEVICE;	// Apply to entire device
	hdr.dwData = 16;	//MAX_BUFFERED_KEYBOARD_EVENTS;

	hr = Di_keyboard->SetProperty( DIPROP_BUFFERSIZE, &hdr.diph );
	if (FAILED(hr)) {
		mprintf(( "SetProperty DIPROP_BUFFERSIZE failed\n" ));
		return FALSE;
	}


	Di_event = CreateEvent( NULL, FALSE, FALSE, NULL );
	Assert(Di_event != NULL);

	Di_thread = CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)di_process, NULL, 0, &Di_thread_id);
	Assert( Di_thread != NULL );

	SetThreadPriority(Di_thread, THREAD_PRIORITY_HIGHEST);

	hr = Di_keyboard->SetEventNotification(Di_event);
	if (FAILED(hr)) {
		mprintf(( "SetEventNotification failed\n" ));
		return FALSE;
	}

	Di_keyboard->Acquire();

	return TRUE;
}

void di_cleanup()
{
    /*
     *  Destroy any lingering IDirectInputDevice object.
     */
    if (Di_keyboard) {

        /*
         *  Cleanliness is next to godliness.  Unacquire the device
         *  one last time just in case we got really confused and tried
         *  to exit while the device is still acquired.
         */
        Di_keyboard->Unacquire();

        Di_keyboard->Release();
        Di_keyboard = NULL;
    }

    /*
     *  Destroy any lingering IDirectInput object.
     */
    if (Di_object) {
        Di_object->Release();
        Di_object = NULL;
    }

	if ( Di_event )	{
		CloseHandle(Di_event);
		Di_event = NULL;
	}

}

#endif
