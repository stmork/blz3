/*
**
**      $Filename:      b3SplineArea.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing BSpline area
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.1  2001/08/06 15:26:00  sm
**      - Splitted shapes into their own files
**      - Some preparations for shapde drawing.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineArea::b3SplineArea(b3_u32 class_type) : b3SplineShape(class_type)
{
}

b3SplineArea::b3SplineArea(b3_u32 *src) : b3SplineShape(src)
{
}

void b3SplineArea::b3ComputeVertices()
{
}

void b3SplineArea::b3ComputeIndices()
{
}

void b3SplineArea::b3Intersect()
{
}
