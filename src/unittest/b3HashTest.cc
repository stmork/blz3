/*
**
**	$Filename:	b3HashTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision: 1454 $
**	$Date: 2006-07-02 10:10:09 +0200 (So, 02 Jul 2006) $
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
