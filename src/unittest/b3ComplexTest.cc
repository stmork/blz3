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
**                        Unit test for complex numbers                 **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ComplexTest);

void b3ComplexTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	a = b3Complex<b3_f64>(2, 3);
	c = new b3Complex<b3_f64>(4, 5);
	d = new b3Complex<b3_f64>(6, 7);
	e = b3Complex<b3_f64>(3, 4);
}

void b3ComplexTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
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
	ca = b3Complex64( 6.0, -7.0);
	CPPUNIT_ASSERT_NO_THROW(ca.b3Square());
	CPPUNIT_ASSERT_EQUAL( -13.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL( -84.0, ca.b3Imag());	

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
