/*
**
**	$Filename:	b3SkeletonTest.h $
**	$Release:	Dortmund 2024 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Skeleton for unit tests.
**
**      (C) Copyright 2001 - 2024  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_SKELETON_TEST_H
#define B3_SKELETON_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3SkeletonTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3SkeletonTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif
