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

b3TimeSpan::b3TimeSpan()
{
	m_uTime = 0;
	m_sTime = 0;
	m_rTime = 0;
}

void b3TimeSpan::b3Start()
{
	ftime(&m_RealTime);
	getrusage(RUSAGE_SELF,&m_UsageTime);
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

	getrusage(RUSAGE_SELF,&usage_stop);
	ftime(&real_stop);
#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL,"%d,%d\n",
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

b3_f64 b3TimeSpan::b3GetUsage()
{
	return (m_rTime > 0 ?
		(b3_f64)(m_uTime + m_sTime) / (b3_f64)m_rTime : 1.0);
}

char *b3TimeSpan::b3PrintTime(char *buffer,b3_s32 time_needed)
{
	sprintf(buffer,"%3d:%02d,%02d",
		 time_needed / 60000,
		(time_needed /  1000) % 60,
		 time_needed %  1000);
	return buffer;
}

void b3TimeSpan::b3Print(b3_log_level level)
{
	char buffer[32];

	b3PrintF(level,"Computation time:\n");
	b3PrintF(level," Time needed: %s\n",b3PrintTime(buffer,m_rTime));
	b3PrintF(level," User time:   %s\n",b3PrintTime(buffer,m_uTime));
	b3PrintF(level," System time: %s\n",b3PrintTime(buffer,m_sTime));
	b3PrintF(level," Load:        %3.2f%%\n",b3GetUsage() * 100.0);
}
