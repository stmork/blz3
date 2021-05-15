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

#endif
