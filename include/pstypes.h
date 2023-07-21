/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/GlobalIncs/PsTypes.h $
 * $Revision: 309 $
 * $Date: 2010-02-08 09:09:23 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Header file containg global typedefs, constants and macros
 *
 * $Log$
 * Revision 1.12  2005/10/01 21:38:32  taylor
 * some reorg of includes to not requires the same headers in many different files
 * handle FS1 specific language changes
 * some OS X changes for paths and to handle socklen_t better
 *
 * Revision 1.11  2004/12/15 04:10:45  taylor
 * outwnd_unix.cpp from fs2_open for logging to file in debug mode
 * fixes for default function values
 * always use vm_* functions for sanity sake
 * make cfilearchiver 64-bit compatible
 * fix crash on exit from double free()
 * fix crash on startup from extra long GL extension string in debug
 *
 * Revision 1.10  2004/07/04 11:31:43  taylor
 * amd64 support, compiler warning fixes, don't use software rendering
 *
 * Revision 1.9  2004/06/11 21:39:44  taylor
 * x86 compile fixes for OSX patch
 *
 * Revision 1.8  2004/06/11 00:15:06  tigital
 * byte-swapping changes for bigendian systems
 *
 * Revision 1.7  2002/07/13 19:47:02  theoddone33
 * Fix some more warnings
 *
 * Change demo building, edit Makefile if you want the demo.
 *
 * Revision 1.6  2002/06/19 04:52:45  relnev
 * MacOS X updates (Ryan)
 *
 * Revision 1.5  2002/06/17 06:33:08  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.4  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.3  2002/06/01 07:12:32  relnev
 * a few NDEBUG updates.
 *
 * removed a few warnings.
 *
 * Revision 1.2  2002/05/28 06:28:20  theoddone33
 * Filesystem mods, actually reads some data files now
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 34    6/16/00 3:16p Jefff
 * sim of the year dvd version changes, a few german soty localization
 * fixes
 * 
 * 33    10/05/99 2:29p Danw
 * 
 * 32    10/01/99 9:10a Daveb
 * V 1.1 PATCH
 * 
 * 31    9/13/99 12:22a Dave
 * Minor build update.
 * 
 * 30    8/28/99 4:54p Dave
 * Fixed directives display for multiplayer clients for wings with
 * multiple waves. Fixed hud threat indicator rendering color.
 * 
 * 29    8/09/99 4:18p Andsager
 * Make french and german defines, needed specifically to enable language
 * under launcher misc. tab
 * 
 * 28    7/20/99 1:49p Dave
 * Peter Drake build. Fixed some release build warnings.
 * 
 * 27    7/18/99 5:19p Dave
 * Jump node icon. Fixed debris fogging. Framerate warning stuff.
 * 
 * 26    7/15/99 9:21a Andsager
 * FS2_DEMO check in
 * 
 * 25    7/09/99 9:51a Dave
 * Added thick polyline code.
 * 
 * 24    6/29/99 10:35a Dave
 * Interface polygon bitmaps! Whee!
 * 
 * 23    6/14/99 10:45a Dave
 * Made beam weapons specify accuracy by skill level in the weapons.tbl
 * 
 * 22    6/03/99 10:15p Dave
 * Put in temporary main hall screen.
 * 
 * 21    5/09/99 8:57p Dave
 * Final E3 build preparations.
 * 
 * 20    4/25/99 7:43p Dave
 * Misc small bug fixes. Made sun draw properly.
 * 
 * 19    4/25/99 3:03p Dave
 * Removed E3_BUILD from pstypes
 * 
 * 18    4/25/99 3:02p Dave
 * Build defines for the E3 build.
 * 
 * 17    4/15/99 1:29p Dave
 * Remove multiplayer beta build define.
 * 
 * 16    4/15/99 1:24p Dave
 * Final Beta 1 checkin.
 * 
 * 15    4/14/99 5:28p Dave
 * Minor bug fixes.
 * 
 * 14    4/12/99 2:22p Dave
 * More checks for dogfight stats.
 * 
 * 13    4/09/99 2:21p Dave
 * Multiplayer beta stuff. CD checking.
 * 
 * 12    2/25/99 4:19p Dave
 * Added multiplayer_beta defines. Added cd_check define. Fixed a few
 * release build warnings. Added more data to the squad war request and
 * response packets.
 * 
 * 11    2/23/99 2:29p Dave
 * First run of oldschool dogfight mode. 
 * 
 * 10    2/07/99 8:51p Andsager
 * Add inner bound to asteroid field.  Inner bound tries to stay astroid
 * free.  Wrap when within and don't throw at ships inside.
 * 
 * 9     1/15/99 11:29a Neilk
 * Fixed D3D screen/texture pixel formatting problem. 
 * 
 * 8     1/08/99 2:08p Dave
 * Fixed software rendering for pofview. Super early support for AWACS and
 * beam weapons.
 * 
 * 7     1/06/99 2:24p Dave
 * Stubs and release build fixes.
 * 
 * 6     11/30/98 1:07p Dave
 * 16 bit conversion, first run.
 * 
 * 5     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 4     10/13/98 9:28a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 3     10/07/98 11:28a Dave
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 75    8/20/98 5:30p Dave
 * Put in handy multiplayer logfile system. Now need to put in useful
 * applications of it all over the code.
 * 
 * 74    6/17/98 11:01a Lawrance
 * set correct offset for English special font chars
 * 
 * 73    6/12/98 4:52p Hoffoss
 * Added support for special characters in in forgeign languages.
 * 
 * 72    6/09/98 6:49p Lawrance
 * Comment out UK_BUILD
 * 
 * 71    6/09/98 5:15p Lawrance
 * French/German localization
 * 
 * 70    6/09/98 12:12p Hoffoss
 * Added XSTR localization code.
 * 
 * 69    6/05/98 9:49a Lawrance
 * OEM changes
 * 
 * 68    5/22/98 3:13p Allender
 * no Int3()'s and Asserts
 * 
 * 67    5/20/98 12:59p John
 * Turned optimizations on for debug builds.   Also turning on automatic
 * function inlining.  Turned off the unreachable code warning.
 * 
 * 66    5/04/98 1:49p Allender
 * make Int3's do nothing when InterplayQA is defined
 * 
 * 65    4/25/98 11:55p Lawrance
 * compile out Int3() and Assert() for release demo build
 * 
 *
 * $NoKeywords: $
 *
 */

#ifndef _PSTYPES_H
#define _PSTYPES_H


// Build defines.  Comment in/out for whatever build is necessary:
// #define OEM_BUILD						// enable for OEM builds
// #define MULTIPLAYER_BETA_BUILD				// enable for multiplayer beta build
// #define E3_BUILD							// enable for 3dfx E3 build						
// #define PRESS_TOUR_BUILD			// enable for press tour build
//#define FS2_DEMO					// enable demo build for FS2
// #define PD_BUILD						// fred documentation/evaluation build
//	#define FRENCH_BUILD				// build for French (obsolete)
// #define GERMAN_BUILD				// build for German (this is now used)
//#define RELEASE_REAL					// this means that it is an actual release candidate, not just an optimized/release build

// uncomment this #define for DVD version (makes popups say DVD instead of CD 2 or whatever): JCF 5/10/2000
// #define DVD_MESSAGE_HACK


#if defined(MULTIPLAYER_BETA_BUILD) || defined(E3_BUILD) || defined(RELEASE_REAL)
	#define GAME_CD_CHECK
#endif

// 4127 is constant conditional (assert)
// 4100 is unreferenced formal parameters,
// 4514 is unreferenced inline function removed, 
// 4201 is nameless struct extension used. (used by windows header files)
// 4410 illegal size for operand... ie... 	fxch st(1)
// 4611 is _setjmp warning.  Since we use setjmp alot, and we don't really use constructors or destructors, this warning doesn't really apply to us.
// 4725 is the pentium division bug warning, and I can't seem to get rid of it, even with this pragma.
//      JS: I figured out the disabling 4725 works, but not on the first function in the module.
//      So to disable this, I add in a stub function at the top of each module that does nothing.
// 4710 is inline function not expanded (who cares?)
// 4711 tells us an inline function was expanded (who cares?)
// 4702 unreachable code.  I care, but too many to deal with
// 4201 nonstandard extension used : nameless struct/union (happens a lot in Windows include headers)
// 4390 emptry control statement (triggered by nprintf and mprintf's inside of one-line if's, etc)
#ifndef PLAT_UNIX
#pragma warning(disable: 4127 4100 4514 4201 4410 4611 4725 4710 4711 4702 4201 4390)
#endif

#include <stdio.h>	// For NULL, etc
#include <stdlib.h>
#include <memory.h>

#ifndef __APPLE__
#include <malloc.h>
#endif

#include <string.h>

#ifdef PLAT_UNIX
#include "unix.h"
#endif

// value to represent an uninitialized state in any int or uint
#define UNINITIALIZED 0x7f8e6d9c

#if defined(DEMO) || defined(OEM_BUILD) // no change for FS2_DEMO
	#define MAX_PLAYERS	1
#else
	#define MAX_PLAYERS	12
#endif

#define MAX_TEAMS		3

#define USE_INLINE_ASM 1		// Define this to use inline assembly
#define STRUCT_CMP(a, b) memcmp((void *) &a, (void *) &b, sizeof(a))

#define LOCAL static			// make module local varilable static.

typedef __int64 longlong;
typedef long fix;
typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int fs_time_t;	// forced 32-bit version of time_t - **don't use this unless required**

// ptr_? is a value matching the size of a pointer on this specific platform
#if ( defined(__x86_64__) || defined(_WIN64) )
typedef __int64 ptr_s;
typedef unsigned __int64 ptr_u;
#else
typedef int ptr_s;
typedef unsigned int ptr_u;
#endif

#define HARDWARE_ONLY

//Stucture to store clipping codes in a word
typedef struct ccodes {
	ubyte vor,vand;	//or is low byte, and is high byte
} ccodes;

typedef struct vector {
	union {
		struct {
			float x,y,z;
		} xyz;
		float a1d[3];
	};
} vector;

// A vector referenced as an array
typedef struct vectora {
	float	xyz[3];
} vectora;

typedef struct vec2d {
	float i,j;
} vec2d;

// Used for some 2d primitives, like gr_poly
typedef struct vert2df {
	float x, y;
} vert2df;

typedef struct angles {
	float	p, b, h;
} angles_t;

typedef struct matrix {
	union {
		struct {
			vector	rvec, uvec, fvec;
		} v;
		float a2d[3][3];
		float a1d[9];
	};
} matrix;

typedef struct uv_pair {
	float u,v;
} uv_pair;

// Used to store rotated points for mines.
// Has flag to indicate if projected.
typedef struct vertex {
	float		x, y, z;				// world space position
	float		sx, sy, sw;			// screen space position (sw == 1/z)
	float		u, v;					// texture position
	ubyte		r, g, b, a;			// color.  Use b for darkening;
	ubyte		codes;				// what sides of view pyramid this point is on/off.  0 = Inside view pyramid.
	ubyte		flags;				// Projection flags.  Indicates whether it is projected or not or if projection overflowed.
	ubyte		pad[2];				// pad structure to be 4 byte aligned.
} vertex;

#define	BMP_AABITMAP		(1<<0)				// antialiased bitmap
#define	BMP_TEX_XPARENT		(1<<1)				// transparent texture
#define	BMP_TEX_NONDARK		(1<<2)				// nondarkening texture
#define	BMP_TEX_OTHER		(1<<3)				// so we can identify all "normal" textures

// any texture type
#define	BMP_TEX_ANY				( BMP_TEX_XPARENT | BMP_TEX_NONDARK | BMP_TEX_OTHER )

// max res == 1024x768. max texture size == 256
#define MAX_BMAP_SECTIONS_X				4
#define MAX_BMAP_SECTIONS_Y				3
#define MAX_BMAP_SECTION_SIZE				256
typedef struct bitmap_section_info {
	ushort sx[MAX_BMAP_SECTIONS_X];		// x offset of each section
	ushort sy[MAX_BMAP_SECTIONS_Y];		// y offset of each section

	ubyte num_x, num_y;						// number of x and y sections
} bitmap_section_info;

typedef struct bitmap {
	short	w, h;		// Width and height
	short	rowsize;	// What you need to add to go to next row
	ubyte	bpp;		// How many bits per pixel it is. (7,8,15,16,24,32)
	ubyte	flags;	// See the BMP_???? defines for values
	ptr_u	data;		// Pointer to data, or maybe offset into VRAM.
	ubyte *palette;	// If bpp==8, this is pointer to palette.   If the BMP_NO_PALETTE_MAP flag
							// is not set, this palette just points to the screen palette. (gr_palette)

	bitmap_section_info sections;
} bitmap;

//This are defined in MainWin.c
extern void _cdecl WinAssert(const char * text, const char *filename, int line);
extern void _cdecl Error( const char * filename, int line, const char * format, ... );
extern void _cdecl Warning( const char * filename, int line, const char * format, ... );

#include "outwnd.h"

// To debug printf do this:
// mprintf(( "Error opening %s\n", filename ));
#ifndef NDEBUG
#define mprintf(args) outwnd_printf2 args
#define nprintf(args) outwnd_printf args
#else
#define mprintf(args) 
#define nprintf(args) 
#endif

#define LOCATION __FILE__,__LINE__

// To flag an error, you can do this:
// Error( __FILE__, __LINE__, "Error opening %s", filename );
// or, 
// Error( LOCATION, "Error opening %s", filename );

#if defined(NDEBUG)
#define Assert(x) do {} while (0)
#define STUB_FUNCTION
#else
void gr_activate(int);
#define Assert(x) do { if (!(x)){ gr_activate(0); WinAssert(#x,__FILE__,__LINE__); gr_activate(1); } } while (0)
#define STUB_FUNCTION mprintf(("STUB: %s at %s, line %d, thread %d\n", __FUNCTION__, LOCATION, getpid()))
#endif

//#define Int3() _asm { int 3 }

#ifdef INTERPLAYQA
	// Interplay QA version of Int3
	#define Int3() do { } while (0) 

	// define to call from Warning function above since it calls Int3, so without this, we
	// get put into infinite dialog boxes
	#define AsmInt3() _asm { int 3 }

#else
	#if defined(NDEBUG)
		// No debug version of Int3
		#define Int3() do { } while (0) 
	#else
		void debug_int3();

		// Debug version of Int3
		#define Int3() debug_int3()
	#endif	// NDEBUG && DEMO
#endif	// INTERPLAYQA

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define PI					3.141592654f
#define PI2					(3.141592654f*2.0f)	// PI*2
#define ANG_TO_RAD(x)	((x)*PI/180)


extern int	Fred_running;  // Is Fred running, or FreeSpace?
extern int Pofview_running;
extern int Nebedit_running;


//======================================================================================
//======          D E B U G    C O N S O L E   S T U F F        ========================
//======================================================================================

// Here is a a sample command to toggle something that would
// be called by doing "toggle it" from the debug console command window/

/*
DCF(toggle_it,"description")
{
	if (Dc_command)	{
		This_var = !This_var;
	}

	if (Dc_help)	{
		dc_printf( "Usage: sample\nToggles This_var on/off.\n" );
	}

	if (Dc_status)	{
		dc_printf( "The status is %d.\n", This_var );
	}
*/

class debug_command {
	public:
	const char *name;
	const char *help;
	void (*func)();
	debug_command(const char *name, const char *help, void (*func)());	// constructor
};

#define DCF(function_name,help_text)			\
		void dcf_##function_name();	\
		debug_command dc_##function_name(#function_name,help_text,dcf_##function_name);	\
		void dcf_##function_name()		

// Starts the debug console
extern void debug_console( void (*func)() = NULL );

// The next three variables tell your function what to do.  It should
// only change something if the dc_command is set.   A minimal function
// needs to process the dc_command.   Usually, these will be called in
// these combinations:
// dc_command=true, dc_status=true  means process it and show status
// dc_help=true means show help only
// dc_status=true means show status only
// I would recommend doing this in each function:
// if (dc_command) { process command }
// if (dc_help) { print out help }
// if (dc_status) { print out status }
// with the last two being optional

extern int Dc_command;	// If this is set, then process the command
extern int Dc_help;		// If this is set, then print out the help text in the form, "usage: ... \nLong description\n" );
extern int Dc_status;		// If this is set, then print out the current status of the command.

void dc_get_arg(uint flags);		// Gets the next argument.   If it doesn't match the flags, this function will print an error and not return.
extern char *Dc_arg;		// The (lowercased) string value of the argument retrieved from dc_arg
extern char *Dc_arg_org;	// Dc_arg before it got converted to lowercase
extern uint Dc_arg_type;	// The type of dc_arg.
extern const char *Dc_command_line;		// The rest of the command line, from the end of the last processed arg on.
extern int Dc_arg_int;		// If Dc_arg_type & ARG_INT or ARG_HEX is set, then this is the value
extern float Dc_arg_float;	// If Dc_arg_type & ARG_FLOAT is set, then this is the value

// Outputs text to the console
void dc_printf( const char *format, ... );

// Each dc_arg_type can have one or more of these flags set.
// This is because some things can fit into two catagories.
// Like 1 can be an integer, a float, a string, or a true boolean
// value.
#define ARG_NONE		(1<<0)	// no argument
#define ARG_ANY		0xFFFFFFFF	// Anything.
#define ARG_STRING	(1<<1)	// any valid string
#define ARG_QUOTE		(1<<2)	// a quoted string
#define ARG_INT		(1<<3)	// a valid integer
#define ARG_FLOAT		(1<<4)	// a valid floating point number

// some specific commonly used predefined types. Can add up to (1<<31)
#define ARG_HEX		(1<<5)	// a valid hexadecimal integer. Note that ARG_INT will always be set also in this case.
#define ARG_TRUE		(1<<6)	// on, true, non-zero number
#define ARG_FALSE		(1<<7)	// off, false, zero
#define ARG_PLUS		(1<<8)	// Plus sign
#define ARG_MINUS		(1<<9)	// Minus sign
#define ARG_COMMA		(1<<10)	// a comma

// A shortcut for boolean only variables.
// Example:  
// DCF_BOOL( lighting, Show_lighting )
//
#define DCF_BOOL( function_name, bool_variable )	\
	void dcf_##function_name();	\
	debug_command dc_##function_name(#function_name,"Toggles "#bool_variable,dcf_##function_name );	\
	void dcf_##function_name()	{	\
	if ( Dc_command )	{	\
		dc_get_arg(ARG_TRUE|ARG_FALSE|ARG_NONE);		\
		if ( Dc_arg_type & ARG_TRUE )	bool_variable = 1;	\
		else if ( Dc_arg_type & ARG_FALSE ) bool_variable = 0;	\
		else if ( Dc_arg_type & ARG_NONE ) bool_variable ^= 1;	\
	}	\
	if ( Dc_help )	dc_printf( "Usage: %s [bool]\nSets %s to true or false.  If nothing passed, then toggles it.\n", #function_name, #bool_variable );	\
	if ( Dc_status )	dc_printf( "%s is %s\n", #function_name, (bool_variable?"TRUE":"FALSE") );	\
}


//======================================================================================
//======================================================================================
//======================================================================================



#include "fix.h"
#include "floating.h"

// Some constants for stuff
#define MAX_FILENAME_LEN	32			// Length for filenames, ie "title.pcx"
#define MAX_PATH_LEN			128		// Length for pathnames, ie "c:\bitmaps\title.pcx"

// contants and defined for byteswapping routines (useful for mac)

#define SWAPSHORT(x)	(							\
						((ubyte)x << 8) |					\
						(((ushort)x) >> 8)			\
						)
						
#define SWAPINT(x)		(							\
						(x << 24) |					\
						(((ulong)x) >> 24) |		\
						((x & 0x0000ff00) << 8) |	\
						((x & 0x00ff0000) >> 8)		\
						)
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
inline 
float SWAPFLOAT( float *x )
{
    #if ! defined( __MWERKS__ )
		//Usage:  void __stwbrx( unsigned int, unsigned int *address, int byteOffsetFromAddress );
		#define __stwbrx( value, base, index ) \
			 __asm__ ( "stwbrx %0, %1, %2" :  : "r" (value), "b%" (index), "r" (base) : "memory" )
	#endif

	union
	{
		int		i;
		float		f;
	}buf;
	
	//load the float into the integer unit
	//unsigned int	a = ((long*) x)[0];
        register int a = ((int*) x )[0];

	//store it to the transfer union, with byteswapping
	__stwbrx( a, 0, &buf.i );	

	//load it into the FPU and return it
	return buf.f;
}
#endif

#if SDL_BYTEORDER != SDL_BIG_ENDIAN
#define INTEL_INT(x)	x
#define INTEL_SHORT(x)	x
#define INTEL_FLOAT(x)	(*x)
#else
#define INTEL_INT(x)	SWAPINT(x)
#define INTEL_SHORT(x)	SWAPSHORT(x)
#define INTEL_FLOAT(x)	SWAPFLOAT(x)
#endif

#define TRUE	1
#define FALSE	0

#define MY_RAND_MAX		0x7fff
int myrand();



// Callback Loading function. 
// If you pass a function to this, that function will get called
// around 10x per second, so you can update the screen.
// Pass NULL to turn it off.
// Call this with the name of a function.  That function will
// then get called around 10x per second.  The callback function
// gets passed a 'count' which is how many times game_busy has
// been called since the callback was set.   It gets called
// one last time with count=-1 when you turn off the callback
// by calling game_busy_callback(NULL).   Game_busy_callback
// returns the current count, so you can tell how many times
// game_busy got called.
// If delta_step is above 0, then it will also make sure it 
// calls the callback each time count steps 'delta_step' even
// if 1/10th of a second hasn't elapsed.
extern int game_busy_callback( void (*callback)(int count), int delta_step = -1 );

// Call whenever loading to display cursor
extern void game_busy();


//=========================================================
// Functions to monitor performance
#ifndef NDEBUG

class monitor {
	public:
	const char	*name;			
	int	value;			// Value that gets cleared to 0 each frame.
	int	min, max, sum, cnt;		// Min & Max of value.  Sum is used to calculate average 
	monitor(const char *name);	// constructor
};

// Creates a monitor variable
#define MONITOR(function_name)				monitor mon_##function_name(#function_name)

// Increments a monitor variable
#define MONITOR_INC(function_name,inc)		do { mon_##function_name.value+=(inc); } while(0)

// Call this once per frame to update monitor file
void monitor_update();

#else

#define MONITOR(function_name)

#define MONITOR_INC(function_name,inc)		do { } while(0)

// Call this once per frame to update monitor file
#define monitor_update() do { } while(0)

#endif

#define NOX(s) s

const char *XSTR(const char *str, int index);

// Caps V between MN and MX.
template <class T> void CAP( T& v, T mn, T mx )
{
	if ( v < mn )	{
		v = mn;
	} else if ( v > mx )	{
		v = mx;
	}
}

// ========================================================
// stamp checksum stuff
// ========================================================

// here is the define for the stamp for this set of code
#define STAMP_STRING "\001\001\001\001\002\002\002\002Read the Foundation Novels from Asimov.  I liked them." 
#define STAMP_STRING_LENGTH	80
#define DEFAULT_CHECKSUM_STRING		"\001\001\001\001"
#define DEFAULT_TIME_STRING			"\002\002\002\002"

// macro to calculate the checksum for the stamp.  Put here so that we can use different methods
// for different applications.  Requires the local variable 'checksum' to be defined!!!
#define CALCULATE_STAMP_CHECKSUM() do {	\
		int i, found;	\
							\
		checksum = 0;	\
		for ( i = 0; i < (int)strlen(ptr); i++ ) {	\
			found = 0;	\
			checksum += ptr[i];	\
			if ( checksum & 0x01 )	\
				found = 1;	\
			checksum = checksum >> 1;	\
			if (found)	\
				checksum |= 0x80000000;	\
		}	\
		checksum |= 0x80000000;	\
	} while (0) ;

//=========================================================
// Memory management functions
//=========================================================

#ifndef NDEBUG
	// Debug versions

	// Returns 0 if not enough RAM.
	int vm_init(int min_heap_size);

	// Allocates some RAM.
	void *vm_malloc( int size, const char *filename=NULL, int line=-1 );

	// 
	char *vm_strdup( const char *ptr, const char *filename, int line );

	// Frees some RAM. 
	void vm_free( void *ptr, const char *filename=NULL, int line=-1 );

	// Frees all RAM.
	void vm_free_all();

	// Easy to use macros
	#define VM_MALLOC(size) vm_malloc((size),__FILE__,__LINE__)
	#define VM_FREE(ptr) vm_free((ptr),__FILE__,__LINE__)

	#define malloc(size) vm_malloc((size),__FILE__,__LINE__)
	#define free(ptr) vm_free((ptr),__FILE__,__LINE__)
	#define strdup(ptr) vm_strdup((ptr),__FILE__,__LINE__)
	
#else
	// Release versions

	// Returns 0 if not enough RAM.
	int vm_init(int min_heap_size);

	// Allocates some RAM.
	void *vm_malloc( int size );

	// 
	char *vm_strdup( const char *ptr );

	// Frees some RAM. 
	void vm_free( void *ptr );

	// Frees all RAM.
	void vm_free_all();

	// Easy to use macros
	#define VM_MALLOC(size) vm_malloc(size)
	#define VM_FREE(ptr) vm_free(ptr)

	#define malloc(size) vm_malloc(size)
	#define free(ptr) vm_free(ptr)
	#define strdup(ptr) vm_strdup(ptr)

#endif


#endif		// PS_TYPES_H

