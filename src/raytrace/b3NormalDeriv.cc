/*
**
**	$Filename:	b3NormalDeriv.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Normal computation routines
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

bool b3Shape::b3NormalDeriv(b3_ray * ray) const
{
	b3Vector::b3CrossProduct(&ray->dir, &ray->normal, &ray->xDeriv);
	b3Vector::b3CrossProduct(&ray->normal, &ray->xDeriv, &ray->yDeriv);
	return true;
}

bool b3Sphere::b3NormalDeriv(b3_ray * ray) const
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
		b3Vector::b3CrossProduct(&ray->normal, &ray->xDeriv, &ray->yDeriv);
	}
	return true;
}

bool b3ShapeBaseTransformation::b3NormalDeriv(b3_ray * ray) const
{
	b3_vector64 EquaNormal;

	b3Vector::b3CrossProduct(&m_Dir1, &m_Dir2, &EquaNormal);
	b3Vector::b3CrossProduct(&EquaNormal, &ray->normal, &ray->xDeriv);
	if (b3Vector::b3SMul(&EquaNormal, &m_Dir3) < 0)
	{
		b3Vector::b3CrossProduct(&ray->xDeriv, &EquaNormal, &ray->yDeriv);
	}
	else
	{
		b3Vector::b3CrossProduct(&EquaNormal, &ray->xDeriv, &ray->yDeriv);
	}
	return true;
}

bool b3Shape2::b3NormalDeriv(b3_ray * ray) const
{
	b3Vector::b3Init(&ray->xDeriv, &m_Dir1);
	b3Vector::b3Init(&ray->yDeriv, &m_Dir2);

	return true;
}
