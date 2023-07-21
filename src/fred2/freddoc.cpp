/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Fred2/FREDDoc.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * FREDDoc.cpp : implementation of the CFREDDoc class
 * Document class for document/view architechure, which we don't really use in
 * Fred, but MFC forces you do use like it or not.  Handles loading/saving
 * mainly.  Most of the MFC related stuff is handled in FredView.
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
 * 7     7/23/99 2:12p Jamesa
 * fix gamepalette request
 * 
 * 6     5/20/99 6:59p Dave
 * Added alternate type names for ships. Changed swarm missile table
 * entries.
 * 
 * 5     10/29/98 6:49p Dave
 * Finished up Fred support for externalizing mission and campaign files.
 * 
 * 4     10/29/98 12:50p Dave
 * Intermediate checkin for fred hash table stuff.
 * 
 * 3     10/28/98 11:30a Dave
 * Temporary checkin.
 * 
 * 2     10/07/98 6:28p Dave
 * Initial checkin. Renamed all relevant stuff to be Fred2 instead of
 * Fred. Globalized mission and campaign file extensions. Removed Silent
 * Threat specific code.
 * 
 * 1     10/07/98 3:00p Dave
 * 
 * 94    9/21/98 8:46p Dave
 * Put in special check in fred for identifying unknown ships.
 * 
 * 93    9/16/98 6:54p Dave
 * Upped  max sexpression nodes to 1800 (from 1600). Changed FRED to sort
 * the ship list box. Added code so that tracker stats are not stored with
 * only 1 player.
 * 
 * 92    8/13/98 9:53a Hoffoss
 * Fixed bug where loading mission doesn't update palette used for that
 * mission.
 * 
 * 91    4/30/98 8:23p John
 * Fixed some bugs with Fred caused by my new cfile code.
 * 
 * 90    4/28/98 2:13p Hoffoss
 * Added code to help keep invalid player ship types from existing in
 * mission.
 * 
 * 89    4/10/98 10:22p Mike
 * Fix bug in parsing medals.tbl.
 * 
 * 88    4/10/98 4:51p Hoffoss
 * Made several changes related to tooltips.
 * 
 * 87    3/25/98 4:14p Hoffoss
 * Split ship editor up into ship editor and a misc dialog, which tracks
 * flags and such.
 * 
 * 86    3/10/98 4:26p Hoffoss
 * Changed jump node structure to include a name.  Position is now taken
 * from the object (each jump node has an associated object now).
 * 
 * 85    2/26/98 4:59p Allender
 * groundwork for team vs team briefings.  Moved weaponry pool into the
 * Team_data structure.  Added team field into the p_info structure.
 * Allow for mutliple structures in the briefing code.
 * 
 * 84    1/28/98 11:00a Johnson
 * Fixed parse code for medals table (new field)
 * 
 * 83    11/06/97 4:37p Allender
 * a ton of medal work.  Removed an uneeded element in the scoring
 * structure.  Fix up medals screen to apprioriate display medals (after
 * mask was changed). Fix Fred to only display medals which may actually
 * be granted.  Added image_filename to player struct for Jason Hoffoss
 * 
 * 82    11/05/97 10:19p Mike
 * Comment out call to parse_medal_tbl() to allow Fred to run.
 * 
 * 81    11/05/97 4:43p Allender
 * reworked medal/rank system to read all data from tables.  Made Fred
 * read medals.tbl.  Changed ai-warp to ai-warp-out which doesn't require
 * waypoint for activation
 * 
 * 80    11/04/97 4:50p Hoffoss
 * Made mission loading failure display an error message stating so.
 * 
 * 79    11/04/97 4:33p Hoffoss
 * Made saving keep the current briefing state intact.
 * 
 * 78    10/20/97 1:20p Hoffoss
 * Fixed bug loading mission file after a save.
 * 
 * 77    10/19/97 11:36p Hoffoss
 * Made mission saving call reload the mission after saving it to verify
 * it was written error free.
 * 
 * 76    10/08/97 11:47a Hoffoss
 * Added better fred handling of Weaponry_pool.
 * 
 * 75    9/20/97 8:16a John
 * Made .clr files go into the Cache directory. Replaced cfopen(name,NULL)
 * to delete a file with cf_delete.
 * 
 * 74    9/18/97 11:52a Johnson
 * Fixed bug with player starts count being incorrect.
 * 
 * 73    9/17/97 11:58a Hoffoss
 * Fixed mission load code to properly set arrival cues to player ships.
 * 
 * 72    9/17/97 10:32a Allender
 * fixup references to old "Player" ship to new real shipname
 * 
 * 71    9/16/97 9:41p Hoffoss
 * Changed Fred code around to stop using Parse_player structure for
 * player information, and use actual ships instead.
 * 
 * 70    9/09/97 6:50p Hoffoss
 * Fixed bug with mission saving.
 * 
 * 69    9/09/97 1:55p Hoffoss
 * Fixed bug with saving missions that are readonly.
 * 
 * 68    8/21/97 1:49p Hoffoss
 * Fixed bugs with loading files.  Wasn't clearing the last mission first,
 * and multiplayer starts in a mission caused it to think it was modified
 * right away.
 * 
 * 67    8/20/97 5:45p Hoffoss
 * Fixed bugs with mission name in sexp in non campaign mode in Fred.
 * 
 * 66    8/20/97 4:03p Hoffoss
 * Fixed bug in reload current mission (revert).
 * 
 * 65    8/17/97 10:22p Hoffoss
 * Fixed several bugs in Fred with Undo feature.  In the process, recoded
 * a lot of CFile.cpp.
 * 
 * 64    8/16/97 9:24p Hoffoss
 * Added support for team of players in multiplayer.
 * 
 * 63    8/16/97 6:44p Hoffoss
 * Changes to allow any player to be in a wing.
 * 
 * 62    8/16/97 3:53p Hoffoss
 * Added OF_NO_SHIELDS define and support in Fred and mission load/save.
 * 
 * 61    8/13/97 5:49p Hoffoss
 * Fixed bugs, made additions.
 * 
 * 60    8/13/97 12:46p Hoffoss
 * Added campaign error checker, accelerator table, and mission goal data
 * listings to sexp tree right click menu.
 * 
 * 59    8/12/97 1:55a Hoffoss
 * Made extensive changes to object reference checking and handling for
 * object deletion call.
 * 
 * 58    7/31/97 5:55p John
 * made so you pass flags to obj_create.
 * Added new collision code that ignores any pairs that will never
 * collide.
 * 
 * 57    7/30/97 2:10p Hoffoss
 * Made new missions that have never been saved yet save backups.
 * 
 * 56    7/28/97 3:50p Hoffoss
 * Added global error checker call to mission load and save.
 * 
 * 55    7/10/97 2:32p Hoffoss
 * Made message editor dialog box modeless.
 * 
 * 54    7/08/97 11:35a Hoffoss
 * Fixed bug in initial orders, and also redid how shields are handled by
 * Fred.
 * 
 * 53    6/26/97 5:18p Hoffoss
 * Major rework of briefing editor functionality.
 * 
 * 52    6/24/97 10:22a Hoffoss
 * Added a forced update for briefing info prior to a mission save.
 * 
 * 51    6/19/97 11:34a Hoffoss
 * Fixed bug in auto mission convertion.
 * 
 * 50    6/18/97 3:25p Hoffoss
 * Added autoconverting of old ship names (for ships in wings) at mission
 * load time, and added an hourglass cursor change when autosaving.
 * 
 * 49    6/17/97 11:07a Hoffoss
 * Fixed a few bugs: revert doesn't reset mission first, and sexp tree
 * copy doesn't act act as a single chain.
 * 
 * 48    6/11/97 10:02a Comet
 * fixed a bug.
 * 
 * 47    6/09/97 4:57p Hoffoss
 * Added autosave and undo to Fred.
 * 
 * 46    6/05/97 6:10p Hoffoss
 * Added features: Autosaving, object hiding.  Also fixed some minor bugs.
 * 
 * 45    5/23/97 3:18p Hoffoss
 * Event editor made modeless.
 * 
 * 44    5/21/97 5:42p Hoffoss
 * Added features requested on Todo list.
 * 
 * 43    5/01/97 4:14p Hoffoss
 * Viewer position saved to missions now for Fred restoration of state.
 * 
 * 42    4/25/97 3:53p Hoffoss
 * Fixed bug in weapons save/load and moved weapon look up function to
 * weapons.cpp.
 * 
 * 41    4/25/97 3:28p Mike
 * Making shield multi-part.
 * 
 * 40    4/21/97 5:02p Hoffoss
 * Player/player status editing supported, and both saved and loaded from
 * Mission files.
 * 
 * 39    3/20/97 3:55p Hoffoss
 * Major changes to how dialog boxes initialize (load) and update (save)
 * their internal data.  This should simplify things and create less
 * problems.
 * 
 * 38    3/04/97 6:27p Hoffoss
 * Changes to Fred to handle new wing structure.
 * 
 * 37    2/28/97 11:31a Hoffoss
 * Implemented modeless dialog saving and restoring, and changed some
 * variables names.
 * 
 * 36    2/27/97 3:09p Allender
 * major wing structure enhancement.  simplified wing code.  All around
 * better wing support
 * 
 * 35    2/20/97 4:03p Hoffoss
 * Several ToDo items: new reinforcement clears arrival cue, reinforcement
 * control from ship and wing dialogs, show grid toggle.
 * 
 * 34    2/17/97 5:28p Hoffoss
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

#include "stdafx.h"
#include "fred.h"
#include <stdlib.h>

#include "freddoc.h"
#include "fredview.h"
#include "prefsdlg.h"

#include "3d.h"
#include "object.h"
#include "editor.h"
#include "ai.h"
#include "ailocal.h"
#include "cfile.h"
#include "ship.h"
#include "missionparse.h"
#include "missiongoals.h"
#include "missionsave.h"
#include "weapon.h"
#include "management.h"
#include "linklist.h"
#include "fredrender.h"
#include "mainfrm.h"
#include "eventeditor.h"
#include "aigoals.h"
#include "messageeditordlg.h"
#include "palman.h"
#include "fhash.h"

extern int num_objects;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//	In editor mode, use class CFile, in game, use CFILE (our file)
#define	XFILE CFile

/////////////////////////////////////////////////////////////////////////////
// CFREDDoc

IMPLEMENT_DYNCREATE(CFREDDoc, CDocument)

BEGIN_MESSAGE_MAP(CFREDDoc, CDocument)
	//{{AFX_MSG_MAP(CFREDDoc)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_DUPLICATE, OnDuplicate)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_HOLD, OnEditHold)
	ON_COMMAND(ID_EDIT_FETCH, OnEditFetch)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_FILE_PREFERENCES, OnFilePreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFREDDoc construction/destruction

//	Global pointer to the FREDDoc class.
//	Used by MK to, among other things, I hope, update the modified flag from
//	outside the FREDDoc class.
CFREDDoc *FREDDoc_ptr = NULL;
int Local_modified = 0;
int Undo_available = 0;
int Undo_count = 0;

extern int Fred_found_unknown_ship_during_parsing;

CFREDDoc::CFREDDoc()
{
	int i;

	FREDDoc_ptr = this;
	confirm_deleting = TRUE;
	show_capital_ships = TRUE;
	show_elevations = TRUE;
	show_fighters = TRUE;
	show_grid = TRUE;
	show_misc_objects = TRUE;
	show_planets = TRUE;
	show_waypoints = TRUE;
	show_starfield = TRUE;

	for (i=0; i<BACKUP_DEPTH; i++)
		undo_desc[i].Empty();
}

CFREDDoc::~CFREDDoc()
{
}

BOOL CFREDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	confirm_deleting = TRUE;
	show_capital_ships = FALSE;
	show_elevations = TRUE;
	show_fighters = TRUE;
	show_grid = FALSE;
	show_misc_objects = TRUE;
	show_planets = FALSE;
	show_waypoints = TRUE;
	show_starfield = FALSE;
	// (SDI documents will reuse this document)

	return TRUE;
}

// read in a new mission file from disk
BOOL CFREDDoc::OnOpenDocument(LPCTSTR pathname)
{
	char name[1024];
	int i, len;

	if (pathname)
		strcpy(mission_pathname, pathname);

	if (Briefing_dialog)
		Briefing_dialog->icon_select(-1);  // clean things up first

	len = strlen(mission_pathname);
	strcpy(name, mission_pathname);
	if (name[len - 4] == '.')
		len -= 4;

	name[len] = 0;  // drop extension
	i = len;
	while (i--)
		if ((name[i] == '\\') || (name[i] == ':'))
			break;

	strcpy(Mission_filename, name + i + 1);
//	for (i=1; i<=BACKUP_DEPTH; i++) {
//		sprintf(name + len, ".%.3d", i);
//		unlink(name);
//	}

	if (load_mission(mission_pathname)) {
		*Mission_filename = 0;
		return FALSE;
	}

	Fred_view_wnd->global_error_check();
	autosave("nothing");
	Undo_count = 0;
	return TRUE;
}

// save mission to a file
BOOL CFREDDoc::OnSaveDocument(LPCTSTR pathname)
{
	CFred_mission_save save;
	char name[1024];
	int len;
	DWORD attrib;
	FILE *fp;

	len = strlen(pathname);
	strcpy(name, pathname);
	if (name[len - 4] == '.')
		len -= 4;

	name[len] = 0;  // drop extension
	while (len--)
		if ((name[len] == '\\') || (name[len] == ':'))
			break;

	strcpy(Mission_filename, name + len + 1);
	Fred_view_wnd->global_error_check();
	if (Briefing_dialog) {
		Briefing_dialog->update_data(1);
		Briefing_dialog->save_editor_state();
	}
	
	if (Event_editor_dlg)
		Fred_main_wnd->MessageBox("Event editor dialog is still open, so changes there won't be saved");

	if (Message_editor_dlg)
		Fred_main_wnd->MessageBox("Message editor dialog is still open, so changes there won't be saved");

	fp = fopen(pathname, "r");
	if (fp) {
		fclose(fp);
		attrib = GetFileAttributes(pathname);
		if (attrib & FILE_ATTRIBUTE_READONLY) {
			Fred_main_wnd->MessageBox("File is read-only.  You need to check it out before saving to it");
			return FALSE;
		}
	}	

	if (save.save_mission_file((char *) pathname)) {
		Fred_main_wnd->MessageBox("An error occured while saving!", NULL, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	SetModifiedFlag(FALSE);
	if (load_mission((char *) pathname))
		Error(LOCATION, "Failed attempting to reload mission after saving.  Report this bug now!");

	if (Briefing_dialog) {
		Briefing_dialog->restore_editor_state();
		Briefing_dialog->update_data(1);
	}

	return TRUE;
//	return CDocument::OnSaveDocument(pathname);
}

int CFREDDoc::check_undo()
{
	char name[256];
	FILE *fp;

	Undo_available = 0;
	if (!Undo_count)
		return 0;

	strcpy(name, MISSION_BACKUP_NAME);
	strcat(name, ".002");
	fp = fopen(name, "r");
	if (!fp)
		return 0;

	fclose(fp);
	Undo_available = 1;
	return 1;
}

int CFREDDoc::autosave(char *desc)
{
	int i;
	CFred_mission_save save;
	CWaitCursor wait;

	if (Autosave_disabled)
		return 0;

	if (Briefing_dialog)
		Briefing_dialog->update_data(1);
	
	if (save.autosave_mission_file(MISSION_BACKUP_NAME)) {
		Undo_count = Undo_available = 0;
		return -1;
	}

	for (i=BACKUP_DEPTH; i>1; i--)
		undo_desc[i] = undo_desc[i - 1];

	if (desc)
		undo_desc[1] = desc;
	else
		undo_desc[1].Empty();

	Undo_count++;
	check_undo();
	return 0;
}

int CFREDDoc::autoload()
{
	char name[256], backup_name[256];
	int i, r, len;
	FILE *fp;

	strcpy(name, MISSION_BACKUP_NAME);
	strcat(name, ".002");
	fp = fopen(name, "r");
	if (!fp)
		return 0;

	fclose(fp);
	if (Briefing_dialog)
		Briefing_dialog->icon_select(-1);  // clean things up first

//	editor_init_mission();  
	r = load_mission(name);
	Update_window = 1;

	strcpy(backup_name, MISSION_BACKUP_NAME);
	len = strlen(backup_name);
	strcat(backup_name, ".001");
	cf_delete(backup_name, CF_TYPE_MISSIONS);

	for (i=1; i<BACKUP_DEPTH; i++) {
		sprintf(backup_name + len, ".%.3d", i + 1);
		sprintf(name + len, ".%.3d", i);
		cf_rename(backup_name, name, CF_TYPE_MISSIONS);
		undo_desc[i] = undo_desc[i + 1];
	}

	Undo_count--;
	check_undo();
	return r;
}

// read in a new mission file from disk
int CFREDDoc::load_mission(char *pathname)
{
	char name[512], *old_name;
	int i, j, k, ob;
	int used_pool[MAX_WEAPON_TYPES];
	waypoint_list *wptr;
	object *objp;

	Parse_viewer_pos = view_pos;
	Parse_viewer_orient = view_orient;

	// activate the localizer hash table
	fhash_flush();	

	clear_mission();
	Fred_found_unknown_ship_during_parsing = 0;
	if (parse_main(pathname)) {
		sprintf(name, "Unable to load the file \"%s\"", pathname);
		Fred_view_wnd->MessageBox(name);
		create_new_mission();		
		return -1;
	}

	if(Fred_found_unknown_ship_during_parsing){
		Fred_view_wnd->MessageBox("Fred encountered unknown ship/weapon classes when parsing the mission file. This may be due to mission disk data you do not have");
	}
	Fred_found_unknown_ship_during_parsing = 0;

	for (i=0; i<Num_waypoint_lists; i++) {
		wptr = &Waypoint_lists[i];
		for (j=0; j<wptr->count; j++){
			ob = obj_create(OBJ_WAYPOINT, -1, i * 65536 + j, NULL, &wptr->waypoints[j], 0.0f, OF_RENDERS);
		}
	}

	obj_merge_created_list();
	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if (objp->flags & OF_PLAYER_SHIP) {
			Assert(objp->type == OBJ_SHIP);
			objp->type = OBJ_START;
//			Player_starts++;
		}

		objp = GET_NEXT(objp);
	}

	for (i=0; i<num_wings; i++) {
		for (j=0; j<Wings[i].wave_count; j++) {
			ob = Ships[Wings[i].ship_index[j]].objnum;
			wing_objects[i][j] = ob;
			Ships[Wings[i].ship_index[j]].wingnum = i;
			Ships[Wings[i].ship_index[j]].arrival_cue = Locked_sexp_false;
		}

		// fix old ship names for ships in wings if needed
		while (j--) {
			if ( (Objects[wing_objects[i][j]].type == OBJ_SHIP) || (Objects[wing_objects[i][j]].type == OBJ_START) ) {  // don't change player ship names
				sprintf(name, "%s %d", Wings[i].name, j + 1);
				old_name = Ships[Wings[i].ship_index[j]].ship_name;
				if (stricmp(name, old_name)) {  // need to fix name
					update_sexp_references(old_name, name);
					ai_update_goal_references(REF_TYPE_SHIP, old_name, name);
					for (k=0; k<Num_reinforcements; k++)
						if (!stricmp(old_name, Reinforcements[k].name)) {
							Assert(strlen(name) < NAME_LENGTH);
							strcpy(Reinforcements[k].name, name);
						}

					strcpy(Ships[Wings[i].ship_index[j]].ship_name, name);
				}
			}
		}
	}

	generate_weaponry_usage_list(used_pool);
	for ( j = 0; j < Num_teams; j++ ) {
		for (i=0; i<Num_weapon_types; i++) {
			Team_data[j].weaponry_pool[i] -= used_pool[i];  // convert weaponry_pool to be extras available beyond the current ships weapons
			if (Team_data[j].weaponry_pool[i] < 0)
				Team_data[j].weaponry_pool[i] = 0;
		}
	}

	Assert(Mission_palette >= 0);
	Assert(Mission_palette <= 98);

	if (The_mission.flags & MISSION_FLAG_SUBSPACE) {
		strcpy(name, NOX("gamepalette-subspace"));
	} else {
		strcpy(name, "gamepalette1-01");
		// sprintf(name, NOX("gamepalette1-%02d"), Mission_palette + 1);
	}

	palette_load_table(name);

	// go through all ships and translate their alternate name indices	
	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		// if this is a ship, check it, and mark its possible alternate name down in the auxiliary array
		if(((objp->type == OBJ_SHIP) || (objp->type == OBJ_START)) && (objp->instance >= 0) && (Ships[objp->instance].alt_type_index >= 0)){
			mission_parse_lookup_alt_index(Ships[objp->instance].alt_type_index, Fred_alt_names[objp->instance]);

			// also zero it
			Ships[objp->instance].alt_type_index = -1;
		}

		objp = GET_NEXT(objp);
	}


	view_pos = Parse_viewer_pos;
	view_orient = Parse_viewer_orient;
	set_modified(0);	

	return 0;
}

//	Editor-level interface to mission load/save.

// Does nothing now.. Handled by OnOpenDocument and OnSaveDocument.  This is because we
// want to avoid using the CArchive for file I/O  -JH
void CFREDDoc::Serialize(CArchive& ar)
{
	return;
/*  The original Serialize code
	int		rw_flag;
	XFILE		*fp;
//	CString	CSfilename;
//	char		filename[128], *tfilename;

	fp = ar.GetFile();
	rw_flag = ar.IsStoring();

//	CSfilename = fp->GetFileName();
//	tfilename = CSfilename.GetBuffer(16);
//	strcpy(filename, tfilename);
//	CSfilename.ReleaseBuffer();
// -- Don't close this, it gets closed by MFC --	ar.Close();

	cfile_serialize(fp, rw_flag);
	cfile_serialize_editor(fp, rw_flag);
*/
}

/////////////////////////////////////////////////////////////////////////////
// CFREDDoc diagnostics

#ifdef _DEBUG
void CFREDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFREDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CFREDDoc commands

void CFREDDoc::OnEditDelete() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnDuplicate() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnEditCopy() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnEditCut() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnEditPaste() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnEditHold() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnEditFetch() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnEditUndo() 
{
	// TODO: Add your command handler code here
	
}

void CFREDDoc::OnFilePreferences() 
{
	CPrefsDlg dlg;
	// Initialize dialog data
	dlg.m_ConfirmDeleting = confirm_deleting;
	dlg.m_ShowCapitalShips = show_capital_ships;
	dlg.m_ShowElevations = show_elevations;
	dlg.m_ShowFighters = show_fighters;
	dlg.m_ShowGrid = show_grid;
	dlg.m_ShowMiscObjects = show_misc_objects;
	dlg.m_ShowPlanets = show_planets;
	dlg.m_ShowWaypoints = show_waypoints;
	dlg.m_ShowStarfield = show_starfield;

	// Invoke the dialog box
	if (dlg.DoModal() == IDOK)
	{
		// retrieve the dialog data
		confirm_deleting = dlg.m_ConfirmDeleting;
		show_capital_ships = dlg.m_ShowCapitalShips;
		show_elevations = dlg.m_ShowElevations;
		show_fighters = dlg.m_ShowFighters;
		show_grid = dlg.m_ShowGrid;
		show_misc_objects = dlg.m_ShowMiscObjects;
		show_planets = dlg.m_ShowPlanets;
		show_waypoints = dlg.m_ShowWaypoints;
		show_starfield = dlg.m_ShowStarfield;
	}

}

// initialize (clear out) the mission, so it's empty and ready to use.
void CFREDDoc::editor_init_mission()
{
	reset_mission();
	SetModifiedFlag(FALSE);
}
/*
void CFREDDoc::OnFileNew() 
{
	// If mission has been modified, offer to save before continuing.
	while (IsModified()) {
		int	rval;
		
		rval = MessageBox(NULL,
			"You have not saved your work.\n(Which isn't surprising...)\nSave it now?",
			"Creating New Mission",
			MB_YESNOCANCEL + MB_ICONEXCLAMATION);

		if (rval == IDYES) {
			OnFileSave();
		} else if (rval == IDCANCEL)
			return;
		else if (rval == IDNO)
			break;
	}

	editor_init_mission();

}
*/
void CFREDDoc::UpdateStatus(int flags)
{
	if (FREDDoc_ptr)
		if (flags & US_WORLD_CHANGED)
			FREDDoc_ptr->SetModifiedFlag();
}

void CFREDDoc::OnEditClearAll()
{
   DeleteContents();
}
  
  
void CFREDDoc::DeleteContents()
{
	editor_init_mission();
}

void set_modified(BOOL arg)
{
	Local_modified = arg;
	FREDDoc_ptr->SetModifiedFlag(arg);
}

// call this if an unknown ship class was discovered during parsing. Sets up a warning message for players
void fred_notify_unknown_ship_during_parse()
{
	Fred_found_unknown_ship_during_parsing = 1;
}

 //////////////////////////////////////////////////////////////////////////
//
// Below is old, obsolete code, kept around just in case it might be found
// useful some time in the future for something.
//
 //////////////////////////////////////////////////////////////////////////

#if 0
#define SerializeFloat(fp, mode, f) if (mode == 1) fp->Write(&f, sizeof(float)); else fp->Read(&f, sizeof(float))
#define SerializeInt(fp, mode, f) if (mode == 1) fp->Write(&f, sizeof(int)); else fp->Read(&f, sizeof(int))

void SerializeVector(XFILE *fp, int mode, vector *v)
{
	SerializeFloat(fp, mode, v->x);
	SerializeFloat(fp, mode, v->y);
	SerializeFloat(fp, mode, v->z);
}

void SerializeMatrix(XFILE *fp, int mode, matrix *m)
{
	SerializeVector(fp, mode, &m->rvec);
	SerializeVector(fp, mode, &m->uvec);
	SerializeVector(fp, mode, &m->fvec);
}

void SerializePhysicsInfo(XFILE *fp, int mode, physics_info *pi)
{
	SerializeFloat(fp, mode, pi->mass);
	SerializeFloat(fp, mode, pi->drag);
	SerializeVector(fp, mode, &pi->max_thrust);
	SerializeVector(fp, mode, &pi->max_rotthrust);
	SerializeFloat(fp, mode, pi->turnroll);
	SerializeInt(fp, mode, pi->flags);
	SerializeVector(fp, mode, &pi->velocity);
	SerializeVector(fp, mode, &pi->rotvel);
	SerializeVector(fp, mode, &pi->thrust);
	SerializeVector(fp, mode, &pi->rotthrust);
}

/////////////////////////////////////////////////////////////////////////////
// CFREDDoc serialization
void SerializeObject(XFILE *fp, int mode, object *objp)
{
	SerializeInt(fp, mode, objp->signature);
	SerializeInt(fp, mode, objp->type);
	SerializeInt(fp, mode, objp->parent);
	SerializeInt(fp, mode, objp->parent_sig);
	SerializeInt(fp, mode, objp->parent_type);
	SerializeInt(fp, mode, objp->instance);
	SerializeInt(fp, mode, objp->flags);
	SerializeFloat(fp, mode, objp->radius);
//	SerializeInt(fp, mode, objp->wing);
	SerializePhysicsInfo(fp, mode, &objp->phys_info);
	SerializeVector(fp, mode, &objp->pos);
	SerializeMatrix(fp, mode, &objp->orient);
}

void SerializeAI(XFILE *fp, int mode, ai_info *aip)
{
	SerializeInt(fp, mode, aip->shipnum);
	SerializeInt(fp, mode, aip->type);
	SerializeInt(fp, mode, aip->wing);
//MWA --	SerializeInt(fp, mode, aip->current_waypoint);
}

void SerializeShip(XFILE *fp, int mode, ship *shipp)
{
	SerializeInt(fp, mode, shipp->objnum);
	SerializeInt(fp, mode, shipp->ai_index);
	SerializeInt(fp, mode, shipp->subtype);
	SerializeInt(fp, mode, shipp->modelnum);
	SerializeInt(fp, mode, shipp->hits);
	SerializeInt(fp, mode, shipp->dying);
}

void SerializeGrid(XFILE *fp, int mode, grid *gridp)
{
	int	i;

	SerializeInt(fp, mode, gridp->nrows);
	SerializeInt(fp, mode, gridp->ncols);
	SerializeMatrix(fp, mode, &gridp->gmatrix);
	SerializePhysicsInfo(fp, mode, &gridp->physics);
	SerializeFloat(fp, mode, gridp->square_size);
	SerializeFloat(fp, mode, gridp->planeD);
	
	for (i=0; i<MAX_GRID_POINTS; i++)
		SerializeVector(fp, mode, &gridp->gpoints[i]);

}

void cfile_serialize(XFILE *fp, int flag)
{
	int	i;
	int	highest_object_index = 0, highest_ship_index = 0, highest_ai_index = 0;

	Assert((flag == 0) || (flag == 1));

//	fp = cfopen(filename, flag ? "wb" : "rb");
//	if (!fp)
//		MessageBox(NULL, strerror(errno), "File Open Error!", MB_ICONSTOP);

	//	Find highest used object if writing.
	if (flag == 1) {
		for (i=MAX_OBJECTS-1; i>0; i--)
			if (Objects[i].type != OBJ_NONE) {
				highest_object_index = i;
				break;
			}
	}

	if (flag == 0) {
		num_ships = 0;
		num_objects = 0;
	}

	SerializeInt(fp, flag, highest_object_index);

	for (i=1; i<=highest_object_index; i++) {
		SerializeObject(fp, flag, &Objects[i]);
		if (flag == 0)
			if (Objects[i].type != OBJ_NONE)
				num_objects++;
	}

	//	Read/write ships
	if (flag == 1) {
		for (i=MAX_SHIPS-1; i>0; i--)
			if (Ships[i].objnum) {
				highest_ship_index = i;
				break;
			}
	}

	SerializeInt(fp, flag, highest_ship_index);

	for (i=1; i<=highest_ship_index; i++) {
		SerializeShip(fp, flag, &Ships[i]);
		if (flag == 0)
			if (Ships[i].objnum)
				num_ships++;
	}

	// Read/write AI info
	if (flag == 1) {
		for (i=MAX_AI_INFO-1; i>0; i--)
			if (Ai_info[i].shipnum) {
				highest_ai_index = i;
				break;
			}
	}

	SerializeInt(fp, flag, highest_ai_index);
	
	for (i=1; i<=highest_ai_index; i++)
		SerializeAI(fp, flag, &Ai_info[i]);
}

void cfile_serialize_editor(XFILE *fp, int flag)
{
	//	Editor only stuff
	SerializeMatrix(fp, flag, &view_orient);
	SerializeVector(fp, flag, &view_pos);
	
	SerializeInt(fp, flag, Control_mode);
	SerializeInt(fp, flag, cur_object_index);
	SerializeInt(fp, flag, cur_wing);

	SerializeGrid(fp, flag, The_grid);

}
#endif

