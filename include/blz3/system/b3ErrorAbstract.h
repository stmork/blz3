/*
**
**	$Filename:	b3ErrorAbstract.h $
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

#ifndef B3_SYSTEM_ERRORABSTRACT_H
#define B3_SYSTEM_ERRORABSTRACT_H

/**
 * This abstract class defines converting an error code into an error message.
 */
class b3ErrorBase
{
public:
	/**
	 * This method returns an error message.
	 * \return The error message.
	 */
	virtual char * b3GetErrorText() = 0;

protected:
	/**
	 * This method retrieves an error.
	 */
	virtual void  b3RetrieveError() = 0;
};

#endif
