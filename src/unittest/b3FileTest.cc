/*
**
**	$Filename:	b3FileTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for file access classes.
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
  
#include "b3FileTest.h"
#include "blz3/base/b3FileMem.h"

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
**                        Unit tests for file access                    **
**                                                                      **
*************************************************************************/

CPPUNIT_TEST_SUITE_REGISTRATION(b3FileTest<b3File>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3FileTest<b3FileMem>);

void b3FileTest<b3File>::setUp()
{
	file.b3Open("Config.tst",B_WRITE);
}

void b3FileTest<b3FileMem>::setUp()
{
	file.b3Open(B_WRITE);
}
