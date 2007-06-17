/*
**
**	$Filename:	b3Thread.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Multithreading routines (for generating dead locks)
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

#include "b3SystemInclude.h"
#include "blz3/system/b3Thread.h"

/*************************************************************************
**                                                                      **
**                        b3IPCMutex implementation                     **
**                                                                      **
*************************************************************************/

// Method for entering a critical code section
b3_bool b3IPCMutex::b3Lock()
{
	return ::WaitForSingleObject(m_hObject,INFINITE) != 0;
}

// Method for leaving a critical code section
b3_bool b3IPCMutex::b3Unlock()
{
	return ::ReleaseMutex(m_hObject) != 0;
}

/*************************************************************************
**                                                                      **
**                        b3Mutex implementation                        **
**                                                                      **
*************************************************************************/

// Initialize critical section
b3Mutex::b3Mutex()
{
	::InitializeCriticalSection(&cs);
}

// Deinitialize critical section
b3Mutex::~b3Mutex()
{
	::DeleteCriticalSection(&cs);
}

// Method for entering a critical code section
b3_bool b3Mutex::b3Lock()
{
	::EnterCriticalSection(&cs);
	return true;
}

// Method for leaving a critical code section
b3_bool b3Mutex::b3Unlock()
{
	::LeaveCriticalSection(&cs);
	return true;
}

/*************************************************************************
**                                                                      **
**                        b3Event implementation                        **
**                                                                      **
*************************************************************************/

// Signal another thread that there has happend something.
void b3Event::b3Pulse()
{
	SetEvent();
}

// Wait for a signal from another thread.
b3_bool b3Event::b3Wait()
{
	b3_bool success;

	success = ::WaitForSingleObject(m_hObject,INFINITE) != 0;
	ResetEvent();
	return success;
}

/*************************************************************************
**                                                                      **
**                        b3Thread implementation                       **
**                                                                      **
*************************************************************************/

b3_count    b3Thread::m_ThreadCount;
b3IPCMutex  b3Thread::m_ThreadMutex;

b3Thread::b3Thread(const char *task_name)
{
	m_Name      = task_name;
	m_Thread    = null;
	m_IsRunning = false;
}

b3Thread::~b3Thread()
{
	b3Stop();
}

void b3Thread::b3Inc()
{
	m_ThreadMutex.b3Lock();
	if (!m_IsRunning)
	{
		m_Span.b3Start();
		m_IsRunning = true;
		m_ThreadCount++;
	}
	m_ThreadMutex.b3Unlock();
}

void b3Thread::b3Dec(b3_bool incl_delete)
{
	m_ThreadMutex.b3Lock();
	if (m_IsRunning)
	{
		m_ThreadCount--;
		m_IsRunning = false;
		m_Span.b3Stop();
	}
	if (incl_delete && (m_Thread != null))
	{
		delete m_Thread;
		m_Thread = null;
	}
	m_ThreadMutex.b3Unlock();

}

void b3Thread::b3Name(const char *task_name)
{
	m_Name = task_name;
}

b3_u32 b3Thread::b3Trampoline(void *ptr)
{
	b3Thread *threadClass = (b3Thread *)ptr;

	threadClass->b3Inc();
	threadClass->m_Result = threadClass->m_CallProc(threadClass->m_CallArg);
	threadClass->b3Dec();

	return threadClass->m_Result;
}

b3_bool b3Thread::b3Start(
	b3ThreadProc    proc,
	void           *ptr,
	b3_s32          prio_rate)
{
	long priority = THREAD_PRIORITY_NORMAL;

	// Stop a previous running thread...
	b3Stop();

	// Recompute priority
	switch (prio_rate)
	{
	case -1 :
		priority = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case  0:
		priority = THREAD_PRIORITY_NORMAL;
		break;
	case  1:
		priority = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	default :
		if (prio_rate < -1)
		{
			priority = THREAD_PRIORITY_LOWEST;
		}
		if (prio_rate >  1)
		{
			priority = THREAD_PRIORITY_HIGHEST;
		}
		break;
	}

	// Start new thread
	m_CallProc = proc;
	m_CallArg  = ptr;
	m_Thread = ::AfxBeginThread(b3Trampoline,this,priority,0,CREATE_SUSPENDED);
	if (m_Thread != null)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # starting thread %02lX with prio %ld (%s).\n",
			m_Thread->m_nThreadID,prio_rate,
			m_Name != null ? m_Name : "no name");
		m_Thread->m_bAutoDelete = false;
		m_Thread->ResumeThread();
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Thrd # Thread not started!\n",
			m_Thread);
	}
	return m_Thread != null;
}

b3_u32 b3Thread::b3Wait()
{
	if (m_Thread != null)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # waiting for thread %02lX to stop (%s).\n",
			m_Thread->m_nThreadID,
			m_Name != null ? m_Name : "no name");
		::WaitForSingleObject (m_Thread->m_hThread,INFINITE);

		m_ThreadMutex.b3Lock();
		delete m_Thread;
		m_Thread = null;
		m_ThreadMutex.b3Unlock();
	}
	else
	{
		m_Result = null;
	}
	return m_Result;
}

b3_bool b3Thread::b3Stop()
{
	b3_bool was_running;

	was_running = m_IsRunning;
	if (m_Thread != null) 
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # terminating thread %02lX (%s).\n",
			m_Thread->m_nThreadID,
			m_Name != null ? m_Name : "no name");
		::TerminateThread (m_Thread->m_hThread,0);
		b3Dec(true);
		m_CallProc  = null;
		m_CallArg   = 0;
	}
	return was_running;
}

b3_bool b3Thread::b3IsRunning()
{
	return m_IsRunning;
}

void b3Thread::b3AddTimeSpan(b3TimeSpan *span)
{
#ifdef WIN32
	span->m_uTime += m_Span.m_uTime;
	span->m_sTime += m_Span.m_sTime;
#endif
} 
