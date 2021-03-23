/*
**
**	$Filename:	b3ListTest.h $
**	$Release:	Dortmund 2006, 2016 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for doubly linked lists.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef TESTMACROS_H
#define TESTMACROS_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT

#define CPPUNIT_ASSERT_TYPED_EQUAL(t,e,a) CppUnit::assertEquals<t>((e), (a), CPPUNIT_SOURCELINE(), "")
#define CPPUNIT_ASSERT_SIZET_EQUAL(e,a)   CppUnit::assertEquals<size_t>((e),   (a), CPPUNIT_SOURCELINE(), "")
#define CPPUNIT_ASSERT_U32_EQUAL(e,a)     CppUnit::assertEquals<uint32_t>((e), (a), CPPUNIT_SOURCELINE(), "")

#endif

#endif
