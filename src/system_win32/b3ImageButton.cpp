/*
**
**	$Filename:	b3ImageButton.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "blz3/system/b3ImageButton.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/01/09 17:55:20  sm
**	- New implementation of CB3ImageButton using timer. Correct implementation
**	  moved to correct position.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3ImageButton implementation                 **
**                                                                      **
*************************************************************************/

CB3ImageButton::CB3ImageButton()
{
	m_PressedCount         =   0;
	m_FirstRepeatMillies   = 400;
	m_FurtherRepeatMillies =  50;
	m_hIcon                =   0;
}

CB3ImageButton::~CB3ImageButton()
{
}


BEGIN_MESSAGE_MAP(CB3ImageButton, CButton)
	//{{AFX_MSG_MAP(CB3ImageButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ImageButton message handlers

void CB3ImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC      *dc          = CDC::FromHandle(lpDrawItemStruct->hDC);
	COLORREF  topleft     = GetSysColor(m_PressedCount > 0 ? COLOR_3DSHADOW  : COLOR_3DHILIGHT);
	COLORREF  bottomright = GetSysColor(m_PressedCount > 0 ? COLOR_3DHILIGHT : COLOR_3DSHADOW);
	CPoint    point(
		(lpDrawItemStruct->rcItem.right  - lpDrawItemStruct->rcItem.left - 32) >> 1,
		(lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top  - 32) >> 1);

	dc->Draw3dRect(&lpDrawItemStruct->rcItem,topleft,bottomright);
	dc->DrawIcon(point,m_hIcon);
}

void CB3ImageButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_PressedCount++;
	CButton::OnLButtonDown(nFlags, point);

	if (m_PressedCount == 1)
	{
		// First delay
		m_RepeatCount = 0;
		SendMessage(BM_CLICK);
	}
}

void CB3ImageButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonUp(nFlags, point);
	if (m_PressedCount > 1)
	{
		// Second and further delay
		SetTimer(1,m_RepeatCount == 0 ?
			m_FirstRepeatMillies :
			m_FurtherRepeatMillies,NULL);
		m_PressedCount--;
	}
	else
	{
		KillTimer(1);
		m_PressedCount = 0;
		Invalidate();
	}
}

void CB3ImageButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_PressedCount > 0)
	{
		m_RepeatCount++;
		SendMessage(BM_CLICK);
	}
	CButton::OnTimer(nIDEvent);
}
