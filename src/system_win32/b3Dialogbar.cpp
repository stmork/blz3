/*
**
**	$Filename:	b3Dialogbar.cpp $  
**	$Release:	Dortmund 2006 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialogbar handling
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
**                        Blizzard III Dialogbar base class             **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3Dialogbar, CDialogBar)
	//{{AFX_MSG_MAP(CB3Dialogbar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CB3Dialogbar, CDialogBar)

CB3Dialogbar::CB3Dialogbar() : b3Link<CB3Dialogbar>(sizeof(CB3Dialogbar))
{
	m_MainFrame = null;
	m_ID        = 0;
	m_Bar       = AFX_IDW_TOOLBAR;
	m_Key.Format("Software\\%s\\%s\\%s",
		BLIZZARD3_REG_COMPANY,
		BLIZZARD3_REG_PRODUCT,
		CB3ClientString());
}

b3_bool CB3Dialogbar::b3Create(CFrameWnd *parent)
{
	b3_bool result;

	m_MainFrame = parent;
	result = Create(m_MainFrame,
		m_ID,
		WS_CHILD | CBRS_ALIGN_ANY | CBRS_GRIPPER,
		m_Bar); // You need this line if you want use LoadBarState()/SaveBarState()!!!!
	if (result)
	{
		SetWindowText(m_Title);
	}
	return result;
}

BOOL CB3Dialogbar::Create(
	CWnd    *pParentWnd,
	UINT     nIDTemplate,
	UINT     nStyle,
	UINT     nID)
{
	BOOL result;
	
	result = CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID);
	if (result)
	{
		SetDlgCtrlID(nID);
	}
	return result;
}

BOOL CB3Dialogbar::Create(
	CWnd    *pParentWnd,
	LPCTSTR  lpszTemplateName,
	UINT     nStyle,
	UINT     nID)
{
	if (CDialogBar::Create(pParentWnd,lpszTemplateName,nStyle,nID))
	{
		SetDlgCtrlID(nID);
		if (OnInitDialog())
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CB3Dialogbar::OnInitDialog()
{
	UpdateData(FALSE);
	b3SetData();

	return TRUE;
}

void CB3Dialogbar::DoDataExchange(CDataExchange *pDX)
{
	ASSERT(pDX);
	CDialogBar::DoDataExchange(pDX);
}

void CB3Dialogbar::b3SetID(
	long id,
	long id_title,
	long id_bar)
{
	m_MainFrame = null;
	m_ID        = id;
	m_Bar       = id_bar;
	m_Visible   = true;
	m_Title.LoadString(id_title);
	m_Value.Format("Toolbar buttons of 0x%04x",m_Bar);
}

b3_bool CB3Dialogbar::b3IsVisible()
{
	m_Visible = (GetSafeHwnd() != 0 ? (GetStyle() & WS_VISIBLE) != 0 : false);

	return m_Visible;
}

b3_bool CB3Dialogbar::b3ToggleVisibility()
{
	m_Visible = !m_Visible;
	m_MainFrame->ShowControlBar(this,m_Visible,FALSE);

	return m_Visible;
}

b3_bool CB3Dialogbar::b3SetVisibility(bool new_visibility)
{
	m_Visible = new_visibility && (GetSafeHwnd() != 0);
	m_MainFrame->ShowControlBar(this,m_Visible,FALSE);
	return m_Visible;
}

// Docking routines
CControlBar *CB3Dialogbar::b3DockRight(CControlBar *Left)
{
	CRect          rect,*rectPtr = null;
	CPoint         pos;
	CControlBar   *result = this;
	unsigned long  n;
	long           dw;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	if (Left == null)
	{
		n       = AFX_IDW_DOCKBAR_LEFT;
		rectPtr = null;
	}
	else
	{
		rectPtr = &rect;
		m_MainFrame->RecalcLayout();
		Left->GetWindowRect(rectPtr);
		rectPtr->OffsetRect(0,0);
		dw = Left->GetBarStyle();
		n  = 0;
		n  = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
		n  = (dw & CBRS_ALIGN_TOP)              ? AFX_IDW_DOCKBAR_TOP    : n;
		n  = (dw & CBRS_ALIGN_RIGHT  && n == 0) ? AFX_IDW_DOCKBAR_RIGHT  : n;
		n  = (dw & CBRS_ALIGN_LEFT   && n == 0) ? AFX_IDW_DOCKBAR_LEFT   : n;

		// When we take the default parameters on rect, DockControlBar will dock
		// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
		// are simulating a Toolbar being dragged to that location and docked.
	}

	m_MainFrame->DockControlBar(this,n,rectPtr);
	return result;
}

void CB3Dialogbar::b3DockSimple()
{
	m_MainFrame->DockControlBar(this);
}

void CB3Dialogbar::b3SaveState(const char *name)
{
//	GetToolBarCtrl().SaveState(HKEY_CURRENT_USER, m_Key,name);
}

void CB3Dialogbar::b3RestoreState(const char *name)
{
	// OK, get values
//	GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER, m_Key,name);
}

void CB3Dialogbar::b3GetData()
{
}

void CB3Dialogbar::b3SetData()
{
}
