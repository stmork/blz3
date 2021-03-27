/*
**
**	$Filename:	b3FileListTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for file lists.
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

#include "b3FileListTest.h"
#include "b3TestMacros.h"

#include <blz3/base/b3FileList.h>

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3FileListTest);

void b3FileListTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3FileListTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3FileListTest::test()
{
	b3FileList list;
	b3_count   count = 0;

	list.b3RecCreateList(".");
	for (b3FileEntry * entry = list.b3First(); entry != nullptr; entry = entry->Succ)
	{
		CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(entry->b3Name()));
		count++;
	}
	CPPUNIT_ASSERT_EQUAL(count, list.b3GetCount());

	list.b3Sort();
	CPPUNIT_ASSERT_EQUAL(count, list.b3GetCount());

	list.b3DeleteList();
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 0, list.b3GetCount());
}

#endif
