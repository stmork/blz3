/*
**
**	$Filename:	b3Pick.cc $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Handling simple pick points
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

#include "blz3/base/b3Pick.h"

#define SEGMENTS 6
#define LINES_PER_SEGMENT 3

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/12/06 16:21:56  sm
**	- Finished CB3ControlLDC - very nice!
**
**	Revision 1.1  2001/12/04 18:23:25  sm
**	- Drawing LDC correctly
**	- Added pick point support.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Pick implementation                         **
**                                                                      **
*************************************************************************/

b3PickBase::b3PickBase()
{
	b3InitBase();
	m_Selected = null;
}

b3_bool b3PickBase::b3Down(b3_coord x,b3_coord y)
{
	b3Pick *pick;

	B3_FOR_BASE(this,pick)
	{
		if (pick->b3Hit(x,y))
		{
			m_Selected = pick;
			return true;
		}
	}
	return false;
}

b3_bool b3PickBase::b3Move(b3_coord x,b3_coord y)
{
	if (m_Selected != null)
	{
		return m_Selected->b3Moved(x,y);
	}
	return false;
}

b3_bool b3PickBase::b3Up(b3_coord x,b3_coord y)
{
	b3_bool result;

	result = m_Selected != null;
	m_Selected = null;
	return result;
}

void b3PickBase::b3Draw(b3DrawContext *dc)
{
	b3Pick *pick;

	B3_FOR_BASE(this,pick)
	{
		pick->b3Draw(dc);
	}
}

/*************************************************************************
**                                                                      **
**                        b3Pick implementation                         **
**                                                                      **
*************************************************************************/

b3Pick::b3Pick(b3_coord x,b3_coord y) : b3Link<b3Pick>(sizeof(b3Pick))
{
	m_x = x;
	m_y = y;
}

b3Pick::~b3Pick()
{
}

void b3Pick::b3Draw(b3DrawContext *dc)
{
}

b3_bool b3Pick::b3Hit(b3_coord x,b3_coord y)
{
	b3_coord xDiff = m_x - x;
	b3_coord yDiff = m_y - y;
	b3_bool  result;

	return (B3_ABS(xDiff) <= B3_PICK_SIZE) && (B3_ABS(yDiff) <= B3_PICK_SIZE);
}

b3_bool b3Pick::b3Moved(b3_coord x,b3_coord y)
{
	b3_bool result;

	result = (x != m_x) || (y != m_y);
	if (result)
	{
		m_x = x;
		m_y = y;
	}
	return result;
}
