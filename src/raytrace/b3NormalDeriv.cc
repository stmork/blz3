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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.10  2006/03/05 21:22:35  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.9  2005/06/06 19:56:22  sm
**	- Some optimizations.
**	
**	Revision 1.8  2005/06/06 14:59:41  smork
**	- More vectorization- More vectorization.
**	
**	Revision 1.7  2005/06/01 12:28:55  smork
**	- Removed some floating point operations.
**	
**	Revision 1.6  2005/04/27 13:55:02  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**	
**	Revision 1.5  2005/01/06 10:02:37  smork
**	- Spline animation fix.
**	
**	Revision 1.4  2005/01/03 10:34:30  smork
**	- Rebalanced some floating point comparisons:
**	  a == 0  -> b3Math::b3NearZero
**	  a == b  -> b3Math::b3IsEqual
**	- Removed some very inlikely fp comparisons
**	  in intersection methods.
**	
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

b3_bool b3ShapeBaseTransformation::b3NormalDeriv(b3_ray *ray)
{
	b3_vector64 EquaNormal;

	b3Vector::b3CrossProduct(&m_Dir1,&m_Dir2,&EquaNormal);
	b3Vector::b3CrossProduct(&EquaNormal,&ray->normal,&ray->xDeriv);
	if (b3Vector::b3SMul(&EquaNormal, &m_Dir3) < 0)
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
	b3Vector::b3Init(&ray->xDeriv, &m_Dir1);
	b3Vector::b3Init(&ray->yDeriv, &m_Dir2);

	return true;
}
