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

#include "stdafx.h"
#include "blz3/system/b3Thread.h"
#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2001/10/20 16:15:00  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**
**	Revision 1.4  2001/07/03 18:14:08  sm
**	- Now having running threads. The system lib
**	  needed the appropriate project options
**	
**	Revision 1.3  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**	
**	Revision 1.2  2001/07/02 19:28:25  sm
**	- Applying console application on Windows 32
**	- Added further Windows environment
**	
**	Revision 1.1  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3 IPC mutex                                  **
**                                                                      **
*************************************************************************/

// Method for entering a critical code section
bool b3IPCMutex::b3Lock()
{
	return ::WaitForSingleObject(m_hObject,INFINITE) != 0;
}

// Method for leaving a critical code section
bool b3IPCMutex::b3Unlock()
{
	return ::ReleaseMutex(m_hObject) != 0;
}

/*************************************************************************
**                                                                      **
**                        b3 thread mutex                               **
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
**                        b3 event                                    **
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
**                        b3 thread                                   **
**                                                                      **
*************************************************************************/

static b3_count    threadCount;
static b3IPCMutex  threadMutex;

b3Thread::b3Thread(const char *task_name)
{
	name       = task_name;
	thread     = null;
	is_running = false;
}

b3Thread::~b3Thread()
{
	b3Stop();
}

void b3Thread::b3Name(const char *task_name)
{
	name = task_name;
}

unsigned int b3Thread::b3Trampoline(void *ptr)
{
	b3Thread *threadClass;
	b3_u32    result;

	threadClass = (b3Thread *)ptr;

	threadMutex.b3Lock();
	threadCount++;
	threadClass->is_running = true;
	threadMutex.b3Unlock();
	
	result = threadClass->callProc(threadClass->callArg);

	threadMutex.b3Lock();
	threadClass->is_running = false;
	threadCount--;
	threadMutex.b3Unlock();

	return result;
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
	callProc = proc;
	callArg  = ptr;
	thread = ::AfxBeginThread(b3Trampoline,this,priority,0,CREATE_SUSPENDED);
	if (thread != null)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # starting thread %02lX with prio %ld (%s).\n",
			thread->m_nThreadID,prio_rate,
			name != null ? name : "no name");
		thread->m_bAutoDelete = false;
		thread->ResumeThread();
	}
	return thread != null;
}

void b3Thread::b3Wait()
{
	if (thread != null)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # waiting for thread %02lX to stop (%s).\n",
			thread->m_nThreadID,
			name != null ? name : "no name");
		::WaitForSingleObject (thread->m_hThread,INFINITE);

		threadMutex.b3Lock();
		delete thread;
		thread = null;
		threadMutex.b3Unlock();
	}
}

b3_bool b3Thread::b3Stop()
{
	b3_bool was_running;

	was_running = is_running;
	if (thread != null) 
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # terminating thread %02lX (%s).\n",
			thread->m_nThreadID,
			name != null ? name : "no name");
		::TerminateThread (thread->m_hThread,0);
		is_running = false;
		
		threadMutex.b3Lock();
		delete thread;
		thread = null;

		threadCount--;
		threadMutex.b3Unlock();
	}
	return was_running;
}

b3_bool b3Thread::b3IsRunning()
{
#if 0 
	b3_bool is_running;
	b3_s32  result;

	if (thread != null)
	{
		result = ::WaitForSingleObject (thread->m_hThread,0);
		is_running = true;
	}
	else
	{
		is_running = false;
	}
#endif
	return is_running;
}

/*************************************************************************
**                                                                      **
**                        b3 count CPUs                               **
**                                                                      **
*************************************************************************/

b3CPU::b3CPU()
{
	SYSTEM_INFO info;

	GetSystemInfo(&info);
	num = info.dwNumberOfProcessors;
	if (info.dwNumberOfProcessors < 1)
	{
		num = 1;
	}
	b3PrintF (B3LOG_DEBUG,"### CLASS: b3CPU  # Number of used CPUs: %ld\n",num);
}

b3_count b3CPU::b3GetNumThreads()
{
	b3_count resuming;

	threadMutex.b3Lock();
	if (num > threadCount)
	{
		resuming = num - threadCount;
	}
	else
	{
		resuming = 1;
	}
	threadMutex.b3Unlock();
	return resuming;
}

b3_count b3CPU::b3GetNumCPUs()
{
	return num;
}
