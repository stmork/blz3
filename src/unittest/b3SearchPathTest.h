/*
**
**	$Filename:	b3SearchPathTest.h $
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

#pragma once

#ifndef B3_SEARCHPATH_TEST_H
#define B3_SEARCHPATH_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3SearchPath.h"

class b3SearchPathTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3SearchPathTest);
	CPPUNIT_TEST(testValid);
	CPPUNIT_TEST(testCut);
	CPPUNIT_TEST_SUITE_END();

	b3SearchPath path;

public:
	void setUp() override;
	void tearDown() override;
	void testValid();
	void testCut();
};

#endif
#endif
