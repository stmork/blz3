/*
**
**	$Filename:	b3SplineVectorTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Skeleton for unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
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

#include "b3SplineVectorTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

#include <blz3/base/b3Vector.h>

CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineVectorTest<b3_f64>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineVectorTest<b3_vector>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3SplineVectorTest<b3_vector4D>);

template<> void b3SplineVectorTest<b3_f64>::setUp()
{
	a = 3.0;
	b = 2.0;
	b3SplineVector::b3Clear(result);
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

template<> void b3SplineVectorTest<b3_vector>::setUp()
{
	a.x = 3.0;
	a.y = 0.0;
	a.z = 4.0;

	b.x = 0.0;
	b.y = 2.0;
	b.z = 1.0;

	b3SplineVector::b3Clear(result);
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

template<> void b3SplineVectorTest<b3_vector4D>::setUp()
{
	a.x = 3.0;
	a.y = 0.0;
	a.z = 4.0;
	a.w = 1.0;

	b.x = 0.0;
	b.y = 2.0;
	b.z = 1.0;
	b.w = 1.5;

	b3SplineVector::b3Clear(result);
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

template<> void b3SplineVectorTest<b3_f64>::test()
{
	CPPUNIT_ASSERT_EQUAL(0.0, result);

	result = b3SplineVector::b3Sub(a, b);
	CPPUNIT_ASSERT_EQUAL(1.0, result);

	b3SplineVector::b3Homogenize(result);
	CPPUNIT_ASSERT_EQUAL(1.0, result);

	b3SplineVector::b3AddScaled(2.5, a, result);
	CPPUNIT_ASSERT_EQUAL(8.5, result);

	result = b3SplineVector::b3LinearCombine(a, b, 2.5);
	CPPUNIT_ASSERT_EQUAL(8.0, result);

	result = b3SplineVector::b3Mix(a, b, 0.25);
	CPPUNIT_ASSERT_EQUAL(2.75, result);

	result = b3SplineVector::b3WeightInit(a, 0.75);
	CPPUNIT_ASSERT_EQUAL( 3.0, result);
}

template<> void b3SplineVectorTest<b3_vector>::test()
{
	CPPUNIT_ASSERT_EQUAL(0.0f, result.x);
	CPPUNIT_ASSERT_EQUAL(0.0f, result.y);
	CPPUNIT_ASSERT_EQUAL(0.0f, result.z);

	result = b3SplineVector::b3Sub(a, b);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, result.y);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.z);

	b3SplineVector::b3Homogenize(result);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, result.y);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.z);

	b3SplineVector::b3AddScaled(2.5, a, result);
	CPPUNIT_ASSERT_EQUAL(10.5f, result.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, result.y);
	CPPUNIT_ASSERT_EQUAL(13.0f, result.z);

	result = b3SplineVector::b3LinearCombine(a, b, 2.5);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.x);
	CPPUNIT_ASSERT_EQUAL( 5.0f, result.y);
	CPPUNIT_ASSERT_EQUAL( 6.5f, result.z);

	result = b3SplineVector::b3Mix(a, b, 0.25);
	CPPUNIT_ASSERT_EQUAL( 2.25f, result.x);
	CPPUNIT_ASSERT_EQUAL( 0.5f,  result.y);
	CPPUNIT_ASSERT_EQUAL( 3.25f, result.z);

	result = b3SplineVector::b3WeightInit(a, 0.75);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.x);
	CPPUNIT_ASSERT_EQUAL( 0.0f, result.y);
	CPPUNIT_ASSERT_EQUAL( 4.0f, result.z);
}

template<> void b3SplineVectorTest<b3_vector4D>::test()
{
	CPPUNIT_ASSERT_EQUAL(0.0f, result.x);
	CPPUNIT_ASSERT_EQUAL(0.0f, result.y);
	CPPUNIT_ASSERT_EQUAL(0.0f, result.z);
	CPPUNIT_ASSERT_EQUAL(0.0f, result.w);

	result = b3SplineVector::b3Sub(a, b);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, result.y);
	CPPUNIT_ASSERT_EQUAL( 3.0f, result.z);
	CPPUNIT_ASSERT_EQUAL(-0.5f, result.w);

	b3SplineVector::b3Homogenize(result);
	CPPUNIT_ASSERT_EQUAL(-6.0f, result.x);
	CPPUNIT_ASSERT_EQUAL( 4.0f, result.y);
	CPPUNIT_ASSERT_EQUAL(-6.0f, result.z);
	CPPUNIT_ASSERT_EQUAL(-0.5f, result.w);

	b3SplineVector::b3AddScaled(2.5, a, result);
	CPPUNIT_ASSERT_EQUAL( 1.5f, result.x);
	CPPUNIT_ASSERT_EQUAL( 4.0f, result.y);
	CPPUNIT_ASSERT_EQUAL( 4.0f, result.z);
	CPPUNIT_ASSERT_EQUAL( 2.0f, result.w);

	result = b3SplineVector::b3LinearCombine(a, b, 2.5);
	CPPUNIT_ASSERT_EQUAL( 3.0f,  result.x);
	CPPUNIT_ASSERT_EQUAL( 5.0f,  result.y);
	CPPUNIT_ASSERT_EQUAL( 6.5f,  result.z);
	CPPUNIT_ASSERT_EQUAL( 4.75f, result.w);

	result = b3SplineVector::b3Mix(a, b, 0.25);
	CPPUNIT_ASSERT_EQUAL( 2.25f,  result.x);
	CPPUNIT_ASSERT_EQUAL( 0.5f,   result.y);
	CPPUNIT_ASSERT_EQUAL( 3.25f,  result.z);
	CPPUNIT_ASSERT_EQUAL( 1.125f, result.w);

	result = b3SplineVector::b3WeightInit(a, 0.75);
	CPPUNIT_ASSERT_EQUAL( 2.25f, result.x);
	CPPUNIT_ASSERT_EQUAL( 0.0f,  result.y);
	CPPUNIT_ASSERT_EQUAL( 3.0f,  result.z);
	CPPUNIT_ASSERT_EQUAL( 0.75f, result.w);

	b3SplineVector::b3Homogenize(result);
	CPPUNIT_ASSERT_EQUAL( 3.0f,  result.x);
	CPPUNIT_ASSERT_EQUAL( 0.0f,  result.y);
	CPPUNIT_ASSERT_EQUAL( 4.0f,  result.z);
	CPPUNIT_ASSERT_EQUAL( 0.75f, result.w);

	result = b3SplineVector::b3WeightSelf(result);
	CPPUNIT_ASSERT_EQUAL( 2.25f, result.x);
	CPPUNIT_ASSERT_EQUAL( 0.0f,  result.y);
	CPPUNIT_ASSERT_EQUAL( 3.0f,  result.z);
	CPPUNIT_ASSERT_EQUAL( 0.75f, result.w);
}

#endif
