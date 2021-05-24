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
		const double angle = i * 2.0 * M_PI / m_Nurbs.m_ControlNum;

		// This is the representation often found in the internet.
		m_Controls[i].x = std::round(cos(angle)) * RADIUS;
		m_Controls[i].y = std::round(sin(angle)) * RADIUS;
		m_Controls[i].z = 0;
		m_Controls[i].w = i & 1 ? sqrt(2.0) * 0.5 : 1.0;

		// This is the representation needed by De Boor's algorithm.
		b3SplineVector::b3WeightSelf(m_Controls[i]);
	}
}

void b3NurbsCurveTest::testFind()
{
	const b3_f64 scale = 8.0;

	m_Nurbs.m_KnotNum = 20;
	for (unsigned j = 0; j < 100; j++)
	{
		for (unsigned k = 0; k < m_Nurbs.m_KnotNum; k++)
		{
			m_Nurbs.m_Knots[k] = B3_IRAN(m_Nurbs.m_KnotNum * 3 / 4);
		}
		std::sort(m_Nurbs.m_Knots, m_Nurbs.m_Knots + m_Nurbs.m_KnotNum);

		const b3_f64 range = m_Nurbs.b3KnotRange();

		for (unsigned k = 0; k < 100; k++)
		{
			const b3Nurbs::b3_knot q = m_Nurbs.b3FirstKnot() + B3_IRAN(range * scale) / scale;
			const unsigned         i = m_Nurbs.iFind(q);

			CPPUNIT_ASSERT_GREATEREQUAL(m_Nurbs.m_Knots[i], q);
			CPPUNIT_ASSERT_LESS(m_Nurbs.m_Knots[i + 1], q);
		}
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

		m_Nurbs.b3MansfieldVector(m_Mansfield[s], m_BasisCoeff, i);
		b3SplineVector::b3Homogenize(m_Mansfield[s]);

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
		const b3_f64   q   = m_Nurbs.b3FirstKnot() + s * range / m_Nurbs.m_SubDiv;
		const unsigned i   = m_Nurbs.b3Mansfield(m_BasisCoeff, q);
		b3_f64         sum = 0.0;

		for (unsigned d = 0; d <= m_Nurbs.m_Degree; d++)
		{
			sum += m_BasisCoeff[d];
		}
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, sum, b3Nurbs::epsilon);

		m_Nurbs.b3MansfieldVector(m_Mansfield[s], m_BasisCoeff, i);
		b3SplineVector::b3Homogenize(m_Mansfield[s]);

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

void b3NurbsOpenedCurveTest::testBezier()
{
	CPPUNIT_ASSERT(m_Nurbs.b3InitCurve(2, 3, false));
	CPPUNIT_ASSERT(m_Nurbs.b3ToBezier());

	CPPUNIT_ASSERT_EQUAL(m_Nurbs.m_ControlNum * 2, m_Nurbs.m_KnotNum);
	for (unsigned i = 1; i < m_Nurbs.m_ControlNum; i++)
	{
		CPPUNIT_ASSERT_EQUAL(
			m_Nurbs.m_Knots[i - 1],
			m_Nurbs.m_Knots[i]);
		CPPUNIT_ASSERT_EQUAL(
			m_Nurbs.m_Knots[m_Nurbs.m_ControlNum - i],
			m_Nurbs.m_Knots[m_Nurbs.m_ControlNum - i - 1]);
	}

	CPPUNIT_ASSERT_EQUAL(m_Nurbs.m_Knots[0], m_Nurbs.b3FirstKnot());
	CPPUNIT_ASSERT_EQUAL(
		m_Nurbs.m_Knots[m_Nurbs.m_KnotNum - 1],
		m_Nurbs.b3LastKnot());
	CPPUNIT_ASSERT_EQUAL(
		m_Nurbs.m_Knots[m_Nurbs.m_KnotNum - 1] - m_Nurbs.m_Knots[0],
		m_Nurbs.b3KnotRange());

	m_Controls[1].x =  RADIUS;
	m_Controls[1].y =  RADIUS;
	b3SplineVector::b3WeightSelf(m_Controls[1]);

	m_Controls[3].x = -RADIUS;
	m_Controls[3].y =  RADIUS;
	b3SplineVector::b3WeightSelf(m_Controls[3]);

	b3_vector4D r;
	const b3_vector4D & a = b3SplineVector::b3Mix(m_Controls[0], m_Controls[1], 0.5);
	const b3_vector4D & b = b3SplineVector::b3Mix(m_Controls[1], m_Controls[2], 0.5);
	const b3_vector4D & c = b3SplineVector::b3Mix(a, b, 0.5);
	b3SplineVector::b3Homogenize(r = c);

	const unsigned i = m_Nurbs.b3Mansfield(m_BasisCoeff, 0.5);
	m_Nurbs.b3MansfieldVector(r, m_BasisCoeff, i);
	b3SplineVector::b3Homogenize(r);

	CPPUNIT_ASSERT_EQUAL(c.w, r.w);
	for (b3_f64 q = m_Nurbs.b3FirstKnot();
		q <= m_Nurbs.b3LastKnot();
		q += m_Nurbs.b3KnotRange() * 0.125)
	{
		b3_vector4D point;

		m_Nurbs.b3DeBoorOpened(point, q);

		const b3_f64 radius = sqrt(
				point.x * point.x +
				point.y * point.y +
				point.z * point.z);
		CPPUNIT_ASSERT_GREATER(0.0, radius);
		CPPUNIT_ASSERT_EQUAL(RADIUS, radius);
	}
}

void b3NurbsOpenedCurveTest::testArc()
{
	m_Nurbs.b3InitCurve(2, 4, false);

	m_Controls[0].x =  1;
	m_Controls[0].y =  0;
	m_Controls[0].w =  1;
	m_Controls[1].x =  1;
	m_Controls[1].y =  1;
	m_Controls[1].w =  0.5;
	m_Controls[2].x = -1;
	m_Controls[2].y =  1;
	m_Controls[2].w =  0.5;
	m_Controls[3].x = -1;
	m_Controls[3].y =  0;
	m_Controls[3].w =  1;

	m_Knots[0] = m_Knots[1] = m_Knots[2] = 0;
	m_Knots[3] = 1;
	m_Knots[4] = m_Knots[5] = m_Knots[6] = 2;

	for (b3_f64 q = m_Nurbs.b3FirstKnot();
		q <= m_Nurbs.b3LastKnot();
		q += m_Nurbs.b3KnotRange() * 0.125)
	{
		b3Nurbs::type point;

		m_Nurbs.b3DeBoorOpened(point, q);

		const b3_f64 radius = sqrt(
				point.x * point.x +
				point.y * point.y +
				point.z * point.z);
		CPPUNIT_ASSERT_GREATER(0.0, radius);
	}
}

void b3NurbsOpenedCurveTest::test()
{
	static const b3_f32 corner = sqrt(2.0) * 0.5;

	static const b3Nurbs::type controls[]
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

	static const b3Nurbs::b3_knot knots[]
	{
		0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4
	};

	const unsigned control_count = sizeof(controls) / sizeof(controls[0]);
	const unsigned knot_count    = sizeof(knots) / sizeof(knots[0]);
	const unsigned degree        = 2;

	CPPUNIT_ASSERT_EQUAL(control_count + degree + 1, knot_count);

	b3Nurbs nurbs;

	nurbs.m_Controls   = m_Controls;
	nurbs.m_ControlMax = control_count;
	nurbs.m_Knots      = m_Knots;
	nurbs.m_KnotMax    = knot_count;

	CPPUNIT_ASSERT(nurbs.b3InitCurve(degree, control_count, false));
	CPPUNIT_ASSERT_EQUAL(control_count, nurbs.m_ControlNum);
	CPPUNIT_ASSERT_EQUAL(knot_count,    nurbs.m_KnotNum);
	CPPUNIT_ASSERT_EQUAL(degree,        nurbs.m_Degree);

	// Transform rational representation.
	for (unsigned i = 0; i < control_count; i++)
	{
		m_Controls[i] = b3SplineVector::b3WeightInit(controls[i], controls[i].w);
	}

	for (unsigned i = 0; i < knot_count; i++)
	{
		m_Knots[i] = knots[i];
	}

	for (b3_f64 q = nurbs.b3FirstKnot();
		q <= nurbs.b3LastKnot() - nurbs.epsilon;
		q += nurbs.b3KnotRange() * 0.125)
	{
		const unsigned i = nurbs.iFind(q);

		b3Nurbs::type b3_result;

		/****** Test using De Boor algorithm (blz3 implementation) ******/
		nurbs.b3DeBoorOpened(b3_result, q);

		const b3_f64 b3_radius = sqrt(
				b3_result.x * b3_result.x +
				b3_result.y * b3_result.y +
				b3_result.z * b3_result.z);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b3_radius, b3Nurbs::epsilon);

		/****** Test using Mansfield algorithm (blz3 implementation) ******/
		const unsigned idx = nurbs.b3Mansfield(m_BasisCoeff, q);
		b3_f64         sum = 0.0;
		b3Nurbs::type  mf_result;

		for (unsigned d = 0; d <= nurbs.m_Degree; d++)
		{
			sum += m_BasisCoeff[d];
		}
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, sum, b3Nurbs::epsilon);

		nurbs.b3MansfieldVector(mf_result, m_BasisCoeff, idx);
		b3SplineVector::b3Homogenize(mf_result);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(mf_result.x, b3_result.x, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(mf_result.y, b3_result.y, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(mf_result.z, b3_result.z, b3Nurbs::epsilon);

		const b3_f64 mf_radius = sqrt(
				mf_result.x * mf_result.x +
				mf_result.y * mf_result.y +
				mf_result.z * mf_result.z);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mf_radius, b3Nurbs::epsilon);

		/****** Test using De Boor algorithm (tinynurbs implementation) ******/
		b3Vector32 db_result   = tinyNurbsDeBoor(nurbs, i, q);
		db_result[b3_vector_index::W] = 0;
		const b3_f64 db_radius = db_result.b3Length();

		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, db_radius, b3Nurbs::epsilon);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(db_result[b3_vector_index::X], b3_result.x, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(db_result[b3_vector_index::Y], b3_result.y, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(db_result[b3_vector_index::Z], b3_result.z, b3Nurbs::epsilon);

		/****** Test using recursive De Boor algorithm ******/
		b3Nurbs::type rc_result = recursiveDeBoor(nurbs, nurbs.m_Degree, i, q);

		b3SplineVector::b3Homogenize(rc_result);
		const b3_f64        rc_radius = sqrt(
				rc_result.x * rc_result.x +
				rc_result.y * rc_result.y +
				rc_result.z * rc_result.z);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rc_radius, b3Nurbs::epsilon);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(rc_result.x, b3_result.x, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(rc_result.y, b3_result.y, b3Nurbs::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(rc_result.z, b3_result.z, b3Nurbs::epsilon);
	}
}

b3Vector32 b3NurbsOpenedCurveTest::tinyNurbsDeBoor(
	const b3Nurbs  &  nurbs,
	const unsigned    i,
	const b3_f64      q)
{
	/*
	 * Compute basis functions.
	 *
	 * Idea from:
	 * https://github.com/pradeep-pyro/tinynurbs/blob/master/include/tinynurbs/core/basis.h
	 */
	b3_f64  N[nurbs.m_Degree + 1];
	b3_f64  left[nurbs.m_Degree + 1];
	b3_f64  right[nurbs.m_Degree + 1];

	N[0]     = 1.0; // Definitely needed.
	left[0]  = 0.0; // Not needed, only nice output.
	right[0] = 0.0; // Not needed, only nice output.
	for (unsigned j = 1; j <= nurbs.m_Degree; j++)
	{
		b3_f64  saved = 0.0;

		left[j]  = (q - nurbs.m_Knots[i + 1 - j]);
		right[j] = nurbs.m_Knots[i + j] - q;

		for (unsigned r = 0; r < j; r++)
		{
			const b3_f64 temp = N[r] / (right[r + 1] + left[j - r]);

			N[r]  = saved + right[r + 1] * temp;
			saved = left[j - r] * temp;
		}
		N[j] = saved;
	}

	/*
	 * Evaluate basis funtions into point:
	 *
	 * Idea from:
	 * https://github.com/pradeep-pyro/tinynurbs/blob/master/include/tinynurbs/core/evaluate.h
	 */
	b3Vector32 db_result(0);

	b3_f64 sum = 0.0;
	for (unsigned j = 0; j <= nurbs.m_Degree; j++)
	{
		sum += N[j];
		db_result += b3Vector32(nurbs.m_Controls[i - nurbs.m_Degree + j]) * N[j];
	}

	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, sum, b3Nurbs::epsilon);

	const b3_f64 w = db_result[b3_vector_index::W];

	db_result /= w;

	return db_result;
}

b3Nurbs::type b3NurbsOpenedCurveTest::recursiveDeBoor(
	const b3Nurbs & nurbs,
	const unsigned  k,
	const unsigned  i,
	const b3_f64    q)
{
	/**
	 * Idea from:
	 * https://chi3x10.wordpress.com/2009/10/18/de-boor-algorithm-in-c/
	 */
	if ( k == 0)
	{
		return nurbs.m_Controls[i];
	}
	else
	{
		const b3_f64 alpha =
			(q - nurbs.m_Knots[i]) /
			(nurbs.m_Knots[i + nurbs.m_Degree + 1 - k] - nurbs.m_Knots[i]);

		const b3Nurbs::type & aVec = recursiveDeBoor(nurbs, k - 1, i - 1, q);
		const b3Nurbs::type & bVec = recursiveDeBoor(nurbs, k - 1, i,     q);

		return b3SplineVector::b3Mix(aVec, bVec, alpha);
	}
}


/*************************************************************************
**                                                                      **
**                        Unit test for NURBS surfaces                  **
**                                                                      **
*************************************************************************/

/**
 * This setups the control points and knot vectors for a spherical NURBS
 * surface.
 *
 * Look at:
 * https://download.blender.org/documentation/htmlI/ch09s02.html
 */
void b3NurbsSurfaceTest::setUp()
{
	m_Horizontal.m_Knots    = m_HorizontalKnots;
	m_Horizontal.m_Controls = m_Controls;
	m_Horizontal.b3InitCurve(2, 8, true);

	m_Vertical.m_Knots      = m_VerticalKnots;
	m_Vertical.m_Controls   = m_Controls;
	m_Vertical.b3InitCurve(2, 5, false, m_Horizontal.m_ControlMax);

	for (unsigned i = 0; i < m_Horizontal.m_KnotNum; i++)
	{
		m_HorizontalKnots[i] = std::ceil(i * 0.5);
	}
	for (unsigned i = 0; i < m_Vertical.m_KnotNum; i++)
	{
		m_VerticalKnots[i] = std::max(0.0, std::ceil(i * 0.5 - 1.0));
	}
	m_Vertical.b3ThroughEndControl();

	bzero(m_Controls, sizeof(m_Controls));
	for (unsigned y = 0; y < m_Vertical.m_ControlNum; y++)
	{
		const double y_angle = y * M_PI / (m_Vertical.m_ControlNum - 1);
		const double height  = sin(y_angle);
		unsigned     i       = y * m_Horizontal.m_ControlMax;

		for (unsigned x = 0; x < m_Horizontal.m_ControlNum; x++)
		{
			b3Nurbs::type point;

			const double   x_angle = x * 2.0 * M_PI / m_Horizontal.m_ControlNum;
			const unsigned level   = height < b3Math::epsilon ? 0 : (x & 1) | ((y & 1) << 1);

			point.x =  std::round(cos(x_angle) * std::round(height)) * RADIUS;
			point.y =  std::round(sin(x_angle) * std::round(height)) * RADIUS;
			point.z = -std::round(cos(y_angle)) * RADIUS;

			switch (level)
			{
			case 1:
				point.w = sqrt(2.0) * 0.25;
				break;
			case 2:
				point.w = sqrt(2.0) * 0.5;
				break;
			case 3:
				point.w = 0.25;
				break;

			default:
				point.w = 1;
				break;
			}

			m_Controls[i] = b3SplineVector::b3WeightSelf(point);
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
	b3Nurbs::type  aux_control_points[b3Nurbs::B3_MAX_CONTROLS * b3Nurbs::B3_MAX_CONTROLS];
	b3Nurbs        aux_nurbs;
	const b3_f64   x_range = m_Horizontal.b3KnotRange();
	const b3_f64   y_range = m_Vertical.b3KnotRange();

	CPPUNIT_ASSERT_EQUAL(4.0, x_range);
	CPPUNIT_ASSERT_EQUAL(2.0, y_range);

	// Building horizontal splines
	// First create controls for segments of vertical spline...
	bzero(aux_control_points, sizeof(aux_control_points));
	b3Nurbs::b3DeBoorSurfaceControl(m_Horizontal, m_Vertical, aux_nurbs, aux_control_points);
	aux_nurbs.m_SubDiv = m_Horizontal.b3GetSegmentKnotCount() * 2;

	for (b3_index y = 0; y < aux_nurbs.m_Offset; y++)
	{
		const unsigned count = aux_nurbs.b3DeBoor(m_Deboor, y);
		const b3_f64   range = aux_nurbs.b3KnotRange();

		for (unsigned s = 0; s <= aux_nurbs.m_SubDiv; s++)
		{
			const b3_f64   q = aux_nurbs.b3FirstKnot() + s * range / aux_nurbs.m_SubDiv;
			const unsigned i = aux_nurbs.b3Mansfield(m_BasisCoeff, q);

			// Do Mansfield computation just for test purposes.
			aux_nurbs.b3MansfieldVector(m_Mansfield[s], m_BasisCoeff, i, y);
			b3SplineVector::b3Homogenize(m_Mansfield[s]);

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

			CPPUNIT_ASSERT_GREATER(0.0, m_Radius[s]);
#if 0
			CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, m_Radius[s], b3Nurbs::epsilon);
#endif
		}

		CPPUNIT_ASSERT_EQUAL(aux_nurbs.m_SubDiv + 1, count);
	}
}

void b3NurbsSurfaceTest::testSphereVertically()
{
	b3Nurbs::type  aux_control_points[b3Nurbs::B3_MAX_CONTROLS * b3Nurbs::B3_MAX_CONTROLS];
	b3Nurbs        aux_nurbs;
	const b3_f64   x_range = m_Horizontal.b3KnotRange();
	const b3_f64   y_range = m_Vertical.b3KnotRange();

	CPPUNIT_ASSERT_EQUAL(4.0, x_range);
	CPPUNIT_ASSERT_EQUAL(2.0, y_range);

	// Building vertical splines
	// First create controls for segments of horizontal spline...
	bzero(aux_control_points, sizeof(aux_control_points));
	b3Nurbs::b3DeBoorSurfaceControl(m_Vertical, m_Horizontal, aux_nurbs, aux_control_points);
	aux_nurbs.m_SubDiv = m_Vertical.b3GetSegmentKnotCount() * 2;

	for (b3_index x = 0; x < aux_nurbs.m_Offset; x++)
	{
		const unsigned count = aux_nurbs.b3DeBoor(m_Deboor, x);
		const b3_f64   range = aux_nurbs.b3KnotRange();

		for (unsigned s = 0; s <= aux_nurbs.m_SubDiv; s++)
		{
			const b3_f64   q = aux_nurbs.b3FirstKnot() + s * range / aux_nurbs.m_SubDiv;
			const unsigned i = aux_nurbs.b3Mansfield(m_BasisCoeff, q);

			// Do Mansfield computation just for test purposes.
			aux_nurbs.b3MansfieldVector(m_Mansfield[s], m_BasisCoeff, i, x);
			b3SplineVector::b3Homogenize(m_Mansfield[s]);

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

			CPPUNIT_ASSERT_GREATER(0.0, m_Radius[s]);
#if 1
			CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, m_Radius[s], b3Nurbs::epsilon);
#endif
		}

		CPPUNIT_ASSERT_EQUAL(aux_nurbs.m_SubDiv + 1, count);
	}
}

void b3NurbsSurfaceTest::testTesselate()
{
	b3Nurbs         aux_nurbs;
	b3Nurbs::type   aux_control_points[(b3Spline::B3_MAX_CONTROLS + 1) * (b3Spline::B3_MAX_SUBDIV + 1)];
	b3Nurbs::type   aux_result[(b3Nurbs::B3_MAX_SUBDIV + 1) * (b3Nurbs::B3_MAX_SUBDIV + 1)];
	b3Nurbs::type   aux_compare[(b3Nurbs::B3_MAX_SUBDIV + 1) * (b3Nurbs::B3_MAX_SUBDIV + 1)];
	b3Nurbs::type * aux_ptr = aux_control_points;

	m_Horizontal.m_SubDiv = m_Horizontal.b3GetSegmentKnotCount() * 2;
	m_Vertical.m_SubDiv   = m_Vertical.b3GetSegmentKnotCount() * 2;

	// Building a series of vertical splines.
	unsigned  segment_count = m_Horizontal.m_SubDiv;

	if (!m_Horizontal.m_Closed)
	{
		segment_count++;
	}

	// @see b3Spline::b3DeBoorSurfaceControl()
	for (unsigned i = 0; i < segment_count; i++)
	{
		const b3_f64   q     = b3Nurbs::b3FuncSubdivision(m_Horizontal, i);
		const unsigned index = m_Horizontal.b3Mansfield(m_BasisCoeff, q);

		for (unsigned x = 0; x < m_Vertical.m_ControlNum; x++)
		{
			m_Horizontal.b3MansfieldVector(aux_ptr[x * segment_count], m_BasisCoeff,
				index, x * m_Vertical.m_Offset);
		}
		aux_ptr++;
	}

	// Create aux BSpline
	aux_nurbs            = m_Vertical;
	aux_nurbs.m_Offset   = segment_count;
	aux_nurbs.m_Controls = aux_control_points;

	// For every sub division of vertical spline compute horizontal spline.
	aux_ptr = aux_result;
	for (b3_index x = 0; x < aux_nurbs.m_Offset; x++)
	{
		const unsigned count = aux_nurbs.b3DeBoor(aux_ptr, x);

		for (unsigned c = 0; c < count; c++)
		{
			const b3_f64 radius = sqrt(
					aux_ptr[c].x * aux_ptr[c].x +
					aux_ptr[c].y * aux_ptr[c].y +
					aux_ptr[c].z * aux_ptr[c].z);

			m_Radius[c] = radius;
			CPPUNIT_ASSERT_GREATER(0.0, radius);
		}

		CPPUNIT_ASSERT_EQUAL(aux_nurbs.m_SubDiv + 1, count);
		aux_ptr += count;
	}

	// Finally compare against standard implementation.
	const unsigned point_count = b3Nurbs::b3DeBoorSurfaceTesselate(
			m_Horizontal, m_Vertical, aux_compare);

	CPPUNIT_ASSERT_EQUAL(
		segment_count * (m_Vertical.m_SubDiv + 1),
		point_count);
	CPPUNIT_ASSERT(std::equal(aux_compare, aux_compare + point_count, aux_result));
}

void b3NurbsSurfaceTest::testRadius()
{
	b3Nurbs::type   aux_compare[(b3Nurbs::B3_MAX_SUBDIV + 1) * (b3Nurbs::B3_MAX_SUBDIV + 1)];

	// Stepping for knot distance.
	m_Horizontal.m_SubDiv = m_Horizontal.b3GetSegmentKnotCount();

	const unsigned point_count = b3Nurbs::b3DeBoorSurfaceTesselate(
			m_Horizontal, m_Vertical, aux_compare, b3Nurbs::b3FuncKnot);

	CPPUNIT_ASSERT_EQUAL(
		m_Horizontal.m_SubDiv * (m_Vertical.m_SubDiv + 1),
		point_count);

	// Comparing radiuses
	for (unsigned i = 0; i < point_count; i++)
	{
		const b3_f64 radius = sqrt(
				aux_compare[i].x * aux_compare[i].x +
				aux_compare[i].y * aux_compare[i].y +
				aux_compare[i].z * aux_compare[i].z);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, radius, b3Nurbs::epsilon);
	}
}

void b3NurbsSurfaceTest::testInsertSurface()
{
	b3Nurbs::type   aux_compare[(b3Nurbs::B3_MAX_SUBDIV + 1) * (b3Nurbs::B3_MAX_SUBDIV + 1)];
	const b3_f64    q_horizontal = m_Horizontal.b3FirstKnot() + 0.25 * m_Horizontal.b3KnotRange();
	const b3_f64    q_vertical   = m_Vertical.b3FirstKnot()   + 0.75 * m_Vertical.b3KnotRange();

	CPPUNIT_ASSERT(!m_Horizontal.b3InsertSurfaceControl(0, 0, 0, 0));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_LOW_MULTIPLICATION, m_Horizontal.bspline_errno);

	CPPUNIT_ASSERT(m_Horizontal.b3InsertSurfaceControl(q_horizontal, 2,
			m_Vertical.m_Offset,   m_Vertical.m_ControlNum));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, m_Horizontal.bspline_errno);

	CPPUNIT_ASSERT(m_Vertical.b3InsertSurfaceControl(q_vertical, 1,
			m_Horizontal.m_Offset, m_Horizontal.m_ControlNum));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, m_Horizontal.bspline_errno);

	// Stepping for knot distance.
	m_Horizontal.m_SubDiv = m_Horizontal.b3GetSegmentKnotCount();

	const unsigned point_count = b3Nurbs::b3DeBoorSurfaceTesselate(
			m_Horizontal, m_Vertical, aux_compare, b3Nurbs::b3FuncKnot);

	CPPUNIT_ASSERT_EQUAL(
		m_Horizontal.m_SubDiv * (m_Vertical.m_SubDiv + 1),
		point_count);

	// Comparing radiuses
	for (unsigned i = 0; i < point_count; i++)
	{
		const b3_f64 radius = sqrt(
				aux_compare[i].x * aux_compare[i].x +
				aux_compare[i].y * aux_compare[i].y +
				aux_compare[i].z * aux_compare[i].z);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(RADIUS, radius, b3Nurbs::epsilon);
	}
}

#endif
