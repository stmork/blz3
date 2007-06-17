/*
**
**	$Filename:	b3CPU.cpp $
**	$Release:	Dortmund 2005 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CPU handling
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

#include "b3SystemInclude.h"
#include "blz3/system/b3Thread.h"

/*************************************************************************
**                                                                      **
**                        b3CPU implementation                          **
**                                                                      **
*************************************************************************/

b3CPU::b3CPU()
{
	if (cpu_count == 0)
	{
		SYSTEM_INFO  info;

		GetSystemInfo(&info);
		cpu_count = info.dwNumberOfProcessors;
		if (info.dwNumberOfProcessors < 1)
		{
			cpu_count = 1;
		}
	}
}

b3_count b3CPU::b3GetNumThreads()
{
	b3_count resuming;

	b3Thread::m_ThreadMutex.b3Lock();
	if (cpu_count > b3Thread::m_ThreadCount)
	{
		resuming = cpu_count - b3Thread::m_ThreadCount;
	}
	else
	{
		resuming = 1;
	}
	b3Thread::m_ThreadMutex.b3Unlock();
	return resuming;
}
