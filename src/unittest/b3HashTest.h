/*
**
**	$Filename:	b3HashTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit test for hash maps.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_HASH_TEST_H
#define B3_HASH_TEST_H

#include "blz3/base/b3Array.h"
#include "blz3/base/b3Hash.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

template<typename KEY, typename OBJECT> class b3HashTest : public CppUnit::TestFixture
{
	b3HashMap<KEY, OBJECT>  hash;
	b3Array<KEY>      *     keys;
	b3Array<OBJECT>    *    objects;

	CPPUNIT_TEST_SUITE(b3HashTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
		keys = null;
		objects = null;
	}

	void tearDown()
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);

		if(keys != null)
		{
			delete keys;
		}

		if(objects != null)
		{
			delete objects;
		}
	}

	void test()
	{
		KEY    key1 = 1;
		KEY    key2 = 2;
		OBJECT object1 = 1;
		OBJECT object2 = 2;

		CPPUNIT_ASSERT(hash.b3IsEmpty());
		CPPUNIT_ASSERT_NO_THROW(hash.b3Add(key1, object1));
		CPPUNIT_ASSERT_THROW(hash.b3Add(key1, object2), b3HashException);
		CPPUNIT_ASSERT(hash.b3Replace(key1, object2));
		CPPUNIT_ASSERT(!hash.b3Replace(key2, object1));
		CPPUNIT_ASSERT(!hash.b3IsEmpty());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(2), hash.b3GetCount());

		CPPUNIT_ASSERT_EQUAL(object2, *hash.b3Find(key1));
		CPPUNIT_ASSERT_EQUAL(object1, *hash.b3Find(key2));

		CPPUNIT_ASSERT(hash.b3Remove(key1));
		CPPUNIT_ASSERT(!hash.b3Remove(key1));
		CPPUNIT_ASSERT(!hash.b3HasKey(key1));
		CPPUNIT_ASSERT(hash.b3HasKey(key2));

		keys = hash.b3GetKeys();
		CPPUNIT_ASSERT(keys != null);
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(1), keys->b3GetCount());

		objects = hash.b3GetObjects();
		CPPUNIT_ASSERT(objects != null);
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(1), objects->b3GetCount());

		CPPUNIT_ASSERT_NO_THROW(hash.b3Clear());
		CPPUNIT_ASSERT(hash.b3IsEmpty());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_count>(0), hash.b3GetCount());
	}
};

#endif
#endif
