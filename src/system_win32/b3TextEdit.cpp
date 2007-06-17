/*
**
**	$Filename:	b3TextEdit.cpp $
**	$Release:	Dortmund 2006 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Editing text
**
**	(C) Copyright 2006  Steffen A. Mork
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
#include "blz3/system/b3TextEdit.h"

/*************************************************************************
**                                                                      **
**                        Editing text                                  **
**                                                                      **
*************************************************************************/

CB3TextEdit::CB3TextEdit(void)
{
}

CB3TextEdit::~CB3TextEdit(void)
{
}

void CB3TextEdit::b3DDX(CDataExchange *pDX,char *buffer, int size)
{
	ASSERT(size > 1);
	if (pDX->m_bSaveAndValidate)
	{
		GetWindowText(buffer, size-1);
	}
	else
	{
		SetLimitText(size-1);
		SetWindowText(buffer);
	}
}

void CB3TextEdit::b3DDX(CDataExchange *pDX, CString &buffer, int size)
{
	ASSERT(size > 1);
	if (pDX->m_bSaveAndValidate)
	{
		GetWindowText(buffer);
	}
	else
	{
		SetLimitText(size-1);
		SetWindowText(buffer);
	}
}

BEGIN_MESSAGE_MAP(CB3TextEdit, CEdit)
	//{{AFX_MSG_MAP(CB3TextEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
