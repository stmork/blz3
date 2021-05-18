/*
**
**	$Filename:	b3Triangles.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing triangle shape
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
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
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Triangles::b3Triangles(b3_u32 class_type) :
	b3TriangleShape(class_type)
{
}

b3Triangles::b3Triangles(b3_u32 * src) : b3TriangleShape(src)
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

	m_Vertices  = b3Item::b3TypedAlloc<b3_vertex>(m_VertexCount);
	m_Triangles = b3Item::b3TypedAlloc<b3_triangle>(m_TriaCount);

	for (i = 0; i < m_VertexCount; i++)
	{
		m_Vertices[i].Point.x  = b3InitFloat();
		m_Vertices[i].Point.y  = b3InitFloat();
		m_Vertices[i].Point.z  = b3InitFloat();
		m_Vertices[i].Normal.x = b3InitFloat();
		m_Vertices[i].Normal.y = b3InitFloat();
		m_Vertices[i].Normal.z = b3InitFloat();
	}

	for (i = 0; i < m_TriaCount; i++)
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

	for (i = 0; i < m_VertexCount; i++)
	{
		b3StoreFloat(m_Vertices[i].Point.x);
		b3StoreFloat(m_Vertices[i].Point.y);
		b3StoreFloat(m_Vertices[i].Point.z);
		b3StoreFloat(m_Vertices[i].Normal.x);
		b3StoreFloat(m_Vertices[i].Normal.y);
		b3StoreFloat(m_Vertices[i].Normal.z);
	}

	for (i = 0; i < m_TriaCount; i++)
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
	b3RenderContext * context B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	vertCount = m_Flags & B3_PHONG ? m_VertexCount : (m_TriaCount * 3);
	gridCount = m_TriaCount * 3;
	polyCount = m_TriaCount;
}

void b3Triangles::b3ComputeVertices()
{
	b3_gl_vertex * Vector;
	b3_vertex  *  Vertex;
	b3_index      i;

	Vertex = m_Vertices;
	Vector = *glVertexElements;

	if (m_Flags & B3_PHONG)
	{
		// Copy positions
		for (i = 0; i < m_VertexCount; i++)
		{
			Vector->v.x = Vertex->Point.x;
			Vector->v.y = Vertex->Point.y;
			Vector->v.z = Vertex->Point.z;
			Vertex++;
			Vector++;
		}

		// Copy texture coordinates if usable
		if ((m_xSize > 0) && (m_ySize > 0) && (((m_xSize + 1) * (m_ySize + 1)) == m_VertexCount))
		{
			b3_index x, y;
			b3_f64   fx, fxStep;
			b3_f64   fy, fyStep;

			Vector = *glVertexElements;
			fy     = 0;
			fxStep = 1.0 / m_xSize;
			fyStep = 1.0 / m_ySize;
			for (y = 0; y <= m_ySize; y++)
			{
				fx = 0;
				for (x = 0; x <= m_xSize; x++)
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
	else
	{
		// Copy position
		for (i = 0; i < m_TriaCount; i++)
		{
			Vertex = &m_Vertices[m_Triangles[i].P1];
			Vector->v.x = Vertex->Point.x;
			Vector->v.y = Vertex->Point.y;
			Vector->v.z = Vertex->Point.z;
			Vector++;

			Vertex = &m_Vertices[m_Triangles[i].P2];
			Vector->v.x = Vertex->Point.x;
			Vector->v.y = Vertex->Point.y;
			Vector->v.z = Vertex->Point.z;
			Vector++;

			Vertex = &m_Vertices[m_Triangles[i].P3];
			Vector->v.x = Vertex->Point.x;
			Vector->v.y = Vertex->Point.y;
			Vector->v.z = Vertex->Point.z;
			Vector++;
		}

		// Copy texture coordinates if usable
		if ((m_xSize > 0) && (m_ySize > 0) && ((m_xSize * m_ySize * 2) == m_TriaCount))
		{
			b3_index x, y;
			b3_f64   fx, fxStep;
			b3_f64   fy, fyStep;

			Vector = *glVertexElements;
			fy     = 0;
			fxStep = 1.0 / m_xSize;
			fyStep = 1.0 / m_ySize;
			for (y = 0; y < m_ySize; y++)
			{
				fx = 0;
				for (x = 0; x < m_xSize; x++)
				{
					// First quad triangle
					Vector->t.s = fx;
					Vector->t.t = fy;
					Vector++;

					Vector->t.s = fx + fxStep;
					Vector->t.t = fy;
					Vector++;

					Vector->t.s = fx + fyStep;
					Vector->t.t = fy;
					Vector++;

					// Second quad triangle
					Vector->t.s = fx + fxStep;
					Vector->t.t = fy + fyStep;
					Vector++;

					Vector->t.s = fx;
					Vector->t.t = fy + fyStep;
					Vector++;

					Vector->t.s = fx + fyStep;
					Vector->t.t = fy;
					Vector++;

					fx += fxStep;
				}
				fy += fyStep;
			}
		}
	}
}

void b3Triangles::b3ComputeNormals(bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3Triangles::b3ComputeIndices()
{
	b3_gl_line   *  gPtr;
	b3_gl_polygon * pPtr;
	b3_triangle  *  Triangle;
	b3_count        i;
	b3_u32          p1, p2, p3;

	Triangle = m_Triangles;
	gPtr     = *glGridElements;
	pPtr     = *glPolygonElements;

	if (m_Flags & B3_PHONG)
	{
		for (i = 0; i < m_TriaCount; i++)
		{
			p1 = (b3_u32)Triangle->P1;
			p2 = (b3_u32)Triangle->P2;
			p3 = (b3_u32)Triangle->P3;
			Triangle++;

			B3_GL_LINIT(gPtr, p1, p2);
			B3_GL_LINIT(gPtr, p2, p3);
			B3_GL_LINIT(gPtr, p3, p1);

			B3_GL_PINIT(pPtr, p1, p2, p3);
		}
	}
	else
	{
		p1 = 0;
		p2 = 1;
		p3 = 2;
		for (i = 0; i < m_TriaCount; i++)
		{
			B3_GL_LINIT(gPtr, p1, p2);
			B3_GL_LINIT(gPtr, p2, p3);
			B3_GL_LINIT(gPtr, p3, p1);

			B3_GL_PINIT(pPtr, p1, p2, p3);
			p1 += 3;
			p2 += 3;
			p3 += 3;
		}
	}
}

void b3Triangles::b3SetupPicking(b3PickInfo * info)
{
	for (int i = 0; i < m_VertexCount; i++)
	{
		info->b3AddPickPoint(&m_Vertices[i].Point);
	}
}
