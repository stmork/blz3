/*
**
**	$Filename:	b3SplineSurfaceTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for increasing spline surfaces.
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

#include "b3SplineSurfaceTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineSurfaceTest);

void b3SplineSurfaceTest::setUp()
{
	m_Horizontal.m_Knots    = m_HorizontalKnots;
	m_Horizontal.m_Controls = m_Controls;
	m_Horizontal.b3InitCurve(2, 8, false);

	m_Vertical.m_Knots      = m_VerticalKnots;
	m_Vertical.m_Controls   = m_Controls;
	m_Vertical.b3InitCurve(3, 9, false, m_Horizontal.m_ControlMax);

	b3InitControls(m_Horizontal, m_xControls);
	b3InitControls(m_Vertical,   m_yControls);

	bzero(m_Controls, sizeof(m_Controls));
	for (unsigned y = 0; y < m_Vertical.m_ControlNum; y++)
	{
		unsigned idx = y * m_Vertical.m_Offset;

		for (unsigned x = 0; x < m_Horizontal.m_ControlNum; x++)
		{
			m_Controls[idx].x = m_xControls[x];
			m_Controls[idx].y = m_yControls[y];
			m_Controls[idx].z = 0;

			idx++;
		}
	}
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3SplineSurfaceTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3SplineSurfaceTest::testExtension()
{
	b3_f64 q;

	q = m_Horizontal.b3FirstKnot() + m_Horizontal.b3KnotRange() * 0.5;
	CPPUNIT_ASSERT(m_Horizontal.b3InsertSurfaceControl(q, 1, m_Vertical.m_Offset, m_Vertical.m_ControlMax));

	q = m_Vertical.b3FirstKnot() + m_Vertical.b3KnotRange() * 0.5;
	CPPUNIT_ASSERT(m_Vertical.b3InsertSurfaceControl(q, 1, m_Horizontal.m_Offset, m_Horizontal.m_ControlMax));

	q = m_Horizontal.b3LastKnot() + 1;
	CPPUNIT_ASSERT(m_Horizontal.b3AppendSurfaceControl(true, q, 1, m_Vertical.m_Offset, m_Vertical.m_ControlMax));

	q = m_Vertical.b3LastKnot() + 1;
	CPPUNIT_ASSERT(m_Vertical.b3AppendSurfaceControl(true, q, 1, m_Horizontal.m_Offset, m_Horizontal.m_ControlMax));

	q = m_Horizontal.b3FirstKnot() - 1;
	CPPUNIT_ASSERT(m_Horizontal.b3AppendSurfaceControl(false, q, 1, m_Vertical.m_Offset, m_Vertical.m_ControlMax));

	q = m_Vertical.b3FirstKnot() - 1;
	CPPUNIT_ASSERT(m_Vertical.b3AppendSurfaceControl(false, q, 1, m_Horizontal.m_Offset, m_Horizontal.m_ControlMax));
}

void b3SplineSurfaceTest::testValidation()
{
	b3_f64 q;

	q = m_Horizontal.b3LastKnot();
	CPPUNIT_ASSERT(!m_Horizontal.b3AppendSurfaceControl(false, q, 1,
			m_Vertical.m_Offset, m_Vertical.m_ControlMax));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OUT_OF_RANGE, m_Horizontal.bspline_errno);

	q = m_Horizontal.b3FirstKnot();
	CPPUNIT_ASSERT(!m_Horizontal.b3AppendSurfaceControl(true, q, 1,
			m_Vertical.m_Offset, m_Vertical.m_ControlMax));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OUT_OF_RANGE, m_Horizontal.bspline_errno);

	m_Horizontal.m_KnotMax    = m_Horizontal.m_KnotNum;
	m_Horizontal.m_ControlMax = b3Spline::B3_MAX_CONTROLS;
	CPPUNIT_ASSERT(!m_Horizontal.b3AppendSurfaceControl(true, q, 1,
			m_Vertical.m_Offset, m_Vertical.m_ControlMax));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXKNOTS, m_Horizontal.bspline_errno);

	m_Horizontal.m_KnotMax    = b3Spline::B3_MAX_KNOTS;
	m_Horizontal.m_ControlMax = m_Horizontal.m_ControlNum;
	CPPUNIT_ASSERT(!m_Horizontal.b3AppendSurfaceControl(
			true, q, m_Horizontal.m_Degree + 2,
			m_Vertical.m_Offset, m_Vertical.m_ControlMax));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXCONTROLS, m_Horizontal.bspline_errno);

	q = m_Horizontal.b3FirstKnot() + m_Horizontal.b3KnotRange() * 0.25;

	m_Horizontal.m_KnotMax    = m_Horizontal.m_KnotNum;
	m_Horizontal.m_ControlMax = b3Spline::B3_MAX_CONTROLS;
	CPPUNIT_ASSERT(!m_Horizontal.b3InsertSurfaceControl(q, 1,
			m_Vertical.m_Offset, m_Vertical.m_ControlMax));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXKNOTS, m_Horizontal.bspline_errno);

	m_Horizontal.m_KnotMax    = b3Spline::B3_MAX_KNOTS;
	m_Horizontal.m_ControlMax = m_Horizontal.m_ControlNum;
	CPPUNIT_ASSERT(!m_Horizontal.b3InsertSurfaceControl(
			q, m_Horizontal.m_Degree + 2,
			m_Vertical.m_Offset, m_Vertical.m_ControlMax));
	CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXCONTROLS, m_Horizontal.bspline_errno);
}

void b3SplineSurfaceTest::b3InitControls(const b3Spline & spline, b3_f64 * controls)
{
	unsigned q = 0;

	for (unsigned i = 0; i < spline.m_ControlNum; i++)
	{
		controls[i] = q;

		q += std::min(q + 1, spline.m_Degree);
	}

	for (unsigned i = spline.m_Degree; i > 0; i--)
	{
		const unsigned idx = spline.m_ControlNum - i;
		const unsigned diff = controls[i] - controls[i - 1];

		controls[idx] = controls[idx - 1] + diff;
	}

	for (unsigned i = spline.m_ControlNum; i < spline.m_ControlMax; i++)
	{
		controls[i] = 0;
	}
}

#endif
