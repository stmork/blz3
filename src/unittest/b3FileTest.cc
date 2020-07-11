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
**                        Unit tests for file access                    **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3FileTest<b3File>);
CPPUNIT_TEST_SUITE_REGISTRATION(b3FileTest<b3FileMem>);

template <> b3_bool b3FileTest<b3File>::openRead(const char * filename)
{
	return file.b3Open(filename, B_READ);
}

template <> b3_bool b3FileTest<b3File>::openWrite(const char * filename)
{
	return file.b3Open(filename, B_WRITE);
}

template <> void b3FileTest<b3File>::removeFile(const char * filename)
{
	::remove(filename);
}

template <> b3_bool b3FileTest<b3FileMem>::openRead(const char * filename)
{
	return file.b3Open(filename, B_READ);
}

template <> b3_bool b3FileTest<b3FileMem>::openWrite(const char * filename)
{
	return file.b3Open(B_WRITE);
}

template <> void b3FileTest<b3FileMem>::removeFile(const char * filename)
{
}

#endif
