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

#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.11  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.10  2003/02/24 17:32:39  sm
**      - Added further picking support.
**      - Fixed geometry update delay.
**
**      Revision 1.9  2002/08/16 11:40:39  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.8  2002/07/29 14:48:11  sm
**      - Circled shapes like cylinder, doughnuts etc. draw
**        textures correctly but renders shading a little bit
**        wrong at seam.
**      - Added support for multiple lights. This should be
**        configurable inside a scene (via b3ModellerInfo?)
**
**      Revision 1.7  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.6  2001/12/30 22:52:36  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
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
	b3_gl_vertex *Vector;
	b3_vertex    *Vertex;
	b3_index      i;

	Vertex        = m_Vertices;
	Vector        = glVertex;
	glVertexCount = m_VertexCount;
	for (i = 0;i < m_VertexCount;i++)
	{
		Vector->v.x = Vertex->Point.x;
		Vector->v.y = Vertex->Point.y;
		Vector->v.z = Vertex->Point.z;
		Vertex++;
		Vector++;
	}

	if ((m_xSize > 0) && (m_ySize > 0) && ((m_xSize + 1) * (m_ySize + 1) == m_VertexCount))
	{
		b3_index x,y;
		b3_f64   fx,fxStep;
		b3_f64   fy,fyStep;

		Vector = glVertex;
		fy     = 0;
		fxStep = 1.0 / m_xSize;
		fyStep = 1.0 / m_ySize;
		for (y = 0;y <= m_ySize;y++)
		{
			fx = 0;
			for (x = 0;x <= m_xSize;x++)
			{
				Vector->t.s = fx;
				Vector->t.t = fy;
				Vector++;
				fx += fxStep;
			}
			fy += fyStep;
		}
	}
}

void b3Triangles::b3ComputeNormals(b3_bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3Triangles::b3ComputeIndices()
{
	b3_gl_line    *gPtr;
	b3_gl_polygon *pPtr;
	b3_triangle   *Triangle;
	b3_vertex     *Vertex;
	b3_count       i;
	b3_u16         p1,p2,p3;

	Vertex   = m_Vertices;
	Triangle = m_Triangles;
	gPtr     = glGrids;
	pPtr     = glPolygons;
	for (i = 0;i < m_TriaCount;i++)
	{
		p1 = (b3_u16)Triangle->P1;
		p2 = (b3_u16)Triangle->P2;
		p3 = (b3_u16)Triangle->P3;
		Triangle++;
		
		B3_GL_LINIT(gPtr,p1,p2);
		B3_GL_LINIT(gPtr,p2,p3);
		B3_GL_LINIT(gPtr,p3,p1);
		
		B3_GL_PINIT(pPtr,p1,p2,p3);
	}
	glGridCount = m_TriaCount * 3;
	glPolyCount = m_TriaCount;
}

void b3Triangles::b3SetupPicking(b3PickInfo *info)
{
	for (int i = 0;i < m_VertexCount;i++)
	{
		info->b3AddPickPoint(&m_Vertices[i].Point);
	}
}
