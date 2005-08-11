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

#ifndef B3_SYSTEM_THREAD_H
#define B3_SYSTEM_THREAD_H

#include "blz3/system/b3MutexAbstract.h"
#include "blz3/system/b3ThreadAbstract.h"
#include "blz3/system/b3CPUBase.h"
#include <pthread.h>

/**
 * This class provides information about the installed CPUs.
 */
class b3CPU : public b3CPUBase
{
	static b3_bool  m_CorrectRUsage;

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
	inline static b3_bool  b3HasCorrectRUsage()
	{
		return m_CorrectRUsage;
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
	         b3Mutex();

	/**
	 * Ths destructor deinitializes the mutex.
	 */
	        ~b3Mutex();
	b3_bool  b3Lock();
	b3_bool  b3Unlock();
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
	        ~b3IPCMutex();
	b3_bool  b3Lock();
	b3_bool  b3Unlock();
};

/**
 * This class implements a simple signalling event pipe..
 */
class b3Event : public b3EventAbstract
{
	         pthread_cond_t  event;
	         pthread_mutex_t mutex;
	volatile b3_bool         pulse;

public:
	/**
	 * This constructor initializes this event handler.
	 */
	         b3Event();

	/**
	 * This destructor deinitializes this event handler.
	 */
	        ~b3Event();

	void     b3Pulse();
	b3_bool  b3Wait();
};

/**
 * This class implements threading.
 */
class b3Thread : public b3ThreadAbstract
{
	         const char   *m_Name;
	         b3TimeSpan    m_Span;
	         int           m_Prio;

	         pthread_t     m_Thread;
	volatile b3_bool       m_IsRunning;
	volatile b3_u32        m_Result;
	volatile b3ThreadProc  m_CallProc;
	volatile void         *m_CallArg;

	static   b3IPCMutex    m_ThreadMutex;
	static   b3_count      m_ThreadCount;

public:
	/**
	 * This constructor initializes this instance and can name the thread.
	 *
	 * @param taskname The new thread name.
	 */
	         b3Thread(const char *taskname = null);

	/**
	 * This destructor terminates a running thread and deinitializes this instance.
	 */
	        ~b3Thread();

	void     b3Name(const char *taskname = null);
	b3_bool  b3Start(b3ThreadProc thread, void *ptr, b3_s32 priority=0);
	b3_bool  b3IsRunning();
	b3_bool  b3Stop();
	b3_u32   b3Wait();
	void     b3AddTimeSpan(b3TimeSpan *span);

private:
	static void *b3Trampoline(void *thread);
	       void  b3Inc();
	       void  b3Dec();

	friend class b3CPU;
};

#endif
