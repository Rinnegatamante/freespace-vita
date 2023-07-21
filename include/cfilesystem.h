/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/CFile/CfileSystem.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Functions to keep track of and find files that can exist
 * on the harddrive, cd-rom, or in a pack file on either of those.
 * This keeps a list of all the files in packfiles or on CD-rom
 * and when you need a file you call one function which then searches
 * all those locations, inherently enforcing precedence orders.
 *
 * $Log$
 * Revision 1.4  2004/07/04 11:26:21  taylor
 * cleanup CFILE code a little, warning fixes, remove redundant dir checks
 *
 * Revision 1.3  2003/05/27 03:23:23  taylor
 * fix second root (gamedir) searching
 *
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 3     10/13/98 9:19a Andsager
 * Add localization support to cfile.  Optional parameter with cfopen that
 * looks for localized files.
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 5     5/01/98 10:21a John
 * Added code to find all pack files in all trees.   Added code to create
 * any directories that we write to.
 * 
 * 4     4/30/98 10:21p John
 * Added code to cleanup cfilesystem
 *
 * $NoKeywords: $
 */

#ifndef _CFILESYSTEM_H
#define _CFILESYSTEM_H

// Builds a list of all the files
void cf_build_secondary_filelist( const char *cdrom_path );
void cf_free_secondary_filelist();

// Internal stuff
typedef struct cf_pathtype {
	int		index;					// To verify that the CF_TYPE define is correctly indexed into this array
	const char	*path;					// Path relative to Freespace root, has ending backslash.
	const char	*extensions;			// Extensions used in this pathtype, separated by spaces
	int		parent_index;			// Index of this directory's parent.  Used for creating directories when writing.
} cf_pathtype;

// During cfile_init, verify that Pathtypes[n].index == n for each item
extern cf_pathtype Pathtypes[CF_MAX_PATH_TYPES];

// Returns the default storage path for files given a 
// particular pathtype.   In other words, the path to 
// the unpacked, non-cd'd, stored on hard drive path.
// If filename isn't null it will also tack the filename
// on the end, creating a completely valid filename.
// Input:   pathtype  - CF_TYPE_??
//          filename  - optional, if set, tacks the filename onto end of path.
// Output:  path      - Fully qualified pathname.
void cf_create_default_path_string( char *path, int pathtype, const char *filename=NULL, bool localize = false);


#endif	//_CFILESYSTEM_H
