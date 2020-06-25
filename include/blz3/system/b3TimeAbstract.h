/*
**
**	$Filename:	b3TimeAbstract.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - time control
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_TIMEABSTRACT_H
#define B3_SYSTEM_TIMEABSTRACT_H

#include "blz3/system/b3Log.h"

#define B3_MAX_TIME_SLICE 100

/**
 * This class is used to compute average time values.
 */
class B3_PLUGIN b3TimeAccumAbstract
{
public:
	/**
	 * This method initializes this instance with the given slices.
	 */
	virtual void  b3Init(unsigned long slice=10) = 0;

	/**
	 * This call is a measurement mark. The returned time is in milliseconds and must
	 * be divided by the given reference count to get an average run time between two
	 * measurement marks.
	 *
	 * @param refSpan The runtime.
	 * @param refCount The measurement mark span count.
	 */
	virtual void  b3Get(unsigned long &refSpan,unsigned long &refCount) = 0;
};

/**
 * This class allows time stopping and time measurements. The values have the
 * accuracy in micro seconds.
 */
class B3_PLUGIN b3TimeSpanAbstract
{
public:
	b3_f64        m_uTime; //!< The accumulated user time the CPU used in this thread.
	b3_f64        m_sTime; //!< The accumulated system time the CPU used in this thread.
	b3_f64        m_rTime; //!< The accumulated runtime the thread was active including blocked time.

public:
	/**
	 * This constructor resets time measurement.
	 */
	b3TimeSpanAbstract();

	/**
	 * This method starts time resource measurement.
	 */
	virtual void    b3Start() = 0;

	/**
	 * This method stops time resource measurement.
	 */
	virtual void    b3Stop() = 0;

	/**
	 * This method resets time measurement.
	 */
	void    b3Clear();

	/**
	 * This method returns the load factor computed from the three measured values.
	 *
	 * @return The load factor.
	 */
	b3_f64  b3GetUsage();

	/**
	 * This method logs the actual time measuring state.
	 *
	 * @param loglevel The log level to log with.
	 */
	void    b3Print(b3_log_level loglevel = B3LOG_NORMAL);

private:
	static char    *b3PrintTime(char *buffer,b3_size size,b3_f64 time_needed);
};

/**
 * This class handles the actual time. The time has micro second accuracy.
 */
class B3_PLUGIN b3TimeAbstract
{
public:
	/**
	 * This method initializes this instance to the actual time point.
	 */
	virtual b3_f64  b3Now() = 0;

	/**
	 * This cast operator returns a time point in seconds since 1. January 1970 in seconds. The
	 * value has mirco second accuracy.
	 *
	 * @return The seconds since 1. January 1970.
	 */
	virtual b3_f64  b3GetTime() = 0;

	/**
	 * This cast operator returns a time point in seconds since 1. January 1970 in seconds. The
	 * value has mirco second accuracy.
	 *
	 * @return The seconds since 1. January 1970.
	 */
	operator b3_f64()
	{
		return b3GetTime();
	}
};

#endif
