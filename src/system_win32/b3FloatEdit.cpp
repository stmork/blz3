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
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CB3FloatEdit, CEdit)

/////////////////////////////////////////////////////////////////////////////
// CB3FloatEdit message handlers

void CB3FloatEdit::b3SetMin(b3_f64 min)
{
	m_Min = min;
	m_MinActive = true;
}

void CB3FloatEdit::b3SetMax(b3_f64 max)
{
	m_Max = max;
	m_MaxActive = true;
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
		sprintf(digit,"%d",pre);
		strcat(m_Format,digit);
	}
	if (post >= 0)
	{
		sprintf(digit,".%d",post);
		strcat(m_Format,digit);
	}
	strcat (m_Format,"lf");
}

void CB3FloatEdit::b3SetValue(b3_f64 value)
{
	sprintf(m_Buffer,m_Format,m_Value = value);
	SetLimitText(sizeof(m_Buffer));
	SetWindowText(m_Buffer);
}

b3_bool CB3FloatEdit::b3Check()
{
	b3_bool result = true;

	GetWindowText(m_Buffer,sizeof(m_Buffer));
	if (sscanf(m_Buffer,"%lf",&m_Value) == 1)
	{
		if ((m_MinActive) && (m_Value < m_Min))
		{
			result = false;
			b3SetValue(m_Min);
		}

		if ((m_MaxActive) && (m_Value > m_Max))
		{
			result = false;
			b3SetValue(m_Max);
		}
	}
	else
	{
		result = false;
	}

	return result;
}

void CB3FloatEdit::OnKillFocus(CWnd* pNewWnd) 
{
	b3Check();
	CEdit::OnKillFocus(pNewWnd);
}

void CB3FloatEdit::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
//	if (!b3Check())
	{
//		B3_BEEP;
	}
}
