/*
**
**	$Filename:	MainFrm.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Lines MainFrame windows
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "MainFrm.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/08/11 15:59:58  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	Revision 1.1  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CMainFrame implementation                     **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_PREF_SAVE, OnPrefSave)
	ON_COMMAND(ID_PREF_AUTOSAVE, OnPrefAutosave)
	ON_UPDATE_COMMAND_UI(ID_PREF_AUTOSAVE, OnUpdatePrefAutosave)
	ON_COMMAND(ID_CUST_MAIN, OnCustMain)
	ON_COMMAND(ID_CUST_VIEW, OnCustView)
	ON_COMMAND(IDM_BAR_VIEW, OnBarView)
	ON_UPDATE_COMMAND_UI(IDM_BAR_VIEW, OnUpdateBarView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT toolbar_bitmaps[] =
{
	IDR_MAINFRAME,
	IDR_TOOLBAR_VIEW
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	CB3App       *app   = b3GetApp();
#ifndef _DEBUG
	b3_log_level  level = B3LOG_NORMAL;
#endif

#ifndef _DEBUG
	level = (b3_log_level)app->b3ReadInt("Settings","DebugLevel",B3LOG_NORMAL);
	b3Log_SetLevel(level);
#endif

	// Get window sizes...
	app->b3GetWindowMode();
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnPrefSave() 
{
	// TODO: Add your command handler code here
	// Write mainframe dimensions to registry
	CB3App *app = b3GetApp();

	app->b3SetWindowMode(true);
}

void CMainFrame::OnPrefAutosave() 
{
	// TODO: Add your command handler code here
	CB3App *app = b3GetApp();

	app->m_AutoSave = !app->m_AutoSave;
}

void CMainFrame::OnUpdatePrefAutosave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CB3App *app = b3GetApp();

	pCmdUI->SetCheck(app->m_AutoSave);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CB3App       *app   = b3GetApp();

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	app->b3GfxType(this);
	app->b3MoveWindow(this);

	app->b3AddMenubar(&m_wndMenuBar,IDR_MAINFRAME);
	app->b3AddToolbar(&m_wndToolBar,IDR_MAINFRAME,   IDS_TOOLBAR_MAINFRAME);
	app->b3AddToolbar(&m_wndViewBar,IDR_TOOLBAR_VIEW,IDS_TOOLBAR_VIEW);
	if (!app->b3CreateToolbars(this))
	{
		b3PrintF(B3LOG_NORMAL,"Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Set docking mode
	app->b3LoadState();

	// install/load cool menus
	m_menuManager.Install(this);
	m_menuManager.LoadToolbars(toolbar_bitmaps,sizeof(toolbar_bitmaps) / sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnDestroy() 
{
	// TODO: Add your message handler code here
	CB3App *app = b3GetApp();

	app->b3SetWindowMode(false);
	CMDIFrameWnd::OnDestroy();
}

void CMainFrame::OnCustMain() 
{
	// TODO: Add your command handler code here
	m_wndToolBar.b3Customize();
}

void CMainFrame::OnCustView() 
{
	// TODO: Add your command handler code here
	m_wndViewBar.b3Customize();
}

void CMainFrame::OnBarView() 
{
	// TODO: Add your command handler code here
	m_wndViewBar.b3ToggleVisibility();
}

void CMainFrame::OnUpdateBarView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndViewBar.b3IsVisible());
}
