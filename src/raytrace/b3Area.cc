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
**      Revision 1.11  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.10  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
**
**      Revision 1.9  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.8  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.7  2001/08/09 15:27:34  sm
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

#ifdef BLZ3_USE_OPENGL
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
#endif

b3Area::b3Area(b3_u32 class_type) : b3Shape2(sizeof(b3Area),class_type)
{
}

b3Area::b3Area(b3_u32 *src) : b3Shape2(src)
{
	glSolid = true;
}

void b3Area::b3AllocVertices(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	glVertices = area_vertices;
	glNormals  = area_normals;
	glGrids    = area_grids;
	glPolygons = area_polygons;

	VertexCount = 4;
	GridCount   = 4;
	PolyCount   = 2;
#endif
}

void b3Area::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	glVertices = null;
	glNormals  = null;
	glGrids    = null;
	glPolygons = null;
#endif
	b3Shape::b3FreeVertices();
}

void b3Area::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	GLfloat     *Vector;
	b3_f32       x1,y1,x2,y2;

	Vector = glVertices;
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
#endif
}

void b3Area::b3ComputeIndices()
{
	b3ComputeBound(&Limit);
}

void b3Area::b3Intersect()
{
}
