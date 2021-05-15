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

#include "blz3/base/b3Spline.h"

class b3AuxTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3AuxTest);
	CPPUNIT_TEST(testAux);
	CPPUNIT_TEST(testRect);
	CPPUNIT_TEST(testBound);
	CPPUNIT_TEST(testPoint);
	CPPUNIT_TEST(testStrCase);
	CPPUNIT_TEST(testStrCaseCmp);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testAux();
	void testRect();
	void testBound();
	void testPoint();
	void testStrCase();
	void testStrCaseCmp();
};

class b3NurbsTest : public CppUnit::TestFixture
{
protected:
	static constexpr double RADIUS = 10.0;

	b3Nurbs           m_Nurbs;
	b3Spline::b3_knot m_Knots[b3Spline::B3_MAX_KNOTS];
	b3_vector4D       m_Controls[b3Spline::B3_MAX_CONTROLS];

	void b3InitControlPoints();
	virtual void b3InitKnotVector() = 0;
};

class b3NurbsClosedTest : public b3NurbsTest
{
	CPPUNIT_TEST_SUITE(b3NurbsClosedTest);
	CPPUNIT_TEST(testCircle);
	CPPUNIT_TEST_SUITE_END();

	virtual void b3InitKnotVector() override;

public:
	void setUp() override;
	void tearDown() override;

	void testCircle();
};

class b3NurbsOpenedTest : public b3NurbsTest
{
	CPPUNIT_TEST_SUITE(b3NurbsOpenedTest);
	CPPUNIT_TEST(testCircle);
	CPPUNIT_TEST_SUITE_END();

	virtual void b3InitKnotVector() override;

public:
	void setUp() override;
	void tearDown() override;

	void testCircle();
};

#endif
#endif
