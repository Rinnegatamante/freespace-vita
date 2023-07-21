/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Weapon/Corkscrew.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for managing corkscrew missiles
 *
 * $NoKeywords: $
 */


#ifndef __FREESPACE_CORKSCREW_H__
#define __FREESPACE_CORKSCREW_H__

#include "object.h"
#include "weapon.h"

extern int Corkscrew_num_missiles_fired;

void	cscrew_level_init();
void	cscrew_delete(int index);
int	cscrew_create(object *obj);

// pre process the corkscrew weapon by putting him in the "center" of his corkscrew
void  cscrew_process_pre(object *objp);

// post process the corkscrew weapon by putting him back to the right spot on his corkscrew
void	cscrew_process_post(object *objp);

// maybe fire another corkscrew-style missile
void	cscrew_maybe_fire_missile(int shipnum);

#endif /* __FREESPACE_CORKSCREW_H__ */

