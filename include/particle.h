/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Particle/Particle.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Includes for particle system
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 7     7/21/99 8:10p Dave
 * First run of supernova effect.
 * 
 * 6     1/29/99 12:47a Dave
 * Put in sounds for beam weapon. A bunch of interface screens (tech
 * database stuff).
 * 
 * 5     1/27/99 9:56a Dave
 * Temporary checkin of beam weapons for Dan to make cool sounds.
 * 
 * 4     1/24/99 11:37p Dave
 * First full rev of beam weapons. Very customizable. Removed some bogus
 * Int3()'s in low level net code.
 * 
 * 3     1/21/99 2:06p Dave
 * Final checkin for multiplayer testing.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 6     5/13/98 3:25p John
 * Added code to make explosion impacts not get removed by other
 * particles.
 * 
 * 5     5/11/98 10:06a John
 * Added new particle for Adam
 * 
 * 4     4/30/98 11:31a Andsager
 * Added particles to big ship explosions.  Modified particle_emit() to
 * take optional range to increase range at which pariticles are created.
 * 
 * 3     1/29/98 11:48a John
 * Added new counter measure rendering as model code.   Made weapons be
 * able to have impact explosion.
 * 
 * 2     1/02/98 5:04p John
 * Several explosion related changes.  Made fireballs not be used as
 * ani's.  Made ship spark system expell particles.  Took away impact
 * explosion for weapon hitting ship... this needs to get added to weapon
 * info and makes shield hit more obvious.  Only make sparks when hit
 * hull, not shields.
 * 
 * 1     12/23/97 8:26a John
 *
 * $NoKeywords: $
 */

#ifndef _PARTICLE_H
#define _PARTICLE_H

//============================================================================
//==================== PARTICLE SYSTEM GAME SEQUENCING CODE ==================
//============================================================================

// Resets particle system.  Call between levels.
void particle_init();

// Moves the particles for each frame
void particle_move_all(float frametime);

// Renders all the particles
void particle_render_all();

// kill all active particles
void particle_kill_all();


//============================================================================
//=============== LOW-LEVEL SINGLE PARTICLE CREATION CODE ====================
//============================================================================

// The different types of particles...
#define PARTICLE_DEBUG		0			// A red sphere, no optional data required
#define PARTICLE_BITMAP		1			// A bitmap, optional data is the bitmap number.  If bitmap is an animation,
												// lifetime is calculated by the number of frames and fps.
#define PARTICLE_FIRE		2			// The vclip used for explosions, optional means nothing
#define PARTICLE_SMOKE		3			// The vclip used for smoke, optional means nothing
#define PARTICLE_SMOKE2		4			// The vclip used for smoke, optional means nothing
#define PARTICLE_BITMAP_PERSISTENT		5		// A bitmap, optional data is the bitmap number.  If bitmap is an animation,
															// lifetime is calculated by the number of frames and fps.

// particle creation stuff
typedef struct particle_info {
	// old-style particle info
	vector pos;
	vector vel;
	float lifetime;
	float rad;
	int type;
	uint optional_data;	

	// new-style particle info
	float tracer_length;
	short attached_objnum;			// if these are set, the pos is relative to the pos of the origin of the attached object
	int	attached_sig;				// to make sure the object hasn't changed or died. velocity is ignored in this case
	ubyte	reverse;						// play any animations in reverse
} particle_info;

// Creates a single particle. See the PARTICLE_?? defines for types.
void particle_create( particle_info *pinfo );
void particle_create( vector *pos, vector *vel, float lifetime, float rad, int type, uint optional_data = 0 );


//============================================================================
//============== HIGH-LEVEL PARTICLE SYSTEM CREATION CODE ====================
//============================================================================

// Use a structure rather than pass a ton of parameters to particle_emit
typedef struct particle_emitter {
	int		num_low;				// Lowest number of particles to create
	int		num_high;			// Highest number of particles to create
	vector	pos;					// Where the particles emit from
	vector	vel;					// Initial velocity of all the particles
	float		min_life;			// How long the particles live
	float		max_life;			// How long the particles live
	vector	normal;				// What normal the particle emit arond
	float		normal_variance;	//	How close they stick to that normal 0=good, 1=360 degree
	float		min_vel;				// How fast the slowest particle can move
	float		max_vel;				// How fast the fastest particle can move
	float		min_rad;				// Min radius
	float		max_rad;				// Max radius
} particle_emitter;

// Creates a bunch of particles. You pass a structure
// rather than a bunch of parameters.
void particle_emit( particle_emitter *pe, int type, uint optional_data, float range=1.0 );

#endif // _PARTICLE_H

