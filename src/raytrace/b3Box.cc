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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.14  2002/07/31 07:30:44  sm
**      - New normal computation. Textures are rendered correctly and
**        quadrics are shaded correctly. Spheres and doughnuts have
**        got their own more simple computation.
**
**      Revision 1.13  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.12  2002/07/22 10:52:16  sm
**      - Added correct chess support
**      - Added texture support for following shapes:
**        o Box
**        o Cone
**        o Spline shapes including rotation shapes
**
**      Revision 1.11  2002/07/21 17:02:36  sm
**      - Finished advanced color mix support (correct Phong/Mork shading)
**      - Added first texture mapping support. Further development on
**        Windows now...
**
**      Revision 1.10  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.9  2002/02/27 20:14:51  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.8  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
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

b3Box::b3Box(b3_u32 class_type) : b3Shape3(sizeof(b3Box),class_type)
{
}

b3Box::b3Box(b3_u32 *src) : b3Shape3(src)
{
}

void b3Box::b3AllocVertices(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	glVertex      = box_vertex;

	glVertexCount = 24;
	glGridCount   = 12;
	glPolyCount   = 12;

	memset(box_vertex,0,sizeof(box_vertex));
#endif
}

void b3Box::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	glVertex   = null;
	glGrids    = null;
	glPolygons = null;
#endif
	b3ShapeRenderObject::b3FreeVertices();
}

void b3Box::b3ComputeVertices()
{
	b3ComputeBoxVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3Box::b3ComputeNormals(b3_bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3Box::b3ComputeIndices()
{
	b3ComputeBoxIndices();
}

void b3Box::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xMin    = 0;
	info->xMax    = 1;
	info->yMin    = 0;
	info->yMax    = 1;
	info->xUnit   = B3_STENCIL_LENGTH;
	info->yUnit   = B3_STENCIL_LENGTH;
	info->xFactor = b3Vector::b3Length(&m_Dir1);
	info->yFactor = b3Vector::b3Length(&m_Dir2);
}
