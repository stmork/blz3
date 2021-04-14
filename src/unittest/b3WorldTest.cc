/*
**
**	$Filename:	b3WorldTest.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3WorldTest.h"
#include "blz3/base/b3List.h"

/*************************************************************************
**                                                                      **
**                        Unit test for Blizzard item access            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3RegisterTest);
CPPUNIT_TEST_SUITE_REGISTRATION(b3WorldTest<0>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3WorldTest<1>);

template <> const char * b3WorldTest<0>::m_Filename = "Data1.bwd";
template <> const char * b3WorldTest<1>::m_Filename = "Data2.bwd";

void b3RegisterTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3RegisterTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3RegisterTest::test()
{
	for (b3_u32 class_type : b3Item::b3GetClassTypeList(false))
	{
		const b3Item * item = b3World::b3AllocNode(class_type, false);

		CPPUNIT_ASSERT_MESSAGE(b3Link<b3Item>::b3Format(class_type), item != nullptr);
		CPPUNIT_ASSERT_EQUAL(class_type & B3_CLASS_MASK, item->b3GetClass());

		delete item;
	}
}

#endif
