/*
**
**      $Filename:      b3Area.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing area
**
**      (C) Copyright 2001  Steffen A. Mork
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

#include "blz3/raytrace/b3Base.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

b3_vector b3Scaling::m_Unit =
{
	1.0,1.0,1.0
};

void b3Scaling::b3ScaleBBox(b3_ray *ray,b3_vector *scale,b3_vector *point)
{
	point->x = ray->polar.m_BoxPolar.x * scale->x;
	point->y = ray->polar.m_BoxPolar.y * scale->y;
	point->z = ray->polar.m_BoxPolar.z * scale->z;
}

void b3Scaling::b3ScaleIPoint(b3_ray *ray,b3_vector *scale,b3_vector *point)
{
	point->x = ray->ipoint.x * scale->x;
	point->y = ray->ipoint.y * scale->y;
	point->z = ray->ipoint.z * scale->z;
}

void b3Scaling::b3ScalePolar(b3_ray *ray,b3_vector *scale,b3_vector *point)
{
	point->x = ray->polar.m_Polar.x * scale->x;
	point->y = ray->polar.m_Polar.y * scale->y;
	point->z = ray->polar.m_Polar.z * scale->z;
}

void b3Scaling::b3ScaleObject(b3_ray *ray,b3_vector *scale,b3_vector *point)
{
	point->x = ray->polar.m_ObjectPolar.x * scale->x;
	point->y = ray->polar.m_ObjectPolar.y * scale->y;
	point->z = ray->polar.m_ObjectPolar.z * scale->z;
}
