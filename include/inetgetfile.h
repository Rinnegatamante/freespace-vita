/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
* $Logfile: /Freespace2/code/Inetfile/inetgetfile.h $
* $Revision: 110 $
* $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
* $Author: relnev $
*
* InternetGetFile Class header
*
* $Log$
* Revision 1.2  2002/06/09 04:41:13  relnev
* added copyright header
*
* Revision 1.1.1.1  2002/05/03 03:28:12  root
* Initial import.
*
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
#ifndef _INET_GETFILE_HEADER_
#define _INET_GETFILE_HEADER_

//At the end of this file is an example of usage

#include "cftp.h"
#include "chttpget.h"

#define INET_ERROR_NO_ERROR			0
#define INET_ERROR_BADPARMS			1
#define INET_ERROR_CANT_WRITE_FILE	2
#define INET_ERROR_CANT_PARSE_URL	3
#define INET_ERROR_BAD_FILE_OR_DIR	4
#define INET_ERROR_HOST_NOT_FOUND	5
#define INET_ERROR_UNKNOWN_ERROR		6
#define INET_ERROR_NO_MEMORY        7

class InetGetFile
{
public:
	InetGetFile(char *URL,char *localfile);
	~InetGetFile();
	BOOL IsFileReceived();
	BOOL IsFileError();
	BOOL IsConnecting();
	BOOL IsReceiving();
	int GetErrorCode();
	int GetBytesIn();
	int GetTotalBytes();
	void AbortGet();

protected:
	CFtpGet *ftp;
	ChttpGet *http;
	BOOL m_bUseHTTP;
	int m_ErrorCode;
	int m_State;
	int m_HardError;

};

#endif

/*

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "inetgetfile.h"

int main(int argc,char **argv)
{
	unsigned int LastPrintbytes = time(NULL);
	InetGetFile *inetfile;
	WSADATA	ws_data;
	WORD ver=MAKEWORD(1,1);
	
	int error=WSAStartup(ver,&ws_data);
	inetfile = new InetGetFile("http://www.volition-inc.com/images/download/freespace/fsdemo1x-12u.exe","e:\\fsdemo1x-12u.exe");
	do
	{
		if(inetfile->IsFileReceived())
		{
			printf("File received\n");
			break;
		}
		if(inetfile->IsFileError())
		{
			printf("File not received. Error code: %d\n",inetfile->GetErrorCode());
			break;
		}
		if(time(NULL)-LastPrintbytes>=1)
		{
			int ipct = 0;
			if(inetfile->GetTotalBytes())
			{
				ipct = 100*(float)((float)inetfile->GetBytesIn()/(float)inetfile->GetTotalBytes());
			}
			printf("Received %d Bytes out of %d (%d%%).\n",inetfile->GetBytesIn(),inetfile->GetTotalBytes(),ipct);
			LastPrintbytes = time(NULL);
		}


	}while(!kbhit());
	return 0;

}



 */

