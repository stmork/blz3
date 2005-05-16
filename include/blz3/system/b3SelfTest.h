/*
**
**	$Filename:	b3SelfTest.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Some basic self testing for platform compatibility tests.
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_SELFTEST_H
#define B3_SYSTEM_SELFTEST_H

#include "blz3/b3Types.h"

class b3SelfTest
{
public:
	static b3_bool b3TestDataSize();
	static b3_bool b3TestMemory();
	static b3_bool b3TestDate();
	static b3_bool b3TestIO();

private:
	static b3_bool b3TestDir();
	static b3_bool b3TestFile(b3FileAbstract &file);
};

#endif
