/*
**
**	$Filename:	b3VectorTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for vector class templates.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "b3VectorTest.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2006/05/11 15:34:23  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**
**
*/

/*************************************************************************
**                                                                      **
**                        Unit test for vector class templates          **
**                                                                      **
*************************************************************************/

typedef b3VectorTest<b3_f32, 3> vector_test_f32_3;
typedef b3VectorTest<b3_f32, 4> vector_test_f32_4;
typedef b3VectorTest<b3_f64, 3> vector_test_f64_3;
typedef b3VectorTest<b3_f64, 4> vector_test_f64_4;

CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f32_3);
CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f32_4);
CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f64_3);
CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f64_4);
