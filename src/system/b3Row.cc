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

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Display.h"

/*************************************************************************
**                                                                      **
**                        row constructor                               **
**                                                                      **
*************************************************************************/

b3Row::b3Row(const b3_coord y, const b3_res xSize) :
		b3Link<b3Row>(sizeof(b3Row)),
		m_xSize(xSize),
		m_y(y)
{
	m_buffer = (b3_color *)b3Alloc(sizeof(b3_color) * xSize);
}

b3Row::b3Row(const b3_coord y, const b3_res xSize, b3_color *buffer) :
		b3Link<b3Row>(sizeof(b3Row)),
		m_xSize(xSize),
		m_y(y)
{
	m_buffer = buffer;
}
