/*
**
**	$Filename:	b3FileTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for file access classes.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_FILE_TEST_H
#define B3_FILE_TEST_H

#include "blz3/system/b3File.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

template<class FILE> class b3FileTest : public CppUnit::TestFixture
{
	FILE file;

	CPPUNIT_TEST_SUITE(b3FileTest);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	}

	void tearDown()
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	b3_bool openRead(const char * filename);
	b3_bool openWrite(const char * filename);
	void    removeFile(const char * filename);

	void testWrite()
	{
		char   array[1024];

		CPPUNIT_ASSERT(openWrite("Config.tst"));
		CPPUNIT_ASSERT(file.b3Buffer(2048));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(1024), file.b3Write(array, 1024));
		CPPUNIT_ASSERT(file.b3Flush());
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(1024), file.b3Write(array, 1024));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(1024), file.b3Write(array, 1024));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(1024), file.b3Write(array, 1024));
		CPPUNIT_ASSERT(file.b3Buffer(512));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(1024), file.b3Write(array, 1024));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(5120), file.b3Size());
		CPPUNIT_ASSERT_NO_THROW(file.b3Close());
		CPPUNIT_ASSERT_NO_THROW(removeFile("Config.tst"));
	}

	void testRead()
	{
		char   array[1024];

		CPPUNIT_ASSERT(openRead("Data1.bwd"));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(128), file.b3Read(array, 128));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(128), file.b3Seek(512, B3_SEEK_START));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(512), file.b3Seek(512, B3_SEEK_CURRENT));
		CPPUNIT_ASSERT_EQUAL(static_cast<b3_size>(1024), file.b3Seek(-120, B3_SEEK_END));
		CPPUNIT_ASSERT_EQUAL(file.b3Size() - 120, file.b3Seek(0, B3_SEEK_CURRENT));
		CPPUNIT_ASSERT_NO_THROW(file.b3Close());
	}
};

#endif
#endif
