/*
**
**	$Filename:	b3Exception.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Blizzard file structure loading
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
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

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Exception.h"
#include <ctype.h>

/*************************************************************************
**                                                                      **
**                        b3Exception implementation                    **
**                                                                      **
*************************************************************************/

char               b3ExceptionBase::m_LocalMessageBuffer[512];
b3ExceptionLogger  b3ExceptionBase::m_Logger;
b3ExceptionMsgFunc b3ExceptionBase::m_GetMessage;

b3ExceptionBase::b3ExceptionBase(
	const b3_errno  ErrNo,
	const b3_excno  ExcNo,
	const b3_count  lineno,
	const char     *filename)
{
	m_ErrorCode     = ErrNo;
	m_ExceptionType = ExcNo;
	m_LineNo        = lineno;
	m_FileName      = filename;

	if (m_Logger == null)     b3SetLogger(null);
	if (m_GetMessage == null) b3SetMsgFunc(null);

	m_Logger(this);	
}

void b3ExceptionBase::b3Log(const b3ExceptionBase *exception)
{
	b3PrintF(B3LOG_NORMAL,"EXCEPTION: %s\n",m_GetMessage(exception->m_ErrorCode));
	b3PrintF(B3LOG_FULL,  "     file: %s\n", exception->m_FileName);
	b3PrintF(B3LOG_FULL,  "     line: %5d\n",exception->m_LineNo);
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

	snprintf(m_LocalMessageBuffer,sizeof(m_LocalMessageBuffer),
		"errno: %c%c%c:%02x",
		isprint(a) ? a : '_',
		isprint(b) ? b : '_',
		isprint(c) ? c : '_',
		ErrNo        & 0xff);
	return m_LocalMessageBuffer;
}

const char *b3ExceptionBase::what() const throw()
{
	char buffer[128];

	b3GetErrorMsg();
	snprintf(buffer, sizeof(buffer), " file: %-32.32s line: %5d",m_FileName, m_LineNo);
	strncat(m_LocalMessageBuffer, buffer, sizeof(m_LocalMessageBuffer));

	return m_LocalMessageBuffer;
}

void b3ExceptionBase::b3SetMsgFunc(b3ExceptionMsgFunc converter)
{
	if (converter == null)
	{
		converter = &b3GetMessage;
	}
	m_GetMessage = converter;
}
