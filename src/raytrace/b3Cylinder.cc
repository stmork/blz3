/*
**
**      $Filename:      b3Cylinder.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing cylinder
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

b3Cylinder::b3Cylinder(b3_u32 class_type) : b3Shape3(class_type)
{
}

b3Cylinder::b3Cylinder(b3_u32 *src) : b3Shape3(src)
{
}

void b3Cylinder::b3ComputeVertices()
{
}

void b3Cylinder::b3ComputeIndices()
{
}

void b3Cylinder::b3Intersect()
{
}
