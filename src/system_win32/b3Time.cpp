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

#define not_VERBOSE

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
**	Revision 1.7  2002/08/11 11:03:41  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**
**	Revision 1.6  2002/08/10 14:36:32  sm
**	- Some shapes had cleared the vertex array whenever the
**	  b3AllocVertices() method were called. Without calling
**	  b3Recomute() the shapes disoccured.
**	- Some methods moved as static methods into the
**	  b3Mem class.
**	
**	Revision 1.5  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
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
#ifdef _VERBOSE
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
#ifdef _VERBOSE
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
	m_uTime += b3DiffDiv10000(&m_uStart,&user_usage);
	m_sTime += b3DiffDiv10000(&m_sStart,&system_usage);
	m_rTime += (
		 real_stop.time    * 1000 + 
		 real_stop.millitm        -
		m_RealTime.time    * 1000 -
		m_RealTime.millitm);
}

static inline void b3_64_div_16(b3_u32 &high,b3_u32 &low,b3_u16 divisor)
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

b3_u32 b3TimeSpan::b3DiffDiv10000(FILETIME *first,FILETIME *last)
{
	b3_u32 highDiff,lowDiff;

	// Make difference
	lowDiff  = last->dwLowDateTime  - first->dwLowDateTime;
	highDiff = last->dwHighDateTime - first->dwHighDateTime;

	// Check carry
	if (last->dwLowDateTime < first->dwLowDateTime)
	{
		highDiff--;
	}

	// Divide by 10,000
	b3_64_div_16(highDiff,lowDiff,10000);

	// highDiff is greater 0 when 49 days time span are measured.
	// I think debugging a session would not take at least 49 days...
	B3_ASSERT(highDiff == 0);
	return lowDiff;
}
