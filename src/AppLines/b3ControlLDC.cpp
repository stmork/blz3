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
#define LINES_PER_SEGMENT 5

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/12/06 07:08:55  sm
**	- Further control programming
**
**	Revision 1.2  2001/12/04 18:23:25  sm
**	- Drawing LDC correctly
**	- Added pick point support.
**	
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

class b3PickLDC : public b3Pick
{
	b3_vector *m_Control;
	b3_res     m_xMax;
	b3_res     m_yMax;
public:
	b3PickLDC(b3_vector *control,b3_res xMax,b3_res yMax) : b3Pick(0,0)
	{
		m_Control = control;
		m_xMax = xMax;
		m_yMax = yMax;
		m_x    = m_xMax * (0.5 + m_Control->x * 0.5);
		m_y    = m_yMax * (1.0 - m_Control->y);
	}

	void b3Draw(b3DrawContext *dc)
	{
		dc->FillSolidRect(
			m_x - B3_PICK_SIZE,m_y - B3_PICK_SIZE,
			B3_PICK_SIZE * 2 + 1,B3_PICK_SIZE * 2 + 1,
			RGB(255,16,64));
	}

	b3_bool b3Moved(b3_coord x,b3_coord y)
	{
		b3_bool result;

		if (y < 0)
		{
			y = 0;
		}
		if (y > m_yMax)
		{
			y = m_yMax;
		}
		result = (y != m_y);
		if (result)
		{
			m_y = y;
			m_Control->y = 1.0 - (b3_f64)y / (b3_f64)m_yMax;
		}
		return result;
	}
};

CB3ControlLDC::CB3ControlLDC()
{
	m_LDC   = null;
	m_Curve = null;
	m_xMax  = 0;
	m_yMax  = 0;
}

CB3ControlLDC::~CB3ControlLDC()
{
	m_PickBase.b3Free();
}


BEGIN_MESSAGE_MAP(CB3ControlLDC, CStatic)
	//{{AFX_MSG_MAP(CB3ControlLDC)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ControlLDC message handlers

void CB3ControlLDC::b3Init(b3Light *light)
{
	b3PickLDC *pick;
	b3_index   i;
	CRect      rect;

	m_LDC = &light->m_Spline;
	m_LDC->subdiv = SEGMENTS * LINES_PER_SEGMENT;
	if (m_Curve == null)
	{
		m_Curve = (b3_vector *)b3Alloc(sizeof(b3_vector) * (m_LDC->subdiv + 1));
	}

	m_PickBase.b3Free();
	for (i = 0;i < m_LDC->control_num;i++)
	{
		pick = new b3PickLDC(&m_LDC->controls[i],m_xMax,m_yMax);
		m_PickBase.b3Append(pick);
	}

	if (::IsWindow(m_hWnd))
	{
		GetClientRect(&rect);
		m_xMax = rect.Width();
		m_yMax = rect.Height();
		b3Update();
	}
}

void CB3ControlLDC::OnPaint() 
{
	CPaintDC  dc(this); // device context for painting
	CPen      pen(PS_SOLID,1,RGB(0,0,0));
	CPen      legend(PS_SOLID,1,RGB(120,120,120));
	CPen     *oldPen;
	b3_index  i;
	b3_coord  x,y;
	b3_res    xHalf;
	
	// TODO: Add your message handler code here
	CStatic::OnPaint();
	B3_ASSERT((m_xMax > 0) && (m_yMax > 0) && (m_LDC != null));
	dc.FillSolidRect(0,0,m_xMax,m_yMax,RGB(255,255,255));

	xHalf = m_xMax >> 1;

	// Draw horizontal legend
	oldPen = dc.SelectObject(&legend);
	for (i = 0;i <= SEGMENTS;i++)
	{
		x = i * xHalf / SEGMENTS;
		dc.MoveTo(xHalf - x,0);
		dc.LineTo(xHalf - x,m_yMax);
		dc.MoveTo(xHalf + x,0);
		dc.LineTo(xHalf + x,m_yMax);
	}
	for (i = 0;i <= 4;i++)
	{
		y = i * m_yMax / 4;
		dc.MoveTo(0,y);
		dc.LineTo(m_xMax,y);
	}
	dc.SelectObject(oldPen);

	// Init curve
	x = m_xMax * (0.5 - m_Curve[m_LDC->subdiv].x * 0.5);
	y = m_yMax * (1.0 - m_Curve[m_LDC->subdiv].y);
	oldPen = dc.SelectObject(&pen);
	dc.MoveTo(x,y);

	// Draw left side
	for (i = 0;i < m_LDC->subdiv;i++)
	{
		x = m_xMax * (0.5 - m_Curve[m_LDC->subdiv - i].x * 0.5);
		y = m_yMax * (1.0 - m_Curve[m_LDC->subdiv - i].y);
		dc.LineTo(x,y);
	}

	// Draw right side
	while(i >= 0)
	{
		x = m_xMax * (0.5 + m_Curve[m_LDC->subdiv - i].x * 0.5);
		y = m_yMax * (1.0 - m_Curve[m_LDC->subdiv - i].y);
		dc.LineTo(x,y);
		i--;
	}
	dc.SelectObject(oldPen);

	// Draw handles
	m_PickBase.b3Draw(&dc);
}

void CB3ControlLDC::b3Update(b3_bool refresh)
{
	B3_ASSERT(m_LDC != null);
	m_LDC->b3DeBoor(m_Curve,0);

	if (refresh)
	{
		Invalidate();
	}
}

void CB3ControlLDC::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_PickBase.b3Down(point.x,point.y))
	{
		::SetCapture(m_hWnd);
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CB3ControlLDC::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_PickBase.b3Move(point.x,point.y))
	{
		b3Update();
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CB3ControlLDC::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_PickBase.b3Up(point.x,point.y))
	{
		::ReleaseCapture();
	}
	CStatic::OnLButtonUp(nFlags, point);
}
