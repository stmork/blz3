/*
**
**	$Filename:	b3Error.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Maintain system error code and error string
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_ERROR_H
#define B3_SYSTEM_ERROR_H

#include "blz3/system/b3ErrorAbstract.h"

/**
 * This class converts an error code into an error message.
 */
class b3Error : public b3ErrorBase
{
	int   m_ErrorCode;
	char  m_ErrorText[1024];

public:
	/**
	 * This constructor initializes the error code interpreter.
	 */
	b3Error();
	char * b3GetErrorText() override;

	/**
	 * This method returns the error code.
	 *
	 * @return The error code of this instance.
	 */
	int   b3GetError();

protected:
	/**
	 * This method computes the error message from an error code.
	 */
	void  b3RetrieveError() override;
};

#endif
