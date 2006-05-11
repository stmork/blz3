/*
**
**	$Filename:	b3WorldTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for Blizzard item access.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_WORLD_TEST_H
#define B3_WORLD_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3World.h"
#include "blz3/system/b3File.h"

using namespace std;

template<int index> class b3WorldTest : public CppUnit::TestFixture
{
	static const char *m_Filename;

	b3World   world1;
	b3World   world2;

	CPPUNIT_TEST_SUITE(b3WorldTest);
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
		b3File          srcFile;
		b3File          dstFile;
		b3_u08         *src = null;
		b3_u08         *dst = null;
		b3_size         srcSize = 0;
		b3_size         dstSize = 0;
		b3Item         *item1 = null;
		b3Item         *item2 = null;
		b3Base<b3Item> *base1 = null;
		b3Base<b3Item> *base2 = null;

		CPPUNIT_ASSERT(world1.b3Read(m_Filename, false));
		CPPUNIT_ASSERT(world1.b3Write("test1.bwd", false));
		CPPUNIT_ASSERT(world1.b3Read("test1.bwd", false));

		CPPUNIT_ASSERT_NO_THROW(item1 = world1.b3GetFirst());
		CPPUNIT_ASSERT(item1 != null);
		CPPUNIT_ASSERT_NO_THROW(base1 = world1.b3GetHead());
		CPPUNIT_ASSERT(base1 != null);

		CPPUNIT_ASSERT_NO_THROW(item2 = b3World::b3Clone(item1, false));
		CPPUNIT_ASSERT(item2 != null);
		CPPUNIT_ASSERT_NO_THROW(world2.b3SetFirst(item2));

		CPPUNIT_ASSERT_NO_THROW(base2 = world2.b3GetHead());
		CPPUNIT_ASSERT(base2 != null);
		CPPUNIT_ASSERT(world2.b3Write("test2.bwd", false));

		CPPUNIT_ASSERT_NO_THROW(src = srcFile.b3ReadBuffer("test1.bwd", srcSize));
		CPPUNIT_ASSERT(src != null);
		CPPUNIT_ASSERT(srcSize != null);

		CPPUNIT_ASSERT(world2.b3Read("test2.bwd", false));
		CPPUNIT_ASSERT_NO_THROW(dst = dstFile.b3ReadBuffer("test2.bwd", dstSize));
		CPPUNIT_ASSERT(dst != null);
		CPPUNIT_ASSERT(dstSize != null);
		
		CPPUNIT_ASSERT(srcSize == dstSize);
		CPPUNIT_ASSERT(memcmp(src, dst, srcSize));
	}
};

#endif
