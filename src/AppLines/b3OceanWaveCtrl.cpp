/*
**
**	$Filename:	b3OceanWaveCtrl.cpp $
**	$Release:	Dortmund 2006 $
**	$Revision: 1467 $
**	$Date: 2006-10-22 11:26:47 +0200 (So, 22 Okt 2006) $
**	$Author: sm $
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
#include "blz3/system/b3PaintDC.h"

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
	m_MouseCapture = true;
	SetCapture();
	b3UpdateUI(&point);
	CStatic::OnLButtonDown(nFlags, point);
}

void CB3OceanWaveCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((m_MouseCapture) && (m_LastPoint != point))
	{
		b3UpdateUI(&point);
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CB3OceanWaveCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
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

	// Send refresh hint to dialog
	CWnd *parent = GetParentOwner();
	if (parent != null)
	{
		parent->SendMessage(WM_USER);
	}
}

void CB3OceanWaveCtrl::OnPaint()
{
	CB3PaintDC dc(this); // device context for painting

	if (m_Ocean != null)
	{
		CRgn region;
		CPen pen(PS_SOLID, 2, RGB(0xff, 0x11, 0x44)), *oldPen;

		// Define clipping at window borders.
		region.CreateRectRgn(m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
		dc.SelectClipRgn(&region, RGN_AND);

		// Blit the height map
		dc.BitBlt(0, 0, m_xSize, m_ySize,m_DDB.b3GetDC(),0,0,SRCCOPY);

		// Draw the wind direction
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

void CB3OceanWaveCtrl::b3Refresh()
{
	m_Ocean->b3CopyHeightField(&m_Tx);
	m_DDB.b3SetImage(&m_Tx, 0, m_ySize);
	Invalidate();
}

void CB3OceanWaveCtrl::b3Update()
{
	m_LastPoint = m_MidPoint + CPoint(
		 m_Ocean->m_Wx * m_Factor,
		-m_Ocean->m_Wy * m_Factor);
	m_Ocean->b3Modified();
	m_Ocean->b3PrepareOceanWave(0.0); // Animate later

	b3Refresh();
}

void CB3OceanWaveCtrl::b3UpdateUI(CPoint *point)
{
	if (point != null)
	{
		CPoint        diff;

		diff          = *point - m_MidPoint;
		m_LastPoint   = *point;

		m_Ocean->m_Wx =  b3_f64(diff.x) / m_Factor;
		m_Ocean->m_Wy = -b3_f64(diff.y) / m_Factor;
		m_Ocean->b3Modified();
		m_Ocean->b3PrepareOceanWave(0.0); // Animate later

		GetParent()->UpdateData(FALSE);
		b3Refresh();
	}
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
