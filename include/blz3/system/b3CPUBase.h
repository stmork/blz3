/*
**
**	$Filename:	b3CPUBase.h $
**	$Release:	Dortmund 2003 $
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

#ifndef B3_SYSTEM_CPUBASE_H
#define B3_SYSTEM_CPUBASE_H

#include "blz3/b3Types.h"

// Info about available CPUs
enum B3_PLUGIN b3_cpu_type
{
	B3_UNKNOWN_ENDIAN = 0,
	B3_BIG_ENDIAN     = 0x4d4d,
	B3_LITTLE_ENDIAN  = 0x4949
};

class B3_PLUGIN b3CPUBase
{
	static b3_cpu_type cpu_type;

protected:
	static b3_count    num;

public:
	                b3CPUBase();

	static inline b3_count b3GetNumCPUs()
	{
		return num;
	}

	static inline b3_cpu_type b3GetCPUType()
	{
		return cpu_type;
	}
};

#endif
