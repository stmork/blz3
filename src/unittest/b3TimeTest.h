/*
**
**	$Filename:	b3SkeletonTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Time handling unit tests.
**
**      (C) Copyright 2006 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_SKELETON_TEST_H
#define B3_SKELETON_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class b3TimeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3TimeTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif
