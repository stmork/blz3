/*
**
**	$Filename:	b3FloatSliderCtrl.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Slider control for float values
**
**	(C) Copyright 2001, 2002, 2003, 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3FloatSliderCtrl.h"
#include "blz3/base/b3Math.h"

#define CB3_FSC_MIN  0
#define CB3_FSC_MAX (1 << 24)

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/04/04 19:28:25  sm
**	- New wood dialog
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3FloatSliderCtrl implementation             **
**                                                                      **
*************************************************************************/

CB3FloatSliderCtrl::CB3FloatSliderCtrl()
{
	m_Min = 0;
	m_Max = 0;
}

CB3FloatSliderCtrl::~CB3FloatSliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CB3FloatSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CB3FloatSliderCtrl)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CB3FloatSliderCtrl::b3DDX(CDataExchange *pDX,b3_f32 &pos)
{
	b3_f64 value = pos;

	b3DDX(pDX,value);
	pos = (b3_f32)value;
}

void CB3FloatSliderCtrl::b3DDX(CDataExchange *pDX,b3_f64 &pos)
{
	B3_ASSERT(m_Min < m_Max);
	if (pDX->m_bSaveAndValidate)
	{
		pos = b3GetPos();
	}
	else
	{
		pos = b3SetPos(pos);
	}
}

void CB3FloatSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd *parent = GetParent();

	CSliderCtrl::OnLButtonUp(nFlags, point);

	if (parent != null)
	{
		NMHDR nmhdr;

		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom   = GetDlgCtrlID();
		nmhdr.code     = WM_LBUTTONUP;
		parent->SendMessage(WM_NOTIFY,nmhdr.idFrom,(LPARAM)&nmhdr);
	}
}

/*************************************************************************
**                                                                      **
**                        service methods                               **
**                                                                      **
*************************************************************************/

b3_f64 CB3FloatSliderCtrl::b3SetRange(b3_f64 min, b3_f64 max)
{
	m_Min = min;
	m_Max = max;
	return b3GetPos();
}

b3_f64 CB3FloatSliderCtrl::b3GetPos()
{
	if (::IsWindow(*this))
	{
		// Get value
		int pos = GetPos();
		
		m_Pos = b3Math::b3Limit((b3_f64)(pos - CB3_FSC_MIN) / (CB3_FSC_MAX - CB3_FSC_MIN),m_Min,m_Max);
	}
	else
	{
		B3_LIMIT(m_Pos,m_Min,m_Max);
	}
	return m_Pos;
}

b3_f64 CB3FloatSliderCtrl::b3SetPos(b3_f64 pos)
{
	m_Pos = b3Math::b3Limit(pos,m_Min,m_Max);

	SetRange(CB3_FSC_MIN,CB3_FSC_MAX);
	SetPos(m_Pos * (CB3_FSC_MAX - CB3_FSC_MIN) + CB3_FSC_MIN);
	return m_Pos;
}
