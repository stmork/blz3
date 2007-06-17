/*
**
**	$Filename:	b3Profile.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Profile template
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_EXCEPTION_LOGGER_H
#define B3_EXCEPTION_LOGGER_H

#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Hash.h"

class CB3ExceptionLogger
{
	static CString                        m_LocalMessageString;
	static b3HashMap<b3_errno,UINT>       m_LocalMessageMap;
	static b3HashContainer<b3_errno,UINT> m_LocalMessages[];

public:
	CB3ExceptionLogger();
	static const char *b3GetMessage(const b3_errno ErrNo);
};

#endif
