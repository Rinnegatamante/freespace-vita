/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

// ShipFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fred.h"
#include "shipflagsdlg.h"
#include "linklist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ship_flags_dlg dialog

ship_flags_dlg::ship_flags_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(ship_flags_dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ship_flags_dlg)
	//}}AFX_DATA_INIT
}

void ship_flags_dlg::DoDataExchange(CDataExchange* pDX)
{
	int n;
	CString str;

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ship_flags_dlg)
	DDX_Control(pDX, IDC_REDALERTCARRY, m_red_alert_carry);
	DDX_Control(pDX, IDC_SCANNABLE, m_scannable);
	DDX_Control(pDX, IDC_REINFORCEMENT, m_reinforcement);
	DDX_Control(pDX, IDC_PROTECT_SHIP, m_protect_ship);
	DDX_Control(pDX, IDC_BEAM_PROTECT_SHIP, m_beam_protect_ship);
	DDX_Control(pDX, IDC_NO_DYNAMIC, m_no_dynamic);
	DDX_Control(pDX, IDC_NO_ARRIVAL_MUSIC, m_no_arrival_music);
	DDX_Control(pDX, IDC_KAMIKAZE, m_kamikaze);
	DDX_Control(pDX, IDC_INVULNERABLE, m_invulnerable);
	DDX_Control(pDX, IDC_IGNORE_COUNT, m_ignore_count);
	DDX_Control(pDX, IDC_HIDDEN_FROM_SENSORS, m_hidden);
	DDX_Control(pDX, IDC_ESCORT, m_escort);
	DDX_Control(pDX, IDC_DESTROY_CHECK, m_destroy);
	DDX_Control(pDX, IDC_CARGO_KNOWN, m_cargo_known);
	DDX_Control(pDX, IDC_DESTROY_SPIN, m_destroy_spin);	
	DDX_Control(pDX, IDC_SPECIAL_WARP, m_special_warp);	
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {  // get dialog control values
		GetDlgItem(IDC_DESTROY_VALUE)->GetWindowText(str);
		n = atoi(str);
		if (n < 0)
			n = 0;

		m_destroy_value.init(n);

		GetDlgItem(IDC_KDAMAGE)->GetWindowText(str);
		m_kdamage.init(atoi(str));

		//  get escort priority
		GetDlgItem(IDC_ESCORT_PRIORITY)->GetWindowText(str);
		m_escort_value.init(atoi(str));

		// get respawn priority
		if(The_mission.game_type & MISSION_TYPE_MULTI){
			GetDlgItem(IDC_RESPAWN_PRIORITY)->GetWindowText(str);
			m_respawn_priority.init(atoi(str));
		}
	}
}

BEGIN_MESSAGE_MAP(ship_flags_dlg, CDialog)
	//{{AFX_MSG_MAP(ship_flags_dlg)
	ON_BN_CLICKED(IDC_CARGO_KNOWN, OnCargoKnown)
	ON_BN_CLICKED(IDC_DESTROY_CHECK, OnDestroyCheck)
	ON_BN_CLICKED(IDC_ESCORT, OnEscort)
	ON_BN_CLICKED(IDC_HIDDEN_FROM_SENSORS, OnHiddenFromSensors)
	ON_BN_CLICKED(IDC_IGNORE_COUNT, OnIgnoreCount)
	ON_BN_CLICKED(IDC_INVULNERABLE, OnInvulnerable)
	ON_BN_CLICKED(IDC_KAMIKAZE, OnKamikaze)
	ON_BN_CLICKED(IDC_NO_ARRIVAL_MUSIC, OnNoArrivalMusic)
	ON_BN_CLICKED(IDC_NO_DYNAMIC, OnNoDynamic)
	ON_BN_CLICKED(IDC_PROTECT_SHIP, OnProtectShip)
	ON_BN_CLICKED(IDC_BEAM_PROTECT_SHIP, OnBeamProtectShip)
	ON_BN_CLICKED(IDC_REINFORCEMENT, OnReinforcement)
	ON_BN_CLICKED(IDC_SCANNABLE, OnScannable)
	ON_BN_CLICKED(IDC_REDALERTCARRY, OnRedalertcarry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ship_flags_dlg message handlers

void ship_flags_dlg::setup(int n)
{
	p_enable = n;
}

BOOL ship_flags_dlg::OnInitDialog() 
{
	int i, j, first;
	int protect_ship = 0, beam_protect_ship = 0, ignore_count = 0, reinforcement = 0, cargo_known = 0, destroy_before_mission = 0;
	int no_arrival_music = 0, escort = 0, invulnerable = 0, hidden_from_sensors = 0;
	int scannable = 0, kamikaze = 0, no_dynamic = 0, red_alert_carry = 0, special_warp = 0;
	object *objp;
	bool ship_in_wing = false;

	first = 1;
	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if ((objp->type == OBJ_START) || (objp->type == OBJ_SHIP)) {
			if (objp->flags & OF_MARKED) {
				i = objp->instance;
				if (first) {
					first = 0;
					scannable = (Ships[i].flags & SF_SCANNABLE) ? 1 : 0;
					red_alert_carry = (Ships[i].flags & SF_RED_ALERT_STORE_STATUS) ? 1 : 0;
					special_warp = (Objects[Ships[i].objnum].flags & OF_SPECIAL_WARP) ? 1 : 0;
					protect_ship = (Objects[Ships[i].objnum].flags & OF_PROTECTED) ? 1 : 0;
					beam_protect_ship = (Objects[Ships[i].objnum].flags & OF_BEAM_PROTECTED) ? 1 : 0;
					invulnerable = (Ships[i].flags & SF_INVULNERABLE) ? 1 : 0;
					hidden_from_sensors = (Ships[i].flags & SF_HIDDEN_FROM_SENSORS) ? 1 : 0;
					ignore_count = (Ships[i].flags & SF_IGNORE_COUNT) ? 1 : 0;
					no_arrival_music = (Ships[i].flags & SF_NO_ARRIVAL_MUSIC) ? 1 : 0;
					cargo_known = (Ships[i].flags & SF_CARGO_REVEALED) ? 1 : 0;
					no_dynamic = (Ai_info[Ships[i].ai_index].ai_flags & AIF_NO_DYNAMIC) ? 1 : 0;

					destroy_before_mission = (Ships[i].flags & SF_KILL_BEFORE_MISSION) ? 1 : 0;
					m_destroy_value.init(Ships[i].final_death_time);

					kamikaze = (Ai_info[Ships[i].ai_index].ai_flags & AIF_KAMIKAZE) ? 1 : 0;
					m_kdamage.init( (int) Ai_info[Ships[i].ai_index].kamikaze_damage );

					escort = (Ships[i].flags & SF_ESCORT) ? 1 : 0;
					m_escort_value.init(Ships[i].escort_priority);
					
					if(The_mission.game_type & MISSION_TYPE_MULTI){
						m_respawn_priority.init(Ships[i].respawn_priority);
					}

					for (j=0; j<Num_reinforcements; j++){
						if (!stricmp(Reinforcements[j].name, Ships[i].ship_name)){
							break;
						}
					}

					reinforcement = (j < Num_reinforcements) ? 1 : 0;

					// check if ship in wing
					ship_in_wing = (Ships[i].wingnum != -1);;
				} else {

					scannable = tristate_set( Ships[i].flags & SF_SCANNABLE, scannable );
					red_alert_carry = tristate_set( Ships[i].flags & SF_RED_ALERT_STORE_STATUS, red_alert_carry );
					special_warp = tristate_set( Objects[Ships[i].objnum].flags & OF_SPECIAL_WARP, special_warp );
					protect_ship = tristate_set(Objects[Ships[i].objnum].flags & OF_PROTECTED, protect_ship);
					beam_protect_ship = tristate_set(Objects[Ships[i].objnum].flags & OF_BEAM_PROTECTED, beam_protect_ship);
					invulnerable = tristate_set(Ships[i].flags & SF_INVULNERABLE, invulnerable);
					hidden_from_sensors = tristate_set(Ships[i].flags & SF_HIDDEN_FROM_SENSORS, hidden_from_sensors);
					ignore_count = tristate_set(Ships[i].flags & SF_IGNORE_COUNT, ignore_count);
					no_arrival_music = tristate_set(Ships[i].flags & SF_NO_ARRIVAL_MUSIC, no_arrival_music);
					cargo_known = tristate_set(Ships[i].flags & SF_CARGO_REVEALED, cargo_known);
					no_dynamic = tristate_set( Ai_info[Ships[i].ai_index].ai_flags & AIF_NO_DYNAMIC, no_dynamic );

					// check the final death time and set the internal variable according to whether or not
					// the final_death_time is set.  Also, the value in the edit box must be set if all the
					// values are the same, and cleared if the values are not the same.
					destroy_before_mission = tristate_set(Ships[i].flags & SF_KILL_BEFORE_MISSION, destroy_before_mission);
					m_destroy_value.set(Ships[i].final_death_time);

					kamikaze = tristate_set( Ai_info[Ships[i].ai_index].ai_flags & AIF_KAMIKAZE, kamikaze );
					m_kdamage.set( (int) Ai_info[Ships[i].ai_index].kamikaze_damage );

					escort = tristate_set(Ships[i].flags & SF_ESCORT, escort);
					m_escort_value.init(Ships[i].escort_priority);

					if(The_mission.game_type & MISSION_TYPE_MULTI){
						m_respawn_priority.init(Ships[i].escort_priority);
					}

					for (j=0; j<Num_reinforcements; j++){
						if (!stricmp(Reinforcements[j].name, Ships[i].ship_name)){
							break;
						}
					}
					reinforcement = tristate_set(j < Num_reinforcements, reinforcement);

					// check if ship in wing
					ship_in_wing = (Ships[i].wingnum != -1);;
				}

			}
		}

		objp = GET_NEXT(objp);
	}

	CDialog::OnInitDialog();
	
	m_protect_ship.SetCheck(protect_ship);
	m_beam_protect_ship.SetCheck(beam_protect_ship);
	m_ignore_count.SetCheck(ignore_count);
	m_reinforcement.SetCheck(reinforcement);
	m_cargo_known.SetCheck(cargo_known);
	m_destroy.SetCheck(destroy_before_mission);
	m_no_arrival_music.SetCheck(no_arrival_music);
	m_escort.SetCheck(escort);
	m_invulnerable.SetCheck(invulnerable);
	m_hidden.SetCheck(hidden_from_sensors);
	m_scannable.SetCheck(scannable);
	m_kamikaze.SetCheck(kamikaze);
	m_no_dynamic.SetCheck(no_dynamic);
	m_red_alert_carry.SetCheck(red_alert_carry);
	m_special_warp.SetCheck(special_warp);
		
	m_kdamage.setup(IDC_KDAMAGE, this);
	m_destroy_value.setup(IDC_DESTROY_VALUE, this);
	m_escort_value.setup(IDC_ESCORT_PRIORITY, this);

	if(The_mission.game_type & MISSION_TYPE_MULTI){
		m_respawn_priority.setup(IDC_RESPAWN_PRIORITY, this);
	}
	m_destroy_spin.SetRange(0, UD_MAXVAL);

	m_destroy_value.display();
	m_kdamage.display();
	m_escort_value.display();

	if(The_mission.game_type & MISSION_TYPE_MULTI){
		m_respawn_priority.display();
	} else {
		GetDlgItem(IDC_RESPAWN_PRIORITY)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_REINFORCEMENT)->EnableWindow(p_enable);
	GetDlgItem(IDC_CARGO_KNOWN)->EnableWindow(p_enable);
	GetDlgItem(IDC_PROTECT_SHIP)->EnableWindow(p_enable);
	GetDlgItem(IDC_DESTROY_CHECK)->EnableWindow(p_enable);
	GetDlgItem(IDC_DESTROY_VALUE)->EnableWindow(p_enable);
	GetDlgItem(IDC_DESTROY_SPIN)->EnableWindow(p_enable);
	GetDlgItem(IDC_SCANNABLE)->EnableWindow(p_enable);

	// disable the spinner and edit window if the corrsponding check box
	// is not checked!
	if (m_destroy.GetCheck() != 1) {
		GetDlgItem(IDC_DESTROY_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DESTROY_SPIN)->EnableWindow(FALSE);
	}

	// disable destroy option for ship in wing
	if (ship_in_wing) {
		GetDlgItem(IDC_DESTROY_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DESTROY_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DESTROY_SPIN)->EnableWindow(FALSE);
	}

	// maybe disable escort priority window
	if (m_escort.GetCheck() == 1)
		GetDlgItem(IDC_ESCORT_PRIORITY)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_ESCORT_PRIORITY)->EnableWindow(FALSE);

	// maybe disable kamikaze damage window
	if (m_kamikaze.GetCheck() == 1)
		GetDlgItem(IDC_KDAMAGE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_KDAMAGE)->EnableWindow(FALSE);

	return TRUE;
}

void ship_flags_dlg::OnOK()
{
	object *objp;

	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if ((objp->type == OBJ_START) || (objp->type == OBJ_SHIP)) {
			if (objp->flags & OF_MARKED)
				update_ship(objp->instance);
		}

		objp = GET_NEXT(objp);
	}

	CDialog::OnOK();
}

void ship_flags_dlg::update_ship(int ship)
{
	if (m_reinforcement.GetCheck() != 2)
		set_reinforcement(Ships[ship].ship_name, m_reinforcement.GetCheck());

	switch (m_cargo_known.GetCheck()) {
		case 1:
			if ( !(Ships[ship].flags & SF_CARGO_REVEALED) )
				set_modified();

			Ships[ship].flags |= SF_CARGO_REVEALED;
			break;

		case 0:
			if ( Ships[ship].flags & SF_CARGO_REVEALED )
				set_modified();

			Ships[ship].flags &= ~SF_CARGO_REVEALED;
			break;
	}

	// update the flags for IGNORE_COUNT and PROTECT_SHIP
	switch (m_protect_ship.GetCheck()) {
		case 1:
			if (!(Objects[Ships[ship].objnum].flags & OF_PROTECTED) )
				set_modified();

			Objects[Ships[ship].objnum].flags |= OF_PROTECTED;
			break;

		case 0:
			if ( Objects[Ships[ship].objnum].flags & OF_PROTECTED )
				set_modified();

			Objects[Ships[ship].objnum].flags &= ~OF_PROTECTED;
			break;
	}

	switch (m_beam_protect_ship.GetCheck()) {
		case 1:
			if (!(Objects[Ships[ship].objnum].flags & OF_BEAM_PROTECTED) )
				set_modified();

			Objects[Ships[ship].objnum].flags |= OF_BEAM_PROTECTED;
			break;

		case 0:
			if ( Objects[Ships[ship].objnum].flags & OF_BEAM_PROTECTED )
				set_modified();

			Objects[Ships[ship].objnum].flags &= ~OF_BEAM_PROTECTED;
			break;
	}

	switch (m_invulnerable.GetCheck()) {
		case 1:
			if ( !(Ships[ship].flags & SF_INVULNERABLE) )
				set_modified();

			Ships[ship].flags |= SF_INVULNERABLE;
			break;

		case 0:
			if ( Ships[ship].flags & SF_INVULNERABLE )
				set_modified();

			Ships[ship].flags &= ~SF_INVULNERABLE;
			break;
	}

	switch (m_hidden.GetCheck()) {
		case 1:
			if ( !(Ships[ship].flags & SF_HIDDEN_FROM_SENSORS) )
				set_modified();

			Ships[ship].flags |= SF_HIDDEN_FROM_SENSORS;
			break;

		case 0:
			if ( Ships[ship].flags & SF_HIDDEN_FROM_SENSORS )
				set_modified();

			Ships[ship].flags &= ~SF_HIDDEN_FROM_SENSORS;
			break;
	}

	switch (m_ignore_count.GetCheck()) {
		case 1:
			if ( !(Ships[ship].flags & SF_IGNORE_COUNT) )
				set_modified();

			Ships[ship].flags |= SF_IGNORE_COUNT;
			break;

		case 0:
			if (Ships[ship].flags & SF_IGNORE_COUNT)
				set_modified();

			Ships[ship].flags &= ~SF_IGNORE_COUNT;
			break;
	}

	switch (m_escort.GetCheck()) {
		case 1:
			if (!(Ships[ship].flags & SF_ESCORT))
				set_modified();

			Ships[ship].flags |= SF_ESCORT;
			m_escort_value.save(&Ships[ship].escort_priority);
			break;

		case 0:
			if (Ships[ship].flags & SF_ESCORT)
				set_modified();

			Ships[ship].flags &= ~SF_ESCORT;
			break;
	}

	// deal with updating the "destroy before the mission" stuff
	switch (m_destroy.GetCheck()) {
		case 0:  // this means no check in checkbox
			if ( Ships[ship].flags & SF_KILL_BEFORE_MISSION )
				set_modified();

			Ships[ship].flags &= ~SF_KILL_BEFORE_MISSION;
			break;

		case 1:  // this means checkbox is checked
			if ( !(Ships[ship].flags & SF_KILL_BEFORE_MISSION) )
				set_modified();

			Ships[ship].flags |= SF_KILL_BEFORE_MISSION;
			m_destroy_value.save(&Ships[ship].final_death_time);
			break;
	}  // a mixed state is 2, and since it's not handled, it doesn't change

	switch (m_no_arrival_music.GetCheck()) {
		case 0:
			if (Ships[ship].flags & SF_NO_ARRIVAL_MUSIC)
				set_modified();

			Ships[ship].flags &= ~SF_NO_ARRIVAL_MUSIC;
			break;

		case 1:
			if (!(Ships[ship].flags & SF_NO_ARRIVAL_MUSIC))
				set_modified();

			Ships[ship].flags |= SF_NO_ARRIVAL_MUSIC;
			break;
	}

	switch (m_scannable.GetCheck()) {
		case 1:
			if ( !(Ships[ship].flags & SF_SCANNABLE) )
				set_modified();

			Ships[ship].flags |= SF_SCANNABLE;
			break;

		case 0:
			if ( Ships[ship].flags & SF_SCANNABLE )
				set_modified();

			Ships[ship].flags &= ~SF_SCANNABLE;
			break;
	}

	switch (m_red_alert_carry.GetCheck()) {
		case 1:
			if ( !(Ships[ship].flags & SF_RED_ALERT_STORE_STATUS) )
				set_modified();

			Ships[ship].flags |= SF_RED_ALERT_STORE_STATUS;
			break;

		case 0:
			if ( Ships[ship].flags & SF_RED_ALERT_STORE_STATUS )
				set_modified();

			Ships[ship].flags &= ~SF_RED_ALERT_STORE_STATUS;
			break;
	}

	switch (m_special_warp.GetCheck()) {
		case 1:
			if ( !(Objects[Ships[ship].objnum].flags & OF_SPECIAL_WARP) )
				set_modified();

			Objects[Ships[ship].objnum].flags |= OF_SPECIAL_WARP;
			break;

		case 0:
			if ( (Objects[Ships[ship].objnum].flags & OF_SPECIAL_WARP) )
				set_modified();

			Objects[Ships[ship].objnum].flags &= (~OF_SPECIAL_WARP);
			break;
	}

	switch (m_no_dynamic.GetCheck()) {
		case 1:
			if ( !(Ai_info[Ships[ship].ai_index].ai_flags & AIF_NO_DYNAMIC) )
				set_modified();

			Ai_info[Ships[ship].ai_index].ai_flags |= AIF_NO_DYNAMIC;
			break;

		case 0:
			if ( Ai_info[Ships[ship].ai_index].ai_flags & AIF_NO_DYNAMIC )
				set_modified();

			Ai_info[Ships[ship].ai_index].ai_flags &= ~AIF_NO_DYNAMIC;
			break;
	}

	switch (m_kamikaze.GetCheck()) {
		case 1: {
			int damage;

			if ( !(Ai_info[Ships[ship].ai_index].ai_flags & AIF_KAMIKAZE) )
				set_modified();

			Ai_info[Ships[ship].ai_index].ai_flags |= AIF_KAMIKAZE;
			m_kdamage.save(&damage);
			Ai_info[Ships[ship].ai_index].kamikaze_damage = i2fl(damage);
			break;
		}

		case 0:
			if ( Ai_info[Ships[ship].ai_index].ai_flags & AIF_KAMIKAZE )
				set_modified();

			Ai_info[Ships[ship].ai_index].ai_flags &= ~AIF_KAMIKAZE;
			Ai_info[Ships[ship].ai_index].kamikaze_damage = 0.0f;
			break;
	}

	Ships[ship].respawn_priority = 0;
	if(The_mission.game_type & MISSION_TYPE_MULTI){
		m_respawn_priority.save(&Ships[ship].respawn_priority);
	}
}

int ship_flags_dlg::tristate_set(int val, int cur_state)
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

// a stub for now, but might be useful later.  Easier than ripping out the calls to this
// everywhere at least.
void ship_flags_dlg::set_modified()
{
}

void ship_flags_dlg::OnCargoKnown() 
{
	if (m_cargo_known.GetCheck() == 1){
		m_cargo_known.SetCheck(0);
	} else {
		m_cargo_known.SetCheck(1);
	}
}

void ship_flags_dlg::OnDestroyCheck() 
{
	if (m_destroy.GetCheck() == 1) {
		m_destroy.SetCheck(0);
		GetDlgItem(IDC_DESTROY_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DESTROY_SPIN)->EnableWindow(FALSE);

	} else {
		m_destroy.SetCheck(1);
		GetDlgItem(IDC_DESTROY_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DESTROY_SPIN)->EnableWindow(TRUE);
	}
}

void ship_flags_dlg::OnEscort() 
{
	if (m_escort.GetCheck() == 1) {
		m_escort.SetCheck(0);
		GetDlgItem(IDC_ESCORT_PRIORITY)->EnableWindow(FALSE);
	} else {
		m_escort.SetCheck(1);
		GetDlgItem(IDC_ESCORT_PRIORITY)->EnableWindow(TRUE);
	}
}

void ship_flags_dlg::OnHiddenFromSensors() 
{
	if (m_hidden.GetCheck() == 1){
		m_hidden.SetCheck(0);
	} else {
		m_hidden.SetCheck(1);
	}
}

void ship_flags_dlg::OnIgnoreCount() 
{
	if (m_ignore_count.GetCheck() == 1){
		m_ignore_count.SetCheck(0);
	} else {
		m_ignore_count.SetCheck(1);
	}
}

void ship_flags_dlg::OnInvulnerable() 
{
	if (m_invulnerable.GetCheck() == 1){
		m_invulnerable.SetCheck(0);
	} else {
		m_invulnerable.SetCheck(1);
	}
}

void ship_flags_dlg::OnKamikaze() 
{
	if (m_kamikaze.GetCheck() == 1) {
		GetDlgItem(IDC_KDAMAGE)->EnableWindow(FALSE);
		m_kamikaze.SetCheck(0);

	} else {
		GetDlgItem(IDC_KDAMAGE)->EnableWindow(TRUE);
		m_kamikaze.SetCheck(1);
	}
}

void ship_flags_dlg::OnNoArrivalMusic() 
{
	if (m_no_arrival_music.GetCheck() == 1){
		m_no_arrival_music.SetCheck(0);
	} else {
		m_no_arrival_music.SetCheck(1);
	}
}

void ship_flags_dlg::OnNoDynamic() 
{
	if (m_no_dynamic.GetCheck() == 1){
		m_no_dynamic.SetCheck(0);
	} else {
		m_no_dynamic.SetCheck(1);
	}
}

void ship_flags_dlg::OnProtectShip() 
{
	if (m_protect_ship.GetCheck() == 1){
		m_protect_ship.SetCheck(0);
	} else {
		m_protect_ship.SetCheck(1);
	}
}

void ship_flags_dlg::OnBeamProtectShip() 
{
	if (m_beam_protect_ship.GetCheck() == 1){
		m_beam_protect_ship.SetCheck(0);
	} else {
		m_beam_protect_ship.SetCheck(1);
	}
}

void ship_flags_dlg::OnReinforcement() 
{
	if (m_reinforcement.GetCheck() == 1){
		m_reinforcement.SetCheck(0);
	} else {
		m_reinforcement.SetCheck(1);
	}
}

void ship_flags_dlg::OnScannable() 
{
	if (m_scannable.GetCheck() == 1){
		m_scannable.SetCheck(0);
	} else {
		m_scannable.SetCheck(1);
	}
}

void ship_flags_dlg::OnRedalertcarry() 
{
	if (m_red_alert_carry.GetCheck() == 1){
		m_red_alert_carry.SetCheck(0);
	} else {
		m_red_alert_carry.SetCheck(1);
	}
}

