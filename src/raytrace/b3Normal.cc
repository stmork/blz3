/*
**
**      $Filename:      b3Normal.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Normal computation routines
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Shape::b3Normal(b3_ray *ray)
{
}

void b3Sphere::b3Normal(b3_ray *ray)
{
	ray->normal.x = ray->ipoint.x - m_Base.x;
	ray->normal.y = ray->ipoint.y - m_Base.y;
	ray->normal.z = ray->ipoint.z - m_Base.z;
}

void b3Shape2::b3Normal(b3_ray *ray)
{
	ray->normal.x = m_Normal.x;
	ray->normal.y = m_Normal.y;
	ray->normal.z = m_Normal.z;
}

void b3Cylinder::b3Normal(b3_ray *ray)
{
	register double	x,y;

	x = m_DirLen[1] * ray->polar.object_polar.x;
	y = m_DirLen[0] * ray->polar.object_polar.y;
	ray->normal.x = x * m_Dir1.x + y * m_Dir2.x;
	ray->normal.y = x * m_Dir1.y + y * m_Dir2.y;
	ray->normal.z = x * m_Dir1.z + y * m_Dir2.z;
}

void b3Cone::b3Normal(b3_ray *ray)
{
	b3_f64     x,y,Factor;
	b3_dVector n1,z3;

	x     = ray->polar.object_polar.x;
	y     = ray->polar.object_polar.y;
	z3.x  = x * m_Dir1.x + y * m_Dir2.x;
	z3.y  = x * m_Dir1.y + y * m_Dir2.y;
	z3.z  = x * m_Dir1.z + y * m_Dir2.z;

	x    *= m_DirLen[1];
	y    *= m_DirLen[0];
	n1.x  = x * m_Dir1.x + y * m_Dir2.x;
	n1.y  = x * m_Dir1.y + y * m_Dir2.y;
	n1.z  = x * m_Dir1.z + y * m_Dir2.z;

	Factor = sqrt(
		(z3.x * z3.x + z3.y * z3.y + z3.z * z3.z) /
		(n1.x * n1.x + n1.y * n1.y + n1.z * n1.z));
	n1.x *= Factor;
	n1.y *= Factor;
	n1.z *= Factor;

	x = sqrt((n1.x * n1.x + n1.y * n1.y + n1.z * n1.z) / m_DirLen[2]);
	y = 1 / x;
	ray->normal.x = x * m_Dir3.x + y * n1.x;
	ray->normal.y = x * m_Dir3.y + y * n1.y;
	ray->normal.z = x * m_Dir3.z + y * n1.z;
}

void b3Ellipsoid::b3Normal(b3_ray *ray)
{
	register double	x,y,z;

	x = m_DirLen[1] * m_DirLen[2] * ray->polar.object_polar.x;
	y = m_DirLen[0] * m_DirLen[2] * ray->polar.object_polar.y;
	z = m_DirLen[0] * m_DirLen[1] * ray->polar.object_polar.z;
	ray->normal.x = x * m_Dir1.x + y * m_Dir2.x + z * m_Dir3.x;
	ray->normal.y = x * m_Dir1.y + y * m_Dir2.y + z * m_Dir3.y;
	ray->normal.z = x * m_Dir1.z + y * m_Dir2.z + z * m_Dir3.z;
}

void b3Box::b3Normal(b3_ray *ray)
{
	ray->normal.x = m_Normals[ray->polar.normal_index].x;
	ray->normal.y = m_Normals[ray->polar.normal_index].y;
	ray->normal.z = m_Normals[ray->polar.normal_index].z;
}

void b3Torus::b3Normal(b3_ray *ray)
{
	b3_f64 Dist,x,y,z;

	x     = ray->polar.object_polar.x;
	y     = ray->polar.object_polar.y;
	z     = ray->polar.object_polar.z;

	Dist  = sqrt (x * x + y * y);
	x    -= (m_aRad * x / Dist);
	y    -= (m_aRad * y / Dist);

	ray->normal.x  = x * m_Dir1.x + y * m_Dir2.x + z * m_Dir3.x;
	ray->normal.y  = x * m_Dir1.y + y * m_Dir2.y + z * m_Dir3.y;
	ray->normal.z  = x * m_Dir1.z + y * m_Dir2.z + z * m_Dir3.z;
}
