/*
**
**	$Filename:	b3ControlLDC.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Controlling the LDC of a light source
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

#include "b3ControlLDC.h"

#define SEGMENTS 6
#define LINES_PER_SEGMENT 3

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/12/03 18:37:51  sm
**	- Added light distribution curve control.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3ControlLDC implementation                  **
**                                                                      **
*************************************************************************/

CB3ControlLDC::CB3ControlLDC()
{
	m_LDC   = null;
	m_Curve = null;
}

CB3ControlLDC::~CB3ControlLDC()
{
}


BEGIN_MESSAGE_MAP(CB3ControlLDC, CStatic)
	//{{AFX_MSG_MAP(CB3ControlLDC)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ControlLDC message handlers

void CB3ControlLDC::OnPaint() 
{
	CPaintDC  dc(this); // device context for painting
	CRect     rect;
	CPen      pen(PS_SOLID,1,RGB(0,0,0));
	CPen      legend(PS_SOLID,1,RGB(120,120,120));
	CPen     *oldPen;
	
	// TODO: Add your message handler code here
	b3_index i;
	b3_coord x,y;
	b3_res   xHalf,xMax,yMax;

	CStatic::OnPaint();
	GetClientRect(&rect);
	dc.FillSolidRect(&rect,RGB(255,255,255));
	if (m_LDC == null)
	{
		// Do nothing!
		return;
	}

	xHalf = rect.Width() >> 1;
	xMax  = rect.Width();
	yMax  = rect.Width();

	// Draw horizontal legend
	oldPen = dc.SelectObject(&legend);
	for (i = 0;i <= SEGMENTS;i++)
	{
		x = i * xHalf / SEGMENTS;
		dc.MoveTo(xHalf - x,0);
		dc.LineTo(xHalf - x,yMax - 1);
		dc.MoveTo(xHalf + x,0);
		dc.LineTo(xHalf + x,yMax - 1);
	}
	for (i = 0;i <= 4;i++)
	{
		y = i * (yMax - 1) / 4;
		dc.MoveTo(0,y);
		dc.LineTo(xMax-1,y);
	}
	dc.SelectObject(oldPen);

	// Init curve
	x = xHalf - m_Curve[m_LDC->subdiv].x * xHalf;
	y = yMax  - m_Curve[m_LDC->subdiv].y * yMax - 1;
	oldPen = dc.SelectObject(&pen);
	dc.MoveTo(x,y);

	// Draw left side
	for (i = 0;i < m_LDC->subdiv;i++)
	{
		x = xHalf - m_Curve[m_LDC->subdiv - i].x * xHalf;
		y = yMax  - m_Curve[m_LDC->subdiv - i].y * yMax - 1;
		dc.LineTo(x,y);
	}

	// Draw right side
	while(i >= 0)
	{
		x = xHalf + m_Curve[m_LDC->subdiv - i].x * xHalf;
		y = yMax -  m_Curve[m_LDC->subdiv - i].y * yMax - 1;
		dc.LineTo(x,y);
		i--;
	}
	dc.SelectObject(oldPen);

	// Do not call CStatic::OnPaint() for painting messages
}

void CB3ControlLDC::b3Update(b3_bool refresh)
{
	B3_ASSERT(m_LDC != null);
	if (m_Curve == null)
	{
		m_LDC->subdiv = SEGMENTS * LINES_PER_SEGMENT;
		m_Curve = (b3_vector *)b3Alloc(sizeof(b3_vector) * (m_LDC->subdiv + 1));
	}
	m_LDC->b3DeBoor(m_Curve,0);

	if (refresh)
	{
		Invalidate();
	}
}
