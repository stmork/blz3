/*
**
**	$Filename:	b3ThreadAbstract.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Threading control
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_THREADABSTRACT_H
#define B3_SYSTEM_THREADABSTRACT_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Time.h"

typedef bool(*b3ThreadProc)(void *);

/**
 * This abstract class defines basic thread handling operations.
 */
class b3ThreadAbstract
{
public:
	/**
	 * This method names this thread instance.
	 *
	 * @param taskname The thread name.
	 */
	virtual void     b3Name(const char * taskname = nullptr) = 0;

	/**
	 * This method starts a new thread. There can be 5 different thread
	 * priorities in range [-2..2].
	 *
	 * @param thread The thread procedure.
	 * @param ptr A pointer for passing custom information to the thread.
	 * @param priority The threads priority.
	 * @return True if there wa no thread running and the new thread started successfully.
	 */
	virtual bool     b3Start(b3ThreadProc thread, void * ptr, b3_s32 priority = 0) = 0;

	/**
	 * This method returns true if the thread is in running state.
	 *
	 * @return True if the thread is currently running.
	 */
	virtual bool     b3IsRunning() = 0;

	/**
	 * This method stops the actually running thread.
	 *
	 * @return True if the thread was running.
	 */
	virtual bool     b3Stop() = 0;

	/**
	 * This method waits until the thread terminates.
	 *
	 * @return The return value of the terminated thread procedure.
	 */
	virtual b3_u32   b3Wait() = 0;

	/**
	 * This method behaves different on each platform to compensate
	 * different time measurements methods.
	 *
	 * @param span The time span to add the cpu time used from the thread.
	 */
	virtual void     b3AddTimeSpan(b3TimeSpan * span) = 0;
};

#endif
