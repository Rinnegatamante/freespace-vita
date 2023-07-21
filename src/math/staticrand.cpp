/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Math/StaticRand.cpp $
 * $Revision: 121 $
 * $Date: 2002-06-17 08:33:11 +0200 (Mon, 17 Jun 2002) $
 * $Author: relnev $
 *
 * static random functions.  Return "random" number based on integer inut
 *
 * $Log$
 * Revision 1.3  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.2  2002/06/09 04:41:22  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 4     3/17/98 12:16a Allender
 * asteroids in multiplayer -- minor problems with position being correct
 * 
 * 3     12/30/97 5:46p Lawrance
 * Rename rnd() to rand_alt().
 * 
 * 2     12/30/97 4:27p Lawrance
 * Add new rnd() function that doesn't affect rand() sequence.
 * 
 * 1     8/08/97 3:38p Allender
 */

#include "staticrand.h"
#include "vecmat.h"

int Semirand_inited = 0;
int Semirand[SEMIRAND_MAX];

//	Initialize Semirand array.
void init_semirand()
{
	int	i;

	Semirand_inited = 1;

	for (i=0; i<SEMIRAND_MAX; i++)
		Semirand[i] = (myrand() << 15) + myrand();
}


//	Return a fairly random 32 bit value given a reasonably small number.
int static_rand(int num)
{
	int	a, b, c;

	if (!Semirand_inited)
		init_semirand();

	a = num & (SEMIRAND_MAX - 1);
	b = (num >> SEMIRAND_MAX_LOG) & (SEMIRAND_MAX - 1);
	c = (num >> (2 * SEMIRAND_MAX_LOG)) & (SEMIRAND_MAX - 1);

	return Semirand[a] ^ Semirand[b] ^ Semirand[c];
}

//	Return a random value in 0.0f .. 1.0f- (ie, it will never return 1.0f).
float static_randf(int num)
{
	int	a;

	a = static_rand(num);

	return (a & 0xffff) / 65536.0f;
}

float static_randf_range(int num, float min, float max)
{
	float	rval;
	
	rval = static_randf(num);
	rval = rval * (max - min) + min;

	return rval;
}


void static_randvec(int num, vector *vp)
{
	vp->xyz.x = static_randf(num) - 0.5f;
	vp->xyz.y = static_randf(num+1) - 0.5f;
	vp->xyz.z = static_randf(num+2) - 0.5f;

	vm_vec_normalize_quick(vp);
}

/////////////////////////////////////////////////////////////////////
// Alternate random number generator, that doesn't affect rand() sequence
/////////////////////////////////////////////////////////////////////
#define RND_MASK	0x6000
#define RND_MAX	0x7fff
int Rnd_seed = 1;

// Seed the random number generator.  Doesn't have to be called.
void srand_alt(int seed)
{
	Rnd_seed = seed;
}

// Get a random integer between 1 and RND_MAX
int rand_alt()
{
	static int x=Rnd_seed;
	int old_x;
	old_x = x;
	x >>= 1;
	if ( old_x & 1 ) {
		x ^= RND_MASK;
	}
	return x;
}

// Get a random float between 0 and 1.0
float frand_alt()
{
	int r = rand_alt();
	return i2fl(r)/RND_MAX;
}

