/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Hud/HUDets.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file that supports code to manage and display the Energy Transfer System (ETS)
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 7     8/11/97 10:51a Lawrance
 * fix problem that was not setting correct weapon energy when
 * change_ship_type() was called
 * 
 * 6     4/03/97 5:29p Mike
 * 
 * 5     2/25/97 4:12p Lawrance
 * using frametime to calculate energy recharge
 * 
 * 4     1/01/97 7:34p Lawrance
 * added cheat (Del+W) which keeps weapon energy at max levels.
 * 
 * 3     12/24/96 4:31p Lawrance
 * refining energy transfer system
 * 
 * 2     12/22/96 3:41p Lawrance
 * ETS system working
 *
 * $NoKeywords: $
 */


#ifndef _HUD_ETS_H
#define _HUD_ETS_H

#include "object.h"

#define	ETS_RECHARGE_RATE	4.0f			//	Recharge this percent of total shields/second

extern float Energy_levels[];
extern int Weapon_energy_cheat;


enum SYSTEM_TYPE {WEAPONS, SHIELDS, ENGINES};

void update_ets(object* obj, float fl_frametime);
void ets_init_ship(object* obj);
void ai_manage_ets(object* obj);

void hud_init_ets();
void hud_show_ets();

void increase_recharge_rate(object* obj, SYSTEM_TYPE enum_value);
void decrease_recharge_rate(object* obj, SYSTEM_TYPE enum_value);
void set_default_recharge_rates(object* obj);

void transfer_energy_to_shields(object* obj);
void transfer_energy_to_weapons(object* obj);


#endif

