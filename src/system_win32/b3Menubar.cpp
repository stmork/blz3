/*
**
**	$Filename:	b3Menubar.cpp $  
**	$Release:	Dortmund 2006 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Menubar handling
**
**	(C) Copyright 2006  Steffen A. Mork
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
#include "blz3/system/b3App.h"
#include "blz3/system/b3Toolbar.h"
							  
#include "afxpriv.h"
#include "afxext.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III Menubar base class               **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3Menubar, CMenuBar)

BEGIN_MESSAGE_MAP(CB3Menubar, CMenuBar)
	//{{AFX_MSG_MAP(CB3Menubar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3Menubar::CB3Menubar() : b3Link<CB3Menubar>(sizeof(CB3Menubar))
{
	m_MainFrame = null;
	m_ID        = 0;
	m_Bar       = 0;
	m_Visible   = true;
}

void CB3Menubar::b3SetID(long id,long id_bar)
{
	m_ID  = id;
	m_Bar = id_bar;
}

b3_bool CB3Menubar::b3Create(CFrameWnd *parent)
{
	m_MainFrame = parent;
	if (CreateEx(m_MainFrame,
		WS_CHILD | WS_VISIBLE |
		CBRS_TOP | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_RAISEDBORDER),CRect(0,0,0,0),m_Bar)
	{
		if (LoadMenuBar(m_ID))
		{
//			SetHeight(FLAT_TOOLBAR_HEIGHT);
			return true;
		}
	}
	return false;
}

b3_bool CB3Menubar::b3IsVisible()
{
	m_Visible = (GetSafeHwnd() != 0 ? (GetStyle() & WS_VISIBLE) != 0 : false);

	return m_Visible;
}

b3_bool CB3Menubar::b3ToggleVisibility()
{
	m_Visible = !m_Visible;
	m_MainFrame->ShowControlBar(this,m_Visible,FALSE);

	return m_Visible;
}

b3_bool CB3Menubar::b3SetVisibility(bool new_visibility)
{
	m_Visible = new_visibility && (GetSafeHwnd() != 0);
	m_MainFrame->ShowControlBar(this,m_Visible,FALSE);
	return m_Visible;
}
