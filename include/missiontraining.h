/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Mission/MissionTraining.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Special code for training missions.  Stuff like displaying training messages in
 * the special training window, listing the training objectives, etc.
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
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
 * 7     4/16/98 4:33p Hoffoss
 * Added support for detecting instructor terminating training due to
 * player shooting at him.
 * 
 * 6     4/15/98 5:25p Lawrance
 * extern Training_msg_visible
 * 
 * 5     1/05/98 4:04p Hoffoss
 * Changed training-msg sexp operator to allow it to control the length of
 * time a message is displayed for.
 * 
 * 4     10/17/97 6:39p Hoffoss
 * Added delayability to key-pressed operator and training-msg operator.
 * 
 * 3     10/10/97 6:15p Hoffoss
 * Implemented a training objective list display.
 * 
 * 2     10/09/97 4:44p Hoffoss
 * Dimmed training window glass and made it less transparent, added flags
 * to events, set he stage for detecting current events.
 * 
 * 1     10/09/97 2:41p Hoffoss
 *
 * $NoKeywords: $
 */

extern int Training_msg_method;
extern int Training_num_lines;
extern int Training_msg_visible;
extern int Training_failure;

void training_mission_init();
void training_mission_shutdown();
void training_check_objectives();
void message_training_que(char *text, int timestamp, int length = -1);
void message_training_setup(int num, int length = -1);
void message_training_display();
void message_translate_tokens(char *buf, char *text);
void training_fail();

