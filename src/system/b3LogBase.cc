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
//#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

b3Mutex b3LogBase::m_LogMutex;
char    b3LogBase::m_Message[B3_MAX_LOGSIZE];
char    b3LogBase::m_LogFile[B3_MAX_LOGFILENAME];
b3_bool b3LogBase::m_AlreadyOpen = false;

b3LogBase::b3LogBase()
{
	if (!m_AlreadyOpen)
	{
		strcpy (m_LogFile,m_DefaultLogFile);
		remove (m_LogFile);

		sprintf (m_Message,
			"*** Blizzard III V%d.%02d # Debug log file ***\n"
			"Debug file:  %s\n"
			"Debug level: %d = 0x%x\n\n",
			B3_VERSION,B3_REVISION,
			m_LogFile,
			m_LogLevel,m_LogLevel);
	}
}

void b3LogBase::b3GetLogFile(char *DebugFile)
{
//	m_LogMutex.b3Lock();
	strcpy (DebugFile,m_LogFile);
//	m_LogMutex.b3Unlock();
}

b3_bool b3LogBase::b3SetLogFile(const char *DebugFile)
{
//	m_LogMutex.b3Lock();
	strcpy (m_LogFile,DebugFile);
//	m_LogMutex.b3Unlock();

	return !m_AlreadyOpen;
}
