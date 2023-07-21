/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/CFile/CfileArchive.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Low-level code for reading data out of large archive files or normal files.  All
 * reads/seeks come through here.
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 4     2/22/99 10:31p Andsager
 * Get rid of unneeded includes.
 * 
 * 3     1/06/99 2:24p Dave
 * Stubs and release build fixes.
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 12    5/14/98 8:10p Lawrance
 * Fix bug with trying to read past the end of a file (when from a
 * packfile).
 * 
 * 11    5/11/98 11:48a John
 * fixed bug with memory mapped files
 * 
 * 10    5/11/98 10:59a John
 * added in debug code
 * 
 * 9     5/11/98 10:59a John
 * Moved the low-level file reading code into cfilearchive.cpp.
 * 
 * 8     4/30/98 4:53p John
 * Restructured and cleaned up cfile code.  Added capability to read off
 * of CD-ROM drive and out of multiple pack files.
 * 
 * 7     4/20/98 11:49a Hoffoss
 * Fixed bug with directory name getting.
 * 
 * 6     4/10/98 12:18a Hoffoss
 * Make pilot image search in pack file possible.
 * 
 * 5     1/19/98 9:37p Allender
 * Great Compiler Warning Purge of Jan, 1998.  Used pragma's in a couple
 * of places since I was unsure of what to do with code.
 * 
 * 4     12/30/97 5:31p Lawrance
 * fixed problem for people that didn't have a VP file
 * 
 * 3     12/30/97 4:31p Sandeep
 * Changed pakfile format
 * 
 * 2     12/28/97 12:42p John
 * Put in support for reading archive files; Made missionload use the
 * cf_get_file_list function.   Moved demos directory out of data tree.
 * 
 * 1     12/28/97 11:48a John
 *
 * $NoKeywords: $
 */

#define _CFILE_INTERNAL 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef PLAT_UNIX
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <winbase.h>		/* needed for memory mapping of file functions */
#endif

#include "pstypes.h"
#include "cfile.h"
//#include "outwnd.h"
//#include "vecmat.h"
//#include "timer.h"
#include "cfilearchive.h"

#define CHECK_POSITION

// Called once to setup the low-level reading code.

void cf_init_lowlevel_read_code( CFILE * cfile, int offset, int size )
{
	Assert(cfile != NULL);

	Cfile_block *cb;
	Assert(cfile->id >= 0 && cfile->id < MAX_CFILE_BLOCKS);
	cb = &Cfile_block_list[cfile->id];	

	cb->lib_offset = offset;
	cb->raw_position = 0;
	cb->size = size;

	if ( cb->fp )	{
		if ( cb->lib_offset )	{
			fseek( cb->fp, cb->lib_offset, SEEK_SET );
		}

		#if defined(CHECK_POSITION) && !defined(NDEBUG)
			int raw_position = ftell(cb->fp) - cb->lib_offset;
			Assert(raw_position == cb->raw_position);
		#endif
	}
}



// cfeof() Tests for end-of-file on a stream
//
// returns a nonzero value after the first read operation that attempts to read
// past the end of the file. It returns 0 if the current position is not end of file.
// There is no error return.

int cfeof(CFILE *cfile)
{
	Assert(cfile != NULL);

	Cfile_block *cb;
	Assert(cfile->id >= 0 && cfile->id < MAX_CFILE_BLOCKS);
	cb = &Cfile_block_list[cfile->id];	

	int result;

	result = 0;

	// cfeof() not supported for memory-mapped files
	Assert( !cb->data );

	Assert(cb->fp != NULL);

	#if defined(CHECK_POSITION) && !defined(NDEBUG)
	int raw_position = ftell(cb->fp) - cb->lib_offset;
	Assert(raw_position == cb->raw_position);
	#endif
		
	if (cb->raw_position >= cb->size ) {
		result = 1;
	} else {
		result = 0;
	}

	return result;	
}

// cftell() returns offset into file
//
// returns:  success ==> offset into the file
//           error   ==> -1
//
int cftell( CFILE * cfile )
{
	Assert(cfile != NULL);
	Cfile_block *cb;
	Assert(cfile->id >= 0 && cfile->id < MAX_CFILE_BLOCKS);
	cb = &Cfile_block_list[cfile->id];	

	// Doesn't work for memory mapped files
	Assert( !cb->data );

	Assert(cb->fp != NULL);

	#if defined(CHECK_POSITION) && !defined(NDEBUG)
	int raw_position = ftell(cb->fp) - cb->lib_offset;
	Assert(raw_position == cb->raw_position);
	#endif

	return cb->raw_position;
}


// cfseek() moves the file pointer
//
// returns:   success ==> 0
//            error   ==> non-zero
//
int cfseek( CFILE *cfile, int offset, int where )
{

	Assert(cfile != NULL);
	Cfile_block *cb;
	Assert(cfile->id >= 0 && cfile->id < MAX_CFILE_BLOCKS);
	cb = &Cfile_block_list[cfile->id];	


	// TODO: seek to offset in memory mapped file
	Assert( !cb->data );
	Assert( cb->fp != NULL );
	
	int goal_position;

	switch( where )	{
	case CF_SEEK_SET:
		goal_position = offset+cb->lib_offset;
		break;
	case CF_SEEK_CUR:	
		{
			goal_position = cb->raw_position+offset+cb->lib_offset;
		}
		break;
	case CF_SEEK_END:
		goal_position = cb->size+offset+cb->lib_offset;
		break;
	default:
		Int3();
		return 1;
	}	

	int result = fseek(cb->fp, goal_position, SEEK_SET );
	cb->raw_position = goal_position - cb->lib_offset;

	#if defined(CHECK_POSITION) && !defined(NDEBUG)
		int tmp_offset = ftell(cb->fp) - cb->lib_offset;
		Assert(tmp_offset==cb->raw_position);
	#endif

	return result;	
}


// cfread() reads from a file
//
// returns:   returns the number of full elements read
//            
//
int cfread(void *buf, int elsize, int nelem, CFILE *cfile)
{
	Assert(cfile != NULL);
	Assert(buf != NULL);
	Assert(cfile->id >= 0 && cfile->id < MAX_CFILE_BLOCKS);

	Cfile_block *cb;
	cb = &Cfile_block_list[cfile->id];	

	// cfread() not supported for memory-mapped files
	Assert( !cb->data );
	Assert(cb->fp != NULL);

	int size = elsize*nelem;

	Assert(nelem > 0);
	Assert(elsize > 0);
	Assert(size > 0);

	if ( (cb->raw_position+size) > cb->size ) {
		size = cb->size - cb->raw_position;
		if ( size < 1 ) {
			return 0;
		}
		//mprintf(( "CFILE: EOF encountered in file\n" ));
	}

	int bytes_read = fread( buf, 1, size, cb->fp );
	if ( bytes_read > 0 )	{
		cb->raw_position += bytes_read;
	}		

	#if defined(CHECK_POSITION) && !defined(NDEBUG)
		int tmp_offset = ftell(cb->fp) - cb->lib_offset;
		Assert(tmp_offset==cb->raw_position);
	#endif

	return bytes_read / elsize;

}

