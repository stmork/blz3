/*
**
**	$Filename:	b3RandomTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for random number generation.
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
  
#include "b3RandomTest.h"

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
**                        Unit test for random number generation        **
**                                                                      **
*************************************************************************/

typedef b3RandomTest<b3PseudoRandom<int>,    int>    random_test_1;
typedef b3RandomTest<b3PseudoRandom<float>,  float>  random_test_2;
typedef b3RandomTest<b3PseudoRandom<double>, double> random_test_3;

typedef b3RandomTest<b3Rand48<int>,    int>    random_test_4;
typedef b3RandomTest<b3Rand48<float>,  float>  random_test_5;
typedef b3RandomTest<b3Rand48<double>, double> random_test_6;

CPPUNIT_TEST_SUITE_REGISTRATION(random_test_1);
CPPUNIT_TEST_SUITE_REGISTRATION(random_test_2);
CPPUNIT_TEST_SUITE_REGISTRATION(random_test_3);       
CPPUNIT_TEST_SUITE_REGISTRATION(random_test_4);
CPPUNIT_TEST_SUITE_REGISTRATION(random_test_5);       
CPPUNIT_TEST_SUITE_REGISTRATION(random_test_6);
