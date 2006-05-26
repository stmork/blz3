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
**	Revision 1.5  2006/05/26 09:09:29  sm
**	- Fixed first run test error.
**
**	Revision 1.4  2006/05/15 12:31:26  sm
**	- Typo fix
**	
**	Revision 1.3  2006/05/15 12:30:36  smork
**	- Templating syntax error fixed.
**	- Searching for some test case problems.
**	
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
**                        Unit tests for file access                    **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3FileTest<b3File>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3FileTest<b3FileMem>);

template <> b3_bool b3FileTest<b3File>::openRead(const char *filename)
{
	return file.b3Open(filename, B_READ);
}

template <> b3_bool b3FileTest<b3File>::openWrite(const char *filename)
{
	return file.b3Open(filename, B_WRITE);
}

template <> void b3FileTest<b3File>::removeFile(const char *filename)
{
	::remove(filename);
}

template <> b3_bool b3FileTest<b3FileMem>::openRead(const char *filename)
{
	return file.b3Open(filename,B_READ);
}

template <> b3_bool b3FileTest<b3FileMem>::openWrite(const char *filename)
{
	return file.b3Open(B_WRITE);
}

template <> void b3FileTest<b3FileMem>::removeFile(const char *filename)
{
}

#endif
