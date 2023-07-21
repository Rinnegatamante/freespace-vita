/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/FRED.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * FRED.h : main header file for the FRED application
 * Global editor dialog box classes are instantiated here, initializes the
 * application (MFC level at least), processes the INI file.
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 6:28p Dave
 * Initial checkin. Renamed all relevant stuff to be Fred2 instead of
 * Fred. Globalized mission and campaign file extensions. Removed Silent
 * Threat specific code.
 * 
 * 1     10/07/98 3:01p Dave
 * 
 * 1     10/07/98 3:00p Dave
 * 
 * 32    3/23/98 4:04p Hoffoss
 * Fixed dialog window initialization so it looks better at startup (they
 * don't flash on for a second).
 * 
 * 31    9/09/97 10:25a Hoffoss
 * Fixed a potential problem.
 * 
 * 30    8/01/97 3:10p Hoffoss
 * Made Sexp help hidable.
 * 
 * 29    6/18/97 11:46a Hoffoss
 * Fixed initial order object reference updating and added briefing dialog
 * window tracking data.
 * 
 * 28    6/05/97 6:10p Hoffoss
 * Added features: Autosaving, object hiding.  Also fixed some minor bugs.
 * 
 * 27    6/02/97 11:52a Hoffoss
 * Custom cursors displayed when over objects in different modes.
 * 
 * 26    5/29/97 5:15p Allender
 * fixed macro for MODIFY so that it will work properly within 'if'
 * statements
 * 
 * 25    4/29/97 12:24p Adam
 * JAS:   Added code for delayed point to vec.   Fixed some FRED
 * sequencing problems with g3_start_frame / g3_end_frame.
 * 
 * 24    4/17/97 2:01p Hoffoss
 * All dialog box window states are saved between sessions now.
 * 
 * 23    4/01/97 11:07p Mike
 * Clean up game sequencing functions.  Get rid of Multiplayer and add
 * Game_mode.  Add SystemVars.cpp
 * 
 * 22    3/26/97 11:45a Hoffoss
 * Fred uses fvi_ray_* functions now (as much as possible) for checking
 * mouse selection stuff.
 * 
 * 21    2/28/97 11:31a Hoffoss
 * Implemented modeless dialog saving and restoring, and changed some
 * variables names.
 * 
 * 20    2/27/97 5:54p Hoffoss
 * Implemented support for saving and restoring window positions.
 * 
 * 19    2/24/97 12:50p Hoffoss
 * First attempt at non-continuous redrawing.
 * 
 * 18    2/20/97 5:42p Hoffoss
 * Fixed bug in modification checking and updating macro.
 * 
 * 17    2/20/97 4:28p Hoffoss
 * Added modification tracking to ship editor dialog box, and support
 * functions.
 * 
 * 16    2/20/97 4:03p Hoffoss
 * Several ToDo items: new reinforcement clears arrival cue, reinforcement
 * control from ship and wing dialogs, show grid toggle.
 * 
 * 15    2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 * 
 * 14    2/12/97 12:25p Hoffoss
 * Expanded on global error checker, added initial orders conflict
 * checking and warning, added waypoint editor dialog and code.
 * 
 * 13    1/30/97 2:24p Hoffoss
 * Added remaining mission file structures and implemented load/save of
 * them.
 *
 * $NoKeywords: $
 */

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "systemvars.h"		//	Low level variables, common to FreeSpace and Fred
#include "resource.h"       // main symbols
#include "missionparse.h"
#include "shipeditordlg.h"
#include "wing_editor.h"
#include "waypointpathdlg.h"
#include "bgbitmapdlg.h"
#include "briefingeditordlg.h"
#include "systemvars.h"

#define MODIFY(a, b) do {	\
	if (a != (b)) {			\
		a = (b);					\
		set_modified();		\
	}								\
} while(0)

#define	F_RENDER_SHIP_MODELS	0x01
#define	F_RENDER_SHIP_ICONS	0x02

// user interface types
#define HOFFOSS_INTERFACE	1
#define ALLENDER_INTERFACE	2

typedef struct window_data {
	WINDOWPLACEMENT p;
	int visible;
	int valid;
	int processed;
} window_data;

/////////////////////////////////////////////////////////////////////////////
// CFREDApp:
// See FRED.cpp for the implementation of this class
//

class CFREDApp : public CWinApp
{
	int app_init;

public:
	void record_window_data(window_data *wndd, CWnd *wnd);
	int init_window(window_data *wndd, CWnd *wnd, int adjust = 0, int pre = 0);
	void read_window(char *name, window_data *wndd);
	void write_window(char *name, window_data *wndd);
	void write_ini_file(int degree = 0);
	CFREDApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFREDApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFREDApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//	Add a message to be processed at the end of this frame.
//	This is useful if you need the display to update before it's useful
//	to process the message.  For example, right click brings up a popup menu.
//	But the menu it brings up depends on where you right clicked.  If you
//	right click on a ship, you get a message that pertains to the chosen
//	ship.  It is useful to have a visual indication that you have changed the
//	current ship.
void add_pending_message(HWND hwnd, int id, int wparam, int lparam, int skip_count);
void init_pending_messages(void);
void update_map_window();

extern int User_interface;
extern int Fred_active;
extern int Update_window;
extern HCURSOR h_cursor_move, h_cursor_rotate;

extern CWnd *Prev_window;
extern CShipEditorDlg	Ship_editor_dialog;
extern wing_editor		Wing_editor_dialog;
extern waypoint_path_dlg	Waypoint_editor_dialog;
extern bg_bitmap_dlg		*Bg_bitmap_dialog;
extern briefing_editor_dlg	*Briefing_dialog;

extern CFREDApp theApp;

extern window_data Main_wnd_data;
extern window_data Ship_wnd_data;
extern window_data Wing_wnd_data;
extern window_data Object_wnd_data;
extern window_data Mission_goals_wnd_data;
extern window_data Messages_wnd_data;
extern window_data Player_wnd_data;
extern window_data Events_wnd_data;
extern window_data Bg_wnd_data;
extern window_data Briefing_wnd_data;
extern window_data Reinforcement_wnd_data;
extern window_data Waypoint_wnd_data;
extern window_data Starfield_wnd_data;
extern window_data Asteroid_wnd_data;
extern window_data Mission_notes_wnd_data;

