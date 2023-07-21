/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/GlobalIncs/version.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 *
 * $Log$
 * Revision 1.4  2003/05/25 02:30:42  taylor
 * Freespace 1 support
 *
 * Revision 1.3  2003/05/06 07:21:43  taylor
 * add version number for linux/unix client
 *
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 23    11/03/99 11:07a Jefff
 * version num -> 1.2
 * 
 * 22    10/27/99 3:05p Jefff
 * upped version to 1.1 for german
 * 
 * 21    10/25/99 5:40p Jefff
 * bump version to 1.02 for German version
 * 
 * 20    10/06/99 10:24a Jefff
 * upped OEM version to 1.0
 * 
 * 19    10/01/99 9:10a Daveb
 * V 1.1 PATCH
 * 
 * 18    9/14/99 7:34a Dave
 * 
 * 17    9/13/99 12:00a Dave
 * Bumped to 0.07
 * 
 * 16    9/11/99 11:01p Dave
 * Upped to 0.06
 * 
 * 15    9/10/99 9:44p Dave
 * Bumped version # up. Make server reliable connects not have such a huge
 * timeout. 
 * 
 * 14    8/16/99 4:04p Dave
 * Big honking checkin.
 * 
 * 13    8/09/99 2:21p Andsager
 * Fix patching from multiplayer direct to launcher update tab.
 * 
 * 12    8/09/99 10:36a Dave
 * Version info for game.
 * 
 * 11    8/06/99 4:37p Andsager
 * demo patch to 1.10
 * 
 * 10    8/06/99 4:35p Andsager
 * test patching - up demo version to 1.01
 * 
 * 9     8/06/99 10:33a Andsager
 * Demo release canditate version number.
 * 
 * 8     8/06/99 12:33a Andsager
 * up demo to 1.2
 * 
 * 7     8/05/99 5:31p Andsager
 * Up demo version 1.01
 * 
 * 6     7/30/99 11:59a Andsager
 * 
 * 5     7/28/99 2:51p Andsager
 * Update demo build to 1.0.0
 * 
 * 4     7/27/99 8:59a Andsager
 * Make major, minor version consistent for all builds.  Only show major
 * and minor for launcher update window.
 * 
 * 3     7/15/99 9:20a Andsager
 * FS2_DEMO initial checkin
 * 
 * 2     5/19/99 4:07p Dave
 * Moved versioning code into a nice isolated common place. Fixed up
 * updating code on the pxo screen. Fixed several stub problems.
 * 
 * 1     5/18/99 4:28p Dave
 * 
 * $NoKeywords: $
 */

#ifndef __FS2_VERSIONING_HEADER_FILE
#define __FS2_VERSIONING_HEADER_FILE

// ----------------------------------------------------------------------------------------------------------------
// VERSION DEFINES/VARS
//

// keep this defined to build a standalone only executable
// STANDALONE_ONLY
// #define STANDALONE_ONLY_BUILD
// #define STANDALONE_ONLY_RELEASE_VERSION		0

// Here are the version defines.  
//	Gets displayed as MAJOR.MINOR, or 1.21 if MAJOR = 1, MINOR = 21.
//	Prior to release, MAJOR should be zero.  After release, it should be 1.  Probably never increase to 2 as that could
//	cause confusion with a sequel.
//	MINOR should increase by 1 for each minor upgrade and by 10 for major upgrades.
//	With each rev we send, we should increase MINOR.
// Version history (full version):
//		1.0	Initial US/UK release
//		1.01	Patch for Win95 volume label bug
//		1.20	German release version

#if defined(FS2_DEMO)
	#define FS_VERSION_MAJOR 01					// Major is 0 for pre-release and 1 for 1.0 release
	#define FS_VERSION_MINOR 10					// Increase by 1 for minor revs, 10 for major.
	#define FS_VERSION_BUILD 00					// Build version 
#elif defined(FS1_DEMO)
	#define FS_VERSION_MAJOR 01					// Major is 0 for pre-release and 1 for 1.0 release
 	#define FS_VERSION_MINOR 20					// Increase by 1 for minor revs, 10 for major.
 	#define FS_VERSION_BUILD 00					// Build version
#elif defined(OEM_BUILD)
	#define FS_VERSION_MAJOR 01					// Major is 0 for pre-release and 1 for 1.0 release
	#define FS_VERSION_MINOR 00					// Increase by 1 for minor revs, 10 for major.
	#define FS_VERSION_BUILD 00					// Build version 
#elif defined(MAKE_FS1)
	#define FS_VERSION_MAJOR 01					// Major is 0 for pre-release and 1 for 1.0 release
	#define FS_VERSION_MINOR 06					// Increase by 1 for minor revs, 10 for major.
	#define FS_VERSION_BUILD 00					// Build version
#else
	#define FS_VERSION_MAJOR 01					// Major is 0 for pre-release and 1 for 1.0 release
	#define FS_VERSION_MINOR 20					// Increase by 1 for minor revs, 10 for major.
	#define FS_VERSION_BUILD 00					// Build version 
#endif

#ifdef PLAT_UNIX
	#define FS_UNIX_VERSION_MAJOR 00
	#define FS_UNIX_VERSION_MINOR 92
#endif

#define VERSION_LOC_FNAME			"version.nfo"
#define MOTD_LOC_FNAME				"motd.txt"

#ifdef MULTIPLAYER_BETA_BUILD
	#define MOTD_URL						"http://www.pxo.net/files/fs2beta/motd.txt"
	#define VERSION_URL					"http://www.pxo.net/files/fs2beta/version.nfo"
#elif defined(FS2_DEMO)
	#define MOTD_URL						"http://www.pxo.net/files/fs2demo/motd.txt"
	#define VERSION_URL					"http://www.pxo.net/files/fs2demo/version.nfo"
#else
	#define MOTD_URL						"http://www.pxo.net/files/fs2/motd.txt"
	#define VERSION_URL					"http://www.pxo.net/files/fs2/version.nfo"
#endif

#define VERSION_FILE_COMMENT_CHAR ';'
#define NO_VERSION_IN_REGISTRY		-2

// ----------------------------------------------------------------------------------------------------------------
// VERSION FUNCTIONS
//

// compare version against the passed version file
// returns -1 on error 
// 0 if we are an earlier version
// 1 if same version
// 2 if higher version
// fills in user version and latest version values if non-NULL
int version_compare(const char *filename, int *u_major, int *u_minor, int *u_build, int *l_major, int *l_minor, int *l_build);

#endif

