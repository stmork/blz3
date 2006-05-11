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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3Array.h"

using namespace std;

template<typename T> class b3ArrayTest : public CppUnit::TestFixture
{
	b3Array<T>  array;

	CPPUNIT_TEST_SUITE(b3ArrayTest);
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
		CPPUNIT_ASSERT(array.b3GetBuffer() != null);
		b3AddElements(4,  4);
		b3AddElements(4,  8);
		array.b3SetIncrement(1);
		b3AddElements(4, 12);
		CPPUNIT_ASSERT_NO_THROW(array.b3Sort(sorter));
		array.b3Clear();
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(0), array.b3GetCount());
	}

	inline b3_count b3AddElements(b3_count count, b3_count whole)
	{
		b3_loop i;
		T       element;

		for (i = 0;i < count;i++)
		{
			element = i;
			CPPUNIT_ASSERT_NO_THROW(array.b3Add(element));
		}
		CPPUNIT_ASSERT_EQUAL(whole, array.b3GetCount());

#ifdef _DEBUG
		CPPUNIT_ASSERT_NO_THROW(array[whole-1]);
		CPPUNIT_ASSERT_THROW(array[whole], b3ArrayException);
#endif
		
		return count;
	}
	
	static int sorter(T *a, T *b)
	{
		return *b - *a;
	}
};

#endif
