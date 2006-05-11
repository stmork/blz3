/*
**
**	$Filename:	b3ComplexTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Complex number unit test.
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
  
#include "b3ComplexTest.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**                        Unit test for complex numbers                 **
**                                                                      **
*************************************************************************/

CPPUNIT_TEST_SUITE_REGISTRATION(b3ComplexTest);

void b3ComplexTest::setUp()
{
	a = b3Complex<b3_f64>(2, 3);
	c = new b3Complex<b3_f64>(4, 5);
	d = new b3Complex<b3_f64>(6, 7);
	e = b3Complex<b3_f64>(3, 4);
}

void b3ComplexTest::tearDown()
{
	delete c;
	delete d;
}

void b3ComplexTest::testOps()
{
	b = a;

	CPPUNIT_ASSERT(a == b);
	CPPUNIT_ASSERT( *c + *d == b3Complex<b3_f64>(10, 12));
	CPPUNIT_ASSERT( *c - *d == b3Complex<b3_f64>(-2, -2));
	CPPUNIT_ASSERT( *c * *d == b3Complex<b3_f64>(-11, 58));
}

void b3ComplexTest::testEqual()
{
	CPPUNIT_ASSERT_EQUAL( 13.0, a.b3SquareLength());
	CPPUNIT_ASSERT_EQUAL(  5.0, e.b3Length());
	CPPUNIT_ASSERT_EQUAL(  2.0, a.b3Real());
	CPPUNIT_ASSERT_EQUAL(  3.0, a.b3Imag());
}
