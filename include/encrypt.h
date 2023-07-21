/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/parse/Encrypt.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header for encryption code common to FreeSpace and related tools
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     3/25/99 11:26a Dave
 * Beefed up encryption scheme so that even someone viewing the
 * disassembly would have a hard time cracking it.
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 4     8/09/98 4:44p Lawrance
 * support alternate encryption scheme (doesn't pack chars into 7 bits)
 * 
 * 3     3/31/98 4:57p Lawrance
 * Add signature at the beginning of encrypted files
 * 
 * 2     3/31/98 1:14a Lawrance
 * Get .tbl and mission file encryption working.
 * 
 * 1     3/30/98 11:02p Lawrance
 *
 * $NoKeywords: $
 */

#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

// initialize encryption
void encrypt_init();

// Return 1 if the file is encrypted, otherwise return 0
int is_encrpyted(char *scrambled_text);

// Encrpyt text data
void encrypt(char *text, int text_len, char *scrambled_text, int *scrambled_len, int use_8bit);

// Decrypt scrambled_text
void unencrypt(char *scrambled_text, int scrambled_len, char *text, int *text_len);

#endif

