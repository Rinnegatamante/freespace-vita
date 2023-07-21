/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_update.h $
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
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 2     7/09/98 4:51p Dave
 * First revision of PXO autoupdate check system.
 * 
 * 1     7/09/98 2:09p Dave
 * 
 *
 * $NoKeywords: $
 */

#ifndef _FREESPACE_AUTOUPDATE_THINGIE_HEADER_FILE
#define _FREESPACE_AUTOUPDATE_THINGIE_HEADER_FILE

// -------------------------------------------------------------------------------------------------------------------
// MULTI UPDATE DEFINES/VARS
//

// operation return codes
#define MULTI_UPDATE_CONTINUE							0				// continue to next screen
#define MULTI_UPDATE_SHUTTING_DOWN					1				// freespace is exiting to the launcher
#define MULTI_UPDATE_MAIN_MENU						2				// caller should move back to the main menu


// -------------------------------------------------------------------------------------------------------------------
// MULTI UPDATE FUNCTIONS
//

// check to see if the version of FS on this machine is not recent. run in a popup
// if the versions don't check out, bail to the launcher
// see MULTI_UPDATE_* return codes, above
int multi_update_gobaby();


#endif

