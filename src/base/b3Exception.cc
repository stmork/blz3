/*
**
**      $Filename:      b3Exception.cc $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Blizzard file structure loading
**
**      (C) Copyright 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Exception.h"
#include <ctype.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.3  2002/08/07 17:25:01  sm
**      - Added new error messages
**      - Changed exception handling a little bit
**
**      Revision 1.2  2002/08/07 14:26:23  sm
**      - Introduced mapping from Blizzard III error codes to human
**        readable error messages supplied from Windows resources.
**
**      Revision 1.1  2002/08/07 12:38:43  sm
**      - Modified exception definition. Exceptions are identified with
**        a three character code to unify error codes. This is necessary
**        to convert error codes into error messages inside applications.
**      - Added some additional b3Hash methods.
**      - Added -Wall compiler option to all C++ files.
**      - Removed some compiler warnings.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3Exception implementation                    **
**                                                                      **
*************************************************************************/

static char        LocalMessageBuffer[512];
b3ExceptionLogger  b3ExceptionBase::m_Logger;
b3ExceptionMsgFunc b3ExceptionBase::m_GetMessage;

b3ExceptionBase::b3ExceptionBase(
	const b3_errno ErrNo,
	const b3_excno ExcNo)
{
	m_ErrorCode     = ErrNo;
	m_ExceptionType = ExcNo;

	if (m_Logger == null)     b3SetLogger(null);
	if (m_GetMessage == null) b3SetMsgFunc(null);

	m_Logger(m_ExceptionType,m_ErrorCode);	
}

const b3_errno b3ExceptionBase::b3GetError()
{
	return m_ErrorCode;
}

const char *b3ExceptionBase::b3GetErrorMsg()
{
	return m_GetMessage(m_ErrorCode);
}

void b3ExceptionBase::b3Log(const b3_excno ExcNo,const b3_errno ErrNo)
{
	b3PrintF(B3LOG_NORMAL,"EXCEPTION: %s\n",m_GetMessage(ErrNo));
}

void b3ExceptionBase::b3SetLogger(b3ExceptionLogger logger)
{
	if (logger == null)
	{
		logger = &b3Log;
	}
	m_Logger = logger;
}

const char *b3ExceptionBase::b3GetMessage(const b3_errno ErrNo)
{
	char a,b,c;

	a = (ErrNo >> 24) & 0xff;
	b = (ErrNo >> 16) & 0xff;
	c = (ErrNo >>  8) & 0xff;

	sprintf(LocalMessageBuffer,"errno: %c%c%c:%02x",
		isprint(a) ? a : '_',
		isprint(b) ? b : '_',
		isprint(c) ? c : '_',
		ErrNo        & 0xff);
	return LocalMessageBuffer;
}

void b3ExceptionBase::b3SetMsgFunc(b3ExceptionMsgFunc converter)
{
	if (converter == null)
	{
		converter = &b3GetMessage;
	}
	m_GetMessage = converter;
}