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

#pragma once

#ifndef B3_SYSTEM_TIME_H
#define B3_SYSTEM_TIME_H

#include "blz3/system/b3TimeAbstract.h"
#include <sys/resource.h>
#include <sys/timeb.h>

/**
 * This class implements the system dependent version of b3TimeSpanAbstract.
 */
class b3TimeSpan : public b3TimeSpanAbstract
{
	struct rusage m_UsageTime;
	struct timeb  m_RealTime;

public:
	void    b3Start();
	void    b3Stop();
};

/**
 * This class implements the system dependent version of b3TimeAbstract.
 */
class b3Time : public b3TimeAbstract
{
	struct timeval m_TimePoint;

public:
	/**
	 * This constructor initializes this instance with the actual time.
	 */
	b3Time();

	/**
	 * This copy constructor copies the time from another instance.
	 */
	b3Time(const b3Time & orig);
	b3_f64  b3Now() override;
	b3_f64  b3GetTime() const override;

	/**
	 * This assignment operator copies the time from another instance.
	 */
	b3Time & operator=(const b3Time & orig);

	/**
	 * This assignment operator copies the time from another instance.
	 */
	b3Time & operator=(b3Time & orig);
};

#endif
