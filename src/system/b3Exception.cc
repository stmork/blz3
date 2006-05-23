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

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Exception.h"
#include <ctype.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.9  2006/05/23 20:25:12  sm
**      - Some test updates.
**      - Changed exception handling a little bit.
**
**      Revision 1.8  2006/03/05 21:22:36  sm
**      - Added precompiled support for faster comiling :-)
**
**      Revision 1.7  2004/12/30 16:27:39  sm
**      - Removed assertion problem when starting Lines III: The
**        image list were initialized twice due to double calling
**        OnInitDialog() of CDialogBar. The CDialogBar::Create()
**        calls OnInitDialog() automatically sinde MFC 7
**      - Removed many global references from raytrace and base lib
**      - Fixed ticket no. 29: The b3RenderObject::b3Recompute
**        method checks the vertex maintainer against a null pointer.
**
**      Revision 1.6  2003/08/27 14:54:23  sm
**      - sprintf changed into snprintf to avoid buffer overflows.
**
**      Revision 1.5  2002/08/15 13:56:44  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.4  2002/08/11 06:38:54  sm
**      - Started some library reorganizations: Moved folowing classes into
**        system lib. Introduced new system library which is platform
**        independend but part of platform dependend library.
**        o b3FileAbstract
**        o b3DirAbstract
**        o b3Exception
**      - The following classes should be reorganized with abstract
**        base classes to:
**        o b3Date
**        o b3Time
**        o b3Log
**
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
