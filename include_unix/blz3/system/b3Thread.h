/*
**
**	$Filename:	b3Thread.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - SMP cotrol (Un*x)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_THREAD_H
#define B3_SYSTEM_THREAD_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Time.h"
#include <pthread.h>

class b3Mutex
{
	pthread_mutex_t mutex;
public:
	         b3Mutex();
	        ~b3Mutex();
	b3_bool  b3Lock();
	b3_bool  b3Unlock();
};

class b3IPCMutex
{
	pthread_mutex_t mutex;
public:
	         b3IPCMutex();
	        ~b3IPCMutex();
	b3_bool  b3Lock();
	b3_bool  b3Unlock();
};

class b3Event
{
	pthread_cond_t  event;
	pthread_mutex_t mutex;

public:
	         b3Event();
	        ~b3Event();
	void     b3Pulse();
	b3_bool  b3Wait();
};

typedef b3_u32 (*b3ThreadProc)(void *);

class b3Thread
{
	const char   *m_Name;
	b3TimeSpan    m_Span;
protected:
	pthread_t     m_Thread;
	b3_bool       m_IsRunning;
	b3_u32        m_Result;
	b3ThreadProc  m_CallProc;
	void         *m_CallArg;
public:
	         b3Thread(const char *task_name = null);
	        ~b3Thread();
	void     b3Name(const char *task_name = null);
	b3_bool  b3Start(b3ThreadProc,void *,b3_s32 pri=0);
	b3_bool  b3IsRunning();
	b3_bool  b3Stop();
	void     b3Wait();
private:
	static void *b3Trampoline(void *thread);
};

// Info about available CPUs
class b3CPU
{
	b3_count num;
public:
	         b3CPU();
	b3_count b3GetNumThreads();
	b3_count b3GetNumCPUs();
};

#endif
