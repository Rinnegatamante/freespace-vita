/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Io/Joy.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Include file for joystick stuff
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 20    5/13/98 1:17a Hoffoss
 * Added joystick axes configurability.
 * 
 * 19    5/05/98 8:38p Hoffoss
 * Added sensitivity adjustment to options menu and made it save to pilot
 * file.
 * 
 * 18    5/04/98 11:08p Hoffoss
 * Expanded on Force Feedback code, and moved it all into Joy_ff.cpp.
 * Updated references everywhere to it.
 * 
 * 17    4/29/98 12:13a Lawrance
 * Add function for reading down count without resetting internal count.
 * Add hook to reset demo trailer timer.
 * 
 * 16    3/21/98 11:29a John
 * Made joy_flush work when a button is held down
 * 
 * 15    3/06/98 10:02a Hoffoss
 * Made dead zone adjustable, and defaulted it to 10% instead of 5%.
 * 
 * 14    1/29/98 11:04a Sandeep
 * 
 * 13    1/27/98 8:40p Sandeep
 * 
 * 12    1/08/98 6:35p Hoffoss
 * Fixed joystick undefined detection.
 * 
 * 11    9/15/97 11:42p Lawrance
 * change button_info to joy_button_info to avoid name conflict
 * 
 * 10    8/07/97 11:26p Lawrance
 * add support for 4th axis (rudder)
 * 
 * 9     7/29/97 5:30p Lawrance
 * move gettime() from keyboard module to timer module
 * 
 * 8     7/10/97 12:29a Lawrance
 * fix problem with NT not recognizing an axis that was set under 95
 * 
 * 7     7/09/97 11:41p Lawrance
 * added throttle and hat support
 * 
 * 6     2/17/97 5:18p John
 * Added a bunch of RCS headers to a bunch of old files that don't have
 * them.
 *
 * $NoKeywords: $
 */

#ifndef __JOY_H__
#define __JOY_H__

#define JOY_NUM_BUTTONS		32
#define JOY_NUM_HAT_POS		4
#define JOY_TOTAL_BUTTONS	(JOY_NUM_BUTTONS + JOY_NUM_HAT_POS)
#define JOY_NUM_AXES			6

#define JOY_HATBACK			(JOY_NUM_BUTTONS)
#define JOY_HATFORWARD		(JOY_NUM_BUTTONS+1)
#define JOY_HATLEFT			(JOY_NUM_BUTTONS+2)
#define JOY_HATRIGHT			(JOY_NUM_BUTTONS+3)

#define JOY_AXIS_UNDEFINED		-10000

typedef struct Joy_info {
	int	axis_valid[JOY_NUM_AXES];
	int	axis_min[JOY_NUM_AXES];
	int	axis_center[JOY_NUM_AXES];
	int	axis_max[JOY_NUM_AXES];
} Joy_info;

extern int Joy_sensitivity;
extern int Dead_zone_size;  // percentage of range that is dead zone

int	joy_init();
void	joy_flush();
int	joy_get_pos(int * x, int * y, int *z, int *r);
int	joy_down_count(int btn, int reset_count = 1);
int	joy_down(int btn);
int	joy_up_count(int btn);
float	joy_down_time(int btn);
void	joy_get_cal_vals(int *axis_min, int *axis_center, int *axis_max);
void	joy_set_cal_vals(int *axis_min, int *axis_center, int *axis_max);
void	joy_set_ul();
void	joy_set_lr();
void	joy_set_cen(); 
void	joy_cheap_cal();
int	joystick_read_raw_axis( int num_axes, int * axis );
void joy_get_delta(int *dx, int *dy);
int joy_get_scaled_reading(int raw, int axn);
int joy_get_unscaled_reading(int raw, int axn);

#endif	/* __JOY_H__ */

