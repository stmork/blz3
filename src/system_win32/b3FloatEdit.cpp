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
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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
