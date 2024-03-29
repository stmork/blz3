/*
**
**	$Filename:	TestThread.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Testing the effects of accessing variables from
**	               multiple threads.
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Thread.h"

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

#define TEST_NUM_THREADS 5
#define TEST_LOOP        5000000

class TestUnit;

class TestInfo
{
public:
	TestUnit * thisClass;
	volatile b3_u32  * counter;
	int       num;
};

class TestUnit
{
	b3Mutex    tMutex;
	b3IPCMutex iMutex;
	b3Thread   thread[TEST_NUM_THREADS];
	volatile b3_u32     counter;
	volatile b3_u32     single_counter[TEST_NUM_THREADS];
	TestInfo   info[TEST_NUM_THREADS];

public:
	TestUnit()
	{
		int i;

		for (i = 0; i < TEST_NUM_THREADS; i++)
		{
			info[i].thisClass = this;
			info[i].counter   = single_counter;
			info[i].num       = i;
			single_counter[i] = 0;
		}
		counter = 0;
	}

	bool TestStart(b3ThreadProc proc)
	{
		int i;

		// Starting...
		for (i = 0; i < TEST_NUM_THREADS; i++)
		{
			if (!thread[i].b3Start(proc, &info[i]))
			{
				return false;
			}
		}
		return true;
	}

	b3_u32 TestWait()
	{
		b3_u32 safe_count = 0;
		int    i;

		// Waiting...
		for (i = 0; i < TEST_NUM_THREADS; i++)
		{
			thread[i].b3Wait();
			safe_count += single_counter[i];
		}
		return safe_count;
	}

	static bool task_unsafe(void * ptr)
	{
		TestInfo * info = (TestInfo *)ptr;
		TestUnit * thisClass = info->thisClass;
		int       i, num;

		num = info->num;
		b3PrintF(B3LOG_NORMAL, "Thread %d started.\n", num + 1);
		for (i = 0; i < TEST_LOOP; i++)
		{
			thisClass->counter++;
			info->counter[num]++;
		}
		b3PrintF(B3LOG_NORMAL, "Thread %d stopped.\n", num + 1);
		return 0;
	}

	static bool task_thread(void * ptr)
	{
		TestInfo * info = (TestInfo *)ptr;
		TestUnit * thisClass = info->thisClass;
		int        i, num;

		num = info->num;
		b3PrintF(B3LOG_NORMAL, "Thread %d started.\n", num + 1);
		for (i = 0; i < TEST_LOOP; i++)
		{
			b3CriticalSection lock(thisClass->tMutex);

			info->counter[num]++;
			thisClass->counter++;
		}
		b3PrintF(B3LOG_NORMAL, "Thread %d stopped.\n", num + 1);
		return 0;
	}

	static bool task_ipc(void * ptr)
	{
		TestInfo * info = (TestInfo *)ptr;
		TestUnit * thisClass = info->thisClass;
		int        i, num;

		num = info->num;
		b3PrintF(B3LOG_NORMAL, "Thread %d started.\n", num + 1);
		for (i = 0; i < TEST_LOOP; i++)
		{
			b3CriticalSection lock(thisClass->iMutex);

			info->counter[num]++;
			thisClass->counter++;
		}
		b3PrintF(B3LOG_NORMAL, "Thread %d stopped.\n", num + 1);
		return 0;
	}

	void test1()
	{
		b3_u32 safe_count;

		if (TestStart(task_unsafe))
		{
			safe_count = TestWait();

			// Printing result
			b3PrintF(B3LOG_NORMAL, "counter %u - counted %u (%s)\n",
				counter, safe_count,
				counter != safe_count ? "expected" : "not expected");
		}
	}

	void test2()
	{
		b3_u32 safe_count;

		if (TestStart(task_thread))
		{
			safe_count = TestWait();

			// Printing result
			b3PrintF(B3LOG_NORMAL, "counter %u - counted %u (%s)\n",
				counter, safe_count,
				counter != safe_count ? "not OK" : "OK");
		}
	}

	void test3()
	{
		b3_u32 safe_count;

		if (TestStart(task_ipc))
		{
			safe_count = TestWait();

			// Printing result
			b3PrintF(B3LOG_NORMAL, "counter %u - counted %u (%s)\n",
				counter, safe_count,
				counter != safe_count ? "not OK" : "OK");
		}
	}
};

#define MAX_TEST_THREAD_COUNT 20000

static void * b3Counter(void * ptr)
{
	int * counter = (int *)ptr;

	counter[0]++;
	return 0;
}

static bool b3CounterThread(void * ptr)
{
	b3_count * counter = (b3_count *)ptr;

	b3Counter(ptr);
	return counter[0] > 0;
}

static void b3TestThreadStart1()
{
#ifndef _WINDOWS
	pthread_t thread;
	int       counter = 0;
	int       i;
	int       error_code;
	int       result = 0;

	b3PrintF(B3LOG_NORMAL, "Threads using original POSIX calls...\n");
	for (i = 0; i < MAX_TEST_THREAD_COUNT; i++)
	{
		thread = 0;
		result = 0;
		error_code = pthread_create(&thread, NULL, &b3Counter, &counter);
		if (error_code == 0)
		{
			pthread_join(thread, (void **)&result);
		}
		else
		{
			fprintf(stderr, "thread not created at attempt %d - errno: %d (%s)\n", i,
				error_code, strerror(error_code));
			exit(EXIT_FAILURE);
		}
	}
	b3PrintF(B3LOG_NORMAL, "  Counter: %d\n", counter);
	b3PrintF(B3LOG_NORMAL, "Done.\n\n");
#endif
}

static void b3TestThreadStart2()
{
	b3Thread thread;
	b3_count counter = 0;
	b3_index i;

	b3PrintF(B3LOG_NORMAL, "Threads using Blizzard III semantics...\n");
	for (i = 0; i < MAX_TEST_THREAD_COUNT; i++)
	{
		if (!thread.b3Start(&b3CounterThread, &counter))
		{
			b3PrintF(B3LOG_NORMAL, "Thread not started!\n");
			b3PrintF(B3LOG_NORMAL, "  attempt: %d\n", i);
			b3PrintF(B3LOG_NORMAL, "  counter: %d\n", counter);
			return;
		}
		thread.b3Wait();
	}
	b3PrintF(B3LOG_NORMAL, "  Counter: %d\n", counter);
	b3PrintF(B3LOG_NORMAL, "Done.\n\n");
}

int main()
{
	TestUnit * ptr;

	b3TestThreadStart1();
	b3TestThreadStart2();

	ptr = new TestUnit();
	ptr->test1();
	delete ptr;

	ptr = new TestUnit();
	ptr->test2();
	delete ptr;

	ptr = new TestUnit();
	ptr->test3();
	delete ptr;

	//	b3LockerTest test;

	return EXIT_SUCCESS;
}
