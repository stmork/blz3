/*
**
**      $Filename:      b3TriangleShape.cc $
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
#include "blz3/base/b3Cubic.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.14  2001/10/18 14:48:26  sm
**      - Fixing refracting problem on some scenes with glasses.
**      - Fixing overlighting problem when using Mork shading.
**      - Finxing some memory leaks (espacially b3TriangleRefs)
**      - Adding texture support to conditions (stencil mapping).
**        Now conditions are ready to work compatible with
**        Blizzard II.
**
**      Revision 1.13  2001/10/17 21:09:06  sm
**      - Triangle support added for intersections, normal computations. So
**        Spline shapes can be computed, too. Now only CSG is missing.
**
**      Revision 1.12  2001/10/17 14:46:02  sm
**      - Adding triangle support.
**      - Renaming b3TriangleShape into b3Triangles and introducing
**        new b3TriangleShape as base class. This results in
**        source file renaming, too.
**      - Fixing soft shadow bug.
**      - Only scene loading background image when activated.
**      - Fixing LDC spline initialization.
**      - Converting Windows paths into right paths on Un*x
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3TriangleShape::b3TriangleShape(b3_size class_size, b3_u32 class_type) :
	b3Shape(class_size, class_type)
{
	m_GridList = null;
	m_GridSize = 0;
}

b3TriangleShape::b3TriangleShape(b3_u32 class_type) :
	b3Shape(sizeof(b3TriangleShape), class_type)
{
	m_GridList = null;
	m_GridSize = 0;
}

b3TriangleShape::b3TriangleShape(b3_u32 *src) :
	b3Shape(src)
{
	m_GridList = null;
	m_GridSize = 0;
}

b3TriangleShape::~b3TriangleShape()
{
	b3FreeTriaRefs();
}

void b3TriangleShape::b3Transform(b3_matrix *transformation)
{
	b3_index i;

	for (i = 0;i < m_VertexCount;i++)
	{
		b3MatrixVMul (
			transformation,
			&m_Vertices[i].Point,
			&m_Vertices[i].Point,true);
	}
	b3Recompute();
}

b3_bool b3TriangleShape::b3AddCubicItem (
	b3_count trianum,
	b3_index index)
{
	b3Base<b3TriangleRef> *GridList;
	b3TriangleRef         *TriaInfo;

	GridList = &m_GridList[trianum];
	TriaInfo = GridList->First;
	if (TriaInfo != null)
	{
		if (TriaInfo->m_Index == index)
		{
			return true;
		}
	}

	TriaInfo = new b3TriangleRef(index);
	if (TriaInfo==null)
	{
		return false;
	}

	GridList->b3First(TriaInfo);
	return true;
}

void b3TriangleShape::b3SearchCubicItem (
	b3_vector *P1,
	b3_vector *P2,
	b3_vector *P3,
	b3_index   index,
	b3_index   rec,
	b3_count   MaxRec)
{
	b3_index  I1,I2,I3;
	b3_index  x1,y1,z1;
	b3_index  x2,y2,z2;
	b3_index  x3,y3,z3;
	b3_vector P4;

	if (rec > MaxRec)
	{
		return;
	}

	P4.x = (P1->x + P2->x + P3->x) / 3.0;
	P4.y = (P1->y + P2->y + P3->y) / 3.0;
	P4.z = (P1->z + P2->z + P3->z) / 3.0;

	x1 = (b3_index)P1->x; y1 = (b3_index)P1->y; z1 = (b3_index)P1->z;
	x2 = (b3_index)P2->x; y2 = (b3_index)P2->y; z2 = (b3_index)P2->z;
	x3 = (b3_index)P3->x; y3 = (b3_index)P3->y; z3 = (b3_index)P3->z;

	I1 = B3_ABS(x1 - x2) + B3_ABS(y1 - y2) + B3_ABS(z1 - z2);
	I2 = B3_ABS(x2 - x3) + B3_ABS(y2 - y3) + B3_ABS(z2 - z3);
	I3 = B3_ABS(x3 - x1) + B3_ABS(y3 - y1) + B3_ABS(z3 - z1);

	if (I1 <= 1)
	{
		b3AddCubicItem (GRID_INDEX(x1,y1,z1,m_GridSize),index);
		b3AddCubicItem (GRID_INDEX(x2,y2,z2,m_GridSize),index);
	}
	else
	{
		b3SearchCubicItem (P1,P2,&P4,index,rec+1,MaxRec);
	}

	if (I2 <= 1)
	{
		b3AddCubicItem (GRID_INDEX(x2,y2,z2,m_GridSize),index);
		b3AddCubicItem (GRID_INDEX(x3,y3,z3,m_GridSize),index);
	}
	else
	{
		b3SearchCubicItem (P2,P3,&P4,index,rec+1,MaxRec);
	}

	if (I3 <= 1)
	{
		b3AddCubicItem (GRID_INDEX(x3,y3,z3,m_GridSize),index);
		b3AddCubicItem (GRID_INDEX(x1,y1,z1,m_GridSize),index);
	}
	else
	{
		b3SearchCubicItem (P3,P1,&P4,index,rec+1,MaxRec);
	}
}

static b3_s32 IntLog2 (b3_u32 value)
{
	b3_s32 Log = -1;

	while (value > 0)
	{
		value = value >> 1;
		Log++;
	}
	return Log;
}

static b3_s32 IntLog3 (b3_u32 value)
{
	b3_s32 Log = -1;

	while (value > 0)
	{
		value = (value + value) / 3;
		Log++;
	}
	return Log;
}

void b3TriangleShape::b3PrepareGridList ()
{
	b3_index  i,MaxRec;
	b3_count  max;
	b3_vector P1,P2,P3;

	MaxRec      = IntLog3(m_GridSize);
	MaxRec     += MaxRec;
	max         = m_GridSize * m_GridSize * m_GridSize;

	for (i = 0;i < max;i++)
	{
		m_GridList[i].b3InitBase(CLASS_TRIANGLE);
	}
	for (i = 0;i < m_TriaCount;i++)
	{
		P1.x = (m_Vertices[m_Triangles[i].P1].Point.x - m_Base.x) / m_Size.x;
		P1.y = (m_Vertices[m_Triangles[i].P1].Point.y - m_Base.y) / m_Size.y;
		P1.z = (m_Vertices[m_Triangles[i].P1].Point.z - m_Base.z) / m_Size.z;
		P2.x = (m_Vertices[m_Triangles[i].P2].Point.x - m_Base.x) / m_Size.x;
		P2.y = (m_Vertices[m_Triangles[i].P2].Point.y - m_Base.y) / m_Size.y;
		P2.z = (m_Vertices[m_Triangles[i].P2].Point.z - m_Base.z) / m_Size.z;
		P3.x = (m_Vertices[m_Triangles[i].P3].Point.x - m_Base.x) / m_Size.x;
		P3.y = (m_Vertices[m_Triangles[i].P3].Point.y - m_Base.y) / m_Size.y;
		P3.z = (m_Vertices[m_Triangles[i].P3].Point.z - m_Base.z) / m_Size.z;
		if ((fabs(m_Triangles->Normal.x) > 0.01)||
		    (fabs(m_Triangles->Normal.y) > 0.01)||
		    (fabs(m_Triangles->Normal.z) > 0.01))
			b3SearchCubicItem (&P1,&P2,&P3,i,-1L,MaxRec);	
	}
}

void b3TriangleShape::b3FreeTriaRefs()
{
	b3TriangleRef *ref;
	b3_count       i,CubeSize;

	CubeSize = m_GridSize * m_GridSize * m_GridSize;
	for (i = 0;i < CubeSize;i++)
	{
		while(ref = m_GridList[i].First)
		{
			m_GridList[i].b3Remove(ref);
			delete ref;
		}
	}
}

b3_bool b3TriangleShape::b3Prepare()
{
	b3_vector Start,End,R1,R2;
	b3_index  P1,P2,P3,i;
	b3_f64    x,y,z,Denom;

	Start.x =  FLT_MAX;
	Start.y =  FLT_MAX;
	Start.z =  FLT_MAX;
	End.x	= -FLT_MAX;
	End.y	= -FLT_MAX;
	End.z	= -FLT_MAX;

	if ((m_xSize < 1) || (m_ySize < 1))
	{
		if (m_xSize < 1) m_xSize = 1;
		if (m_ySize < 1) m_ySize = 1;
		B3_ASSERT(b3GetConditionHead()->First == null);
	}
	for (i = 0;i < m_TriaCount;i++)
	{
		P1 = m_Triangles[i].P1;		/* Base */
		P2 = m_Triangles[i].P2;		/* Dir1 */
		P3 = m_Triangles[i].P3;		/* Dir2 */
		R1.x = m_Vertices[P2].Point.x - m_Vertices[P1].Point.x;
		R1.y = m_Vertices[P2].Point.y - m_Vertices[P1].Point.y;
		R1.z = m_Vertices[P2].Point.z - m_Vertices[P1].Point.z;
		R2.x = m_Vertices[P3].Point.x - m_Vertices[P1].Point.x;
		R2.y = m_Vertices[P3].Point.y - m_Vertices[P1].Point.y;
		R2.z = m_Vertices[P3].Point.z - m_Vertices[P1].Point.z;
		if ((m_Flags & NORMAL_FACE_VALID)==0)
		{
			m_Triangles[i].Normal.x = x = R1.y * R2.z - R1.z * R2.y;
			m_Triangles[i].Normal.y = y = R1.z * R2.x - R1.x * R2.z;
			m_Triangles[i].Normal.z = z = R1.x * R2.y - R1.y * R2.x;
		}
		if ((m_Flags & NORMAL_VERTEX_VALID)==0)
		{
#ifdef NORMAL_NORMALIZED
			Denom = x * x + y * y + z * z;
			if (Denom != 0)
			{
				Denom = 1 / sqrt(Denom);
				x *= Denom;
				y *= Denom;
				z *= Denom;
			}
#endif
			m_Vertices[P1].Normal.x += x;
			m_Vertices[P1].Normal.y += y;
			m_Vertices[P1].Normal.z += z;
			m_Vertices[P2].Normal.x += x;
			m_Vertices[P2].Normal.y += y;
			m_Vertices[P2].Normal.z += z;
			m_Vertices[P3].Normal.x += x;
			m_Vertices[P3].Normal.y += y;
			m_Vertices[P3].Normal.z += z;
		}
	}

	for (i = 0;i < m_VertexCount;i++)
	{
		if (m_Vertices[i].Point.x < Start.x) Start.x = m_Vertices[i].Point.x;
		if (m_Vertices[i].Point.x > End.x)   End.x   = m_Vertices[i].Point.x;
		if (m_Vertices[i].Point.y < Start.y) Start.y = m_Vertices[i].Point.y;
		if (m_Vertices[i].Point.y > End.y)   End.y   = m_Vertices[i].Point.y;
		if (m_Vertices[i].Point.z < Start.z) Start.z = m_Vertices[i].Point.z;
		if (m_Vertices[i].Point.z > End.z)   End.z   = m_Vertices[i].Point.z;

		if ((m_Flags & NORMAL_VERTEX_VALID)==0)
		{
			x = m_Vertices[i].Normal.x;
			y = m_Vertices[i].Normal.y;
			z = m_Vertices[i].Normal.z;
			Denom = x * x + y * y + z * z;
			if (Denom != 0)
			{
				Denom = 1 / sqrt(Denom);
				m_Vertices[i].Normal.x = x * Denom;
				m_Vertices[i].Normal.y = y * Denom;
				m_Vertices[i].Normal.z = z * Denom;
			}
		}
	}

#	ifndef ONE_GRID
	m_GridSize = (b3_count)cbrt (0.125 * m_TriaCount);
	if (m_GridSize < 1)
	{
		m_GridSize = 1;
	}
#	else
	m_GridSize = 1;
#	endif

	if ((End.x - Start.x) < 0.1) m_GridSize = 1;
	if ((End.y - Start.y) < 0.1) m_GridSize = 1;
	if ((End.z - Start.z) < 0.1) m_GridSize = 1;
	Start.x -= 0.1f;
	Start.y -= 0.1f;
	Start.z -= 0.1f;
	End.x   += 0.1f;
	End.y   += 0.1f;
	End.z   += 0.1f;

	Denom = 1.0 / m_GridSize;
	m_Base     = Start;
	m_Size.x	= (End.x - Start.x) * Denom;
	m_Size.y	= (End.y - Start.y) * Denom;
	m_Size.z	= (End.z - Start.z) * Denom;
	if (m_Size.x < epsilon) m_Size.x = epsilon;
	if (m_Size.y < epsilon) m_Size.y = epsilon;
	if (m_Size.z < epsilon) m_Size.z = epsilon;

	if (m_GridList == null)
	{
		m_GridList = (b3Base<b3TriangleRef> *)b3Item::b3Alloc
			(m_GridSize * m_GridSize * m_GridSize * sizeof(b3Base<b3TriangleRef>));
		if (m_GridList==null)
		{
			return false;
		}
	}
	else
	{
		b3FreeTriaRefs();
	}
	b3PrepareGridList();
	return true;
}
