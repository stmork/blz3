/*
**
**      $Filename:      b3CSGSphere.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG sphere
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
**      Revision 1.2  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
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

b3CSGSphere::b3CSGSphere(b3_u32 class_type) : b3Shape(sizeof(b3CSGSphere), class_type)
{
}

b3CSGSphere::b3CSGSphere(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&Base);
	b3InitVector(&Dir);

	b3InitFloat(); // This is QuadRadius
	b3InitInt();   // This is Index

	Operation = b3InitInt();
}

void b3CSGSphere::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = context->b3GetSubdiv();

	vertCount = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
}

void b3CSGSphere::b3ComputeVertices()
{
}

void b3CSGSphere::b3ComputeIndices()
{
}

void b3CSGSphere::b3Intersect()
{
}
