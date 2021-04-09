/*
**
**	$Filename:	b3CPUBase.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3SystemIndInclude.h"
#include "blz3/system/b3CPUBase.h"

/*************************************************************************
**                                                                      **
**                        CPU info base class                           **
**                                                                      **
*************************************************************************/

b3CPUBase::b3CPUBase()
{
	if (cpu_type == B3_UNKNOWN_ENDIAN)
	{
		volatile b3_u32   value = (((b3_u32)B3_BIG_ENDIAN) << 16) | B3_LITTLE_ENDIAN;
		volatile b3_u16 * ptr   = (b3_u16 *)&value;

		cpu_type = (b3_cpu_type)ptr[0];
	}
}
