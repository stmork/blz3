/*
**
**	$Filename:	b3PathTest.cc $
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

#include "b3PathTest.h"

/*************************************************************************
**                                                                      **
**                        Unit test for directory handling              **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3PathTest);

void b3PathTest::setUp()
{
	getcwd(m_CurrentDir, sizeof(m_CurrentDir));

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3PathTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3PathTest::testSplitting()
{
	b3Path pwd(m_CurrentDir);
	b3Path full;
	b3Path file_splitted;
	b3Path dir_splitted;
	b3Path full_linked;

	full.b3LinkFileName(pwd, __FILE__);
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(pwd));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(full));

	full.b3SplitFileName(nullptr, nullptr);

	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(pwd));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(full));
	testEmpty(dir_splitted);
	testEmpty(file_splitted);

	full.b3SplitFileName(nullptr, file_splitted);

	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(pwd));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(full));
	testEmpty(dir_splitted);
	CPPUNIT_ASSERT_EQUAL(0, strcmp(__FILE__, file_splitted));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(file_splitted));

	file_splitted.b3Empty();
	testEmpty(file_splitted);

	full.b3SplitFileName(dir_splitted, nullptr);

	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(pwd));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(full));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(pwd, dir_splitted));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(dir_splitted));
	testEmpty(file_splitted);

	dir_splitted.b3Empty();
	testEmpty(dir_splitted);

	full.b3SplitFileName(dir_splitted, file_splitted);

	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(pwd));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(full));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_DIR,  b3Dir::b3Exists(dir_splitted));
	CPPUNIT_ASSERT_EQUAL(B3_TYPE_FILE, b3Dir::b3Exists(file_splitted));

	const char * file_ptr = file_splitted;
	const char * dir_ptr  = dir_splitted;

	CPPUNIT_ASSERT(file_ptr != nullptr);
	CPPUNIT_ASSERT(dir_ptr != nullptr);
	CPPUNIT_ASSERT(file_ptr != dir_ptr);
	CPPUNIT_ASSERT_EQUAL(0, strcmp(__FILE__, file_splitted));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(pwd, dir_splitted));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(__FILE__, file_ptr));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(pwd, dir_ptr));

	full_linked.b3LinkFileName(dir_splitted, file_splitted);
	CPPUNIT_ASSERT_EQUAL(0, strcmp(full, full_linked));
}

void b3PathTest::testExt()
{
	b3Path pwd(m_CurrentDir);
	b3Path full;
	b3Path ext_full;
	b3Path ext_file;
	b3Path rm_full;
	b3Path rm_file;
	b3Path linked;

	full.b3LinkFileName(pwd, __FILE__);

	ext_full = full;
	ext_full.b3ExtractExt();
	ext_file.b3ExtractExt(__FILE__);
	testEqual(ext_full, ext_file);

	full.b3SplitFileName(nullptr, rm_full);
	rm_full.b3RemoveExt();
	rm_file.b3RemoveExt(__FILE__);
	testEqual(rm_full, rm_file);

	linked.b3LinkFileName(pwd, rm_full);
	linked.b3Append(b3Path::EXT_DELIMITER);
	linked.b3Append(ext_full);
	testEqual(full, linked);
}

void b3PathTest::testEmpty(const b3Path & path)
{
	CPPUNIT_ASSERT_EQUAL(0, strcmp("", path));
	CPPUNIT_ASSERT_EQUAL(B3_NOT_EXISTANT, b3Dir::b3Exists(path));
}

void b3PathTest::testEqual(const b3Path & left, const b3Path & right)
{
	CPPUNIT_ASSERT_EQUAL(0, strcmp(left, right));
	CPPUNIT_ASSERT(left == right);
}

#endif
