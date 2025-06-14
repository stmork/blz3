/*
**
**	$Filename:	b3SplineCylinder.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing BSpline cylinder
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

#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineCylinder::b3SplineCylinder(const b3_u32 class_type) :
	b3SplineShape(sizeof(b3SplineCylinder), class_type)
{
}

b3SplineCylinder::b3SplineCylinder(const b3_u32 * src) : b3SplineShape(src)
{
}
