/*
**
**	$Filename:	b3Time.cpp $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Time.h"
#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2001/11/07 15:55:09  sm
**	- Introducing b3TimeSpan to Windows to get computation time on
**	  Windows as well.
**	- Changed some include dependencies.
**
**	Revision 1.3  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.2  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**	
**	Revision 1.1  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Time routines                             **
**                                                                      **
*************************************************************************/

b3TimeAccum::b3TimeAccum()
{
	count = 0;
	pos   = 0;
	size  = 0;
}

void b3TimeAccum::b3Init(b3_u32 slice)
{
	if (slice >= B3_MAX_TIME_SLICE)
	{
		slice = B3_MAX_TIME_SLICE - 1;
	}
	if (slice < 1)
	{
		slice = 1;
	}
	count = 0;
	pos   = 1;
	size  = slice + 1;
	_ftime(&buffer[0]);
}

void b3TimeAccum::b3Get(
	b3_u32 &refTime,
	b3_u32 &refCount)
{
	// Get actual time and compute time difference to scan start
	// This algorithm doesn't work at 24:00 o'clock!
	struct _timeb *time_start;
	struct _timeb *time_actual;
	b3_u32         index_start;
	b3_u32         index_actual;
	b3_u32         time_span;

	// Set buffer full ratio
	if (pos < size)
	{
		pos++;
	}

	// Set time positions
	count++;
	index_start  = (count - pos + 1) % size;
	index_actual =  count            % size;
	time_start   = &buffer[index_start];
	time_actual  = &buffer[index_actual];
	_ftime (time_actual);

	b3PrintF (B3LOG_FULL,"### CLASS: b3Time ################### "
		"Count: %4ld Pos: %3ld ## S: %3ld A: %3ld (%ld)\n",
		count,pos - 1,index_start,index_actual,size);

	// Difference in seconds
	time_span = (time_actual->time - time_start->time) * 1000;

	// Test for overflow in milliseconds
	if (time_actual->millitm < time_start->millitm)
	{
		time_span += (1000 + time_actual->millitm - time_start->millitm);
	}
	else
	{
		time_span +=        (time_actual->millitm - time_start->millitm);
	}
	refTime  = time_span;
	refCount = pos - 1;
}

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
	m_ThreadHandle = GetCurrentThread();
}

void b3TimeSpan::b3Start()
{
	FILETIME start_time,exit_time;

	GetThreadTimes(m_ThreadHandle,
		&start_time,&exit_time,
		&m_sStart,&m_uStart);
	ftime(&m_RealTime);
#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL,"%d,%d\n",
		m_uStart.dwLowDateTime,
		m_uStart.dwHighDateTime);
#endif
}

void b3TimeSpan::b3Stop()
{
	FILETIME      start_time,exit_time;
	FILETIME      system_usage,user_usage;
	struct timeb  real_stop;

	GetThreadTimes(m_ThreadHandle,
		&start_time,&exit_time,
		&system_usage,&user_usage);
	ftime(&real_stop);
#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL,"%d,%d\n",
		user_usage.dwLowDateTime,
		user_usage.dwHighDateTime);
#endif

/*
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
*/
	m_uTime += b3Diff(&m_uStart,&user_usage)   / 10000;
	m_sTime += b3Diff(&m_sStart,&system_usage) / 10000;
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

b3_u32 b3TimeSpan::b3Diff(FILETIME *first,FILETIME *last)
{
	b3_s32 highDiff,lowDiff;

	lowDiff  = last->dwLowDateTime  - first->dwLowDateTime;
	highDiff = last->dwHighDateTime - first->dwHighDateTime;
	return lowDiff;
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
