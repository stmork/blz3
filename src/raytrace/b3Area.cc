/*
**
**      $Filename:      b3Area.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing area
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
**      Revision 1.6  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.5  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.4  2001/08/06 20:49:39  sm
**      - Some conflicts solved
**
**      Revision 1.3  2001/08/06 19:58:58  sm
**      - Drawing area - the first shape we can see with OpenGL
**
**      Revision 1.2  2001/08/06 16:35:35  sm
**      - Drawing first area
**
**      Revision 1.1  2001/08/06 15:25:59  sm
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

static GLushort area_grids[] =
{
	0,1,
	1,2,
	2,3,
	3,0
};

static GLushort area_polygons[] =
{
	0,1,3,
	2,3,1
};

b3Area::b3Area(b3_u32 class_type) : b3Shape2(sizeof(b3Area),class_type)
{
}

b3Area::b3Area(b3_u32 *src) : b3Shape2(src)
{
}

void b3Area::b3AllocVertices(b3RenderContext *context)
{
	Vertices = area_vertices;
	Normals  = area_normals;
	Grids    = area_grids;
	Polygons = area_polygons;

	VertexCount = 4;
	GridCount   = 4;
	PolyCount   = 2;
}

void b3Area::b3FreeVertices()
{
	Vertices = null;
	Normals  = null;
	Grids    = null;
	Polygons = null;
	b3Shape::b3FreeVertices();
}

void b3Area::b3ComputeVertices()
{
	GLfloat     *Vector;
	b3_f32       x1,y1,x2,y2;

	Vector = Vertices;
	b3ComputeBound(&Limit);
	x1     = Limit.x1;
	y1     = Limit.y1;
	x2     = Limit.x2;
	y2     = Limit.y2;

	*Vector++ = (GLfloat)(Base.x + x1 * Dir1.x + y1 * Dir2.x);
	*Vector++ = (GLfloat)(Base.y + x1 * Dir1.y + y1 * Dir2.y);
	*Vector++ = (GLfloat)(Base.z + x1 * Dir1.z + y1 * Dir2.z);
															 
	*Vector++ = (GLfloat)(Base.x + x1 * Dir1.x + y2 * Dir2.x);
	*Vector++ = (GLfloat)(Base.y + x1 * Dir1.y + y2 * Dir2.y);
	*Vector++ = (GLfloat)(Base.z + x1 * Dir1.z + y2 * Dir2.z);
															 
	*Vector++ = (GLfloat)(Base.x + x2 * Dir1.x + y2 * Dir2.x);
	*Vector++ = (GLfloat)(Base.y + x2 * Dir1.y + y2 * Dir2.y);
	*Vector++ = (GLfloat)(Base.z + x2 * Dir1.z + y2 * Dir2.z);
															 
	*Vector++ = (GLfloat)(Base.x + x2 * Dir1.x + y1 * Dir2.x);
	*Vector++ = (GLfloat)(Base.y + x2 * Dir1.y + y1 * Dir2.y);
	*Vector++ = (GLfloat)(Base.z + x2 * Dir1.z + y1 * Dir2.z);

	xSize = 1;
	ySize = 1;
}

void b3Area::b3ComputeIndices()
{
}

void b3Area::b3Intersect()
{
}
