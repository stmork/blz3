/*
**
**	$Filename:	b3Document.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 28 $
**	$Date: 2001-07-08 15:02:19 +0200 (So, 08 Jul 2001) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - base for some MFC document classes
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DOCUMENT_H
#define B3_SYSTEM_DOCUMENT_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Thread.h"
#include "blz3/image/b3Tx.h"

class b3Document
{
	b3Mutex *m_Mutex;
public:
	b3Tx    *m_Tx;

public:
	        b3Document();
	       ~b3Document();
	void    b3Dump();
	void    b3Lock  (const char *CallingFunction = null);
	void    b3Unlock(const char *CallingFunction = null);
};

#endif
