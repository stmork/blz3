/*
**
**	$Filename:	b3SplineRing.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing BSpline ring
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

b3SplineRing::b3SplineRing(const b3_u32 class_type) : b3SplineShape(sizeof(b3SplineRing), class_type)
{
}

b3SplineRing::b3SplineRing(const b3_u32 * src) : b3SplineShape(src)
{
}
