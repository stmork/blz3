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
#include <string.h>

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
	const char   *  filename) :
	m_ExceptionType(ExcNo),
	m_ErrorCode(ErrNo),
	m_LineNo(lineno),
	m_FileName(filename)
{
	if (m_Logger == nullptr)
	{
		b3SetLogger(nullptr);
	}
	if (m_GetMessage == nullptr)
	{
		b3SetMsgFunc(nullptr);
	}

	m_Logger(this);
}

b3ExceptionBase::b3ExceptionBase(const b3ExceptionBase & exc)
{
	operator=(exc);
}

void b3ExceptionBase::b3Log(const b3ExceptionBase * exception)
{
	b3PrintF(B3LOG_NORMAL, "EXCEPTION: %s\n", m_GetMessage(exception->m_ErrorCode));
	b3PrintF(B3LOG_FULL,  "     file: %s\n", exception->m_FileName);
	b3PrintF(B3LOG_FULL,  "     line: %5d\n", exception->m_LineNo);
}

void b3ExceptionBase::b3SetLogger(b3ExceptionLogger logger)
{
	if (logger == nullptr)
	{
		logger = &b3Log;
	}
	m_Logger = logger;
}

const char * b3ExceptionBase::b3GetMessage(const b3_errno ErrNo)
{
	char a, b, c;

	a = (ErrNo >> 24) & 0xff;
	b = (ErrNo >> 16) & 0xff;
	c = (ErrNo >>  8) & 0xff;

	snprintf(m_LocalMessageBuffer, sizeof(m_LocalMessageBuffer),
		"errno: %c%c%c:%02x",
		isprint(a) ? a : '_',
		isprint(b) ? b : '_',
		isprint(c) ? c : '_',
		ErrNo        & 0xff);
	return m_LocalMessageBuffer;
}

b3ExceptionBase & b3ExceptionBase::operator=(const b3ExceptionBase & exc)
{
	m_ErrorCode     = exc.m_ErrorCode;
	m_ExceptionType = exc.m_ExceptionType;
	m_LineNo        = exc.m_LineNo;
	m_FileName      = exc.m_FileName;

	if (m_Logger == nullptr)
	{
		b3SetLogger(nullptr);
	}
	if (m_GetMessage == nullptr)
	{
		b3SetMsgFunc(nullptr);
	}

	m_Logger(this);

	return *this;
}

const char * b3ExceptionBase::what() const noexcept
{
	char buffer[128];

	b3GetErrorMsg();
	snprintf(buffer, sizeof(buffer), " file: %-32.32s line: %5d", m_FileName, m_LineNo);
	strncat(m_LocalMessageBuffer, buffer, sizeof(m_LocalMessageBuffer) - strlen(m_LocalMessageBuffer));

	return m_LocalMessageBuffer;
}

void b3ExceptionBase::b3SetMsgFunc(b3ExceptionMsgFunc converter)
{
	if (converter == nullptr)
	{
		converter = &b3GetMessage;
	}
	m_GetMessage = converter;
}
