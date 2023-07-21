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
 * low level parse routines common to all types of parsers
 *
 * $Log$
 * Revision 1.7  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.6  2002/06/17 06:15:25  relnev
 * ryan's struct patch (and cr removal)
 *
 * Revision 1.5  2002/06/09 04:41:25  relnev
 * added copyright header
 *
 * Revision 1.4  2002/05/28 06:45:38  theoddone33
 * Cleanup some stuff
 *
 * Revision 1.3  2002/05/28 06:28:20  theoddone33
 * Filesystem mods, actually reads some data files now
 *
 * Revision 1.2  2002/05/07 03:16:48  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 12    8/10/99 6:54p Dave
 * Mad optimizations. Added paging to the nebula effect.
 * 
 * 11    6/02/99 2:22p Andsager
 * Clean up warning.
 * 
 * 10    5/20/99 11:25a Andsager
 * Added error checking for parsing table files.
 * 
 * 9     2/23/99 11:18a Andsager
 * Localize launcher using strings.tbl
 * 
 * 8     2/03/99 6:06p Dave
 * Groundwork for FS2 PXO usertracker support.  Gametracker support next.
 * 
 * 7     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 6     10/29/98 12:49p Dave
 * Intermediate checkin for Fred hash table stuff. 
 * 
 * 5     10/28/98 11:30a Dave
 * Temporary checkin
 * 
 * 4     10/22/98 6:14p Dave
 * Optimized some #includes in Anim folder. Put in the beginnings of
 * parse/localization support for externalized strings and tstrings.tbl
 * 
 * 3     10/14/98 1:15p Andsager
 * Fix fred
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 98    9/21/98 2:37p Adam
 * fixed font translation bug
 * 
 * 97    7/29/98 9:39a Hoffoss
 * Fixed bug with stuff_and_malloc_string
 * 
 * 96    6/22/98 11:18a Hoffoss
 * Fixed bug where mission notes field gets an extra linefeed with each
 * save.
 * 
 * 95    6/19/98 3:53p Lawrance
 * account for some new foreign chars
 * 
 * 94    6/12/98 7:37p Hoffoss
 * Made � translate to ss, since we don't have this character in our font.
 * 
 * 93    6/12/98 4:52p Hoffoss
 * Added support for special characters in in forgeign languages.
 * 
 * 92    5/21/98 3:26p Dave
 * Fixed bug in new split string function.  Newline as last character did
 * strange things.
 * 
 * 91    5/21/98 3:01p Dave
 * Make sure split_str handles 0 length strings correctly.
 * 
 * 90    5/21/98 2:14a Hoffoss
 * Rewrote split_str() to solve a problem and it was too hard for me to
 * figure out (oow, my head hurts still!)
 * 
 * $NoKeywords: $
*/

#include	<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>

#include "pstypes.h"
#include "parselo.h"
#include "sexp.h"
#include "cfile.h"
#include "missionparse.h"
#include "ctype.h"
#include "encrypt.h"
#include "localize.h"
#include "fhash.h"

#define	ERROR_LENGTH	64
#define	RS_MAX_TRIES	5

char		Current_filename[128];
char		Error_str[ERROR_LENGTH];
int		my_errno;
int		Warning_count, Error_count;
int		fred_parse_flag = 0;
int		Token_found_flag;
jmp_buf	parse_abort;

char	Mission_text[MISSION_TEXT_SIZE];
char	Mission_text_raw[MISSION_TEXT_SIZE];
char	*Mp;
const char	*token_found;

//	Return true if this character is white space, else false.
int is_white_space(char ch)
{
	return ((ch == ' ') || (ch == '\t') || (ch == EOLN));
}

// Returns true if this character is gray space, else false (gray space is white space except for EOLN).
int is_gray_space(char ch)
{
	return ((ch == ' ') || (ch == '\t'));
}

//	Advance global Mp (mission pointer) past all current white space.
//	Leaves Mp pointing at first non white space character.
void ignore_white_space()
{
	while ((*Mp != EOF_CHAR) && is_white_space(*Mp))
		Mp++;
}

void ignore_gray_space()
{
	while ((*Mp != EOF_CHAR) && is_gray_space(*Mp))
		Mp++;
}

//	Truncate *str, eliminating all trailing white space.
//	Eg: "abc   "   becomes "abc"
//		 "abc abc " becomes "abc abc"
//		 "abc \t"   becomes "abc"
void drop_trailing_white_space(char *str)
{
	int	i;

	i = strlen(str) - 1;

	while ((i >= 0) && is_white_space(str[i]))
		i--;

	str[i+1] = 0;
}

//	Eliminate any leading whitespace in str
void drop_leading_white_space(char *str)
{
	int len, i;

	len = strlen(str);
	i = 0;

	while ((i < len) && is_white_space(str[i]))
		i++;

	memmove(str, str+i, len-i);
	str[len-i] = 0;
}

// eliminates all leading and trailing white space from a string.  Returns pointer passed in.
char *drop_white_space(char *str)
{
	int s, e;

	s = 0;
	while (str[s] && is_white_space(str[s]))
		s++;

	e = strlen(str) - 1;
	while (e > s) {
		if (!is_white_space(str[e]))
			break;

		e--;
	}

	if (e > s)
		memmove(str, str + s, e - s + 1);

	str[e - s + 1] = 0;
	return str;
}

//	Advances Mp past current token.
void skip_token()
{
	ignore_white_space();

	while ((*Mp != EOF_CHAR) && !is_white_space(*Mp))
		Mp++;
}

//	Display a diagnostic message if Verbose is set.
//	(Verbose is set if -v command line switch is present.)
void diag_printf(const char *format, ...)
{
	char	buffer[8192];
	va_list args;

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	nprintf(("Parse", "%s", buffer));
}

//	Grab and return (a pointer to) a bunch of tokens, terminating at
// ERROR_LENGTH chars, or end of line.
char *next_tokens()
{
	int	count = 0;
	char	*pstr = Mp;
	char	ch;

	while (((ch = *pstr++) != EOLN) && (ch != EOF_CHAR) && (count < ERROR_LENGTH-1))
		Error_str[count++] = ch;

	Error_str[count] = 0;
	return Error_str;
}

//	Return the line number given by the current mission pointer, ie Mp.
//	A very slow function (scans all processed text), but who cares how long
//	an error reporting function takes?
int get_line_num()
{
	int	count = 1;
	int	incomment = 0;
	int	multiline = 0;
	char	*stoploc;
	char	*p;

	p = Mission_text_raw;
	stoploc = (Mp - Mission_text) + p;

	while (p < stoploc)
	{
		if (*p == EOF_CHAR)
			Assert(0);

		if ( !incomment && (*p == COMMENT_CHAR) )
			incomment = 1;

		if ( !incomment && (*p == '/') && (*(p+1) == '*') ) {
			multiline = 1;
			incomment = 1;
		}

		if ( incomment )
			stoploc++;

		if ( multiline && (*(p-1) == '*') && (*p == '/') ) {
			multiline = 0;
			incomment = 0;
		}

#ifdef MAKE_FS1
		if ( !incomment && (*p == '/') && (*(p+1) == '/') )
			incomment = 1;
#endif

		if (*p++ == EOLN) {
			if ( !multiline && incomment )
				incomment = 0;
			count++;
		}
	}

	return count;
}

//	Call this function to display an error message.
//	error_level == 0 means this is just a warning.
//	!0 means it's an error message.
//	Prints line number and other useful information.
void error_display(int error_level, const char *format, ...)
{
	char	buffer[1024];
	char	error_text[128];
	va_list args;

	if (error_level == 0) {
		strcpy(error_text, "Warning");
		Warning_count++;
	} else {
		strcpy(error_text, "Error");
		Error_count++;
	}

	nprintf((error_text, "%s(%i):%s: ", Current_filename, get_line_num(), error_text));

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	Assert(strlen(buffer) < 1024);

	nprintf((error_text, "%s", buffer));
	Warning(LOCATION, "%s(%i):\n%s: %s", Current_filename, get_line_num(), error_text, buffer);
}

//	Advance Mp to the next eoln character.
void advance_to_eoln(const char *more_terminators)
{
	char	terminators[128];

	Assert((more_terminators == NULL) || (strlen(more_terminators) < 125));

	terminators[0] = EOLN;
	terminators[1] = (char)EOF_CHAR;
	if (more_terminators != NULL)
		strcpy(&terminators[2], more_terminators);
	else
		terminators[2] = 0;

	while (strchr(terminators, *Mp) == NULL)
		Mp++;
}

// Advance Mp to the next white space (ignoring white space inside of " marks)
void advance_to_next_white()
{
	int in_quotes = 0;

	while ((*Mp != EOLN) && (*Mp != EOF_CHAR)) {
		if (*Mp == '\"')
			in_quotes = !in_quotes;

		if (!in_quotes && is_white_space(*Mp))
			break;
		Mp++;
	}
}

// Search for specified string, skipping everything up to that point.  Returns 1 if found,
// 0 if string wasn't found (and hit end of file), or -1 if not found, but end of checking
// block was reached.
int skip_to_string(const char *pstr, const char *end)
{
	int len, len2 = 0;

	ignore_white_space();
	len = strlen(pstr);
	if (end)
		len2 = strlen(end);

	while ((*Mp != EOF_CHAR) && strnicmp(pstr, Mp, len)) {
		if (end && *Mp == '#')
			return 0;

		if (end && !strnicmp(pstr, Mp, len2))
			return -1;

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || (*Mp == EOF_CHAR))
		return 0;

	Mp += strlen(pstr);
	return 1;
}

// Advance to start of either pstr1 or pstr2.  Return 0 is successful, otherwise return !0
int skip_to_start_of_strings(const char *pstr1, const char *pstr2)
{
	int len1, len2;

	ignore_white_space();
	len1 = strlen(pstr1);
	len2 = strlen(pstr2);

	while ( (*Mp != EOF_CHAR) && strnicmp(pstr1, Mp, len1) && strnicmp(pstr2, Mp, len2) ) {
		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || (*Mp == EOF_CHAR))
		return 0;

	return 1;
}

// Find a required string.
// If not found, display an error message, but try up to RS_MAX_TRIES times
// to find the string.  (This is the groundwork for ignoring non-understood
// lines.
//	If unable to find the required string after RS_MAX_TRIES tries, then
//	abort using longjmp to parse_abort.
int required_string(const char *pstr)
{
	int	count = 0;

	ignore_white_space();

	while (strnicmp(pstr, Mp, strlen(pstr)) && (count < RS_MAX_TRIES)) {
		error_display(1, "Required token = [%s], found [%.32s].\n", pstr, next_tokens());
		advance_to_eoln(NULL);
		ignore_white_space();
		count++;
	}

	if (count == RS_MAX_TRIES) {
		nprintf(("Error", "Error: Unable to find required token [%s]\n", pstr));
		Warning(LOCATION, "Error: Unable to find required token [%s]\n", pstr);
		longjmp(parse_abort, 1);
	}

	Mp += strlen(pstr);
	diag_printf("Found required string [%s]\n", token_found = pstr);
	return 1;
}

// similar to optional_string, but just checks if next token is a match.
// It doesn't advance Mp.
//
int check_for_string(const char *pstr)
{
	ignore_white_space();

	if (!strnicmp(pstr, Mp, strlen(pstr)))
		return 1;

	return 0;
}

// like check for string, but doesn't skip past any whitespace
int check_for_string_raw(const char *pstr)
{
	if (!strnicmp(pstr, Mp, strlen(pstr))){
		return 1;
	}

	return 0;
}

// Find an optional string.
//	If found, return 1, else return 0.
//	If found, point past string, else don't update pointer.
int optional_string(const char *pstr)
{
	ignore_white_space();

	if (!strnicmp(pstr, Mp, strlen(pstr))) {
		Mp += strlen(pstr);
		return 1;
	}

	return 0;
}

int required_string_fred(const char *pstr, const char *end)
{
	char *backup = Mp;;

	token_found = pstr;
	if (fred_parse_flag)
		return 0;

	ignore_white_space();
	while (*Mp != EOF_CHAR && strnicmp(pstr, Mp, strlen(pstr))) {
		if ((*Mp == '#') || (end && !strnicmp(end, Mp, strlen(end)))) {
			Mp = NULL;
			break;
		}

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || (*Mp == EOF_CHAR)) {
		diag_printf("Required string [%s] not found\n", pstr);
		Mp = backup;
		Token_found_flag = 0;
		return 0;
	}

	Mp += strlen(pstr);
	diag_printf("Found required string [%s]\n", pstr);
	Token_found_flag = 1;
	return 1;
}

// attempt to find token in buffer.  It might not exist, however, in which case we don't need
// to do anything.  If it is found, then we advance the pointer to just after the token.  To
// further complicate things, we should only search to a certain point, since we don't want
// a token that belongs to another section which might match the token we want.  Thus, we
// also pass in an ending token, which marks the point we should stop looking at.
int optional_string_fred(const char *pstr, const char *end, const char *end2)
{
	char *mp_save = Mp;

	token_found = pstr;
	if (fred_parse_flag)
		return 0;

	ignore_white_space();
	while ((*Mp != EOF_CHAR) && strnicmp(pstr, Mp, strlen(pstr))) {
		if ((*Mp == '#') || (end && !strnicmp(end, Mp, strlen(end))) ||
			(end2 && !strnicmp(end2, Mp, strlen(end2)))) {
			Mp = NULL;
			break;
		}

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || (*Mp == EOF_CHAR)) {
		diag_printf("Optional string [%s] not found\n", pstr);
		Mp = mp_save;
		Token_found_flag = 0;
		return 0;
	}

	Mp += strlen(pstr);
	diag_printf("Found optional string [%s]\n", pstr);
	Token_found_flag = 1;
	return 1;
}

//	Return 0 or 1 for str1 match, str2 match.  Return -1 if neither matches.
//	Does not update Mp if token found.  If not found, advances, trying to
//	find the string.  Doesn't advance past the found string.
int required_string_either(const char *str1, const char *str2)
{
	int	count = 0;

	ignore_white_space();

	while (count < RS_MAX_TRIES) {
		if (strnicmp(str1, Mp, strlen(str1)) == 0) {
			// Mp += strlen(str1);
			diag_printf("Found required string [%s]\n", token_found = str1);
			return 0;
		} else if (strnicmp(str2, Mp, strlen(str2)) == 0) {
			// Mp += strlen(str2);
			diag_printf("Found required string [%s]\n", token_found = str2);
			return 1;
		}

		error_display(1, "Required token = [%s] or [%s], found [%.32s].\n", str1, str2, next_tokens());

		advance_to_eoln(NULL);
		ignore_white_space();
		count++;
	}

	if (count == RS_MAX_TRIES) {
		nprintf(("Error", "Error: Unable to find either required token [%s] or [%s]\n", str1, str2));
		Warning(LOCATION, "Error: Unable to find either required token [%s] or [%s]\n", str1, str2);
		longjmp(parse_abort, 2);
	}

	return -1;
	// exit (1);
}

//	Return 0 or 1 for str1 match, str2 match.  Return -1 if neither matches.
//	Does not update Mp if token found.  If not found, advances, trying to
//	find the string.  Doesn't advance past the found string.
int required_string_3(const char *str1, const char *str2, const char *str3)
{
	int	count = 0;

	ignore_white_space();

	while (count < RS_MAX_TRIES) {
		if (strnicmp(str1, Mp, strlen(str1)) == 0) {
			// Mp += strlen(str1);
			diag_printf("Found required string [%s]\n", token_found = str1);
			return 0;
		} else if (strnicmp(str2, Mp, strlen(str2)) == 0) {
			// Mp += strlen(str2);
			diag_printf("Found required string [%s]\n", token_found = str2);
			return 1;
		} else if (strnicmp(str3, Mp, strlen(str3)) == 0) {
			diag_printf("Found required string [%s]\n", token_found = str3);
			return 2;
		}

		error_display(1, "Required token = [%s], [%s] or [%s], found [%.32s].\n", str1, str2, str3, next_tokens());

		advance_to_eoln(NULL);
		ignore_white_space();
		count++;
	}

	return -1;
	// exit (1);
}

int required_string_either_fred(const char *str1, const char *str2)
{
	ignore_white_space();

	while (*Mp != EOF_CHAR) {
		if (!strnicmp(str1, Mp, strlen(str1))) {
			// Mp += strlen(str1);
			diag_printf("Found required string [%s]\n", token_found = str1);
			return fred_parse_flag = 0;
		
		} else if (!strnicmp(str2, Mp, strlen(str2))) {
			// Mp += strlen(str2);
			diag_printf("Found required string [%s]\n", token_found = str2);
			return fred_parse_flag = 1;
		}

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (*Mp == EOF_CHAR)
		diag_printf("Unable to find either required token [%s] or [%s]\n", str1, str2);

	return -1;
	// exit (1);
}

//	Copy characters from instr to outstr until eoln is found, or until max
//	characters have been copied (including terminator).
void copy_to_eoln(char *outstr, const char *more_terminators, const char *instr, int max)
{
	int	count = 0;
	char	ch;
	char	terminators[128];

	Assert((more_terminators == NULL) || (strlen(more_terminators) < 125));

	terminators[0] = EOLN;
	terminators[1] = (char)EOF_CHAR;
	if (more_terminators != NULL)
		strcpy(&terminators[2], more_terminators);
	else
		terminators[2] = 0;

	while (((ch = *instr++) != 0) && (strchr(terminators, ch) == NULL)  && (count < max)) {
		*outstr++ = ch;
		count++;
	}

	if (count == max)
		error_display(0, "Token too long: [%s].  Length = %i.  Max is %i.\n", next_tokens(), strlen(next_tokens()), max);

	*outstr = 0;
}

//	Copy characters from instr to outstr until next white space is found, or until max
//	characters have been copied (including terminator).
void copy_to_next_white(char *outstr, const char *instr, int max)
{
	int	count = 0;
	int	in_quotes = 0;
	char	ch;

	while (((ch = *instr++)>0) && (ch != EOLN) && (ch != EOF_CHAR) && (count < max)) {
		if ( ch == '\"' ) {
			in_quotes = !in_quotes;
			continue;
		}
		if ( !in_quotes && is_white_space(ch) )	// not in quotes, white space terminates string
			break;
		*outstr++ = ch;
		count++;
	}

	if (count == max)
		error_display(0, "Token too long: [%s].  Length = %i.  Max is %i.\n", next_tokens(), strlen(next_tokens()), max);

	*outstr = 0;
}

//	Copy text until a certain string is matched.
//	For example, this is used to copy mission notes, scanning until $END NOTES:
// is found.
void copy_text_until(char *outstr, const char *instr, const char *endstr, int max_chars)
{
	const char *foundstr;

	foundstr = strstr(instr, endstr);

	if (foundstr == NULL) {
		nprintf(("Error", "Error.  Looking for [%s], but never found it.\n", endstr));
		longjmp(parse_abort, 3);
	}

	if (foundstr - instr + strlen(endstr) < (uint) max_chars) {
		strncpy(outstr, instr, foundstr - instr);
		outstr[foundstr - instr] = 0;

	} else {
		nprintf(("Error", "Error.  Too much text (%i chars, %i allowed) before %s\n",
			foundstr - instr - strlen(endstr), max_chars, endstr));

		longjmp(parse_abort, 4);
	}

	diag_printf("Here's the partial wad of text:\n%.30s", outstr);
}

// stuffs a string into a buffer.  Can get a string between " marks and stops
// when whitespace in encounted -- not to end of line
void stuff_string_white(char *pstr)
{
	ignore_white_space();
	copy_to_next_white(pstr, Mp, NAME_LENGTH-1);
	advance_to_next_white();
}

//	Stuff a string into a string buffer.
//	Supports various FreeSpace primitive types.  If 'len' is supplied, it will override
// the default string length if using the F_NAME case.
void stuff_string(char *pstr, int type, const char *terminators, int len)
{	
	char read_str[2048] = "";
	int read_len = 2048;	
	int final_len = len;
	int tag_id;

	switch (type) {
		case F_NAME:
			if (!len){
				final_len = NAME_LENGTH;
			}
			ignore_gray_space();
			copy_to_eoln(read_str, terminators, Mp, read_len);
			drop_trailing_white_space(read_str);
			advance_to_eoln(terminators);
			break;

		case F_DATE:
			ignore_gray_space();			
			final_len = DATE_LENGTH;
			copy_to_eoln(read_str, terminators, Mp, read_len);
			drop_trailing_white_space(read_str);
			advance_to_eoln(terminators);
			break;

		case F_NOTES:
			ignore_white_space();
			final_len = NOTES_LENGTH;
			copy_text_until(read_str, Mp, "$End Notes:", read_len);
			Mp += strlen(read_str);
			required_string("$End Notes:");
			break;

		case F_FILESPEC:
			ignore_gray_space();
			final_len = FILESPEC_LENGTH;
			copy_to_eoln(read_str, terminators, Mp, read_len);
			drop_trailing_white_space(read_str);
			advance_to_eoln(terminators);
			break;
		// F_MULTITEXTOLD keeping for backwards compatability with old missions
		// can be deleted once all missions are using new briefing format

		case F_MULTITEXTOLD:		
			ignore_white_space();
			final_len = NOTES_LENGTH;
			copy_text_until(read_str, Mp, "$End Briefing Text:", read_len);
			Mp += strlen(read_str);
			required_string("$End Briefing Text:");
			break;

		case F_MULTITEXT:		
			if (!len){
				final_len = MULTITEXT_LENGTH;
			}
			ignore_white_space();
			copy_text_until(read_str, Mp, "$end_multi_text", read_len);
			Mp += strlen(read_str);
			drop_trailing_white_space(read_str);
			required_string("$end_multi_text");
			break;

		case F_PATHNAME:
			ignore_gray_space();
			final_len = PATHNAME_LENGTH;
			copy_to_eoln(read_str, terminators, Mp, read_len);
			drop_trailing_white_space(read_str);
			advance_to_eoln(terminators);
			break;

		case F_MESSAGE:
			ignore_gray_space();
			final_len = MESSAGE_LENGTH;
			copy_to_eoln(read_str, terminators, Mp, read_len);
			drop_trailing_white_space(read_str);
			advance_to_eoln(terminators);
			break;		

		default:
			Assert(0);
	}

	// now we want to do any final localization
	lcl_ext_localize(read_str, pstr, final_len, &tag_id);

	// if the hash localized text hash table is active and we have a valid external string - hash it
	if(fhash_active() && (tag_id > -2)){
		fhash_add_str(pstr, tag_id);
	}

	diag_printf("Stuffed string = [%.30s]\n", pstr);
}

// stuff a string, but only until the end of a line. don't ignore leading whitespace. close analog of fgets()/cfgets()
void stuff_string_line(char *pstr, int len)
{
	char read_str[2048] = "";
	int read_len = 2048;	
	int final_len = len;
	int tag_id;
	
	// read in a line
	final_len = len;
	copy_to_eoln(read_str, "\n", Mp, read_len);
	drop_trailing_white_space(read_str);
	advance_to_eoln("");
	Mp++;
			
	// now we want to do any final localization
	lcl_ext_localize(read_str, pstr, final_len, &tag_id);

	// if the hash localized text hash table is active and we have a valid external string - hash it
	if(fhash_active() && (tag_id > -2)){
		fhash_add_str(pstr, tag_id);
	}

	diag_printf("Stuffed string = [%.30s]\n", pstr);
}

// 1K on the stack? seems to work...
// JH: 1k isn't enough!  Command briefs can be 16k max, so changed this.
#define MAX_TMP_STRING_LENGTH 16384

// Exactly the same as stuff string only Malloc's the buffer. 
//	Supports various FreeSpace primitive types.  If 'len' is supplied, it will override
// the default string length if using the F_NAME case.
char *stuff_and_malloc_string( int type, const char *terminators, int len)
{
	int l;

	char tmp_result[MAX_TMP_STRING_LENGTH];

	stuff_string(tmp_result, type, terminators, len);
	drop_white_space(tmp_result);

	l = strlen(tmp_result);
	Assert(l < MAX_TMP_STRING_LENGTH);		// Get John!!
	if (l < 1)
		return NULL;

	return strdup(tmp_result);
}

// After reading a multitext string, you can call this function to convert any newlines into
// spaces, so it's a one paragraph string (I.e. as in MS-Word).
//
void compact_multitext_string(char *str)
{
	unsigned int i;

	for (i=0; i<strlen(str); i++)
		if (str[i] == '\n')
			str[i] = ' ';
}

#define	BUF_SIZE 2048

// Strip comments from a line of input.
int strip_comments_fred(char *readp, int in_comment)
{	
	int	ch;
	char	*writep = readp;

	while ((ch = *readp) != COMMENT_CHAR) {
		if (*readp == 0) {
			*writep = 0;
			return in_comment;
		}

		if (!in_comment) {				
			*writep = (char)ch;
			writep++;
		}
		
		readp++;		
	}

	*writep = EOLN;
	writep[1] = 0;
	
	return in_comment;	
}


// Strip comments from a line of input.
int strip_comments(char *readp, int in_comment)
{	
	int	ch;
	char	*writep = readp;

	while ((ch = *readp) != COMMENT_CHAR) {
		if (*readp == 0) {
			*writep = 0;
			return in_comment;
		}

#ifdef MAKE_FS1
		if (!in_comment && (*readp == '/') && (*(readp+1) == '/')) {
			*writep = 0;
			return in_comment;
		}
#endif

		if (!in_comment) {
			// time to do some special foreign character conversion			
			switch (ch) {
				case -4:
					ch = 129;
					break;

				case -28:
					ch = 132;
					break;

				case -10:
					ch = 148;
					break;

				case -23:
					ch = 130;
					break;

				case -30:
					ch = 131;
					break;

				case -25:
					ch = 135;
					break;

				case -21:
					ch = 137;
					break;

				case -24:
					ch = 138;
					break;

				case -17:
					ch = 139;
					break;

				case -18:
					ch = 140;
					break;

				case -60:
					ch = 142;
					break;

				case -55:
					ch = 144;
					break;

				case -12:
					ch = 147;
					break;

				case -14:
					ch = 149;
					break;

				case -5:
					ch = 150;
					break;

				case -7:
					ch = 151;
					break;

				case -42:
					ch = 153;
					break;

				case -36:
					ch = 154;
					break;

				case -31:
					ch = 160;
					break;

				case -19:
					ch = 161;
					break;

				case -13:
					ch = 162;
					break;

				case -6:
					ch = 163;
					break;

				case -32:
					ch = 133;
					break;

				case -22:
					ch = 136;
					break;

				case -20:
					ch = 141;
					break;
			}			

			*writep = (char)ch;
			writep++;
		}
		
		readp++;	
	}

	*writep = EOLN;
	writep[1] = 0;
	
	return in_comment;	
}

#ifdef MAKE_FS1
void strip_all_comments( char *readp, char *writep )
{
	int	ch;
	//char	*writep = readp;

	while ( *readp && *readp != EOF_CHAR ) {
		ch = *readp;
		if ( ch == COMMENT_CHAR ) {
			while ( *readp != EOLN )
				readp++;

			*writep = EOLN;
			writep++;
			// get to next character after EOLN
			readp++;
		} else if ( (ch == '/') && (readp[1] == '*')) {			// Start of multi-line comment
			int done;
			
			done = 0;
			while ( !done ) {
				while ( *readp != '*' )
					readp++;
				if ( readp[1] == '/' ) {
					readp += 2;
					done = 1;
				} else {
					readp++;
				}
			}
		} else {
			*writep = (char)ch;
			writep++;
			readp++;
		}
	}

	*writep = (char)EOF_CHAR;
}
#endif

int parse_get_line(char *lineout, int max_line_len, const char *start, int max_size, const char *cur)
{
	char * t = lineout;
	int i, num_chars_read=0;
	char c;


	for ( i = 0; i < max_line_len-1; i++ ) {
		do {
			if ( (cur - start) >= max_size ) {
				*lineout = 0;
				if ( lineout > t ) {		
					return num_chars_read;
				} else {
					return 0;
				}
			}
			c = *cur++;
			num_chars_read++;
		} while ( c == 13 );

		*lineout++ = c;
		if ( c=='\n' ) break;
	}

	*lineout++ = 0;
	return  num_chars_read;
}

//	Read mission text, stripping comments.
//	When a comment is found, it is removed.  If an entire line
//	consisted of a comment, a blank line is left in the input file.
void read_file_text(const char *filename, int mode)
{
	CFILE	*mf;
	char	outbuf[BUF_SIZE], *str;
	char	*mp = Mission_text;
	char	*mp2 = Mission_text_raw;
	int	file_is_encrypted = 0, in_comment = 0;

	if (!filename)
		longjmp(parse_abort, 10);

	strcpy(Current_filename, filename);
	mf = cfopen(filename, "rb", CFILE_NORMAL, mode);
	if (mf == NULL) {
		nprintf(("Error", "Wokka!  Error opening mission.txt!\n"));
		longjmp(parse_abort, 5);
	}

	// read the entire file in
	int file_len = cfilelength(mf);

	// read first 10 bytes to determine if file is encrypted
	cfread(Mission_text_raw, min(file_len, 10), 1, mf);
	file_is_encrypted = is_encrpyted(Mission_text_raw);
	cfseek(mf, 0, CF_SEEK_SET);

	if ( file_is_encrypted ) {
		int	unscrambled_len;
		char	*scrambled_text;
		scrambled_text = (char*)malloc(file_len+1);
		Assert(scrambled_text);
		cfread(scrambled_text, file_len, 1, mf);
		// unscramble text
		unencrypt(scrambled_text, file_len, Mission_text_raw, &unscrambled_len);
		file_len = unscrambled_len;
		free(scrambled_text);
	} else {
		cfread(Mission_text_raw, file_len, 1, mf);
	}

	cfclose(mf);

	//	If you hit this assert, it is probably telling you the obvious.  The file
	//	you are trying to read is truly too large.  Look at *filename to see the file name.
	Assert(file_len < MISSION_TEXT_SIZE);

	// strip comments from raw text, reading into Mission_text

	int num_chars_read = 0;

#ifndef MAKE_FS1
	mp2 = Mission_text_raw;
#else
	strip_all_comments(Mission_text_raw, mp2);
#endif
	while ( (num_chars_read = parse_get_line(outbuf, BUF_SIZE, Mission_text_raw, file_len, mp2)) != 0 ) {
		mp2 += num_chars_read;

		if(Fred_running){
			in_comment = strip_comments_fred(outbuf, in_comment);
		} else {
			in_comment = strip_comments(outbuf, in_comment);
		}
		str = outbuf;
		while (*str) {
			if (*str == -33) {
				*mp++ = 's';
				*mp++ = 's';
				str++;

			} else
				*mp++ = *str++;
		}

//		strcpy(mp, outbuf);
//		mp += strlen(outbuf);
	}
	
	*mp = *mp2 = (char)EOF_CHAR;
/*
	while (cfgets(outbuf, BUF_SIZE, mf) != NULL) {
		if (strlen(outbuf) >= BUF_SIZE-1)
			error_display(0, "Input string too long.  Max is %i characters.\n%.256s\n", BUF_SIZE, outbuf);

		//	If you hit this assert, it is probably telling you the obvious.  The file
		//	you are trying to read is truly too large.  Look at *filename to see the file name.
		Assert(mp2 - Mission_text_raw + strlen(outbuf) < MISSION_TEXT_SIZE);
		strcpy(mp2, outbuf);
		mp2 += strlen(outbuf);

		in_comment = strip_comments(outbuf, in_comment);
		strcpy(mp, outbuf);
		mp += strlen(outbuf);
	}
	
	*mp = *mp2 = (char)EOF_CHAR;
*/

}

void debug_show_mission_text()
{
	char	*mp = Mission_text;
	char	ch;

	while ((ch = *mp++) != EOF_CHAR)
		printf("%c", ch);
}

float atof2()
{
	char	ch;

	my_errno = 0;
	ignore_white_space();

	ch = *Mp;

	if ((ch != '.') && (ch != '-') && (ch != '+') && ((ch < '0') || (ch > '9'))) {
		error_display(1, "Expecting float, found [%.32s].\n", next_tokens());
		my_errno = 1;
		return 0.0f;
	} else
		return (float)atof(Mp);

}

int atoi2()
{
	char	ch;

	my_errno = 0;

	ignore_white_space();

	ch = *Mp;

	if ((ch != '-') && (ch != '+') && ((ch < '0') || (ch > '9'))) {
		error_display(1, "Expecting int, found [%.32s].\n", next_tokens());
		my_errno = 1;
		return 0;
	} else
		return atoi(Mp);

}


//	Stuff a floating point value pointed at by Mp.
//	Advances past float characters.
void stuff_float(float *f)
{
	*f = atof2();

	if (my_errno)
		skip_token();
	else
		Mp += strspn(Mp, "+-0123456789.");

	if (*Mp ==',')
		Mp++;

	diag_printf("Stuffed float: %f\n", *f);
}

//	Stuff an integer value pointed at by Mp.
//	Advances past integer characters.
void stuff_int(int *i)
{
	*i = atoi2();

	if (my_errno)
		skip_token();
	else
		Mp += strspn(Mp, "+-0123456789");

	if (*Mp ==',')
		Mp++;

	diag_printf("Stuffed int: %i\n", *i);
}


// Stuffs a boolean value pointed at by Mp.  
// YES/NO (supporting 1/0 now as well)
//
void stuff_boolean(int *i)
{
	char token[512];
	ignore_white_space();
	copy_to_eoln(token, NULL, Mp, 512);
	drop_trailing_white_space(token);
	advance_to_eoln(NULL);

	if ( isdigit(token[0]) ) {					// using 1/0 instead of YES/NO
		int int_value;
		int_value = atoi(token);
		if ( int_value )
			*i = 1;
		else
			*i = 0;
	}
	else {
		if ( !stricmp(token, "yes") ) {
			*i = 1;
		}
		else if ( !stricmp(token, "no") ) {
			*i = 0;
		}
		else
			Assert(0);	// can't happen
	}
}


//	Stuff an integer value pointed at by Mp.
//	Advances past integer characters.
void stuff_byte(ubyte *i)
{
	int	temp;

	temp = atoi2();

	*i = (ubyte)temp;

	if (my_errno)
		skip_token();
	else
		Mp += strspn(Mp, "+-0123456789");

	if (*Mp == ',')
		Mp++;

	diag_printf("Stuffed byte: %i\n", *i);
}

// Stuffs a list of strings
int stuff_string_list(char slp[][NAME_LENGTH], int max_strings)
{
	int count = 0;
	ignore_white_space();

	if ( *Mp != '(' ) {
		error_display(1, "Reading string list.  Found [%c].  Expecting '('.\n", *Mp);
		longjmp(parse_abort, 100);
	}

	Mp++;

	ignore_white_space();

	while (*Mp != ')') {
		assert ( count < max_strings );
		Assert ( *Mp == '\"' );					// should always be enclosed in quotes

		get_string( slp[count++] );
		ignore_white_space();
	}

	Mp++;

	return count;
}

//	Stuffs an integer list.
//	This is of the form ( i* )
//	  where i is an integer.
// For example, (1) () (1 2 3) ( 1 ) are legal integer lists.
int stuff_int_list(int *ilp, int max_ints, int lookup_type)
{
	int	count = 0, ok_flag = 1, dummy;
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading integer list.  Found [%c].  Expecting '('.\n", *Mp);
		longjmp(parse_abort, 6);
	}

	Mp++;
	ignore_white_space();

	while (*Mp != ')') {
		Assert(count < max_ints);
		if (*Mp == '"') {
			int num = 0;
			char str[128];

			get_string(str);
			switch (lookup_type) {
				case SHIP_TYPE:
					num = ship_name_lookup(str);	// returns index of Ship[] entry with name
					break;

				case SHIP_INFO_TYPE:
					ok_flag = 1;
					num = ship_info_lookup(str);	// returns index of Ship_info[] entry with name
					if (num < 0)
						ok_flag = 0;
					break;

				case WEAPON_POOL_TYPE:
					ok_flag = 1;
					num = weapon_info_lookup(str);
					if (num < 0)
						ok_flag = 0;
					break;

				case WEAPON_LIST_TYPE:
					num = weapon_info_lookup(str);
					if (num < 0)
						num = -2;
					break;

				case RAW_INTEGER_TYPE:
					num = atoi(str);
					break;

				default:
					Error(LOCATION,"Unknown lookup_type in stuff_int_list");
					break;
			}

			if (ok_flag) {
				if (num == -1) {
					Error(LOCATION, "Unable to find string \"%s\" in stuff_int_list\n\nMany possible sources for this error.  Get a programmer!\n", str);
				} else if (num == -2) {
					if (strlen(str) > 0) {
						Warning(LOCATION, "Unable to find WEAPON_LIST_TYPE string \"%s\" in stuff_int_list\n\nMany possible sources for this error.  Get a programmer!\n", str);
					}
				}

				if (num < 0)  // other negatives used to bypass the above error trap, but should be -1
					num = -1;

				ilp[count++] = num;
			}

		} else {
			if (ok_flag)
				stuff_int(&ilp[count++]);
			else
				stuff_int(&dummy);
		}
		
		ignore_white_space();
	}

	Mp++;

	return count;
}

//	Marks an integer list.
//	This is of the form ( i* )
//	  where i is an integer.
//	If a specified string is found in the lookup and its value is 7, then the 7th value
//	in the array is set.
void mark_int_list(int *ilp, int max_ints, int lookup_type)
{
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Marking integer list.  Found [%c].  Expecting '('.\n", *Mp);
		longjmp(parse_abort, 6);
	}

	Mp++;
	ignore_white_space();

	while (*Mp != ')') {
		if (*Mp == '"') {
			int num = 0;
			char str[128];

			get_string(str);
			switch(lookup_type) {
				case SHIP_TYPE:
					num = ship_name_lookup(str);	// returns index of Ship[] entry with name
					break;
			
				case SHIP_INFO_TYPE:
					num = ship_info_lookup(str);	// returns index of Ship_info[] entry with name
					break;
	
				case WEAPON_LIST_TYPE:
					num = weapon_info_lookup(str);
					break;

				default:
					Error(LOCATION,"Unknown lookup_type in stuff_int_list");
					break;
			}

			if ( (num < 0) || (num >= max_ints) )
				Error(LOCATION, "Unable to find string \"%s\" in mark_int_list.\n", str);

//			ilp[num] = 1;
		
		} else {
			int	tval;

			stuff_int(&tval);
			Assert((tval >= 0) && (tval < max_ints));
			ilp[tval] = 1;
		}
		
		ignore_white_space();
	}

	Mp++;

}


//	Stuff a vector, which is 3 floats.
void stuff_vector(vector *vp)
{
	stuff_float(&vp->xyz.x);
	stuff_float(&vp->xyz.y);
	stuff_float(&vp->xyz.z);
}

void stuff_parenthesized_vector(vector *vp)
{
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading parenthesized vector.  Found [%c].  Expecting '('.\n", *Mp);
		longjmp(parse_abort, 11);
	} else {
		Mp++;
		stuff_vector(vp);
		ignore_white_space();
		if (*Mp != ')') {
			error_display(1, "Reading parenthesized vector.  Found [%c].  Expecting ')'.\n", *Mp);
			longjmp(parse_abort, 12);
		}
		Mp++;
	}

}

//	Stuffs vector list.
//	This is of the form ( (vector)* )
//	  where vector is a vector
// For example, ( (1 2 3) (2 3 4) (2 3 5) )
//		 is a vector list of three vectors.
int stuff_vector_list(vector *vlp, int max_vecs)
{
	int	count = 0;

	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading integer list.  Found [%c].  Expecting '('.\n", *Mp);
		longjmp(parse_abort, 6);
	}

	Mp++;

	ignore_white_space();

	while (*Mp != ')') {
		Assert(count < max_vecs);
		stuff_parenthesized_vector(&vlp[count++]);
		
		ignore_white_space();
	}

	Mp++;

	return count;
}


//	Stuff a matrix, which is 3 vectors.
void stuff_matrix(matrix *mp)
{
	stuff_vector(&mp->v.rvec);
	stuff_vector(&mp->v.uvec);
	stuff_vector(&mp->v.fvec);
}


//	Given a string, find it in a string array.
//	*descrtiption is only used for diagnostics in case it can't be found.
//	*str1 is the string to be found.
//	*strlist is the list of strings to search.
//	max is the number of entries in *strlist to scan.
int string_lookup(const char *str1, const char *strlist[], int max, const char *description, int say_errors)
{
	int	i;

	for (i=0; i<max; i++) {
		Assert(strlen(strlist[i]) != 0);

		if (!stricmp(str1, strlist[i]))
			return i;
	}

	if (say_errors)
		error_display(0, "Unable to find [%s] in %s list.\n", str1, description);

	return -1;
}

//	Find a required string (*id), then stuff the text of type f_type that
// follows it at *addr.  *strlist[] contains the strings it should try to
// match.
void find_and_stuff(const char *id, int *addr, int f_type, const char *strlist[], int max, const char *description)
{
	char	token[128];

	required_string(id);
	stuff_string(token, f_type, NULL);
	*addr = string_lookup(token, strlist, max, description, 1);
}

//	Mp points at a string.
//	Find the string in the list of strings *strlist[].
// Returns the index of the match, -1 if none.
int match_and_stuff(int f_type, const char *strlist[], int max, const char *description)
{
	char	token[128];

	stuff_string(token, f_type, NULL);
	return string_lookup(token, strlist, max, description, 0);
}

void find_and_stuff_or_add(const char *id, int *addr, int f_type, char *strlist[], int *total,
	int max, const char *description)
{
	char	token[128];

	*addr = -1;
	required_string(id);
	stuff_string(token, f_type, NULL);
	if (*total)
		*addr = string_lookup(token, (const char **)strlist, *total, description, 0);

	if (*addr == -1)  // not in list, so lets try and add it.
	{
		Assert(*total < max);
		strcpy(strlist[*total], token);
		*addr = (*total)++;
	}
}

// Initialize a parse process.
void init_parse()
{
	int i;

	Mp = Mission_text;

	Warning_count = 0;
	Error_count = 0;
	for (i=0; i<MAX_CARGO; i++)
		Cargo_names[i] = Cargo_names_buf[i]; // make a pointer array for compatibility

	Total_goal_ship_names = 0;
	init_sexp();
}

void reset_parse()
{
	Mp = Mission_text;

	Warning_count = 0;
	Error_count = 0;
}

// Display number of warnings and errors at the end of a parse.
void display_parse_diagnostics()
{
	nprintf(("Parse", "\nParse complete.\n"));
	nprintf(("Parse", "%i errors.  %i warnings.\n", Error_count, Warning_count));
}

// Splits a string into 2 lines if the string is wider than max_pixel_w pixels.  A null
// terminator is placed where required to make the first line <= max_pixel_w.  The remaining
// text is returned (leading whitespace removed).  If the line doesn't need to be split,
// NULL is returned.
char *split_str_once(char *src, int max_pixel_w)
{
	char *brk = NULL;
	int i, w, len, last_was_white = 0;

	Assert(src);
	Assert(max_pixel_w > 0);
	
	gr_get_string_size(&w, NULL, src);
	if ( w <= max_pixel_w )
		return NULL;  // string doesn't require a cut

	len = strlen(src);
	for (i=0; i<len; i++) {
		gr_get_string_size(&w, NULL, src, i);
		if ( w > max_pixel_w )
			break;

		if (src[i] == '\n') {  // reached natural end of line
			src[i] = 0;
			return src + i + 1;
		}

		if (is_white_space(src[i])) {
			if (!last_was_white)
				brk = src + i;

			last_was_white = 1;

		} else {
			last_was_white = 0;
		}
	}

	if (!brk) {
		brk = src + i;
	}

	*brk = 0;
	src = brk + 1;
	while (is_white_space(*src))
		src++;

	if (!*src)
		return NULL;  // end of the string anyway
		
	if (*src == '\n')
		src++;

	return src;
}

#define SPLIT_STR_BUFFER_SIZE	512

// --------------------------------------------------------------------------------------
// split_str() 
//
// A general function that will split a string into several lines.  Lines are allowed up
// to max_pixel_w pixels.  Breaks are found in white space.
//
// Supports \n's in the strings!
//
// parameters:		src			=>		source string to be broken up
//						max_pixel_w	=>		max width of line in pixels
//						n_chars		=>		output array that will hold number of characters in each line
//						p_str			=>		output array of pointers to start of lines within src
//						max_lines	=>		limit of number of lines to break src up into
//						ignore_char	=>		OPTIONAL parameter (default val -1).  Ignore words starting with this character
//												This is useful when you want to ignore embedded control information that starts
//												with a specific character, like $ or #
//
//	returns:			number of lines src is broken into
//						-1 is returned when an error occurs
//
int split_str(const char *src, int max_pixel_w, int *n_chars, char **p_str, int max_lines, char ignore_char)
{
	char buffer[SPLIT_STR_BUFFER_SIZE];
	char *breakpoint = NULL;
	int sw, new_line = 1, line_num = 0, last_was_white = 0;
	int ignore_until_whitespace, buf_index;
	
	// check our assumptions..
	Assert(src != NULL);
	Assert(n_chars != NULL);
	Assert(p_str != NULL);
	Assert(max_lines > 0);
	Assert(max_pixel_w > 0);
	
	memset(buffer, 0, SPLIT_STR_BUFFER_SIZE);
	buf_index = 0;
	ignore_until_whitespace = 0;

	// get rid of any leading whitespace
	while (is_white_space(*src))
		src++;

	new_line = 1;
	p_str[0] = NULL;

	// iterate through chars in line, keeping track of most recent "white space" location that can be used
	// as a line splitting point if necessary
	for (; *src; src++) {
		if (line_num >= max_lines)
			return line_num;  // time to bail out

		// starting a new line of text, init stuff for that
		if (new_line) {
			p_str[line_num] = NULL;
			if (is_gray_space(*src))
				continue;

			p_str[line_num] = (char*)src;
			breakpoint = NULL;
			new_line = 0;
		}

		// maybe skip leading whitespace
		if (ignore_until_whitespace) {
			if ( is_white_space(*src) )
				ignore_until_whitespace = 0;

			continue;
		}

		// if we have a newline, split the line here
		if (*src == '\n') {
			n_chars[line_num] = src - p_str[line_num];  // track length of line
			line_num++;
			p_str[line_num] = NULL;
			new_line = 1;

			memset(buffer, 0, SPLIT_STR_BUFFER_SIZE);
			buf_index = 0;
			continue;
		}

		if (*src == ignore_char) {
			ignore_until_whitespace = 1;
			continue;
		}

		if (is_gray_space(*src)) {
			if (!last_was_white)  // track at first whitespace in a series of whitespace
				breakpoint = (char*)src;

			last_was_white = 1;

		} else {
			// indicate next time around that this wasn't a whitespace character
			last_was_white = 0;
		}

		// throw it in our buffer
		buffer[buf_index] = *src;
		buf_index++;
		buffer[buf_index] = 0;  // null terminate it
	
		gr_get_string_size(&sw, NULL, buffer);
		if (sw >= max_pixel_w) {
			char *end;

			if (breakpoint) {
				end = breakpoint;
				src = breakpoint;
			} else {
				end = (char*)src;  // force a split here since to whitespace
				src--;  // reuse this character in next line
			}

			n_chars[line_num] = end - p_str[line_num];  // track length of line
			Assert(n_chars[line_num]);
			line_num++;
			p_str[line_num] = NULL;
			new_line = 1;

			memset(buffer, 0, SPLIT_STR_BUFFER_SIZE);
			buf_index = 0;
			continue;
		}
	}	// end for

	if (p_str[line_num]) {
		n_chars[line_num] = src - p_str[line_num];  // track length of line
		Assert(n_chars[line_num]);
		line_num++;
	}

	return line_num;
}

