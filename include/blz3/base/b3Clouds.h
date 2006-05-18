/*
**
**	$Filename:	b3Clouds.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Clouds.
**	               functions.
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_CLOUDS_H
#define B3_BASE_CLOUDS_H

#include "blz3/b3Config.h"

/**
 * This class computes cloud simulation.
 */
class B3_PLUGIN b3Clouds
{
	b3_f64    m_EarthRadiusSqr;
	b3_f64    m_CloudRadiusSqr;

public:
	b3_vector m_Anim;        //!< x/y are wind direction, z is time scaling.
	b3_vector m_PosScale;    //!< Position scaling.
	b3_u32    m_Flags;       //!< Some flags.
	b3_f32    m_EarthRadius; //!< The plant radius to use.
	b3_f32    m_CloudHeight; //!< The height of the clouds.
	b3_f32    m_Seeing;      //!< Fog
	b3_f32    m_Scaling;     //!< Scaling.
	b3_f32    m_Sharpness;   //!< The sharpness of the cloud border.

public:
	/**
	 * This constructor initializes this instance with default values.
	 */
	b3Clouds();

	/**
	 * This method precomputes some calculation invariant values.
	 */
	void   b3PrepareClouds();

	/**
	 * This method returns a cloud value in range [0..1]. The value 0 means
	 * clear sky and 1 means full cloud.
	 *
	 * @param ray The ray containing the view direction.
	 * @param r The distance to the cloud.
	 * @param time The time point for animation.
	 * @return The cloudiness.
	 */
	b3_f64 b3ComputeClouds(const b3_line64 *ray,b3_f64 &r, const b3_f64 time);
};

#endif
