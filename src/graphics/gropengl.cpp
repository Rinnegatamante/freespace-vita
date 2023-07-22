/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Graphics/GrOpenGL.cpp $
 * $Revision: 314 $
 * $Date: 2010-02-09 12:11:53 +0100 (Tue, 09 Feb 2010) $
 * $Author: taylor $
 *
 * Code that uses the OpenGL graphics library
 *
 * $Log$
 * Revision 1.76  2005/10/01 21:44:43  taylor
 * slight speedup to font rendering by not doing something that should be global for each letter
 * little cleanup to better deal with FS2_Open related changes
 * don't deal with the extra texmem stuff if we don't have to, saves on temporary memory usage and speed
 *
 * Revision 1.75  2005/08/12 20:24:31  taylor
 * some OSX GCC4 fixin (LEgregius)
 *
 * Revision 1.74  2005/08/12 08:57:20  taylor
 * don't show hardware S-RAM value on HUD in debug
 * do show in use GL texture memory
 * have an actual fade effect for the credits screen artwork
 *
 * Revision 1.73  2005/04/02 18:58:08  taylor
 * attempt to fix garbage at end of GL extension string
 *
 * Revision 1.72  2005/03/30 01:20:12  taylor
 * Screenshot function filled, will output into ~/.freespace(2)/Data
 * Use glTexSubImage2D on reloaded texture slots, this is only used by ANIs
 *
 * Revision 1.71  2004/12/15 04:10:45  taylor
 * outwnd_unix.cpp from fs2_open for logging to file in debug mode
 * fixes for default function values
 * always use vm_* functions for sanity sake
 * make cfilearchiver 64-bit compatible
 * fix crash on exit from double free()
 * fix crash on startup from extra long GL extension string in debug
 *
 * Revision 1.70  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.69  2004/07/04 11:31:43  taylor
 * amd64 support, compiler warning fixes, don't use software rendering
 *
 * Revision 1.68  2004/06/11 01:01:07  tigital
 * added FSAA support: switched on in .ini
 *
 * Revision 1.67  2003/08/03 15:59:40  taylor
 * GL_RGB5_A1 as TexImage internal format; cleaner input grab; min window title; cleanup
 *
 * Revision 1.66  2003/06/22 12:52:34  taylor
 * more texture size fixin
 *
 * Revision 1.65  2003/06/19 11:52:47  taylor
 * fix texture size issue with lower detail settings
 *
 * Revision 1.64  2003/05/28 06:02:04  taylor
 * fix transparency in green weapon blobs
 *
 * Revision 1.63  2003/05/09 05:05:52  taylor
 * improve gr_(de)activate, mouse grab, default fullscreen
 *
 * Revision 1.62  2003/05/04 04:56:53  taylor
 * move SDL_Quit to os_deinit to fix fonttool segfault
 *
 * Revision 1.61  2003/02/02 21:13:27  relnev
 * minor updates (made functions static, tried to improve texture sizes)
 *
 * Revision 1.60  2003/02/02 20:20:03  relnev
 * add large texture support (Taylor Richards)
 *
 * Revision 1.59  2002/09/04 01:38:09  relnev
 * revert
 *
 * Revision 1.57  2002/08/31 01:39:13  theoddone33
 * Speed up the renderer a tad
 *
 * Revision 1.56  2002/08/01 05:03:11  relnev
 * minor change
 *
 * Revision 1.55  2002/08/01 04:55:45  relnev
 * experimenting with texture state
 *
 * Revision 1.54  2002/07/30 15:00:15  relnev
 * not use luminance alpha by default
 *
 * Revision 1.53  2002/06/22 17:08:16  relnev
 * more fixes to unused alpha code
 *
 * Revision 1.52  2002/06/21 23:59:14  relnev
 * moved a gr_opengl_set_state to after the gr_tcache_set
 *
 * Revision 1.51  2002/06/19 04:52:45  relnev
 * MacOS X updates (Ryan)
 *
 * Revision 1.50  2002/06/18 23:20:44  relnev
 * fixed _splitpath.
 *
 * adjusted line offsets.
 *
 * Revision 1.49  2002/06/17 23:11:39  relnev
 * enable sdl key repeating.
 *
 * swap '/` keys.
 *
 * Revision 1.48  2002/06/09 04:41:17  relnev
 * added copyright header
 *
 * Revision 1.47  2002/06/09 03:16:04  relnev
 * added _splitpath.
 *
 * removed unneeded asm, old sdl 2d setup.
 *
 * fixed crash caused by opengl_get_region.
 *
 * Revision 1.46  2002/06/05 04:03:32  relnev
 * finished cfilesystem.
 *
 * removed some old code.
 *
 * fixed mouse save off-by-one.
 *
 * sound cleanups.
 *
 * Revision 1.45  2002/06/03 09:25:37  relnev
 * implement mouse cursor and screen save/restore
 *
 * Revision 1.44  2002/06/02 18:46:59  relnev
 * updated
 *
 * Revision 1.43  2002/06/02 11:34:00  relnev
 * adjust z coords
 *
 * Revision 1.42  2002/06/02 10:28:17  relnev
 * fix texture handle leak
 *
 * Revision 1.41  2002/06/01 09:00:34  relnev
 * silly debug memmanager
 *
 * Revision 1.40  2002/06/01 07:12:33  relnev
 * a few NDEBUG updates.
 *
 * removed a few warnings.
 *
 * Revision 1.39  2002/06/01 05:33:15  relnev
 * copied more code over.
 *
 * added scissor clipping.
 *
 * Revision 1.38  2002/06/01 03:35:27  relnev
 * fix typo
 *
 * Revision 1.37  2002/06/01 03:32:00  relnev
 * fix texture loading mistake.
 *
 * enable some d3d stuff for opengl also
 *
 * Revision 1.36  2002/05/31 23:25:03  relnev
 * line fixes
 *
 * Revision 1.34  2002/05/31 22:15:22  relnev
 * BGRA
 *
 * Revision 1.33  2002/05/31 22:04:55  relnev
 * use d3d rect_internal
 *
 * Revision 1.32  2002/05/31 06:28:23  relnev
 * more stuff
 *
 * Revision 1.31  2002/05/31 06:04:39  relnev
 * fog
 *
 * Revision 1.30  2002/05/31 03:56:11  theoddone33
 * Change tmapper polygon winding and enable culling
 *
 * Revision 1.29  2002/05/31 03:34:02  theoddone33
 * Fix Keyboard
 * Add titlebar
 *
 * Revision 1.28  2002/05/31 00:06:59  relnev
 * minor change
 *
 * Revision 1.27  2002/05/30 23:46:29  theoddone33
 * some minor key changes (not necessarily fixes)
 *
 * Revision 1.26  2002/05/30 23:33:12  relnev
 * implemented a few more functions.
 *
 * Revision 1.25  2002/05/30 23:01:16  relnev
 * implement gr_opengl_set_state.
 *
 * Revision 1.24  2002/05/30 22:12:57  relnev
 * finish default texture case
 *
 * Revision 1.23  2002/05/30 22:02:30  theoddone33
 * More gl changes
 *
 * Revision 1.22  2002/05/30 21:44:48  relnev
 * implemented some missing texture stuff.
 *
 * enable bitmap polys for opengl.
 *
 * work around greenness in bitmaps.
 *
 * Revision 1.21  2002/05/30 17:29:30  theoddone33
 * Fix some more stubs, change at least one polygon winding since culling is now
 * enabled.
 *
 * Revision 1.20  2002/05/30 16:50:24  theoddone33
 * Keyboard partially fixed
 *
 * Revision 1.19  2002/05/30 08:13:14  relnev
 * fonts are fixed
 *
 * Revision 1.18  2002/05/29 23:37:36  relnev
 * fix bitmap bug
 *
 * Revision 1.17  2002/05/29 23:17:49  theoddone33
 * Non working text code and fixed keys
 *
 * Revision 1.16  2002/05/29 19:45:13  theoddone33
 * More changes on texture loading
 *
 * Revision 1.15  2002/05/29 19:06:48  theoddone33
 * Enable string printing.  Enable texture mapping
 *
 * Revision 1.14  2002/05/29 08:54:40  relnev
 * "fixed" bitmap drawing.
 *
 * copied more d3d code over.
 *
 * Revision 1.13  2002/05/29 06:25:13  theoddone33
 * Keyboard input, mouse tracking now work
 *
 * Revision 1.12  2002/05/29 04:52:45  relnev
 * bitmap
 *
 * Revision 1.11  2002/05/29 04:29:56  relnev
 * removed some unncessary stubbing, implemented opengl rect
 *
 * Revision 1.10  2002/05/29 04:13:27  theoddone33
 * enable opengl_line
 *
 * Revision 1.9  2002/05/29 03:35:51  relnev
 * added rest of init
 *
 * Revision 1.8  2002/05/29 03:30:05  relnev
 * update opengl stubs
 *
 * Revision 1.7  2002/05/29 02:52:32  theoddone33
 * Enable OpenGL renderer
 *
 * Revision 1.6  2002/05/28 04:56:51  theoddone33
 * runs a little bit now
 *
 * Revision 1.5  2002/05/28 04:07:28  theoddone33
 * New graphics stubbing arrangement
 *
 * Revision 1.4  2002/05/27 23:39:34  relnev
 * 0
 *
 * Revision 1.3  2002/05/27 22:35:01  theoddone33
 * more symbols
 *
 * Revision 1.2  2002/05/27 22:32:02  theoddone33
 * throw all d3d stuff at opengl
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 10    7/14/99 9:42a Dave
 * Put in clear_color debug function. Put in base for 3dnow stuff / P3
 * stuff
 * 
 * 9     7/09/99 9:51a Dave
 * Added thick polyline code.
 * 
 * 8     6/29/99 10:35a Dave
 * Interface polygon bitmaps! Whee!
 * 
 * 7     2/03/99 11:44a Dave
 * Fixed d3d transparent textures.
 * 
 * 6     1/24/99 11:37p Dave
 * First full rev of beam weapons. Very customizable. Removed some bogus
 * Int3()'s in low level net code.
 * 
 * 5     12/18/98 1:13a Dave
 * Rough 1024x768 support for Direct3D. Proper detection and usage through
 * the launcher.
 * 
 * 4     12/06/98 2:36p Dave
 * Drastically improved nebula fogging.
 * 
 * 3     11/11/98 5:37p Dave
 * Checkin for multiplayer testing.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 14    5/20/98 9:46p John
 * added code so the places in code that change half the palette don't
 * have to clear the screen.
 * 
 * 13    5/06/98 5:30p John
 * Removed unused cfilearchiver.  Removed/replaced some unused/little used
 * graphics functions, namely gradient_h and _v and pixel_sp.   Put in new
 * DirectX header files and libs that fixed the Direct3D alpha blending
 * problems.
 * 
 * 12    4/14/98 12:15p John
 * Made 16-bpp movies work.
 * 
 * 11    3/12/98 5:36p John
 * Took out any unused shaders.  Made shader code take rgbc instead of
 * matrix and vector since noone used it like a matrix and it would have
 * been impossible to do in hardware.   Made Glide implement a basic
 * shader for online help.  
 * 
 * 10    3/10/98 4:18p John
 * Cleaned up graphics lib.  Took out most unused gr functions.   Made D3D
 * & Glide have popups and print screen.  Took out all >8bpp software
 * support.  Made Fred zbuffer.  Made zbuffer allocate dynamically to
 * support Fred.  Made zbuffering key off of functions rather than one
 * global variable.
 * 
 * 9     12/02/97 4:00p John
 * Added first rev of thruster glow, along with variable levels of
 * translucency, which retquired some restructing of palman.
 * 
 * 8     10/03/97 9:10a John
 * added better antialiased line drawer
 * 
 * 7     9/23/97 10:45a John
 * made so you can tell bitblt code to rle a bitmap by passing flag to
 * gr_set_bitmap
 * 
 * 6     9/09/97 11:01a Sandeep
 * fixed warning level 4 bugs
 * 
 * 5     7/10/97 2:06p John
 * added code to specify alphablending type for bitmaps.
 * 
 * 4     6/17/97 7:04p John
 * added d3d support for gradients.
 * fixed some color bugs by adding screen signatures instead of watching
 * flags and palette changes.
 * 
 * 3     6/12/97 2:50a Lawrance
 * bm_unlock() now passed bitmap number, not pointer
 * 
 * 2     6/11/97 1:12p John
 * Started fixing all the text colors in the game.
 * 
 * 1     5/12/97 12:14p John
 *
 * $NoKeywords: $
 */

#ifndef PLAT_UNIX
#include <windows.h>
#include <windowsx.h>
#endif

#ifdef __vita__
#include <vitaGL.h>
#endif

#include "pstypes.h"
#include "osapi.h"
#include "2d.h"
#include "3d.h"
#include "bmpman.h"
#include "floating.h"
#include "palman.h"
#include "systemvars.h"
#include "grinternal.h"
#include "gropengl.h"
#include "line.h"
#include "neb.h"
#include "mouse.h"
#include "osregistry.h"
#include "cmdline.h"
#include "cfile.h"

#ifdef HAVE_GLES
// some helper functions
#define glColor3ub(r, g, b) glColor4ub(r, g, b, 255)
#define glFogi glFogf
#define glOrtho	glOrthof
#define glDepthRange glDepthRangef
#include "eglport.h"
#endif

#ifdef __vita__
float *mapped_vtx;
uint8_t *mapped_clr;
float *mapped_tex;
float *mapped_vtx_head;
uint8_t *mapped_clr_head;
float *mapped_tex_head;
uint16_t *mapped_idx;
#endif

static int Inited = 0;

typedef enum gr_texture_source {
	TEXTURE_SOURCE_NONE,
	TEXTURE_SOURCE_DECAL,
	TEXTURE_SOURCE_NO_FILTERING,
} gr_texture_source;

typedef enum gr_alpha_blend {
        ALPHA_BLEND_NONE,			// 1*SrcPixel + 0*DestPixel
        ALPHA_BLEND_ADDITIVE,			// 1*SrcPixel + 1*DestPixel
        ALPHA_BLEND_ALPHA_ADDITIVE,             // Alpha*SrcPixel + 1*DestPixel
        ALPHA_BLEND_ALPHA_BLEND_ALPHA,          // Alpha*SrcPixel + (1-Alpha)*DestPixel
        ALPHA_BLEND_ALPHA_BLEND_SRC_COLOR,      // Alpha*SrcPixel + (1-SrcPixel)*DestPixel
} gr_alpha_blend;

typedef enum gr_zbuffer_type {
        ZBUFFER_TYPE_NONE,
        ZBUFFER_TYPE_READ,
        ZBUFFER_TYPE_WRITE,
        ZBUFFER_TYPE_FULL,
} gr_zbuffer_type;
                        
#define NEBULA_COLORS 20

volatile int GL_activate = 0;
volatile int GL_deactivate = 0;

static int GL_use_luminance_alpha;
#ifndef HAVE_GLES
static int FSAA;
#endif
static ubyte GL_xlat[256] = { 0 };

static char *Gr_saved_screen = NULL;
static int Gr_saved_screen_bitmap;

static int Gr_opengl_mouse_saved = 0;
static int Gr_opengl_mouse_saved_x1 = 0;
static int Gr_opengl_mouse_saved_y1 = 0;
static int Gr_opengl_mouse_saved_x2 = 0;
static int Gr_opengl_mouse_saved_y2 = 0;
static int Gr_opengl_mouse_saved_w = 0;
static int Gr_opengl_mouse_saved_h = 0;
#define MAX_SAVE_SIZE (32*32)
static ubyte Gr_opengl_mouse_saved_data[MAX_SAVE_SIZE*2];

#define CLAMP(x,r1,r2) do { if ( (x) < (r1) ) (x) = (r1); else if ((x) > (r2)) (x) = (r2); } while(0)

#ifdef PLAT_UNIX
// Throw in some dummy functions - DDOI

int D3D_32bit = 0;		// grd3d.cpp
int D3D_fog_mode = -1;		// grd3d.cpp
int D3D_inited = 0;		// grd3d.cpp
int D3D_zbias = 1;		// grd3d.cpp
int D3d_rendition_uvs = 0;	// grd3d.cpp

void d3d_flush ()
{
	STUB_FUNCTION;
}

void d3d_zbias (int a)
{
	STUB_FUNCTION;
}
#endif

static void gr_opengl_set_texture_state(gr_texture_source ts);

static gr_alpha_blend GL_current_alpha_blend = (gr_alpha_blend) -1;
static gr_zbuffer_type GL_current_zbuffer_type = (gr_zbuffer_type) -1;

static void gr_opengl_set_state(gr_texture_source ts, gr_alpha_blend ab, gr_zbuffer_type zt)
{
	gr_opengl_set_texture_state(ts);
	
	if (ab != GL_current_alpha_blend) {
		switch (ab) {
			case ALPHA_BLEND_NONE:			// 1*SrcPixel + 0*DestPixel
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
			case ALPHA_BLEND_ADDITIVE:		// 1*SrcPixel + 1*DestPixel
				glBlendFunc(GL_ONE, GL_ONE);
				break;
			case ALPHA_BLEND_ALPHA_ADDITIVE:	// Alpha*SrcPixel + 1*DestPixel
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case ALPHA_BLEND_ALPHA_BLEND_ALPHA:	// Alpha*SrcPixel + (1-Alpha)*DestPixel
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case ALPHA_BLEND_ALPHA_BLEND_SRC_COLOR:	// Alpha*SrcPixel + (1-SrcPixel)*DestPixel
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
				break;
			default:
				break;
		}
		
		GL_current_alpha_blend = ab;
	}
	
	if (zt != GL_current_zbuffer_type) {
		switch (zt) {
			case ZBUFFER_TYPE_NONE:
				glDepthFunc(GL_ALWAYS);
				glDepthMask(GL_FALSE);
				break;
			case ZBUFFER_TYPE_READ:
				glDepthFunc(GL_LESS);
				glDepthMask(GL_FALSE);	
				break;
			case ZBUFFER_TYPE_WRITE:
				glDepthFunc(GL_ALWAYS);
				glDepthMask(GL_TRUE);
				break;
			case ZBUFFER_TYPE_FULL:
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
				break;
			default:
				break;
		}
		
		GL_current_zbuffer_type = zt;
	}	
}

void gr_opengl_activate(int active)
{
	if (active) {
		GL_activate++;
		
		// don't grab key/mouse if cmdline says so or if we're fullscreen
		if(!Cmdline_no_grab && !(SDL_GetVideoSurface()->flags & SDL_FULLSCREEN)) {
			SDL_WM_GrabInput(SDL_GRAB_ON);
		}
	} else {
		GL_deactivate++;

		// let go of mouse/keyboard
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
}


static void opengl_tcache_flush ();

void gr_opengl_preload_init()
{
	if (gr_screen.mode != GR_OPENGL) {
		return;
	}

	opengl_tcache_flush ();
}

int GL_should_preload = 0;
int gr_opengl_preload(int bitmap_num, int is_aabitmap)
{
	if ( gr_screen.mode != GR_OPENGL) {
		return 0;
	}

	if ( !GL_should_preload )      {
		return 0;
	}

	float u_scale, v_scale;

	int retval;
	if ( is_aabitmap )      {
		retval = gr_tcache_set(bitmap_num, TCACHE_TYPE_AABITMAP, &u_scale, &v_scale, 1, -1, -1, 0 );
	} else {
		retval = gr_tcache_set(bitmap_num, TCACHE_TYPE_NORMAL, &u_scale, &v_scale, 1, -1, -1, 0 );
	}

	if ( !retval )  {
		mprintf(("Texture upload failed!\n" ));
	}

	return retval;
}

void gr_opengl_pixel(int x, int y)
{
	gr_line(x,y,x,y);
}

void gr_opengl_clear()
{
	glClearColor(gr_screen.current_clear_color.red / 255.0, 
		gr_screen.current_clear_color.green / 255.0, 
		gr_screen.current_clear_color.blue / 255.0, 1.0);
	glClear ( GL_COLOR_BUFFER_BIT );
}

void gr_opengl_save_mouse_area(int x, int y, int w, int h);
static void opengl_tcache_frame ();

void gr_opengl_flip()
{
	if (!Inited) return;

	gr_reset_clip();

	mouse_eval_deltas();

	Gr_opengl_mouse_saved = 0;
	
	if ( mouse_is_visible() )       {
		int mx, my;
		
	 	gr_reset_clip();
	 	mouse_get_pos( &mx, &my );
	 	
	 	gr_opengl_save_mouse_area(mx,my,32,32);
	 	
	 	if ( Gr_cursor == -1 )  {
	 		// stuff
	 	} else {
	 		gr_set_bitmap(Gr_cursor, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
			gr_bitmap( mx, my );
	 	}
	}
	 
	 
#ifndef NDEBUG
	GLenum error = glGetError();
	int ic = 0;
	do {
		error = glGetError();
		
		if (error != GL_NO_ERROR) {
			nprintf(("Warning", "!!DEBUG!! OpenGL Error: %d (%d this frame)\n", error, ic));
		}
		ic++;
	} while (error != GL_NO_ERROR);
#endif
	
#if defined(HAVE_GLES) && !defined(__vita__)
	EGL_SwapBuffers();
#else
	SDL_GL_SwapBuffers ();
#ifdef __vita__
	mapped_vtx_head = mapped_vtx;
	mapped_tex_head = mapped_tex;
	mapped_clr_head = mapped_clr;
	glViewport(0, 0, 960, 544);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport((960 - 725) / 2, 0, 725, 544);
#endif
#endif

	opengl_tcache_frame ();
	
	int cnt = GL_activate;
	if ( cnt )      {
		GL_activate-=cnt;
		opengl_tcache_flush();
		// gr_opengl_clip_cursor(1); /* mouse grab, see opengl_activate */
	}
	
	cnt = GL_deactivate;
	if ( cnt )      {
		GL_deactivate-=cnt;
		// gr_opengl_clip_cursor(0);  /* mouse grab, see opengl_activate */
	}
	// restaure mouse if saved ???
/*	if (Gr_opengl_mouse_saved) {		
 		gr_set_bitmap(Gr_opengl_mouse_saved_data, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
		gr_bitmap( Gr_opengl_mouse_saved_x1, Gr_opengl_mouse_saved_y1 );
	}*/
}

void gr_opengl_flip_window(uint _hdc, int x, int y, int w, int h )
{
	// Not used.
}

void gr_opengl_set_clip(int x,int y,int w,int h)
{
	// check for sanity of parameters
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (x >= gr_screen.max_w)
		x = gr_screen.max_w - 1;
	if (y >= gr_screen.max_h)
		y = gr_screen.max_h - 1;

	if (x + w > gr_screen.max_w)
		w = gr_screen.max_w - x;
	if (y + h > gr_screen.max_h)
		h = gr_screen.max_h - y;
	
	if (w > gr_screen.max_w)
		w = gr_screen.max_w;
	if (h > gr_screen.max_h)
		h = gr_screen.max_h;
	
	gr_screen.offset_x = x;
	gr_screen.offset_y = y;
	gr_screen.clip_left = 0;
	gr_screen.clip_right = w-1;
	gr_screen.clip_top = 0;
	gr_screen.clip_bottom = h-1;
	gr_screen.clip_width = w;
	gr_screen.clip_height = h;
	
	glEnable(GL_SCISSOR_TEST);
#ifdef PANDORA
	glScissor(x+80, gr_screen.max_h-y-h, w, h);
#elif defined(__vita__)
	#define X_RATIO (725.0f / 640.0f)
	#define Y_RATIO (544.0f / 480.0f)
	glScissor((960 - 725) / 2 + (int)((float)x * X_RATIO), (int)(544- (float)(y + h) * Y_RATIO), (int)((float)w * X_RATIO), (int)((float)h * Y_RATIO));
#else
	glScissor(x, gr_screen.max_h-y-h, w, h);
#endif
}

void gr_opengl_reset_clip()
{
	gr_screen.offset_x = 0;
	gr_screen.offset_y = 0;
	gr_screen.clip_left = 0;
	gr_screen.clip_top = 0;
	gr_screen.clip_right = gr_screen.max_w - 1;
	gr_screen.clip_bottom = gr_screen.max_h - 1;
	gr_screen.clip_width = gr_screen.max_w;
	gr_screen.clip_height = gr_screen.max_h;
	glDisable(GL_SCISSOR_TEST);
//	glScissor(0, 0, gr_screen.max_w, gr_screen.max_h);
}

void gr_opengl_set_bitmap( int bitmap_num, int alphablend_mode = GR_ALPHABLEND_NONE, int bitblt_mode = GR_BITBLT_MODE_NORMAL, float alpha = 1.0f, int sx = -1, int sy = -1 )
{
	gr_screen.current_alpha = alpha;
	gr_screen.current_alphablend_mode = alphablend_mode;
	gr_screen.current_bitblt_mode = bitblt_mode;
	gr_screen.current_bitmap = bitmap_num;

	gr_screen.current_bitmap_sx = sx;
	gr_screen.current_bitmap_sy = sy;
}

void gr_opengl_create_shader(shader * shade, float r, float g, float b, float c )
{
	shade->screen_sig = gr_screen.signature;
	shade->r = r;
	shade->g = g;
	shade->b = b;
	shade->c = c;	
}

void gr_opengl_set_shader( shader * shade )
{	
	if ( shade )	{
		if (shade->screen_sig != gr_screen.signature)	{
			gr_create_shader( shade, shade->r, shade->g, shade->b, shade->c );
		}
		gr_screen.current_shader = *shade;
	} else {
		gr_create_shader( &gr_screen.current_shader, 0.0f, 0.0f, 0.0f, 0.0f );
	}
}


void gr_opengl_bitmap_ex(int x,int y,int w,int h,int sx,int sy)
{
	STUB_FUNCTION; /* who called me? */
}

void gr_opengl_bitmap(int x, int y)
{
	STUB_FUNCTION; /* who called me? */
}

static void gr_opengl_rect_internal(int x, int y, int w, int h, int r, int g, int b, int a)
{
	int saved_zbuf;
	vertex v[4];
	vertex *verts[4] = {&v[0], &v[1], &v[2], &v[3]};

	saved_zbuf = gr_zbuffer_get();
	
	// start the frame, no zbuffering, no culling
	g3_start_frame(1);	
	gr_zbuffer_set(GR_ZBUFF_NONE);		
	gr_set_cull(0);		

	// stuff coords		
	v[0].sx = i2fl(x);
	v[0].sy = i2fl(y);
	v[0].sw = 0.0f;
	v[0].u = 0.0f;
	v[0].v = 0.0f;
	v[0].flags = PF_PROJECTED;
	v[0].codes = 0;
	v[0].r = (ubyte)r;
	v[0].g = (ubyte)g;
	v[0].b = (ubyte)b;
	v[0].a = (ubyte)a;

	v[1].sx = i2fl(x + w);
	v[1].sy = i2fl(y);	
	v[1].sw = 0.0f;
	v[1].u = 0.0f;
	v[1].v = 0.0f;
	v[1].flags = PF_PROJECTED;
	v[1].codes = 0;
	v[1].r = (ubyte)r;
	v[1].g = (ubyte)g;
	v[1].b = (ubyte)b;
	v[1].a = (ubyte)a;

	v[2].sx = i2fl(x + w);
	v[2].sy = i2fl(y + h);
	v[2].sw = 0.0f;
	v[2].u = 0.0f;
	v[2].v = 0.0f;
	v[2].flags = PF_PROJECTED;
	v[2].codes = 0;
	v[2].r = (ubyte)r;
	v[2].g = (ubyte)g;
	v[2].b = (ubyte)b;
	v[2].a = (ubyte)a;

	v[3].sx = i2fl(x);
	v[3].sy = i2fl(y + h);
	v[3].sw = 0.0f;
	v[3].u = 0.0f;
	v[3].v = 0.0f;
	v[3].flags = PF_PROJECTED;
	v[3].codes = 0;				
	v[3].r = (ubyte)r;
	v[3].g = (ubyte)g;
	v[3].b = (ubyte)b;
	v[3].a = (ubyte)a;

	// draw the polys
	g3_draw_poly_constant_sw(4, verts, TMAP_FLAG_GOURAUD | TMAP_FLAG_RGB | TMAP_FLAG_ALPHA, 0.1f);		

	g3_end_frame();

	// restore zbuffer and culling
	gr_zbuffer_set(saved_zbuf);
	gr_set_cull(1);	
}

void gr_opengl_rect(int x,int y,int w,int h)
{
	gr_opengl_rect_internal(x, y, w, h, gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, gr_screen.current_color.alpha);
}

void gr_opengl_shade(int x,int y,int w,int h)
{
	int r,g,b,a;
	
	float shade1 = 1.0f;
	float shade2 = 6.0f;

	r = fl2i(gr_screen.current_shader.r*255.0f*shade1);
	if ( r < 0 ) r = 0; else if ( r > 255 ) r = 255;
	g = fl2i(gr_screen.current_shader.g*255.0f*shade1);
	if ( g < 0 ) g = 0; else if ( g > 255 ) g = 255;
	b = fl2i(gr_screen.current_shader.b*255.0f*shade1);
	if ( b < 0 ) b = 0; else if ( b > 255 ) b = 255;
	a = fl2i(gr_screen.current_shader.c*255.0f*shade2);
	if ( a < 0 ) a = 0; else if ( a > 255 ) a = 255;

        gr_opengl_rect_internal(x, y, w, h, r, g, b, a);	
}

static void gr_opengl_aabitmap_ex_internal(int x,int y,int w,int h,int sx,int sy)
{
	if ( w < 1 ) return;
	if ( h < 1 ) return;

	if ( !gr_screen.current_color.is_alphacolor )	return;

	float u_scale, v_scale;

	if ( !gr_tcache_set( gr_screen.current_bitmap, TCACHE_TYPE_AABITMAP, &u_scale, &v_scale, 0, -1, -1, 0 ) )	{
		// Couldn't set texture
		mprintf(( "WARNING: Error setting aabitmap texture!\n" ));
		return;
	}

	gr_opengl_set_state( TEXTURE_SOURCE_NO_FILTERING, ALPHA_BLEND_ALPHA_BLEND_ALPHA, ZBUFFER_TYPE_NONE );
	
	float u0, u1, v0, v1;
	float x1, x2, y1, y2;
	int bw, bh;

	bm_get_info( gr_screen.current_bitmap, &bw, &bh );

	u0 = u_scale*i2fl(sx)/i2fl(bw);
	v0 = v_scale*i2fl(sy)/i2fl(bh);

	u1 = u_scale*i2fl(sx+w)/i2fl(bw);
	v1 = v_scale*i2fl(sy+h)/i2fl(bh);

	x1 = i2fl(x+gr_screen.offset_x);
	y1 = i2fl(y+gr_screen.offset_y);
	x2 = i2fl(x+w+gr_screen.offset_x);
	y2 = i2fl(y+h+gr_screen.offset_y);

	if ( gr_screen.current_color.is_alphacolor )	{
		glColor4ub(gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue,gr_screen.current_color.alpha);
	} else {
		glColor3ub(gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue);
	}

#ifdef HAVE_GLES
#ifdef __vita__
	mapped_vtx_head[0] = mapped_vtx_head[9] = x1;
	mapped_vtx_head[3] = mapped_vtx_head[6] = x2;
	mapped_vtx_head[7] = mapped_vtx_head[10] = y1;
	mapped_vtx_head[1] = mapped_vtx_head[4] = y2;
	mapped_vtx_head[2] = mapped_vtx_head[5] = mapped_vtx_head[8] = mapped_vtx_head[11] = -0.99;
	mapped_tex_head[0] = mapped_tex_head[6] = u0;
	mapped_tex_head[2] = mapped_tex_head[4] = u1;
	mapped_tex_head[5] = mapped_tex_head[7] = v0;
	mapped_tex_head[1] = mapped_tex_head[3] = v1;
#else
	GLfloat vtx1[] = {
	 x1, y2, -0.99,
	 x2, y2, -0.99,
	 x2, y1, -0.99,
	 x1, y1, -0.99
	};
	GLfloat tex1[] = {
	 u0, v1,
	 u1, v1,
	 u1, v0,
	 u0, v0
	};
#endif
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#ifdef __vita__
	vglVertexPointerMapped(3, mapped_vtx_head);
	vglTexCoordPointerMapped(mapped_tex_head);
	vglDrawObjects(GL_TRIANGLE_FAN, 4, GL_TRUE);
	mapped_vtx_head += 12;
	mapped_tex_head += 8;
#else
	glVertexPointer(3, GL_FLOAT, 0, vtx1);
	glTexCoordPointer(2, GL_FLOAT, 0, tex1);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
	glBegin (GL_QUADS);
	  glTexCoord2f (u0, v1);
	  glVertex3f (x1, y2, -0.99);

	  glTexCoord2f (u1, v1);
	  glVertex3f (x2, y2, -0.99);

	  glTexCoord2f (u1, v0);
	  glVertex3f (x2, y1, -0.99);

	  glTexCoord2f (u0, v0);
	  glVertex3f (x1, y1, -0.99);
	glEnd ();
#endif
}

void gr_opengl_aabitmap_ex(int x,int y,int w,int h,int sx,int sy)
{
	int reclip;
#ifndef NDEBUG
	int count = 0;
#endif

	int dx1=x, dx2=x+w-1;
	int dy1=y, dy2=y+h-1;

	int bw, bh;
	bm_get_info( gr_screen.current_bitmap, &bw, &bh, NULL );

	do {
		reclip = 0;
#ifndef NDEBUG
			if ( count > 1 ) Int3();
			count++;
#endif
	
		if ((dx1 > gr_screen.clip_right ) || (dx2 < gr_screen.clip_left)) return;
		if ((dy1 > gr_screen.clip_bottom ) || (dy2 < gr_screen.clip_top)) return;
		if ( dx1 < gr_screen.clip_left ) { sx += gr_screen.clip_left-dx1; dx1 = gr_screen.clip_left; }
		if ( dy1 < gr_screen.clip_top ) { sy += gr_screen.clip_top-dy1; dy1 = gr_screen.clip_top; }
		if ( dx2 > gr_screen.clip_right )	{ dx2 = gr_screen.clip_right; }
		if ( dy2 > gr_screen.clip_bottom )	{ dy2 = gr_screen.clip_bottom; }

		if ( sx < 0 ) {
			dx1 -= sx;
			sx = 0;
			reclip = 1;
		}

		if ( sy < 0 ) {
			dy1 -= sy;
			sy = 0;
			reclip = 1;
		}

		w = dx2-dx1+1;
		h = dy2-dy1+1;

		if ( sx + w > bw ) {
			w = bw - sx;
			dx2 = dx1 + w - 1;
		}

		if ( sy + h > bh ) {
			h = bh - sy;
			dy2 = dy1 + h - 1;
		}

		if ( w < 1 ) return;		// clipped away!
		if ( h < 1 ) return;		// clipped away!

	} while (reclip);

	// Make sure clipping algorithm works
#ifndef NDEBUG
		Assert( w > 0 );
		Assert( h > 0 );
		Assert( w == (dx2-dx1+1) );
		Assert( h == (dy2-dy1+1) );
		Assert( sx >= 0 );
		Assert( sy >= 0 );
		Assert( sx+w <= bw );
		Assert( sy+h <= bh );
		Assert( dx2 >= dx1 );
		Assert( dy2 >= dy1 );
		Assert( (dx1 >= gr_screen.clip_left ) && (dx1 <= gr_screen.clip_right) );
		Assert( (dx2 >= gr_screen.clip_left ) && (dx2 <= gr_screen.clip_right) );
		Assert( (dy1 >= gr_screen.clip_top ) && (dy1 <= gr_screen.clip_bottom) );
		Assert( (dy2 >= gr_screen.clip_top ) && (dy2 <= gr_screen.clip_bottom) );
#endif

	// We now have dx1,dy1 and dx2,dy2 and sx, sy all set validly within clip regions.
	gr_opengl_aabitmap_ex_internal(dx1,dy1,dx2-dx1+1,dy2-dy1+1,sx,sy);
}

void gr_opengl_aabitmap(int x, int y)
{
	int w, h;

	bm_get_info( gr_screen.current_bitmap, &w, &h, NULL );
	int dx1=x, dx2=x+w-1;
	int dy1=y, dy2=y+h-1;
	int sx=0, sy=0;

	if ((dx1 > gr_screen.clip_right ) || (dx2 < gr_screen.clip_left)) return;
	if ((dy1 > gr_screen.clip_bottom ) || (dy2 < gr_screen.clip_top)) return;
	if ( dx1 < gr_screen.clip_left ) { sx = gr_screen.clip_left-dx1; dx1 = gr_screen.clip_left; }
	if ( dy1 < gr_screen.clip_top ) { sy = gr_screen.clip_top-dy1; dy1 = gr_screen.clip_top; }
	if ( dx2 > gr_screen.clip_right )	{ dx2 = gr_screen.clip_right; }
	if ( dy2 > gr_screen.clip_bottom )	{ dy2 = gr_screen.clip_bottom; }

	if ( sx < 0 ) return;
	if ( sy < 0 ) return;
	if ( sx >= w ) return;
	if ( sy >= h ) return;

	// Draw bitmap bm[sx,sy] into (dx1,dy1)-(dx2,dy2)
	gr_aabitmap_ex(dx1,dy1,dx2-dx1+1,dy2-dy1+1,sx,sy);
}

void gr_opengl_string( int sx, int sy, const char *s )
{
	int width, spacing, letter;
	int x, y;

	if ( !Current_font )	{
		return;
	}

	gr_set_bitmap(Current_font->bitmap_id, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);

	x = sx;
	y = sy;

	if (sx==0x8000) {			//centered
		x = get_centered_x(s);
	} else {
		x = sx;
	}
	
	spacing = 0;

	while (*s)	{
		x += spacing;

		while (*s== '\n' )	{
			s++;
			y += Current_font->h;
			if (sx==0x8000) {			//centered
				x = get_centered_x(s);
			} else {
				x = sx;
			}
		}
		if (*s == 0 ) break;

		letter = get_char_width(s[0],s[1],&width,&spacing);
		s++;

		//not in font, draw as space
		if (letter<0)	{
			continue;
		}

		int xd, yd, xc, yc;
		int wc, hc;

		// Check if this character is totally clipped
		if ( x + width < gr_screen.clip_left ) continue;
		if ( y + Current_font->h < gr_screen.clip_top ) continue;
		if ( x > gr_screen.clip_right ) continue;
		if ( y > gr_screen.clip_bottom ) continue;

		xd = yd = 0;
		if ( x < gr_screen.clip_left ) xd = gr_screen.clip_left - x;
		if ( y < gr_screen.clip_top ) yd = gr_screen.clip_top - y;
		xc = x+xd;
		yc = y+yd;

		wc = width - xd; hc = Current_font->h - yd;
		if ( xc + wc > gr_screen.clip_right ) wc = gr_screen.clip_right - xc;
		if ( yc + hc > gr_screen.clip_bottom ) hc = gr_screen.clip_bottom - yc;

		if ( wc < 1 ) continue;
		if ( hc < 1 ) continue;

		int u = Current_font->bm_u[letter];
		int v = Current_font->bm_v[letter];

		gr_opengl_aabitmap_ex_internal( xc, yc, wc, hc, u+xd, v+yd );
	}
}

void gr_opengl_line(int x1,int y1,int x2,int y2)
{
	int clipped = 0, swapped=0;

	gr_opengl_set_state( TEXTURE_SOURCE_NONE, ALPHA_BLEND_ALPHA_BLEND_ALPHA, ZBUFFER_TYPE_NONE );
	
	INT_CLIPLINE(x1,y1,x2,y2,gr_screen.clip_left,gr_screen.clip_top,gr_screen.clip_right,gr_screen.clip_bottom,return,clipped=1,swapped=1);
	
	float sx1, sy1;
	float sx2, sy2;
	
	sx1 = i2fl(x1 + gr_screen.offset_x)+0.5;
	sy1 = i2fl(y1 + gr_screen.offset_y)+0.5;
	sx2 = i2fl(x2 + gr_screen.offset_x)+0.5;
	sy2 = i2fl(y2 + gr_screen.offset_y)+0.5;
	
	if ( x1 == x2 && y1 == y2 ) {
#ifdef HAVE_GLES
		glColor4ub (gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, gr_screen.current_color.alpha);
		glEnableClientState(GL_VERTEX_ARRAY);
#ifdef __vita__
		mapped_vtx_head[0] = sx1;
		mapped_vtx_head[1] = sy1;
		mapped_vtx_head[2] = -0.99f;
		vglVertexPointerMapped(3, mapped_vtx_head);
		vglDrawObjects(GL_POINTS, 1, GL_TRUE);
		mapped_vtx_head += 3;
#else
		GLfloat vtx1[] = {sx1, sy1, -0.99f};
		glVertexPointer(3, GL_FLOAT, 0, vtx1);
		glDrawArrays(GL_POINTS,0,1);
#endif
		glDisableClientState(GL_VERTEX_ARRAY);
#else
		glBegin (GL_POINTS);
		  glColor4ub (gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, gr_screen.current_color.alpha);
		  glVertex3f (sx1, sy1, -0.99f);
		glEnd ();
#endif
		
		return;
	}
	
	if ( x1 == x2 ) {
		if ( sy1 < sy2 )    {
			sy2 += 0.5f;
		} else {
			sy1 += 0.5f;
		}
	} else if ( y1 == y2 )  {
		if ( sx1 < sx2 )    {
			sx2 += 0.5f;
		} else {
			sx1 += 0.5f;
		}
	}
	
#ifdef HAVE_GLES
	glColor4ub (gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, gr_screen.current_color.alpha);
	glEnableClientState(GL_VERTEX_ARRAY);
#ifdef __vita__
	mapped_vtx_head[0] = sx2;
	mapped_vtx_head[1] = sy2;
	mapped_vtx_head[2] = -0.99f;
	mapped_vtx_head[3] = sx1;
	mapped_vtx_head[4] = sy1;
	mapped_vtx_head[5] = -0.99f;
	vglVertexPointerMapped(3, mapped_vtx_head);
	vglDrawObjects(GL_LINES, 2, GL_TRUE);
	mapped_vtx_head += 6;
#else	
	GLfloat vtx1[] = {sx2, sy2, -0.99f, sx1, sy1, -0.99f};
	glVertexPointer(3, GL_FLOAT, 0, vtx1);
	glDrawArrays(GL_LINES,0,2);
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
#else
	glBegin (GL_LINES);
	  glColor4ub (gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, gr_screen.current_color.alpha);
	  glVertex3f (sx2, sy2, -0.99f);
	  glVertex3f (sx1, sy1, -0.99f);
	glEnd ();
#endif
}

void gr_opengl_aaline(vertex *v1, vertex *v2)
{
	gr_opengl_line( fl2i(v1->sx), fl2i(v1->sy), fl2i(v2->sx), fl2i(v2->sy) );
}

void gr_opengl_gradient(int x1,int y1,int x2,int y2)
{
	int clipped = 0, swapped=0;

	if ( !gr_screen.current_color.is_alphacolor )   {
		gr_line( x1, y1, x2, y2 );
		return;
	}

	INT_CLIPLINE(x1,y1,x2,y2,gr_screen.clip_left,gr_screen.clip_top,gr_screen.clip_right,gr_screen.clip_bottom,return,clipped=1,swapped=1);

	gr_opengl_set_state( TEXTURE_SOURCE_NONE, ALPHA_BLEND_ALPHA_BLEND_ALPHA, ZBUFFER_TYPE_NONE );

	int aa = swapped ? 0 : gr_screen.current_color.alpha;
	int ba = swapped ? gr_screen.current_color.alpha : 0;
	
	float sx1, sy1;
	float sx2, sy2;
	
	sx1 = i2fl(x1 + gr_screen.offset_x)+0.5;
	sy1 = i2fl(y1 + gr_screen.offset_y)+0.5;
	sx2 = i2fl(x2 + gr_screen.offset_x)+0.5;
	sy2 = i2fl(y2 + gr_screen.offset_y)+0.5;

	if ( x1 == x2 ) {
		if ( sy1 < sy2 )    {
			sy2 += 0.5f;
		} else {
			sy1 += 0.5f;
		}
	} else if ( y1 == y2 )  {
		if ( sx1 < sx2 )    {
			sx2 += 0.5f;
		} else {
			sx1 += 0.5f;
		}
	}
	
#ifdef HAVE_GLES
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
#ifdef __vita__
	mapped_vtx_head[0] = sx2;
	mapped_vtx_head[1] = sy2;
	mapped_vtx_head[2] = -0.99f;
	mapped_vtx_head[3] = sx1;
	mapped_vtx_head[4] = sy1;
	mapped_vtx_head[5] = -0.99f;
	mapped_clr_head[0] = mapped_clr_head[4] = gr_screen.current_color.red;
	mapped_clr_head[1] = mapped_clr_head[5] = gr_screen.current_color.green;
	mapped_clr_head[2] = mapped_clr_head[6] = gr_screen.current_color.blue;
	mapped_clr_head[3] = ba;
	mapped_clr_head[7] = aa;
	vglVertexPointerMapped(3, mapped_vtx_head);
	vglColorPointerMapped(GL_UNSIGNED_BYTE, mapped_clr_head);
	vglDrawObjects(GL_LINES, 2, GL_TRUE);
	mapped_vtx_head += 6;
	mapped_clr_head += 8;
#else
	GLfloat vtx1[] = {sx2, sy2, -0.99f, sx1, sy1, -0.99f};
	GLfloat col1[] = {
		gr_screen.current_color.red/255.0f, gr_screen.current_color.green/255.0f, gr_screen.current_color.blue/255.0f, ba/255.0f,
		gr_screen.current_color.red/255.0f, gr_screen.current_color.green/255.0f, gr_screen.current_color.blue/255.0f, aa/255.0f };
	glVertexPointer(3, GL_FLOAT, 0, vtx1);
	glColorPointer(4, GL_FLOAT, 0, col1);
	glDrawArrays(GL_LINES,0,2);
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
#else
	glBegin (GL_LINES);
	  glColor4ub (gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, ba);
	  glVertex3f (sx2, sy2, -0.99f);
	  glColor4ub (gr_screen.current_color.red, gr_screen.current_color.green, gr_screen.current_color.blue, aa);
	  glVertex3f (sx1, sy1, -0.99f);
	glEnd ();	
#endif
}

void gr_opengl_circle( int xc, int yc, int d )
{
	int p,x, y, r;

	r = d/2;
	p=3-d;
	x=0;
	y=r;

	// Big clip
	if ( (xc+r) < gr_screen.clip_left ) return;
	if ( (xc-r) > gr_screen.clip_right ) return;
	if ( (yc+r) < gr_screen.clip_top ) return;
	if ( (yc-r) > gr_screen.clip_bottom ) return;

	while(x<y)	{
		// Draw the first octant
		gr_opengl_line( xc-y, yc-x, xc+y, yc-x );
		gr_opengl_line( xc-y, yc+x, xc+y, yc+x );
                                
		if (p<0) 
			p=p+(x<<2)+6;
		else	{
			// Draw the second octant
			gr_opengl_line( xc-x, yc-y, xc+x, yc-y );
			gr_opengl_line( xc-x, yc+y, xc+x, yc+y );
                                                
			p=p+((x-y)<<2)+10;
			y--;
		}
		x++;
	}
	if(x==y) {
		gr_opengl_line( xc-x, yc-y, xc+x, yc-y );
		gr_opengl_line( xc-x, yc+y, xc+x, yc+y );
	}
	return;
}

static void gr_opengl_stuff_fog_value(float z, int *r, int *g, int *b, int *a)
{
	float f_float;
	
	f_float = (gr_screen.fog_far - z) / (gr_screen.fog_far - gr_screen.fog_near);
	if (f_float < 0.0f) {
		f_float = 0.0f;
	} else {
		f_float = 1.0f;
	}
	*r = 0;
	*g = 0;
	*b = 0;
	*a = (int)(f_float * 255.0);
}

static void gr_opengl_tmapper_internal( int nv, vertex ** verts, uint flags, int is_scaler )
{
	int i;
	float u_scale = 1.0f, v_scale = 1.0f;

	// Make nebula use the texture mapper... this blends the colors better.
	if ( flags & TMAP_FLAG_NEBULA ){
		Int3 ();
	}

	gr_texture_source texture_source = (gr_texture_source)-1;
	gr_alpha_blend alpha_blend = (gr_alpha_blend)-1;
	gr_zbuffer_type zbuffer_type = (gr_zbuffer_type)-1;
	
	if ( gr_zbuffering )    {
		if ( is_scaler || (gr_screen.current_alphablend_mode == GR_ALPHABLEND_FILTER)   )       {
			zbuffer_type = ZBUFFER_TYPE_READ;
		} else {
			zbuffer_type = ZBUFFER_TYPE_FULL;
		}
	} else {
		zbuffer_type = ZBUFFER_TYPE_NONE;
	}
	
	int alpha;

	int tmap_type = TCACHE_TYPE_NORMAL;

	int r, g, b;

	if ( flags & TMAP_FLAG_TEXTURED )       {
		r = g = b = 255;
	} else {
		r = gr_screen.current_color.red;
		g = gr_screen.current_color.green;
		b = gr_screen.current_color.blue;
	}

	if ( gr_screen.current_alphablend_mode == GR_ALPHABLEND_FILTER )        
	{
		if (1) {
			tmap_type = TCACHE_TYPE_NORMAL;
			alpha_blend = ALPHA_BLEND_ALPHA_ADDITIVE;
			
			// Blend with screen pixel using src*alpha+dst
			float factor = gr_screen.current_alpha;
			
			alpha = 255;
			
			if ( factor <= 1.0f )   {
				int tmp_alpha = fl2i(gr_screen.current_alpha*255.0f);
				r = (r*tmp_alpha)/255;
				g = (g*tmp_alpha)/255;
				b = (b*tmp_alpha)/255;
			}
		} else {
			tmap_type = TCACHE_TYPE_XPARENT;
			
			alpha_blend = ALPHA_BLEND_ALPHA_BLEND_ALPHA;
			
			// Blend with screen pixel using src*alpha+dst
			float factor = gr_screen.current_alpha;
				
			if ( factor > 1.0f )    {
				alpha = 255;
			} else {
				alpha = fl2i(gr_screen.current_alpha*255.0f);
			}
		}
	} else {
		if(Bm_pixel_format == BM_PIXEL_FORMAT_ARGB) {
			alpha_blend = ALPHA_BLEND_ALPHA_BLEND_ALPHA;
		} else {
			alpha_blend = ALPHA_BLEND_NONE;
		}
		alpha = 255;
	}

	if(flags & TMAP_FLAG_BITMAP_SECTION){
		tmap_type = TCACHE_TYPE_BITMAP_SECTION;
	}
	
	texture_source = TEXTURE_SOURCE_NONE;
	
	if ( flags & TMAP_FLAG_TEXTURED )       {
		if ( !gr_tcache_set(gr_screen.current_bitmap, tmap_type, &u_scale, &v_scale, 0, gr_screen.current_bitmap_sx, gr_screen.current_bitmap_sy, 0 ))
		{
			mprintf(( "Not rendering a texture because it didn't fit in VRAM!\n" ));
			return;
		}

		// use nonfiltered textures for bitmap sections
		if(flags & TMAP_FLAG_BITMAP_SECTION){
			texture_source = TEXTURE_SOURCE_NO_FILTERING;
		} else {
			texture_source = TEXTURE_SOURCE_DECAL;
		}
	}


	gr_opengl_set_state( texture_source, alpha_blend, zbuffer_type );
	
	if ( flags & TMAP_FLAG_TEXTURED )
	{
		// rendition junk
		// STUB_FUNCTION;
	}

	if (flags & TMAP_FLAG_PIXEL_FOG) {
		int r, g, b;
		int ra, ga, ba;
		ra = ga = ba = 0;
	
		/* argh */
		for (i=nv-1;i>=0;i--)	// DDOI - change polygon winding
		{
			vertex * va = verts[i];
			float sx, sy;
		
			int x, y;
			x = fl2i(va->sx*16.0f);
			y = fl2i(va->sy*16.0f);
		
			x += gr_screen.offset_x*16;
			y += gr_screen.offset_y*16;
		
			sx = i2fl(x) / 16.0f;
			sy = i2fl(y) / 16.0f;
		
			neb2_get_pixel((int)sx, (int)sy, &r, &g, &b);
			
			ra += r;
			ga += g;
			ba += b;
		}
		
		ra /= nv;
		ga /= nv;
		ba /= nv;
		
		gr_fog_set(GR_FOGMODE_FOG, ra, ga, ba, -1.0f, -1.0f);
	}
	
#ifdef HAVE_GLES
#ifdef __vita__
	float *vtx = mapped_vtx_head;
	uint8_t *col = mapped_clr_head;
	float *tex = mapped_tex_head;
#else
/*	GLfloat *vtx = new GLfloat[3*nv];
	GLfloat *col = new GLfloat[4*nv];
	GLFloat *tex;
	if ( flags & TMAP_FLAG_TEXTURED ) tex = new GLfloat[2*nv];*/
	static GLfloat *vtx=NULL;
	static GLfloat *col=NULL;
	static GLfloat *tex=NULL;
	static int taille=0;
	if (nv>taille) {
		delete[] vtx;
		delete[] col;
		delete[] tex;
		taille=nv;
		vtx=new GLfloat[4*nv];
		col=new GLfloat[4*nv];
		tex=new GLfloat[2*nv];
	}
#endif
	int glidx = 0;
#else
	glBegin(GL_TRIANGLE_FAN);
#endif
	for (i = nv-1; i >= 0; i--) {		
		vertex * va = verts[i];
		float sx, sy, sz;
		float tu, tv;
		float rhw;
		int a;
		
		if ( gr_zbuffering || (flags & TMAP_FLAG_NEBULA) )      {
			sz = 1.0 - 1.0 / (1.0 + va->z / (32768.0 / 256.0));
			
			if ( sz > 0.98f ) {
				sz = 0.98f;
			}
		} else {
			sz = 0.99f;
		}

		if ( flags & TMAP_FLAG_CORRECT )        {
			rhw = va->sw;
		} else {
			rhw = 1.0f;
		}
		
		if (flags & TMAP_FLAG_ALPHA) {
			a = verts[i]->a;
		} else {
			a = alpha;
		}

		if (flags & TMAP_FLAG_NEBULA ) {
			int pal = (verts[i]->b*(NEBULA_COLORS-1))/255;
			r = gr_palette[pal*3+0];
			g = gr_palette[pal*3+1];
			b = gr_palette[pal*3+2];
		} else if ( (flags & TMAP_FLAG_RAMP) && (flags & TMAP_FLAG_GOURAUD) )   {
			r = Gr_gamma_lookup[verts[i]->b];
			g = Gr_gamma_lookup[verts[i]->b];
			b = Gr_gamma_lookup[verts[i]->b];
		} else if ( (flags & TMAP_FLAG_RGB)  && (flags & TMAP_FLAG_GOURAUD) )   {
			// Make 0.75 be 256.0f
			r = Gr_gamma_lookup[verts[i]->r];
			g = Gr_gamma_lookup[verts[i]->g];
			b = Gr_gamma_lookup[verts[i]->b];
		} else {
			// use constant RGB values...
		}
#ifdef HAVE_GLES
#ifdef __vita__
		col[glidx*4+0] = r; col[glidx*4+1] = g;
		col[glidx*4+2] = b; col[glidx*4+3] = a;
#else
		col[glidx*4+0] = r/255.0f; col[glidx*4+1] = g/255.0f;
		col[glidx*4+2] = b/255.0f; col[glidx*4+3] = a/255.0f;
#endif
#else
		glColor4ub (r,g,b,a);
#endif

		if((gr_screen.current_fog_mode != GR_FOGMODE_NONE) && (D3D_fog_mode == 1)){
			int sr, sg, sb, sa;
			
			/* this is for GL_EXT_SECONDARY_COLOR */
			gr_opengl_stuff_fog_value(va->z, &sr, &sg, &sb, &sa);
			/* do separate color call here */
			
			STUB_FUNCTION;
		}
		
		int x, y;
		x = fl2i(va->sx*16.0f);
		y = fl2i(va->sy*16.0f);

		x += gr_screen.offset_x*16;
		y += gr_screen.offset_y*16;
		
		sx = i2fl(x) / 16.0f;
		sy = i2fl(y) / 16.0f;

		if ( flags & TMAP_FLAG_TEXTURED )       {
			tu = va->u*u_scale;
			tv = va->v*v_scale;
#ifdef HAVE_GLES
			tex[glidx*2+0] = tu; tex[glidx*2+1] = tv;
#else
			glTexCoord2f(tu, tv);
#endif
		}
#ifdef HAVE_GLES
		vtx[glidx*4+0] = sx/rhw; vtx[glidx*4+1] = sy/rhw;
		vtx[glidx*4+2] = -sz/rhw; vtx[glidx*4+3] = 1.0/rhw;
		glidx++;
#else
		glVertex4f(sx/rhw, sy/rhw, -sz/rhw, 1.0/rhw);
#endif
	}
#ifdef HAVE_GLES
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	if ( flags & TMAP_FLAG_TEXTURED ) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#ifdef __vita__
	vglVertexPointerMapped(4, mapped_vtx_head);
	vglColorPointerMapped(GL_UNSIGNED_BYTE, mapped_clr_head);
	if ( flags & TMAP_FLAG_TEXTURED ) vglTexCoordPointerMapped(mapped_tex_head);
	vglDrawObjects(GL_TRIANGLE_FAN, glidx, GL_TRUE);
	mapped_vtx_head += 4 * glidx;
	mapped_clr_head += 4 * glidx;
	mapped_tex_head += 2 * glidx;
#else
	glVertexPointer(4, GL_FLOAT, 0, vtx);
	glColorPointer(4, GL_FLOAT, 0, col);
	if ( flags & TMAP_FLAG_TEXTURED ) glTexCoordPointer(2, GL_FLOAT, 0, tex);
	glDrawArrays(GL_TRIANGLE_FAN,0,glidx);
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	if ( flags & TMAP_FLAG_TEXTURED ) glDisableClientState(GL_TEXTURE_COORD_ARRAY);

/*	GLfloat *vtx = new GLfloat[3*nv];
	GLfloat *col = new GLfloat[4*nv];
	GLFloat *tex;
	if ( flags & TMAP_FLAG_TEXTURED ) tex = new GLfloat[2*nv];*/	
#else
	glEnd();
#endif
}

void gr_opengl_tmapper( int nverts, vertex **verts, uint flags )
{
	gr_opengl_tmapper_internal( nverts, verts, flags, 0 );
}

#define FIND_SCALED_NUM(x,x0,x1,y0,y1) (((((x)-(x0))*((y1)-(y0)))/((x1)-(x0)))+(y0))

void gr_opengl_scaler(vertex *va, vertex *vb )
{
	float x0, y0, x1, y1;
	float u0, v0, u1, v1;
	float clipped_x0, clipped_y0, clipped_x1, clipped_y1;
	float clipped_u0, clipped_v0, clipped_u1, clipped_v1;
	float xmin, xmax, ymin, ymax;
	int dx0, dy0, dx1, dy1;

	//============= CLIP IT =====================

	x0 = va->sx; y0 = va->sy;
	x1 = vb->sx; y1 = vb->sy;

	xmin = i2fl(gr_screen.clip_left); ymin = i2fl(gr_screen.clip_top);
	xmax = i2fl(gr_screen.clip_right); ymax = i2fl(gr_screen.clip_bottom);

	u0 = va->u; v0 = va->v;
	u1 = vb->u; v1 = vb->v;

	// Check for obviously offscreen bitmaps...
	if ( (y1<=y0) || (x1<=x0) ) return;
	if ( (x1<xmin ) || (x0>xmax) ) return;
	if ( (y1<ymin ) || (y0>ymax) ) return;

	clipped_u0 = u0; clipped_v0 = v0;
	clipped_u1 = u1; clipped_v1 = v1;

	clipped_x0 = x0; clipped_y0 = y0;
	clipped_x1 = x1; clipped_y1 = y1;

	// Clip the left, moving u0 right as necessary
	if ( x0 < xmin ) 	{
		clipped_u0 = FIND_SCALED_NUM(xmin,x0,x1,u0,u1);
		clipped_x0 = xmin;
	}

	// Clip the right, moving u1 left as necessary
	if ( x1 > xmax )	{
		clipped_u1 = FIND_SCALED_NUM(xmax,x0,x1,u0,u1);
		clipped_x1 = xmax;
	}

	// Clip the top, moving v0 down as necessary
	if ( y0 < ymin ) 	{
		clipped_v0 = FIND_SCALED_NUM(ymin,y0,y1,v0,v1);
		clipped_y0 = ymin;
	}

	// Clip the bottom, moving v1 up as necessary
	if ( y1 > ymax ) 	{
		clipped_v1 = FIND_SCALED_NUM(ymax,y0,y1,v0,v1);
		clipped_y1 = ymax;
	}
	
	dx0 = fl2i(clipped_x0); dx1 = fl2i(clipped_x1);
	dy0 = fl2i(clipped_y0); dy1 = fl2i(clipped_y1);

	if (dx1<=dx0) return;
	if (dy1<=dy0) return;

	//============= DRAW IT =====================

	vertex v[4];
	vertex *vl[4];

	vl[0] = &v[0];	
	v->sx = clipped_x0;
	v->sy = clipped_y0;
	v->sw = va->sw;
	v->z = va->z;
	v->u = clipped_u0;
	v->v = clipped_v0;

	vl[1] = &v[1];	
	v[1].sx = clipped_x1;
	v[1].sy = clipped_y0;
	v[1].sw = va->sw;
	v[1].z = va->z;
	v[1].u = clipped_u1;
	v[1].v = clipped_v0;

	vl[2] = &v[2];	
	v[2].sx = clipped_x1;
	v[2].sy = clipped_y1;
	v[2].sw = va->sw;
	v[2].z = va->z;
	v[2].u = clipped_u1;
	v[2].v = clipped_v1;

	vl[3] = &v[3];	
	v[3].sx = clipped_x0;
	v[3].sy = clipped_y1;
	v[3].sw = va->sw;
	v[3].z = va->z;
	v[3].u = clipped_u0;
	v[3].v = clipped_v1;

	gr_opengl_tmapper_internal( 4, vl, TMAP_FLAG_TEXTURED, 1 );
}

void gr_opengl_set_palette(ubyte *new_palette, int is_alphacolor)
{
}

void gr_opengl_get_color( int * r, int * g, int * b )
{
	if (r) *r = gr_screen.current_color.red;
	if (g) *g = gr_screen.current_color.green;
	if (b) *b = gr_screen.current_color.blue;
}

void gr_opengl_init_color(color *c, int r, int g, int b)
{
	c->screen_sig = gr_screen.signature;
	c->red = (unsigned char)r;
	c->green = (unsigned char)g;
	c->blue = (unsigned char)b;
	c->alpha = 255;
	c->ac_type = AC_TYPE_NONE;
	c->alphacolor = -1;
	c->is_alphacolor = 0;
	c->magic = 0xAC01;
}

void gr_opengl_init_alphacolor( color *clr, int r, int g, int b, int alpha, int type )
{
	if ( r < 0 ) r = 0; else if ( r > 255 ) r = 255;
	if ( g < 0 ) g = 0; else if ( g > 255 ) g = 255;
	if ( b < 0 ) b = 0; else if ( b > 255 ) b = 255;
	if ( alpha < 0 ) alpha = 0; else if ( alpha > 255 ) alpha = 255;

        gr_opengl_init_color( clr, r, g, b );

        clr->alpha = (unsigned char)alpha;
        clr->ac_type = (ubyte)type;
	clr->alphacolor = -1;
	clr->is_alphacolor = 1;
}

void gr_opengl_set_color( int r, int g, int b )
{
	Assert((r >= 0) && (r < 256));
	Assert((g >= 0) && (g < 256));
	Assert((b >= 0) && (b < 256));

	gr_opengl_init_color( &gr_screen.current_color, r, g, b );	
}

void gr_opengl_set_color_fast(color *dst)
{
	if ( dst->screen_sig != gr_screen.signature )	{
		if ( dst->is_alphacolor )       {
			gr_opengl_init_alphacolor( dst, dst->red, dst->green, dst->blue, dst->alpha, dst->ac_type );
		} else {
			gr_opengl_init_color( dst, dst->red, dst->green, dst->blue );
		}
	}
	gr_screen.current_color = *dst;
}

void gr_opengl_print_screen(const char *filename)
{
#if defined(GL_VERSION_1_2) || defined(HAVE_GLES)
	char tmp[MAX_FILENAME_LEN];
	ubyte *buf = NULL;

	strcpy( tmp, filename );
	strcat( tmp, NOX(".tga"));

	CFILE *f = cfopen(tmp, "wb", CFILE_NORMAL, CF_TYPE_ROOT);

	if (f == NULL)
		return;

	// Write the TGA header
	cfwrite_ubyte( 0, f );	//	IDLength;
	cfwrite_ubyte( 0, f );	//	ColorMapType;
	cfwrite_ubyte( 2, f );	//	ImageType;		// 2 = 24bpp, uncompressed, 10=24bpp rle compressed
	cfwrite_ushort( 0, f );	// CMapStart;
	cfwrite_ushort( 0, f );	//	CMapLength;
	cfwrite_ubyte( 0, f );	// CMapDepth;
	cfwrite_ushort( 0, f );	//	XOffset;
	cfwrite_ushort( 0, f );	//	YOffset;
	cfwrite_ushort( (ushort)gr_screen.max_w, f );	//	Width;
	cfwrite_ushort( (ushort)gr_screen.max_h, f );	//	Height;
	cfwrite_ubyte( 24, f );	//PixelDepth;
	cfwrite_ubyte( 0, f );	//ImageDesc;

	buf = (ubyte*)malloc(gr_screen.max_w * gr_screen.max_h * 3);

	if (buf == NULL)
		return;

	memset(buf, 0, gr_screen.max_w * gr_screen.max_h * 3);

	#ifdef HAVE_GLES
	int p2width = 1;
	int p2height = 1;
	while (p2width<gr_screen.max_w) p2width*=2;
	while (p2height<gr_screen.max_h) p2height*=2;
	ubyte *ptmp=(ubyte*)malloc(p2width * p2height * 4);
	glReadPixels(0, 0, p2width, p2height, GL_RGBA, GL_UNSIGNED_BYTE, ptmp);
	for (int y=0; y<gr_screen.max_h; y++)
		for (int x=0; x<gr_screen.max_w; x++)
			for (int aa=0; aa<3; aa++)
				buf[(y*gr_screen.max_w+x)*3+(3-aa)]=ptmp[(y*p2width+x)*4+aa];
	free(ptmp);
	#else
	glReadPixels(0, 0, gr_screen.max_w, gr_screen.max_h, GL_BGR, GL_UNSIGNED_BYTE, buf);
	#endif

	cfwrite(buf, gr_screen.max_w * gr_screen.max_h * 3, 1, f);

	cfclose(f);

	free(buf);
#endif
}

int gr_opengl_supports_res_ingame(int res)
{
	STUB_FUNCTION;
	
	return 1;
}

int gr_opengl_supports_res_interface(int res)
{
	STUB_FUNCTION;
	
	return 1;
}

static void opengl_tcache_cleanup ();

void gr_opengl_cleanup()
{
	if ( !Inited )	return;


	gr_reset_clip();
	gr_clear();
	gr_flip();

	opengl_tcache_cleanup ();

	Inited = 0;
}

void gr_opengl_fog_set(int fog_mode, int r, int g, int b, float fog_near, float fog_far)
{
	Assert((r >= 0) && (r < 256));
	Assert((g >= 0) && (g < 256));
	Assert((b >= 0) && (b < 256));
	
	if (fog_mode == GR_FOGMODE_NONE) {
		if (gr_screen.current_fog_mode != fog_mode) {
			glDisable(GL_FOG);
		}
		gr_screen.current_fog_mode = fog_mode;
		
		return;
	}
	
	if (gr_screen.current_fog_mode != fog_mode) {
		glEnable(GL_FOG);
		
		if (D3D_fog_mode == 2) {
			glFogi(GL_FOG_MODE, GL_LINEAR);
		}
		
		gr_screen.current_fog_mode = fog_mode;
	}
	
	if ( (gr_screen.current_fog_color.red != r) ||
			(gr_screen.current_fog_color.green != g) ||
			(gr_screen.current_fog_color.blue != b) ) {
		GLfloat fc[4];
		
		gr_opengl_init_color( &gr_screen.current_fog_color, r, g, b );
	
		fc[0] = (float)r/255.0;
		fc[1] = (float)g/255.0;
		fc[2] = (float)b/255.0;
		fc[3] = 1.0;
		
		glFogfv(GL_FOG_COLOR, fc);
	}
	
	if( (fog_near >= 0.0f) && (fog_far >= 0.0f) && 
			((fog_near != gr_screen.fog_near) || 
			(fog_far != gr_screen.fog_far)) ) {
		gr_screen.fog_near = fog_near;
		gr_screen.fog_far = fog_far;
		
		if (D3D_fog_mode == 2) {
			glFogf(GL_FOG_START, fog_near);
			glFogf(GL_FOG_END, fog_far);
		}
	}
}

void gr_opengl_get_pixel(int x, int y, int *r, int *g, int *b)
{
	// Not used.
}

void gr_opengl_set_cull(int cull)
{
	if (cull) {
		glEnable (GL_CULL_FACE);
		glFrontFace (GL_CCW);
	} else {
		glDisable (GL_CULL_FACE);
	}
}

void gr_opengl_filter_set(int filter)
{
}

// cross fade
void gr_opengl_cross_fade(int bmap1, int bmap2, int x1, int y1, int x2, int y2, float pct)
{
	gr_set_bitmap(bmap1, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, 1.0f - pct );
	gr_bitmap(x1, y1);

	gr_set_bitmap(bmap2, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, pct );
	gr_bitmap(x2, y2);	
}


typedef struct tcache_slot_opengl {
	GLuint	texture_handle;
	float	u_scale, v_scale;
	int	bitmap_id;
	int	size;
	int	used_this_frame;
	int	time_created;
	ushort	w,h;

	// sections
	tcache_slot_opengl	*data_sections[MAX_BMAP_SECTIONS_X][MAX_BMAP_SECTIONS_Y];
	tcache_slot_opengl	*parent;
	
	gr_texture_source	texture_mode;
} tcache_slot_opengl;

static void *Texture_sections = NULL;
static tcache_slot_opengl *Textures = NULL;

static tcache_slot_opengl *GL_bound_texture;

int GL_texture_sections = 0;
int GL_texture_ram = 0;
int GL_frame_count = 0;
int GL_min_texture_width = 0;
int GL_max_texture_width = 0;
int GL_min_texture_height = 0;
int GL_max_texture_height = 0;
int GL_square_textures = 0;
int GL_textures_in = 0;
int GL_textures_in_frame = 0;
int GL_last_bitmap_id = -1;
int GL_last_detail = -1;
int GL_last_bitmap_type = -1;
int GL_last_section_x = -1;
int GL_last_section_y = -1;

int vram_full = 0;

static gr_texture_source GL_current_texture_source = (gr_texture_source) -1;

static void gr_opengl_set_texture_state(gr_texture_source ts)
{
	if (ts == TEXTURE_SOURCE_NONE) {
		GL_bound_texture = NULL;
				
		glBindTexture(GL_TEXTURE_2D, 0);
		gr_tcache_set(-1, -1, NULL, NULL, 0, -1, -1, 0 );
	} else if (GL_bound_texture &&
		GL_bound_texture->texture_mode != ts) {
		switch (ts) {
			case TEXTURE_SOURCE_DECAL:
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case TEXTURE_SOURCE_NO_FILTERING:
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			default:
				break;
		}
			
		GL_bound_texture->texture_mode = ts;
	}
	
	GL_current_texture_source = ts;
}

static int opengl_max_tex_size_get()
{
	GLint max_texture_size = 0;

	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &max_texture_size );
	return max_texture_size;
}

static void opengl_tcache_init (int use_sections)
{
	int i, idx, s_idx;

	// DDOI - FIXME skipped a lot of stuff here
	GL_should_preload = 0;

	//uint tmp_pl = os_config_read_uint( NULL, NOX("D3DPreloadTextures"), 255 );
	uint tmp_pl = 1;

	if ( tmp_pl == 0 )      {
		GL_should_preload = 0;
	} else if ( tmp_pl == 1 )       {
		GL_should_preload = 1;
	} else {
		GL_should_preload = 1;
	}

	STUB_FUNCTION;

	GL_min_texture_width = 16;
	GL_min_texture_height = 16;

#ifdef HAVE_GLES
	GL_max_texture_width = 1024;
	GL_max_texture_height = 1024;
#else
	GL_max_texture_width = opengl_max_tex_size_get();
	GL_max_texture_height = opengl_max_tex_size_get();
#endif

	Textures = (tcache_slot_opengl *)malloc(MAX_BITMAPS*sizeof(tcache_slot_opengl));
	if ( !Textures )        {
		exit(1);
	}

	if(use_sections){
		Texture_sections = (tcache_slot_opengl*)malloc(MAX_BITMAPS * MAX_BMAP_SECTIONS_X * MAX_BMAP_SECTIONS_Y * sizeof(tcache_slot_opengl));
		if(!Texture_sections){
			exit(1);
		}
		memset(Texture_sections, 0, MAX_BITMAPS * MAX_BMAP_SECTIONS_X * MAX_BMAP_SECTIONS_Y * sizeof(tcache_slot_opengl));
	}

	// Init the texture structures
	int section_count = 0;
	for( i=0; i<MAX_BITMAPS; i++ )  {
		/*
		Textures[i].vram_texture = NULL;
		Textures[i].vram_texture_surface = NULL;
		*/
		Textures[i].texture_handle = 0;

		Textures[i].bitmap_id = -1;
		Textures[i].size = 0;
		Textures[i].used_this_frame = 0;

		Textures[i].parent = NULL;

		// allocate sections
		if(use_sections){
			for(idx=0; idx<MAX_BMAP_SECTIONS_X; idx++){
				for(s_idx=0; s_idx<MAX_BMAP_SECTIONS_Y; s_idx++){
					Textures[i].data_sections[idx][s_idx] = &((tcache_slot_opengl*)Texture_sections)[section_count++];
					Textures[i].data_sections[idx][s_idx]->parent = &Textures[i];
					/*
					Textures[i].data_sections[idx][s_idx]->vram_texture = NULL;
					Textures[i].data_sections[idx][s_idx]->vram_texture_surface = NULL;
					*/
					Textures[i].data_sections[idx][s_idx]->texture_handle = 0;
					Textures[i].data_sections[idx][s_idx]->bitmap_id = -1;
					Textures[i].data_sections[idx][s_idx]->size = 0;
					Textures[i].data_sections[idx][s_idx]->used_this_frame = 0;
				}
			}
		} else {
			for(idx=0; idx<MAX_BMAP_SECTIONS_X; idx++){
				for(s_idx=0; s_idx<MAX_BMAP_SECTIONS_Y; s_idx++){
					Textures[i].data_sections[idx][s_idx] = NULL;
				}
			}
		}
	}

	GL_texture_sections = use_sections;

	GL_last_detail = Detail.hardware_textures;
	GL_last_bitmap_id = -1;
	GL_last_bitmap_type = -1;

	GL_last_section_x = -1;
	GL_last_section_y = -1;

	GL_textures_in = 0;
	GL_textures_in_frame = 0;
}

static int opengl_free_texture (tcache_slot_opengl *t);

void opengl_free_texture_with_handle(int handle)
{
	for(int i=0; i<MAX_BITMAPS; i++ )  {
		if (Textures[i].bitmap_id == handle) {
			Textures[i].used_this_frame = 0; // this bmp doesn't even exist any longer...
			opengl_free_texture ( &Textures[i] );
		}
	}
}

static void opengl_tcache_flush ()
{
	int i;

	for( i=0; i<MAX_BITMAPS; i++ )  {
		opengl_free_texture ( &Textures[i] );
	}
	if (GL_textures_in != 0) {
		mprintf(( "WARNING: VRAM is at %d instead of zero after flushing!\n", GL_textures_in ));
		GL_textures_in = 0;
	}

	GL_last_bitmap_id = -1;
	GL_last_section_x = -1;
	GL_last_section_y = -1;
}

static void opengl_tcache_cleanup ()
{
	opengl_tcache_flush ();

	GL_textures_in = 0;
	GL_textures_in_frame = 0;

	if ( Textures ) {
		free(Textures);
		Textures = NULL;
	}

	if( Texture_sections != NULL ){
		free(Texture_sections);
		Texture_sections = NULL;
	}
}

static void opengl_tcache_frame ()
{
	GL_last_bitmap_id = -1;
	GL_textures_in_frame = 0;

	GL_frame_count++;

	/*
	int idx, s_idx;
	int i;
	for( i=0; i<MAX_BITMAPS; i++ )  {
		Textures[i].used_this_frame = 0;

		// data sections
		if(Textures[i].data_sections[0][0] != NULL){
			Assert(GL_texture_sections);
			if(GL_texture_sections){
				for(idx=0; idx<MAX_BMAP_SECTIONS_X; idx++){
					for(s_idx=0; s_idx<MAX_BMAP_SECTIONS_Y; s_idx++){
						if(Textures[i].data_sections[idx][s_idx] != NULL){
							Textures[i].data_sections[idx][s_idx]->used_this_frame = 0;
						}
					}
				}
			}
		}
	}
	*/

	if ( vram_full )        {
		opengl_tcache_flush();
		vram_full = 0;
	}
}

static int opengl_free_texture ( tcache_slot_opengl *t )
{
	int idx, s_idx;
	

	// Bitmap changed!!     
	if ( t->bitmap_id > -1 )        {
		// if I, or any of my children have been used this frame, bail  
		if(t->used_this_frame == GL_frame_count){
			return 0;
		}
		for(idx=0; idx<MAX_BMAP_SECTIONS_X; idx++){
			for(s_idx=0; s_idx<MAX_BMAP_SECTIONS_Y; s_idx++){
				if((t->data_sections[idx][s_idx] != NULL) && (t->data_sections[idx][s_idx]->used_this_frame == GL_frame_count)){
					return 0;
				}
			}
		}

		// ok, now we know its legal to free everything safely
		t->texture_mode = (gr_texture_source) -1;
		glDeleteTextures (1, &t->texture_handle);
		t->texture_handle = 0;

		if ( GL_last_bitmap_id == t->bitmap_id )       {
			GL_last_bitmap_id = -1;
		}

		// if this guy has children, free them too, since the children
		// actually make up his size
		for(idx=0; idx<MAX_BMAP_SECTIONS_X; idx++){
			for(s_idx=0; s_idx<MAX_BMAP_SECTIONS_Y; s_idx++){
				if(t->data_sections[idx][s_idx] != NULL){
					opengl_free_texture(t->data_sections[idx][s_idx]);
				}
			}
		}

		t->bitmap_id = -1;
		t->used_this_frame = 0;
		GL_textures_in -= t->size;
		t->size = 0;
	}

	return 1;
}

static void opengl_tcache_get_adjusted_texture_size(int w_in, int h_in, int *w_out, int *h_out)
{
	int tex_w, tex_h;
	int i;
	
	// bogus
	if((w_out == NULL) ||  (h_out == NULL)){
		return;
	}

	// starting size
	tex_w = w_in;
	tex_h = h_in;

	// set height and width to a power of 2
	for (i=0; i<16; i++ )	{
		if ( (tex_w > (1<<i)) && (tex_w <= (1<<(i+1))) )	{
			tex_w = 1 << (i+1);
			break;
		}
	}

	for (i=0; i<16; i++ )	{
		if ( (tex_h > (1<<i)) && (tex_h <= (1<<(i+1))) )	{
			tex_h = 1 << (i+1);
			break;
		}
	}

	// try to keep an 8:1 size ratio
	if (tex_w/tex_h > 8)
		tex_h = tex_w/8;
	if (tex_h/tex_w > 8)
		tex_w = tex_h/8;

	if ( tex_w < GL_min_texture_width ) {
		tex_w = GL_min_texture_width;
	} else if ( tex_w > GL_max_texture_width )     {
		tex_w = GL_max_texture_width;
	}

	if ( tex_h < GL_min_texture_height ) {
		tex_h = GL_min_texture_height;
	} else if ( tex_h > GL_max_texture_height )    {
		tex_h = GL_max_texture_height;
	}

	if ( GL_square_textures) {
		int new_size;
		// Make the both be equal to larger of the two
		new_size = max(tex_w, tex_h);
		tex_w = new_size;
		tex_h = new_size;
	}

	// store the outgoing size
	*w_out = tex_w;
	*h_out = tex_h;
}

// data == start of bitmap data
// sx == x offset into bitmap
// sy == y offset into bitmap
// src_w == absolute width of section on source bitmap
// src_h == absolute height of section on source bitmap
// bmap_w == width of source bitmap
// bmap_h == height of source bitmap
// tex_w == width of final texture
// tex_h == height of final texture
static int opengl_create_texture_sub(int bitmap_type, int texture_handle, ushort *data, int sx, int sy, int src_w, int src_h, int bmap_w, int bmap_h, int tex_w, int tex_h, tcache_slot_opengl *t, int reload, int resize, int fail_on_full)
{
	int ret_val = 1;
	int size;
	int i, j;
	ubyte *bmp_data = ((ubyte*)data);
#ifdef HAVE_GLES
	unsigned short *wtexmemp = NULL, wtexmem;
#endif
	ubyte *texmem = NULL, *texmemp;
	
	// bogus
	if((t == NULL) || (bmp_data == NULL)){
		return 0;
	}

	if ( t->used_this_frame == GL_frame_count )       {
		mprintf(( "ARGHH!!! Texture already used this frame!  Cannot free it!\n" ));
		return 0;
	}

	if ( !reload )  {
		// gah
		if(!opengl_free_texture(t)){
			return 0;
		}
	}

	if ( bitmap_type == TCACHE_TYPE_AABITMAP )      {
		t->u_scale = (float)bmap_w / (float)tex_w;
		t->v_scale = (float)bmap_h / (float)tex_h;
	} else if(bitmap_type == TCACHE_TYPE_BITMAP_SECTION){
		t->u_scale = (float)src_w / (float)tex_w;
		t->v_scale = (float)src_h / (float)tex_h;
	} else {
		t->u_scale = 1.0f;
		t->v_scale = 1.0f;
	}

	if (!reload) {
		glGenTextures (1, &t->texture_handle);
	}

	if (t->texture_handle == 0) {
		nprintf(("Error", "!!DEBUG!! t->texture_handle == 0"));
		return 0;
	}
		
	GL_bound_texture = t;
	
	GL_bound_texture->texture_mode = (gr_texture_source) -1;
	
	glBindTexture (GL_TEXTURE_2D, t->texture_handle);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/* this should be set next anyway */	
//	if (GL_current_texture_source != TEXTURE_SOURCE_NONE) {
//		gr_opengl_set_texture_state(GL_current_texture_source);
//	}

	size = 0;
		
	switch (bitmap_type) {

		case TCACHE_TYPE_AABITMAP:
			{
				texmem = (ubyte *) malloc (tex_w*tex_h);
				texmemp = texmem;

				for (i=0;i<tex_h;i++)
				{
					for (j=0;j<tex_w;j++)
					{
						if (i < bmap_h && j < bmap_w) {
							*texmemp++ = GL_xlat[bmp_data[i*bmap_w+j]];
						} else {
							*texmemp++ = 0;
						}
					}
				}

				size = tex_w*tex_h;
#ifndef __vita__
				if (!reload) {
#endif
					glTexImage2D (GL_TEXTURE_2D, 0, GL_ALPHA, tex_w, tex_h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, texmem);
#ifndef __vita__
				} else {
					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_ALPHA, GL_UNSIGNED_BYTE, texmem);
				}
#endif
				free (texmem);

				break;
			}

		case TCACHE_TYPE_BITMAP_SECTION:
			{
				// if we aren't resizing in any way then we can just use bmp_data directly
#ifdef HAVE_GLES
				resize=1;
#endif
//				wtexmemp = (unsigned short *)texmem;
//				#else
				if ( resize ) {
//				#endif
					texmem = (ubyte *) malloc (tex_w*tex_h*2);
					texmemp = texmem;
				
					for (i=0;i<tex_h;i++) {
						for (j=0;j<tex_w;j++) {
							if (i < src_h && j < src_w) {
								*texmemp++ = bmp_data[((i+sy)*bmap_w+(j+sx))*2+0];
								*texmemp++ = bmp_data[((i+sy)*bmap_w+(j+sx))*2+1];
#ifdef HAVE_GLES
								// invert 1555/BGRA to 5551/RGBA (0x1f / 0x3e0 / 7c00)
								wtexmemp=(unsigned short*)texmemp-1;
								wtexmem=*(wtexmemp);
								*wtexmemp = ((wtexmem&0x8000)>>15) | ((wtexmem&0x7fff)<<1);
//								*wtexmemp = ((wtexmem&0x8000)>>15) | ((wtexmem&0x7c00)>>9) | ((wtexmem&0x03e0)<<1) | ((wtexmem&0x001f)<<11);
#endif
							} else {
								*texmemp++ = 0;
								*texmemp++ = 0;
#ifdef HAVE_GLES
//								wtexmemp++;
#endif
							}
						}
					}
//				#ifndef HAVE_GLES
				}
//				#endif
				
				size = tex_w*tex_h*2;
#ifndef __vita__
				if (!reload) {
#endif
#ifdef HAVE_GLES
//					glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, texmem);
					glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, (resize) ? texmem : bmp_data);
#else
					glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB5_A1, tex_w, tex_h, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, (resize) ? texmem : bmp_data);
#endif
#ifndef __vita__
				} else {
#ifdef HAVE_GLES
					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, (resize) ? texmem : bmp_data);
//					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, texmem);
#else
					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, (resize) ? texmem : bmp_data);
#endif
				}
#endif
				if (texmem != NULL)
					free(texmem);

				break;
			}

		default:
			{
				// if we aren't resizing then we can just use bmp_data directly
#ifdef HAVE_GLES
				resize=1;
#endif
//				wtexmemp = (unsigned short *)texmem;
//				#else
				if ( resize ) {
//				#endif
					texmem = (ubyte *) malloc (tex_w*tex_h*2);
					texmemp = texmem;

					Assert( texmem != NULL );

					fix u, utmp, v, du, dv;
				
					u = v = 0;
				
					du = ( (bmap_w-1)*F1_0 ) / tex_w;
					dv = ( (bmap_h-1)*F1_0 ) / tex_h;
				
					for (j=0;j<tex_h;j++) {
						utmp = u;
						for (i=0;i<tex_w;i++) {
							*texmemp++ = bmp_data[(f2i(v)*bmap_w+f2i(utmp))*2+0];
							*texmemp++ = bmp_data[(f2i(v)*bmap_w+f2i(utmp))*2+1];
							utmp += du;
#ifdef HAVE_GLES
							// invert 5551/BGRA to 1555/ARGB (0x1f / 0x3e0 / 7c00)
							wtexmemp=(unsigned short*)texmemp-1;
							wtexmem=*(wtexmemp);
							*wtexmemp = ((wtexmem&0x8000)>>15) | ((wtexmem&0x7fff)<<1);
//							*wtexmemp = ((wtexmem&0x8000)>>15) | ((wtexmem&0x7c00)>>9) | ((wtexmem&0x03e0)<<1) | ((wtexmem&0x001f)<<11);
#endif
						}
						v += dv;
					}
//				#ifndef HAVE_GLES
				}
//				#endif

				size = tex_w*tex_h*2;
#ifndef __vita__		
				if (!reload) {
#endif
#ifdef HAVE_GLES
//					glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, texmem);
					glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, (resize) ? texmem : bmp_data);
#else
					glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB5_A1, tex_w, tex_h, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, (resize) ? texmem : bmp_data);
#endif
#ifndef __vita__
				} else {
#ifdef HAVE_GLES
//					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, texmem);
					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, (resize) ? texmem : bmp_data);
#else
					glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, (resize) ? texmem : bmp_data);
#endif
				}
#endif
				if (texmem != NULL)
					free(texmem);

				break;
			}

		break;
	}
	
	t->bitmap_id = texture_handle;
	t->time_created = GL_frame_count;
	t->used_this_frame = 0;
	t->size = size;
	t->w = (ushort)tex_w;
	t->h = (ushort)tex_h;
	
	GL_textures_in_frame += t->size;
	if (!reload) {
		GL_textures_in += t->size;
	}

	return ret_val;
}

static int opengl_create_texture (int bitmap_handle, int bitmap_type, tcache_slot_opengl *tslot, int fail_on_full)
{
	ubyte flags;
	bitmap *bmp;
	int final_w, final_h;
	ubyte bpp = 16;
	int reload = 0;
	int resize = 0;

	// setup texture/bitmap flags
	flags = 0;
	switch(bitmap_type){
		case TCACHE_TYPE_AABITMAP:
			flags |= BMP_AABITMAP;
			bpp = 8;
			break;
		case TCACHE_TYPE_NORMAL:
			flags |= BMP_TEX_OTHER;
		case TCACHE_TYPE_XPARENT:
			flags |= BMP_TEX_XPARENT;
			break;
		case TCACHE_TYPE_NONDARKENING:
			Int3();
			flags |= BMP_TEX_NONDARK;
			break;
	}

	// lock the bitmap into the proper format
	bmp = bm_lock(bitmap_handle, bpp, flags);
	if ( bmp == NULL ) {
		mprintf(("Couldn't lock bitmap %d.\n", bitmap_handle ));
		return 0;
	}

	int max_w = bmp->w;
	int max_h = bmp->h;

	
	   // DDOI - TODO
	if ( bitmap_type != TCACHE_TYPE_AABITMAP )      {
		// max_w /= D3D_texture_divider;
		// max_h /= D3D_texture_divider;

		// if we are going to cull the size then we need to force a resize
		if (Detail.hardware_textures < 4) {
			resize = 1;
		}

		// Detail.debris_culling goes from 0 to 4.
		max_w /= (16 >> Detail.hardware_textures);
		max_h /= (16 >> Detail.hardware_textures);
	}
	

	// get final texture size as it will be allocated as a DD surface
	opengl_tcache_get_adjusted_texture_size(max_w, max_h, &final_w, &final_h); 

	if ( (final_w < 1) || (final_h < 1) ) {
		mprintf(("Bitmap is too small at %dx%d.\n", final_w, final_h));
		return 0;
	}

	// if we don't have to resize the image (to get power of 2, etc.) then skip that extra work
	if ( (max_w != final_w) || (max_h != final_h) ) {
		resize = 1;
	}

	// if this tcache slot has no bitmap
	if ( tslot->bitmap_id < 0) {
		reload = 0;
	}
	// different bitmap altogether - determine if the new one can use the old one's slot
	else if (tslot->bitmap_id != bitmap_handle)     {
		if((final_w == tslot->w) && (final_h == tslot->h)){
			reload = 1;
			//ml_printf("Reloading texture %d\n", bitmap_handle);
		} else {
			reload = 0;
		}
	}

	// call the helper
	int ret_val = opengl_create_texture_sub(bitmap_type, bitmap_handle, (ushort*)bmp->data, 0, 0, bmp->w, bmp->h, bmp->w, bmp->h, final_w, final_h, tslot, reload, resize, fail_on_full);

	// unlock the bitmap
	bm_unlock(bitmap_handle);

	return ret_val;
}

static int opengl_create_texture_sectioned(int bitmap_handle, int bitmap_type, tcache_slot_opengl *tslot, int sx, int sy, int fail_on_full)
{
	ubyte flags;
	bitmap *bmp;
	int final_w, final_h;
	int section_x, section_y;
	int reload = 0;
	int resize = 1;

	// setup texture/bitmap flags
	Assert(bitmap_type == TCACHE_TYPE_BITMAP_SECTION);
	if(bitmap_type != TCACHE_TYPE_BITMAP_SECTION){
		bitmap_type = TCACHE_TYPE_BITMAP_SECTION;
	}
	flags = BMP_TEX_XPARENT;

	// lock the bitmap in the proper format
	bmp = bm_lock(bitmap_handle, 16, flags);
	if ( bmp == NULL ) {
		mprintf(("Couldn't lock bitmap %d.\n", bitmap_handle ));
		return 0;
	}
	// determine the width and height of this section
	bm_get_section_size(bitmap_handle, sx, sy, &section_x, &section_y);

	// get final texture size as it will be allocated as an opengl texture
	opengl_tcache_get_adjusted_texture_size(section_x, section_y, &final_w, &final_h);

	if ( (final_w < 1) || (final_h < 1) ) {
		mprintf(("Bitmap is too small at %dx%d.\n", final_w, final_h));
		return 0;
	}

	// if we don't have to resize the image (to get power of 2, etc.) then skip that extra work
	if ( (bmp->sections.num_x == 1) && (bmp->sections.num_y == 1) && (section_x == final_w) && (section_y == final_h) ) {
		resize = 0;
	}

	// if this tcache slot has no bitmap
	if ( tslot->bitmap_id < 0) {
		reload = 0;
	}
	// different bitmap altogether - determine if the new one can use the old one's slot
	else if (tslot->bitmap_id != bitmap_handle)     {
		if((final_w == tslot->w) && (final_h == tslot->h)){
			reload = 1;
		} else {
			reload = 0;
		}
	}

	// call the helper
	int ret_val = opengl_create_texture_sub(bitmap_type, bitmap_handle, (ushort*)bmp->data, bmp->sections.sx[sx], bmp->sections.sy[sy], section_x, section_y, bmp->w, bmp->h, final_w, final_h, tslot, reload, resize, fail_on_full);

	// unlock the bitmap
	bm_unlock(bitmap_handle);

	return ret_val;
}

		
extern int bm_get_cache_slot( int bitmap_id, int separate_ani_frames );
static int gr_opengl_tcache_set(int bitmap_id, int bitmap_type, float *u_scale, float *v_scale, int fail_on_full = 0, int sx = -1, int sy = -1, int force = 0)
{
	bitmap *bmp = NULL;

	int idx, s_idx;
	int ret_val = 1;

	if (bitmap_id < 0)
	{
		GL_last_bitmap_id = -1;
		return 0;
	}

	if ( GL_last_detail != Detail.hardware_textures )      {
		GL_last_detail = Detail.hardware_textures;
		opengl_tcache_flush();
	}

	if (vram_full) {
		return 0;
	}

	int n = bm_get_cache_slot (bitmap_id, 1);
	tcache_slot_opengl *t = &Textures[n];

	if ( (GL_last_bitmap_id == bitmap_id) && (GL_last_bitmap_type==bitmap_type) && (t->bitmap_id == bitmap_id) && (GL_last_section_x == sx) && (GL_last_section_y == sy))       {
		t->used_this_frame = GL_frame_count;

		// mark all children as used
		if(GL_texture_sections){
			for(idx=0; idx<MAX_BMAP_SECTIONS_X; idx++){
				for(s_idx=0; s_idx<MAX_BMAP_SECTIONS_Y; s_idx++){
					if(t->data_sections[idx][s_idx] != NULL){
						t->data_sections[idx][s_idx]->used_this_frame = GL_frame_count;
					}
				}
			}
		}

		*u_scale = t->u_scale;
		*v_scale = t->v_scale;
		return 1;
	}

	if (bitmap_type == TCACHE_TYPE_BITMAP_SECTION){
		Assert((sx >= 0) && (sy >= 0) && (sx < MAX_BMAP_SECTIONS_X) && (sy < MAX_BMAP_SECTIONS_Y));
		if(!((sx >= 0) && (sy >= 0) && (sx < MAX_BMAP_SECTIONS_X) && (sy < MAX_BMAP_SECTIONS_Y))){
			return 0;
		}

		ret_val = 1;

		// if the texture sections haven't been created yet
		if((t->bitmap_id < 0) || (t->bitmap_id != bitmap_id)){

			// lock the bitmap in the proper format
			bmp = bm_lock(bitmap_id, 16, BMP_TEX_XPARENT);
			bm_unlock(bitmap_id);

			// now lets do something for each texture

			for(idx=0; idx<bmp->sections.num_x; idx++){
				for(s_idx=0; s_idx<bmp->sections.num_y; s_idx++){
					// hmm. i'd rather we didn't have to do it this way...
					if(!opengl_create_texture_sectioned(bitmap_id, bitmap_type, t->data_sections[idx][s_idx], idx, s_idx, fail_on_full)){
						ret_val = 0;
					}

					// not used this frame
					t->data_sections[idx][s_idx]->used_this_frame = 0;
				}
			}

			// zero out pretty much everything in the parent struct since he's just the root
			t->bitmap_id = bitmap_id;
			t->texture_handle = 0;
			t->time_created = t->data_sections[sx][sy]->time_created;
			t->used_this_frame = 0;
			/*
			t->vram_texture = NULL;
			t->vram_texture_surface = NULL
			*/
		}

		// argh. we failed to upload. free anything we can
		if(!ret_val){
			opengl_free_texture(t);
		}
		// swap in the texture we want
		else {
			t = t->data_sections[sx][sy];
		}
	}
	// all other "normal" textures
	else if((bitmap_id < 0) || (bitmap_id != t->bitmap_id)){
		ret_val = opengl_create_texture( bitmap_id, bitmap_type, t, fail_on_full );
	}

	// everything went ok
	if(ret_val && (t->texture_handle) && !vram_full){
		*u_scale = t->u_scale;
		*v_scale = t->v_scale;

		GL_bound_texture = t;
		
		glBindTexture (GL_TEXTURE_2D, t->texture_handle );

		/* this should be set next anyway */
//		if (GL_current_texture_source != TEXTURE_SOURCE_NONE) {
//			gr_opengl_set_texture_state(GL_current_texture_source);
//		}
		
		GL_last_bitmap_id = t->bitmap_id;
		GL_last_bitmap_type = bitmap_type;
		GL_last_section_x = sx;
		GL_last_section_y = sy;

		t->used_this_frame = GL_frame_count;
	}
	// gah
	else {
		GL_last_bitmap_id = -1;
		GL_last_bitmap_type = -1;

		GL_last_section_x = -1;
		GL_last_section_y = -1;

		GL_bound_texture = NULL;
		
		glBindTexture (GL_TEXTURE_2D, 0);	// test - DDOI
		return 0;
	}

	return 1;
}

void gr_opengl_set_clear_color(int r, int g, int b)
{
	gr_init_color (&gr_screen.current_clear_color, r, g, b);
}

void gr_opengl_flash(int r, int g, int b)
{
	CAP(r,0,255);
	CAP(g,0,255);
	CAP(b,0,255);
	
	if ( r || g || b ) {
		gr_opengl_set_state( TEXTURE_SOURCE_NONE, ALPHA_BLEND_ALPHA_ADDITIVE, ZBUFFER_TYPE_NONE );
		
		float x1, x2, y1, y2;
		x1 = i2fl(gr_screen.clip_left+gr_screen.offset_x);
		y1 = i2fl(gr_screen.clip_top+gr_screen.offset_y);
		x2 = i2fl(gr_screen.clip_right+gr_screen.offset_x);
		y2 = i2fl(gr_screen.clip_bottom+gr_screen.offset_y);
		
		glColor4ub(r, g, b, 255);
#ifdef HAVE_GLES
		glEnableClientState(GL_VERTEX_ARRAY);
#ifdef __vita__
		mapped_vtx_head[0] = mapped_vtx_head[9] = x1;
		mapped_vtx_head[3] = mapped_vtx_head[6] = x2;
		mapped_vtx_head[7] = mapped_vtx_head[10] = y1;
		mapped_vtx_head[1] = mapped_vtx_head[4] = y2;
		mapped_vtx_head[2] = mapped_vtx_head[5] = mapped_vtx_head[8] = mapped_vtx_head[11] = -0.99;
		vglVertexPointerMapped(3, mapped_vtx_head);
		vglDrawObjects(GL_TRIANGLE_FAN, 4, GL_TRUE);
		mapped_vtx_head += 12;
#else
		GLfloat vtx1[] = {
		 x1, y2, -0.99,
		 x2, y2, -0.99,
		 x2, y1, -0.99,
		 x1, y1, -0.99
		};

		glVertexPointer(3, GL_FLOAT, 0, vtx1);
		glDrawArrays(GL_TRIANGLE_FAN,0,4);
#endif
		glDisableClientState(GL_VERTEX_ARRAY);
#else
		glBegin (GL_QUADS);
		  glVertex3f (x1, y2, -0.99);

		  glVertex3f (x2, y2, -0.99);

		  glVertex3f (x2, y1, -0.99);

		  glVertex3f (x1, y1, -0.99);
		glEnd ();	  
#endif
	}
}

int gr_opengl_zbuffer_get()
{
	if ( !gr_global_zbuffering )    {
		return GR_ZBUFF_NONE;
	}
	return gr_zbuffering_mode;
}

int gr_opengl_zbuffer_set(int mode)
{
	int tmp = gr_zbuffering_mode;

	gr_zbuffering_mode = mode;

	if (gr_zbuffering_mode == GR_ZBUFF_NONE )      {
		gr_zbuffering = 0;
	} else {
		gr_zbuffering = 1;
	}
	return tmp;
}

void gr_opengl_zbuffer_clear(int mode)
{
	if (mode) {
		gr_zbuffering = 1;
		gr_zbuffering_mode = GR_ZBUFF_FULL;
		gr_global_zbuffering = 1;
		
		gr_opengl_set_state( TEXTURE_SOURCE_NONE, ALPHA_BLEND_NONE, ZBUFFER_TYPE_FULL );
		glClear ( GL_DEPTH_BUFFER_BIT );
	} else {
		gr_zbuffering = 0;
		gr_zbuffering_mode = GR_ZBUFF_NONE;
		gr_global_zbuffering = 0;
	}
}

void gr_opengl_set_gamma(float gamma)
{
	Gr_gamma = gamma;
	Gr_gamma_int = int (Gr_gamma*10);

	// Create the Gamma lookup table
	int i;
	for (i=0;i<256; i++) {
		int v = fl2i(pow(i2fl(i)/255.0f, 1.0f/Gr_gamma)*255.0f);
		if ( v > 255 ) {
			v = 255;
		} else if ( v < 0 )     {
			v = 0;
		}
		Gr_gamma_lookup[i] = v;
	}

	// set the alpha gamma settings (for fonts)
	for (i=0; i<16; i++) {
		GL_xlat[i] = (ubyte)Gr_gamma_lookup[(i*255)/15];
	}

	GL_xlat[15] = GL_xlat[1];

	for (; i<256; i++) {
		GL_xlat[i] = GL_xlat[0];
	}

	// Flush any existing textures
	opengl_tcache_flush();
}

void gr_opengl_fade_in(int instantaneous)
{
	// Empty - DDOI
}

void gr_opengl_fade_out(int instantaneous)
{
	// Empty - DDOI
}

void gr_opengl_get_region(int front, int w, int h, ubyte *data)
{
#if !defined(HAVE_GLES)
	if (front) {
		glReadBuffer(GL_FRONT);
	} else {
		glReadBuffer(GL_BACK);
	}
#endif
	
	gr_opengl_set_state(TEXTURE_SOURCE_NO_FILTERING, ALPHA_BLEND_NONE, ZBUFFER_TYPE_NONE);
#if !defined(HAVE_GLES)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, gr_screen.max_w);
#endif
	
#if defined(HAVE_GLES)
		// on GLES, just grab everything, in RGBA format, and then copy the required datas
	int p2width = 1;
	int p2height = 1;
	while (p2width<w) p2width*=2;
	while (p2height<h) p2height*=2;
	ubyte *ptmp=(ubyte*)malloc(p2width * p2height * 4);
	glReadPixels(0, 0, p2width, p2height, GL_RGBA, GL_UNSIGNED_BYTE, ptmp);
	ubyte r,g,b,a;
	for (int y=gr_screen.max_h-h; y<h; y++)
		for (int x=0; x<w; x++) {
			r=ptmp[(y*p2width+x)*4+0];
			g=ptmp[(y*p2width+x)*4+1];
			b=ptmp[(y*p2width+x)*4+2];
			a=ptmp[(y*p2width+x)*4+3];
			if (gr_screen.bits_per_pixel == 15) {
				data[(y*w+x)*2+0] = (r>>3)<<3 | (g>>3)>>2;
				data[(y*w+x)*2+1] = ((g>>3)&3)<<6 | (b>>3)<<1 | (a)?1:0;
			} else if (gr_screen.bits_per_pixel == 32) {
				data[(y*w+x)*4+0] = r;
				data[(y*w+x)*4+0] = g;
				data[(y*w+x)*4+0] = b;
				data[(y*w+x)*4+0] = a;
			}
		}
	free(ptmp);
#else
	if (gr_screen.bits_per_pixel == 15) {
		glReadPixels(0, gr_screen.max_h-h-1, w, h, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, data);
	} else if (gr_screen.bits_per_pixel == 32) {
		glReadPixels(0, gr_screen.max_h-h-1, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
#endif
	
#if !defined(HAVE_GLES)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
}

void gr_opengl_save_mouse_area(int x, int y, int w, int h)
{
	Gr_opengl_mouse_saved_x1 = x;
	Gr_opengl_mouse_saved_y1 = y;
	Gr_opengl_mouse_saved_x2 = x+w-1;
	Gr_opengl_mouse_saved_y2 = y+h-1;
	
	CLAMP(Gr_opengl_mouse_saved_x1, gr_screen.clip_left, gr_screen.clip_right );
	CLAMP(Gr_opengl_mouse_saved_x2, gr_screen.clip_left, gr_screen.clip_right );
	CLAMP(Gr_opengl_mouse_saved_y1, gr_screen.clip_top, gr_screen.clip_bottom );
	CLAMP(Gr_opengl_mouse_saved_y2, gr_screen.clip_top, gr_screen.clip_bottom );
	
	Gr_opengl_mouse_saved_w = Gr_opengl_mouse_saved_x2 - Gr_opengl_mouse_saved_x1 + 1;
	Gr_opengl_mouse_saved_h = Gr_opengl_mouse_saved_y2 - Gr_opengl_mouse_saved_y1 + 1;

	if ( Gr_opengl_mouse_saved_w < 1 ) return;
	if ( Gr_opengl_mouse_saved_h < 1 ) return;
	
	Assert( (Gr_opengl_mouse_saved_w*Gr_opengl_mouse_saved_h) <= MAX_SAVE_SIZE );

	gr_opengl_set_state(TEXTURE_SOURCE_NO_FILTERING, ALPHA_BLEND_NONE, ZBUFFER_TYPE_NONE);
	
#if defined(HAVE_GLES)
	int p2width = 1;
	int p2height = 1;
	while (p2width<w) p2width*=2;
	while (p2height<h) p2height*=2;
	ubyte *ptmp=(ubyte*)malloc(p2width * p2height * 4);
	glReadPixels(x, gr_screen.max_h-y-1-h, p2width, p2height, GL_RGBA, GL_UNSIGNED_BYTE, ptmp);
	ubyte r,g,b,a;
	for (int yy=0; yy<h; yy++)
		for (int xx=0; xx<w; xx++) {
			r=ptmp[(yy*p2width+xx)*4+0];
			g=ptmp[(yy*p2width+xx)*4+1];
			b=ptmp[(yy*p2width+xx)*4+2];
			a=ptmp[(yy*p2width+xx)*4+3];
			Gr_opengl_mouse_saved_data[(yy*w+xx)*2+0] = (r>>3)<<3 | (g>>3)>>2;
			Gr_opengl_mouse_saved_data[(yy*w+xx)*2+1] = ((g>>3)&3)<<6 | (b>>3)<<1 | (a)?1:0;
		}
	free(ptmp);
//	glReadPixels(x, gr_screen.max_h-y-1-h, w, h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, Gr_opengl_mouse_saved_data);
#else
	glReadBuffer(GL_BACK);
	glReadPixels(x, gr_screen.max_h-y-1-h, w, h, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, Gr_opengl_mouse_saved_data);
#endif
	
	Gr_opengl_mouse_saved = 1;
}

int gr_opengl_save_screen()
{
	gr_reset_clip();

	if ( Gr_saved_screen )  {
		mprintf(( "Screen alread saved!\n" ));
		return -1;
	}

#ifdef HAVE_GLES
	int p2width = 1;
	int p2height = 1;
	while (p2width<gr_screen.max_w) p2width*=2;
	while (p2height<gr_screen.max_h) p2height*=2;
	Gr_saved_screen = (char*)malloc( p2width * p2height * gr_screen.bytes_per_pixel );
#else
	int p2width = gr_screen.max_w;
	int p2height = gr_screen.max_h;
	Gr_saved_screen = (char*)malloc( gr_screen.max_w * gr_screen.max_h * gr_screen.bytes_per_pixel );
#endif
	if (!Gr_saved_screen) {
		mprintf(( "Couldn't get memory for saved screen!\n" ));
		return -1;
	}

	char *Gr_saved_screen_tmp = (char*)malloc( p2width * p2height * gr_screen.bytes_per_pixel );
	if (!Gr_saved_screen_tmp) {
		mprintf(( "Couldn't get memory for temporary saved screen!\n" ));
		return -1;
	}
	
	gr_opengl_set_state(TEXTURE_SOURCE_NO_FILTERING, ALPHA_BLEND_NONE, ZBUFFER_TYPE_NONE);
	
#if defined(HAVE_GLES)
	ubyte *ptmp=(ubyte*)malloc(p2width * p2height * 4);
	glReadPixels(0, 0, p2width, p2height, GL_RGBA, GL_UNSIGNED_BYTE, ptmp);
	ubyte r,g,b,a;
	for (int yy=0; yy<p2height; yy++)
		for (int xx=0; xx<p2width; xx++) {
			r=ptmp[(yy*p2width+xx)*4+0];
			g=ptmp[(yy*p2width+xx)*4+1];
			b=ptmp[(yy*p2width+xx)*4+2];
			a=ptmp[(yy*p2width+xx)*4+3];
			Gr_saved_screen_tmp[(yy*p2width+xx)*2+0] = (r>>3)<<3 | (g>>3)>>2;
			Gr_saved_screen_tmp[(yy*p2height+xx)*2+1] = ((g>>3)&3)<<6 | (b>>3)<<1 | (a)?1:0;
		}
	free(ptmp);
//	glReadPixels(0, 0, gr_screen.max_w, gr_screen.max_h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, Gr_saved_screen_tmp);
#else
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, gr_screen.max_w, gr_screen.max_h, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, Gr_saved_screen_tmp);
#endif
	
	ubyte *sptr, *dptr;
	
	sptr = (ubyte *)&Gr_saved_screen_tmp[p2width*p2height*2];
//	sptr = (ubyte *)&Gr_saved_screen_tmp[gr_screen.max_w*gr_screen.max_h*2];
	dptr = (ubyte *)Gr_saved_screen;
//	for (int j = 0; j < gr_screen.max_h; j++) {
	for (int j = 0; j < p2height; j++) {
		sptr -= p2width*2;
		memcpy(dptr, sptr, p2width*2);
		dptr += p2width*2;
	}
	
	free(Gr_saved_screen_tmp);
	
	if (Gr_opengl_mouse_saved) {
		sptr = (ubyte *)Gr_opengl_mouse_saved_data;
		dptr = (ubyte *)&Gr_saved_screen[2*(Gr_opengl_mouse_saved_x1+(Gr_opengl_mouse_saved_y2)*p2width)];
		for (int i = 0; i < Gr_opengl_mouse_saved_h; i++) {
			memcpy(dptr, sptr, Gr_opengl_mouse_saved_w*2);
		
			sptr += 32*2;
			dptr -= p2width*2;
		}
	}

	// this leaks texture handles, and the opengl doesn't currently 
	// perform some sort of garbage collection, so a hack was added
	// to bmpman to make it free textures when released
	Gr_saved_screen_bitmap = bm_create(16, p2width, p2height, Gr_saved_screen, 0);
	
	return Gr_saved_screen_bitmap;
}

void gr_opengl_restore_screen(int id)
{
	gr_reset_clip();
	
	if ( !Gr_saved_screen ) {
		gr_clear();
		return;
	}

	gr_opengl_set_state(TEXTURE_SOURCE_NO_FILTERING, ALPHA_BLEND_NONE, ZBUFFER_TYPE_NONE);
	
	gr_set_bitmap(Gr_saved_screen_bitmap, GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
	gr_bitmap(0, 0);		
}

void gr_opengl_free_screen(int id)
{
	bm_release(Gr_saved_screen_bitmap);
	
	if ( Gr_saved_screen )  {
		free( Gr_saved_screen );
		Gr_saved_screen = NULL;
	}
}

void gr_opengl_dump_frame_start(int first_frame, int frames_between_dumps)
{
	STUB_FUNCTION;
}

void gr_opengl_dump_frame_stop()
{
	STUB_FUNCTION;
}

void gr_opengl_dump_frame()
{
	STUB_FUNCTION;
}

uint gr_opengl_lock()
{
	STUB_FUNCTION;
	
	return 1;
}
        
void gr_opengl_unlock()
{
}

void opengl_zbias(int bias)
{
	if (bias) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.0, -bias);
	} else {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
}
	        
extern const char *Osreg_title;
void gr_opengl_init()
{
	if ( Inited )	{
		gr_opengl_cleanup();
		Inited = 0;
	}

	mprintf(( "Initializing opengl graphics device...\n" ));
	Inited = 1;

#ifdef __vita__
	mapped_idx = malloc(32768 * sizeof(uint16_t));
	mapped_vtx = malloc(16 * 1024 * 1024);
	mapped_clr = malloc(16 * 1024 * 1024);
	mapped_tex = malloc(16 * 1024 * 1024);
	mapped_vtx_head = mapped_vtx;
	mapped_clr_head = mapped_clr;
	mapped_tex_head = mapped_tex;
	for (int i = 0; i < 32768; i++) {
			mapped_idx[i] = i;
	}
	vglIndexPointerMapped(mapped_idx);
#endif

#ifdef PLAT_UNIX	
	if (SDL_InitSubSystem (SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "Couldn't init SDL: %s", SDL_GetError());
		exit (1);
	}
	#if defined(HAVE_GLES) && !defined(__vita__)
	EGL_Open();
	int flags = 0;
	#ifdef PANDORA
	flags=SDL_FULLSCREEN;
//	gr_screen.max_w = 800;
//	gr_screen.max_h = 480;
	#endif
	#else

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	int flags = SDL_OPENGL;
	#ifdef __vita__
	flags |= SDL_FULLSCREEN;
//	gr_screen.max_w = 960;
//	gr_screen.max_h = 544;
	#endif
	#endif
	if (!Cmdline_window && ( (os_config_read_uint( NULL, "Fullscreen", 1 ) == 1) || Cmdline_fullscreen ))
		flags |= SDL_FULLSCREEN;

	// grab mouse/key unless told otherwise, ignore when we are going fullscreen
	if ( !((flags & SDL_FULLSCREEN) || Cmdline_no_grab) ) {
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}

	#ifndef HAVE_GLES
	FSAA = os_config_read_uint( NULL, "FSAA", 1 );
	if ( FSAA ) {
	    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, FSAA );
	}
	#endif
#ifdef __vita__
	if (SDL_SetVideoMode (960, 544,0,flags) == NULL)
#else
	if (SDL_SetVideoMode (gr_screen.max_w, gr_screen.max_h,0,flags) == NULL)
#endif
	{
		#if !defined(HAVE_GLES) || defined(__vita__)
	    mprintf(( "Couldn't set FSAA video mode: %s\n", SDL_GetError () ));
	    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );
	    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 0 );
		
	    if (SDL_SetVideoMode (gr_screen.max_w, gr_screen.max_h,0,flags) == NULL)
	    {
		#endif
		    fprintf (stderr, "Couldn't set video mode: %s\n", SDL_GetError ());
		    exit (1);
		#if !defined(HAVE_GLES) || defined(__vita__)
	    }
		#endif
	}
	#if defined(HAVE_GLES) && !defined(__vita__)
	EGL_Init();
	#endif

	mprintf(( "Screen BPP: %d\n", SDL_GetVideoSurface()->format->BitsPerPixel ));
	mprintf(( "\n" ));
	mprintf(( "Vendor     : %s\n", glGetString(GL_VENDOR) ));
	mprintf(( "Renderer   : %s\n", glGetString(GL_RENDERER) ));
	mprintf(( "Version    : %s\n", glGetString(GL_VERSION) ));

#ifndef NDEBUG
	// print out extensions - taken from FS2_Open (credits: phreak, taylor)
	mprintf(( "Extensions : \n"));

	static const char *OGL_extensions = (const char*)glGetString(GL_EXTENSIONS);

	// we use the "+1" here to have an extra NULL char on the end (with the memset())
	// this is to fix memory errors when the last char in extlist is the same as the token
	// we are looking for and ultra evil strtok() may still return non-NULL at EOS
	char *extlist = (char*)malloc(strlen(OGL_extensions) + 1);
	memset(extlist, 0, strlen(OGL_extensions) + 1);

	if (extlist != NULL) {
		memcpy(extlist, OGL_extensions, strlen(OGL_extensions));

		char *curext = strtok(extlist, " ");

		while (curext) {
			mprintf(( "     %s\n", curext ));
			curext = strtok(NULL, " ");
		}

		free(extlist);
		extlist = NULL;
	}

	mprintf(( "\n" ));
#endif
	
	int bpp = 15;	
	
	#if !defined(HAVE_GLES) || defined(__vita__)
	int value;
	int rgb_size[3];
	rgb_size[0]=5;
	rgb_size[1]=5;
	rgb_size[2]=5;

	SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value );
	mprintf(( "SDL_GL_RED_SIZE: requested %d, got %d\n", rgb_size[0],value ));
	SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value );
	mprintf(( "SDL_GL_GREEN_SIZE: requested %d, got %d\n", rgb_size[1],value ));
	SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value );
	mprintf(( "SDL_GL_BLUE_SIZE: requested %d, got %d\n", rgb_size[2],value ));
	SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value );
	mprintf(( "SDL_GL_DEPTH_SIZE: requested %d, got %d\n", bpp, value ));
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &value );
	mprintf(( "SDL_GL_DOUBLEBUFFER: requested 1, got %d\n", value ));
	#ifndef __vita__
	if ( FSAA ) {
		SDL_GL_GetAttribute( SDL_GL_MULTISAMPLEBUFFERS, &value );
		mprintf(( "SDL_GL_MULTISAMPLEBUFFERS: requested 1, got %d\n", value ));
		SDL_GL_GetAttribute( SDL_GL_MULTISAMPLESAMPLES, &value );
		mprintf(( "SDL_GL_MULTISAMPLESAMPLES: requested %d, got %d\n", FSAA, value ));
	}
	#endif
	#endif
	SDL_ShowCursor(0);
	SDL_WM_SetCaption (Osreg_title, NULL);
	
	/* might as well put this here */
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#endif

	GL_use_luminance_alpha = os_config_read_uint(NOX("OpenGL"), NOX("UseLuminanceAlpha"), 0);
#ifdef PANDORA
	glViewport(80, 0, gr_screen.max_w, gr_screen.max_h);
#else
	glViewport((960 - 725) / 2, 0, 725, 544);
#endif

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, gr_screen.max_w, gr_screen.max_h, 0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#ifndef __vita__
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DITHER);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_NICEST);
#endif
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	glEnable(GL_TEXTURE_2D);
	
	glDepthRange(0.0, 1.0);
	
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	D3D_32bit = 1;              // grd3d.cpp
	extern int D3D_enabled;
	D3D_enabled = 1;
	/* 
	  TODO: set fog_mode to 1 if EXT_secondary_color found and wanted 
	  1 = use secondary color ext
	  2 = use opengl linear fog
	 */
	D3D_fog_mode = 2;          // grd3d.cpp

	glFlush();
	
	Bm_pixel_format = BM_PIXEL_FORMAT_ARGB;
	Gr_bitmap_poly = 1;
	
	switch( bpp )	{
	case 15:
		gr_screen.bits_per_pixel = 15;
		gr_screen.bytes_per_pixel = 2;
		
		Gr_red.bits = 5;
		Gr_red.shift = 10;
		Gr_red.scale = 8;
		Gr_red.mask = 0x7C00;

		Gr_green.bits = 5;
		Gr_green.shift = 5;
		Gr_green.scale = 8;
		Gr_green.mask = 0x3E0;

		Gr_blue.bits = 5;
		Gr_blue.shift = 0;
		Gr_blue.scale = 8;
		Gr_blue.mask = 0x1F;

		break;

	case 16:
		gr_screen.bits_per_pixel = 16;
		gr_screen.bytes_per_pixel = 2;
		
		Gr_red.bits = 5;
		Gr_red.shift = 11;
		Gr_red.scale = 8;
		Gr_red.mask = 0xF800;

		Gr_green.bits = 6;
		Gr_green.shift = 5;
		Gr_green.scale = 4;
		Gr_green.mask = 0x7E0;

		Gr_blue.bits = 5;
		Gr_blue.shift = 0;
		Gr_blue.scale = 8;
		Gr_blue.mask = 0x1F;

		break;

	case 32:
		gr_screen.bits_per_pixel = 32;
		gr_screen.bytes_per_pixel = 4;
		
		Gr_red.bits = 8;
		Gr_red.shift = 16;
		Gr_red.scale = 1;
		Gr_red.mask = 0xff0000;

		Gr_green.bits = 8;
		Gr_green.shift = 8;
		Gr_green.scale = 1;
		Gr_green.mask = 0xff00;

		Gr_blue.bits = 8;
		Gr_blue.shift = 0;
		Gr_blue.scale = 1;
		Gr_blue.mask = 0xff;

		break;

	default:
		Int3();	// Illegal bpp
	}

	// DDOI - set these so no one else does!
	Gr_t_red.mask = Gr_red.mask;
	Gr_t_red.shift = Gr_red.shift;
	Gr_t_red.scale = Gr_red.scale;

	Gr_t_green.mask = Gr_green.mask;
	Gr_t_green.shift = Gr_green.shift;
	Gr_t_green.scale = Gr_green.scale;
	
	Gr_t_blue.mask = Gr_blue.mask;
	Gr_t_blue.shift = Gr_blue.shift;
	Gr_t_blue.scale = Gr_blue.scale;
	
	Gr_t_alpha.mask = 0x8000;
	Gr_t_alpha.scale = 255;
	Gr_t_alpha.shift = 15;

	Gr_ta_red.mask = 0x0f00;
	Gr_ta_red.shift = 8;
	Gr_ta_red.scale = 16;

	Gr_ta_green.mask = 0x00f0;
	Gr_ta_green.shift = 4;
	Gr_ta_green.scale = 16;
	
	Gr_ta_blue.mask = 0x000f;
	Gr_ta_blue.shift = 0;
	Gr_ta_blue.scale = 16;

	Gr_ta_alpha.mask = 0xf000;
	Gr_ta_alpha.shift = 12;
	Gr_ta_alpha.scale = 16;


	opengl_tcache_init (1);
	gr_opengl_clear();

	Gr_current_red = &Gr_red;
	Gr_current_blue = &Gr_blue;
	Gr_current_green = &Gr_green;
	Gr_current_alpha = &Gr_alpha;

	gr_screen.gf_flip = gr_opengl_flip;
	gr_screen.gf_flip_window = gr_opengl_flip_window;
	gr_screen.gf_set_clip = gr_opengl_set_clip;
	gr_screen.gf_reset_clip = gr_opengl_reset_clip;
	gr_screen.gf_set_font = grx_set_font;
	
	gr_screen.gf_set_color = gr_opengl_set_color;
	gr_screen.gf_set_bitmap = gr_opengl_set_bitmap;
	gr_screen.gf_create_shader = gr_opengl_create_shader;
	gr_screen.gf_set_shader = gr_opengl_set_shader;
	gr_screen.gf_clear = gr_opengl_clear;
	// gr_screen.gf_bitmap = gr_opengl_bitmap;
	// gr_screen.gf_bitmap_ex = gr_opengl_bitmap_ex;
	gr_screen.gf_aabitmap = gr_opengl_aabitmap;
	gr_screen.gf_aabitmap_ex = gr_opengl_aabitmap_ex;
	
	gr_screen.gf_rect = gr_opengl_rect;
	gr_screen.gf_shade = gr_opengl_shade;
	gr_screen.gf_string = gr_opengl_string;
	gr_screen.gf_circle = gr_opengl_circle;

	gr_screen.gf_line = gr_opengl_line;
	gr_screen.gf_aaline = gr_opengl_aaline;
	gr_screen.gf_pixel = gr_opengl_pixel;
	gr_screen.gf_scaler = gr_opengl_scaler;
	gr_screen.gf_tmapper = gr_opengl_tmapper;

	gr_screen.gf_gradient = gr_opengl_gradient;

	gr_screen.gf_set_palette = gr_opengl_set_palette;
	gr_screen.gf_get_color = gr_opengl_get_color;
	gr_screen.gf_init_color = gr_opengl_init_color;
	gr_screen.gf_init_alphacolor = gr_opengl_init_alphacolor;
	gr_screen.gf_set_color_fast = gr_opengl_set_color_fast;
	gr_screen.gf_print_screen = gr_opengl_print_screen;

	gr_screen.gf_fade_in = gr_opengl_fade_in;
	gr_screen.gf_fade_out = gr_opengl_fade_out;
	gr_screen.gf_flash = gr_opengl_flash;
	
	gr_screen.gf_zbuffer_get = gr_opengl_zbuffer_get;
	gr_screen.gf_zbuffer_set = gr_opengl_zbuffer_set;
	gr_screen.gf_zbuffer_clear = gr_opengl_zbuffer_clear;
	
	gr_screen.gf_save_screen = gr_opengl_save_screen;
	gr_screen.gf_restore_screen = gr_opengl_restore_screen;
	gr_screen.gf_free_screen = gr_opengl_free_screen;
	
	gr_screen.gf_dump_frame_start = gr_opengl_dump_frame_start;
	gr_screen.gf_dump_frame_stop = gr_opengl_dump_frame_stop;
	gr_screen.gf_dump_frame = gr_opengl_dump_frame;
	
	gr_screen.gf_set_gamma = gr_opengl_set_gamma;
	
	gr_screen.gf_lock = gr_opengl_lock;
	gr_screen.gf_unlock = gr_opengl_unlock;
	
	gr_screen.gf_fog_set = gr_opengl_fog_set;	

	gr_screen.gf_get_region = gr_opengl_get_region;

	gr_screen.gf_get_pixel = gr_opengl_get_pixel;

	gr_screen.gf_set_cull = gr_opengl_set_cull;

	gr_screen.gf_cross_fade = gr_opengl_cross_fade;

	gr_screen.gf_filter_set = gr_opengl_filter_set;

	gr_screen.gf_tcache_set = gr_opengl_tcache_set;

	gr_screen.gf_set_clear_color = gr_opengl_set_clear_color;
	
	Mouse_hidden++;
	gr_reset_clip();
	gr_clear();
	gr_flip();
	Mouse_hidden--;
}
