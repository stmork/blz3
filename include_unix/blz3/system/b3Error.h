/*
**
**	$Filename:	b3Error.h $
**	$Release:	Dortmund 2005 $
**	$Revision: 1220 $
**	$Date: 2005-08-08 10:59:05 +0200 (Mo, 08 Aug 2005) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Maintain system error code and error string
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

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
	char *b3GetErrorText();

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
	void  b3RetrieveError();
};

#endif
