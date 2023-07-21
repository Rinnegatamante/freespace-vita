/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Hud/HUDObserver.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * $NoKeywords: $
 *
 */

#include "hud.h"
#include "hudobserver.h"
#include "freespace.h"
#include "multi.h"
#include "font.h"
#include "missiongoals.h"
#include "3d.h"
#include "linklist.h"
#include "debris.h"
#include "hudtargetbox.h"
#include "sound.h"
#include "gamesnd.h"
#include "radar.h"
#include "hudconfig.h"
#include "alphacolors.h"

// use these to redirect Player_ship and Player_ai when switching into ai mode
ship Hud_obs_ship;
ai_info Hud_obs_ai;


// initialize observer hud stuff
void hud_observer_init(ship *shipp,ai_info *aip)
{
	// setup the pseduo ship and ai
	memcpy(&Hud_obs_ship,shipp,sizeof(ship));
	memcpy(&Hud_obs_ai,aip,sizeof(ai_info));

	HUD_config.is_observer = 1;
	HUD_config.show_flags = HUD_observer_default_flags;
	HUD_config.show_flags2 = HUD_observer_default_flags2;

	HUD_config.popup_flags = 0x0;
	HUD_config.popup_flags2 = 0x0;

	// shutdown any playing static animations
	hud_targetbox_static_init();
}

void hud_obs_render_player(int loc,net_player *pl)
{
}

void hud_obs_render_players_all()
{
	int idx,count;

	// render kills and stats information for all players
	count = 0;
	for(idx=0;idx<MAX_PLAYERS;idx++){
		if(MULTI_CONNECTED(Net_players[idx]) && !MULTI_STANDALONE(Net_players[idx]) && !MULTI_PERM_OBSERVER(Net_players[idx]) ){
			hud_obs_render_player(count,&Net_players[idx]);
		}
	}
}

void hud_render_observer()
{
	Assert((Game_mode & GM_MULTIPLAYER) && (Net_player->flags & NETINFO_FLAG_OBSERVER));

	// render individual player text
	hud_obs_render_players_all();
}

