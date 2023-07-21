/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

 /*
 * $Logfile: /Freespace2/code/Inetfile/inetgetfile.cpp $
 * $Revision: 306 $
 * $Date: 2008-09-07 03:06:29 +0200 (Sun, 07 Sep 2008) $
 * $Author: taylor $
 *
 * InternetGetFile Class
 *
 * $Log$
 * Revision 1.6  2005/08/12 08:50:09  taylor
 * recursively create directories (hurt more on OSX) and update all _mkdir() calls accordingly
 *
 * Revision 1.5  2002/06/09 04:41:21  relnev
 * added copyright header
 *
 * Revision 1.4  2002/05/26 20:32:24  theoddone33
 * Fix some minor stuff
 *
 * Revision 1.3  2002/05/26 20:20:54  relnev
 * unix.h: updated
 *
 * inetfile: complete
 *
 * Revision 1.2  2002/05/07 03:16:45  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 4     8/22/99 1:19p Dave
 * Fixed up http proxy code. Cleaned up scoring code. Reverse the order in
 * which d3d cards are detected.
 * 
 * 3     6/16/99 10:27a Andsager
 * Make directory if it does not already exist.
 * 
 * 2     4/20/99 6:39p Dave
 * Almost done with artillery targeting. Added support for downloading
 * images on the PXO screen.
 * 
 * 1     4/20/99 4:37p Dave
 *
 * Initial version
 *
 * $NoKeywords: $
 */

#ifndef PLAT_UNIX	// this isn't working yet (really only needed by PXO anyway)

#ifndef PLAT_UNIX
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>	// mkdir
#include <sys/types.h>	// mkdir
              
#include "pstypes.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cftp.h"
#include "chttpget.h"

#include "inetgetfile.h"

#define INET_STATE_CONNECTING		1
#define INET_STATE_ERROR			2
#define INET_STATE_RECEIVING		3
#define INET_STATE_GOT_FILE		4

void InetGetFile::AbortGet()
{
	if(m_bUseHTTP)
	{
		http->AbortGet();
	}
	else
	{
		ftp->AbortGet();
	}
}

InetGetFile::InetGetFile(char *URL,char *localfile)
{
	m_HardError = 0;
	http=NULL;
	ftp=NULL;
	if ((URL==NULL)||(localfile==NULL)) {
		m_HardError = INET_ERROR_BADPARMS;
	}

	// create directory if not already there.
	char dir_name[256], *end;

	// make sure localfile has \ in it or we'll be here a long time.
	if (strstr(localfile, "\\")) {
		strcpy(dir_name, localfile);
		int len = strlen(localfile);
		end = dir_name + len;

		// start from end of localfile and go to first \ to get dirname
		while ( *end != '\\' ) {
			end--;
		}
		*end = '\0';

		if ( _mkdir(dir_name)==0 )	{	
			mprintf(( "CFILE: Created new directory '%s'\n", dir_name ));
		}
	}

	if (strstr(URL,"http:")) {
		m_bUseHTTP = TRUE;

		// using http proxy?
		extern char Multi_options_proxy[512];
		extern ushort Multi_options_proxy_port;
		if(strlen(Multi_options_proxy) > 0){
			http = new ChttpGet(URL, localfile, Multi_options_proxy, Multi_options_proxy_port);
		} else {
			http = new ChttpGet(URL, localfile);
		}

		if (http==NULL) {
			m_HardError = INET_ERROR_NO_MEMORY;
		}
	} else if (strstr(URL,"ftp:")) {
		m_bUseHTTP = FALSE;
		ftp = new CFtpGet(URL,localfile);
		if (ftp==NULL) {
			m_HardError = INET_ERROR_NO_MEMORY;
		}
	} else {
		m_HardError = INET_ERROR_CANT_PARSE_URL;
	}
	Sleep(1000);
}

InetGetFile::~InetGetFile()
{
	if(http!=NULL) delete http;
	if(ftp!=NULL) delete ftp;
}

BOOL InetGetFile::IsConnecting()
{
	int state;
	if(m_bUseHTTP)
	{
		state = http->GetStatus();
	}
	else
	{
		state = ftp->GetStatus();
	}
	if(state == FTP_STATE_CONNECTING)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL InetGetFile::IsReceiving()
{
	int state;
	if(m_bUseHTTP)
	{
		state = http->GetStatus();
	}
	else
	{
		state = ftp->GetStatus();
	}
	if(state == FTP_STATE_RECEIVING)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL InetGetFile::IsFileReceived()
{
	int state;
	if(m_bUseHTTP)
	{
		state = http->GetStatus();
	}
	else
	{
		state = ftp->GetStatus();
	}
	if(state == FTP_STATE_FILE_RECEIVED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL InetGetFile::IsFileError()
{
	int state;
	if(m_HardError) return TRUE;
	if(m_bUseHTTP)
	{
		state = http->GetStatus();
	}
	else
	{
		state = ftp->GetStatus();
	}
	switch(state)
	{

	case FTP_STATE_URL_PARSING_ERROR:
	case FTP_STATE_HOST_NOT_FOUND:
	case FTP_STATE_DIRECTORY_INVALID:
	case FTP_STATE_FILE_NOT_FOUND:
	case FTP_STATE_CANT_CONNECT:
	case FTP_STATE_LOGIN_ERROR:
	case FTP_STATE_INTERNAL_ERROR:
	case FTP_STATE_SOCKET_ERROR:
	case FTP_STATE_UNKNOWN_ERROR:
	case FTP_STATE_RECV_FAILED:
	case FTP_STATE_CANT_WRITE_FILE:
		return TRUE;
	case FTP_STATE_CONNECTING:
		return FALSE;
	default:
		return FALSE;
	}
}

int InetGetFile::GetErrorCode()
{
	int state;
	if(m_HardError) return m_HardError;
	if(m_bUseHTTP)
	{
		state = http->GetStatus();
	}
	else
	{
		state = ftp->GetStatus();
	}
	switch(state)
	{

	case FTP_STATE_URL_PARSING_ERROR:
		return INET_ERROR_CANT_PARSE_URL;

	case FTP_STATE_HOST_NOT_FOUND:
		return INET_ERROR_HOST_NOT_FOUND;


	case FTP_STATE_DIRECTORY_INVALID:
	case FTP_STATE_FILE_NOT_FOUND:
		return INET_ERROR_BAD_FILE_OR_DIR;

	case FTP_STATE_CANT_CONNECT:
	case FTP_STATE_LOGIN_ERROR:
	case FTP_STATE_INTERNAL_ERROR:
	case FTP_STATE_SOCKET_ERROR:
	case FTP_STATE_UNKNOWN_ERROR:
	case FTP_STATE_RECV_FAILED:

		return INET_ERROR_UNKNOWN_ERROR;

	case FTP_STATE_CANT_WRITE_FILE:
		return INET_ERROR_CANT_WRITE_FILE;
	default:
		return INET_ERROR_NO_ERROR;
	}
}

int InetGetFile::GetTotalBytes()
{
	if(m_bUseHTTP)
	{
		return http->GetTotalBytes();
	}
	else
	{
		return ftp->GetTotalBytes();
	}
}

int InetGetFile::GetBytesIn()
{
	if(m_bUseHTTP)
	{
		return http->GetBytesIn();
	}
	else
	{
		return ftp->GetBytesIn();
	}
}

#endif // !PLAT_UNIX
