/*
**
**	$Filename:	b3ComboBox.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Combobox handling
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3ComboBox.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3ComboBox class                **
**                                                                      **
*************************************************************************/
  
CB3ComboBox::CB3ComboBox()
{
}

b3_bool CB3ComboBox::b3Create(CB3Toolbar *toolbar,b3_s32 id,b3_res width,b3_bool sort)
{
	CRect    rect;
	b3_s32   index;
	b3_s32   style;
	b3_bool  success = false;
	b3_bool  is_flat;

	m_ID      = id;
	m_Toolbar = toolbar;
	is_flat   = m_Toolbar->GetStyle() & TBSTYLE_FLAT;
	index     = m_Toolbar->CommandToIndex(m_ID);
	if (index >= 0)
	{
		// Only if document type button is specified, create doctype
		// combobox
		m_Toolbar->SetButtonInfo(index, m_ID, TBBS_SEPARATOR, width);
		m_Toolbar->GetItemRect  (index, &rect);
		rect.top    = (is_flat ? 0 : 2);
		rect.bottom = rect.top + 250;
		style       = CBS_DROPDOWNLIST|CBS_HASSTRINGS|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL;
		if (sort)
		{
			style |= CBS_SORT;
		}
		success = Create(style, rect, m_Toolbar, m_ID);
	}

	if (!success)
	{
		b3PrintF (B3LOG_NORMAL,"### CLASS: ComboBox # Failed to create combo-box!\n");
	}
	return success;
}

void CB3ComboBox::b3Clear()
{
	ResetContent();
}

b3_s32 CB3ComboBox::b3AddString(const char *text,void *ptr)
{
	b3_s32 index;

	index = AddString(text);
	if (index != CB_ERR)
	{
		SetItemDataPtr(index,ptr);
	}
	return index;
}

b3_s32 CB3ComboBox::b3FindString(const char *text)
{
	return FindStringExact(-1,text);
}

b3_s32 CB3ComboBox::b3SetString(const char *text,b3_bool notify_parent)
{
	b3_s32 index;

	index = FindStringExact(-1,text);
	if (index == CB_ERR)
	{
		index = 0;
	}
	index = SetCurSel(index);
	if (index != CB_ERR)
	{
		GetWindowText((char *)text,GetWindowTextLength() + 1);
	}
	b3PrintF (B3LOG_FULL,"### CLASS: b3CBox # b3SetString(%s) = %d\n",
		text,index);

	if (notify_parent)
	{
		CWnd *parent = GetParentOwner();
		int   id     = GetDlgCtrlID();
		
		parent->SendMessage(
			WM_COMMAND,
			MAKELONG(id,CBN_SELCHANGE),
			(LPARAM)m_hWnd);
	}
	return index;
}
