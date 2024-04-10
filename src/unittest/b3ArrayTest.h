/*
**
**	$Filename:	b3ArrayTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base and system unit tests
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_ARRAY_TEST_H
#define B3_ARRAY_TEST_H

#include "blz3/base/b3Array.h"
#include "b3TestMacros.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

template<typename T> class b3ArrayTest : public CppUnit::TestFixture
{
	b3Array<T>  array;

	CPPUNIT_TEST_SUITE(b3ArrayTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testUnique);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override
	{
		b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	}

	void tearDown() override
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void test()
	{
		b3AddElements(4,  4);
		b3AddElements(4,  8);
		b3AddElements(4, 12);

		T * buffer = array.b3GetBuffer();
		CPPUNIT_ASSERT(buffer != nullptr);

		CPPUNIT_ASSERT_NO_THROW(array.b3Sort());

		for (b3_loop i = 1; i < array.b3GetCount(); i++)
		{
			T l = array[i - 1];
			T r = array[i];

			CPPUNIT_ASSERT(l <= r);
		}

		array.b3Clear();
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 0, array.b3GetCount());

		b3Array<int> integers { 7, 34, 1, 4, 7643, 2, 324 };

		integers.b3Sort();
		for (b3_loop i = 1; i < integers.b3GetCount(); i++)
		{
			T l = integers[i - 1];
			T r = integers[i];

			CPPUNIT_ASSERT(l < r);
		}
	}

	inline b3_count b3AddElements(b3_count count, b3_count whole)
	{
		b3_loop i;
		T       element;

		for (i = 0; i < count; i++)
		{
			element = i;
			CPPUNIT_ASSERT_NO_THROW(array.b3Add(element));
		}
		CPPUNIT_ASSERT_EQUAL(whole, array.b3GetCount());

#ifdef _DEBUG
		CPPUNIT_ASSERT_NO_THROW(array[whole - 1]);
		CPPUNIT_ASSERT_THROW(array[whole], b3ArrayException);
#endif

		return count;
	}

	inline void testUnique()
	{
		static T zero[]   { };
		static T one[]    { 1 };
		static T two2[]   { 1, 2 };
		static T two1[]   { 1, 1 };
		static T four4[]  { 1, 2, 3, 4 };
		static T four3[]  { 1, 2, 3, 1 };
		static T four2[]  { 1, 2, 1, 2 };
		static T four22[] { 1, 2, 1, 1 };
		static T four1[]  { 1, 1, 1, 1 };
		static T five4a[] { 1, 1, 2, 3, 4 };
		static T five4b[] { 1, 2, 3, 4, 4 };
		static T five4c[] { 1, 2, 3, 3, 4 };

		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 0, b3MakeUnique<T>(zero,   0));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3MakeUnique<T>(one,    std::size(one)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3MakeUnique<T>(two2,   std::size(two2)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3MakeUnique<T>(two1,   std::size(two1)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3MakeUnique<T>(four1,  std::size(four1)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3MakeUnique<T>(four2,  std::size(four2)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3MakeUnique<T>(four22, std::size(four22)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 3, b3MakeUnique<T>(four3,  std::size(four3)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 4, b3MakeUnique<T>(four4,  std::size(four4)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 4, b3MakeUnique<T>(five4a, std::size(five4a)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 4, b3MakeUnique<T>(five4b, std::size(five4b)));
		CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 4, b3MakeUnique<T>(five4c, std::size(five4c)));
	}

	static int sorter(T * a, T * b)
	{
		return *b - *a;
	}
};

#endif
#endif
