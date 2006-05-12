/*
**
**	$Filename:	b3VectorTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for vector class templates.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_VECTOR_TEST_H
#define B3_VECTOR_TEST_H

#include "blz3/base/b3Vector.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

template<typename T, b3_loop dim> class b3VectorTest : public CppUnit::TestFixture
{
	typedef b3VectorTemplate<T, dim> VECTOR;

	VECTOR as, bs, cs, ds;

	CPPUNIT_TEST_SUITE(b3VectorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void test()
	{
		as.b3Init();
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(0.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(0.0), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(0.0), as[Z]);

		as.b3Zero();
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(0.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(0.0), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(0.0), as[Z]);

		as.b3Init(1.0, 2.0, 3.0);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(1.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(2.0), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(3.0), as[Z]);

		bs = as;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(1.0), bs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(2.0), bs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(3.0), bs[Z]);

		as.b3Init( -3.0, 2.0, -1.0);
		CPPUNIT_ASSERT(as != bs);
		bs = as;
		CPPUNIT_ASSERT(as == bs);
		bs.b3Negate();
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 3.0), bs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(-2.0), bs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 1.0), bs[Z]);

		bs.b3Init(2.0, -1.0, -2.0);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(-6.0),   VECTOR::b3SMul(as, bs));
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 3.0),   bs.b3Length());
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 9.0),   bs.b3QuadLength());
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 3.0),   bs.b3Normalize());
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 1.0),   bs.b3Length());
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 1.0),   bs.b3Normalize( 6.0));
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( 6.0),   bs.b3Length());

		as.b3Init( 1.0, -3.0, -4.0);
		bs.b3Init( 2.0, -1.0, -2.0);
		cs = as + bs;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  3.0), cs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -4.0), cs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -6.0), cs[Z]);
		cs = as - bs;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -1.0), cs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -2.0), cs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -2.0), cs[Z]);
		cs = as * bs;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), cs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  3.0), cs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  8.0), cs[Z]);
		cs = VECTOR::b3CrossProduct(as, bs);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), cs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -6.0), cs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  5.0), cs[Z]);
		bs += as;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  3.0), bs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -4.0), bs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -6.0), bs[Z]);
		bs -= as;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), bs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -1.0), bs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -2.0), bs[Z]);
		bs *= as;
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), bs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  3.0), bs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  8.0), bs[Z]);

		as.b3Init( 1.0, -1.0, 2.0);
		bs.b3Init( 2.0,  1.0, 4.0);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(3.0), VECTOR::b3Distance(as, bs));

		as.b3Init( 1.0,  1.5, 4.0);
		bs.b3Init( 2.0, -1.0, 2.0);
		cs = as;
		ds = bs;
		VECTOR::b3Sort(as, bs);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>( -1.0), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), as[Z]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), bs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.5), bs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  4.0), bs[Z]);	
		as.b3SetMinimum( 0.5);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  0.5), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), as[Z]);
		as.b3CheckLowerBound(bs);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  0.5), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), as[Z]);
		as.b3SetMaximum( 1.5);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  0.5), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.5), as[Z]);
		as.b3CheckUpperBound(bs);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), as[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.5), as[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  4.0), as[Z]);
		as.b3AdjustBound(cs, ds);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.0), cs[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.5), cs[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  4.0), cs[Z]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  2.0), ds[X]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  1.5), ds[Y]);
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(  4.0), ds[Z]);
	}
};

#endif
#endif
