/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/fred2/ShipEditorDlg.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Single ship editing dialog
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:17  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:44  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 8     8/18/99 1:32p Andsager
 * Mark Vasudan wingmen as such.
 * 
 * 7     8/18/99 1:12p Andsager
 * Allow ships to be assigned Vasudan persona (wingman 6)
 * 
 * 6     8/16/99 10:52p Andsager
 * Allow closer ship positioning for NEAR_SHIP ship and wing arrivals.
 * 
 * 5     8/11/99 9:27a Andsager
 * Fix compile warning.
 * 
 * 4     5/20/99 6:59p Dave
 * Added alternate type names for ships. Changed swarm missile table
 * entries.
 * 
 * 3     2/11/99 2:15p Andsager
 * Add ship explosion modification to FRED
 * 
 * 2     10/07/98 6:28p Dave
 * Initial checkin. Renamed all relevant stuff to be Fred2 instead of
 * Fred. Globalized mission and campaign file extensions. Removed Silent
 * Threat specific code.
 * 
 * 1     10/07/98 3:02p Dave
 * 
 * 1     10/07/98 3:00p Dave
 * 
 * 157   6/16/98 10:17a Hoffoss
 * Fixed define that was backwards and no one caught before release (ya,
 * great testing..)
 * 
 * 156   5/23/98 3:33p Hoffoss
 * Removed unused code in reinforcements editor and make ships.tbl button
 * in ship editor disappear in release build.
 * 
 * 155   5/21/98 12:58a Hoffoss
 * Fixed warnings optimized build turned up.
 * 
 * 154   4/28/98 2:13p Hoffoss
 * Added code to help keep invalid player ship types from existing in
 * mission.
 * 
 * 153   4/24/98 2:57p Jim
 * Fixed typo in code.
 * 
 * 152   4/22/98 11:55a Hoffoss
 * Fixed bug with ship editor's hide cue button when sexp help is on at
 * startup.
 * 
 * 151   4/16/98 5:59p Duncan
 * Fixed bad assumption, apparently.
 * 
 * 150   4/07/98 9:42a Allender
 * put in persona combo box into ship editor.  Removed code to assign
 * personas based on message
 * 
 * 149   3/30/98 1:15p Hoffoss
 * Fixed bug with arrival/departure cue window size calculation.  Wasn't
 * working anymore after my changes to dialog initing at startup.
 * 
 * 148   3/27/98 12:02p Sandeep
 * 
 * 147   3/25/98 4:14p Hoffoss
 * Split ship editor up into ship editor and a misc dialog, which tracks
 * flags and such.
 * 
 * 146   3/21/98 7:36p Lawrance
 * Move jump nodes to own lib.
 * 
 * 145   3/16/98 8:27p Allender
 * Fred support for two new AI flags -- kamikaze and no dynamic goals.
 * 
 * 144   3/10/98 6:11p Hoffoss
 * Added jump node renaming abilities to Fred.
 * 
 * 143   3/09/98 4:30p Allender
 * multiplayer secondary weapon changes.  red-alert and cargo-known-delay
 * sexpressions.  Add time cargo revealed to ship structure
 * 
 * 142   2/22/98 1:21a Hoffoss
 * Disabled weapon editor if mutliple ship types marked.
 * 
 * 141   2/17/98 12:07p Hoffoss
 * Changed over to using SF_CARGO_REVEALED in fred.
 * 
 * 140   2/17/98 11:42a Hoffoss
 * Added support for hidden from sensors condition.
 * 
 * 139   2/13/98 2:42p Duncan
 * Fixed bug, moved line down where it needs to be because
 * pre-initializations need to be done first.
 * 
 * 138   2/10/98 1:42p Allender
 * allow > MAX_ESCORT_SHIPS to be marked as escort ships
 * 
 * 137   2/06/98 2:54p Hoffoss
 * Fixed some bugs in dialog init, and cleared up some of the confusion
 * about how it works by renaming some variables and adding comments.
 * 
 * 136   2/06/98 10:48a Hoffoss
 * Fixed bug with not being able to make ships players in multi mode
 * missions.
 * 
 * 135   2/04/98 4:32p Allender
 * support for multiple briefings and debriefings.  Changes to mission
 * type (now a bitfield).  Bitfield defs for multiplayer modes
 * 
 * 134   1/29/98 5:14p Hoffoss
 * Added support for a SF_INVULNERABLE ship flag in Fred.
 * 
 * 133   1/12/98 10:41a Allender
 * fixed minor bug with ship editor and ignore orders dialog
 * 
 * 132   12/05/97 4:07p Hoffoss
 * Changed code to allow WHO_FROM type ship sources to only show flyable
 * ships in list.
 * 
 * 131   11/24/97 2:31p Allender
 * allow ignore orders dialog to be active for players
 * 
 * 130   11/24/97 9:07a Allender
 * ignore orders dialog should function as multi-edit now
 * 
 * 129   11/22/97 4:17p Allender
 * first pass of making ignore orders dialog multi edit
 * 
 * 128   11/13/97 4:14p Allender
 * automatic assignment of hotkeys for starting wings.  Appripriate
 * warnings when they are incorrectly used.  hotkeys correctly assigned to
 * ships/wing arriving after mission start
 * 
 * 127   11/11/97 3:32p Johnson
 * allender:  Combo boxes need to have lists initialized before setting
 * internal class variable for arrival/depature targets
 * 
 * 126   11/11/97 2:13p Allender
 * docking bay support for Fred and Freespace.  Added hook to ai code for
 * arrival/departure from dock bays.  Fred support now sufficient.
 * 
 * 125   11/10/97 10:13p Allender
 * added departure anchor to Fred and Freespace in preparation for using
 * docking bays.  Functional in Fred, not in FreeSpace.
 * 
 * 124   10/21/97 4:49p Allender
 * added flags to Fred and FreeSpace to forgo warp effect (toggle in ship
 * editor in Fred)
 * 
 * 123   10/14/97 5:33p Hoffoss
 * Added Fred support (and fsm support) for the no_arrival_music flags in
 * ships and wings.
 * 
 * 122   9/18/97 10:49a Allender
 * increment/decrement Player_start variable when making a ship a player
 * start
 * 
 * 121   9/17/97 5:43p Hoffoss
 * Added Fred support for new player start information.
 * 
 * 120   9/16/97 9:41p Hoffoss
 * Changed Fred code around to stop using Parse_player structure for
 * player information, and use actual ships instead.
 * 
 * 119   9/15/97 2:35p Hoffoss
 * Fixed bug in Fred where a player ship and normal ship both marked
 * clobbered each other's ship type in ship editor dialog.
 * 
 * 118   9/09/97 10:29a Hoffoss
 * Added support for neutral team, and fixed changes made to how team is
 * used in ship structure.
 * 
 * 117   9/09/97 9:27a Hoffoss
 * Removed #Jason Hoffoss# comments from code.  Code is already set up to
 * handle the situation properly.
 * 
 * 116   9/06/97 2:13p Mike
 * Replace support for TEAM_NEUTRAL
 * 
 * 115   9/04/97 5:35p Hoffoss
 * Fixed arrival distance stuff.
 * 
 * 114   9/04/97 5:04p Johnson
 * Fixed bug with arrival target distance checking.
 * 
 * 113   9/04/97 4:31p Hoffoss
 * Fixed bug: Changed ship editor to not touch wing info (arrival or
 * departure cues) to avoid conflicts with wing editor's changes.
 * 
 * 112   8/30/97 9:52p Hoffoss
 * Implemented arrival location, distance, and anchor in Fred.
 * 
 * 111   8/28/97 8:56a Hoffoss
 * Added more checking to sexp error checker, fixed some bugs.
 * 
 * 110   8/25/97 5:56p Hoffoss
 * Added multiple asteroid field support, loading and saving of asteroid
 * fields, and ship score field to Fred.
 * 
 * 109   8/22/97 4:16p Hoffoss
 * added support for arrival and departure info in ship editor using
 * wing's info if editing marked ships in a wing instead of using ship's.
 * 
 * 108   8/21/97 11:37p Hoffoss
 * Fixed bug: when renaming a ship that is a reinforcement, an extra
 * instance of it is added to the reinforcement list.
 * 
 * 107   8/20/97 6:53p Hoffoss
 * Implemented escort flag support in Fred.
 * 
 * 106   8/19/97 2:53p Hoffoss
 * Fixed bug where multiple ships editing doesn't change players.
 * 
 * 105   8/19/97 1:44p Hoffoss
 * Fixed bug with updating too quickly (i.e. via prev and next buttons).
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "fred.h"
#include "freddoc.h"
#include "fredview.h"
#include "mainfrm.h"
#include "3d.h"
#include "physics.h"
#include "editor.h"
#include "ailocal.h"
#include "aigoals.h"
#include "parselo.h"
#include "management.h"
#include "linklist.h"
#include "initialstatus.h"
#include "weaponeditordlg.h"
#include "ship.h"
#include "textviewdlg.h"
#include "player.h"				// used for the max_keyed_target stuff
#include "ignoreordersdlg.h"
#include "missionparse.h"
#include "model.h"
#include "starfield.h"
#include "jumpnode.h"
#include "shipflagsdlg.h"
#include "missionmessage.h"
#include "shipspecialdamage.h"

#define ID_SHIP_MENU 9000

#define NO_PERSONA_INDEX	999

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void numeric_edit_control::setup(int id, CWnd *wnd)
{
	control_id = id;
	dlg = wnd;
}

void numeric_edit_control::init(int n)
{
	value = n;
	unique = 1;
}

void numeric_edit_control::set(int n)
{
	if (n != value){
		unique = 0;
	}
}

void numeric_edit_control::display()
{
	CString str;

	if (unique){
		str.Format("%d", value);
	}

	dlg->GetDlgItem(control_id)->SetWindowText(str);
}

void numeric_edit_control::save(int *n)
{
	CString str;

	if (control_id) {
		dlg->GetDlgItem(control_id)->GetWindowText(str);
		if (!str.IsEmpty()){
			MODIFY(*n, atoi(str));
		}
	}
}

void numeric_edit_control::fix(int n)
{
	if (unique) {
		CString str;
		CWnd *w;

		value = n;
		str.Format("%d", n);
		w = dlg->GetDlgItem(control_id);
		dlg->GetDlgItem(control_id)->SetWindowText(str);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShipEditorDlg dialog

CShipEditorDlg::CShipEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShipEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShipEditorDlg)
	m_ship_name = _T("");
	m_cargo1 = _T("");
	m_ship_class = -1;
	m_team = -1;
	m_arrival_location = -1;
	m_departure_location = -1;
	m_ai_class = -1;
	m_hotkey = -1;
	m_update_arrival = FALSE;
	m_update_departure = FALSE;
	m_arrival_target = -1;
	m_departure_target = -1;
	m_persona = -1;	
	//}}AFX_DATA_INIT

	m_pSEView = NULL;
	initialized = editing = multi_edit = 0;
	select_sexp_node = -1;
	bypass_errors = 0;
}

//	Modeless constructor, MK
CShipEditorDlg::CShipEditorDlg(CView* pView)
{
	m_pSEView = pView;
	initialized = editing = 0;
	select_sexp_node = -1;
}

void CShipEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	int n;
	CString str;

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShipEditorDlg)
	DDX_Control(pDX, IDC_NO_DEPARTURE_WARP, m_no_departure_warp);
	DDX_Control(pDX, IDC_NO_ARRIVAL_WARP, m_no_arrival_warp);
	DDX_Control(pDX, IDC_PLAYER_SHIP, m_player_ship);
	DDX_Control(pDX, IDC_DEPARTURE_DELAY_SPIN, m_departure_delay_spin);
	DDX_Control(pDX, IDC_ARRIVAL_DELAY_SPIN, m_arrival_delay_spin);
	DDX_Control(pDX, IDC_DEPARTURE_TREE, m_departure_tree);
	DDX_Control(pDX, IDC_ARRIVAL_TREE, m_arrival_tree);
	DDX_Text(pDX, IDC_SHIP_NAME, m_ship_name);
	DDX_CBString(pDX, IDC_SHIP_CARGO1, m_cargo1);
	DDX_CBIndex(pDX, IDC_SHIP_CLASS, m_ship_class);
	DDX_CBIndex(pDX, IDC_SHIP_TEAM, m_team);
	DDX_CBIndex(pDX, IDC_ARRIVAL_LOCATION, m_arrival_location);
	DDX_CBIndex(pDX, IDC_DEPARTURE_LOCATION, m_departure_location);
	DDX_CBIndex(pDX, IDC_AI_CLASS, m_ai_class);
	DDX_CBIndex(pDX, IDC_HOTKEY, m_hotkey);
	DDX_Check(pDX, IDC_UPDATE_ARRIVAL, m_update_arrival);
	DDX_Check(pDX, IDC_UPDATE_DEPARTURE, m_update_departure);
	DDX_CBIndex(pDX, IDC_ARRIVAL_TARGET, m_arrival_target);
	DDX_CBIndex(pDX, IDC_DEPARTURE_TARGET, m_departure_target);
	DDX_CBIndex(pDX, IDC_SHIP_PERSONA, m_persona);	
	//}}AFX_DATA_MAP
	DDV_MaxChars(pDX, m_ship_name, NAME_LENGTH - 1);
	DDV_MaxChars(pDX, m_cargo1, NAME_LENGTH - 1);

	if (pDX->m_bSaveAndValidate) {  // get dialog control values
		GetDlgItem(IDC_ARRIVAL_DELAY)->GetWindowText(str);
		n = atoi(str);
		if (n < 0){
			n = 0;
		}

		m_arrival_delay.init(n);

		GetDlgItem(IDC_ARRIVAL_DISTANCE)->GetWindowText(str);
		m_arrival_dist.init(atoi(str));

		GetDlgItem(IDC_DEPARTURE_DELAY)->GetWindowText(str);
		n = atoi(str);
		if (n < 0)
			n = 0;
		m_departure_delay.init(n);

		GetDlgItem(IDC_SCORE)->GetWindowText(str);
		m_score.init(atoi(str));
	}
}

BEGIN_MESSAGE_MAP(CShipEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CShipEditorDlg)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_ARRIVAL_TREE, OnRclickArrivalTree)
	ON_NOTIFY(NM_RCLICK, IDC_DEPARTURE_TREE, OnRclickDepartureTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_ARRIVAL_TREE, OnBeginlabeleditArrivalTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_DEPARTURE_TREE, OnBeginlabeleditDepartureTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_ARRIVAL_TREE, OnEndlabeleditArrivalTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_DEPARTURE_TREE, OnEndlabeleditDepartureTree)
	ON_BN_CLICKED(IDC_GOALS, OnGoals)
	ON_CBN_SELCHANGE(IDC_SHIP_CLASS, OnSelchangeShipClass)
	ON_BN_CLICKED(IDC_INITIAL_STATUS, OnInitialStatus)
	ON_BN_CLICKED(IDC_WEAPONS, OnWeapons)
	ON_BN_CLICKED(IDC_SHIP_RESET, OnShipReset)
	ON_BN_CLICKED(IDC_DELETE_SHIP, OnDeleteShip)
	ON_BN_CLICKED(IDC_SHIP_TBL, OnShipTbl)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_NOTIFY(TVN_SELCHANGED, IDC_ARRIVAL_TREE, OnSelchangedArrivalTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DEPARTURE_TREE, OnSelchangedDepartureTree)
	ON_BN_CLICKED(IDC_HIDE_CUES, OnHideCues)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_CBN_SELCHANGE(IDC_ARRIVAL_LOCATION, OnSelchangeArrivalLocation)
	ON_BN_CLICKED(IDC_PLAYER_SHIP, OnPlayerShip)
	ON_BN_CLICKED(IDC_NO_ARRIVAL_WARP, OnNoArrivalWarp)
	ON_BN_CLICKED(IDC_NO_DEPARTURE_WARP, OnNoDepartureWarp)
	ON_CBN_SELCHANGE(IDC_DEPARTURE_LOCATION, OnSelchangeDepartureLocation)
	ON_CBN_SELCHANGE(IDC_HOTKEY, OnSelchangeHotkey)
	ON_BN_CLICKED(IDC_FLAGS, OnFlags)
	ON_BN_CLICKED(IDC_IGNORE_ORDERS, OnIgnoreOrders)
	ON_WM_INITMENU()
	ON_BN_CLICKED(IDC_SPECIAL_EXP, OnSpecialExp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShipEditorDlg message handlers

BOOL CShipEditorDlg::Create()
{
	int i, index;
	BOOL r;
	CComboBox *ptr;

	r = CDialog::Create(IDD, Fred_main_wnd);

	ptr = (CComboBox *) GetDlgItem(IDC_ARRIVAL_LOCATION);
	ptr->ResetContent();
	for (i=0; i<MAX_ARRIVAL_NAMES; i++){
		ptr->AddString(Arrival_location_names[i]);
	}

	ptr = (CComboBox *) GetDlgItem(IDC_DEPARTURE_LOCATION);
	ptr->ResetContent();
	for (i=0; i<MAX_DEPARTURE_NAMES; i++){
		ptr->AddString(Departure_location_names[i]);
	}

	ptr = (CComboBox *) GetDlgItem(IDC_SHIP_CLASS);
	ptr->ResetContent();
	for (i=0; i<Num_ship_types; i++){
		ptr->AddString(Ship_info[i].name);
	}

	ptr = (CComboBox *) GetDlgItem(IDC_AI_CLASS);
	ptr->ResetContent();
	for (i=0; i<Num_ai_classes; i++){
		ptr->AddString(Ai_class_names[i]);
	}

	// alternate ship name combobox		
	ptr = (CComboBox *)GetDlgItem(IDC_SHIP_ALT);
	ptr->ResetContent();
	ptr->AddString("<none>");
	ptr->SetCurSel(0);

	// deal with the persona dialog
	ptr = (CComboBox *)GetDlgItem(IDC_SHIP_PERSONA);
	ptr->ResetContent();
	index = ptr->AddString("<None>");
	if ( index >= 0 ){
		ptr->SetItemData(index, NO_PERSONA_INDEX);
	}	

	for ( i = 0; i < Num_personas; i++ ) {
		if ( Personas[i].flags & PERSONA_FLAG_WINGMAN ) {
			int index;

			// don't bother putting any vasudan personas on the list -- done automatically by code
//			if ( Personas[i].flags & PERSONA_FLAG_VASUDAN ){
//				continue;
//			}

			CString persona_name = Personas[i].name;
			if ( Personas[i].flags & PERSONA_FLAG_VASUDAN ){
				persona_name += " -Vas";
			}

			index = ptr->AddString(persona_name);
			if ( index >= 0 ){
				ptr->SetItemData(index, i);
			}
		}
	}

	m_score.setup(IDC_SCORE, this);
	m_arrival_dist.setup(IDC_ARRIVAL_DISTANCE, this);
	m_arrival_delay.setup(IDC_ARRIVAL_DELAY, this);
	m_departure_delay.setup(IDC_DEPARTURE_DELAY, this);

	m_hotkey = 0;
	m_arrival_tree.link_modified(&modified);  // provide way to indicate trees are modified in dialog
	m_arrival_tree.setup((CEdit *) GetDlgItem(IDC_HELP_BOX));
	m_departure_tree.link_modified(&modified);
	m_departure_tree.setup();
	m_arrival_delay_spin.SetRange(0, 999);
	m_departure_delay_spin.SetRange(0, 999);
	initialize_data(1);	

	return r;
}

//	This gets called when you click on the "X" button.  Note that OnClose
//	does not destroy the window.  It only hides it.
void CShipEditorDlg::OnClose()
{
	if (verify() && (!bypass_errors)) {
		SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		bypass_errors = 0;
		return;
	}

	if (update_data()) {
		SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		bypass_errors = 0;
		return;
	}

	SetWindowPos(Fred_main_wnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
	Fred_main_wnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

BOOL CShipEditorDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL r;

	r = CDialog::Create(IDD, pParentWnd);
	return r;
}

int CShipEditorDlg::tristate_set(int val, int cur_state)
{
	if (val) {
		if (!cur_state){
			return 2;
		}

	} else {
		if (cur_state){
			return 2;
		}
	}

	return cur_state;
}

// called to initialize the dialog box to reflect what ships we currently have marked.  Any
// time what we have marked changes, this should get called again.
//
// Notes: player_count is the number of player starts marked, when we are in a non-multiplayer
// mission (NMM).  In a multiplayer mission (MM), player_count will always be zero.
// ship_count in NMM is the number of ships (i.e. not player starts) that are marked.  In MM,
// ship_count is the number of ships and player starts.  Total_count is the sum of ship_count
// and player_count in all cases.  The reason player_count isn't used in MM, and ship_count
// is used instead to track player starts is because in MM, player starts can be edited as
// freely as AI ships, and are very likely to be AI ships sometimes.  Thus, treating them like
// AI ships instead of player starts simplifies processing.
//
void CShipEditorDlg::initialize_data(int full_update)
{
	int i, type, ship_count, player_count, total_count, wing = -1, pvalid_count;
	int a_cue, d_cue, cue_init = 0, cargo = 0, base_ship, base_player, pship = -1;
	int no_arrival_warp = 0, no_departure_warp = 0, escort_count, ship_orders, current_orders;
	int pship_count;  // a total count of the player ships not marked
	object *objp;
	CWnd *w = NULL;
	CString str;
	CComboBox *box, *departure_box;
	CSingleLock sync(&CS_update);

	nprintf(("Fred routing", "Ship dialog load\n"));
	if (!GetSafeHwnd() || bypass_all)
		return;

	sync.Lock();  // don't initialize if we are still updating.  Wait until update is done.

	box = (CComboBox *) GetDlgItem(IDC_ARRIVAL_TARGET);
	management_add_ships_to_combo( box, SHIPS_2_COMBO_SPECIAL | SHIPS_2_COMBO_ALL_SHIPS );

	departure_box = (CComboBox *)GetDlgItem(IDC_DEPARTURE_TARGET);
	management_add_ships_to_combo( box, SHIPS_2_COMBO_DOCKING_BAY_ONLY );

	if (The_mission.game_type & MISSION_TYPE_MULTI){
		mission_type = 0;  // multi player mission
	} else {
		mission_type = 1;  // non-multiplayer mission (implies single player mission I guess)
	}

	// figure out what all we are editing.
	ship_count = player_count = escort_count = pship_count = pvalid_count = 0;
	base_ship = base_player = -1;
	enable = p_enable = 1;
	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if ((objp->type == OBJ_SHIP) && (Ships[objp->instance].flags & SF_ESCORT)){
			escort_count++;  // get a total count of escort ships
		}

		if (objp->type == OBJ_START){
			pship_count++;  // count all player starts in mission
		}

		if (objp->flags & OF_MARKED) {
			type = objp->type;
			if ((type == OBJ_START) && !mission_type){  // in multiplayer missions, starts act like ships
				type = OBJ_SHIP;
			}

			i = -1;
			if (type == OBJ_START) {
				player_count++;
				// if player_count is 1, base_player will be the one and only player
				i = base_player = objp->instance;

			} else if (type == OBJ_SHIP) {
				ship_count++;
				// if ship_count is 1, base_ship will be the one and only ship
				i = base_ship = objp->instance;
			}

			if (i >= 0){
				if (Ship_info[Ships[i].ship_info_index].flags & SIF_PLAYER_SHIP){
					pvalid_count++;
				}
			}
		}

		objp = GET_NEXT(objp);
	}

	total_count = ship_count + player_count;  // get total number of objects being edited.
	if (total_count > 1){
		multi_edit = 1;
	} else {
		multi_edit = 0;
	}

	a_cue = d_cue = -1;
	m_arrival_location = -1;
	m_arrival_dist.blank();
	m_arrival_target = -1;
	m_arrival_delay.blank();
	m_departure_location = -1;
	m_departure_target = -1;
	m_departure_delay.blank();

	player_ship = single_ship = -1;
	m_arrival_tree.select_sexp_node = m_departure_tree.select_sexp_node = select_sexp_node;
	select_sexp_node = -1;
	ship_orders = 0;				// assume they are all the same type
	if (ship_count) {
		box = (CComboBox *) GetDlgItem(IDC_SHIP_CARGO1);
		box->ResetContent();
		for (i=0; i<Num_cargo; i++){
			box->AddString(Cargo_names[i]);
		}
		
		if (!multi_edit) {
			Assert((ship_count == 1) && (base_ship >= 0));
			m_ship_name = Ships[base_ship].ship_name;			
		} else {
			m_ship_name = _T("");
		}

		m_update_arrival = m_update_departure = 1;
		base_player = 0;
		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list)) {
			if ((objp->type == OBJ_START) || (objp->type == OBJ_SHIP)) {
				if (objp->flags & OF_MARKED) {
					// do processing for both ships and players
					i = get_ship_from_obj(objp);
					if (base_player >= 0) {
						m_ship_class = Ships[i].ship_info_index;
						m_team = bitmask_2_bitnum(Ships[i].team);
						pship = (objp->type == OBJ_START) ? 1 : 0;
						base_player = -1;

					} else {
						if (Ships[i].ship_info_index != m_ship_class)
							m_ship_class = -1;
						if (bitmask_2_bitnum(Ships[i].team) != m_team)
							m_team = -1;

						pship = tristate_set(Objects[Ships[i].objnum].type == OBJ_START, pship);
					}

					// 'and' in the ship type of this ship to our running bitfield
					current_orders = ship_get_default_orders_accepted( &Ship_info[Ships[i].ship_info_index] );
					if (!ship_orders){
						ship_orders = current_orders;
					} else if (ship_orders != current_orders){
						ship_orders = -1;
					}

					if (Ships[i].flags & SF_ESCORT){
						escort_count--;  // remove marked escorts from count
					}

					if (Objects[Ships[i].objnum].type == OBJ_START){
						pship_count--;  // removed marked starts from count
					}

					// do processing only for ships (plus players if in a multiplayer mission
					if ((objp->type == OBJ_SHIP) || ((objp->type == OBJ_START) && !mission_type)) {
						// process this if ship not in a wing
						if (Ships[i].wingnum < 0) {
							if (!cue_init) {
								cue_init = 1;
								a_cue = Ships[i].arrival_cue;
								d_cue = Ships[i].departure_cue;
								m_arrival_location = Ships[i].arrival_location;
								m_arrival_dist.init(Ships[i].arrival_distance);
								m_arrival_target = Ships[i].arrival_anchor;
								m_arrival_delay.init(Ships[i].arrival_delay);
								m_departure_location = Ships[i].departure_location;
								m_departure_delay.init(Ships[i].departure_delay);
								m_departure_target = Ships[i].departure_anchor;

							} else {
								cue_init++;
								if (Ships[i].arrival_location != m_arrival_location){
									m_arrival_location = -1;
								}

								if (Ships[i].departure_location != m_departure_location){
									m_departure_location = -1;
								}

								m_arrival_dist.set(Ships[i].arrival_distance);
								m_arrival_delay.set(Ships[i].arrival_delay);
								m_departure_delay.set(Ships[i].departure_delay);

								if (Ships[i].arrival_anchor != m_arrival_target){
									m_arrival_target = -1;
								}

								if (!cmp_sexp_chains(a_cue, Ships[i].arrival_cue)) {
									a_cue = -1;
									m_update_arrival = 0;
								}

								if (!cmp_sexp_chains(d_cue, Ships[i].departure_cue)) {
									d_cue = -1;
									m_update_departure = 0;
								}

								if ( Ships[i].departure_anchor != m_departure_target ){
									m_departure_target = -1;
								}
							}
						}

						// process the first ship in group, else process the rest
						if (base_ship >= 0) {
							m_ai_class = Ships[i].weapons.ai_class;
							cargo = Ships[i].cargo1;
							m_cargo1 = Cargo_names[cargo];
							m_hotkey = Ships[i].hotkey + 1;
							m_score.init(Ships[i].score);

							m_persona = Ships[i].persona_index + 1;

							// we use final_death_time member of ship structure for holding the amount of time before a mission
							// to destroy this ship
							wing = Ships[i].wingnum;
							if (wing < 0) {
								GetDlgItem(IDC_WING) -> SetWindowText("None");

							} else {
								GetDlgItem(IDC_WING) -> SetWindowText(Wings[wing].name);
								if (!query_whole_wing_marked(wing))
									m_update_arrival = m_update_departure = 0;
							}

							// set routine local varaiables for ship/object flags
							no_arrival_warp = (Ships[i].flags & SF_NO_ARRIVAL_WARP) ? 1 : 0;
							no_departure_warp = (Ships[i].flags & SF_NO_DEPARTURE_WARP) ? 1 : 0;

							base_ship = -1;
							if (!multi_edit)
								single_ship = i;

						} else {
							if (Ships[i].weapons.ai_class != m_ai_class){
								m_ai_class = -1;
							}

							if (Ships[i].cargo1 != cargo){
								m_cargo1 = _T("");
							}

							m_score.set(Ships[i].score);

							if (Ships[i].hotkey != m_hotkey - 1){
								m_hotkey = -1;
							}

							if ( Ships[i].persona_index != (m_persona-1) ){
								m_persona = -1;
							}
							
							if (Ships[i].wingnum != wing){
								GetDlgItem(IDC_WING) -> SetWindowText("");
							}

							no_arrival_warp = tristate_set(Ships[i].flags & SF_NO_ARRIVAL_WARP, no_arrival_warp);
							no_departure_warp = tristate_set(Ships[i].flags & SF_NO_DEPARTURE_WARP, no_departure_warp);
						}
					}
				}
			}

			objp = GET_NEXT(objp);
		}

		if (multi_edit) {
			m_arrival_tree.clear_tree("");
			m_departure_tree.clear_tree("");
		}

		if (cue_init) {
			m_arrival_tree.load_tree(a_cue);
			m_departure_tree.load_tree(d_cue, "false");

		} else {
			m_arrival_tree.clear_tree();
			m_arrival_tree.DeleteAllItems();
			m_departure_tree.clear_tree();
			m_departure_tree.DeleteAllItems();
		}

		m_player_ship.SetCheck(pship);
		m_no_arrival_warp.SetCheck(no_arrival_warp);
		m_no_departure_warp.SetCheck(no_departure_warp);

		if (!multi_edit) {
			i = m_arrival_tree.select_sexp_node;
			if (i != -1) {
				w = GetDlgItem(IDC_ARRIVAL_TREE);
				m_arrival_tree.hilite_item(i);

			} else {
				i = m_departure_tree.select_sexp_node;
				if (i != -1) {
					w = GetDlgItem(IDC_DEPARTURE_TREE);
					m_departure_tree.hilite_item(i);
				}
			}
		}

	} else {  // no ships selected, 0 or more player ships selected
		if (player_count > 1) {  // multiple player ships selected
			Assert(base_player >= 0);
			m_ship_name = _T("");
			m_player_ship.SetCheck(TRUE);
			objp = GET_FIRST(&obj_used_list);
			while (objp != END_OF_LIST(&obj_used_list)) {
				if ((objp->type == OBJ_START) && (objp->flags & OF_MARKED)) {
					i = objp->instance;
					if (base_player >= 0) {
						m_ship_class = Ships[i].ship_info_index;
						m_team = bitmask_2_bitnum(Ships[i].team);
						base_player = -1;

					} else {
						if (Ships[i].ship_info_index != m_ship_class)
							m_ship_class = -1;
						if (bitmask_2_bitnum(Ships[i].team) != m_team)
							m_team = -1;
					}
				}

				objp = GET_NEXT(objp);
			}

		// only 1 player selected..
		} else if (query_valid_object() && (Objects[cur_object_index].type == OBJ_START)) {
			Assert((player_count == 1) && !multi_edit);
			player_ship = Objects[cur_object_index].instance;
			m_ship_name = Ships[player_ship].ship_name;
			m_ship_class = Ships[player_ship].ship_info_index;
			m_team = bitmask_2_bitnum(Ships[player_ship].team);
			m_player_ship.SetCheck(TRUE);

		} else {  // no ships or players selected..
			m_ship_name = _T("");
			m_ship_class = -1;
			m_team = -1;
			m_persona = -1;
			m_player_ship.SetCheck(FALSE);
		}

		m_ai_class = -1;
		m_cargo1 = _T("");
		m_hotkey = 0;
		m_score.blank();  // cause control to be blank
		m_arrival_location = -1;
		m_departure_location = -1;
		m_arrival_delay.blank();
		m_departure_delay.blank();
		m_arrival_dist.blank();
		m_arrival_target = -1;
		m_departure_target = -1;
		m_arrival_tree.clear_tree();
		m_arrival_tree.DeleteAllItems();
		m_departure_tree.clear_tree();
		m_departure_tree.DeleteAllItems();
		m_no_arrival_warp.SetCheck(0);
		m_no_departure_warp.SetCheck(0);
		enable = p_enable = 0;
		GetDlgItem(IDC_WING)->SetWindowText(_T("None"));
	}

	box = (CComboBox *) GetDlgItem(IDC_ARRIVAL_TARGET);
	// must put the appropriate ships into the list depending on arrival location
	if ( m_arrival_location != ARRIVE_FROM_DOCK_BAY ){
		management_add_ships_to_combo( box, SHIPS_2_COMBO_SPECIAL | SHIPS_2_COMBO_ALL_SHIPS );
	} else {
		management_add_ships_to_combo( box, SHIPS_2_COMBO_DOCKING_BAY_ONLY );
	}

	// set the internal variable appropriatly
	if (m_arrival_target >= SPECIAL_ARRIVAL_ANCHORS_OFFSET){
		m_arrival_target -= SPECIAL_ARRIVAL_ANCHORS_OFFSET;
	} else if (m_arrival_target >= 0) {
		m_arrival_target = box->FindStringExact(-1, Ships[m_arrival_target].ship_name);
	}

	box = (CComboBox *)GetDlgItem(IDC_DEPARTURE_TARGET);
	// must put the appropriate ships into the list depending on departure location
	if ( m_departure_location == DEPART_AT_DOCK_BAY ){
		management_add_ships_to_combo( box, SHIPS_2_COMBO_DOCKING_BAY_ONLY );
	} else {
		box->ResetContent();
	}

	if ( m_departure_target >= 0 ){
		m_departure_target = box->FindStringExact( -1, Ships[m_departure_target].ship_name );
	}

	initialized = 1;
	if (player_count) {
		box = (CComboBox *) GetDlgItem(IDC_SHIP_TEAM);
		if (!mission_type){  // multiplayer mission
			box->EnableWindow(TRUE);
		}

		else {
			box->EnableWindow(FALSE);
			m_team = -1;
		}

		box->ResetContent();
		for (i=0; i<2; i++)  // hard coded: only allow friendly and hostile
			box->AddString(Team_names[i]);
	} else {
		box = (CComboBox *) GetDlgItem(IDC_SHIP_TEAM);
		box->EnableWindow(enable);
		box->ResetContent();
		for (i=0; i<Num_team_names; i++){
			box->AddString(Team_names[i]);
		}
	}	

	m_score.display();
	m_arrival_dist.display();
	m_arrival_delay.display();
	m_departure_delay.display();

	if (full_update)
		UpdateData(FALSE);

	if (!cue_init) {
		GetDlgItem(IDC_ARRIVAL_LOCATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_ARRIVAL_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_ARRIVAL_DISTANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ARRIVAL_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_ARRIVAL_DELAY_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ARRIVAL_TREE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEPARTURE_LOCATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEPARTURE_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEPARTURE_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEPARTURE_DELAY_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEPARTURE_TREE)->EnableWindow(FALSE);
		GetDlgItem(IDC_NO_ARRIVAL_WARP)->EnableWindow(FALSE);
		GetDlgItem(IDC_NO_DEPARTURE_WARP)->EnableWindow(FALSE);

	} else {
		GetDlgItem(IDC_ARRIVAL_LOCATION)->EnableWindow(enable);
		if (m_arrival_location) {
			GetDlgItem(IDC_ARRIVAL_DISTANCE)->EnableWindow(enable);
			GetDlgItem(IDC_ARRIVAL_TARGET)->EnableWindow(enable);
		} else {
			GetDlgItem(IDC_ARRIVAL_DISTANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_ARRIVAL_TARGET)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_DEPARTURE_LOCATION)->EnableWindow(enable);
		if ( m_departure_location ) {
			GetDlgItem(IDC_DEPARTURE_TARGET)->EnableWindow(enable);
		} else {
			GetDlgItem(IDC_DEPARTURE_TARGET)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_ARRIVAL_DELAY)->EnableWindow(enable);
		GetDlgItem(IDC_ARRIVAL_DELAY_SPIN)->EnableWindow(enable);
		GetDlgItem(IDC_ARRIVAL_TREE)->EnableWindow(enable);
		GetDlgItem(IDC_DEPARTURE_LOCATION)->EnableWindow(enable);
		GetDlgItem(IDC_DEPARTURE_DELAY)->EnableWindow(enable);
		GetDlgItem(IDC_DEPARTURE_DELAY_SPIN)->EnableWindow(enable);
		GetDlgItem(IDC_DEPARTURE_TREE)->EnableWindow(enable);
		GetDlgItem(IDC_NO_ARRIVAL_WARP)->EnableWindow(enable);
		GetDlgItem(IDC_NO_DEPARTURE_WARP)->EnableWindow(enable);
	}

	if (total_count) {
		GetDlgItem(IDC_SHIP_NAME)->EnableWindow(!multi_edit);
		GetDlgItem(IDC_SHIP_CLASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHIP_ALT)->EnableWindow(TRUE);
		GetDlgItem(IDC_INITIAL_STATUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_WEAPONS)->EnableWindow(m_ship_class >= 0);
		GetDlgItem(IDC_FLAGS)->EnableWindow(TRUE);

	} else {
		GetDlgItem(IDC_SHIP_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIP_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIP_ALT)->EnableWindow(FALSE);
		GetDlgItem(IDC_INITIAL_STATUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_WEAPONS)->EnableWindow(FALSE);
		GetDlgItem(IDC_FLAGS)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_AI_CLASS)->EnableWindow(enable);
	GetDlgItem(IDC_SHIP_CARGO1)->EnableWindow(enable);
	GetDlgItem(IDC_HOTKEY)->EnableWindow(enable);
	if ((m_ship_class >= 0) && !(Ship_info[m_ship_class].flags & SIF_CARGO) && !(Ship_info[m_ship_class].flags & SIF_NO_SHIP_TYPE))
		GetDlgItem(IDC_GOALS)->EnableWindow(enable);
	else if (multi_edit)
		GetDlgItem(IDC_GOALS)->EnableWindow(enable);
	else
		GetDlgItem(IDC_GOALS)->EnableWindow(FALSE);

	// !pship_count used because if allowed to clear, we would have no player starts
	if (mission_type || !pship_count || (pship_count + total_count > MAX_PLAYERS) || (pvalid_count < total_count))
		m_player_ship.EnableWindow(FALSE);
	else
		m_player_ship.EnableWindow(TRUE);

	GetDlgItem(IDC_DELETE_SHIP)->EnableWindow(enable);
	GetDlgItem(IDC_SHIP_RESET)->EnableWindow(enable);
	GetDlgItem(IDC_SCORE)->EnableWindow(enable);

#ifndef NDEBUG
	GetDlgItem(IDC_SHIP_TBL)->EnableWindow(m_ship_class >= 0);
#else
	GetDlgItem(IDC_SHIP_TBL)->EnableWindow(0);
	GetDlgItem(IDC_SHIP_TBL)->ShowWindow(SW_HIDE);
#endif

	if (cue_init > 1) {  // more than one ship (players don't have cues to edit)
		GetDlgItem(IDC_UPDATE_ARRIVAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_UPDATE_DEPARTURE)->ShowWindow(SW_SHOW);

	} else {
		GetDlgItem(IDC_UPDATE_ARRIVAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_UPDATE_DEPARTURE)->ShowWindow(SW_HIDE);
	}

	if (multi_edit || (total_count > 1)) {
		// we will allow the ignore orders dialog to be multi edit if all selected
		// ships are the same type.  the ship_type (local) variable holds the ship types
		// for all ships.  Determine how may bits set and enable/diable window
		// as appropriate
		if ( /*(m_team == -1) ||*/ (ship_orders == -1) ){
			GetDlgItem(IDC_IGNORE_ORDERS)->EnableWindow(FALSE);
		} else {
			GetDlgItem(IDC_IGNORE_ORDERS)->EnableWindow(TRUE);
		}
	} else
		// always enabled when one ship is selected
		GetDlgItem(IDC_IGNORE_ORDERS)->EnableWindow(enable);

	// always enabled if >= 1 ship selected
	GetDlgItem(IDC_SHIP_PERSONA)->EnableWindow(enable);	

	if (multi_edit){
		SetWindowText("Edit Marked Ships");
	} else if (player_count) {
		SetWindowText("Edit Player Ship");
	} else {
		SetWindowText("Edit Ship");
	}

	// setup alternate name stuff	
	if(player_ship >= 0){				
		ship_alt_name_init(player_ship);
	} else {				
		ship_alt_name_init(single_ship);
	}

	modified = 0;
	if (w){
		w->SetFocus();
	}
}

// update ship structure(s) with dialog data.  The data is first checked for errors.  If
// no errors occur, returns 0.  If an error occurs, returns -1.  If the update is bypassed,
// returns 1.  Bypass is necessary to avoid an infinite loop, and it doesn't actually
// update the data.  Bypass only occurs if bypass mode is active and we still get an error.
// Once the error no longer occurs, bypass mode is cleared and data is updated.
int CShipEditorDlg::update_data(int redraw)
{
	char *str, old_name[255];
	object *ptr;
	int i, z, wing;
	CSingleLock sync(&CS_cur_object_index), sync2(&CS_update);

	nprintf(("Fred routing", "Ship dialog save\n"));
	if (!GetSafeHwnd() || !initialized || bypass_all)
		return 0;

	sync.Lock();  // don't allow cur_object_index to change while we are using it
	sync2.Lock();  // don't allow reinitialization until we are done updating
	UpdateData(TRUE);
	UpdateData(TRUE);
	Wing_editor_dialog.update_data_safe();
	if (multi_edit) {  // editing multiple ships (ships and/or players)
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_START) || (ptr->type == OBJ_SHIP)) && (ptr->flags & OF_MARKED))
				update_ship(get_ship_from_obj(ptr));

			ptr = GET_NEXT(ptr);
		}

	} else if (player_ship >= 0) {  // editing a single player
		update_ship(player_ship);

	} else if (single_ship >= 0) {  // editing a single ship
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)) && (cur_object_index != OBJ_INDEX(ptr))) {
				str = Ships[ptr->instance].ship_name;
				if (!stricmp(m_ship_name, str)) {
					if (bypass_errors)
						return 1;

					bypass_errors = 1;
					z = MessageBox("This ship name is already being used by another ship\n"
						"Press OK to restore old name", "Error", MB_ICONEXCLAMATION | MB_OKCANCEL);

					if (z == IDCANCEL)
						return -1;

					m_ship_name = _T(Ships[single_ship].ship_name);
					UpdateData(FALSE);
				}
			}

			ptr = GET_NEXT(ptr);
		}

		for (i=0; i<MAX_WINGS; i++)
			if (Wings[i].wave_count && !stricmp(Wings[i].name, m_ship_name)) {
				if (bypass_errors)
					return 1;

				bypass_errors = 1;
				z = MessageBox("This ship name is already being used by a wing\n"
					"Press OK to restore old name", "Error", MB_ICONEXCLAMATION | MB_OKCANCEL);

				if (z == IDCANCEL)
					return -1;

				m_ship_name = _T(Ships[single_ship].ship_name);
				UpdateData(FALSE);
			}

		for (i=0; i<MAX_WAYPOINT_LISTS; i++)
			if (Waypoint_lists[i].count && !stricmp(Waypoint_lists[i].name, m_ship_name)) {
				if (bypass_errors)
					return 0;

				bypass_errors = 1;
				z = MessageBox("This ship name is already being used by a waypoint path\n"
					"Press OK to restore old name", "Error", MB_ICONEXCLAMATION | MB_OKCANCEL);

				if (z == IDCANCEL)
					return -1;

				m_ship_name = _T(Ships[single_ship].ship_name);
				UpdateData(FALSE);
			}

		for (i=0; i<Num_jump_nodes; i++)
			if (!stricmp(Jump_nodes[i].name, m_ship_name)) {
				if (bypass_errors)
					return 1;

				bypass_errors = 1;
				z = MessageBox("This ship name is already being used by a jump node\n"
					"Press OK to restore old name", "Error", MB_ICONEXCLAMATION | MB_OKCANCEL);

				if (z == IDCANCEL)
					return -1;

				m_ship_name = _T(Ships[single_ship].ship_name);
				UpdateData(FALSE);
			}

		wing = Ships[single_ship].wingnum;
		if (wing >= 0) {
			Assert((wing < MAX_WINGS) && Wings[wing].wave_count);
			for (i=0; i<Wings[wing].wave_count; i++)
				if (wing_objects[wing][i] == Ships[single_ship].objnum)
					break;

			Assert(i < Wings[wing].wave_count);
			sprintf(old_name, "%s %d", Wings[wing].name, i + 1);
			if (stricmp(old_name, m_ship_name)) {
				if (bypass_errors)
					return 0;

				if (MessageBox("This ship is part of a wing, and it's name cannot be changed",
					NULL, MB_OKCANCEL) == IDCANCEL)
						return -1;

				m_ship_name = _T(old_name);
				UpdateData(FALSE);
			}
		}

		z = update_ship(single_ship);
		if (z)
			return z;

		strcpy(old_name, Ships[single_ship].ship_name);
		string_copy(Ships[single_ship].ship_name, m_ship_name, NAME_LENGTH, 1);
		str = Ships[single_ship].ship_name;
		if (stricmp(old_name, str)) {
			update_sexp_references(old_name, str);
			ai_update_goal_references(REF_TYPE_SHIP, old_name, str);
			for (i=0; i<Num_reinforcements; i++)
				if (!stricmp(old_name, Reinforcements[i].name)) {
					Assert(strlen(str) < NAME_LENGTH);
					strcpy(Reinforcements[i].name, str);
				}

			Update_window = 1;
		}
	}

	if (Player_start_shipnum < 0 || Objects[Ships[Player_start_shipnum].objnum].type != OBJ_START) {  // need a new single player start.
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (ptr->type == OBJ_START) {
				Player_start_shipnum = ptr->instance;
				break;
			}

			ptr = GET_NEXT(ptr);
		}
	}

	if (modified)
		set_modified();

	Wing_editor_dialog.initialize_data_safe(1);
	bypass_errors = 0;
	modified = 0;

	if (redraw)
		update_map_window();

	return 0;
}

int CShipEditorDlg::update_ship(int ship)
{
	int z, d;
	CString str;
	CComboBox *box;
	int persona;

	// THIS DIALOG IS THE SOME OF THE WORST CODE I HAVE EVER SEEN IN MY ENTIRE LIFE. 
	// IT TOOK A RIDICULOUSLY LONG AMOUNT OF TIME TO ADD 2 FUNCTIONS. OMG
	ship_alt_name_close(ship);

	if ((Ships[ship].ship_info_index != m_ship_class) && (m_ship_class != -1)) {
		change_ship_type(ship, m_ship_class);
		set_modified();
	}

	if (m_team != -1)
		MODIFY(Ships[ship].team, 1 << m_team);

	if (Objects[Ships[ship].objnum].type != OBJ_SHIP){
		if (mission_type || (Objects[Ships[ship].objnum].type != OBJ_START)){
			return 0;
		}
	}

	if (m_ai_class != -1){
		MODIFY(Ships[ship].weapons.ai_class, m_ai_class);
	}
	if (strlen(m_cargo1)) {
		z = string_lookup(m_cargo1, Cargo_names, Num_cargo);
		if (z == -1) {
			Assert(Num_cargo < MAX_CARGO);
			z = Num_cargo++;
			strcpy(Cargo_names[z], m_cargo1);
		}

		MODIFY(Ships[ship].cargo1, (char)z);
	}

	m_score.save(&Ships[ship].score);
	if (m_arrival_location != -1)
		MODIFY(Ships[ship].arrival_location, m_arrival_location);
	if (m_departure_location != -1)
		MODIFY(Ships[ship].departure_location, m_departure_location);

	// do the persona update
	// m_persona holds the index into the list.  Get the item data associated with this index and then
	// assign to the ship taking care that we check for the NO_PERSONA_INDEX id
	box = (CComboBox *)GetDlgItem(IDC_SHIP_PERSONA);
	persona = box->GetItemData(m_persona);
	if ( persona == NO_PERSONA_INDEX )
		persona = -1;

	MODIFY(Ships[ship].persona_index, persona);

	if (Ships[ship].wingnum < 0) {
		if (!multi_edit || m_update_arrival) {  // should we update the arrival cue?
			if (Ships[ship].arrival_cue >= 0)
				free_sexp2(Ships[ship].arrival_cue);

			Ships[ship].arrival_cue = m_arrival_tree.save_tree();
		}

		if (!multi_edit || m_update_departure) {
			if (Ships[ship].departure_cue >= 0)
				free_sexp2(Ships[ship].departure_cue);

			Ships[ship].departure_cue = m_departure_tree.save_tree();
		}

		m_arrival_dist.save(&Ships[ship].arrival_distance);
		m_arrival_delay.save(&Ships[ship].arrival_delay);
		m_departure_delay.save(&Ships[ship].departure_delay);
		if (m_arrival_target >= 0) {
			z = ((CComboBox *) GetDlgItem(IDC_ARRIVAL_TARGET)) -> GetItemData(m_arrival_target);
			MODIFY(Ships[ship].arrival_anchor, z);

			// if the arrival is not hyperspace or docking bay -- force arrival distance to be
			// greater than 2*radius of target.
			if (((m_arrival_location != ARRIVE_FROM_DOCK_BAY) && (m_arrival_location != ARRIVE_AT_LOCATION)) && (z >= 0) && (z < SPECIAL_ARRIVAL_ANCHORS_OFFSET)) {
			d = int(min(500, 2.0f * Objects[Ships[ship].objnum].radius));
				if ((Ships[ship].arrival_distance < d) && (Ships[ship].arrival_distance > -d)) {
					str.Format("Ship must arrive at least %d meters away from target.\n"
						"Value has been reset to this.  Use with caution!\r\n"
						"Reccomended distance is %d meters.\r\n", d, (int)(2.0f * Objects[Ships[ship].objnum].radius) );

					MessageBox(str);
					if (Ships[ship].arrival_distance < 0)
						Ships[ship].arrival_distance = -d;
					else
						Ships[ship].arrival_distance = d;

					m_arrival_dist.fix(Ships[ship].arrival_distance);
				}
			}
		}
		z = ((CComboBox *)GetDlgItem(IDC_DEPARTURE_TARGET))->GetItemData(m_departure_target);
		MODIFY(Ships[ship].departure_anchor, z );
	}

	if (m_hotkey != -1)
		MODIFY(Ships[ship].hotkey, m_hotkey - 1);

	switch( m_no_arrival_warp.GetCheck() ) {
		case 0:
			if (Ships[ship].flags & SF_NO_ARRIVAL_WARP)
				set_modified();

			Ships[ship].flags &= ~SF_NO_ARRIVAL_WARP;
			break;

		case 1:
			if (!(Ships[ship].flags & SF_NO_ARRIVAL_WARP))
				set_modified();

			Ships[ship].flags |= SF_NO_ARRIVAL_WARP;
			break;
	}

	switch( m_no_departure_warp.GetCheck() ) {
		case 0:
			if (Ships[ship].flags & SF_NO_DEPARTURE_WARP)
				set_modified();

			Ships[ship].flags &= ~SF_NO_DEPARTURE_WARP;
			break;

		case 1:
			if (!(Ships[ship].flags & SF_NO_DEPARTURE_WARP))
				set_modified();

			Ships[ship].flags |= SF_NO_DEPARTURE_WARP;
			break;
	}

	switch (m_player_ship.GetCheck()) {
		case 1:
			if (Objects[Ships[ship].objnum].type != OBJ_START) {
				Player_starts++;
				set_modified();
			}

			Objects[Ships[ship].objnum].type = OBJ_START;
			break;

		case 0:
			if (Objects[Ships[ship].objnum].type == OBJ_START) {
				Player_starts--;
				set_modified();
			}

			Objects[Ships[ship].objnum].type = OBJ_SHIP;
			break;
	}	

	Update_ship = 1;
	return 0;
}

void CShipEditorDlg::OnOK()
{
	HWND h;
	CWnd *w;

	w = GetFocus();
	if (w) {
		h = w->m_hWnd;
		GetDlgItem(IDC_ARRIVAL_TREE)->SetFocus();
		::SetFocus(h);
	}
}

void CShipEditorDlg::OnInitMenu(CMenu *pMenu)
{
	CMenu *m;

	m = pMenu->GetSubMenu(0);
	clear_menu(m);
	generate_ship_popup_menu(m, ID_SHIP_MENU, MF_ENABLED, SHIP_FILTER_PLAYERS);
	if (cur_ship != -1)
		m->CheckMenuItem(ID_SHIP_MENU + cur_ship, MF_BYCOMMAND | MF_CHECKED);

	CWnd::OnInitMenu(pMenu);
}

BOOL CShipEditorDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int id, ship;

	id = LOWORD(wParam);
	if (id >= ID_SHIP_MENU && id < ID_SHIP_MENU + MAX_SHIPS) {
		if (!update_data()) {
			ship = id - ID_SHIP_MENU;
			unmark_all();
			set_cur_object_index(Ships[ship].objnum);
			return 1;
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CShipEditorDlg::OnRclickArrivalTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_arrival_tree.right_clicked();	
	*pResult = 0;
}

void CShipEditorDlg::OnRclickDepartureTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_departure_tree.right_clicked();
	*pResult = 0;
}

void CShipEditorDlg::OnBeginlabeleditArrivalTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if (m_arrival_tree.edit_label(pTVDispInfo->item.hItem) == 1)	{
		*pResult = 0;
		modified = editing = 1;

	} else
		*pResult = 1;
}

void CShipEditorDlg::OnBeginlabeleditDepartureTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if (m_departure_tree.edit_label(pTVDispInfo->item.hItem) == 1)	{
		*pResult = 0;
		modified = editing = 1;

	} else
		*pResult = 1;
}

void CShipEditorDlg::OnEndlabeleditArrivalTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	*pResult = m_arrival_tree.end_label_edit(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	editing = 0;
}

void CShipEditorDlg::OnEndlabeleditDepartureTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	*pResult = m_departure_tree.end_label_edit(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	editing = 0;
}

int CShipEditorDlg::verify()
{
	nprintf(("Fred routing", "Ship dialog verify\n"));
	if (!GetSafeHwnd() || !modified)
		return 0;

	if (bypass_errors)
		return 1;

	return 0;
}

void CShipEditorDlg::OnGoals()
{
	ShipGoalsDlg dlg_goals;

	Assert(query_valid_object());
//	if (multi_edit)
//		dlg_goals.initialize_multi();
//
//	else {
//		Assert(single_ship != -1);
//		dlg_goals.self_ship = single_ship;
//		dlg_goals.initialize(Ai_info[Ships[single_ship].ai_index].goals);
//	}

	if (!multi_edit) {
		Assert(single_ship != -1);
		dlg_goals.self_ship = single_ship;
	}

	dlg_goals.DoModal();
	if (!multi_edit && !query_initial_orders_empty(Ai_info[Ships[single_ship].ai_index].goals))
		if ((Ships[single_ship].wingnum >= 0) && (query_initial_orders_conflict(Ships[single_ship].wingnum)))
			MessageBox("This ship's wing also has initial orders", "Possible conflict");
}

void CShipEditorDlg::OnSelchangeShipClass() 
{
	object *ptr;

	UpdateData(TRUE);
	UpdateData(TRUE);
	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if (((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)) && (ptr->flags & OF_MARKED))
			if (Ships[ptr->instance].ship_info_index != m_ship_class) {
				change_ship_type(ptr->instance, m_ship_class);
				set_modified();
			}

		ptr = GET_NEXT(ptr);
	}

	update_map_window();
}

void CShipEditorDlg::OnInitialStatus() 
{
	initial_status dlg;

	dlg.m_multi_edit = multi_edit;
	dlg.DoModal();
}

void CShipEditorDlg::OnWeapons() 
{
	int i, ship = -1;
	WeaponEditorDlg dlg;
	object *objp;
	CComboBox *box;

	dlg.m_multi_edit = multi_edit;
	dlg.DoModal();

	if (multi_edit) {
		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list)) {
			if (objp->flags & OF_MARKED)
				if ((objp->type == OBJ_SHIP) || (objp->type == OBJ_START)) {
					i = objp->instance;
					if (ship) {
						if (Ships[i].weapons.ai_class != Ships[ship].weapons.ai_class)
							m_ai_class = -1;

					} else {
						ship = i;
						m_ai_class = Ships[i].weapons.ai_class;
					}
				}

			objp = GET_NEXT(objp);
		}

	} else {
		ship = single_ship;
		if (ship < 0)
			ship = player_ship;

		Assert(ship >= 0);
		m_ai_class = Ships[ship].weapons.ai_class;
	}

	box = (CComboBox *) GetDlgItem(IDC_AI_CLASS);
	box->SetCurSel(m_ai_class);
}

void CShipEditorDlg::OnShipReset() 
{
	int i, j, index, ship;
	object *objp;
	ship_info *sip;
	ship_subsys *ptr;
	ship_weapon *wp;
	model_subsystem *sp;

	m_cargo1 = "Nothing";
	m_ai_class = AI_DEFAULT_CLASS;
	if (m_ship_class) {
		if (Ship_info[m_ship_class].species == SPECIES_SHIVAN)
			m_team = TEAM_HOSTILE;
		else
			m_team = TEAM_FRIENDLY;
	}

	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if (((objp->type == OBJ_SHIP) || ((objp->type == OBJ_START) && !mission_type)) && (objp->flags & OF_MARKED)) {
			ship = objp->instance;

			// reset ship goals
			for (i=0; i<MAX_AI_GOALS; i++){
				Ai_info[Ships[ship].ai_index].goals[i].ai_mode = AI_GOAL_NONE;
			}

			objp->phys_info.speed = 0.0f;
			objp->shields[0] = 100.0f;
			objp->hull_strength = 100.0f;

			sip = &Ship_info[Ships[ship].ship_info_index];
			for (i=0; i<sip->num_primary_banks; i++)
				Ships[ship].weapons.primary_bank_weapons[i] = sip->primary_bank_weapons[i];

			for (i=0; i<sip->num_secondary_banks; i++) {
				Ships[ship].weapons.secondary_bank_weapons[i] = sip->secondary_bank_weapons[i];
				Ships[ship].weapons.secondary_bank_capacity[i] = sip->secondary_bank_ammo_capacity[i];
			}

			index = 0;
			ptr = GET_FIRST(&Ships[ship].subsys_list);
			while (ptr != END_OF_LIST(&Ships[ship].subsys_list)) {
				ptr->current_hits = 0.0f;
				if (ptr->system_info->type == SUBSYSTEM_TURRET) {
					wp = &ptr->weapons;
					sp = &Ship_info[Ships[ship].ship_info_index].subsystems[index];

					j = 0;
					for (i=0; i<MAX_PRIMARY_BANKS; i++){
						if (sp->primary_banks[i] != -1){
							wp->primary_bank_weapons[j++] = sp->primary_banks[i];
						}
					}

					wp->num_primary_banks = j;
					j = 0;
					for (i=0; i<MAX_SECONDARY_BANKS; i++){
						if (sp->secondary_banks[i] != -1) {
							wp->secondary_bank_weapons[j] = sp->secondary_banks[i];
							wp->secondary_bank_capacity[j++] = sp->secondary_bank_capacity[i];
						}
					}

					wp->num_secondary_banks = j;
					for (i=0; i<MAX_SECONDARY_BANKS; i++){
						wp->secondary_bank_ammo[i] = 100;
					}
				}

				index++;
				ptr = GET_NEXT(ptr);
			}
		}

		objp = GET_NEXT(objp);
	}

	UpdateData(FALSE);
	if (multi_edit){
		MessageBox("Ships reset to ship class defaults");
	} else {
		MessageBox("Ship reset to ship class defaults");
	}
}

void CShipEditorDlg::OnDeleteShip() 
{
	delete_marked();
	unmark_all();
}

void CShipEditorDlg::OnShipTbl()
{
	text_view_dlg dlg;

	dlg.set(m_ship_class);
	dlg.DoModal();
}

int CShipEditorDlg::make_ship_list(int *arr)
{
	int n = 0;
	object *ptr;

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if ((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)){
			arr[n++] = OBJ_INDEX(ptr);
		}

		ptr = GET_NEXT(ptr);
	}

	return n;
}

void CShipEditorDlg::OnPrev() 
{
	int i, n, arr[MAX_SHIPS];

	if (!update_data()) {
		n = make_ship_list(arr);
		if (!n){
			return;
		}

		if (cur_ship < 0){
			i = n - 1;
		}

		else {
			for (i=0; i<n; i++){
				if (Ships[cur_ship].objnum == arr[i]){
					break;
				}
			}

			Assert(i < n);
			i--;
			if (i < 0){
				i = n - 1;
			}
		}

		unmark_all();
		set_cur_object_index(arr[i]);
		Ship_editor_dialog.initialize_data(1);
		Update_ship = 0;
	}

	return;
}

void CShipEditorDlg::OnNext() 
{
	int i, n, arr[MAX_SHIPS];

	if (!update_data()) {
		n = make_ship_list(arr);
		if (!n)
			return;

		if (cur_ship < 0)
			i = 0;

		else {
			for (i=0; i<n; i++)
				if (Ships[cur_ship].objnum == arr[i])
					break;

			Assert(i < n);
			i++;
			if (i == n)
				i = 0;
		}

		unmark_all();
		set_cur_object_index(arr[i]);
		Ship_editor_dialog.initialize_data(1);
		Update_ship = 0;
	}

	return;
}

void CShipEditorDlg::OnSelchangedArrivalTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM h;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	h = pNMTreeView->itemNew.hItem;
	if (h){
		m_arrival_tree.update_help(h);
	}

	*pResult = 0;
}

void CShipEditorDlg::OnSelchangedDepartureTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM h;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	h = pNMTreeView->itemNew.hItem;
	if (h){
		m_departure_tree.update_help(h);
	}

	*pResult = 0;
}

void CShipEditorDlg::calc_cue_height()
{
	CRect cue;

	GetDlgItem(IDC_CUE_FRAME)->GetWindowRect(cue);
	cue_height = cue.bottom - cue.top + 10;
	if (Show_sexp_help){
		cue_height += SEXP_HELP_BOX_SIZE;
	}

	if (Hide_ship_cues) {
		((CButton *) GetDlgItem(IDC_HIDE_CUES)) -> SetCheck(1);
		OnHideCues();
	}
}

void CShipEditorDlg::show_hide_sexp_help()
{
	CRect rect;

	if (Show_sexp_help){
		cue_height += SEXP_HELP_BOX_SIZE;
	} else {
		cue_height -= SEXP_HELP_BOX_SIZE;
	}

	if (((CButton *) GetDlgItem(IDC_HIDE_CUES)) -> GetCheck()){
		return;
	}

	GetWindowRect(rect);

	if (Show_sexp_help){
		rect.bottom += SEXP_HELP_BOX_SIZE;
	} else {
		rect.bottom -= SEXP_HELP_BOX_SIZE;
	}

	MoveWindow(rect);
}

void CShipEditorDlg::OnHideCues() 
{
	CRect rect;

	GetWindowRect(rect);
	if (((CButton *) GetDlgItem(IDC_HIDE_CUES)) -> GetCheck()) {
		rect.bottom -= cue_height;
		Hide_ship_cues = 1;

	} else {
		rect.bottom += cue_height;
		Hide_ship_cues = 0;
	}

	MoveWindow(rect);
}

void CShipEditorDlg::OnSelchangeArrivalLocation() 
{
	CComboBox *box;

	UpdateData();
	box = (CComboBox *)GetDlgItem( IDC_ARRIVAL_TARGET );
	if (m_arrival_location) {
		GetDlgItem(IDC_ARRIVAL_DISTANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_ARRIVAL_TARGET)->EnableWindow(TRUE);
		if (m_arrival_target < 0) {
			m_arrival_target = 0;
		}

		// determine which items we should put into the arrival target combo box
		if ( m_arrival_location == ARRIVE_FROM_DOCK_BAY ) {
			management_add_ships_to_combo( box, SHIPS_2_COMBO_DOCKING_BAY_ONLY );
		} else {
			management_add_ships_to_combo( box, SHIPS_2_COMBO_SPECIAL | SHIPS_2_COMBO_ALL_SHIPS );
		}

	} else {
		m_arrival_target = -1;
		GetDlgItem(IDC_ARRIVAL_DISTANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ARRIVAL_TARGET)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CShipEditorDlg::OnSelchangeDepartureLocation() 
{
	CComboBox *box;

	UpdateData();
	box = (CComboBox *)GetDlgItem(IDC_DEPARTURE_TARGET);
	if ( m_departure_location ) {
		box->EnableWindow(TRUE);
		if ( m_departure_target < 0 ) {
			m_departure_target = 0;
		}

		// we need to build up the list box content based on the departure type.  When
		// from a docking bay, only show ships in the list which have them.  Show all ships otherwise
		if ( m_departure_location == DEPART_AT_DOCK_BAY ) {
			management_add_ships_to_combo( box, SHIPS_2_COMBO_DOCKING_BAY_ONLY );
		} else {
			// I think that this section is currently illegal
			Int3();
		}

	} else {
		m_departure_target = -1;
		box->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}


void CShipEditorDlg::OnPlayerShip() 
{
	if (m_player_ship.GetCheck() == 1)
		m_player_ship.SetCheck(0);
	else
		m_player_ship.SetCheck(1);

	update_map_window();
}

void CShipEditorDlg::OnNoArrivalWarp() 
{
	if (m_no_arrival_warp.GetCheck() == 1)
		m_no_arrival_warp.SetCheck(0);
	else
		m_no_arrival_warp.SetCheck(1);
}

void CShipEditorDlg::OnNoDepartureWarp() 
{
	if (m_no_departure_warp.GetCheck() == 1)
		m_no_departure_warp.SetCheck(0);
	else
		m_no_departure_warp.SetCheck(1);
}


// function to possibly warn user when he selects a hotkey which might be used for
// a player wing.
void CShipEditorDlg::OnSelchangeHotkey() 
{
	int set_num;
	char buf[256];

	UpdateData(TRUE);
	set_num = m_hotkey-1;			// use -1 since values associated with hotkey sets are 1 index based

	// the first three sets are generally reserved for player starting wings.
	if ( set_num < MAX_STARTING_WINGS ) {
		sprintf( buf, "This hotkey set should probably be reserved\nfor wing %s", Starting_wing_names[set_num] );
		MessageBox(buf, NULL, MB_OK);
	}
}

void CShipEditorDlg::OnFlags() 
{
	ship_flags_dlg dlg;

	dlg.setup(p_enable);
	dlg.DoModal();
}

void CShipEditorDlg::OnIgnoreOrders() 
{
	// TODO: Add your control notification handler code here
	ignore_orders_dlg player_order_dlg;

	Assert(query_valid_object());

	if (!multi_edit) {
		if ( single_ship != -1 ){
			player_order_dlg.m_ship = single_ship;
		} else {
			player_order_dlg.m_ship = player_ship;
		}
	} else {
		player_order_dlg.m_ship = -1;
	}

	player_order_dlg.DoModal();
}

void CShipEditorDlg::OnSpecialExp() 
{
	// TODO: Add your control notification handler code here
	ShipSpecialDamage dlg;
	dlg.DoModal();
}

// alternate ship name stuff
void CShipEditorDlg::ship_alt_name_init(int base_ship)
{
	int idx;
	CComboBox *ptr = (CComboBox*)GetDlgItem(IDC_SHIP_ALT);
	if(ptr == NULL){
		Int3();
		return;
	}

	// multi-edit. bah	
	if(multi_edit){		
		GetDlgItem(IDC_SHIP_ALT)->EnableWindow(FALSE);
		return;
	} 
	GetDlgItem(IDC_SHIP_ALT)->EnableWindow(TRUE);	

	// reset the combobox and add all relevant strings
	ptr->ResetContent();
	ptr->AddString("<none>");
	for(idx=0; idx<Mission_alt_type_count; idx++){
		ptr->AddString(Mission_alt_types[idx]);
	}

	// "none"
	if(base_ship < 0){
		ptr->SetCurSel(0);
	}

	// otherwise look his stuff up
	if(strlen(Fred_alt_names[base_ship])){
		ptr->SelectString(0, Fred_alt_names[base_ship]);
	} else {
		ptr->SetCurSel(0);
	}
}

void CShipEditorDlg::ship_alt_name_close(int base_ship)
{
	CString cstr;
	char str[NAME_LENGTH+2] = "";
	char *p;	
	CComboBox *ptr = (CComboBox*)GetDlgItem(IDC_SHIP_ALT);

	if(multi_edit){
		return;
	}
	
	if(ptr == NULL){
		Int3();
		return;
	}

	// see if we have something besides "none" selected
	ptr->GetWindowText(cstr);
	if(cstr == CString("<none>")){
		// zero the entry
		strcpy(Fred_alt_names[base_ship], "");

		return;
	}	
	p = cstr.GetBuffer(0);
	if(p == NULL){
		return;
	}
	strcpy(str, p);

	// otherwise see if it already exists
	if(mission_parse_lookup_alt(str) >= 0){
		strcpy(Fred_alt_names[base_ship], str);

		return;
	}

	// otherwise try and add it
	if(mission_parse_add_alt(str) >= 0){
		strcpy(Fred_alt_names[base_ship], str);

		return;
	}

	// bad - couldn't add
	strcpy(Fred_alt_names[base_ship], "");
	MessageBox("Couldn't add new alternate type name. Already using too many!");
}
