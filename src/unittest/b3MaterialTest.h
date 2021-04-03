/*
**
**	$Filename:	b3MaterialTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for complex materials.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_MATERIAL_TEST_H
#define B3_MATERIAL_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3Color;
class b3Path;
class b3Scene;

class b3MaterialTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3MaterialTest);
	CPPUNIT_TEST(testGranite);
	CPPUNIT_TEST(testCookTorrance);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testGranite();
	void testCookTorrance();

private:
	static void b3SaveGranite();
	static void b3SaveCookTorrance(b3Color & color, int y);
	static void b3Save(b3Scene * scene, b3Path & path);
};

#endif
#endif
