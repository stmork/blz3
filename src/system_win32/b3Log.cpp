/*
**
**	$Filename:	b3Log.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - logging routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#define no_PRINT_THREAD

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include <stdio.h>

#include "stdafx.h"
#include "afxmt.h"

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Thread.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**
**	Revision 1.2  2001/07/02 19:28:25  sm
**	- Applying console application on Windows 32
**	- Added further Windows environment
**	
**	Revision 1.1  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        static variables                              **
**                                                                      **
*************************************************************************/

static FILE *bout;
#define B3_DEFAULT_FILE "C:\\temp\\b3.log"

static char  B3_OUT[B3_FILESTRINGLEN] = B3_DEFAULT_FILE;
static bool  alreadyOpen             = false;

#ifndef _DEBUG
static long  logLevel    = B3LOG_NORMAL;	// normal version
#else
static long  logLevel    = B3LOG_FULL;	// debug version
#endif

static b3IPCMutex LogMutex;

/*************************************************************************
**                                                                      **
**                        The Blizzard III logging functions            **
**                                                                      **
*************************************************************************/

void b3Log_SetLevel(const b3_log_level debug_limit)
{
	logLevel = debug_limit;
}

b3_bool b3CheckLevel(const b3_log_level debug_limit)
{
	return logLevel >= debug_limit;
}

void b3Log_GetFile(char *DebugFile)
{
	strcpy (DebugFile,B3_DEFAULT_FILE);
}

b3_bool b3Log_SetFile(const char *DebugFile)
{
	if (!alreadyOpen)
	{
		strcpy (B3_OUT,DebugFile);
	}
	return !alreadyOpen;
}

void b3PrintT(const char *comment)
{
	struct _timeb timebuffer;

	_ftime (&timebuffer);
	b3PrintF (B3LOG_FULL,"                      timecode %ld - %3hd (%s)\n",
		timebuffer.time,timebuffer.millitm,comment ? comment : "-");
}

/* MPrintF() has got the same proto types as printf(). This routine */
/* is defined because MS-Windows has got no stdout. So MPrintF() */
/* makes it possible... */
/* ---------------------------------------------------------------- */
/* level:  debug level which decides if output is performed. */
/* format: format string for output */
/* ...:    arguments used */

void b3PrintF (
	const b3_log_level  level,
	const char         *format,...)
{
	va_list argptr;
	bool    start_message = false;
#ifdef _DEBUG
	char    message[20480];
#endif

	if (logLevel < 0) return;
	if (level <= logLevel)
	{
		// Possibly we have multiple threads which are
		// doing logging. So we need to save this
		// piece of code.
		LogMutex.b3Lock();

		// If we are the first using this code, we must
		// init logging...
		if (!alreadyOpen)
		{
			remove (B3_OUT);
			alreadyOpen   = true;
			start_message = true;
		}

		// Open the file
		if (bout == null) bout = fopen (B3_OUT,TAPPEND);
		if (bout == null) bout = stderr;

		// Make start message if necessary
		if (start_message)
		{
#ifdef _DEBUG
			sprintf (message,
#else
			fprintf (bout,
#endif
				"*** Blizzard III V%ld.%02ld # Debug log file ***\n"
				"Debug file:  %s\n"
				"Debug level: %ld = 0x%lx\n\n",
				B3_VERSION,B3_REVISION,
				B3_OUT,
				logLevel,logLevel);
#ifdef _DEBUG
			fprintf (bout,message);
#ifdef _WINDOWS
			OutputDebugString (message);
#endif
#endif
		}

		// Now we are doing to format the logging text
#ifdef PRINT_THREAD
		fprintf  (bout,"(%02lX)",GetCurrentThreadId());
#endif
		va_start (argptr,format);
		vfprintf (bout,format,argptr);
		fflush   (bout);	// We want to do the output immediately!

		// Close the file
		if (bout != stderr)
		{
			fclose (bout);
			bout = null;
		}

		// If we are debugging - do the output in the VC++ output window
#ifdef _DEBUG
		message[0] = 0;
#ifdef PRINT_THREAD
		sprintf           (message,"(%02lX)",GetCurrentThreadId());
#ifdef _WINDOWS
		OutputDebugString (message);
#endif
#endif

		vsprintf          (message,format,argptr);
#ifdef _WINDOWS
		OutputDebugString (message);
#endif
#endif
		va_end   (argptr);

		// That's it! Let's doing other to make the same...
		LogMutex.b3Unlock();
	}
}
