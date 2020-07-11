/*
**
**	$Filename:	b3Scaling.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Basic scaling methods for diverse mappings
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_SCALING_H
#define B3_RAYTRACE_SCALING_H

#include "blz3/raytrace/b3BBox.h"

/**
 * This enumeration lists all scaling modes.
 */
enum b3_scaling_mode
{
	B3_SCALE_BOX_POLAR,       //!< Scale the hit bounding box polar coordinates (3d).
	B3_SCALE_IPOINT,          //!< Scale intersection point (3d).
	B3_SCALE_POLAR,           //!< Scale surface polar coordinates (2d).
	B3_SCALE_OBJECT_POLAR,    //!< Scale object relative polar coordinates (3d).
	B3_SCALE_IPOINT_ORIGINAL  //!< Scale intersection point inverse transformed by the collected transformations in the bounding box (3d).
};

typedef b3_vector64 * (*b3_scale_method)(b3_ray * ray);

/**
 * This class provides scaling of any form of the intersection point to a point
 * used for different procedural mappings such as materials or bump mapping.
 */
class B3_PLUGIN b3Scaling
{
	b3_scale_method  m_ScaleMethod;

public:
	b3_vector        m_Scale;      //!< The scaling factor in three coordinates.
	b3_scaling_mode  m_ScaleFlags; //!< Which value to scale.

public:
	/**
	 * This constructor initializes this scaling instance with default values.
	 */
	b3Scaling();

	/**
	 * This initializes scaling with three scaling factors and a scaling mode.
	 *
	 * @param scale The scaling factors as vector.
	 * @param mode  The scaling mode.
	 */
	void            b3InitScaling(b3_vector * scale, b3_scaling_mode mode);

	/**
	 * This initializes scaling with the same scaling factor for all three directions
	 * and a scaling mode.
	 *
	 * @param a     The scaling factor in all directions.
	 * @param mode  The scaling mode.
	 */
	void            b3InitScaling(b3_f64 a, b3_scaling_mode mode);

	/**
	 * This initializes scaling with three independent scaling factors
	 * and a scaling mode.
	 *
	 * @param x     The x scaling factor.
	 * @param y     The y scaling factor.
	 * @param z     The z scaling factor.
	 * @param mode  The scaling mode.
	 */
	void            b3InitScaling(b3_f64 x, b3_f64 y, b3_f64 z, b3_scaling_mode mode);

	/**
	 * This method setups a new scaling mode.
	 *
	 * @param mode The new scaling mode.
	 */
	void            b3SetScaleMode(int mode);

	/**
	 * This method setups a new scaling mode.
	 *
	 * @param mode The new scaling mode.
	 */
	void            b3SetScaleMode(b3_scaling_mode mode);

	/**
	 * This method returns the scaling mode.
	 *
	 * @return The used scaling mode.
	 */
	b3_scaling_mode b3GetScaleMode();

	/**
	 * This prepares the scaling function.
	 */
	void            b3PrepareScaling();

	/**
	 * This method scales a point with a given scale factor. The internal
	 * scale mode determines the point source to scale. If the scale input
	 * is null this parameter is not used.
	 *
	 * @param ray The structure which contain the scale source.
	 * @param scale An optional scale vector.
	 * @param point The resulting scaled point.
	 */
	inline void b3Scale(b3_ray * ray, b3_vector * scale, b3_vector * point)
	{
		b3_vector64 * polar = m_ScaleMethod(ray);

		if(scale == null)
		{
			point->x = polar->x;
			point->y = polar->y;
			point->z = polar->z;
		}
		else
		{
			point->x = polar->x * scale->x;
			point->y = polar->y * scale->y;
			point->z = polar->z * scale->z;
		}
	}

private:
	static b3_vector64 * b3ScaleBoxPolar(b3_ray * ray);
	static b3_vector64 * b3ScaleIPoint(b3_ray * ray);
	static b3_vector64 * b3ScalePolar(b3_ray * ray);
	static b3_vector64 * b3ScaleObjectPolar(b3_ray * ray);
	static b3_vector64 * b3ScaleIPointOriginal(b3_ray * ray);
};

#endif
