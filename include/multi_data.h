/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_data.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 * 
 * 
 * 4     12/14/98 12:13p Dave
 * Spiffed up xfer system a bit. Put in support for squad logo file xfer.
 * Need to test now.
 * 
 * 3     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 4     3/26/98 6:01p Dave
 * Put in file checksumming routine in cfile. Made pilot pic xferring more
 * robust. Cut header size of voice data packets in half. Put in
 * restricted game host query system.
 * 
 * 3     3/23/98 5:42p Dave
 * Put in automatic xfer of pilot pic files. Changed multi_xfer system so
 * that it can support multiplayer sends/received between client and
 * server simultaneously.
 * 
 * 2     3/21/98 7:14p Dave
 * Fixed up standalone player slot switching. Made training missions not
 * count towards player stats.
 * 
 * 1     2/19/98 6:21p Dave
 * Player data file xfer module.
 * 
 * $NoKeywords: $
 */

#ifndef _MULTI_PLAYER_DATA_HEADER_FILE
#define _MULTI_PLAYER_DATA_HEADER_FILE

// -------------------------------------------------------------------------
// MULTI DATA DEFINES/VARS
//


// -------------------------------------------------------------------------
// MULTI DATA FUNCTIONS
//

// reset the data xfer system
void multi_data_reset();

// handle a player join (clear out lists of files, etc)
void multi_data_handle_join(int player_index);

// handle a player drop (essentially the same as multi_data_handle_join)
void multi_data_handle_drop(int player_index);

// do all sync related data stuff (server-side only)
void multi_data_do();

// handle an incoming xfer request from the xfer system
void multi_data_handle_incoming(int handle);

// send all my files as necessary
void multi_data_send_my_junk();


#endif

