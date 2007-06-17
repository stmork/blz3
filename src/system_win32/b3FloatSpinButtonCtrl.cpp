/*
**
**	$Filename:	b3FloatSpinButtonCtrl.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Spin button control for float values
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"

#define B3_VAL_TO_RANGE(x) ((int)floor(((x) - m_Min) / m_Increment + 0.5))

/*************************************************************************
**                                                                      **
**                        CB3FloatSpinButtonCtrl implementation         **
**                                                                      **
*************************************************************************/

CB3FloatSpinButtonCtrl::CB3FloatSpinButtonCtrl()
{
	m_Min   = 0;
	m_Max   = 1;
	m_Pos   = 0;
	m_Accel = B3_FSBC_DEFAULT_ACCEL;
	b3SetIncrement();
	b3SetDigits();
}

CB3FloatSpinButtonCtrl::~CB3FloatSpinButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CB3FloatSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CB3FloatSpinButtonCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3FloatSpinButtonCtrl message handlers

void CB3FloatSpinButtonCtrl::b3DDX(CDataExchange *pDX,b3_f32 &pos)
{
	b3_f64 value = pos;

	b3DDX(pDX,value);
	pos = (b3_f32)value;
}

void CB3FloatSpinButtonCtrl::b3DDX(CDataExchange *pDX,b3_f64 &pos)
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

void CB3FloatSpinButtonCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN *pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CWnd      *edit = GetBuddy();
	CString    value;
	
	B3_ASSERT(edit != null);
	edit->GetWindowText(value);
	b3SetPos(atof(value) / b3GetUnitScale() + (b3_f64)pNMUpDown->iDelta * m_Increment);
	*pResult = 1;
}

void CB3FloatSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd *parent = GetParent();

	CSpinButtonCtrl::OnLButtonUp(nFlags, point);

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

b3_f64 CB3FloatSpinButtonCtrl::b3SetRange(b3_f64 min, b3_f64 max)
{
	// Ensure something...
	B3_ASSERT(min < max);

	m_Min = min;
	m_Max = max;
	
	return b3GetPos();
}

void CB3FloatSpinButtonCtrl::b3SetIncrement(b3_f64 increment)
{
	B3_ASSERT(increment > 0);
	m_Increment = increment;
}

void CB3FloatSpinButtonCtrl::b3SetDigits(int pre,int post)
{
	char pre_digit[16];
	char post_digit[16];
	
	B3_ASSERT((post >= 0) && (post < 6));

	pre_digit[0]  =
	post_digit[0] = 0;

	if (pre >= 0)
	{
		snprintf(pre_digit,sizeof(pre_digit),"%d",pre);
	}
	if (post >= 0)
	{
		snprintf(post_digit,sizeof(post_digit),".%d",post);
	}
	snprintf(m_Format,sizeof(m_Format),"%%%s%slf",pre_digit,post_digit);
}

void CB3FloatSpinButtonCtrl::b3SetUnit(b3_unit unit)
{
	b3ControlUnits::b3SetUnit(unit);
	switch(unit)
	{
	case B3_UNIT_PERCENT:
		b3SetRange(0.0,1.0);
		b3SetIncrement(0.01);
		b3SetDigits(3,1);
		break;

	case B3_UNIT_PERMILLE:
		b3SetRange(0.0,1.0);
		b3SetIncrement(0.001);
		b3SetDigits(4,1);
		break;

	case B3_UNIT_IOR:
		b3SetRange(-5.0,5.0);
		b3SetIncrement(0.01);
		b3SetDigits(0,2);
		break;
	
	case B3_UNIT_SPECULAR_EXP:
		b3SetRange(1.0,100000);
		b3SetIncrement(10.0);
		b3SetDigits(0,1);
		break;

	case B3_UNIT_BUMP:
		b3SetRange(0.0,10000.0);
		b3SetIncrement(0.001);
		b3SetDigits(4,1);
		break;
	}
}

b3_f64 CB3FloatSpinButtonCtrl::b3GetPos()
{
	CWnd    *edit;
	CString  value;
	b3_f64   pos;

	if (::IsWindow(*this))
	{
		// Get value
		edit = GetBuddy();
		B3_ASSERT(edit != null);
		edit->GetWindowText(value);
		m_Pos = pos = atof(value) / b3GetUnitScale();
		B3_LIMIT(m_Pos,m_Min,m_Max);
		if (m_Pos != pos)
		{
			b3SetPos(m_Pos);
		}
	}
	else
	{
		B3_LIMIT(m_Pos,m_Min,m_Max);
	}
	return m_Pos;
}

b3_f64 CB3FloatSpinButtonCtrl::b3SetPos(b3_f64 pos)
{
	CWnd    *edit = GetBuddy();
	CString  value;
	b3_f64   diff;
	int      maxint;

	B3_ASSERT(edit != null);

	// Set range
	diff   = (m_Max - m_Min) / m_Increment;
	maxint = (int)(diff > INT_MAX ? INT_MAX : diff);
	SetRange32(0,maxint);

	// Set position
	m_Pos = pos;
	B3_LIMIT(m_Pos,m_Min,m_Max);
	value.Format(m_Format,m_Pos * b3GetUnitScale());
	edit->SetWindowText(value);
	SetPos(B3_VAL_TO_RANGE(m_Pos));
	return m_Pos;
}

void CB3FloatSpinButtonCtrl::b3SetAccel(b3_f64 increment,int secs)
{
	UDACCEL  accel[10];
	b3_count count,i;

	count = GetAccel(10,accel);
	accel[1].nInc = (int)floor(increment / m_Increment + 0.5);
	accel[1].nSec = secs;
	for (i = 2;i < count;i++)
	{
		accel[i].nInc = accel[i - 1].nInc * 4;
		accel[i].nSec = accel[i - 1].nSec * 5 / 2;
	}
	SetAccel(count,accel);
}

b3_f64 CB3FloatSpinButtonCtrl::b3GetAccel()
{
	UDACCEL accel;

	GetAccel(1,&accel);
	return (b3_f64)accel.nInc * m_Increment;
}
