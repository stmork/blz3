/*
**
**	$Filename:	b3ConstantMapTest.h $
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

#ifndef B3_CONSTANT_MAP_TEST_H
#define B3_CONSTANT_MAP_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3ConstantMap.h"

typedef b3ConstantMap<int> constant_map;

class b3ConstantMapTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3ConstantMapTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	static const constant_map map;

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif
