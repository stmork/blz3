/*
**
**	$Filename:	b3FloatSpinButtonCtrl.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3FloatSpinButtonCtrl.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**
**	Revision 1.2  2002/03/01 21:25:36  sm
**	- Fixed a problem in create material dialog: Use the
**	  correct function proto types depending on the
**	  message type!
**	
**	Revision 1.1  2002/03/01 20:26:41  sm
**	- Added CB3FloatSpinButtonCtrl for conveniant input.
**	- Made some minor changes and tests.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3FloatSpinButtonCtrl implementation         **
**                                                                      **
*************************************************************************/

CB3FloatSpinButtonCtrl::CB3FloatSpinButtonCtrl()
{
	m_Min       = 0;
	m_Max       = 0;
	m_Increment = B3_FSBC_DEFAULT_INCREMENT;
	m_Accel     = B3_FSBC_DEFAULT_ACCEL;
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

void CB3FloatSpinButtonCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CWnd    *edit = GetBuddy();
	CString  value;
	
	B3_ASSERT(edit != null);
	edit->GetWindowText(value);
	b3SetPos(atof(value) + (b3_f64)pNMUpDown->iDelta * m_Increment);
	*pResult = 1;
}

void CB3FloatSpinButtonCtrl::b3SetRange(b3_f64 min, b3_f64 max,int digits,b3_f64 increment)
{
	int maxint;
	double maxf;
	
	// Ensure something...
	B3_ASSERT(min < max);
	B3_ASSERT(increment > 0);
	B3_ASSERT((digits > 0) && (digits < 6));

	m_Min       = min;
	m_Max       = max;
	m_Increment = increment;
	b3SetDigits(digits);

	maxf = floor((max - min) / increment + 0.5);
	maxint = (int)(maxf > UD_MAXVAL ? UD_MAXVAL : maxf);
	SetRange(0,maxint);
}

void CB3FloatSpinButtonCtrl::b3SetDigits(int digits)
{
	sprintf(m_Format,"%%.%dlf",digits);
}

void CB3FloatSpinButtonCtrl::b3SetPos(b3_f64 pos)
{
	CWnd    *edit = GetBuddy();
	CString  value;

	B3_ASSERT(edit != null);

	B3_LIMIT(pos,m_Min,m_Max);
	value.Format(m_Format,pos);
	edit->SetWindowText(value);
	SetPos((int)floor((pos - m_Min) / m_Increment + 0.5));
}

b3_f64 CB3FloatSpinButtonCtrl::b3GetPos()
{
	CEdit   *edit = (CEdit *)GetBuddy();
	CString  value;
	b3_f64   pos,check;

	B3_ASSERT(edit != null);
	edit->GetWindowText(value);
	check = pos = atof(value);
	B3_LIMIT(check,m_Min,m_Max);
	if (check != pos)
	{
		value.Format(m_Format,check);
		edit->SetWindowText(value);
	}
	return check;
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
