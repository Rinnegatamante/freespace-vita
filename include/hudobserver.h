/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Hud/HUDObserver.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * $NoKeywords: $
 *
 */

#ifndef _HUD_OBSERVER_FILE
#define _HUD_OBSERVER_FILE

#include "hud.h"

// prototypes
struct ship;
struct ai_info;

// use these to redirect Player_ship and Player_ai when switching into ai mode
extern ship Hud_obs_ship;
extern ai_info Hud_obs_ai;

// initialize observer hud stuff
void hud_observer_init(ship *shipp,ai_info *aip);

// render any specific observer stuff
void hud_render_observer();


#endif

