/*
**
**	$Filename:	b3Filter.h $ 
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing Structure Definitions
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_FILTER_H
#define B3_BASE_FILTER_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Array.h"

enum b3_filter
{
	B3_FILTER_BOX     = 0,
	B3_FILTER_GAUSS   = 1,
	B3_FILTER_SHUTTER = 2
};

class b3BoxFilter;
class b3GaussFilter;
class b3ShutterFilter;

class b3Filter
{
public:
	virtual b3_f64    b3Func(b3_f64 value) = 0;
	virtual b3_f64    b3Integral(b3_f64 value) = 0;
	static  b3Filter *b3New(b3_filter filter);
};

#define GAUSS_ND_MAX      3.0
#define GAUSS_ND_STEP     0.0078125

class b3BoxFilter : public b3Filter
{
public:
	inline b3_f64 b3Func(b3_f64 value)
	{
		return 1.0;
	}

	inline b3_f64 b3Integral(b3_f64 value)
	{
		return value;
	}
};


class b3GaussFilter : public b3Filter
{
	static b3Array<b3_f64> m_GaussNDTable;
public:
	              b3GaussFilter(b3_f64 step = GAUSS_ND_STEP,b3_f64 max = GAUSS_ND_MAX);
	b3_f64 b3Func(b3_f64 value);
	b3_f64 b3Integral(b3_f64 value);
};

class b3ShutterFilter : public b3Filter
{
	b3_f64 m_lMax;
	b3_f64 m_uMax;
	b3_f64 m_Slope;
	b3_f64 m_Area;

public:
	b3ShutterFilter(b3_f64 max = 0.25)
	{
		B3_ASSERT((max >= 0) && (max <= 0.5));
		m_lMax  = max;
		m_uMax  = 1.0 - max;
		m_Slope = 1.0 / max;
		m_Area  = 1.0 - max;
	}

	inline b3_f64 b3Func(b3_f64 value)
	{
		if ((value < 0) || (value > 1))
		{
			return 0;
		}
		if (value < m_lMax)
		{
			return value * m_Slope;
		}
		if (value > m_uMax)
		{
			return (1.0 - value) * m_Slope;
		}
		return 1.0;
	}

	inline b3_f64 b3Integral(b3_f64 value)
	{
		b3_f64 result;

		if (value < 0)
		{
			return 0;
		}
		if (value > 1)
		{
			return 1;
		}
		if (value < m_lMax)
		{
			b3_f64 x,y;

			// Rising edge
			x = value;
			y = x * m_Slope;
			result = x * y * 0.5;
		}
		else if (value > m_uMax)
		{
			b3_f64 x,y;

			// falling edge
			x = 1.0 - value;
			y = x * m_Slope;
			result = m_Area - x * y * 0.5;
		}
		else
		{
			result = value - m_lMax * 0.25;
		}

		return result / m_Area;
	}
};

#endif
