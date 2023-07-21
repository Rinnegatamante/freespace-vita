/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/ShieldSysDlg.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Shield generator system editor.  This dialog allows one to indicate whether all ships
 * (on a certain team or of a certain type) have a shield system or not.
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:44  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
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
 * 2     8/18/97 9:31p Hoffoss
 * Added grid adjustment dialog and shield system editor dialog.
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "fred.h"
#include "shieldsysdlg.h"
#include "ship.h"
#include "missionparse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int Shield_sys_teams[MAX_TEAM_NAMES] = {0};
int Shield_sys_types[MAX_SHIP_TYPES] = {0};

/////////////////////////////////////////////////////////////////////////////
// shield_sys_dlg dialog

shield_sys_dlg::shield_sys_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(shield_sys_dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(shield_sys_dlg)
	m_team = 0;
	m_type = 0;
	//}}AFX_DATA_INIT
}

void shield_sys_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(shield_sys_dlg)
	DDX_CBIndex(pDX, IDC_TEAM, m_team);
	DDX_CBIndex(pDX, IDC_TYPE, m_type);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(shield_sys_dlg, CDialog)
	//{{AFX_MSG_MAP(shield_sys_dlg)
	ON_CBN_SELCHANGE(IDC_TEAM, OnSelchangeTeam)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// shield_sys_dlg message handlers

BOOL shield_sys_dlg::OnInitDialog() 
{
	int i, z;
	int teams[MAX_TEAM_NAMES];
	int types[MAX_SHIP_TYPES];
	CComboBox *box;

	for (i=0; i<MAX_TEAM_NAMES; i++)
		teams[i] = 0;

	for (i=0; i<MAX_SHIP_TYPES; i++)
		types[i] = 0;

	for (i=0; i<MAX_SHIPS; i++)
		if (Ships[i].objnum >= 0) {
			z = (Objects[Ships[i].objnum].flags & OF_NO_SHIELDS) ? 1 : 0;
			if (!teams[Ships[i].team])
				Shield_sys_teams[Ships[i].team] = z;
			else if (Shield_sys_teams[Ships[i].team] != z)
				Shield_sys_teams[Ships[i].team] = 2;

			if (!types[Ships[i].ship_info_index])
				Shield_sys_types[Ships[i].ship_info_index] = z;
			else if (Shield_sys_types[Ships[i].ship_info_index] != z)
				Shield_sys_types[Ships[i].ship_info_index] = 2;

			teams[Ships[i].team]++;
			types[Ships[i].ship_info_index]++;
		}

	box = (CComboBox *) GetDlgItem(IDC_TYPE);
	box->ResetContent();
	for (i=0; i<Num_ship_types; i++)
		box->AddString(Ship_info[i].name);

	box = (CComboBox *) GetDlgItem(IDC_TEAM);
	box->ResetContent();
	for (i=0; i<Num_team_names; i++)
		box->AddString(Team_names[i]);

	CDialog::OnInitDialog();
	set_team();
	set_type();
	return TRUE;
}

void shield_sys_dlg::OnOK() 
{
	int i, z;

	OnSelchangeTeam();
	OnSelchangeType();
	for (i=0; i<MAX_SHIPS; i++)
		if (Ships[i].objnum >= 0) {
			z = Shield_sys_teams[Ships[i].team];
			if (!Shield_sys_types[Ships[i].ship_info_index])
				z = 0;
			else if (Shield_sys_types[Ships[i].ship_info_index] == 1)
				z = 1;

			if (!z)
				Objects[Ships[i].objnum].flags &= ~OF_NO_SHIELDS;
			else if (z == 1)
				Objects[Ships[i].objnum].flags |= OF_NO_SHIELDS;
		}

	CDialog::OnOK();
}

void shield_sys_dlg::OnSelchangeTeam()
{
	Assert(m_team >= 0);
	if (((CButton *) GetDlgItem(IDC_TEAM_YES))->GetCheck())
		Shield_sys_teams[m_team] = 0;
	else if (((CButton *) GetDlgItem(IDC_TEAM_NO))->GetCheck())
		Shield_sys_teams[m_team] = 1;

	UpdateData(TRUE);
	set_team();
}

void shield_sys_dlg::set_team()
{
	if (!Shield_sys_teams[m_team])
		((CButton *) GetDlgItem(IDC_TEAM_YES))->SetCheck(TRUE);
	else
		((CButton *) GetDlgItem(IDC_TEAM_YES))->SetCheck(FALSE);

	if (Shield_sys_teams[m_team] == 1)
		((CButton *) GetDlgItem(IDC_TEAM_NO))->SetCheck(TRUE);
	else
		((CButton *) GetDlgItem(IDC_TEAM_NO))->SetCheck(FALSE);
}

void shield_sys_dlg::OnSelchangeType()
{
	Assert(m_type >= 0);
	if (((CButton *) GetDlgItem(IDC_TYPE_YES))->GetCheck())
		Shield_sys_types[m_type] = 0;
	else if (((CButton *) GetDlgItem(IDC_TYPE_NO))->GetCheck())
		Shield_sys_types[m_type] = 1;

	UpdateData(TRUE);
	set_type();
}

void shield_sys_dlg::set_type()
{
	if (!Shield_sys_types[m_type])
		((CButton *) GetDlgItem(IDC_TYPE_YES))->SetCheck(TRUE);
	else
		((CButton *) GetDlgItem(IDC_TYPE_YES))->SetCheck(FALSE);

	if (Shield_sys_types[m_type] == 1)
		((CButton *) GetDlgItem(IDC_TYPE_NO))->SetCheck(TRUE);
	else
		((CButton *) GetDlgItem(IDC_TYPE_NO))->SetCheck(FALSE);
}

