/*
**
**      $Filename:      b3CSGEllipsoid.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG ellipsoid
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
**      Revision 1.2  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
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

b3CSGEllipsoid::b3CSGEllipsoid(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGEllipsoid),class_type)
{
}

b3CSGEllipsoid::b3CSGEllipsoid(b3_u32 *src) : b3CSGShape3(src)
{
}

void b3CSGEllipsoid::b3ComputeVertices()
{
}

void b3CSGEllipsoid::b3ComputeIndices()
{
}

void b3CSGEllipsoid::b3Intersect()
{
}
