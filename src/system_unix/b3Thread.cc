/*
**
**	$Filename:	b3Thread.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
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
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.19  2004/11/26 16:28:17  smork
**	- Equalizing VERBOSE print outs.
**	- pthread error messages only in _DEBUG version.
**
**	Revision 1.18  2004/08/18 07:32:00  sm
**	- Fixed b3Thread::b3Stop() bug on gcc 2.95 systems.
**	
**	Revision 1.17  2003/10/16 08:25:55  sm
**	- Moved CPU bit count into b3CPUBase class
**	
**	Revision 1.16  2003/08/11 08:21:40  sm
**	- Added priority scheduling to b3Thread class.
**	- Cleaned up brt3 comments.
**	
**	Revision 1.15  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
**	Revision 1.14  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.13  2002/11/16 14:24:00  sm
**	- Added a CPU benchmark
**	- Removed system dependend #IF from raytracing
**	
**	Revision 1.12  2002/09/16 16:49:59  sm
**	- Done some setup cleanups
**	- Using _SC_NPROC_ONLN instead of _SC_NPROCESSORS_ONLN if
**	  available.
**	
**	Revision 1.11  2002/09/15 09:08:30  sm
**	- Some adjustments to compile on SGI Mips R10000:-) Which seems
**	  to be the most efficient CPU on world...
**	
**	Revision 1.10  2002/08/24 13:59:54  sm
**	- Corrected timing inside threads
**	
**	Revision 1.9  2002/08/24 13:22:02  sm
**	- Extensive debugging on threading code done!
**	  o Cleaned up POSIX threads
**	  o Made safe thread handling available in raytracing code
**	  o b3PrepareInfo instantiates threads only once.
**	- Added new thread options to gcc: "-D_REENTRAND -pthread"
**	  which I only can assume what they are doing;-)
**	- Time window in motion blur moved from [-0.5,0.5] to [0,1]
**	  and corrected upper time limit.
**	
**	Revision 1.8  2002/08/23 13:40:28  sm
**	- b3Event on Un*x platforms were broken.
**	
**	Revision 1.7  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.6  2001/11/07 15:55:09  sm
**	- Introducing b3TimeSpan to Windows to get computation time on
**	  Windows as well.
**	- Changed some include dependencies.
**	
**	Revision 1.5  2001/11/06 17:14:02  sm
**	- Introducing JPEG saving
**	- Made some library fine tunings on TIFF and JPEG
**	- Conversion tool any image in/ JPEG or TIFF image out. Includes:
**	  o scaling
**	  o filtering
**	  o B/W conversion
**	
**	Revision 1.4  2001/10/20 16:15:00  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
**	Revision 1.3  2001/07/08 12:30:07  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        pthread logging                               **
**                                                                      **
*************************************************************************/

#ifdef _DEBUG
static b3_bool b3LogPThread(int error_code)
{
	if(error_code != 0)
	{
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3PLog # errno: %d (%s)!\n",
			error_code,strerror(error_code));
	}
	return error_code == 0;
}
#else
#define b3LogPThread(error_code) (b3_bool)((error_code) == 0)
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III mutex                            **
**                                                                      **
*************************************************************************/

b3Mutex::b3Mutex()
{
	b3LogPThread(pthread_mutex_init(&mutex,NULL));
}

b3Mutex::~b3Mutex()
{
	b3LogPThread(pthread_mutex_destroy(&mutex));
}

// Method for entering a critical code section
b3_bool b3Mutex::b3Lock()
{
	return b3LogPThread(pthread_mutex_lock(&mutex));
}

// Method for leaving a critical code section
b3_bool b3Mutex::b3Unlock()
{
	return b3LogPThread(pthread_mutex_unlock(&mutex));
}

/*************************************************************************
**                                                                      **
**                        Blizzard III IPC mutex                        **
**                                                                      **
*************************************************************************/


b3IPCMutex::b3IPCMutex()
{
	b3LogPThread(pthread_mutex_init(&mutex,NULL));
}

b3IPCMutex::~b3IPCMutex()
{
	b3LogPThread(pthread_mutex_destroy(&mutex));
}

// Method for entering a critical code section
b3_bool b3IPCMutex::b3Lock()
{
	return b3LogPThread(pthread_mutex_lock(&mutex));
}

// Method for leaving a critical code section
b3_bool b3IPCMutex::b3Unlock()
{
	return b3LogPThread(pthread_mutex_unlock(&mutex));
}

/*************************************************************************
**                                                                      **
**                        b3 event                                      **
**                                                                      **
*************************************************************************/

b3Event::b3Event()
{
	b3LogPThread(pthread_cond_init(&event,NULL));
	b3LogPThread(pthread_mutex_init(&mutex,NULL));
	pulse = false;
}

b3Event::~b3Event()
{
	b3LogPThread(pthread_mutex_destroy(&mutex));
	b3LogPThread(pthread_cond_destroy(&event));
}

// Signal another thread that there has happend something.
void b3Event::b3Pulse()
{
	b3LogPThread(pthread_mutex_lock(&mutex));
	pulse = true;
	b3LogPThread(pthread_cond_broadcast(&event));
	b3LogPThread(pthread_mutex_unlock(&mutex));
}

// Wait for a signal from another thread.
b3_bool b3Event::b3Wait()
{
	b3_bool success = true;

	success &= b3LogPThread(pthread_mutex_lock(&mutex));
	if (!pulse)
	{
		success &= b3LogPThread(pthread_cond_wait(&event,&mutex));
	}
	pulse = false;
	success &= b3LogPThread(pthread_mutex_unlock(&mutex));

	return success;
}

/*************************************************************************
**                                                                      **
**                        b3 thread                                     **
**                                                                      **
*************************************************************************/

static b3_count   threadCount;
static b3IPCMutex threadMutex;
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
	threadMutex.b3Lock();
	if (!m_IsRunning)
	{
		m_Span.b3Start();
		m_IsRunning = true;
		threadCount++;
	}
	threadMutex.b3Unlock();
}

void b3Thread::b3Dec()
{
	threadMutex.b3Lock();
	if (m_IsRunning)
	{
		threadCount--;
		m_IsRunning = false;
		m_Span.b3Stop();
	}
	threadMutex.b3Unlock();

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

	success = b3LogPThread(error_code = pthread_create(&m_Thread,NULL,&b3Trampoline,this));
	if (success)
	{
		threadSuccess++;
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # started thread %02lX (%s).\n",
			m_Thread,
			m_Name != null ? m_Name : "no name");
	}
	else
	{
		threadMutex.b3Lock();
		threadError++;
		b3PrintF(B3LOG_NORMAL,"### CLASS: b3Thrd # Thread (%x) not started!\n",
			m_Thread);
		b3PrintF(B3LOG_NORMAL,"    OK/error count: %d/%d\n",
			threadSuccess,threadError);
		b3PrintF(B3LOG_NORMAL,"    thread count:   %d\n",
			threadCount);
		threadMutex.b3Unlock();
	}
	return success;
}

void * b3Thread::b3Trampoline(void *ptr)
{
	b3Thread *threadClass = (b3Thread *)ptr;

	nice(threadClass->m_Prio);
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

	b3LogPThread(pthread_join(m_Thread,&ptr));
	return m_Result;
}

void b3Thread::b3AddTimeSpan(b3TimeSpan *span)
{
#ifdef __linux__
	span->m_uTime += m_Span.m_uTime;
	span->m_sTime += m_Span.m_sTime;
#endif
} 

/*************************************************************************
**                                                                      **
**                        b3 count CPUs                                 **
**                                                                      **
*************************************************************************/

#ifndef _SC_NPROCESSORS_ONLN
#ifdef  _SC_NPROC_ONLN
#define _SC_NPROCESSORS_ONLN _SC_NPROC_ONLN
#endif
#endif

b3CPU::b3CPU()
{
	if (cpu_count == 0)
	{
		long    result;

#ifdef _SC_NPROCESSORS_ONLN
		result = sysconf(_SC_NPROCESSORS_ONLN);
#else
		result = 1;
#endif
		if ((result < 1) || (errno == EINVAL))
		{
			cpu_count = 1;
		}
		else
		{
			cpu_count = result;
		}
	}
}

b3_count b3CPU::b3GetNumThreads()
{
	b3_count resuming;

	threadMutex.b3Lock();
	if (cpu_count > threadCount)
	{
		resuming = cpu_count - threadCount;
	}
	else
	{
		resuming = 1;
	}
	threadMutex.b3Unlock();
	return resuming;
}
