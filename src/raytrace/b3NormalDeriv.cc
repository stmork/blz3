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

#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**
**	Revision 1.2  2004/04/12 15:41:50  sm
**	- Right computation of normal derivation.
**	
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

b3_bool b3Shape::b3NormalDeriv(b3_ray *ray)
{
	b3Vector::b3CrossProduct(&ray->dir,&ray->normal,&ray->xDeriv);
	b3Vector::b3CrossProduct(&ray->normal,&ray->xDeriv,&ray->yDeriv);
	return true;
}

b3_bool b3Sphere::b3NormalDeriv(b3_ray *ray)
{
	if ((ray->normal.x == 0) && (ray->normal.y == 0))
	{
		// Gimble lock
		return b3Shape::b3NormalDeriv(ray);
	}
	else
	{
		ray->xDeriv.x = -ray->normal.y;
		ray->xDeriv.y =  ray->normal.x;
		ray->xDeriv.z =  0;
		b3Vector::b3CrossProduct(&ray->normal,&ray->xDeriv,&ray->yDeriv);
	}
	return true;
}

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
