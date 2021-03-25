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

alignas(16) b3_f64 b3Math::epsilon = FLT_EPSILON;
alignas(16) b3_f64 b3Math::m_CbrtCoeffs[15]
{
	3.077639180760657e+00,
	-1.519854532158648e+01,
	7.930706084447468e+01,
	-3.106172426853194e+02,
	9.151589468494492e+02,
	-2.062358446605380e+03,
	3.597304181046540e+03,
	-4.882101094623766e+03,
	5.147158409952417e+03,
	-4.177391100917827e+03,
	2.561593805777631e+03,
	-1.148437885477766e+03,
	3.553023262165486e+02,
	-6.781799589991569e+01,
	6.019941663742065e+00
};

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
