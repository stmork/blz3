/*
**
**	$Filename:	b3MathTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for math routines.
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

#include "b3MathTest.h"
#include "b3TestMacros.h"
#include <blz3/base/b3FFT.h>
#include <blz3/base/b3Polynom.h>

/*************************************************************************
**                                                                      **
**                        Unit test for math routines                   **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MathTest);

b3MathTest::b3MathTest()
{
	epsilon_s = epsilon<b3_f32>(loops_s);
	epsilon_d = epsilon<b3_f64>(loops_d);
	epsilon_x = epsilon<b3_f96>(loops_x);

	recipr_s = 1.0 / epsilon_s;
	recipr_d = 1.0 / epsilon_d;
	recipr_x = 1.0 / epsilon_x;
}

void b3MathTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3MathTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3MathTest::test()
{
	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Frac( 4.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Frac(-4.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(0.0,   b3Math::b3Frac( 4.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(0.125, b3Math::b3FracOne(  4.125));
	CPPUNIT_ASSERT_EQUAL(0.875, b3Math::b3FracOne(-84.125));
	CPPUNIT_ASSERT_EQUAL(1.25,  b3Math::b3Mix(1.0, 2.0, 0.25));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Min(1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Min(2.0, 1.0));

	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Max(1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Max(2.0, 1.0));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Clamp( 2.0, 0.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(0.5,   b3Math::b3Clamp( 0.5, 0.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(0.0,   b3Math::b3Clamp(-1.0, 0.0, 1.0));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Clamp(0.5, 1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(2.5,   b3Math::b3Clamp(2.5, 1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(4.0,   b3Math::b3Clamp(4.5, 1.0, 4.0));

	for (b3_u32 base = 0; base < 13; base++)
	{
		for (b3_u32 exp = 0; exp < 16; exp++)
		{
			const b3_f64 f_base(base);
			const b3_f64 f_exp(exp);

			CPPUNIT_ASSERT_EQUAL(pow(f_base, f_exp), b3Math::b3FastPow(f_base, exp));
		}
	}
}

void b3MathTest::testIsZero()
{
	CPPUNIT_ASSERT( b3Math::b3IsZero(0.0));
	CPPUNIT_ASSERT(!b3Math::b3IsZero(b3Math::epsilon));
	CPPUNIT_ASSERT(!b3Math::b3IsZero(0.1));
}

void b3MathTest::testSolveOrd2()
{
	b3_f64                  result[2];
	static constexpr b3_f64 coeffs_zero[3] {}; // simply x^2

	static constexpr b3_f64 coeffs_null[3]
	{
		// x^0, x^1
		5.0, -2.0
	};
	CPPUNIT_ASSERT_EQUAL( 0, verifyQuadric(coeffs_null, result));

	CPPUNIT_ASSERT_EQUAL( 1, verifyQuadric(coeffs_zero, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, result[0], b3Math::epsilon);

	static constexpr b3_f64 coeffs_two[3] =
	{
		// x^0, x^1
		8.0 * -1, -2.0
	};
	CPPUNIT_ASSERT_EQUAL( 2, verifyQuadric(coeffs_two, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  4.0, result[1], b3Math::epsilon);
}

void b3MathTest::testSolveOrd3()
{
	b3_f64              result[3];
	static const b3_f64 coeffs_zero[3] {}; // simply x^3

	CPPUNIT_ASSERT_EQUAL( 1, verifyCubic(coeffs_zero, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, result[0], b3Math::epsilon);

	static constexpr b3_f64 coeffs_one1[3]
	{
		// x^0, x^1, x^2
		8.0, 0.0, 0.0
	};
	CPPUNIT_ASSERT_EQUAL( 1, verifyCubic(coeffs_one1, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[0], b3Math::epsilon);

	static constexpr b3_f64 coeffs_one2[3]
	{
		// x^0, x^1, x^2
		1.0, 3.0, 3.0
	};
	CPPUNIT_ASSERT_EQUAL( 1, verifyCubic(coeffs_one2, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, result[0], b3Math::epsilon);

	static constexpr b3_f64 coeffs_two1[3]
	{
		// x^0, x^1, x^2
		0.0, 4.0, 4.0
	};
	CPPUNIT_ASSERT_EQUAL( 2, verifyCubic(coeffs_two1, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[1], b3Math::epsilon);

	static constexpr b3_f64 coeffs_two2[3]
	{
		// x^0, x^1, x^2
		0.0, 0.0, 1.0
	};
	CPPUNIT_ASSERT_EQUAL( 2, verifyCubic(coeffs_two2, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, result[1], b3Math::epsilon);

	static constexpr b3_f64 coeffs_three1[3]
	{
		// x^0, x^1, x^2
		2.0 * -1, 0.0, 3.0
	};
	CPPUNIT_ASSERT_EQUAL( 3, verifyCubic(coeffs_three1, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.73205, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0,     result[1], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.73205, result[2], b3Math::epsilon);
}

void b3MathTest::testSolveOrd4()
{
	b3_f64           result[4];
	static constexpr b3_f64 coeffs_zero[4] {}; // simply x^4

	CPPUNIT_ASSERT_EQUAL( 1, verifyQuartic(coeffs_zero, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, result[0], b3Math::epsilon);

	static constexpr b3_f64 coeffs_one[4]
	{
		// x^0, x^1, x^2, x^3
		0.0, 8.0, 0.0, 0.0
	};
	CPPUNIT_ASSERT_EQUAL( 2, verifyQuartic(coeffs_one, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, result[1], b3Math::epsilon);

	static constexpr b3_f64 coeffs_two1[4]
	{
		// x^0, x^1, x^2, x^3
		0.0, 0.0, 4.0, 4.0
	};
	CPPUNIT_ASSERT_EQUAL( 2, verifyQuartic(coeffs_two1, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, result[1], b3Math::epsilon);

	static constexpr b3_f64 coeffs_two2[4]
	{
		// x^0, x^1, x^2, x^3
		0.0, 0.0, 0.0, -4.0
	};
	CPPUNIT_ASSERT_EQUAL( 2, verifyQuartic(coeffs_two2, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  4.0, result[1], b3Math::epsilon);

	static constexpr b3_f64 coeffs_three1[4]
	{
		// x^0, x^1, x^2, x^3
		1.0 * -1.0, 0.0, 4.0, 4.0
	};
	CPPUNIT_ASSERT_EQUAL( 3, verifyQuartic(coeffs_three1, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.41421, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0,     result[1], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.41421, result[2], b3Math::epsilon);

	static constexpr b3_f64 coeffs_three2[4]
	{
		// x^0, x^1, x^2, x^3
		4.0 * -1, -4.0, 3.0, 4.0
	};
	CPPUNIT_ASSERT_EQUAL( 3, verifyQuartic(coeffs_three2, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[1], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, result[2], b3Math::epsilon);

	static constexpr b3_f64 coeffs_three3[4]
	{
		// x^0, x^1, x^2, x^3
		0.0, 0.0, -4.0, 0.0
	};
	CPPUNIT_ASSERT_EQUAL( 3, verifyQuartic(coeffs_three3, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, result[1], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[2], b3Math::epsilon);

	static constexpr b3_f64 coeffs_four1[4]
	{
		// x^0, x^1, x^2, x^3
		3.0 * -1, -1.0, 16.0, -8.0
	};
	CPPUNIT_ASSERT_EQUAL( 4, verifyQuartic(coeffs_four1, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  3.22665, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  4.59936, result[1], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.37095, result[2], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.54494, result[3], b3Math::epsilon);

	static constexpr b3_f64 coeffs_four2[4]
	{
		// x^0, x^1, x^2, x^3
		4.0, 0.0, -5.0, 0.0
	};
	CPPUNIT_ASSERT_EQUAL( 4, verifyQuartic(coeffs_four2, result));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, result[0], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, result[1], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, result[2], b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, result[3], b3Math::epsilon);
}

void b3MathTest::testHorner()
{
	static constexpr b3_f64 coeffs[]
	{
		// x^0, x^1, x^2, x^3
		5.0, -4.0, 3.0, -2.0
	};

	CPPUNIT_ASSERT_DOUBLES_EQUAL(57.0, b3Polynom::b3Horner(coeffs, -2.0, std::size(coeffs)), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, b3Polynom::b3Horner(coeffs, -1.0, std::size(coeffs)), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, b3Polynom::b3Horner(coeffs,  0.0, std::size(coeffs)), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0, b3Polynom::b3Horner(coeffs,  1.0, std::size(coeffs)), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, b3Polynom::b3Horner(coeffs,  2.0, std::size(coeffs)), b3Math::epsilon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Polynom::b3Horner(coeffs,  0.0, 0), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Polynom::b3Horner(coeffs,  2.0, 0), b3Math::epsilon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0, b3Polynom::b3Horner(coeffs, -2.0, 1), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.0, b3Polynom::b3Horner(coeffs, -1.0, 1), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, b3Polynom::b3Horner(coeffs,  0.0, 1), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.0, b3Polynom::b3Horner(coeffs,  1.0, 1), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0, b3Polynom::b3Horner(coeffs,  2.0, 1), b3Math::epsilon);
}

void b3MathTest::testRound()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, b3Math::b3Round(  0.0,   0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, b3Math::b3Round(  0.125, 0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, b3Math::b3Round(  0.25,  0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, b3Math::b3Round(  0.375, 0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, b3Math::b3Round(  0.5,   0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, b3Math::b3Round(  0.625, 0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  0.75,  0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  0.875, 0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  1.0,   0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  1.125, 0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, b3Math::b3Round(  1.25,  0.25), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, b3Math::b3Round(  1.5,   0.25), b3Math::epsilon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, b3Math::b3Round(  0.0,   0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, b3Math::b3Round(  0.125, 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, b3Math::b3Round(  0.25,  0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, b3Math::b3Round(  0.375, 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  0.5,   0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  0.625, 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  0.75,  0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  0.875, 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  1.0,   0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  1.125, 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, b3Math::b3Round(  1.25,  0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, b3Math::b3Round(  1.5,   0.5), b3Math::epsilon);
}

void b3MathTest::testLogPow()
{
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 0, b3Math::b3Log2(1));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3Math::b3Log2(2));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3Math::b3Log2(3));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(4));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(5));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(6));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(7));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 3, b3Math::b3Log2(8));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 3, b3Math::b3Log2(9));

	CPPUNIT_ASSERT_EQUAL(  1, b3Math::b3PowOf2(0));
	CPPUNIT_ASSERT_EQUAL(  1, b3Math::b3PowOf2(1));
	CPPUNIT_ASSERT_EQUAL(  2, b3Math::b3PowOf2(2));
	CPPUNIT_ASSERT_EQUAL(  4, b3Math::b3PowOf2(3));
	CPPUNIT_ASSERT_EQUAL(  4, b3Math::b3PowOf2(4));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(5));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(6));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(7));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(8));
	CPPUNIT_ASSERT_EQUAL( 16, b3Math::b3PowOf2(9));
}

void b3MathTest::testAsin()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-M_PI * 0.5, b3Math::b3Asin(-1.0), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(asin(-0.5),  b3Math::b3Asin(-0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(asin( 0.0),  b3Math::b3Asin( 0.0), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(asin( 0.5),  b3Math::b3Asin( 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( M_PI * 0.5, b3Math::b3Asin( 1.0), b3Math::epsilon);
}

void b3MathTest::testAcos()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL(M_PI,       b3Math::b3Acos(-1.0), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(acos(-0.5), b3Math::b3Acos(-0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(acos( 0.0), b3Math::b3Acos( 0.0), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(acos( 0.5), b3Math::b3Acos( 0.5), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,        b3Math::b3Acos( 1.0), b3Math::epsilon);
}

void b3MathTest::testSmoothStep1()
{
	CPPUNIT_ASSERT_EQUAL(0.0,     b3Math::b3Smoothstep(-1.0));
	CPPUNIT_ASSERT_EQUAL(0.0,     b3Math::b3Smoothstep( 0.0));
	CPPUNIT_ASSERT_EQUAL(0.15625, b3Math::b3Smoothstep( 0.25));
	CPPUNIT_ASSERT_EQUAL(0.5,     b3Math::b3Smoothstep( 0.5));
	CPPUNIT_ASSERT_EQUAL(0.84375, b3Math::b3Smoothstep( 0.75));
	CPPUNIT_ASSERT_EQUAL(1.0,     b3Math::b3Smoothstep( 1.0));
	CPPUNIT_ASSERT_EQUAL(1.0,     b3Math::b3Smoothstep( 2.0));
}

void b3MathTest::testSmoothStep2()
{
	CPPUNIT_ASSERT_EQUAL(0.0,     b3Math::b3Smoothstep(3.0, 5.0,  2.0));
	CPPUNIT_ASSERT_EQUAL(0.0,     b3Math::b3Smoothstep(3.0, 5.0,  3.0));
	CPPUNIT_ASSERT_EQUAL(0.15625, b3Math::b3Smoothstep(3.0, 5.0,  3.5));
	CPPUNIT_ASSERT_EQUAL(0.5,     b3Math::b3Smoothstep(3.0, 5.0,  4.0));
	CPPUNIT_ASSERT_EQUAL(0.84375, b3Math::b3Smoothstep(3.0, 5.0,  4.5));
	CPPUNIT_ASSERT_EQUAL(1.0,     b3Math::b3Smoothstep(3.0, 5.0,  5.0));
	CPPUNIT_ASSERT_EQUAL(1.0,     b3Math::b3Smoothstep(3.0, 5.0,  6.0));

	CPPUNIT_ASSERT_EQUAL(0.0,     b3Math::b3Smoothstep(-3.0, 5.0,  -5.0));
	CPPUNIT_ASSERT_EQUAL(0.0,     b3Math::b3Smoothstep(-3.0, 5.0,  -3.0));
	CPPUNIT_ASSERT_EQUAL(0.15625, b3Math::b3Smoothstep(-3.0, 5.0,  -1.0));
	CPPUNIT_ASSERT_EQUAL(0.5,     b3Math::b3Smoothstep(-3.0, 5.0,   1.0));
	CPPUNIT_ASSERT_EQUAL(0.84375, b3Math::b3Smoothstep(-3.0, 5.0,   3.0));
	CPPUNIT_ASSERT_EQUAL(1.0,     b3Math::b3Smoothstep(-3.0, 5.0,   5.0));
	CPPUNIT_ASSERT_EQUAL(1.0,     b3Math::b3Smoothstep(-3.0, 5.0,   7.0));
}

b3_count b3MathTest::verifyQuadric(const b3_f64 * coeffs, b3_f64 * result)
{
	const b3_count num = b3Polynom::b3SolveOrd2(coeffs, result);

	return std::count_if(result, result + num, [&coeffs](const b3_f64 x)
	{
		// Horner's method
		const b3_f64 fx = b3Polynom::b3Horner(coeffs, x, 2);

		return b3Math::b3IsZero(fx);
	});
}

b3_count b3MathTest::verifyCubic(const b3_f64 * coeffs, b3_f64 * result)
{
	const b3_count num = b3Polynom::b3SolveOrd3(coeffs, result);

	return std::count_if(result, result + num, [&coeffs](const b3_f64 x)
	{
		// Horner's method
		const b3_f64 fx = b3Polynom::b3Horner(coeffs, x, 3);

		return b3Math::b3IsZero(fx);
	});
}

b3_count b3MathTest::verifyQuartic(const b3_f64 * coeffs, b3_f64 * result)
{
	const b3_count num = b3Polynom::b3SolveOrd4(coeffs, result);

	return std::count_if(result, result + num, [&coeffs](const b3_f64 x)
	{
		// Horner's method
		const b3_f64 fx = b3Polynom::b3Horner(coeffs, x, 4);

		return b3Math::b3IsZero(fx);
	});
}

#endif
