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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.18  2002/07/21 17:02:36  sm
**      - Finished advanced color mix support (correct Phong/Mork shading)
**      - Added first texture mapping support. Further development on
**        Windows now...
**
**      Revision 1.17  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.16  2002/02/27 20:14:51  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.15  2002/02/17 21:25:06  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.14  2001/09/22 16:19:51  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.13  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.12  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
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
}

void b3Area::b3AllocVertices(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	glVertices = area_vertices;
	glNormals  = area_normals;
	glTexCoord = area_texcoord;
	glGrids    = area_grids;
	glPolygons = area_polygons;

	glVertexCount = 4;
	glGridCount   = 4;
	glPolyCount   = 2;
#endif
}

void b3Area::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	glVertices = null;
	glNormals  = null;
	glTexCoord = null;
	glGrids    = null;
	glPolygons = null;
#endif
	b3FreeVertices();
}

void b3Area::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	GLfloat     *Vector;
	b3_f32       x1,y1,x2,y2;

	x1     = Limit.x1;
	y1     = Limit.y1;
	x2     = Limit.x2;
	y2     = Limit.y2;

	// Setup world coordinates
	Vector    = glVertices;
	*Vector++ = (GLfloat)(m_Base.x + x1 * m_Dir1.x + y1 * m_Dir2.x);
	*Vector++ = (GLfloat)(m_Base.y + x1 * m_Dir1.y + y1 * m_Dir2.y);
	*Vector++ = (GLfloat)(m_Base.z + x1 * m_Dir1.z + y1 * m_Dir2.z);
															 
	*Vector++ = (GLfloat)(m_Base.x + x1 * m_Dir1.x + y2 * m_Dir2.x);
	*Vector++ = (GLfloat)(m_Base.y + x1 * m_Dir1.y + y2 * m_Dir2.y);
	*Vector++ = (GLfloat)(m_Base.z + x1 * m_Dir1.z + y2 * m_Dir2.z);
															 
	*Vector++ = (GLfloat)(m_Base.x + x2 * m_Dir1.x + y2 * m_Dir2.x);
	*Vector++ = (GLfloat)(m_Base.y + x2 * m_Dir1.y + y2 * m_Dir2.y);
	*Vector++ = (GLfloat)(m_Base.z + x2 * m_Dir1.z + y2 * m_Dir2.z);
															 
	*Vector++ = (GLfloat)(m_Base.x + x2 * m_Dir1.x + y1 * m_Dir2.x);
	*Vector++ = (GLfloat)(m_Base.y + x2 * m_Dir1.y + y1 * m_Dir2.y);
	*Vector++ = (GLfloat)(m_Base.z + x2 * m_Dir1.z + y1 * m_Dir2.z);

	// Setup texture coordinates
	Vector    = glTexCoord;
	*Vector++ = 0;
	*Vector++ = 0;

	*Vector++ = 0;
	*Vector++ = 1;

	*Vector++ = 1;
	*Vector++ = 1;

	*Vector++ = 1;
	*Vector++ = 0;

	xSize = 1;
	ySize = 1;
#endif
}

void b3Area::b3ComputeIndices()
{
	b3ComputeBound(&Limit);
}

void b3Area::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xMin    = -1;
	info->xMax    =  1;
	info->yMin    = -1;
	info->yMax    =  1;
	info->xUnit   = B3_STENCIL_LENGTH;
	info->yUnit   = B3_STENCIL_LENGTH;
	info->xFactor = b3Vector::b3Length(&m_Dir1);
	info->yFactor = b3Vector::b3Length(&m_Dir2);;
}
