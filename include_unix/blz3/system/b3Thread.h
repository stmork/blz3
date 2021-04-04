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
	static bool     m_CorrectRUsage;

public:
	/**
	 * This constructor initializes information abount the installed CPUs.
	 */
	b3CPU();

	/**
	 * This method returns the active amount of threads.
	 *
	 * @return The actually running number of threads.
	 */
	static b3_count b3GetNumThreads();

	/**
	 * This method returns true if the underlying operating system
	 * provides a correct getrusage() method. Linux has an error
	 * in that method until kernel version 2.6.9 with incorrect
	 * time measurement on thread handling.
	 *
	 * @return True if getrusage() is correct.
	 */
	inline static bool     b3HasCorrectRUsage()
	{
		return m_CorrectRUsage;
	}
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
	pthread_mutex_t mutex;

public:
	/**
	 * This constructor initializes the mutex.
	 */
	b3Mutex()
	{
		b3PThread::b3CheckResult(pthread_mutex_init(&mutex, NULL));
	}

	/**
	 * Ths destructor deinitializes the mutex.
	 */
	virtual ~b3Mutex()
	{
		b3PThread::b3CheckResult(pthread_mutex_destroy(&mutex));
	}

	inline bool     b3Lock() override
	{
		return b3PThread::b3CheckResult(pthread_mutex_lock(&mutex));
	}

	inline bool     b3Unlock() override
	{
		return b3PThread::b3CheckResult(pthread_mutex_unlock(&mutex));
	}
};

/**
 * This class defines a critical sections where
 * only one process can use a resource. This class uses
 * UNIX semantics for interprocess communications (IPC) between
 * two processes. Note that this class can be used
 * as global variables.
 */
class b3IPCMutex : public b3MutexAbstract
{
	pthread_mutex_t mutex;
public:
	b3IPCMutex();
	virtual ~b3IPCMutex();
	bool     b3Lock() override;
	bool     b3Unlock() override;
};

/**
 * This class implements a simple signalling event pipe.
 */
class b3Event : public b3EventAbstract
{
	pthread_cond_t  event;
	pthread_mutex_t mutex;

	volatile bool   pulse;

public:
	/**
	 * This constructor initializes this event handler.
	 */
	b3Event();

	/**
	 * This destructor deinitializes this event handler.
	 */
	virtual ~b3Event();

	void     b3Pulse() override;
	bool     b3Wait() override;
};

/**
 * This class implements threading.
 */
class b3Thread : public b3ThreadAbstract
{
	const char  * m_Name;
	b3TimeSpan    m_Span;
	int           m_Prio = 0;

	pthread_t              m_Thread    = 0;
	volatile bool          m_IsRunning = false;
	volatile b3_u32        m_Result    = 0;
	volatile b3ThreadProc  m_CallProc  = nullptr;
	volatile void     *    m_CallArg   = nullptr;

	static   b3IPCMutex    m_ThreadMutex;
	static   b3_count      m_ThreadCount;

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
	void     b3AddTimeSpan(b3TimeSpan * span) override;

private:
	static void * b3Trampoline(void * thread);
	void  b3Inc();
	void  b3Dec();

	friend class b3CPU;
};

#endif
