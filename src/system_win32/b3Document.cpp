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

#include "b3SystemInclude.h"
#include "blz3/system/b3Document.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2001/07/08 12:56:43  sm
**	- Now displaying Mandelbrot set. It's like switching all light on together.
**	
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

