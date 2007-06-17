/*
**
**	$Filename:	b3Docment.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3Document.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III b3Document                       **
**                                                                      **
*************************************************************************/

b3Document::b3Document()
{
	m_Tx    = new b3Tx;
	m_Mutex = new b3Mutex;

	b3PrintF (B3LOG_FULL,"### CLASS: b3Document instantiated (%p)\n",this);
}

b3Document::~b3Document()
{
	// No page reference
	if (m_Tx != null)
	{
		delete m_Tx;
	}
	if (m_Mutex != null)
	{
		delete m_Mutex;
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3Document destroyed (%p)\n",this);
}

void b3Document::b3Lock(const char *CallingFunction)
{
	b3PrintF (B3LOG_FULL,"### CLASS: docBase  # b3Lock()   - %s\n",
		CallingFunction != null ? CallingFunction : "[-]");
	m_Mutex->b3Lock();
}

void b3Document::b3Unlock(const char *CallingFunction)
{
	m_Mutex->b3Unlock();
	b3PrintF (B3LOG_FULL,"### CLASS: docBase  # b3Unlock() - %s\n",
		CallingFunction != null ? CallingFunction : "[-]");
}

void b3Document::b3Dump()
{
	b3PrintF (B3LOG_NORMAL,"### DUMP of b3Document class (%p)\n");
	b3PrintF (B3LOG_NORMAL,"###      m_Tx:     %p\n",m_Tx);
}

