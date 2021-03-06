/*
**
**	$Filename:	b3IntSpinButtonCtrl.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Spin button control for int values
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
#include "blz3/system/b3IntSpinButtonCtrl.h"

/*************************************************************************
**                                                                      **
**                        CB3FloatSpinButtonCtrl implementation         **
**                                                                      **
*************************************************************************/

CB3IntSpinButtonCtrl::CB3IntSpinButtonCtrl()
{
	m_Min   = 0;
	m_Max   = 0;
	m_Pos   = 0;
	m_Accel = B3_ISBC_DEFAULT_ACCEL;
	b3SetIncrement();
}

CB3IntSpinButtonCtrl::~CB3IntSpinButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CB3IntSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CB3IntSpinButtonCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3FloatSpinButtonCtrl message handlers

void CB3IntSpinButtonCtrl::b3DDX(CDataExchange *pDX,b3_s32 &pos)
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

void CB3IntSpinButtonCtrl::b3DDX(CDataExchange *pDX,b3_offset &pos)
{
	B3_ASSERT(m_Min < m_Max);
	if (pDX->m_bSaveAndValidate)
	{
		pos = b3GetPos();
	}
	else
	{
		pos = b3SetPos((b3_s32)pos);
	}
}

void CB3IntSpinButtonCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN *pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CWnd      *edit = GetBuddy();
	CString    value;
	
	B3_ASSERT(edit != null);
	edit->GetWindowText(value);
	b3SetPos(atof(value) + (b3_f64)pNMUpDown->iDelta * m_Increment);
	*pResult = 1;
}

void CB3IntSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
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

b3_s32 CB3IntSpinButtonCtrl::b3SetRange(b3_s32 min, b3_s32 max)
{
	// Ensure something...
	B3_ASSERT(min < max);

	m_Min       = min;
	m_Max       = max;
	
	return b3GetPos();
}

void CB3IntSpinButtonCtrl::b3SetIncrement(b3_s32 increment)
{
	B3_ASSERT(increment > 0);
	m_Increment = increment;
}

b3_s32 CB3IntSpinButtonCtrl::b3GetPos()
{
	CWnd    *edit;
	CString  value;
	b3_s32   pos;

	if (::IsWindow(*this))
	{
		// Get value
		edit = GetBuddy();
		B3_ASSERT(edit != null);
		edit->GetWindowText(value);
		m_Pos = pos = atoi(value);
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

b3_s32 CB3IntSpinButtonCtrl::b3SetPos(b3_s32 pos)
{
	CWnd    *edit = GetBuddy();
	CString  value;
	
	B3_ASSERT(edit != null);

	// Set range
	SetRange32(m_Min,m_Max);

	// Set position
	m_Pos = pos;
	B3_LIMIT(m_Pos,m_Min,m_Max);
	value.Format("%d",m_Pos);
	edit->SetWindowText(value);
	SetPos(m_Pos);
	return m_Pos;
}

void CB3IntSpinButtonCtrl::b3SetAccel(b3_s32 increment,int secs)
{
	UDACCEL  accel[10];
	b3_count count,i;

	count = GetAccel(sizeof(accel) / sizeof(UDACCEL),accel);
	accel[1].nInc = increment;
	accel[1].nSec = secs;
	for (i = 2;i < count;i++)
	{
		accel[i].nInc = accel[i - 1].nInc * 4;
		accel[i].nSec = accel[i - 1].nSec * 5 / 2;
	}
	SetAccel(count,accel);
}

b3_s32 CB3IntSpinButtonCtrl::b3GetAccel()
{
	UDACCEL accel;

	GetAccel(1,&accel);
	return accel.nInc;
}
