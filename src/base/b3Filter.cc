/*
**
**      $Filename:      b3Filter.cc $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Some filter methods
**
**      (C) Copyright 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Filter.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/08/21 20:13:32  sm
**	- Introduced distributed raytracing with all sampling methods
**	  and filter computations. This made some class movements
**	  inside files necessary. The next step would be to integrate
**	  motion blur.
**
**
*/

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

b3Filter *b3Filter::b3New(b3_filter filter)
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

#define GAUSS_ND_ENTRIES  ((int)(GAUSS_ND_MAX / GAUSS_ND_STEP) + 2)

b3Array<b3_f64> b3GaussFilter::m_GaussNDTable;

b3GaussFilter::b3GaussFilter(b3_f64 step,b3_f64 max)
{
	if (m_GaussNDTable.b3GetCount() == 0)
	{
		b3_f64   start=0.5,i,x,val,c;

		c = 1.0 / sqrt(M_PI * 2.0);
		for (i = 0;i <= max;i += step)
		{
			m_GaussNDTable.b3Add(start);
			x      = i + 0.5 * step;
			val    = c * exp(-x * x * 0.5);
			start += step * val;
		}
		m_GaussNDTable.b3Add(1.0);
	}
}

b3_f64 b3GaussFilter::b3Func(b3_f64 val)
{
	return exp(-val * val * 0.5) / sqrt(M_PI * 2.0);
}

b3_f64 b3GaussFilter::b3Integral(b3_f64 val)
{
	b3_count lower,upper,half,diff;
	b3_f64   q,x,xl,xu;
	b3_f64   result;

	x     = (val < 0.5 ? 1.0 - val : val);
	lower = 0;
	upper = GAUSS_ND_ENTRIES - 1;
	do
	{
		half = (lower + upper) >> 1;
		if ((m_GaussNDTable[lower] <= x) && (x <= m_GaussNDTable[half]))
		{
			upper = half;
		}
		else
		{
			lower = half;
		}
		diff =  upper - lower;
	}
	while (diff > 1);
	xl = lower * GAUSS_ND_STEP;
	xu = upper * GAUSS_ND_STEP;
	q  = (x - m_GaussNDTable[lower]) / (m_GaussNDTable[upper] - m_GaussNDTable[lower]);
	if (val < 0.5) result =  - xu * q - xl * (1.0 - q);
	else           result =    xu * q + xl * (1.0 - q);
	
	return result / GAUSS_ND_MAX + 0.5;
}
