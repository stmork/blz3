/*
**
**	$Filename:	b3OceanWaveCtrl.cpp $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Control for wind speed
**
**	(C) Copyright 2006  Steffen A. Mork
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
#include "b3OceanWaveCtrl.h"
#include "PageOcean.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2006/05/22 20:15:24  sm
**	- Added ocean wave control.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3OceanWaveCtrl implementation               **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3OceanWaveCtrl, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CB3OceanWaveCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.

	CStatic::OnLButtonDown(nFlags, point);
	m_MouseCapture = true;
	m_LastPoint = point;
	GetCapture();
}

void CB3OceanWaveCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.

	CStatic::OnMouseMove(nFlags, point);
	if ((m_MouseCapture) && (m_LastPoint != point))
	{
		GetParent()->UpdateData(FALSE);
		b3UpdateUI(&point);
	}
}

void CB3OceanWaveCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.

	CStatic::OnLButtonUp(nFlags, point);
	if (m_MouseCapture)
	{
		if (m_LastPoint != point)
		{
			GetParent()->UpdateData(FALSE);
			b3UpdateUI(&point);
		}
		::ReleaseCapture();
		m_MouseCapture = false;
	}
}


void CB3OceanWaveCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CStatic::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.

	if (m_Ocean != null)
	{
		b3Complex64 *buffer = m_Ocean->b3GetBuffer();
		b3_res       xSize, ySize;
		b3_loop      x,y;

		dc.FillSolidRect(&m_Rect,GetSysColor(COLOR_BTNFACE));
		xSize = m_Rect.Width();
		ySize = m_Rect.Height();
		for (y = 0;y < ySize;y++)
		{
			for(x = 0;x < xSize;x++)
			{
				BYTE g = buffer[x].b3Real() * 255;
				RGB(g,g,g);
			}
			buffer += ySize;
		}
		CPen pen(PS_SOLID, 2, RGB(0xff, 0x11, 0x44)), *oldPen;

		oldPen = dc.SelectObject(&pen);
		dc.MoveTo(m_MidPoint);
		dc.LineTo(m_EndPoint);
		dc.SelectObject(oldPen);
	}
	else
	{
		dc.FillSolidRect(&m_Rect,GetSysColor(COLOR_BTNFACE));
	}
}

void CB3OceanWaveCtrl::b3UpdateUI(CPoint *point)
{
	if (point != null)
	{
		CPoint diff;

		m_LastPoint = *point;
		diff        = *point - m_MidPoint;
		m_Ocean->m_Wx =  b3_f64(diff.x) / m_Factor;
		m_Ocean->m_Wy = -b3_f64(diff.y) / m_Factor;
	}
	m_EndPoint = m_MidPoint + CPoint(
		 m_Ocean->m_Wx * m_Factor,
		-m_Ocean->m_Wy * m_Factor);
//	m_Ocean->b3PrepareOceanWave(0.0); // Animate later
	InvalidateRect(NULL);
}

void CB3OceanWaveCtrl::b3SetOcean(b3OceanWave *ocean)
{
	CRect rect;

	GetClientRect(&m_Rect);
	m_MidPoint = CPoint(m_Rect.Width() >> 1, m_Rect.Height() >> 1);
	m_Ocean    = ocean;
	b3UpdateUI();
}
