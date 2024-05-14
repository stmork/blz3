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
**	(C) Copyright 2005 - 2024  Steffen A. Mork
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
#include <cerrno>
#include <thread>

#ifdef __linux__
#include <sys/utsname.h>
#endif

/*************************************************************************
**                                                                      **
**                        b3 count CPUs                                 **
**                                                                      **
*************************************************************************/

bool b3CPU::m_CorrectRUsage = true;

b3CPU::b3CPU()
{
	if (cpu_count == 0)
	{
		cpu_count = std::thread::hardware_concurrency();

#ifdef __linux__
		struct utsname uinfo;
		int            a = 0, b = 0, c = 0;

		if (uname(&uinfo) == 0)
		{
			if (sscanf(uinfo.release, "%d.%d.%d", &a, &b, &c) == 3)
			{
				m_CorrectRUsage = (a * 100000 + b * 1000 + c) >= 206009;
			}
		}

		if (!m_CorrectRUsage)
		{
			b3PrintF(B3LOG_NORMAL,
				"Found Linux kernel %d.%d.%d with wrong resource usage measurement.\n",
				a, b, c);
		}
#endif
	}
}
