/*
**
**	$Filename:	b3Log.c $
**	$Release:	Dortmund 1998, 1999, 2000 $
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
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>

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
**	Revision 1.8  2002/08/24 13:08:22  sm
**	- Removed old time measuring code which were commented out.
**
**	Revision 1.7  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.6  2002/07/25 13:22:32  sm
**	- Introducing spot light
**	- Optimized light settings when drawing
**	- Further try of stencil maps
**	
**	Revision 1.5  2002/03/13 19:01:59  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.4  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.3  2001/07/02 16:09:46  sm
**	- Added bounding box reorganization.
**	
**	Revision 1.2  2001/07/01 16:48:00  sm
**	- FILESTRINGLEN -> B3_FILESTRINGLEN
**	- Cleaned up some makefiles
**	- Cleaned up some CVS conflicts
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
*/

/*************************************************************************
**                                                                      **
**                        static variables                              **
**                                                                      **
*************************************************************************/

static FILE *bout;

#define B3_DEFAULT_FILE "b3.log"

static char  B3_OUT[B3_FILESTRINGLEN] = B3_DEFAULT_FILE;
static bool  alreadyOpen              = false;

#ifndef _DEBUG
static b3_log_level  logLevel = B3LOG_NORMAL;	// normal version
#else
static b3_log_level  logLevel = B3LOG_FULL;	// debug version
#endif

static b3IPCMutex LogMutex;

/*************************************************************************
**                                                                      **
**                        The Blizzard III logging functions            **
**                                                                      **
*************************************************************************/

b3_log_level b3Log_SetLevel(const b3_log_level debug_limit)
{
	b3_log_level oldLevel = logLevel;

	logLevel = debug_limit;
	return oldLevel;
}

b3_bool b3CheckLevel(const b3_log_level debug_limit)
{
	return logLevel >= debug_limit;
}

void b3Log_GetFile(char *DebugFile)
{
	strcpy (DebugFile,B3_DEFAULT_FILE);
}

bool b3Log_SetFile(const char *DebugFile)
{
	if (!alreadyOpen)
	{
		strcpy (B3_OUT,DebugFile);
	}
	return !alreadyOpen;
}

void b3PrintT(const char *comment)
{
	struct timeval stamp;

	gettimeofday(&stamp,0);
	b3PrintF (B3LOG_FULL,"                      timecode %ld - %3hd (%s)\n",
		stamp.tv_sec,stamp.tv_usec / 1000,comment ? comment : "-");
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
		if (bout == null) bout = fopen (B3_OUT,B3_TAPPEND);
		if (bout == null) bout = stderr;

		// Make start message if necessary
		if (start_message)
		{
#ifdef _DEBUG
			sprintf (message,
#else
			fprintf (bout,
#endif
				"*** Blizzard III V%d.%02d # Debug log file ***\n"
				"Debug file:  %s\n"
				"Debug level: %d = 0x%x\n\n",
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
		vfprintf (bout,  format,argptr);
		vfprintf (stdout,format,argptr);
		fflush   (bout);	// We want to do the output immediately!
		fflush   (stdout);

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
