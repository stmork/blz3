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

#include "blz3/system/b3Display.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2005/10/09 12:05:34  sm
**	- Changed to HDR image computation.
**
**	Revision 1.3  2002/08/11 11:03:40  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**	
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
	m_buffer = (b3_color *)b3Alloc(sizeof(b3_color) * xSize);
}

b3Row::b3Row(b3_coord y,b3_res xSize,b3_color *buffer) :
	b3Link<b3Row>(sizeof(b3Row))
{
	m_xSize  = xSize;
	m_y      = y;
	m_buffer = buffer;
}
