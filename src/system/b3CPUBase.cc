/*
**
**	$Filename:	b3CPUBase.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III CPU info base class
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3CPUBase.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/10/16 08:25:55  sm
**	- Moved CPU bit count into b3CPUBase class
**
**	Revision 1.1  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
*/

/*************************************************************************
**                                                                      **
**                        CPU info base class                           **
**                                                                      **
*************************************************************************/

b3_cpu_type b3CPUBase::cpu_type  = B3_UNKNOWN_ENDIAN;
b3_count    b3CPUBase::cpu_count = 0;
b3_count    b3CPUBase::cpu_bits  = 0;

b3CPUBase::b3CPUBase()
{
	if (cpu_type == B3_UNKNOWN_ENDIAN)
	{
		volatile b3_u32  value = (((b3_u32)B3_BIG_ENDIAN) << 16) | B3_LITTLE_ENDIAN;
		volatile b3_u16 *ptr   = (b3_u16 *)&value;

		cpu_type = (b3_cpu_type)ptr[0];
		cpu_bits = sizeof(void *) * 8;
	}
}
