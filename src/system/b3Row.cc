/*
**
**	$Filename:	b3Row.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Controlling one single row
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

#include "blz3/base/b3Display.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**
**	Revision 1.1  2001/11/04 10:54:14  sm
**	- Redesigned b3Display for control use.
**	
*/

/*************************************************************************
**                                                                      **
**                        row constructor                               **
**                                                                      **
*************************************************************************/

b3Row::b3Row(b3_coord y,b3_res xSize) :
	b3Link<b3Row>(sizeof(b3Row))
{
	m_xSize  = xSize;
	m_y      = y;
	m_buffer = (b3_pkd_color *)b3Alloc(sizeof(b3_pkd_color) * xSize);
}

b3Row::b3Row(b3_coord y,b3_res xSize,b3_pkd_color *buffer) :
	b3Link<b3Row>(sizeof(b3Row))
{
	m_xSize  = xSize;
	m_y      = y;
	m_buffer = buffer;
}
