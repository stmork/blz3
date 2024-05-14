/*
**
**	$Filename:	b3Assert.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Assertion
**
**	(C) Copyright 2006 - 2024  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_SYSTEM_ASSERT_H
#define B3_SYSTEM_ASSERT_H

#ifdef _DEBUG
#include <cassert>

#define B3_ASSERT(a) assert(a)
#else
#define B3_ASSERT(a)
#endif

#endif
