/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Math/Floating.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Low-level floating point math macros and routines
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     4/07/99 6:22p Dave
 * Fred and Freespace support for multiple background bitmaps and suns.
 * Fixed link errors on all subprojects. Moved encrypt_init() to
 * cfile_init() and lcl_init(), since its safe to call twice.
 * 
 * 3     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 13    2/26/98 3:28p John
 * Changed all sqrt's to use fl_sqrt.  Took out isqrt function
 * 
 * 12    1/26/98 10:43p Mike
 * Make ships not all zoom away from an impending shockwave at the same
 * time.  Based on ai class and randomness
 * 
 * 11    1/17/98 3:32p Mike
 * Add rand_range(), returns random float in min..max.
 * 
 * 10    7/29/97 2:36p Hoffoss
 * Added header file required by _isnan().
 * 
 * 9     7/29/97 2:35p Hoffoss
 * Added a NaN check macro.
 * 
 * 8     2/17/97 5:18p John
 * Added a bunch of RCS headers to a bunch of old files that don't have
 * them.
 *
 * $NoKeywords: $
 */

#ifndef _FLOATING_H
#define _FLOATING_H

#include <math.h>
#include <float.h>

extern float fl_isqrt( float a );
extern float frand();
extern int rand_chance(float frametime, float chance = 1.0f);
float frand_range(float min, float max);

// determine if a floating point number is NaN (Not a Number)
#define fl_is_nan(fl) _isnan(fl)

// Handy macros to prevent type casting all over the place

#define fl_sqrt(fl) (float)sqrt((float)(fl))
#define fl_isqrt(fl) (1.0f/(float)sqrt((float)(fl)))
#define fl_abs(fl) (float)fabs((double)(fl))
#define i2fl(i) ((float)(i))
#define fl2i(fl) ((int)(fl))
#define flceil(fl) (int)ceil(fl)
#define flfloor(fl) (int)floor(fl)
#define f2fl(fx) ((float)(fx)/65536.0f)
#define fl2f(fl) (int)((fl)*65536.0f)

// convert a measurement in degrees to radians
#define fl_radian(fl)	((float)((fl * 3.14159f)/180.0f))

// convert a measurement in radians to degrees
#define fl_degrees(fl)	((float)((fl * 180.0f)/3.14159))

// use this instead of:
// for:  (int)floor(x+0.5f) use fl_round_2048(x)
//       (int)ceil(x-0.5f)  use fl_round_2048(x)
//       (int)floor(x-0.5f) use fl_round_2048(x-1.0f)
//       (int)floor(x)      use fl_round_2048(x-0.5f)
// for values in the range -2048 to 2048
// use this instead of:
// for:  (int)floor(x+0.5f) use fl_round_2048(x)
//       (int)ceil(x-0.5f)  use fl_round_2048(x)
//       (int)floor(x-0.5f) use fl_round_2048(x-1.0f)
//       (int)floor(x)      use fl_round_2048(x-0.5f)
// for values in the range -2048 to 2048

extern const float *p_fl_magic;

inline int fl_round_2048( float x )
{
	double tmp_quad;
	tmp_quad = x + *p_fl_magic;
	return *((int *)&tmp_quad);
}

/*
inline float fl_sqrt( float x)
{
	float retval;

	_asm fld x
	_asm fsqrt
	_asm fstp retval
	
	return retval;
}

float fl_isqrt( float x )
{
	float retval;

	_asm fld x
	_asm fsqrt
	_asm fstp retval
	
	return 1.0f / retval;
} 
*/



// rounds off a floating point number to a multiple of some number
extern float fl_roundoff(float x, int multiple);


#endif

