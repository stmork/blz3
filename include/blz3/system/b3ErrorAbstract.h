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

#ifndef B3_SYSTEM_ERRORABSTRACT_H
#define B3_SYSTEM_ERRORABSTRACT_H

class b3ErrorBase
{
public:
	virtual char *b3GetErrorText() = 0;

protected:
	virtual void  b3RetrieveError() = 0;
};

#endif
