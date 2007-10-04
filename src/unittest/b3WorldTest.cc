/*
**
**	$Filename:	b3WorldTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for Blizzard item access.
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

#include "b3WorldTest.h"

/*************************************************************************
**                                                                      **
**                        Unit test for Blizzard item access            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3WorldTest<0>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3WorldTest<1>);

template <> const char *b3WorldTest<0>::m_Filename = "Data1.bwd";
template <> const char *b3WorldTest<1>::m_Filename = "Data2.bwd";

#endif
