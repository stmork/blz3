/*
**
**	$Filename:	b3CPU.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
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

#ifdef __linux__
#include <sys/utsname.h>
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2005/06/03 09:00:34  smork
**	- Moved b3CPU into own file.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        b3 count CPUs                                 **
**                                                                      **
*************************************************************************/

#ifndef _SC_NPROCESSORS_ONLN
#ifdef  _SC_NPROC_ONLN
#define _SC_NPROCESSORS_ONLN _SC_NPROC_ONLN
#endif
#endif

b3_bool b3CPU::m_CorrectRUsage = true;

b3CPU::b3CPU()
{
	if (cpu_count == 0)
	{
		long    result;

#ifdef _SC_NPROCESSORS_ONLN
		result = sysconf(_SC_NPROCESSORS_ONLN);
#else
		result = 1;
#endif
		if ((result < 1) || (errno == EINVAL))
		{
			cpu_count = 1;
		}
		else
		{
			cpu_count = result;
		}

#ifdef __linux__
		struct utsname uinfo;
		int            a,b,c;

		if (uname(&uinfo) == 0)
		{
			if (sscanf(uinfo.release,"%d.%d.%d",&a,&b,&c) == 3)
			{
				if ((a * 100000 + b * 1000 + c) < 206009)
				{
					m_CorrectRUsage = false;
				}
			}
		}

		if (!m_CorrectRUsage)
		{
			b3PrintF(B3LOG_NORMAL,"Found Linux kernel %d.%d.%d with wrong resource usage measurement.\n",
				a,b,c);
		}
#endif
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
