/*
**
**	$Filename:	b3Time.cpp $
**	$Release:	Dortmund 1999 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Maui 2000 - time control
**
**	(C) Copyright 1999-2000  M-DMS GmbH
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Maui 2000 includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Time.h"
#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Maui development log                          **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

b3Time::b3Time()
{
	count = 0;
	pos   = 0;
	size  = 0;
}

void b3Time::b3Init(b3_u32 slice)
{
	if (slice >= MAX_B3TIME_SLICE)
	{
		slice = MAX_B3TIME_SLICE - 1;
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

void b3Time::b3Get(
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
