/*
**
**	$Filename:	b3CPU.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III CPU info
**
**	(C) Copyright 2005  Steffen A. Mork
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
#include <unistd.h>
#include <errno.h>
#include <thread>

#ifdef __linux__
#include <sys/utsname.h>
#endif

/*************************************************************************
**                                                                      **
**                        b3 count CPUs                                 **
**                                                                      **
*************************************************************************/

b3_bool b3CPU::m_CorrectRUsage = true;

b3CPU::b3CPU()
{
	if (cpu_count == 0)
	{
		cpu_count = std::thread::hardware_concurrency();

#ifdef __linux__
		struct utsname uinfo;
		int            a, b, c;

		if (uname(&uinfo) == 0)
		{
			if (sscanf(uinfo.release, "%d.%d.%d", &a, &b, &c) == 3)
			{
				if ((a * 100000 + b * 1000 + c) < 206009)
				{
					m_CorrectRUsage = false;
				}
			}
		}

		if (!m_CorrectRUsage)
		{
			b3PrintF(B3LOG_NORMAL, "Found Linux kernel %d.%d.%d with wrong resource usage measurement.\n",
				a, b, c);
		}
#endif
	}
}

b3_count b3CPU::b3GetNumThreads()
{
	b3_count          resuming;
	b3CriticalSection lock(b3Thread::m_ThreadMutex);

	if (cpu_count > b3Thread::m_ThreadCount)
	{
		resuming = cpu_count - b3Thread::m_ThreadCount;
	}
	else
	{
		resuming = 1;
	}
	return resuming;
}
