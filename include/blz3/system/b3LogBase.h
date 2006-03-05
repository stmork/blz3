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
#include <assert.h>

#include "blz3/b3Types.h"
#include "blz3/b3PluginDef.h"

/**
 * This enumeration specifies some log levels.
 */
enum b3_log_level
{
	B3LOG_NONE   =  0, //!< No logging.
	B3LOG_NORMAL = 10, //!< Normal logging.
	B3LOG_DEBUG  = 20, //!< Logging with software settings.
	B3LOG_FULL   = 30  //!< Full logging for software debugging purposes.
};

class B3_PLUGIN b3Mutex;

/**
 * This class provides several logging methods.
 */
class B3_PLUGIN b3LogBase
{
	static const b3_count B3_MAX_LOGSIZE     = 20480;
	static const b3_count B3_MAX_LOGFILENAME =   128;

protected:
	static b3LogBase    *m_Logger;     //!< The logging instance.
	static FILE         *m_Out;        //!< The log file where to log into.
	static b3_log_level  m_LogLevel;   //!< The log level to use.
	static b3Mutex       m_LogMutex;   //!< A mutex for thread safe logging.
	static char          m_Message[B3_MAX_LOGSIZE];     //!< A temporary message buffer.
	static char          m_LogFile[B3_MAX_LOGFILENAME]; //!< The log file name.

protected:
	/**
	 * This constructor prepares for logging.
	 */
	b3LogBase();

	/**
	 * This method prepares the log file.
	 *
	 * @return True on success.
	 */
	b3_bool b3OpenLogFile();

public:
	/**
	 * This method sets a new log level.
	 *
	 * @param loglevel The new log level.
	 * @return The old log level.
	 */
	static inline b3_log_level b3SetLevel(const b3_log_level loglevel)
	{
		b3_log_level oldLevel = m_LogLevel;

		m_LogLevel = loglevel;
		return oldLevel;
	}

	/**
	 * This method checks for logging. If the specified debug limit
	 * is lower than the logging level of this instance nothing is logged.
	 *
	 * @param loglevel The log level to check.
	 * @return True if logging is permitted.
	 */
	static inline b3_bool b3CheckLevel(const b3_log_level loglevel)
	{
		return (m_LogLevel >= 0) && (m_LogLevel >= loglevel);
	}

	/**
	 * This method returns the logging instance.
	 *
	 * @return The logging instance.
	 */
	static  inline b3LogBase *b3GetLogger()
	{
		assert(m_Logger != null);
		return m_Logger;
	}

	/**
	 * This method returns the actual log file name.
	 *
	 * @return The new log file name.
	 */
	static  void    b3GetLogFile (      char         *logfile);
	
	/**
	 * This method specifies the log file wher to log into.
	 *
	 * @param logfile The new log file name.
	 */
	static  b3_bool b3SetLogFile (const char         *logfile);

	/**
	 * This method logs a comment including a time stamp.
	 *
	 * \param *comment The comment to print.
	 */
	virtual void    b3LogTime    (const char         *comment = 0) = 0;

	/**
	 * Tis method logs a message with a specified log level. The message is a
	 * printf like format string. The agruments are specified after this format
	 * string.
	 *
	 * \param debug_level The log level to use.
	 * \param *format     The format string.
	 * \param ...         The arguments used by the format string.
	 */
	virtual void    b3LogFunction(const b3_log_level  debug_level,const char *format,...) = 0;
};

#define b3PrintT b3LogBase::b3GetLogger()->b3LogTime
#define b3PrintF b3LogBase::b3GetLogger()->b3LogFunction

#endif
