/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Starfield/Supernova.cpp $
 * $Revision: 309 $
 * $Date: 2010-02-08 09:09:23 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Include file for nebula stuff
 *
 * $Log$
 * Revision 1.4  2002/06/17 06:33:11  relnev
 * ryan's struct patch for gcc 2.95
 *
 * Revision 1.3  2002/06/09 04:41:27  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:52  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 5     9/09/99 11:40p Dave
 * Handle an Assert() in beam code. Added supernova sounds. Play the right
 * 2 end movies properly, based upon what the player did in the mission.
 * 
 * 4     9/03/99 1:32a Dave
 * CD checking by act. Added support to play 2 cutscenes in a row
 * seamlessly. Fixed super low level cfile bug related to files in the
 * root directory of a CD. Added cheat code to set campaign mission # in
 * main hall.
 * 
 * 3     7/31/99 4:15p Dave
 * Fixed supernova particle velocities. Handle OBJ_NONE in target
 * monitoring view. Properly use objectives notify gauge colors.
 * 
 * 2     7/21/99 8:10p Dave
 * First run of supernova effect.
 *  
 *
 * $NoKeywords: $
 */

#include "freespace.h"
#include "vecmat.h"
#include "object.h"
#include "lighting.h"
#include "starfield.h"
#include "supernova.h"
#include "particle.h"
#include "ship.h"
#include "timer.h"
#include "model.h"
#include "popupdead.h"
#include "key.h"
#include "missioncampaign.h"
#include "gamesequence.h"
#include "gamesnd.h"
#include "sound.h"


// --------------------------------------------------------------------------------------------------------------------------
// SUPERNOVA DEFINES/VARS
//

// supernova time 1
#define SUPERNOVA_SOUND_1_TIME					15.0f
#define SUPERNOVA_SOUND_2_TIME					5.0f
int Supernova_sound_1_played = 0;
int Supernova_sound_2_played = 0;

// countdown for supernova
float Supernova_time_total = -1.0f;
float Supernova_time = -1.0f;
int Supernova_finished = 0;
int Supernova_popup = 0;
float Supernova_fade_to_white = 0.0f;
int Supernova_particle_stamp = -1;

// supernova camera pos
vector Supernova_camera_pos;
matrix Supernova_camera_orient;

int Supernova_status = SUPERNOVA_NONE;

// --------------------------------------------------------------------------------------------------------------------------
// SUPERNOVA FUNCTIONS
//

// level init
void supernova_level_init()
{
	Supernova_time_total = -1.0f;
	Supernova_time = -1.0f;	
	Supernova_finished = 0;
	Supernova_fade_to_white = 0.0f;
	Supernova_popup = 0;
	Supernova_particle_stamp = -1;

	Supernova_sound_1_played = 0;
	Supernova_sound_2_played = 0;

	Supernova_status = SUPERNOVA_NONE;
}

// start a supernova
void supernova_start(int seconds)
{
	// bogus time
	if((float)seconds < SUPERNOVA_CUT_TIME){
		return;
	}

	// no supernova in multiplayer
	if(Game_mode & GM_MULTIPLAYER){
		return;
	}

	// only good if we have one sun
	if(Num_suns != 1){
		return;
	}
	
	Supernova_time_total = (float)seconds;
	Supernova_time = (float)seconds;
	Supernova_finished = 0;
	Supernova_fade_to_white = 0.0f;
	Supernova_popup = 0;
	Supernova_particle_stamp = -1;

	Supernova_status = SUPERNOVA_STARTED;	
}

int sn_particles = 100;
DCF(sn_part, "")
{
	dc_get_arg(ARG_INT);
	sn_particles = Dc_arg_int;
}
void supernova_do_particles()
{	
	int idx;
	vector a, b, ta, tb;
	vector norm, sun_temp;

	// no player ship
	if((Player_obj == NULL) || (Player_ship == NULL)){
		return;
	}

	// timestamp
	if((Supernova_particle_stamp == -1) || timestamp_elapsed(Supernova_particle_stamp)){
		Supernova_particle_stamp = timestamp(sn_particles);

		// get particle norm		
		stars_get_sun_pos(0, &sun_temp);
		vm_vec_add2(&sun_temp, &Player_obj->pos);
		vm_vec_sub(&norm, &Player_obj->pos, &sun_temp);
		vm_vec_normalize(&norm);

		particle_emitter whee;
		whee.max_life = 1.0f;
		whee.min_life = 0.6f;
		whee.max_vel = 50.0f;
		whee.min_vel = 25.0f;
		whee.normal_variance = 0.75f;
		whee.num_high = 5;
		whee.num_low = 2;
		whee.min_rad = 0.5f;
		whee.max_rad = 1.25f;		

		// emit
		for(idx=0; idx<10; idx++){			
			submodel_get_two_random_points(Player_ship->modelnum, 0, &ta, &tb);

			// rotate into world space
			vm_vec_unrotate(&a, &ta, &Player_obj->orient);			
			vm_vec_add2(&a, &Player_obj->pos);			
			whee.pos = a;
			whee.vel = norm;
			vm_vec_scale(&whee.vel, 30.0f);						
			vm_vec_add2(&whee.vel, &Player_obj->phys_info.vel);			
			whee.normal = norm;			
			particle_emit(&whee, PARTICLE_FIRE, (uint)-1);

			vm_vec_unrotate(&b, &tb, &Player_obj->orient);
			vm_vec_add2(&b, &Player_obj->pos);
			whee.pos = b;			
			particle_emit(&whee, PARTICLE_FIRE, (uint)-1);
		}
	}
}

// call once per frame
float sn_shudder = 0.45f;
DCF(sn_shud, "")
{
	dc_get_arg(ARG_FLOAT);
	sn_shudder = Dc_arg_float;
}

void supernova_process()
{	
	int sn_stage;	

	// if the supernova is running
	sn_stage = supernova_active();
	if(sn_stage){
		Supernova_time -= flFrametime;

		// sound stuff
		if((Supernova_time <= SUPERNOVA_SOUND_1_TIME) && !Supernova_sound_1_played){
			Supernova_sound_1_played = 1;
			snd_play(&Snds[SND_SUPERNOVA_1], 0.0f, 1.0f, SND_PRIORITY_MUST_PLAY);
		}
		if((Supernova_time <= SUPERNOVA_SOUND_2_TIME) && !Supernova_sound_2_played){
			Supernova_sound_2_played = 1;
			snd_play(&Snds[SND_SUPERNOVA_2], 0.0f, 1.0f, SND_PRIORITY_MUST_PLAY);
		}

		// if we've crossed from stage 1 to stage 2 kill all particles and stick a bunch on the player ship
		if((sn_stage == 1) && (supernova_active() == 2)){
			// first kill all active particles so we have a bunch of free ones
			particle_kill_all();				
		}		

		// if we're in stage 2, emit particles
		if((sn_stage >= 2) && (sn_stage != 5)){
			supernova_do_particles();
		}

		// if we've got negative. the supernova is done
		if(Supernova_time < 0.0f){							
			Supernova_finished = 1;
			Supernova_fade_to_white += flFrametime;

			// start the dead popup
			if(Supernova_fade_to_white >= SUPERNOVA_FADE_TO_WHITE_TIME){
				if(!Supernova_popup){
					// main freespace 2 campaign? if so - end it now
					if((Game_mode & GM_CAMPAIGN_MODE) && !stricmp(Campaign.filename, "freespace2") && Campaign_ended_in_mission){
						gameseq_post_event(GS_EVENT_END_CAMPAIGN);
					} else {
						popupdead_start();
					}
					Supernova_popup = 1;
				}
				Supernova_finished = 2;
			}
		}
	} 			
}

// is there a supernova active
int supernova_active()
{
	// if the supernova has "finished". fade to white and dead popup
	if(Supernova_finished == 1){
		Supernova_status = SUPERNOVA_HIT;
		return 4;
	}
	if(Supernova_finished == 2){
		Supernova_status = SUPERNOVA_HIT;
		return 5;
	}

	// no supernova
	if((Supernova_time_total <= 0.0f) || (Supernova_time <= 0.0f)){
		return 0;
	}	

	// final stage, 
	if(Supernova_time < (SUPERNOVA_CUT_TIME - SUPERNOVA_CAMERA_MOVE_TIME)){		
		Supernova_status = SUPERNOVA_HIT;
		return 3;
	}	

	// 2nd stage
	if(Supernova_time < SUPERNOVA_CUT_TIME){
		Supernova_status = SUPERNOVA_HIT;
		return 2;
	}

	// first stage
	return 1;
}

// time left before the supernova hits
float supernova_time_left()
{
	return Supernova_time;
}

// pct complete the supernova (0.0 to 1.0)
float supernova_pct_complete()
{
	// bogus
	if(!supernova_active()){
		return -1.0f;
	}

	return (Supernova_time_total - Supernova_time) / Supernova_time_total;
}

// if the camera should cut to the "you-are-toast" cam
int supernova_camera_cut()
{
	// if we're not in a supernova
	if(!supernova_active()){
		return 0;
	}

	// if we're past the critical time
	if(Supernova_time <= SUPERNOVA_CUT_TIME){
		return 1;
	}

	// no cut yet
	return 0;
}

// apply a shake to the orient matrix
void supernova_apply_shake(matrix *eye_orient, float intensity)
{	
	angles	tangles;

	tangles.p = 0.0f;
	tangles.h = 0.0f;
	tangles.b = 0.0f;	

	// Make eye shake due to engine wash		
	int r1 = myrand();
	int r2 = myrand();
	tangles.p += 0.07f * intensity * (float) (r1-MY_RAND_MAX/2)/MY_RAND_MAX;
	tangles.h += 0.07f * intensity * (float) (r2-MY_RAND_MAX/2)/MY_RAND_MAX;			

	matrix	tm, tm2;
	vm_angles_2_matrix(&tm, &tangles);
	Assert(vm_vec_mag(&tm.v.fvec) > 0.0f);
	Assert(vm_vec_mag(&tm.v.rvec) > 0.0f);
	Assert(vm_vec_mag(&tm.v.uvec) > 0.0f);
	vm_matrix_x_matrix(&tm2, eye_orient, &tm);
	*eye_orient = tm2;	
}

// get view params from supernova
float sn_distance = 300.0f;				// shockwave moving at 1000/ms ?
float sn_cam_distance = 25.0f;
DCF(sn_dist, "")
{
	dc_get_arg(ARG_FLOAT);
	sn_distance = Dc_arg_float;
}
DCF(sn_cam_dist, "")
{
	dc_get_arg(ARG_FLOAT);
	sn_cam_distance = Dc_arg_float;
}
void supernova_set_view(vector *eye_pos, matrix *eye_orient)
{
	vector at;
	vector sun_temp, sun;
	vector move;
	vector view;
	float cut_pct = 1.0f - (Supernova_time / SUPERNOVA_CUT_TIME);		
	
	// set the controls for the heart of the sun	
	stars_get_sun_pos(0, &sun_temp);
	vm_vec_add2(&sun_temp, &Player_obj->pos);
	vm_vec_sub(&sun, &sun_temp, &Player_obj->pos);
	vm_vec_normalize(&sun);

	// always set the camera pos
	matrix whee;
	vm_vector_2_matrix(&whee, &move, NULL, NULL);
	vm_vec_scale_add(&Supernova_camera_pos, &Player_obj->pos, &whee.v.rvec, sn_cam_distance);
	vm_vec_scale_add2(&Supernova_camera_pos, &whee.v.uvec, 30.0f);
	*eye_pos = Supernova_camera_pos;

	// if we're no longer moving the camera
	if(Supernova_time < (SUPERNOVA_CUT_TIME - SUPERNOVA_CAMERA_MOVE_TIME)){
		// *eye_pos = Supernova_camera_pos;
		*eye_orient = Supernova_camera_orient;		

		// shake the eye		
		supernova_apply_shake(eye_orient, cut_pct * sn_shudder);

		return;
	} 
	// otherwise move it
	else {
		// get a vector somewhere between the supernova shockwave and the player ship	
		at = Player_obj->pos;
		vm_vec_scale_add2(&at, &sun, sn_distance);
		vm_vec_sub(&move, &Player_obj->pos, &at);
		vm_vec_normalize(&move);
				
		// linearly move towards the player pos
		float pct = ((SUPERNOVA_CUT_TIME - Supernova_time) / SUPERNOVA_CAMERA_MOVE_TIME);
		vm_vec_scale_add2(&at, &move, sn_distance * pct);	

		*eye_pos = Supernova_camera_pos;
		vm_vec_sub(&view, &at, eye_pos);
		vm_vec_normalize(&view);
		vm_vector_2_matrix(&Supernova_camera_orient, &view, NULL, NULL);
		*eye_orient = Supernova_camera_orient;
	}	

	// shake the eye
	supernova_apply_shake(eye_orient, cut_pct * sn_shudder);
}
