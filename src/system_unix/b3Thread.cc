/*
**
**	$Filename:	b3Thread.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III multi threading support
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Thread.h"
#include "blz3/system/b3Log.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III IPC mutex                        **
**                                                                      **
*************************************************************************/

b3IPCMutex::b3IPCMutex()
{
	b3PThread::b3CheckResult(pthread_mutex_init(&mutex,NULL));
}

b3IPCMutex::~b3IPCMutex()
{
	b3PThread::b3CheckResult(pthread_mutex_destroy(&mutex));
}

// Method for entering a critical code section
b3_bool b3IPCMutex::b3Lock()
{
	return b3PThread::b3CheckResult(pthread_mutex_lock(&mutex));
}

// Method for leaving a critical code section
b3_bool b3IPCMutex::b3Unlock()
{
	return b3PThread::b3CheckResult(pthread_mutex_unlock(&mutex));
}

/*************************************************************************
**                                                                      **
**                        b3 event                                      **
**                                                                      **
*************************************************************************/

b3Event::b3Event()
{
	b3PThread::b3CheckResult(pthread_cond_init(&event,NULL));
	b3PThread::b3CheckResult(pthread_mutex_init(&mutex,NULL));
	pulse = false;
}

b3Event::~b3Event()
{
	b3PThread::b3CheckResult(pthread_mutex_destroy(&mutex));
	b3PThread::b3CheckResult(pthread_cond_destroy(&event));
}

// Signal another thread that there has happend something.
void b3Event::b3Pulse()
{
	b3PThread::b3CheckResult(pthread_mutex_lock(&mutex));
	pulse = true;
	b3PThread::b3CheckResult(pthread_cond_broadcast(&event));
	b3PThread::b3CheckResult(pthread_mutex_unlock(&mutex));
}

// Wait for a signal from another thread.
b3_bool b3Event::b3Wait()
{
	b3_bool success = true;

	success &= b3PThread::b3CheckResult(pthread_mutex_lock(&mutex));
	if (!pulse)
	{
		success &= b3PThread::b3CheckResult(pthread_cond_wait(&event,&mutex));
	}
	pulse = false;
	success &= b3PThread::b3CheckResult(pthread_mutex_unlock(&mutex));

	return success;
}

/*************************************************************************
**                                                                      **
**                        b3 thread                                     **
**                                                                      **
*************************************************************************/

b3_count   b3Thread::m_ThreadCount;
b3IPCMutex b3Thread::m_ThreadMutex;

static b3_count   threadSuccess;
static b3_count   threadError;

b3Thread::b3Thread(const char *task_name)
{
	m_Name      = task_name;
	m_IsRunning = false;
	m_Result    = 0;
	m_Thread    = 0;
}

b3Thread::~b3Thread()
{
	b3Stop();
}

void b3Thread::b3Inc()
{
	b3CriticalSection lock(m_ThreadMutex);

	if (!m_IsRunning)
	{
		m_Span.b3Start();
		m_IsRunning = true;
		m_ThreadCount++;
	}
}

void b3Thread::b3Dec()
{
	b3CriticalSection lock(m_ThreadMutex);

	if (m_IsRunning)
	{
		m_ThreadCount--;
		m_IsRunning = false;
		m_Span.b3Stop();
	}
}

void b3Thread::b3Name(const char *task_name)
{
	m_Name = task_name;
}

b3_bool b3Thread::b3Start(
	b3ThreadProc  proc,
	void         *ptr,
	b3_s32        priority)
{
	b3_bool success;
	int     error_code;

	b3Stop();
	assert(!m_IsRunning);

	m_CallProc = proc;
	m_CallArg  = ptr;
	m_Result   = 0;
	m_Thread   = 0;
	m_Prio     = -priority * 5;

	success = b3PThread::b3CheckResult(error_code = pthread_create(&m_Thread,NULL,&b3Trampoline,this));
	if (success)
	{
		threadSuccess++;
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # started thread %02lX (%s).\n",
				  m_Thread,
				  m_Name != null ? m_Name : "no name");
	}
	else
	{
		b3CriticalSection lock(m_ThreadMutex);

		threadError++;
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Thrd # Thread (%x) not started!\n",
				 m_Thread);
		b3PrintF(B3LOG_NORMAL,"    OK/error count: %d/%d\n",
				 threadSuccess,threadError);
		b3PrintF(B3LOG_NORMAL,"    thread count:   %d\n",
				 m_ThreadCount);
	}
	return success;
}

void * b3Thread::b3Trampoline(void *ptr)
{
	b3Thread *threadClass = (b3Thread *)ptr;

	if (nice(threadClass->m_Prio) == -1)
	{
		b3PrintF(B3LOG_NORMAL,"   Nicing error!\n");
	}
	threadClass->b3Inc();
	threadClass->m_Result = threadClass->m_CallProc((void *)threadClass->m_CallArg);
	threadClass->b3Dec();

	return null;
}

b3_bool b3Thread::b3IsRunning()
{
	return m_IsRunning;
}

b3_bool b3Thread::b3Stop()
{
	b3_bool was_running;

	if (b3IsRunning())
	{
		pthread_cancel(m_Thread);
	}
	was_running = m_IsRunning;
	if (m_IsRunning)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # terminated thread %02lX (%s).\n",
				  m_Thread,
				  m_Name != null ? m_Name : "no name");
	}

	b3Dec();
	m_CallProc  = null;
	m_CallArg   = 0;
	m_Thread    = 0;

	return was_running;
}

b3_u32 b3Thread::b3Wait()
{
	int   result;
	void *ptr = &result;

	b3PThread::b3CheckResult(pthread_join(m_Thread,&ptr));
	return m_Result;
}

void b3Thread::b3AddTimeSpan(b3TimeSpan *span)
{
#ifdef __linux__
	if (!b3CPU::b3HasCorrectRUsage())
	{
		span->m_uTime += m_Span.m_uTime;
		span->m_sTime += m_Span.m_sTime;
	}
#endif
}
