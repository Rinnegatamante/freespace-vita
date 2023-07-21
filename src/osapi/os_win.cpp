/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/OsApi/OsApi.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Low level Windows code
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:25  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:48  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 * 
 * 
 * 7     6/30/99 5:53p Dave
 * Put in new anti-camper code.
 * 
 * 6     6/03/99 6:37p Dave
 * More TNT fun. Made perspective bitmaps more flexible.
 * 
 * 5     6/02/99 6:18p Dave
 * Fixed TNT lockup problems! Wheeeee!
 * 
 * 4     12/18/98 1:13a Dave
 * Rough 1024x768 support for Direct3D. Proper detection and usage through
 * the launcher.
 * 
 * 3     10/09/98 2:57p Dave
 * Starting splitting up OS stuff.
 * 
 * 2     10/08/98 2:38p Dave
 * Cleanup up OsAPI code significantly. Removed old functions, centralized
 * registry functions.
 * 
 * 118   7/10/98 5:04p Dave
 * Fix connection speed bug on standalone server.
 * 
 * 117   5/24/98 2:28p Hoffoss
 * Because we never really care about if the left or the right shift or
 * alt key was used, but rather than either shift or alt was used, made
 * both map to the left one.  Solves some problems, causes none.
 * 
 * 116   5/18/98 9:22p John
 * Took out the annoying tab check.
 * 
 * 115   5/18/98 11:17a John
 * Fixed some bugs with software window and output window.
 * 
 * 114   5/16/98 2:20p John
 * Changed the os_suspend and resume to use a critical section to prevent
 * threads from executing rather than just suspending the thread.  Had to
 * make sure gr_close was called before os_close.
 * 
 * 113   5/15/98 4:49p John
 * 
 * 112   5/15/98 3:36p John
 * Fixed bug with new graphics window code and standalone server.  Made
 * hwndApp not be a global anymore.
 * 
 * 111   5/14/98 5:42p John
 * Revamped the whole window position/mouse code for the graphics windows.
 * 
 * 110   5/04/98 11:08p Hoffoss
 * Expanded on Force Feedback code, and moved it all into Joy_ff.cpp.
 * Updated references everywhere to it.
 *
 * $NoKeywords: $
 */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <winsock.h>
#include <stdarg.h>
#include <direct.h>

#include "pstypes.h"
#include "osapi.h"
#include "key.h"
#include "palman.h"
#include "mouse.h"
#include "outwnd.h"
#include "2d.h"
#include "cfile.h"
#include "sound.h"
#include "freespaceresource.h"
#include "managepilot.h"
#include "joy.h"
#include "joy_ff.h"
#include "gamesequence.h"
#include "freespace.h"
#include "osregistry.h"
#include "cmdline.h"

// ----------------------------------------------------------------------------------------------------
// OSAPI DEFINES/VARS
//

// os-wide globals
static HINSTANCE	hInstApp;
static HWND			hwndApp = NULL;
static HANDLE		hThread=NULL;
static DWORD		ThreadID;
static int			fAppActive = 0;
static int			main_window_inited = 0;
static char			szWinTitle[128];
static char			szWinClass[128];
static int			WinX, WinY, WinW, WinH;
static int			Os_inited = 0;

static CRITICAL_SECTION Os_lock;

int Os_debugger_running = 0;

// ----------------------------------------------------------------------------------------------------
// OSAPI FORWARD DECLARATIONS
//

#ifdef THREADED
	// thread handler for the main message thread
	DWORD win32_process(DWORD lparam);
#else
	DWORD win32_process1(DWORD lparam);
	DWORD win32_process1(DWORD lparam);
#endif

// Fills in the Os_debugger_running with non-zero if debugger detected.
void os_check_debugger();

// called at shutdown. Makes sure all thread processing terminates.
void os_deinit();

// go through all windows and try and find the one that matches the search string
BOOL __stdcall os_enum_windows( HWND hwnd, char * search_string );

// message handler for the main thread
LRESULT CALLBACK win32_message_handler(HWND hwnd,UINT msg,WPARAM wParam, LPARAM lParam);

// create the main window
BOOL win32_create_window();


// ----------------------------------------------------------------------------------------------------
// OSAPI FUNCTIONS
//

// initialization/shutdown functions -----------------------------------------------

// If app_name is NULL or ommited, then TITLE is used
// for the app name, which is where registry keys are stored.
void os_init(char * wclass, char * title, char *app_name, char *version_string )
{
	os_init_registry_stuff(Osreg_company_name, title, version_string);
	
	strcpy( szWinTitle, title );
	strcpy( szWinClass, wclass );	

	InitializeCriticalSection( &Os_lock );

	#ifdef THREADED
		// Create an even to signal that the window is created, 
		// so that we don't return from this function until 
		// the window is all properly created.
		HANDLE Window_created = CreateEvent( NULL, FALSE, FALSE, NULL );
		hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)win32_process, Window_created, 0, &ThreadID );
		if ( WaitForSingleObject( Window_created, 5000 )==WAIT_TIMEOUT)	{			//INFINITE );
			mprintf(( "Wait timeout!\n" ));
		}
		CloseHandle(Window_created);
		Window_created = NULL;
	#else
		win32_process1(0);
	#endif // THREADED

	// initialized
	Os_inited = 1;

	// check to see if we're running under msdev
	os_check_debugger();

	atexit(os_deinit);
}

// set the main window title
void os_set_title( char * title )
{
	strcpy( szWinTitle, title );
	SetWindowText( hwndApp, szWinTitle );
}

// call at program end
void os_cleanup()
{
	// Tell the app to quit
	PostMessage( hwndApp, WM_DESTROY, 0, 0 );
	
	#ifndef NDEBUG
		outwnd_close();
	#endif
}


// window management -----------------------------------------------------------------

// Returns 1 if app is not the foreground app.
int os_foreground()
{
	return fAppActive;
}

// Returns the handle to the main window
uint os_get_window()
{
	return (uint)hwndApp;
}


// process management -----------------------------------------------------------------

// Sleeps for n milliseconds or until app becomes active.
void os_sleep(int ms)
{
	Sleep(ms);
}

// Used to stop message processing
void os_suspend()
{
	ENTER_CRITICAL_SECTION(&Os_lock);	
}

// resume message processing
void os_resume()
{
	LEAVE_CRITICAL_SECTION(&Os_lock);	
}


// ----------------------------------------------------------------------------------------------------
// OSAPI FORWARD DECLARATIONS
//

#pragma warning(disable:4702)
#ifdef THREADED

// thread handler for the main message thread
DWORD win32_process(DWORD lparam)
{
	MSG msg;
	HANDLE Window_created = (HANDLE)lparam;

	if ( !win32_create_window() )
		return 0;

	// Let the app continue once the window is created
	SetEvent(Window_created);

	while (1)	{	
		if (WaitMessage() == TRUE)	{
			EnterCriticalSection(&Os_lock);			
			while(PeekMessage(&msg,0,0,0,PM_REMOVE))	{
				if ( msg.message == WM_DESTROY )	{
					LeaveCriticalSection(&Os_lock);

					// cleanup and exit this thread!!
					DeleteCriticalSection(&Os_lock);
					return 0;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			LeaveCriticalSection(&Os_lock);
		} 
	}
	return 0;
}

#else
DWORD win32_process1(DWORD lparam)
{
	if ( !win32_create_window() )
		return 0;

	return 0;
}


DWORD win32_process2(DWORD lparam)
{
	MSG msg;

	while(PeekMessage(&msg,0,0,0,PM_REMOVE))	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
#endif // THREADED
#pragma warning(default:4702)

// Fills in the Os_debugger_running with non-zero if debugger detected.
void os_check_debugger()
{
	HMODULE hMod;
	char search_string[256];
	char myname[128];
	int namelen;
	char * p;

	Os_debugger_running = 0;		// Assume its not

	// Find my EXE file name
	hMod = GetModuleHandle(NULL);
	if ( !hMod ) return;
	namelen = GetModuleFileName( hMod, myname, 127 );	
	if ( namelen < 1 ) return;
	
	// Strip off the .EXE
	p = strstr( myname, ".exe" );
	if (!p) return;
	*p = '\0';

	// Move p to point to first letter of EXE filename
	while( (*p!='\\') && (*p!='/') && (*p!=':') )
		p--;
	p++;	
	if ( strlen(p) < 1 ) return;

	// Build what the debugger's window title would be if the debugger is running...
	sprintf( search_string, "[run] - %s -", p );

	// ... and then search for it.
	EnumWindows( (int (__stdcall *)(struct HWND__ *,long))os_enum_windows, (long)&search_string );
}

// called at shutdown. Makes sure all thread processing terminates.
void os_deinit()
{
	if (hThread)	{
		CloseHandle(hThread);
		hThread = NULL;
	}
}

// go through all windows and try and find the one that matches the search string
BOOL __stdcall os_enum_windows( HWND hwnd, char * search_string )
{
	char tmp[128];
	int len;

	len = GetWindowText( hwnd, tmp, 127 );
	 
	if ( len )	{
		if ( strstr( tmp, search_string ))	{
			Os_debugger_running = 1;		// found the search string!
			return FALSE;	// stop enumerating windows
		}
	}

	return TRUE;	// continue enumeration
}


int Got_message = 0;
// message handler for the main thread
LRESULT CALLBACK win32_message_handler(HWND hwnd,UINT msg,WPARAM wParam, LPARAM lParam)
{
	// Got_message++;

	switch(msg)	{

	case WM_QUERYNEWPALETTE:
		// mprintf(( "WM: QueryNewPalette\n" ));
		return TRUE;	// Say that I've realized my own palette
		break;
	case WM_PALETTECHANGED:
		// mprintf(( "WM: PaletteChanged\n" ));
		break;
	case WM_PALETTEISCHANGING:
		// mprintf(( "WM: PaletteIsChanging\n" ));
		break;

	case WM_DISPLAYCHANGE:
		// mprintf(( "WM: DisplayChange\n" ));
		break;

	case WM_LBUTTONDOWN:
		mouse_mark_button( MOUSE_LEFT_BUTTON, 1 );
		break;

	case WM_LBUTTONUP:
		mouse_mark_button( MOUSE_LEFT_BUTTON, 0 );
		break;

	case WM_RBUTTONDOWN:
		mouse_mark_button( MOUSE_RIGHT_BUTTON, 1 );
		break;

	case WM_RBUTTONUP:
		mouse_mark_button( MOUSE_RIGHT_BUTTON, 0 );
		break;

	case WM_MBUTTONDOWN:
		mouse_mark_button( MOUSE_MIDDLE_BUTTON, 1 );
		break;

	case WM_MBUTTONUP:
		mouse_mark_button( MOUSE_MIDDLE_BUTTON, 0 );
		break;

	case WM_TIMER:
		break;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:	{		
			int nVirtKey;
			uint lKeyData;

			int latency;
			latency = timeGetTime() - GetMessageTime();
			if ( latency < 0 )
				latency=0;

			nVirtKey = (int)wParam;    // virtual-key code 
			lKeyData = (lParam>>16) & 255;          // key data 
			if ( (lParam>>16) & 256 ) lKeyData += 0x80;

			// Fix up print screen, whose OEM code is wrong under 95.
			if ( nVirtKey == VK_SNAPSHOT )	{
				lKeyData = KEY_PRINT_SCRN;	
			}

			if (lKeyData == KEY_RSHIFT)  // either shift is just a shift to us..
				lKeyData = KEY_LSHIFT;

			if (lKeyData == KEY_RALT)  // Same with alt keys..
				lKeyData = KEY_LALT;

//			mprintf(( "Key down = 0x%x|%x\n", lKeyData, nVirtKey ));
			key_mark( lKeyData, 1, latency );
//			mprintf(( "Key down = 0x%x\n", lKeyData ));
			//Warning( LOCATION, "Key = 0x%x", lKeyData );			
		}
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:	 {		
			int nVirtKey;
			uint lKeyData;

			int latency;
			latency = timeGetTime() - GetMessageTime();
			if ( latency < 0 )
				latency=0;

			nVirtKey = (int) wParam;    // virtual-key code 
			lKeyData = (lParam>>16) & 255;          // key data 
			if ( (lParam>>16) & 256 ) lKeyData += 0x80;

			// Fix up print screen, whose OEM code is wrong under 95.
			if ( nVirtKey == VK_SNAPSHOT )	{
				lKeyData = KEY_PRINT_SCRN;	
			}

			if (lKeyData == KEY_RSHIFT)  // either shift is just a shift to us..
				lKeyData = KEY_LSHIFT;

			if (lKeyData == KEY_RALT)  // Same with alt keys..
				lKeyData = KEY_LALT;

//			mprintf(( "Key up = 0x%x|%x\n", lKeyData, nVirtKey ));
			if ( lKeyData == 0xB7 )	{
				// Hack for PrintScreen which only sends one up message!
				key_mark( lKeyData, 1, latency );		
				key_mark( lKeyData, 0, latency );

			} else {
				key_mark( lKeyData, 0, latency );
			}			
		}		
		break;

	case WM_KILLFOCUS:
		key_lost_focus();

		gr_activate(0);
		break;

	case WM_SETFOCUS:
		key_got_focus();

		gr_activate(1);
		break;

	case WM_ACTIVATE:			//APP:	//
	case WM_ACTIVATEAPP:
		{
			BOOL OldfAppActive = fAppActive;
			
			// The application z-ordering has changed. If we are now the
			// foreground application wParm will be TRUE.
			if ( msg == WM_ACTIVATE )	{
				if ( (LOWORD(wParam) == WA_ACTIVE) || (LOWORD(wParam)==WA_CLICKACTIVE))	{
					fAppActive = TRUE;	//(BOOL)wParam;
				} else {
					fAppActive = FALSE;	//(BOOL)wParam;
				}

			} else {
				fAppActive = (BOOL)wParam;
			}

			//mprintf(( "Activate: %d\n", fAppActive ));

			if ( OldfAppActive != fAppActive )	{

				if ( fAppActive )	{
					// maximize it
					//	mprintf(( "Priority: HIGH\n" ));
					joy_reacquire_ff();

#ifdef THREADED
					SetThreadPriority( hThread, THREAD_PRIORITY_HIGHEST );
#endif

					gr_activate(fAppActive);
					//SetThreadPriority( hThread, THREAD_PRIORITY_TIME_CRITICAL );
				} else {
					//mprintf(( "Priority: LOW\n" ));
					joy_unacquire_ff();
					if (Mouse_hidden)	{
						Mouse_hidden=0;
						//ShowCursor(1);
						//mprintf(( "Mouse:Alive\n" ));		
					}
					// minimize it
					// key_outkey( KEY_PAUSE );

#ifdef THREADED
					SetThreadPriority( hThread, THREAD_PRIORITY_NORMAL );
#endif
					gr_activate(fAppActive);
				}
			}
		}
		break;

	case WM_DESTROY:
		// mprintf(( "WM_DESTROY called\n" ));
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		gameseq_post_event(GS_EVENT_QUIT_GAME);
		break;

	case WM_SYSCOMMAND:
		// mprintf(( "Sys command called '%x'\n", wParam ));
		 if ( wParam != SC_SCREENSAVE ){
			 return DefWindowProc(hwnd, msg, wParam, lParam);
		 }
		 break;

/*
	case MM_WIM_DATA:
		rtvoice_stream_data((uint)hwnd, (uint)wParam, (uint)lParam);
		break;
*/

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

// create the main window
BOOL win32_create_window()
{
	int windowed = 0;
	int hires = 0;
	char *ptr = os_config_read_string(NULL, NOX("Videocard"), NULL);	
	if(ptr && strstr(ptr, NOX("Direct 3D -")) && Cmdline_window){
		windowed = 1;
	}
	if(ptr && strstr(ptr, NOX("1024")) && Cmdline_window){
		hires = 1;
	}

	WNDCLASSEX wclass;
	HINSTANCE hInst = GetModuleHandle(NULL);

	wclass.hInstance 		= hInst;
	wclass.lpszClassName = szWinClass;
	wclass.lpfnWndProc	= (WNDPROC)win32_message_handler;		
	if(windowed){
		wclass.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	} else {
		wclass.style			= CS_BYTEALIGNCLIENT|CS_VREDRAW | CS_HREDRAW;	// | CS_OWNDC;	//CS_DBLCLKS | CS_PARENTDC| CS_VREDRAW | CS_HREDRAW |;	
	}		
	wclass.cbSize			= sizeof(WNDCLASSEX);
	wclass.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_APP_ICON) );
	wclass.hIconSm			= NULL;	//LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1) );
	wclass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wclass.lpszMenuName	= NULL;	//"FreeSpaceMenu";
	wclass.cbClsExtra		= 0;
	wclass.cbWndExtra		= 0;
	wclass.hbrBackground = (HBRUSH)NULL;

	if (!RegisterClassEx(&wclass)) return FALSE;
	
	int style;
	if(windowed){
		style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	} else {
		style = WS_POPUP;
	}	

	// make sure we adjust for the actual window border	
	int x_add = 0;
	int y_add = 0;
	x_add += GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	y_add += GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION);

	// Make a 32x32 window.  It never should get shown, because the graphics code
	// will then size it.
	if(windowed){
		hwndApp = CreateWindow( szWinClass, szWinTitle,
									style,   
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									hires ? 1024 : 640 + x_add,
									hires ? 768 : 480 + y_add,
									NULL, (HMENU)NULL, hInst, (LPSTR)NULL);	
	} else {
		// Make a 32x32 window.  It never should get shown, because the graphics code
		// will then size it.
		hwndApp = CreateWindow( szWinClass, szWinTitle,
									style,   
									(GetSystemMetrics( SM_CXSCREEN )-32 )/2,	//x
									(GetSystemMetrics( SM_CYSCREEN )-32 )/2,	//y
									32,32,									// wh
									NULL, (HMENU)NULL, hInst, (LPSTR)NULL);	
	}

	// Hack!! Turn off Window's cursor.
	ShowCursor(0);
	ClipCursor(NULL);

	main_window_inited = 1;
	#ifndef NDEBUG
		outwnd_init(1);
	#endif	

	if(windowed){
		ShowWindow( hwndApp, SW_SHOWNORMAL );
		UpdateWindow( hwndApp );
	}

	return TRUE;
}

void os_poll()
{
#ifndef THREADED
	win32_process2(0);
#else
	MSG msg;
	EnterCriticalSection(&Os_lock);
	while(PeekMessage(&msg,0,0,0,PM_NOREMOVE))	{		
		if ( msg.message == WM_DESTROY )	{
			break;
		}
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
		Got_message++;
	}
	LeaveCriticalSection(&Os_lock);
#endif
}

void debug_int3()
{
	gr_activate(0);
	_asm { int 3 };
	gr_activate(1);
}

