/*
**
**      $Filename:      b3Triangles.cc $
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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.5  2001/12/30 16:54:15  sm
**      - Inserted safe b3Write() into Lines III
**      - Fixed b3World saving: b3StoreXXX() methods must ensure
**        buffer before the buffer itself is used.
**      - Extended b3Shape format with shape activation flag. Nice: The
**        new data structures don't confuse the old Lines II/Blizzard II and
**        even stores these new values.
**
**      Revision 1.4  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.3  2001/10/18 14:48:26  sm
**      - Fixing refracting problem on some scenes with glasses.
**      - Fixing overlighting problem when using Mork shading.
**      - Finxing some memory leaks (espacially b3TriangleRefs)
**      - Adding texture support to conditions (stencil mapping).
**        Now conditions are ready to work compatible with
**        Blizzard II.
**
**      Revision 1.2  2001/10/17 21:09:06  sm
**      - Triangle support added for intersections, normal computations. So
**        Spline shapes can be computed, too. Now only CSG is missing.
**
**      Revision 1.1  2001/10/17 14:46:02  sm
**      - Adding triangle support.
**      - Renaming b3TriangleShape into b3Triangles and introducing
**        new b3TriangleShape as base class. This results in
**        source file renaming, too.
**      - Fixing soft shadow bug.
**      - Only scene loading background image when activated.
**      - Fixing LDC spline initialization.
**      - Converting Windows paths into right paths on Un*x
**
**      Revision 1.11  2001/09/22 16:19:53  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.10  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.9  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.8  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.7  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.6  2001/08/11 16:29:08  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.5  2001/08/11 15:59:59  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.4  2001/08/10 15:14:37  sm
**      - Now having all shapes implemented for drawing lines.
**
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

b3Triangles::b3Triangles(b3_u32 class_type) :
	b3TriangleShape(class_type)
{
}

b3Triangles::b3Triangles(b3_u32 *src) : b3TriangleShape(src)
{
	b3_index i;

	b3InitNOP();
	b3InitVector(&m_Base);
	b3InitVector(&m_Size);
	m_GridSize    = b3InitInt();
	m_TriaCount   = b3InitInt();
	m_VertexCount = b3InitInt();
	m_xSize       = b3InitInt();
	m_ySize       = b3InitInt();
	m_Flags       = b3InitInt();
	b3InitNOP();
	b3InitNOP();
	b3InitNOP();

	m_Vertices  = (b3_vertex *)b3Item::b3Alloc(m_VertexCount * sizeof(b3_vertex));
	m_Triangles = (b3_triangle *)b3Item::b3Alloc(m_TriaCount * sizeof(b3_triangle));

	for (i = 0;i < m_VertexCount;i++)
	{
		m_Vertices[i].Point.x  = b3InitFloat();
		m_Vertices[i].Point.y  = b3InitFloat();
		m_Vertices[i].Point.z  = b3InitFloat();
		m_Vertices[i].Normal.x = b3InitFloat();
		m_Vertices[i].Normal.y = b3InitFloat();
		m_Vertices[i].Normal.z = b3InitFloat();
	}

	for (i = 0;i < m_TriaCount;i++)
	{
		m_Triangles[i].P1       = b3InitInt();
		m_Triangles[i].P2       = b3InitInt();
		m_Triangles[i].P3       = b3InitInt();
		m_Triangles[i].Normal.x = b3InitFloat();
		m_Triangles[i].Normal.y = b3InitFloat();
		m_Triangles[i].Normal.z = b3InitFloat();
	}
	b3InitActivation();
}

void b3Triangles::b3StoreShape()
{
	b3_index i;

	b3StoreNull(); // This is m_GridList
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Size);
	b3StoreRes(m_GridSize);
	b3StoreCount(m_TriaCount);
	b3StoreCount(m_VertexCount);
	b3StoreRes(m_xSize);
	b3StoreRes(m_ySize);
	b3StoreInt(m_Flags);
	b3StoreNOP(); // This is IndexHit
	b3StoreNOP(); // This is aValue
	b3StoreNOP(); // This is bValue

	for (i = 0;i < m_VertexCount;i++)
	{
		b3StoreFloat(m_Vertices[i].Point.x);
		b3StoreFloat(m_Vertices[i].Point.y);
		b3StoreFloat(m_Vertices[i].Point.z);
		b3StoreFloat(m_Vertices[i].Normal.x);
		b3StoreFloat(m_Vertices[i].Normal.y);
		b3StoreFloat(m_Vertices[i].Normal.z);
	}

	for (i = 0;i < m_TriaCount;i++)
	{
		b3StoreIndex(m_Triangles[i].P1);
		b3StoreIndex(m_Triangles[i].P2);
		b3StoreIndex(m_Triangles[i].P3);
		b3StoreFloat(m_Triangles[i].Normal.x);
		b3StoreFloat(m_Triangles[i].Normal.y);
		b3StoreFloat(m_Triangles[i].Normal.z);
	}
}

void b3Triangles::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = m_VertexCount;
	gridCount = m_TriaCount * 3;
	polyCount = m_TriaCount;
}

void b3Triangles::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *Vector;
	b3_vertex *Vertex;
	b3_index   i;

	Vertex   = (b3_vertex *)m_Vertices;
	Vector   = (b3_vector *)glVertices;

	glVertexCount = m_VertexCount;
	for (i = 0;i < m_VertexCount;i++)
	{
		Vector->x = Vertex->Point.x;
		Vector->y = Vertex->Point.y;
		Vector->z = Vertex->Point.z;
		Vertex++;
		Vector++;
	}
#endif
}

void b3Triangles::b3ComputeNormals(b3_bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3Triangles::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort    *gPtr;
	GLushort    *pPtr;
	b3_triangle *Triangle;
	b3_vertex   *Vertex;
	b3_count     i;

	Vertex   = m_Vertices;
	Triangle = m_Triangles;
	gPtr     = glGrids;
	pPtr     = glPolygons;
	for (i = 0;i < m_TriaCount;i++)
	{
		*gPtr++ = (unsigned short)Triangle->P1;
		*gPtr++ = (unsigned short)Triangle->P2;

		*gPtr++ = (unsigned short)Triangle->P2;
		*gPtr++ = (unsigned short)Triangle->P3;

		*gPtr++ = (unsigned short)Triangle->P3;
		*gPtr++ = (unsigned short)Triangle->P1;

		*pPtr++ = (unsigned short)Triangle->P1;
		*pPtr++ = (unsigned short)Triangle->P2;
		*pPtr++ = (unsigned short)Triangle->P3;

		Triangle++;
	}
	glGridCount = m_TriaCount * 3;
	glPolyCount = m_TriaCount;
#endif
}
