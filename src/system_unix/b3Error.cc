/*
**
**	$Filename:	b3Error.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Error.h"
#include <errno.h>
#include <string.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2005/01/21 15:41:06  smork
**	- Added error code/text preparation.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3Error implementation                        **
**                                                                      **
*************************************************************************/

b3Error::b3Error()
{
	m_ErrorCode = errno;
	strerror_r(m_ErrorCode,m_ErrorText,sizeof(m_ErrorText));
}

int b3Error::b3GetError()
{
	b3RetrieveError();
	return m_ErrorCode;
}

char *b3Error::b3GetErrorText()
{
	return m_ErrorText;
}

void b3Error::b3RetrieveError()
{
	m_ErrorCode = errno;
	strerror_r(m_ErrorCode,m_ErrorText,sizeof(m_ErrorText));
}
