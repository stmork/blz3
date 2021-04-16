/*
**
**	$Filename:	b3PathTest.h $
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

#pragma once

#ifndef B3PATHTEST_H
#define B3PATHTEST_H

#include <blz3/b3Config.h>
#include <blz3/system/b3Dir.h>

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <unistd.h>

class b3PathTest : public CppUnit::TestFixture
{
	char m_CurrentDir[PATH_MAX];

	CPPUNIT_TEST_SUITE(b3PathTest);
	CPPUNIT_TEST(testSplitting);
	CPPUNIT_TEST(testExt);
	CPPUNIT_TEST(testAppend);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testSplitting();
	void testExt();
	void testAppend();

private:
	void testEmpty(const b3Path & path);
	void testEqual(const b3Path & left, const b3Path & right);
};

#endif
#endif // B3PATHTEST_H
