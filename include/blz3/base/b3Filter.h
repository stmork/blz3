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
#include "blz3/system/b3Exception.h"

enum b3_filter
{
	B3_FILTER_BOX     = 0,
	B3_FILTER_GAUSS   = 1,
	B3_FILTER_SHUTTER = 2
};

enum b3_error_filter
{
	B3_FILTER_ERROR = -1,
	B3_FILTER_OK    =  0,
	B3_FILTER_OUT_OF_RANGE
};

typedef b3Exception<b3_error_filter,'FIL'> b3FilterException;

class b3BoxFilter;
class b3GaussFilter;
class b3ShutterFilter;

class B3_PLUGIN b3Filter
{
public:
	virtual b3_f64    b3Func(b3_f64 x) = 0;
	virtual b3_f64    b3Integral(b3_f64 x) = 0;
	virtual b3_f64    b3InvIntegral(b3_f64 val,b3_bool throw_exception = false);

	static  b3Filter *b3New(b3_filter filter);
};

class B3_PLUGIN b3BoxFilter : public b3Filter
{
public:
	inline b3_f64 b3Func(b3_f64 x)
	{
		return fabs(x) <= 1 ? 1 : 0;
	}

	inline b3_f64 b3Integral(b3_f64 x)
	{
		if (x < -1) return 0;
		if (x >  1) return 1;
		return (x + 1) * 0.5;
	}

	inline b3_f64 b3InvIntegral(b3_f64 x,b3_bool throw_exception = false)
	{
		if ((fabs(x) > 1) && throw_exception)
		{
			B3_THROW(b3FilterException,B3_FILTER_OUT_OF_RANGE);
		}
		return x;
	}
};


class B3_PLUGIN b3GaussFilter : public b3Filter
{
	static b3Array<b3_f64> m_GaussNDTable;
	static b3_f64          m_Area;

public:
	              b3GaussFilter();
	b3_f64 b3Func(b3_f64 value);
	b3_f64 b3Integral(b3_f64 value);
};

class B3_PLUGIN b3ShutterFilter : public b3Filter
{
	b3_f64 m_lMax,m_uMax,m_Max;
	b3_f64 m_Slope;
	b3_f64 m_Area;

public:
	       b3ShutterFilter(b3_f64 max = 0.25);
	b3_f64 b3Func(b3_f64 value);
	b3_f64 b3Integral(b3_f64 value);
};

#endif
