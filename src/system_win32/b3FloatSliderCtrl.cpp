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
#define CB3_FSC_MAX (1 << 12)

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2004/04/10 14:33:25  sm
**	- Added oak plank support.
**
**	Revision 1.3  2004/04/09 17:30:31  sm
**	- Wood dialog fine tuning.
**	
**	Revision 1.2  2004/04/09 14:09:36  sm
**	- Wood sampling corrected.
**	- b3FloatSliderCtrls range computation corrected.
**	
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
	m_Max = 1;
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
	b3SetTicks((m_Max - m_Min) / 10.0,(m_Max - m_Min) / 10.0);
	return b3GetPos();
}

void CB3FloatSliderCtrl::b3SetTicks(b3_f64 page,b3_f64 ticks)
{
	m_PageSize     = b3ConvertRel(page);
	m_TicFrequence = b3ConvertRel(ticks);
}

b3_f64 CB3FloatSliderCtrl::b3GetPos()
{
	if (::IsWindow(*this))
	{
		// Get value
		int pos = GetPos();
		
		m_Pos = b3Math::b3Limit(b3ConvertPos(pos),m_Min,m_Max);
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

	if (::IsWindow(*this))
	{
		int index = b3ConvertPos(m_Pos);
	
		SetRange(CB3_FSC_MIN,CB3_FSC_MAX);
		SetPageSize(m_PageSize);
		SetTicFreq(m_TicFrequence);
		SetPos(index);
	}
	return m_Pos;
}

b3_f64 CB3FloatSliderCtrl::b3ConvertPos(int pos)
{
	return (b3_f64)(pos - CB3_FSC_MIN) * (m_Max - m_Min) /
			(CB3_FSC_MAX - CB3_FSC_MIN) + m_Min;
}

int CB3FloatSliderCtrl::b3ConvertPos(b3_f64 pos)
{
	return
		(pos   - m_Min) * (CB3_FSC_MAX - CB3_FSC_MIN) /
		(m_Max - m_Min) + CB3_FSC_MIN;
}

b3_f64 CB3FloatSliderCtrl::b3ConvertRel(int rel)
{
	return rel * (m_Max - m_Min) /
			(CB3_FSC_MAX - CB3_FSC_MIN);
}

int CB3FloatSliderCtrl::b3ConvertRel(b3_f64 rel)
{
	return
		rel * (CB3_FSC_MAX - CB3_FSC_MIN) /
		(m_Max - m_Min);
}
