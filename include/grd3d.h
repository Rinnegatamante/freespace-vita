/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Graphics/GrD3D.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Include file for our Direct3D renderer
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 6     9/13/99 11:25p Dave
 * Fixed problem with mode-switching and D3D movies.
 * 
 * 5     9/04/99 8:00p Dave
 * Fixed up 1024 and 32 bit movie support.
 * 
 * 4     6/29/99 10:35a Dave
 * Interface polygon bitmaps! Whee!
 * 
 * 3     1/15/99 11:29a Neilk
 * Fixed D3D screen/texture pixel formatting problem. 
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 2     5/12/97 12:27p John
 * Restructured Graphics Library to add support for multiple renderers.
 * 
 * 1     5/01/97 2:17p John
 *
 * $NoKeywords: $
 */

#ifndef _GRD3D_H
#define _GRD3D_H

void gr_d3d_init();
void gr_d3d_cleanup();

// call this to safely fill in the texture shift and scale values for the specified texture type (Gr_t_*)
void gr_d3d_get_tex_format(int alpha);

// bitmap functions
void gr_d3d_bitmap(int x, int y);
void gr_d3d_bitmap_ex(int x, int y, int w, int h, int sx, int sy);

// create all rendering objects (surfaces, d3d device, viewport, etc)
int gr_d3d_create_rendering_objects(int clear);
void gr_d3d_release_rendering_objects();


void gr_d3d_set_initial_render_state();

#endif

