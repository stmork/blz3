/*
**
**      $Filename:      b3NormalDeriv.cc $
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
**	Revision 1.1  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_bool b3ShapeBaseTrans::b3NormalDeriv(b3_ray *ray)
{
	b3_vector64 EquaNormal;

	b3Vector::b3CrossProduct(&m_Dir1,&m_Dir2,&EquaNormal);
	b3Vector::b3CrossProduct(&EquaNormal,&ray->normal,&ray->xDeriv);
	if ((EquaNormal.x * m_Dir3.x +
	     EquaNormal.y * m_Dir3.y +
	     EquaNormal.z * m_Dir3.z) < 0)
	{
		b3Vector::b3CrossProduct(&ray->xDeriv,&EquaNormal,&ray->yDeriv);
	}
	else
	{
		b3Vector::b3CrossProduct(&EquaNormal,&ray->xDeriv,&ray->yDeriv);
	}
	return true;
}

b3_bool b3TriangleShape::b3NormalDeriv(b3_ray *ray)
{
	b3_vertex   *V1,*V2,*V3;
	b3_triangle *Triangle;

	Triangle = &m_Triangles[ray->TriaIndex];
	V1       = &m_Vertices[Triangle->P1];
	V2       = &m_Vertices[Triangle->P2];
	V3       = &m_Vertices[Triangle->P3];

	if (ray->TriaIndex & 1)			/* odd triangle number */
	{
		// Odd triangle
		ray->xDeriv.x = V1->Point.x - V2->Point.x;
		ray->xDeriv.y = V1->Point.y - V2->Point.y;
		ray->xDeriv.z = V1->Point.z - V2->Point.z;

		ray->yDeriv.x = V1->Point.x - V3->Point.x;
		ray->yDeriv.y = V1->Point.y - V3->Point.y;
		ray->yDeriv.z = V1->Point.z - V3->Point.z;
	}
	else
	{
		// Even triangle
		ray->xDeriv.x = V2->Point.x - V1->Point.x;
		ray->xDeriv.y = V2->Point.y - V1->Point.y;
		ray->xDeriv.z = V2->Point.z - V1->Point.z;

		ray->yDeriv.x = V3->Point.x - V1->Point.x;
		ray->yDeriv.y = V3->Point.y - V1->Point.y;
		ray->yDeriv.z = V3->Point.z - V1->Point.z;
	}
	return true;
}

b3_bool b3Shape2::b3NormalDeriv(b3_ray *ray)
{
	ray->xDeriv.x = m_Dir1.x;
	ray->xDeriv.y = m_Dir1.y;
	ray->xDeriv.z = m_Dir1.z;

	ray->yDeriv.x = m_Dir2.x;
	ray->yDeriv.y = m_Dir2.y;
	ray->yDeriv.z = m_Dir2.z;

	return true;
}
