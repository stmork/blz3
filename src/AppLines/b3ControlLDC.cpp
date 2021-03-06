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

#include "AppLinesInclude.h"
#include "b3ControlLDC.h"

#define SEGMENTS 6
#define LINES_PER_SEGMENT 5

/*************************************************************************
**                                                                      **
**                        b3PickLDC implementation                      **
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
			m_x - b3PickBase::m_PickSize,
			m_y - b3PickBase::m_PickSize,
			b3PickBase::m_PickSize * 2 + 1,
			b3PickBase::m_PickSize * 2 + 1,
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

	inline b3UndoOperation *b3GetOperation(b3RenderObject *object)
	{
		return null;
	}
};

/*************************************************************************
**                                                                      **
**                        CB3ControlLDC implementation                  **
**                                                                      **
*************************************************************************/

CB3ControlLDC::CB3ControlLDC()
{
	m_Mode  = LDC_EDIT;
	m_LDC   = null;
	m_Curve = null;
	m_xMax  = 0;
	m_yMax  = 0;
	m_Init  = false;
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
	CDC       *dc;

	m_LDC = &light->m_Spline;
	m_LDC->m_SubDiv = SEGMENTS * LINES_PER_SEGMENT;
	if (m_Curve == null)
	{
		m_Curve = (b3_vector *)b3Alloc(sizeof(b3_vector) * (m_LDC->m_SubDiv + 1));
	}

	if ((!m_Init) && ::IsWindow(m_hWnd))
	{
		GetClientRect(&rect);
		m_xMax = rect.Width();
		m_yMax = rect.Height();
		dc = GetDC();
		m_DC.CreateCompatibleDC(dc);
		m_Bitmap.CreateCompatibleBitmap(dc,m_xMax,m_yMax);
		m_DC.SelectObject(&m_Bitmap);
		ReleaseDC(dc);
		b3Update();
		m_Init = true;
	}

	if (m_Mode == LDC_EDIT)
	{
		m_PickBase.b3Free();
		for (i = 0;i < m_LDC->m_ControlNum;i++)
		{
			pick = new b3PickLDC(&m_LDC->m_Controls[i], m_xMax, m_yMax);
			m_PickBase.b3Append(pick);
		}
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
	B3_ASSERT((m_xMax > 0) && (m_yMax > 0) && (m_LDC != null));
	m_DC.FillSolidRect(0,0,m_xMax,m_yMax,RGB(255,255,255));

	switch (m_Mode)
	{
	case LDC_EDIT:
		xHalf = m_xMax >> 1;

		// Draw horizontal legend
		oldPen = m_DC.SelectObject(&legend);
		for (i = 0;i <= SEGMENTS;i++)
		{
			x = i * xHalf / SEGMENTS;
			m_DC.MoveTo(xHalf - x,0);
			m_DC.LineTo(xHalf - x,m_yMax);
			m_DC.MoveTo(xHalf + x,0);
			m_DC.LineTo(xHalf + x,m_yMax);
		}
		for (i = 0;i <= 4;i++)
		{
			y = i * m_yMax / 4;
			m_DC.MoveTo(0,y);
			m_DC.LineTo(m_xMax,y);
		}
		m_DC.SelectObject(oldPen);

		// Init curve
		x = m_xMax * (0.5 - m_Curve[m_LDC->m_SubDiv].x * 0.5);
		y = m_yMax * (1.0 - m_Curve[m_LDC->m_SubDiv].y);
		oldPen = m_DC.SelectObject(&pen);
		m_DC.MoveTo(x,y);

		// Draw left side
		for (i = 0;i < m_LDC->m_SubDiv;i++)
		{
			x = m_xMax * (0.5 - m_Curve[m_LDC->m_SubDiv - i].x * 0.5);
			y = m_yMax * (1.0 - m_Curve[m_LDC->m_SubDiv - i].y);
			m_DC.LineTo(x,y);
		}

		// Draw right side
		while(i >= 0)
		{
			x = m_xMax * (0.5 + m_Curve[m_LDC->m_SubDiv - i].x * 0.5);
			y = m_yMax * (1.0 - m_Curve[m_LDC->m_SubDiv - i].y);
			m_DC.LineTo(x,y);
			i--;
		}
		m_DC.SelectObject(oldPen);

		// Draw handles
		m_PickBase.b3Draw(&m_DC);
		break;

	case LDC_DIAGRAM:
		xHalf = m_xMax >> 1;

		// Draw horizontal legend
		oldPen = m_DC.SelectObject(&legend);
		for (i = 0;i <= SEGMENTS;i++)
		{
			x = xHalf  * sin((double)i * M_PI * 0.5 / SEGMENTS);
			y = m_yMax * cos((double)i * M_PI * 0.5 / SEGMENTS);
			m_DC.MoveTo(xHalf,0);
			m_DC.LineTo(xHalf - x,y);
			m_DC.MoveTo(xHalf,0);
			m_DC.LineTo(xHalf + x,y);
		}
		for (i = 0;i <= 4;i++)
		{
			x = i * xHalf / 4;
			y = i * m_yMax / 4;
			m_DC.Arc(xHalf - x,-y,xHalf + x,y,xHalf - x,0,xHalf + x,0);
		}
		m_DC.SelectObject(oldPen);

		// Init curve
		x = m_xMax * (0.5 - sin(m_Curve[m_LDC->m_SubDiv].x * M_PI * 0.5) * m_Curve[m_LDC->m_SubDiv].y * 0.5);
		y = m_yMax *       (cos(m_Curve[m_LDC->m_SubDiv].x * M_PI * 0.5) * m_Curve[m_LDC->m_SubDiv].y);
		oldPen = m_DC.SelectObject(&pen);
		m_DC.MoveTo(x,y);

		// Draw left side
		for (i = 0;i < m_LDC->m_SubDiv;i++)
		{
			x = m_xMax * (0.5 - sin(m_Curve[m_LDC->m_SubDiv - i].x * M_PI * 0.5) * m_Curve[m_LDC->m_SubDiv - i].y * 0.5);
			y = m_yMax *       (cos(m_Curve[m_LDC->m_SubDiv - i].x * M_PI * 0.5) * m_Curve[m_LDC->m_SubDiv - i].y);
			m_DC.LineTo(x,y);
		}

		// Draw right side
		while(i >= 0)
		{
			x = m_xMax * (0.5 + sin(m_Curve[m_LDC->m_SubDiv - i].x * M_PI * 0.5) * m_Curve[m_LDC->m_SubDiv - i].y * 0.5);
			y = m_yMax *       (cos(m_Curve[m_LDC->m_SubDiv - i].x * M_PI * 0.5) * m_Curve[m_LDC->m_SubDiv - i].y);
			m_DC.LineTo(x,y);
			i--;
		}
		m_DC.SelectObject(oldPen);
		break;
	}
	dc.BitBlt(0,0,m_xMax,m_yMax,&m_DC,0,0,SRCCOPY);
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

void CB3ControlLDC::b3SetMode(CB3LDCMode mode)
{
	m_Mode = mode;
}

void CB3ControlLDC::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ((m_Mode == LDC_EDIT) && m_PickBase.b3Down(point.x,point.y))
	{
		SetCapture();
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CB3ControlLDC::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_PickBase.b3Move(point.x,point.y))
	{
		CWnd *parent = GetParent();

		b3Update();
		if (parent != null)
		{
			NMHDR nmhdr;

			nmhdr.hwndFrom = m_hWnd;
			nmhdr.idFrom   = GetDlgCtrlID();
			nmhdr.code     = WM_MOUSEMOVE;
			parent->SendMessage(WM_NOTIFY,nmhdr.idFrom,(LPARAM)&nmhdr);

//			parent->PostMessage(WM_B3_LDC_MOVED);
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CB3ControlLDC::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_PickBase.b3Up(point.x,point.y))
	{
		CWnd *parent = GetParent();

		::ReleaseCapture();
		if (parent != null)
		{
			NMHDR nmhdr;

			nmhdr.hwndFrom = m_hWnd;
			nmhdr.idFrom   = GetDlgCtrlID();
			nmhdr.code     = WM_LBUTTONUP;
			parent->SendMessage(WM_NOTIFY,nmhdr.idFrom,(LPARAM)&nmhdr);

//			parent->PostMessage(WM_B3_LDC_CHANGED);
		}
	}
	CStatic::OnLButtonUp(nFlags, point);
}
