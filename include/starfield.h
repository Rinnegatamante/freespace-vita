/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Starfield/StarField.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Code to handle and draw starfields, background space image bitmaps, floating
 * debris, etc.
 *
 * $Log$
 * Revision 1.3  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 11    7/21/99 8:10p Dave
 * First run of supernova effect.
 * 
 * 10    6/03/99 6:37p Dave
 * More TNT fun. Made perspective bitmaps more flexible.
 * 
 * 9     5/09/99 6:00p Dave
 * Lots of cool new effects. E3 build tweaks.
 * 
 * 8     5/07/99 1:59p Johnson
 * Tweaked background bitmaps a bit.
 * 
 * 7     4/23/99 5:53p Dave
 * Started putting in new pof nebula support into Fred.
 * 
 * 6     4/07/99 6:22p Dave
 * Fred and Freespace support for multiple background bitmaps and suns.
 * Fixed link errors on all subprojects. Moved encrypt_init() to
 * cfile_init() and lcl_init(), since its safe to call twice.
 * 
 * 5     3/31/99 8:24p Dave
 * Beefed up all kinds of stuff, incluging beam weapons, nebula effects
 * and background nebulae. Added per-ship non-dimming pixel colors.
 * 
 * 4     11/14/98 5:33p Dave
 * Lots of nebula work. Put in ship contrails.
 * 
 * 3     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 21    4/11/98 6:53p John
 * Added first rev of subspace effect.
 * 
 * 20    4/07/98 4:17p John
 * Made Fred be able to move suns.  Made suns actually affect the lighting
 * in the game.
 * 
 * 19    3/21/98 7:36p Lawrance
 * Move jump nodes to own lib.
 * 
 * 18    3/19/98 12:29p Lawrance
 * Fix jumpnode targeting bug on the target monitor
 * 
 * 17    3/15/98 3:41p Allender
 * new sexpression to gauge whether a ship warped out in proximity of jump
 * node
 * 
 * 16    3/11/98 5:33p Lawrance
 * Support rendering and targeting of jump nodes
 * 
 * 15    3/10/98 4:26p Hoffoss
 * Changed jump node structure to include a name.  Position is now taken
 * from the object (each jump node has an associated object now).
 * 
 * 14    3/06/98 5:30p Hoffoss
 * Added jump node rendering code to FreeSpace.
 * 
 * 13    2/26/98 10:08p Hoffoss
 * Rewrote state saving and restoring to fix bugs and simplify the code.
 * 
 * 12    2/06/98 3:08p Mike
 * More asteroid stuff, including resolving conflicts between the two
 * asteroid_field structs!
 * 
 * 11    1/18/98 4:24p John
 * Detect when view camera "cuts" and tell the star code about it so it
 * won't blur stars and debris between the last frame and this frame.
 * 
 * 10    8/25/97 5:56p Hoffoss
 * Added multiple asteroid field support, loading and saving of asteroid
 * fields, and ship score field to Fred.
 * 
 * 9     8/05/97 10:18a Lawrance
 * my_rand() being used temporarily instead of rand()
 * 
 * 8     2/17/97 3:06p Hoffoss
 * Changed header description.
 * 
 * 7     2/14/97 3:29p Hoffoss
 * Added header for MSDEV to fill in.
 *
 * $NoKeywords: $
 */

#ifndef _STARFIELD_H
#define _STARFIELD_H

#include "parselo.h"
#include "cfile.h"

#define MAX_STARFIELD_BITMAP_LISTS	1
#define MAX_STARFIELD_BITMAPS			60
#define MAX_ASTEROID_FIELDS			4

// nice low polygon background
#define BACKGROUND_MODEL_FILENAME					"spherec.pof"

// global info (not individual instances)
typedef struct starfield_bitmap {
	char filename[MAX_FILENAME_LEN+1];				// bitmap filename
	char glow_filename[MAX_FILENAME_LEN+1];		// only for suns	
	int bitmap;												// bitmap handle
	int glow_bitmap;										// only for suns
	int xparent;	
	float r, g, b, i;										// only for suns
} starfield_bitmap;

// starfield bitmap instance
typedef struct starfield_bitmap_instance {
	char filename[MAX_FILENAME_LEN+1];				// used to match up into the starfield_bitmap array	
	float scale_x, scale_y;								// x and y scale
	int div_x, div_y;										// # of x and y divisions
	angles ang;												// angles from fred
#ifdef MAKE_FS1
	matrix m;			// straight matrix instead of angles to matrix
	int sun_light;		// FIXME: not sure what this is supposed to do
	float rot;			// rotation rate - always 0?
#endif
} starfield_bitmap_instance;

// background bitmaps
extern starfield_bitmap Starfield_bitmaps[MAX_STARFIELD_BITMAPS];
extern starfield_bitmap_instance Starfield_bitmap_instance[MAX_STARFIELD_BITMAPS];
extern int Num_starfield_bitmaps;

// sun bitmaps and sun glow bitmaps
extern starfield_bitmap Sun_bitmaps[MAX_STARFIELD_BITMAPS];
extern starfield_bitmap_instance Suns[MAX_STARFIELD_BITMAPS];
extern int Num_suns;

extern const int MAX_STARS;
extern int Num_stars;

// call on game startup
void stars_init();

// call this in game_post_level_init() so we know whether we're running in full nebula mode or not
void stars_level_init();

// This *must* be called to initialize the lighting.
// You can turn off all the stars and suns and nebulas, though.
void stars_draw(int show_stars, int show_suns, int show_nebulas, int show_subspace);
// void calculate_bitmap_matrix(starfield_bitmaps *bm, vector *v);
// void calculate_bitmap_points(starfield_bitmaps *bm, float bank = 0.0f);

// draw the corresponding glow for sun_n
void stars_draw_sun_glow(int sun_n);

// Call when the viewer camera "cuts" so stars and debris
// don't draw incorrect blurs between last frame and this frame.
void stars_camera_cut();

// call this to set a specific model as the background model
void stars_set_background_model(const char *model_name, const char *texture_name);

// lookup a starfield bitmap, return index or -1 on fail
int stars_find_bitmap(const char *name);

// lookup a sun by bitmap filename, return index or -1 on fail
int stars_find_sun(const char *name);

// get the world coords of the sun pos on the unit sphere.
void stars_get_sun_pos(int sun_n, vector *pos);

#endif

