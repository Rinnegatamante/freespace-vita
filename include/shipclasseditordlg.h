/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/ShipClassEditorDlg.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Ship class editor dialog handling code
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
 * 2     2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 *
 * $NoKeywords: $
 */

/////////////////////////////////////////////////////////////////////////////
// CShipClassEditorDlg dialog

class CShipClassEditorDlg : public CDialog
{
// Construction
public:
	CShipClassEditorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShipClassEditorDlg)
	enum { IDD = IDD_SHIP_CLASS_EDITOR };
	CButton	m_SoundsEditor;
	CStatic	m_ShipClassWindow;
	CButton	m_ShipClassNew;
	CButton	m_ShipClassDelete;
	CButton	m_ModelsEditor;
	CButton	m_GoalsEditor;
	BOOL	m_ShipClassAfterburner;
	int		m_ShipClassAIClass;
	CString	m_ShipClassArmor;
	BOOL	m_ShipClassCloak;
	int		m_ShipClassDebrisModel;
	int		m_ShipClassModel;
	CString	m_ShipClassEngine;
	CString	m_ShipClassExplosion1;
	CString	m_ShipClassExplosion2;
	CString	m_ShipClassIFF;
	CString	m_ShipClassManufacturer;
	int		m_ShipClassMaxBank;
	int		m_ShipClassMaxPitch;
	int		m_ShipClassMaxRoll;
	int		m_ShipClassMaxSpeed;
	CString	m_ShipClassName;
	CString	m_ShipClassPowerPlant;
	int		m_ShipClassScore;
	int		m_ShipClassShields;
	BOOL	m_ShipClassWarpdrive;
	CString	m_ShipClassTurretWeapon1;
	CString	m_ShipClassTurretWeapon2;
	CString	m_ShipClassWeaponSpecial;
	CString	m_ShipClassWeapon1;
	CString	m_ShipClassWeapon2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShipClassEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShipClassEditorDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

