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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

b3Mutex  b3LogBase::m_LogMutex;
FILE    *b3LogBase::m_Out = null;
char     b3LogBase::m_Message[B3_MAX_LOGSIZE];

b3LogBase::b3LogBase()
{
	if (m_Out == null)
	{
		// Delete old file
		remove (m_LogFile);

		// Prepare startup message
		sprintf (m_Message,
			"*** Blizzard III V%d.%02d # Debug log file ***\n"
			"Debug file:  %s\n"
			"Debug level: %d = 0x%x\n\n",
			B3_VERSION,B3_REVISION,
			m_LogFile,
			m_LogLevel,m_LogLevel);

		// Do output
		m_Out = fopen (m_LogFile,B3_TAPPEND);
		if (m_Out != null)
		{
			fprintf(m_Out,m_Message);
			fflush (m_Out);
		}
		else
		{
			fprintf(stderr,m_Message);
			fflush (stderr);
		}
	}
}

void b3LogBase::b3GetLogFile(char *DebugFile)
{
	strcpy (DebugFile,m_LogFile);
}

b3_bool b3LogBase::b3SetLogFile(const char *DebugFile)
{
	strcpy (m_LogFile,DebugFile);

	return m_Out == null;
}
