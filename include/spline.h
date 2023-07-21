/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Math/spline.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     7/08/99 10:53a Dave
 * New multiplayer interpolation scheme. Not 100% done yet, but still
 * better than the old way.
 * 
 * 2     7/06/99 4:24p Dave
 * Mid-level checkin. Starting on some potentially cool multiplayer
 * smoothness crap.
 *  
 *
 * $NoKeywords: $
 */

#ifndef __FS2_SPLINE_HEADER_FILE
#define __FS2_SPLINE_HEADER_FILE

#include "vecmat.h"

// -------------------------------------------------------------------------------------------------
// SPLINE DEFINES/VARS
//

struct color;

// max bezier degree - note the # of points directly corresponds to the degree (degree == n_points - 1).
// more points means more expensive!
#define MAX_BEZ_PTS			3

// bezier class. whee
class bez_spline {
public :
	vector	pts[MAX_BEZ_PTS];
	int		num_pts;

public :
	// constructor
	bez_spline();
	bez_spline(int _num_pts, vector *_pts[MAX_BEZ_PTS]);

	// set the points
	void bez_set_points(int _num_pts, vector *_pts[MAX_BEZ_PTS]);

	// bezier blend function
	float BEZ(int k, int n, float u);
	
	// get a point on the bez curve. u goes from 0.0 to 1.0
	void bez_get_point(vector *out, float u);

	// render a bezier
	void bez_render(int divs, color *c);
};

// hermite splines. cool cubic stuff
#define MAX_HERM_PTS			3
class herm_spline {
public :
	vector	pts[MAX_HERM_PTS];			// control points
	vector	d_pts[MAX_HERM_PTS];			// derivative of control points (think of as velocity)
	int		num_pts;
public :
	// constructor
	herm_spline();
	herm_spline(int _num_pts, vector *_pts[MAX_HERM_PTS], vector *_d_pts[MAX_HERM_PTS]);

	// set the points
	void herm_set_points(int _num_pts, vector *_pts[MAX_HERM_PTS], vector *_d_pts[MAX_HERM_PTS]);	
	
	// get a point on the hermite curve.
	void herm_get_point(vector *out, float u, int k);

	// the derivative of a point on the hermite curve
	void herm_get_deriv(vector *deriv, float u, int k);

	// render a bezier
	void herm_render(int divs, color *c);
};


// -------------------------------------------------------------------------------------------------
// SPLINE FUNCTIONS
//


#endif

