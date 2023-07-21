/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /freespace2/code/Scramble/scramble.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for file scrambler
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/24/98 11:41p Dave
 * 
 * 1     10/24/98 11:31p Dave
 * 
 * 4     4/14/98 1:39p Lawrance
 * Add command line switches to preprocess ship and weapon tables
 * 
 * 3     3/31/98 1:14a Lawrance
 * Get .tbl and mission file encryption working.
 * 
 * 2     3/30/98 5:51p Lawrance
 * file encryption and decryption
 * 
 * 1     3/30/98 5:19p Lawrance
 *
 * $NoKeywords: $
 */

#ifndef __SCRAMBLE_H__
#define __SCRAMBLE_H__

#define PREPROCESS_SHIPS_TBL			0
#define PREPROCESS_WEAPONS_TBL		1

void scramble_file(char *src_filename, char *dest_filename = NULL, int preprocess = -1);
void unscramble_file(char *src_filename, char *dest_filename = NULL);

#endif

