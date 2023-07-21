/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_pinfo.h $
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
 * 3     1/30/99 1:29a Dave
 * Fixed nebula thumbnail problem. Full support for 1024x768 choose pilot
 * screen.  Fixed beam weapon death messages.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 2     4/20/98 6:04p Dave
 * Implement multidata cache flushing and xferring mission files to
 * multidata. Make sure observers can't change hud config. Fix pilot image
 * viewing in popup. Put in game status field. Tweaked multi options. 
 * 
 * 1     3/05/98 8:20p Dave
 * 
 * $NoKeywords: $
 */

#ifndef _MULTI_PLAYER_INFO_HEADER_FILE
#define _MULTI_PLAYER_INFO_HEADER_FILE

// ---------------------------------------------------------------------------------------
// MULTI PLAYER INFO DEFINES/VARS
//

// prototypes
struct net_player;


// ---------------------------------------------------------------------------------------
// MULTI PLAYER INFO FUNCTIONS
//

// fire up the player info popup
void multi_pinfo_popup(net_player *np);

// is the pilot info popup currently active?
int multi_pinfo_popup_active();

// kill the currently active popup (if any)
void multi_pinfo_popup_kill();

// notify the popup that a player has left
void multi_pinfo_notify_drop(net_player *np);

#endif

