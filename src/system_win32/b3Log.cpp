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

#include <stdio.h>

#include "stdafx.h"
#include "afxmt.h"

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**
**	Revision 1.8  2003/02/01 12:57:17  sm
**	- Ready to undo/redo!
**	
**	Revision 1.7  2002/08/02 14:52:13  sm
**	- Vertex/normal computation is now multithreaded, too.
**	- Minor changes on b3PrepareInfo class.
**	- Last changes to Windows port.
**	
**	Revision 1.6  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.5  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.4  2001/11/11 11:51:21  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
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
**                        The Blizzard III logging functions            **
**                                                                      **
*************************************************************************/

const char *b3LogBase::m_DefaultLogFile = "C:\\temp\\b3.log";

b3Log __logger;

b3Log::b3Log() : b3LogBase()
{
	if (!m_AlreadyOpen)
	{
#ifdef _DEBUG
		OutputDebugString(m_Message);
#endif
		FILE *out;

		out = fopen (m_LogFile,B3_TAPPEND);
		if (out != null)
		{
			fprintf(out,m_Message);
			fflush (out);
			fclose (out);
		}
		m_AlreadyOpen = true;
	}
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
	FILE    *out;

	B3_ASSERT(m_AlreadyOpen);
	if (m_LogLevel < 0) return;
	if (level <= m_LogLevel)
	{
		// Possibly we have multiple threads which are
		// doing logging. So we need to save this
		// piece of code.
		m_LogMutex.b3Lock();

		va_start (argptr,format);
#ifdef _DEBUG
		vsprintf(m_Message,format,argptr);
		OutputDebugString(m_Message);
#endif
		out = fopen (m_LogFile,B3_TAPPEND);
		if (out != null)
		{
			vfprintf (out,  format,argptr);
			fflush   (out);	// We want to do the output immediately!
			fclose   (out);

			vfprintf (stdout,format,argptr);
			fflush   (stdout);
		}
		else
		{
			vfprintf (stderr,format,argptr);
			fflush   (stderr);
		}
		va_end   (argptr);

		// That's it! Let's doing other to make the same...
		m_LogMutex.b3Unlock();
	}
}
