/*
**
**	$Filename:	b3SplineArea.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing BSpline area
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

b3SplineArea::b3SplineArea(const b3_u32 class_type) : b3SplineShape(sizeof(b3SplineArea), class_type)
{
}

b3SplineArea::b3SplineArea(const b3_u32 * src) : b3SplineShape(src)
{
}
