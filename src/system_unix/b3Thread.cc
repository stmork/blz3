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
**                        includes                                      **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Thread.h"
#include "blz3/system/b3Log.h"

#include <unistd.h>
#include <errno.h>

/*************************************************************************
**                                                                      **
**                        b3 mutex                                      **
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
**                        b3 IPC mutex                                  **
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

static b3_u32     threadCount;
static b3IPCMutex threadMutex;

b3Thread::b3Thread(const char *task_name)
{
	name       = task_name;
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

b3_bool b3Thread::b3Start(
	b3ThreadProc  proc,
	void         *ptr,
	b3_s32        priority)
{
	b3Stop();

	callProc = proc;
	callArg  = ptr;
	pthread_create(&thread,NULL,b3Trampoline,this);
	return true;
}

void * b3Thread::b3Trampoline(void *ptr)
{
	b3Thread *threadClass = (b3Thread *)ptr;
	b3_u32   *result      = &threadClass->result;

	threadMutex.b3Lock();
	threadCount++;
	threadMutex.b3Unlock();

	threadClass->is_running = true;
	*result = threadClass->callProc(threadClass->callArg);
	threadClass->is_running = false;

	threadMutex.b3Lock();
	threadCount--;
	threadMutex.b3Unlock();

	return result;
}

b3_bool b3Thread::b3IsRunning()
{
	return is_running;
}

b3_bool b3Thread::b3Stop()
{
	b3_bool was_runnung = is_running;

	if(is_running)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: mauiThrd # terminating thread %02lX (%s).\n",
			0,
			name != null ? name : "no name");
		is_running = false;
		callProc   = null;
		callArg    = 0;

		threadMutex.b3Lock();
		threadCount--;
		threadMutex.b3Unlock();
	}
}

void b3Thread::b3Wait()
{
	void *ptr;

	pthread_join(thread,&ptr);
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

b3_u32 b3CPU::b3GetCPU()
{
	return num;
}

b3_u32 b3CPU::b3GetThreads()
{
	b3_u32 resuming;

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

b3_u32 b3CPU::b3GetNumCPU()
{
	return num;
}
