/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_rate.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 * 
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     3/09/99 6:24p Dave
 * More work on object update revamping. Identified several sources of
 * unnecessary bandwidth.
 *  
 *   
 * $NoKeywords: $
 */

#ifndef _FS2_MULTI_DATA_RATE_HEADER_FILE
#define _FS2_MULTI_DATA_RATE_HEADER_FILE

// -----------------------------------------------------------------------------------------------------------------------
// MULTI RATE DEFINES/VARS
//

#define MAX_RATE_TYPE_LEN			50				// max length of a type string
#define MAX_RATE_PLAYERS			12				// how many player we'll keep track of
#define MAX_RATE_TYPES				32				// how many types we'll keep track of per player

// -----------------------------------------------------------------------------------------------------------------------
// MULTI RATE FUNCTIONS
//

// notify of a player join
void multi_rate_reset(int np_index);

// add data of the specified type to datarate processing, returns 0 on fail (if we ran out of types, etc, etc)
int multi_rate_add(int np_index, const char *type, int size);

// process. call _before_ doing network operations each frame
void multi_rate_process();

// display
void multi_rate_display(int np_index, int x, int y);

#endif

