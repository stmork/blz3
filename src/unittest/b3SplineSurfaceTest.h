/*
**
**	$Filename:	b3SplineSurfaceTest.h $
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

#pragma once

#ifndef B3_SPLINESURFACE_TEST_H
#define B3_SPLINESURFACE_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3Spline.h"

class b3SplineSurfaceTest : public CppUnit::TestFixture
{
	b3Spline          m_Horizontal;
	b3Spline          m_Vertical;
	b3Spline::b3_knot m_HorizontalKnots[b3Spline::B3_MAX_KNOTS];
	b3Spline::b3_knot m_VerticalKnots[b3Spline::B3_MAX_KNOTS];
	b3Spline::type    m_Controls[b3Spline::B3_MAX_CONTROLS * b3Spline::B3_MAX_CONTROLS];
	b3_f64            m_xControls[b3Spline::B3_MAX_CONTROLS];
	b3_f64            m_yControls[b3Spline::B3_MAX_CONTROLS];

	CPPUNIT_TEST_SUITE(b3SplineSurfaceTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();

private:
	static void b3InitControls(const b3Spline & spline, b3_f64 * controls);
};

#endif
#endif
