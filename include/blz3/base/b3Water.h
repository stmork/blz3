/*
**
**	$Filename:	b3Water.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple water surface bump map
**	               functions.
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_WATER_H
#define B3_BASE_WATER_H

#include "blz3/b3Config.h"

/**
 * This class computes water simulation.
 */
class B3_PLUGIN b3Water
{
	b3_f64    m_Factor;
	b3_f64    m_t;

public:
	b3_count  m_Octaves;    //!< The number of octaves to use for b3Turbulence().
	b3_f32    m_Km;         //!< A scaling factor for the resulting turbulence.
	b3_f32    m_WindAmp;    //!< The wind amplitude.
	b3_f32    m_WindFreq;   //!< The wind frequency
	b3_f32    m_MinWind;    //!< The minimal wind component.
	b3_f32    m_ScaleTime;  //!< The wind speed as time scaling.
	b3_vector m_Anim;       //!< The wind direction.

public:
	/**
	 * This constructor initializes this instance with default values.
	 */
	b3Water();

	/**
	 * This method precomputes some calculation invariant values.
	 *
	 * @param time The time point for animation.
	 */
	void   b3PrepareWater(const b3_f64 time);

	/**
	 * This method returns a water value in range [0..1]. This value
	 * is a relative height value.
	 *
	 * @param point The position in space.
	 * @return The water height.
	 */
	b3_f64 b3ComputeWater(const b3_vector *point);
};

#endif
