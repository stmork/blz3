/*
**
**	$Filename:	b3HashTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit test for hash maps.
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
  
#include "b3HashTest.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/05/12 14:06:28  smork
**	- Added configurable CPPUNIT tests.
**
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
**                        Unit test for hash maps                       **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

typedef b3HashTest<int,  int>    hash_test_1;
typedef b3HashTest<int,  double> hash_test_2;
typedef b3HashTest<char, int>    hash_test_3;
typedef b3HashTest<int,  float>  hash_test_4;

CPPUNIT_TEST_SUITE_REGISTRATION(hash_test_1);
CPPUNIT_TEST_SUITE_REGISTRATION(hash_test_2);
CPPUNIT_TEST_SUITE_REGISTRATION(hash_test_3);
CPPUNIT_TEST_SUITE_REGISTRATION(hash_test_4);

#endif
