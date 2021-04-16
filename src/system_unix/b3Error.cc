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
#include "blz3/b3Config.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>

/*************************************************************************
**                                                                      **
**                        b3Error implementation                        **
**                                                                      **
*************************************************************************/

b3Error::b3Error()
{
	b3RetrieveError();
}

int b3Error::b3GetError()
{
	b3RetrieveError();
	return m_ErrorCode;
}

const char * b3Error::b3GetErrorText() const
{
	return m_ErrorText;
}

void b3Error::b3RetrieveError()
{
	m_ErrorCode = errno;

#ifdef HAVE_STRERROR
	strncpy(m_ErrorText, strerror(m_ErrorCode), sizeof(m_ErrorText));
#else
	snprintf(m_ErrorText, sizeof(m_ErrorText), "errno.: %d\n", m_ErrorCode);
#endif
}
