/*
**
**	$Filename:	b3Scaling.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Scaling.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**
**	Revision 1.1  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Scaling::b3Scaling()
{
	b3InitScaling(20.0,20.0,20.0,B3_SCALE_BOX_POLAR);
}

void b3Scaling::b3InitScaling(b3_vector *scale,b3_scaling_mode mode)
{
	m_Scale = *scale;
	b3SetScaleMode(mode);
	b3PrepareScaling();
}

void b3Scaling::b3InitScaling(b3_f64 x,b3_f64 y,b3_f64 z,b3_scaling_mode mode)
{
	b3Vector::b3Init(&m_Scale,x,y,z);
	b3SetScaleMode(mode);
	b3PrepareScaling();
}

void b3Scaling::b3InitScaling(b3_f64 a,b3_scaling_mode mode)
{
	b3Vector::b3Init(&m_Scale,a,a,a);
	b3SetScaleMode(mode);
	b3PrepareScaling();
}

void b3Scaling::b3SetScaleMode(int mode)
{
	m_ScaleFlags = (b3_scaling_mode)mode;
}

void b3Scaling::b3SetScaleMode(b3_scaling_mode mode)
{
	m_ScaleFlags = mode;
}

b3_scaling_mode b3Scaling::b3GetScaleMode()
{
	return m_ScaleFlags;
}

void b3Scaling::b3PrepareScaling()
{
	switch(b3GetScaleMode())
	{
	case B3_SCALE_BOX_POLAR:
		m_ScaleMethod = b3ScaleBoxPolar;
		break;

	case B3_SCALE_IPOINT:
		m_ScaleMethod = b3ScaleIPoint;
		break;

	case B3_SCALE_POLAR:
		m_ScaleMethod = b3ScalePolar;
		break;

	case B3_SCALE_OBJECT_POLAR:
		m_ScaleMethod = b3ScaleObjectPolar;
		break;

	case B3_SCALE_IPOINT_ORIGINAL:
		m_ScaleMethod = b3ScaleIPointOriginal;
		break;
	}
}

b3_vector64 * b3Scaling::b3ScaleBoxPolar(b3_ray *ray)
{
	return &ray->polar.m_BoxPolar;
}

b3_vector64 * b3Scaling::b3ScaleIPoint(b3_ray *ray)
{
	return &ray->ipoint;
}

b3_vector64 * b3Scaling::b3ScalePolar(b3_ray *ray)
{
	return &ray->polar.m_Polar;
}

b3_vector64 * b3Scaling::b3ScaleObjectPolar(b3_ray *ray)
{
	return &ray->polar.m_ObjectPolar;
}

b3_vector64 * b3Scaling::b3ScaleIPointOriginal(b3_ray *ray)
{
	return &ray->polar.m_BBoxOriginal;
}
