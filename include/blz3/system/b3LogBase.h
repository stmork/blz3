/*
**
**	$Filename:	b3LogBase.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - logging routines (proto types)
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_LOGBASE_H
#define B3_SYSTEM_LOGBASE_H

#include <stdio.h>

#include "blz3/b3Types.h"

enum b3_log_level
{
	B3LOG_NONE   =  0,
	B3LOG_NORMAL = 10,
	B3LOG_DEBUG  = 20,
	B3LOG_FULL   = 30 
};

#define B3_MAX_LOGSIZE     20480
#define B3_MAX_LOGFILENAME   128

class b3Mutex;

class b3LogBase
{
protected:
	static FILE         *m_Out;
	static b3_log_level  m_LogLevel;
	static b3Mutex       m_LogMutex;
	static char          m_Message[B3_MAX_LOGSIZE];
	static char          m_LogFile[B3_MAX_LOGFILENAME];

protected:
	b3LogBase();

public:
	static inline b3_log_level b3SetLevel(const b3_log_level debug_limit)
	{
		b3_log_level oldLevel = m_LogLevel;

		m_LogLevel = debug_limit;
		return oldLevel;
	}

	static inline b3_bool b3CheckLevel(const b3_log_level debug_limit)
	{
		return (m_LogLevel >= 0) && (m_LogLevel >= debug_limit);
	}

	static  void    b3GetLogFile (      char         *debug_file);
	static  b3_bool b3SetLogFile (const char         *debug_file);

	virtual void    b3LogTime    (const char         *comment = 0) = 0;
	virtual void    b3LogFunction(const b3_log_level  debug_level,const char *format,...) = 0;
};

#define b3PrintT __logger.b3LogTime
#define b3PrintF __logger.b3LogFunction

#endif
