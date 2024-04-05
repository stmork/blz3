/*
**
**	$Filename:	b3SplineRot.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing BSpline rotation shape
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
**                        b3SplineRotShape Implementation               **
**                                                                      **
*************************************************************************/

b3SplineRotShape::b3SplineRotShape(b3_u32 class_type) :
	b3TriangleShape(sizeof(b3SplineRotShape), class_type)
{
	m_Controls = nullptr;
	b3Vector::b3Init(&m_Axis.pos);
	b3Vector::b3Init(&m_Axis.dir, 0, 0, 1);
}

b3SplineRotShape::b3SplineRotShape(b3_u32 * src) :
	b3TriangleShape(src)
{
	m_rSubDiv = b3InitInt();
	for (unsigned i = 0; i < b3Spline::B3_MAX_KNOTS; i++)
	{
		m_Knots[i] = b3InitFloat();
	}
	b3InitSpline(&m_Spline, nullptr, m_Knots);
	b3InitVector(&m_Axis.pos);
	b3InitVector(&m_Axis.dir);

	m_Controls = b3Item::b3TypedAlloc<b3_vector>(m_Spline.m_ControlMax);
	m_Spline.m_Controls = m_Controls;
	for (unsigned i = 0; i < m_Spline.m_ControlMax; i++)
	{
		b3InitVector(&m_Controls[i]);
	}
	b3InitActivation();
}

void b3SplineRotShape::b3StoreShape()
{
	b3StoreInt(m_rSubDiv);
	for (unsigned i = 0; i < b3Spline::B3_MAX_KNOTS; i++)
	{
		b3StoreFloat(m_Knots[i]);
	}
	b3StoreSpline(&m_Spline);
	b3StoreVector(&m_Axis.pos);
	b3StoreVector(&m_Axis.dir);
	for (unsigned i = 0; i < m_Spline.m_ControlMax; i++)
	{
		b3StoreVector(&m_Controls[i]);
	}
}


void b3SplineRotShape::b3Init(
	b3_count Degree,
	b3_count ControlNum,
	bool     Closed,
	b3_count SubDiv)
{
	m_rSubDiv = SubDiv;

	// Allocate controls
	m_Controls = b3Item::b3TypedAlloc<b3_vector>(m_Spline.m_ControlMax);

	// Init horizontal spline
	m_Spline.m_Knots    = m_Knots;
	m_Spline.m_Controls = m_Controls;
	m_Spline.m_Offset   = 1;
	m_Spline.b3InitCurve(Degree, ControlNum, Closed);
}

bool b3SplineRotShape::b3Prepare(b3_preparation_info * prep_info)
{
	b3Spline      MySpline;
	b3_triangle * Triangle;
	b3_matrix     Matrix;
	b3_count      VertexCount, TriaCount;
	b3_vector     Between[b3Spline::B3_MAX_CONTROLS + 1];
	b3_vector     VertexField[b3Spline::B3_MAX_SUBDIV + 1];

	// Create aux BSpline
	MySpline            = m_Spline;
	MySpline.m_Controls = Between;

	m_xVertices       = m_rSubDiv;
	m_yVertices       = MySpline.m_SubDiv;
	TriaCount = m_xVertices * m_yVertices * 2;

	if (!MySpline.m_Closed)
	{
		m_yVertices++;
	}
	VertexCount = m_xVertices * m_yVertices;

	// Reallocating new tria shape
	if (!b3TriangleShape::b3Init(VertexCount, TriaCount, m_rSubDiv, MySpline.m_SubDiv))
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}


	// computing vertices
	MySpline.m_Offset = 1;
	for (unsigned x = 0; x < m_xVertices; x++)
	{
		b3Matrix::b3RotateVector(nullptr, &Matrix, &m_Axis, M_PI * 2.0 * x / m_xVertices);
		for (unsigned y = 0; y < MySpline.m_ControlNum; y++)
		{
			b3Matrix::b3VMul(&Matrix, &m_Controls[y], &Between[y], true);
		}

		MySpline.b3DeBoor(VertexField, 0);
		for (unsigned y = 0; y < m_yVertices; y++)
		{
			m_Vertices[y * m_xVertices + x].Point.x = VertexField[y].x;
			m_Vertices[y * m_xVertices + x].Point.y = VertexField[y].y;
			m_Vertices[y * m_xVertices + x].Point.z = VertexField[y].z;
		}
	}


	// computing triangles
	Triangle = m_Triangles;
	for (unsigned y = 0; y < MySpline.m_SubDiv; y++)
	{
		for (unsigned x = 0; x < m_xVertices; x++)
		{
			Triangle->P1 =  x              + m_xVertices *  y;
			Triangle->P2 = (x + 1) % m_xVertices + m_xVertices *  y;
			Triangle->P3 =  x              + m_xVertices * ((y + 1) % m_yVertices);
			Triangle++;

			Triangle->P1 = (x + 1) % m_xVertices + m_xVertices * ((y + 1) % m_yVertices);
			Triangle->P2 =  x              + m_xVertices * ((y + 1) % m_yVertices);
			Triangle->P3 = (x + 1) % m_xVertices + m_xVertices *  y;
			Triangle++;
		}
	}


	/* initializing values */
	m_xTxSubDiv = m_xVertices;
	m_yTxSubDiv = MySpline.m_SubDiv;
	m_Flags = B3_PHONG;

	return b3TriangleShape::b3Prepare(prep_info);
}

void b3SplineRotShape::b3Transform(b3_matrix * transformation, bool is_affine)
{
	b3_vector * control = m_Spline.m_Controls;

	// Transform rotation axis
	b3Matrix::b3VMul(transformation, &m_Axis.pos, &m_Axis.pos, true);
	b3Matrix::b3VMul(transformation, &m_Axis.dir, &m_Axis.dir, false);

	// Transform control points
	for (unsigned x = 0; x < m_Spline.m_ControlNum; x++)
	{
		b3Vector::b3MatrixMul4D(transformation, control);
		control += m_Spline.m_Offset;
	}
	b3TriangleShape::b3Transform(transformation, is_affine);
}

void b3SplineRotShape::b3SetupPicking(b3PickInfo * info)
{
	b3_vector * control = m_Spline.m_Controls;

	info->b3AddPickPoint(&m_Axis.pos, "b");
	info->b3AddPickDir(&m_Axis.pos, &m_Axis.dir, "d");

	for (unsigned x = 0; x < m_Spline.m_ControlNum; x++)
	{
		info->b3AddPickPoint(control);
		info->b3AddVertex(control);
		control += m_Spline.m_Offset;
	}
}

void b3SplineRotShape::b3SetupGrid(b3PickInfo * info)
{
	b3_vector * control = m_Spline.m_Controls;

	for (unsigned x = 0; x < m_Spline.m_ControlNum; x++)
	{
		info->b3AddPickPoint(control);
		info->b3AddVertex(control);
		control += m_Spline.m_Offset;
	}

	for (unsigned x = (m_Spline.m_Closed ? 0 : 1); x < m_Spline.m_ControlNum; x++)
	{
		info->b3AddLine(x, (x + m_Spline.m_ControlNum - 1) % m_Spline.m_ControlNum);
	}
}

void b3SplineRotShape::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	m_ySubDiv   = m_Spline.m_SubDiv;
	m_xSubDiv   = m_rSubDiv;
	if (!m_Spline.m_Closed)
	{
		m_ySubDiv++;
	}

	vertCount = (m_Spline.m_SubDiv + 1) * m_rSubDiv;
	gridCount = m_rSubDiv * (m_Spline.m_SubDiv + m_ySubDiv);
	polyCount = m_rSubDiv *  m_Spline.m_SubDiv * 2;
}

void b3SplineRotShape::b3ComputeVertices()
{
	b3_matrix      Matrix;
	b3Spline       AuxSpline;
	b3_vector      AuxControls[b3Spline::B3_MAX_CONTROLS + 1];
	b3_vector      SplVector[b3Spline::B3_MAX_SUBDIV + 1];
	b3_gl_vertex * Vector;
	unsigned       i;
	b3_count       x, count;
	b3_f64         fx, fxStep;
	b3_f64         fy, fyStep;

	// Build rotation matrix
	b3Matrix::b3RotateVector(nullptr, &Matrix, &m_Axis, M_PI * 2 / m_rSubDiv);

	// Copy BSpline
	AuxSpline            = m_Spline;
	AuxSpline.m_Controls = AuxControls;
	for (i = 0; i < AuxSpline.m_ControlNum; i++)
	{
		AuxControls[i] = m_Controls[i];
	}

	Vector = *glVertexElements;
	fy     = 0;
	fyStep = 1.0 / (b3_f64)m_rSubDiv;
	for (unsigned a = 0; a < m_rSubDiv; a++)
	{
		// Compute curve
		count  = AuxSpline.b3DeBoor(SplVector, 0);
		fx     = 0;
		fxStep = 1.0 / (b3_f64)count;
		for (x = 0; x < count; x++)
		{
			Vector->t.s = fx;
			Vector->t.t = fy;
			Vector->v.x = SplVector[x].x;
			Vector->v.y = SplVector[x].y;
			Vector->v.z = SplVector[x].z;
			Vector++;

			fx += fxStep;
		}
		fy += fyStep;

		// Rotate control points
		for (i = 0; i < AuxSpline.m_ControlNum; i++)
		{
			b3Matrix::b3VMul(&Matrix, &AuxControls[i], &AuxControls[i], true);
		}
	}

	m_xVertices  = m_rSubDiv;
	m_yVertices  = AuxSpline.m_SubDiv;
}

void b3SplineRotShape::b3ComputeIndices()
{
	b3_gl_line   *  gPtr;
	b3_gl_polygon * pPtr;
	b3_index        x1, y2;
	b3_count        yStep;

	yStep = m_Spline.m_SubDiv + 1;
	gPtr  = *glGridElements;
	pPtr  = *glPolygonElements;
	x1    = 0;

	// for each curve
	for (unsigned a = 0; a < m_rSubDiv; a++)
	{
		unsigned x2 = (a + 1) % m_rSubDiv * yStep;

		// curve itself
		for (unsigned y1 = 0; y1 < m_Spline.m_SubDiv; y1++)
		{
			y2 = (y1 + 1) % m_ySubDiv;

			B3_GL_LINIT(gPtr, x1 + y1, x1 + y2);

			B3_GL_PINIT(pPtr, x1 + y1, x2 + y1, x1 + y2);
			B3_GL_PINIT(pPtr, x2 + y2, x1 + y2, x2 + y1);
		}

		// lines between curves
		for (b3_count y1 = 0; y1 < m_ySubDiv; y1++)
		{
			B3_GL_LINIT(gPtr, x1 + y1, x2 + y1);
		}
		x1 += yStep;
	}
}
