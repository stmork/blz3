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

	static constexpr b3_f64 DURATION           = 4.0;
	static const     b3_res FRAMES_PER_SECONDS = 25;

	CPPUNIT_TEST_SUITE(b3MovieTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testUneven);
	CPPUNIT_TEST(testAudio);
	CPPUNIT_TEST(testAvi);
	CPPUNIT_TEST(testMpg);
	CPPUNIT_TEST(testMp4);
	CPPUNIT_TEST(testMkv);
	CPPUNIT_TEST(testMov);
	CPPUNIT_TEST(testWebm);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;

	void test();
	void testEmpty();
	void testUneven();
	void testAudio();
	void testAvi();
	void testMpg();
	void testMp4();
	void testMkv();
	void testMov();
	void testWebm();

private:
	void createEmptyVideo(const char * filename, const bool use_audio = false);
};

#endif
#endif
