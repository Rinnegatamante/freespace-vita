/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/fred2/PlayerStartEditor.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Player starting point editor dialog box handling code
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     2/23/99 7:03p Dave
 * Rewrote a horribly mangled and evil team loadout dialog. Bugs gone.
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
 * 17    1/15/98 5:04p Hoffoss
 * Fixed functionality of team loadout editor.
 * 
 * 16    10/03/97 6:16p Hoffoss
 * Added weapon loadout editing to player editor (now called team loadout
 * editor)
 * 
 * 15    9/16/97 9:41p Hoffoss
 * Changed Fred code around to stop using Parse_player structure for
 * player information, and use actual ships instead.
 * 
 * 14    8/16/97 2:16p Hoffoss
 * Made changed to how checkboxes work.
 * 
 * 13    8/13/97 10:17p Hoffoss
 * Added player entry delay support to Fred, and moved the
 * Player_entry_delay variable to a common library instead of FreeSpace.
 * 
 * 12    5/20/97 2:28p Hoffoss
 * Added message box queries for close window operation on all modal
 * dialog boxes.
 * 
 * 11    4/17/97 2:01p Hoffoss
 * All dialog box window states are saved between sessions now.
 * 
 * 10    3/11/97 5:41p Hoffoss
 * Player start editor now displays wing ship counts.  Removed default
 * player ship selection box.
 * 
 * 9     3/11/97 4:52p Hoffoss
 * changed player start editor dialog functioning.
 * 
 * 8     2/26/97 5:31p Hoffoss
 * Added support for default player ship in player editor dialog box.
 * 
 * 7     2/21/97 5:34p Hoffoss
 * Added extensive modification detection and fixed a bug in initial
 * orders editor.
 * 
 * 6     2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 * 
 * 5     1/30/97 2:24p Hoffoss
 * Added remaining mission file structures and implemented load/save of
 * them.
 *
 * $NoKeywords: $
 */

#include "shipchecklistbox.h"

/////////////////////////////////////////////////////////////////////////////
// player_start_editor dialog

class player_start_editor : public CDialog
{
// Construction
public:	
	player_start_editor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(player_start_editor)
	enum { IDD = IDD_LOADOUT_EDITOR };
	CSpinButtonCtrl	m_pool_spin;
	CSpinButtonCtrl	m_delay_spin;
	CSpinButtonCtrl	m_spin1;
	CCheckListBox		m_ship_list;
	CCheckListBox		m_weapon_list;	
	int					m_delay;	
	int					m_weapon_pool;
	int					m_ship_pool;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(player_start_editor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(player_start_editor)
	virtual BOOL OnInitDialog();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnSelchangeShipList();		
	afx_msg void OnSelchangeWeaponList();	
	afx_msg void OnUpdateShipPool();
	afx_msg void OnUpdateWeaponPool();
	void OnCancel();	
	void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// if we've finished initializing the dialog
	int dlg_inited;

	// team we're currently working with
	int selected_team;

	// ship pool info
	int ship_pool[MAX_TEAMS][MAX_SHIP_TYPES];

	// weapon pool info
	int weapon_pool[MAX_TEAMS][MAX_WEAPON_TYPES];

	// regenerate all controls
	void reset_controls();
};

