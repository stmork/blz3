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
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3FloatSpinButtonCtrl.h"

#define B3_VAL_TO_RANGE(x) ((int)floor(((x) - m_Min) / m_Increment + 0.5))

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.11  2004/05/06 18:13:52  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**
**	Revision 1.10  2004/04/10 15:59:51  sm
**	- Added control units as base class for
**	  o CB3FloatSliderCtrl
**	  o CB3FloatSpinButtonCtrl
**	
**	Revision 1.9  2004/04/10 14:33:25  sm
**	- Added oak plank support.
**	
**	Revision 1.8  2003/08/31 08:56:23  sm
**	- Windows support for the snprintf functions
**	
**	Revision 1.7  2003/08/28 14:44:27  sm
**	- Further buffer overflow prevention:
**	  o added b3Path::b3Format
**	  o added b3Path::b3Append
**	- Further strcat/strcpy removal necessary
**	
**	Revision 1.6  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.5  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**	Revision 1.4  2002/03/08 16:46:15  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
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
	b3SetPos(atof(value) / b3GetUnit() + (b3_f64)pNMUpDown->iDelta * m_Increment);
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
		m_Pos = pos = atof(value) / b3GetUnit();
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
	value.Format(m_Format,m_Pos * b3GetUnit());
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
