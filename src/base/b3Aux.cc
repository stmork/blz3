/*
**
**	$Filename:	b3Aux.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some helpers
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

#include "b3BaseInclude.h"
#include "blz3/base/b3Aux.h"

/*************************************************************************
**                                                                      **
**                    Blizzard III color representation                 **
**                                                                      **
*************************************************************************/

b3RGB::b3RGB()
{
	r = 0xff;
	g = 0x11;
	b = 0x44;
}

/*************************************************************************
**                                                                      **
**                    Blizzard III rect class                           **
**                                                                      **
*************************************************************************/

b3Rect::b3Rect(
	const b3_coord x1,
	const b3_coord y1,
	const b3_coord x2,
	const b3_coord y2)
{
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
}

b3_res b3Rect::b3Width() const
{
	return m_x2 - m_x1;
}

b3_res b3Rect::b3Height() const
{
	return m_y2 - m_y1;
}

b3_bool b3Rect::b3UpdateBound(b3Rect * rect)
{
	b3_bool changed = false;

	if (rect->m_x1 < m_x1)
	{
		m_x1 = rect->m_x1;
		changed = true;
	}
	if (rect->m_y1 < m_y1)
	{
		m_y1 = rect->m_y1;
		changed = true;
	}
	if (rect->m_x2 > m_x2)
	{
		m_x2 = rect->m_x2;
		changed = true;
	}
	if (rect->m_y2 > m_y2)
	{
		m_y2 = rect->m_y2;
		changed = true;
	}
	return changed;
}

b3_bool b3Rect::b3CheckBound(b3Rect * rect)
{
	bool changed = false;

	if (m_x1 < rect->m_x1)
	{
		m_x1 = rect->m_x1;
		changed = true;
	}
	if (m_y1 < rect->m_y1)
	{
		m_y1 = rect->m_y1;
		changed = true;
	}
	if (m_x2 > rect->m_x2)
	{
		m_x2 = rect->m_x2;
		changed = true;
	}
	if (m_y2 > rect->m_y2)
	{
		m_y2 = rect->m_y2;
		changed = true;
	}
	if (m_x1 > m_x2)
	{
		m_x1 = m_x2;
		changed = true;
	}
	if (m_y1 > m_y2)
	{
		m_y1 = m_y2;
		changed = true;
	}
	return changed;
}
