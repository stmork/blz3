/*
**
**	$Filename:	b3SplineTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for spline methods.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_SPLINE_TEST_H
#define B3_SPLINE_TEST_H

#include "blz3/base/b3Spline.h"
#include "b3TestMacros.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

template<class SPLINE> class b3SplineTest : public CppUnit::TestFixture
{
	SPLINE            a;
	SPLINE            b;
	b3Spline::b3_knot knots[b3Spline::B3_MAX_KNOTS];
	b3_f64            curve_controls[b3Spline::B3_MAX_CONTROLS];
	b3_vector         spline_controls[b3Spline::B3_MAX_CONTROLS];
	b3_vector4D       nurbs_controls[b3Spline::B3_MAX_CONTROLS];

	CPPUNIT_TEST_SUITE(b3SplineTest<SPLINE>);
	CPPUNIT_TEST(testInit);
	CPPUNIT_TEST(testClosed);
	CPPUNIT_TEST(testOpened);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;

	void tearDown() override
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void testInit()
	{
		CPPUNIT_ASSERT(a.b3InitCurve(3, 10, true));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, b.bspline_errno);

		CPPUNIT_ASSERT(!a.b3InitCurve(0, 10, true));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_INVALID_ARGUMENT, b.bspline_errno);

		CPPUNIT_ASSERT(!a.b3InitCurve(3, 0, true));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_INVALID_ARGUMENT, b.bspline_errno);

		CPPUNIT_ASSERT(!b.b3InitCurve(3, b.m_ControlMax * 2, false));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_MUCH_CONTROLS, b.bspline_errno);

		CPPUNIT_ASSERT(!b.b3InitCurve(b.m_KnotMax, 10, false));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXKNOTS, b.bspline_errno);

		CPPUNIT_ASSERT(!b.b3InitCurve(3, 10, false));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_MISSING_KNOTS, b.bspline_errno);

		b = a;
		CPPUNIT_ASSERT(b.b3InitCurve(3, 10, false));

		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 10, a.b3GetSegmentCount());
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count,  7, b.b3GetSegmentCount());
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 10, a.b3GetSegmentKnotCount());
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count,  8, b.b3GetSegmentKnotCount());
	}

	void testClosed()
	{
		CPPUNIT_ASSERT(a.b3InitCurve(3, 10, true));

		CPPUNIT_ASSERT(!a.b3ThroughEndControl());
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_CLOSED, a.bspline_errno);

		CPPUNIT_ASSERT(!a.b3ToBezier());
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_CLOSED, a.bspline_errno);

		CPPUNIT_ASSERT(!a.b3Degree(a.m_KnotMax));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_TOO_FEW_MAXKNOTS, a.bspline_errno);
	}

	void testOpened()
	{
		b = a;
		CPPUNIT_ASSERT(b.b3InitCurve(3, 10, false));

		CPPUNIT_ASSERT(b.b3ThroughEndControl());
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, b.bspline_errno);

		CPPUNIT_ASSERT(b.b3ToBezier());
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, b.bspline_errno);

		CPPUNIT_ASSERT(b.b3Degree(2));
		CPPUNIT_ASSERT_EQUAL(B3_BSPLINE_OK, b.bspline_errno);
	}
};

#endif
#endif
