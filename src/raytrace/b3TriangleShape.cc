/*
**
**      $Filename:      b3TriangleShape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing triangle shape
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

b3TriangleShape::b3TriangleShape(b3_u32 class_type) : b3Shape(sizeof(b3TriangleShape), class_type)
{
}

b3TriangleShape::b3TriangleShape(b3_u32 *src) : b3Shape(src)
{
	b3InitNOP();
	b3InitVector(&Base);
	b3InitVector(&Size);
	GridSize  = b3InitInt();
	Triangles = b3InitInt();
	Vertices  = b3InitInt();
	xSize     = b3InitInt();
	ySize     = b3InitInt();

	// FIX ME: We have to convert the indices and vertices to
	//         and have to initialize the grid!
}

void b3TriangleShape::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = context->b3GetSubdiv();

	vertCount = Vertices;
	gridCount = Triangles * 3;
	polyCount = Triangles;
}

void b3TriangleShape::b3ComputeVertices()
{
}

void b3TriangleShape::b3ComputeIndices()
{
}

void b3TriangleShape::b3Intersect()
{
}
