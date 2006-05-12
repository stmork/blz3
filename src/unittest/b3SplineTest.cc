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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/05/12 14:06:28  smork
**	- Added configurable CPPUNIT tests.
**
**	Revision 1.1  2006/05/11 15:34:23  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**	
**
*/

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

void b3SplineTest<b3Curve>::setUp()
{
	a.m_Knots    = knots;
	a.m_Controls = curve_controls;
}

static b3_vector spline_controls[B3_MAX_CONTROLS];

void b3SplineTest<b3Spline>::setUp()
{
	a.m_Knots    = knots;
	a.m_Controls = spline_controls;
}

static b3_vector4D nurbs_controls[B3_MAX_CONTROLS];

void b3SplineTest<b3Nurbs>::setUp()
{
	a.m_Knots    = knots;
	a.m_Controls = nurbs_controls;
}

#endif
