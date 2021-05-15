/*
**
**	$Filename:	b3NurbsTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for NURBS.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_NURBS_TEST_H
#define B3_NURBS_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3Spline.h"

class b3NurbsCurveTest : public CppUnit::TestFixture
{
protected:
	static constexpr double RADIUS = 10.0;

	b3Nurbs           m_Nurbs;
	b3Spline::b3_knot m_Knots[b3Nurbs::B3_MAX_KNOTS];
	b3_vector4D       m_Controls[b3Nurbs::B3_MAX_CONTROLS];

	b3_vector4D       m_Deboor[b3Nurbs::B3_MAX_SUBDIV];
	b3_vector4D       m_Mansfield[b3Nurbs::B3_MAX_SUBDIV];
	b3_f64            m_Radius[b3Nurbs::B3_MAX_SUBDIV + 1];
	b3_f64            m_BasisCoeff[b3Nurbs::B3_MAX_DEGREE];

	void b3InitArrays();
	void b3InitControlPoints();
	virtual void b3InitKnotVector() = 0;

public:
	virtual void tearDown() override;
	virtual void testCircle() = 0;

	void testInsertValidation();
	void testInsertion();
};

class b3NurbsClosedCurveTest : public b3NurbsCurveTest
{
	CPPUNIT_TEST_SUITE(b3NurbsClosedCurveTest);
	CPPUNIT_TEST(testCircle);
	CPPUNIT_TEST(testInsertValidation);
	CPPUNIT_TEST(testInsertion);
	CPPUNIT_TEST_SUITE_END();

	virtual void b3InitKnotVector() override;

public:
	void setUp() override;

	void testCircle() override;
};

class b3NurbsOpenedCurveTest : public b3NurbsCurveTest
{
	CPPUNIT_TEST_SUITE(b3NurbsOpenedCurveTest);
	CPPUNIT_TEST(testCircle);
	CPPUNIT_TEST(testInsertValidation);
	CPPUNIT_TEST(testInsertion);
	CPPUNIT_TEST_SUITE_END();

	virtual void b3InitKnotVector() override;

public:
	void setUp() override;

	void testCircle() override;
};

#endif
#endif
