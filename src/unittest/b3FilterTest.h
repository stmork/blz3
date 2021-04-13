/*
**
**	$Filename:	b3FilterTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for filtering.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_FILTER_TEST_H
#define B3_FILTER_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3Filter.h"

class b3FilterTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3FilterTest);
	CPPUNIT_TEST(testBoxFilter);
	CPPUNIT_TEST(testShutterFilter);
	CPPUNIT_TEST(testGaussFilter);
	CPPUNIT_TEST_SUITE_END();

	static const      unsigned STEPS = 40;
	static constexpr double    range = 10.0;
	static constexpr double    step  = range * 2.0 / STEPS;

public:
	void setUp() override;
	void tearDown() override;
	void testBoxFilter();
	void testShutterFilter();
	void testGaussFilter();

private:
	b3_f64 testFunc(const b3Filter & filter);
	void testIntegral(const b3Filter & filter);
};

#endif
#endif
