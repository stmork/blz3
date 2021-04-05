/*
**
**	$Filename:	b3TriangleShape.cc $
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3TriangleShape::b3TriangleShape(b3_size class_size, b3_u32 class_type) :
	b3SimpleShape(class_size, class_type)
{
	b3Clear();
}

b3TriangleShape::b3TriangleShape(b3_u32 class_type) :
	b3SimpleShape(sizeof(b3TriangleShape), class_type)
{
	b3Clear();
}

b3TriangleShape::b3TriangleShape(b3_u32 * src) :
	b3SimpleShape(src)
{
	b3Clear();
}

b3TriangleShape::~b3TriangleShape()
{
	b3FreeTriaRefs();
}

void b3TriangleShape::b3Clear()
{
	m_GridComputed = false;
	m_Vertices     = nullptr;
	m_Triangles    = nullptr;
	m_GridList     = nullptr;
	m_GridCount    = 0;
	m_GridSize     = 0;
	m_VertexCount  = 0;
	m_TriaCount    = 0;
	m_xSize        = 0;
	m_ySize        = 0;
	m_Flags        = 0;
}

bool b3TriangleShape::b3Init(
	b3_count vertex_count,
	b3_count tria_count,
	b3_res   xSize,
	b3_res   ySize)
{
	if (m_VertexCount != vertex_count)
	{
		b3Item::b3Free(m_Vertices);
		m_VertexCount = vertex_count;
		m_Vertices    = b3Item::b3TypedAlloc<b3_vertex>(m_VertexCount);
	}

	if (m_TriaCount != tria_count)
	{
		b3Item::b3Free(m_Triangles);
		m_TriaCount = tria_count;
		m_Triangles = b3Item::b3TypedAlloc<b3_triangle>(m_TriaCount);
	}
	m_xSize = xSize;
	m_ySize = ySize;

	return (m_Vertices != nullptr) && (m_Triangles != nullptr);
}

void b3TriangleShape::b3Transform(b3_matrix * transformation, bool is_affine)
{
	b3_index   i;
	b3_vertex * ptr = m_Vertices;

	m_GridComputed = false;
	for (i = 0; i < m_VertexCount; i++)
	{
		b3Vector::b3MatrixMul4D(transformation, &ptr->Point);
		ptr++;
	}
	b3TransformVertices(transformation, is_affine);
}

b3_count b3TriangleShape::b3IntLog2(b3_count value)
{
	b3_count result = -1;

	while (value > 0)
	{
		value = value >> 1;
		result++;
	}
	return result;
}

b3_count b3TriangleShape::b3IntLog3(b3_count value)
{
	b3_count result = -1;

	while (value > 0)
	{
		value = (value + value) / 3;
		result++;
	}
	return result;
}

void b3TriangleShape::b3SubdivideIntoGrid(
	b3_vector * P1,
	b3_vector * P2,
	b3_vector * P3,
	b3_index   triangle,
	b3_count   max)
{
	b3_index i1 = b3GetGrid(P1);
	b3_index i2 = b3GetGrid(P2);
	b3_index i3 = b3GetGrid(P3);

	if ((i1 == i2) && (i2 == i3))
	{
		// Entire triangle is in a single voxel
		b3AddTriangleToGrid(i1, triangle);
	}
	else
	{
		b3_index dSum =
			b3GridDistance(P1->x, P2->x, P3->x) +
			b3GridDistance(P1->y, P2->y, P3->y) +
			b3GridDistance(P1->z, P2->z, P3->z);

		if (dSum == 1)
		{
			// triangles span over two nieghboured grid voxels.
			b3AddTriangleToGrid(i1, triangle);
			b3AddTriangleToGrid(i2, triangle);
			b3AddTriangleToGrid(i3, triangle);
		}
		else if (max > 0)
		{
			b3_vector P12h;
			b3_vector P23h;
			b3_vector P31h;

			b3Vector::b3Add(P1, P2, &P12h);
			b3Vector::b3Scale(&P12h, 0.5);

			b3Vector::b3Add(P2, P3, &P23h);
			b3Vector::b3Scale(&P23h, 0.5);

			b3Vector::b3Add(P3, P1, &P31h);
			b3Vector::b3Scale(&P31h, 0.5);

			// Further subdivision
			b3SubdivideIntoGrid(P1,  &P12h, &P31h, triangle, max - 1);
			b3SubdivideIntoGrid(P2,  &P23h, &P12h, triangle, max - 1);
			b3SubdivideIntoGrid(P3,  &P31h, &P23h, triangle, max - 1);
			b3SubdivideIntoGrid(&P12h, &P23h, &P31h, triangle, max - 1);
		}
		else
		{
			// End!
			b3AddTriangleToGrid(i1, triangle);
			b3AddTriangleToGrid(i2, triangle);
			b3AddTriangleToGrid(i3, triangle);
		}
	}
}

void b3TriangleShape::b3PrepareGridList()
{
	b3_index  i, MaxRec;
	b3_count  max;
	b3_vector P1, P2, P3;
#ifdef _DEBUG
	b3_count  degenerated = 0;
#endif

	max    = m_GridSize * m_GridSize * m_GridSize;
	MaxRec = b3IntLog2(m_GridSize) + 1;

	if (max > 1)
	{
		for (i = 0; i < m_TriaCount; i++)
		{
			if (b3Vector::b3QuadLength(&m_Triangles[i].Normal) > (b3Scene::epsilon * b3Scene::epsilon))
			{
				b3ToGridSpace(&m_Vertices[m_Triangles[i].P1].Point, &P1);
				b3ToGridSpace(&m_Vertices[m_Triangles[i].P2].Point, &P2);
				b3ToGridSpace(&m_Vertices[m_Triangles[i].P3].Point, &P3);

				b3SubdivideIntoGrid(&P1, &P2, &P3, i, MaxRec);
			}
#ifdef _DEBUG
			else
			{
				degenerated++;
			}
#endif
		}
	}
	else
	{
		for (i = 0; i < m_TriaCount; i++)
		{
			if (b3Vector::b3QuadLength(&m_Triangles[i].Normal) > (b3Scene::epsilon * b3Scene::epsilon))
			{
				m_GridList[0].b3Add(i);
			}
#ifdef _DEBUG
			else
			{
				degenerated++;
			}
#endif
		}
	}

#ifdef _DEBUG
	if (degenerated > 0)
	{
		b3PrintF(B3LOG_DEBUG, "%ld triangles degenerated.\n", degenerated);
	}
#endif
}

void b3TriangleShape::b3FreeTriaRefs()
{
	if (m_GridList != nullptr)
	{
		delete [] m_GridList;
		m_GridList  = nullptr;
		m_GridCount = 0;
	}
}

bool b3TriangleShape::b3Prepare(b3_preparation_info * prep_info)
{
	b3_vector   Start, End, disp, diff;
	b3_index    P1, P2, P3, i, max;
	b3_f64      Denom;
	b3_triainfo info;

	if ((m_xSize < 1) || (m_ySize < 1))
	{
		if (m_xSize < 1)
		{
			m_xSize = 1;
		}
		if (m_ySize < 1)
		{
			m_ySize = 1;
		}
		B3_ASSERT(b3GetConditionHead()->First == nullptr);
	}

	if (!m_GridComputed)
	{
		m_TriaInfos.b3Clear();
		for (i = 0; i < m_TriaCount; i++)
		{
			P1 = m_Triangles[i].P1;		/* Base */
			P2 = m_Triangles[i].P2;		/* Dir1 */
			P3 = m_Triangles[i].P3;		/* Dir2 */

			info.base  = m_Vertices[P1].Point;

			b3Vector::b3Sub(&m_Vertices[P2].Point, &m_Vertices[P1].Point, &info.dir1);
			b3Vector::b3Sub(&m_Vertices[P3].Point, &m_Vertices[P1].Point, &info.dir2);
			b3Vector::b3CrossProduct(&info.dir1, &info.dir2, &info.Normal);

			m_TriaInfos.b3Add(info);

			if ((m_Flags & B3_NORMAL_FACE_VALID) == 0)
			{
				disp = m_Triangles[i].Normal = info.Normal;

				if ((m_Flags & B3_NORMAL_VERTEX_VALID) == 0)
				{
#ifdef NORMAL_NORMALIZED
					b3Vector::b3Normalize(&disp);
#endif
					b3Vector::b3Add(&disp, &m_Vertices[P1].Normal);
					b3Vector::b3Add(&disp, &m_Vertices[P2].Normal);
					b3Vector::b3Add(&disp, &m_Vertices[P3].Normal);
				}
			}
		}

		b3Vector::b3InitBound(&Start, &End);
		for (i = 0; i < m_VertexCount; i++)
		{
			b3Vector::b3AdjustBound(&m_Vertices[i].Point, &Start, &End);

			if ((m_Flags & B3_NORMAL_VERTEX_VALID) == 0)
			{
				b3Vector::b3Normalize(&m_Vertices[i].Normal);
			}
		}

#ifndef ONE_GRID
		m_GridSize = (b3_count)cbrt(m_TriaCount);
		if (m_GridSize < 1)
		{
			m_GridSize = 1;
		}
#else
		m_GridSize = 1;
#endif

		Start.x -= 0.1f;
		Start.y -= 0.1f;
		Start.z -= 0.1f;
		End.x   += 0.1f;
		End.y   += 0.1f;
		End.z   += 0.1f;
		b3Vector::b3Sub(&End, &Start, &diff);
		if ((diff.x < 0.1) ||
			(diff.y < 0.1) ||
			(diff.z < 0.1))
		{
			m_GridSize = 1;
		}

		Denom    = 1.0 / m_GridSize;
		m_Base   = Start;
		b3Vector::b3Scale(&diff, &m_Size, Denom);
		b3Vector::b3SetMinimum(&m_Size, b3Scene::epsilon);

		max = m_GridSize * m_GridSize * m_GridSize;
		if (max != m_GridCount)
		{
			b3FreeTriaRefs();
		}
		if (m_GridList == nullptr)
		{
			m_GridList  = new b3Array<b3_index>[max];
			m_GridCount = max;
		}
		else
		{
			for (i = 0; i < max; i++)
			{
				m_GridList[i].b3Clear();
			}
		}
		b3PrepareGridList();
		m_GridComputed = true;
	}
	return b3Shape::b3Prepare(prep_info);
}
