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

#include <unistd.h>
#include <errno.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**                        Blizzard III mutex                            **
**                                                                      **
*************************************************************************/

b3Mutex::b3Mutex()
{
	pthread_mutex_init(&mutex,NULL);
}

b3Mutex::~b3Mutex()
{
	pthread_mutex_destroy(&mutex);
}

// Method for entering a critical code section
b3_bool b3Mutex::b3Lock()
{
	pthread_mutex_lock(&mutex);
	return true;
}

// Method for leaving a critical code section
b3_bool b3Mutex::b3Unlock()
{
	pthread_mutex_unlock(&mutex);
	return true;
}

/*************************************************************************
**                                                                      **
**                        Blizzard III IPC mutex                        **
**                                                                      **
*************************************************************************/


b3IPCMutex::b3IPCMutex()
{
	pthread_mutex_init(&mutex,NULL);
}

b3IPCMutex::~b3IPCMutex()
{
	pthread_mutex_destroy(&mutex);
}

// Method for entering a critical code section
b3_bool b3IPCMutex::b3Lock()
{
	pthread_mutex_lock(&mutex);
	return true;
}

// Method for leaving a critical code section
b3_bool b3IPCMutex::b3Unlock()
{
	pthread_mutex_unlock(&mutex);
	return true;
}

/*************************************************************************
**                                                                      **
**                        b3 event                                      **
**                                                                      **
*************************************************************************/

b3Event::b3Event()
{
	pthread_cond_init(&event,NULL);
	pthread_mutex_init(&mutex,NULL);
}

b3Event::~b3Event()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&event);
}

// Signal another thread that there has happend something.
void b3Event::b3Pulse()
{
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&event);
	pthread_mutex_unlock(&mutex);
}

// Wait for a signal from another thread.
b3_bool b3Event::b3Wait()
{
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&event,&mutex);
	pthread_mutex_unlock(&mutex);
	return true;
}

/*************************************************************************
**                                                                      **
**                        b3 thread                                     **
**                                                                      **
*************************************************************************/

static b3_count   threadCount;
static b3IPCMutex threadMutex;

b3Thread::b3Thread(const char *task_name)
{
	m_Name      = task_name;
	m_IsRunning = false;
}

b3Thread::~b3Thread()
{
	b3Stop();
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
	b3Stop();

	m_CallProc = proc;
	m_CallArg  = ptr;
	pthread_create(&m_Thread,NULL,b3Trampoline,this);
	return true;
}

void * b3Thread::b3Trampoline(void *ptr)
{
	b3Thread *threadClass = (b3Thread *)ptr;
	b3_u32   *result      = &threadClass->m_Result;

	threadClass->m_Span.b3Start();
	threadMutex.b3Lock();
	threadCount++;
	threadMutex.b3Unlock();

	threadClass->m_IsRunning = true;
	*result = threadClass->m_CallProc(threadClass->m_CallArg);
	threadClass->m_IsRunning = false;

	threadMutex.b3Lock();
	threadCount--;
	threadMutex.b3Unlock();
	threadClass->m_Span.b3Stop();

	return result;
}

b3_bool b3Thread::b3IsRunning()
{
	return m_IsRunning;
}

b3_bool b3Thread::b3Stop()
{
	b3_bool was_running = m_IsRunning;

	if(m_IsRunning)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Thrd # terminating thread %02lX (%s).\n",
			0,
			m_Name != null ? m_Name : "no name");
		m_IsRunning = false;
		m_CallProc  = null;
		m_CallArg   = 0;

		threadMutex.b3Lock();
		threadCount--;
		threadMutex.b3Unlock();
		m_Span.b3Stop();
	}
	return was_running;
}

void b3Thread::b3Wait()
{
	void *ptr;

	pthread_join(m_Thread,&ptr);
}

void b3Thread::b3AddTimeSpan(b3TimeSpan *span)
{
	span->m_uTime += m_Span.m_uTime;
	span->m_sTime += m_Span.m_sTime;
} 

/*************************************************************************
**                                                                      **
**                        b3 count CPUs                                 **
**                                                                      **
*************************************************************************/

b3CPU::b3CPU()
{
	long result;

	result = sysconf(_SC_NPROCESSORS_ONLN);
	if ((result < 1) || (errno == EINVAL))
	{
		num = 1;
	}
	else
	{
		num = result;
	}
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
