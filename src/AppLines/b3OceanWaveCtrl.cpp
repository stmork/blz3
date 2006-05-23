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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/05/23 20:23:41  sm
**	- Some view/bitmap cleanups.
**	- Some more ocean wave ctrl development.
**	- Some preview property page cleanups.
**	- Changed data access methods of b3Tx.
**
**	Revision 1.2  2006/05/22 20:42:37  sm
**	- Added ocean wave control.
**	
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
	m_MouseCapture = true;
	SetCapture();
	b3UpdateUI(&point);
	CStatic::OnLButtonDown(nFlags, point);
}

void CB3OceanWaveCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	if ((m_MouseCapture) && (m_LastPoint != point))
	{
		b3UpdateUI(&point);
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CB3OceanWaveCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	if (m_MouseCapture)
	{
		if (m_LastPoint != point)
		{
			b3UpdateUI(&point);
		}
		::ReleaseCapture();
		m_MouseCapture = false;
	}
	CStatic::OnLButtonUp(nFlags, point);
}


void CB3OceanWaveCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CStatic::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	if (m_Ocean != null)
	{
		CPen pen(PS_SOLID, 2, RGB(0xff, 0x11, 0x44)), *oldPen;

		dc.BitBlt(0, 0, m_xSize, m_ySize,m_DDB.b3GetDC(),0,0,SRCCOPY);

		oldPen = dc.SelectObject(&pen);
		dc.MoveTo(m_MidPoint);
		dc.LineTo(m_LastPoint);
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
		CPoint        diff;
		b3Complex64  *buffer = m_Ocean->b3GetBuffer();
		b3_pkd_color *ptr    = m_Tx.b3GetTrueColorData();
		b3_loop       x,y;

		diff          = *point - m_MidPoint;
		m_LastPoint   = *point;

		m_Ocean->m_Wx =  b3_f64(diff.x) / m_Factor;
		m_Ocean->m_Wy = -b3_f64(diff.y) / m_Factor;
//		m_Ocean->b3PrepareOceanWave(0.0); // Animate later

		GetParent()->UpdateData(FALSE);

		for (y = 0;y < m_ySize;y++)
		{
			for(x = 0;x < m_xSize;x++)
			{
				b3_f32 c = buffer[x].b3Real() * 0.0005 + 0.5;

				ptr[x] = b3Color(c, c, c);
			}
			buffer += m_xSize;
			ptr    += m_xSize;
		}
		m_DDB.b3SetImage(&m_Tx, 0, m_ySize);
	}
	InvalidateRect(NULL);
}

void CB3OceanWaveCtrl::b3SetOcean(b3OceanWave *ocean)
{
	CRect  rect;
	CDC   *dc;

	GetClientRect(&m_Rect);
	m_xSize     = m_Rect.Width();
	m_ySize     = m_Rect.Height();
	m_MidPoint  = CPoint(m_xSize >> 1, m_ySize >> 1);
	m_Ocean     = ocean;
	m_LastPoint = m_MidPoint + CPoint(
		 m_Ocean->m_Wx * m_Factor,
		-m_Ocean->m_Wy * m_Factor);
	if (!m_Tx.b3AllocTx(m_xSize, m_ySize, 24))
	{
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	dc = GetDC();
	m_DDB.b3InitImage(&m_Tx, dc);
	ReleaseDC(dc);

	b3UpdateUI(&m_LastPoint);
}
