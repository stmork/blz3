/*
**
**	$Filename:	b3Checkbox.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Checkbox control
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
#include "blz3/system/b3Checkbox.h"

/*************************************************************************
**                                                                      **
**                        Editing a checkbox                            **
**                                                                      **
*************************************************************************/

CB3Checkbox::CB3Checkbox(void)
{
}

CB3Checkbox::~CB3Checkbox(void)
{
}

void CB3Checkbox::b3DDX(CDataExchange *pDX,b3_bool &value)
{
	if (pDX->m_bSaveAndValidate)
	{
		value = GetCheck() == BST_CHECKED;
	}
	else
	{
		SetCheck(value ? BST_CHECKED : BST_UNCHECKED);
	}
}


BEGIN_MESSAGE_MAP(CB3Checkbox, CButton)
	//{{AFX_MSG_MAP(CB3Checkbox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
