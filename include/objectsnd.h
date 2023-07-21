/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Object/ObjectSnd.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for managing object-linked persistant sounds
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     7/01/99 4:23p Dave
 * Full support for multiple linked ambient engine sounds. Added "big
 * damage" flag.
 * 
 * 3     7/01/99 11:44a Dave
 * Updated object sound system to allow multiple obj sounds per ship.
 * Added hit-by-beam sound. Added killed by beam sound.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 11    3/17/98 5:55p Lawrance
 * Support object-linked sounds for asteroids.
 * 
 * 10    9/03/97 5:02p Lawrance
 * add engine stuttering when a ship is dying
 * 
 * 9     7/14/97 12:04a Lawrance
 * make Obj_snd_enabled visible
 * 
 * 8     6/09/97 11:50p Lawrance
 * integrating DirectSound3D
 * 
 * 7     6/06/97 4:13p Lawrance
 * use an index instead of a pointer for object-linked sounds
 * 
 * 6     6/05/97 1:07a Lawrance
 * changes to support sound interface
 * 
 * 5     6/02/97 1:50p Lawrance
 * supporting integration with Direct3D
 * 
 * 4     5/09/97 4:33p Lawrance
 * doppler effects
 * 
 * 3     5/09/97 9:41a Lawrance
 * added #ifndef to avoid multiple inclusions
 * 
 * 2     5/08/97 4:30p Lawrance
 * split off object sound stuff into separate file
 *
 * $NoKeywords: $
 */

#ifndef __OBJECTSND_H__
#define __OBJECTSND_H__

#define	OS_USED	(1<<0)
#define	OS_DS3D	(1<<1)
#define  OS_MAIN	(1<<2)		// "main" sound. attentuation does not apply until outside the radius of the object

extern int Obj_snd_enabled;

void	obj_snd_level_init();
void	obj_snd_level_close();
void	obj_snd_do_frame();

// pos is the position of the sound source in the object's frame of reference.
// so, if the objp->pos was at the origin, the pos passed here would be the exact
// model coords of the location of the engine
// by passing vmd_zero_vector here, you get a sound centered directly on the object
// NOTE : if main is true, the attentuation factors don't apply if you're within the radius of the object
int	obj_snd_assign(int objnum, int sndnum, vector *pos, int main);

// if sndnum is not -1, deletes all instances of the given sound within the object
void	obj_snd_delete(int objnum, int sndnum = -1);

void	obj_snd_delete_all();
void	obj_snd_stop_all();
int	obj_snd_is_playing(int index);
int	obj_snd_return_instance(int index);

#endif

