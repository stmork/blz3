/*
**
**	$Filename:	b3AuxTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Auxiliary for unit tests.
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

#include "b3AuxTest.h"
#include "b3TestMacros.h"

#include <blz3/base/b3Aux.h>
#include <blz3/base/b3Color.h>

/*************************************************************************
**                                                                      **
**                        Unit test for some auxiliary compation jobs   **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3AuxTest);
CPPUNIT_TEST_SUITE_REGISTRATION(b3NurbsSurfaceTest);

void b3AuxTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3AuxTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3AuxTest::testAux()
{
	b3RGB rgb;

	rgb.r = 0x33;
	rgb.g = 0x55;
	rgb.b = 0x77;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x335577, rgb);
	rgb = 0xffffff;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xffffff, rgb);

	b3_color color = b3Color(1.0, 1.0, 1.0);
	rgb = color;
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xffffff, rgb);
}

void b3AuxTest::testRect()
{
	b3Rect rect;
	b3Rect r1(-1, -2, 3, 4);
	b3Rect r2( 1,  2, 3, 4);

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.b3Height());

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, -1, r1.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, -2, r1.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  3, r1.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r1.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r1.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  6, r1.b3Height());

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  1, r2.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r2.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  3, r2.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r2.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r2.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r2.b3Height());
}

void b3AuxTest::testBound()
{
	b3Rect rect( 3, 4, 6,  8);
	b3Rect inner(3, 5, 5,  7);
	b3Rect left( 2, 3, 4,  9);
	b3Rect right(5, 2, 7, 10);

	CPPUNIT_ASSERT(!rect.b3UpdateBound(&rect));
	CPPUNIT_ASSERT_EQUAL(3, rect.m_x1);
	CPPUNIT_ASSERT_EQUAL(4, rect.m_y1);
	CPPUNIT_ASSERT_EQUAL(6, rect.m_x2);
	CPPUNIT_ASSERT_EQUAL(8, rect.m_y2);

	CPPUNIT_ASSERT(!rect.b3UpdateBound(&inner));
	CPPUNIT_ASSERT_EQUAL(3, rect.m_x1);
	CPPUNIT_ASSERT_EQUAL(4, rect.m_y1);
	CPPUNIT_ASSERT_EQUAL(6, rect.m_x2);
	CPPUNIT_ASSERT_EQUAL(8, rect.m_y2);

	CPPUNIT_ASSERT(rect.b3UpdateBound(&left));
	CPPUNIT_ASSERT_EQUAL(2, rect.m_x1);
	CPPUNIT_ASSERT_EQUAL(3, rect.m_y1);
	CPPUNIT_ASSERT_EQUAL(6, rect.m_x2);
	CPPUNIT_ASSERT_EQUAL(9, rect.m_y2);

	CPPUNIT_ASSERT(rect.b3UpdateBound(&right));
	CPPUNIT_ASSERT_EQUAL( 2, rect.m_x1);
	CPPUNIT_ASSERT_EQUAL( 2, rect.m_y1);
	CPPUNIT_ASSERT_EQUAL( 7, rect.m_x2);
	CPPUNIT_ASSERT_EQUAL(10, rect.m_y2);
}

void b3AuxTest::testPoint()
{
	b3Point zero, point(-1.2, 2.5);

	CPPUNIT_ASSERT_EQUAL(0.0, zero.x);
	CPPUNIT_ASSERT_EQUAL(0.0, zero.y);

	CPPUNIT_ASSERT_EQUAL(-1.2, point.x);
	CPPUNIT_ASSERT_EQUAL( 2.5, point.y);
}

void b3AuxTest::testStrCase()
{
	const std::string test1 = "AA12BB-Öß-Αθήνα";
	const std::string test2 = "Aa12Bb-öß-Αθήνα";
	const std::string test3 = "Aa12bb-Öß-Αθήνα";
	const std::string lower = "aa12bb-öß-αθήνα";
	const std::string upper = "AA12BB-Öß-ΑΘΉΝΑ";

	CPPUNIT_ASSERT_EQUAL(lower, b3StringTool::b3ToLower(test1));
	CPPUNIT_ASSERT_EQUAL(lower, b3StringTool::b3ToLower(test2));
	CPPUNIT_ASSERT_EQUAL(lower, b3StringTool::b3ToLower(test3));

	CPPUNIT_ASSERT_EQUAL(upper, b3StringTool::b3ToUpper(test1));
	CPPUNIT_ASSERT_EQUAL(upper, b3StringTool::b3ToUpper(test2));
	CPPUNIT_ASSERT_EQUAL(upper, b3StringTool::b3ToUpper(test3));
}

void b3AuxTest::testStrCaseCmp()
{
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("", "") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("", "a") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "") > 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "aa") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "AA") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "aA") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "AA") < 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "b") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "B") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "b") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "B") < 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "a") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "A") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "a") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "A") == 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("b", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("b", "A") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("B", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("B", "A") > 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("aa", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("aa", "A") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("AA", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("AA", "A") > 0);
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
