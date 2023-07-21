/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_rate.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 * 
 * $Log$
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
 * 6     7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 5     4/25/99 3:02p Dave
 * Build defines for the E3 build.
 * 
 * 4     4/09/99 2:21p Dave
 * Multiplayer beta stuff. CD checking.
 * 
 * 3     3/20/99 5:09p Dave
 * Fixed release build fred warnings and unhandled exception.
 * 
 * 2     3/09/99 6:24p Dave
 * More work on object update revamping. Identified several sources of
 * unnecessary bandwidth.
 *  
 *   
 * $NoKeywords: $
 */

#include "timer.h"
#include "2d.h"
#include "alphacolors.h"
#include "multi_rate.h"

// -----------------------------------------------------------------------------------------------------------------------
// MULTI RATE DEFINES/VARS
//

// keep this defined to compile in rate checking
#if !defined(NDEBUG) || defined(MULTIPLAYER_BETA_BUILD) || defined(FS2_DEMO)
	#define MULTI_RATE
#endif

// how many records in the past we'll keep track of
#define NUM_UPDATE_RECORDS							5

// rate monitoring info
typedef struct mr_info {
	// type
	char type[MAX_RATE_TYPE_LEN+1];

	// all time info		
	int total_bytes;												// total bytes alltime

	// per second info
	int stamp_second;												// stamp for one second
	int bytes_second;												// how many bytes we've sent in the last second	
	int records_second[NUM_UPDATE_RECORDS];				// records
	int records_second_count;									// how many records we have
	float avg_second;												// avg bytes/sec

	// per frame info
	int bytes_frame;												// how many bytes we've sent this frame	
	int records_frame[NUM_UPDATE_RECORDS];					// records
	int records_frame_count;									// how many records we have
	float avg_frame;												// avg bytes/frame				
} mr_info;

#ifdef MULTI_RATE
// all records
mr_info Multi_rate[MAX_RATE_PLAYERS][MAX_RATE_TYPES];
#endif


// -----------------------------------------------------------------------------------------------------------------------
// MULTI RATE FUNCTIONS
//

// notify of a player join
void multi_rate_reset(int np_index)
{
	// release builds
#ifndef MULTI_RATE
	return;
#else
	int idx;

	// sanity checks
	if((np_index < 0) || (np_index >= MAX_RATE_PLAYERS)){
		return;
	}

	// blast the index clear
	for(idx=0; idx<MAX_RATE_TYPES; idx++){
		memset(&Multi_rate[np_index][idx], 0, sizeof(mr_info));
		Multi_rate[np_index][idx].stamp_second = -1;
	}
#endif
}

// add data of the specified type to datarate processing, returns 0 on fail (if we ran out of types, etc, etc)
int multi_rate_add(int np_index, const char *type, int size)
{	
	// release builds
#ifndef MULTI_RATE
	return 0;
#else
	int idx;
	mr_info *m;
	// sanity checks
	if((np_index < 0) || (np_index >= MAX_RATE_PLAYERS)){
		return 0;
	}
	if((type == NULL) || (strlen(type) <= 0)){
		return 0;
	}

	// see if the type already exists
	for(idx=0; idx<MAX_RATE_TYPES; idx++){
		// empty slot
		if(strlen(Multi_rate[np_index][idx].type) <= 0){
			break;
		}
		// existing
		else if(!stricmp(Multi_rate[np_index][idx].type, type)){
			break;
		}
	}

	// if we couldn't find a slot
	if(idx >= MAX_RATE_TYPES){
		return 0;
	}

	// otherwise add the data
	m = &Multi_rate[np_index][idx];

	// type string
	strcpy(m->type, type);

	// alltime
	m->total_bytes += size;

	// per-second
	m->bytes_second += size;

	// per-frame
	m->bytes_frame += size;		

	// success
	return 1;
#endif
}

// process
#define R_AVG(ct, ar, avg)		do {int av_idx; float av_sum = 0.0f; if(ct == 0){ avg = 0;} else { for(av_idx=0; av_idx<ct; av_idx++){ av_sum += (float)ar[av_idx]; } avg = av_sum / (float)ct; } }while(0)
void multi_rate_process()
{
	// release builds
#ifndef MULTI_RATE
	return;
#else

	int idx, s_idx;
	mr_info *m;

	// process all active players
	for(idx=0; idx<MAX_RATE_PLAYERS; idx++){
		for(s_idx=0; s_idx<MAX_RATE_TYPES; s_idx++){
			m = &Multi_rate[idx][s_idx];

			// invalid entries
			if(strlen(m->type) <= 0){
				continue;
			}

			// process alltime
			if(m->stamp_second == -1){
				m->stamp_second = timestamp(1000);
			} else if(timestamp_elapsed(m->stamp_second)){
				// if we've reached max records
				if(m->records_second_count >= NUM_UPDATE_RECORDS){
					memmove(m->records_second, m->records_second+1, sizeof(int) * (NUM_UPDATE_RECORDS - 1)); 
					m->records_second[NUM_UPDATE_RECORDS-1] = m->bytes_second; 
				}
				// haven't reached max records
				else {
					m->records_second[m->records_second_count++] = m->bytes_second;
				}

				// recalculate the average
				R_AVG(m->records_second_count, m->records_second, m->avg_second);

				// reset bytes/second and timestamp
				m->bytes_second = 0;
				m->stamp_second = timestamp(1000);
			}

			// process per-frame
			// if we've reached max records
			if(m->records_frame_count >= NUM_UPDATE_RECORDS){
				memmove(m->records_frame, m->records_frame+1, sizeof(int) * (NUM_UPDATE_RECORDS - 1)); 
				m->records_frame[NUM_UPDATE_RECORDS-1] = m->bytes_frame; 
			}
			// haven't reached max records
			else {
				m->records_frame[m->records_frame_count++] = m->bytes_frame;
			}

			// recalculate the average
			R_AVG(m->records_frame_count, m->records_frame, m->avg_frame);

			// reset bytes/frame
			m->bytes_frame = 0;			
		}
	}	
#endif
}

// display
void multi_rate_display(int np_index, int x, int y)
{	
	// release builds
#ifndef MULTI_RATE
	return;
#else

	int idx;
	mr_info *m;

	// sanity checks
	if((np_index < 0) || (np_index >= MAX_RATE_PLAYERS)){
		return;
	}

	// get info
	for(idx=0; idx<MAX_RATE_TYPES; idx++){
		m = &Multi_rate[np_index][idx];

		// if we have a 0 length string, we're done
		if(strlen(m->type) <= 0){
			break;
		}

		// display
		gr_set_color_fast(&Color_red);
		gr_printf(x, y, "%s %d (%d/s) (%f/f)", m->type, m->total_bytes, (int)m->avg_second, m->avg_frame);
		y += 10;
	}
#endif
}

