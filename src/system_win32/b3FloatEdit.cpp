/*
**
**	$Filename:	b3FloatEdit.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Editing a floating point number
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3FloatEdit.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**
**	Revision 1.8  2002/03/08 16:46:15  sm
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
**	Revision 1.7  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.6  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.5  2001/12/31 16:39:41  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.4  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.3  2001/12/21 16:46:16  sm
**	- New dialog for camera properties
**	- Done some bugfixes concerning CB3FloatEdit
**	
**	Revision 1.2  2001/12/02 16:30:39  sm
**	- CDlgScene draw error fix
**	
**	Revision 1.1  2001/11/18 13:49:26  sm
**	- Introduced new CB3FloatEdit derived from CEdit
**	- DlgNebular implemented
**	- DlgLensFlare implemented
**	- Adjusting far clipping plane inside b3RenderView
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Editing a floating point number               **
**                                                                      **
*************************************************************************/

CB3FloatEdit::CB3FloatEdit()
{
	m_Value = 0;
	b3SetDigits(-1,-1);
	b3DisableBound();
}

CB3FloatEdit::~CB3FloatEdit()
{
}


BEGIN_MESSAGE_MAP(CB3FloatEdit, CEdit)
	//{{AFX_MSG_MAP(CB3FloatEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CB3FloatEdit, CEdit)

/////////////////////////////////////////////////////////////////////////////
// CB3FloatEdit message handlers

void CB3FloatEdit::b3DDX(CDataExchange *pDX,b3_f32 &pos)
{
	b3_f64 value = pos;

	b3DDX(pDX,value);
	pos = (b3_f32)value;
}

void CB3FloatEdit::b3DDX(CDataExchange *pDX,b3_f64 &pos)
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

b3_f64 CB3FloatEdit::b3SetMin(b3_f64 min)
{
	m_Min = min;
	m_MinActive = true;
	return b3Check();
}

b3_f64 CB3FloatEdit::b3SetMax(b3_f64 max)
{
	m_Max = max;
	m_MaxActive = true;
	return b3Check();
}

b3_f64 CB3FloatEdit::b3SetRange(b3_f64 min,b3_f64 max)
{
	m_Min       = min;
	m_MinActive = true;
	m_Max       = max;
	m_MaxActive = true;
	return b3Check();
}

void CB3FloatEdit::b3DisableBound()
{
	m_MinActive = false;
	m_MaxActive = false;
}

void CB3FloatEdit::b3SetDigits(int pre, int post)
{
	char digit[16];
	
	m_PreDigits  = pre;
	m_PostDigits = post;
	strcpy (m_Format,"%");
	if (pre >= 0)
	{
		snprintf(digit,sizeof(digit),"%d",pre);
		strcat(m_Format,digit);
	}
	if (post >= 0)
	{
		snprintf(digit,sizeof(digit),".%d",post);
		strcat(m_Format,digit);
	}
	strcat (m_Format,"lf");
}

b3_f64 CB3FloatEdit::b3SetPos(b3_f64 value)
{
	m_Value = value;
	b3Check();
	snprintf(m_Buffer,sizeof(m_Buffer),m_Format,m_Value);
	SetLimitText(sizeof(m_Buffer));
	SetWindowText(m_Buffer);
	return m_Value;
}

b3_f64 CB3FloatEdit::b3GetPos()
{
	b3_f64 pos;

	GetWindowText(m_Buffer,sizeof(m_Buffer));
	m_Value = pos = atof(m_Buffer);
	b3Check();
	if (m_Value != pos)
	{
		b3SetPos(m_Value);
	}
	return m_Value;
}

b3_f64 CB3FloatEdit::b3Check()
{
	if ((m_MinActive) && (m_Value < m_Min))
	{
		m_Value = m_Min;
	}

	if ((m_MaxActive) && (m_Value > m_Max))
	{
		m_Value = m_Max;
	}
	return m_Value;
}

void CB3FloatEdit::OnKillFocus(CWnd* pNewWnd) 
{
	m_Value = b3GetPos();
	CEdit::OnKillFocus(pNewWnd);
}
