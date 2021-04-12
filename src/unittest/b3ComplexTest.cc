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

using namespace std::complex_literals;

void b3ComplexTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	a = b3Complex<b3_f32>(2, 3);
	c = new b3Complex<b3_f32>(4, 5);
	d = new b3Complex<b3_f32>(6, 7);
	e = b3Complex<b3_f32>(3, 4);
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
	CPPUNIT_ASSERT(*c + *d == b3Complex<b3_f32>(10, 12));
	CPPUNIT_ASSERT(*c - *d == b3Complex<b3_f32>(-2, -2));
	CPPUNIT_ASSERT(*c * *d == b3Complex<b3_f32>(-11, 58));
}

void b3ComplexTest::testEqual()
{
	CPPUNIT_ASSERT_EQUAL(13.0f, a.b3SquareLength());
	CPPUNIT_ASSERT_EQUAL( 5.0f, e.b3Length());
	CPPUNIT_ASSERT_EQUAL( 2.0f, a.b3Real());
	CPPUNIT_ASSERT_EQUAL( 3.0f, a.b3Imag());
}

void b3ComplexTest::testComplex64()
{
	ca = b3Complex64(6.0, -7.0);
	CPPUNIT_ASSERT_NO_THROW(ca.b3Square());
	CPPUNIT_ASSERT_EQUAL(-13.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(-84.0, ca.b3Imag());

	ca = 0.0;
	cb = 0.0;
	ca = a;
	CPPUNIT_ASSERT_EQUAL(2.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(3.0, ca.b3Imag());
	cb = ca;
	CPPUNIT_ASSERT_EQUAL(2.0, cb.b3Real());
	CPPUNIT_ASSERT_EQUAL(3.0, cb.b3Imag());
	CPPUNIT_ASSERT(ca == cb);

	// BUG: This does not work in conjunction with valgrind.
#if 0
	cc = b3Complex64(4.0, -9.0);
	CPPUNIT_ASSERT_EQUAL( 4.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(-9.0, cc.b3Imag());
	CPPUNIT_ASSERT_THROW(cd = b3Complex64::b3Sqrt(cc), std::domain_error);
#endif

	cc = b3Complex64(4.0, 9.0);
	CPPUNIT_ASSERT_EQUAL(4.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(9.0, cc.b3Imag());
	CPPUNIT_ASSERT_NO_THROW(cd = b3Complex64::b3Sqrt(cc));
	CPPUNIT_ASSERT_EQUAL(2.0, cd.b3Real());
	CPPUNIT_ASSERT_EQUAL(3.0, cd.b3Imag());

	cc.b3Real() =  1.0;
	cc.b3Imag() = -2.0;
	CPPUNIT_ASSERT_EQUAL( 1.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(-2.0, cc.b3Imag());
	cc = 9.0;
	CPPUNIT_ASSERT_EQUAL(9.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(0.0, cc.b3Imag());
	cc = *d;
	CPPUNIT_ASSERT_EQUAL(6.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(7.0, cc.b3Imag());
	cd = *c;
	CPPUNIT_ASSERT_EQUAL(4.0, cd.b3Real());
	CPPUNIT_ASSERT_EQUAL(5.0, cd.b3Imag());

	b3Complex64::b3Swap(cc, cd);
	CPPUNIT_ASSERT_EQUAL(4.0, cc.b3Real());
	CPPUNIT_ASSERT_EQUAL(5.0, cc.b3Imag());
	CPPUNIT_ASSERT_EQUAL(6.0, cd.b3Real());
	CPPUNIT_ASSERT_EQUAL(7.0, cd.b3Imag());

	ce = e;
	CPPUNIT_ASSERT_EQUAL(3.0, ce.b3Real());
	CPPUNIT_ASSERT_EQUAL(4.0, ce.b3Imag());
	CPPUNIT_ASSERT_EQUAL(5.0, ce.b3Length());
	CPPUNIT_ASSERT_EQUAL(13.0, ca.b3SquareLength());

	ca = cc + cd;
	CPPUNIT_ASSERT_EQUAL(10.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(12.0, ca.b3Imag());
	ca = cc - cd;
	CPPUNIT_ASSERT_EQUAL(-2.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(-2.0, ca.b3Imag());
	ca = cb * 2.0;
	CPPUNIT_ASSERT_EQUAL(4.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(6.0, ca.b3Imag());
	ca = cb / 2.0;
	CPPUNIT_ASSERT_EQUAL(1.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(1.5, ca.b3Imag());
	ca = cc * cd;
	CPPUNIT_ASSERT_EQUAL(-11.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(58.0, ca.b3Imag());

	ca  = cc;
	ca += cd;
	CPPUNIT_ASSERT_EQUAL(10.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(12.0, ca.b3Imag());
	ca  = cc;
	ca -= cd;
	CPPUNIT_ASSERT_EQUAL(-2.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(-2.0, ca.b3Imag());
	ca  = cb;
	ca *= 2.0;
	CPPUNIT_ASSERT_EQUAL(4.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(6.0, ca.b3Imag());
	ca  = cb;
	ca /= 2.0;
	CPPUNIT_ASSERT_EQUAL(1.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(1.5, ca.b3Imag());
	ca  = cc;
	ca *= cd;
	CPPUNIT_ASSERT_EQUAL(-11.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(58.0, ca.b3Imag());
	ca = cc;
	ca.b3Scale(cd);
	CPPUNIT_ASSERT_EQUAL(24.0, ca.b3Real());
	CPPUNIT_ASSERT_EQUAL(35.0, ca.b3Imag());
}

void b3ComplexTest::testStdComplex()
{
	std::complex<b3_f32> std_c32;
	std::complex<b3_f64> std_c64;
	b3Complex<b3_f32>    b3_c32 = 0;
	b3Complex64          b3_c64 = 0;

	CPPUNIT_ASSERT_EQUAL(0.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL(0.0f, b3_c32.b3Imag());
	CPPUNIT_ASSERT(!b3_c32.b3Normalize());

	std_c32 = 3i + 4.0;
	b3_c32  = std_c32;

	CPPUNIT_ASSERT_EQUAL( 4.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL( 3.0f, b3_c32.b3Imag());
	CPPUNIT_ASSERT(b3_c32.b3Normalize(10));
	CPPUNIT_ASSERT_EQUAL(10.0f, b3_c32.b3Length());
	CPPUNIT_ASSERT_EQUAL( 8.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL( 6.0f, b3_c32.b3Imag());

	b3_c32 = b3Complex<b3_f32>(1.0, 7.0);
	CPPUNIT_ASSERT_EQUAL( 1.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL( 7.0f, b3_c32.b3Imag());

	std_c32 = b3_c32;
	CPPUNIT_ASSERT_EQUAL( 1.0f, std_c32.real());
	CPPUNIT_ASSERT_EQUAL( 7.0f, std_c32.imag());

	b3_c32 = 8.0if;
	CPPUNIT_ASSERT_EQUAL( 0.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL( 8.0f, b3_c32.b3Imag());

	b3_c32 = 8.0f;
	CPPUNIT_ASSERT_EQUAL( 8.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL( 0.0f, b3_c32.b3Imag());

	b3_c32 = 2.0if + 1.0f;
	CPPUNIT_ASSERT_EQUAL( 1.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL( 2.0f, b3_c32.b3Imag());

	// 32 bit init tests

	b3Complex<b3_f32>    b3_init1(std_c32);
	b3Complex<b3_f32>    b3_init2(b3_c32);

	CPPUNIT_ASSERT_EQUAL( 1.0f, b3_init1.b3Real());
	CPPUNIT_ASSERT_EQUAL( 7.0f, b3_init1.b3Imag());
	CPPUNIT_ASSERT_EQUAL( 1.0f, b3_init2.b3Real());
	CPPUNIT_ASSERT_EQUAL( 2.0f, b3_init2.b3Imag());

	b3_c32 = b3_init1 + b3_init2;
	CPPUNIT_ASSERT_EQUAL(2.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL(9.0f, b3_c32.b3Imag());

	b3_c32 = b3_init1 - b3_init2;
	CPPUNIT_ASSERT_EQUAL(0.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL(5.0f, b3_c32.b3Imag());

	b3_c32 = b3_init1 * b3_init2;
	CPPUNIT_ASSERT_EQUAL(-13.0f, b3_c32.b3Real());
	CPPUNIT_ASSERT_EQUAL(  9.0f, b3_c32.b3Imag());

	// 64 bit

	CPPUNIT_ASSERT_EQUAL(0.0,  b3_c64.b3Real());
	CPPUNIT_ASSERT_EQUAL(0.0,  b3_c64.b3Imag());

	std_c64 = 4i - 2.0;
	b3_c64  = std_c64;

	CPPUNIT_ASSERT_EQUAL(-2.0, b3_c64.b3Real());
	CPPUNIT_ASSERT_EQUAL( 4.0, b3_c64.b3Imag());

	b3_c64 = b3Complex64(2.5, 3.5);
	CPPUNIT_ASSERT_EQUAL( 2.5, b3_c64.b3Real());
	CPPUNIT_ASSERT_EQUAL( 3.5, b3_c64.b3Imag());

	std_c64 = b3_c64;
	CPPUNIT_ASSERT_EQUAL( 2.5, std_c64.real());
	CPPUNIT_ASSERT_EQUAL( 3.5, std_c64.imag());

	b3_c64 = 9.0i;
	CPPUNIT_ASSERT_EQUAL( 0.0, b3_c64.b3Real());
	CPPUNIT_ASSERT_EQUAL( 9.0, b3_c64.b3Imag());

	b3_c64 = 9.0;
	CPPUNIT_ASSERT_EQUAL( 9.0, b3_c64.b3Real());
	CPPUNIT_ASSERT_EQUAL( 0.0, b3_c64.b3Imag());

	b3_c64 = 3.0 + 4i;
	CPPUNIT_ASSERT_EQUAL( 3.0, b3_c64.b3Real());
	CPPUNIT_ASSERT_EQUAL( 4.0, b3_c64.b3Imag());
}

#endif
