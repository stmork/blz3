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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Time.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/11/02 19:05:36  sm
**	- Introducing time mearuring.
**	- Fixed wrong lens flare handling.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Date routines                             **
**                                                                      **
*************************************************************************/

b3TimeSpan::b3TimeSpan()
{
	m_uTime = 0;
	m_sTime = 0;
	m_rTime = 0;
}

void b3TimeSpan::b3Start()
{
	ftime(&m_RealTime);
	getrusage(RUSAGE_CHILDREN,&m_UsageTime);
#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL,"%d,%d\n",
		m_UsageTime.ru_utime.tv_sec,
		m_UsageTime.ru_utime.tv_usec);
#endif
}

void b3TimeSpan::b3Stop()
{
	struct rusage usage_stop;
	struct timeb  real_stop;

	getrusage(RUSAGE_CHILDREN,&usage_stop);
	ftime(&real_stop);
#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL,"%d,%d\n",
		usage_stop.ru_utime.tv_sec,
		usage_stop.ru_utime.tv_usec);
#endif

	m_uTime =
		 usage_stop.ru_utime.tv_sec  *   100 + 
		 usage_stop.ru_utime.tv_usec / 10000 -
		m_UsageTime.ru_utime.tv_sec  *   100 -
		m_UsageTime.ru_utime.tv_usec / 10000;

	m_sTime =
		 usage_stop.ru_stime.tv_sec  *   100 + 
		 usage_stop.ru_stime.tv_usec / 10000 -
		m_UsageTime.ru_stime.tv_sec  *   100 -
		m_UsageTime.ru_stime.tv_usec / 10000;

	m_rTime =
		 real_stop.time    * 100 + 
		 real_stop.millitm /  10 -
		m_RealTime.time    * 100 -
		m_RealTime.millitm /  10;
}

b3_f64 b3TimeSpan::b3GetUsage()
{
	return (m_rTime > 0 ?
		(b3_f64)(m_uTime + m_sTime) / (b3_f64)m_rTime : 1.0);
}

char *b3TimeSpan::b3PrintTime(char *buffer,b3_s32 time_needed)
{
	sprintf(buffer,"%3d:%02d,%02d",
		 time_needed / 6000,
		(time_needed / 100) % 60,
		 time_needed % 100);
	return buffer;
}

void b3TimeSpan::b3Print()
{
	char buffer[32];

	b3PrintF(B3LOG_NORMAL,"Computation time:\n");
	b3PrintF(B3LOG_NORMAL," Time needed: %s\n",b3PrintTime(buffer,m_rTime));
	b3PrintF(B3LOG_NORMAL," User time:   %s\n",b3PrintTime(buffer,m_uTime));
	b3PrintF(B3LOG_NORMAL," System time: %s\n",b3PrintTime(buffer,m_sTime));
	b3PrintF(B3LOG_NORMAL," Load:        %3.2f%%\n",b3GetUsage());
}
