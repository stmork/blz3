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

#include "blz3/system/b3Log.h"
#include "blz3/system/b3Thread.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2002/03/16 15:12:57  sm
**	- Making b3Mem system independend using new system dependen b3MemAccess
**	  class with static methods.
**	- Adjusting TestThread with volatile keyword to avoid the compiler
**	  optimizeing access to the test counter.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

#define TEST_NUM_THREADS 5
#define TEST_LOOP        500000

class TestUnit;

class TestInfo
{
public:
	         TestUnit *thisClass;
	volatile b3_u32   *counter;
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

		for (i = 0;i < TEST_NUM_THREADS;i++)
		{
			info[i].thisClass = this;
			info[i].counter   = single_counter;
			info[i].num       = i;
			single_counter[i] = 0;
		}
		counter = 0;
	}

	void TestStart(b3ThreadProc proc)
	{
		int i;

		// Starting...
		for (i = 0;i < TEST_NUM_THREADS;i++)
		{
			thread[i].b3Start(proc,&info[i]);
		}
	}

	b3_u32 TestWait()
	{
		b3_u32 safe_count = 0;
		int    i;

		// Waiting...
		for (i = 0;i < TEST_NUM_THREADS;i++)
		{
			thread[i].b3Wait();
			safe_count += single_counter[i];
		}
		return safe_count;
	}

	static b3_u32 task_unsafe(void *ptr)
	{
		TestInfo *info = (TestInfo *)ptr;
		TestUnit *thisClass = info->thisClass;
		int       i,num;

		num = info->num;
		b3PrintF (B3LOG_NORMAL,"Thread %d started.\n",num + 1);
		for (i = 0;i < TEST_LOOP;i++)
		{
			thisClass->counter++;
			info->counter[num]++;
		}
		b3PrintF (B3LOG_NORMAL,"Thread %d stopped.\n",num + 1);
		return 0;
	}

	static b3_u32 task_thread(void *ptr)
	{
		TestInfo *info = (TestInfo *)ptr;
		TestUnit *thisClass = info->thisClass;
		int       i,num;

		num = info->num;
		b3PrintF (B3LOG_NORMAL,"Thread %d started.\n",num + 1);
		for (i = 0;i < TEST_LOOP;i++)
		{
			thisClass->tMutex.b3Lock();
			info->counter[num]++;
			thisClass->counter++;
			thisClass->tMutex.b3Unlock();
		}
		b3PrintF (B3LOG_NORMAL,"Thread %d stopped.\n",num + 1);
		return 0;
	}

	static b3_u32 task_ipc(void *ptr)
	{
		TestInfo *info = (TestInfo *)ptr;
		TestUnit *thisClass = info->thisClass;
		int       i,num;

		num = info->num;
		b3PrintF (B3LOG_NORMAL,"Thread %d started.\n",num + 1);
		for (i = 0;i < TEST_LOOP;i++)
		{
			thisClass->iMutex.b3Lock();
			info->counter[num]++;
			thisClass->counter++;
			thisClass->iMutex.b3Unlock();
		}
		b3PrintF (B3LOG_NORMAL,"Thread %d stopped.\n",num + 1);
		return 0;
	}

	void test1()
	{
		b3_u32 safe_count;

		TestStart(task_unsafe);
		safe_count = TestWait();

		// Printing result
		b3PrintF (B3LOG_NORMAL,"counter %u - counted %u (%s)\n",
			counter,safe_count,
			counter != safe_count ? "expected" : "not expected");
	}

	void test2()
	{
		b3_u32 safe_count;

		TestStart(task_thread);
		safe_count = TestWait();

		// Printing result
		b3PrintF (B3LOG_NORMAL,"counter %u - counted %u (%s)\n",
			counter,safe_count,
			counter != safe_count ? "not OK" : "OK");
	}

	void test3()
	{
		b3_u32 safe_count;

		TestStart(task_ipc);
		safe_count = TestWait();

		// Printing result
		b3PrintF (B3LOG_NORMAL,"counter %u - counted %u (%s)\n",
			counter,safe_count,
			counter != safe_count ? "not OK" : "OK");
	}
};

int main(int argc,char *argv[])
{
	TestUnit *ptr;

	ptr = new TestUnit();
	ptr->test1();
	delete ptr;

	ptr = new TestUnit();
	ptr->test2();
	delete ptr;

	ptr = new TestUnit();
	ptr->test3();
	delete ptr;

	return 0;
}
