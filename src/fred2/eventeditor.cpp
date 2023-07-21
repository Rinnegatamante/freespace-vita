/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/fred2/EventEditor.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Event editor dialog box class and event tree class
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
 * 12    9/13/99 8:03a Andsager
 * Add command heads 3,4,5 as allowable head animations.
 * 
 * 11    9/09/99 5:07a Andsager
 * Make sure TP2 is available in FRED head ani
 * 
 * 10    9/01/99 2:52p Andsager
 * Add new heads to FRED and some debug code for playing heads
 * 
 * 9     8/28/99 7:29p Dave
 * Fixed wingmen persona messaging. Make sure locked turrets don't count
 * towards the # attacking a player.
 * 
 * 8     8/26/99 8:52p Dave
 * Gave multiplayer TvT messaging a heavy dose of sanity. Cheat codes.
 * 
 * 7     5/04/99 5:21p Andsager
 * 
 * 6     2/17/99 2:11p Dave
 * First full run of squad war. All freespace and tracker side stuff
 * works.
 * 
 * 5     1/21/99 9:29a Andsager
 * 
 * 4     12/17/98 2:41p Andsager
 * Changed input into sexp_tree::insert() to include bitmaps
 * 
 * 3     11/06/98 11:21a Johnson
 * Put in handling code for wacky event editor Assert().
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
 * 55    9/25/98 1:33p Andsager
 * Add color to event editor (root and chain) indicating mission directive
 * 
 * 54    7/09/98 10:57a Hoffoss
 * Fixed bug where the 'update stuff' button was reverting changes made to
 * various message fields.
 * 
 * 53    5/15/98 5:51p Hoffoss
 * Fixed escape key and cancel button bugs.
 * 
 * 52    5/12/98 11:44a Hoffoss
 * Made escape key not close dialog (and lose changes made).
 * 
 * 51    4/30/98 9:53p Hoffoss
 * Added "Head-VC" to ani list at Sandeep's request.
 * 
 * 50    4/30/98 8:23p John
 * Fixed some bugs with Fred caused by my new cfile code.
 * 
 * 49    4/22/98 9:56a Sandeep
 * 
 * 48    4/20/98 4:40p Hoffoss
 * Added a button to 4 editors to play the chosen wave file.
 * 
 * 47    4/03/98 5:20p Hoffoss
 * Changed code so that changing a message's wave file will update the
 * persona as well, if the wave file has the proper prefix.
 * 
 * 46    4/03/98 12:39p Hoffoss
 * Changed starting directory for browse buttons in several editors.
 * 
 * 45    3/10/98 4:06p Hoffoss
 * Fixed browse button blues.
 * 
 * 44    3/06/98 2:24p Hoffoss
 * Fixed bug with going to reference with deleting a entity referenced by
 * an sexp tree.
 * 
 * 43    2/16/98 6:25p Hoffoss
 * Did major rework of the whole right_clicked() handler to simplify it
 * all, break it down and make it more flexible.  Should be a lot easier
 * to work with from now on.
 * 
 * 42    2/16/98 2:42p Hoffoss
 * Added new code in preparation to simplify the sexp_tree monster.
 * Checking in code now as a good foundation point that I can revert back
 * to if needed.
 * 
 * 41    1/23/98 3:06p Hoffoss
 * Added an explicit <none> item to the filename combo boxes at designers
 * request.
 * 
 * 40    1/09/98 3:41p Hoffoss
 * Fixed bug with event moving not updating fields properly.
 * 
 * 39    1/08/98 11:18a Hoffoss
 * Fixed several bugs in new Event Editor.
 * 
 * 38    1/08/98 10:24a Johnson
 * Fixed bug with null strings for filenames.
 * 
 * 37    1/07/98 5:58p Hoffoss
 * Combined message editor into event editor.
 * 
 * 36    1/06/98 8:25p Hoffoss
 * Added insert event functionality to event editor.
 * 
 * 35    1/06/98 3:31p Hoffoss
 * Added image to indicate chained events, and added code to support it.
 * 
 * 34    10/20/97 5:13p Allender
 * new subsystem sabotage/repair/set sexpressions.  Added new event/goal
 * status checking sexpressions (not fully implemented yet).  Change
 * campaign save files to save all events as well as goals
 * 
 * 33    10/10/97 6:21p Hoffoss
 * Put in Fred support for training object list editing.
 * 
 * 32    10/10/97 2:53p Johnson
 * Fixed bug with new items being selected before they are fully
 * registered as added.
 * 
 * $NoKeywords: $
 */

#include "stdafx.h"
#include <mmsystem.h>
#include "fred.h"
#include "freddoc.h"
#include "eventeditor.h"
#include "fredview.h"
#include "management.h"
#include "sexp_tree.h"
#include "missionmessage.h"
#include "cfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

event_editor *Event_editor_dlg = NULL; // global reference needed by event tree class

// determine the node number that would be allocated without actually allocating it yet.
int sexp_event_tree::get_new_node_position()
{
	int i;

	for (i=0; i<MAX_SEXP_TREE_SIZE; i++)
		if (nodes[i].type == SEXPT_UNUSED)
			return i;

	return -1;
}

// construct tree nodes for an sexp, adding them to the list and returning first node
int sexp_event_tree::load_sub_tree(int index)
{
	int cur;

	if (index < 0) {
		cur = allocate_node(-1);
		set_node(cur, SEXPT_OPERATOR, "do-nothing");  // setup a default tree if none
		return cur;
	}

	// assumption: first token is an operator.  I require this because it would cause problems
	// with child/parent relations otherwise, and it should be this way anyway, since the
	// return type of the whole sexp is boolean, and only operators can satisfy this.
	Assert(Sexp_nodes[index].subtype == SEXP_ATOM_OPERATOR);
	cur = get_new_node_position();
	load_branch(index, -1);
	return cur;
}

/////////////////////////////////////////////////////////////////////////////
// event_editor dialog

event_editor::event_editor(CWnd* pParent /*=NULL*/)
	: CDialog(event_editor::IDD, pParent)
{
	//{{AFX_DATA_INIT(event_editor)
	m_repeat_count = 0;
	m_interval = 0;
	m_event_score = 0;
	m_chain_delay = 0;
	m_chained = FALSE;
	m_obj_text = _T("");
	m_obj_key_text = _T("");
	m_avi_filename = _T("");
	m_message_name = _T("");
	m_message_text = _T("");
	m_persona = -1;
	m_wave_filename = _T("");
	m_cur_msg = -1;
	m_team = -1;
	m_message_team = -1;
	m_last_message_node = -1;
	//}}AFX_DATA_INIT
	m_event_tree.m_mode = MODE_EVENTS;
	m_num_events = 0;
	m_event_tree.link_modified(&modified);
	modified = 0;
	select_sexp_node = -1;
}

void event_editor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(event_editor)
	DDX_Control(pDX, IDC_EVENT_TREE, m_event_tree);
	DDX_Text(pDX, IDC_REPEAT_COUNT, m_repeat_count);
	DDX_Text(pDX, IDC_INTERVAL_TIME, m_interval);
	DDX_Text(pDX, IDC_EVENT_SCORE, m_event_score);
	DDX_Text(pDX, IDC_CHAIN_DELAY, m_chain_delay);
	DDX_Check(pDX, IDC_CHAINED, m_chained);
	DDX_Text(pDX, IDC_OBJ_TEXT, m_obj_text);
	DDX_Text(pDX, IDC_OBJ_KEY_TEXT, m_obj_key_text);
	DDX_CBString(pDX, IDC_AVI_FILENAME, m_avi_filename);
	DDX_Text(pDX, IDC_MESSAGE_NAME, m_message_name);
	DDX_Text(pDX, IDC_MESSAGE_TEXT, m_message_text);
	DDX_CBIndex(pDX, IDC_PERSONA_NAME, m_persona);
	DDX_CBString(pDX, IDC_WAVE_FILENAME, m_wave_filename);
	DDX_LBIndex(pDX, IDC_MESSAGE_LIST, m_cur_msg);

	// m_team == -1 maps to 2
	if(m_team == -1){
		m_team = 2;
	}
	DDX_CBIndex(pDX, IDC_EVENT_TEAM, m_team);

	// m_message_team == -1 maps to 2
	if(m_message_team == -1){
		m_message_team = 2;
	}
	DDX_CBIndex(pDX, IDC_MESSAGE_TEAM, m_message_team);
	//}}AFX_DATA_MAP

	DDV_MaxChars(pDX, m_obj_text, NAME_LENGTH - 1);
	DDV_MaxChars(pDX, m_obj_key_text, NAME_LENGTH - 1);
	DDV_MaxChars(pDX, m_message_name, NAME_LENGTH - 1);
	DDV_MaxChars(pDX, m_message_text, MESSAGE_LENGTH - 1);
	DDV_MaxChars(pDX, m_avi_filename, MAX_FILENAME_LEN - 1);
	DDV_MaxChars(pDX, m_wave_filename, MAX_FILENAME_LEN - 1);
}

BEGIN_MESSAGE_MAP(event_editor, CDialog)
	//{{AFX_MSG_MAP(event_editor)
	ON_NOTIFY(NM_RCLICK, IDC_EVENT_TREE, OnRclickEventTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_EVENT_TREE, OnBeginlabeleditEventTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_EVENT_TREE, OnEndlabeleditEventTree)
	ON_BN_CLICKED(IDC_BUTTON_NEW_EVENT, OnButtonNewEvent)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(ID_OK, OnOk)
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_EVENT_TREE, OnSelchangedEventTree)
	ON_EN_UPDATE(IDC_REPEAT_COUNT, OnUpdateRepeatCount)
	ON_BN_CLICKED(IDC_CHAINED, OnChained)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_LBN_SELCHANGE(IDC_MESSAGE_LIST, OnSelchangeMessageList)
	ON_BN_CLICKED(IDC_NEW_MSG, OnNewMsg)
	ON_BN_CLICKED(IDC_DELETE_MSG, OnDeleteMsg)
	ON_BN_CLICKED(IDC_BROWSE_AVI, OnBrowseAvi)
	ON_BN_CLICKED(IDC_BROWSE_WAVE, OnBrowseWave)
	ON_CBN_SELCHANGE(IDC_WAVE_FILENAME, OnSelchangeWaveFilename)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(ID_CANCEL, On_Cancel)
	ON_CBN_SELCHANGE(IDC_EVENT_TEAM, OnSelchangeTeam)
	ON_CBN_SELCHANGE(IDC_MESSAGE_TEAM, OnSelchangeMessageTeam)
	ON_LBN_DBLCLK(IDC_MESSAGE_LIST, OnDblclkMessageList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// event_editor message handlers

void maybe_add_head(CComboBox *box, char* name)
{
	if (box->FindStringExact(-1, name) == CB_ERR) {
		box->AddString(name);
	}
}

BOOL event_editor::OnInitDialog() 
{
	int i, adjust = 0;
	BOOL r = TRUE;
	CListBox *list;
	CComboBox *box;

	CDialog::OnInitDialog();  // let the base class do the default work
	m_play_bm.LoadBitmap(IDB_PLAY);
	((CButton *) GetDlgItem(IDC_PLAY)) -> SetBitmap(m_play_bm);

	if (!Show_sexp_help)
		adjust = -SEXP_HELP_BOX_SIZE;

	theApp.init_window(&Events_wnd_data, this, adjust);
	m_event_tree.setup((CEdit *) GetDlgItem(IDC_HELP_BOX));
	load_tree();
	create_tree();
	if (m_num_events >= MAX_MISSION_EVENTS){
		GetDlgItem(IDC_BUTTON_NEW_EVENT)->EnableWindow(FALSE);
	}

	update_cur_event();
	i = m_event_tree.select_sexp_node;
	if (i != -1) {
		GetDlgItem(IDC_EVENT_TREE) -> SetFocus();
		m_event_tree.hilite_item(i);
		r = FALSE;
	}

	m_num_messages = Num_messages - Num_builtin_messages;
	for (i=0; i<m_num_messages; i++) {
		m_messages[i] = Messages[i + Num_builtin_messages];
		if (m_messages[i].avi_info.name){
			m_messages[i].avi_info.name = strdup(m_messages[i].avi_info.name);
		}
		if (m_messages[i].wave_info.name){
			m_messages[i].wave_info.name = strdup(m_messages[i].wave_info.name);
		}
		m_msg_sig[i] = i + Num_builtin_messages;
	}

	((CEdit *) GetDlgItem(IDC_MESSAGE_NAME))->LimitText(NAME_LENGTH - 1);
	((CEdit *) GetDlgItem(IDC_MESSAGE_TEXT))->LimitText(MESSAGE_LENGTH - 1);
	((CComboBox *) GetDlgItem(IDC_AVI_FILENAME))->LimitText(MAX_FILENAME_LEN - 1);
	((CComboBox *) GetDlgItem(IDC_WAVE_FILENAME))->LimitText(MAX_FILENAME_LEN - 1);

	list = (CListBox *) GetDlgItem(IDC_MESSAGE_LIST);
	list->ResetContent();
	for (i=0; i<m_num_messages; i++) {
		list->AddString(m_messages[i].name);
	}

	box = (CComboBox *) GetDlgItem(IDC_AVI_FILENAME);
	box->ResetContent();
	box->AddString("<None>");
	for (i=0; i<Num_messages; i++) {
		if (Messages[i].avi_info.name) {
			maybe_add_head(box, Messages[i].avi_info.name);
		}
	}

	// add new heads, if not already in
	maybe_add_head(box, "Head-TP2");
	maybe_add_head(box, "Head-VC2");
	maybe_add_head(box, "Head-TP4");
	maybe_add_head(box, "Head-TP5");
	maybe_add_head(box, "Head-TP6");
	maybe_add_head(box, "Head-TP7");
	maybe_add_head(box, "Head-TP8");
	maybe_add_head(box, "Head-VP2");
	maybe_add_head(box, "Head-VP2");
	maybe_add_head(box, "Head-CM2");
	maybe_add_head(box, "Head-CM3");
	maybe_add_head(box, "Head-CM4");
	maybe_add_head(box, "Head-CM5");
	maybe_add_head(box, "Head-BSH");

/*
	box->AddString("Head-VC");  // force it in, since Sandeep wants it and it's not used in built-in messages
	box->AddString("Head-VC2");

	// add terran pilot heads
	box->AddString("Head-TP4");
	box->AddString("Head-TP5");
	box->AddString("Head-TP6");
	box->AddString("Head-TP7");
	box->AddString("Head-TP8");

	// add vasudan pilot heads
	box->AddString("Head-VP2");

	// BSH and CM2
	box->AddString("Head-CM2");
	box->AddString("Head-BSH");
	*/

	box = (CComboBox *) GetDlgItem(IDC_WAVE_FILENAME);
	box->ResetContent();
	box->AddString("<None>");
	for (i=0; i<Num_messages; i++){
		if (Messages[i].wave_info.name){
			if (box->FindStringExact(i, Messages[i].wave_info.name) == CB_ERR){
				box->AddString(Messages[i].wave_info.name);
			}
		}
	}

	// add the persona names into the combo box
	box = (CComboBox *) GetDlgItem(IDC_PERSONA_NAME);
	box->ResetContent();
	box->AddString("<None>");
	for (i = 0; i < Num_personas; i++ ){
		box->AddString( Personas[i].name );
	}

	// set the first message to be the first non-builtin message (if it exists)
	if ( Num_messages > Num_builtin_messages ){
		m_cur_msg = 0;
	} else {
		m_cur_msg = -1;
	}

	if (Num_messages >= MAX_MISSION_MESSAGES){
		GetDlgItem(IDC_NEW_MSG)->EnableWindow(FALSE);
	}

	update_cur_message();
	return r;
}

void event_editor::load_tree()
{
	int i;

	m_event_tree.select_sexp_node = select_sexp_node;
	select_sexp_node = -1;

	m_event_tree.clear_tree();
	m_num_events = Num_mission_events;
	for (i=0; i<m_num_events; i++) {
		m_events[i] = Mission_events[i];
		if (Mission_events[i].objective_text){
			m_events[i].objective_text = strdup(Mission_events[i].objective_text);
		} else {
			m_events[i].objective_text = NULL;
		}

		if (Mission_events[i].objective_key_text){
			m_events[i].objective_key_text = strdup(Mission_events[i].objective_key_text);
		} else {
			m_events[i].objective_key_text = NULL;
		}
		
		m_sig[i] = i;
		if (!(*m_events[i].name)){
			strcpy(m_events[i].name, "<Unnamed>");
		}

		m_events[i].formula = m_event_tree.load_sub_tree(Mission_events[i].formula);

		// we must check for the case of the repeat count being 0.  This would happen if the repeat
		// count is not specified in a mission
		if ( m_events[i].repeat_count <= 0 ){
			m_events[i].repeat_count = 1;
		}
	}

	m_event_tree.post_load();
	cur_event = -1;
}

void event_editor::create_tree()
{
	int i;
	HTREEITEM h;

	m_event_tree.DeleteAllItems();
	for (i=0; i<m_num_events; i++) {

		// set the proper bitmap
		int image;
		if (m_events[i].chain_delay >= 0) {
			image = BITMAP_CHAIN;
			if (m_events[i].objective_text) {
				image = BITMAP_CHAIN_DIRECTIVE;
			}
		} else {
			image = BITMAP_ROOT;
			if (m_events[i].objective_text) {
				image = BITMAP_ROOT_DIRECTIVE;
			}
		}

		h = m_event_tree.insert(m_events[i].name, image, image);

		m_event_tree.SetItemData(h, m_events[i].formula);
		m_event_tree.add_sub_tree(m_events[i].formula, h);
	}

	cur_event = -1;
}

void event_editor::OnRclickEventTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	save();
	m_event_tree.right_clicked(MODE_EVENTS);
	*pResult = 0;
}

void event_editor::OnBeginlabeleditEventTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CEdit *edit;

	if (m_event_tree.edit_label(pTVDispInfo->item.hItem) == 1)	{
		*pResult = 0;
		modified = 1;
		edit = m_event_tree.GetEditControl();
		Assert(edit);
		edit->SetLimitText(NAME_LENGTH - 1);

	} else
		*pResult = 1;
}

void event_editor::OnEndlabeleditEventTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	*pResult = m_event_tree.end_label_edit(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
}

// This is needed as a HACK around default MFC standard
// It is not required, but overrides default MFC and links no errors without.
void event_editor::OnOK()
{
	HWND h;
	CWnd *w;

	save();
	w = GetFocus();
	if (w) {
		h = w->m_hWnd;
		GetDlgItem(IDC_EVENT_TREE)->SetFocus();
		::SetFocus(h);
	}
}

int event_editor::query_modified()
{
	int i;
	char *ptr, buf[MESSAGE_LENGTH];

	UpdateData(TRUE);
	if (modified)
		return 1;

	if (Num_mission_events != m_num_events)
		return 1;

	for (i=0; i<m_num_events; i++) {
		if (stricmp(m_events[i].name, Mission_events[i].name))
			return 1;
		if (m_events[i].repeat_count != Mission_events[i].repeat_count)
			return 1;
		if (m_events[i].interval != Mission_events[i].interval)
			return 1;
		if (m_events[i].score != Mission_events[i].score)
			return 1;
		if (m_events[i].chain_delay != Mission_events[i].chain_delay)
			return 1;
		if (advanced_stricmp(m_events[i].objective_text, Mission_events[i].objective_text))
			return 1;
		if (advanced_stricmp(m_events[i].objective_key_text, Mission_events[i].objective_key_text))
			return 1;
	}

	if (m_cur_msg < 0)
		return 0;

	if (m_num_messages != Num_messages)
		return 1;

	ptr = (char *) (LPCTSTR) m_message_name;
	for (i=0; i<Num_builtin_messages; i++)
		if (!stricmp(ptr, Messages[i].name))
			return 1;

	for (i=0; i<m_num_messages; i++) {
		if (m_msg_sig[i] < 0)
			return 1;

		if ((i != m_cur_msg) && (!stricmp(ptr, m_messages[m_cur_msg].name)))
			return 1;
	}

	if (stricmp(ptr, m_messages[m_cur_msg].name))
		return 1;  // name is different and allowed to update

	string_copy(buf, m_message_text, MESSAGE_LENGTH - 1);
	if (stricmp(buf, m_messages[m_cur_msg].message))
		return 1;

	ptr = (char *) (LPCTSTR) m_avi_filename;
	if (advanced_stricmp(ptr, m_messages[m_cur_msg].avi_info.name))
		return 1;

	ptr = (char *) (LPCTSTR) m_wave_filename;
	if (advanced_stricmp(ptr, m_messages[m_cur_msg].wave_info.name))
		return 1;

	// check to see if persona changed.  use -1 since we stuck a "None" for persona
	// at the beginning of the list.
	if ( (m_persona - 1 ) != m_messages[m_cur_msg].persona_index )
		return 1;

	return 0;
}

void event_editor::OnOk()
{
	char buf[256], names[2][MAX_MISSION_EVENTS][NAME_LENGTH];
	int i, count;

	save();
	if (query_modified())
		set_modified();

	for (i=0; i<Num_mission_events; i++) {
		free_sexp2(Mission_events[i].formula);
		if (Mission_events[i].objective_text)
			free(Mission_events[i].objective_text);
		if (Mission_events[i].objective_key_text)
			free(Mission_events[i].objective_key_text);
	}

	count = 0;
	for (i=0; i<Num_mission_events; i++)
		Mission_events[i].result = 0;  // use this as a processed flag
	
	// rename all sexp references to old events
	for (i=0; i<m_num_events; i++)
		if (m_sig[i] >= 0) {
			strcpy(names[0][count], Mission_events[m_sig[i]].name);
			strcpy(names[1][count], m_events[i].name);
			count++;
			Mission_events[m_sig[i]].result = 1;
		}

	// invalidate all sexp references to deleted events.
	for (i=0; i<Num_mission_events; i++)
		if (!Mission_events[i].result) {
			sprintf(buf, "<%s>", Mission_events[i].name);
			strcpy(buf + NAME_LENGTH - 2, ">");  // force it to be not too long
			strcpy(names[0][count], Mission_events[i].name);
			strcpy(names[1][count], buf);
			count++;
		}

	Num_mission_events = m_num_events;
	for (i=0; i<m_num_events; i++) {
		Mission_events[i] = m_events[i];
		Mission_events[i].formula = m_event_tree.save_tree(m_events[i].formula);
		Mission_events[i].objective_text = m_events[i].objective_text;
		Mission_events[i].objective_key_text = m_events[i].objective_key_text;
	}

	// now update all sexp references
	while (count--)
		update_sexp_references(names[0][count], names[1][count], OPF_EVENT_NAME);

	for (i=Num_builtin_messages; i<Num_messages; i++) {
		if (Messages[i].avi_info.name)
			free(Messages[i].avi_info.name);

		if (Messages[i].wave_info.name)
			free(Messages[i].wave_info.name);
	}

	Num_messages = m_num_messages + Num_builtin_messages;
	for (i=0; i<m_num_messages; i++)
		Messages[i + Num_builtin_messages] = m_messages[i];

	theApp.record_window_data(&Events_wnd_data, this);
	delete Event_editor_dlg;
	Event_editor_dlg = NULL;
}

// load controls with structure data
void event_editor::update_cur_message()
{
	int enable = TRUE;

	if (m_cur_msg < 0) {
		enable = FALSE;
		m_message_name = _T("");
		m_message_text = _T("");
		m_avi_filename = _T("");
		m_wave_filename = _T("");
		m_persona = 0;
		m_message_team = -1;
	} else {
		m_message_name = m_messages[m_cur_msg].name;
		m_message_text = m_messages[m_cur_msg].message;
		if (m_messages[m_cur_msg].avi_info.name){
			m_avi_filename = _T(m_messages[m_cur_msg].avi_info.name);
		} else {
			m_avi_filename = _T("<None>");
		}

		if (m_messages[m_cur_msg].wave_info.name){
			m_wave_filename = _T(m_messages[m_cur_msg].wave_info.name);
		} else {
			m_wave_filename = _T("<None>");
		}

		// add persona id
		if ( m_messages[m_cur_msg].persona_index != -1 ){
			m_persona = m_messages[m_cur_msg].persona_index + 1;  // add one for the "none" at the beginning of the list
		} else {
			m_persona = 0;
		}

		if(m_messages[m_cur_msg].multi_team >= 2){
			m_message_team = -1;
			m_messages[m_cur_msg].multi_team = -1;
		} else {
			m_message_team = m_messages[m_cur_msg].multi_team;
		}
/*
		m_event_num = find_event();
		if (m_event_num < 0) {
			node = -1;
			m_sender = m_priority = 0;

		} else
			node = CADR(Mission_events[m_event_num].formula);
*/	}

	GetDlgItem(IDC_MESSAGE_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_MESSAGE_TEXT)->EnableWindow(enable);
	GetDlgItem(IDC_AVI_FILENAME)->EnableWindow(enable);
	GetDlgItem(IDC_BROWSE_AVI)->EnableWindow(enable);
	GetDlgItem(IDC_BROWSE_WAVE)->EnableWindow(enable);
	GetDlgItem(IDC_WAVE_FILENAME)->EnableWindow(enable);
	GetDlgItem(IDC_DELETE_MSG)->EnableWindow(enable);
	GetDlgItem(IDC_PERSONA_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_MESSAGE_TEAM)->EnableWindow(enable);
	UpdateData(FALSE);
}

int event_editor::handler(int code, int node, char *str)
{
	int i;

	switch (code) {
		case ROOT_DELETED:
			for (i=0; i<m_num_events; i++)
				if (m_events[i].formula == node)
					break;

			Assert(i < m_num_events);
			while (i < m_num_events - 1) {
				m_events[i] = m_events[i + 1];
				m_sig[i] = m_sig[i + 1];
				i++;
			}

			m_num_events--;
			GetDlgItem(IDC_BUTTON_NEW_EVENT)->EnableWindow(TRUE);
			return node;

		case ROOT_RENAMED:
			for (i=0; i<m_num_events; i++)
				if (m_events[i].formula == node)
					break;

			Assert(i < m_num_events);
			Assert(strlen(str) < NAME_LENGTH);
			strcpy(m_events[i].name, str);
			return node;

		default:
			Int3();
	}

	return -1;
}

void event_editor::OnButtonNewEvent() 
{
	if (m_num_events == MAX_MISSION_EVENTS) {
		MessageBox("You have reached the limit on mission events.\n"
			"Can't add any more.");
		return;
	}

	reset_event(m_num_events++, TVI_LAST);
}

void event_editor::OnInsert() 
{
	int i;

	if (m_num_events == MAX_MISSION_EVENTS) {
		MessageBox("You have reached the limit on mission events.\n"
			"Can't add any more.");
		return;
	}

	for (i=m_num_events; i>cur_event; i--) {
		m_events[i] = m_events[i - 1];
		m_sig[i] = m_sig[i - 1];
	}

	if (cur_event){
		reset_event(cur_event, get_event_handle(cur_event - 1));
	} else {
		reset_event(cur_event, TVI_FIRST);
	}

	m_num_events++;
}

HTREEITEM event_editor::get_event_handle(int num)
{
	HTREEITEM h;

	h = m_event_tree.GetRootItem();
	while (h) {
		if ((int) m_event_tree.GetItemData(h) == m_events[num].formula){
			return h;
		}

		h = m_event_tree.GetNextSiblingItem(h);
	}

	return 0;
}

void event_editor::reset_event(int num, HTREEITEM after)
{
	int index;
	HTREEITEM h;

	strcpy(m_events[num].name, "Event name");
	h = m_event_tree.insert(m_events[num].name, BITMAP_ROOT, BITMAP_ROOT, TVI_ROOT, after);

	m_events[num].repeat_count = 1;
	m_events[num].interval = 1;
	m_events[num].score = 0;
	m_events[num].chain_delay = -1;
	m_events[num].objective_text = NULL;
	m_events[num].objective_key_text = NULL;
	m_sig[num] = -1;

	m_event_tree.item_index = -1;
	m_event_tree.add_operator("when", h);
	index = m_events[num].formula = m_event_tree.item_index;
	m_event_tree.SetItemData(h, index);
	m_event_tree.add_operator("true");
	m_event_tree.item_index = index;
	m_event_tree.add_operator("do-nothing");

	m_event_tree.SelectItem(h);
//	GetDlgItem(IDC_CHAIN_DELAY) -> EnableWindow(FALSE);
	if (num >= MAX_MISSION_EVENTS){
		GetDlgItem(IDC_BUTTON_NEW_EVENT)->EnableWindow(FALSE);
	}
}

void event_editor::OnDelete() 
{
	HTREEITEM h;

	// call update_cur_event to clean up local class variables so that we can correctly
	// set up the newly selected item.
	cur_event = -1;
	update_cur_event();

	h = m_event_tree.GetSelectedItem();
	if (h) {
		while (m_event_tree.GetParentItem(h))
			h = m_event_tree.GetParentItem(h);

		m_event_tree.setup_selected(h);
		m_event_tree.OnCommand(ID_DELETE, 0);
	}
}

// this is called when you hit the escape key..
void event_editor::OnCancel()
{
}

// this is called the clicking the ID_CANCEL button
void event_editor::On_Cancel()
{
	theApp.record_window_data(&Events_wnd_data, this);
	delete Event_editor_dlg;
	Event_editor_dlg = NULL;
}

void event_editor::OnClose() 
{
	int z;

	if (query_modified()) {
		z = MessageBox("Do you want to keep your changes?", "Close", MB_ICONQUESTION | MB_YESNOCANCEL);
		if (z == IDCANCEL){
			return;
		}

		if (z == IDYES) {
			OnOk();
			return;
		}
	}
	
	theApp.record_window_data(&Events_wnd_data, this);
	delete Event_editor_dlg;
	Event_editor_dlg = NULL;
}

void event_editor::insert_handler(int old, int node)
{
	int i;

	for (i=0; i<m_num_events; i++){
		if (m_events[i].formula == old){
			break;
		}
	}

	Assert(i < m_num_events);
	m_events[i].formula = node;
	return;
}

void event_editor::save()
{
	int m = m_cur_msg;

	save_event(cur_event);
	save_message(m);
}

void event_editor::save_event(int e)
{
	if (e < 0) {
		return;
	}

	UpdateData(TRUE);
	m_events[e].repeat_count = m_repeat_count;
	m_events[e].interval = m_interval;
	m_events[e].score = m_event_score;

	// handle chain
	if (m_chained) {
		m_events[e].chain_delay = m_chain_delay;
	} else {
		m_events[e].chain_delay = -1;
	}

	// handle objective text
	if (m_events[e].objective_text) {
		free(m_events[e].objective_text);
	}

	if (m_obj_text.IsEmpty()) {
		m_events[e].objective_text = NULL;
	} else {
		m_events[e].objective_text = strdup(m_obj_text);
	}

	// handle objective key text
	if (m_events[e].objective_key_text) {
		free(m_events[e].objective_key_text);
	}

	if (m_obj_key_text.IsEmpty()) {
		m_events[e].objective_key_text = NULL;
	} else {
		m_events[e].objective_key_text = strdup(m_obj_key_text);
	}

	// update bitmap
	int bitmap;
	if (m_chained) {
		if (m_obj_text.IsEmpty()) {
			bitmap = BITMAP_CHAIN;
		} else {
			bitmap = BITMAP_CHAIN_DIRECTIVE;
		}
	} else {
		// not chained
		if (m_obj_text.IsEmpty()) {
			bitmap = BITMAP_ROOT;
		} else {
			bitmap = BITMAP_ROOT_DIRECTIVE;
		}
	}

	// Search for item to update
	HTREEITEM h = m_event_tree.GetRootItem();
	while (h) {
		if ((int) m_event_tree.GetItemData(h) == m_events[e].formula) {
			m_event_tree.SetItemImage(h, bitmap, bitmap);
			return;
		}

		h = m_event_tree.GetNextSiblingItem(h);
	}

}

// this function deals with the left click on an event in the tree view.  We get into this
// function so that we may update the other data on the screen (i.e repeat count and interval
// count)
void event_editor::OnSelchangedEventTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i, z;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM h, h2;

	// before we do anything, we must check and save off any data from the current event (i.e.
	// the repeat count and interval count)
	save();
	h = pNMTreeView->itemNew.hItem;
	if (!h){
		return;
	}

	m_event_tree.update_help(h);
	while ((h2 = m_event_tree.GetParentItem(h))>0){
		h = h2;
	}

	z = m_event_tree.GetItemData(h);
	for (i=0; i<m_num_events; i++){
		if (m_events[i].formula == z){
			break;
		}
	}

	Assert(i < m_num_events);
	cur_event = i;
	update_cur_event();
	
	*pResult = 0;
}

void event_editor::update_cur_event()
{
	if (cur_event < 0) {
		m_repeat_count = 1;
		m_interval = 1;
		m_chain_delay = 0;
		m_team = -1;
		m_obj_text.Empty();
		m_obj_key_text.Empty();
		GetDlgItem(IDC_INTERVAL_TIME) -> EnableWindow(FALSE);
		GetDlgItem(IDC_REPEAT_COUNT) -> EnableWindow(FALSE);
		GetDlgItem(IDC_EVENT_SCORE) -> EnableWindow(FALSE);
		GetDlgItem(IDC_CHAINED) -> EnableWindow(FALSE);
		GetDlgItem(IDC_CHAIN_DELAY) -> EnableWindow(FALSE);
		GetDlgItem(IDC_OBJ_TEXT) -> EnableWindow(FALSE);
		GetDlgItem(IDC_OBJ_KEY_TEXT) -> EnableWindow(FALSE);
		GetDlgItem(IDC_EVENT_TEAM)->EnableWindow(FALSE);
		return;
	}

	m_team = m_events[cur_event].team;

	m_repeat_count = m_events[cur_event].repeat_count;
	m_interval = m_events[cur_event].interval;
	m_event_score = m_events[cur_event].score;
	if (m_events[cur_event].chain_delay >= 0) {
		m_chained = TRUE;
		m_chain_delay = m_events[cur_event].chain_delay;
		GetDlgItem(IDC_CHAIN_DELAY) -> EnableWindow(TRUE);

	} else {
		m_chained = FALSE;
		m_chain_delay = 0;
		GetDlgItem(IDC_CHAIN_DELAY) -> EnableWindow(FALSE);
	}

	if (m_events[cur_event].objective_text){
		m_obj_text = m_events[cur_event].objective_text;
	} else {
		m_obj_text.Empty();
	}

	if (m_events[cur_event].objective_key_text){
		m_obj_key_text = m_events[cur_event].objective_key_text;
	} else {
		m_obj_key_text.Empty();
	}

	GetDlgItem(IDC_REPEAT_COUNT)->EnableWindow(TRUE);
	if ( m_repeat_count <= 1 ) {
		m_interval = 1;
		GetDlgItem(IDC_INTERVAL_TIME) -> EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_INTERVAL_TIME) -> EnableWindow(TRUE);
	}

	GetDlgItem(IDC_EVENT_SCORE) -> EnableWindow(TRUE);
	GetDlgItem(IDC_CHAINED) -> EnableWindow(TRUE);
	GetDlgItem(IDC_OBJ_TEXT) -> EnableWindow(TRUE);
	GetDlgItem(IDC_OBJ_KEY_TEXT) -> EnableWindow(TRUE);
	GetDlgItem(IDC_EVENT_TEAM)->EnableWindow(FALSE);
	if ( The_mission.game_type & MISSION_TYPE_MULTI_TEAMS ){
		GetDlgItem(IDC_EVENT_TEAM)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void event_editor::OnUpdateRepeatCount()
{
	char buf[128];
	int count;

	count = 128;
	GetDlgItem(IDC_REPEAT_COUNT)->GetWindowText(buf, count);
	m_repeat_count = atoi(buf);

	if ( m_repeat_count <= 1 ){
		GetDlgItem(IDC_INTERVAL_TIME)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_INTERVAL_TIME)->EnableWindow(TRUE);
	}
}

void event_editor::swap_handler(int node1, int node2)
{
	int index1, index2;
	mission_event m;

	save();
	for (index1=0; index1<m_num_events; index1++){
		if (m_events[index1].formula == node1){
			break;
		}
	}

	Assert(index1 < m_num_events);
	for (index2=0; index2<m_num_events; index2++){
		if (m_events[index2].formula == node2){
			break;
		}
	}

	Assert(index2 < m_num_events);
	m = m_events[index1];
//	m_events[index1] = m_events[index2];
	while (index1 < index2) {
		m_events[index1] = m_events[index1 + 1];
		m_sig[index1] = m_sig[index1 + 1];
		index1++;
	}

	while (index1 > index2 + 1) {
		m_events[index1] = m_events[index1 - 1];
		m_sig[index1] = m_sig[index1 - 1];
		index1--;
	}

	m_events[index1] = m;
	cur_event = index1;
	update_cur_event();
}

void event_editor::OnChained() 
{
	int image;
	HTREEITEM h;

	UpdateData(TRUE);
	if (m_chained) {
		GetDlgItem(IDC_CHAIN_DELAY) -> EnableWindow(TRUE);
		if (m_obj_text.IsEmpty()) {
			image = BITMAP_CHAIN;
		} else {
			image = BITMAP_CHAIN_DIRECTIVE;
		}
	} else {
		GetDlgItem(IDC_CHAIN_DELAY) -> EnableWindow(FALSE);
		if (m_obj_text.IsEmpty()) {
			image = BITMAP_ROOT;
		} else {
			image = BITMAP_ROOT_DIRECTIVE;
		}
	}

	h = m_event_tree.GetRootItem();
	while (h) {
		if ((int) m_event_tree.GetItemData(h) == m_events[cur_event].formula) {
			m_event_tree.SetItemImage(h, image, image);
			return;
		}

		h = m_event_tree.GetNextSiblingItem(h);
	}
}

void event_editor::OnSelchangeMessageList() 
{	
	static flag = 0;

	if (flag)
		return;
/*
	if (save_message(m_cur_msg)) {
		flag = 1;
		((CListBox *) GetDlgItem(IDC_MESSAGE_LIST)) -> SetCurSel(old);
		m_cur_msg = old;
		flag = 0;
		return;
	}*/

	save();
	update_cur_message();
}

int event_editor::save_message(int num)
{
	char *ptr;
	int i, conflict = 0;
	CListBox *list;

	UpdateData(TRUE);
	if (num >= 0) {
		ptr = (char *) (LPCTSTR) m_message_name;
		for (i=0; i<Num_builtin_messages; i++){
			if (!stricmp(m_message_name, Messages[i].name)) {
				conflict = 1;
				break;
			}
		}

		for (i=0; i<m_num_messages; i++){
			if ((i != num) && (!stricmp(m_message_name, m_messages[i].name))) {
				conflict = 1;
				break;
			}
		}

		if (!conflict) {  // update name if no conflicts, otherwise keep old name
			string_copy(m_messages[num].name, m_message_name, NAME_LENGTH - 1);

			list = (CListBox *) GetDlgItem(IDC_MESSAGE_LIST);
			list->DeleteString(num);
			list->InsertString(num, m_message_name);
		}

		string_copy(m_messages[num].message, m_message_text, MESSAGE_LENGTH - 1);
		if (m_messages[num].avi_info.name){
			free(m_messages[num].avi_info.name);
		}

		ptr = (char *) (LPCTSTR) m_avi_filename;
		if (!ptr || !strlen(ptr) || !stricmp(ptr, "none") || !stricmp(ptr, "<none>")){
			m_messages[num].avi_info.name = NULL;
		} else {
			m_messages[num].avi_info.name = strdup(ptr);
		}

		if (m_messages[num].wave_info.name){
			free(m_messages[num].wave_info.name);
		}

		ptr = (char *) (LPCTSTR) m_wave_filename;
		if (!ptr || !strlen(ptr) || !stricmp(ptr, "none") || !stricmp(ptr, "<none>")){
			m_messages[num].wave_info.name = NULL;
		} else {
			m_messages[num].wave_info.name = strdup(ptr);
		}

		// update the persona to the message.  We subtract 1 for the "None" at the beginning of the combo
		// box list.
		m_messages[num].persona_index = m_persona - 1;

		if(m_message_team >= 2){
			m_messages[num].multi_team = -1;
			m_message_team = -1;
		} else {
			m_messages[num].multi_team = m_message_team;
		}

		// possible TODO: auto-update event tree references to this message if we renamed it.
	}

	return 0;
}

void event_editor::OnNewMsg() 
{
//	if (save_message(m_cur_msg))
//		return;

	save();
	Assert(m_num_messages + Num_builtin_messages < MAX_MISSION_MESSAGES);
	strcpy(m_messages[m_num_messages].name, "<new message>");
	((CListBox *) GetDlgItem(IDC_MESSAGE_LIST))->AddString("<new message>");

	strcpy(m_messages[m_num_messages].message, "<put description here>");
	m_messages[m_num_messages].avi_info.name = NULL;
	m_messages[m_num_messages].wave_info.name = NULL;
	m_messages[m_num_messages].persona_index = -1;
	m_messages[m_num_messages].multi_team = -1;
	m_cur_msg = m_num_messages++;
	if (m_num_messages + Num_builtin_messages >= MAX_MISSION_MESSAGES){
		GetDlgItem(IDC_NEW_MSG)->EnableWindow(FALSE);
	}

	modified = 1;
	update_cur_message();
}

void event_editor::OnDeleteMsg() 
{
	char buf[256];
	int i;

	// handle this case somewhat gracefully
	Assert((m_cur_msg >= 0) && (m_cur_msg < m_num_messages));
	if((m_cur_msg < 0) || (m_cur_msg >= m_num_messages)){
		return;
	}
	
	if (m_messages[m_cur_msg].avi_info.name){
		free(m_messages[m_cur_msg].avi_info.name);
	}
	if (m_messages[m_cur_msg].wave_info.name){
		free(m_messages[m_cur_msg].wave_info.name);
	}

	((CListBox *) GetDlgItem(IDC_MESSAGE_LIST))->DeleteString(m_cur_msg);
	sprintf(buf, "<%s>", m_messages[m_cur_msg].name);
	update_sexp_references(m_messages[m_cur_msg].name, buf, OPF_MESSAGE);

	for (i=m_cur_msg; i<m_num_messages-1; i++){
		m_messages[i] = m_messages[i + 1];
	}

	m_num_messages--;
	if (m_cur_msg >= m_num_messages){
		m_cur_msg = m_num_messages - 1;
	}

	GetDlgItem(IDC_NEW_MSG)->EnableWindow(TRUE);
	modified = 1;
	update_cur_message();
}

void event_editor::OnBrowseAvi() 
{
	int z;
	CString name;	

	UpdateData(TRUE);
	if (!stricmp(m_avi_filename, "<None>"))
		m_avi_filename = _T("");

	z = cfile_push_chdir(CF_TYPE_INTERFACE);
	CFileDialog dlg(TRUE, "ani", m_avi_filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
		"Ani Files (*.ani)|*.ani|Avi Files (*.avi)|*.avi|Both (*.ani, *.avi)|*.ani;*.avi||");

	if (dlg.DoModal() == IDOK) {
		m_avi_filename = dlg.GetFileName();
		UpdateData(FALSE);
		modified = 1;
	}

	if (!z)
		cfile_pop_dir();
}

void event_editor::OnBrowseWave() 
{
	int z;
	CString name;

	UpdateData(TRUE);
	if (!stricmp(m_wave_filename, "<None>"))
		m_wave_filename = _T("");

	if (The_mission.game_type & MISSION_TYPE_TRAINING)
		z = cfile_push_chdir(CF_TYPE_VOICE_TRAINING);
	else
		z = cfile_push_chdir(CF_TYPE_VOICE_SPECIAL);

	CFileDialog dlg(TRUE, "wav", m_wave_filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
		"Wave Files (*.wav)|*.wav||");

	if (dlg.DoModal() == IDOK) {
		m_wave_filename = dlg.GetFileName();
		update_persona();
	}

	if (!z){
		cfile_pop_dir();
	}
}

char *event_editor::current_message_name(int i)
{
	if ( (i < 0) || (i >= m_num_messages) ){
		return NULL;
	}

	return m_messages[i].name;
}

char *event_editor::get_message_list_item(int i)
{
	return m_messages[i].name;
}

void event_editor::update_persona()
{
	int i, mask;

	if ((m_wave_filename[0] >= '1') && (m_wave_filename[0] <= '9') && (m_wave_filename[1] == '_') ) {
		i = m_wave_filename[0] - '1';
		if ( (i < Num_personas) && (Personas[i].flags & PERSONA_FLAG_WINGMAN) )	{
			m_persona = i + 1;
			if ((m_persona==1) || (m_persona==2)) 
				m_avi_filename = "HEAD-TP1";
			else if ((m_persona==3) || (m_persona==4))
				m_avi_filename = "HEAD-TP2";
			else if ((m_persona==5))
				m_avi_filename = "HEAD-TP3";
			else if ((m_persona==6))
				m_avi_filename = "HEAD-VP1";
		}
	} else {
		mask = 0;
		if (!strnicmp(m_wave_filename, "S_", 2)) {
			mask = PERSONA_FLAG_SUPPORT;
			m_avi_filename = "HEAD-CM1";
		}
		else if (!strnicmp(m_wave_filename, "L_", 2)) {
			mask = PERSONA_FLAG_LARGE;
			m_avi_filename = "HEAD-CM1";
		}
		else if (!strnicmp(m_wave_filename, "TC_", 3)) {
			mask = PERSONA_FLAG_COMMAND;
			m_avi_filename = "HEAD-CM1";
		}

		for (i=0; i<Num_personas; i++)
			if (Personas[i].flags & mask)
				m_persona = i + 1;
	}
	//GetDlgItem(IDC_ANI_FILENAME)->SetWindowText(m_avi_filename);
	UpdateData(FALSE);
	modified = 1;
}

void event_editor::OnSelchangeWaveFilename() 
{
	int z;
	CComboBox *box;

	box = (CComboBox *) GetDlgItem(IDC_WAVE_FILENAME);
	z = box -> GetCurSel();
	UpdateData(TRUE);
	UpdateData(TRUE);

	box -> GetLBText(z, m_wave_filename);
	UpdateData(FALSE);
	update_persona();
}

BOOL event_editor::DestroyWindow() 
{
	m_play_bm.DeleteObject();
	return CDialog::DestroyWindow();
}

void event_editor::OnPlay() 
{
	char path[MAX_PATH_LEN + 1];
	GetDlgItem(IDC_WAVE_FILENAME)->GetWindowText(m_wave_filename);

	int size, offset;
	cf_find_file_location((char *)(LPCSTR)m_wave_filename, CF_TYPE_ANY, path, &size, &offset );

	PlaySound(path, NULL, SND_ASYNC | SND_FILENAME);
}

void event_editor::OnUpdate() 
{
//	GetDlgItem(IDC_WAVE_FILENAME)->GetWindowText(m_wave_filename);
	UpdateData(TRUE);
	update_persona();
}

// code when the "team" selection in the combo box changes
void event_editor::OnSelchangeTeam() 
{
	if ( cur_event < 0 ){
		return;
	}

	UpdateData(TRUE);

	// team == 2, means no team
	if(m_team == 2){
		m_team = -1;
	}

	m_events[cur_event].team = m_team;
}

// code when the "team" selection in the combo box changes
void event_editor::OnSelchangeMessageTeam() 
{
	if ( m_cur_msg < 0 ){
		return;
	}

	UpdateData(TRUE);

	// team == 2, means no team
	if(m_message_team == 2){
		m_message_team = -1;
	}

	m_messages[m_cur_msg].multi_team = m_message_team;
}

// Cycles among sexp nodes with message text
void event_editor::OnDblclkMessageList() 
{
	CListBox *list = (CListBox*) GetDlgItem(IDC_MESSAGE_LIST);
	int num_messages;
	int message_nodes[MAX_SEARCH_MESSAGE_DEPTH];

	// get current message index and message name
	int cur_index = list->GetCurSel();

	// check if message name is in event tree
	char buffer[256];
	list->GetText(cur_index, buffer);


	num_messages = m_event_tree.find_text(buffer, message_nodes);

	if (num_messages == 0) {
		char message[256];
		sprintf(message, "No events using message '%s'", buffer);
		MessageBox(message);
	} else {
		// find last message_node
		if (m_last_message_node == -1) {
			m_last_message_node = message_nodes[0];
		} else {

			if (num_messages == 1) {
				// only 1 message
				m_last_message_node = message_nodes[0];
			} else {
				// find which message and go to next message
				int found_pos = -1;
				for (int i=0; i<num_messages; i++) {
					if (message_nodes[i] == m_last_message_node) {
						found_pos = i;
						break;
					}
				}

				if (found_pos == -1) {
					// no previous message
					m_last_message_node = message_nodes[0];
				} else if (found_pos == num_messages-1) {
					// cycle back to start
					m_last_message_node = message_nodes[0];
				} else {
					// go to next
					m_last_message_node = message_nodes[found_pos+1];
				}
			}
		}

		// highlight next
		m_event_tree.hilite_item(m_last_message_node);
	}
}

