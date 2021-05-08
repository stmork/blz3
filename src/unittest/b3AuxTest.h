/*
**
**	$Filename:	b3AuxTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Auxiliary for unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
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

class b3AuxTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3AuxTest);
	CPPUNIT_TEST(testAux);
	CPPUNIT_TEST(testRect);
	CPPUNIT_TEST(testPoint);
	CPPUNIT_TEST(testStrCase);
	CPPUNIT_TEST(testStrCaseCmp);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testAux();
	void testRect();
	void testPoint();
	void testStrCase();
	void testStrCaseCmp();
};

#endif
#endif
