/*
**
**	$Filename:	b3Atomic.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Atomic counter class
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_ATOMIC_H
#define B3_SYSTEM_ATOMIC_H

#include "blz3/system/b3Thread.h"

/**
 * This class provides an atomic counter. The counter
 * is 64 bit wide.
 */
class b3AtomicCounter
{
	b3Mutex m_Mutex;
	b3_u64  m_Counter;

public:
	/**
	 * This constructor initializes the counter with the given value.
	 * The initial counter value is of type <code>int</code>.
	 *
	 * @param init The initial counter value.
	 */
	b3AtomicCounter(int init)
	{
		m_Counter = (b3_u64)init;
	}

	b3AtomicCounter(b3_u64 init = 0)
	{
		// The construction must not be thread safe!
		m_Counter = init;
	}

	/**
	 * This method increases the counter by one.
	 */
	inline void b3Inc()
	{
		m_Mutex.b3Lock();
		m_Counter++;
		m_Mutex.b3Unlock();
	}

	/**
	 * This method increases the counter by the given value.
	 *
	 * @param summand The value added to the counter.
	 */
	inline b3_u64 b3Add(b3_u64 summand)
	{
		m_Mutex.b3Lock();
		m_Counter += summand;
		m_Mutex.b3Unlock();
		return m_Counter;
	}

	/**
	 * This operator initializes the counter to the specified value.
	 *
	 * @param init The integer value to assign to this instance.
	 */
	inline void operator=(int init)
	{
		// The int32 to int64 may not be atomic!!!
		m_Mutex.b3Lock();
		m_Counter = init;
		m_Mutex.b3Unlock();
	}

	/**
	 * This operator initializes the counter to the specified value.
	 *
	 * @param init The 64 bit integer value to assign to this instance.
	 */
	inline void operator=(b3_u64 init)
	{
		// The assignment may not be atomic on 32 bit machines!!!
		m_Mutex.b3Lock();
		m_Counter = init;
		m_Mutex.b3Unlock();
	}

	/**
	 * This operator increases the counter by one.
	 */
	inline b3_u64 operator++(int dummy)
	{
		m_Mutex.b3Lock();
		m_Counter++;
		m_Mutex.b3Unlock();

		return m_Counter;
	}

	/**
	 * This method returns the actual counter.
	 *
	 * @return The actual counter.
	 */
	inline operator b3_u64()
	{
		b3_u64 result;

		// The assignment may not be atomic on 32 bit machines!!!
		m_Mutex.b3Lock();
		result = m_Counter;
		m_Mutex.b3Unlock();

		return result;
	}
};

#endif
