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

#include "blz3/b3Config.h"
#include "blz3/system/b3App.h"
#include "blz3/system/b3Toolbar.h"
							  
#include "afxpriv.h"
#include "afxext.h"

#define FLAT_TOOLBAR_HEIGHT 28

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.14  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**
**	Revision 1.13  2003/02/08 14:04:18  sm
**	- Started support for document wise bar state
**	
**	Revision 1.12  2003/01/30 16:19:59  sm
**	- Added undo/redo list support.
**	
**	Revision 1.11  2002/01/13 19:24:12  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.10  2002/01/08 15:45:50  sm
**	- Added support for repeating CButtons for button movement/rotation mode.
**	
**	Revision 1.9  2002/01/05 22:17:48  sm
**	- Recomputing bounding boxes correctly
**	- Found key input bug: The accelerator are the problem
**	- Code cleanup
**	
**	Revision 1.8  2001/12/31 16:39:41  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.7  2001/12/27 21:33:35  sm
**	- Further docking handling done
**	- CDocument cleanups done
**	
**	Revision 1.6  2001/12/26 18:17:56  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
**	Revision 1.5  2001/12/26 12:00:36  sm
**	- Fixed modeller info dialog
**	
**	Revision 1.4  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**	Revision 1.3  2001/11/11 11:51:21  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
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
**                        Blizzard III - docking window collections     **
**                                                                      **
*************************************************************************/
									
CB3ToolbarState::CB3ToolbarState(const char *appName)
{
	m_MainFrame = null;
	m_MenuCount = 0;
	m_ToolCount = 0;
	snprintf (m_Code,sizeof(m_Code),"_ToolbarState of %s",appName);
}

void CB3ToolbarState::b3AddToolbar(
	CB3Toolbar *toolbar,
	long        id_bitmap,
	long        id_title)
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

void CB3ToolbarState::b3AddDialogbar(
	CB3Dialogbar *dialogbar,
	long          id_dialog,
	long          id_title)
{
	if (dialogbar != null)
	{
		m_Dialogbars.b3First(dialogbar);
		dialogbar->b3SetID(id_dialog,id_title,m_ToolCount == 0 ? AFX_IDW_DIALOGBAR : id_dialog);
	}
}

b3_bool CB3ToolbarState::b3CreateToolbars(CFrameWnd *parent)
{
	CB3Menubar   *mb;
	CB3Toolbar   *tb;
	CB3Dialogbar *db;

	m_MainFrame = parent;
	ASSERT(m_MainFrame != null);

	// Create menubars
	B3_FOR_BASE(&m_Menubars,mb)
	{
		if (!mb->b3Create(m_MainFrame))
		{
			return false;
		}
	}

	// Create toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		if (!tb->b3Create(m_MainFrame))
		{
			return false;
		}
	}

	// Create toolbars
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		if (!db->b3Create(m_MainFrame))
		{
			return false;
		}
	}
	return true;
}

void CB3ToolbarState::b3UpdateUI()
{
	CB3Menubar   *mb;
	CB3Toolbar   *tb;
	CB3Dialogbar *db;

	// Do nothing if no MainFrame was specified.
	if (m_MainFrame == null)
	{
		return;
	}

	// Update menubars
	B3_FOR_BASE(&m_Menubars,mb)
	{
		mb->OnUpdateCmdUI(m_MainFrame,TRUE);
	}

	// Update toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->OnUpdateCmdUI(m_MainFrame,TRUE);
	}

	// Update dialogbars
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->OnUpdateCmdUI(m_MainFrame,TRUE);
	}
}

void CB3ToolbarState::b3GetData()
{
	CB3Dialogbar *db;

	// Read data out of controls
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->b3GetData();
	}
}

void CB3ToolbarState::b3SetData()
{
	CB3Dialogbar *db;

	// Put data into controls
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->b3SetData();
	}
}

void CB3ToolbarState::b3Free()
{
	CB3Toolbar   *tb;

	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->b3Free();
	}
}

void CB3ToolbarState::b3LoadState(const char *name)
{
	CB3Menubar   *mb;
	CB3Toolbar   *tb;
	CB3Dialogbar *db;
	CControlBar  *last = null;

	// Do nothing if no MainFrame was specified.
	if (m_MainFrame == null)
	{
		return;
	}

	// Enable docking for menubars
	B3_FOR_BASE(&m_Menubars,mb)
	{
		mb->EnableDockingEx(CBRS_ALIGN_ANY);
	}

	// Enable docking for toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->EnableDocking(CBRS_ALIGN_ANY);
	}

	// Enable docking for dialogbars
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->EnableDocking(CBRS_ALIGN_ANY);
	}

	// Enable docking for main frame window
	m_MainFrame->EnableDocking(CBRS_ALIGN_ANY);

	// Dock menus
	B3_FOR_BASE(&m_Menubars,mb)
	{
		m_MainFrame->DockControlBar(mb);
	}

	// Dock toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->b3InitCustomization();
		tb->b3RestoreState(name);
		last = tb->b3DockRight(last);
	}

	// Dock Dialogbars
	last = null;
	B3_FOR_BASE(&m_Dialogbars,db)
	{
//		db->b3InitCustomization();
		db->b3RestoreState(name);
		last = db->b3DockRight(last);
	}

	// Load bar state
	try
	{
		if (name == null)
		{
			name = m_Code;
		}
		m_MainFrame->LoadBarState(name);
	}
	catch (...)
	{
		b3PrintF (B3LOG_NORMAL,"### CLASS: b3TBar # b3LoadState() failed.\n");
	}
}

void CB3ToolbarState::b3SaveState(const char *name)
{
	CB3Toolbar   *tb;
	CB3Dialogbar *db;

	if (m_MainFrame != null)
	{
		try
		{
			// Write bar state
			B3_FOR_BASE(&m_Toolbars,tb)
			{
				tb->b3SaveState(name);
			}

			// Write bar state
			B3_FOR_BASE(&m_Dialogbars,db)
			{
				db->b3SaveState(name);
			}
			if (name == null)
			{
				name = m_Code;
			}
			m_MainFrame->SaveBarState(name);
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

	B3_FOR_BASE(&m_Menubars,mb)
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
			child->SetWindowPos(&wndTop,rect.left,rect.top,0,0,SWP_NOSIZE);
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

	GetToolBarCtrl().SaveState(HKEY_CURRENT_USER, m_Key,b3GetValueName(name,value));
}

void CB3Toolbar::b3RestoreState(const char *name)
{
	// OK, get values
	CString value;

	GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER, m_Key,b3GetValueName(name,value));
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
	if (CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID))
	{
		SetDlgCtrlID(nID);
		if (OnInitDialog())
		{
			return TRUE;
		}
	}
	return FALSE;
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
