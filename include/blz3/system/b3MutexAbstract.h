/*
**
**	$Filename:	b3MutexAbstract.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Interfaces to critical section handling.
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_MUTEXABSTRACT_H
#define B3_SYSTEM_MUTEXABSTRACT_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Assert.h"

/**
 * This abstract class defines an interface for critical sections where
 * only one thread or process can use a resource.
 */
class b3MutexAbstract
{
public:
	/**
	 * This method locks a critical section for exclusive use.
	 */
	virtual bool     b3Lock() = 0;

	/**
	 * This method unlocks a critical section.
	 */
	virtual bool     b3Unlock() = 0;
};

/**
 * This class represents a critical section. The critical section persists
 * during the life cycle of this class instance. It automatically locks
 * a mutex on creation and unlocks this mutex on deletion.
 */
class B3_PLUGIN b3CriticalSection
{
	b3MutexAbstract & m_Mutex;

public:
	/**
	 * This constructor locks the specified mutex.
	 *
	 * @param mutex The mutex to use for automatical locking/unlocking.
	 */
	inline explicit b3CriticalSection(b3MutexAbstract & mutex) : m_Mutex(mutex)
	{
#ifdef _DEBUG
		bool    locked = m_Mutex.b3Lock();

		B3_ASSERT(locked);
#else
		m_Mutex.b3Lock();
#endif
	}

	/**
	 * This destructor unlocks the mutex given by the constructor mutex.
	 */
	inline virtual ~b3CriticalSection()
	{
#ifdef _DEBUG
		bool    unlocked = m_Mutex.b3Unlock();

		B3_ASSERT(unlocked);
#else
		m_Mutex.b3Unlock();
#endif
	}
};

/**
 * This class is an interface for a simple signalling event. One
 * thread waits for a signal from another thread to proceed with
 * something.
 */
class b3EventAbstract
{
public:
	/**
	 * This method signalizes this instance an event.
	 */
	virtual void b3Pulse() = 0;

	/**
	 * This method returns if another thread triggers
	 * this instance an event via the b3Pulse() method.
	 *
	 * @return True on success.
	 */
	virtual bool    b3Wait()  = 0;
};

#endif
