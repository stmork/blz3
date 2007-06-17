/*
**
**	$Filename:	b3ImageButton.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Repeating image button
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

#include "b3SystemInclude.h"
#include "blz3/system/b3ImageButton.h"

/*************************************************************************
**                                                                      **
**                        CB3ImageButton implementation                 **
**                                                                      **
*************************************************************************/

CB3ImageButton::CB3ImageButton()
{
	m_FirstRepeatMillies   = 400;
	m_FurtherRepeatMillies =  50;
	m_RepeatCount          =   0;
	m_hIcon                =   0;
	m_MainFrame            = AfxGetApp()->m_pMainWnd;
}

CB3ImageButton::~CB3ImageButton()
{
}


BEGIN_MESSAGE_MAP(CB3ImageButton, CButton)
	//{{AFX_MSG_MAP(CB3ImageButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ImageButton message handlers

void CB3ImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	UINT      flags       = DST_ICON;
	CDC      *dc          = CDC::FromHandle(lpDrawItemStruct->hDC);
	COLORREF  topleft     = GetSysColor(m_RepeatCount > 0 ? COLOR_3DSHADOW  : COLOR_3DHILIGHT);
	COLORREF  bottomright = GetSysColor(m_RepeatCount > 0 ? COLOR_3DHILIGHT : COLOR_3DSHADOW);
	CSize     size(32,32);
	CPoint    point(
		(lpDrawItemStruct->rcItem.right  - lpDrawItemStruct->rcItem.left - size.cx) >> 1,
		(lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top  - size.cy) >> 1);

	dc->Draw3dRect(&lpDrawItemStruct->rcItem,topleft,bottomright);

	// Compute draw mode depending on activation of this button
	if (GetStyle() & WS_DISABLED)
	{
		flags |= DSS_DISABLED;
	}
	dc->DrawState(point,size,m_hIcon,flags,(HBRUSH)NULL);
}

void CB3ImageButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonDown(nFlags, point);

	// First delay
	if (m_MainFrame == NULL)
	{
		m_MainFrame = AfxGetApp()->m_pMainWnd;
	}
	if (m_MainFrame != NULL)
	{
		SetCapture();
		m_RepeatCount = 0;
		m_MainFrame->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),0),(LPARAM)m_hWnd);
		SetTimer(1,m_FirstRepeatMillies,NULL);
		m_RepeatCount++;
		Invalidate();
	}
}

void CB3ImageButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_RepeatCount > 0)
	{
		::ReleaseCapture();
	}
	CButton::OnLButtonUp(nFlags, point);
}

void CB3ImageButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if ((m_MainFrame != NULL) && (nIDEvent == 1))
	{
		m_MainFrame->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),0),(LPARAM)m_hWnd);
		m_RepeatCount++;
		SetTimer(1,m_FurtherRepeatMillies,NULL);
	}
	CButton::OnTimer(nIDEvent);
}

void CB3ImageButton::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	if ((pWnd != this) && (m_RepeatCount > 0))
	{
		KillTimer(1);
		Invalidate();
		m_RepeatCount = 0;
	}
	CButton::OnCaptureChanged(pWnd);
}
