/*
**
**	$Filename:	b3Filter.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

#ifndef B3_BASE_FILTER_H
#define B3_BASE_FILTER_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Array.h"
#include "blz3/system/b3Exception.h"

enum b3_filter
{
	B3_FILTER_BOX     = 0, //!< A box filter.
	B3_FILTER_GAUSS   = 1, //!< A Gauss filter.
	B3_FILTER_SHUTTER = 2  //!< A filter like a camera shutter.
};

enum b3_error_filter
{
	B3_FILTER_ERROR = -1,
	B3_FILTER_OK    =  0,
	B3_FILTER_OUT_OF_RANGE
};

typedef b3Exception<b3_error_filter, 0x46494c> b3FilterException;

class b3BoxFilter;
class b3GaussFilter;
class b3ShutterFilter;

/**
 * This base class defines some virtual method for filter handling.
 */
class B3_PLUGIN b3Filter
{
public:
	virtual ~b3Filter() = default;

	/**
	 * This computes a filter value from the given position.
	 *
	 * \param x The filter position.
	 * \return The filter value.
	 */
	virtual b3_f64    b3Func(b3_f64 x) = 0;

	/**
	 * This method integrates over a specified domain.
	 * \param x The integral input value.
	 * \return The integral.
	 */
	virtual b3_f64    b3Integral(b3_f64 x) = 0;

	/**
	 * This method integrates over a filter kernel.
	 *
	 * \param val The value position to integrate.
	 * \param throw_exception A flag if throwing an exception on error.
	 * \throws b3FilterException
	 * \return The inverse integral.
	 */
	virtual b3_f64    b3InvIntegral(b3_f64 val, bool throw_exception = false);

	/**
	 * A factory method for generating several filters.
	 *
	 * \param filter The filter enum.
	 * \return The filter class instance.
	 */
	static  b3Filter * b3New(b3_filter filter);
};

/**
 * This class represents a box filter.
 */
class B3_PLUGIN b3BoxFilter : public b3Filter
{
public:
	inline b3_f64 b3Func(b3_f64 x) override
	{
		return fabs(x) <= 1 ? 1 : 0;
	}

	inline b3_f64 b3Integral(b3_f64 x) override
	{
		if (x < -1)
		{
			return 0;
		}
		else if (x >  1)
		{
			return 1;
		}
		else
		{
			return (x + 1) * 0.5;
		}
	}

	inline b3_f64 b3InvIntegral(b3_f64 val, bool throw_exception = false) override
	{
		if ((fabs(val) > 1) && throw_exception)
		{
			B3_THROW(b3FilterException, B3_FILTER_OUT_OF_RANGE);
		}
		return val;
	}
};


/**
 * This class represents a Gauss filter.
 */
class B3_PLUGIN b3GaussFilter : public b3Filter
{
	static b3Array<b3_f64> m_GaussNDTable;
	static b3_f64          m_Area;

public:
	b3GaussFilter();
	b3_f64 b3Func(b3_f64 x) override;
	b3_f64 b3Integral(b3_f64 x) override;
};

/**
 * This class represents a photographic shutter filter.
 */
class B3_PLUGIN b3ShutterFilter : public b3Filter
{
	b3_f64 m_lMax, m_uMax, m_Max;
	b3_f64 m_Slope;
	b3_f64 m_Area;

public:
	b3ShutterFilter(b3_f64 max = 0.25);
	b3_f64 b3Func(b3_f64 x) override;
	b3_f64 b3Integral(b3_f64 x) override;
};

#endif
