/*
**
**	$Filename:	b3Scaling.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
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

#ifndef B3_RAYTRACE_SCALING_H
#define B3_RAYTRACE_SCALING_H

#include "blz3/raytrace/b3BBox.h"

enum b3_scaling_mode
{
	B3_SCALE_BOX_POLAR,
	B3_SCALE_IPOINT,
	B3_SCALE_POLAR,
	B3_SCALE_OBJECT_POLAR,
	B3_SCALE_IPOINT_ORIGINAL
};

typedef b3_vector64 * (*b3_scale_method)(b3_ray *ray);

class b3Scaling
{
	b3_scale_method  m_ScaleMethod;

public:
	b3_vector        m_Scale;
	b3_scaling_mode  m_ScaleFlags;

public:
	                b3Scaling();
	void            b3InitScaling(b3_vector *scale,b3_scaling_mode mode);
	void            b3InitScaling(b3_f64 a,b3_scaling_mode mode);
	void            b3InitScaling(b3_f64 x,b3_f64 y,b3_f64 z,b3_scaling_mode mode);
	void            b3SetScaleMode(int mode);
	void            b3SetScaleMode(b3_scaling_mode mode);
	b3_scaling_mode b3GetScaleMode();
	void            b3PrepareScaling();

	inline void b3Scale(b3_ray *ray,b3_vector *scale,b3_vector *point)
	{
		b3_vector64 *polar = m_ScaleMethod(ray);

		if (scale == null)
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
	static b3_vector64 *b3ScaleBoxPolar(b3_ray *ray);
	static b3_vector64 *b3ScaleIPoint(b3_ray *ray);
	static b3_vector64 *b3ScalePolar(b3_ray *ray);
	static b3_vector64 *b3ScaleObjectPolar(b3_ray *ray);
	static b3_vector64 *b3ScaleIPointOriginal(b3_ray *ray);
};

#endif
