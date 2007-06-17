/*
**
**	$Filename:	b3Error.cc $
**	$Release:	Dortmund 2005 $
**	$Revision: 1454 $
**	$Date: 2006-07-02 10:10:09 +0200 (So, 02 Jul 2006) $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Error.h"
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
	m_ErrorCode = errno;
#ifdef HAVE_STRERROR
	strerror_r(m_ErrorCode,m_ErrorText,sizeof(m_ErrorText));
#else
	snprintf(m_ErrorText,sizeof(m_ErrorText),"errno.: %d\n",m_ErrorCode);
#endif
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
#ifdef HAVE_STRERROR
	strerror_r(m_ErrorCode,m_ErrorText,sizeof(m_ErrorText));
#else
	snprintf(m_ErrorText,sizeof(m_ErrorText),"errno.: %d\n",m_ErrorCode);
#endif
}
