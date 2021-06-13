/*
**
**	$Filename:	b3DirTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Directory access unit tests.
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

#include "b3DirTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3DirTest);

void b3DirTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3DirTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3DirTest::test()
{
	const char * fail_name = "X";
	const char * work_dir  = ".";
	const char * test_dir  = "/tmp/blz3-testdir";
	b3Path       path;
	b3Dir        dir;


	CPPUNIT_ASSERT_EQUAL(B3_NOT_EXISTANT, b3Dir::b3Exists(fail_name));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,     b3Dir::b3Exists(work_dir));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE,    b3Dir::b3Exists(__FILE__));

	CPPUNIT_ASSERT(!b3Dir::b3ChkDir(fail_name));
	CPPUNIT_ASSERT(b3Dir::b3ChkDir(work_dir));
	CPPUNIT_ASSERT(!b3Dir::b3ChkDir(__FILE__));

	CPPUNIT_ASSERT(!b3Dir::b3ChDir("/blz3"));
	CPPUNIT_ASSERT(b3Dir::b3ChDir(work_dir));

	CPPUNIT_ASSERT(!b3Dir::b3MkDir("/blz3"));
	CPPUNIT_ASSERT(b3Dir::b3MkDir(test_dir));
	CPPUNIT_ASSERT(b3Dir::b3MkDir(test_dir));
	CPPUNIT_ASSERT(b3Dir::b3RmDir(test_dir));
	CPPUNIT_ASSERT_EQUAL(B3_NOT_EXISTANT, b3Dir::b3Exists(test_dir));

	CPPUNIT_ASSERT_THROW(b3Dir("/blz3"), b3DirException);
	CPPUNIT_ASSERT_NO_THROW(b3Dir working(work_dir));

	CPPUNIT_ASSERT_NO_THROW(dir.b3OpenDir(work_dir));
	CPPUNIT_ASSERT(dir.b3DirNext(path) != B3_NOT_EXISTANT);
	while (dir.b3DirNext(path) != B3_NOT_EXISTANT)
	{
	}
}

#endif
