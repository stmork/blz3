/*
**
**	$Filename:	b3Toolbar.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Toolbar handling
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
#include "blz3/system/b3App.h"
#include "blz3/system/b3Toolbar.h"
							  
#include "afxpriv.h"
#include "afxext.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III toolbar base class               **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3Toolbar, CToolBar)
	//{{AFX_MSG_MAP(CB3Toolbar)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TBN_BEGINADJUST, OnToolBarBeginAdjust)
	ON_NOTIFY_REFLECT(TBN_BEGINDRAG, OnToolBarBeginDrag)
	ON_NOTIFY_REFLECT(TBN_CUSTHELP, OnToolBarCustomHelp)
	ON_NOTIFY_REFLECT(TBN_ENDADJUST, OnToolBarEndAdjust)
	ON_NOTIFY_REFLECT(TBN_ENDDRAG, OnToolBarEndDrag)
	ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, OnToolBarGetButtonInfo)
	ON_NOTIFY_REFLECT(TBN_QUERYDELETE, OnToolBarQueryDelete)
	ON_NOTIFY_REFLECT(TBN_QUERYINSERT, OnToolBarQueryInsert)
	ON_NOTIFY_REFLECT(TBN_RESET, OnToolBarReset)
	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnToolBarChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CB3Toolbar, CToolBar)

CB3Toolbar::CB3Toolbar() : b3Link<CB3Toolbar>(sizeof(CB3Toolbar))
{
	m_MainFrame   = null;
	m_ID          = 0;
	m_Bar         = AFX_IDW_TOOLBAR;
	m_ButtonCount = 0;
	m_ToolbarInfo = null;
	m_Visible     = true;
	m_Key.Format("Software\\%s\\%s\\%s",
		BLIZZARD3_REG_COMPANY,
		BLIZZARD3_REG_PRODUCT,
		CB3ClientString());
}

CB3Toolbar::~CB3Toolbar()
{
	b3Free();
}

void CB3Toolbar::b3Free()
{
	if (m_ToolbarInfo != null)
	{
		delete [] m_ToolbarInfo;
		m_ToolbarInfo = null;
	}
}

void CB3Toolbar::b3SetID(
	long id_bitmap,
	long id_title,
	long id_bar)
{
	m_ID  = id_bitmap;
	m_Bar = id_bar;
	m_Title.LoadString(id_title);
	m_Value.Format("Toolbar buttons of 0x%04x",m_Bar);
}

b3_bool CB3Toolbar::b3Create(CFrameWnd *parent)
{
	m_MainFrame = parent;
	if (CreateEx(m_MainFrame,
		TBSTYLE_FLAT|TBSTYLE_ALTDRAG,
		WS_VISIBLE | WS_CHILD |
		CCS_ADJUSTABLE |
		CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(0,0,0,0),m_Bar)) // You need this line if you want use LoadBarState()/SaveBarState()!!!!
		// Bloody f*cking Bill!
	{
		if (LoadToolBar(m_ID))
		{
			SetHeight(FLAT_TOOLBAR_HEIGHT);
			SetWindowText(m_Title);
			return true;
		}
	}
	return false;
}

void CB3Toolbar::b3AddArrow(int id)
{
	DWORD dwStyle;
	int   index;

	GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	index   = CommandToIndex(id);
	dwStyle = GetButtonStyle(index);
	SetButtonStyle(index, dwStyle | TBSTYLE_DROPDOWN);
}

b3_bool CB3Toolbar::b3InitCustomization()
{
	CToolBarCtrl &ctrl = GetToolBarCtrl();
	CString       button_text;
	unsigned int  style,id_command;
	int           idx_bitmap;
	long          i,len,pos;

	m_ButtonCount = ctrl.GetButtonCount();
	m_ToolbarInfo = new CB3ToolbarInfo[m_ButtonCount];
	for (i = 0;i < m_ButtonCount;i++)
	{	   
		GetButtonInfo(i,id_command,style,idx_bitmap);

		// Copy some values
		m_ToolbarInfo[i].tbButton.iBitmap   = idx_bitmap;
		m_ToolbarInfo[i].tbButton.idCommand = id_command;
		m_ToolbarInfo[i].tbButton.fsState   = (unsigned char)ctrl.GetState(id_command);
		m_ToolbarInfo[i].tbButton.fsStyle   = (unsigned char)style;
		m_ToolbarInfo[i].tbButton.dwData    = 0;
		m_ToolbarInfo[i].tbButton.iString   = i;

		// Prepare explanation of customize dialog
		button_text.LoadString(id_command);	
		pos = button_text.Find("\n");
		len = button_text.GetLength() - pos - 1;
		if (len > 0)
		{
			m_ToolbarInfo[i].btnText = new char[len + 1];
			strcpy (m_ToolbarInfo[i].btnText,button_text.Right(len));
		}

		CWnd *child      = GetDlgItem(id_command);

		// Found something that sholdn't be there...
		if (child != null)
		{
			CRect rect;

			child->GetWindowRect(&rect);
			m_ToolbarInfo[i].width = rect.Width();
		}
		else
		{
			m_ToolbarInfo[i].width = 0;
		}
	}
	return m_ToolbarInfo != null;
}

void CB3Toolbar::b3Reset()
{
	CWnd *child;
	long  i,id_command;

	// Get default state...
	LoadToolBar(m_ID);
	for (i = 0;i < m_ButtonCount;i++)
	{
		id_command = m_ToolbarInfo[i].tbButton.idCommand;
		child      = GetDlgItem(id_command);
		if (child != null)
		{
			SetButtonInfo(i, id_command, TBBS_SEPARATOR, m_ToolbarInfo[i].width);
		}
	}
}

void CB3Toolbar::b3AdjustComboboxes()
{
	CWnd         *child;
	CRect         rect;
	unsigned int  id_command;
	long          i;

	// Check if we have to reposition combobox
	for (i = 0;i < m_ButtonCount;i++)
	{
		id_command = m_ToolbarInfo[i].tbButton.idCommand;
		child      = GetDlgItem(id_command);

		// Found something that sholdn't be there...
		if (child != null)
		{
			// Get the actual position and move this "something"
			// to the right place.
			GetItemRect(CommandToIndex(id_command),&rect);
			child->SetWindowPos(&wndTop,rect.left,rect.top-1,0,0,SWP_NOSIZE);
		}
	}
}

CString & CB3Toolbar::b3GetValueName(const char *name,CString &result)
{
	if (name != null)
	{
		result = CString(name) + "_";
	}
	result += m_Value;
	return result;

}
void CB3Toolbar::b3SaveState(const char *name)
{
	CString value;

	GetToolBarCtrl().SaveState(HKEY_CURRENT_USER, m_Key, b3GetValueName(name,value));
}

void CB3Toolbar::b3RestoreState(const char *name)
{
	// OK, get values
	CString value;

	GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER, m_Key, b3GetValueName(name,value));
	b3AdjustComboboxes();
}

b3_bool CB3Toolbar::b3IsVisible()
{
	m_Visible = (GetSafeHwnd() != 0 ? (GetStyle() & WS_VISIBLE) != 0 : false);

	return m_Visible;
}

b3_bool CB3Toolbar::b3ToggleVisibility()
{
	m_Visible = !m_Visible;
	m_MainFrame->ShowControlBar(this,m_Visible,FALSE);

	return m_Visible;
}

b3_bool CB3Toolbar::b3SetVisibility(bool new_visibility)
{
	m_Visible = new_visibility && (GetSafeHwnd() != 0);
	m_MainFrame->ShowControlBar(this,m_Visible,FALSE);
	return m_Visible;
}

// Docking routines
CControlBar *CB3Toolbar::b3DockRight(CControlBar *Left)
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
		n       = 0;
		rectPtr = null;
	}
	else
	{
		rectPtr = &rect;
		m_MainFrame->RecalcLayout();
		Left->GetWindowRect(rectPtr);
		rectPtr->OffsetRect(1,0);
		dw = Left->GetBarStyle();
		n  = 0;
		n  = (dw & CBRS_ALIGN_TOP)              ? AFX_IDW_DOCKBAR_TOP    : n;
		n  = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
		n  = (dw & CBRS_ALIGN_LEFT   && n == 0) ? AFX_IDW_DOCKBAR_LEFT   : n;
		n  = (dw & CBRS_ALIGN_RIGHT  && n == 0) ? AFX_IDW_DOCKBAR_RIGHT  : n;

		// When we take the default parameters on rect, DockControlBar will dock
		// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
		// are simulating a Toolbar being dragged to that location and docked.
	}

	m_MainFrame->DockControlBar(this,n,rectPtr);
	return result;
}

void CB3Toolbar::b3DockSimple()
{
	m_MainFrame->DockControlBar(this);
}

// CB3Toolbar message handlers

// This function is called to populate the toolbar customization dialog box
// with information regarding all of the possible toolbar buttons.
void CB3Toolbar::OnToolBarGetButtonInfo(NMHDR *notify, LRESULT *result)
{
	TBNOTIFY *tbStruct;		// data needed by customize dialog box
	char     *text;
	long      i;

	// init the pointer
	tbStruct = (TBNOTIFY *)notify;
	*result  = FALSE;

	// if the index is valid
	i = tbStruct->iItem;
	if ((0 <= i) && (i < m_ButtonCount))
	{
		// copy the stored button structure
		tbStruct->tbButton = m_ToolbarInfo[i].tbButton;

		// copy the text for the button label in the dialog
		text = m_ToolbarInfo[i].btnText;
		strcpy(tbStruct->pszText, text != null ? text : "");

		// indicate valid data was sent
		*result = TRUE;
	}
}

// This function is called when the user begins dragging a toolbar
// button or when the customization dialog is being populated with
// toolbar information.  Basically, *result should be populated with
// your answer to the question, "is the user allowed to insert a
// button to the left of this one?".
void CB3Toolbar::OnToolBarQueryInsert(NMHDR *notify, LRESULT *result)
{
	// We can insert!
	*result = TRUE;
}

// This function is called when the user begins dragging a toolbar
// button or when the customization dialog is being populated with
// toolbar information.  Basically, *result should be populated with
// your answer to the question, "is the user allowed to delete this
// button?".
void CB3Toolbar::OnToolBarQueryDelete(NMHDR *notify, LRESULT *result)
{
	// if we're not floating - user can delete anything
	TBNOTIFY *tbStruct;		// data needed by customize dialog box
	CWnd     *child;
	long      id;

	// If this command id has got a child we assume a combobox
	// which couldn't be deleted.
	tbStruct = (TBNOTIFY *)notify;
	id       = tbStruct->tbButton.idCommand;
	child    = GetDlgItem(id);
	*result  = (child == null);
}

// This function is called when the user begins dragging a toolbar button.
void CB3Toolbar::OnToolBarBeginDrag(NMHDR *notify, LRESULT *result)
{
	// We cannot drag!
	*result = FALSE;
}

// This function is called when the user has completed a dragging operation.
void CB3Toolbar::OnToolBarEndDrag(NMHDR *notify, LRESULT *result)
{
	// We cannot drag!
	*result = FALSE;
}

// This function is called when the user initially calls up the toolbar
// customization dialog box.
void CB3Toolbar::OnToolBarBeginAdjust(NMHDR *notify, LRESULT *result)
{
	// We cannot drag!
	*result = FALSE;
}

// This function is called when the user clicks on the help button on the
// toolbar customization dialog box.
void CB3Toolbar::OnToolBarCustomHelp(NMHDR *notify, LRESULT *result)
{
	// We cannot help (... Hospi)!
	*result = FALSE;
}

// This function is called when the user dismisses the toolbar customization
// dialog box.
void CB3Toolbar::OnToolBarEndAdjust(NMHDR *notify, LRESULT *result)
{
	// save the state of the toolbar for reinitialization
//	SaveState();
}

// This function is called whenever the user makes a change to the
// layout of the toolbar.  Calling the mainframe's RecalcLayout forces
// the toolbar to repaint itself.
void CB3Toolbar::OnToolBarChange(NMHDR *notify, LRESULT *result)
{
	// Move combobox to the right place
	b3AdjustComboboxes();

	// Force the frame window to recalculate the size
	m_MainFrame->RecalcLayout();

	if (IsFloating())
	{
		CalcDynamicLayout(-1, LM_STRETCH|LM_HORZ|LM_COMMIT);
	}
	OnIdleUpdateCmdUI(TRUE, 0L);//	RecalcLayout();
}

// This function is called when the user clicks on the reset button on the
// toolbar customization dialog box.
void CB3Toolbar::OnToolBarReset(NMHDR *notify, LRESULT *result)
{
	// ...and move combobox to right place
	b3Reset();
	b3AdjustComboboxes();

	// Force the frame window to recalculate the size
	m_MainFrame->RecalcLayout();

	if (IsFloating())
	{
		CalcDynamicLayout(-1, LM_STRETCH|LM_HORZ|LM_COMMIT);
	}
	OnIdleUpdateCmdUI(TRUE, 0L);
	*result = TRUE;
}

void CB3Toolbar::b3Customize()
{
	GetToolBarCtrl().Customize();
}

void CB3Toolbar::b3Print(CRect &rect,long dw)
{
	if (b3IsVisible())
	{
		char *where_docked;
		long  n;

		n  = 0;
		n  = (dw & CBRS_ALIGN_TOP)              ? AFX_IDW_DOCKBAR_TOP    : n;
		n  = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
		n  = (dw & CBRS_ALIGN_LEFT   && n == 0) ? AFX_IDW_DOCKBAR_LEFT   : n;
		n  = (dw & CBRS_ALIGN_RIGHT  && n == 0) ? AFX_IDW_DOCKBAR_RIGHT  : n;

		switch (n)
		{
		case AFX_IDW_DOCKBAR_TOP:
			where_docked = "top   ";
			break;

		case AFX_IDW_DOCKBAR_BOTTOM:
			where_docked = "bottom";
			break;

		case AFX_IDW_DOCKBAR_LEFT:
			where_docked = "left  ";
			break;

		case AFX_IDW_DOCKBAR_RIGHT:
			where_docked = "right ";
			break;

		default:
			where_docked = "";
			break;
		}
#ifdef _DEBUG
		b3PrintF (B3LOG_FULL,"### CLASS: b3TBar # %04x: %s %s %4ld,%4ld - %4ld,%4ld # %08lx [%s]\n",
			m_ID,
			(GetBarStyle() & CBRS_ORIENT_HORZ) != 0 ? "HORZ" : "VERT",
			IsFloating() != 0 ? "Float" : "  -  ",
			rect.left,rect.top,rect.right,rect.bottom,dw,where_docked);
#endif
	}
	else
	{
#ifdef _DEBUG
		b3PrintF (B3LOG_FULL,"### CLASS: b3TBar # %04x: Not visible\n",
			m_ID);
#endif
	}
}
