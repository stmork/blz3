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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2002/08/19 16:50:39  sm
**	- Now having animation running, running, running...
**	- Activation handling modified to reflect animation
**	  and user transformation actions.
**	- Made some architectual redesigns.
**
**	Revision 1.4  2002/08/16 13:20:14  sm
**	- Removed some unused methods.
**	- Allocation bug found in brt3 - the Un*x version of the
**	  Blizzard III raytracer: It's necessary to use b3ShapeRenderContext
**	  rather than b3renderContext which doesn't initialize subdivision
**	  for shapes.
**	
**	Revision 1.3  2002/08/11 11:03:41  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**	
**	Revision 1.2  2001/11/07 15:55:09  sm
**	- Introducing b3TimeSpan to Windows to get computation time on
**	  Windows as well.
**	- Changed some include dependencies.
**	
**	Revision 1.1  2001/11/02 19:05:36  sm
**	- Introducing time mearuring.
**	- Fixed wrong lens flare handling.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3TimeSpan routines                           **
**                                                                      **
*************************************************************************/

void b3TimeSpan::b3Start()
{
	ftime(&m_RealTime);
	getrusage(RUSAGE_SELF,&m_UsageTime);
#ifdef _VERBOSE
	b3PrintF(B3LOG_NORMAL,"Thread time start: %9d,%06d\n",
		m_UsageTime.ru_utime.tv_sec,
		m_UsageTime.ru_utime.tv_usec);
#endif
}

void b3TimeSpan::b3Stop()
{
	struct rusage usage_stop;
	struct timeb  real_stop;

	getrusage(RUSAGE_SELF,&usage_stop);
	ftime(&real_stop);
#ifdef _VERBOSE
	b3PrintF(B3LOG_NORMAL,"Thread time stop:  %9d,%06d\n",
		usage_stop.ru_utime.tv_sec,
		usage_stop.ru_utime.tv_usec);
#endif

	m_uTime += (
		 usage_stop.ru_utime.tv_sec  * 1000 + 
		 usage_stop.ru_utime.tv_usec / 1000 -
		m_UsageTime.ru_utime.tv_sec  * 1000 -
		m_UsageTime.ru_utime.tv_usec / 1000);

	m_sTime += (
		 usage_stop.ru_stime.tv_sec  * 1000 + 
		 usage_stop.ru_stime.tv_usec / 1000 -
		m_UsageTime.ru_stime.tv_sec  * 1000 -
		m_UsageTime.ru_stime.tv_usec / 1000);

	m_rTime += (
		 real_stop.time    * 1000 + 
		 real_stop.millitm        -
		m_RealTime.time    * 1000 -
		m_RealTime.millitm);
}

/*************************************************************************
**                                                                      **
**                        b3Time routines                               **
**                                                                      **
*************************************************************************/

b3Time::b3Time()
{
	b3Now();
}

b3Time::b3Time(b3Time &orig)
{
	m_TimePoint = orig.m_TimePoint;
}

void b3Time::b3Now()
{
	gettimeofday(&m_TimePoint,0);
}

b3_f64 b3Time::b3GetTime()
{
	return m_TimePoint.tv_sec + (double)m_TimePoint.tv_usec / 1000000.0;
}

b3Time &b3Time::operator=(b3Time &orig)
{
	m_TimePoint = orig.m_TimePoint;
	return *this;
}
