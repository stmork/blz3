/*
**
**	$Filename:	b3CPU.cpp $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.1  2005/06/03 09:00:34  smork
**	- Moved b3CPU into own file.
**	
**	
*/


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
