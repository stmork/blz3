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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineCylinder::b3SplineCylinder(b3_u32 class_type) :
	b3SplineShape(sizeof(b3SplineCylinder), class_type)
{
}

b3SplineCylinder::b3SplineCylinder(b3_u32 * src) : b3SplineShape(src)
{
}
