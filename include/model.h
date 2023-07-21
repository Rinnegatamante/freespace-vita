/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Model/MODEL.H $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * header file for information about polygon models
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/03 13:34:33  theoddone33
 * More stuff compiles
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 38    9/13/99 10:09a Andsager
 * Add debug console commands to lower model render detail and fireball
 * LOD for big ship explosiosns.
 * 
 * 37    9/01/99 10:09a Dave
 * Pirate bob.
 * 
 * 36    8/24/99 8:55p Dave
 * Make sure nondimming pixels work properly in tech menu.
 * 
 * 35    7/19/99 12:02p Andsager
 * Allow AWACS on any ship subsystem. Fix sexp_set_subsystem_strength to
 * only blow up subsystem if its strength is > 0
 * 
 * 34    7/15/99 2:13p Dave
 * Added 32 bit detection.
 * 
 * 33    7/06/99 10:45a Andsager
 * Modify engine wash to work on any ship that is not small.  Add AWACS
 * ask for help.
 * 
 * 32    6/18/99 5:16p Dave
 * Added real beam weapon lighting. Fixed beam weapon sounds. Added MOTD
 * dialog to PXO screen.
 * 
 * 31    5/26/99 11:46a Dave
 * Added ship-blasting lighting and made the randomization of lighting
 * much more customizable.
 * 
 * 30    5/26/99 9:09a Andsager
 * Increase number of live debris and MAX_MODEL_TEXTURES (for rebel base)
 * 
 * 29    5/24/99 5:45p Dave
 * Added detail levels to the nebula, with a decent speedup. Split nebula
 * lightning into its own section.
 * 
 * 28    5/11/99 10:16p Andsager
 * First pass on engine wash effect.  Rotation (control input), damage,
 * shake.  
 * 
 * 27    4/26/99 8:49p Dave
 * Made all pof based nebula stuff full customizable through fred.
 * 
 * 26    4/23/99 5:53p Dave
 * Started putting in new pof nebula support into Fred.
 * 
 * 25    4/19/99 12:51p Andsager
 * Add function to find the nearest point on extneded bounding box and
 * check if inside bounding box.
 * 
 * 24    4/06/99 9:50a Enricco
 * Bump up max live_debris from 4 to 5
 * 
 * 23    3/31/99 8:24p Dave
 * Beefed up all kinds of stuff, incluging beam weapons, nebula effects
 * and background nebulae. Added per-ship non-dimming pixel colors.
 * 
 * 22    3/23/99 5:17p Dave
 * Changed model file format somewhat to account for LOD's on insignias
 * 
 * 21    3/19/99 9:51a Dave
 * Checkin to repair massive source safe crash. Also added support for
 * pof-style nebulae, and some new weapons code.
 * 
 * 20    3/08/99 7:03p Dave
 * First run of new object update system. Looks very promising.
 * 
 * 19    3/02/99 9:25p Dave
 * Added a bunch of model rendering debug code. Started work on fixing
 * beam weapon wacky firing.
 * 
 * 18    2/19/99 11:42a Dave
 * Put in model rendering autocentering.
 * 
 * 17    2/19/99 11:09a Jasen
 * Upped MAX_MODEL_SUBSYSTEMS to 128 (for the souper cap
 * 
 * 16    1/25/99 5:03a Dave
 * First run of stealth, AWACS and TAG missile support. New mission type
 * :)
 * 
 * 15    1/14/99 6:06p Dave
 * 100% full squad logo support for single player and multiplayer.
 * 
 * 14    1/12/99 12:53a Dave
 * More work on beam weapons - made collision detection very efficient -
 * collide against all object types properly - made 3 movement types
 * smooth. Put in test code to check for possible non-darkening pixels on
 * object textures.
 * 
 * 13    1/11/99 12:56p Andsager
 * stupid merge error
 * 
 * 12    1/11/99 12:42p Andsager
 * Add live debris - debris which is created from a destroyed subsystem,
 * when the ship is still alive
 * 
 * 11    1/08/99 2:08p Dave
 * Fixed software rendering for pofview. Super early support for AWACS and
 * beam weapons.
 * 
 * 10    12/23/98 2:53p Andsager
 * Added stepped rotation support
 * 
 * 9     12/09/98 7:34p Dave
 * Cleanup up nebula effect. Tweaked many values.
 * 
 * 8     12/04/98 3:34p Andsager
 * Handle norotating submodels
 * 
 * 7     12/03/98 3:14p Andsager
 * Check in code that checks rotating submodel actually has ship subsystem
 * 
 * 6     11/19/98 11:07p Andsager
 * Check in of physics and collision detection of rotating submodels
 * 
 * 5     11/11/98 5:37p Dave
 * Checkin for multiplayer testing.
 * 
 * 4     10/23/98 3:03p Andsager
 * Initial support for changing rotation rate.
 * 
 * 3     10/16/98 9:40a Andsager
 * Remove ".h" files from model.h
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 159   8/28/98 3:29p Dave
 * EMP effect done. AI effects may need some tweaking as required.
 * 
 * 158   5/19/98 8:31p Andsager
 * Added split planes (for big ship explosions)
 * 
 * 157   5/07/98 5:39p Andsager
 * Changes to model to hold cross section info
 * 
 * 156   4/22/98 9:58p John
 * Added code to view invisible faces.
 * 
 * 155   4/22/98 9:43p John
 * Added code to allow checking of invisible faces, flagged by any texture
 * name with invisible in it.
 * 
 * 154   4/01/98 5:34p John
 * Made only the used POFs page in for a level.   Reduced some interp
 * arrays.    Made custom detail level work differently.
 * 
 * 153   3/31/98 5:18p John
 * Removed demo/save/restore.  Made NDEBUG defined compile.  Removed a
 * bunch of debug stuff out of player file.  Made model code be able to
 * unload models and malloc out only however many models are needed.
 *  
 * 
 * 152   3/31/98 11:19a Allender
 * upped MAX_MODEL_SUBSYSTEMS to a resonable value
 * 
 * 151   3/31/98 11:02a Adam
 * upped MAX_MODEL_SUBSYSTEMS to 33
 * 
 * 150   3/25/98 11:23a Mike
 * Fix stack overwrite due to async between MAX_SECONDARY_WEAPONS and
 * MAX_WL_SECONDARY.
 * 
 * 149   3/24/98 10:11p Sandeep
 * 
 * 148   3/24/98 4:03p Lawrance
 * JOHN: Fix up outline drawing code to support different colors
 * 
 * 147   3/22/98 10:19a Adam
 * upped subsystem & secondary weapon constants for Frank's new capital
 * ship.
 * 
 * 146   3/21/98 3:33p Lawrance
 * Allow model outline color to be set directly
 * 
 * 145   3/19/98 5:24p John
 * Added code to find the closest point on a model to another point.  Used
 * this for detail levels and for targetting info.
 * 
 * 144   3/16/98 4:51p John
 * Added low-level code to clip all polygons against an arbritary plane.
 * Took out all old model_interp_zclip and used this new method instead.  
 * 
 * 143   3/13/98 9:06a John
 * Made missile thrusters use a different min dist for scaling.   Made
 * missilies not use lighting.
 * 
 * 142   3/02/98 5:42p John
 * Removed WinAVI stuff from Freespace.  Made all HUD gauges wriggle from
 * afterburner.  Made gr_set_clip work good with negative x &y.  Made
 * model_caching be on by default.  Made each cached model have it's own
 * bitmap id.  Made asteroids not rotate when model_caching is on.  
 * 
 * 141   2/24/98 5:04p Allender
 * allow different ship classes to use the same model.  Lot's of subsystem
 * stuff to deal with
 * 
 * 140   2/24/98 1:58p John
 * Made asteroids use model_caching.  Made asteroids darken with distance.
 * 
 * 139   1/29/98 5:50p John
 * Made electrical arcing on debris pieces be persistent from frame to
 * frame
 * 
 * 138   1/27/98 11:02a John
 * Added first rev of sparks.   Made all code that calls model_render call
 * clear_instance first.   Made debris pieces not render by default when
 * clear_instance is called.
 * 
 * 137   1/23/98 5:08p John
 * Took L out of vertex structure used B (blue) instead.   Took all small
 * fireballs out of fireball types and used particles instead.  Fixed some
 * debris explosion things.  Restructured fireball code.   Restructured
 * some lighting code.   Made dynamic lighting on by default. Made groups
 * of lasers only cast one light.  Made fireballs not cast light.
 * 
 * 136   1/15/98 3:28p John
 * Took out the unused ptnorms array.  Saved a measly 8MB of RAM,  so it
 * might not have been worth ripping out.  :-)
 * 
 * 135   1/09/98 11:25a John
 * Took the thruster animation stuff out of the model.
 * 
 * 134   12/31/97 2:35p John
 * Added code for core_radius
 * 
 * 133   12/22/97 8:55a John
 * added parameters for checking models.
 * 
 * 132   12/17/97 5:11p John
 * Added brightening back into fade table.  Added code for doing the fast
 * dynamic gun flashes and thruster flashes.
 * 
 * 131   12/17/97 9:54a John
 * added code for precalculated weapon flash lighting.
 * 
 * 130   12/05/97 3:46p John
 * made ship thruster glow scale instead of being an animation.
 * 
 *
 * $NoKeywords: $
 */

#ifndef _MODEL_H
#define _MODEL_H

#include "pstypes.h"

struct object;

#define MAX_PRIMARY_BANKS		3
#define MAX_SECONDARY_BANKS	4	//	Lowered from 5 to 4 by MK on 3/25/98.  This needs to be <= MAX_WL_SECONDARY or you'll get stack overwrites.

#define MAX_POLYGON_MODELS	128
#define MAX_DEBRIS_OBJECTS	32
#define MAX_MODEL_TEXTURES	64
#define FILENAME_LEN			64
#define MAX_MODEL_DETAIL_LEVELS	8
#define MAX_PROP_LEN			256
#define MAX_NAME_LEN			32
#define MAX_ARC_EFFECTS		8

#define MOVEMENT_TYPE_NONE				-1
#define MOVEMENT_TYPE_POS				0
#define MOVEMENT_TYPE_ROT				1
#define MOVEMENT_TYPE_ROT_SPECIAL	2		// for turrets only

// DA 11/13/98 Reordered to account for difference between max and game
#define MOVEMENT_AXIS_NONE	-1
#define MOVEMENT_AXIS_X		0
#define MOVEMENT_AXIS_Y		2
#define MOVEMENT_AXIS_Z		1

#define MAX_ROTATING_SUBMODELS 10

// defines for special objects like gun and missile points, docking point, etc
// Hoffoss: Please make sure that subsystem NONE is always index 0, and UNKNOWN is
// always the last subsystem in the list.  Also, make sure that MAX is correct.
// Otherwise, problems will arise in Fred.

#define SUBSYSTEM_NONE				0
#define SUBSYSTEM_ENGINE			1
#define SUBSYSTEM_TURRET			2
#define SUBSYSTEM_RADAR				3
#define SUBSYSTEM_NAVIGATION		4
#define SUBSYSTEM_COMMUNICATION	5
#define SUBSYSTEM_WEAPONS			6
#define SUBSYSTEM_SENSORS			7
#define SUBSYSTEM_SOLAR				8
#define SUBSYSTEM_GAS_COLLECT		9
#define SUBSYSTEM_ACTIVATION		10
#define SUBSYSTEM_UNKNOWN			11

#define SUBSYSTEM_MAX				12				//	maximum value for subsystem_xxx, for error checking

#define MAX_TFP						4				// maximum number of turret firing points

#define MAX_SPLIT_PLANE				3				// number of artist specified split planes (used in big ship explosions)

// Data specific to a particular instance of a submodel.  This gets stuffed/unstuffed using
// the model_clear_instance, model_set_instance, model_get_instance functions.
typedef struct submodel_instance_info {
	int		blown_off;								// If set, this subobject is blown off
	angles	angs;										// The current angle this thing is turned to.
	angles	prev_angs;
	vector	pt_on_axis;								// in ship RF
	float		cur_turn_rate;
	float		desired_turn_rate;
	float		turn_accel;
	int		axis_set;
	int		step_zero_timestamp;		// timestamp determines when next step is to begin (for stepped rotation)

} submodel_instance_info;

#define MAX_MODEL_SUBSYSTEMS		128				// used in ships.cpp (only place?) for local stack variable
															// when reading in ships.tbl

#define MSS_FLAG_ROTATES			(1<<0)		// This means the object rotates automatically with "turn_rate"
#define MSS_FLAG_STEPPED_ROTATE	(1<<1)		// This means that the rotation occurs in steps
#define MSS_FLAG_AI_ROTATE			(1<<2)		// This means that the rotation is controlled by ai
#define MSS_FLAG_CREWPOINT			(1<<3)		// If set, this is a crew point.
#define MSS_FLAG_TURRET_MATRIX	(1<<4)		// If set, this has it's turret matrix created correctly.
#define MSS_FLAG_AWACS				(1<<5)		// If set, this subsystem has AWACS capability

// definition of stepped rotation struct
typedef struct stepped_rotation {
	int num_steps;				// number of steps in complete revolution
	float fraction;			// fraction of time in step spent in accel
	float t_transit;			// time spent moving from one step to next
	float t_pause;				// time at rest between steps
	float max_turn_rate;		// max turn rate going betweens steps
	float max_turn_accel;	// max accel going between steps
} stepped_rotation_t;

typedef struct ai_rotation {
	void *p_rotation;
	int type;
	float dist;
	float speed;
} ai_rotation_t;

// definition for model subsystems.
typedef struct model_subsystem {					/* contains rotation rate info */
	uint		flags;									// See MSS_FLAG_* defines above
	char		name[MAX_NAME_LEN];					// name of the subsystem.  Probably displayed on HUD
	char		subobj_name[MAX_NAME_LEN];			// Temporary (hopefully) parameter used to match stuff in ships.tbl
	int		subobj_num;								// subobject number (from bspgen) -- used to match subobjects of subsystems to these entries
	int		model_num;								// Which model this is attached to (i.e. the polymodel[] index)
	int		type;										// type. see SUBSYSTEM_* types above.  A generic type thing
	vector	pnt;										// center point of this subsystem
	float		radius;									// the extent of the subsystem
	float		max_hits;								// maximum hits of this subsystem

	//	The following items are specific to turrets and will probably be moved to
	//	a separate struct so they don't take up space for all subsystem types.
	char		crewspot[MAX_NAME_LEN];				// unique identifying name for this turret -- used to assign AI class and multiplayer people
	int		turret_weapon_type;					// index in Weapon_info of weapon this fires
	vector	turret_norm;							//	direction this turret faces
	matrix	turret_matrix;							// turret_norm converted to a matrix.
	float		turret_fov;								//	dot of turret_norm:vec_to_enemy > this means can see
	int		turret_num_firing_points;			// number of firing points on this turret
	vector	turret_firing_point[MAX_TFP];		//	in parent object's reference frame, point from which to fire.
	int		turret_gun_sobj;						// Which subobject in this model the firing points are linked to.
	float		turret_turning_rate;					// How fast the turret turns. Read from ships.tbl

	// engine wash info
	char		engine_wash_index;					// index into Engine_wash_info

	// Rotation specific info
	float		turn_rate;								// The turning rate of this subobject, if MSS_FLAG_ROTATES is set.
	stepped_rotation_t *stepped_rotation;			// turn rotation struct
	ai_rotation_t *ai_rotation;						// ai controlled rotation struct

	// AWACS specific information
	float		awacs_intensity;						// awacs intensity of this subsystem
	float		awacs_radius;							// radius of effect of the AWACS

	int		primary_banks[MAX_PRIMARY_BANKS];					// default primary weapons -hoffoss
	int		secondary_banks[MAX_SECONDARY_BANKS];				// default secondary weapons -hoffoss
	int		secondary_bank_capacity[MAX_SECONDARY_BANKS];	// capacity of a bank -hoffoss
	int		path_num;								// path index into polymodel .paths array.  -2 if none exists, -1 if not defined
} model_subsystem;

typedef struct model_special {
	struct	model_special *next, *prev;		// for using as a linked list
	int		bank;										// used for sequencing gun/missile backs. approach/docking points
	int		slot;										// location for gun or missile in this bank
	vector	pnt;										// point where this special submodel thingy is at
	vector	norm;										// normal for the special submodel thingy
} model_special;

// model arc types
#define MARC_TYPE_NORMAL					0		// standard freespace 1 blue lightning arcs
#define MARC_TYPE_EMP						1		// EMP blast type arcs

#define MAX_LIVE_DEBRIS	7

typedef struct bsp_info {
	char		name[MAX_NAME_LEN];	// name of the subsystem.  Probably displayed on HUD
	int		movement_type;			// -1 if no movement, otherwise rotational or positional movement -- subobjects only
	int		movement_axis;			// which axis this subobject moves or rotates on.

	vector	offset;					// 3d offset from parent object

	int		bsp_data_size;
	ubyte		*bsp_data;

	vector	geometric_center;		// geometric center of this subobject.  In the same Frame Of 
	                              //  Reference as all other vertices in this submodel. (Relative to pivot point)
	float		rad;						// radius for each submodel

	vector	min;						// The min point of this object's geometry
	vector	max;						// The max point of this object's geometry
	vector	bounding_box[8];		// caclulated fron min/max

	int		blown_off;				// If set, this subobject is blown off. Stuffed by model_set_instance
	int		my_replacement;		// If not -1 this subobject is what should get rendered instead of this one
	int		i_replace;				// If this is not -1, then this subobject will replace i_replace when it is damaged
	angles	angs;						// The angles from parent.  Stuffed by model_set_instance

	int		is_live_debris;		// whether current submodel is a live debris model
	int		num_live_debris;		// num live debris models assocaiated with a submodel
	int		live_debris[MAX_LIVE_DEBRIS];	// array of live debris submodels for a submodel

	submodel_instance_info	*sii;	// stuff needed for collision from rotations

	int		is_thruster;
	int		is_damaged;

	// Tree info
	int		parent;					// what is parent for each submodel, -1 if none
	int		num_children;			// How many children this model has
	int		first_child;			// The first_child of this model, -1 if none
	int		next_sibling;			// This submodel's next sibling, -1 if none

	int		num_details;			// How many submodels are lower detail "mirrors" of this submodel
	int		details[MAX_MODEL_DETAIL_LEVELS];		// A list of all the lower detail "mirrors" of this submodel

	// Electrical Arc Effect Info
	// Sets a spark for this submodel between vertex v1 and v2	
	int		num_arcs;												// See model_add_arc for more info	
	vector	arc_pts[MAX_ARC_EFFECTS][2];	
	ubyte		arc_type[MAX_ARC_EFFECTS];							// see MARC_TYPE_* defines
} bsp_info;



#define MP_TYPE_UNUSED 0
#define MP_TYPE_SUBSYS 1

typedef struct mp_vert {
	vector		pos;				// xyz coordinates of vertex in object's frame of reference
	int			nturrets;		// number of turrets guarding this vertex
	int			*turret_ids;	// array of indices into ship_subsys linked list (can't index using [] though)
	float			radius;			// How far the closest obstruction is from this vertex
} mp_vert;

typedef struct model_path {
	char			name[MAX_NAME_LEN];					// name of the subsystem.  Probably displayed on HUD
	char			parent_name[MAX_NAME_LEN];			// parent name of submodel that path is linked to in POF
	int			parent_submodel;
	int			nverts;
	mp_vert		*verts;
	int			goal;			// Which of the verts is the one closest to the goal of this path
	int			type;			// What this path takes you to... See MP_TYPE_??? defines above for details
	int			value;		// This depends on the type.
									// For MP_TYPE_UNUSED, this means nothing.
									// For MP_TYPE_SUBSYS, this is the subsystem number this path takes you to.
} model_path;

typedef struct model_tmap_vert {
	short vertnum;
	short normnum;
	float u,v;
} model_tmap_vert;

// info for gun and missile banks.  Also used for docking points.  There should always
// only be two slots for each docking bay

#define MAX_SLOTS		25
#define MAX_THRUSTER_SLOTS		10

typedef struct w_bank {
	int		num_slots;
	vector	pnt[MAX_SLOTS];
	vector	norm[MAX_SLOTS];
	float		radius[MAX_SLOTS];
} w_bank;

typedef struct thruster_bank {
	int		num_slots;
	vector	pnt[MAX_THRUSTER_SLOTS];
	vector	norm[MAX_THRUSTER_SLOTS];
	float		radius[MAX_THRUSTER_SLOTS];

	// Engine wash info
	char		wash_info_index;			// index into Engine_wash_info
} thruster_bank;
	

// defines for docking bay things.  The types are essentially flags since docking bays can probably
// be used for multiple things in some cases (i.e. rearming and general docking)

#define DOCK_TYPE_CARGO				(1<<0)
#define DOCK_TYPE_REARM				(1<<1)
#define DOCK_TYPE_GENERIC			(1<<2)

#define MAX_DOCK_SLOTS	2

typedef struct dock_bay {
	int		num_slots;
	int		type_flags;					// indicates what this docking bay can be used for (i.e. cargo/rearm, etc)
	int		num_spline_paths;			// number of spline paths which lead to this docking bay
	int		*splines;					// array of indices into the Spline_path array
	char		name[MAX_NAME_LEN];		// name of this docking location
	vector	pnt[MAX_DOCK_SLOTS];
	vector	norm[MAX_DOCK_SLOTS];
} dock_bay;

// struct that holds the indicies into path information associated with a fighter bay on a capital ship
// NOTE: Fighter bay paths are identified by the path_name $bayN (where N is numbered from 1).
//			Capital ships only have ONE fighter bay on the entire ship
#define MAX_SHIP_BAY_PATHS		10
typedef struct ship_bay {
	int	num_paths;							// how many paths are associated with the model's fighter bay
	int	paths[MAX_SHIP_BAY_PATHS];		// index into polymodel->paths[] array
	int	arrive_flags;	// bitfield, set to 1 when that path number is reserved for an arrival
	int	depart_flags;	// bitfield, set to 1 when that path number is reserved for a departure
} ship_bay_t;

// three structures now used for representing shields.
// shield_tri structure stores information concerning each face of the shield.
// verts indexes into the verts array in the higher level structure
// neighbors indexes into the tris array in the higher level structure
typedef struct shield_tri {
  int used;
  int verts[3];			// 3 indices into vertex list of the shield.  list found in shield_info struct
  int neighbors[3];		// indices into array of triangles. neighbor = shares edge.  list found in shield_info struct
  vector norm;				// norm of this triangle
} shield_tri;

// a list of these shield_vertex structures comprimises the vertex list of the shield.
// The verts array in the shield_tri structure points to one of these members
typedef struct shield_vertex {
	vector	pos;
	float		u,v;
} shield_vertex;

// the high level shield structure.  A ship without any shield has nverts and ntris set to 0.
// The vertex list and the tris list are used by the shield_tri structure
typedef struct shield_info {
	int				nverts;
	int				ntris;
	shield_vertex	*verts;
	shield_tri		*tris;
} shield_info;

#define BSP_LIGHT_TYPE_WEAPON 1
#define BSP_LIGHT_TYPE_THRUSTER 2

typedef struct bsp_light {
	vector			pos;
	int				type;		// See BSP_LIGHT_TYPE_?? for values
	float				value;	// How much to light up this light.  0-1.
} bsp_light;

// model_octant - There are 8 of these per model.  They are a handy way to catagorize
// a lot of model properties to get some easy 8x optimizations for model stuff.
typedef struct model_octant {
	vector		min, max;				// The bounding box that makes up this octant defined as 2 points.
	int			nverts;					// how many vertices are in this octant
	vector		**verts;					// The vertices in this octant in the high-res hull.  A vertex can only be in one octant.
	int			nshield_tris;			// how many shield triangles are in the octant
	shield_tri	**shield_tris;			// the shield triangles that make up this octant. A tri could be in multiple octants.
} model_octant;

#define MAX_EYES	10

typedef struct eye {
	int		parent;			// parent's subobject number
	vector	pnt;				// the point for the eye
	vector	norm;				// direction the eye faces.  Not used with first eye since player orient is used
} eye;

typedef struct cross_section {
	float z;
	float radius;
} cross_section;

#define MAX_MODEL_INSIGNIAS		6
#define MAX_INS_FACE_VECS			3
#define MAX_INS_VECS					20
#define MAX_INS_FACES				10
typedef struct insignia {
	int detail_level;
	int num_faces;					
	int faces[MAX_INS_FACES][MAX_INS_FACE_VECS];		// indices into the vecs array	
	float u[MAX_INS_FACES][MAX_INS_FACE_VECS];		// u tex coords on a per-face-per-vertex basis
	float v[MAX_INS_FACES][MAX_INS_FACE_VECS];		// v tex coords on a per-face-per-vertex bases
	vector vecs[MAX_INS_VECS];								// vertex list	
	vector offset;												// global position offset for this insignia
} insignia;

#define PM_FLAG_ALLOW_TILING	(1<<0)					// Allow texture tiling
#define PM_FLAG_AUTOCEN			(1<<1)					// contains autocentering info	

//used to describe a polygon model
typedef struct polymodel {
	int			id;				// what the polygon model number is.  (Index in Polygon_models)
	int			version;
	char			filename[FILENAME_LEN];

	uint			flags;			// 1=allow tiling
	int			n_detail_levels;
	int			detail[MAX_MODEL_DETAIL_LEVELS];
	float			detail_depth[MAX_MODEL_DETAIL_LEVELS];

	int			num_debris_objects;
	int			debris_objects[MAX_DEBRIS_OBJECTS];

	int			n_models;

	vector		mins,maxs;							//min,max for whole model
	vector		bounding_box[8];

	int			num_lights;							// how many lights there are
	bsp_light *	lights;								// array of light info

	int			n_view_positions;					// number of viewing positions available on this ship
	eye			view_positions[MAX_EYES];		//viewing positions.  Default to {0,0,0}. in location 0

	float			rad;									// The radius of everything in the model; shields, thrusters.
	float			core_radius;						// The radius to be used for collision detection in small ship vs big ships.
															// This is equal to 1/2 of the smallest dimension of the hull's bounding box.
	int			n_textures;
	int			original_textures[MAX_MODEL_TEXTURES];		// what gets read in from file
	int			textures[MAX_MODEL_TEXTURES];					// what textures you draw with.  reset to original_textures by model_set_instance

	vector		autocenter;							// valid only if PM_FLAG_AUTOCEN is set
	
	bsp_info		*submodel;							// an array of size n_models of submodel info.

	// linked lists for special polygon types on this model.  Most ships I think will have most
	// of these.  (most ships however, probably won't have approach points).
	int			n_guns;								// number of primary gun points (not counting turrets)
	int			n_missiles;							// number of secondary missile points (not counting turrets)
	int			n_docks;								// number of docking points
	int			n_thrusters;						// number of thrusters on this ship.
	w_bank		*gun_banks;							// array of gun banks
	w_bank		*missile_banks;					// array of missile banks
	dock_bay		*docking_bays;						// array of docking point pairs
	thruster_bank		*thrusters;							// array of thruster objects -- likely to change in the future
	ship_bay_t		*ship_bay;							// contains path indexes for ship bay approach/depart paths

	shield_info	shield;								// new shield information

	int			n_paths;
	model_path	*paths;

	// physics info
	float			mass;
	vector		center_of_mass;
	matrix		moment_of_inertia;
	
	model_octant	octants[8];

	int num_xc;				// number of cross sections
	cross_section* xc;	// pointer to array of cross sections (used in big ship explosions)

	int num_split_plane;	// number of split planes
	float split_plane[MAX_SPLIT_PLANE];	// actual split plane z coords (for big ship explosions)

	insignia		ins[MAX_MODEL_INSIGNIAS];
	int			num_ins;

#ifndef NDEBUG
	int			ram_used;		// How much RAM this model uses
	int			debug_info_size;
	char			*debug_info;
#endif
} polymodel;


// Call once to initialize the model system
void model_init();

// call at the beginning of a level. after the level has been loaded
void model_level_post_init();

// Call to free all existing models
void model_free_all();

// Loads a model from disk and returns the model number it loaded into.
int model_load(const char *filename, int n_subsystems, model_subsystem *subsystems);

// notify the model system that a ship has died
void model_notify_dead_ship(int objnum);

// Returns a pointer to the polymodel structure for model 'n'
polymodel * model_get(int model_num);

// routine to copy susbsystems.  Must be called when subsystems sets are the same -- see ship.cpp
void model_copy_subsystems( int n_subsystems, model_subsystem *d_sp, model_subsystem *s_sp );

// If MR_FLAG_OUTLINE bit set this color will be used for outlines.
// This defaults to black.
void model_set_outline_color(int r, int g, int b );

void model_set_outline_color_fast(void *outline_color);

// IF MR_LOCK_DETAIL is set, then it will always draw detail level 'n'
// This defaults to 0. (0=highest, larger=lower)
void model_set_detail_level(int n);

// Flags you can pass to model_render
#define MR_NORMAL						(0)			// Draw a normal object
#define MR_SHOW_OUTLINE				(1<<0)		// Draw the object in outline mode. Color specified by model_set_outline_color
#define MR_SHOW_PIVOTS				(1<<1)		// Show the pivot points
#define MR_SHOW_PATHS				(1<<2)		// Show the paths associated with a model
#define MR_SHOW_RADIUS				(1<<3)		// Show the radius around the object
#define MR_SHOW_DAMAGE				(1<<4)		// Show the "destroyed" subobjects
#define MR_SHOW_SHIELDS				(1<<5)		// Show the sheild mesh
#define MR_SHOW_THRUSTERS			(1<<6)		// Show the engine thrusters. See model_set_thrust for how long it draws.
#define MR_LOCK_DETAIL				(1<<7)		// Only draw the detail level defined in model_set_detail_level
#define MR_NO_POLYS					(1<<8)		// Don't draw the polygons.
#define MR_NO_LIGHTING				(1<<9)		// Don't perform any lighting on the model.
#define MR_NO_TEXTURING				(1<<10)		// Draw textures as flat-shaded polygons.
#define MR_NO_CORRECT				(1<<11)		// Don't to correct texture mapping
#define MR_NO_SMOOTHING				(1<<12)		// Don't perform smoothing on vertices.
#define MR_ALWAYS_REDRAW			(1<<13)		// Don't do any model caching; redraw this model each frame!
#define MR_IS_ASTEROID				(1<<14)		// When set, treat this as an asteroid.  
#define MR_IS_MISSILE				(1<<15)		// When set, treat this as a missilie.  No lighting, small thrusters.
#define MR_SHOW_OUTLINE_PRESET	(1<<16)		// Draw the object in outline mode. Color assumed to be set already.	
#define MR_SHOW_INVISIBLE_FACES	(1<<17)		// Show invisible faces as green...
#define MR_AUTOCENTER				(1<<18)		// Always use the center of the hull bounding box as the center, instead of the pivot point
#define MR_BAY_PATHS					(1<<19)		// draw bay paths
#define MR_ALL_XPARENT				(1<<20)		// render it fully transparent
#define MR_NO_ZBUFFER				(1<<21)		// switch z-buffering off completely 
#define MR_NO_CULL					(1<<22)		// don't cull backfacing poly's
#define MR_FORCE_TEXTURE			(1<<23)		// force a given texture to always be used
#define MR_FORCE_LOWER_DETAIL		(1<<24)		// force the model to draw 1 LOD lower, if possible

// Renders a model and all it's submodels.
// See MR_? defines for values for flags
void model_render(int model_num, matrix *orient, vector * pos, uint flags = MR_NORMAL, int objnum = -1, int lighting_skip = -1 );

// Renders just one particular submodel on a model.
// See MR_? defines for values for flags
void submodel_render(int model_num,int submodel_num, matrix *orient, vector * pos, uint flags=MR_NORMAL, int light_ignore_id=-1 );


// Returns the radius of a model
float model_get_radius(int modelnum);
float submodel_get_radius( int modelnum, int submodelnum );

// Returns the core radius (smallest dimension of hull's bounding box, used for collision detection with big ships only)
float model_get_core_radius( int modelnum );

// Returns zero is x1,y1,x2,y2 are valid
// returns 1 for invalid model, 2 for point offscreen.
// note that x1,y1,x2,y2 aren't clipped to 2d screen coordinates!
// This function just looks at the radius, and not the orientation, so the
// bounding box won't change depending on the obj's orient.
extern int model_find_2d_bound(int model_num,matrix *orient, vector * pos,int *x1, int *y1, int *x2, int *y2 );

// Returns zero is x1,y1,x2,y2 are valid
// returns 1 for invalid model, 2 for point offscreen.
// note that x1,y1,x2,y2 aren't clipped to 2d screen coordinates!
// This function looks at the object's bounding box and it's orientation,
// so the bounds will change as the object rotates, to give the minimum bouding
// rect.
extern int model_find_2d_bound_min(int model_num,matrix *orient, vector * pos,int *x1, int *y1, int *x2, int *y2 );

// Returns zero is x1,y1,x2,y2 are valid
// returns 1 for invalid model, 2 for point offscreen.
// note that x1,y1,x2,y2 aren't clipped to 2d screen coordinates!
// This function looks at the object's bounding box and it's orientation,
// so the bounds will change as the object rotates, to give the minimum bouding
// rect.
int submodel_find_2d_bound_min(int model_num,int submodel, matrix *orient, vector * pos,int *x1, int *y1, int *x2, int *y2 );


// Returns zero is x1,y1,x2,y2 are valid
// Returns 2 for point offscreen.
// note that x1,y1,x2,y2 aren't clipped to 2d screen coordinates!
// This function just looks at the radius, and not the orientation, so the
// bounding box won't change depending on the obj's orient.
int subobj_find_2d_bound(float radius, matrix *orient, vector * pos,int *x1, int *y1, int *x2, int *y2 );

// stats variables
#ifndef NDEBUG
extern int modelstats_num_polys;
extern int modelstats_num_polys_drawn;
extern int modelstats_num_verts;
extern int modelstats_num_sortnorms;
#endif

// Tries to move joints so that the turrent points to the point dst.
// turret1 is the angles of the turret, turret2 is the angles of the gun from turret
extern int model_rotate_gun(int model_num, model_subsystem * turret, matrix *orient, angles * turret1, angles *turret2, vector * pos, vector * dst);

// Rotates the angle of a submodel.  Use this so the right unlocked axis
// gets stuffed.
extern void submodel_rotate(model_subsystem *psub, submodel_instance_info * sii );

// Rotates the angle of a submodel.  Use this so the right unlocked axis
// gets stuffed.  Does this for stepped rotations
void submodel_stepped_rotate(model_subsystem *psub, submodel_instance_info *sii);

// Given a point (pnt) that is in sub_model_num's frame of
// reference, and given the object's orient and position, 
// return the point in 3-space in outpnt.
extern void model_find_world_point(vector * outpnt, vector *mpnt,int model_num, int sub_model_num, matrix * objorient, vector * objpos );

// Given a point in the world RF, find the corresponding point in the model RF.
// This is special purpose code, specific for model collision.
// NOTE - this code ASSUMES submodel is 1 level down from hull (detail[0])
void world_find_model_point(vector *out, vector *world_pt, polymodel *pm, int submodel_num, matrix *orient, vector *pos);

// Given a polygon model index, find a list of rotating submodels to be used for collision
void model_get_rotating_submodel_list(int *submodel_list, int *num_rotating_submodesl, object *objp);

// For a rotating submodel, find a point on the axis
void model_init_submodel_axis_pt(submodel_instance_info *sii, int model_num, int submodel_num);

// Given a direction (pnt) that is in sub_model_num's frame of
// reference, and given the object's orient and position, 
// return the point in 3-space in outpnt.
extern void model_find_world_dir(vector * out_dir, vector *in_dir,int model_num, int sub_model_num, matrix * objorient, vector * objpos );

// Clears all the submodel instances stored in a model to their defaults.
extern void model_clear_instance(int model_num);

// Sets rotating submodel turn info to that stored in model
void model_set_instance_info(submodel_instance_info *sii, float turn_rate, float turn_accel);

// Clears all the values in a particular instance to their defaults.
extern void model_clear_instance_info(submodel_instance_info * sii);

// Sets the submodel instance data in a submodel
extern void model_set_instance(int model_num, int sub_model_num, submodel_instance_info * sii );

// Adds an electrical arcing effect to a submodel
void model_add_arc(int model_num, int sub_model_num, vector *v1, vector *v2, int arc_type );

// Fills in an array with points from a model.  Only gets up to max_num verts.
// Returns number of verts found
extern int submodel_get_points(int model_num, int submodel_num, int max_num, vector **nts );

// Gets two random points on the surface of a submodel
extern void submodel_get_two_random_points(int model_num, int submodel_num, vector *v1, vector *v2, vector *n1 = NULL, vector *n2 = NULL);

// gets the index into the docking_bays array of the specified type of docking point
// Returns the index.  second functions returns the index of the docking bay with
// the specified name
extern int model_find_dock_index(int modelnum, int dock_type);
extern int model_find_dock_name_index( int modelnum, char *name );

// returns the actual name of a docking point on a model, needed by Fred.
char *model_get_dock_name(int modelnum, int index);

// Returns number of verts in a submodel;
int submodel_get_num_verts(int model_num, int submodel_num );

// Returns number of polygons in a submodel;
int submodel_get_num_polys(int model_num, int submodel_num );

// returns number of docking points for a model
int model_get_num_dock_points(int modelnum);
int model_get_dock_index_type(int modelnum, int index);

// get all the different docking point types on a model
int model_get_dock_types(int modelnum);

// Given a vector that is in sub_model_num's frame of
// reference, and given the object's orient and position,
// return the vector in the model's frame of reference.
void model_find_obj_dir(vector *w_vec, vector *m_vec, object *ship_obj, int sub_model_num);


// This is the interface to model_check_collision.  Rather than passing all these
// values and returning values in globals, just fill in a temporary variable with
// the input values and call model_check_collision
typedef struct mc_info {
	// Input values
	int		model_num;			// What model to check
	int		submodel_num;		// What submodel to check if MC_SUBMODEL is set
	matrix	*orient;				// The orient of the model
	vector	*pos;					// The pos of the model in world coordinates
	vector	*p0;					// The starting point of the ray (sphere) to check
	vector	*p1;					// The ending point of the ray (sphere) to check
	int		flags;				// Flags that the model_collide code looks at.  See MC_??? defines
	float		radius;				// If MC_CHECK_THICK is set, checks a sphere moving with the radius.
	
	// Return values
	int		num_hits;			// How many collisions were found
	float		hit_dist;			// The distance from p0 to hitpoint
	vector	hit_point;			// Where the collision occurred at in hit_submodel's coordinate system
	vector	hit_point_world;	// Where the collision occurred at in world coordinates
	int		hit_submodel;		// Which submodel got hit.
	int		hit_bitmap;			// Which texture got hit.  -1 if not a textured poly
	float		hit_u, hit_v;		// Where on hit_bitmap the ray hit.  Invalid if hit_bitmap < 0
	int		shield_hit_tri;	// Which triangle on the shield got hit or -1 if none
	vector	hit_normal;			//	Vector normal of polygon of collision.  (This is in submodel RF)
	int		edge_hit;			// Set if an edge got hit.  Only valid if MC_CHECK_THICK is set.	
	ubyte		*f_poly;				// pointer to flat poly where we intersected
	ubyte		*t_poly;				// pointer to tmap poly where we intersected
		
										// flags can be changed for the case of sphere check finds an edge hit
} mc_info;



//======== MODEL_COLLIDE ============

//	Model Collision flags, used in model_collide()
#define MC_CHECK_MODEL			(1<<0)			// Check the polygons in the model.
#define MC_CHECK_SHIELD			(1<<1)			//	check for collision against shield, if it exists.
#define MC_ONLY_SPHERE			(1<<2)			// Only check bounding sphere. Not accurate, but fast.  
															// NOTE!  This doesn't set hit_point correctly with MC_CHECK_SPHERELINE
#define MC_ONLY_BOUND_BOX		(1<<3)			// Only check bounding boxes.  Pretty accurate and slower than MC_ONLY_SPHERE.
															// Checks the rotatated bounding box of each submodel.  
															// NOTE!  This doesn't set hit_point correctly with MC_CHECK_SPHERELINE
#define MC_CHECK_RAY				(1<<4)			// Checks a ray from p0 *through* p1 on to infinity
#define MC_CHECK_SPHERELINE	(1<<5)			// Checks a moving sphere rather than just a ray.  Radius
#define MC_SUBMODEL				(1<<6)			// If this is set, only check the submodel specified in mc->submodel_num. Use with MC_CHECK_MODEL
#define MC_SUBMODEL_INSTANCE	(1<<7)			// Check submodel and its children (of a rotating submodel)
#define MC_CHECK_INVISIBLE_FACES (1<<8)		// Check the invisible faces.


/*
   Checks to see if a vector from p0 to p0 collides with a model of
   type 'model_num' at 'orient' 'pos'.

   Returns the number of polys that were hit.  Zero is none, obviously.
  	Return true if a collision with hull (or shield, if MC_CHECK_SHIELD set), 
	else return false.

   If it did it one or more, then hitpt is the closest 3d point that the
   vector hit.  See the MC_? defines for flag values.

   Model_collide can test a sphere against either (1) shield or (2) model.

   To check a sphere, set the radius of sphere in mc_info structure and
   set the flag MC_CHECK_SPHERE.

   Here is a sample for how to use:
  
	mc_info mc;

	mc.model_num = ???;			// Fill in the model to check
	mc.orient = &obj->orient;	// The object's orient
	mc.pos = &obj->pos;			// The object's position
	mc.p0 = &p0;					// Point 1 of ray to check
	mc.p1 = &p1;					// Point 2 of ray to check
	mc.flags = MC_CHECK_MODEL;	// flags

** TO COLLIDE AGAINST A LINE SEGMENT

  model_collide(&mc);
	if (mc.num_hits) {		
		// We hit submodel mc.hit_submodel on texture mc.hitbitmap,
		// at point mc.hit_point_world, with uv's of mc.hit_u, mc.hit_v.
	}

** TO COLLIDE AGAINST A SPHERE
	mc.flags |= MC_CHECK_SPHERELINE;
	mc.radius = radius;

	model_collide(&mc, radius);
	if (mc.num_hits) {		
		// We hit submodel mc.hit_submodel on texture mc.hitbitmap,
		// at point mc.hit_point_world, with uv's of mc.hit_u, mc.hit_v.
		// Check (mc.edge_hit) to see if we hit an edge
	}
*/

int model_collide(mc_info * mc_info);

// Sets the submodel instance data in a submodel
// If show_damaged is true it shows only damaged submodels.
// If it is false it shows only undamaged submodels.
void model_show_damaged(int model_num, int show_damaged );


//=========================== MODEL OCTANT STUFF ================================

//  Models are now divided into 8 octants.    Sheilds too.
//  This made the collision code faster.   Shield is 4x and ship faces
//  are about 2x faster.

//  Before, calling model_collide with flags=0 didn't check the shield
//  but did check the model itself.   Setting the sheild flags caused
//  the shield to get check along with the ship.
//  Now, you need to explicitly tell the model_collide code to check
//  the model, so you can check the model or shield or both.

//  If you need to check them both, do it in one call; this saves some
//  time.    If checking the shield is sufficient for determining 
//  something   (like if it is under the hud) then use just sheild 
//  check, it is at least 5x faster than checking the model itself.


// Model octant ordering - this is a made up ordering, but it makes sense.
// X Y Z  index description
// - - -  0     left bottom rear
// - - +  1     left bottom front
// - + -  2     left top rear
// - + +  3     left top front
// + - -  4     right bottom rear
// + - +  5     right bottom front
// + + -  6     right top rear
// + + +  7     right top front

// Returns which octant point 'pnt' is closet to. This will always return 
// a valid octant (0-7) since the point doesn't have to be in an octant.
// If model_orient and/or model_pos are NULL, pnt is assumed to already 
// be rotated into the model's local coordinates.  
// If oct is not null, it will be filled in with a pointer to the octant
// data.
int model_which_octant_distant( vector *pnt, int model_num,matrix *model_orient, vector * model_pos, model_octant **oct );

// Returns which octant point 'pnt' is in. This might return
// -1 if the point isn't in any octant.
// If model_orient and/or model_pos are NULL, pnt is assumed to already 
// be rotated into the model's local coordinates.
// If oct is not null, it will be filled in with a pointer to the octant
// data.  Or NULL if the pnt isn't in the octant.
int model_which_octant( vector *pnt, int model_num,matrix *model_orient, vector * model_pos, model_octant **oct );

// scale the engines thrusters by this much
// Only enabled if MR_SHOW_THRUSTERS is on
void model_set_thrust(int model_num, float length, int bitmapnum, int glow_bitmapnum=-1, float glow_noise=1.0f);

//=========================================================
// model caching

// Call once to init the model caching stuff
void model_cache_init();

// Call before every level to clean up the model caching stuff
void model_cache_reset();

// If TRUE, then model caching is enabled
extern int Model_caching;


//=======================================================================================
// Finds the closest point on a model to a point in space.  Actually only finds a point
// on the bounding box of the model.    
// Given:
//   model_num      Which model
//   submodel_num   Which submodel, -1 for hull
//   orient         Orientation of the model
//   pos            Position of the model
//   eye_pos        Point that you want to find the closest point to
// Returns:
//   distance from eye_pos to closest_point.  0 means eye_pos is 
//   on or inside the bounding box.
//   Also fills in outpnt with the actual closest point.
float model_find_closest_point( vector *outpnt, int model_num, int submodel_num, matrix *orient, vector * pos, vector *eye_pos );

// set the insignia bitmap to be used when rendering a ship with an insignia (-1 switches it off altogether)
void model_set_insignia_bitmap(int bmap);

// set model transparency for use with MR_ALL_XPARENT
void model_set_alpha(float alpha);

// set the forces bitmap
void model_set_forced_texture(int bmap);

// see if the given texture is used by the passed model. 0 if not used, 1 if used, -1 on error
int model_find_texture(int model_num, int bitmap);

// find closest point on extended bounding box (the bounding box plus all the planes that make it up)
// returns closest distance to extended box
// positive return value means start_point is outside extended box
// displaces closest point an optional amount delta to the outside of the box
// closest_box_point can be NULL.
float get_world_closest_box_point_with_delta(vector *closest_box_point, object *box_obj, vector *start_point, int *is_inside, float delta);

// given a newly loaded model, page in all textures
void model_page_in_textures(int modelnum, int ship_info_index);

// is the given model a pirate ship?
int model_is_pirate_ship(int modelnum);

#endif

