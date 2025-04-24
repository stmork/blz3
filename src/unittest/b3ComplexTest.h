/*
**
**	$Filename:	b3ComplexTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Complex number unit test.
**
**      (C) Copyright 2006 - 2024  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_COMPLEX_TEST_H
#define B3_COMPLEX_TEST_H

#include "blz3/base/b3Complex.h"
#include "blz3/base/b3Complex64.h"
#include "blz3/base/b3Math.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "b3TestMacros.h"

using namespace std::literals::complex_literals;

class b3ComplexTest : public CppUnit::TestFixture
{
	b3Complex<b3_f32>   a, b, e;
	b3Complex<b3_f32> * c, *d;
	b3Complex64         ca, cb, cc, cd, ce;

	CPPUNIT_TEST_SUITE(b3ComplexTest);
	CPPUNIT_TEST(testOps);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST(testComplex64);
	CPPUNIT_TEST(testStdComplex);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testOps();
	void testEqual();
	void testComplex64();
	void testStdComplex();

private:
	template<class T, class C> void testStdComplexInit()
	{
		std::complex<T> std_cpx;
		C               b3_cpx(0);

		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, b3_cpx.b3Imag());
		CPPUNIT_ASSERT(!b3_cpx.b3Normalize());

		b3_cpx  = C(1, 2);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   1.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   2.0, b3_cpx.b3Imag());

		b3_cpx  = C(3.5, 4);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   3.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   4.0, b3_cpx.b3Imag());

		b3_cpx  = 6.5;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   6.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   0.0, b3_cpx.b3Imag());

		b3_cpx  = C(0, 7.5);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   0.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   7.5, b3_cpx.b3Imag());

		std_cpx = 3i + 4.0;
		b3_cpx  = std_cpx;

		CPPUNIT_ASSERT_TYPED_EQUAL(T,  25.0, b3_cpx.b3SquareLength());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   4.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   3.0, b3_cpx.b3Imag());
		CPPUNIT_ASSERT(b3_cpx.b3Normalize(10));
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  10.0, b3_cpx.b3Length());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   8.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   6.0, b3_cpx.b3Imag());

		b3_cpx = C(64, 36);
		b3_cpx = b3_cpx.b3Sqrt();
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   8.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   6.0, b3_cpx.b3Imag());

		b3_cpx = 0;
		CPPUNIT_ASSERT_NO_THROW(b3_cpx.b3Sqrt());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   0.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   0.0, b3_cpx.b3Imag());

		b3_cpx = -1;
		CPPUNIT_ASSERT_THROW(b3_cpx.b3Sqrt(), std::domain_error);

		b3_cpx = C(0, -1);
		CPPUNIT_ASSERT_THROW(b3_cpx.b3Sqrt(), std::domain_error);

		b3_cpx = b3Complex<T>(1.0, 7.0);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  7.0, b3_cpx.b3Imag());

		std_cpx = b3_cpx;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, std_cpx.real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  7.0, std_cpx.imag());

		b3_cpx = C(0, 8);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  8.0, b3_cpx.b3Imag());

		b3_cpx = 8.0;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  8.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.0, b3_cpx.b3Imag());

		b3_cpx = C(1, 2);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, b3_cpx.b3Imag());

		b3_cpx += 2.5;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, b3_cpx.b3Imag());

		b3_cpx *= 6.0;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  21.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  12.0, b3_cpx.b3Imag());

		b3_cpx /= 3.0;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   7.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   4.0, b3_cpx.b3Imag());

		b3_cpx -= 1.5;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   5.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,   4.0, b3_cpx.b3Imag());
	}

	template<class T, class C> void testStdComplexOps()
	{
		std::complex<T> std_cpx(1.0 + 7i);
		C               b3_cpx(5.5, 4);
		C               b3_cpx_init1(std_cpx);
		C               b3_cpx_init2(b3_cpx);

		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, b3_cpx_init1.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  7.0, b3_cpx_init1.b3Imag());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  5.5, b3_cpx_init2.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.0, b3_cpx_init2.b3Imag());

		b3_cpx = b3_cpx_init1 / b3_cpx_init2;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.724324, b3_cpx.b3Real(),  0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.745946, b3_cpx.b3Imag(),  0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8001,   b3_cpx.b3Phase(), 0.0001);

		b3_cpx = 1;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b3Math::b3Degree(b3_cpx.b3Phase()), 0.0001);

		b3_cpx = C(2, 2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(45.0, b3Math::b3Degree(b3_cpx.b3Phase()), 0.0001);

		b3_cpx = C(0, 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(90.0, b3Math::b3Degree(b3_cpx.b3Phase()), 0.0001);

		b3_cpx = C(10.5) / C(3.0);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.0, b3_cpx.b3Imag());

		b3_cpx = C(0.0, 10.5) / C(0.0, 3.0);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.0, b3_cpx.b3Imag());

		b3_cpx = b3_cpx_init2 + std_cpx;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  6.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 11.0, b3_cpx.b3Imag());

		b3_cpx = b3_cpx_init2 - std_cpx;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -3.0, b3_cpx.b3Imag());

		b3_cpx = b3_cpx_init1 + b3_cpx_init2;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  6.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 11.0, b3_cpx.b3Imag());

		b3_cpx = b3_cpx_init1 - b3_cpx_init2;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -4.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.0, b3_cpx.b3Imag());

		b3_cpx = b3_cpx_init1 * b3_cpx_init2;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -22.5, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  42.5, b3_cpx.b3Imag());

		b3_cpx *= b3_cpx_init1;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -320.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -115.0, b3_cpx.b3Imag());

		C::b3Swap(b3_cpx_init1, b3_cpx_init2);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  5.5, b3_cpx_init1.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.0, b3_cpx_init1.b3Imag());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, b3_cpx_init2.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  7.0, b3_cpx_init2.b3Imag());

		b3_cpx_init2 = b3_cpx.b3Scale(b3_cpx_init1);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,     5.5, b3_cpx_init1.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,     4.0, b3_cpx_init1.b3Imag());
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -1760.0, b3_cpx.b3Real());
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  -460.0, b3_cpx.b3Imag());
		CPPUNIT_ASSERT_EQUAL(b3_cpx,   b3_cpx_init2);

		b3_cpx.b3Dump("complex test");

		std::string message = b3_cpx;

		CPPUNIT_ASSERT(message.length() > 0);
	}
};

#endif
#endif
