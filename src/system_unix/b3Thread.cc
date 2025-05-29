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
**	(C) Copyright 2001 - 2024  Steffen A. Mork
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

#include <cassert>
#include <cstdio>
#include <cstring>
#include <unistd.h>

/*************************************************************************
**                                                                      **
**                        b3 event                                      **
**                                                                      **
*************************************************************************/

// Signal another thread that there has happend something.
void b3Event::b3Pulse()
{
	m_Pulsed = true;
	m_Conditional.notify_all();
}

// Wait for a signal from another thread.
bool b3Event::b3Wait()
{
	std::unique_lock lock(m_Mutex);

	m_Conditional.wait(lock, [&]
	{
		return m_Pulsed;
	});
	m_Pulsed = false;
	return true;
}

/*************************************************************************
**                                                                      **
**                        b3 thread                                     **
**                                                                      **
*************************************************************************/

std::mutex           b3Thread::m_ThreadMutex;
std::atomic_size_t   b3Thread::m_ThreadCount;

static std::atomic_int   threadSuccess;
static std::atomic_int   threadError;

b3Thread::b3Thread(const char * task_name)
{
	b3Name(task_name);
}

b3Thread::~b3Thread()
{
	b3Thread::b3Stop();
}

void b3Thread::b3Inc()
{
	std::lock_guard<std::mutex> lock(m_ThreadMutex);

	if (!m_IsRunning)
	{
		m_Span.b3Start();
		m_IsRunning = true;
		m_ThreadCount++;
	}
}

void b3Thread::b3Dec()
{
	std::lock_guard<std::mutex> lock(m_ThreadMutex);

	if (m_IsRunning)
	{
		m_ThreadCount--;
		m_IsRunning = false;
		m_Span.b3Stop();
	}
}

void b3Thread::b3Name(const char * task_name)
{
	if (task_name != nullptr)
	{
		m_Name.assign(task_name);
	}
	else
	{
		m_Name = "no name";
	}
}

bool b3Thread::b3Start(
	b3ThreadProc  proc,
	void     *    ptr,
	b3_s32        priority)
{
	bool    success;
	int     error_code;

	b3Stop();
	assert(!m_IsRunning);

	m_CallProc = proc;
	m_CallArg  = ptr;
	m_Result   = 0;
	m_Thread   = 0;
	m_Prio     = -priority * 5;

	success = b3PThread::b3CheckResult(error_code = pthread_create(&m_Thread, NULL, &b3Trampoline, this));
	if (success)
	{
		threadSuccess++;
		b3PrintF(B3LOG_FULL, "### CLASS: b3Thrd # started thread %02lX (%s).\n",
			m_Thread, m_Name.c_str());
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_ThreadMutex);

		threadError++;
		b3PrintF(B3LOG_NORMAL, "### CLASS: b3Thrd # Thread (%lx) not started!\n",
			m_Thread);
		b3PrintF(B3LOG_NORMAL, "    OK/error count: %d/%d\n",
			threadSuccess.load(),
			threadError.load());
		b3PrintF(B3LOG_NORMAL, "    thread count:   %zu\n",
			m_ThreadCount.load());
	}
	return success;
}

void * b3Thread::b3Trampoline(void * ptr)
{
	b3Thread * threadClass = static_cast<b3Thread *>(ptr);

	if (nice(threadClass->m_Prio) == -1)
	{
		b3PrintF(B3LOG_NORMAL, "   Nicing error!\n");
	}
	threadClass->b3Inc();
	// cppcheck-suppress cstyleCast
	threadClass->m_Result = threadClass->m_CallProc((void *)threadClass->m_CallArg);
	threadClass->b3Dec();

	return nullptr;
}

bool b3Thread::b3IsRunning()
{
	return m_IsRunning;
}

bool b3Thread::b3Stop()
{
	bool was_running;

	if (b3IsRunning())
	{
		pthread_cancel(m_Thread);
	}
	was_running = m_IsRunning;
	if (m_IsRunning)
	{
		b3PrintF(B3LOG_FULL, "### CLASS: b3Thrd # terminated thread %02lX (%s).\n",
			m_Thread, m_Name.c_str());
	}

	b3Dec();
	m_CallProc  = nullptr;
	m_CallArg   = 0;
	m_Thread    = 0;

	return was_running;
}

b3_u32 b3Thread::b3Wait()
{
	int   result;
	void * ptr = &result;

	b3PThread::b3CheckResult(pthread_join(m_Thread, &ptr));
	return m_Result;
}
