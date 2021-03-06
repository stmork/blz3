/*
**
**	$Filename:	b3Assert.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Debug assertion
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_ASSERT_H
#define B3_SYSTEM_ASSERT_H

#ifdef _DEBUG
#	include <assert.h>
#	define B3_ASSERT(cond) ASSERT(cond)
#else
#	define B3_ASSERT(cond)
#endif

#endif
