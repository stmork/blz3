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
**	Revision 1.14  2005/05/07 14:06:06  sm
**	- Corrected va-list handling in Windows version.
**	- Using correct lib directory on 64 bit architectures.
**	- Added login script version for (ba)sh.
**
**	Revision 1.13  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
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
		m_LogMutex.b3Lock();

		va_start (argptr,format);
#ifdef _DEBUG
		vsnprintf(m_Message,sizeof(m_Message),format,argptr);
		OutputDebugString(m_Message);
#endif
		va_end   (argptr);

		if (m_Out != null)
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

		// That's it! Let's doing other to make the same...
		m_LogMutex.b3Unlock();
	}
}
