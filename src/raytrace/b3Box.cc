/*
**
**      $Filename:      b3Box.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing box
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
**      Revision 1.4  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.3  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.2  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
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

b3Box::b3Box(b3_u32 class_type) : b3Shape3(sizeof(b3Box),class_type)
{
}

b3Box::b3Box(b3_u32 *src) : b3Shape3(src)
{
}

void b3Box::b3AllocVertices(b3RenderContext *context)
{
	glVertices = box_vertices;
	glNormals  = box_normals;

	VertexCount = 24;
	GridCount   = 12;
	PolyCount   = 12;
}

void b3Box::b3FreeVertices()
{
	glVertices = null;
	glNormals  = null;
	glGrids    = null;
	glPolygons = null;
	b3Shape::b3FreeVertices();
}

void b3Box::b3ComputeVertices()
{
	b3ComputeBoxVertices(Base,Dir1,Dir2,Dir3);
}

void b3Box::b3ComputeIndices()
{
	b3ComputeBoxIndices();
}

void b3Box::b3Intersect()
{
}
