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

#ifndef B3_SYSTEM_EXCEPTION_H
#define B3_SYSTEM_EXCEPTION_H

#include "blz3/b3Config.h"

typedef unsigned int b3_errno;
typedef unsigned int b3_excno;

#define B3_MK_ERRNO(e,t) (((b3_errno)(e) & 0xff) | ((b3_errno)(t) << 8))

class b3ExceptionBase;

typedef void         (*b3ExceptionLogger)(const b3ExceptionBase *exception);
typedef const char * (*b3ExceptionMsgFunc)(const b3_errno ErrNo);

class b3ExceptionBase
{
	static b3ExceptionLogger   m_Logger;
	static b3ExceptionMsgFunc  m_GetMessage;
	       b3_excno            m_ExceptionType;
	       b3_errno            m_ErrorCode;
		   b3_count            m_LineNo;
		   const char         *m_FileName;

protected:
	                   b3ExceptionBase(const b3_errno ErrNo,const b3_excno ExcNo,const b3_count lineno,const char *filename);
	static void        b3Log(const b3ExceptionBase *exception);
	static const char *b3GetMessage(const b3_errno ErrNo);

public:
	inline const b3_errno b3GetError()
	{
		return m_ErrorCode;
	}

	inline const char *b3GetErrorMsg()
	{
		return m_GetMessage(m_ErrorCode);
	}

	static void        b3SetLogger(b3ExceptionLogger logger = null);
	static void        b3SetMsgFunc(b3ExceptionMsgFunc converter = null);
};

template <class T,b3_excno C> class b3Exception : public b3ExceptionBase
{
public:
	b3Exception(
		const T     error,
		const char *FileName,
		const int   LineNo) : b3ExceptionBase(B3_MK_ERRNO(error,C),C,LineNo,FileName)
	{
	}
};

#define B3_THROW(e,no) throw e(no,__FILE__,__LINE__);

#endif
