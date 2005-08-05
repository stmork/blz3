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

#ifndef B3_SYSTEM_MUTEXABSTRACT_H
#define B3_SYSTEM_MUTEXABSTRACT_H

#include "blz3/b3Types.h"

/**
 * This abstract class defines an interface for critical sections where
 * only one thread or process can use a resource.
 */
class B3_PLUGIN b3MutexAbstract
{
public:
	/**
	 * This method locks a critical section for exclusive use.
	 */
	virtual b3_bool  b3Lock() = 0;

	/**
	 * This method unlocks a critical section.
	 */
	virtual b3_bool  b3Unlock() = 0;
};

/**
 * This class is an interface for a simple signalling event. One
 * thread waits for a signal from another thread to proceed with
 * something.
 */
class B3_PLUGIN b3EventAbstract
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
	virtual b3_bool b3Wait()  = 0;
};

#endif
