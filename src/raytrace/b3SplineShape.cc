/*
**
**	$Filename:	b3SplineShape.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing BSpline shapes
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
**                        BSpline shapes                                **
**                                                                      **
*************************************************************************/

b3SplineShape::b3SplineShape(b3_size class_size, b3_u32 class_type) :
	b3TriangleShape(class_size, class_type)
{
	m_Controls = nullptr;
}

b3SplineShape::b3SplineShape(b3_u32 class_type) : b3TriangleShape(sizeof(b3SplineShape), class_type)
{
	m_Controls = nullptr;
}

b3SplineShape::b3SplineShape(b3_u32 * src) : b3TriangleShape(src)
{
	b3_index i;
	b3_count control_count;

	b3InitVector();
	b3InitVector();
	b3InitSpline(&m_Spline[0], nullptr, m_Knots[0]);
	b3InitSpline(&m_Spline[1], nullptr, m_Knots[1]);

	// Copy knots
	for (i = 0; i < B3_MAX_KNOTS; i++)
	{
		m_Knots[0][i] = b3InitFloat();
	}
	for (i = 0; i < B3_MAX_KNOTS; i++)
	{
		m_Knots[1][i] = b3InitFloat();
	}

	// Copy control points
	control_count = m_Spline[0].m_ControlMax * m_Spline[1].m_ControlMax;
	m_Controls    = (b3_vector *)b3Item::b3Alloc(control_count * sizeof(b3_vector));
	m_Spline[0].m_Controls =
		m_Spline[1].m_Controls = m_Controls;
	for (i = 0; i < control_count; i++)
	{
		b3InitVector(&m_Controls[i]);
	}
	b3InitActivation();
}

void b3SplineShape::b3StoreShape()
{
	b3_index i;
	b3_count control_count;

	b3StoreVector();
	b3StoreVector();
	b3StoreSpline(&m_Spline[0]);
	b3StoreSpline(&m_Spline[1]);

	// Store knots
	for (i = 0; i < B3_MAX_KNOTS; i++)
	{
		b3StoreFloat(m_Knots[0][i]);
	}
	for (i = 0; i < B3_MAX_KNOTS; i++)
	{
		b3StoreFloat(m_Knots[1][i]);
	}

	// Store control points
	control_count = m_Spline[0].m_ControlMax * m_Spline[1].m_ControlMax;
	for (i = 0; i < control_count; i++)
	{
		b3StoreVector(&m_Controls[i]);
	}
}

void b3SplineShape::b3Init(
	b3_count hDegree,
	b3_count vDegree,
	b3_count hControlNum,
	b3_count vControlNum)
{
	// Allocate controls
	m_Controls      = (b3_vector *)b3Item::b3Alloc(
			m_Spline[0].m_ControlMax *
			m_Spline[1].m_ControlMax * sizeof(b3_vector));

	// Init horizontal spline
	m_Spline[0].m_Knots    = m_Knots[0];
	m_Spline[0].m_Controls = m_Controls;
	m_Spline[0].m_Offset   = 1;
	m_Spline[0].b3InitCurve(hDegree, hControlNum, b3GetClassType() != SPLINES_AREA);

	// Init vertical spline
	m_Spline[1].m_Knots    = m_Knots[1];
	m_Spline[1].m_Controls = m_Controls;
	m_Spline[1].m_Offset   = m_Spline[0].m_ControlMax;
	m_Spline[1].b3InitCurve(vDegree, vControlNum, b3GetClassType() == SPLINES_RING);
}

void b3SplineShape::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	// Compute number of grid vertices
	m_GridVertexCount = (B3_MAX_CONTROLS + B3_MAX_CONTROLS) * (B3_MAX_SUBDIV + 1);

	// Compute number of solid vertices. That
	// are usually much more.
	m_xSubDiv = m_Spline[0].m_SubDiv;
	m_ySubDiv = m_Spline[1].m_SubDiv;

	if (!m_Spline[0].m_Closed)
	{
		m_xSubDiv++;
	}
	if (!m_Spline[1].m_Closed)
	{
		m_ySubDiv++;
	}
	m_SolidVertexCount = (m_xSubDiv + 1) * (m_ySubDiv * 1);

	// Get enough memory
	vertCount = m_GridVertexCount + m_SolidVertexCount;
	gridCount =
		B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]) * m_Spline[0].m_SubDiv +
		B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]) * m_Spline[1].m_SubDiv;
	polyCount = m_Spline[0].m_SubDiv * m_Spline[1].m_SubDiv * 2;
}

void b3SplineShape::b3ComputeGridVertices()
{
	b3_gl_vertex * Vector = *glVertexElements;
	b3_vector     SplVector[B3_MAX_SUBDIV + 1];
	b3_vector     ControlArray[B3_MAX_CONTROLS * B3_MAX_CONTROLS];
	b3_count      CurveNum, Points = 0;
	b3_index      x, y, t;
	b3Spline      MySpline;

	m_Spline[0].m_Controls =  m_Controls;
	m_Spline[1].m_Controls =  m_Controls;
	m_Spline[0].m_Knots    = &m_Knots[0][0];
	m_Spline[1].m_Knots    = &m_Knots[1][0];

	// building horizontal splines
	// first create controls for segments of vertical spline...
	b3Spline::b3DeBoorSurfaceControl(&m_Spline[0], &m_Spline[1], ControlArray);
	MySpline            = m_Spline[0];
	MySpline.m_Offset   = CurveNum = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]);
	MySpline.m_Controls = ControlArray;

	// ... then create real horizontal spline curve.
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "SplineShape: horizontal\n");
	m_Spline[0].b3Dump();
#endif
	for (y = 0; y < CurveNum; y++)
	{
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL, "     y: %d\n", y);
#endif
		Points = MySpline.b3DeBoor(SplVector, y);
		for (t = 0; t < Points; t++)
		{
			Vector->v.x = SplVector[t].x;
			Vector->v.y = SplVector[t].y;
			Vector->v.z = SplVector[t].z;
			Vector++;
		}
	}
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "SplineShape: horizontal done\n");
#endif

	// building vertical splines
	// first create controls for segments of horizontal spline...
	b3Spline::b3DeBoorSurfaceControl(&m_Spline[1], &m_Spline[0], ControlArray);
	MySpline          = m_Spline[1];
	MySpline.m_Offset   = CurveNum = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]);
	MySpline.m_Controls = ControlArray;

	// ... then create real vertical spline curve.
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "SplineShape: vertical\n");
	m_Spline[1].b3Dump();
#endif
	for (x = 0; x < CurveNum; x++)
	{
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL, "     x: %d\n", x);
#endif
		Points = MySpline.b3DeBoor(SplVector, x);
		for (t = 0; t < Points; t++)
		{
			Vector->v.x = SplVector[t].x;
			Vector->v.y = SplVector[t].y;
			Vector->v.z = SplVector[t].z;
			Vector++;
		}
	}
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "SplineShape: vertical done\n");
#endif
}

void b3SplineShape::b3ComputeSolidVertices()
{
	b3Spline      MySpline;
	b3_index      x, y;
	b3_f64        fx, fxStep;
	b3_f64        fy, fyStep;
	b3_count      SubDiv, index, count;
	b3_gl_vertex * Vector;
	b3_gl_vertex * glVertex = *glVertexElements;
	b3_vector  *  Aux;
	b3_vector     SplVector[B3_MAX_SUBDIV + 1];
	b3_vector     Between[(B3_MAX_SUBDIV + 1) * (B3_MAX_SUBDIV + 1)];

	// Building horizontal BSplines
	Aux    = Between;
	SubDiv = m_Spline[0].m_SubDiv + 1;
	for (x = 0; x < SubDiv; x++)
	{
		Aux += m_Spline[1].b3DeBoor(Aux, x * m_Spline[0].m_Offset);
	}

	// Create aux BSpline
	MySpline          = m_Spline[0];
	MySpline.m_Offset   = m_Spline[1].m_SubDiv + 1;
	MySpline.m_Controls = Between;

	Vector = &glVertex[m_GridVertexCount];
	index  = 0;
	fy     = 0;
	fyStep = 1.0 / (b3_f64)m_ySubDiv;
	for (y = 0; y < m_ySubDiv; y++)
	{
		count   = MySpline.b3DeBoor(SplVector, y);
		index  += count;

		fx = 0;
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
	}
	B3_ASSERT(index <= m_SolidVertexCount);
}

void b3SplineShape::b3ComputeVertices()
{
	if (m_GridVertexCount  > 0)
	{
		b3ComputeGridVertices();
	}
	if (m_SolidVertexCount > 0)
	{
		b3ComputeSolidVertices();
	}
}

void b3SplineShape::b3ComputeGridIndices()
{
	b3_gl_line * gPtr = *glGridElements;
	b3_index    x, y, i = 0;
	b3_count    max;

	// horizontal splines
	max = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[1]);
	for (y = 0; y < max; y++)
	{
		for (x = 1; x < m_Spline[0].m_SubDiv; x++)
		{
			B3_GL_LINIT(gPtr, i + x - 1, i + x);
		}

		B3_GL_LINIT(gPtr, i + x - 1, m_Spline[0].m_Closed ? i : i + x);

		i += (m_Spline[0].m_SubDiv + 1);
	}

	// vertical splines
	max = B3_BSPLINE_SEGMENTKNOTS(&m_Spline[0]);
	for (x = 0; x < max; x++)
	{
		for (y = 1; y < m_Spline[1].m_SubDiv; y++)
		{
			B3_GL_LINIT(gPtr, i + y - 1, i + y);
		}

		B3_GL_LINIT(gPtr, i + y - 1, m_Spline[1].m_Closed ? i : i + y);

		i += (m_Spline[1].m_SubDiv + 1);
	}
}

void b3SplineShape::b3ComputeSolidIndices()
{
	b3_gl_polygon * pPtr;
	b3_index       x, y;
	b3_count       xSubDiv = m_Spline[0].m_SubDiv, xModulo, xOffset;
	b3_count       ySubDiv = m_Spline[1].m_SubDiv, yModulo;

	pPtr = *glPolygonElements;
	xModulo = m_Spline[0].m_Closed ? xSubDiv : xSubDiv + 1;
	yModulo = m_Spline[1].m_Closed ? ySubDiv : ySubDiv + 1;
	xOffset = m_Spline[0].m_SubDiv + 1;
	for (y = 0; y < ySubDiv; y++)
	{
		for (x = 0; x < xSubDiv; x++)
		{
			B3_GL_PINIT(pPtr,
				m_GridVertexCount +  x              + xOffset *   y,
				m_GridVertexCount + (x + 1) % xModulo + xOffset *   y,
				m_GridVertexCount +  x              + xOffset * ((y + 1) % yModulo));

			B3_GL_PINIT(pPtr,
				m_GridVertexCount + (x + 1) % xModulo + xOffset * ((y + 1) % yModulo),
				m_GridVertexCount +  x              + xOffset * ((y + 1) % yModulo),
				m_GridVertexCount + (x + 1) % xModulo + xOffset *   y);
		}
	}
	glPolygonElements->b3SetCount(xSubDiv * ySubDiv * 2);
}

void b3SplineShape::b3ComputeIndices()
{
	if (glGridElements->b3GetCount() > 0)
	{
		b3ComputeGridIndices();
	}
	if (glPolygonElements->b3GetCount() > 0)
	{
		b3ComputeSolidIndices();
	}
}

void b3SplineShape::b3GetVertexRange(b3_index & start, b3_index & end)
{
	start = m_GridVertexCount;
	end   = m_GridVertexCount + m_ySubDiv * (m_Spline[0].m_SubDiv + 1);
}

#ifdef GLU_NURBS
void b3SplineShape::b3Draw()
{
	// This doesn't work due to the fact that Linux GLU
	// doesn't support NURBS rendering. This doesn't matter
	// because the GLU NURBS support isn't very fast...
	if (!glSolid)
	{
		b3RenderObject::b3Draw();
	}
	else
	{
		b3_color color;

		b3GetDiffuseColor(&color);

		// Leave this for documentation purposes...
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(color.r, color.g, color.b);
		gluBeginSurface(glNURBS);
		gluNurbsSurface(glNURBS,
			m_Spline[0].m_KnotNum, m_Spline[0].m_Knots,
			m_Spline[1].m_KnotNum, m_Spline[1].m_Knots,
			m_Spline[0].m_Offset * 3,
			m_Spline[1].m_Offset * 3,
			(GLfloat *)m_Controls,
			m_Spline[0].degree + 1,
			m_Spline[1].degree + 1,
			GL_MAP2_VERTEX_3);
		gluEndSurface(glNURBS);
	}
}
#endif

void b3SplineShape::b3Transform(b3_matrix * transformation, b3_bool is_affine)
{
	b3_vector * control;
	b3_index   offset;
	b3_index   x, y;

	control = m_Spline[0].m_Controls;
	offset  = m_Spline[0].m_Offset;

	// Transform control points
	for (y = 0; y < m_Spline[1].m_ControlNum; y++)
	{
		control  = &m_Spline[0].m_Controls[y * m_Spline[1].m_Offset];
		for (x = 0; x < m_Spline[0].m_ControlNum; x++)
		{
			b3Vector::b3MatrixMul4D(transformation, control);
			control += offset;
		}
	}
	b3TriangleShape::b3Transform(transformation, is_affine);
}

void b3SplineShape::b3SetupPicking(b3PickInfo * info)
{
	b3_vector  *  control;
	b3_index      offset;
	b3_index      x, y;

	control = m_Spline[0].m_Controls;
	offset  = m_Spline[0].m_Offset;

	// Transform control points
	for (y = 0; y < m_Spline[1].m_ControlNum; y++)
	{
		control  = &m_Spline[0].m_Controls[y * m_Spline[1].m_Offset];
		for (x = 0; x < m_Spline[0].m_ControlNum; x++)
		{
			info->b3AddPickPoint(control);
			control += offset;
		}
	}
}

void b3SplineShape::b3SetupGrid(b3PickInfo * info)
{
	b3_vector  *  control;
	b3_index      offset;
	b3_index      x, y, start;

	control = m_Spline[0].m_Controls;
	offset  = m_Spline[0].m_Offset;

	// Transform control points
	for (y = 0; y < m_Spline[1].m_ControlNum; y++)
	{
		control  = &m_Spline[0].m_Controls[y * m_Spline[1].m_Offset];
		for (x = 0; x < m_Spline[0].m_ControlNum; x++)
		{
			info->b3AddVertex(control);
			control += offset;
		}
	}
	// Compute horizontal grid lines
	for (y = 0; y < m_Spline[1].m_ControlNum; y++)
	{
		start = y * m_Spline[0].m_ControlNum;
		for (x = 1; x < m_Spline[0].m_ControlNum; x++)
		{
			info->b3AddLine(
				start + x - 1,
				start + x);
		}
		if (m_Spline[0].m_Closed)
		{
			info->b3AddLine(start, start + m_Spline[0].m_ControlNum - 1);
		}
	}

	// Compute vertical grid lines
	offset = m_Spline[0].m_ControlNum;
	for (x = 0; x < m_Spline[0].m_ControlNum; x++)
	{
		for (y = 1; y < m_Spline[1].m_ControlNum; y++)
		{
			info->b3AddLine(
				x + (y - 1) * offset,
				x +  y      * offset);
		}
		if (m_Spline[1].m_Closed)
		{
			info->b3AddLine(x, x + (m_Spline[1].m_ControlNum - 1) * offset);
		}
	}
}

b3_bool b3SplineShape::b3Prepare(b3_preparation_info * prep_info)
{
	b3_vertex  * Vertex;
	b3_triangle * Triangle;
	b3_vector  * Vector;
	b3_vector  * Between;
	b3Spline     MySpline;
	b3_res       xSize, ySize, x, y;
	b3_count     SubDiv, TriaCount, VertexCount;
	b3_vector    VertexField[B3_MAX_SUBDIV + 1];

	Between = (b3_vector *)b3Item::b3Alloc(sizeof(b3_vector) *
			(B3_MAX_SUBDIV + 1) * (B3_MAX_SUBDIV + 1));
	if (Between == nullptr)
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}

	xSize       = m_Spline[0].m_SubDiv;
	ySize       = m_Spline[1].m_SubDiv;
	TriaCount = xSize * ySize * 2;

	if (!m_Spline[0].m_Closed)
	{
		xSize++;
	}
	if (!m_Spline[1].m_Closed)
	{
		ySize++;
	}
	VertexCount = xSize * ySize;

	// Reallocating new tria shape
	if (!b3TriangleShape::b3Init(VertexCount, TriaCount, m_Spline[0].m_SubDiv, m_Spline[1].m_SubDiv))
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}

	// building horizontal splines
	Vector = Between;
	SubDiv = m_Spline[0].m_SubDiv + 1;
	for (x = 0; x < SubDiv; x++)
	{
		Vector += m_Spline[1].b3DeBoor(Vector, x * m_Spline[0].m_Offset);
	}

	MySpline            = m_Spline[0];
	MySpline.m_Offset   = m_Spline[1].m_SubDiv + 1;
	MySpline.m_Controls = Between;

	Vertex = m_Vertices;
	for (y = 0; y < ySize; y++)
	{
		MySpline.b3DeBoor(VertexField, y);
		for (x = 0; x < xSize; x++)
		{
			Vertex->Point.x = VertexField[x].x;
			Vertex->Point.y = VertexField[x].y;
			Vertex->Point.z = VertexField[x].z;
			Vertex++;
		}
	}

	Triangle = m_Triangles;
	for (y = 0; y < m_Spline[1].m_SubDiv; y++)
	{
		for (x = 0; x < m_Spline[0].m_SubDiv; x++)
		{
			Triangle->P1  =  x            + xSize *  y;
			Triangle->P2  = (x + 1) % xSize + xSize *  y;
			Triangle->P3  =  x            + xSize * ((y + 1) % ySize);
			Triangle++;

			Triangle->P1  = (x + 1) % xSize + xSize * ((y + 1) % ySize);
			Triangle->P2  =  x            + xSize * ((y + 1) % ySize);
			Triangle->P3  = (x + 1) % xSize + xSize *  y;
			Triangle++;
		}
	}

	m_xSize = m_Spline[0].m_SubDiv;
	m_ySize = m_Spline[1].m_SubDiv;
	m_Flags = B3_PHONG;

	b3Item::b3Free(Between);

	return b3TriangleShape::b3Prepare(prep_info);
}
