/*
**
**	$Filename:	b3Toolbar.cpp $  
**	$Release:	Dortmund 2000 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	B3 2000 - Toolbar handling
**
**	(C) Copyright 2000  M-DMS GmbH
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        B3 2000 includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3App.h"
#include "blz3/system/b3Toolbar.h"
							  
#include "afxpriv.h"

#define FLAT_TOOLBAR_HEIGHT 30

/*************************************************************************
**                                                                      **
**                        B3 development log                          **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/08/14 13:34:40  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**
**	Revision 1.1  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        B3 CMoolbar collections                     **
**                                                                      **
*************************************************************************/
									
CB3ToolbarState::CB3ToolbarState()
{
	m_ToolbarMain = null;
	m_MenuCount   = 0;
	m_ToolCount   = 0;
	sprintf (m_Code,"_ToolbarState of %s",b3ClientString());
}

void CB3ToolbarState::b3AddToolbar(
	CB3Toolbar *toolbar,
	long         id_bitmap,
	long         id_title)
{
	if (toolbar != null)
	{
		m_Toolbars.b3Append(toolbar);
		toolbar->b3SetID(id_bitmap,id_title,m_ToolCount == 0 ? AFX_IDW_TOOLBAR : id_bitmap);
		m_ToolCount++;
	}
}

void CB3ToolbarState::b3AddMenubar(
	CB3Menubar *menubar,
	long          id_menu)
{
	if (menubar != null)
	{
		m_Menubars.b3Append(menubar);
		menubar->b3SetID(id_menu,m_MenuCount == 0 ? AFX_IDW_MENUBAR : id_menu);
	}
}

b3_bool CB3ToolbarState::b3CreateToolbars(CFrameWnd *parent)
{
	CB3Menubar *mb;
	CB3Toolbar *tb;

	m_ToolbarMain = parent;
	ASSERT(m_ToolbarMain != null);

	// Create menubars
	for (mb = m_Menubars.First;mb != null;mb = mb->Succ)
	{
		if (!mb->b3Create(m_ToolbarMain))
		{
			return false;
		}
	}

	// Create toolbars
	for (tb = m_Toolbars.First;tb != null;tb = tb->Succ)
	{
		if (!tb->b3Create(m_ToolbarMain))
		{
			return false;
		}
	}
	return true;
}

void CB3ToolbarState::b3UpdateUI()
{
	CB3Menubar *mb;
	CB3Toolbar *tb;

	// Do nothing if no MainFrame was specified.
	if (m_ToolbarMain == null)
	{
		return;
	}

	// Update menubars
	for (mb = m_Menubars.First;mb != null;mb = mb->Succ)
	{
		mb->OnUpdateCmdUI(m_ToolbarMain,TRUE);
	}

	// Update toolbars
	for (tb = m_Toolbars.First;tb != null;tb = tb->Succ)
	{
		tb->OnUpdateCmdUI(m_ToolbarMain,TRUE);
	}
}

void CB3ToolbarState::b3LoadState()
{
	CB3Menubar *mb;
	CB3Toolbar *tb;
	CB3Toolbar *last = null;

	// Do nothing if no MainFrame was specified.
	if (m_ToolbarMain == null)
	{
		return;
	}

	// Enable docking for menubars
	for (mb = m_Menubars.First;mb != null;mb = mb->Succ)
	{
		mb->EnableDockingEx(CBRS_ALIGN_ANY);
	}

	// Enable docking for toolbars
	for (tb = m_Toolbars.First;tb != null;tb = tb->Succ)
	{
		tb->EnableDocking(CBRS_ALIGN_ANY);
	}

	// Enable docking for main frame window
	m_ToolbarMain->EnableDocking(CBRS_ALIGN_ANY);

	// Dock menus
	for (mb = m_Menubars.First;mb != null;mb = mb->Succ)
	{
		m_ToolbarMain->DockControlBar(mb);
	}

	// Dock toolbars
	for (tb = m_Toolbars.First;tb != null;tb = tb->Succ)
	{
		tb->b3InitCustomization();
		tb->b3RestoreState();
		last = tb->b3DockRight(last);
	}

	// Load bar state
	try
	{
		m_ToolbarMain->LoadBarState(m_Code);
	}
	catch (...)
	{
		b3PrintF (B3LOG_NORMAL,"### CLASS: b3TBar # b3LoadState() failed.\n");
	}
}

void CB3ToolbarState::b3SaveState()
{
	CB3Toolbar *tb;

	if (m_ToolbarMain != null)
	{
		try
		{
			// Write bar state
			for (tb = m_Toolbars.First;tb != null;tb = tb->Succ)
			{
				tb->b3SaveState();
			}
			m_ToolbarMain->SaveBarState(m_Code);
		}
		catch (...)
		{
			b3PrintF (B3LOG_NORMAL,"### CLASS: b3TBar # b3SaveState() failed.\n");
		}
	}
}

b3_bool CB3ToolbarState::b3PreTranslateMsg(MSG *pMSG)
{
	CB3Menubar *mb;

	for (mb = m_Menubars.First;mb != null;mb = mb->Succ)
	{
		if (mb->TranslateFrameMessage(pMSG))
		{
			return true;
		}
	}
	return false;
}

/*************************************************************************
**                                                                      **
**                        B3 toolbar base class                       **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3Toolbar, CToolBar)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CB3Toolbar)
	//}}AFX_MSG_MAP
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
END_MESSAGE_MAP()

CB3Toolbar::CB3Toolbar() : b3Link<CB3Toolbar>(sizeof(CB3Toolbar))
{
	m_ToolbarMain = null;
	m_ID          = 0;
	m_Bar         = AFX_IDW_TOOLBAR;
	m_ButtonCount = 0;
	m_ToolbarInfo = null;
	m_Visible     = true;
	m_Key.Format("Software\\%s\\%s\\%s",
		BLIZZARD3_REG_COMPANY,
		BLIZZARD3_REG_PRODUCT,
		b3ClientString());
}

CB3Toolbar::~CB3Toolbar()
{
	long i;

	for (i = 0;i < m_ButtonCount;i++)
	{
		m_ToolbarInfo[i].b3FreeText();
	}
	if (m_ToolbarInfo != null)
	{
		delete [] m_ToolbarInfo;
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
	m_ToolbarMain = parent;
	if (CreateEx(m_ToolbarMain,
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
			child->SetWindowPos(&wndTop,rect.left,rect.top,0,0,SWP_NOSIZE);
		}
	}
}

void CB3Toolbar::b3SaveState()
{
	GetToolBarCtrl().SaveState(HKEY_CURRENT_USER, m_Key,m_Value);
}

void CB3Toolbar::b3RestoreState()
{
	// OK, get values
	GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER, m_Key,m_Value);
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
	m_ToolbarMain->ShowControlBar(this,m_Visible,FALSE);

	return m_Visible;
}

b3_bool CB3Toolbar::b3SetVisibility(bool new_visibility)
{
	m_Visible = new_visibility && (GetSafeHwnd() != 0);
	m_ToolbarMain->ShowControlBar(this,m_Visible,FALSE);
	return m_Visible;
}

// Docking routines
CB3Toolbar *CB3Toolbar::b3DockRight(CB3Toolbar *Left)
{
	CRect          rect,*rectPtr = null;
	CPoint         pos;
	CB3Toolbar   *result = this;
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
		m_ToolbarMain->RecalcLayout();
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

	m_ToolbarMain->DockControlBar(this,n,rectPtr);
	return result;
}

void CB3Toolbar::b3DockSimple()
{
	m_ToolbarMain->DockControlBar(this);
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
	m_ToolbarMain->RecalcLayout();

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
	m_ToolbarMain->RecalcLayout();

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

/*************************************************************************
**                                                                      **
**                        B3 toolbar base class                       **
**                                                                      **
*************************************************************************/

CB3Menubar::CB3Menubar() : b3Link<CB3Menubar>(sizeof(CB3Menubar))
{
	m_MenubarMain = null;
	m_ID          = 0;
	m_Bar         = 0;
	m_Visible     = true;
}

void CB3Menubar::b3SetID(long id,long id_bar)
{
	m_ID  = id;
	m_Bar = id_bar;
}

b3_bool CB3Menubar::b3Create(CFrameWnd *parent)
{
	m_MenubarMain = parent;
	if (CreateEx(m_MenubarMain,
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
	m_MenubarMain->ShowControlBar(this,m_Visible,FALSE);

	return m_Visible;
}

b3_bool CB3Menubar::b3SetVisibility(bool new_visibility)
{
	m_Visible = new_visibility && (GetSafeHwnd() != 0);
	m_MenubarMain->ShowControlBar(this,m_Visible,FALSE);
	return m_Visible;
}
