#/*
**
**	$Filename:	b3TimeAbstract.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - time control
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_TIMEABSTRACT_H
#define B3_SYSTEM_TIMEABSTRACT_H

#include "blz3/system/b3Log.h"

#define B3_MAX_TIME_SLICE 100

class B3_PLUGIN b3TimeAccumAbstract
{
public:
	virtual void  b3Init(unsigned long slice=10) = 0;
	virtual void  b3Get(unsigned long &refSpan,unsigned long &refCount) = 0;
};

class B3_PLUGIN b3TimeSpanAbstract
{
public:
	b3_f64        m_uTime;
	b3_f64        m_sTime;
	b3_f64        m_rTime;

public:
	                b3TimeSpanAbstract();
	virtual void    b3Start() = 0;
	virtual void    b3Stop() = 0;
	        void    b3Clear();
	        b3_f64  b3GetUsage();
	        void    b3Print(b3_log_level level = B3LOG_NORMAL);

private:
	static char    *b3PrintTime(char *buffer,b3_f64 time_needed);
};

class B3_PLUGIN b3TimeAbstract
{
public:
	virtual void    b3Now() = 0;
	virtual b3_f64  b3GetTime() = 0;
	
	operator b3_f64()
	{
		return b3GetTime();
	}
};

#endif
