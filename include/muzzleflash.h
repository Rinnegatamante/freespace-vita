/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Weapon/MuzzleFlash.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * all sorts of cool stuff about ships
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     5/18/99 1:30p Dave
 * Added muzzle flash table stuff.
 * 
 * 3     3/19/99 9:52a Dave
 * Checkin to repair massive source safe crash. Also added support for
 * pof-style nebulae, and some new weapons code.
 * 
 * 2     1/08/99 2:08p Dave
 * Fixed software rendering for pofview. Super early support for AWACS and
 * beam weapons.
 * 
 * 
 * $NoKeywords: $
 */

#ifndef __FS2_MUZZLEFLASH_HEADER_FILE
#define __FS2_MUZZLEFLASH_HEADER_FILE

// ---------------------------------------------------------------------------------------------------------------------
// MUZZLE FLASH DEFINES/VARS
// 

// prototypes
struct object;

// muzzle flash types
#define MAX_MUZZLE_FLASH_TYPES				10
extern int Num_mflash_types;

// ---------------------------------------------------------------------------------------------------------------------
// MUZZLE FLASH FUNCTIONS
// 

// initialize muzzle flash stuff for the whole game
void mflash_game_init();

// initialize muzzle flash stuff for the level
void mflash_level_init();

// shutdown stuff for the level
void mflash_level_close();

// create a muzzle flash on the guy
void mflash_create(vector *gun_pos, vector *gun_dir, int mflash_type);

// process muzzle flash stuff
void mflash_process_all();

// render all muzzle flashes
void mflash_render_all();

// lookup type by name
int mflash_lookup(char *name);

#endif

