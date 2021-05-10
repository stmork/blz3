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

#include "blz3/system/b3LogBase.h"
#include "blz3/system/b3File.h"

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

b3LogBase  *  b3LogBase::m_Logger;
b3Mutex       b3LogBase::m_LogMutex;
FILE     *    b3LogBase::m_Out = nullptr;
char          b3LogBase::m_Message[B3_MAX_LOGSIZE];

b3_bool b3LogBase::b3OpenLogFile()
{
	if (m_Out == nullptr)
	{
		// Delete old file
		remove(m_LogFile);

		// Prepare startup message
		snprintf(m_Message, sizeof(m_Message),
			"*** Blizzard III V%s # Debug log file ***\n"
			"Debug file:  %s\n"
			"Debug level: %d = 0x%x\n"
			"*******************************************\n\n",
			B3_VERSION,
			m_LogFile,
			m_LogLevel, m_LogLevel);

		// Do output
		m_Out = fopen(m_LogFile, B3_TAPPEND);
		if (m_Out != nullptr)
		{
			fputs(m_Message, m_Out);
			fflush(m_Out);
		}
		else
		{
			fprintf(stderr,
				"Cannot open log file %s\n"
				"Reason: %s\n"
				"Errno:  %d\n\n",
				m_LogFile,
				strerror(errno),
				errno);
			fflush(stderr);
		}
	}
	return m_Out != nullptr;
}

const char * b3LogBase::b3GetLogFile()
{
	return m_LogFile;
}

b3_bool b3LogBase::b3SetLogFile(const char * DebugFile)
{
	if (strlen(DebugFile) < (sizeof(m_LogFile) - 1))
	{
		strcpy(m_LogFile, DebugFile);
	}

	return m_Out == nullptr;
}
