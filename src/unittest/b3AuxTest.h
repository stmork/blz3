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

class b3NurbsSurfaceTest : public CppUnit::TestFixture
{
	static constexpr double RADIUS = 10.0;

	b3Nurbs           m_Horizontal;
	b3Nurbs           m_Vertical;
	b3Spline::b3_knot m_HorizontalKnots[b3Nurbs::B3_MAX_KNOTS];
	b3Spline::b3_knot m_VerticalKnots[b3Nurbs::B3_MAX_KNOTS];
	b3_vector4D       m_Controls[b3Nurbs::B3_MAX_CONTROLS * b3Nurbs::B3_MAX_CONTROLS];

	b3_vector4D       m_Deboor[b3Nurbs::B3_MAX_SUBDIV];
	b3_vector4D       m_Mansfield[b3Nurbs::B3_MAX_SUBDIV];
	b3_f64            m_Radius[b3Nurbs::B3_MAX_SUBDIV + 1];
	b3_f64            m_BasisCoeff[b3Nurbs::B3_MAX_DEGREE];

	CPPUNIT_TEST_SUITE(b3NurbsSurfaceTest);
	CPPUNIT_TEST(testSphereHorizontally);
	CPPUNIT_TEST(testSphereVertically);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;

	void testSphereHorizontally();
	void testSphereVertically();
};

#endif
#endif
