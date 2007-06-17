/*
**
**	$Filename:	b3ErrorAbstract.h $
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
	virtual char *b3GetErrorText() = 0;

protected:
	/**
	 * This method retrieves an error.
	 */
	virtual void  b3RetrieveError() = 0;
};

#endif
