/*
**
**	$Filename:	b3ConstantMapTest.cc $
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

#include "b3ConstantMapTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ConstantMapTest);

const constant_map b3ConstantMapTest::map
{
	B3_CONSTANT(EINVAL),
	B3_CONSTANT(EBUSY)
};

void b3ConstantMapTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3ConstantMapTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3ConstantMapTest::test()
{
	CPPUNIT_ASSERT(map.at(EBUSY)  == "EBUSY");
	CPPUNIT_ASSERT(map.b3Get(EBUSY)  == "EBUSY");
	CPPUNIT_ASSERT(map.at(EINVAL) == "EINVAL");
	CPPUNIT_ASSERT(map.b3Get(EINVAL) == "EINVAL");
	CPPUNIT_ASSERT(map.b3Get(EAGAIN) == "0x0B");

	CPPUNIT_ASSERT(map.b3FindKey("EINVAL")->first == EINVAL);
	CPPUNIT_ASSERT(map.b3FindKey("XYZ") == map.end());

	CPPUNIT_ASSERT_THROW(map.at(EAGAIN), std::out_of_range);
}

#endif
