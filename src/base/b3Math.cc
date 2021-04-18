/*
**
**	$Filename:	b3Math.cc $
**	$Release:	Dortmund 2004 $
**	$Revision: 2.02
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Basic math methods
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#define MATRIX_INV_4D

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3Math.h"
#include <float.h>

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

b3_loop b3Math::b3PowOf2(const b3_loop value)
{
	b3_loop result = 1;

	// Prevent busy loop
	if ((value & 0x80000000) != 0)
	{
		return 0x80000000;
	}

	while (value > result)
	{
		result = result << 1;
	}
	return result;
}

b3_count b3Math::b3Log2(b3_u32 value)
{
	b3_count count = -1;

	while (value > 0)
	{
		value = value >> 1;
		count++;
	}
	return count;
}
