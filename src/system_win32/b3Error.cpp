/*
**
**	$Filename:	b3Error.cpp $
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
	m_ErrorCode = GetLastError();
	m_ErrorText = NULL;
	b3ConvertError();	
}

b3Error::b3Error()
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
	if (m_ErrorText != null)
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
