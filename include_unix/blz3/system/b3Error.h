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

#ifndef B3_SYSTEM_ERROR_H
#define B3_SYSTEM_ERROR_H

#include "blz3/system/b3ErrorAbstract.h"

class b3Error : public b3ErrorBase
{
	int   m_ErrorCode;
	char  m_ErrorText[1024];

public:
	      b3Error();
	char *b3GetErrorText();
	int   b3GetError();

protected:
	void  b3RetrieveError();
};

#endif
