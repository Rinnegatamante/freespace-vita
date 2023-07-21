/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Source$
 * $Revision: 307 $
 * $Author: taylor $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * 
 * Header for parselo.c
 * 
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 14    9/13/99 10:40a Mikeb
 * Bumped up MISSION_TEXT_SIZE from 380000 to 390000 for ships.tbl ship
 * descriptions.
 * 
 * 13    8/02/99 4:19p Andsager
 * Bump up MISSION_TEXT_SIZE to 380000 for ships.tbl
 * 
 * 12    5/03/99 10:10a Davidg
 * DA:  Bump Mission_text_size to handle large ship.tbl
 * 
 * 11    2/19/99 11:10a Jasen
 * Upped MISSION_TEXT_SIZE to 300000 (for souper cap)
 * 
 * 10    2/03/99 6:06p Dave
 * Groundwork for FS2 PXO usertracker support.  Gametracker support next.
 * 
 * 9     1/07/99 1:52p Andsager
 * Initial check in of Sexp_variables
 * 
 * 8     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 7     10/26/98 9:42a Dave
 * Early flak gun support.
 * 
 * 6     10/22/98 6:14p Dave
 * Optimized some #includes in Anim folder. Put in the beginnings of
 * parse/localization support for externalized strings and tstrings.tbl
 * 
 * 5     10/16/98 1:22p Andsager
 * clean up header files
 * 
 * 4     10/14/98 1:15p Andsager
 * Fix fred
 * 
 * 3     10/14/98 12:25p Andsager
 * Cleaned up and removed unnecessary include files
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 54    6/10/98 6:47p Lawrance
 * Increase allowed mission length to 512, to accommodate longer French
 * text
 * 
 * 53    5/20/98 2:27a Sandeep
 * 
 * 52    4/30/98 4:53p John
 * Restructured and cleaned up cfile code.  Added capability to read off
 * of CD-ROM drive and out of multiple pack files.
 * 
 * 51    4/03/98 10:31a John
 * Made briefing and debriefing arrays be malloc'd
 * 
 * 50    3/30/98 6:22p Hoffoss
 * Added a new parsing function.
 * 
 * 49    3/29/98 12:55a Lawrance
 * Get demo build working with limited set of data.
 * 
 * 48    2/02/98 4:59p Hoffoss
 * Added a promotion text field to rank.tbl and code to support it in
 * FreeSpace.
 * 
 * 47    11/03/97 10:11p Hoffoss
 * Added new split_str_once() function.  Works similar to split_str(),
 * only it only splits a string into 2 lines.
 * 
 * 46    10/29/97 9:02a Jasen
 * Raised MISSION_TEXT_SIZE limit to 128000.
 * 
 * 45    10/17/97 3:12p Hoffoss
 * Added a whitespace elimination function, and utilized it in briefing
 * icon label saving.
 * 
 * 44    10/15/97 4:46p Lawrance
 * add drop_leading_whitespace() function
 * 
 * 43    10/08/97 4:48p Dave
 * Moved file id function into missionparse (from parselo). Finished
 * tracker logging in and out of games. Changed how file checksumming
 * works. 
 * 
 * 42    10/07/97 5:08p Dave
 * Added file versioning/checksumming for multiplayer situations. Began
 * master tracker changes for finalized version.
 * 
 * 41    10/01/97 5:07p Hoffoss
 * Weapon loadout load and save from fsm files added.
 * 
 * 40    9/22/97 5:47p Lawrance
 * re-write split_str() to allow special control words that don't get
 * considered as part of printable string
 * 
 * 39    9/09/97 3:39p Sandeep
 * warning level 4 bugs
 * 
 * 38    8/17/97 12:47p Hoffoss
 * Changed code so I can force missions to load from the missions
 * directory regardless of it's extension.
 * 
 * 37    8/13/97 10:54a Hoffoss
 * Added function to help extract only parts of a mission file.
 * 
 * 36    7/22/97 5:38p Jasen
 * Fixed bug with sexp error checking code.  Wasn't accounting for player
 * names in ship name lookup.
 * 
 * 35    6/23/97 12:02p Lawrance
 * move split_str() here, support \n's.
 * 
 * 34    6/11/97 6:14p Hoffoss
 * Made messages able to be longer.
 * 
 * 33    4/28/97 3:30p Hoffoss
 * Changed mission saving to not rely on required strings being present in
 * mission files being overwritten.
 * 
 * 32    4/22/97 4:50p Hoffoss
 * Added some flexibility to stuff_string().
 * 
 * 31    4/21/97 5:02p Hoffoss
 * Player/player status editing supported, and both saved and loaded from
 * Mission files.
 * 
 * 30    3/09/97 2:23p Allender
 * Major changes to player messaging system.  Added messages.tbl.  Made
 * all currently player messages go through new system.  Not done yet.
 * 
 * 29    3/04/97 11:19a Lawrance
 * added stuff_booleanI() that recognizes 1/0 and YES/NO
 * 
 * 28    2/18/97 9:52a Adam
 * Raised parsing text max size limit (Jason)
 * 
 * 27    1/31/97 12:07p John
 * Add turret gun type on end of $subsystem line.  Changed match_and_find
 * to return index rather than stuffing it.
 * 
 * 26    1/30/97 5:15p Mike
 * Skill levels.
 * Better named AI classes.
 * Optional override of AI class in fsm file.
 * 
 * 25    1/29/97 2:59p Mike
 * Table-driven support for John's laser rendering.
 * Null hook for AVI weapons.
 * 
 * 24    1/17/97 3:50p Hoffoss
 * Fred bug fixes and slight improvements in order to be able to create
 * mission 5.
 * 
 * 23    12/12/96 3:06p Mike
 * Fix bug in parsing code created by adding support for optional
 * terminators in required_string.
 * 
 * 22    12/12/96 2:35p Mike
 * Subsystem support
 * 
 * 21    12/11/96 3:31p Hoffoss
 * Added support for more flexible ship cargos.
 * 
 * 20    12/09/96 9:47a Hoffoss
 * Fixed silly error.  #define was in wrong file, so I moved it.
 * 
 * 19    12/03/96 2:44p Mike
 * Support for ship class and ship specific AI abilities: evasion,
 * courage, patience, accuracy
 * 
 * 18    11/15/96 1:43p Hoffoss
 * Improvements to the Ship Dialog editor window.  It is now an
 * independant window that updates data correctly.
 * 
 * 17    11/07/96 1:56p Allender
 * externed function for use in sexp.cpp
 * 
 * 16    10/30/96 9:05a Hoffoss
 * Expanded mission file max size to 32k
 * 
 * 15    10/29/96 3:28p Allender
 * added stuff_string_list function.
 * 
 * 14    10/28/96 12:18p Allender
 * added a couple of functions to deal with getting and moving past
 * strings on a whitespace only basis (instead ot to EOL)
 * 
 * 13    10/23/96 11:06a Mike
 * New weapon system.
 * Revision 1.4  1996/09/24  13:50:41  mike
 * Clean up some low level code in parselo.c.
 * Add support for Player info in mission.txt.
 * 
 * Revision 1.3  1996/09/23  20:42:03  allender
 * added filename as parameter to read_mission_text
 * 
 * Revision 1.2  1996/09/23  17:18:39  mike
 * Split parse.h into parse.h and parselo.h
 * 
 * Revision 1.1  1996/09/23  17:11:36  mike
 * Initial revision
 * 
 * 
 */

#ifndef _PARSELO_H
#define _PARSELO_H

#include <setjmp.h>
#include "cfile.h"

#define	MISSION_TEXT_SIZE	390000

extern char	Mission_text[MISSION_TEXT_SIZE];
extern char	Mission_text_raw[MISSION_TEXT_SIZE];
extern char	*Mp;
extern const char	*token_found;
extern int fred_parse_flag;
extern int Token_found_flag;
extern jmp_buf parse_abort;


#define	COMMENT_CHAR	(char)';'
#define	EOF_CHAR			(char)-128
#define	EOLN				(char)0x0a

#define	F_NAME					1
#define	F_DATE					2
#define	F_NOTES					3
#define	F_FILESPEC				4
#define	F_MULTITEXTOLD			5	// needed for backwards compatability with old briefing format
#define	F_SEXP					6
#define	F_PATHNAME				7
#define	F_SHIPCHOICE			8
#define	F_MESSAGE				9	// this is now obsolete for mission messages - all messages in missions should now use $MessageNew and stuff strings as F_MULTITEXT
#define	F_MULTITEXT				10

#define	PATHNAME_LENGTH		192
#define	NAME_LENGTH				32
#define	SEXP_LENGTH				128
#define	DATE_LENGTH				32
#define	TIME_LENGTH				16
#define	DATE_TIME_LENGTH		48
#define	NOTES_LENGTH			1024
#define	MULTITEXT_LENGTH		1024
#define	FILESPEC_LENGTH		64
#define	MESSAGE_LENGTH			512

#define	SHIP_TYPE			0	// used to identify which kind of array to do a search for a name in
#define	SHIP_INFO_TYPE		1
#define	WEAPON_LIST_TYPE	2	//	to parse an int_list of weapons
#define	RAW_INTEGER_TYPE	3	//	to parse a list of integers
#define	WEAPON_POOL_TYPE	4

#define SEXP_SAVE_MODE				1
#define SEXP_ERROR_CHECK_MODE		2

// white space
extern int is_white_space(char ch);
extern void ignore_white_space();
extern void drop_trailing_white_space(char *str);
extern void drop_leading_white_space(char *str);
extern char *drop_white_space(char *str);

// gray space
void ignore_gray_space();

// error
extern int get_line_num();
extern char *next_tokens();
extern void diag_printf(const char *format, ...);
extern void error_display(int error_level, const char *format, ...);

// skip
extern int skip_to_string(const char *pstr, const char *end = NULL);
extern int skip_to_start_of_strings(const char *pstr1, const char *pstr2);
extern void advance_to_eoln(const char *terminators);
extern void skip_token();

// required
extern int required_string(const char *pstr);
extern int optional_string(const char *pstr);
extern int required_string_either(const char *str1, const char *str2);
extern int required_string_3(const char *str1, const char *str2, const char *str3);

// stuff
extern void copy_to_eoln(char *outstr, const char *more_terminators, const char *instr, int max);
extern void copy_text_until(char *outstr, const char *instr, const char *endstr, int max_chars);
extern void stuff_string_white(char *pstr);
extern void stuff_string(char *pstr, int type, const char *terminators, int len = 0);
extern void stuff_string_line(char *pstr, int len);

// Exactly the same as stuff string only Malloc's the buffer. 
//	Supports various FreeSpace primitive types.  If 'len' is supplied, it will override
// the default string length if using the F_NAME case.
char *stuff_and_malloc_string( int type, const char *terminators, int len);
extern void stuff_float(float *f);
extern void stuff_int(int *i);
extern void stuff_byte(ubyte *i);
extern int stuff_string_list(char slp[][NAME_LENGTH], int max_strings);
extern int stuff_int_list(int *ilp, int max_ints, int lookup_type);
extern int stuff_vector_list(vector *vlp, int max_vecs);
extern void stuff_vector(vector *vp);
extern void stuff_matrix(matrix *mp);
extern int string_lookup(const char *str1, const char *strlist[], int max, const char *description = NULL, int say_errors = 0);
extern void find_and_stuff(const char *id, int *addr, int f_type, const char *strlist[], int max, const char *description);
extern int match_and_stuff(int f_type, const char *strlist[], int max, const char *description);
extern void find_and_stuff_or_add(const char *id, int *addr, int f_type, char *strlist[], int *total,
	int max, const char *description);
extern int get_string(char *str);
extern void stuff_parenthesized_vector(vector *vp);
void stuff_boolean(int *i);
int check_for_string(const char *pstr);
int check_for_string_raw(const char *pstr);

// general
extern void init_parse();
extern void reset_parse();
extern void display_parse_diagnostics();
extern void parse_main();

// utility
extern void mark_int_list(int *ilp, int max_ints, int lookup_type);
extern void compact_multitext_string(char *str);
extern void read_file_text(const char *filename, int mode = CF_TYPE_ANY );
extern void debug_show_mission_text();
extern void convert_sexp_to_string(int cur_node, char *outstr, int mode);
char *split_str_once(char *src, int max_pixel_w);
int split_str(const char *src, int max_pixel_w, int *n_chars, char **p_str, int max_lines, char ignore_char = -1);

// fred
extern int required_string_fred(const char *pstr, const char *end = NULL);
extern int required_string_either_fred(const char *str1, const char *str2);
extern int optional_string_fred(const char *pstr, const char *end = NULL, const char *end2 = NULL);
#endif

