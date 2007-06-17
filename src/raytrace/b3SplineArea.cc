/*
**
**	$Filename:	b3SplineArea.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineArea::b3SplineArea(b3_u32 class_type) : b3SplineShape(sizeof(b3SplineArea),class_type)
{
}

b3SplineArea::b3SplineArea(b3_u32 *src) : b3SplineShape(src)
{
}
