/*
**
**	$Filename:	b3Time.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - date routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Time.h"
#include <sys/time.h>

/*************************************************************************
**                                                                      **
**                        b3TimeSpan routines                           **
**                                                                      **
*************************************************************************/

void b3TimeSpan::b3Start()
{
	gettimeofday(&m_RealTime, 0);
	getrusage(RUSAGE_SELF, &m_UsageTime);
#ifdef VERBOSE
	b3PrintF(B3LOG_NORMAL, "Thread time start: %9d,%06d\n",
		m_UsageTime.ru_utime.tv_sec,
		m_UsageTime.ru_utime.tv_usec);
#endif
}

void b3TimeSpan::b3Stop()
{
	struct rusage  usage_stop;
	struct timeval real_stop;

	getrusage(RUSAGE_SELF, &usage_stop);
	gettimeofday(&real_stop, 0);
#ifdef VERBOSE
	b3PrintF(B3LOG_NORMAL, "Thread time stop:  %9d,%06d\n",
		usage_stop.ru_utime.tv_sec,
		usage_stop.ru_utime.tv_usec);
#endif

	m_uTime += (
			usage_stop.ru_utime.tv_sec  + (b3_f64)usage_stop.ru_utime.tv_usec  / 1000000.0 -
			m_UsageTime.ru_utime.tv_sec - (b3_f64)m_UsageTime.ru_utime.tv_usec / 1000000.0);

	m_sTime += (
			usage_stop.ru_stime.tv_sec  + (b3_f64)usage_stop.ru_stime.tv_usec  / 1000000.0 -
			m_UsageTime.ru_stime.tv_sec - (b3_f64)m_UsageTime.ru_stime.tv_usec / 1000000.0);

	m_rTime += (
			real_stop.tv_sec  + (b3_f64)real_stop.tv_usec  / 1000000.0  -
			m_RealTime.tv_sec - (b3_f64)m_RealTime.tv_usec / 1000000.0);
}

/*************************************************************************
**                                                                      **
**                        b3Time routines                               **
**                                                                      **
*************************************************************************/

b3Time::b3Time()
{
	b3Time::b3Now();
}

b3Time::b3Time(const b3Time & orig) : m_TimePoint(orig.m_TimePoint)
{
}

b3_f64 b3Time::b3Now()
{
	gettimeofday(&m_TimePoint, 0);
	return *this;
}

b3_f64 b3Time::b3GetTime() const
{
	return m_TimePoint.tv_sec + (double)m_TimePoint.tv_usec / 1000000.0;
}

std::chrono::microseconds b3Time::b3GetStdTime() const
{
	std::chrono::seconds      s(m_TimePoint.tv_sec);
	std::chrono::microseconds ms(m_TimePoint.tv_usec);

	return s + ms;
}

b3Time & b3Time::operator=(const b3Time & orig)
{
	m_TimePoint = orig.m_TimePoint;
	return *this;
}
