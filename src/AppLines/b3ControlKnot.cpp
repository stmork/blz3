/*
**
**	$Filename:	b3ControlKnot.cpp $  
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Displaying the knot vector of a BSpline
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"
#include "b3ControlKnot.h"
#include "blz3/system/b3PaintDC.h"

/*************************************************************************
**                                                                      **
**                        CB3ControlKnot implementation                 **
**                                                                      **
*************************************************************************/

int CB3ControlKnot::m_LeftOffset  = 12;
int CB3ControlKnot::m_RightOffset = 13;

CB3ControlKnot::CB3ControlKnot()
{
}

CB3ControlKnot::~CB3ControlKnot()
{
}


BEGIN_MESSAGE_MAP(CB3ControlKnot, CStatic)
	//{{AFX_MSG_MAP(CB3ControlKnot)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ControlKnot message handlers

void CB3ControlKnot::b3Init(b3Spline *spline)
{
	m_Spline = spline;
	b3Update();
}

void CB3ControlKnot::b3Update(b3_bool refresh)
{
	if (refresh && ::IsWindow(m_hWnd))
	{
		Invalidate();
	}
}

void CB3ControlKnot::OnPaint() 
{
	CB3PaintDC   dc(this); // device context for painting
	b3_index     i;
	b3_f64       iMin,iMax;
	b3_knots     knots = m_Spline->m_Knots;
	CRect        rect;
	b3_count     repeat;
	CPen         pen(PS_SOLID,1,RGB(0,0,0));
	b3_knot      last;
	b3_res       xMax,yMax;
	b3_coord     x;
	b3_f64       quotient;

	// TODO: Add your message handler code here
	iMin = knots[0];
	iMax = knots[m_Spline->m_KnotNum - 1];
	last = iMin - 1; // force different knot

	GetClientRect(&rect);
	xMax     = rect.Width();
	yMax     = rect.Height();
	quotient = (xMax - m_LeftOffset - m_RightOffset) / iMax;
	dc.FillSolidRect(0,0,xMax,yMax,RGB(255,255,255));
	
	for (i = 0;i < m_Spline->m_KnotNum;i++)
	{
		if (last != knots[i])
		{
			repeat = 0;
		}
		else
		{
			repeat++;
		}

		x = m_LeftOffset + knots[i] * quotient;
		if (repeat == 0)
		{
			dc.MoveTo(x,yMax);
			dc.LineTo(x,yMax - 10);
		}

		// Draw caret
		dc.MoveTo(x,yMax - 10 - repeat * 10);
		dc.LineTo(x + 5,yMax - 10 - repeat * 10 -  5);
		dc.LineTo(x,    yMax - 10 - repeat * 10 - 10);
		dc.LineTo(x - 5,yMax - 10 - repeat * 10 -  5);
		dc.LineTo(x,    yMax - 10 - repeat * 10);
		last = knots[i];
	}
}
