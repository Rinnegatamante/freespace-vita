/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Sound/Sound.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * <insert description of file here>
 *
 * $Log$
 * Revision 1.4  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.3  2002/05/31 07:24:28  relnev
 * warning
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 6     9/08/99 3:22p Dave
 * Updated builtin mission list.
 * 
 * 5     8/27/99 6:38p Alanl
 * crush the blasted repeating messages bug
 * 
 * 4     6/18/99 5:16p Dave
 * Added real beam weapon lighting. Fixed beam weapon sounds. Added MOTD
 * dialog to PXO screen.
 * 
 * 3     5/23/99 8:11p Alanl
 * Added support for EAX
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 30    5/12/98 5:39p Lawrance
 * Increase MAX_SOUNDS to 175.. to account for missions with lots of voice
 * 
 * 29    5/12/98 2:43p Lawrance
 * Make snd_time_remaining() work for arbitrary format
 * 
 * 28    4/13/98 5:04p Lawrance
 * Write functions to determine how many milliseconds are left in a sound
 * 
 * 27    3/29/98 12:56a Lawrance
 * preload the warp in and explosions sounds before a mission.
 * 
 * 26    3/25/98 6:10p Lawrance
 * Work on DirectSound3D
 * 
 * 25    3/23/98 10:34a Lawrance
 * fix parameter in snd_size()
 * 
 * 24    3/23/98 10:32a Lawrance
 * Add functions for extracting raw sound data
 * 
 * 23    3/21/98 3:34p Lawrance
 * Allow 3d sounds to have their ranges modified dynamically
 * 
 * 22    3/19/98 5:36p Lawrance
 * Add some sound debug functions to see how many sounds are playing, and
 * to start/stop random looping sounds.
 * 
 * 21    3/17/98 5:55p Lawrance
 * Support object-linked sounds for asteroids.
 * 
 * 20    2/20/98 8:32p Lawrance
 * Add radius parm to sound_play_3d()
 * 
 * 19    1/07/98 11:08a Lawrance
 * pass priority to snd_play_raw()
 * 
 * 18    12/05/97 5:19p Lawrance
 * re-do sound priorities to make more general and extensible
 * 
 * 17    11/20/97 5:36p Dave
 * Hooked in a bunch of main hall changes (including sound). Made it
 * possible to reposition (rewind/ffwd) 
 * sound buffer pointers. Fixed animation direction change framerate
 * problem.
 * 
 * 16    11/20/97 1:06a Lawrance
 * Add Master_voice_volume, make voices play back at correctly scaled
 * volumes
 * 
 * 15    10/14/97 11:33p Lawrance
 * get RSX implemented
 * 
 * 14    10/13/97 7:41p Lawrance
 * store duration of sound
 * 
 * 13    10/01/97 5:55p Lawrance
 * change call to snd_play_3d() to allow for arbitrary listening position
 * 
 * 12    7/15/97 11:15a Lawrance
 * limit the max instances of simultaneous sound effects, implement
 * priorities to force critical sounds
 * 
 * 11    6/12/97 5:16p Lawrance
 * allow snd_play_looping to take just one parm
 * 
 * 10    6/09/97 11:50p Lawrance
 * integrating DirectSound3D
 * 
 * 9     6/05/97 11:25a Lawrance
 * use sound signatures to ensure correct sound is loaded
 * 
 * 8     6/05/97 1:36a Lawrance
 * using a new interface to play sounds
 * 
 * 7     6/05/97 1:08a Lawrance
 * new sound play interface
 * 
 * 6     6/02/97 1:45p Lawrance
 * implementing hardware mixing
 * 
 * 5     5/29/97 3:32p Lawrance
 * added call to snd_do_frame()
 * 
 * 4     5/29/97 12:04p Lawrance
 * split off acm, direct sound, and direct sound 3d portions to separate
 * files
 * 
 * 3     5/26/97 10:25a Lawrance
 * extern ds_convert_volume() 
 * 
 * 2     5/06/97 9:36a Lawrance
 * added support for min and max distances for 3d sounds
 * 
 * 1     4/28/97 4:45p John
 * Initial version of ripping sound & movie out of OsAPI.
 * 
 * 18    4/24/97 4:14p Lawrance
 * make ds_initialized extern
 * 
 * 17    4/21/97 8:58a Lawrance
 * keep track of how much mem is used for sounds
 * 
 * 16    4/20/97 11:19a Lawrance
 * sndman_ interface obsolete.  Using snd_ functions to load, play, and
 * manage static sound fx
 * 
 * 15    4/18/97 2:54p Lawrance
 * sounds now have a default volume, when playing, pass a scaling factor
 * not the actual volume
 * 
 * 14    4/09/97 4:34p Lawrance
 * allow looped sounds to be cut off after they complete the full sample
 * duration
 * 
 * 13    4/03/97 4:27p Lawrance
 * expanding functionality to support event driven music
 * 
 * 12    3/25/97 10:49a Lawrance
 * ADPCM -> PCM conversion working
 * 
 * 11    3/24/97 5:50p Lawrance
 * added code to convert from ADPCM -> PCM using Audio Compression Manager
 * 
 * 10    3/19/97 11:48a Lawrance
 * make each sound effect have a DirectSound secondary buffer
 * 
 * 9     3/10/97 4:16p Allender
 * new messaging system.  builtin5 fixed to support it.  made new sound
 * function to determine if sound is still playing.
 * 
 * 8     2/25/97 11:13a Lawrance
 * added Master_sound_volume to scale volume passed to snd_play()
 * 
 * 7     1/10/97 10:44a John
 * added functions to dynamically set volume/pan
 * 
 * 6     12/03/96 3:50p John
 * Added some initial support for looping sounds.
 * 
 * 5     11/05/96 11:02a Mike
 * Add sound toggle to Delete-D menu.
 * Make some global variables look global - ie, capitalize them.
 * Fix bug in collision code to make ship:ship collision detection work.
 * Fix setting of parent_obj in obj_create().
 * 
 * 4     11/04/96 3:16p John
 * First rev of working sound.
 * 
 * 3     11/04/96 10:37a John
 * Closed the DirectSound object on program termination.
 * 
 * 2     10/30/96 5:27p John
 * some hacked in sound stuff so i can experiment with directsound.
 * 
 * 1     10/28/96 11:36a John
 *
 * $NoKeywords: $
 */

#ifndef __SOUND_H__
#define __SOUND_H__

#define MAX_SOUNDS	256

// Used for keeping track which low-level sound library is being used
#define SOUND_LIB_DIRECTSOUND		0
#define SOUND_LIB_RSX				1

#define GAME_SND_USE_DS3D			(1<<1)
#define GAME_SND_VOICE				(1<<2)

// Priorities that can be passed to snd_play() functions to limit how many concurrent sounds of a 
// given type are played.
#define SND_PRIORITY_MUST_PLAY				0
#define SND_PRIORITY_SINGLE_INSTANCE		1
#define SND_PRIORITY_DOUBLE_INSTANCE		2
#define SND_PRIORITY_TRIPLE_INSTANCE		3

typedef struct game_snd
{
	int	sig;						// number of sound in sounds.tbl (not used)
	char	filename[MAX_FILENAME_LEN];
	float	default_volume;		// range: 0.0 -> 1.0
	int	min, max;				// min: distance at which sound will stop getting louder  max: distance at which sound is inaudible
	int	preload;					// preload sound (ie read from disk before mission starts)
	int	id;						// index into Sounds[], where sound data is stored
	int	id_sig;					// signature of Sounds[] element
	int	flags;
} game_snd;

typedef struct sound_env
{
	unsigned long id;
	float volume;
	float damping;
	float decay;
} sound_env;

extern int		Sound_enabled;
extern float	Master_sound_volume;		// 0 -> 1.0
extern float	Master_voice_volume;		// 0 -> 1.0
extern int		Snd_sram;					// System memory consumed by sound data	
extern int		Snd_hram;					// Soundcard memory consumed by sound data

//int	snd_load( char *filename, int hardware=0, int three_d=0, int *sig=NULL );
int	snd_load( game_snd *gs, int allow_hardware_load = 0);

int	snd_unload( int sndnum );
void	snd_unload_all();

// Plays a sound with volume between 0 and 1.0, where 0 is the
// inaudible and 1.0 is the loudest sound in the game.
// Pan goes from -1.0 all the way left to 0.0 in center to 1.0 all the way right.
int snd_play( game_snd *gs, float pan=0.0f, float vol_scale=1.0f, int priority = SND_PRIORITY_SINGLE_INSTANCE, bool voice_message = false );

// Play a sound directly from index returned from snd_load().  Bypasses
// the sound management process of using game_snd.
int snd_play_raw( int soundnum, float pan, float vol_scale=1.0f, int priority = SND_PRIORITY_MUST_PLAY );

// Plays a sound with volume between 0 and 1.0, where 0 is the
// inaudible and 1.0 is the loudest sound in the game.  It scales
// the pan and volume relative to the current viewer's location.
int snd_play_3d(game_snd *gs, vector *source_pos, vector *listen_pos, float radius=0.0f, vector *vel = NULL, int looping = 0, float vol_scale=1.0f, int priority = SND_PRIORITY_SINGLE_INSTANCE, vector *sound_fvec = NULL, float range_factor = 1.0f, int force = 0 );

// update the given 3d sound with a new position
void snd_update_3d_pos(int soudnnum, game_snd *gs, vector *new_pos);

// Use these for looping sounds.
// Returns the handle of the sound. -1 if failed.
// If startloop or stoploop are not -1, then then are used.
int	snd_play_looping( game_snd *gs, float pan=0.0f, int start_loop=-1, int stop_loop=-1, float vol_scale=1.0f, int priority = SND_PRIORITY_MUST_PLAY, int force = 0 );

void	snd_stop( int snd_handle );

// Sets the volume of a sound that is already playing.
// The volume is between 0 and 1.0, where 0 is the
// inaudible and 1.0 is the loudest sound in the game.
void snd_set_volume( int snd_handle, float volume );

// Sets the panning location of a sound that is already playing.
// Pan goes from -1.0 all the way left to 0.0 in center to 1.0 all the way right.
void snd_set_pan( int snd_handle, float pan );

// Sets the pitch (frequency) of a sound that is already playing
// Valid values for pitch are between 100 and 100000
void	snd_set_pitch( int snd_handle, int pitch );
int	snd_get_pitch( int snd_handle );

// Stops all sounds from playing, even looping ones.
void	snd_stop_all();

// determines if the sound handle is still palying
int	snd_is_playing( int snd_handle );

// change the looping status of a sound that is playing
void	snd_chg_loop_status(int snd_handle, int loop);

// return the time in ms for the duration of the sound
int snd_get_duration(int snd_id);

// get a 3D vol and pan for a particular sound
int	snd_get_3d_vol_and_pan(game_snd *gs, vector *pos, float* vol, float *pan, float radius=0.0f);

int	snd_init(int use_a3d, int use_eax);
void	snd_close();

// Return 1 or 0 to show that sound system is inited ok
int	snd_is_inited();

// Returns a pointer to the direct sound object
uint	sound_get_ds();

void	snd_update_listener(vector *pos, vector *vel, matrix *orient);

void 	snd_use_lib(int lib_id);

int snd_num_playing();

int snd_get_data(int handle, char *data);
int snd_size(int handle, int *size);
void snd_do_frame();

// repositioning of the sound buffer pointer
void snd_rewind(int snd_handle, game_snd *sg, float seconds);					// rewind N seconds from the current position
void snd_ffwd(int snd_handle, game_snd *sg, float seconds);						// fast forward N seconds from the current position
void snd_set_pos(int snd_handle, game_snd *sg, float val,int as_pct);		// set the position val as either a percentage (if as_pct) or as a # of seconds into the sound

void snd_get_format(int handle, int *bits_per_sample, int *frequency);
int snd_time_remaining(int handle, int bits_per_sample=8, int frequency=11025);

// sound environment

enum
{
    SND_ENV_GENERIC,
    SND_ENV_PADDEDCELL,
    SND_ENV_ROOM,
    SND_ENV_BATHROOM,
    SND_ENV_LIVINGROOM,
    SND_ENV_STONEROOM,
    SND_ENV_AUDITORIUM,
    SND_ENV_CONCERTHALL,
    SND_ENV_CAVE,
    SND_ENV_ARENA,
    SND_ENV_HANGAR,
    SND_ENV_CARPETEDHALLWAY,
    SND_ENV_HALLWAY,
    SND_ENV_STONECORRIDOR,
    SND_ENV_ALLEY,
    SND_ENV_FOREST,
    SND_ENV_CITY,
    SND_ENV_MOUNTAINS,
    SND_ENV_QUARRY,
    SND_ENV_PLAIN,
    SND_ENV_PARKINGLOT,
    SND_ENV_SEWERPIPE,
    SND_ENV_UNDERWATER,
    SND_ENV_DRUGGED,
    SND_ENV_DIZZY,
    SND_ENV_PSYCHOTIC
};

int sound_env_set(sound_env *se);
int sound_env_get(sound_env *se);
int sound_env_disable();
int sound_env_supported();

#endif

