/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/CFile/CfileArchive.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * External def's for CfileArchive.cpp.  This should only be used 
 * internally by cfile stuff.
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 4     5/11/98 10:59a John
 * Moved the low-level file reading code into cfilearchive.cpp.
 * 
 * 3     4/30/98 4:53p John
 * Restructured and cleaned up cfile code.  Added capability to read off
 * of CD-ROM drive and out of multiple pack files.
 * 
 * 2     12/28/97 12:42p John
 * Put in support for reading archive files; Made missionload use the
 * cf_get_file_list function.   Moved demos directory out of data tree.
 * 
 * 1     12/28/97 11:48a John
 *
 * $NoKeywords: $
 */

#ifndef _CFILEARCHIVE_H
#define _CFILEARCHIVE_H

#ifndef _CFILE_INTERNAL 
#error This file should only be included internally in CFILE!!
#endif

// The following Cfile_block data is private to cfile.cpp
// DO NOT MOVE the Cfile_block* information to cfile.h / do not extern this data
//
#define CFILE_BLOCK_UNUSED		0
#define CFILE_BLOCK_USED		1

typedef struct Cfile_block {
	int		type;				// CFILE_BLOCK_UNUSED, CFILE_BLOCK_USED
	int		dir_type;		// directory location
	FILE		*fp;				// File pointer if opening an individual file
	void		*data;			// Pointer for memory-mapped file access.  NULL if not mem-mapped.
	HANDLE	hInFile;			// Handle from CreateFile()
	HANDLE	hMapFile;		// Handle from CreateFileMapping()
	int		lib_offset;
	int		raw_position;
	int		size;				// for packed files
	
} Cfile_block;

#define MAX_CFILE_BLOCKS	64
extern Cfile_block Cfile_block_list[MAX_CFILE_BLOCKS];
extern CFILE Cfile_list[MAX_CFILE_BLOCKS];

// Called once to setup the low-level reading code.
void cf_init_lowlevel_read_code( CFILE * cfile, int offset, int size );

#endif

