#include "blz3/system/b3Log.h"
#include "blz3/system/b3Thread.h"

#define TEST_NUM_THREADS 5
#define TEST_LOOP        500000

class TestUnit;

class TestInfo
{
public:
	TestUnit *thisClass;
	b3_u32   *counter;
	int       num;
};

class TestUnit
{
	b3Mutex    tMutex;
	b3IPCMutex iMutex;
	b3Thread   thread[TEST_NUM_THREADS];
	b3_u32     counter;
	b3_u32     single_counter[TEST_NUM_THREADS];
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
		b3_u32    old;
		int       i,num;

		num = info->num;
		b3PrintF (B3LOG_NORMAL,"Thread %d started.\n",num + 1);
		for (i = 0;i < TEST_LOOP;i++)
		{
			old = thisClass->counter;
			info->counter[num]++;
			thisClass->counter = old + 1;
		}
		b3PrintF (B3LOG_NORMAL,"Thread %d stopped.\n",num + 1);
	}

	static b3_u32 task_thread(void *ptr)
	{
		TestInfo *info = (TestInfo *)ptr;
		TestUnit *thisClass = info->thisClass;
		b3_u32    old;
		int       i,num;

		num = info->num;
		b3PrintF (B3LOG_NORMAL,"Thread %d started.\n",num + 1);
		for (i = 0;i < TEST_LOOP;i++)
		{
			thisClass->tMutex.b3Lock();
			old = thisClass->counter;
			info->counter[num]++;
			thisClass->counter = old + 1;
			thisClass->tMutex.b3Unlock();
		}
		b3PrintF (B3LOG_NORMAL,"Thread %d stopped.\n",num + 1);
	}

	static b3_u32 task_ipc(void *ptr)
	{
		TestInfo *info = (TestInfo *)ptr;
		TestUnit *thisClass = info->thisClass;
		b3_u32    old;
		int       i,num;

		num = info->num;
		b3PrintF (B3LOG_NORMAL,"Thread %d started.\n",num + 1);
		for (i = 0;i < TEST_LOOP;i++)
		{
			thisClass->iMutex.b3Lock();
			old = thisClass->counter;
			info->counter[num]++;
			thisClass->counter = old + 1;
			thisClass->iMutex.b3Unlock();
		}
		b3PrintF (B3LOG_NORMAL,"Thread %d stopped.\n",num + 1);
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

void main(int argc,char *argv[])
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
