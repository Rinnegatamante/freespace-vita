/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Nebula/NebLightning.cpp $
 * $Revision: 308 $
 * $Date: 2010-02-08 09:09:21 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Nebula effect
 *
 * $Log$
 * Revision 1.7  2004/09/20 01:31:44  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.6  2004/07/04 11:39:06  taylor
 * fix missing debrief text, crash on exit, path separator's, warning fixes, no GR_SOFT
 *
 * Revision 1.5  2003/05/25 02:30:43  taylor
 * Freespace 1 support
 *
 * Revision 1.4  2002/06/17 06:33:09  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.3  2002/06/09 04:41:23  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:47  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 * 
 * 
 * 10    8/15/99 3:50p Dave
 * Don't process lightning at the very beginning of a mission.
 * 
 * 9     8/12/99 10:38a Anoop
 * Removed unnecessary Int3().
 * 
 * 8     8/05/99 2:06a Dave
 * Whee.
 * 
 * 7     7/27/99 9:51p Andsager
 * make mprintf's into nprintf's
 * 
 * 6     7/03/99 5:50p Dave
 * Make rotated bitmaps draw properly in padlock views.
 * 
 * 5     7/02/99 4:31p Dave
 * Much more sophisticated lightning support.
 * 
 * 4     6/09/99 10:32a Dave
 * Made random lighting bolts behave more like the E3 demo. Generally more
 * active.
 * 
 * 3     5/26/99 11:46a Dave
 * Added ship-blasting lighting and made the randomization of lighting
 * much more customizable.
 * 
 * 2     5/24/99 5:45p Dave
 * Added detail levels to the nebula, with a decent speedup. Split nebula
 * lightning into its own section.
 * 
 * $NoKeywords: $
 */

#include "parselo.h"
#include "linklist.h"
#include "bmpman.h"
#include "timer.h"
#include "freespace.h"
#include "gamesnd.h"
#include "3d.h"
#include "missionparse.h"
#include "neb.h"
#include "neblightning.h"
#include "multi.h"
#include "emp.h"
#include "multimsgs.h"

// ------------------------------------------------------------------------------------------------------
// NEBULA LIGHTNING DEFINES/VARS
//

// debug stuff
#define MAX_BOLT_TYPES_INTERNAL		11

// see lightning.tbl for explanations of these values
typedef struct bolt_type {
	char		name[NAME_LENGTH];

	float		b_scale;
	float		b_shrink;
	float		b_poly_pct;	
	float		b_add;
	float		b_rand;

	float		noise;
	int		lifetime;
	int		num_strikes;

	float		emp_intensity;
	float		emp_time;
	
	int		texture;
	int		glow;	

	float		b_bright;
} bolt_type;

int Num_bolt_types = 0;
bolt_type Bolt_types[MAX_BOLT_TYPES_INTERNAL];

// storm types
int Num_storm_types = 0;
storm_type Storm_types[MAX_STORM_TYPES];


// actual lightning bolt stuff -------

#define MAX_LIGHTNING_NODES					500

// nodes in a lightning bolt
#define LINK_LEFT		0
#define LINK_RIGHT	1
#define LINK_CHILD	2
typedef struct l_node {
	vector	pos;				// world position
	l_node	*links[3];		// 3 links for lightning children

	l_node *next, *prev;		// for used and free-lists only
} l_node;


// nodes
l_node Nebl_nodes[MAX_LIGHTNING_NODES];
int Num_lnodes = 0;

// lightning node lists
l_node Nebl_free_list;
l_node Nebl_used_list;

// actual lightning bolt themselves
typedef struct l_bolt {
	l_node *head;				// head of the lightning bolt
	int bolt_life;				// remaining life timestamp
	ubyte used;					// used or not
	ubyte	first_frame;		// if he hasn't been rendered at least once	
	char type;

	// bolt info
	vector start, strike, midpoint;
	int delay;					// delay stamp
	int strikes_left;			// #of strikes left
	float width;
} l_bolt;

#define MAX_LIGHTNING_BOLTS					10

// lightning bolts
l_bolt Nebl_bolts[MAX_LIGHTNING_BOLTS];
int Nebl_bolt_count = 0;

// one cross-section of a lightning bolt
typedef struct l_section {		
	vertex	vex[3];
	vertex	glow_vex[2];
} l_section;

// points on the basic cross section
vector Nebl_ring[3] = {	
	{ { { -1.0f, 0.0f, 0.0f } } },
	{ { { 1.0f, 0.70f, 0.0f } } },
	{ { { 1.0f, -0.70f, 0.0f } } }	
};

// pinched off cross-section
vector Nebl_ring_pinched[3] = {	
	{ { { -0.05f, 0.0f, 0.0f } } },
	{ { { 0.05f, 0.035f, 0.0f } } },
	{ { { 0.05f, -0.035f, 0.0f } } }	
};

// globals used for rendering and generating bolts
int Nebl_flash_count = 0;		// # of points rendered onscreen for this bolt
float Nebl_flash_x = 0.0f;		// avg x of the points rendered
float Nebl_flash_y = 0.0f;		// avg y of the points rendered
float Nebl_bang = 0.0;			// distance to the viewer object
float Nebl_alpha = 0.0f;		// alpha to use when rendering the bolt itself
float Nebl_glow_alpha = 0.0f;	// alpha to use when rendering the bolt glow
int Nebl_stamp = -1;				// random timestamp for making bolts
float Nebl_bolt_len;				// length of the current bolt being generated
bolt_type *Nebl_type;			// bolt type
matrix Nebl_bolt_dir;			// orientation matrix of the bolt being generated
vector Nebl_bolt_start;			// start point of the bolt being generated
vector Nebl_bolt_strike;		// strike point of the bolt being generated

// the type of active storm
storm_type *Storm = NULL;

// vars
DCF(b_scale, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].b_scale = Dc_arg_float;
}
DCF(b_rand, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].b_rand = Dc_arg_float;
}
DCF(b_shrink, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].b_shrink = Dc_arg_float;
}
DCF(b_poly_pct, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].b_poly_pct = Dc_arg_float;
}
DCF(b_add, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].b_add = Dc_arg_float;
}
DCF(b_strikes, "")
{
	dc_get_arg(ARG_INT);
	Bolt_types[DEBUG_BOLT].num_strikes = Dc_arg_int;
}
DCF(b_noise, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].noise = Dc_arg_float;
}
DCF(b_bright, "")
{
	dc_get_arg(ARG_FLOAT);
	Bolt_types[DEBUG_BOLT].b_bright = Dc_arg_float;
}
DCF(b_lifetime, "")
{
	dc_get_arg(ARG_INT);
	Bolt_types[DEBUG_BOLT].lifetime = Dc_arg_int;
}
DCF(b_list, "")
{
	dc_printf("Debug lightning bolt settings :\n");

	dc_printf("b_scale : %f\n", Bolt_types[DEBUG_BOLT].b_scale);
	dc_printf("b_rand : %f\n", Bolt_types[DEBUG_BOLT].b_rand);
	dc_printf("b_shrink : %f\n", Bolt_types[DEBUG_BOLT].b_shrink);
	dc_printf("b_poly_pct : %f\n", Bolt_types[DEBUG_BOLT].b_poly_pct);
	dc_printf("b_add : %f\n", Bolt_types[DEBUG_BOLT].b_add);
	dc_printf("b_strikes : %d\n", Bolt_types[DEBUG_BOLT].num_strikes);
	dc_printf("b_noise : %f\n", Bolt_types[DEBUG_BOLT].noise);
	dc_printf("b_bright : %f\n", Bolt_types[DEBUG_BOLT].b_bright);
	dc_printf("b_lifetime : %d\n", Bolt_types[DEBUG_BOLT].lifetime);
}


// nebula lightning intensity (0.0 to 1.0)
float Nebl_intensity = 0.6667f;

// min and max times for random lightning
int Nebl_random_min = 750;				// min random time
int Nebl_random_max = 10000;			// max random time

// min and max times for cruiser lightning
int Nebl_cruiser_min = 5000;			// min cruiser time
int Nebl_cruiser_max = 25000;			// max cruiser time

// min and max times for cap ships
int Nebl_cap_min = 4000;				// min cap time
int Nebl_cap_max = 18000;				// max cap time

// min and max time for super caps
int Nebl_supercap_min = 3000;			// min supercap time
int Nebl_supercap_max = 12000;		// max supercap time

DCF(lightning_intensity, "")
{
	dc_get_arg(ARG_FLOAT);
	float val = Dc_arg_float;
	if(val < 0.0f){
		val = 0.0f;
	} else if(val > 1.0f){
		val = 1.0f;
	}

	Nebl_intensity = 1.0f - val;
}

// ------------------------------------------------------------------------------------------------------
// NEBULA LIGHTNING FORWARD DECLARATIONS
//

// "new" a lightning node
l_node *nebl_new();

// "delete" a lightning node
void nebl_delete(l_node *lp);

// free up a the nodes of the passed in bolt
void nebl_release(l_node *bolt_head);

// generate a lightning bolt, returns l_left (the "head") and l_right (the "tail")
int nebl_gen(vector *left, vector *right, float depth, float max_depth, int child, l_node **l_left, l_node **l_right);

// output top and bottom vectors
// fvec == forward vector (eye viewpoint basically. in world coords)
// pos == world coordinate of the point we're calculating "around"
// w == width of the diff between top and bottom around pos
void nebl_calc_facing_pts_smart( vector *top, vector *bot, vector *fvec, vector *pos, float w, float z_add );

// render a section of the bolt
void nebl_render_section(bolt_type *bi, l_section *a, l_section *b);

// generate a section
void nebl_generate_section(bolt_type *bi, float width, l_node *a, l_node *b, l_section *c, l_section *cap, int pinch_a, int pinch_b);

// render the bolt
void nebl_render(bolt_type *bi, l_node *whee, float width, l_section *prev = NULL);

// given a valid, complete bolt, jitter him based upon his noise
void nebl_jitter(l_bolt *b);

// return the index of a given bolt type by name
int nebl_get_bolt_index(char *name);

// return the index of a given storm type by name
int nebl_get_storm_index(char *name);


// ------------------------------------------------------------------------------------------------------
// NEBULA LIGHTNING FUNCTIONS
//

// initialize nebula lightning at game startup
void nebl_init()
{
#ifndef MAKE_FS1
	char name[NAME_LENGTH+10] = "";
	bolt_type bogus_lightning, *l;
	storm_type bogus_storm, *s;
	int temp;

	// parse the lightning table
	read_file_text("lightning.tbl");
	reset_parse();

	Num_bolt_types = 0;
	Num_storm_types = 0;

	memset(Bolt_types, 0, sizeof(bolt_type) * MAX_BOLT_TYPES_INTERNAL);

	// parse the individual lightning bolt types
	required_string("#Bolts begin");
	while(!optional_string("#Bolts end")){
		// get a pointer
		if(Num_bolt_types >= MAX_BOLT_TYPES){
			l = &bogus_lightning;
		} else {
			l = &Bolt_types[Num_bolt_types];
		}

		// bolt title
		required_string("$Bolt:");
		stuff_string(l->name, F_NAME, NULL);

		// b_scale
		required_string("+b_scale:");
		stuff_float(&l->b_scale);

		// b_shrink
		required_string("+b_shrink:");
		stuff_float(&l->b_shrink);

		// b_poly_pct
		required_string("+b_poly_pct:");
		stuff_float(&l->b_poly_pct);		

		// child rand
		required_string("+b_rand:");
		stuff_float(&l->b_rand);

		// z add
		required_string("+b_add:");
		stuff_float(&l->b_add);

		// # strikes
		required_string("+b_strikes:");
		stuff_int(&l->num_strikes);

		// lifetime
		required_string("+b_lifetime:");
		stuff_int(&l->lifetime);

		// noise
		required_string("+b_noise:");
		stuff_float(&l->noise);

		// emp effect
		required_string("+b_emp:");
		stuff_float(&l->emp_intensity);
		stuff_float(&l->emp_time);

		// texture
		required_string("+b_texture:");
		stuff_string(name, F_NAME, NULL);
		if((l != &bogus_lightning) && !Fred_running){
			l->texture = bm_load(name);
		}

		// glow
		required_string("+b_glow:");
		stuff_string(name, F_NAME, NULL);
		if((l != &bogus_lightning) && !Fred_running){
			l->glow = bm_load(name);
		}

		// brightness
		required_string("+b_bright:");
		stuff_float(&l->b_bright);

		// increment the # of bolt types
		if(l != &bogus_lightning){
			Num_bolt_types++;
		}
	}

	// copy the first bolt to the debug bolt
	memcpy(&Bolt_types[DEBUG_BOLT], &Bolt_types[0], sizeof(bolt_type));

	// parse storm types
	required_string("#Storms begin");
	while(!optional_string("#Storms end")){
		// get a pointer
		if(Num_storm_types >= MAX_STORM_TYPES){
			s = &bogus_storm;
		} else {
			s = &Storm_types[Num_storm_types];
		}

		// bolt title
		required_string("$Storm:");
		stuff_string(s->name, F_NAME, NULL);

		// bolt types
		s->num_bolt_types = 0;
		while(optional_string("+bolt:")){			
			stuff_string(name, F_NAME, NULL);			

			// fill this guy in
			if(s->num_bolt_types < MAX_BOLT_TYPES){
				s->bolt_types[s->num_bolt_types] = (char)nebl_get_bolt_index(name);
				Assert(s->bolt_types[s->num_bolt_types] != -1);								

				s->num_bolt_types++;
			} 
			// bogus 
			else {
				required_string("+bolt_prec:");
				stuff_int(&temp);
			}			
		}

		// flavor
		required_string("+flavor:");
		stuff_float(&s->flavor.xyz.x);
		stuff_float(&s->flavor.xyz.y);
		stuff_float(&s->flavor.xyz.z);

		// frequencies
		required_string("+random_freq:");
		stuff_int(&s->min);
		stuff_int(&s->max);

		// counts
		required_string("+random_count:");
		stuff_int(&s->min_count);
		stuff_int(&s->max_count);

		// increment the # of bolt types
		if(s != &bogus_storm){
			Num_storm_types++;
		}
	}
#endif
}

// initialize lightning before entering a level
void nebl_level_init()
{
	int idx;	

	// zero all lightning bolts
	for(idx=0; idx<MAX_LIGHTNING_BOLTS; idx++){
		Nebl_bolts[idx].head = NULL;
		Nebl_bolts[idx].bolt_life = -1;
		Nebl_bolts[idx].used = 0;
	}	
	
	// initialize node list
	Num_lnodes = 0;
	list_init( &Nebl_free_list );
	list_init( &Nebl_used_list );

	// Link all object slots into the free list
	for (idx=0; idx<MAX_LIGHTNING_NODES; idx++)	{
		list_append(&Nebl_free_list, &Nebl_nodes[idx] );
	}

	// zero the random timestamp
	Nebl_stamp = -1;		

	// null the storm. let mission parsing set it up
	Storm = NULL;
}

// render all lightning bolts
void nebl_render_all()
{
	int idx;
	l_bolt *b;
	bolt_type *bi;

	// no lightning in non-nebula missions
	if(!(The_mission.flags & MISSION_FLAG_FULLNEB)){
		return;
	}

	// if we have no storm
	if(Storm == NULL){
		return;
	}

	// traverse the list
	for(idx=0; idx<MAX_LIGHTNING_BOLTS; idx++){
		b = &Nebl_bolts[idx];		

		// if this is being used
		if(b->used){
			Assert(b->head != NULL);

			// bogus bolt
			if(b->head == NULL){
				b->used = 0;
				continue;
			}
			if((b->type < 0) || ((b->type >= Num_bolt_types) && (b->type != DEBUG_BOLT)) ){
				Int3();
				b->used = 0;
				continue;
			}
			bi = &Bolt_types[(int)b->type];

			// if this guy is still on a delay
			if(b->delay != -1){
				if(timestamp_elapsed(b->delay)){
					b->delay = -1;
				} else {
					continue;
				}
			}

			// if the timestamp on this guy has expired
			if((b->bolt_life < 0) || timestamp_elapsed(b->bolt_life)){
				// if this is a multiple strike bolt, jitter it and reset
				if(b->strikes_left-1 > 0){
					b->bolt_life = timestamp(bi->lifetime / bi->num_strikes);
					b->first_frame = 1;
					b->strikes_left--;
					nebl_jitter(b);

					// by continuing here we skip rendering for one frame, which makes it look more like real lightning
					continue;
				}
				// otherwise he's completely done, so release him
				else {
					// maybe free up node data
					if(b->head != NULL){
						nebl_release(b->head);
						b->head = NULL;

						Nebl_bolt_count--;

						nprintf(("lightning", "Released bolt. %d used nodes!\n", Num_lnodes));
					}

					b->used = 0;
				}
			}

			// pick some cool alpha values
			Nebl_alpha = frand();
			Nebl_glow_alpha = frand();

			// otherwise render him
			Nebl_flash_count = 0;
			Nebl_flash_x = 0.0f;
			Nebl_flash_y = 0.0f;
			Nebl_bang = 10000000.0f;
			nebl_render(bi, b->head, b->width);

			// if this is the first frame he has been rendered, determine if we need to make a flash and sound effect
			if(b->first_frame){
				float flash = 0.0f;				

				b->first_frame = 0;

				// if we rendered any points
				if(Nebl_flash_count){
					Nebl_flash_x /= (float)Nebl_flash_count;
					Nebl_flash_y /= (float)Nebl_flash_count;

					// quick distance from the center of the screen			
					float x = Nebl_flash_x - (gr_screen.max_w / 2.0f);
					float y = Nebl_flash_y - (gr_screen.max_h / 2.0f);
					float dist = fl_sqrt((x * x) + (y * y));		
					if(dist / (gr_screen.max_w / 2.0f) < 1.0f){
						flash = 1.0f - (dist / (gr_screen.max_w / 2.0f));										

						// scale the flash by bolt type
						flash *= bi->b_bright;

						game_flash(flash, flash, flash);										
					}					

					// do some special stuff on the very first strike of the bolt
					if(b->strikes_left == bi->num_strikes){					
						// play a sound						
						float bang;
						if(Nebl_bang < 40.0f){
							bang = 1.0f;
						} else if(Nebl_bang > 400.0f){
							bang = 0.0f;
						} else {
							bang = 1.0f - (Nebl_bang / 400.0f);
						}
						if(frand_range(0.0f, 1.0f) < 0.5f){
							snd_play(&Snds[SND_LIGHTNING_2], 0.0f, bang, SND_PRIORITY_DOUBLE_INSTANCE);
						} else {
							snd_play(&Snds[SND_LIGHTNING_1], 0.0f, bang, SND_PRIORITY_DOUBLE_INSTANCE);
						}						

						// apply em pulse
						if(bi->emp_intensity > 0.0f){
							emp_apply(&b->midpoint, 0.0f, vm_vec_dist(&b->start, &b->strike), bi->emp_intensity, bi->emp_time);
						}
					}
				}				
			}
		}
	}	
}

// process lightning (randomly generate bolts, etc, etc);
void nebl_process()
{		
	int num_bolts, idx;

	// non-nebula mission
	if(!(The_mission.flags & MISSION_FLAG_FULLNEB)){
		return;
	}		
	
	// non servers in multiplayer don't do this
	if((Game_mode & GM_MULTIPLAYER) && !MULTIPLAYER_MASTER){
		return;
	}

	// if there's no chosen storm
	if(Storm == NULL){
		return;
	}

	// don't process lightning bolts unless we're a few seconds in
	if(f2fl(Missiontime) < 3.0f){
		return;
	}
		
	// random stamp
	if(Nebl_stamp == -1){
		Nebl_stamp = timestamp((int)frand_range((float)Storm->min, (float)Storm->max));
		return;
	}	

	// maybe make a bolt
	if(timestamp_elapsed(Nebl_stamp)){
		// determine how many bolts to spew
		num_bolts = (int)frand_range((float)Storm->min_count, (float)Storm->max_count);
		for(idx=0; idx<num_bolts; idx++){
			// hmm. for now just pick a random bolt type and run with it
			int s1, s2, s3;
			int e1, e2, e3;
			do {
				s1 = (int)frand_range(0.0f, (float)Neb2_slices);
				s2 = (int)frand_range(0.0f, (float)Neb2_slices);
				s3 = (int)frand_range(0.0f, (float)Neb2_slices);

				e1 = (int)frand_range(0.0f, (float)Neb2_slices);
				e2 = (int)frand_range(0.0f, (float)Neb2_slices);
				e3 = (int)frand_range(0.0f, (float)Neb2_slices);
			
				// never choose the middle cube
				if((s1 == 2) && (s2 == 2) && (s3 == 2)){
					s1 = 4;
					s2 = 0;
				}
				if((e1 == 2) && (e2 == 2) && (e3 == 2)){
					e1 = 0;
					e2 = 4;
				}

			// sanity
			} while((s1 == e1) && (s2 == e2) && (s3 == e3));

			vector start = Neb2_cubes[s1][s2][s3].pt;
			vector strike = Neb2_cubes[e1][e2][e3].pt;

			// add some flavor to the bolt. mmmmmmmm, lightning
			if(!IS_VEC_NULL(&Storm->flavor)){			
				// start with your basic hot sauce. measure how much you have			
				vector your_basic_hot_sauce;
				vm_vec_sub(&your_basic_hot_sauce, &strike, &start);
				float how_much_hot_sauce = vm_vec_normalize(&your_basic_hot_sauce);

				// now figure out how much of that good wing sauce to add
				vector wing_sauce = Storm->flavor;
				if(frand_range(0.0, 1.0f) < 0.5f){
					vm_vec_scale(&wing_sauce, -1.0f);
				}
				float how_much_of_that_good_wing_sauce_to_add = vm_vec_normalize(&wing_sauce);

				// mix the two together, taking care not to add too much
				vector the_mixture;
				if(how_much_of_that_good_wing_sauce_to_add > 1000.0f){
					how_much_of_that_good_wing_sauce_to_add = 1000.0f;
				}
				vm_vec_interp_constant(&the_mixture, &your_basic_hot_sauce, &wing_sauce, how_much_of_that_good_wing_sauce_to_add / 1000.0f);

				// take the final sauce and store it in the proper container
				vm_vec_scale(&the_mixture, how_much_hot_sauce);

				// make sure to put it on everything! whee!			
				vm_vec_add(&strike, &start, &the_mixture);
			}

			int type = (int)frand_range(0.0f, (float)(Storm->num_bolt_types-1));
			nebl_bolt(Storm->bolt_types[type], &start, &strike);
		}

		// reset the timestamp
		Nebl_stamp = timestamp((int)frand_range((float)Storm->min, (float)Storm->max));
	}	
}

// create a lightning bolt
void nebl_bolt(int type, vector *start, vector *strike)
{
	vector dir;
	l_bolt *bolt;
	l_node *tail;
	int idx;
	int found;		
	bolt_type *bi;
	float bolt_len;

	if(!(The_mission.flags & MISSION_FLAG_FULLNEB)){
		return;
	}

	// find a free bolt
	found = 0;
	for(idx=0; idx<MAX_LIGHTNING_BOLTS; idx++){
		if(!Nebl_bolts[idx].used){
			found = 1;
			break;
		}
	}
	if(!found){
		// Int3();
		return;
	}

	if((type < 0) || ((type >= Num_bolt_types) && (type != DEBUG_BOLT)) ){
		return;
	}
	bi = &Bolt_types[type];	

	// get a pointer to the bolt
	bolt = &Nebl_bolts[idx];	

	// setup bolt into
	bolt->start = *start;
	bolt->strike = *strike;
	bolt->strikes_left = bi->num_strikes;
	bolt->delay = -1;
	bolt->type = (char)type;
	bolt->first_frame = 1;
	bolt->bolt_life = timestamp(bi->lifetime / bi->num_strikes);		

	Nebl_bolt_start = *start;
	Nebl_bolt_strike = *strike;

	// setup fire delay
	if(bolt->delay != -1){
		bolt->delay = timestamp(bolt->delay);
	}

	// setup the rest of the important bolt data
	if(vm_vec_same(&Nebl_bolt_start, &Nebl_bolt_strike)){
		Nebl_bolt_strike.xyz.z += 150.0f;
	}
	Nebl_bolt_len = vm_vec_dist(&Nebl_bolt_start, &Nebl_bolt_strike);	
	vm_vec_sub(&dir, &Nebl_bolt_strike, &Nebl_bolt_start);

	// setup midpoint
	vm_vec_scale_add(&bolt->midpoint, &Nebl_bolt_start, &dir, 0.5f);

	bolt_len = vm_vec_normalize(&dir);
	vm_vector_2_matrix(&Nebl_bolt_dir, &dir, NULL, NULL);

	// global type for generating the bolt
	Nebl_type = bi;

	// try and make the bolt
	if(!nebl_gen(&Nebl_bolt_start, &Nebl_bolt_strike, 0, 4, 0, &bolt->head, &tail)){
		if(bolt->head != NULL){
			nebl_release(bolt->head);
		}

		return;
	}

	Nebl_bolt_count++;	
	
	// setup the rest of the data	
	bolt->used = 1;	
	bolt->width = bi->b_poly_pct * bolt_len;

	// if i'm a multiplayer master, send a bolt packet
	if(MULTIPLAYER_MASTER){
		send_lightning_packet(type, start, strike);
	}
}

// get the current # of active lightning bolts
int nebl_get_active_bolts()
{
	return Nebl_bolt_count;
}

// get the current # of active nodes
int nebl_get_active_nodes()
{
	return Num_lnodes;
}

// set the storm (call from mission parse)
void nebl_set_storm(char *name)
{
	int index = nebl_get_storm_index(name);

	// sanity
	Storm = NULL;
	if((index >= 0) && (index < Num_storm_types)){
		Storm = &Storm_types[index];
	}
}

// ------------------------------------------------------------------------------------------------------
// NEBULA LIGHTNING FORWARD DEFINITIONS
//

// "new" a lightning node
l_node *nebl_new()
{
	l_node *lp;

	// if we're out of nodes
	if(Num_lnodes >= MAX_LIGHTNING_NODES){
		// Int3();
		nprintf(("lightning", "Out of lightning nodes!\n"));
		return NULL;
	}

	// get a new node off the freelist
	lp = GET_FIRST(&Nebl_free_list);
	Assert( lp != &Nebl_free_list );		// shouldn't have the dummy element

	// remove trailp from the free list
	list_remove( &Nebl_free_list, lp );
	
	// insert trailp onto the end of used list
	list_append( &Nebl_used_list, lp );

	// increment counter
	Num_lnodes++;

	lp->links[0] = NULL;
	lp->links[1] = NULL;
	lp->links[2] = NULL;	

	// return the pointer
	return lp;
}

// "delete" a lightning node
void nebl_delete(l_node *lp)
{
	// remove objp from the used list
	list_remove( &Nebl_used_list, lp );

	// add objp to the end of the free
	list_append( &Nebl_free_list, lp );

	// decrement counter
	Num_lnodes--;
}

// free a lightning bolt
void nebl_release(l_node *whee)
{
	// if we're invalid
	if(whee == NULL){
		return;
	}

	// release all of our children
	if(whee->links[LINK_RIGHT] != NULL){
		nebl_release(whee->links[LINK_RIGHT]);
	}	
	if(whee->links[LINK_CHILD] != NULL){
		nebl_release(whee->links[LINK_CHILD]);
	}	

	// delete this node
	nebl_delete(whee);
}

int nebl_gen(vector *left, vector *right, float depth, float max_depth, int child, l_node **l_left, l_node **l_right)
{
	l_node *child_node = NULL;
	float d = vm_vec_dist_quick( left, right );		

	// if we've reached the critical point
	if ( d < 0.30f || (depth > max_depth) ){
		// generate ne items
		l_node *new_left = nebl_new();
		if(new_left == NULL){
			return 0;
		}		
		new_left->links[0] = NULL; new_left->links[1] = NULL; new_left->links[2] = NULL;
		new_left->pos = vmd_zero_vector;
		l_node *new_right = nebl_new();
		if(new_right == NULL){
			nebl_delete(new_left);			
			return 0;
		}		
		new_right->links[0] = NULL; new_right->links[1] = NULL; new_right->links[2] = NULL;
		new_right->pos = vmd_zero_vector;

		// left side
		new_left->pos = *left;		
		new_left->links[LINK_RIGHT] = new_right;		
		*l_left = new_left;
		
		// right side
		new_right->pos = *right;
		new_right->links[LINK_LEFT] = new_left;
		*l_right = new_right;

		// done
		return 1;
	}  

	// divide in half
	vector tmp;
	vm_vec_avg( &tmp, left, right );

	// sometimes generate children
	if(!child && (frand() <= Nebl_type->b_rand)){
		// get a point on the plane of the strike
		vector tmp2;
		vm_vec_random_in_circle(&tmp2, &Nebl_bolt_strike, &Nebl_bolt_dir, Nebl_bolt_len * Nebl_type->b_scale, 0);

		// maybe move away from the plane
		vector dir;
		vm_vec_sub(&dir, &tmp2, &tmp);		
		vm_vec_scale_add(&tmp2, &tmp, &dir, Nebl_type->b_shrink);

		// child
		l_node *argh;		
		if(!nebl_gen(&tmp, &tmp2, 0, 2, 1, &child_node, &argh)){
			if(child_node != NULL){
				nebl_release(child_node);
			}
			return 0;
		}
	}
	
	float scaler = 0.30f;
	tmp.xyz.x += (frand()-0.5f)*d*scaler;
	tmp.xyz.y += (frand()-0.5f)*d*scaler;
	tmp.xyz.z += (frand()-0.5f)*d*scaler;

	// generate left half
	l_node *ll = NULL;
	l_node *lr = NULL;
	if(!nebl_gen( left, &tmp, depth+1, max_depth, child, &ll, &lr )){
		if(child_node != NULL){
			nebl_release(child_node);
		}
		if(ll != NULL){
			nebl_release(ll);
		}
		return 0;
	}

	// generate right half
	l_node *rl = NULL;
	l_node *rr = NULL;
	if(!nebl_gen( &tmp, right, depth+1, max_depth, child, &rl, &rr )){
		if(child_node != NULL){
			nebl_release(child_node);
		}
		if(ll != NULL){
			nebl_release(ll);
		}
		if(rl != NULL){
			nebl_release(rl);
		}
		return 0;
	}
	
	// splice the two together
	lr->links[LINK_RIGHT] = rl->links[LINK_RIGHT];
	lr->links[LINK_RIGHT]->links[LINK_LEFT] = lr;
	nebl_delete(rl);

	// if we generated a child, stick him on
	if(child_node != NULL){
		lr->links[LINK_CHILD] = child_node;
	}

	// return these
	*l_left = ll;
	*l_right = rr;

	return 1;
}


// output top and bottom vectors
// fvec == forward vector (eye viewpoint basically. in world coords)
// pos == world coordinate of the point we're calculating "around"
// w == width of the diff between top and bottom around pos
void nebl_calc_facing_pts_smart( vector *top, vector *bot, vector *fvec, vector *pos, float w, float z_add )
{
	vector uvec, rvec;
	vector temp;	

	temp = *pos;

	vm_vec_sub( &rvec, &Eye_position, &temp );
	vm_vec_normalize( &rvec );	

	vm_vec_crossprod(&uvec,fvec,&rvec);
	vm_vec_normalize(&uvec);

	vm_vec_scale_add( top, &temp, &uvec, w/2.0f );
	vm_vec_scale_add( bot, &temp, &uvec, -w/2.0f );	
	
	vm_vec_scale_add2( top, &rvec, z_add );
	vm_vec_scale_add2( bot, &rvec, z_add );
}

// render a section of the bolt
void nebl_render_section(bolt_type *bi, l_section *a, l_section *b)
{		
	vertex v[4];
	vertex *verts[4] = {&v[0], &v[1], &v[2], &v[3]};
	int idx;

	// Sets mode.  Returns previous mode.
	gr_zbuffer_set(GR_ZBUFF_FULL);	

	// draw some stuff
	for(idx=0; idx<2; idx++){		
		v[0] = a->vex[idx];		
		v[0].u = 0.0f; v[0].v = 0.0f;

		v[1] = a->vex[idx+1];		
		v[1].u = 1.0f; v[1].v = 0.0f;

		v[2] = b->vex[idx+1];		
		v[2].u = 1.0f; v[2].v = 1.0f;

		v[3] = b->vex[idx];		
		v[3].u = 0.0f; v[3].v = 1.0f;

		// draw
		gr_set_bitmap(bi->texture, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, Nebl_alpha, -1, -1);
		g3_draw_poly(4, verts, TMAP_FLAG_TEXTURED | TMAP_FLAG_CORRECT);		
	}

	// draw
	v[0] = a->vex[2];		
	v[0].u = 0.0f; v[0].v = 0.0f;

	v[1] = a->vex[0];		
	v[1].u = 1.0f; v[1].v = 0.0f;

	v[2] = b->vex[0];		
	v[2].u = 1.0f; v[2].v = 1.0f;

	v[3] = b->vex[2];		
	v[3].u = 0.0f; v[3].v = 1.0f;

	gr_set_bitmap(bi->texture, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, Nebl_alpha, -1, -1);
	g3_draw_poly(4, verts, TMAP_FLAG_TEXTURED | TMAP_FLAG_CORRECT);	

	// draw the glow beam	
	verts[0] = &a->glow_vex[0];
	verts[0]->v = 0.0f; verts[0]->u = 0.0f;

	verts[1] = &a->glow_vex[1];
	verts[1]->v = 1.0f; verts[1]->u = 0.0f;

	verts[2] = &b->glow_vex[1];
	verts[2]->v = 1.0f; verts[2]->u = 1.0f;

	verts[3] = &b->glow_vex[0];
	verts[3]->v = 0.0f; verts[3]->u = 1.0f;

	gr_set_bitmap(bi->glow, GR_ALPHABLEND_FILTER, GR_BITBLT_MODE_NORMAL, Nebl_glow_alpha, -1, -1);
	g3_draw_poly(4, verts, TMAP_FLAG_TEXTURED | TMAP_FLAG_CORRECT);	
}

// generate a section
void nebl_generate_section(bolt_type *bi, float width, l_node *a, l_node *b, l_section *c, l_section *cap, int pinch_a, int pinch_b)
{
	vector dir;
	vector dir_normal;
	matrix m;
	int idx;	
	vector temp, pt;
	vector glow_a, glow_b;

	// direction matrix
	vm_vec_sub(&dir, &a->pos, &b->pos);
	vm_vec_copy_normalize(&dir_normal, &dir);
	vm_vector_2_matrix(&m, &dir_normal, NULL, NULL);

	// distance to player
	float bang_dist = vm_vec_dist_quick(&Eye_position, &a->pos);
	if(bang_dist < Nebl_bang){
		Nebl_bang = bang_dist;
	}

	// rotate the basic section into world	
	for(idx=0; idx<3; idx++){
		// rotate to world		
		if(pinch_a){			
			vm_vec_rotate(&pt, &Nebl_ring_pinched[idx], &m);
		} else {
			vm_vec_copy_scale(&temp, &Nebl_ring[idx], width);
			vm_vec_rotate(&pt, &temp, &m);
		}
		vm_vec_add2(&pt, &a->pos);
			
		// transform
		g3_rotate_vertex(&c->vex[idx], &pt);
		g3_project_vertex(&c->vex[idx]);		

		// if first frame, keep track of the average screen pos
		if((c->vex[idx].sx >= 0) && (c->vex[idx].sx < gr_screen.max_w) && (c->vex[idx].sy >= 0) && (c->vex[idx].sy < gr_screen.max_h)){
			Nebl_flash_x += c->vex[idx].sx;
			Nebl_flash_y += c->vex[idx].sy;
			Nebl_flash_count++;
		}
	}
	// calculate the glow points		
	nebl_calc_facing_pts_smart(&glow_a, &glow_b, &dir_normal, &a->pos, pinch_a ? 0.5f : width * 6.0f, Nebl_type->b_add);
	g3_rotate_vertex(&c->glow_vex[0], &glow_a);
	g3_project_vertex(&c->glow_vex[0]);
	g3_rotate_vertex(&c->glow_vex[1], &glow_b);
	g3_project_vertex(&c->glow_vex[1]);	

	// maybe do a cap
	if(cap != NULL){		
		// rotate the basic section into world
		for(idx=0; idx<3; idx++){
			// rotate to world		
			if(pinch_b){
				vm_vec_rotate(&pt, &Nebl_ring_pinched[idx], &m);
			} else {
				vm_vec_copy_scale(&temp, &Nebl_ring[idx], width);
				vm_vec_rotate(&pt, &temp, &m);		
			}
			vm_vec_add2(&pt, &b->pos);
			
			// transform
			g3_rotate_vertex(&cap->vex[idx], &pt);
			g3_project_vertex(&cap->vex[idx]);			

			// if first frame, keep track of the average screen pos			
			if( (c->vex[idx].sx >= 0) && (c->vex[idx].sx < gr_screen.max_w) && (c->vex[idx].sy >= 0) && (c->vex[idx].sy < gr_screen.max_h)){
				Nebl_flash_x += c->vex[idx].sx;
				Nebl_flash_y += c->vex[idx].sy;
				Nebl_flash_count++;
			}
		}
		
		// calculate the glow points		
		nebl_calc_facing_pts_smart(&glow_a, &glow_b, &dir_normal, &b->pos, pinch_b ? 0.5f : width * 6.0f, bi->b_add);
		g3_rotate_vertex(&cap->glow_vex[0], &glow_a);
		g3_project_vertex(&cap->glow_vex[0]);
		g3_rotate_vertex(&cap->glow_vex[1], &glow_b);
		g3_project_vertex(&cap->glow_vex[1]);
	}
}

// render the bolt
void nebl_render(bolt_type *bi, l_node *whee, float width, l_section *prev)
{		
	l_section start;
	l_section end;
	l_section child_start;

	// bad
	if(whee == NULL){
		return;
	}

	// if prev is NULL, we're just starting so we need our start point
	if(prev == NULL){
		Assert(whee->links[LINK_RIGHT] != NULL);
		nebl_generate_section(bi, width, whee, whee->links[LINK_RIGHT], &start, NULL, 1, 0);
	} else {
		start = *prev;
	}
	
	// if we have a child section	
	if(whee->links[LINK_CHILD]){		
		// generate section
		nebl_generate_section(bi, width * 0.5f, whee, whee->links[LINK_CHILD], &child_start, &end, 0, whee->links[LINK_CHILD]->links[LINK_RIGHT] == NULL ? 1 : 0);

		// render
		nebl_render_section(bi, &child_start, &end);			

		// maybe continue
		if(whee->links[LINK_CHILD]->links[LINK_RIGHT] != NULL){
			nebl_render(bi, whee->links[LINK_CHILD], width * 0.5f, &end);
		}
	}	
		
	// if the next section is an end section
	if(whee->links[LINK_RIGHT]->links[LINK_RIGHT] == NULL){
		l_section temp;

		// generate section
		nebl_generate_section(bi, width, whee, whee->links[LINK_RIGHT], &temp, &end, 0, 1);

		// render the section
		nebl_render_section(bi, &start, &end);		
	}
	// a middle section
	else if(whee->links[LINK_RIGHT]->links[LINK_RIGHT] != NULL){
		// generate section
		nebl_generate_section(bi, width, whee->links[LINK_RIGHT], whee->links[LINK_RIGHT]->links[LINK_RIGHT], &end, NULL, 0, 0);

		// render the section
		nebl_render_section(bi, &start, &end);

		// recurse through him
		nebl_render(bi, whee->links[LINK_RIGHT], width, &end);
	}
}

// given a valid, complete bolt, jitter him based upon his noise
void nebl_jitter(l_bolt *b)
{
	matrix m;
	vector temp;
	float length;
	l_node *moveup;
	bolt_type *bi = NULL;

	// sanity
	if(b == NULL){
		return;
	}
	if((b->type < 0) || ((b->type >= Num_bolt_types) && (b->type != DEBUG_BOLT)) ){
		return;		
	}
	bi = &Bolt_types[(int)b->type];

	// get the bolt direction
	vm_vec_sub(&temp, &b->strike, &b->start);
	length = vm_vec_normalize_quick(&temp);
	vm_vector_2_matrix(&m, &temp, NULL, NULL);

	// jitter all nodes on the main trunk
	moveup = b->head;
	while(moveup != NULL){
		temp = moveup->pos;
		vm_vec_random_in_circle(&moveup->pos, &temp, &m, frand_range(0.0f, length * bi->noise), 0);

		// just on the main trunk
		moveup = moveup->links[LINK_RIGHT];
	}	
}

// return the index of a given bolt type by name
int nebl_get_bolt_index(char *name)
{
	int idx;

	for(idx=0; idx<Num_bolt_types; idx++){
		if(!strcmp(name, Bolt_types[idx].name)){
			return idx;
		}
	}

	return -1;
}

// return the index of a given storm type by name
int nebl_get_storm_index(char *name)
{
	int idx;

	for(idx=0; idx<Num_bolt_types; idx++){
		if(!strcmp(name, Storm_types[idx].name)){
			return idx;
		}
	}

	return -1;
}

