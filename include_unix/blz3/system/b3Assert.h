/*
**
**	$Filename:	b3Assert.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1388 $
**	$Date: 2006-05-11 17:34:23 +0200 (Do, 11 Mai 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Assertion
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_ASSERT_H
#define B3_SYSTEM_ASSERT_H

#ifdef _DEBUG
#include <assert.h>

#define B3_ASSERT(a) assert(a)
#else
#define B3_ASSERT(a)
#endif

#endif
