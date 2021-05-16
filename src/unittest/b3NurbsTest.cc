/*
**
**	$Filename:	b3NurbsTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for NURBS.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
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

#include "b3NurbsTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3NurbsClosedCurveTest);
CPPUNIT_TEST_SUITE_REGISTRATION(b3NurbsOpenedCurveTest);
CPPUNIT_TEST_SUITE_REGISTRATION(b3NurbsSurfaceTest);

/*************************************************************************
**                                                                      **
**                        Init methods for NURBS testing                **
**                                                                      **
*************************************************************************/

void b3NurbsCurveTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

/**
 * This initializes the control points for a circled NURBS. Against most
 * articles in the internet they are not located on a square but rather on
 * a circle. The rational weights are 1.0 on the bounding boxes edges and
 * 1.0 / sqrt(2.0) on the corners as everywhere stated.
 *
 * @see "Computer Graphics - Principals and Practice", 2nd edition, page 504.
 */
void b3NurbsCurveTest::b3InitArrays()
{
	bzero(m_Deboor,     sizeof(m_Deboor));
	bzero(m_Mansfield,  sizeof(m_Mansfield));
	bzero(m_Radius,     sizeof(m_Radius));
	bzero(m_BasisCoeff, sizeof(m_BasisCoeff));
}

void b3NurbsCurveTest::b3InitControlPoints()
{
	for (unsigned i = 0; i < m_Nurbs.m_ControlNum; i++)
	{
		const double angle = i * 2.0 * M_PI;

		m_Controls[i].x = cos(angle / m_Nurbs.m_ControlNum) * RADIUS;
		m_Controls[i].y = sin(angle / m_Nurbs.m_ControlNum) * RADIUS;
		m_Controls[i].z = 0;
		m_Controls[i].w = i & 1 ? sqrt(2.0) * 0.5 : 1.0;
	}
}

void b3NurbsCurveTest::testInsertValidation()
{
	const b3_f64 half = (m_Nurbs.b3FirstKnot() + m_Nurbs.b3LastKnot()) * 0.5;

	CPPUNIT_ASSERT(!m_Nurbs.b3InsertCurveControl(half, 0));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_LOW_MULTIPLICATION, m_Nurbs.bspline_errno);

	CPPUNIT_ASSERT(!m_Nurbs.b3InsertCurveControl(half, b3Nurbs::B3_MAX_CONTROLS));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXCONTROLS, m_Nurbs.bspline_errno);

	CPPUNIT_ASSERT(!m_Nurbs.b3InsertCurveControl(half, b3Nurbs::B3_MAX_KNOTS));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXKNOTS, m_Nurbs.bspline_errno);
}

void b3NurbsCurveTest::testInsertion()
{
	const b3_f64 quarter = b3Math::b3Mix(m_Nurbs.b3FirstKnot(), m_Nurbs.b3LastKnot(), 0.25f);
	const b3_f64 half    = b3Math::b3Mix(m_Nurbs.b3FirstKnot(), m_Nurbs.b3LastKnot(), 0.5f);

	CPPUNIT_ASSERT(m_Nurbs.b3InsertCurveControl(half));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, m_Nurbs.bspline_errno);

	CPPUNIT_ASSERT(m_Nurbs.b3InsertCurveControl(quarter, 2));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, m_Nurbs.bspline_errno);

	testCircle();
}

/*************************************************************************
**                                                                      **
**                        Unit tests for closed NURBS                   **
**                                                                      **
*************************************************************************/

void b3NurbsClosedCurveTest::b3InitKnotVector()
{
	for (unsigned i = 0; i < m_Nurbs.m_KnotNum; i++)
	{
		m_Knots[i] = std::ceil(i * 0.5);
	}
}

void b3NurbsClosedCurveTest::setUp()
{
	m_Nurbs.m_Knots    = m_Knots;
	m_Nurbs.m_Controls = m_Controls;
	m_Nurbs.b3InitCurve(2, 8, true);

	b3InitArrays();
	b3InitControlPoints();
	b3InitKnotVector();

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3NurbsClosedCurveTest::testCircle()
{
	const b3_f64   range = m_Nurbs.b3KnotRange();
	CPPUNIT_ASSERT_EQUAL(4.0, range);

	const unsigned count = m_Nurbs.b3DeBoor(m_Deboor);
	CPPUNIT_ASSERT_EQUAL(m_Nurbs.m_SubDiv + 1, count);

	for (unsigned s = 0; s < m_Nurbs.m_SubDiv; s++)
	{
		const b3_f64 q   = m_Nurbs.b3FirstKnot() + s * range / m_Nurbs.m_SubDiv;
		const unsigned i = m_Nurbs.b3Mansfield(m_BasisCoeff, q);

		m_Nurbs.b3MansfieldVector(&m_Mansfield[s], m_BasisCoeff, i);

		// Compare De Boor computed values agains Mansfield computed ones
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].x, m_Mansfield[s].x, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].y, m_Mansfield[s].y, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].z, m_Mansfield[s].z, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].w, m_Mansfield[s].w, b3Nurbs::epsilon);

		// Validate radius agains circle.
		m_Radius[s] = sqrt(
				m_Mansfield[s].x * m_Mansfield[s].x +
				m_Mansfield[s].y * m_Mansfield[s].y +
				m_Mansfield[s].z * m_Mansfield[s].z);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, m_Radius[s], b3Nurbs::epsilon);

		// Validate if q is inside range.
		CPPUNIT_ASSERT_GREATEREQUAL( m_Nurbs.b3FirstKnot(), b3Nurbs::b3_knot(q));
		CPPUNIT_ASSERT_LESSEQUAL(    m_Nurbs.b3LastKnot(),  b3Nurbs::b3_knot(q));
	}
	b3PrintF(B3LOG_DEBUG, "%p\n", m_Radius);
}

/*************************************************************************
**                                                                      **
**                        Unit test for opened NURBS                    **
**                                                                      **
*************************************************************************/

void b3NurbsOpenedCurveTest::b3InitKnotVector()
{
	for (unsigned i = 0; i < m_Nurbs.m_KnotNum; i++)
	{
		m_Knots[i] = std::max(0.0, std::ceil(i * 0.5 - 1.0));
	}
}

void b3NurbsOpenedCurveTest::setUp()
{
	m_Nurbs.m_Knots    = m_Knots;
	m_Nurbs.m_Controls = m_Controls;
	m_Nurbs.b3InitCurve(2, 8, false);

	b3InitArrays();
	b3InitControlPoints();
	b3InitKnotVector();

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3NurbsOpenedCurveTest::testCircle()
{
	const b3_f64   range = m_Nurbs.b3KnotRange();
	CPPUNIT_ASSERT_EQUAL(3.0, range);

	const unsigned count = m_Nurbs.b3DeBoor(m_Deboor);
	CPPUNIT_ASSERT_EQUAL(m_Nurbs.m_SubDiv + 1, count);

	for (unsigned s = 0; s <= m_Nurbs.m_SubDiv; s++)
	{
		const b3_f64 q   = m_Nurbs.b3FirstKnot() + s * range / m_Nurbs.m_SubDiv;
		const unsigned i = m_Nurbs.b3Mansfield(m_BasisCoeff, q);

		m_Nurbs.b3MansfieldVector(&m_Mansfield[s], m_BasisCoeff, i);

		// Compare De Boor computed values agains Mansfield computed ones
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].x, m_Mansfield[s].x, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].y, m_Mansfield[s].y, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].z, m_Mansfield[s].z, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].w, m_Mansfield[s].w, b3Nurbs::epsilon);

		// Validate radius agains circle.
		m_Radius[s] = sqrt(
				m_Mansfield[s].x * m_Mansfield[s].x +
				m_Mansfield[s].y * m_Mansfield[s].y +
				m_Mansfield[s].z * m_Mansfield[s].z);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, m_Radius[s], b3Nurbs::epsilon);

		// Validate if q is inside range.
		CPPUNIT_ASSERT_GREATEREQUAL( m_Nurbs.b3FirstKnot(), b3Nurbs::b3_knot(q));
		CPPUNIT_ASSERT_LESSEQUAL(    m_Nurbs.b3LastKnot(),  b3Nurbs::b3_knot(q));
	}
	b3PrintF(B3LOG_DEBUG, "%p\n", m_Radius);
}

#include "blz3/base/b3Vector.h"

void b3NurbsOpenedCurveTest::test()
{
	static const b3_f32 corner = sqrt(2.0) * 0.5;

	static b3_vector4D controls[]
	{
		{  1,  0, 0, 1 },
		{  1,  1, 0, corner },
		{  0,  1, 0, 1 },
		{ -1,  1, 0, corner },
		{ -1,  0, 0, 1 },
		{ -1, -1, 0, corner },
		{  0, -1, 0, 1 },
		{  1, -1, 0, corner },
		{  1,  0, 0, 1 }
	};

	static b3Nurbs::b3_knot knots[]
	{
		0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4
	};

	const unsigned control_count = sizeof(controls) / sizeof(controls[0]);
	const unsigned knot_count    = sizeof(knots) / sizeof(knots[0]);
	const unsigned degree        = 2;

	CPPUNIT_ASSERT_EQUAL(control_count + degree + 1, knot_count);

	b3Nurbs nurbs;

	nurbs.m_Controls   = controls;
	nurbs.m_ControlMax = control_count;
	nurbs.m_Knots      = knots;
	nurbs.m_KnotMax    = knot_count;
	CPPUNIT_ASSERT(nurbs.b3InitCurve(degree, control_count, false));
	CPPUNIT_ASSERT_EQUAL(control_count, nurbs.m_ControlNum);
	CPPUNIT_ASSERT_EQUAL(knot_count,    nurbs.m_KnotNum);
	CPPUNIT_ASSERT_EQUAL(degree,        nurbs.m_Degree);

	for (unsigned i = degree; i < control_count - degree; i++)
	{
		const float q = knots[i] * 0.75 + knots[i + 1] * 0.25;

		if (q > knots[i])
		{
			b3_vector4D point;

			nurbs.b3DeBoorOpened(&point, q);

			const double radius = sqrt(point.x * point.x + point.y * point.y);
			CPPUNIT_ASSERT_GREATER(0.0, radius);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Unit test for NURBS surfaces                  **
**                                                                      **
*************************************************************************/

void b3NurbsSurfaceTest::setUp()
{
	m_Horizontal.m_Knots    = m_HorizontalKnots;
	m_Horizontal.m_Controls = m_Controls;
	m_Horizontal.b3InitCurve(2, 8, true);

	m_Vertical.m_Knots      = m_VerticalKnots;
	m_Vertical.m_Controls   = m_Controls;
	m_Vertical.b3InitCurve(2, 5, false);
	m_Vertical.m_Offset     = m_Horizontal.m_ControlMax;

	for (unsigned i = 0; i < m_Horizontal.m_KnotNum; i++)
	{
		m_HorizontalKnots[i] = std::ceil(i * 0.5);
	}
	for (unsigned i = 0; i < m_Vertical.m_KnotNum; i++)
	{
		m_VerticalKnots[i] = std::max(0.0, std::ceil(i * 0.5 - 1.0));
	}

	for (unsigned y = 0; y < m_Vertical.m_ControlNum; y++)
	{
		const double y_angle = y * M_PI / (m_Vertical.m_ControlNum - 1);
		const double height = sin(y_angle);
		unsigned i = y * m_Horizontal.m_ControlMax;

		for (unsigned x = 0; x < m_Horizontal.m_ControlNum; x++)
		{
			const double   x_angle = x * 2.0 * M_PI / m_Horizontal.m_ControlNum;
			const unsigned level   = (x & 1) + (y & 1);

			m_Controls[i].x =  cos(x_angle) * RADIUS * height;
			m_Controls[i].y =  sin(x_angle) * RADIUS * height;
			m_Controls[i].z = -cos(y_angle) * RADIUS;

			switch (level)
			{
			case 0:
				m_Controls[i].w = 1;
				break;
			case 1:
				m_Controls[i].w = sqrt(2.0) * 0.5;
				break;
			case 2:
				m_Controls[i].w = 0.5;
				break;

			default:
				// Intentionally do nothing
				break;
			}

			i++;
		}
	}

	bzero(m_Deboor,     sizeof(m_Deboor));
	bzero(m_Mansfield,  sizeof(m_Mansfield));
	bzero(m_Radius,     sizeof(m_Radius));
	bzero(m_BasisCoeff, sizeof(m_BasisCoeff));

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3NurbsSurfaceTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3NurbsSurfaceTest::testSphereHorizontally()
{
	const b3_f64   x_range = m_Horizontal.b3KnotRange();
	const b3_f64   y_range = m_Vertical.b3KnotRange();

	CPPUNIT_ASSERT_EQUAL(4.0, x_range);
	CPPUNIT_ASSERT_EQUAL(2.0, y_range);

	b3_vector4D aux_control_points[b3Nurbs::B3_MAX_CONTROLS * b3Nurbs::B3_MAX_CONTROLS];
	b3Nurbs     aux_nurbs;

	// building horizontal splines
	// first create controls for segments of vertical spline...
	b3Nurbs::b3DeBoorSurfaceControl(&m_Horizontal, &m_Vertical, aux_control_points);
	aux_nurbs            = m_Horizontal;
	aux_nurbs.m_Offset   = m_Vertical.b3GetSegmentKnotCount();
	aux_nurbs.m_Controls = aux_control_points;

	for (b3_index y = 0; y < aux_nurbs.m_Offset; y++)
	{
		const unsigned count = aux_nurbs.b3DeBoor(m_Deboor, y);
		CPPUNIT_ASSERT_EQUAL(aux_nurbs.m_SubDiv + 1, count);

		const b3_f64 range = aux_nurbs.b3KnotRange();

		for (unsigned s = 0; s <= aux_nurbs.m_SubDiv; s++)
		{
			const b3_f64 q   = aux_nurbs.b3FirstKnot() + s * range / aux_nurbs.m_SubDiv;
			const unsigned i = aux_nurbs.b3Mansfield(m_BasisCoeff, q);

			aux_nurbs.b3MansfieldVector(&m_Mansfield[s], m_BasisCoeff, i, y);

			// Compare De Boor computed values agains Mansfield computed ones
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].x, m_Mansfield[s].x, b3Nurbs::epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].y, m_Mansfield[s].y, b3Nurbs::epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].z, m_Mansfield[s].z, b3Nurbs::epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].w, m_Mansfield[s].w, b3Nurbs::epsilon);

			// Validate radius agains circle.
			m_Radius[s] = sqrt(
					m_Mansfield[s].x * m_Mansfield[s].x +
					m_Mansfield[s].y * m_Mansfield[s].y +
					m_Mansfield[s].z * m_Mansfield[s].z);
#if 0
			CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, m_Radius[s], b3Nurbs::epsilon);
#else
			b3PrintF(B3LOG_FULL, "Radius: %p\n", m_Radius);
#endif
		}
	}
}

void b3NurbsSurfaceTest::testSphereVertically()
{
	const b3_f64   x_range = m_Horizontal.b3KnotRange();
	const b3_f64   y_range = m_Vertical.b3KnotRange();

	CPPUNIT_ASSERT_EQUAL(4.0, x_range);
	CPPUNIT_ASSERT_EQUAL(2.0, y_range);

	b3_vector4D aux_control_points[b3Nurbs::B3_MAX_CONTROLS * b3Nurbs::B3_MAX_CONTROLS];
	b3Nurbs     aux_nurbs;

	// building horizontal splines
	// first create controls for segments of vertical spline...
	b3Nurbs::b3DeBoorSurfaceControl(&m_Vertical, &m_Horizontal, aux_control_points);
	aux_nurbs            = m_Vertical;
	aux_nurbs.m_Offset   = m_Horizontal.b3GetSegmentKnotCount();
	aux_nurbs.m_Controls = aux_control_points;

	for (b3_index y = 0; y < aux_nurbs.m_Offset; y++)
	{
		const unsigned count = aux_nurbs.b3DeBoor(m_Deboor, y);
		CPPUNIT_ASSERT_EQUAL(aux_nurbs.m_SubDiv + 1, count);

		const b3_f64 range = aux_nurbs.b3KnotRange();

		for (unsigned s = 0; s <= aux_nurbs.m_SubDiv; s++)
		{
			const b3_f64 q   = aux_nurbs.b3FirstKnot() + s * range / aux_nurbs.m_SubDiv;
			const unsigned i = aux_nurbs.b3Mansfield(m_BasisCoeff, q);

			aux_nurbs.b3MansfieldVector(&m_Mansfield[s], m_BasisCoeff, i, y);

			// Compare De Boor computed values agains Mansfield computed ones
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].x, m_Mansfield[s].x, b3Nurbs::epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].y, m_Mansfield[s].y, b3Nurbs::epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].z, m_Mansfield[s].z, b3Nurbs::epsilon);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(m_Deboor[s].w, m_Mansfield[s].w, b3Nurbs::epsilon);

			// Validate radius agains circle.
			m_Radius[s] = sqrt(
					m_Mansfield[s].x * m_Mansfield[s].x +
					m_Mansfield[s].y * m_Mansfield[s].y +
					m_Mansfield[s].z * m_Mansfield[s].z);
#if 0
			CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, m_Radius[s], b3Nurbs::epsilon);
#else
			b3PrintF(B3LOG_FULL, "Radius: %p\n", m_Radius);
#endif
		}
	}
}

#endif
