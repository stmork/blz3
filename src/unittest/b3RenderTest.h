/*
**
**	$Filename:	b3RenderTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Testing OpenGL rendering preparation.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RENDER_TEST_H
#define B3_RENDER_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3SearchPath.h"
#include "blz3/raytrace/b3RenderView.h"

class b3RenderTest : public CppUnit::TestFixture
{
	b3SearchPath search_path;

	CPPUNIT_TEST_SUITE(b3RenderTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();

private:
	void testView(b3RenderView & view, const b3_view_mode view_mode);
	bool testViewInfo(const b3_view_info & first, const b3_view_info & second, const b3_view_mode view_mode);
	bool testStepper(const b3RenderView & view, const b3_action_mode action_mode, const b3_f64 step = 0.5);
	b3_count testMover(const b3_f64 mover, const b3_f64 step);
};

#endif
#endif
