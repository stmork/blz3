/*
**
**	$Filename:	b3Time.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - time control
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

#include "b3SystemInclude.h"
#include "blz3/system/b3Time.h"

/*************************************************************************
**                                                                      **
**                        b3Time routines                               **
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
	m_ThreadHandle = GetCurrentThread();
}

void b3TimeSpan::b3Start()
{
	FILETIME start_time,exit_time;

	GetThreadTimes(m_ThreadHandle,
		&start_time,&exit_time,
		&m_sStart,&m_uStart);
	ftime(&m_RealTime);
#ifdef VERBOSE
	b3PrintF(B3LOG_NORMAL,"Thread time start: %9d,%06d\n",
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
#ifdef VERBOSE
	b3PrintF(B3LOG_NORMAL,"Thread time stop:  %9d,%06d\n",
		user_usage.dwLowDateTime,
		user_usage.dwHighDateTime);
#endif

	m_uTime += b3DiffDiv10000(&m_uStart,&user_usage);
	m_sTime += b3DiffDiv10000(&m_sStart,&system_usage);
	m_rTime += (
		 real_stop.time + (b3_f64)real_stop.millitm  / 1000.0 -
		m_RealTime.time - (b3_f64)m_RealTime.millitm / 1000.0);
}

inline void b3TimeSpan::b3Div_64_By_16(b3_u32 &high,b3_u32 &low,b3_u16 divisor)
{
#if 0
	// This one is nice and fast
	b3_u32   digits[4];
	b3_u32   result[4];
	b3_u32   diff = 0;
	b3_count i;

	digits[0] = (high & 0xffff0000) >> 16;
	digits[1] = (high & 0x0000ffff);
	digits[2] =  (low & 0xffff0000) >> 16;
	digits[3] =  (low & 0x0000ffff);

	for (i = 0;i < 4;i++)
	{
		diff      += digits[i];
		result[i]  =  diff / divisor;
		diff       = (diff - result[i] * divisor) << 16;
	}
	high = (result[0] << 16) + result[1];
	low  = (result[2] << 16) + result[3];
#else
	// This one is ugly and faster
	b3_u32   aux;
	b3_u32   diff = 0;

	diff  =  (high & 0xffff0000) >> 16;
	aux   =   diff / divisor;
	diff  = ((diff - aux * divisor) << 16) + (high & 0x0000ffff);
	high  =   diff / divisor;
	diff  = ((diff - high * divisor) << 16) + ((low & 0xffff0000) >> 16);
	high +=  (aux << 16);
	aux   =   diff / divisor;
	diff  = ((diff - aux * divisor) << 16) + (low & 0x0000ffff);
	low   =  (diff / divisor) + (aux << 16);
#endif
}

b3_f64 b3TimeSpan::b3DiffDiv10000(FILETIME *first,FILETIME *last)
{
	b3_f64 start,stop;

	// Make difference
	start = ldexp((double)first->dwHighDateTime,32) + first->dwLowDateTime;
	stop  = ldexp((double) last->dwHighDateTime,32) +  last->dwLowDateTime;

	return (stop - start) / 10000000; // Correct: 10 million
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

b3_f64 b3Time::b3Now()
{
	ftime(&m_TimePoint);
	return b3GetTime();
}

b3_f64 b3Time::b3GetTime()
{
	return m_TimePoint.time + (double)m_TimePoint.millitm / 1000.0;
}

b3Time &b3Time::operator=(b3Time &orig)
{
	m_TimePoint = orig.m_TimePoint;
	return *this;
}

