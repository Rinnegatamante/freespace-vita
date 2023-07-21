/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Fred2/FREDView.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * View class for a document/view architechure design program, which we don't
 * want or need, but MFC forces us to use.  This is the main place we handle
 * MFC messages, events, etc.  Sort of the interface between our code and MFC.
 * There is also a lot of our code in here related to these things.
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
 * 17    7/15/99 3:07p Dave
 * 32 bit detection support. Mouse coord commandline.
 * 
 * 16    7/09/99 5:54p Dave
 * Seperated cruiser types into individual types. Added tons of new
 * briefing icons. Campaign screen.
 * 
 * 15    6/04/99 2:20p Andsager
 * Add dump stats basic functionality
 * 
 * 14    4/07/99 6:21p Dave
 * Fred and Freespace support for multiple background bitmaps and suns.
 * Fixed link errors on all subprojects. Moved encrypt_init() to
 * cfile_init() and lcl_init(), since its safe to call twice.
 * 
 * 13    3/26/99 4:49p Dave
 * Made cruisers able to dock with stuff. Made docking points and paths
 * visible in fred.
 * 
 * 12    3/20/99 5:09p Dave
 * Fixed release build fred warnings and unhandled exception.
 * 
 * 11    3/04/99 11:56a Johnson
 * Fixed an undo-rotation bug.
 * 
 * 10    3/02/99 9:25p Dave
 * Added a bunch of model rendering debug code. Started work on fixing
 * beam weapon wacky firing.
 * 
 * 9     3/01/99 10:00a Dave
 * Fxied several dogfight related stats bugs.
 * 
 * 8     2/23/99 2:32p Dave
 * First run of oldschool dogfight mode.
 * 
 * 7     1/27/99 4:09p Andsager
 * Added highlight to ship subsystems
 * 
 * 6     1/07/99 1:52p Andsager
 * Initial check in of Sexp_variables
 * 
 * 5     12/18/98 1:49a Dave
 * Fixed Fred initialization problem resulting from hi-res mode changes.
 * 
 * 4     10/13/98 9:27a Dave
 * Started neatening up freespace.h
 * 
 * 3     10/12/98 1:01p Dave
 * Fixed object rotation bug (uninitialized data). Changed a few stubs to
 * correspond to new var names.
 * 
 * 2     10/07/98 6:28p Dave
 * Initial checkin. Renamed all relevant stuff to be Fred2 instead of
 * Fred. Globalized mission and campaign file extensions. Removed Silent
 * Threat specific code.
 * 
 * 1     10/07/98 3:00p Dave
 * 
 * 248   9/16/98 6:54p Dave
 * Upped  max sexpression nodes to 1800 (from 1600). Changed FRED to sort
 * the ship list box. Added code so that tracker stats are not stored with
 * only 1 player.
 * 
 * 247   9/14/98 3:40p Allender
 * better error checking for invalid number of waves for player wings in a
 * multiplayer game.  Better popup message in FreeSpace side.
 * 
 * 246   6/18/98 4:28p Hoffoss
 * Made invalid dock points in an initial order flag an error in Fred.
 * 
 * 245   6/17/98 4:50p Hoffoss
 * Added error checking for arrival delays used on wing player is in.
 * 
 * 244   6/16/98 10:24a Hoffoss
 * Switched internal errors over to normal looking errors for release
 * build.
 * 
 * 243   5/21/98 11:48a Hoffoss
 * Removed check in and check out options.
 * 
 * 242   5/21/98 12:58a Hoffoss
 * Fixed warnings optimized build turned up.
 * 
 * 241   5/19/98 1:19p Allender
 * new low level reliable socket reading code.  Make all missions/campaign
 * load/save to data missions folder (i.e. we are rid of the player
 * missions folder)
 * 
 * 240   5/14/98 5:31p Hoffoss
 * Added some more error checking.
 * 
 * 239   5/10/98 10:05p Allender
 * only show cutscenes which have been seen before.  Made Fred able to
 * write missions anywhere, defaulting to player misison folder, not data
 * mission folder.  Fix FreeSpace code to properly read missions from
 * correct locations
 * 
 * 238   4/28/98 2:13p Hoffoss
 * Added code to help keep invalid player ship types from existing in
 * mission.
 * 
 * 237   4/26/98 6:05p Hoffoss
 * Added multiplayer error checks requested by Dave B.
 * 
 * 236   4/17/98 2:37p Duncan
 * Added check to error checker for arrival/depature targets valid.
 * 
 * 235   4/14/98 4:35p Hoffoss
 * Fixed bug with launching FreeSpace from Fred.  Current working
 * directory wasn't being set properly.
 * 
 * 234   4/14/98 3:38p Hoffoss
 * Fixed the FreeSpace launch code in Fred.
 * 
 * 233   4/07/98 9:42a Allender
 * put in persona combo box into ship editor.  Removed code to assign
 * personas based on message
 * 
 * 232   4/01/98 10:48a Hoffoss
 * Changed Fred to not allow command briefings in multiplayer missions.
 * 
 * 231   3/26/98 3:01p Hoffoss
 * Put in check for alpha wing num ships < 4 for furball multiplayer
 * missions.
 * 
 * 230   3/25/98 4:14p Hoffoss
 * Split ship editor up into ship editor and a misc dialog, which tracks
 * flags and such.
 * 
 * 229   3/23/98 4:04p Hoffoss
 * Fixed dialog window initialization so it looks better at startup (they
 * don't flash on for a second).
 * 
 * 228   3/21/98 7:36p Lawrance
 * Move jump nodes to own lib.
 * 
 * 227   3/19/98 3:37p Adam
 * Fixed Int3() when you try to right click on a briefing icon.
 * 
 * 226   3/16/98 5:05p Hoffoss
 * Fixed viewpoint bug.
 * 
 * 225   3/12/98 2:21p Johnson
 * Fixed some Fred bugs related to jump nodes.
 * 
 * 224   3/10/98 6:11p Hoffoss
 * Added jump node renaming abilities to Fred.
 * 
 * 223   3/10/98 4:26p Hoffoss
 * Changed jump node structure to include a name.  Position is now taken
 * from the object (each jump node has an associated object now).
 * 
 * 222   3/09/98 10:03p Hoffoss
 * Added support for loading/saving jump nodes to mission files.
 * 
 * 221   3/09/98 10:56a Hoffoss
 * Added jump node objects to Fred.
 * 
 * 220   3/05/98 3:59p Hoffoss
 * Added a bunch of new command brief stuff, and asteroid initialization
 * to Fred.
 * 
 * 219   2/26/98 4:59p Allender
 * groundwork for team vs team briefings.  Moved weaponry pool into the
 * Team_data structure.  Added team field into the p_info structure.
 * Allow for mutliple structures in the briefing code.
 * 
 * 218   2/09/98 9:25p Allender
 * team v team support.  multiple pools and breifings
 * 
 * 217   2/07/98 9:13p Hoffoss
 * Added some more error checking to global error checker.
 * 
 * 216   2/06/98 4:39p Hoffoss
 * Added better checking for whether Fred is in the foreground or
 * background.
 * 
 * 215   2/04/98 4:32p Allender
 * support for multiple briefings and debriefings.  Changes to mission
 * type (now a bitfield).  Bitfield defs for multiplayer modes
 * 
 * 214   2/02/98 4:36p Hoffoss
 * Made no mission title given flag an error.
 * 
 * 213   1/29/98 5:14p Hoffoss
 * Added error checking for more than 4 ships in a player starting wing.
 * 
 * 212   1/21/98 5:22p Hoffoss
 * Added check to global error checker to make sure Alpha wing exists for
 * multiplayer missions.
 * 
 * 211   1/14/98 8:40p Allender
 * don't check builtin messages when checking persona stuff
 * 
 * 210   1/07/98 11:24p Allender
 * add latency of 0 to key_mark function calls
 * 
 * 209   12/31/97 3:56p Hoffoss
 * Forced alpha wing to always have a true arrival cue.
 * 
 * 208   12/09/97 8:11a Allender
 * have Fred warn on certain starting wing conditions
 * 
 * 207   11/24/97 10:14p Allender
 * removed num_ships from use!!!!  it was creating problems
 * 
 * 206   11/24/97 11:07a Allender
 * error checking on wings ignoring player orders
 * 
 * 205   11/17/97 4:57p Allender
 * added persona support in FreeSpace.  A couple of new messages for
 * message.tbl which Dan didn't have
 * 
 * 204   11/14/97 5:21p Johnson
 * Added debriefing formulas to error checker.
 * 
 * 203   11/05/97 4:43p Allender
 * reworked medal/rank system to read all data from tables.  Made Fred
 * read medals.tbl.  Changed ai-warp to ai-warp-out which doesn't require
 * waypoint for activation
 * 
 * 202   10/30/97 3:30p Hoffoss
 * Made anti-aliased gridlines an option in Fred.
 * 
 * 201   10/28/97 9:54a Jasen
 * don't check for personas when wing is sending message
 * 
 * 200   10/22/97 3:15p Hoffoss
 * Fixed ai-stay-still initial order to use waypoints instead of waypoint
 * paths.
 * 
 * 199   10/22/97 1:58p Hoffoss
 * Added support for AI_GOALS_PLAY_DEAD and changed AI_GOALS_STAY_STILL
 * for Fred.
 * 
 * 198   10/14/97 10:59a Allender
 * more persona work.  Made global error checker call funciton to assign
 * and check personas
 * 
 * 197   10/10/97 5:03p Allender
 * started work on ai-stay-still
 * 
 * 196   10/01/97 12:37p Hoffoss
 * Changed Fred (and FreeSpace) to utilize alpha, beta and gamma as player
 * starting wing names.
 * 
 * 195   9/16/97 9:41p Hoffoss
 * Changed Fred code around to stop using Parse_player structure for
 * player information, and use actual ships instead.
 * 
 * 194   9/10/97 3:48p Duncan
 * Added a case that was never added in the past (probably overlooked).
 * 
 * 193   9/09/97 3:39p Sandeep
 * warning level 4 bugs
 * 
 * 192   9/09/97 2:12p Hoffoss
 * Added code to allow briefing editor view to be a 1:1 pixel size fixed
 * as the FreeSpace view will have.
 * 
 * 191   9/06/97 2:13p Mike
 * Replace support for TEAM_NEUTRAL
 * 
 * 190   9/03/97 4:32p Hoffoss
 * Added error number range error checking to Fred, and defaulted numbers
 * for # of times docked to 1 in sexp trees.
 * 
 * 189   9/02/97 4:32p Hoffoss
 * Made minimized child windows restore if the window is activated.
 * 
 * 188   9/02/97 1:34p Johnson
 * Fixed bug I swear wasn't there when I checked this in originally.
 * Strange.
 * 
 * 187   9/01/97 6:59p Hoffoss
 * Added source safe checkin and checkout capabilities to Fred.
 * 
 * 186   8/28/97 8:56a Hoffoss
 * Added more checking to sexp error checker, fixed some bugs.
 * 
 * 185   8/26/97 4:18p Hoffoss
 * Added error checking to initial orders dialog when ok is clicked.
 * 
 * 184   8/25/97 5:58p Hoffoss
 * Created menu items for keypress functions in Fred, and fixed bug this
 * uncovered with wing_delete function.
 * 
 * 183   8/22/97 4:16p Hoffoss
 * added support for arrival and departure info in ship editor using
 * wing's info if editing marked ships in a wing instead of using ship's.
 * 
 * 182   8/18/97 9:31p Hoffoss
 * Added grid adjustment dialog and shield system editor dialog.
 * 
 * 181   8/17/97 10:22p Hoffoss
 * Fixed several bugs in Fred with Undo feature.  In the process, recoded
 * a lot of CFile.cpp.
 * 
 * 180   8/16/97 5:23p Hoffoss
 * Added restrictions for single player mission type to allow only 1
 * player start.
 * 
 * 179   8/16/97 2:02a Hoffoss
 * Made docked objects move together in Fred.
 * 
 * 178   8/15/97 11:08a Hoffoss
 * Created a list of order types that can be used for several things, and
 * yet easily changable.  Added order error checking against ship types.
 * 
 * 177   8/15/97 1:07a Hoffoss
 * Changed code to disallow some ship types from being in a wing, and
 * disallowing some ship types from being able to have initial orders.
 * 
 * 176   8/14/97 6:37p Hoffoss
 * Added briefing icon id support to Fred.
 * 
 * 175   8/14/97 2:32p Hoffoss
 * fixed bug where controlling an object doesn't cause screen updates, and
 * added a number of cool features to viewpoint/control object code.
 * 
 * 174   8/14/97 9:30a Hoffoss
 * Added support for new ai goals "stay near ship" and "keep safe
 * distance".
 * 
 * 173   8/13/97 6:23p Hoffoss
 * Fixed initially docked code in Fred.
 * 
 * 172   8/13/97 5:40p Hoffoss
 * Fixed typo.
 * 
 * 171   8/12/97 10:42p Hoffoss
 * Changed code to campaign editor load an accelerator table when created.
 * 
 * 170   8/12/97 6:32p Hoffoss
 * Added code to allow hiding of arrival and departure cues in editors.
 * 
 * 169   8/12/97 5:56p Johnson
 * Fixed bug with global error checker.  Initially docked ships flagged as
 * error when they aren't really.
 * 
 * 168   8/12/97 1:29p Duncan
 * Fixed bug with ai_undock initial order.
 * 
 * 167   8/11/97 7:11p Hoffoss
 * Somehow SS didn't merge the files, so I've reconstructed the changes
 * Allender made.
 * 
 * 166   8/11/97 6:54p Hoffoss
 * Groups now supported in Fred.
 *
 * 165   8/11/97 11:51a Allender
 * added stamp stuff to Fred
 * 
 * 164   8/07/97 6:01p Hoffoss
 * Added a rotate about selected object button to toolbar and
 * functionality, as requested by Comet.
 * 
 * 163   8/07/97 2:07p Hoffoss
 * Added duplicate icon label error checking to global error checker, and
 * made briefign mode boxmark only icons if at least one icon is in the
 * selection box.
 * 
 * 162   8/06/97 10:20a Hoffoss
 * Changed rotation speeds to be less linear and hopefully more useful.
 * 
 * 161   8/05/97 2:27p Jasen
 * Fixed bug in global error checker that caused crashes when in briefing
 * mode.
 * 
 * 160   8/05/97 1:31p Hoffoss
 * Removed excessive source control info from header.
 * 
 * 159   8/01/97 5:52p Hoffoss
 * Changed initially docked to disallow illegal docking combinations, and
 * changed error checker to check for this as well.
 * 
 * 158   8/01/97 3:10p Hoffoss
 * Made Sexp help hidable.
 * 
 * 157   8/01/97 12:52p Hoffoss
 * Added variable, fixed bug with global error check.
 * 
 * 156   7/31/97 6:41p Hoffoss
 * Added checks to global error checker.
 * 
 * 155   7/30/97 4:30p Hoffoss
 * Made Fred sexp error checker more explicit.
 * 
 * 154   7/29/97 5:16p Hoffoss
 * Fixed bug in ship_query_general_type() and added docking checking to
 * Fred's error checking.
 * 
 * 153   7/29/97 1:44p Hoffoss
 * Make player ships exception to ship target within same wing error
 * traping.
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "fred.h"

#include "freddoc.h"
#include "fredview.h"
#include "fredrender.h"
#include "cfile.h"
#include "grid.h"
#include "mainfrm.h"
#include "editor.h"
#include "management.h"
#include "2d.h"
#include "3d.h"
#include "object.h"
#include "linklist.h"
#include "fvi.h"	//	For find_plane_line_intersection
#include "vecmat.h"
#include "key.h"
#include "ailocal.h"
#include "ai.h"
#include "aigoals.h"
#include "ship.h"	// for ship names
#include "missiongoalsdlg.h"
#include "wing.h"
#include "ship_select.h"
#include "playerstarteditor.h"
#include "orienteditor.h"
#include "eventeditor.h"
#include "messageeditordlg.h"
#include "starfield.h"
#include "starfieldeditor.h"
#include "floating.h"
#include "reinforcementeditordlg.h"
#include "asteroideditordlg.h"
#include "campaigntreewnd.h"
#include "debriefingeditordlg.h"
#include "adjustgriddlg.h"
#include "shieldsysdlg.h"
#include "cmdbrief.h"
#include "jumpnode.h"
#include "dumpstats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

subsys_to_render Render_subsys;

// the next variable is used for executable stamping -- please leave it alone!!!
#define FRED_EXPIRE_TIME	(7 * 1000)
char stamp[STAMP_STRING_LENGTH] = { STAMP_STRING };
int expire_game;

#define EXPIRE_BAD_CHECKSUM			1
#define EXPIRE_BAD_TIME					2

#define SHIP_TYPES			8000
#define REDUCER				100.0f
#define DUP_DRAG_OF_WING	2

LOCAL int Duped_wing;

int Autosave_disabled = 0;
int Show_sexp_help = 1;
int Show_ships = 1;
int Show_starts = 1;
int Show_friendly = 1;
int Show_hostile = 1;
int Show_neutral = 1;
int Show_ship_info = 1;
int Show_ship_models = 0;
int Show_compass = 1;
int Show_dock_points = 0;
int Show_paths_fred = 0;
int Selection_lock = 0;
int viewpoint = 0;
int view_obj;
int button_down = 0;
int Marked = 0, moved = 0;
int on_object = -1;
int Cursor_over = -1;
int Dup_drag = 0;
int physics_speed = 1;
int physics_rot = 20;
int box_marking = 0;
int last_mouse_x, last_mouse_y, mouse_dx, mouse_dy;
int Cur_bitmap = -1;
int Id_select_type_jump_node;
int Id_select_type_start = 0;
int Id_select_type_waypoint = 0;
int Hide_ship_cues = 0, Hide_wing_cues = 0;
vector original_pos, saved_cam_pos;
matrix bitmap_matrix_backup, saved_cam_orient = { 0.0f };
Marking_box	marking_box;
object_orient_pos	rotation_backup[MAX_OBJECTS];

// used by error checker, but needed in more than just one function.
char *names[MAX_OBJECTS], flags[MAX_OBJECTS];
int obj_count = 0;
int g_err = 0;

void view_universe(int just_marked = 0);
void select_objects();
void drag_rotate_save_backup();

/////////////////////////////////////////////////////////////////////////////
// CFREDView

CFREDView *Fred_view_wnd = NULL;

IMPLEMENT_DYNCREATE(CFREDView, CView)

BEGIN_MESSAGE_MAP(CFREDView, CView)
	ON_MESSAGE(WM_GOODBYE, OnGoodbye)
	ON_MESSAGE(WM_MENU_POPUP_SHIPS, OnMenuPopupShips)
	ON_MESSAGE(WM_MENU_POPUP_EDIT, OnMenuPopupEdit)

	//{{AFX_MSG_MAP(CFREDView)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_COMMAND(ID_SHOW_WAYPOINTS, OnViewWaypoints)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WAYPOINTS, OnUpdateViewWaypoints)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDITORS_SHIPS, OnEditorsShips)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_MISCSTUFF_SHOWSHIPSASICONS, OnMiscstuffShowshipsasicons)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_POPUP_SHOW_SHIP_ICONS, OnEditPopupShowShipIcons)
	ON_UPDATE_COMMAND_UI(ID_EDIT_POPUP_SHOW_SHIP_ICONS, OnUpdateEditPopupShowShipIcons)
	ON_COMMAND(ID_EDIT_POPUP_SHOW_SHIP_MODELS, OnEditPopupShowShipModels)
	ON_UPDATE_COMMAND_UI(ID_EDIT_POPUP_SHOW_SHIP_MODELS, OnUpdateEditPopupShowShipModels)

	ON_COMMAND(ID_SHOW_PATHS, OnShowPaths)
	ON_UPDATE_COMMAND_UI(ID_SHOW_PATHS, OnUpdateShowPaths)
	ON_COMMAND(ID_SHOW_DOCK_POINTS, OnShowDockPoints)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DOCK_POINTS, OnUpdateShowDockPoints)

	ON_COMMAND(ID_MISC_STATISTICS, OnMiscStatistics)
	ON_COMMAND(ID_EDIT_POPUP_SHOW_COMPASS, OnEditPopupShowCompass)
	ON_UPDATE_COMMAND_UI(ID_EDIT_POPUP_SHOW_COMPASS, OnUpdateEditPopupShowCompass)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_VIEWPOINT_EXTERNAL, OnUpdateChangeViewpointExternal)
	ON_COMMAND(ID_CHANGE_VIEWPOINT_EXTERNAL, OnChangeViewpointExternal)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_VIEWPOINT_FOLLOW, OnUpdateChangeViewpointFollow)
	ON_COMMAND(ID_CHANGE_VIEWPOINT_FOLLOW, OnChangeViewpointFollow)
	ON_COMMAND(ID_EDITORS_GOALS, OnEditorsGoals)
	ON_COMMAND(ID_SPEED1, OnSpeed1)
	ON_COMMAND(ID_SPEED2, OnSpeed2)
	ON_COMMAND(ID_SPEED5, OnSpeed5)
	ON_COMMAND(ID_SPEED10, OnSpeed10)
	ON_UPDATE_COMMAND_UI(ID_SPEED1, OnUpdateSpeed1)
	ON_COMMAND(ID_SPEED3, OnSpeed3)
	ON_COMMAND(ID_SPEED8, OnSpeed8)
	ON_COMMAND(ID_ROT1, OnRot1)
	ON_COMMAND(ID_ROT2, OnRot2)
	ON_COMMAND(ID_ROT3, OnRot3)
	ON_COMMAND(ID_ROT4, OnRot4)
	ON_COMMAND(ID_ROT5, OnRot5)
	ON_UPDATE_COMMAND_UI(ID_SPEED2, OnUpdateSpeed2)
	ON_UPDATE_COMMAND_UI(ID_SPEED3, OnUpdateSpeed3)
	ON_UPDATE_COMMAND_UI(ID_SPEED5, OnUpdateSpeed5)
	ON_UPDATE_COMMAND_UI(ID_SPEED8, OnUpdateSpeed8)
	ON_UPDATE_COMMAND_UI(ID_SPEED10, OnUpdateSpeed10)
	ON_UPDATE_COMMAND_UI(ID_ROT1, OnUpdateRot1)
	ON_UPDATE_COMMAND_UI(ID_ROT2, OnUpdateRot2)
	ON_UPDATE_COMMAND_UI(ID_ROT3, OnUpdateRot3)
	ON_UPDATE_COMMAND_UI(ID_ROT4, OnUpdateRot4)
	ON_UPDATE_COMMAND_UI(ID_ROT5, OnUpdateRot5)
	ON_COMMAND(ID_CONTROL_MODE_CAMERA, OnControlModeCamera)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_MODE_CAMERA, OnUpdateControlModeCamera)
	ON_COMMAND(ID_CONTROL_MODE_SHIP, OnControlModeShip)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_MODE_SHIP, OnUpdateControlModeShip)
	ON_COMMAND(ID_SHOW_GRID_POSITIONS, OnShowGridPositions)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID_POSITIONS, OnUpdateShowGridPositions)
	ON_COMMAND(ID_SHOW_COORDINATES, OnShowCoordinates)
	ON_UPDATE_COMMAND_UI(ID_SHOW_COORDINATES, OnUpdateShowCoordinates)
	ON_COMMAND(ID_SPEED50, OnSpeed50)
	ON_UPDATE_COMMAND_UI(ID_SPEED50, OnUpdateSpeed50)
	ON_COMMAND(ID_SPEED100, OnSpeed100)
	ON_UPDATE_COMMAND_UI(ID_SPEED100, OnUpdateSpeed100)
	ON_COMMAND(ID_SELECT, OnSelect)
	ON_UPDATE_COMMAND_UI(ID_SELECT, OnUpdateSelect)
	ON_COMMAND(ID_SELECT_AND_MOVE, OnSelectAndMove)
	ON_UPDATE_COMMAND_UI(ID_SELECT_AND_MOVE, OnUpdateSelectAndMove)
	ON_COMMAND(ID_SELECT_AND_ROTATE, OnSelectAndRotate)
	ON_UPDATE_COMMAND_UI(ID_SELECT_AND_ROTATE, OnUpdateSelectAndRotate)
	ON_COMMAND(ID_CONSTRAIN_X, OnConstrainX)
	ON_UPDATE_COMMAND_UI(ID_CONSTRAIN_X, OnUpdateConstrainX)
	ON_COMMAND(ID_CONSTRAIN_Y, OnConstrainY)
	ON_UPDATE_COMMAND_UI(ID_CONSTRAIN_Y, OnUpdateConstrainY)
	ON_COMMAND(ID_CONSTRAIN_Z, OnConstrainZ)
	ON_UPDATE_COMMAND_UI(ID_CONSTRAIN_Z, OnUpdateConstrainZ)
	ON_COMMAND(ID_CONSTRAIN_XZ, OnConstrainXz)
	ON_UPDATE_COMMAND_UI(ID_CONSTRAIN_XZ, OnUpdateConstrainXz)
	ON_COMMAND(ID_SELECTION_LOCK, OnSelectionLock)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_LOCK, OnUpdateSelectionLock)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_DOUBLE_FINE_GRIDLINES, OnDoubleFineGridlines)
	ON_UPDATE_COMMAND_UI(ID_DOUBLE_FINE_GRIDLINES, OnUpdateDoubleFineGridlines)
	ON_COMMAND(ID_SHOW_DISTANCES, OnShowDistances)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DISTANCES, OnUpdateShowDistances)
	ON_COMMAND(ID_UNIVERSAL_HEADING, OnUniversalHeading)
	ON_UPDATE_COMMAND_UI(ID_UNIVERSAL_HEADING, OnUpdateUniversalHeading)
	ON_COMMAND(ID_FLYING_CONTROLS, OnFlyingControls)
	ON_UPDATE_COMMAND_UI(ID_FLYING_CONTROLS, OnUpdateFlyingControls)
	ON_COMMAND(ID_ROTATE_LOCALLY, OnRotateLocally)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_LOCALLY, OnUpdateRotateLocally)
	ON_COMMAND(ID_CONSTRAIN_XY, OnConstrainXy)
	ON_UPDATE_COMMAND_UI(ID_CONSTRAIN_XY, OnUpdateConstrainXy)
	ON_UPDATE_COMMAND_UI(ID_CONSTRAIN_YZ, OnUpdateConstrainYz)
	ON_COMMAND(ID_CONSTRAIN_YZ, OnConstrainYz)
	ON_COMMAND(ID_SELECT_LIST, OnSelectList)
	ON_COMMAND(ID_ZOOM_EXTENTS, OnZoomExtents)
	ON_COMMAND(ID_ZOOM_SELECTED, OnZoomSelected)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_SELECTED, OnUpdateZoomSelected)
	ON_COMMAND(ID_FORM_WING, OnFormWing)
	ON_UPDATE_COMMAND_UI(ID_FORM_WING, OnUpdateFormWing)
	ON_COMMAND(ID_DISBAND_WING, OnDisbandWing)
	ON_UPDATE_COMMAND_UI(ID_DISBAND_WING, OnUpdateDisbandWing)
	ON_COMMAND(ID_SHOW_HORIZON, OnShowHorizon)
	ON_UPDATE_COMMAND_UI(ID_SHOW_HORIZON, OnUpdateShowHorizon)
	ON_COMMAND(ID_EDITORS_WING, OnEditorsWing)
	ON_COMMAND(ID_EDITORS_PLAYER, OnEditorsPlayer)
	ON_COMMAND(ID_EDITORS_ORIENT, OnEditorsOrient)
	ON_COMMAND(ID_EDITORS_EVENTS, OnEditorsEvents)
	ON_UPDATE_COMMAND_UI(ID_EDITORS_ORIENT, OnUpdateEditorsOrient)
	ON_COMMAND(ID_EDITORS_MESSAGE, OnEditorsMessage)
	ON_COMMAND(ID_EDITORS_STARFIELD, OnEditorsStarfield)
	ON_COMMAND(ID_EDITORS_BG_BITMAPS, OnEditorsBgBitmaps)
	ON_COMMAND(ID_EDITORS_REINFORCEMENT, OnEditorsReinforcement)
	ON_COMMAND(ID_ERROR_CHECKER, OnErrorChecker)
	ON_COMMAND(ID_EDITORS_WAYPOINT, OnEditorsWaypoint)
	ON_COMMAND(ID_VIEW_OUTLINES, OnViewOutlines)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLINES, OnUpdateViewOutlines)
	ON_UPDATE_COMMAND_UI(ID_NEW_SHIP_TYPE, OnUpdateNewShipType)
	ON_COMMAND(ID_SHOW_STARFIELD, OnShowStarfield)
	ON_UPDATE_COMMAND_UI(ID_SHOW_STARFIELD, OnUpdateShowStarfield)
	ON_COMMAND(ID_ASTEROID_EDITOR, OnAsteroidEditor)
	ON_COMMAND(ID_RUN_FREESPACE, OnRunFreespace)
	ON_COMMAND(ID_EDITOR_CAMPAIGN, OnEditorCampaign)
	ON_COMMAND(ID_SHOW_SHIPS, OnShowShips)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHIPS, OnUpdateShowShips)
	ON_COMMAND(ID_SHOW_STARTS, OnShowStarts)
	ON_UPDATE_COMMAND_UI(ID_SHOW_STARTS, OnUpdateShowStarts)
	ON_COMMAND(ID_SHOW_FRIENDLY, OnShowFriendly)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRIENDLY, OnUpdateShowFriendly)
	ON_COMMAND(ID_SHOW_HOSTILE, OnShowHostile)
	ON_UPDATE_COMMAND_UI(ID_SHOW_HOSTILE, OnUpdateShowHostile)
	ON_COMMAND(ID_TOGGLE_VIEWPOINT, OnToggleViewpoint)
	ON_COMMAND(ID_REVERT, OnRevert)
	ON_UPDATE_COMMAND_UI(ID_REVERT, OnUpdateRevert)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_HIDE_OBJECTS, OnHideObjects)
	ON_COMMAND(ID_SHOW_HIDDEN_OBJECTS, OnShowHiddenObjects)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDITORS_BRIEFING, OnEditorsBriefing)
	ON_COMMAND(ID_EDITORS_DEBRIEFING, OnEditorsDebriefing)
	ON_COMMAND(ID_SAVE_CAMERA, OnSaveCamera)
	ON_COMMAND(ID_RESTORE_CAMERA, OnRestoreCamera)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_CAMERA, OnUpdateRestoreCamera)
	ON_COMMAND(ID_SHOW_SEXP_HELP, OnShowSexpHelp)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SEXP_HELP, OnUpdateShowSexpHelp)
	ON_COMMAND(ID_LOOKAT_OBJ, OnLookatObj)
	ON_UPDATE_COMMAND_UI(ID_LOOKAT_OBJ, OnUpdateLookatObj)
	ON_COMMAND(ID_EDITORS_ADJUST_GRID, OnEditorsAdjustGrid)
	ON_COMMAND(ID_EDITORS_SHIELD_SYS, OnEditorsShieldSys)
	ON_COMMAND(ID_LEVEL_OBJ, OnLevelObj)
	ON_COMMAND(ID_ALIGN_OBJ, OnAlignObj)
	ON_COMMAND(ID_CONTROL_OBJ, OnControlObj)
	ON_COMMAND(ID_NEXT_OBJ, OnNextObj)
	ON_COMMAND(ID_PREV_OBJ, OnPrevObj)
	ON_COMMAND(ID_EDIT_DELETE_WING, OnEditDeleteWing)
	ON_COMMAND(ID_MARK_WING, OnMarkWing)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_OBJ, OnUpdateControlObj)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_AA_GRIDLINES, OnAaGridlines)
	ON_UPDATE_COMMAND_UI(ID_AA_GRIDLINES, OnUpdateAaGridlines)
	ON_COMMAND(ID_CMD_BRIEF, OnCmdBrief)
	ON_COMMAND(ID_DISABLE_UNDO, OnDisableUndo)
	ON_UPDATE_COMMAND_UI(ID_DISABLE_UNDO, OnUpdateDisableUndo)
	ON_UPDATE_COMMAND_UI(ID_CMD_BRIEF, OnUpdateCmdBrief)
	ON_COMMAND(ID_NEXT_SUBSYS, OnNextSubsys)
	ON_COMMAND(ID_PREV_SUBSYS, OnPrevSubsys)
	ON_COMMAND(ID_CANCEL_SUBSYS, OnCancelSubsys)
	ON_COMMAND(ID_DUMP_STATS, OnDumpStats)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_GROUP1, ID_GROUP9, OnGroup)
	ON_COMMAND_RANGE(ID_SET_GROUP1, ID_SET_GROUP9, OnSetGroup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFREDView construction/destruction

CFREDView::CFREDView()
{
	//m_ConfirmDeleting = TRUE;
	//m_ShowCapitalShips = TRUE;
	//m_ShowElevations = TRUE;
	//m_ShowFighters = TRUE;
	//m_ShowGrid = TRUE;
	//m_ShowMiscObjects = TRUE;
	//m_ShowPlanets = TRUE;
	//m_ShowWaypoints = TRUE;
	
	m_pGDlg = new CGrid(this);

	fred_init();

	//if (!(int errno = gr_init(640, 480, 32)))
	//	Error(LOCATION, "Hey, gr_init failed! Error code = %i", errno);
	Fred_view_wnd = this;
}

CFREDView::~CFREDView()
{
	delete m_pGDlg;

	gr_close();
}

void CALLBACK expire_game_proc( HWND wnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	KillTimer(wnd, 1);
	if ( expire_game == EXPIRE_BAD_CHECKSUM )
		MessageBox (wnd, "Fred can no longer run due to internal overlay error", NULL, MB_OK | MB_ICONERROR |MB_TASKMODAL|MB_SETFOREGROUND);
	else
		MessageBox (wnd, "Error: cannot enter DOS mode for 80x40 color text mode display.", NULL, MB_OK | MB_ICONERROR|MB_TASKMODAL|MB_SETFOREGROUND);
	exit(1);
}

BOOL CFREDView::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL casperl;

	casperl = CView::PreCreateWindow(cs);
	cs.y = 0;  // doesn't seem to do anything. :(

// other miscellaneous initializations
	cfile_chdir("data\\missions");
	set_physics_controls();
	return casperl;
}

/////////////////////////////////////////////////////////////////////////////
// CFREDView drawing

void CFREDView::OnDraw(CDC* pDC)
{
	CRect clip;

	CFREDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Update_window = 1;
	if (Fred_active)
		return;

	pDC->GetClipBox(&clip);
	gr_set_clip(clip.left, clip.top, clip.right - clip.left + 1, clip.bottom - clip.top + 1);
	Assert(clip.left <= clip.right);
	Assert(clip.top <= clip.bottom);
	gr_flip_window((uint) pDC->m_hDC, clip.left, clip.top,
		clip.right - clip.left + 1, clip.bottom - clip.top + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CFREDView printing

BOOL CFREDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFREDView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFREDView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFREDView diagnostics

#ifdef _DEBUG
void CFREDView::AssertValid() const
{
	CView::AssertValid();
}

void CFREDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFREDDoc* CFREDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFREDDoc)));
	return (CFREDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFREDView message handlers

void CFREDView::OnViewGrid() 
{
	Show_grid = !Show_grid;
	Update_window = 1;
}

void CFREDView::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_grid);
}

void CFREDView::OnViewWaypoints() 
{
	Show_waypoints = !Show_waypoints;
	Update_window = 1;
}

void CFREDView::OnUpdateViewWaypoints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_waypoints);
}

#define MAX_MOVE_DISTANCE 25.0f

//	If cur_object_index references a valid object, drag it from its current
//	location to the new cursor location specified by "point".
//	It is dragged relative to the main grid.  It's y coordinate is not changed.
//	Return value: 0/1 = didn't/did move object all the way to goal.
int drag_objects()
{
	int z, cobj, flag, rval = 1;
	float r;
	float	distance_moved = 0.0f;
	vector cursor_dir, int_pnt;
	vector movement_vector;
	vector obj;
	vector vec1, vec2;
	object *objp, *ptr;
	// starfield_bitmaps *bmp;

	/*
	if (Bg_bitmap_dialog) {
		if (Cur_bitmap < 0)
			return -1;

		bmp = &Starfield_bitmaps[Cur_bitmap];
		if (Single_axis_constraint && Constraint.z) {
			bmp->dist *= 1.0f + mouse_dx / -800.0f;
			calculate_bitmap_points(bmp, 0.0f);

		} else {
			g3_point_to_vec_delayed(&bmp->m.fvec, marking_box.x2, marking_box.y2);
			vm_orthogonalize_matrix(&bmp->m);
			calculate_bitmap_points(bmp, 0.0f);
		}
		return rval;
	}
	*/

	if (!query_valid_object())
		return -1;

	if ((Dup_drag == 1) && (Briefing_dialog))
		Dup_drag = 0;

	if (Dup_drag == 1) {
		dup_object(NULL);  // reset waypoint list
		cobj = Duped_wing = -1;
		flag = 0;
		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list))	{
			Assert(objp->type != OBJ_NONE);
			if (objp->flags & OF_MARKED) {
				if ((objp->type == OBJ_SHIP) || (objp->type == OBJ_START)) {
					z = Ships[objp->instance].wingnum;
					if (!flag)
						Duped_wing = z;
					else if (Duped_wing != z)
						Duped_wing = -1;

				} else
					Duped_wing = -1;

				flag = 1;
				z = dup_object(objp);
				if (z == -1) {
					cobj = -1;
					break;
				}

				if (cur_object_index == OBJ_INDEX(objp) )
					cobj = z;
			}

			objp = GET_NEXT(objp);
		}

		obj_merge_created_list();
		if (cobj == -1) {
			objp = GET_FIRST(&obj_used_list);
			while (objp != END_OF_LIST(&obj_used_list))	{
				ptr = GET_NEXT(objp);
				if (objp->flags & OF_TEMP_MARKED)
					delete_object(objp);

				objp = ptr;
			}

			button_down = 0;
			return -1;
		}

		unmark_all();

		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list))	{
			if (objp->flags & OF_TEMP_MARKED) {
				objp->flags &= ~OF_TEMP_MARKED;
				mark_object(OBJ_INDEX(objp));
			}

			objp = GET_NEXT(objp);
		}

		set_cur_object_index(cobj);
		if (Duped_wing != -1)
			Dup_drag = DUP_DRAG_OF_WING;  // indication for later that we duped objects in a wing
		else
			Dup_drag = 0;

		drag_rotate_save_backup();
		set_modified();
		Update_window = 1;
	}

	objp = &Objects[cur_object_index];
	Assert(objp->type != OBJ_NONE);
	obj = int_pnt = objp->pos;

	//	Get 3d vector specified by mouse cursor location.
	g3_point_to_vec_delayed(&cursor_dir, marking_box.x2, marking_box.y2);
	if (Single_axis_constraint)	{
//		if (fvi_ray_plane(&int_pnt, &obj, &view_orient.fvec, &view_pos, &cursor_dir, 0.0f) >= 0.0f )	{
//			vm_vec_add(&p1, &obj, &Constraint);
//			find_nearest_point_on_line(&nearest_point, &obj, &p1, &int_pnt);
//			int_pnt = nearest_point;
//			distance_moved = vm_vec_dist(&obj, &int_pnt);
//		}

		vector tmpAnticonstraint = Anticonstraint;
		vector tmpObject = obj;

		tmpAnticonstraint.x = 0.0f;
		r = fvi_ray_plane(&int_pnt, &tmpObject, &tmpAnticonstraint, &view_pos, &cursor_dir, 0.0f);

		//	If intersected behind viewer, don't move.  Too confusing, not what user wants.
		vm_vec_sub(&vec1, &int_pnt, &view_pos);
		vm_vec_sub(&vec2, &obj, &view_pos);
		if ((r>=0.0f) && (vm_vec_dot(&vec1, &vec2) >= 0.0f))	{
			vector tmp1;
			vm_vec_sub( &tmp1, &int_pnt, &obj );
			tmp1.x *= Constraint.x;
			tmp1.y *= Constraint.y;
			tmp1.z *= Constraint.z;
			vm_vec_add( &int_pnt, &obj, &tmp1 );
				
			distance_moved = vm_vec_dist(&obj, &int_pnt);
		}


	} else {  // Move in x-z plane, defined by grid.  Preserve height.
		r = fvi_ray_plane(&int_pnt, &obj, &Anticonstraint, &view_pos, &cursor_dir, 0.0f);

		//	If intersected behind viewer, don't move.  Too confusing, not what user wants.
		vm_vec_sub(&vec1, &int_pnt, &view_pos);
		vm_vec_sub(&vec2, &obj, &view_pos);
		if ((r>=0.0f) && (vm_vec_dot(&vec1, &vec2) >= 0.0f))
			distance_moved = vm_vec_dist(&obj, &int_pnt);
	}

	//	If moved too far, then move max distance along vector.
	vm_vec_sub(&movement_vector, &int_pnt, &obj);
/*	if (distance_moved > MAX_MOVE_DISTANCE)	{
		vm_vec_normalize(&movement_vector);
		vm_vec_scale(&movement_vector, MAX_MOVE_DISTANCE);
	} */

	if (distance_moved) {
		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list))	{
			Assert(objp->type != OBJ_NONE);
			if (objp->flags & OF_MARKED) {
				vm_vec_add(&objp->pos, &objp->pos, &movement_vector);
				if (objp->type == OBJ_WAYPOINT) {
					Waypoint_lists[objp->instance / 65536].waypoints[objp->instance & 0xffff] =
						objp->pos;
				}
			}

			objp = GET_NEXT(objp);
		}

		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list)) {
			if (objp->flags & OF_MARKED)
				object_moved(objp);

			objp = GET_NEXT(objp);
		}
	}

	if (Briefing_dialog)
		Briefing_dialog->update_positions();

	set_modified();
	return rval;
}

void drag_rotate_save_backup()
{
	object *objp;

	/*
	if (Cur_bitmap != -1)
		bitmap_matrix_backup = Starfield_bitmaps[Cur_bitmap].m;
		*/

	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list))			{
		Assert(objp->type != OBJ_NONE);
		if (objp->flags & OF_MARKED)	{
			rotation_backup[OBJ_INDEX(objp)].pos = objp->pos;
			rotation_backup[OBJ_INDEX(objp)].orient = objp->orient;
		}

		objp = GET_NEXT(objp);
	}
}

int drag_rotate_objects()
{
	int rval = 1;	
	vector int_pnt, obj;
	angles a;
	matrix leader_orient, leader_transpose, tmp, newmat, rotmat;
	object *leader, *objp;
	// starfield_bitmaps *bmp;

	Update_window = 1;
	/*
	if (Bg_bitmap_dialog) {
		if (Cur_bitmap < 0)
			return -1;

		bmp = &Starfield_bitmaps[Cur_bitmap];
		calculate_bitmap_points(bmp, mouse_dx / -300.0f);
		return rval;
	}
	*/

	if (!query_valid_object()){
		return -1;
	}

	objp = &Objects[cur_object_index];
	Assert(objp->type != OBJ_NONE);
	obj = int_pnt = objp->pos;

	memset(&a, 0, sizeof(angles));
	if (Single_axis_constraint) {
		if (Constraint.x)
			a.p = mouse_dy / REDUCER;
		else if (Constraint.y)
			a.h = mouse_dx / REDUCER;
		else if (Constraint.z)
			a.b = -mouse_dx / REDUCER;

	} else {
		if (!Constraint.x) {				// yz
			a.b = -mouse_dx / REDUCER;
			a.h = mouse_dy / REDUCER;
		} else if (!Constraint.y) {	// xz
			a.p = mouse_dy / REDUCER;
			a.b = -mouse_dx / REDUCER;
		} else if (!Constraint.z) {	// xy
			a.p = mouse_dy / REDUCER;
			a.h = mouse_dx / REDUCER;
		}
	}

	leader = &Objects[cur_object_index];
	leader_orient = leader->orient;			// save original orientation
	vm_copy_transpose_matrix(&leader_transpose, &leader_orient);

	vm_angles_2_matrix(&rotmat, &a);
	vm_matrix_x_matrix(&newmat, &leader->orient, &rotmat);
	leader->orient = newmat;

	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list))			{
		Assert(objp->type != OBJ_NONE);
		if ((objp->flags & OF_MARKED) && (cur_object_index != OBJ_INDEX(objp) )) {
			if (Group_rotate) {
				matrix rot_trans;
				vector tmpv1, tmpv2;

				// change rotation matrix to rotate in opposite direction.  This rotation
				// matrix is what the leader ship has rotated by.
				vm_copy_transpose_matrix(&rot_trans, &rotmat);

				// get point relative to our point of rotation (make POR the origin).
				vm_vec_sub(&tmpv1, &objp->pos, &leader->pos);

				// convert point from real-world coordinates to leader's relative coordinate
				// system (z=forward vec, y=up vec, x=right vec
				vm_vec_rotate(&tmpv2, &tmpv1, &leader_orient);

				// now rotate the point by the transpose from above.
				vm_vec_rotate(&tmpv1, &tmpv2, &rot_trans);

				// convert point back into real-world coordinates
				vm_vec_rotate(&tmpv2, &tmpv1, &leader_transpose);

				// and move origin back to real-world origin.  Object is now at it's correct
				// position.
				vm_vec_add(&objp->pos, &leader->pos, &tmpv2);

				// Now fix the object's orientation to what it should be.
				vm_matrix_x_matrix(&tmp, &objp->orient, &rotmat);
				vm_orthogonalize_matrix(&tmp);  // safety check
				objp->orient = tmp;

			} else {
				vm_matrix_x_matrix(&tmp, &objp->orient, &rotmat);
				objp->orient = tmp;
			}
		}
		
		objp = GET_NEXT(objp);
	}

	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if (objp->flags & OF_MARKED)
			object_moved(objp);

		objp = GET_NEXT(objp);
	}

	set_modified();
	return rval;
}

void cancel_drag()
{
	Update_window = 1;

	/*
	if (Bg_bitmap_dialog) {
		Assert(!vm_check_matrix_for_zeros(&bitmap_matrix_backup));
		Starfield_bitmaps[Cur_bitmap].m = bitmap_matrix_backup;
		calculate_bitmap_points(&Starfield_bitmaps[Cur_bitmap], 0.0f);
		button_down = box_marking = 0;
		Update_window = 1;
		return;
	}
	*/

	if (button_down) {
		if (Editing_mode == 1) {
			vector movement_vector;
			object *objp;

			if (query_valid_object()) {
				objp = &Objects[cur_object_index];
				Assert(objp->type != OBJ_NONE);
				vm_vec_sub(&movement_vector, &original_pos, &objp->pos);

				objp = GET_FIRST(&obj_used_list);
				while (objp != END_OF_LIST(&obj_used_list))	{
					Assert(objp->type != OBJ_NONE);
					if (objp->flags & OF_MARKED)
						vm_vec_add(&objp->pos, &objp->pos, &movement_vector);

					objp = GET_NEXT(objp);
				}
			}

		} else if (Editing_mode == 2) {
			object *objp;

			objp = GET_FIRST(&obj_used_list);
			while (objp != END_OF_LIST(&obj_used_list))	{
				Assert(objp->type != OBJ_NONE);
				if (objp->flags & OF_MARKED) {
					int obj_index = OBJ_INDEX(objp);

					if(!IS_VEC_NULL(&rotation_backup[obj_index].orient.rvec) && 
						!IS_VEC_NULL(&rotation_backup[obj_index].orient.uvec) && 
						!IS_VEC_NULL(&rotation_backup[obj_index].orient.fvec)){

						objp->pos = rotation_backup[obj_index].pos;
						objp->orient = rotation_backup[obj_index].orient;
					}
				}

				objp = GET_NEXT(objp);
			}
		}
	}

	button_down = box_marking = 0;
	if (Briefing_dialog)
		Briefing_dialog->update_positions();
}

void CFREDView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int list = -1;
	
	if (!Fred_active) {
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	if (cur_waypoint != -1)
		list = cur_waypoint_list * 65536 + cur_waypoint;

	marking_box.x1 = point.x;
	marking_box.y1 = point.y;
	Dup_drag = 0;
	
	on_object = select_object(point.x, point.y);
	button_down = 1;
	SetCapture();
	drag_rotate_save_backup();
	
	if (nFlags & MK_CONTROL)	{  // add a new object
		if (!Bg_bitmap_dialog) {
			if (on_object == -1) {
				Selection_lock = 0;  // force off selection lock
				on_object = create_object_on_grid(list);

			} else
				Dup_drag = 1;

		} else {
			/*
			Selection_lock = 0;  // force off selection lock
			on_object = Cur_bitmap = create_bg_bitmap();
			Bg_bitmap_dialog->update_data();
			Update_window = 1;
			if (Cur_bitmap == -1)
				MessageBox("Background bitmap limit reached.\nCan't add more.");
			*/
		}

	} else if (!Selection_lock) {
		if (Bg_bitmap_dialog) {
			Cur_bitmap = on_object;
			Bg_bitmap_dialog -> update_data();

		} else if ((nFlags & MK_SHIFT) || (on_object == -1) || !(Objects[on_object].flags & OF_MARKED)) {
			if (!(nFlags & MK_SHIFT))
				unmark_all();

			if (on_object != -1) {
				if (Objects[on_object].flags & OF_MARKED)
					unmark_object(on_object);
				else
					mark_object(on_object);
			}
		}
	}

	if (query_valid_object())
		original_pos = Objects[cur_object_index].pos;

	moved = 0;
	if (Selection_lock) {
		if (Editing_mode == 1)
			drag_objects();
		else if (Editing_mode == 2)
			drag_rotate_objects();

		Update_window = 1;
	}

	if (query_valid_object() && (Marked == 1) && (Objects[cur_object_index].type == OBJ_POINT)) {
		Assert(Briefing_dialog);
		Briefing_dialog->icon_select(Objects[cur_object_index].instance);

	} else {
		if (Briefing_dialog)
			Briefing_dialog->icon_select(-1);
	}

	CView::OnLButtonDown(nFlags, point);
}

void CFREDView::OnMouseMove(UINT nFlags, CPoint point) 
{
	mouse_dx = point.x - last_mouse_x;
	mouse_dy = point.y - last_mouse_y;
	last_mouse_x = marking_box.x2 = point.x;
	last_mouse_y = marking_box.y2 = point.y;
	Cursor_over = select_object(point.x, point.y);

	if (!(nFlags & MK_LBUTTON))
		button_down = 0;

	// The following will cancel a drag operation if another program running in memory
	// happens to jump in and take over (such as new email has arrived popup boxes).
	if (button_down && GetCapture() != this)
		cancel_drag();

	if (!button_down && GetCapture() == this)
		ReleaseCapture();

	if (button_down) {
		if (abs(marking_box.x1 - marking_box.x2) > 1 || abs(marking_box.y1 - marking_box.y2) > 1)
			moved = 1;

		if (moved) {
			if (on_object != -1 || Selection_lock) {
				if (Editing_mode == 1)
					drag_objects();
				else if (Editing_mode == 2)
					drag_rotate_objects();

			} else if (!Bg_bitmap_dialog)
				box_marking = 1;

			if (mouse_dx || mouse_dy)
				Update_window = 1;
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CFREDView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	marking_box.x2 = point.x;
	marking_box.y2 = point.y;

	if (button_down && GetCapture() != this)
		cancel_drag();

	if (GetCapture() == this)
		ReleaseCapture();

	if (button_down) {
		if ((abs(marking_box.x1 - marking_box.x2) > 1) || (abs(marking_box.y1 - marking_box.y2) > 1))
			moved = 1;

		if (moved) {
			if ((on_object != -1) || Selection_lock) {
				if (Editing_mode == 1)
					drag_objects();
				else if ((Editing_mode == 2) || (Editing_mode == 3))
					drag_rotate_objects();

				set_modified();
				FREDDoc_ptr->autosave("object move");

			} else
				box_marking = 1;
		}

		if (Bg_bitmap_dialog) {
			box_marking = 0;

		} else {
			if (box_marking) {
				select_objects();
				box_marking = 0;

			} else if ((!moved && on_object != -1) && !Selection_lock && !(nFlags & MK_SHIFT)) {
				unmark_all();
				mark_object(on_object);
			}
		}
	
		button_down = 0;
		Update_window = 1;
		if (Dup_drag == DUP_DRAG_OF_WING) {
			char msg[256];
			int ship;
			object *objp;

			sprintf(msg, "Add cloned ships to wing %s?", Wings[Duped_wing].name);
			if (MessageBox(msg, "Query", MB_YESNO) == IDYES) {
				objp = GET_FIRST(&obj_used_list);
				while (objp != END_OF_LIST(&obj_used_list))	{
					if (objp->flags & OF_MARKED) {
						if (Wings[Duped_wing].wave_count >= MAX_SHIPS_PER_WING) {
							MessageBox("Max ships per wing limit reached");
							break;
						}

// Can't do player starts, since only player 1 is currently allowed to be in a wing

						Assert(objp->type == OBJ_SHIP);
						ship = objp->instance;
						Assert(Ships[ship].wingnum == -1);
						sprintf(Ships[ship].ship_name, "%s %d", Wings[Duped_wing].name,
							Wings[Duped_wing].wave_count + 1);

						Wings[Duped_wing].ship_index[Wings[Duped_wing].wave_count] = ship;
						Ships[ship].wingnum = Duped_wing;

						wing_objects[Duped_wing][Wings[Duped_wing].wave_count] = OBJ_INDEX(objp);
						Wings[Duped_wing].wave_count++;
					}

					objp = GET_NEXT(objp);
				}
			}
		}
	}

	if (query_valid_object() && (Marked == 1) && (Objects[cur_object_index].type == OBJ_POINT)) {
		Assert(Briefing_dialog);
		Briefing_dialog->icon_select(Objects[cur_object_index].instance);

	} else {
		if (Briefing_dialog)
			Briefing_dialog->icon_select(-1);
	}

	CView::OnLButtonUp(nFlags, point);
}

//	This function never gets called because nothing causes
//	the WM_GOODBYE event to occur.
// False! When you close the Ship Dialog, this function is called! --MK, 8/30/96
LONG CFREDView::OnGoodbye(UINT wParam, LONG lParam)
{
	Ship_editor_dialog.DestroyWindow();
	Wing_editor_dialog.DestroyWindow();

	return 0L;
}

void CFREDView::OnEditorsShips() 
{
	int adjust = 0;

	Assert(Ship_editor_dialog.GetSafeHwnd());
	if (!Show_sexp_help)
		adjust = -SEXP_HELP_BOX_SIZE;

	if (!theApp.init_window(&Ship_wnd_data, &Ship_editor_dialog, adjust))
		return;

	Ship_editor_dialog.SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Ship_editor_dialog.ShowWindow(SW_RESTORE);
}

void CFREDView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT lParam)
{
	uint lKeyData;

	lKeyData = lParam & 255;          // key data 
	if (lParam & 256) lKeyData += 0x80;
	key_mark(lKeyData, 1, 0);
	
	CView::OnKeyDown(nChar, nRepCnt, lParam);
}

void CFREDView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT lParam) 
{
	uint lKeyData;

	lKeyData = lParam & 255;          // key data 
	if (lParam & 256) lKeyData += 0x80;
	key_mark(lKeyData, 0, 0);

	CView::OnKeyUp(nChar, nRepCnt, lParam);
}

void CFREDView::OnSetFocus(CWnd* pOldWnd) 
{
	static int flag = 0;

	if (flag)
		return;

	flag = 1;
	nprintf(("Fred routing", "OnSetFocus() called\n"));
	if (Update_ship) {
		Ship_editor_dialog.initialize_data(1);
		Update_ship = 0;
	}

	if (Update_wing) {
		Wing_editor_dialog.initialize_data(1);
		Update_wing = 0;
	}

/*	if (Wing_editor_dialog.verify() == -1)
		return;  // abort

	if (Ship_editor_dialog.verify() == -1)
		return;  // abort*/

	if (update_dialog_boxes()) {
		nprintf(("Fred routing", "OnSetFocus() returned (error occured)\n"));
		flag = 0;
		Ship_editor_dialog.bypass_errors = 0;
		Wing_editor_dialog.bypass_errors = 0;
		return;  // abort
	}

	if (Local_modified) {
		FREDDoc_ptr->autosave("Editing");
		Local_modified = 0;
	}

	Fred_active = 1;
	CView::OnSetFocus(pOldWnd);
	nprintf(("Fred routing", "Main window focus accepted\n"));
	key_got_focus();

	Cursor_over = -1;
	if (GetActiveWindow() != Fred_main_wnd) {
		Fred_main_wnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		nprintf(("Fred routing", "OnSetFocus() had to put main window back on top\n"));
	}

	flag = 0;
}

void CFREDView::OnKillFocus(CWnd* pNewWnd) 
{
	nprintf(("Fred routing", "OnKillFocus() called\n"));
	Fred_active = 0;
	Local_modified = 0;
	CView::OnKillFocus(pNewWnd);
	key_lost_focus();
	Cursor_over = -1;
}

void CFREDView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if ((cx > 0) && (cy > 0)) {
		gr_init(GR_640, GR_SOFTWARE, 8, cx, cy);
	}
}

void do_trackball_stuff(int nFlags, CPoint point)
{
	int	btn = 0;

	if (nFlags & MK_LBUTTON){
		btn |= 1;
	}
	if (nFlags & MK_RBUTTON){
		btn |= 2;
	}

	move_mouse(btn, point.x, point.y);
}

//	If add_flag != 0, then add found objects to current wing, else create new wing.
void select_objects()
{
	int	x, y, valid, icon_mode = 0;		
	vertex	v;
	object	*ptr;

	if (marking_box.x1 > marking_box.x2) {
		x = marking_box.x1;
		marking_box.x1 = marking_box.x2;
		marking_box.x2 = x;
	}

	if (marking_box.y1 > marking_box.y2) {
		y = marking_box.y1;
		marking_box.y1 = marking_box.y2;
		marking_box.y2 = y;
	}

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		valid = 1;
		if (ptr->flags & OF_HIDDEN)
			valid = 0;

		Assert(ptr->type != OBJ_NONE);
		switch (ptr->type) {
			case OBJ_WAYPOINT:
				if (!Show_waypoints)
					valid = 0;
				break;

			case OBJ_START:
				if (!Show_starts)
					valid = 0;
				break;

			case OBJ_SHIP:
				if (!Show_ships)
					valid = 0;
				switch (Ships[ptr->instance].team) {
					case TEAM_FRIENDLY:
						if (!Show_friendly)
							valid = 0;
						break;

					case TEAM_HOSTILE:
						if (!Show_hostile)
							valid = 0;
						break;

					case TEAM_NEUTRAL:
						if (!Show_neutral)
							valid = 0;
						break;
				}

				break;
		}

		g3_rotate_vertex(&v, &ptr->pos);
		if (!(v.codes & CC_BEHIND) && valid)
			if (!(g3_project_vertex(&v) & PF_OVERFLOW)) {
				x = (int) v.sx;
				y = (int) v.sy;

				if (x >= marking_box.x1 && x <= marking_box.x2 && y >= marking_box.y1 && y <= marking_box.y2) {
					if (ptr->flags & OF_MARKED)
						unmark_object(OBJ_INDEX(ptr));
					else
						mark_object(OBJ_INDEX(ptr));

					if (ptr->type == OBJ_POINT)
						icon_mode = 1;
				}
			}
		
		ptr = GET_NEXT(ptr);
	}

	if (icon_mode) {
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if ((ptr->flags & OF_MARKED) && (ptr->type != OBJ_POINT))
				unmark_object(OBJ_INDEX(ptr));

			ptr = GET_NEXT(ptr);
		}
	}

	Update_ship = Update_wing = 1;
}

LONG CFREDView::OnMenuPopupShips(UINT wParam, LONG lParam)
{
	CMenu	menu;
	CPoint	point;

	point = * ((CPoint*) lParam);
	
	ClientToScreen(&point);

	menu.LoadMenu(IDR_MENU_SHIP_POPUP);
	menu.GetSubMenu(0)->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	return 0L;
}

LONG CFREDView::OnMenuPopupEdit(UINT wParam, LONG lParam)
{
	CMenu	menu;
	CPoint	point;

	point = * ((CPoint*) lParam);
	
	ClientToScreen(&point);

	menu.LoadMenu(IDR_MENU_EDIT_POPUP);
	menu.GetSubMenu(0)->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	return 0L;
}

int	g_Ships_as_icons = 0;

void CFREDView::OnMiscstuffShowshipsasicons() 
{
	Update_window = 1;
	if (g_Ships_as_icons == 0)
		g_Ships_as_icons = 1;
	else
		g_Ships_as_icons = 0;
}

// right mouse button popup menu stuff
void CFREDView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	// make sure window is active
//	GetParentFrame()->ActivateFrame();

	CMenu menu;
	int	objnum;
	CPoint local = point;

	if (button_down) {
		cancel_drag();
		return;
	}

	ScreenToClient(&local);
	objnum = select_object(local.x, local.y);

	if (objnum != -1) {
		set_cur_object_index(objnum);
		if (menu.LoadMenu(IDR_MENU_SHIP_POPUP)) {
			int id = ID_EDITORS_SHIPS;
			CMenu* pPopup = menu.GetSubMenu(0);

			ASSERT(pPopup != NULL);
			if (Marked > 1)
				pPopup->ModifyMenu(ID_EDITORS_SHIPS, MF_BYCOMMAND | MF_STRING, ID_EDITORS_SHIPS, "Edit Marked Ships");
			else {
				CString str;

				if ((Objects[objnum].type == OBJ_START) || (Objects[objnum].type == OBJ_SHIP))
					str.Format("Edit %s", Ships[Objects[objnum].instance].ship_name);

				else if (Objects[objnum].type == OBJ_JUMP_NODE) {
					id = ID_EDITORS_WAYPOINT;
					str.Format("Edit %s", Jump_nodes[Objects[objnum].instance].name);

				} else if (Objects[objnum].type == OBJ_WAYPOINT) {
					id = ID_EDITORS_WAYPOINT;
					str.Format("Edit %s:%d", Waypoint_lists[Objects[objnum].instance / 65536].name,
						(Objects[objnum].instance & 0xffff) + 1);

				} else if (Objects[objnum].type == OBJ_POINT) {
					return;

				} else {
					Int3();
					str = _T("Unknown");
				}

				pPopup->ModifyMenu(ID_EDITORS_SHIPS, MF_BYCOMMAND | MF_STRING, id, str);
			}

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,	point.x, point.y, AfxGetMainWnd()); // use main window for cmds
		}

	} else {
		if (menu.LoadMenu(IDR_MENU_EDIT_POPUP)) {
			int i;
			CMenu* pPopup = menu.GetSubMenu(0);
			CMenu shipPopup, player_submenu, species_submenu[MAX_SPECIES_NAMES];
			ASSERT(pPopup != NULL);

			// create a popup menu based on the ship models read in ship.cpp.
			shipPopup.CreatePopupMenu();
			shipPopup.AppendMenu(MF_STRING | MF_ENABLED, SHIP_TYPES + Id_select_type_waypoint, "Waypoint");
			shipPopup.AppendMenu(MF_STRING | MF_ENABLED, SHIP_TYPES + Id_select_type_start, "Player Start");
			shipPopup.AppendMenu(MF_STRING | MF_ENABLED, SHIP_TYPES + Id_select_type_jump_node, "Jump Node");
			for (i=0; i<MAX_SPECIES_NAMES; i++) {
				species_submenu[i].CreatePopupMenu();
				shipPopup.AppendMenu(MF_STRING | MF_POPUP | MF_ENABLED,
					(UINT) species_submenu[i].m_hMenu, Species_names[i]);
			}

			for (i=0; i<Num_ship_types; i++)
				species_submenu[Ship_info[i].species].AppendMenu(MF_STRING |
					MF_ENABLED, SHIP_TYPES + i, Ship_info[i].name);

			pPopup->AppendMenu(MF_STRING | MF_POPUP | MF_ENABLED,
				(UINT) shipPopup.m_hMenu, "New Object Type");

			CWnd::DrawMenuBar();	// AppendMenu documentation says to do this.
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
		}
	}
}

void CFREDView::OnEditPopupShowShipIcons() 
{
	Show_ship_info = !Show_ship_info;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateEditPopupShowShipIcons(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_ship_info);
}

void CFREDView::OnEditPopupShowShipModels() 
{
	Show_ship_models = !Show_ship_models;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateEditPopupShowShipModels(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_ship_models);
}

void CFREDView::OnEditPopupShowCompass() 
{
	Show_compass = !Show_compass;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateEditPopupShowCompass(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_compass);
}

// View implementation file
CFREDView *CFREDView::GetView()
{
  CFrameWnd *pFrame = (CFrameWnd *) (AfxGetApp()->m_pMainWnd);

  CView *pView = pFrame->GetActiveView();

  if (!pView)
     return NULL;

  // Fail if view is of wrong kind
  // (this could occur with splitter windows, or additional
  // views on a single document
  if (! pView->IsKindOf(RUNTIME_CLASS(CFREDView)))
     return NULL;

  return (CFREDView *) pView;
}

/*void CFREDView::OnShipType0() 
{
	cur_model_index = 1;
}

void CFREDView::OnShipType1() 
{
	cur_model_index = 2;
}

void CFREDView::OnShipType2() 
{
	cur_model_index = 3;
}

void CFREDView::OnShipType3() 
{
	cur_model_index = 4;
}

void CFREDView::OnShipType4() 
{
	cur_model_index = 5;
}

void CFREDView::OnShipType5() 
{
	cur_model_index = 6;
}

void CFREDView::OnUpdateShipType1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 2);
}

void CFREDView::OnUpdateShipType2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 3);
}

void CFREDView::OnUpdateShipType3(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 4);
}

void CFREDView::OnUpdateShipType4(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 5);
}

void CFREDView::OnUpdateShipType5(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 6);
}


void CFREDView::OnUpdateShipType0(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 1);
	
}

void CFREDView::OnEditShipType6() 
{
	cur_model_index = 7;
	
}

void CFREDView::OnUpdateEditShipType6(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(cur_model_index == 7);
} */


// following code added by MWA 09/04/96
// Implements messages for popup menu built on the fly.
// not sure how stable the code is, but appears to work for now.
// id's for the menu items are simply the model numbers for
// the ships.  Shouldn't conflict with any other ID_* thingys.
BOOL CFREDView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	int id = (int) nID;

	if (!pHandlerInfo) {
		if ((id >= SHIP_TYPES) && (id < SHIP_TYPES + Num_ship_types + 3)) {
			if (nCode == CN_COMMAND) {
				cur_model_index = id - SHIP_TYPES;
				m_new_ship_type_combo_box.SetCurSelNEW(cur_model_index);

			} else if (nCode == CN_UPDATE_COMMAND_UI)	{
				// Update UI element state
				((CCmdUI*) pExtra)->SetCheck(cur_model_index + SHIP_TYPES == id);
				((CCmdUI*) pExtra)->Enable(TRUE);
			}

			return TRUE;
		}
	}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CFREDView::OnMiscStatistics() 
{
	char buf[2048];

	sprintf(buf,
		"Number of Objects: %d\n"
		"Number of Ships:   %d\n"
		"Number of Wings:   %d\n",
		num_objects, ship_get_num_ships(), num_wings);

	MessageBox(buf, "FRED Statistics");
}

void CFREDView::OnUpdateChangeViewpointExternal(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!viewpoint);
}

void CFREDView::OnChangeViewpointExternal() 
{
	viewpoint = 0;
	Update_window = 1;
}

void CFREDView::OnUpdateChangeViewpointFollow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(viewpoint == 1);
}

void CFREDView::OnChangeViewpointFollow() 
{
	viewpoint = 1;
	view_obj = cur_object_index;
	Update_window = 1;
}

void CFREDView::OnEditorsGoals()
{
	CMissionGoalsDlg dlg;

	dlg.DoModal();
}

void CFREDView::OnSpeed1() 
{
	physics_speed = 1;
	set_physics_controls();
}

void CFREDView::OnSpeed2() 
{
	physics_speed = 2;
	set_physics_controls();
}

void CFREDView::OnSpeed3() 
{
	physics_speed = 3;
	set_physics_controls();
}

void CFREDView::OnSpeed5() 
{
	physics_speed = 5;
	set_physics_controls();
}

void CFREDView::OnSpeed8() 
{
	physics_speed = 8;
	set_physics_controls();
}

void CFREDView::OnSpeed10() 
{
	physics_speed = 10;
	set_physics_controls();
}

void CFREDView::OnSpeed50() 
{
	physics_speed = 50;
	set_physics_controls();
}

void CFREDView::OnSpeed100() 
{
	physics_speed = 100;
	set_physics_controls();
}

void CFREDView::OnRot1() 
{
	physics_rot = 2;
	set_physics_controls();
}

void CFREDView::OnRot2() 
{
	physics_rot = 10;
	set_physics_controls();
}

void CFREDView::OnRot3() 
{
	physics_rot = 25;
	set_physics_controls();
}

void CFREDView::OnRot4() 
{
	physics_rot = 50;
	set_physics_controls();
}

void CFREDView::OnRot5() 
{
	physics_rot = 100;
	set_physics_controls();
}

void CFREDView::OnUpdateSpeed1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 1);
}

void CFREDView::OnUpdateSpeed2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 2);
}

void CFREDView::OnUpdateSpeed3(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 3);
}

void CFREDView::OnUpdateSpeed5(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 5);
}

void CFREDView::OnUpdateSpeed8(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 8);
}

void CFREDView::OnUpdateSpeed10(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 10);
}

void CFREDView::OnUpdateSpeed50(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 50);
}

void CFREDView::OnUpdateSpeed100(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_speed == 100);
}

void CFREDView::OnUpdateRot1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_rot == 2);
}

void CFREDView::OnUpdateRot2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_rot == 10);
}

void CFREDView::OnUpdateRot3(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_rot == 25);
}

void CFREDView::OnUpdateRot4(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_rot == 50);
}

void CFREDView::OnUpdateRot5(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(physics_rot == 100);
}

void CFREDView::OnControlModeCamera() 
{
	Control_mode = 0;
}

void CFREDView::OnUpdateControlModeCamera(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!Control_mode);
}

void CFREDView::OnControlModeShip() 
{
	Control_mode = 1;
}

void CFREDView::OnUpdateControlModeShip(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Control_mode == 1);
}

void CFREDView::OnShowGridPositions() 
{
	Show_grid_positions = !Show_grid_positions;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowGridPositions(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_grid_positions);
}

void CFREDView::OnShowCoordinates() 
{
	Show_coordinates = !Show_coordinates;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowCoordinates(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_coordinates);
}

void CFREDView::OnSelect() 
{
	Editing_mode = 0;
}

void CFREDView::OnUpdateSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!Editing_mode);
}

void CFREDView::OnSelectAndMove() 
{
	Editing_mode = 1;
}

void CFREDView::OnUpdateSelectAndMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Editing_mode == 1);
}

void CFREDView::OnSelectAndRotate() 
{
	Editing_mode = 2;
}

void CFREDView::OnUpdateSelectAndRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Editing_mode == 2);
}

void CFREDView::OnConstrainX() 
{
	vm_vec_make(&Constraint, 1.0f, 0.0f, 0.0f);
	vm_vec_make(&Anticonstraint, 0.0f, 1.0f, 1.0f);
	Single_axis_constraint = 1;
}

void CFREDView::OnUpdateConstrainX(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(Constraint.x && !Constraint.y && !Constraint.z);
}

void CFREDView::OnConstrainY() 
{
	vm_vec_make(&Constraint, 0.0f, 1.0f, 0.0f);
	vm_vec_make(&Anticonstraint, 1.0f, 0.0f, 1.0f);
	Single_axis_constraint = 1;
}

void CFREDView::OnUpdateConstrainY(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(!Constraint.x && Constraint.y && !Constraint.z);
}

void CFREDView::OnConstrainZ() 
{
	vm_vec_make(&Constraint, 0.0f, 0.0f, 1.0f);
	vm_vec_make(&Anticonstraint, 1.0f, 1.0f, 0.0f);
	Single_axis_constraint = 1;
}

void CFREDView::OnUpdateConstrainZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(!Constraint.x && !Constraint.y && Constraint.z);
}

void CFREDView::OnConstrainXz() 
{
	vm_vec_make(&Constraint, 1.0f, 0.0f, 1.0f);
	vm_vec_make(&Anticonstraint, 0.0f, 1.0f, 0.0f);
	Single_axis_constraint = 0;
}

void CFREDView::OnUpdateConstrainXz(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(Constraint.x && !Constraint.y && Constraint.z);
}

void CFREDView::OnConstrainXy()
{
	vm_vec_make(&Constraint, 1.0f, 1.0f, 0.0f);
	vm_vec_make(&Anticonstraint, 0.0f, 0.0f, 1.0f);
	Single_axis_constraint = 0;
}

void CFREDView::OnUpdateConstrainXy(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(Constraint.x && Constraint.y && !Constraint.z);
}

void CFREDView::OnConstrainYz() 
{
	vm_vec_make(&Constraint, 0.0f, 1.0f, 1.0f);
	vm_vec_make(&Anticonstraint, 1.0f, 0.0f, 0.0f);
	Single_axis_constraint = 0;
}

void CFREDView::OnUpdateConstrainYz(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(!Constraint.x && Constraint.y && Constraint.z);
}

void CFREDView::OnSelectionLock() 
{
	Selection_lock = !Selection_lock;
}

void CFREDView::OnUpdateSelectionLock(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Selection_lock);
}

void CFREDView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDblClk(nFlags, point);
	if (Cursor_over != -1) {
		switch (Objects[Cursor_over].type) {
			case OBJ_SHIP:
			case OBJ_START:
				OnEditorsShips();
				break;

			case OBJ_WAYPOINT:
			case OBJ_JUMP_NODE:
				OnEditorsWaypoint();
				break;
		}

	} else if (Briefing_dialog)
		Fixed_briefing_size = !Fixed_briefing_size;
}

void CFREDView::OnDoubleFineGridlines() 
{
	double_fine_gridlines = !double_fine_gridlines;	
	maybe_create_new_grid(The_grid, &eye_pos, &eye_orient, 1);
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateDoubleFineGridlines(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(double_fine_gridlines);	
}

void CFREDView::OnShowDistances() 
{
	Show_distances = !Show_distances;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowDistances(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_distances);	
}

void CFREDView::OnUniversalHeading() 
{
	Universal_heading = !Universal_heading;	
}

void CFREDView::OnUpdateUniversalHeading(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Universal_heading);	
}

void CFREDView::OnFlyingControls() 
{
	Flying_controls_mode = !Flying_controls_mode;	
}

void CFREDView::OnUpdateFlyingControls(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Flying_controls_mode);	
}

void CFREDView::OnRotateLocally() 
{
	Group_rotate = !Group_rotate;	
}

void CFREDView::OnUpdateRotateLocally(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!Group_rotate);	
}

void CFREDView::OnSelectList() 
{
	ship_select dlg;

	dlg.DoModal();
}

// position camera to view all objects on the screen at once.  Doesn't change orientation.
void view_universe(int just_marked)
{
	int i, max = 0, flags[MAX_OBJECTS];
	float dist, largest = 20.0f;
	vector center, p1, p2;		// center of all the objects collectively
	vertex v;
	object *ptr;

	for (i=0; i<MAX_OBJECTS; i++)
		flags[i] = 0;

	if (just_marked)
		ptr = &Objects[cur_object_index];
	else
		ptr = GET_FIRST(&obj_used_list);

	p1.x = p2.x = ptr->pos.x;
	p1.y = p2.y = ptr->pos.y;
	p1.z = p2.z = ptr->pos.z;

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if (!just_marked || (ptr->flags & OF_MARKED)) {
			center = ptr->pos;
			if (center.x < p1.x)
				p1.x = center.x;
			if (center.x > p2.x)
				p2.x = center.x;
			if (center.y < p1.y)
				p1.y = center.y;
			if (center.y > p2.y)
				p2.y = center.y;
			if (center.z < p1.z)
				p1.z = center.z;
			if (center.z > p2.z)
				p2.z = center.z;
		}
		
		ptr = GET_NEXT(ptr);
	}

	vm_vec_avg(&center, &p1, &p2);
	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if (!just_marked || (ptr->flags & OF_MARKED)) {
			dist = vm_vec_dist_squared(&center, &ptr->pos);
			if (dist > largest)
				largest = dist;

			flags[OBJ_INDEX(ptr)] = 1;  // flag object as needing on-screen check
			if (OBJ_INDEX(ptr) > max)
				max = OBJ_INDEX(ptr);
		}

		ptr = GET_NEXT(ptr);
	}

	dist = fl_sqrt(largest) + 1.0f;
	vm_vec_scale_add(&view_pos, &center, &view_orient.fvec, -dist);
	g3_set_view_matrix(&view_pos, &view_orient, 0.5f);

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if (!just_marked || (ptr->flags & OF_MARKED)) {
			g3_rotate_vertex(&v, &ptr->pos);
			Assert(!(v.codes & CC_BEHIND));
			if (g3_project_vertex(&v) & PF_OVERFLOW)
				Int3();

			while (v.codes & CC_OFF) {  // is point off screen?
				dist += 5.0f;  // zoom out a little and check again.
				vm_vec_scale_add(&view_pos, &center, &view_orient.fvec, -dist);
				g3_set_view_matrix(&view_pos, &view_orient, 0.5f);
				g3_rotate_vertex(&v, &ptr->pos);
				if (g3_project_vertex(&v) & PF_OVERFLOW)
					Int3();
			}
		}

		ptr = GET_NEXT(ptr);
	}

	dist *= 1.1f;
	vm_vec_scale_add(&view_pos, &center, &view_orient.fvec, -dist);
	g3_set_view_matrix(&view_pos, &view_orient, 0.5f);
	Update_window = 1;
}

void CFREDView::cycle_constraint()
{
	if (Single_axis_constraint) {
		if (Constraint.x)
			OnConstrainY();
		else if (Constraint.y)
			OnConstrainZ();
		else if (Constraint.z)
			OnConstrainXz();

	} else {
		if (!Constraint.x)
			OnConstrainXy();
		else if (!Constraint.y)
			OnConstrainYz();
		else if (!Constraint.z)
			OnConstrainX();
	}
}

void CFREDView::OnZoomExtents() 
{
	view_universe();
}

void CFREDView::OnZoomSelected() 
{
	if (query_valid_object()) {
		if (Marked > 1)
			view_universe(1);
		else
			vm_vec_scale_add(&view_pos, &Objects[cur_object_index].pos, &view_orient.fvec, Objects[cur_object_index].radius * -3.0f);
	}

	Update_window = 1;
}

void CFREDView::OnUpdateZoomSelected(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(query_valid_object());
}

void CFREDView::OnFormWing() 
{
	if (!create_wing())
		FREDDoc_ptr->autosave("form wing");
}

void CFREDView::OnUpdateFormWing(CCmdUI* pCmdUI) 
{
	int count = 0;
	object *ptr;

	if (query_valid_object()) {
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (ptr->flags & OF_MARKED) {
				if (ptr->type == OBJ_SHIP)
					switch (ship_query_general_type(ptr->instance))
					{
						case SHIP_TYPE_FIGHTER_BOMBER:
						case SHIP_TYPE_CRUISER:
						case SHIP_TYPE_AWACS:
						case SHIP_TYPE_GAS_MINER:
						case SHIP_TYPE_CORVETTE:
						case SHIP_TYPE_FREIGHTER:
						case SHIP_TYPE_CAPITAL:
						case SHIP_TYPE_TRANSPORT:
							count++;
					}

				if (ptr->type == OBJ_START)
					count++;
			}

			ptr = GET_NEXT(ptr);
		}
	}

	pCmdUI->Enable(count > 0);
}

int query_single_wing_marked()
{
	int i, obj;

	if (!query_valid_object())
		return 0;

	if (cur_wing == -1)
		return 0;

	i = Wings[cur_wing].wave_count;
	if (Marked != i)  // does marked object count match number of ships in wing?
		return 0;

	while (i--) {
		obj = wing_objects[cur_wing][i];
		if ((Objects[obj].type != OBJ_SHIP) && (Objects[obj].type != OBJ_START))
			Error(LOCATION, "Invalid objects detected in wing \"%s\"", Wings[cur_wing].name);

//		if (Ships[Objects[obj].instance].wingnum != cur_wing)
//			return 0;
		Assert(Ships[Objects[obj].instance].wingnum == cur_wing);
		if (!(Objects[obj].flags & OF_MARKED))  // ensure all ships in wing.are marked
			return 0;
	}

	return 1;
}

void CFREDView::OnDisbandWing() 
{
	if (query_single_wing_marked()) {
		remove_wing(cur_wing);
		FREDDoc_ptr->autosave("wing disband");

	} else
		MessageBox("One and only one wing must be selected for this operation");
}

void CFREDView::OnUpdateDisbandWing(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(query_single_wing_marked());
}

void CFREDView::OnShowHorizon() 
{
	Show_horizon = !Show_horizon;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowHorizon(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_horizon);
}

void CFREDView::OnEditorsWing() 
{
	int adjust = 0;

	Assert(Wing_editor_dialog.GetSafeHwnd());
	if (!Show_sexp_help)
		adjust = -SEXP_HELP_BOX_SIZE;

	if (!theApp.init_window(&Wing_wnd_data, &Wing_editor_dialog, adjust))
		return;

	Wing_editor_dialog.SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Wing_editor_dialog.ShowWindow(SW_RESTORE);
}

void CFREDView::OnEditorsPlayer() 
{
	player_start_editor dlg;

	dlg.DoModal();
}

void CFREDView::OnEditorsOrient() 
{
	orient_editor dlg;

	dlg.DoModal();
}

void CFREDView::OnEditorsEvents() 
{
	if (Message_editor_dlg) {
		MessageBox("You must close the message editor before opening the event editor");
		return;
	}

	if (!Event_editor_dlg) {
		Event_editor_dlg = new event_editor;
		Event_editor_dlg->Create(event_editor::IDD);
	}

	Event_editor_dlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Event_editor_dlg->ShowWindow(SW_RESTORE);
}

void CFREDView::OnUpdateEditorsOrient(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(query_valid_object());
}

void CFREDView::OnEditorsMessage()
{
	if (Event_editor_dlg) {
		MessageBox("You must close the event editor before opening the message editor");
		return;
	}

	if (!Message_editor_dlg) {
		Message_editor_dlg = new CMessageEditorDlg;
		Message_editor_dlg->Create(CMessageEditorDlg::IDD);
	}

	Message_editor_dlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Message_editor_dlg->ShowWindow(SW_RESTORE);
}

void CFREDView::OnEditorsStarfield() 
{
	starfield_editor dlg;

	dlg.DoModal();
}

void CFREDView::place_background_bitmap(vector v)
{
}

void CFREDView::OnEditorsBgBitmaps()
{
	if (!Bg_bitmap_dialog) {
		Bg_bitmap_dialog = new bg_bitmap_dlg;
		Bg_bitmap_dialog->create();
	}

	Bg_bitmap_dialog->SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Bg_bitmap_dialog->ShowWindow(SW_RESTORE);
}

void CFREDView::OnEditorsReinforcement()
{
	reinforcement_editor_dlg dlg;

	dlg.DoModal();
}

void CFREDView::OnErrorChecker()
{
	int z;

	z = global_error_check();
	if (!z)
		MessageBox("No errors were detected in this mission", "Woohoo!");

	for (z=0; z<obj_count; z++)
		if (flags[z])
			delete [] names[z];

	obj_count = 0;
}

int CFREDView::global_error_check()
{
	char buf[256], *str;
	int bs, i, j, n, s, t, z, ai, count, ship, wing, obj, team, point, multi;
	object *ptr;
	brief_stage *sp;
	int starting_orders;

	g_err = multi = 0;
	if ( The_mission.game_type & MISSION_TYPE_MULTI )
		multi = 1;

//	if (!stricmp(The_mission.name, "Untitled"))
//		if (error("You haven't given this mission a title yet.\nThis is done from the Mission Specs Editor (Shift-N)."))
//			return 1;

	// cycle though all the objects and verify every possible aspect of them
	obj_count = t = 0;
	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		names[obj_count] = NULL;
		flags[obj_count] = 0;
		i = ptr->instance;
		if ((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)) {
			if (i < 0 || i >= MAX_SHIPS){
				return internal_error("An object has an illegal ship index");
			}

			z = Ships[i].ship_info_index;
			if ((z < 0) || (z >= Num_ship_types)){
				return internal_error("A ship has an illegal class");
			}

			if (ptr->type == OBJ_START) {
				t++;
				if (!(Ship_info[z].flags & SIF_PLAYER_SHIP)) {
					ptr->type = OBJ_SHIP;
					Player_starts--;
					t--;
					if (error("Invalid ship type for a player.  Ship has been reset to non-player ship.")){
						return 1;
					}
				}

				for (n=count=0; n<MAX_PRIMARY_BANKS; n++){
					if (Ships[i].weapons.primary_bank_weapons[n] >= 0){
						count++;
					}
				}

				if (!count){
					if (error("Player \"%s\" has no primary weapons.  Should have at least 1", Ships[i].ship_name)){
						return 1;
					}
				}

				for (n=count=0; n<MAX_SECONDARY_BANKS; n++){
					if (Ships[i].weapons.secondary_bank_weapons[n] >= 0){
						count++;
					}
				}

				if (!count){
					if (error("Player \"%s\" has no secondary weapons.  Should have at least 1", Ships[i].ship_name)){
						return 1;
					}
				}
			}

			if (Ships[i].objnum != OBJ_INDEX(ptr)){
				return internal_error("Object/ship references are corrupt");
			}

			names[obj_count] = Ships[i].ship_name;
			wing = Ships[i].wingnum;
			if (wing >= 0) {  // ship is part of a wing, so check this
				if (wing < 0 || wing >= MAX_WINGS){  // completely out of range?
					return internal_error("A ship has an illegal wing index");
				}

				j = Wings[wing].wave_count;
				if (!j){
					return internal_error("A ship is in a non-existant wing");
				}

				if (j < 0 || j > MAX_SHIPS_PER_WING){
					return internal_error("Invalid number of ships in wing \"%s\"", Wings[z].name);
				}

				while (j--){
					if (wing_objects[wing][j] == OBJ_INDEX(ptr)){  // look for object in wing's table
						break;
					}
				}

				if (j < 0){
					return internal_error("Ship/wing references are corrupt");
				}
			}

			if ( (Ships[i].flags & SF_KILL_BEFORE_MISSION) && (Ships[i].hotkey >= 0) ){
				if (error("Ship flagged as \"destroy before mission start\" has a hotkey assignment")){
					return 1;
				}
			}

			if ( (Ships[i].flags & SF_KILL_BEFORE_MISSION) && (ptr->type == OBJ_START) ){
				if (error("Player start flagged as \"destroy before mission start\"")){
					return 1;
				}
			}
		} else if (ptr->type == OBJ_WAYPOINT) {
			j = i / 65536;  // waypoint path number
			z = i & 0xffff;  // waypoint number in path
			if (j < 0 || j >= Num_waypoint_lists){
				return internal_error("Object references an illegal waypoint path number");
			}

			if (z >= Waypoint_lists[j].count){
				return internal_error("Object references an illegal waypoint number in path");
			}

			sprintf(buf, "%s:%d", Waypoint_lists[j].name, z + 1);
			names[obj_count] = new char[strlen(buf) + 1];
			strcpy(names[obj_count], buf);
			flags[obj_count] = 1;
		} else if (ptr->type == OBJ_POINT) {
			if (!Briefing_dialog){
				return internal_error("Briefing icon detected when not in briefing edit mode");
			}
		} else if (ptr->type == OBJ_JUMP_NODE) {
			if (i < 0 || i >= Num_jump_nodes){
				return internal_error("Object has illegal jump node index");
			}
		} else {
			return internal_error("An unknown object type (%d) was detected", ptr->type);
		}

		for (i=0; i<obj_count; i++){
			if (names[i] && names[obj_count]){
				if (!stricmp(names[i], names[obj_count])){
					return internal_error("Duplicate object names (%s)", names[i]);
				}
			}
		}
		
		obj_count++;
		ptr = GET_NEXT(ptr);
	}

	if (t != Player_starts){
		return internal_error("Total number of player ships is incorrect");
	}

	if (obj_count != num_objects){
		return internal_error("num_objects is incorrect");
	}

	count = 0;
	for (i=0; i<MAX_SHIPS; i++) {
		if (Ships[i].objnum >= 0) {  // is ship being used?
			count++;
			if (!query_valid_object(Ships[i].objnum)){
				return internal_error("Ship uses an unused object");
			}

			z = Objects[Ships[i].objnum].type;
			if ((z != OBJ_SHIP) && (z != OBJ_START)){
				return internal_error("Object should be a ship, but isn't");
			}

			if (fred_check_sexp(Ships[i].arrival_cue, OPR_BOOL, "arrival cue of ship \"%s\"", Ships[i].ship_name)){
				return -1;
			}

			if (fred_check_sexp(Ships[i].departure_cue, OPR_BOOL, "departure cue of ship \"%s\"", Ships[i].ship_name)){
				return -1;
			}

			if (Ships[i].arrival_location != ARRIVE_AT_LOCATION) {
				if (Ships[i].arrival_anchor < 0){
					if (error("Ship \"%s\" requires a valid arrival target", Ships[i].ship_name)){
						return 1;
					}
				}
			}

			if (Ships[i].departure_location != DEPART_AT_LOCATION) {
				if (Ships[i].departure_anchor < 0){
					if (error("Ship \"%s\" requires a valid departure target", Ships[i].ship_name)){
						return 1;
					}
				}
			}

			ai = Ships[i].ai_index;
			if (ai < 0 || ai >= MAX_AI_INFO){
				return internal_error("AI index out of range for ship \"%s\"", Ships[i].ship_name);
			}

			if (Ai_info[ai].shipnum != i){
				return internal_error("AI/ship references are corrupt");
			}

			if ((str = error_check_initial_orders(Ai_info[ai].goals, i, -1))>0) {
				if (*str == '*')
					return internal_error("Initial orders error for ship \"%s\"\n\n%s", Ships[i].ship_name, str + 1);
				else if (*str == '!')
					return 1;
				else if (error("Initial orders error for ship \"%s\"\n\n%s", Ships[i].ship_name, str))
					return 1;
			}

			obj = Ai_info[ai].dock_objnum;
			if (obj >= 0) {
				if (!query_valid_object(obj)){
					return internal_error("Ship \"%s\" initially docked with non-existant ship", Ships[i].ship_name);
				}

				if (Objects[obj].type != OBJ_SHIP && Objects[obj].type != OBJ_START){
					return internal_error("Ship \"%s\" initially docked with non-ship object", Ships[i].ship_name);
				}

				ship = get_ship_from_obj(obj);
				if (!ship_docking_valid(i, ship) && !ship_docking_valid(ship, i)){
					return internal_error("Docking illegal between \"%s\" and \"%s\" (initially docked)", Ships[i].ship_name, Ships[ship].ship_name);
				}

				z = get_docking_list(Ships[i].modelnum);
				point = Ai_info[ai].dock_index;
				if (point < 0 || point >= z){
					internal_error("Invalid docker point (\"%s\" initially docked with \"%s\")", Ships[i].ship_name, Ships[ship].ship_name);
				}

				z = get_docking_list(Ships[ship].modelnum);
				point = Ai_info[ai].dockee_index;
				if (point < 0 || point >= z){
					internal_error("Invalid dockee point (\"%s\" initially docked with \"%s\")", Ships[i].ship_name, Ships[ship].ship_name);
				}
			}
		}
	}

	if (count != ship_get_num_ships()){
		return internal_error("num_ships is incorrect");
	}

	count = 0;
	for (i=0; i<MAX_WINGS; i++) {
		team = -1;
		j = Wings[i].wave_count;
		if (j) {  // is wing being used?
			count++;
			if (j < 0 || j > MAX_SHIPS_PER_WING){
				return internal_error("Invalid number of ships in wing \"%s\"", Wings[i].name);
			}

			while (j--) {
				obj = wing_objects[i][j];
				if (obj < 0 || obj >= MAX_OBJECTS){
					return internal_error("Wing_objects has an illegal object index");
				}

				if (!query_valid_object(obj)){
					return internal_error("Wing_objects references an unused object");
				}

// Now, at this point, we can assume several things.  We have a valid object because
// we passed query_valid_object(), and all valid objects were already checked above,
// so this object has valid information, such as the instance.
				
				if ((Objects[obj].type == OBJ_SHIP) || (Objects[obj].type == OBJ_START)) {
					ship = Objects[obj].instance;
					sprintf(buf, "%s %d", Wings[i].name, j + 1);
					if (stricmp(buf, Ships[ship].ship_name)){
						return internal_error("Ship \"%s\" in wing should be called \"%s\"", Ships[ship].ship_name, buf);
					}

					switch (ship_query_general_type(ship))
					{
						case SHIP_TYPE_FIGHTER_BOMBER:
						case SHIP_TYPE_CRUISER:
						case SHIP_TYPE_AWACS:
						case SHIP_TYPE_GAS_MINER:
						case SHIP_TYPE_CORVETTE:
						case SHIP_TYPE_FREIGHTER:
						case SHIP_TYPE_CAPITAL:
						case SHIP_TYPE_TRANSPORT:
						case SHIP_TYPE_SUPERCAP:
							break;

						default:
							if (error("Ship \"%s\" is an illegal type to be in a wing", Ships[ship].ship_name)){
								return 1;
							}
					}
				} else {
					return internal_error("Wing_objects of \"%s\" references an illegal object type", Wings[i].name);
				}

				if (Ships[ship].wingnum != i){
					return internal_error("Wing/ship references are corrupt");
				}

				if (ship != Wings[i].ship_index[j]){
					return internal_error("Ship/wing references are corrupt");
				}

				if (team < 0){
					team = Ships[ship].team;
				} else if (team != Ships[ship].team && team < 999){
					if (error("ship teams mixed within same wing (\"%s\")", Wings[i].name)){
						return 1;
					}
				}
			}

			if ((Wings[i].special_ship < 0) || (Wings[i].special_ship >= Wings[i].wave_count)){
				return internal_error("Special ship out of range for \"%s\"", Wings[i].name);
			}

			if (Wings[i].num_waves < 0){
				return internal_error("Number of waves for \"%s\" is negative", Wings[i].name);
			}

			if ((Wings[i].threshold < 0) || (Wings[i].threshold >= Wings[i].wave_count)){
				return internal_error("Threshold for \"%s\" is invalid", Wings[i].name);
			}

			if (Wings[i].threshold + Wings[i].wave_count > MAX_SHIPS_PER_WING) {
				Wings[i].threshold = MAX_SHIPS_PER_WING - Wings[i].wave_count;
				if(error("Threshold for wing \"%s\" is higher than allowed.  Reset to %d", Wings[i].name, Wings[i].threshold)){
					return 1;
				}
			}

			for (j=0; j<obj_count; j++){
				if (names[j]){
					if (!stricmp(names[j], Wings[i].name)){
						return internal_error("Wing name is also used by an object (%s)", names[j]);
					}
				}
			}

			if(fred_check_sexp(Wings[i].arrival_cue, OPR_BOOL, "arrival cue of wing \"%s\"", Wings[i].name)){
				return -1;
			}

			if(fred_check_sexp(Wings[i].departure_cue, OPR_BOOL, "departure cue of wing \"%s\"", Wings[i].name)){
				return -1;
			}

			if (Wings[i].arrival_location != ARRIVE_AT_LOCATION) {
				if (Wings[i].arrival_anchor < 0)
					if (error("Wing \"%s\" requires a valid arrival target", Wings[i].name))
						return 1;
			}

			if (Wings[i].departure_location != DEPART_AT_LOCATION) {
				if (Wings[i].departure_anchor < 0)
					if (error("Wing \"%s\" requires a valid departure target", Wings[i].name))
						return 1;
			}

			if ((str = error_check_initial_orders(Wings[i].ai_goals, -1, i))>0) {
				if (*str == '*')
					return internal_error("Initial orders error for wing \"%s\"\n\n%s", Wings[i].name, str + 1);
				else if (*str == '!')
					return 1;
				else if (error("Initial orders error for wing \"%s\"\n\n%s", Wings[i].name, str))
					return 1;
			}

		}
	}

	if (count != num_wings){
		return internal_error("num_wings is incorrect");
	}

	for (i=0; i<Num_waypoint_lists; i++) {
		for (z=0; z<obj_count; z++){
			if (names[z]){
				if (!stricmp(names[z], Waypoint_lists[i].name)){
					return internal_error("Waypoint path name is also used by an object (%s)", names[z]);
				}
			}
		}

		j = Waypoint_lists[i].count;
		while (j--) {
			sprintf(buf, "%s:%d", Waypoint_lists[i].name, j + 1);
			for (z=0; z<obj_count; z++){
				if (names[z]){
					if (!stricmp(names[z], buf)){
						break;
					}
				}
			}
			
			if (z == obj_count){
				return internal_error("Waypoint \"%s\" not linked to an object", buf);
			}
		}
	}

	if (Player_starts > MAX_PLAYERS){
		return internal_error("Number of player starts exceeds max limit");
	}

	if (!multi && (Player_starts > 1)){
		if (error("Multiple player starts exist, but this is a single player mission")){
			return 1;
		}
	}

	if (Num_reinforcements > MAX_REINFORCEMENTS){
		return internal_error("Number of reinforcements exceeds max limit");
	}

	for (i=0; i<Num_reinforcements; i++) {
		z = 0;
		for (ship=0; ship<MAX_SHIPS; ship++){
			if ((Ships[ship].objnum >= 0) && !stricmp(Ships[ship].ship_name, Reinforcements[i].name)) {
				z = 1;
				break;
			}
		}

		for (wing=0; wing<MAX_WINGS; wing++){
			if (Wings[wing].wave_count && !stricmp(Wings[wing].name, Reinforcements[i].name)) {
				z = 1;
				break;
			}
		}

		if (!z){
			return internal_error("Reinforcement name not found in ships or wings");
		}
	}

/*	for (i=0; i<num_messages; i++) {
		if (Messages[i].num_times < 0)
			return internal_error("Number of times to play message is negative");

		z = Messages[i].who_from;
		if (z < -1 || z >= MAX_SHIPS)  // hacked!  -1 should be illegal..
			return internal_error("Message originator index is out of range");

		if (Ships[z].objnum == -1)
			return internal_error("Message originator points to nonexistant ship");

		if (fred_check_sexp(Messages[i].sexp, OPR_BOOL,
			"Message formula from \"%s\"", Ships[Messages[i].who_from].ship_name))
				return -1;
	}*/

	Assert((Player_start_shipnum >= 0) && (Player_start_shipnum < MAX_SHIPS) && (Ships[Player_start_shipnum].objnum >= 0));
	i = global_error_check_player_wings(multi);
	if (i){
		return i;
	}

	for (i=0; i<Num_mission_events; i++){
		if (fred_check_sexp(Mission_events[i].formula, OPR_NULL, "mission event \"%s\"", Mission_events[i].name)){
			return -1;
		}
	}

	for (i=0; i<Num_goals; i++){
		if (fred_check_sexp(Mission_goals[i].formula, OPR_BOOL, "mission goal \"%s\"", Mission_goals[i].name)){
			return -1;
		}
	}

	for ( bs = 0; bs < Num_teams; bs++ ) {
		for (s=0; s<Briefings[bs].num_stages; s++) {
			sp = &Briefings[bs].stages[s];
			t = sp->num_icons;
			for (i=0; i<t-1; i++){
				for (j=i+1; j<t; j++) {
					if ((sp->icons[i].id > 0) && (sp->icons[i].id == sp->icons[j].id)){
						if (error("Duplicate icon IDs %d in briefing stage %d", sp->icons[i].id, s + 1)){
							return 1;
						}
					}
				}
			}
		}
	}

	for ( j = 0; j < Num_teams; j++ ) {
		for (i=0; i<Debriefings[j].num_stages; i++) {
			if (fred_check_sexp(Debriefings[j].stages[i].formula, OPR_BOOL, "debriefing stage %d", i + 1)){
				return -1;
			}
		}
	}

	// for all wings, be sure that the orders accepted for all ships are the same for all ships
	// in the wing
	starting_orders = -1;
	for (i=0; i<MAX_WINGS; i++) {
		int default_orders, starting_wing;

		if ( !Wings[i].wave_count ){
			continue;
		}
		
		// determine if this wing is a starting wing of the player
		starting_wing = 1;
		for ( j = 0; j < MAX_STARTING_WINGS; j++ ) {
			if ( !stricmp( Wings[i].name, Starting_wing_names[j]) ){
				break;
			}
		}
		if ( j == MAX_STARTING_WINGS ){
			starting_wing = 0;
		}

		// first, be sure this isn't a reinforcement wing.
		if ( starting_wing && (Wings[i].flags & WF_REINFORCEMENT) ) {
			if ( error("Starting Wing %s marked as reinforcement.  This wing\nshould either be renamed, or unmarked as reinforcement.", Wings[i].name) ){
				return 1;
			}
		}

		default_orders = 0;
		for ( j = 0; j < Wings[i].wave_count; j++ ) {
			int orders;

			orders = Ships[Wings[i].ship_index[j]].orders_accepted;
			if ( j == 0 ) {
				default_orders = orders;
			} else if ( default_orders != orders ) {
				if (error("Wing %s has ships with different player orders which\nare ignored.  They must all be the same", Wings[i].name ) ){
					return 1;
				}
			}
		}

		// make sure that these ignored orders are the same for all starting wings of the player
		if ( starting_wing ) {
			if ( starting_orders == -1 ) {
				starting_orders = default_orders;
			} else {
				if ( starting_orders != default_orders ) {
					if ( error("Player starting wing %s has orders which don't match other starting wings\n", Wings[i].name) ){
						return 1;
					}
				}
			}
		}
	}

	if (Num_jump_nodes < 0 || Num_jump_nodes > MAX_JUMP_NODES){
		return internal_error("Jump node count is illegal");
	}

	fred_check_message_personas();

	return g_err;
}

int CFREDView::global_error_check_mixed_player_wing(int w)
{
	int i, s, species = -1, mixed = 0;

	for (i=0; i<Wings[w].wave_count; i++) {
		s = Wings[w].ship_index[i];
		if (species < 0)
			species = Ship_info[Ships[s].ship_info_index].species;
		else if (Ship_info[Ships[s].ship_info_index].species != species)
			mixed = 1;
	}

	if (mixed)
		if (error("%s wing must all be of the same species", Wings[w].name))
			return 1;

	return 0;
}

int CFREDView::global_error_check_player_wings(int multi)
{
	int i, z, alpha, beta, gamma, zeta, err, alpha_count, zeta_count;
	object *ptr;

	z = Ships[Player_start_shipnum].wingnum;
	alpha = wing_name_lookup("alpha", 1);
	beta = wing_name_lookup("beta", 1);
	gamma = wing_name_lookup("gamma", 1);
	zeta = wing_name_lookup( "zeta", 1 );

	if (alpha >= 0) {
		free_sexp2(Wings[alpha].arrival_cue);
		Wings[alpha].arrival_cue = Locked_sexp_true;
	}

	if (multi && (alpha < 0)){
		if (error("Alpha wing is required for multiplayer missions")){
			return 1;
		}
	}

	// Check to be sure that any player starting wing doesn't have > 1 wave for multiplayer
	if ( multi ) {
		if ( The_mission.game_type & MISSION_TYPE_MULTI_TEAMS ) {
			if ( alpha && (Wings[alpha].num_waves > 1) ) {
				error("Alpha wing must contain only 1 wave.\nThis change has been made for you.");
				Wings[alpha].num_waves = 1;
				return 1;
			}
			if ( zeta && (Wings[zeta].num_waves > 1) ) {
				error("Zeta wing must contain only 1 wave.\nThis change has been made for you.");
				Wings[zeta].num_waves = 1;
				return 1;
			}

		} else {
			if ( alpha && (Wings[alpha].num_waves > 1) ) {
				error("Alpha wing must contain only 1 wave.\nThis change has been made for you.");
				Wings[alpha].num_waves = 1;
				return 1;
			}
			if ( beta && (Wings[beta].num_waves > 1) ) {
				error("Beta wing must contain only 1 wave.\nThis change has been made for you.");
				Wings[beta].num_waves = 1;
				return 1;
			}
			if ( gamma && (Wings[gamma].num_waves > 1) ) {
				error("Gamma wing must contain only 1 wave.\nThis change has been made for you.");
				Wings[gamma].num_waves = 1;
				return 1;
			}
		}
	}

	// if not a multiplayer misison, or a coop multiplayer mission, then do "normal"
	// wing name checking.
	if ( !multi || (The_mission.game_type & MISSION_TYPE_MULTI_COOP) ) {
		if (((alpha >= 0) || (z >= 0)) && (alpha != z)){
			if (error("Player start is not in Alpha wing")){
				return 1;
			}
		}

		if ((beta >= 0) && (alpha < 0)){
			if (error("Alpha wing required, but not present")){
				return 1;
			}
		}

		if ((gamma >= 0) && (beta < 0)){
			if (error("Beta wing required, but not present")){
				return 1;
			}
		}

		if ((alpha >= 0) && (Wings[alpha].wave_count > 4)){
			if (error("Alpha wing has too many ships.  Should only have 4 max.")){
				return 1;
			}
		}

		if ((beta >= 0) && (Wings[beta].wave_count > 4)){
			if (error("Beta wing has too many ships.  Should only have 4 max.")){
				return 1;
			}
		}

		if ((gamma >= 0) && (Wings[gamma].wave_count > 4)){
			if (error("Gamma wing has too many ships.  Should only have 4 max.")){
				return 1;
			}
		}

		if ((alpha >= 0) && Wings[alpha].arrival_delay){
			if (error("Alpha wing shouldn't have a non-zero arrival delay")){
				return 1;
			}
		}
	} else if ( The_mission.game_type & MISSION_TYPE_MULTI_TEAMS ) {
		if ( zeta == -1 ){
			if (error("Zeta wing is required for multiplayer team vs. team missions")){
				return 1;
			}
		}

		if ( Wings[alpha].wave_count > 4 ){
			if (error("Alpha wing has too many ships.  Should only have 4 max.")){
				return 1;
			}
		}

		if ( Wings[zeta].wave_count > 4 ) {
			if (error("Zeta wing has too many ships.  Should only have 4 max.")){
				return 1;
			}
		}

		if ((alpha >= 0) && Wings[alpha].arrival_delay){
			if (error("Alpha wing shouldn't have a non-zero arrival delay")){
				return 1;
			}
		}

		if ((zeta >= 0) && Wings[zeta].arrival_delay){
			if (error("Zeta wing shouldn't have a non-zero arrival delay")){
				return 1;
			}
		}
	} else if ( The_mission.game_type & MISSION_TYPE_MULTI_DOGFIGHT ) {
		if ( Wings[alpha].wave_count > 4 ) {
			if (error("Alpha wing has too many ships.  Should only have 4 max.")){
				return 1;
			}
		}

		if ((alpha >= 0) && Wings[alpha].arrival_delay){
			if (error("Alpha wing shouldn't have a non-zero arrival delay")){
				return 1;
			}
		}
	} else {
		error("Unknown game type: %d", The_mission.game_type);
		return 1;
	}

	if (multi) {
		if (alpha >= 0){
			if (global_error_check_mixed_player_wing(alpha)){
				return 1;
			}
		}

		if (beta >= 0){
			if (global_error_check_mixed_player_wing(beta)){
				return 1;
			}
		}

		if (gamma >= 0){
			if (global_error_check_mixed_player_wing(gamma)){
				return 1;
			}
		}

		if (zeta >= 0){
			if (global_error_check_mixed_player_wing(zeta)){
				return 1;
			}
		}
	}

	alpha_count = zeta_count = 0;
	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		i = ptr->instance;
		err = 0;
		if (ptr->type == OBJ_START) {
			z = Ships[i].wingnum;
			if (z < 0) {
				err = 1;
			} else {
				if (z == alpha){
					alpha_count++;
				} else if (z == zeta){
					zeta_count++;
				}

				if (The_mission.game_type & MISSION_TYPE_MULTI_TEAMS) {
					if ((z != alpha) && (z != zeta)){
						err = 1;
					}
				} else {
					if ((z != alpha) && (z != beta) && (z != gamma)){
						err = 1;
					}
				}
			}

			if (err) {
				if (The_mission.game_type & MISSION_TYPE_MULTI_TEAMS) {
					if (error("Player \"%s\" should be part Alpha or Zeta wing", Ships[i].ship_name)){
						return 1;
					}
				} else {
					if (error("Player \"%s\" should be part Alpha, Beta or Gamma wing", Ships[i].ship_name)){
						return 1;
					}
				}
			}
		}

		ptr = GET_NEXT(ptr);
	}

	if ((alpha >= 0) && !alpha_count){
		if (error("Alpha wing doesn't contain any players, which it should.")){
			return 1;
		}
	}

	if (The_mission.game_type & MISSION_TYPE_MULTI_TEAMS){
		if ((zeta >= 0) && !zeta_count){
			if (error("Zeta wing doesn't contain any players, which it should.")){
				return 1;
			}
		}
	}

	return 0;
}

int CFREDView::error(char *msg, ...)
{
	char buf[2048];
	va_list args;

	va_start(args, msg);
	vsprintf(buf, msg, args);
	va_end(args);

	g_err = 1;
	if (MessageBox(buf, "Error", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK)
		return 0;

	return 1;
}

int CFREDView::internal_error(char *msg, ...)
{
	char buf[2048];
	va_list args;

	va_start(args, msg);
	vsprintf(buf, msg, args);
	va_end(args);

	g_err = 1;

#ifndef NDEBUG
	char buf2[2048];

	sprintf(buf2, "%s\n\nThis is an internal error.  Please let Jason\n"
		"know about this so he can fix it.  Click cancel to debug.", buf);

	if (MessageBox(buf2, "Internal Error", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
		Int3();  // drop to debugger so the problem can be analyzed.

#else
	MessageBox(buf, "Error", MB_OK | MB_ICONEXCLAMATION);
#endif

	return -1;
}

int CFREDView::fred_check_sexp(int sexp, int type, char *msg, ...)
{
	char buf[512], buf2[2048], buf3[4096];
	int err = 0, z, faulty_node;
	va_list args;

	va_start(args, msg);
	vsprintf(buf, msg, args);
	va_end(args);

	if (sexp == -1)
		return 0;

	z = check_sexp_syntax(sexp, type, 1, &faulty_node);
	if (!z)
		return 0;

	convert_sexp_to_string(sexp, buf2, SEXP_ERROR_CHECK_MODE);
	sprintf(buf3, "Error in %s: %s\n\nIn sexpression: %s\n(Error appears to be: %s)",
		buf, sexp_error_message(z), buf2, Sexp_nodes[faulty_node].text);

	if (z < 0 && z > -100)
		err = 1;

	if (err)
		return internal_error(buf3);

	if (error(buf3))
		return 1;

	return 0;
}

void CFREDView::OnEditorsWaypoint() 
{
	int adjust = 0;

	Assert(Waypoint_editor_dialog.GetSafeHwnd());
	if (!Show_sexp_help)
		adjust = -SEXP_HELP_BOX_SIZE;

	if (!theApp.init_window(&Waypoint_wnd_data, &Waypoint_editor_dialog))
		return;

	Waypoint_editor_dialog.SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Waypoint_editor_dialog.ShowWindow(SW_RESTORE);
}

char *error_check_initial_orders(ai_goal *goals, int ship, int wing)
{
	char *source;
	int i, j, num, flag, found, inst, team, team2;
	object *ptr;

	if (ship >= 0) {
		source = Ships[ship].ship_name;
		team = Ships[ship].team;
		for (i=0; i<MAX_AI_GOALS; i++)
			if (!ai_query_goal_valid(ship, goals[i].ai_mode)) {
				if (Fred_view_wnd->error("Order \"%s\" isn't allowed for ship \"%s\"", get_order_name(goals[i].ai_mode), source))
					return "!";
			}

	} else {
		Assert(wing >= 0);
		Assert(Wings[wing].wave_count > 0);
		source = Wings[wing].name;
		team = Ships[Objects[wing_objects[wing][0]].instance].team;
		for (j=0; j<Wings[wing].wave_count; j++)
			for (i=0; i<MAX_AI_GOALS; i++)
				if (!ai_query_goal_valid(Wings[wing].ship_index[j], goals[i].ai_mode)) {
					if (Fred_view_wnd->error("Order \"%s\" isn't allowed for ship \"%s\"", get_order_name(goals[i].ai_mode),
						Ships[Wings[wing].ship_index[j]].ship_name))
							return "!";
				}
	}

	for (i=0; i<MAX_AI_GOALS; i++) {
		switch (goals[i].ai_mode) {
			case AI_GOAL_NONE:
			case AI_GOAL_CHASE_ANY:
			case AI_GOAL_UNDOCK:
			case AI_GOAL_KEEP_SAFE_DISTANCE:
			case AI_GOAL_PLAY_DEAD:
			case AI_GOAL_WARP:
				flag = 0;
				break;

			case AI_GOAL_WAYPOINTS:
			case AI_GOAL_WAYPOINTS_ONCE:
				flag = 1;
				break;

			case AI_GOAL_DOCK:
				if (ship < 0)
					return "Wings can't dock";
				// fall through..

			case AI_GOAL_DESTROY_SUBSYSTEM:
			case AI_GOAL_CHASE:
			case AI_GOAL_GUARD:
			case AI_GOAL_DISARM_SHIP:
			case AI_GOAL_DISABLE_SHIP:
			case AI_GOAL_EVADE_SHIP:
			case AI_GOAL_STAY_NEAR_SHIP:
			case AI_GOAL_IGNORE:
				flag = 2;
				break;

			case AI_GOAL_CHASE_WING:
			case AI_GOAL_GUARD_WING:
				flag = 3;
				break;

			case AI_GOAL_STAY_STILL:
				flag = 4;
				break;

			default:
				return "*Invalid goal type";
		}

		found = 0;
		if (flag > 0) {
			if (*goals[i].ship_name == '<')
				return "Invalid target";

			if (!stricmp(goals[i].ship_name, source))
				if (ship >= 0)
					return "Target of ship's goal is itself";
				else
					return "Target of wing's goal is itself";
		}

		inst = team2 = -1;
		if (flag == 1) {  // target waypoint required
			for (j=0; j<Num_waypoint_lists; j++)
				if (!stricmp(goals[i].ship_name, Waypoint_lists[j].name))
					found = 1;

			if (!found)
				return "*Invalid target waypoint path name";

		} else if (flag == 2) {  // target ship required
			ptr = GET_FIRST(&obj_used_list);
			while (ptr != END_OF_LIST(&obj_used_list)) {
				if (ptr->type == OBJ_SHIP || ptr->type == OBJ_START) {
					inst = ptr->instance;
					if (!stricmp(goals[i].ship_name, Ships[inst].ship_name)) {
						found = 1;
						break;
					}
				}

				ptr = GET_NEXT(ptr);
			}

			if (!found)
				return "*Invalid target ship name";

			if (wing >= 0) {  // check if target ship is in wing
				if (Ships[inst].wingnum == wing && Objects[Ships[inst].objnum].type != OBJ_START)
					return "Target ship of wing's goal is within said wing";
			}

			team2 = Ships[inst].team;

		} else if (flag == 3) {  // target wing required
			for (j=0; j<MAX_WINGS; j++)
				if (Wings[j].wave_count && !stricmp(Wings[j].name, goals[i].ship_name))
					break;

			if (j >= MAX_WINGS)
				return "*Invalid target wing name";

			if (ship >= 0) {  // check if ship is in target wing
				if (Ships[ship].wingnum == j)
					return "Target wing of ship's goal is same wing said ship is part of";
			}

			team2 = Ships[Objects[wing_objects[j][0]].instance].team;

		} else if (flag == 4) {
			ptr = GET_FIRST(&obj_used_list);
			while (ptr != END_OF_LIST(&obj_used_list)) {
				if (ptr->type == OBJ_SHIP || ptr->type == OBJ_START) {
					inst = ptr->instance;
					if (!stricmp(goals[i].ship_name, Ships[inst].ship_name)) {
						found = 2;
						break;
					}

				} else if (ptr->type == OBJ_WAYPOINT) {
					if (!stricmp(goals[i].ship_name, object_name(OBJ_INDEX(ptr)))) {
						found = 1;
						break;
					}
				}

				ptr = GET_NEXT(ptr);
			}

			if (!found)
				return "*Invalid target ship or waypoint name";

			if (found == 2) {
				if (wing >= 0) {  // check if target ship is in wing
					if (Ships[inst].wingnum == wing && Objects[Ships[inst].objnum].type != OBJ_START)
						return "Target ship of wing's goal is within said wing";
				}

				team2 = Ships[inst].team;
			}
		}

		switch (goals[i].ai_mode) {
			case AI_GOAL_DESTROY_SUBSYSTEM:
				Assert(flag == 2 && inst >= 0);
				if (ship_get_subsys_index(&Ships[inst], goals[i].docker.name, 1) < 0)
					return "Unknown subsystem type";

				break;

			case AI_GOAL_DOCK: {
				int dock1 = -1, dock2 = -1, model1, model2;

				Assert(flag == 2 && inst >= 0);
				if (!ship_docking_valid(ship, inst))
					return "Docking illegal between given ship types";

				model1 = Ships[ship].modelnum;
				num = get_docking_list(model1);
				for (j=0; j<num; j++) {
					Assert(Docking_bay_list[j]);
					if (!stricmp(goals[i].docker.name, Docking_bay_list[j])) {
						dock1 = j;
						break;
					}
				}

				model2 = Ships[inst].modelnum;
				num = get_docking_list(model2);
				for (j=0; j<num; j++) {
					Assert(Docking_bay_list[j]);
					if (!stricmp(goals[i].dockee.name, Docking_bay_list[j])) {
						dock2 = j;
						break;
					}
				}

				if (dock1 < 0)
					return "Invalid docker point";

				if (dock2 < 0)
					return "Invalid dockee point";

				if ((dock1 >= 0) && (dock2 >= 0)) {
					if ( !(model_get_dock_index_type(model1, dock1) & model_get_dock_index_type(model2, dock2)) )
						return "Dock points are incompatible";
				}

				break;
			}
		}

		switch (goals[i].ai_mode) {
			case AI_GOAL_GUARD:
			case AI_GOAL_GUARD_WING:
				// if ((team == TEAM_HOSTILE && team2 == TEAM_FRIENDLY) || (team == TEAM_FRIENDLY && team2 == TEAM_HOSTILE)) {
				if (team != team2) {	//	MK, added support for TEAM_NEUTRAL.  Won't this work?
					if (ship >= 0)
						return "Ship assigned to guard opposite team";
					else
						return "Wing assigned to guard opposite team";
				}

				break;

			case AI_GOAL_CHASE:
			case AI_GOAL_CHASE_WING:
			case AI_GOAL_DESTROY_SUBSYSTEM:
			case AI_GOAL_DISARM_SHIP:
			case AI_GOAL_DISABLE_SHIP:
				if (team == team2) {
					if (ship >= 0)
						return "Ship assigned to attack same team";
					else
						return "Wings assigned to attack same team";
				}
					
				break;
		}
	}

	return NULL;
}

// function (which is called externally from message editor and event editor) so skip through
// the sexpression nodes to look for send-message commands to try to associate message personas
// to ships
void fred_check_message_personas()
{
/*
	int i, op, j, ship_index;
	char *mname, *who_from;
	object *objp;

	// this function is responsible for assigning personas to ships as well as error checking them.
	// clear out the persona index on all ship objects
	for ( objp = GET_FIRST(&obj_used_list); objp != END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
		if ( objp->type == OBJ_SHIP ) {
			Ships[objp->instance].persona_index = -1;
		}
	}


	for (i = 0; i < MAX_SEXP_NODES; i++ ) {
		if ( Sexp_nodes[i].type == SEXP_NOT_USED )
			continue;

		// look for only operator nodes
		if ( Sexp_nodes[i].subtype != SEXP_ATOM_OPERATOR )
			continue;

		// now look for the send-message opeator
		op = find_operator( Sexp_nodes[i].text );
		if ( op != OP_SEND_MESSAGE )
			continue;

		// have the message.  parse through the message to determine who is sending the message.
		who_from = CTEXT(CDR(i));

		// we can ignore messages from any wingman, and allied, or from God.
		if ( !stricmp(who_from, "<Any wingman>") || !stricmp(who_from, "<Any allied>") || (who_from[0] == '#') )
			continue;

		mname = CTEXT(CDR(CDR(CDR(i))));

		// check to see if who_from is a wing.  Don't do processing if so.
		if ( wing_name_lookup(who_from, 1) != -1 )
			continue;

		ship_index = ship_name_lookup( who_from );
		if ( ship_index == -1 ) {
			Int3();			// get allender.  something funny is up with shipnames in send-message
			continue;
		}

		for ( j = Num_builtin_messages; j < Num_messages; j++ ) {
			if ( !stricmp(mname, Messages[j].name) ) {

				// check to see if there is a persona for this message -- if not, bail
				if ( Messages[j].persona_index == -1 )
					break;

				// if a ship isn't assigned a persona, and this message says that he is, assign it, and move on
				if ( Ships[ship_index].persona_index == -1 ) {
					Ships[ship_index].persona_index = Messages[j].persona_index;
					continue;
				}

				// we must be sure of the following conditions:
				// 1) a ship isn't assigned > 1 persona
				
				if ( Ships[ship_index].persona_index != Messages[j].persona_index )
					Fred_view_wnd->error("Ship %s has at least two personas attached to it:\n%s and %s", Ships[ship_index].ship_name, Personas[Ships[ship_index].persona_index].name, Personas[Messages[j].persona_index].name );
			}
		}
	}

	// check that two or more ships are not using the same persona
	for (i = 0; i < Num_personas; i++ ) {
		int persona_count;
		object *objp;

		// move through object list looking for number of shis using this persona
		persona_count = 0;
		for ( objp = GET_FIRST(&obj_used_list); objp != END_OF_LIST(&obj_used_list); objp = GET_NEXT(objp) ) {
			if ( objp->type != OBJ_SHIP )
				continue;
			if (Ships[objp->instance].persona_index == i )
				persona_count++;
		}

		if ( persona_count > 1 )
			Fred_view_wnd->error("Persona %s used by more than 1 ship", Personas[Messages[j].persona_index].name );
	}
*/

}

void CFREDView::OnViewOutlines() 
{
	Show_outlines = !Show_outlines;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateViewOutlines(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_outlines);
}

void CFREDView::OnUpdateNewShipType(CCmdUI* pCmdUI) 
{
	int z;
	CWnd *bar;

	z = m_new_ship_type_combo_box.GetCurSelNEW();
	if (z == CB_ERR)
		m_new_ship_type_combo_box.SetCurSelNEW(cur_model_index);
	else
		cur_model_index = z;

	bar = GetDlgItem(pCmdUI->m_nID);
	if (!bar) {
		pCmdUI -> ContinueRouting();
		return; // not for us
	}

	pCmdUI -> SetCheck((bar->GetStyle() & WS_VISIBLE) != 0);
}

void CFREDView::OnShowStarfield() 
{
	Show_stars = !Show_stars;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowStarfield(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_stars);
}

void CFREDView::OnAsteroidEditor() 
{
	asteroid_editor dlg;

	dlg.DoModal();
}

void CFREDView::OnRunFreespace() 
{
	BOOL r;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char *lpMsgBuf;

	if (!FREDDoc_ptr->SaveModified())
		return;

	si.cb = sizeof(si);
	si.lpReserved = NULL;
	si.lpDesktop = NULL;
	si.lpTitle = NULL;
	si.dwFlags = 0;
	si.cbReserved2 = 0;
	si.lpReserved2 = NULL;

	r = CreateProcess("..\\..\\Fs.exe", NULL, NULL, NULL, FALSE, 0, NULL, "..\\..", &si, &pi);
	if (!r) {
		FormatMessage(
			 FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			 NULL,
			 GetLastError(),
			 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			 (LPTSTR) &lpMsgBuf,
			 0,
			 NULL
		);

		// Display the string.
		MessageBox(lpMsgBuf);

		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
}

void CFREDView::OnEditorCampaign() 
{
	if (!FREDDoc_ptr->SaveModified())
		return;

	Assert(!Campaign_wnd);
	Campaign_wnd = new campaign_tree_wnd;
	if (Campaign_wnd->Create(NULL, "Campaign Editor", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
		CFrameWnd::rectDefault, NULL, "IDR_MENU_CAMPAIGN")) {
		Campaign_wnd->ShowWindow(SW_SHOW);
		Campaign_wnd->UpdateWindow();
	}
}

void CFREDView::OnShowShips() 
{
	Show_ships = !Show_ships;	
	correct_marking();
	Update_window = 1;
}

void CFREDView::OnUpdateShowShips(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_ships);
}

void CFREDView::OnShowStarts() 
{
	Show_starts = !Show_starts;
	correct_marking();
	Update_window = 1;
}

void CFREDView::OnUpdateShowStarts(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_starts);
}

void CFREDView::OnShowFriendly() 
{
	Show_friendly = !Show_friendly;
	correct_marking();
	Update_window = 1;
}

void CFREDView::OnUpdateShowFriendly(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_friendly);
}

void CFREDView::OnShowHostile() 
{
	Show_hostile = !Show_hostile;
	correct_marking();
	Update_window = 1;
}

void CFREDView::OnUpdateShowHostile(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_hostile);
}

void CFREDView::OnToggleViewpoint() 
{
	if (viewpoint || !query_valid_object())
		viewpoint = 0;

	else {
		viewpoint = 1;
		view_obj = cur_object_index;
	}

	Update_window = 1;
}

void CFREDView::OnRevert() 
{
	if (!FREDDoc_ptr->SaveModified())
		return;

	FREDDoc_ptr->DeleteContents();
	FREDDoc_ptr->OnOpenDocument(NULL);
}

void CFREDView::OnUpdateRevert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(*Mission_filename);
}

BOOL CFREDView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ((Cursor_over >= 0) || Selection_lock) {
		if (Editing_mode == 1) {
			SetCursor(h_cursor_move);
			return TRUE;

		} else if (Editing_mode == 2) {
			SetCursor(h_cursor_rotate);
			return TRUE;
		}
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CFREDView::OnHideObjects() 
{
	object *ptr;

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if (ptr->flags & OF_MARKED) {
			ptr->flags |= OF_HIDDEN;
			unmark_object(OBJ_INDEX(ptr));
		}

		ptr = GET_NEXT(ptr);
	}
}

void CFREDView::OnShowHiddenObjects() 
{
	object *ptr;

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		ptr->flags &= ~OF_HIDDEN;
		ptr = GET_NEXT(ptr);
	}

	Update_window = 1;
}

void CFREDView::OnEditUndo() 
{
	vector viewer_pos;
	matrix viewer_orient;

	if (Undo_available) {
		viewer_pos = view_pos;
		viewer_orient = view_orient;
		FREDDoc_ptr->autoload();
		view_pos = viewer_pos;
		view_orient = viewer_orient;
	}
}

void CFREDView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(Undo_available);
}

void CFREDView::OnEditorsBriefing() 
{
	if (!Briefing_dialog) {
		Briefing_dialog = new briefing_editor_dlg;
		Briefing_dialog->create();
	}

	Briefing_dialog->SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	Briefing_dialog->ShowWindow(SW_RESTORE);
}

void CFREDView::OnEditorsDebriefing() 
{
	debriefing_editor_dlg dlg;

	dlg.DoModal();
}

void CFREDView::OnSaveCamera() 
{
	saved_cam_pos = view_pos;
	saved_cam_orient = view_orient;
}

void CFREDView::OnRestoreCamera() 
{
	view_pos = saved_cam_pos;
	view_orient = saved_cam_orient;
	Update_window = 1;
}

void CFREDView::OnUpdateRestoreCamera(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IS_VEC_NULL(&saved_cam_orient.fvec));
}

void CFREDView::OnShowSexpHelp() 
{
	CRect rect;

	Show_sexp_help = !Show_sexp_help;
	Ship_editor_dialog.show_hide_sexp_help();
	Wing_editor_dialog.show_hide_sexp_help();

	if (Event_editor_dlg) {
		Event_editor_dlg->GetWindowRect(rect);
		if (Show_sexp_help)
			rect.bottom += SEXP_HELP_BOX_SIZE;
		else
			rect.bottom -= SEXP_HELP_BOX_SIZE;

		Event_editor_dlg->MoveWindow(rect);
	}
}

void CFREDView::OnUpdateShowSexpHelp(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_sexp_help);
}

void CFREDView::OnLookatObj() 
{
	Lookat_mode = !Lookat_mode;
	if (Lookat_mode && query_valid_object()) {
		vector v, loc;
		matrix m;

		loc = Objects[cur_object_index].pos;
		vm_vec_sub(&v, &loc, &view_pos);

		if (v.x || v.y || v.z) {
			vm_vector_2_matrix(&m, &v, NULL, NULL);
			view_orient = m;
		}
	}
}

void CFREDView::OnUpdateLookatObj(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Lookat_mode);
}

void CFREDView::OnGroup(UINT nID)
{
	int n = 1 << (nID - ID_GROUP1);
	object *objp;

	unmark_all();
	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list))	{
		if (objp->type == OBJ_SHIP) {
			if (Ships[objp->instance].group & n)
				mark_object(OBJ_INDEX(objp));
		}

		objp = GET_NEXT(objp);
	}

	Update_window = 1;
}

void CFREDView::OnSetGroup(UINT nID)
{
	int i, err = 0, n = 1 << (nID - ID_SET_GROUP1);
	object *objp;

	for (i=0; i<MAX_SHIPS; i++)
		Ships[i].group &= ~n;

	objp = GET_FIRST(&obj_used_list);
	while (objp != END_OF_LIST(&obj_used_list)) {
		if (objp->flags & OF_MARKED) {
			if (objp->type == OBJ_SHIP) {
				Ships[objp->instance].group |= n;

			} else
				err = 1;
		}

		objp = GET_NEXT(objp);
	}

	if (err)
		Fred_main_wnd->MessageBox("Only ships can be in groups, and not players or waypoints, etc.\n"
			"These illegal objects you marked were not placed in the group");

	Update_window = 1;
}

void CFREDView::OnInitialUpdate() 
{
	char *ptr, text[512];

	CView::OnInitialUpdate();
	
	// check the time/checksum strings.
	expire_game = 0;
	ptr = &stamp[0];
	if ( memcmp(ptr, DEFAULT_CHECKSUM_STRING, strlen(DEFAULT_CHECKSUM_STRING)) ) {
		int stamped_checksum, checksum;

		// the checksum is not the default checksum.  Calculate the checksum of the string
		// and compare it.
		memcpy(&stamped_checksum, ptr, sizeof(stamped_checksum) );
		ptr = &stamp[0];
		ptr += 8;			// get us to the actual string to calculate the checksum
		CALCULATE_STAMP_CHECKSUM();

		if ( checksum != stamped_checksum ){
			expire_game = EXPIRE_BAD_CHECKSUM;
		}

		// now check the time
		ptr = &stamp[0];
		ptr += 4;
		if ( memcmp( ptr, DEFAULT_TIME_STRING, strlen(DEFAULT_TIME_STRING)) ) {
			int expire_time, current_time;

			// not the default time -- check against the current time
			memcpy( &expire_time, ptr, sizeof(expire_time) );
			time( (long *)&current_time );
			if ( current_time > expire_time )
				expire_game = EXPIRE_BAD_TIME;
		}

		// since the default checksum has changed -- put up a message which shows who the program
		// is stamped for
		ptr = &stamp[0];
		ptr += 8;
		sprintf(text, "This version of Fred has been compiled for %s", ptr);
		MessageBox(text, NULL, MB_OK);

		if ( expire_game )
			SetTimer(1, FRED_EXPIRE_TIME, expire_game_proc);
	}
}

void CFREDView::OnEditorsAdjustGrid() 
{
	adjust_grid_dlg dlg;

	dlg.DoModal();
	Update_window = 1;
}

void CFREDView::OnEditorsShieldSys() 
{
	shield_sys_dlg dlg;

	dlg.DoModal();
}

void CFREDView::OnLevelObj() 
{
	level_controlled();
	Update_window = 1;
}

void CFREDView::OnAlignObj() 
{
	verticalize_controlled();
	Update_window = 1;
}

void CFREDView::OnControlObj() 
{
	Control_mode = (Control_mode + 1) % 2;
}

void CFREDView::OnNextObj() 
{	
	object *ptr;

	if (Bg_bitmap_dialog) {
		if (Cur_bitmap == -1)
		{
			if (Num_starfield_bitmaps)
			{
				Cur_bitmap = 0;
				Bg_bitmap_dialog -> update_data();
			}

			return;
		}

		Cur_bitmap++;
		if (Cur_bitmap >= Num_starfield_bitmaps)
			Cur_bitmap = 0;

		Bg_bitmap_dialog -> update_data();
		return;
	}

	if (EMPTY(&obj_used_list))
		return;

	if (query_valid_object())	{
		ptr = Objects[cur_object_index].next;
		if (ptr == END_OF_LIST(&obj_used_list))
			ptr = GET_NEXT(ptr);
	
	} else
		ptr = GET_FIRST(&obj_used_list);

	if (Marked > 1) {  // cycle through marked list
		while (!(ptr->flags & OF_MARKED))
		{
			ptr = GET_NEXT(ptr);
			if (ptr == END_OF_LIST(&obj_used_list))
				ptr = GET_NEXT(ptr);
		}

		set_cur_object_index(OBJ_INDEX(ptr));

	} else {
		if (Marked)
			unmark_object(cur_object_index);

		mark_object(OBJ_INDEX(ptr));
	}
}

void CFREDView::OnPrevObj() 
{
	int arr[MAX_OBJECTS], i = 0, n = 0;
	object *ptr;

	if (Bg_bitmap_dialog) {
		if (Cur_bitmap == -1)
		{
			if (Num_starfield_bitmaps)
			{
				Cur_bitmap = Num_starfield_bitmaps - 1;
				Bg_bitmap_dialog -> update_data();
			}

			return;
		}

		Cur_bitmap--;
		if (Cur_bitmap < 0)
			Cur_bitmap = Num_starfield_bitmaps - 1;

		Bg_bitmap_dialog -> update_data();
		return;
	}

	if (EMPTY(&obj_used_list))
		return;

	ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if (cur_object_index == OBJ_INDEX(ptr))
			i = n;

		arr[n++] = OBJ_INDEX(ptr);
		ptr = GET_NEXT(ptr);
	}

	Assert(n);
	if (query_valid_object()) {
		i--;
		if (i < 0)
			i = n - 1;
	
	} else
		i = n - 1;

	if (Marked > 1) {  // cycle through marked list
		while (!(Objects[i].flags & OF_MARKED))
		{
			i--;
			if (i < 0)
				i = n - 1;
		}

		set_cur_object_index(i);

	} else {
		if (Marked)
			unmark_object(cur_object_index);

		mark_object(i);
	}
}

void CFREDView::OnEditDelete() 
{
	if (!button_down && Marked) {
		delete_marked();
		FREDDoc_ptr->autosave("object delete");
	}

	Update_window = 2;  // For some strange reason, need to redraw twice for it to take.
}

void CFREDView::OnEditDeleteWing() 
{
	if (!button_down && (cur_wing >= 0)) {
		delete_wing();
		FREDDoc_ptr->autosave("wing delete");
		cur_wing = -1;
		if (!Marked)
			Ship_editor_dialog.initialize_data(1);

		Wing_editor_dialog.initialize_data(1);
	}

	Update_window = 2;  // For some strange reason, need to redraw twice for it to take.
}

void CFREDView::OnMarkWing() 
{
	int i, wing = cur_wing;

	if (wing != -1)
	{
		unmark_all();
		for (i=0; i<Wings[wing].wave_count; i++)
			mark_object(wing_objects[wing][i]);

		Assert(Wings[wing].special_ship >= 0 && Wings[wing].special_ship < Wings[wing].wave_count);
		set_cur_object_index(wing_objects[wing][Wings[wing].special_ship]);
	}
}

void CFREDView::OnUpdateControlObj(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Control_mode != 0);
}

void CFREDView::OnAaGridlines()
{
	Aa_gridlines = !Aa_gridlines;
}

void CFREDView::OnUpdateAaGridlines(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Aa_gridlines);
	Update_window = 1;
}

void CFREDView::OnCmdBrief() 
{
	cmd_brief_dlg dlg;

	dlg.DoModal();
	Update_window = 1;
}

void CFREDView::OnDisableUndo() 
{
	Autosave_disabled = !Autosave_disabled;
}

void CFREDView::OnUpdateDisableUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Autosave_disabled);
}

void CFREDView::OnUpdateCmdBrief(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!(The_mission.game_type & MISSION_TYPE_MULTI));
}

int get_visible_sub_system_count(ship *shipp)
{
	int count = 0;
	ship_subsys *cur_subsys;

	for (cur_subsys = GET_FIRST(&shipp->subsys_list); cur_subsys != END_OF_LIST(&shipp->subsys_list); cur_subsys = GET_NEXT(cur_subsys)) {
		if (cur_subsys->system_info->subobj_num != -1) {
			count++;
		}
	}

	return count;
}

int get_next_visible_subsys(ship *shipp, ship_subsys **next_subsys)
{
	int count = get_visible_sub_system_count(shipp);

	// return don't try to display
	if (count == 0) {
		return 0;
	}

	// first timer
	if (*next_subsys == NULL) {
		*next_subsys = &shipp->subsys_list;
	}

	// look before wrap
	for (*next_subsys = GET_NEXT(*next_subsys); *next_subsys != END_OF_LIST(&shipp->subsys_list); *next_subsys = GET_NEXT(*next_subsys)) {
		if ((*next_subsys)->system_info->subobj_num != -1) {
			Update_window = 1;
			return 1;
		}
	}

	// look for first after wrap
	for (*next_subsys = GET_FIRST(&shipp->subsys_list); *next_subsys != END_OF_LIST(&shipp->subsys_list); *next_subsys = GET_NEXT(*next_subsys)) {
		if ((*next_subsys)->system_info->subobj_num != -1) {
			Update_window = 1;
			return 1;
		}
	}

	Int3();	// should be impossible to miss
	return 0;
}

int get_prev_visible_subsys(ship *shipp, ship_subsys **prev_subsys)
{
	int count = get_visible_sub_system_count(shipp);

	// return don't try to display
	if (count == 0) {
		return 0;
	}

	// first timer
	Assert(*prev_subsys != NULL);

	// look before wrap
	for (*prev_subsys = GET_PREV(*prev_subsys); *prev_subsys != END_OF_LIST(&shipp->subsys_list); *prev_subsys = GET_PREV(*prev_subsys)) {
		if ((*prev_subsys)->system_info->subobj_num != -1) {
			Update_window = 1;
			return 1;
		}
	}

	// look for first after wrap
	for (*prev_subsys = GET_LAST(&shipp->subsys_list); *prev_subsys != END_OF_LIST(&shipp->subsys_list); *prev_subsys = GET_PREV(*prev_subsys)) {
		if ((*prev_subsys)->system_info->subobj_num != -1) {
			Update_window = 1;
			return 1;
		}
	}

	Int3();	// should be impossible to miss
	return 0;
}

// update next subsystem to view
void CFREDView::OnNextSubsys() 
{
	object *objp;

	if (cur_object_index < 0) {
		OnCancelSubsys();
	}

	objp = &Objects[cur_object_index];

	// check if cur object is ship type
	if (objp->type == OBJ_SHIP) {

		// check if same ship
		if (Render_subsys.ship_obj == objp) {

			// if already on, advance to next
			if (Render_subsys.do_render) {
				if ( !get_next_visible_subsys(&Ships[objp->instance], &Render_subsys.cur_subsys) ) {
					OnCancelSubsys();
				}
			} else {
				Int3();
			}
		} else {
			// clean up
			OnCancelSubsys();

			// set up new and advance to first
			Render_subsys.do_render = true;
			Render_subsys.ship_obj = objp;
			if ( !get_next_visible_subsys(&Ships[objp->instance], &Render_subsys.cur_subsys) ) {
				OnCancelSubsys();
			}
		}
	} else {
		// not ship type
		OnCancelSubsys();
	}
}

void CFREDView::OnPrevSubsys() 
{
	if (!Render_subsys.do_render) {
		return;
	}

	if ( (cur_object_index < 0)  || (Objects[cur_object_index].type != OBJ_SHIP) || (&Objects[cur_object_index] != Render_subsys.ship_obj) ) {
		OnCancelSubsys();
		return;
	}

	if ( !get_prev_visible_subsys(&Ships[Objects[cur_object_index].instance], &Render_subsys.cur_subsys) ) {
		OnCancelSubsys();
	}

}

void CFREDView::OnCancelSubsys()
{
	Render_subsys.do_render = false;
	Render_subsys.ship_obj = NULL;
	Render_subsys.cur_subsys = NULL;
	Update_window = 1;
}

void CFREDView::OnShowPaths()
{
	Show_paths_fred = !Show_paths_fred;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowPaths(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Show_paths_fred);
}

void CFREDView::OnShowDockPoints()
{
	Show_dock_points = !Show_dock_points;
	theApp.write_ini_file();
	Update_window = 1;
}

void CFREDView::OnUpdateShowDockPoints(CCmdUI* pCmdUI)
{	
	pCmdUI->SetCheck(Show_dock_points);
}

void CFREDView::OnDumpStats()
{
	DumpStats dlg;

	dlg.DoModal();
}	
