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
**      Revision 1.3  2001/08/09 15:27:34  sm
**      - Following shapes are newly supported now:
**        o disk
**        o cylinder
**        o cone
**        o ellipsoid
**        o torus
**        o triangles
**      - Done some makefile fixes
**      - Everything is Windozable
**
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
	b3_index i;

	b3InitNOP();
	b3InitVector(&Base);
	b3InitVector(&Size);
	GridSize    = b3InitInt();
	TriaCount   = b3InitInt() << 1;
	VertexCount = b3InitInt();
	xSize       = b3InitInt();
	ySize       = b3InitInt();
	Flags       = b3InitInt();
	b3InitNOP();
	b3InitNOP();
	b3InitNOP();

	// FIX ME: We have to convert the indices and vertices to
	//         and have to initialize the grid!
	vertices  = (b3_vertex *)b3Item::b3Alloc(VertexCount * sizeof(b3_vertex));
	triangles = (b3_triangle *)b3Item::b3Alloc(TriaCount * sizeof(b3_triangle));

	for (i = 0;i < VertexCount;i++)
	{
		vertices[i].x  = b3InitFloat();
		vertices[i].y  = b3InitFloat();
		vertices[i].z  = b3InitFloat();
		vertices[i].nx = b3InitFloat();
		vertices[i].ny = b3InitFloat();
		vertices[i].nz = b3InitFloat();
	}

	for (i = 0;i < TriaCount;i++)
	{
		triangles[i].P1 = b3InitInt();
		triangles[i].P2 = b3InitInt();
		triangles[i].P3 = b3InitInt();
	}
}

void b3TriangleShape::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = context->b3GetSubdiv();

	vertCount = VertexCount;
	gridCount = TriaCount * 3;
	polyCount = TriaCount;
}

void b3TriangleShape::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_vertex *Vertex;
	b3_index   i;

	Vertex   = (b3_vertex *)vertices;
	Vector   = (b3_vector *)Vertices;

	for (i = 0;i < VertexCount;i++)
	{
		Vector->x = Vertex->x;
		Vector->y = Vertex->y;
		Vector->z = Vertex->z;
		Vertex++;
		Vector++;
	}
}

void b3TriangleShape::b3ComputeIndices()
{
	GLushort    *Index;
	b3_triangle *Triangle;
	b3_vertex   *Vertex;
	b3_count     i;

	Index    = Grids;
	Vertex   = vertices;
	Triangle = triangles;
	for (i = 0;i < TriaCount;i++)
	{
		*Index++ = (short)Triangle->P1;
		*Index++ = (short)Triangle->P2;

		*Index++ = (short)Triangle->P2;
		*Index++ = (short)Triangle->P3;

		*Index++ = (short)Triangle->P3;
		*Index++ = (short)Triangle->P1;
		Triangle++;
	}
}

void b3TriangleShape::b3Intersect()
{
}
