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

#ifndef B3_SYSTEM_ATOMIC_H
#define B3_SYSTEM_ATOMIC_H

#include "blz3/system/b3Thread.h"

class b3AtomicCounter : protected b3Mutex
{
	b3_u64 m_Counter;

public:
	b3AtomicCounter(int init)
	{
		m_Counter = (b3_u64)init;
	}

	b3AtomicCounter(b3_u64 init = 0)
	{
		m_Counter = init;
	}

	inline void b3Inc()
	{
		b3Lock();
		m_Counter++;
		b3Unlock();
	}

	inline b3_u64 b3Add(b3_u64 summand)
	{
		b3Lock();
		m_Counter += summand;
		b3Unlock();
		return m_Counter;
	}

	inline b3_u64 operator++(int dummy)
	{
		b3Lock();
		m_Counter++;
		b3Unlock();
		
		return m_Counter;
	}

	inline operator b3_u64()
	{
		return m_Counter;
	}
};

#endif
