/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/OrientEditor.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Object orientation editor (or just object editor) dialog box handling code
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
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
 * 8     5/20/97 2:28p Hoffoss
 * Added message box queries for close window operation on all modal
 * dialog boxes.
 * 
 * 7     4/17/97 2:01p Hoffoss
 * All dialog box window states are saved between sessions now.
 * 
 * 6     3/12/97 4:33p Hoffoss
 * added spin controls to orient editor, light intensity level can be
 * specified in BG editor.
 * 
 * 5     2/21/97 5:34p Hoffoss
 * Added extensive modification detection and fixed a bug in initial
 * orders editor.
 * 
 * 4     2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 *
 * $NoKeywords: $
 */

#include "object.h"

/////////////////////////////////////////////////////////////////////////////
// orient_editor dialog

class orient_editor : public CDialog
{
// Construction
public:
	int query_modified();
	void OnCancel();
	void OnOK();
	orient_editor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(orient_editor)
	enum { IDD = IDD_ORIENT_EDITOR };
	CSpinButtonCtrl	m_spin6;
	CSpinButtonCtrl	m_spin5;
	CSpinButtonCtrl	m_spin4;
	CSpinButtonCtrl	m_spin3;
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	int	m_object_index;
	BOOL	m_point_to;
	CString	m_position_z;
	CString	m_position_y;
	CString	m_position_x;
	CString	m_location_x;
	CString	m_location_y;
	CString	m_location_z;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(orient_editor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(orient_editor)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	float convert(CString &str);
	int total;
	int index[MAX_OBJECTS];
	void update_object(object *ptr);
};

