/*
**
**	$Filename:	b3Pick.cc $  
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3BaseInclude.h"
#include "blz3/base/b3Pick.h"

#define SEGMENTS 6
#define LINES_PER_SEGMENT 3

/*************************************************************************
**                                                                      **
**                        b3Pick implementation                         **
**                                                                      **
*************************************************************************/

#define B3_PICK_SIZE 2

b3_coord b3PickBase::m_PickSize = B3_PICK_SIZE;

b3PickBase::b3PickBase()
{
	m_Selected = null;
}

b3_bool b3PickBase::b3IsActive()
{
	return m_Selected != null;
}

void b3PickBase::b3Update()
{
	b3Pick *pick;

	B3_FOR_BASE(this,pick)
	{
		pick->b3Update();
	}
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

b3UndoOperation *b3PickBase::b3GetOperation(b3RenderObject *object)
{
	return m_Selected != null ? m_Selected->b3GetOperation(object) : null;
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

void b3Pick::b3Update()
{
}

void b3Pick::b3Draw(b3DrawContext *dc)
{
}

b3_bool b3Pick::b3Hit(b3_coord x,b3_coord y)
{
	b3_coord xDiff = m_x - x;
	b3_coord yDiff = m_y - y;

	return
		(B3_ABS(xDiff) <= b3PickBase::m_PickSize) &&
		(B3_ABS(yDiff) <= b3PickBase::m_PickSize);
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
