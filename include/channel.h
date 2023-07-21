/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Sound/channel.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Common types for the sound channels shared by the DirectSound modules
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     10/25/99 5:56p Jefff
 * increase num software channels to the number the users hardware can
 * handle.  not less than 16, tho.
 * 
 * 3     8/27/99 6:38p Alanl
 * crush the blasted repeating messages bug
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 6     12/05/97 5:19p Lawrance
 * re-do sound priorities to make more general and extensible
 * 
 * 5     7/17/97 9:32a John
 * made all directX header files name start with a v
 * 
 * 4     7/15/97 11:15a Lawrance
 * limit the max instances of simultaneous sound effects, implement
 * priorities to force critical sounds
 * 
 * 3     6/09/97 8:53a Lawrance
 * add #ifndef ... #endif to avoid multiple inclusion
 * 
 * 2     6/08/97 6:00p Lawrance
 * created to share channel defs between ds.cpp and ds3d.cpp
 *
 * $NoKeywords: $
 */


#include <windows.h>
#include "vdsound.h"

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

typedef struct channel
{
	int							sig;			// uniquely identifies the sound playing on the channel
	int							snd_id;		// identifies which kind of sound is playing
	LPDIRECTSOUNDBUFFER		pdsb;			// pointer to the secondary buffer that was duplicated 
	LPDIRECTSOUND3DBUFFER	pds3db;		// 3D interface, only used if sound buffer created with CTRL3D flag
	int							looping;		// flag to indicate that the sound is looping
	int							vol;			// in DirectSound units
	int							priority;	// implementation dependant priority
	bool							is_voice_msg;
	DWORD                   last_position;                    
} channel;


// #define	MAX_CHANNELS  16
extern	channel* Channels;    //[MAX_CHANNELS];

#endif /* __CHANNEL_H__ */

