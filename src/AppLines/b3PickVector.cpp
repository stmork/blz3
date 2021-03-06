/*
**
**	$Filename:	b3PickVector.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Implementation of pick points and pick direction vectors
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "b3PickVector.h"

/*************************************************************************
**                                                                      **
**                        b3PickPoint implementation                    **
**                                                                      **
*************************************************************************/

COLORREF b3PickPoint::m_PickColor = RGB(0xff,0x11,0x44);

b3PickPoint::b3PickPoint(
	b3RenderView   *renderview,
	b3_vector      *pos,
	const char     *text,
	b3ModellerInfo *info)
{
	m_RenderView = renderview;
	m_StartVector = *pos;
	m_Pos         =  pos;
	m_Text        =  text != null ? text : "";
	m_Info        =  info;
}

void b3PickPoint::b3Draw(b3DrawContext *dc)
{
	// Compute window coords
	m_RenderView->b3Project(m_Pos,m_x,m_y,m_z);

	dc->FillSolidRect(
		m_x - b3PickBase::m_PickSize,
		m_y - b3PickBase::m_PickSize,
		b3PickBase::m_PickSize * 2 + 1,
		b3PickBase::m_PickSize * 2 + 1,
		m_PickColor);
	dc->TextOut(m_x + 3,m_y - 4,m_Text);
}

b3_bool b3PickPoint::b3Moved(b3_coord x,b3_coord y)
{
	b3_bool   changed = false;
	
	if ((m_x != x) || (m_y != y))
	{
		b3_vector point = *m_Pos;

		// Unproject and snap to grid if possible
		m_RenderView->b3Unproject(x,y,m_z,&point);
		if (m_Info != null)
		{
			m_Info->b3SnapToGrid(&point);
		}

		// Modify when really moved
		if (!b3Vector::b3IsEqual(m_Pos,&point))
		{
			m_x = x;
			m_y = y;
			*m_Pos  = point;
			changed = true;
		}
	}
	return changed;
}

b3UndoOperation *b3PickPoint::b3GetOperation(b3RenderObject *object)
{
	return new b3OpPickPoint(&m_StartVector, m_Pos, object);
}

/*************************************************************************
**                                                                      **
**                        b3PickDir implementation                      **
**                                                                      **
*************************************************************************/

b3PickDir::b3PickDir(
	b3RenderView   *renderview,
	b3_vector      *pos,
	b3_vector      *dir,
	const char     *text,
	b3ModellerInfo *info) : b3PickPoint(renderview,&m_AuxPos,text,info)
{
	m_OrigPos     =  pos;
	m_OrigDir     =  dir;
	m_StartVector = *dir;
}

void b3PickDir::b3Draw(b3DrawContext *dc)
{
	b3_coord x,y;

	// May be the m_OrigPos has changed: Correct before drawing
	b3Vector::b3Add(m_OrigPos,m_OrigDir,&m_AuxPos);
	b3PickPoint::b3Draw(dc);
	
	m_RenderView->b3Project(m_OrigPos,x,y,m_z);
	dc->MoveTo(m_x,m_y);
	dc->LineTo(x,y);
}

b3_bool b3PickDir::b3Moved(b3_coord x,b3_coord y)
{
	b3_bool changed = b3PickPoint::b3Moved(x,y);

	if (changed)
	{
		b3Vector::b3Sub(&m_AuxPos,m_OrigPos,m_OrigDir);
	}
	return changed;
}

b3UndoOperation *b3PickDir::b3GetOperation(b3RenderObject *object)
{
	return new b3OpPickDir(&m_StartVector, m_OrigDir, object);
}
