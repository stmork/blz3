/*
**
**	$Filename:	b3SearchPathTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for search paths.
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

#include "b3SearchPathTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3SearchPathTest);

void b3SearchPathTest::setUp()
{
	path.b3AddPath(nullptr);
	path.b3AddPath(getenv("WORKSPACE"));
	path.b3AddPath(getenv("BLZ3_HOME"));
	path.b3AddPath("../..");

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3SearchPathTest::tearDown()
{
	path.b3Empty();
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3SearchPathTest::testValid()
{
	b3Path       file("src/unittest");
	b3Path       fqfn;

	file += __FILE__;

	CPPUNIT_ASSERT(!path.b3IsValid(nullptr));
	CPPUNIT_ASSERT(!path.b3IsValid("XYZ"));
	CPPUNIT_ASSERT(path.b3IsValid("Makefile"));
	CPPUNIT_ASSERT(path.b3IsValid(file, fqfn));

	path.b3Empty();
	fqfn = "XYZ";
	CPPUNIT_ASSERT(!path.b3IsValid(file, fqfn));
	CPPUNIT_ASSERT(strcmp(fqfn, "XYZ") == 0);
}

void b3SearchPathTest::testCut()
{
	b3Path       file("src/unittest");
	b3Path       fqfn;
	b3Path       result;

	file += __FILE__;
	CPPUNIT_ASSERT(path.b3IsValid(file, fqfn));
	CPPUNIT_ASSERT_THROW(path.b3CutName(fqfn, nullptr), std::invalid_argument);
	CPPUNIT_ASSERT(path.b3CutName(fqfn, result));
}

#endif
