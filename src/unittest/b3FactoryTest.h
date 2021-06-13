/*
**
**	$Filename:	b3FactoryTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Factory unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_FACTORY_TEST_H
#define B3_FACTORY_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <blz3/system/b3Display.h>

class b3Scene;

class b3FactoryTest : public CppUnit::TestFixture
{
	b3Scene  *  m_Scene = nullptr;
	b3Display   m_Display;

	CPPUNIT_TEST_SUITE(b3FactoryTest);
	CPPUNIT_TEST(testNew);
	CPPUNIT_TEST(testGlobal);
	CPPUNIT_TEST(testMaterial);
	CPPUNIT_TEST(testBump);
	CPPUNIT_TEST_SUITE_END();

public:
	b3FactoryTest();

	void setUp() override;
	void tearDown() override;
	void testNew();
	void testGlobal();
	void testMaterial();
	void testBump();
};

#endif
#endif
