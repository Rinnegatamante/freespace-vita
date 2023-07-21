/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_log.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Header file to support multiplayer logging functions
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:47  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 8     8/17/99 2:24p Dave
 * Fixed wacky squad color stuff.
 * 
 * 7     6/07/99 9:51p Dave
 * Consolidated all multiplayer ports into one.
 * 
 * 6     4/27/99 2:59p Dave
 * Potential fix for reliable socket connection problem.
 * 
 * 5     4/09/99 2:21p Dave
 * Multiplayer beta stuff. CD checking.
 * 
 * 4     1/24/99 11:37p Dave
 * First full rev of beam weapons. Very customizable. Removed some bogus
 * Int3()'s in low level net code.
 * 
 * 3     11/19/98 4:19p Dave
 * Put IPX sockets back in psnet. Consolidated all multiplayer config
 * files into one.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 3     8/21/98 1:14p Dave
 * Put in log system hooks in useful places.
 * 
 * 2     8/20/98 5:31p Dave
 * Put in handy multiplayer logfile system. Now need to put in useful
 * applications of it all over the code.
 * 
 * 1     8/20/98 2:00p Dave
 *  
 *
 * $NoKeywords: $
 */

#include <stdio.h>
#include <stdarg.h>
#include "multi_log.h"
#include "multi_options.h"
#include "cmdline.h"
#include "cfile.h"

// ----------------------------------------------------------------------------------------------------
// MULTI LOGFILE DEFINES/VARS
//

// max length for a line of the logfile
#define MAX_LOGFILE_LINE_LEN					256

// name of the multiplayer logfile
#define MULTI_LOGFILE_NAME						"multi.log"

// echo all ml_printf's to the debug window
#define MULTI_LOGFILE_ECHO_TO_DEBUG

// how often we'll write an update to the logfile (in seconds)
#define MULTI_LOGFILE_UPDATE_TIME			2520			// every 42 minutes

// outfile itself
CFILE *Multi_log_out = NULL;

// time when the logfile was opened
int Multi_log_open_systime = -1;

// time when we last updated the logfile
int Multi_log_update_systime = -1;

// ----------------------------------------------------------------------------------------------------
// MULTI LOGFILE FUNCTIONS
//

// write the standard header to the logfile
void multi_log_write_header()
{
	char str[1024];
	time_t timer;	

	// header message
	timer = time(NULL);	
	strftime(str, 1024, "Freespace Multi Log - Opened %a, %b %d, %Y  at %I:%M%p\n----\n----\n----\n\n", localtime(&timer));
	ml_string(str, 0);	
}

// write the standard shutdown trailer
void multi_log_write_trailer()
{
	char str[1024];
	time_t timer;		

	// header message
	timer = time(NULL);
	strftime(str, 1024, "\n\n----\n----\n----\nFreespace Multi Log - Closing on %a, %b %d, %Y  at %I:%M%p", localtime(&timer));
	ml_string(str, 0);	
}

// write out some info about stuff
void multi_log_write_update()
{
	int diff = (int)difftime(time(NULL), Multi_log_open_systime);
	int hours, mins, seconds;

	// figure out some time values
	hours = diff / 3600;
	mins = (diff - (hours * 3600)) / 60;
	seconds = (diff - (hours * 3600) - (mins * 60));

	// print it out
	ml_printf("Server has been active for %d hours, %d minutes, and %d seconds", hours, mins, seconds);
}

// initialize the multi logfile
void multi_log_init()
{
	// attempt to open the file
	Multi_log_out = cfopen(MULTI_LOGFILE_NAME, "wt", CFILE_NORMAL, CF_TYPE_DATA);

	// if we successfully opened the file, write the header
	if(Multi_log_out != NULL){
		multi_log_write_header();

		// initialize our timer info
		Multi_log_open_systime = time(NULL);
		Multi_log_update_systime = Multi_log_open_systime;
	} else {
		nprintf(("Network","Error opening %s for writing!!\n",MULTI_LOGFILE_NAME));
	}
}

// close down the multi logfile
void multi_log_close()
{
	// if we have a valid file, write a trailer and close
	if(Multi_log_out != NULL){
		multi_log_write_trailer();

		cfclose(Multi_log_out);
		Multi_log_out = NULL;
	}
}

// give some processing time to the logfile system so it can check up on stuff
void multi_log_process()
{
	// if we don't have a valid logfile, do nothing
	if(Multi_log_out == NULL){
		return;
	}

	// check to see if we've been active a long enough time, and 
	if(time(NULL) - Multi_log_update_systime > MULTI_LOGFILE_UPDATE_TIME){
		// write the update
		multi_log_write_update();

		Multi_log_update_systime = time(NULL);
	}
}

// printf function itself called by the ml_printf macro
void ml_printf(const char *format, ...)
{
	char tmp[MAX_LOGFILE_LINE_LEN*4];
	va_list args;

	// if we don't have a valid logfile do nothing
	if(Multi_log_out == NULL){
		return;
	}
	
	// format the text
	va_start(args, format);
	vsprintf(tmp, format, args);
	va_end(args);
	
	// log the string
	ml_string(tmp);
}

// string print function
void ml_string(const char *string, int add_time)
{
	char tmp[MAX_LOGFILE_LINE_LEN*4];
	char time_str[128];
	time_t timer;	

	// if we don't have a valid logfile do nothing
	if(Multi_log_out == NULL){
		return;
	}

	// if the passed string is NULL, do nothing
	if(string == NULL){
		return;
	}

	// maybe add the time
	if(add_time){
		timer = time(NULL);

		strftime(time_str, 128, "%m/%d %H:%M:%S~   ", localtime(&timer));
		strcpy(tmp, time_str);
		strcat(tmp, string);
	} else{
		strcpy(tmp, string);
	}
	strcat(tmp, "\n");

	// now print it to the logfile if necessary	
	cfputs(tmp, Multi_log_out);
	cflush(Multi_log_out);

#if defined(MULTI_LOGFILE_ECHO_TO_DEBUG)
	// nprintf(("Network","%s\n",tmp));
	mprintf(("ML %s", tmp));
#endif
}
