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

class b3SelfTest
{
public:
	static void b3TestDataSize();
	static void b3TestMemory();
	static void b3TestDate();
	static void b3TestIO();

private:
	static void b3TestDir();
	static void b3TestFile(b3FileAbstract &file);
};

#endif
