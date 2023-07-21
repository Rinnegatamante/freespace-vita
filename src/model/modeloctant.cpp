/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Model/ModelOctant.cpp $
 * $Revision: 121 $
 * $Date: 2002-06-17 08:33:11 +0200 (Mon, 17 Jun 2002) $
 * $Author: relnev $
 *
 * Routines for model octants
 *
 * $Log$
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
 * 3     1/06/99 2:24p Dave
 * Stubs and release build fixes.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 16    4/29/98 5:01p Mike
 * Large overhaul in how turrets fire.
 * 
 * 15    4/02/98 8:16a John
 * Fixed Assert in model_collide with large ships
 * 
 * 14    4/01/98 5:34p John
 * Made only the used POFs page in for a level.   Reduced some interp
 * arrays.    Made custom detail level work differently.
 * 
 * 13    3/31/98 5:18p John
 * Removed demo/save/restore.  Made NDEBUG defined compile.  Removed a
 * bunch of debug stuff out of player file.  Made model code be able to
 * unload models and malloc out only however many models are needed.
 *  
 * 
 * 12    10/31/97 3:19p John
 * changed id field in face to be radius
 * 
 * 11    9/10/97 11:40a John
 * took out unused parts of model_octant, like faces and submodels.   Made
 * the vertices actually be face center points.   Made the find_faces code
 * save the poly center as the normal_point in the face structure.
 * 
 * 10    8/15/97 4:10p John
 * new code to use the new octrees style bsp trees
 * 
 * 9     7/22/97 9:41a John
 * Made flat faces appear in octant list, so collision detection now
 * works.  Made them do smoothing if needed.
 * 
 * 8     7/03/97 9:14a John
 * fixed incorrect octant vertices.
 * 
 * 7     6/26/97 12:37p John
 * fixed bug with octant submodels.  disabled octant submodels correctly,
 * since they aren't yet implemented.
 * 
 * 6     6/26/97 11:19a John
 * Made model face & shield collisions look only at octants it needs to.
 * Shield sped up 4x, faces sped up about 2x.
 * 
 * 5     6/26/97 9:02a Mike
 * Comment out frequent mprintf().
 * 
 * 4     6/25/97 6:08p John
 * made which_octant functions also return a pointer to the octant data.
 * 
 * 3     6/25/97 5:34p John
 * Added functions to tell which octant a point is in.
 * 
 * 2     6/25/97 5:11p John
 * added foundation for model octants.
 * 
 * 1     6/25/97 4:07p John
 *
 * $NoKeywords: $
 */


#include <math.h>

#define MODEL_LIB

#include "2d.h"
#include "3d.h"
#include "model.h"
#include "tmapper.h"
#include "floating.h"
#include "fvi.h"
#include "lighting.h"
#include "modelsinc.h"


// returns 1 if a point is in an octant.
int point_in_octant( polymodel * pm, model_octant * oct, vector *vert )
{
	if ( vert->xyz.x < oct->min.xyz.x ) return 0;
	if ( vert->xyz.x > oct->max.xyz.x ) return 0;

	if ( vert->xyz.y < oct->min.xyz.y ) return 0;
	if ( vert->xyz.y > oct->max.xyz.y ) return 0;

	if ( vert->xyz.z< oct->min.xyz.z) return 0;
	if ( vert->xyz.z> oct->max.xyz.z) return 0;

	return 1;
}


void model_octant_find_shields( polymodel * pm, model_octant * oct )
{
	int i, j, n;
	shield_tri *tri;

	n = 0;
		
	//	Scan all the triangles in the mesh to find how many tris there are.
	for (i=0; i<pm->shield.ntris; i++ )	{

		tri = &pm->shield.tris[i];

		for (j=0; j<3; j++ )	{
			if ( point_in_octant( pm, oct, &pm->shield.verts[tri->verts[j]].pos ))	{
				n++;
				break;
			}
		}
	}

	//mprintf(( "Octant has %d shield polys in it\n", n ));

	oct->nshield_tris = n;
	oct->shield_tris = (shield_tri **)malloc( sizeof(shield_tri *) * oct->nshield_tris );
	Assert(oct->shield_tris!=NULL);

	n = 0;
		
	//	Rescan all the triangles in the mesh.
	for (i=0; i<pm->shield.ntris; i++ )	{

		tri = &pm->shield.tris[i];

		for (j=0; j<3; j++ )	{
			if ( point_in_octant( pm, oct, &pm->shield.verts[tri->verts[j]].pos ))	{
				oct->shield_tris[n++] = tri;
				break;
			}
		}
	}

	Assert( oct->nshield_tris == n );
}


    
void moff_defpoints(ubyte * p)
{
	int n;
	int nverts = w(p+8);	
	int offset = w(p+16);	

	ubyte * normcount = p+20;
	vector *src = vp(p+offset);

	Assert( nverts < MAX_POLYGON_VECS );
	// Assert( nnorms < MAX_POLYGON_NORMS );

	for (n=0; n<nverts; n++ )	{

		Interp_verts[n] = src;

		src += normcount[n]+1;
	} 
}



// Textured Poly
// +0      int         id
// +4      int         size 
// +8      vector      normal
// +20     vector      center
// +32     float      radius
// +36     int         nverts
// +40     int         tmap_num
// +44     nverts*(model_tmap_vert) vertlist (n,u,v)
void moff_tmappoly(ubyte * p, polymodel * pm, model_octant * oct, int just_count )
{
	int i, nv;
	model_tmap_vert *verts;

	nv = w(p+36);
	if ( nv < 0 ) return;

	verts = (model_tmap_vert *)(p+44);

	if ( pm->version < 2003 )	{
		// Set the "normal_point" part of field to be the center of the polygon
		vector center_point;
		vm_vec_zero( &center_point );

		for (i=0;i<nv;i++)	{
			vm_vec_add2( &center_point, Interp_verts[verts[i].vertnum] );
		}

		center_point.xyz.x /= nv;
		center_point.xyz.y /= nv;
		center_point.xyz.z /= nv;

		*vp(p+20) = center_point;

		float rad = 0.0f;

		for (i=0;i<nv;i++)	{
			float dist = vm_vec_dist( &center_point, Interp_verts[verts[i].vertnum] );
			if ( dist > rad )	{
				rad = dist;
			}
		}
		fl(p+32) = rad;
	}

	// Put each face into a particular octant
	if ( point_in_octant( pm, oct, vp(p+20) ) )	{
		if (just_count)
			oct->nverts++;
		else
			oct->verts[oct->nverts++] = vp(p+20);
		return;
	}
}


// Flat Poly
// +0      int         id
// +4      int         size 
// +8      vector      normal
// +20     vector      center
// +32     float       radius
// +36     int         nverts
// +40     byte        red
// +41     byte        green
// +42     byte        blue
// +43     byte        pad
// +44     nverts*int  vertlist
void moff_flatpoly(ubyte * p, polymodel * pm, model_octant * oct, int just_count )
{
	int i, nv;
	short *verts;

	nv = w(p+36);
	if ( nv < 0 ) return;

	verts = (short *)(p+44);

	if ( pm->version < 2003 )	{
		// Set the "normal_point" part of field to be the center of the polygon
		vector center_point;
		vm_vec_zero( &center_point );

		for (i=0;i<nv;i++)	{
			vm_vec_add2( &center_point, Interp_verts[verts[i*2]] );
		}

		center_point.xyz.x /= nv;
		center_point.xyz.y /= nv;
		center_point.xyz.z /= nv;

		*vp(p+20) = center_point;

		float rad = 0.0f;

		for (i=0;i<nv;i++)	{
			float dist = vm_vec_dist( &center_point, Interp_verts[verts[i*2]] );
			if ( dist > rad )	{
				rad = dist;
			}
		}
		fl(p+32) = rad;
	}

	// Put each face's center point into a particular octant
	if ( point_in_octant( pm, oct, vp(p+20) ) )	{
		if (just_count)
			oct->nverts++;
		else
			oct->verts[oct->nverts++] = vp(p+20);
	}
}



int model_octant_find_faces_sub(polymodel * pm, model_octant * oct, void *model_ptr, int just_count )
{
	ubyte *p = (ubyte *)model_ptr;
	int chunk_type, chunk_size;

	chunk_type = w(p);
	chunk_size = w(p+4);
	
	while (chunk_type != OP_EOF)	{

		switch (chunk_type) {
		case OP_EOF:			return 1;
		case OP_DEFPOINTS:	
			moff_defpoints(p); 
			break;
		case OP_FLATPOLY:		moff_flatpoly(p, pm, oct, just_count ); break;
		case OP_TMAPPOLY:		moff_tmappoly(p, pm, oct, just_count ); break;
		case OP_SORTNORM:		{
				int frontlist = w(p+36);
				int backlist = w(p+40);
				int prelist = w(p+44);
				int postlist = w(p+48);
				int onlist = w(p+52);

				if (prelist) model_octant_find_faces_sub(pm,oct,p+prelist,just_count);
				if (backlist) model_octant_find_faces_sub(pm,oct,p+backlist,just_count);
				if (onlist) model_octant_find_faces_sub(pm,oct,p+onlist,just_count);
				if (frontlist) model_octant_find_faces_sub(pm,oct,p+frontlist,just_count);
				if (postlist) model_octant_find_faces_sub(pm,oct,p+postlist,just_count);
			}
			break;
		case OP_BOUNDBOX:		break;
		default:
			mprintf(( "Bad chunk type %d, len=%d in model_octant_find_faces_sub\n", chunk_type, chunk_size ));
			Int3();		// Bad chunk type!
			return 0;
		}
		p += chunk_size;
		chunk_type = w(p);
		chunk_size = w(p+4);
	}
	return 1;
}


void model_octant_find_faces( polymodel * pm, model_octant * oct )
{
	ubyte *p;
	int submodel_num = pm->detail[0];

	p = pm->submodel[submodel_num].bsp_data;

	oct->nverts = 0;
	model_octant_find_faces_sub(pm, oct, p, 1 );

	if ( oct->nverts < 1 ) {
		oct->nverts = 0;
		oct->verts = NULL;
		return;
	}

	oct->verts = (vector **)malloc( sizeof(vector *) * oct->nverts );
	Assert(oct->verts!=NULL);

	oct->nverts = 0;
	model_octant_find_faces_sub(pm, oct, p, 0 );

//	mprintf(( "Octant has %d faces\n", oct->nfaces ));
}


// Creates the octants for a given polygon model
void model_octant_create( polymodel * pm )
{
	vector min, max, center;
	int i, x, y, z;

	min = pm->mins;
	max = pm->maxs;
	
	vm_vec_avg( &center, &min, &max );

	for (i=0; i<8; i++ )	{
		x = i & 4;
		y = i & 2;
		z = i & 1;
	
		if ( x )	{
			pm->octants[i].max.xyz.x = max.xyz.x;
			pm->octants[i].min.xyz.x = center.xyz.x;
		} else {
			pm->octants[i].max.xyz.x = center.xyz.x;
			pm->octants[i].min.xyz.x = min.xyz.x;
		}

		if ( y )	{
			pm->octants[i].max.xyz.y = max.xyz.y;
			pm->octants[i].min.xyz.y = center.xyz.y;
		} else {
			pm->octants[i].max.xyz.y = center.xyz.y;
			pm->octants[i].min.xyz.y = min.xyz.y;
		}

		if ( z )	{
			pm->octants[i].max.xyz.z = max.xyz.z;
			pm->octants[i].min.xyz.z = center.xyz.z;
		} else {
			pm->octants[i].max.xyz.z = center.xyz.z;
			pm->octants[i].min.xyz.z = min.xyz.z;
		}

		model_octant_find_shields( pm, &pm->octants[i] );
		model_octant_find_faces( pm, &pm->octants[i] );

	}
	
}


// frees the memory the octants use for a given polygon model
void model_octant_free( polymodel * pm )
{
	int i;

	for (i=0; i<8; i++ )	{
		model_octant * oct = &pm->octants[i];

		if ( oct->verts )	{
			free(oct->verts);
			oct->verts = NULL;
		}

		if ( oct->shield_tris )	{
			free( oct->shield_tris );
			oct->shield_tris = NULL;
		}

	}	
}


// Returns which octant point pnt is closet to. This will always return 
// a valid octant (0-7) since the point doesn't have to be in an octant.
// If model_orient and/or model_pos are NULL, pnt is assumed to already 
// be rotated into the model's local coordinates.
// If oct is not null, it will be filled in with a pointer to the octant
// data.
int model_which_octant_distant_many( vector *pnt, int model_num,matrix *model_orient, vector * model_pos, polymodel **pm, int *octs)
{
	vector tempv, rotpnt;

	*pm = model_get(model_num);

	if ( model_orient && model_pos )	{
		// First, rotate pnt into the model's frame of reference.
		vm_vec_sub( &tempv, pnt, model_pos );
		vm_vec_rotate( &rotpnt, &tempv, model_orient );
	} else {
		rotpnt = *pnt;
	}

	vector center;
	vm_vec_avg( &center, &((*pm)->mins), &((*pm)->maxs ));
	int i, x, y, z;

	if ( rotpnt.xyz.x > center.xyz.x ) x = 1; else x = 0;
	if ( rotpnt.xyz.y > center.xyz.y ) y = 1; else y = 0;
	if ( rotpnt.xyz.z > center.xyz.z ) z = 1; else z = 0;

	i = ( (x<<2) | (y<<1) | z );

	octs[0] = i;
	octs[1] = i ^ 4;	//	Adjacent octant in x dimension
	octs[2] = i ^ 2;	//	Adjacent octant in y dimension
	octs[3] = i ^ 1;	//	Adjacent octant in z dimension

	return i;
}


// Returns which octant point pnt is closet to. This will always return 
// a valid octant (0-7) since the point doesn't have to be in an octant.
// If model_orient and/or model_pos are NULL, pnt is assumed to already 
// be rotated into the model's local coordinates.
// If oct is not null, it will be filled in with a pointer to the octant
// data.
int model_which_octant_distant( vector *pnt, int model_num,matrix *model_orient, vector * model_pos, model_octant **oct )
{
	polymodel * pm;
	vector tempv, rotpnt;

	pm = model_get(model_num);

	if ( model_orient && model_pos )	{
		// First, rotate pnt into the model's frame of reference.
		vm_vec_sub( &tempv, pnt, model_pos );
		vm_vec_rotate( &rotpnt, &tempv, model_orient );
	} else {
		rotpnt = *pnt;
	}

	vector center;
	vm_vec_avg( &center, &pm->mins, &pm->maxs );
	int i, x, y, z;

	if ( rotpnt.xyz.x > center.xyz.x ) x = 1; else x = 0;
	if ( rotpnt.xyz.y > center.xyz.y ) y = 1; else y = 0;
	if ( rotpnt.xyz.z > center.xyz.z ) z = 1; else z = 0;

	i = ( (x<<2) | (y<<1) | z );

	if ( oct )
		*oct = &pm->octants[i];
	
	return i;
}



// Returns which octant point pnt is in. This might return
// -1 if the point isn't in any octant.
// If model_orient and/or model_pos are NULL, pnt is assumed to already 
// be rotated into the model's local coordinates.
// If oct is not null, it will be filled in with a pointer to the octant
// data.  Or NULL if the pnt isn't in the octant.
int model_which_octant( vector *pnt, int model_num,matrix *model_orient, vector * model_pos, model_octant **oct )
{
	polymodel * pm;
	vector tempv, rotpnt;
	
	pm = model_get(model_num);

	if ( model_orient && model_pos )	{
		// First, rotate pnt into the model's frame of reference.
		vm_vec_sub( &tempv, pnt, model_pos );
		vm_vec_rotate( &rotpnt, &tempv, model_orient );
	} else {
		rotpnt = *pnt;
	}

	vector center;
	vm_vec_avg( &center, &pm->mins, &pm->maxs );
	int i, x, y, z;

	if ( rotpnt.xyz.x > center.xyz.x ) x = 1; else x = 0;
	if ( rotpnt.xyz.y > center.xyz.y ) y = 1; else y = 0;
	if ( rotpnt.xyz.z > center.xyz.z ) z = 1; else z = 0;

	i =  (x<<2) | (y<<1) | z;

	if ( point_in_octant( pm, &pm->octants[i], &rotpnt ) )	{
		if ( oct )
			*oct = &pm->octants[i];
		return i;
	}

	if ( oct )
		*oct = NULL;

	return -1;
}

