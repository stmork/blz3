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

b3Area::b3Area(b3_u32 class_type) : b3Shape2(class_type)
{
}

b3Area::b3Area(b3_u32 *src) : b3Shape2(src)
{
}

void b3Area::b3AllocVertices()
{
	Vertices = area_vertices;
	Grids    = area_grids;
	Polygons = area_polygons;

	VertexCount = 4;
	GridCount   = 4;
	PolyCount   = 2;
}

void b3Area::b3FreeVertices()
{
	Vertices = null;
	Grids    = null;
	Polygons = null;
	b3Shape::b3FreeVertices();
}

void b3Area::b3ComputeVertices()
{
	GLfloat     *Vector;
	b3CondLimit  limit;
	b3_f32       x1,y1,x2,y2;

	Vector = Vertices;
	b3ComputeBound(&limit);
	x1     = limit.x1;
	y1     = limit.y1;
	x2     = limit.x2;
	y2     = limit.y2;

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
