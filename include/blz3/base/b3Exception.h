/*
**
**	$Filename:  b3Exception.h $
**	$Release:   Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer: Steffen A. Mork $
**
**	Blizzard III - Exception template
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_EXCEPTION_H
#define B3_BASE_EXCEPTION_H

#include "blz3/b3Config.h"

typedef unsigned int b3_errno;
typedef unsigned int b3_excno;

#define B3_MK_ERRNO(e,t) (((b3_errno)(e) & 0xff) | ((b3_errno)(t) << 8))

typedef void         (*b3ExceptionLogger)(const b3_excno ExcNo,const b3_errno ErrNo);
typedef const char * (*b3ExceptionMsgFunc)(const b3_errno ErrNo);

class b3ExceptionBase
{
protected:
	static b3ExceptionLogger  m_Logger;
	static b3ExceptionMsgFunc m_GetMessage;

protected:
	                   b3ExceptionBase();
	static void        b3Log(const b3_excno ExcNo,const b3_errno ErrNo);
	static const char *b3GetMessage(const b3_errno ErrNo);

public:
	static void        b3SetLogger(b3ExceptionLogger logger = null);
	static void        b3SetMsgFunc(b3ExceptionMsgFunc converter = null);
};

template <class T,b3_excno C> class b3Exception : protected b3ExceptionBase
{
protected:
	b3_excno   m_ExceptionType;
	b3_errno   m_ErrorCode;

public:
	b3Exception(const T error)
	{
		m_ExceptionType = C;
		m_ErrorCode     = B3_MK_ERRNO(error,m_ExceptionType);
		m_Logger(m_ExceptionType,m_ErrorCode);	
	}

	inline const b3_errno b3GetError()
	{
		return m_ErrorCode;
	}

	inline const char *b3GetErrorMsg()
	{
		return m_GetMessage(errno);
	}
};

#endif
