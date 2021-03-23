/*
**
**	$Filename:	b3SelfTestSuite.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - self tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3SELFTEST_H
#define B3SELFTEST_H

#include <blz3/system/b3SelfTest.h>

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3SelfTestSuite : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3SelfTestSuite);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void test();
};

#endif

#endif // B3SELFTEST_H
