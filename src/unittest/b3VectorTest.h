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
#include "b3TestMacros.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

template<typename T, b3_loop dim> class b3VectorTest : public CppUnit::TestFixture
{
	typedef b3VectorTemplate<T, dim> VECTOR;

	constexpr static T DELTA = 0.00001;

	VECTOR as, bs, cs, ds;

	CPPUNIT_TEST_SUITE(b3VectorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override
	{
		b3PrintF(B3LOG_DEBUG, "Setup: %s<%zd, %u>\n", __FILE__, sizeof(T), dim);
	}

	void tearDown() override
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void test()
	{
		b3PrintF(B3LOG_FULL, "b3Init()\n");
		as.b3Init();
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, as[Z]);

		b3PrintF(B3LOG_FULL, "b3Zero()\n");
		as.b3Zero();
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 0.0, as[Z]);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		as.b3Init(1.0, 2.0, 3.0);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 1.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 2.0, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 3.0, as[Z]);

		b3PrintF(B3LOG_FULL, "assign\n");
		bs = as;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 1.0, bs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 2.0, bs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 3.0, bs[Z]);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		as.b3Init(-3.0, 2.0, -1.0);
		CPPUNIT_ASSERT(as != bs);

		b3PrintF(B3LOG_FULL, "assign\n");
		bs = as;
		CPPUNIT_ASSERT(as == bs);

		b3PrintF(B3LOG_FULL, "b3Negate()\n");
		bs.b3Negate();
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.0, bs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -2.0, bs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, bs[Z]);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		bs.b3Init(2.0, -1.0, -2.0);

		b3PrintF(B3LOG_FULL, "b3SMul()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.0,  VECTOR::b3SMul(as, bs), DELTA);

		b3PrintF(B3LOG_FULL, "b3Length()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,   bs.b3Length(), DELTA);

		b3PrintF(B3LOG_FULL, "b3QuadLength()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0,   bs.b3QuadLength(), DELTA);

		b3PrintF(B3LOG_FULL, "b3Normalize()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,   bs.b3Normalize(), DELTA);

		b3PrintF(B3LOG_FULL, "b3Length()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,   bs.b3Length(), DELTA);

		b3PrintF(B3LOG_FULL, "b3Normalize()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,   bs.b3Normalize(6.0), DELTA);

		b3PrintF(B3LOG_FULL, "b3Length()\n");
		CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0,   bs.b3Length(), DELTA);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		as.b3Init(1.0, -3.0, -4.0);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		bs.b3Init(2.0, -1.0, -2.0);

		b3PrintF(B3LOG_FULL, "+\n");
		cs = as + bs;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.0, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -4.0, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -6.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "-\n");
		cs = as - bs;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -1.0, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -2.0, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -2.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "*\n");
		cs = as * bs;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 2.0, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 3.0, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, 8.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "* scalar\n");
		cs = as * 2.0;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -6.0, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -8.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "/ scalar\n");
		cs = as / 2.0;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.5, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -1.5, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -2.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "negate\n");
		cs = -cs;
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -0.5, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.5, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "b3CrossProduct()\n");
		cs = VECTOR::b3CrossProduct(as, bs);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -6.0, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  5.0, cs[Z]);

		b3PrintF(B3LOG_FULL, "+=\n");
		bs += as;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.0, bs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -4.0, bs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -6.0, bs[Z]);

		b3PrintF(B3LOG_FULL, "-=\n");
		bs -= as;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, bs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -1.0, bs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -2.0, bs[Z]);

		b3PrintF(B3LOG_FULL, "*=\n");
		bs *= as;
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, bs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  3.0, bs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  8.0, bs[Z]);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		as.b3Init(1.0, -1.0, 2.0);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		bs.b3Init(2.0,  1.0, 4.0);

		b3PrintF(B3LOG_FULL, "b3Distance()\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<T>(3.0), VECTOR::b3Distance(as, bs));

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		as.b3Init(1.0,  1.5, 4.0);

		b3PrintF(B3LOG_FULL, "b3Init()\n");
		bs.b3Init(2.0, -1.0, 2.0);
		cs = as;
		ds = bs;

		b3PrintF(B3LOG_FULL, "b3Sort()\n");
		VECTOR::b3Sort(as, bs);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T, -1.0, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, as[Z]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, bs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.5, bs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.0, bs[Z]);

		as.b3SetMinimum(0.5);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.5, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, as[Z]);

		as.b3CheckLowerBound(bs);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.5, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, as[Z]);

		as.b3SetMaximum(1.5);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  0.5, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.5, as[Z]);

		as.b3CheckUpperBound(bs);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, as[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.5, as[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.0, as[Z]);

		as.b3AdjustBound(cs, ds);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.0, cs[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.5, cs[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.0, cs[Z]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  2.0, ds[X]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  1.5, ds[Y]);
		CPPUNIT_ASSERT_TYPED_EQUAL(T,  4.0, ds[Z]);
	}
};

#ifdef BLZ3_USE_OPENGL
class b3VectorOpenGlTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3VectorOpenGlTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();
};
#endif

#endif
#endif
