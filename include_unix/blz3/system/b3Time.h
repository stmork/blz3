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

#include "blz3/system/b3TimeAbstract.h"
#include <sys/resource.h>
#include <sys/timeb.h>

class b3TimeSpan : public b3TimeSpanAbstract
{
	struct rusage m_UsageTime;
	struct timeb  m_RealTime;

public:
	       void    b3Start();
	       void    b3Stop();
};

class b3Time : public b3TimeAbstract
{
	struct timeval m_TimePoint;

public:
	        b3Time();
			b3Time(b3Time &orig);
	void    b3Now();
	b3_f64  b3GetTime();
	b3Time &operator=(b3Time &orig);
};

#endif
