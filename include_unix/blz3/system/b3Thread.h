/*
**
**	$Filename:	b3Thread.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Multithreading control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_THREAD_H
#define B3_SYSTEM_THREAD_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <string>
#include "blz3/system/b3MutexAbstract.h"
#include "blz3/system/b3ThreadAbstract.h"
#include "blz3/system/b3CPUBase.h"
#include <pthread.h>
#include <string.h>

/**
 * This class provides information about the installed CPUs.
 */
class b3CPU : public b3CPUBase
{
public:
	/**
	 * This constructor initializes information abount the installed CPUs.
	 */
	b3CPU() = default;
};

class b3PThread
{
public:
	inline static bool    b3CheckResult(int error_code)
	{
#ifdef _DEBUG
		if (error_code != 0)
		{
			b3PrintF(B3LOG_NORMAL, "### CLASS: b3PLog # errno: %d (%s)!\n",
				error_code, strerror(error_code));
		}
#endif
		return error_code == 0;
	}
};

/**
 * This class defines a critical sections where
 * only one thread can use a resource. This class uses
 * POSIX threads semantics for multi threading inside
 * one process. Note that this class cannot be used
 * as global variables.
 */
class b3Mutex : public b3MutexAbstract
{
	std::mutex m_Mutex;

public:
	/**
	 * This constructor initializes the mutex.
	 */
	b3Mutex() = default;

	/**
	 * Ths destructor deinitializes the mutex.
	 */
	virtual ~b3Mutex() = default;

	inline bool b3Lock() override
	{
		m_Mutex.lock();

		return true;
	}

	inline bool b3Unlock() override
	{
		m_Mutex.unlock();

		return true;
	}
};

/**
 * This class defines a critical sections where
 * only one process can use a resource. This class uses
 * UNIX semantics for interprocess communications (IPC) between
 * two processes. Note that this class can be used
 * as global variables.
 */
typedef b3Mutex b3IPCMutex;

/**
 * This class implements a simple signalling event pipe.
 */
class b3Event : public b3EventAbstract
{
	std::condition_variable m_Conditional;
	std::mutex              m_Mutex;
	bool                    m_Pulsed = false;

public:
	/**
	 * This constructor initializes this event handler.
	 */
	b3Event() = default;

	/**
	 * This destructor deinitializes this event handler.
	 */
	virtual ~b3Event() = default;

	void     b3Pulse() override;
	bool     b3Wait() override;
};

/**
 * This class implements threading.
 */
class b3Thread : public b3ThreadAbstract
{
	std::string                  m_Name;
	b3TimeSpan                   m_Span;
	int                          m_Prio = 0;

	pthread_t                    m_Thread    = 0;
	volatile bool                m_IsRunning = false;
	volatile b3_u32              m_Result    = 0;
	volatile b3ThreadProc        m_CallProc  = nullptr;
	volatile void     *          m_CallArg   = nullptr;

	static   std::atomic_size_t  m_ThreadCount;
	static   std::mutex          m_ThreadMutex;

public:
	/**
	 * This constructor initializes this instance and can name the thread.
	 *
	 * @param taskname The new thread name.
	 */
	explicit b3Thread(const char * taskname = nullptr);

	/**
	 * This destructor terminates a running thread and deinitializes this instance.
	 */
	virtual ~b3Thread();

	void     b3Name(const char * taskname = nullptr) override;
	bool     b3Start(b3ThreadProc thread, void * ptr, b3_s32 priority = 0) override;
	bool     b3IsRunning() override;
	bool     b3Stop() override;
	b3_u32   b3Wait() override;

private:
	static void * b3Trampoline(void * thread);
	void  b3Inc();
	void  b3Dec();

	friend class b3CPU;
};

#endif
