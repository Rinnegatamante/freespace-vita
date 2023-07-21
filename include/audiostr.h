/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Sound/AudioStr.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Routines to stream large WAV files from disk
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 9     5/24/98 4:42p Dan
 * AL: Fix several bugs related to pausing and enabling/disabling event
 * music
 * 
 * 8     3/31/98 4:50p Dan
 * AL: Clean up all audio streams if necessary in
 * event_music_level_close()
 * 
 * 7     12/10/97 10:04p Lawrance
 * modify what happens in Audio_stream constructor
 * 
 * 6     12/09/97 6:14p Lawrance
 * add -nomusic flag
 * 
 * 5     11/20/97 1:06a Lawrance
 * Add Master_voice_volume, make voices play back at correctly scaled
 * volumes
 * 
 * 4     10/03/97 8:24a Lawrance
 * When unpausing, be sure to retain looping status
 * 
 * 3     9/18/97 10:31p Lawrance
 * add functions to pause and unpause all audio streams
 * 
 * 2     6/04/97 1:19p Lawrance
 * added function to check if system is initialized
 * 
 * 1     4/28/97 4:45p John
 * Initial version of ripping sound & movie out of OsAPI.
 * 
 * 8     4/14/97 1:52p Lawrance
 * making transitions happen on measure boundries
 * 
 * 7     4/09/97 11:14a Lawrance
 * working on event music transitions
 * 
 * 6     4/07/97 3:15p Lawrance
 * allowing event music to pause
 * 
 * 5     4/03/97 4:27p Lawrance
 * expanding functionality to support event driven music
 * 
 * 4     4/01/97 1:31p Lawrance
 * make music fade quickly out when stopping.  Delay onset of new music to
 * allow old music to fade out.
 * 
 * 3     3/31/97 5:45p Lawrance
 * supporting changes to allow multiple streamed audio files
 * 
 * 2     3/31/97 3:56p Lawrance
 * decompress ADPCM->PCM for streaming sounds working
 * 
 * 1     1/22/97 10:43a John
 *
 * $NoKeywords: $
 */

#ifndef _AUDIOSTR_H
#define _AUDIOSTR_H

// type of audio stream
#define ASF_SOUNDFX			0
#define ASF_EVENTMUSIC		1
#define ASF_VOICE				2
#define ASF_NONE				3		// used to catch errors


// Initializes the audio streaming library.  Called
// automatically when the sound stuff is inited.
void audiostream_init();

// Closes down the audio streaming library
void audiostream_close();

// Opens a wave file but doesn't play it.
int audiostream_open( const char * filename, int type );

// Closes the opened wave file.  This doesn't have to be
// called between songs, because when you open the next
// song, it will call this internally.
void audiostream_close_file(int i, int fade = 1);

void audiostream_close_all(int fade);

// Plays the currently opened wave file
void audiostream_play(int i, float volume = -1.0f, int looping = 1);

// See if a particular stream is playing
int audiostream_is_playing(int i);

// Stops the currently opened wave file
void audiostream_stop(int i, int rewind = 1, int paused = 0);

// set the volume for every audio stream of a particular type
void audiostream_set_volume_all(float volume, int type);

// set the volume for a particular audio stream
void audiostream_set_volume(int i, float volume);

// see if a particular stream is paused
int audiostream_is_paused(int i);

// set the number of bytes that the sound should cutoff after
void audiostream_set_byte_cutoff(int i, unsigned int cutoff);

// return the number of bytes streamed to the Direct Sound buffer so far
unsigned int audiostream_get_bytes_committed(int i);

// check if the streaming has read all the bytes from disk yet
int audiostream_done_reading(int i);

// return if audiostream has initialized ok
int audiostream_is_inited();

void audiostream_pause(int i);	// pause a particular stream
void audiostream_pause_all();	// pause all audio streams											

void audiostream_unpause(int i);	// unpause a particular stream
void audiostream_unpause_all();	// unpause all audio streams

#endif // _AUDIOSTR_H

