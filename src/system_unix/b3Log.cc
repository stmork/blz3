/*
**
**	$Filename:	b3Log.c $
**	$Release:	Dortmund 1998, 1999, 2000 $
**	$Revision: 1454 $
**	$Date: 2006-07-02 10:10:09 +0200 (So, 02 Jul 2006) $
**	$Author: smork $
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

#include <sys/time.h>
#include <stdarg.h>
#include <pwd.h>
#include <unistd.h>

#include "blz3/b3Config.h"
#include "blz3/system/b3MutexAbstract.h"

/*************************************************************************
**                                                                      **
**                        The Blizzard III logging functions            **
**                                                                      **
*************************************************************************/

char b3LogBase::m_LogFile[B3_MAX_LOGFILENAME]= "/tmp/b3.log";

b3Log b3Log::m_Log;

b3Log::b3Log()
{
	uid_t          uid    = getuid();
	struct passwd *pw_ent = getpwuid(uid);

	snprintf(b3LogBase::m_LogFile, sizeof(b3LogBase::m_LogFile), "%s/b3.log", pw_ent->pw_dir);

	m_Logger = &m_Log;
}

void b3Log::b3LogTime(const char *comment)
{
	struct timeval stamp;

	gettimeofday(&stamp,0);
	b3LogFunction (B3LOG_FULL,"                      timecode %ld - %3hd (%s)\n",
		stamp.tv_sec,stamp.tv_usec / 1000,comment ? comment : "-");
}

/* b3PrintF() has got the same proto types as printf(). This routine */
/* is defined because MS-Windows has got no stdout. So MPrintF() */
/* makes it possible... */
/* ---------------------------------------------------------------- */
/* level:  debug level which decides if output is performed. */
/* format: format string for output */
/* ...:    arguments used */

void b3Log::b3LogFunction (
	const b3_log_level  level,
	const char         *format,...)
{
	va_list  argptr;

	if (b3CheckLevel(level))
	{
		b3CriticalSection lock(m_LogMutex);

		// Possibly we have multiple threads which are
		// doing logging. So we need to save this
		// piece of code.
		if (b3OpenLogFile())
		{
			va_start (argptr,format);
			vfprintf (m_Out,  format, argptr);
			fflush   (m_Out);	// We want to do the output immediately!
			va_end   (argptr);

			va_start (argptr,format);
			vfprintf (stdout, format, argptr);
			fflush   (stdout);
			va_end   (argptr);
		}
		else
		{
			va_start (argptr,format);
			vfprintf (stderr,format,argptr);
			fflush   (stderr);
			va_end   (argptr);
		}
		// That's it! Let's doing other to make the same...
	}
}
