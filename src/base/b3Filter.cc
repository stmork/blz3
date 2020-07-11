/*
**
**	$Filename:	b3Filter.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Some filter methods
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3Filter.h"

/*************************************************************************
**                                                                      **
**                        b3Filter implementation                       **
**                                                                      **
*************************************************************************/

b3Filter * b3Filter::b3New(b3_filter filter)
{
	switch(filter)
	{
	case B3_FILTER_BOX:
		return new b3BoxFilter();

	case B3_FILTER_GAUSS:
		return new b3GaussFilter();

	case B3_FILTER_SHUTTER:
		return new b3ShutterFilter();
	}
	return null;
}

b3_f64 b3Filter::b3InvIntegral(b3_f64 val, b3_bool throw_exception)
{
	b3_f64 y, xLower, xMid, xUpper, diff;

	if(fabs(val) > 1)
	{
		if(throw_exception)
		{
			B3_THROW(b3FilterException, B3_FILTER_OUT_OF_RANGE);
		}
		return val;
	}

	y      = (val + 1) * 0.5;
	xLower = -1;
	xUpper =  1;

	do
	{
		xMid = (xLower + xUpper) * 0.5;

		if(b3Integral(xMid) < y)
		{
			xLower = xMid;
		}
		else
		{
			xUpper = xMid;
		}
		diff = xUpper - xLower;
	}
	while(diff > 0.001);

	return xMid;
}

/*************************************************************************
**                                                                      **
**                        b3GaussFilter implementation                  **
**                                                                      **
*************************************************************************/

#define GAUSS_ND_MAX      3.0
#define GAUSS_ND_STEP     (1.0 / 128.0)

#define GAUSS_ND_ENTRIES  ((b3_count)(GAUSS_ND_MAX / GAUSS_ND_STEP) + 2)

#define GAUSS_ND_INDEX(x) (((x) + GAUSS_ND_MAX) /  GAUSS_ND_STEP)

b3Array<b3_f64> b3GaussFilter::m_GaussNDTable;
b3_f64          b3GaussFilter::m_Area;

b3GaussFilter::b3GaussFilter()
{
	if(m_GaussNDTable.b3GetCount() == 0)
	{
		b3_f64 x;

		b3PrintF(B3LOG_FULL, "Init b3GaussFilter...\n");
		m_Area = 0;
		for(x = -GAUSS_ND_MAX; x <= (GAUSS_ND_MAX + GAUSS_ND_STEP * 0.5); x += GAUSS_ND_STEP)
		{
			m_GaussNDTable.b3Add(m_Area);
			m_Area += b3Func(x) * GAUSS_ND_STEP;
		}
		b3PrintF(B3LOG_FULL, "  area: %3.8f %d entries\n", m_Area, m_GaussNDTable.b3GetCount());
	}
}

b3_f64 b3GaussFilter::b3Func(b3_f64 x)
{
	return exp(-x * x * M_PI);
}

b3_f64 b3GaussFilter::b3Integral(b3_f64 val)
{
	b3_f64   lower, result;
	b3_index index;

	if(val <=  -GAUSS_ND_MAX)
	{
		return 0;
	}
	if(val >=   GAUSS_ND_MAX)
	{
		return 1;
	}

	lower = GAUSS_ND_INDEX(val);
	index = (b3_index)floor(lower);

	if(lower == index)
	{
		result = m_GaussNDTable[index];
	}
	else
	{
		b3_f64 a, b;

		a = lower - index;
		b = 1 - a;
		result = b * m_GaussNDTable[index] + a * m_GaussNDTable[index + 1];
	}
	return result / m_Area;
}

/*************************************************************************
**                                                                      **
**                        b3ShutterFilter implementation                **
**                                                                      **
*************************************************************************/

b3ShutterFilter::b3ShutterFilter(b3_f64 max)
{
	m_Max   = max * 2;
	m_lMax  = max * 2 - 1;
	m_uMax  = 1 - max * 2;
	m_Slope = 0.5 / max;
	m_Area  = 2.0 - 2.0 * max;
}

b3_f64 b3ShutterFilter::b3Func(b3_f64 x)
{
	b3_f64 ax = fabs(x);

	if(ax > 1)
	{
		return 0;
	}
	if(ax < m_uMax)
	{
		return 1;
	}
	return (1 - ax) * m_Slope;
}

b3_f64 b3ShutterFilter::b3Integral(b3_f64 x)
{
	b3_f64 result;

	if(x < -1)
	{
		return 0;
	}
	if(x >  1)
	{
		return 1;
	}

	if(x < m_lMax)
	{
		// Rising edge
		b3_f64 y = (x + 1) / m_Max;

		result = (x + 1) * y * 0.5;
	}
	else if(x > m_uMax)
	{
		// Falling edge
		b3_f64 y = (1 - x) / m_Max;

		result = m_Area - (1 - x) * y * 0.5;
	}
	else
	{
		// High level
		result = x + 1 - 0.5 * m_Max;
	}
	return result / m_Area;
}
