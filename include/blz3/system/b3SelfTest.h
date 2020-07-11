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

#pragma once

#ifndef B3_SYSTEM_SELFTEST_H
#define B3_SYSTEM_SELFTEST_H

#include "blz3/b3Types.h"

/**
 * This class constains some self checks.
 */
class b3SelfTest
{
public:
	/**
	 * This method checks for correct data type sizes.
	 *
	 * \return Correct data sizes.
	 */
	static b3_bool b3TestDataSize();

	/**
	 * This method checks correct memory handling.
	 *
	 * \return Correct memory handling.
	 */
	static b3_bool b3TestMemory();

	/**
	 * This method checks for correct data handling.
	 *
	 * \return Correct data handling.
	 */
	static b3_bool b3TestDate();

	/**
	 * This method checks for correct IO handling.
	 *
	 * \return Correct IO handling.
	 */
	static b3_bool b3TestIO();

private:
	static b3_bool b3TestDir();
	static b3_bool b3TestFile(b3FileAbstract & file);
};

#endif
