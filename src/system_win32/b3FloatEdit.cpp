/*
**
**	$Filename:	b3FloatEdit.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3SystemInclude.h"
#include "blz3/system/b3FloatEdit.h"

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
	char pre_digit[16];
	char post_digit[16];
	
	m_PreDigits   = pre;
	m_PostDigits  = post;
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
