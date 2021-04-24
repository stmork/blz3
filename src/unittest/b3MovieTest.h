/*
**
**	$Filename:	b3MovieTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for movie streaming.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_MOVIE_TEST_H
#define B3_MOVIE_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/raytrace/b3Scene.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

#include "b3ImageSetup.h"

class b3MovieTest : public CppUnit::TestFixture, protected b3ImageSetup
{
	b3World               world;
	b3Scene       *       scene      = nullptr;
	b3Animation     *     animation  = nullptr;
	b3Distribute     *    distribute = nullptr;
	b3ShapeRenderContext  context;

	CPPUNIT_TEST_SUITE(b3MovieTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testUneven);
	CPPUNIT_TEST(testMpg);
	CPPUNIT_TEST(testMp4);
	CPPUNIT_TEST(testMkv);
	CPPUNIT_TEST(testHevc);
	CPPUNIT_TEST(testWebm);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;

	void test();
	void testEmpty();
	void testUneven();
	void testMpg();
	void testMp4();
	void testMkv();
	void testHevc();
	void testWebm();

private:
	void createEmptyVideo(const char * filename);
};

#endif
#endif
