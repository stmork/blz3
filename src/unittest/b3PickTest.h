/*
**
**	$Filename:	b3PickTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Tests for pick points.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_PICK_TEST_H
#define B3_PICK_TEST_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Pick.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3PickTest : public CppUnit::TestFixture
{
	b3PickBase m_PickPoints;

	CPPUNIT_TEST_SUITE(b3PickTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif
