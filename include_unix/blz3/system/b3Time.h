/*
**
**	$Filename:	b3Time.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - time control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_TIME_H
#define B3_SYSTEM_TIME_H

#include <sys/resource.h>
#include <sys/timeb.h>
#include "blz3/system/b3Log.h"

#define B3_MAXTIME_SLICE 100

class b3TimeAccum
{
	unsigned long size;
	unsigned long count;
	unsigned long pos;
public:
	      b3TimeAccum();
	void  b3Init(unsigned long slice=10);
	void  b3Get(unsigned long &refSpan,unsigned long &refCount);
};

class b3TimeSpan
{
	struct rusage m_UsageTime;
	struct timeb  m_RealTime;

public:
	b3_s32        m_uTime;
	b3_s32        m_sTime;
	b3_s32        m_rTime;

public:
	               b3TimeSpan();
	       void    b3Start();
	       void    b3Stop();
	       void    b3Print(b3_log_level level = B3LOG_NORMAL);
	       b3_f64  b3GetUsage();
	static char   *b3PrintTime(char *buffer,b3_s32 time_needed);
};

#endif
