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

#include "blz3/base/b3World.h"
#include "blz3/system/b3File.h"
#include "blz3/raytrace/b3Scene.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

template<int index> class b3WorldTest : public CppUnit::TestFixture
{
	static const char * m_Filename;

	b3World   world1;
	b3World   world2;

	CPPUNIT_TEST_SUITE(b3WorldTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override
	{
		b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
		b3RaytracingItems::b3Register();
	}

	void tearDown() override
	{
		world1.b3Free();
		world2.b3Free();
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void test()
	{
		b3File           srcFile;
		b3File           dstFile;
		b3_u08     *     src = nullptr;
		b3_u08     *     dst = nullptr;
		b3_size          srcSize = 0;
		b3_size          dstSize = 0;
		b3Item     *     item1 = nullptr;
		b3Item     *     item2 = nullptr;
		b3Base<b3Item> * base1 = nullptr;
		b3Base<b3Item> * base2 = nullptr;
		b3Path           cwd;

		getcwd(cwd, B3_FILESTRINGLEN);
		CPPUNIT_ASSERT(world1.b3Read(m_Filename, false));
		CPPUNIT_ASSERT(world1.b3Write("test1.bwd", false));
		CPPUNIT_ASSERT(world1.b3Read("test1.bwd", false));

		CPPUNIT_ASSERT_NO_THROW(item1 = world1.b3GetFirst());
		CPPUNIT_ASSERT(item1 != nullptr);
		CPPUNIT_ASSERT_NO_THROW(base1 = world1.b3GetHead());
		CPPUNIT_ASSERT(base1 != nullptr);

		CPPUNIT_ASSERT_NO_THROW(item2 = b3World::b3Clone(item1, false));
		CPPUNIT_ASSERT(item2 != nullptr);
		CPPUNIT_ASSERT_NO_THROW(world2.b3SetFirst(item2));

		CPPUNIT_ASSERT_NO_THROW(base2 = world2.b3GetHead());
		CPPUNIT_ASSERT(base2 != nullptr);
		CPPUNIT_ASSERT(world2.b3Write("test2.bwd", false));

		CPPUNIT_ASSERT_NO_THROW(src = srcFile.b3ReadBuffer("test1.bwd", srcSize));
		CPPUNIT_ASSERT(src != nullptr);
		CPPUNIT_ASSERT(srcSize != 0);

		CPPUNIT_ASSERT(world2.b3Read("test2.bwd", false));
		CPPUNIT_ASSERT_NO_THROW(dst = dstFile.b3ReadBuffer("test2.bwd", dstSize));
		CPPUNIT_ASSERT(dst != nullptr);
		CPPUNIT_ASSERT(dstSize != 0);

		CPPUNIT_ASSERT(srcSize == dstSize);
		CPPUNIT_ASSERT(memcmp(src, dst, srcSize));
	}
};

#endif
#endif
