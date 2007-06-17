/*
**
**	$Filename:	b3Error.cpp $
**	$Release:	Dortmund 2005 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3SystemInclude.h"
#include "blz3/system/b3Error.h"

/*************************************************************************
**                                                                      **
**                        b3Error implementation                        **
**                                                                      **
*************************************************************************/

b3Error::b3Error()
{
	m_ErrorCode = GetLastError();
	m_ErrorText = NULL;
	b3ConvertError();	
}

b3Error::~b3Error()
{
	if (m_ErrorText != NULL)
	{
		LocalFree(m_ErrorText);
	}
}

DWORD b3Error::b3GetError()
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
	m_ErrorCode = GetLastError();
	b3ConvertError();
}

void b3Error::b3ConvertError()
{
	if (m_ErrorText != NULL)
	{
		LocalFree(m_ErrorText);
		m_ErrorText = NULL;
	}

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        m_ErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &m_ErrorText,
        0, NULL );

}
