/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/Grid.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Grid dialog box created by Mike.  Probably will never be used again.
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
 * 5     6/18/97 11:36p Lawrance
 * move grid rendering code to MissionGrid.cpp
 * 
 * 4     2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "fred.h"
#include "grid.h"

#include "3d.h"
#include "object.h"
#include "editor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int double_fine_gridlines;

/////////////////////////////////////////////////////////////////////////////
// CGrid dialog

//	Modeless constructor, MK
CGrid::CGrid(CView* pView)
{
	m_pGView = pView;
}


CGrid::CGrid(CWnd* pParent /*=NULL*/)
	: CDialog(CGrid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrid)
	m_GridSize = 0;
	//}}AFX_DATA_INIT

	m_pGView = NULL;

}


void CGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrid)
	DDX_Text(pDX, IDC_GRID_SIZE, m_GridSize);
	DDV_MinMaxUInt(pDX, m_GridSize, 2, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGrid, CDialog)
	//{{AFX_MSG_MAP(CGrid)
	ON_BN_CLICKED(IDC_GRID_XY_PLANE, OnGridXyPlane)
	ON_BN_CLICKED(IDC_GRID_XZ_PLANE, OnGridXzPlane)
	ON_BN_CLICKED(IDC_GRID_YZ_PLANE, OnGridYzPlane)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrid message handlers

extern grid *The_grid;

void GridOrient(vector *forward, vector *right)
{
	vector	center;
	int		nrows, ncols;
	float	square_size;

	if (The_grid != NULL){
		center = The_grid->center;
		nrows = The_grid->nrows;
		ncols = The_grid->ncols;
		square_size = The_grid->square_size;
	} else {
		vm_vec_make(&center, 0.0f, 0.0f, 0.0f);
		nrows = 20;
		ncols = 20;
		square_size = 2.0f;
	}

	The_grid = create_grid(The_grid, forward,
		right,
		&center,
		nrows, ncols,
		square_size);

	physics_init(&The_grid->physics);
}

void CGrid::OnGridXyPlane() 
{
	vector	forward, right;

	vm_vec_make(&forward, 0.0f, 1.0f, 0.0f);
	vm_vec_make(&right, 1.0f, 0.0f, 0.0f);

	GridOrient(&forward, &right);
}

void CGrid::OnGridXzPlane() 
{
	vector	forward, right;

	vm_vec_make(&forward, 0.0f, 0.0f, 1.0f);
	vm_vec_make(&right, 1.0f, 0.0f, 0.0f);

	GridOrient(&forward, &right);
}

void CGrid::OnGridYzPlane() 
{
	vector	forward, right;

	vm_vec_make(&forward, 0.0f, 1.0f, 0.0f);
	vm_vec_make(&right, 0.0f, 0.0f, 1.0f);

	GridOrient(&forward, &right);	
}

BOOL CGrid::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CGrid::Create()
{
	return CDialog::Create(CGrid::IDD);
}


void CGrid::OnClose() 
{
	DestroyWindow();
}

void CGrid::OnDestroy() 
{
	UpdateData(TRUE);

	CDialog::OnDestroy();
	
}

void CGrid::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	DestroyWindow();

}

BOOL CGrid::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData(TRUE);
	
	return CDialog::DestroyWindow();
}

BOOL CGrid::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_GRID_SIZE);
	pSpin->SetRange(2, 20);
	if ((m_GridSize < 2) || (m_GridSize > 20))
		m_GridSize = The_grid->ncols/5;
	pSpin->SetPos(m_GridSize);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	strValue;

	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_GRID_SIZE);
	strValue.Format("%i", pSpin->GetPos());
	pSpin->GetBuddy()->SetWindowText(strValue);

	The_grid->nrows = pSpin->GetPos()*5;
	The_grid->ncols = The_grid->nrows;

	modify_grid(The_grid);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

