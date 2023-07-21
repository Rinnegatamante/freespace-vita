/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

#include "shipeditordlg.h"

// ShipFlagsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ship_flags_dlg dialog

class ship_flags_dlg : public CDialog
{
// Construction
public:
	ship_flags_dlg(CWnd* pParent = NULL);   // standard constructor
	void OnOK();
	void update_ship(int ship);
	void setup(int n);
	int tristate_set(int val, int cur_state);
	void set_modified();

// Dialog Data
	//{{AFX_DATA(ship_flags_dlg)
	enum { IDD = IDD_SHIP_FLAGS };
	CButton	m_red_alert_carry;
	CButton	m_scannable;
	CButton	m_reinforcement;
	CButton	m_protect_ship;
	CButton	m_beam_protect_ship;
	CButton	m_no_dynamic;
	CButton	m_no_arrival_music;
	CButton	m_kamikaze;
	CButton	m_invulnerable;
	CButton	m_ignore_count;
	CButton	m_hidden;
	CButton	m_escort;
	CButton	m_destroy;
	CButton	m_cargo_known;
	CButton	m_special_warp;
	CSpinButtonCtrl	m_destroy_spin;
	numeric_edit_control m_kdamage;
	numeric_edit_control	m_destroy_value;
	numeric_edit_control m_escort_value;
	numeric_edit_control m_respawn_priority;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ship_flags_dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int p_enable;  // used to enable(1)/disable(0) controls based on if a player ship

	// Generated message map functions
	//{{AFX_MSG(ship_flags_dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCargoKnown();
	afx_msg void OnDestroyCheck();
	afx_msg void OnEscort();
	afx_msg void OnHiddenFromSensors();
	afx_msg void OnIgnoreCount();
	afx_msg void OnInvulnerable();
	afx_msg void OnKamikaze();
	afx_msg void OnNoArrivalMusic();
	afx_msg void OnNoDynamic();
	afx_msg void OnProtectShip();
	afx_msg void OnBeamProtectShip();
	afx_msg void OnReinforcement();
	afx_msg void OnScannable();
	afx_msg void OnRedalertcarry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

