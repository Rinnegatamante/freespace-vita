/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Fred2/MissionSave.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Mission saving in Fred.
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 33    8/28/99 7:29p Dave
 * Fixed wingmen persona messaging. Make sure locked turrets don't count
 * towards the # attacking a player.
 * 
 * 32    8/27/99 12:04a Dave
 * Campaign loop screen.
 * 
 * 31    8/26/99 8:52p Dave
 * Gave multiplayer TvT messaging a heavy dose of sanity. Cheat codes.
 * 
 * 30    8/17/99 5:20p Andsager
 * Control campaign editor bug.
 * 
 * 29    8/16/99 3:53p Andsager
 * Add special warp in interface in Fred and saving / reading.
 * 
 * 28    7/02/99 4:30p Dave
 * Much more sophisticated lightning support.
 * 
 * 27    6/10/99 11:06a Andsager
 * Mission designed selection of asteroid types.
 * 
 * 26    6/03/99 6:37p Dave
 * More TNT fun. Made perspective bitmaps more flexible.
 * 
 * 25    5/20/99 6:59p Dave
 * Added alternate type names for ships. Changed swarm missile table
 * entries.
 * 
 * 24    5/09/99 6:00p Dave
 * Lots of cool new effects. E3 build tweaks.
 * 
 * 23    4/26/99 8:47p Dave
 * Made all pof related nebula stuff customizable through Fred.
 * 
 * 22    4/26/99 12:49p Andsager
 * Add protect object from beam support to Fred
 * 
 * 21    4/16/99 2:34p Andsager
 * Second pass on debris fields
 * 
 * 20    4/15/99 5:00p Andsager
 * Frist pass on Debris field
 * 
 * 19    4/07/99 6:21p Dave
 * Fred and Freespace support for multiple background bitmaps and suns.
 * Fixed link errors on all subprojects. Moved encrypt_init() to
 * cfile_init() and lcl_init(), since its safe to call twice.
 * 
 * 18    3/31/99 9:50a Andsager
 * Interface for generalization of asteroid field (debris field)
 * 
 * 17    3/24/99 4:05p Dave
 * Put in support for assigning the player to a specific squadron with a
 * specific logo. Preliminary work for doing pos/orient checksumming in
 * multiplayer to reduce bandwidth.
 * 
 * 16    3/01/99 7:39p Dave
 * Added prioritizing ship respawns. Also fixed respawns in TvT so teams
 * don't mix respawn points.
 * 
 * 15    2/26/99 6:01p Andsager
 * Add sexp has-been-tagged-delay and cap-subsys-cargo-known-delay
 * 
 * 14    2/17/99 2:11p Dave
 * First full run of squad war. All freespace and tracker side stuff
 * works.
 * 
 * 13    2/11/99 2:15p Andsager
 * Add ship explosion modification to FRED
 * 
 * 12    2/07/99 8:51p Andsager
 * Add inner bound to asteroid field.  Inner bound tries to stay astroid
 * free.  Wrap when within and don't throw at ships inside.
 * 
 * 11    2/03/99 12:42p Andsager
 * Add escort priority.  Modify ship_flags_dlg to include field.  Save and
 * Load.  Add escort priority field to ship.
 * 
 * 10    1/25/99 5:03a Dave
 * First run of stealth, AWACS and TAG missile support. New mission type
 * :)
 * 
 * 9     1/19/99 3:57p Andsager
 * Round 2 of variables
 * 
 * 8     1/07/99 1:52p Andsager
 * Initial check in of Sexp_variables
 * 
 * 7     12/17/98 2:43p Andsager
 * Modify fred campaign save file to include optional mission loops
 * 
 * 6     11/14/98 5:37p Dave
 * Put in support for full nebulas.
 * 
 * 5     11/05/98 4:18p Dave
 * Modified mission file format.
 * 
 * 4     10/29/98 9:22p Dave
 * Removed minor bug concering externalization of campaign files.
 * 
 * 3     10/29/98 6:49p Dave
 * Finished up Fred support for externalizing mission and campaign files.
 * 
 * 2     10/07/98 6:28p Dave
 * Initial checkin. Renamed all relevant stuff to be Fred2 instead of
 * Fred. Globalized mission and campaign file extensions. Removed Silent
 * Threat specific code.
 * 
 * 1     10/07/98 3:00p Dave
 * 
 * 207   9/16/98 10:42a Hoffoss
 * Added sexp node counting to fsm files for end user designers.
 * 
 * 206   9/15/98 7:24p Dave
 * Minor UI changes. Localized bunch of new text.
 * 
 * 205   9/15/98 11:44a Dave
 * Renamed builtin ships and wepaons appropriately in FRED. Put in scoring
 * scale factors. Fixed standalone filtering of MD missions to non-MD
 * hosts.
 * 
 * 204   9/10/98 1:17p Dave
 * Put in code to flag missions and campaigns as being MD or not in Fred
 * and Freespace. Put in multiplayer support for filtering out MD
 * missions. Put in multiplayer popups for warning of non-valid missions.
 * 
 * 203   5/21/98 12:58a Hoffoss
 * Fixed warnings optimized build turned up.
 * 
 * 202   5/20/98 1:04p Hoffoss
 * Made credits screen use new artwork and removed rating field usage from
 * Fred (a goal struct member).
 * 
 * 201   5/19/98 1:19p Allender
 * new low level reliable socket reading code.  Make all missions/campaign
 * load/save to data missions folder (i.e. we are rid of the player
 * missions folder)
 * 
 * 200   5/13/98 5:13p Allender
 * red alert support to go back to previous mission
 * 
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "freespace.h"
#include "missionsave.h"
#include "missiongoals.h"
#include "missionmessage.h"
#include "missionparse.h"
#include "fredrender.h"
#include "aigoals.h"
#include "starfield.h"
#include "lighting.h"
#include "linklist.h"
#include "weapon.h"
#include "missioncampaign.h"
#include "campaigntreewnd.h"
#include "campaigntreeview.h"
#include "campaigneditordlg.h"
#include "sexp.h"
#include "missionbriefcommon.h"
#include "management.h"
#include "eventmusic.h"
#include "nebula.h"
#include "asteroid.h"
#include "missioncmdbrief.h"
#include "jumpnode.h"
#include "mainfrm.h"
#include "fhash.h"
#include "neb.h"
#include "osapi.h"

int CFred_mission_save::save_mission_file(char *pathname)
{
	char backup_name[256], savepath[MAX_PATH_LEN], *p;
	CTime t;

	t = CTime::GetCurrentTime();
	strcpy(The_mission.modified, t.Format("%x at %X"));

	strcpy(savepath, "");
	p = strrchr(pathname, '\\');
	if ( p ) {
		*p = '\0';
		strcpy(savepath, pathname);
		*p = '\\';
		strcat(savepath, "\\");
	}
	strcat(savepath, "saving.xxx");

	reset_parse();
	fred_parse_flag = 0;
	fp = cfopen(savepath, "wt", CFILE_NORMAL);
	if (!fp)	{
		nprintf(("Error", "Can't open mission file to save.\n"));
		return -1;
	}	

	if (save_mission_info())
		err = -2;
	else if (save_plot_info())
		err = -3;
	else if (save_variables())
		err = -3;
//	else if (save_briefing_info())
//		err = -4;
	else if (save_cmd_briefs())
		err = -4;
	else if (save_briefing())
		err = -4;
	else if (save_debriefing())
		err = -5;
	else if (save_players())
		err = -6;
	else if (save_objects())
		err = -7;
	else if (save_wings())
		err = -8;
	else if (save_events())
		err = -9;
	else if (save_goals())
		err = -10;
	else if (save_waypoints())
		err = -11;
	else if (save_messages())
		err = -12;
	else if (save_reinforcements())
		err = -13;
	else if (save_bitmaps())
		err = -14;
	else if (save_asteroid_fields())
		err = -15;
	else if (save_music())
		err = -16;
	else {
		required_string_fred("#End");
		parse_comments(2);
		token_found = NULL;
		parse_comments();
		fout("\n");
	}

	cfclose(fp);
	if (err) {
		mprintf(("Mission saving error code #%d", err));

	} else {
		strcpy(backup_name, pathname);
		if (backup_name[strlen(backup_name) - 4] == '.')
			backup_name[strlen(backup_name) - 4] = 0;

		strcat(backup_name, ".bak");
		cf_attrib(pathname, 0, FILE_ATTRIBUTE_READONLY, CF_TYPE_MISSIONS);
		cf_delete(backup_name, CF_TYPE_MISSIONS);
		cf_rename(pathname, backup_name, CF_TYPE_MISSIONS);
		cf_rename(savepath, pathname, CF_TYPE_MISSIONS);
	}

	return err;
}

int CFred_mission_save::autosave_mission_file(char *pathname)
{
	char backup_name[256], name2[256];
	int i, len;
	CTime t;
	
	t = CTime::GetCurrentTime();
	strcpy(The_mission.modified, t.Format("%x at %X"));

	len = strlen(pathname);
	strcpy(backup_name, pathname);
	strcpy(name2, pathname);
	sprintf(backup_name + len, ".%.3d", BACKUP_DEPTH);
	cf_delete(backup_name, CF_TYPE_MISSIONS);
	for (i=BACKUP_DEPTH; i>1; i--) {
		sprintf(backup_name + len, ".%.3d", i - 1);
		sprintf(name2 + len, ".%.3d", i);
		cf_rename(backup_name, name2, CF_TYPE_MISSIONS);
	}
	
	strcpy(backup_name + len, ".001");
	reset_parse();
	fred_parse_flag = 0;
	fp = cfopen(backup_name, "wt", CFILE_NORMAL, CF_TYPE_MISSIONS);
	if (!fp)	{
		nprintf(("Error", "Can't open mission file to save.\n"));
		return -1;
	}

	if (save_mission_info())
		err = -2;
	else if (save_plot_info())
		err = -3;
	else if (save_variables())
		err = -3;
//	else if (save_briefing_info())
//		err = -4;
	else if (save_cmd_briefs())
		err = -4;
	else if (save_briefing())
		err = -4;
	else if (save_debriefing())
		err = -5;
	else if (save_players())
		err = -6;
	else if (save_objects())
		err = -7;
	else if (save_wings())
		err = -8;
	else if (save_events())
		err = -9;
	else if (save_goals())
		err = -10;
	else if (save_waypoints())
		err = -11;
	else if (save_messages())
		err = -12;
	else if (save_reinforcements())
		err = -13;
	else if (save_bitmaps())
		err = -14;
	else if (save_asteroid_fields())
		err = -15;
	else if (save_music())
		err = -16;
	else {
		required_string_fred("#End");
		parse_comments(2);
		token_found = NULL;
		parse_comments();
		fout("\n");
	}

	cfclose(fp);
	if (err)
		mprintf(("Mission saving error code #%d", err));

	return err;
}

int CFred_mission_save::save_mission_info()
{
	// int f = count_free_sexp_nodes();
	// fout("// Of %d total sexp nodes, %d free, %d used\n\n",MAX_SEXP_NODES, f, MAX_SEXP_NODES - f);

	required_string_fred("#Mission Info");
	parse_comments(0);

	required_string_fred("$Version:");
	parse_comments(2);
	fout(" %.2f", FRED_MISSION_VERSION);

	// XSTR
	required_string_fred("$Name:");
	parse_comments();
	fout(" ");
	fout_ext("%s", The_mission.name);

	required_string_fred("$Author:");
	parse_comments();
	fout(" %s", The_mission.author);

	required_string_fred("$Created:");
	parse_comments();
	fout(" %s", The_mission.created);

	required_string_fred("$Modified:");
	parse_comments();
	fout(" %s", The_mission.modified);

	required_string_fred("$Notes:");
	parse_comments();
	fout("\n%s", The_mission.notes);

	required_string_fred("$End Notes:");
	parse_comments(0);

	// XSTR
	required_string_fred("$Mission Desc:");
	parse_comments(2);
	fout("\n");
	fout_ext("%s", The_mission.mission_desc);
	fout("\n");

	required_string_fred("$end_multi_text");
	parse_comments(0);

#if 0
	if (optional_string_fred("+Game Type:"))
		parse_comments(2);
	else
		fout("\n\n+Game Type:");
	fout("\n%s", Game_types[The_mission.game_type]);	
#endif		

	if ( optional_string_fred("+Game Type Flags:")){
		parse_comments(2);
	} else {
		fout("\n+Game Type Flags:");
	}	

	fout(" %d", The_mission.game_type);

	if (optional_string_fred("+Flags:")){
		parse_comments(2);
	} else {
		fout("\n+Flags:");
	}

	fout(" %d", The_mission.flags);

	// maybe write out Nebula intensity
	if(The_mission.flags & MISSION_FLAG_FULLNEB){
		Assert(Neb2_awacs > 0.0f);
		fout("\n+NebAwacs: %f\n", Neb2_awacs);

		// storm name
		fout("\n+Storm: %s\n", Mission_parse_storm_name);
	}

	// For multiplayer missions -- write out the number of player starts and number of respawns
	if (The_mission.game_type & MISSION_TYPE_MULTI) {
		if (optional_string_fred("+Num Players:"))
			parse_comments(2);
		else
			fout("\n+Num Players:");

		fout(" %d", Player_starts);

		if (optional_string_fred("+Num Respawns:"))
			parse_comments(2);
		else
			fout("\n+Num Respawns:");

		fout(" %d", The_mission.num_respawns);
	}
	
	if ( optional_string_fred("+Red Alert:")){
		parse_comments(2);
	} else {
		fout("\n+Red Alert:");
	}
	fout(" %d", The_mission.red_alert );
	
	if ( optional_string_fred("+Scramble:")){
		parse_comments(2);
	} else {
		fout("\n+Scramble:");
	}
	fout(" %d", The_mission.scramble);

	if ( optional_string_fred("+Disallow Support:")){
		parse_comments(2);
	} else {
		fout("\n+Disallow Support:");
	}

	fout(" %d", The_mission.disallow_support);

	if (Mission_all_attack) {
		if (optional_string_fred("+All Teams Attack")){
			parse_comments();
		} else {
			fout("\n+All Teams Attack");
		}
	}

	if (Entry_delay_time) {
		if (optional_string_fred("+Player Entry Delay:"))
			parse_comments(2);
		else
			fout("\n\n+Player Entry Delay:");

		fout("\n%f", f2fl(Entry_delay_time));
	}

	if (optional_string_fred("+Viewer pos:")){
		parse_comments(2);
	} else {
		fout("\n\n+Viewer pos:");
	}

	save_vector(view_pos);

	if (optional_string_fred("+Viewer orient:")){
		parse_comments();
	} else {
		fout("\n+Viewer orient:");
	}

	save_matrix(view_orient);

	// squadron info
	if(!(The_mission.game_type & MISSION_TYPE_MULTI) && (strlen(The_mission.squad_name) > 0)){
		// squad name
		fout("\n+SquadReassignName: %s", The_mission.squad_name);

		// maybe squad logo
		if(strlen(The_mission.squad_filename) > 0){
			fout("\n+SquadReassignLogo: %s", The_mission.squad_filename);
		}
	}

	return err;
}

int CFred_mission_save::save_plot_info()
{
	required_string_fred("#Plot Info");
	parse_comments(2);

	// XSTR
	required_string_fred("$Tour:");
	parse_comments(2);
	fout(" ");
	fout_ext("%s", The_mission.tour_name);

	required_string_fred("$Pre-Briefing Cutscene:");
	parse_comments();
	fout(" %s", The_mission.pre_briefing_cutscene);

	required_string_fred("$Pre-Mission Cutscene:");
	parse_comments();
	fout(" %s", The_mission.pre_mission_cutscene);

	required_string_fred("$Next Mission Success:");
	parse_comments();
	fout(" %s", The_mission.next_mission_success);

	required_string_fred("$Next Mission Partial:");
	parse_comments();
	fout(" %s", The_mission.next_mission_partial);

	required_string_fred("$Next Mission Failure:");
	parse_comments();
	fout(" %s", The_mission.next_mission_failure);

	return err;
}

int CFred_mission_save::save_cmd_brief()
{
	int stage;

	stage = 0;
	required_string_fred("#Command Briefing");
	parse_comments(2);

	if (The_mission.game_type & MISSION_TYPE_MULTI)
		return err;  // no command briefings allowed in multiplayer missions.

	for (stage=0; stage<Cur_cmd_brief->num_stages; stage++) {
		required_string_fred("$Stage Text:");
		parse_comments(2);

		// XSTR
		fout("\n");
		fout_ext("%s", Cur_cmd_brief->stage[stage].text);

		required_string_fred("$end_multi_text", "$Stage Text:");
		parse_comments();

		required_string_fred("$Ani Filename:");
		parse_comments();
		fout(" %s", Cur_cmd_brief->stage[stage].ani_filename);

		required_string_fred("+Wave Filename:", "$Ani Filename:");
		parse_comments();
		fout(" %s", Cur_cmd_brief->stage[stage].wave_filename);
	}

	return err;
}

int CFred_mission_save::save_cmd_briefs()
{
	int i;

	for (i=0; i<Num_teams; i++) {
		Cur_cmd_brief = &Cmd_briefs[i];
		save_cmd_brief();
	}

	return err;
}

int CFred_mission_save::save_briefing()
{
	int			i,j,k, nb;
	char			out[4096];
	brief_stage	*bs;
	brief_icon	*bi;

	for ( nb = 0; nb < Num_teams; nb++ ) {

		required_string_fred("#Briefing");
		parse_comments(2);

		required_string_fred("$start_briefing");
		parse_comments();

		Assert(Briefings[nb].num_stages <= MAX_BRIEF_STAGES);
		required_string_fred("$num_stages:");
		parse_comments();
		fout(" %d", Briefings[nb].num_stages);

		for (i=0; i<Briefings[nb].num_stages; i++) {
			bs = &Briefings[nb].stages[i];

			required_string_fred("$start_stage");
			parse_comments();

			required_string_fred("$multi_text");
			parse_comments();

			// XSTR
			fout("\n");
			sprintf(out,"%s", bs->new_text);
			fout_ext(out);

			required_string_fred("$end_multi_text", "$start_stage");
			parse_comments();

			if (!drop_white_space(bs->voice)[0]){
				strcpy(bs->voice, "None");
			}

			required_string_fred("$voice:");
			parse_comments();
			fout(" %s", bs->voice);

			required_string_fred("$camera_pos:");
			parse_comments();
			save_vector(bs->camera_pos);
			
			required_string_fred("$camera_orient:");
			parse_comments();
			save_matrix(bs->camera_orient);

			required_string_fred("$camera_time:");
			parse_comments();
			fout(" %d", bs->camera_time);

			required_string_fred("$num_lines:");
			parse_comments();
			fout(" %d", bs->num_lines);

			for (k=0; k<bs->num_lines; k++) {
				required_string_fred("$line_start:");
				parse_comments();
				fout(" %d", bs->lines[k].start_icon);

				required_string_fred("$line_end:");
				parse_comments();
				fout(" %d", bs->lines[k].end_icon);
			}

			required_string_fred("$num_icons:");
			parse_comments();
			Assert(bs->num_icons <= MAX_STAGE_ICONS );
			fout(" %d", bs->num_icons);

			required_string_fred("$Flags:");
			parse_comments();
			fout(" %d", bs->flags);

			required_string_fred("$Formula:");
			parse_comments();
			convert_sexp_to_string(bs->formula, out, SEXP_SAVE_MODE);
			fout(" %s", out);

			for ( j = 0; j < bs->num_icons; j++ ) {
				bi = &bs->icons[j];

				required_string_fred("$start_icon");
				parse_comments();

				required_string_fred("$type:");
				parse_comments();
				fout(" %d", bi->type);

				required_string_fred("$team:");
				parse_comments();
				fout(" %s", Team_names[bitmask_2_bitnum(bi->team)]);

				required_string_fred("$class:");
				parse_comments();
				if (bi->ship_class < 0)
					bi->ship_class = 0;

				fout(" %s", Ship_info[bi->ship_class].name);

				required_string_fred("$pos:");
				parse_comments();
				save_vector(bi->pos);

				if (drop_white_space(bi->label)[0]) {
					if (optional_string_fred("$label:"))
						parse_comments();
					else
						fout("\n$label:");

					fout(" %s", bi->label);
				}

				if (optional_string_fred("+id:"))
					parse_comments(); 
				else
					fout("\n+id:");

				fout(" %d", bi->id);

				required_string_fred("$hlight:");
				parse_comments();
				fout(" %d", (bi->flags & BI_HIGHLIGHT)?1:0 );

				required_string_fred("$multi_text");
				parse_comments();

//				sprintf(out,"\n%s", bi->text);
//				fout(out);

				required_string_fred("$end_multi_text");
				parse_comments();

				required_string_fred("$end_icon");
				parse_comments();
			}

			required_string_fred("$end_stage");
			parse_comments();
		}
		required_string_fred("$end_briefing");
		parse_comments();
	}

	return err;
}

int CFred_mission_save::save_debriefing()
{
	int j, i;
	char out[8192];

	for ( j = 0; j < Num_teams; j++ ) {

		Debriefing = &Debriefings[j];

		required_string_fred("#Debriefing_info");
		parse_comments(2);

		required_string_fred("$Num stages:");
		parse_comments(2);
		fout(" %d", Debriefing->num_stages);

		for (i=0; i<Debriefing->num_stages; i++) {
			required_string_fred("$Formula:");
			parse_comments(2);
			convert_sexp_to_string(Debriefing->stages[i].formula, out, SEXP_SAVE_MODE);
			fout(" %s", out);

			// XSTR
			required_string_fred("$Multi text");
			parse_comments();
			fout("\n   ");
			fout_ext("%s", Debriefing->stages[i].new_text);

			required_string_fred("$end_multi_text");
			parse_comments();

			if (!drop_white_space(Debriefing->stages[i].voice)[0]){
				strcpy(Debriefing->stages[i].voice, "None");
			}

			required_string_fred("$Voice:");
			parse_comments();
			fout(" %s", Debriefing->stages[i].voice);

			// XSTR
			required_string_fred("$Recommendation text:");
			parse_comments();
			fout("\n   ");
			fout_ext("%s", Debriefing->stages[i].new_recommendation_text);

			required_string_fred("$end_multi_text");
			parse_comments();
		}
	}

	return err;
}

int sexp_variable_block_count();
// save variables
int CFred_mission_save::save_variables()
{
	char *type;
	char number[] = "number";
	char string[] = "string";
	char block[] = "block";
	int i;

	// sort sexp_variables
	sexp_variable_sort();

	// get count
	int num_variables = sexp_variable_count();
	int num_block_vars = sexp_variable_block_count();
	int total_variables = num_variables + num_block_vars;

	if (total_variables > 0) {

		// write 'em out
		required_string_fred("#Sexp_variables");
		parse_comments(2);

		required_string_fred("$Variables:");
		parse_comments(2);

		fout ("\n(");
//		parse_comments();

		for (i=0; i<num_variables; i++) {
			if (Sexp_variables[i].type & SEXP_VARIABLE_NUMBER) {
				type = number;
			} else {
				type = string;
			}
			// index "var name" "default" "type"
			fout("\n\t\t%d\t\t\"%s\"\t\t\"%s\"\t\t\"%s\"", i, Sexp_variables[i].variable_name, Sexp_variables[i].text, type);
//			parse_comments();
		}

		for (i=MAX_SEXP_VARIABLES-num_block_vars; i<MAX_SEXP_VARIABLES; i++) {
			type = block;
			fout("\n\t\t%d\t\t\"%s\"\t\t\"%s\"\t\t\"%s\"", i, Sexp_variables[i].variable_name, Sexp_variables[i].text, type);
		}

		fout("\n)");
	}

	return err;
}


int CFred_mission_save::save_players()
{
	int i, j;
	int used_pool[MAX_WEAPON_TYPES];

	// write out alternate name list
	if(Mission_alt_type_count > 0){
		fout("\n\n#Alternate Types:\n");

		// write them all out
		for(i=0; i<Mission_alt_type_count; i++){
			fout("$Alt: %s\n", Mission_alt_types[i]);
		}

		// end
		fout("\n#end\n");
	}

	required_string_fred("#Players");
	parse_comments(2);
	fout("\t\t;! %d total\n", Player_starts);

	for (i=0; i<Num_teams; i++) {
		required_string_fred("$Starting Shipname:");
		parse_comments();
		Assert(Player_start_shipnum >= 0);
		fout(" %s", Ships[Player_start_shipnum].ship_name);
		
		required_string_fred("$Ship Choices:");
		parse_comments();
		fout(" (\n");

		for (j=0; j<Team_data[i].number_choices; j++)
			fout("\t\"%s\"\t%d\n", Ship_info[Team_data[i].ship_list[j]].name,
				Team_data[i].ship_count[j]);

		fout(")");

		if (optional_string_fred("+Weaponry Pool:", "$Starting Shipname:")){
			parse_comments(2);
		} else {
			fout("\n\n+Weaponry Pool:");
		}

		fout(" (\n");
		generate_weaponry_usage_list(used_pool);
		for (j=0; j<Num_weapon_types; j++){
			if (Team_data[i].weaponry_pool[j] + used_pool[j] > 0){
				fout("\t\"%s\"\t%d\n", Weapon_info[j].name, Team_data[i].weaponry_pool[j] + used_pool[j]);
			}
		}

		fout(")");
	}

	return err;
}

int CFred_mission_save::save_objects()
{
	char out[4096];
	int i, j, z;
	ai_info *aip;
	object *objp;
	ship_info *sip;
	
	required_string_fred("#Objects");
	parse_comments(2);
	fout("\t\t;! %d total\n", ship_get_num_ships() );

	for (i=z=0; i<MAX_SHIPS; i++) {
		if (Ships[i].objnum < 0){
			continue;
		}

		j = Objects[Ships[i].objnum].type;
		if ((j != OBJ_SHIP) && (j != OBJ_START)){
			continue;
		}

		objp = &Objects[Ships[i].objnum];
		sip = &Ship_info[Ships[i].ship_info_index];
		required_string_either_fred("$Name:", "#Wings");
		required_string_fred("$Name:");
		parse_comments(z ? 2 : 1);
		fout(" %s\t\t;! Object #%d\n", Ships[i].ship_name, i);

		required_string_fred("$Class:");
		parse_comments(0);
		fout(" %s", Ship_info[Ships[i].ship_info_index].name);

		// optional alternate type name
		if(strlen(Fred_alt_names[i])){
			fout("\n$Alt: %s\n", Fred_alt_names[i]);
		}

		required_string_fred("$Team:");
		parse_comments();
		fout(" %s", Team_names[bitmask_2_bitnum(Ships[i].team)]);

		required_string_fred("$Location:");
		parse_comments();
		save_vector(Objects[Ships[i].objnum].pos);

		required_string_fred("$Orientation:");
		parse_comments();
		save_matrix(Objects[Ships[i].objnum].orient);

		required_string_fred("$IFF:");
		parse_comments();
		fout(" %s", Iff_names[0]);

		Assert(Ships[i].ai_index >= 0);
		aip = &Ai_info[Ships[i].ai_index];

		required_string_fred("$AI Behavior:");
		parse_comments();
		fout(" %s", Ai_behavior_names[aip->behavior]);

		if (Ships[i].weapons.ai_class != Ship_info[Ships[i].ship_info_index].ai_class) {
			if (optional_string_fred("+AI Class:", "$Name:"))
				parse_comments();
			else
				fout("\n+AI Class:");

			fout(" %s", Ai_class_names[Ships[i].weapons.ai_class]);
		}

		save_ai_goals(Ai_info[Ships[i].ai_index].goals, i);

		// XSTR
		required_string_fred("$Cargo 1:");
		parse_comments();
		fout(" ");
		fout_ext("%s", Cargo_names[Ships[i].cargo1]);

		save_common_object_data(&Objects[Ships[i].objnum], &Ships[i]);

		if (Ships[i].wingnum >= 0){
			Ships[i].arrival_location = ARRIVE_AT_LOCATION;
		}

		required_string_fred("$Arrival Location:");
		parse_comments();
		fout(" %s", Arrival_location_names[Ships[i].arrival_location]);

		if (Ships[i].arrival_location != ARRIVE_AT_LOCATION) {
			if (optional_string_fred("+Arrival Distance:", "$Name:")){
				parse_comments();
			} else {
				fout("\n+Arrival Distance:");
			}

			fout(" %d", Ships[i].arrival_distance);
			if (optional_string_fred("$Arrival Anchor:", "$Name:")){
				parse_comments();
			} else {
				fout("\n$Arrival Anchor:");
			}

			z = Ships[i].arrival_anchor;
			if (z >= SPECIAL_ARRIVAL_ANCHORS_OFFSET){
				fout(" %s", Special_arrival_anchor_names[z - SPECIAL_ARRIVAL_ANCHORS_OFFSET]);
			} else if (z >= 0) {
				fout(" %s", Ships[z].ship_name);
			} else {
				fout(" <error>");
			}
		}

		if (Ships[i].arrival_delay) {
			if (optional_string_fred("+Arrival Delay:", "$Name:")){
				parse_comments();
			} else {
				fout("\n+Arrival Delay:");
			}

			fout(" %d", Ships[i].arrival_delay);
		}

		required_string_fred("$Arrival Cue:");
		parse_comments();
		convert_sexp_to_string(Ships[i].arrival_cue, out, SEXP_SAVE_MODE);
		fout(" %s", out);

		required_string_fred("$Departure Location:");
		parse_comments();
		fout(" %s", Departure_location_names[Ships[i].departure_location]);


		if ( Ships[i].departure_location == DEPART_AT_DOCK_BAY ) {
			required_string_fred("$Departure Anchor:");
			parse_comments();
			
			if ( Ships[i].departure_anchor >= 0 ){
				fout(" %s", Ships[Ships[i].departure_anchor].ship_name );
			} else {
				fout(" <error>");
			}
		}

		if (Ships[i].departure_delay) {
			if (optional_string_fred("+Departure delay:", "$Name:")){
				parse_comments();
			} else {
				fout("\n+Departure delay:");
			}

			fout(" %d", Ships[i].departure_delay);
		}

		required_string_fred("$Departure Cue:");
		parse_comments();
		convert_sexp_to_string(Ships[i].departure_cue, out, SEXP_SAVE_MODE);
		fout(" %s", out);

		required_string_fred("$Determination:");
		parse_comments();
		fout(" %d", Ships[i].determination);

		if (optional_string_fred("+Flags:", "$Name:")) {
			parse_comments();
			fout (" (");
		} else
			fout("\n+Flags: (");

		if (Ships[i].flags & SF_CARGO_REVEALED)
			fout(" \"cargo-known\"");
		if (Ships[i].flags & SF_IGNORE_COUNT)
			fout(" \"ignore-count\"");
		if (objp->flags & OF_PROTECTED)
			fout(" \"protect-ship\"");
		if (objp->flags & OF_BEAM_PROTECTED)
			fout(" \"beam-protect-ship\"");
		if (Ships[i].flags & SF_REINFORCEMENT)
			fout(" \"reinforcement\"");
		if (objp->flags & OF_NO_SHIELDS)
			fout(" \"no-shields\"");
		if (Ships[i].flags & SF_ESCORT)
			fout(" \"escort\"");
		if (objp->type == OBJ_START)
			fout(" \"player-start\"");
		if (Ships[i].flags & SF_NO_ARRIVAL_MUSIC)
			fout(" \"no-arrival-music\"");
		if (Ships[i].flags & SF_NO_ARRIVAL_WARP)
			fout(" \"no-arrival-warp\"");
		if (Ships[i].flags & SF_NO_DEPARTURE_WARP)
			fout(" \"no-departure-warp\"");
		if (Ships[i].flags & SF_LOCKED)
			fout(" \"locked\"");
		if (Ships[i].flags & SF_INVULNERABLE)
			fout(" \"invulnerable\"");
		if (Ships[i].flags & SF_HIDDEN_FROM_SENSORS)
			fout(" \"hidden-from-sensors\"");
		if ( Ships[i].flags & SF_SCANNABLE )
			fout(" \"scannable\"");
		if ( Ai_info[Ships[i].ai_index].ai_flags & AIF_KAMIKAZE )
			fout(" \"kamikaze\"");
		if ( Ai_info[Ships[i].ai_index].ai_flags & AIF_NO_DYNAMIC )
			fout(" \"no-dynamic\"");
		if ( Ships[i].flags & SF_RED_ALERT_STORE_STATUS )
			fout(" \"red-alert-carry\"");
		if ( objp->flags & OF_SPECIAL_WARP )
			fout(" \"special-warp\"");
		fout(" )");

		fout("+Respawn priority: %d\n", Ships[i].respawn_priority);

		if (Ships[i].flags & SF_ESCORT) {
			if (optional_string_fred("+Escort priority:", "$Name:")) {
				parse_comments();
			} else {
				fout("\n+Escort priority:");
			}

			fout(" %d", Ships[i].escort_priority);
		}

		if (Ships[i].special_exp_index != -1) {
			if (optional_string_fred("+Special Exp index:", "$Name:")) {
				parse_comments();
			} else {
				fout("+Special Exp index:");
			}

			fout(" %d", Ships[i].special_exp_index);
		}


		if ( Ai_info[Ships[i].ai_index].ai_flags & AIF_KAMIKAZE ) {
			if ( optional_string_fred("+Kamikaze Damage:", "$Name:")){
				parse_comments();
			} else {
				fout("\n+Kamikaze Damage:");
			}

			fout(" %d", (int)(Ai_info[Ships[i].ai_index].kamikaze_damage) );
		}

		if (Ships[i].hotkey != -1) {
			if (optional_string_fred("+Hotkey:", "$Name:")){
				parse_comments();
			} else {
				fout("\n+Hotkey:");
			}

			fout(" %d", Ships[i].hotkey);
		}

		// mwa -- new code to save off information about initially docked ships.
		if ( Ships[i].flags & SF_INITIALLY_DOCKED ) {
			Assert ( aip->dock_objnum != -1 );		// ge allender if you hit this

			if (optional_string_fred("+Docked With:", "$Name:"))
				parse_comments();
			else
				fout("\n+Docked With:");

			switch (Objects[aip->dock_objnum].type) {
				case OBJ_SHIP:
				case OBJ_START:
					j = Objects[aip->dock_objnum].instance;
					break;

				default:
					Int3();
			}

			fout(" %s", Ships[j].ship_name);

			required_string_fred("$Docker Point:", "$Name:");
			parse_comments();
			fout(" %s", model_get_dock_name(Ships[j].modelnum, aip->dockee_index));

			required_string_fred("$Dockee Point:", "$Name:");
			parse_comments();
			fout(" %s", model_get_dock_name(Ships[i].modelnum, aip->dock_index));

		}

		// check the ship flag about killing off the ship before a missino starts.  Write out the appropriate
		// variable if necessary
		if ( Ships[i].flags & SF_KILL_BEFORE_MISSION ) {
			if ( optional_string_fred("+Destroy At:", "$Name:"))
				parse_comments();
			else
				fout ("\n+Destroy At: ");

			fout(" %d", Ships[i].final_death_time );
		}

		// possibly write out the orders that this ship will accept.  We'll only do it if the orders
		// are not the default set of orders
		if ( Ships[i].orders_accepted != ship_get_default_orders_accepted( &Ship_info[Ships[i].ship_info_index]) ) {
			if ( optional_string_fred("+Orders Accepted:", "$Name:") )
				parse_comments();
			else
				fout("\n+Orders Accepted:");

			fout(" %d\t\t;! note that this is a bitfield!!!", Ships[i].orders_accepted);
		}

		if (Ships[i].group >= 0) {
			if (optional_string_fred("+Group:", "$Name:"))
				parse_comments();
			else
				fout("\n+Group:");

			fout(" %d", Ships[i].group);
		}

		if (Ships[i].score) {
			if (optional_string_fred("+Score:", "$Name:"))
				parse_comments();
			else
				fout("\n+Score:");

			fout(" %d", Ships[i].score);
		}

		// deal with the persona for this ship as well.
		if ( Ships[i].persona_index != -1 ) {
			if (optional_string_fred("+Persona Index:", "$Name:"))
				parse_comments();
			else
				fout("\n+Persona Index:");

			fout(" %d", Ships[i].persona_index);
		}

		z++;
	}

	return err;
}

int CFred_mission_save::save_common_object_data(object *objp, ship *shipp)
{
	int j, z;
	ship_subsys *ptr;
	ship_info *sip;
	ship_weapon *wp;

	sip = &Ship_info[shipp->ship_info_index];
	if ((int) objp->phys_info.speed) {
		if (optional_string_fred("+Initial Velocity:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Initial Velocity:");

		fout(" %d", (int) objp->phys_info.speed);
	}

	if ((int) objp->hull_strength != (int) sip->initial_hull_strength) {
		if (optional_string_fred("+Initial Hull:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Initial Hull:");

		fout(" %d", (int) objp->hull_strength);
	}

	if ((int) get_shield_strength(objp) != 100) {
		if (optional_string_fred("+Initial Shields:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Initial Shields:");

		fout(" %d", (int) objp->shields[0]);
	}

	// save normal ship weapons info
	required_string_fred("+Subsystem:", "$Name:");
	parse_comments();
	fout(" Pilot");

	wp = &shipp->weapons;
	z = 0;
	j = wp->num_primary_banks;
	while (j--)
		if (wp->primary_bank_weapons[j] != sip->primary_bank_weapons[j])
			z = 1;

	if (z) {
		if (optional_string_fred("+Primary Banks:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Primary Banks:");

		fout(" ( ");
		for (j=0; j<wp->num_primary_banks; j++)
			fout("\"%s\" ", Weapon_info[wp->primary_bank_weapons[j]].name);

		fout(")");
	}

	z = 0;
	j = wp->num_secondary_banks;
	while (j--)
		if (wp->secondary_bank_weapons[j] != sip->secondary_bank_weapons[j])
			z = 1;

	if (z) {
		if (optional_string_fred("+Secondary Banks:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Secondary Banks:");

		fout(" ( ");
		for (j=0; j<wp->num_secondary_banks; j++)
			fout("\"%s\" ", Weapon_info[wp->secondary_bank_weapons[j]].name);

		fout(")");
	}

	z = 0;
	j = wp->num_secondary_banks;
	while (j--)
		if (wp->secondary_bank_ammo[j] != 100)
			z = 1;

	if (z) {
		if (optional_string_fred("+Sbank Ammo:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Sbank Ammo:");

		fout(" ( ");
		for (j=0; j<wp->num_secondary_banks; j++)
			fout("%d ", wp->secondary_bank_ammo[j]);

		fout(")");
	}

	ptr = GET_FIRST(&shipp->subsys_list);
	while (ptr != END_OF_LIST(&shipp->subsys_list)) {
		if ( (ptr->current_hits) || (ptr->system_info->type == SUBSYSTEM_TURRET) || (ptr->subsys_cargo_name != -1)) {
			if (optional_string_fred("+Subsystem:", "$Name:"))
				parse_comments();
			else
				fout("\n+Subsystem:");

			fout(" %s", ptr->system_info->subobj_name);
		}

		if (ptr->current_hits) {
			if (optional_string_fred("$Damage:", "$Name:", "+Subsystem:"))
				parse_comments();
			else
				fout("\n$Damage:");

			fout(" %d", (int) ptr->current_hits);
		}

		if (ptr->subsys_cargo_name != -1) {
			if (optional_string_fred("+Cargo Name:", "$Name:", "+Subsystem:"))
				parse_comments();
			else
				fout("\n+Cargo Name:");

			fout_ext("%s", Cargo_names[ptr->subsys_cargo_name]);
		}

		if (ptr->system_info->type == SUBSYSTEM_TURRET)
			save_turret_info(ptr, shipp - Ships);

		ptr = GET_NEXT(ptr);
	}

/*	for (j=0; j<shipp->status_count; j++) {
		required_string_fred("$Status Description:");
		parse_comments(-1);
		fout(" %s", Status_desc_names[shipp->status_type[j]]);

		required_string_fred("$Status:");
		parse_comments(-1);
		fout(" %s", Status_type_names[shipp->status[j]]);

		required_string_fred("$Target:");
		parse_comments(-1);
		fout(" %s", Status_target_names[shipp->target[j]]);
	}*/

	return err;
}

int CFred_mission_save::save_wings()
{
	char out[4096];
	int i, j, z, ship, count = 0;

	fred_parse_flag = 0;
	required_string_fred("#Wings");
	parse_comments(2);
	fout("\t\t;! %d total", num_wings);

	for (i=0; i<MAX_WINGS; i++) {
		if (!Wings[i].wave_count)
			continue;

		count++;
		required_string_either_fred("$Name:", "#Events");
		required_string_fred("$Name:");
		parse_comments(2);
		fout(" %s", Wings[i].name);

		required_string_fred("$Waves:");
		parse_comments();
		fout(" %d", Wings[i].num_waves);

		required_string_fred("$Wave Threshold:");
		parse_comments();
		fout(" %d", Wings[i].threshold);

		required_string_fred("$Special Ship:");
		parse_comments();
		fout(" %d\t\t;! %s\n", Wings[i].special_ship,
			Ships[Wings[i].ship_index[Wings[i].special_ship]].ship_name);

		required_string_fred("$Arrival Location:");
		parse_comments();
		fout(" %s", Arrival_location_names[Wings[i].arrival_location]);

		if (Wings[i].arrival_location != ARRIVE_AT_LOCATION) {
			if (optional_string_fred("+Arrival Distance:", "$Name:"))
				parse_comments();
			else
				fout("\n+Arrival Distance:");

			fout(" %d", Wings[i].arrival_distance);
			if (optional_string_fred("$Arrival Anchor:", "$Name:"))
				parse_comments();
			else
				fout("\n$Arrival Anchor:");

			z = Wings[i].arrival_anchor;
			if (z >= SPECIAL_ARRIVAL_ANCHORS_OFFSET)
				fout(" %s", Special_arrival_anchor_names[z - SPECIAL_ARRIVAL_ANCHORS_OFFSET]);
			else if (z >= 0)
				fout(" %s", Ships[z].ship_name);
			else
				fout(" <error>");
		}

		if (Wings[i].arrival_delay) {
			if (optional_string_fred("+Arrival delay:", "$Name:"))
				parse_comments();
			else
				fout("\n+Arrival delay:");

			fout(" %d", Wings[i].arrival_delay);
		}

		required_string_fred("$Arrival Cue:");
		parse_comments();
		convert_sexp_to_string(Wings[i].arrival_cue, out, SEXP_SAVE_MODE);
		fout(" %s", out);

		required_string_fred("$Departure Location:");
		parse_comments();
		fout(" %s", Departure_location_names[Wings[i].departure_location]);

		if ( Wings[i].departure_location == DEPART_AT_DOCK_BAY ) {
			required_string_fred("$Departure Anchor:");
			parse_comments();

			if ( Wings[i].departure_anchor >= 0 )
				fout(" %s", Ships[Wings[i].departure_anchor].ship_name );
			else
				fout(" <error>");
		}

		if (Wings[i].departure_delay) {
			if (optional_string_fred("+Departure delay:", "$Name:"))
				parse_comments();
			else
				fout("\n+Departure delay:");

			fout(" %d", Wings[i].departure_delay);
		}

		required_string_fred("$Departure Cue:");
		parse_comments();
		convert_sexp_to_string(Wings[i].departure_cue, out, SEXP_SAVE_MODE);
		fout(" %s", out);

		required_string_fred("$Ships:");
		parse_comments();
		fout(" (\t\t;! %d total\n", Wings[i].wave_count);

		for (j=0; j<Wings[i].wave_count; j++) {
			ship = Wings[i].ship_index[j];
//			if (Objects[Ships[ship].objnum].type == OBJ_START)
//				fout("\t\"Player 1\"\n");
//			else
				fout("\t\"%s\"\n", Ships[Wings[i].ship_index[j]].ship_name);
		}

		fout(")");
		save_ai_goals(Wings[i].ai_goals, -1);

		if (Wings[i].hotkey != -1) {
			if (optional_string_fred("+Hotkey:", "$Name:"))
				parse_comments();
			else
				fout("\n+Hotkey:");

			fout(" %d", Wings[i].hotkey);
		}

		if ( optional_string_fred("+Flags:", "$Name:")) {
			parse_comments();
			fout( "(" );
		} else 
			fout("\n+Flags: (");

		if ( Wings[i].flags & WF_IGNORE_COUNT )
			fout(" \"ignore-count\"");
		if ( Wings[i].flags & WF_REINFORCEMENT )
			fout(" \"reinforcement\"");
		if ( Wings[i].flags & WF_NO_ARRIVAL_MUSIC )
			fout(" \"no-arrival-music\"");
		if ( Wings[i].flags & WF_NO_ARRIVAL_MESSAGE )
			fout(" \"no-arrival-message\"");
		if ( Wings[i].flags & WF_NO_ARRIVAL_WARP )
			fout(" \"no-arrival-warp\"");
		if ( Wings[i].flags & WF_NO_DEPARTURE_WARP )
			fout(" \"no-departure-warp\"");
		if ( Wings[i].flags & WF_NO_DYNAMIC )
			fout( " \"no-dynamic\"" );

		fout(" )");

		if (Wings[i].wave_delay_min) {
			if (optional_string_fred("+Wave Delay Min:", "$Name:"))
				parse_comments();
			else
				fout("\n+Wave Delay Min:");

			fout(" %d", Wings[i].wave_delay_min);
		}

		if (Wings[i].wave_delay_max) {
			if (optional_string_fred("+Wave Delay Max:", "$Name:"))
				parse_comments();
			else
				fout("\n+Wave Delay Max:");

			fout(" %d", Wings[i].wave_delay_max);
		}
	}

	Assert(count == num_wings);
	return err;
}

int CFred_mission_save::save_goals()
{
	char out[4096];
	int i;

	fred_parse_flag = 0;
	required_string_fred("#Goals");
	parse_comments(2);
	fout("\t\t;! %d total\n", Num_goals);

	for (i=0; i<Num_goals; i++) {
		int type;

		required_string_either_fred("$Type:", "#Waypoints");
		required_string_fred("$Type:");
		parse_comments(i ? 2 : 1);

		type = Mission_goals[i].type & GOAL_TYPE_MASK;
		fout(" %s", Goal_type_names[type]);

		if (*Mission_goals[i].name) {
			if (optional_string_fred("+Name:", "$Type:"))
				parse_comments();
			else
				fout("\n+Name:");

			fout(" %s", Mission_goals[i].name);
		}

		// XSTR
		required_string_fred("$MessageNew:");
		parse_comments();
		fout(" ");
		fout_ext("%s", Mission_goals[i].message);
		fout("\n");
		required_string_fred("$end_multi_text");
		parse_comments(0);

		required_string_fred("$Formula:");
		parse_comments();
		convert_sexp_to_string(Mission_goals[i].formula, out, SEXP_SAVE_MODE);
		fout(" %s", out);

		if ( Mission_goals[i].type & INVALID_GOAL ) {
			if (optional_string_fred("+Invalid", "$Type:"))
				parse_comments();
			else
				fout("\n+Invalid");
		}

		if ( Mission_goals[i].flags & MGF_NO_MUSIC ) {
			if (optional_string_fred("+No music", "$Type:"))
				parse_comments();
			else
				fout("\n+No music");
		}

		if ( Mission_goals[i].score != 0 ) {
			if ( optional_string_fred("+Score:", "$Type:"))
				parse_comments();
			else
				fout("\n+Score:");
			fout(" %d", Mission_goals[i].score );
		}

		if ( The_mission.game_type & MISSION_TYPE_MULTI_TEAMS ) {
			if ( optional_string_fred("+Team:", "$Type:"))
				parse_comments();
			else
				fout("\n+Team:");
			fout(" %d", Mission_goals[i].team );
		}
	}

	return err;
}

int CFred_mission_save::save_waypoints()
{
	int i;
	object *ptr;

	fred_parse_flag = 0;
	required_string_fred("#Waypoints");
	parse_comments(2);
	fout("\t\t;! %d lists total\n", Num_waypoint_lists);

	for (i=0; i<Num_jump_nodes; i++) {
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if ((ptr->type == OBJ_JUMP_NODE) && (ptr->instance == i))
				break;

			ptr = GET_NEXT(ptr);
		}

		Assert(ptr != END_OF_LIST(&obj_used_list));

		required_string_fred("$Jump Node:", "$Jump Node Name:");
		parse_comments(2);
		save_vector(ptr->pos);

		required_string_fred("$Jump Node Name:", "$Jump Node:");
		parse_comments();
		fout(" %s", Jump_nodes[i].name);
	}

	for (i=0; i<Num_waypoint_lists; i++) {
		required_string_either_fred("$Name:", "#Messages");
		required_string_fred("$Name:");
		parse_comments(i ? 2 : 1);
		fout(" %s", Waypoint_lists[i].name);

		required_string_fred("$List:");
		parse_comments();
		fout(" (\t\t;! %d points in list\n", Waypoint_lists[i].count);

		save_waypoint_list(Waypoint_lists[i]);
		fout(")");
	}

	return err;
}

int CFred_mission_save::save_waypoint_list(waypoint_list &w)
{
	int i;

	for (i=0; i<w.count; i++)
		fout("\t( %f, %f, %f )\n", w.waypoints[i].x, w.waypoints[i].y, w.waypoints[i].z);

	return 0;
}

int CFred_mission_save::save_messages()
{
	int i;

	fred_parse_flag = 0;
	required_string_fred("#Messages");
	parse_comments(2);
	fout("\t\t;! %d total\n", Num_messages);

	for (i=Num_builtin_messages; i<Num_messages; i++) {
		required_string_either_fred("$Name:", "#Reinforcements");
		required_string_fred("$Name:");
		parse_comments(i ? 2 : 1);
		fout(" %s", Messages[i].name);

		// team
		required_string_fred("$Team:");
		parse_comments(i ? 2 : 1);
		if((Messages[i].multi_team < 0) || (Messages[i].multi_team >= 2)){
			fout(" %d", -1);
		} else {
			fout(" %d", Messages[i].multi_team);
		}

		// XSTR
		required_string_fred("$MessageNew:");
		parse_comments();
		fout(" ");
		fout_ext("%s", Messages[i].message);
		fout("\n");
		required_string_fred("$end_multi_text");
		parse_comments(0);

		if ( Messages[i].persona_index != -1 ) {
			if ( optional_string_fred("+Persona:", "$Name:"))
				parse_comments();
			else
				fout("\n+Persona:");

			fout(" %s", Personas[Messages[i].persona_index].name );
		}

		if (Messages[i].avi_info.name) {
			if (optional_string_fred("+AVI Name:", "$Name:"))
				parse_comments();
			else
				fout("\n+AVI Name:");

			fout(" %s", Messages[i].avi_info.name);
		}

		if (Messages[i].wave_info.name) {
			if (optional_string_fred("+Wave Name:", "$Name:"))
				parse_comments();
			else
				fout("\n+Wave Name:");

			fout(" %s", Messages[i].wave_info.name);
		}
	}

	return err;
}

int CFred_mission_save::save_vector(vector &v)
{
	fout(" %f, %f, %f", v.x, v.y, v.z);
	return 0;
}

int CFred_mission_save::save_matrix(matrix &m)
{
	fout("\n\t%f, %f, %f,\n", m.rvec.x, m.rvec.y, m.rvec.z);
	fout("\t%f, %f, %f,\n", m.uvec.x, m.uvec.y, m.uvec.z);
	fout("\t%f, %f, %f", m.fvec.x, m.fvec.y, m.fvec.z);
	return 0;
}

// saves comments from previous campaign/mission file
void CFred_mission_save::parse_comments(int newlines)
{
	char *comment_start = NULL;
	int state = 0, same_line = 0, first_comment = 1, tab = 0, flag = 0;

	if (newlines < 0) {
		newlines = -newlines;
		tab = 1;
	}

	if (newlines)
		same_line = 1;

	if (fred_parse_flag || !Token_found_flag || !token_found || (token_found && (*Mission_text_raw == EOF_CHAR))) {
		while (newlines-- > 0)
			fout("\n");

		if (tab)
			fout("\t");

		if (token_found)
			fout("%s", token_found);

		return;
	}

	while (*raw_ptr != EOF_CHAR) {
		if (!state) {
			if (token_found && (*raw_ptr == *token_found))
				if (!strnicmp(raw_ptr, token_found, strlen(token_found))) {
					same_line = newlines - 1 + same_line;
					while (same_line-- > 0)
						fout("\n");
					
					if (tab)
						fout("\t");

					fout("%s", token_found);
					return;
				}

			if ((*raw_ptr == '/') && (raw_ptr[1] == '*')) {
				comment_start = raw_ptr;
				state = 1;
			}

			if ((*raw_ptr == ';') && (raw_ptr[1] != '!')) {
				comment_start = raw_ptr;
				state = 2;
			}

			if ((*raw_ptr == '/') && (raw_ptr[1] == '/')) {
				comment_start = raw_ptr;
				state = 2;
			}

			if (*raw_ptr == '\n')
				flag = 1;

			if (state && flag)
				fout("\n");

		} else {
			if ((*raw_ptr == '\n') && (state == 2)) {
				if (first_comment && !flag)
					fout("\t\t");

				*raw_ptr = 0;
				fout("%s\n", comment_start);
				*raw_ptr = '\n';
				state = first_comment = same_line = flag = 0;
			}

			if ((*raw_ptr == '*') && (raw_ptr[1] == '/') && (state == 1)) {
				if (first_comment && !flag)
					fout("\t\t");

				state = raw_ptr[2];
				raw_ptr[2] = 0;
				fout("%s", comment_start);
				raw_ptr[2] = (char)state;
				state = first_comment = flag = 0;
			}
		}

		raw_ptr++;
	}

	return;
}

int CFred_mission_save::fout(char *format, ...)
{
	char str[16384];
	va_list args;
	
	if (err){
		return err;
	}

	va_start(args, format);
	vsprintf(str, format, args);
	va_end(args);
	Assert(strlen(str) < 16384);

	cfputs(str, fp);
	return 0;
}

int CFred_mission_save::fout_ext(char *format, ...)
{
	char str[16384];
	char str_out[16384] = "";
	va_list args;
	int str_id;
	
	if (err){
		return err;
	}

	va_start(args, format);
	vsprintf(str, format, args);
	va_end(args);
	Assert(strlen(str) < 16384);

	// lookup the string in the hash table
	str_id = fhash_string_exists(str);
	// doesn't exist, so assign it an ID of -1 and stick it in the table
	if(str_id <= -2){
		sprintf(str_out, " XSTR(\"%s\", -1)", str);

		// add the string to the table		
		fhash_add_str(str, -1);
	}
	// _does_ exist, so just write it out as it is
	else {
		sprintf(str_out, " XSTR(\"%s\", %d)", str, str_id);
	}

	cfputs(str_out, fp);
	return 0;
}

void CFred_mission_save::save_ai_goals(ai_goals *goalp, int ship)
{
	char *str = NULL, buf[80];
	int i, valid, flag = 1;

	for (i=0; i<MAX_AI_GOALS; i++) {
		if (goalp[i].ai_mode == AI_GOAL_NONE)
			continue;

		if (flag) {
			if (optional_string_fred("$AI Goals:", "$Name:"))
				parse_comments();
			else
				fout("\n$AI Goals:");

			fout(" ( goals ");
			flag = 0;
		}

		if (goalp[i].ai_mode == AI_GOAL_CHASE_ANY) {
			fout("( ai-chase-any %d ) ", goalp[i].priority);

		} else if (goalp[i].ai_mode == AI_GOAL_UNDOCK) {
			fout("( ai-undock %d ) ", goalp[i].priority);

		} else if (goalp[i].ai_mode == AI_GOAL_KEEP_SAFE_DISTANCE) {
			fout("( ai-keep-safe-distance %d ) ", goalp[i].priority);
		
		} else if (goalp[i].ai_mode == AI_GOAL_PLAY_DEAD) {
			fout("( ai-play-dead %d ) ", goalp[i].priority);

		} else if (goalp[i].ai_mode == AI_GOAL_WARP) {
			fout("( ai-warp-out %d ) ", goalp[i].priority);
		
		} else {
			valid = 1;
			if (!goalp[i].ship_name) {
				Warning(LOCATION, "Ai goal has no target where one is required");

			} else {
				sprintf(buf, "\"%s\"", goalp[i].ship_name);
				switch (goalp[i].ai_mode) {
					case AI_GOAL_WAYPOINTS:
						str = "ai-waypoints";
						break;

					case AI_GOAL_WAYPOINTS_ONCE:
						str = "ai-waypoints-once";
						break;

					case AI_GOAL_DESTROY_SUBSYSTEM:
						if (goalp[i].docker.index == -1 || !goalp[i].docker.index) {
							valid = 0;
							Warning(LOCATION, "AI destroy subsystem goal invalid subsystem name\n");

						} else {
							sprintf(buf, "\"%s\" \"%s\"", goalp[i].ship_name, goalp[i].docker.name);
							str = "ai-destroy-subsystem";
						}

						break;

					case AI_GOAL_DOCK:
						if (ship < 0) {
							valid = 0;
							Warning(LOCATION, "Wings aren't allowed to have a docking goal\n");
							
						} else if (goalp[i].docker.index == -1 || !goalp[i].docker.index) {
							valid = 0;
							Warning(LOCATION, "AI dock goal for \"%s\" has invalid docker point "
								"(docking with \"%s\")\n", Ships[ship].ship_name, goalp[i].ship_name);

						} else if (goalp[i].dockee.index == -1 || !goalp[i].dockee.index) {
							valid = 0;
							Warning(LOCATION, "AI dock goal for \"%s\" has invalid dockee point "
								"(docking with \"%s\")\n", Ships[ship].ship_name, goalp[i].ship_name);

						} else {
							sprintf(buf, "\"%s\" \"%s\" \"%s\"", goalp[i].ship_name,
								goalp[i].docker.name, goalp[i].dockee.name);

							str = "ai-dock";
						}
						break;

					case AI_GOAL_CHASE:
						str = "ai-chase";
						break;

					case AI_GOAL_CHASE_WING:
						str = "ai-chase-wing";
						break;

					case AI_GOAL_GUARD:
						str = "ai-guard";
						break;

					case AI_GOAL_GUARD_WING:
						str = "ai-guard-wing";
						break;

					case AI_GOAL_DISABLE_SHIP:
						str = "ai-disable-ship";
						break;

					case AI_GOAL_DISARM_SHIP:
						str = "ai-disarm-ship";
						break;

					case AI_GOAL_IGNORE:
						str = "ai-ignore";
						break;

					case AI_GOAL_EVADE_SHIP:
						str = "ai-evade-ship";
						break;

					case AI_GOAL_STAY_NEAR_SHIP:
						str = "ai-stay-near-ship";
						break;

					case AI_GOAL_STAY_STILL:
						str = "ai-stay-still";
						break;

					default:
						Assert(0);
				}

				if (valid)
					fout("( %s %s %d ) ", str, buf, goalp[i].priority);
			}
		}
	}

	if (!flag)
		fout(")");
}

int CFred_mission_save::save_events()
{
	char out[4096];
	int i;

	fred_parse_flag = 0;
	required_string_fred("#Events");
	parse_comments(2);
	fout("\t\t;! %d total\n", Num_mission_events);

	for (i=0; i<Num_mission_events; i++) {
		required_string_either_fred("$Formula:", "#Goals");
		required_string_fred("$Formula:");
		parse_comments(i ? 2 : 1);
		convert_sexp_to_string(Mission_events[i].formula, out, SEXP_SAVE_MODE);
		fout(" %s", out);

		if (*Mission_events[i].name) {
			if (optional_string_fred("+Name:", "$Formula:")){
				parse_comments();
			} else {
				fout("\n+Name:");
			}

			fout(" %s", Mission_events[i].name);
		}

		if ( optional_string_fred("+Repeat Count:", "$Formula:")){
			parse_comments();
		} else {
			fout("\n+Repeat Count:");
		}

		fout(" %d", Mission_events[i].repeat_count);

		if ( optional_string_fred("+Interval:", "$Formula:")){
			parse_comments();
		} else {
			fout("\n+Interval:");
		}

		fout(" %d", Mission_events[i].interval);

		if ( Mission_events[i].score != 0 ) {
			if ( optional_string_fred("+Score:", "$Formula:")){
				parse_comments();
			} else {
				fout("\n+Score:");
			}
			fout(" %d", Mission_events[i].score);
		}

		if ( Mission_events[i].chain_delay >= 0 ) {
			if ( optional_string_fred("+Chained:", "$Formula:")){
				parse_comments();
			} else {
				fout("\n+Chained:");
			}

			fout(" %d", Mission_events[i].chain_delay);
		}

		//XSTR
		if (Mission_events[i].objective_text) {
			if (optional_string_fred("+Objective:", "$Formula:")){
				parse_comments();
			} else {
				fout("\n+Objective:");
			}

			fout(" ");
			fout_ext("%s", Mission_events[i].objective_text);
		}

		//XSTR
		if (Mission_events[i].objective_key_text) {
			if (optional_string_fred("+Objective key:", "$Formula:")){
				parse_comments();
			} else {
				fout("\n+Objective key:");
			}

			fout(" ");
			fout_ext("%s", Mission_events[i].objective_key_text);
		}

		// save team
		if (Mission_events[i].team >= 0){
			if (optional_string_fred("+Team:")){
				parse_comments();
			} else {
				fout("\n+Team:");
			} 
			fout(" ");
			fout("%d", Mission_events[i].team);
		}
	}

	return err;
}

int CFred_mission_save::save_reinforcements()
{
	int i, j, type;

	fred_parse_flag = 0;
	required_string_fred("#Reinforcements");
	parse_comments(2);
	fout("\t\t;! %d total\n", Num_reinforcements);

	for (i=0; i<Num_reinforcements; i++) {
		required_string_either_fred("$Name:", "#Background bitmaps");
		required_string_fred("$Name:");
		parse_comments(i ? 2 : 1);
		fout(" %s", Reinforcements[i].name);

		type = TYPE_ATTACK_PROTECT;
		for (j=0; j<MAX_SHIPS; j++)
			if ((Ships[j].objnum != -1) && !stricmp(Ships[j].ship_name, Reinforcements[i].name)) {
				if (Ship_info[Ships[j].ship_info_index].flags & SIF_SUPPORT)
					type = TYPE_REPAIR_REARM;
				break;
			}

		required_string_fred("$Type:");
		parse_comments();
		fout(" %s", Reinforcement_type_names[type]);

		required_string_fred("$Num times:");
		parse_comments();
		fout(" %d", Reinforcements[i].uses);

		if ( optional_string_fred("+Arrival Delay:", "$Name:"))
			parse_comments();
		else
			fout("\n+Arrival Delay:");
		fout(" %d", Reinforcements[i].arrival_delay );

		if (optional_string_fred("+No Messages:", "$Name:"))
			parse_comments();
		else
			fout("\n+No Messages:");
		fout(" (");
		for (j = 0; j < MAX_REINFORCEMENT_MESSAGES; j++) {
			if ( strlen(Reinforcements[i].no_messages[j]) )
				fout(" \"%s\"", Reinforcements[i].no_messages[j]);
		}
		fout(" )");

		if (optional_string_fred("+Yes Messages:", "$Name:"))
			parse_comments();
		else
			fout("\n+Yes Messages:");
		fout(" (");
		for (j = 0; j < MAX_REINFORCEMENT_MESSAGES; j++) {
			if ( strlen(Reinforcements[i].yes_messages[j]) )
				fout(" \"%s\"", Reinforcements[i].yes_messages[j]);
		}
		fout(" )");

	}

	return err;
}

int CFred_mission_save::save_bitmaps()
{	
	int idx;

	fred_parse_flag = 0;
	required_string_fred("#Background bitmaps");
	parse_comments(2);
	fout("\t\t;! %d total\n", Num_starfield_bitmaps);

	required_string_fred("$Num stars:");
	parse_comments();
	fout(" %d", Num_stars);

	float Ambient_light_level = 1.0f;	// JAS: Should this be set to something?
	required_string_fred("$Ambient light level:");
	parse_comments();
	fout(" %d", Ambient_light_level);

	// neb2 stuff
	if(The_mission.flags & MISSION_FLAG_FULLNEB){
		required_string_fred("+Neb2:");
		parse_comments();
		fout(" %s\n", Neb2_texture_name);

		required_string_fred("+Neb2Flags:");
		parse_comments();
		fout(" %d\n", Neb2_poof_flags);
	}
	// neb 1 stuff
	else {
		if (Nebula_index >= 0) {
			if (optional_string_fred("+Nebula:")){
				parse_comments();
			} else {
				fout("\n+Nebula:");
			}		
			fout(" %s", Nebula_filenames[Nebula_index]);		

			required_string_fred("+Color:");
			parse_comments();
			fout(" %s", Nebula_colors[Mission_palette]);

			required_string_fred("+Pitch:");
			parse_comments();
			fout(" %d", Nebula_pitch);

			required_string_fred("+Bank:");
			parse_comments();
			fout(" %d", Nebula_bank);

			required_string_fred("+Heading:");
			parse_comments();
			fout(" %d", Nebula_heading);
		}
	}

	// save suns by sun bitmap filename
	for(idx=0; idx<Num_suns; idx++){
		// sun name, angles and scale
		required_string_fred("$Sun:");
		parse_comments();
		fout(" %s", Suns[idx].filename);

		required_string_fred("+Angles:");
		parse_comments();
		fout(" %f %f %f", Suns[idx].ang.p, Suns[idx].ang.b, Suns[idx].ang.h);

		required_string_fred("+Scale:");
		parse_comments();
		fout(" %f", Suns[idx].scale_x);
	}

	// save background bitmaps by filename
	for(idx=0; idx<Num_starfield_bitmaps; idx++){
		// sun name, angles and scale
		required_string_fred("$Starbitmap:");
		parse_comments();
		fout(" %s", Starfield_bitmap_instance[idx].filename);

		required_string_fred("+Angles:");
		parse_comments();
		fout(" %f %f %f", Starfield_bitmap_instance[idx].ang.p, Starfield_bitmap_instance[idx].ang.b, Starfield_bitmap_instance[idx].ang.h);

		required_string_fred("+ScaleX:");
		parse_comments();
		fout(" %f", Starfield_bitmap_instance[idx].scale_x);

		required_string_fred("+ScaleY:");
		parse_comments();
		fout(" %f", Starfield_bitmap_instance[idx].scale_y);

		required_string_fred("+DivX:");
		parse_comments();
		fout(" %d", Starfield_bitmap_instance[idx].div_x);

		required_string_fred("+DivY:");
		parse_comments();
		fout(" %d", Starfield_bitmap_instance[idx].div_y);
	}

	return err;
}

int CFred_mission_save::save_asteroid_fields()
{
	int i, idx;

	fred_parse_flag = 0;
	required_string_fred("#Asteroid Fields");
	parse_comments(2);

	for (i=0; i<1 /*MAX_ASTEROID_FIELDS*/; i++) {
		if (!Asteroid_field.num_initial_asteroids)
			continue;

		required_string_fred("$Density:");
		parse_comments(2);
		fout(" %d", Asteroid_field.num_initial_asteroids);

		// field type
		if (optional_string_fred("+Field Type:")){
			parse_comments();
		} else {
			fout("\n+Field Type:");
		}
		fout(" %d", Asteroid_field.field_type);

		// debris type
		if (optional_string_fred("+Debris Genre:")){
			parse_comments();
		} else {
			fout("\n+Debris Genre:");
		}
		fout(" %d", Asteroid_field.debris_genre);

		// field_debris_type (only if ship genre)
		if (Asteroid_field.debris_genre == DG_SHIP) {
			for (int idx=0; idx<3; idx++) {
				if (Asteroid_field.field_debris_type[idx] != -1) {
					if (optional_string_fred("+Field Debris Type:")){
						parse_comments();
					} else {
						fout("\n+Field Debris Type:");
					}
					fout(" %d", Asteroid_field.field_debris_type[idx]);
				}
			}
		} else {
			// asteroid subtypes stored in field_debris_type as -1 or 1
			for (idx=0; idx<3; idx++) {
				if (Asteroid_field.field_debris_type[idx] != -1) {
					if (optional_string_fred("+Field Debris Type:")){
						parse_comments();
					} else {
						fout("\n+Field Debris Type:");
					}
					fout(" %d", idx);
				}
			}
		}


		required_string_fred("$Average Speed:");
		parse_comments();
		fout(" %f", vm_vec_mag(&Asteroid_field.vel));

		required_string_fred("$Minimum:");
		parse_comments();
		save_vector(Asteroid_field.min_bound);

		required_string_fred("$Maximum:");
		parse_comments();
		save_vector(Asteroid_field.max_bound);

		if (Asteroid_field.has_inner_bound == 1) {
			if (optional_string_fred("+Inner Bound:")){
				parse_comments();
			} else {
				fout("\n+Inner Bound:");
			}

			required_string_fred("$Minimum:");
			parse_comments();
			save_vector(Asteroid_field.inner_min_bound);

			required_string_fred("$Maximum:");
			parse_comments();
			save_vector(Asteroid_field.inner_max_bound);
		}
	}

	return err;
}

int CFred_mission_save::save_music()
{
	required_string_fred("#Music");
	parse_comments(2);

	required_string_fred("$Event Music:");
	parse_comments(2);
	if (Current_soundtrack_num < 0)
		fout(" None");
	else
		fout(" %s", Soundtracks[Current_soundtrack_num].name);

	required_string_fred("$Briefing Music:");
	parse_comments();
	if (Mission_music[SCORE_BRIEFING] < 0)
		fout(" None");
	else
		fout(" %s", Spooled_music[Mission_music[SCORE_BRIEFING]].name);

	return err;
}

void CFred_mission_save::save_turret_info(ship_subsys *ptr, int ship)
{
	int i, z;
	ship_weapon *wp = &ptr->weapons;

	if (wp->ai_class != Ship_info[Ships[ship].ship_info_index].ai_class) {
		if (optional_string_fred("+AI Class:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+AI Class:");

		fout(" %s", Ai_class_names[wp->ai_class]);
	}

	z = 0;
	i = wp->num_primary_banks;
	while (i--)
		if (wp->primary_bank_weapons[i] != ptr->system_info->primary_banks[i])
			z = 1;

	if (z) {
		if (optional_string_fred("+Primary Banks:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Primary Banks:");

		fout(" ( ");
		for (i=0; i<wp->num_primary_banks; i++)
			fout("\"%s\" ", Weapon_info[wp->primary_bank_weapons[i]].name);

		fout(")");
	}

	z = 0;
	i = wp->num_secondary_banks;
	while (i--)
		if (wp->secondary_bank_weapons[i] != ptr->system_info->secondary_banks[i])
			z = 1;

	if (z) {
		if (optional_string_fred("+Secondary Banks:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Secondary Banks:");

		fout(" ( ");
		for (i=0; i<wp->num_secondary_banks; i++)
			fout("\"%s\" ", Weapon_info[wp->secondary_bank_weapons[i]].name);

		fout(")");
	}

	z = 0;
	i = wp->num_secondary_banks;
	while (i--)
		if (wp->secondary_bank_ammo[i] != 100)
			z = 1;

	if (z) {
		if (optional_string_fred("+Sbank Ammo:", "$Name:", "+Subsystem:"))
			parse_comments();
		else
			fout("\n+Sbank Ammo:");

		fout(" ( ");
		for (i=0; i<wp->num_secondary_banks; i++)
			fout("%d ", wp->secondary_bank_ammo[i]);

		fout(")");
	}
}

int CFred_mission_save::save_campaign_file(char *pathname)
{
	int i, j, m, flag;

	Campaign_tree_formp->save_tree();  // flush all changes so they get saved.
	Campaign_tree_viewp->sort_elements();
	reset_parse();
	fred_parse_flag = 0;

	pathname = cf_add_ext(pathname, FS_CAMPAIGN_FILE_EXT);
	fp = cfopen(pathname, "wt", CFILE_NORMAL, CF_TYPE_MISSIONS);
	if (!fp)	{
		nprintf(("Error", "Can't open campaign file to save.\n"));
		return -1;
	}

	required_string_fred("$Name:");
	parse_comments(0);
	fout(" %s", Campaign.name);

	Assert((Campaign.type >= 0) && (Campaign.type < MAX_CAMPAIGN_TYPES));
	required_string_fred("$Type:");
	parse_comments();
	fout(" %s", campaign_types[Campaign.type]);

	// XSTR
	if (Campaign.desc) {
		required_string_fred("+Description:");
		parse_comments();
		fout("\n");
		fout_ext("%s", Campaign.desc);
		fout("\n$end_multi_text");
	}

	if ( Campaign.type != CAMPAIGN_TYPE_SINGLE ) {
		required_string_fred("+Num Players:");
		parse_comments();
		fout(" %d", Campaign.num_players);
	}	

	// write out the ships and weapons which the player can start the campaign with
	optional_string_fred("+Starting Ships: (");
	parse_comments(2);
	for (i = 0; i < MAX_SHIP_TYPES; i++ ) {
		if ( Campaign.ships_allowed[i] )
			fout(" \"%s\" ", Ship_info[i].name );
	}
	fout( ")\n" );

	optional_string_fred("+Starting Weapons: (");
	parse_comments();
	for (i = 0; i < MAX_WEAPON_TYPES; i++ ) {
		if ( Campaign.weapons_allowed[i] )
			fout(" \"%s\" ", Weapon_info[i].name );
	}
	fout( ")\n" );

	fred_parse_flag = 0;
	for (i=0; i<Campaign.num_missions; i++) {
		m = Sorted[i];
		required_string_either_fred("$Mission:", "#End");
		required_string_fred("$Mission:");
		parse_comments(2);
		fout(" %s", Campaign.missions[m].name);

		if ( strlen(Campaign.missions[i].briefing_cutscene) ) {
			if (optional_string_fred("+Briefing Cutscene:", "$Mission"))
				parse_comments();
			else
				fout("\n+Briefing Cutscene:");

			fout( " %s", Campaign.missions[i].briefing_cutscene );
		}

		required_string_fred("+Flags:", "$Mission:");
		parse_comments();
		fout(" %d", Campaign.missions[m].flags);

		// save campaign link sexp
		bool mission_loop = false;
		flag = 0;
		for (j=0; j<Total_links; j++) {
			if (Links[j].from == m) {
				if (!flag) {
					if (optional_string_fred("+Formula:", "$Mission:"))
						parse_comments();
					else
						fout("\n+Formula:");

					fout(" ( cond\n");
					flag = 1;
				}

				//save_campaign_sexp(Links[j].sexp, Campaign.missions[Links[j].to].name);
				if (Links[j].mission_loop) {
					mission_loop = true;
				} else {
					save_campaign_sexp(Links[j].sexp, Links[j].to);
				}
			}
		}

		if (flag) {
			fout(")");
		}

		// now save campaign loop sexp
		if (mission_loop) {
			required_string_fred("\n+Mission Loop:");
			parse_comments();

			int num_mission_loop = 0;
			for (j=0; j<Total_links; j++) {
				if ( (Links[j].from == m) && (Links[j].mission_loop) ) {

					num_mission_loop++;

					// maybe write out mission loop descript
					if ((num_mission_loop == 1) && Links[j].mission_loop_txt) {
						required_string_fred("+Mission Loop Text:");
						parse_comments();
						fout("\n");
						fout_ext("%s", Links[j].mission_loop_txt);
						fout("\n$end_multi_text");
					}

					// maybe write out mission loop descript
					if ((num_mission_loop == 1) && Links[j].mission_loop_brief_anim) {
						required_string_fred("+Mission Loop Brief Anim:");
						parse_comments();
						fout("\n");
						fout_ext("%s", Links[j].mission_loop_brief_anim);
						fout("\n$end_multi_text");
					}

					// maybe write out mission loop descript
					if ((num_mission_loop == 1) && Links[j].mission_loop_brief_sound) {
						required_string_fred("+Mission Loop Brief Sound:");
						parse_comments();
						fout("\n");
						fout_ext("%s", Links[j].mission_loop_brief_sound);
						fout("\n$end_multi_text");
					}

					if (num_mission_loop == 1) {
						// write out mission loop formula
						fout("\n+Formula:");
						fout(" ( cond\n");
						save_campaign_sexp(Links[j].sexp, Links[j].to);
						fout(")");
					}
				}
			}
			if (num_mission_loop > 1) {
				char buffer[1024];
				sprintf(buffer, "Multiple branching loop error from mission %s\nEdit campaign for *at most* 1 loop from each mission.", Campaign.missions[m].name);
				MessageBox((HWND)os_get_window(), buffer, "Error", MB_OK);
			}
		}

		if (optional_string_fred("+Level:", "$Mission:")){
			parse_comments();
		} else {
			fout("\n\n+Level:");
		}

		fout(" %d", Campaign.missions[m].level);

		if (optional_string_fred("+Position:", "$Mission:")){
			parse_comments();
		} else {
			fout("\n+Position:");
		}

		fout(" %d", Campaign.missions[m].pos);
	}

	required_string_fred("#End");
	parse_comments(2);
	token_found = NULL;
	parse_comments();
	fout("\n");

	cfclose(fp);
	if (err)
		mprintf(("Campaign saving error code #%d", err));
	else
		Campaign_wnd->error_checker();

	return err;
}

void CFred_mission_save::save_campaign_sexp(int node, int link_num)
{
	char out[4096];

	Sexp_string = out;
	*out = 0;
	Assert(node >= 0);

	// if the link num is -1, then this is a end-of-campaign location
	if ( link_num != -1 ) {
		if (build_sexp_string(node, 2, SEXP_SAVE_MODE))
			fout("   (\n      %s\n      ( next-mission \"%s\" )\n   )\n", out, Campaign.missions[link_num].name);
		else
			fout("   ( %s( next-mission \"%s\" ) )\n", out, Campaign.missions[link_num].name);
	} else {
		if (build_sexp_string(node, 2, SEXP_SAVE_MODE)){
			fout("   (\n      %s\n      ( end-of-campaign )\n   )\n", out);
		} else {
			fout("   ( %s( end-of-campaign ) )\n", out );
		}
	}
}


