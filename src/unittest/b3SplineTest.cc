/*
**
**	$Filename:	b3SplineTest.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SplineTest.h"

/*************************************************************************
**                                                                      **
**                        Unit tests for splines                        **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineTest<b3Curve>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineTest<b3Spline>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineTest<b3Nurbs>);

static b3_f64 curve_controls[B3_MAX_CONTROLS];

template <> void b3SplineTest<b3Curve>::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);

	a.m_Knots    = knots;
	a.m_Controls = curve_controls;
}

static b3_vector spline_controls[B3_MAX_CONTROLS];

template <> void b3SplineTest<b3Spline>::setUp()
{
	a.m_Knots    = knots;
	a.m_Controls = spline_controls;
}

static b3_vector4D nurbs_controls[B3_MAX_CONTROLS];

template <> void b3SplineTest<b3Nurbs>::setUp()
{
	a.m_Knots    = knots;
	a.m_Controls = nurbs_controls;
}

#endif
