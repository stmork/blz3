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

#include <sys/time.h>
#include <time.h>
#include <stdarg.h>

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.13  2004/06/19 13:56:55  sm
**	- Changed log file location to /tmp.
**
**	Revision 1.12  2003/06/15 14:18:18  sm
**	- Updated item maintain dialog to icons
**	- Changed b3Log into a singleton
**	
**	Revision 1.11  2003/05/30 14:44:09  sm
**	- Plugin support for Windows added. All exported classes got an
**	  additional keyword: B3_PLUGIN
**	
**	Revision 1.10  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
**	Revision 1.9  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
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
**                        The Blizzard III logging functions            **
**                                                                      **
*************************************************************************/

char b3LogBase::m_LogFile[B3_MAX_LOGFILENAME]= "/tmp/b3.log";

b3Log b3Log::m_Log;

b3Log::b3Log()
{
	m_Logger = &m_Log;
}

void b3Log::b3LogTime(const char *comment)
{
	struct timeval stamp;

	gettimeofday(&stamp,0);
	b3LogFunction (B3LOG_FULL,"                      timecode %ld - %3hd (%s)\n",
		stamp.tv_sec,stamp.tv_usec / 1000,comment ? comment : "-");
}

/* MPrintF() has got the same proto types as printf(). This routine */
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
		// Possibly we have multiple threads which are
		// doing logging. So we need to save this
		// piece of code.
		m_LogMutex.b3Lock();

		va_start (argptr,format);
		if (m_Out != null)
		{
			vfprintf (m_Out,  format,argptr);
			fflush   (m_Out);	// We want to do the output immediately!

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
