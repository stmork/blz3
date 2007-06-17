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

#include "b3SystemIndInclude.h"
#include <errno.h>

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
		remove (m_LogFile);

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
