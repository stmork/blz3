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
**	Revision 1.5  2006/05/19 15:58:47  smork
**	- Correct exception handling in complex number computation.
**
**	Revision 1.4  2006/05/14 11:30:39  sm
**	- Added complex number classes to FFT class.
**	
**	Revision 1.3  2006/05/13 10:01:01  sm
**	- Introduced special complex number computation for FFT handling.
**	
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
**                        Unit test for complex numbers                 **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

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

void b3ComplexTest::testComplex64()
{
	ca = 0.0;
	cb = 0.0;
	ca = a;
	CPPUNIT_ASSERT_EQUAL(  2.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  3.0, ca.b3Imag());
	cb = ca;
	CPPUNIT_ASSERT_EQUAL(  2.0, cb.b3Real());
	CPPUNIT_ASSERT_EQUAL(  3.0, cb.b3Imag());
	CPPUNIT_ASSERT(ca == cb);

	cc = b3Complex64(4.0, -9.0);
	CPPUNIT_ASSERT_EQUAL(   4.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(  -9.0, cc.b3Imag());
	CPPUNIT_ASSERT_THROW(cd = b3Complex64::b3Sqrt(cc), std::domain_error);

	cc = b3Complex64(4.0, 9.0);
	CPPUNIT_ASSERT_EQUAL(  4.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(  9.0, cc.b3Imag());
	CPPUNIT_ASSERT_NO_THROW(cd = b3Complex64::b3Sqrt(cc));
	CPPUNIT_ASSERT_EQUAL(  2.0, cd.b3Real());
	CPPUNIT_ASSERT_EQUAL(  3.0, cd.b3Imag());

	cc.b3Real() =  1.0;
	cc.b3Imag() = -2.0;
	CPPUNIT_ASSERT_EQUAL(  1.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL( -2.0, cc.b3Imag());
	cc = 9.0;
	CPPUNIT_ASSERT_EQUAL(  9.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(  0.0, cc.b3Imag());
	cc = *d;
	CPPUNIT_ASSERT_EQUAL(  6.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(  7.0, cc.b3Imag());
	cd = *c;
	CPPUNIT_ASSERT_EQUAL(  4.0, cd.b3Real());
	CPPUNIT_ASSERT_EQUAL(  5.0, cd.b3Imag());

	b3Complex64::b3Swap(cc, cd);
	CPPUNIT_ASSERT_EQUAL(  4.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(  5.0, cc.b3Imag());
	CPPUNIT_ASSERT_EQUAL(  6.0, cd.b3Real());
	CPPUNIT_ASSERT_EQUAL(  7.0, cd.b3Imag());

	ce = e;
	CPPUNIT_ASSERT_EQUAL(  3.0, ce.b3Real());
	CPPUNIT_ASSERT_EQUAL(  4.0, ce.b3Imag());
	CPPUNIT_ASSERT_EQUAL(  5.0, ce.b3Length());
	CPPUNIT_ASSERT_EQUAL( 13.0, ca.b3SquareLength());

	ca = cc + cd;
	CPPUNIT_ASSERT_EQUAL(  10.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  12.0, ca.b3Imag());
	ca = cc -cd;
	CPPUNIT_ASSERT_EQUAL(  -2.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  -2.0, ca.b3Imag());
	ca = cb * 2.0;
	CPPUNIT_ASSERT_EQUAL(  4.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  6.0, ca.b3Imag());
	ca = cb / 2.0;
	CPPUNIT_ASSERT_EQUAL(  1.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  1.5, ca.b3Imag());
	ca = cc * cd;
	CPPUNIT_ASSERT_EQUAL( -11.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  58.0, ca.b3Imag());

	ca  = cc;
	ca += cd;
	CPPUNIT_ASSERT_EQUAL(  10.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  12.0, ca.b3Imag());
	ca  = cc;
	ca -= cd;
	CPPUNIT_ASSERT_EQUAL( -2.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL( -2.0, ca.b3Imag());
	ca  = cb;
	ca *= 2.0;
	CPPUNIT_ASSERT_EQUAL(  4.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  6.0, ca.b3Imag());
	ca  = cb;
	ca /= 2.0;
	CPPUNIT_ASSERT_EQUAL(  1.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  1.5, ca.b3Imag());
	ca  = cc;
	ca *= cd;
	CPPUNIT_ASSERT_EQUAL( -11.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(  58.0, ca.b3Imag());
	ca = cc;
	ca.b3Scale(cd);
	CPPUNIT_ASSERT_EQUAL( 24.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL( 35.0, ca.b3Imag());
}

#endif
