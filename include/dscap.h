/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Sound/dscap.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for DirectSoundCapture code
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 4     2/15/98 11:10p Lawrance
 * more work on real-time voice system
 * 
 * 3     2/15/98 4:43p Lawrance
 * work on real-time voice
 * 
 * 2     2/03/98 11:53p Lawrance
 * Adding support for DirectSoundCapture
 * 
 * 1     2/03/98 4:48p Lawrance
 *
 * $NoKeywords: $
 */

#ifndef __DSCAP_H__
#define __DSCAP_H__

int	dscap_init();
void	dscap_close();
int	dscap_supported();
int	dscap_create_buffer(int freq, int bits_per_sample, int nchannels, int nseconds);
void	dscap_release_buffer();

int	dscap_start_record();
int	dscap_stop_record();
int	dscap_max_buffersize();
int	dscap_get_raw_data(unsigned char *outbuf, unsigned int max_size);


#endif	// __DSCAP_H__

