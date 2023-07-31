/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

#include "pstypes.h"
#include "joy.h"
#include "fix.h"
#include "key.h"
#include "timer.h"
#include "osregistry.h"
#include "joy_ff.h"
#include "osapi.h"

static int Joy_inited = 0;
int joy_num_sticks = 0;
int Dead_zone_size = 10;
int Cur_joystick = -1;
int Joy_sensitivity = 9;

int joy_pollrate = 1000 / 18;  // poll at 18Hz

static int Joy_last_x_reading = 0;
static int Joy_last_y_reading = 0;

typedef struct joy_button_info {
	int     actual_state;           // Set if the button is physically down
	int     state;                          // Set when the button goes from up to down, cleared on down to up.  Different than actual_state after a flush.
	int     down_count;
	int     up_count;
	int     down_time;
	uint    last_down_check;        // timestamp in milliseconds of last 
} joy_button_info;

Joy_info joystick;

int JOYSTICKID1 = 0; 	// DDOI - temporary
static SDL_Joystick *sdljoy;

joy_button_info joy_buttons[JOY_TOTAL_BUTTONS];


void joy_close()
{
	if (!Joy_inited)
		return;

	Joy_inited = 0;
	joy_num_sticks = 0;
	
	if (sdljoy)
		SDL_JoystickClose(sdljoy);
	sdljoy = NULL;
	
	SDL_QuitSubSystem (SDL_INIT_JOYSTICK);
}

void joy_get_caps (int max)
{
	SDL_Joystick *joy;
	int j;

	for (j=0; j < JOY_NUM_AXES; j++)
		joystick.axis_valid[j] = 0;

	for (j=JOYSTICKID1; j<JOYSTICKID1+max; j++) {
		joy = SDL_JoystickOpen (j);
		if (joy)
		{
			nprintf (("JOYSTICK", "Joystick #%d: %s\n", j - JOYSTICKID1 + 1, SDL_JoystickName(j)));
			if (j == Cur_joystick) {
				for (int i = 0; i < SDL_JoystickNumAxes(joy); i++)
				{
					joystick.axis_valid[i] = 1;
					#ifdef PANDORA
					joystick.axis_valid[i+2] = 1;
					#endif
				}
			}
			SDL_JoystickClose (joy);
		}
	}
}

int joy_down(int btn)
{
	int tmp;

	if ( joy_num_sticks < 1 ) return 0;
	if ( (btn < 0) || (btn >= JOY_TOTAL_BUTTONS )) return 0;

	tmp = joy_buttons[btn].state;

	return tmp;
}

int joy_down_count(int btn, int reset_count)
{
	int tmp;

	if ( joy_num_sticks < 1 ) return 0;
	if ( (btn < 0) || (btn >= JOY_TOTAL_BUTTONS)) return 0;

	tmp = joy_buttons[btn].down_count;
	if ( reset_count ) {
		joy_buttons[btn].down_count = 0;
	}

	return tmp;
}

float joy_down_time(int btn)
{
	float                           rval;
	unsigned int    now;
	joy_button_info         *bi;

	if ( joy_num_sticks < 1 ) return 0.0f;
	if ( (btn < 0) || (btn >= JOY_TOTAL_BUTTONS)) return 0.0f;
	bi = &joy_buttons[btn];

	now = timer_get_milliseconds();

	if ( bi->down_time == 0 && joy_down(btn) ) {
		bi->down_time += joy_pollrate;
	}

	if ( (now - bi->last_down_check) > 0)
		rval = i2fl(bi->down_time) / (now - bi->last_down_check);
	else
		rval = 0.0f;

	bi->down_time = 0;
	bi->last_down_check = now;

	if (rval < 0)
		rval = 0.0f;
	if (rval > 1)
		rval = 1.0f;

	return rval;
}

void joy_flush()
{
	int                     i;
	joy_button_info *bi;

	if ( joy_num_sticks < 1 ) return;

	for ( i = 0; i < JOY_TOTAL_BUTTONS; i++) {
		bi = &joy_buttons[i];
		bi->state               = 0;
		bi->down_count  = 0;
		bi->up_count    = 0;
		bi->down_time   = 0;
		bi->last_down_check = timer_get_milliseconds();
	}
}

int joy_get_unscaled_reading(int raw, int axn)
{
	int rng;

	// Make sure it's calibrated properly.
	if (joystick.axis_center[axn] - joystick.axis_min[axn] < 5)
		return 0;

	if (joystick.axis_max[axn] - joystick.axis_center[axn] < 5)
		return 0;

	rng = joystick.axis_max[axn] - joystick.axis_min[axn];
	raw -= joystick.axis_min[axn];  // adjust for linear range starting at 0
	
	// cap at limits
	if (raw < 0)
		raw = 0;
	if (raw > rng)
		raw = rng;

	return (int) ((unsigned int) raw * (unsigned int) F1_0 / (unsigned int) rng);  // convert to 0 - F1_0 range.
}

// --------------------------------------------------------------
//	joy_get_scaled_reading()
//
//	input:	raw	=>	the raw value for an axis position
//				axn	=>	axis number, numbered starting at 0
//
// return:	joy_get_scaled_reading will return a value that represents
//				the joystick pos from -1 to +1 for the specified axis number 'axn', and
//				the raw value 'raw'
//
int joy_get_scaled_reading(int raw, int axn)
{
	int x, d, dead_zone, rng;
	float percent, sensitivity_percent, non_sensitivity_percent;

	// Make sure it's calibrated properly.
	if (joystick.axis_center[axn] - joystick.axis_min[axn] < 5)
		return 0;

	if (joystick.axis_max[axn] - joystick.axis_center[axn] < 5)
		return 0;

	raw -= joystick.axis_center[axn];

	dead_zone = (joystick.axis_max[axn] - joystick.axis_min[axn]) * Dead_zone_size / 100;

	if (raw < -dead_zone) {
		rng = joystick.axis_center[axn] - joystick.axis_min[axn] - dead_zone;
		d = -raw - dead_zone;

	} else if (raw > dead_zone) {
		rng = joystick.axis_max[axn] - joystick.axis_center[axn] - dead_zone;
		d = raw - dead_zone;

	} else
		return 0;

	if (d > rng)
		d = rng;

	Assert(Joy_sensitivity >= 0 && Joy_sensitivity <= 9);

	// compute percentages as a range between 0 and 1
	sensitivity_percent = (float) Joy_sensitivity / 9.0f;
	non_sensitivity_percent = (float) (9 - Joy_sensitivity) / 9.0f;

	// find percent of max axis is at
	percent = (float) d / (float) rng;

	// work sensitivity on axis value
	percent = (percent * sensitivity_percent + percent * percent * percent * percent * percent * non_sensitivity_percent);

	x = (int) ((float) F1_0 * percent);

	//nprintf(("AI", "d=%6i, sens=%3i, percent=%6.3f, val=%6i, ratio=%6.3f\n", d, Joy_sensitivity, percent, (raw<0) ? -x : x, (float) d/x));

	if (raw < 0)
		return -x;

	return x;
}

// --------------------------------------------------------------
//	joy_get_pos()
//
//	input:	x		=>		OUTPUT PARAMETER: x-axis position of stick (-1 to 1)
//				y		=>		OUTPUT PARAMETER: y-axis position of stick (-1 to 1)
//				z		=>		OUTPUT PARAMETER: z-axis (throttle) position of stick (-1 to 1)
//				r		=>		OUTPUT PARAMETER: rudder position of stick (-1 to 1)
//
//	return:	success	=> 1
//				failure	=> 0
//
int joy_get_pos(int *x, int *y, int *z, int *rx)
{
	int axis[JOY_NUM_AXES];

	if (x) *x = 0;
	if (y) *y = 0;
	if (z) *z = 0;
	if (rx) *rx = 0;

	if (joy_num_sticks < 1) return 0;

	joystick_read_raw_axis( 6, axis );

	//	joy_get_scaled_reading will return a value represents the joystick pos from -1 to +1
	if (x && joystick.axis_valid[0])
		*x = joy_get_scaled_reading(axis[0], 0);
	if (y && joystick.axis_valid[1]) 
		*y = joy_get_scaled_reading(axis[1], 1);
	if (z && joystick.axis_valid[2])
		*z = joy_get_unscaled_reading(axis[2], 2);
	if (rx && joystick.axis_valid[3])
		*rx = joy_get_scaled_reading(axis[3], 3);

	if (x)
		Joy_last_x_reading = *x;

	if (y)
		Joy_last_y_reading = *y;

	return 1;
}

void joy_process(int time_delta)
{
	int i;
	
	if (!Joy_inited)
		return;
	if (sdljoy == NULL)
		return;
	
	int buttons = SDL_JoystickNumButtons(sdljoy);
	int hat = SDL_JoystickGetHat(sdljoy, 0);
	
	for (i=0; i < JOY_TOTAL_BUTTONS; i++) {
		int state = 0;
		
		if (i < JOY_NUM_BUTTONS) {
			if (i < buttons) {			
#ifdef __vita__ // We map controls to keyboard so that we can use mode shifting for more bindings
				state = 0;
#else
				state = SDL_JoystickGetButton(sdljoy, i);
#endif
			}
		} else { 
			switch (i) {
				case JOY_HATBACK:
					state = (hat & SDL_HAT_DOWN) ? 1 : 0;
					break;
				case JOY_HATFORWARD:
					state = (hat & SDL_HAT_UP) ? 1 : 0;
					break;
				case JOY_HATLEFT:
					state = (hat & SDL_HAT_LEFT) ? 1 : 0;
					break;
				case JOY_HATRIGHT:
					state = (hat & SDL_HAT_RIGHT) ? 1 : 0;
					break;
				default:
					break;
			}
		}
		
		if (state != joy_buttons[i].actual_state) {
			// Button position physically changed.
			joy_buttons[i].actual_state = state;
			
			if ( state )    {
				// went from up to down
				joy_buttons[i].down_count++;
				joy_buttons[i].down_time = 0;
				
				joy_buttons[i].state = 1;
			} else {
				// went from down to up
				if ( joy_buttons[i].state )     {
					joy_buttons[i].up_count++;
				}
				
				joy_buttons[i].state = 0;
			}
		} else {
			// Didn't move... increment time down if down.
			if (joy_buttons[i].state) {
				//joy_buttons[i].down_time += joy_pollrate;
				joy_buttons[i].down_time += time_delta;
			}
		}	
	}
}

void joy_read()
{
    static Uint32 lasttic = 0;
    Uint32 curtic = SDL_GetTicks();
    Uint32 delta = curtic - lasttic;
    
    while (delta >= (uint)joy_pollrate) {
        joy_process(delta);
        
        lasttic += joy_pollrate;
        
        delta = curtic - lasttic;
    }
}

int joy_init()
{
	int i, n;

	if (Joy_inited)
		return 0;

	if (SDL_InitSubSystem (SDL_INIT_JOYSTICK)<0)
	{
		mprintf(("Could not initialize joystick\n"));
		return 0;
	}

	Joy_inited = 1;
	n = SDL_NumJoysticks ();

	#ifdef PANDORA
	if (n>0) Cur_joystick=0;
	else
	#else
	Cur_joystick = os_config_read_uint (NULL, "CurrentJoystick", JOYSTICKID1);
	#endif

	joy_get_caps(n);

	if (n < 1) {
		mprintf(("No joysticks found\n"));
		return 0;
	}

	sdljoy = SDL_JoystickOpen(Cur_joystick);
	if (sdljoy == NULL) {
		mprintf(("Unable to init joystick %d\n", Cur_joystick));
	}
	
	
	joy_flush ();

	joy_num_sticks = n;

	// Fake a calibration
	if (joy_num_sticks > 0) {
		// joy_set_cen();
		for (i=0; i<4; i++) {
			joystick.axis_center[i] = 32768;
			joystick.axis_min[i] = 0;
			joystick.axis_max[i] = 65536;
		}
	}

	return joy_num_sticks;
}

void joy_set_cen()
{
	joystick_read_raw_axis( 2, joystick.axis_center );
}

int joystick_read_raw_axis(int num_axes, int *axis)
{
	int i;
	int num;
	
	if (sdljoy == NULL)
		return 0;
	
	num = SDL_JoystickNumAxes(sdljoy);
	
	for (i = 0; i < num_axes; i++) {
		#ifdef PANDORA
		if ((i > 1) && (i < 4)) {
			if (i==3)
				axis[i] = SDL_JoystickGetAxis(sdljoy, 0) + 32768;
			else
				axis[i] = 32768 - SDL_JoystickGetAxis(sdljoy, 1);
		} else {
			axis[i] = 32768;
		}
		#else
		if (i < num) {
			axis[i] = SDL_JoystickGetAxis(sdljoy, i) + 32768;
		} else {
			axis[i] = 32768;
		}
		#endif
	}
	
	return 1;
}

void joy_ff_adjust_handling(int speed)
{
//	STUB_FUNCTION;
}

void joy_ff_afterburn_off()
{
//	STUB_FUNCTION;
}

void joy_ff_afterburn_on()
{
//	STUB_FUNCTION;
}

void joy_ff_deathroll()
{
//	STUB_FUNCTION;
}

void joy_ff_docked()
{
//	STUB_FUNCTION;
}

void joy_ff_explode()
{
//	STUB_FUNCTION;
}

void joy_ff_fly_by(int mag)
{
//	STUB_FUNCTION;
}

void joy_ff_mission_init(vector v)
{
//	STUB_FUNCTION;
}

void joy_ff_play_dir_effect(float x, float y)
{
//	STUB_FUNCTION;
}

void joy_ff_play_primary_shoot(int gain)
{
//	STUB_FUNCTION;
}

void joy_ff_play_reload_effect()
{
//	STUB_FUNCTION;
}

void joy_ff_play_secondary_shoot(int gain)
{
//	STUB_FUNCTION;
}

void joy_ff_play_vector_effect(vector *v, float scaler)
{
//	STUB_FUNCTION;
}

void joy_ff_stop_effects()
{
	joy_ff_afterburn_off();
}
