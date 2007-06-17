/*
**
**	$Filename:	b3Log.c $
**	$Release:	Dortmund 2001, 2003 $
**	$Revision: 1215 $
**	$Date: 2005-08-07 11:31:35 +0200 (So, 07 Aug 2005) $
**	$Author: sm $
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

/**
 * This class provides logging.
 */
class B3_PLUGIN b3Log : public b3LogBase
{
	static b3Log m_Log;

private:
	        b3Log();
public:
	void    b3LogTime    (const char *comment = null);
	void    b3LogFunction(const b3_log_level debug_level,const char *format,...);
};

#endif
