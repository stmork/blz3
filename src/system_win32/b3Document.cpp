/*
**
**	$Filename:	b3Docment.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "blz3/system/b3Document.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III b3Document                       **
**                                                                      **
*************************************************************************/

b3Document::b3Document()
{
	m_Tx   = null;

	b3PrintF (B3LOG_FULL,"### CLASS: b3Document instantiated (%p)\n",this);
}

b3Document::~b3Document()
{
	// No page reference
	if (m_Tx != null)
	{
		delete m_Tx;
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3Document destroyed (%p)\n",this);
}

void b3Document::b3Lock(const char *CallingFunction)
{
	b3PrintF (B3LOG_FULL,"### CLASS: docBase  # b3Lock()   - %s\n",
		CallingFunction != null ? CallingFunction : "[-]");
	m_Mutex.b3Lock();
}

void b3Document::b3Unlock(const char *CallingFunction)
{
	m_Mutex.b3Unlock();
	b3PrintF (B3LOG_FULL,"### CLASS: docBase  # b3Unlock() - %s\n",
		CallingFunction != null ? CallingFunction : "[-]");
}

b3_bool b3Document::b3OpenDocument(const char *name)
{
	b3_bool result;

	if (m_Tx == null)
	{
		try
		{
			// This is a copy
			m_Tx  = new b3Tx;

		}
		catch (...)
		{
			m_Tx  = null;
		}
	}

	result = (m_Tx != null);
	return result;
}

void b3Document::b3Clear()
{
	b3Lock("b3Clear");
	m_Tx  = null;
	b3Unlock("b3Clear");
}

void b3Document::b3SetTexture(b3Tx *texture)
{
	// Use pages image
	b3Lock("b3SetTexture(b3Tx)");
	m_Tx = texture;
	b3Unlock("b3SetTexture(b3Tx)");
}

void b3Document::b3CloneTexture(b3Tx *tx)
{
	b3Lock("b3CloneTexture");

	// Create copy of pages image
	try
	{
		if (m_Tx != null)
		{
			delete m_Tx;
		}
		m_Tx = new b3Tx(tx);
	}
	catch (...)
	{
		// To prevent leaving a locked mutex
		m_Tx = null;
	}
	b3Unlock("b3CloneTexture");
}

void b3Document::b3Dump()
{
	b3PrintF (B3LOG_NORMAL,"### DUMP of b3Document class (%p)\n");
	b3PrintF (B3LOG_NORMAL,"###      m_Tx:     %p\n",m_Tx);
}

