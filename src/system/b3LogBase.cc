/*
**
**	$Filename:	b3Logbase.c $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - logging routines
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#include "blz3/b3Config.h"
#include <errno.h>
#include <unistd.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.10  2005/06/14 13:24:08  smork
**	- Changed remove to truncate.
**
**	Revision 1.9  2005/06/13 10:43:41  smork
**	- Log file moved into home directory.
**	
**	Revision 1.8  2005/06/02 13:21:19  smork
**	- Cleanup.
**	
**	Revision 1.7  2005/06/02 13:20:01  smork
**	- Write log file error reason on stderr.
**	
**	Revision 1.6  2004/01/18 13:51:58  sm
**	- Done further security issues.
**	
**	Revision 1.5  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.4  2003/06/15 14:18:18  sm
**	- Updated item maintain dialog to icons
**	- Changed b3Log into a singleton
**	
**	Revision 1.3  2003/02/22 15:17:18  sm
**	- Added support for selected shapes in object modeller
**	- Glued b3Shape and b3ShapeRenderObject. There was no
**	  distinct access method in use.
**	- Made some b3Shape methods inline and/or static which
**	  saves some memory.
**	
**	Revision 1.2  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
**	Revision 1.1  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
*/

/*************************************************************************
**                                                                      **
**                        static variables                              **
**                                                                      **
*************************************************************************/

#ifndef _DEBUG
b3_log_level  b3LogBase::m_LogLevel = B3LOG_NORMAL;	// normal version
#else
b3_log_level  b3LogBase::m_LogLevel = B3LOG_FULL;	// debug version
#endif

b3LogBase    *b3LogBase::m_Logger;
b3Mutex       b3LogBase::m_LogMutex;
FILE         *b3LogBase::m_Out = null;
char          b3LogBase::m_Message[B3_MAX_LOGSIZE];

b3LogBase::b3LogBase()
{
}

b3_bool b3LogBase::b3OpenLogFile()
{
	if (m_Out == null)
	{
		// Delete old file
		truncate (m_LogFile, 0);

		// Prepare startup message
		snprintf (m_Message,sizeof(m_Message),
			"*** Blizzard III V%d.%02d # Debug log file ***\n"
			"Debug file:  %s\n"
			"Debug level: %d = 0x%x\n"
			"*******************************************\n\n",
			B3_VERSION,B3_REVISION,
			m_LogFile,
			m_LogLevel,m_LogLevel);

		// Do output
		m_Out = fopen (m_LogFile, B3_TAPPEND);
		if (m_Out != null)
		{
			fprintf(m_Out, m_Message);
			fflush (m_Out);
		}
		else
		{
			fprintf(stderr, m_Message);
			fprintf(stderr, 
				"Cannot open log file %s\n"
				"Reason: %s\n"
				"Errno:  %d\n\n",
				m_LogFile,
				strerror(errno),
				errno);
			fflush (stderr);
		}
	}
	return m_Out != null;
}

void b3LogBase::b3GetLogFile(char *DebugFile)
{
	strcpy (DebugFile,m_LogFile);
}

b3_bool b3LogBase::b3SetLogFile(const char *DebugFile)
{
	strlcpy (m_LogFile,DebugFile,sizeof(m_LogFile));

	return m_Out == null;
}
