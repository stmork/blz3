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
#include <stdafx.h>
#include <sys/timeb.h>

class b3TimeAccum : b3TimeAccumAbstract
{
	b3_u32        size;
	b3_u32        count;
	b3_u32        pos;
	struct _timeb buffer[B3_MAX_TIME_SLICE];

public:
	      b3TimeAccum();
	void  b3Init(b3_u32 slice=10);
	void  b3Get(b3_u32 &refSpan,b3_u32 &refCount);
};

class b3TimeSpan : public b3TimeSpanAbstract
{
	HANDLE        m_ThreadHandle;
	FILETIME      m_uStart;
	FILETIME      m_sStart;
	struct timeb  m_RealTime;

public:
	               b3TimeSpan();
	       void    b3Start();
	       void    b3Stop();

private:
	static b3_f64  b3DiffDiv10000(FILETIME *first,FILETIME *last);
};

class b3Time : public b3TimeAbstract
{
	struct timeb m_TimePoint;

public:
	        b3Time();
			b3Time(b3Time &orig);
	void    b3Now();
	b3_f64  b3GetTime();
	b3Time &operator=(b3Time &orig);
};

#endif
