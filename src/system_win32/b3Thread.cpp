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
**	Revision 1.13  2002/11/16 14:24:00  sm
**	- Added a CPU benchmark
**	- Removed system dependend #IF from raytracing
**
**	Revision 1.12  2002/08/25 13:32:20  sm
**	- Renamed members in b3Thread of Windows implementation.
**	
**	Revision 1.10  2002/08/24 13:07:34  sm
**	- Added error message for errornous thread starting.
**	
**	Revision 1.9  2002/08/02 14:52:13  sm
**	- Vertex/normal computation is now multithreaded, too.
**	- Minor changes on b3PrepareInfo class.
**	- Last changes to Windows port.
**	
**	Revision 1.8  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.7  2002/03/10 21:14:41  sm
**	- Fixed rotation shapes with custom subdivision for rotation.
**	
**	Revision 1.6  2001/11/07 15:55:09  sm
**	- Introducing b3TimeSpan to Windows to get computation time on
**	  Windows as well.
**	- Changed some include dependencies.
**	
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
**                        b3IPCMutex implementation                     **
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

static b3_count    threadCount;
static b3IPCMutex  threadMutex;

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
	threadMutex.b3Lock();
	if (!m_IsRunning)
	{
		m_Span.b3Start();
		m_IsRunning = true;
		threadCount++;
	}
	threadMutex.b3Unlock();
}

void b3Thread::b3Dec(b3_bool incl_delete)
{
	threadMutex.b3Lock();
	if (m_IsRunning)
	{
		threadCount--;
		m_IsRunning = false;
		m_Span.b3Stop();
	}
	if (incl_delete && (m_Thread != null))
	{
		delete m_Thread;
		m_Thread = null;
	}
	threadMutex.b3Unlock();

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

		threadMutex.b3Lock();
		delete m_Thread;
		m_Thread = null;
		threadMutex.b3Unlock();
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

/*************************************************************************
**                                                                      **
**                        b3CPU implementation                          **
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
