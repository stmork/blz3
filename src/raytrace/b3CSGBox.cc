/*
**
**      $Filename:      b3CSGBox.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG box
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
**      Revision 1.9  2002/02/17 21:25:06  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.8  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.7  2001/09/22 16:19:51  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.6  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.5  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
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

b3CSGBox::b3CSGBox(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGBox),class_type)
{
}

b3CSGBox::b3CSGBox(b3_u32 *src) : b3CSGShape3(src)
{
}

void b3CSGBox::b3AllocVertices(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	glVertices = box_vertices;
	glNormals  = box_normals;

	glVertexCount = 24;
	glGridCount   = 12;
	glPolyCount   = 12;
#endif
}

void b3CSGBox::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	glVertices = null;
	glNormals  = null;
	glGrids    = null;
	glPolygons = null;
#endif
	b3FreeVertices();
}

void b3CSGBox::b3ComputeVertices()
{
	b3ComputeBoxVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3CSGBox::b3ComputeIndices()
{
	b3ComputeBoxIndices();
}

void b3CSGBox::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
	b3_polar  *polar  = &ray->polar;
	b3_line64 *BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;
	b3_f64     x,y;

	x = BTLine->pos.x + Q * BTLine->dir.x;
	y = BTLine->pos.y + Q * BTLine->dir.y;
	if (x < 0)               x = 0;
	else if (x >= 0.9999999) x = 0.9999999;
	if (y < 0)               y = 0;
	else if (y >= 0.9999999) y = 0.9999999;

	polar->polar.x = polar->object_polar.x = x;
	polar->polar.y = polar->object_polar.y = y;
	polar->polar.z = polar->object_polar.z = BTLine->pos.z + Q * BTLine->dir.z;
	polar->normal_index = point->m_Index;
}
