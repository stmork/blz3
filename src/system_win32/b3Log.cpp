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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"

#include <stdio.h>

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

#include "stdafx.h"
#include "afxmt.h"

/*************************************************************************
**                                                                      **
**                        The Blizzard III logging functions            **
**                                                                      **
*************************************************************************/

char b3LogBase::m_LogFile[B3_MAX_LOGFILENAME]= "C:\\temp\\b3.log";

b3Log b3Log::m_Log;

b3Log::b3Log()
{
	m_Logger = &m_Log;
#ifdef _DEBUG
	OutputDebugString(m_Message);
#endif
}

void b3Log::b3LogTime(const char *comment)
{
	struct _timeb timebuffer;

	_ftime (&timebuffer);
	b3LogFunction (B3LOG_FULL,"                      timecode %ld - %3hd (%s)\n",
		timebuffer.time,timebuffer.millitm,comment ? comment : "-");
}

/* b3PrintF() has got the same proto types as printf(). This routine */
/* is defined because MS-Windows has got no stdout. So b3PrintF() */
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
		// Possibly we have multiple threads which are
		// doing logging. So we need to save this
		// piece of code.
		b3CriticalSection lock(m_LogMutex);

		va_start (argptr,format);
#ifdef _DEBUG
		vsnprintf(m_Message,sizeof(m_Message),format,argptr);
		OutputDebugString(m_Message);
#endif
		va_end(argptr);

		if (b3OpenLogFile())
		{
			va_start (argptr,format);
			vfprintf (m_Out, format,argptr);
			fflush   (m_Out);
			va_end   (argptr);

			va_start (argptr,format);
			vfprintf (stdout,format,argptr);
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
	}
}
