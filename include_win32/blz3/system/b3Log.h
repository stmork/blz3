/*
**
**	$Filename:	b3Log.h $
**	$Release:	Dortmund 2001, 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - logging routines (proto types)
**
**	(C) Copyright 2001, 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_LOG_H
#define B3_SYSTEM_LOG_H

#include "blz3/system/b3LogBase.h"

class B3_PLUGIN b3Log : public b3LogBase
{
	static b3Log m_Log;

private:
	        /**
	         * The constructor for initializing the logger.
	         */
	        b3Log();
public:
	/**
	 * This method logs a comment including a time stamp.
	 *
	 * \param *comment The comment to print.
	 */
	void    b3LogTime    (const char         *comment = null);

	/**
	 * Tis method logs a message with a specified log level. The message is a
	 * printf like format string. The agruments are specified after this format
	 * string.
	 *
	 * \param debug_level The log level to use.
	 * \param *format     The format string.
	 * \param ...         The arguments used by the format string.
	 */
	void    b3LogFunction(const b3_log_level  debug_level,const char *format,...);
};

#endif
