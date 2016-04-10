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

const b3_bool b3PickBase::b3IsActive() const
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

const b3_bool b3PickBase::b3Down(const b3_coord x, const b3_coord y)
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

const b3_bool b3PickBase::b3Move(const b3_coord x, const b3_coord y)
{
	if (m_Selected != null)
	{
		return m_Selected->b3Moved(x,y);
	}
	return false;
}

const b3_bool b3PickBase::b3Up(const b3_coord x, const b3_coord y)
{
	b3_bool result;

	result = m_Selected != null;
	m_Selected = null;
	return result;
}

b3UndoOperation *b3PickBase::b3GetOperation(const b3RenderObject *object) const
{
	return m_Selected != null ? m_Selected->b3GetOperation(object) : null;
}

void b3PickBase::b3Draw(const b3DrawContext *dc)
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

b3Pick::b3Pick(const b3_coord x, const b3_coord y) : b3Link<b3Pick>(sizeof(b3Pick))
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

void b3Pick::b3Draw(const b3DrawContext *dc)
{
}

const b3_bool b3Pick::b3Hit(const b3_coord x, const b3_coord y) const
{
	b3_coord xDiff = m_x - x;
	b3_coord yDiff = m_y - y;

	return
		(B3_ABS(xDiff) <= b3PickBase::m_PickSize) &&
		(B3_ABS(yDiff) <= b3PickBase::m_PickSize);
}

const b3_bool b3Pick::b3Moved(const b3_coord x, const b3_coord y)
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
