/*
**
**	$Filename:	b3LocaleTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Locale unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_LOCALE_TEST_H
#define B3_LOCALE_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3LocaleTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3LocaleTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();

private:
	void testIsoSequence(const b3_u08 * iso, const char * expected = nullptr);
	void testUtfSequence(const char * utf8);
};

#endif
#endif
