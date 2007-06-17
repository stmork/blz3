/*
**
**	$Filename:	b3SplineRing.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineRing::b3SplineRing(b3_u32 class_type) : b3SplineShape(sizeof(b3SplineRing),class_type)
{
}

b3SplineRing::b3SplineRing(b3_u32 *src) : b3SplineShape(src)
{
}
