/*
**
**	$Filename:	MainFrm.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1471 $
**	$Date: 2006-12-25 13:41:42 +0100 (Mo, 25 Dez 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "b3Splash.h"
#include "MainFrm.h"
#include ".\mainfrm.h"

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
	ON_COMMAND(ID_CUST_DISPLAY, OnCustDisplay)
	ON_COMMAND(ID_CUST_ACTION, OnCustAction)
	ON_COMMAND(ID_CUST_CAMERA, OnCustCamera)
	ON_COMMAND(ID_CUST_LIGHT, OnCustLight)
	ON_COMMAND(ID_CUST_SCENE, OnCustScene)
	ON_COMMAND(ID_CUST_ANIM, OnCustAnim)
	ON_COMMAND(IDM_BAR_VIEW, OnBarView)
	ON_COMMAND(IDM_BAR_DISPLAY, OnBarDisplay)
	ON_COMMAND(IDM_BAR_ACTION, OnBarAction)
	ON_COMMAND(IDM_BAR_CAMERA, OnBarCamera)
	ON_COMMAND(IDM_BAR_LIGHT, OnBarLight)
	ON_COMMAND(IDM_BAR_SCENE, OnBarScene)
	ON_COMMAND(IDM_BAR_ANIM, OnBarAnim)
	ON_COMMAND(IDM_BAR_FULCRUM, OnBarFulcrum)
	ON_COMMAND(IDM_BAR_STEP_MOVE, OnBarStepMove)
	ON_COMMAND(IDM_BAR_STEP_ROTATE, OnBarStepRotate)
	ON_UPDATE_COMMAND_UI(IDM_BAR_VIEW, OnUpdateBarView)
	ON_UPDATE_COMMAND_UI(IDM_BAR_DISPLAY, OnUpdateBarDisplay)
	ON_UPDATE_COMMAND_UI(IDM_BAR_ACTION, OnUpdateBarAction)
	ON_UPDATE_COMMAND_UI(IDM_BAR_CAMERA, OnUpdateBarCamera)
	ON_UPDATE_COMMAND_UI(IDM_BAR_LIGHT, OnUpdateBarLight)
	ON_UPDATE_COMMAND_UI(IDM_BAR_SCENE, OnUpdateBarScene)
	ON_UPDATE_COMMAND_UI(IDM_BAR_ANIM, OnUpdateBarAnim)
	ON_UPDATE_COMMAND_UI(IDM_BAR_FULCRUM, OnUpdateBarFulcrum)
	ON_UPDATE_COMMAND_UI(IDM_BAR_STEP_MOVE, OnUpdateBarStepMove)
	ON_UPDATE_COMMAND_UI(IDM_BAR_STEP_ROTATE, OnUpdateBarStepRotate)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILE_VERT, OnWindowTileVert)
	ON_MESSAGE(WM_USER_UPDATE_CONTROLS, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SELECT, OnUpdateCamSelect)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SELECT, OnUpdateLightSelect)
	ON_COMMAND(ID_HIERACHY, OnHierachy)
	ON_UPDATE_COMMAND_UI(ID_HIERACHY, OnUpdateHierachy)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT CMainFrame::m_Indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_COORD_X,
	ID_COORD_Y,
	ID_COORD_Z,
	ID_DRAWING_PERF,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

UINT CMainFrame::m_ToolbarBitmaps[] =
{
	IDR_MAINFRAME,
	IDR_TOOLBAR_VIEW,
	IDR_TOOLBAR_ACTION,
	IDR_TOOLBAR_CAMERA,
	IDR_TOOLBAR_LIGHT,
	IDR_TOOLBAR_OBJECT,
	IDR_TOOLBAR_DISPLAY,
	IDR_TOOLBAR_ANIMATION
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	CB3App       *app   = CB3GetApp();

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
	CB3App *app = CB3GetApp();

	app->b3SetWindowMode(true);
}

void CMainFrame::OnPrefAutosave() 
{
	// TODO: Add your command handler code here
	CB3App *app = CB3GetApp();

	app->m_AutoSave = !app->m_AutoSave;
}

void CMainFrame::OnUpdatePrefAutosave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CB3App *app = CB3GetApp();

	pCmdUI->SetCheck(app->m_AutoSave);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CB3App *app = CB3GetApp();

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	app->b3GfxType(this);
	app->b3MoveWindow(this);

#ifdef USE_MENUBAR
	app->b3AddMenubar(&m_wndMenuBar,     IDR_MAINFRAME);
#endif
	app->b3AddToolbar(&m_wndToolBar,     IDR_MAINFRAME,         IDS_TOOLBAR_MAINFRAME);
	app->b3AddToolbar(&m_wndObjtBar,     IDR_TOOLBAR_OBJECT,    IDS_TOOLBAR_OBJECT);
	app->b3AddToolbar(&m_wndViewBar,     IDR_TOOLBAR_VIEW,      IDS_TOOLBAR_VIEW);
	app->b3AddToolbar(&m_wndAnimBar,     IDR_TOOLBAR_ANIMATION, IDS_TOOLBAR_ANIMATION);
	app->b3AddToolbar(&m_wndActnBar,     IDR_TOOLBAR_ACTION,    IDS_TOOLBAR_ACTION);
	app->b3AddToolbar(&m_wndCamrBar,     IDR_TOOLBAR_CAMERA,    IDS_TOOLBAR_CAMERA);
	app->b3AddToolbar(&m_wndLghtBar,     IDR_TOOLBAR_LIGHT,     IDS_TOOLBAR_LIGHT);
	app->b3AddToolbar(&m_wndDispBar,     IDR_TOOLBAR_DISPLAY,   IDS_TOOLBAR_DISPLAY);
	app->b3AddDialogbar(&m_dlgHierarchy, IDD_HIERARCHY,         IDS_DIALOGBAR_HIERARCHY);
	app->b3AddDialogbar(&m_dlgStepMove,  IDD_STEP_MOVE,         IDS_DIALOGBAR_STEP_MOVE);
	app->b3AddDialogbar(&m_dlgFulcrum,   IDD_FULCRUM,           IDS_DIALOGBAR_FULCRUM);
	app->b3AddDialogbar(&m_dlgStepRotate,IDD_STEP_ROTATE,       IDS_DIALOGBAR_STEP_ROTATE);
	if (!app->b3CreateToolbars(this))
	{
		b3PrintF(B3LOG_NORMAL,"Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.b3AddArrow(ID_EDIT_UNDO);
	m_wndToolBar.b3AddArrow(ID_EDIT_REDO);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(m_Indicators,
		  sizeof(m_Indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	b3SetPosition(null);
	m_wndStatusBar.SetPaneText(4,"");

	m_cameraBox.b3Create(&m_wndCamrBar,ID_CAMERA_SELECT);
	m_lightBox.b3Create(&m_wndLghtBar,ID_LIGHT_SELECT);

	// Set docking mode
	app->b3LoadState();

	// install/load cool menus
	m_menuManager.Install(this);
	m_menuManager.LoadToolbars(m_ToolbarBitmaps,sizeof(m_ToolbarBitmaps) / sizeof(UINT));

#ifndef _DEBUG
	CSplashWnd::ShowSplashScreen(this);
#endif
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_MAXIMIZE;
	return TRUE;
}

void CMainFrame::OnDestroy() 
{
	// TODO: Add your message handler code here
	CB3App *app = CB3GetApp();

	app->b3SaveState();
	app->b3SetWindowMode(false);
	CMDIFrameWnd::OnDestroy();
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

/*************************************************************************
**                                                                      **
**                        Toolbar actions for undo/redo                 **
**                                                                      **
*************************************************************************/

void CMainFrame::OnToolbarDropDown(NMHDR* pNmHdr, LRESULT *plr)
{
	NMTOOLBAR     *pnmtb = (NMTOOLBAR *)pNmHdr;
	CView         *view = GetActiveFrame()->GetActiveView();
	CAppRenderDoc *pDoc = (CAppRenderDoc *)view->GetDocument();
	CRect          rect;
	
	// Switch on button command id's.
	switch (pnmtb->iItem)
	{
	case ID_EDIT_UNDO:
		m_wndToolBar.GetItemRect(m_wndToolBar.CommandToIndex(pnmtb->iItem),&rect);
		m_wndToolBar.ClientToScreen(&rect);
		pDoc->b3UndoList(&rect);
		CB3GetMainFrame()->SetActiveWindow();
		break;
	
	case ID_EDIT_REDO:
		m_wndToolBar.GetItemRect(m_wndToolBar.CommandToIndex(pnmtb->iItem),&rect);
		m_wndToolBar.ClientToScreen(&rect);
		pDoc->b3RedoList(&rect);
		CB3GetMainFrame()->SetActiveWindow();
		break;

	default:
		return;
	}
}

/*************************************************************************
**                                                                      **
**                        Toolbar customization                         **
**                                                                      **
*************************************************************************/

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

void CMainFrame::OnCustDisplay() 
{
	// TODO: Add your command handler code here
	m_wndDispBar.b3Customize();
}

void CMainFrame::OnCustAction() 
{
	// TODO: Add your command handler code here
	m_wndActnBar.b3Customize();
}

void CMainFrame::OnCustCamera() 
{
	// TODO: Add your command handler code here
	m_wndCamrBar.b3Customize();
}

void CMainFrame::OnCustLight() 
{
	// TODO: Add your command handler code here
	m_wndLghtBar.b3Customize();
}

void CMainFrame::OnCustScene() 
{
	// TODO: Add your command handler code here
	m_wndObjtBar.b3Customize();
}

void CMainFrame::OnCustAnim() 
{
	// TODO: Add your command handler code here
	m_wndAnimBar.b3Customize();
}

void CMainFrame::OnBarView() 
{
	// TODO: Add your command handler code here
	m_wndViewBar.b3ToggleVisibility();
}

void CMainFrame::OnBarDisplay() 
{
	// TODO: Add your command handler code here
	m_wndDispBar.b3ToggleVisibility();
}

void CMainFrame::OnBarAction() 
{
	// TODO: Add your command handler code here
	m_wndActnBar.b3ToggleVisibility();
}

void CMainFrame::OnBarCamera() 
{
	// TODO: Add your command handler code here
	m_wndCamrBar.b3ToggleVisibility();
}

void CMainFrame::OnBarLight() 
{
	// TODO: Add your command handler code here
	m_wndLghtBar.b3ToggleVisibility();
}

void CMainFrame::OnBarScene() 
{
	// TODO: Add your command handler code here
	m_wndObjtBar.b3ToggleVisibility();
}

void CMainFrame::OnBarAnim() 
{
	// TODO: Add your command handler code here
	m_wndAnimBar.b3ToggleVisibility();
}

void CMainFrame::OnBarFulcrum() 
{
	// TODO: Add your command handler code here
	m_dlgFulcrum.b3ToggleVisibility();
}

void CMainFrame::OnBarStepMove() 
{
	// TODO: Add your command handler code here
	m_dlgStepMove.b3ToggleVisibility();
}

void CMainFrame::OnBarStepRotate() 
{
	// TODO: Add your command handler code here
	m_dlgStepRotate.b3ToggleVisibility();
}

void CMainFrame::OnHierachy() 
{
	// TODO: Add your command handler code here
	m_dlgHierarchy.b3ToggleVisibility();
}

void CMainFrame::OnUpdateBarView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndViewBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarDisplay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndDispBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarAction(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndActnBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarCamera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndCamrBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarLight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndLghtBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarScene(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndObjtBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarAnim(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_wndAnimBar.b3IsVisible());
}

void CMainFrame::OnUpdateBarFulcrum(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_dlgFulcrum.b3IsVisible());
}

void CMainFrame::OnUpdateBarStepMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_dlgStepMove.b3IsVisible());
}

void CMainFrame::OnUpdateBarStepRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_dlgStepRotate.b3IsVisible());
}

void CMainFrame::OnUpdateHierachy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_dlgHierarchy.b3IsVisible());
}

/*************************************************************************
**                                                                      **
**                        Some other handlings...                       **
**                                                                      **
*************************************************************************/

void CMainFrame::OnWindowTileHorz() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;

	MDITile (MDITILE_HORIZONTAL);
}

void CMainFrame::OnWindowTileVert() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;

	MDITile (MDITILE_VERTICAL);
}

LRESULT CMainFrame::OnUpdateControls(WPARAM wParam,LPARAM lParam)
{
	CAppLinesApp *app = (CAppLinesApp *)AfxGetApp();

	app->b3UpdateUI();
	return 0;
}

/*************************************************************************
**                                                                      **
**                        Combobox handling                             **
**                                                                      **
*************************************************************************/

void CMainFrame::b3Clear()
{
	m_cameraBox.b3Clear();
	m_lightBox.b3Clear();
}

void CMainFrame::b3UpdateCameraBox(b3Scene *scene,b3CameraPart *act)
{
	b3CameraPart *camera;

	m_cameraBox.b3Clear();
	for (camera  = scene->b3GetFirstCamera();
	     camera != null;
		 camera  = scene->b3GetNextCamera(camera))
	{
		m_cameraBox.b3AddString(camera->m_CameraName,camera);
	}
	m_cameraBox.b3SetString(act->m_CameraName,true);
}

b3CameraPart *CMainFrame::b3GetSelectedCamera()
{
	b3_s32 index = m_cameraBox.GetCurSel();

	return index != CB_ERR ? (b3CameraPart *)m_cameraBox.GetItemDataPtr(index) : null;
}

void CMainFrame::b3UpdateLightBox(b3Scene *scene,b3Light *act)
{
	b3Light *light;

	m_lightBox.b3Clear();
	for (light = scene->b3GetLight();light != null;light = (b3Light *)light->Succ)
	{
		m_lightBox.b3AddString(light->m_Name,light);
	}
	m_lightBox.b3SetString(act->m_Name,true);
}

b3Light *CMainFrame::b3GetSelectedLight()
{
	b3_s32 index = m_lightBox.GetCurSel();

	return index != CB_ERR ? (b3Light *)m_lightBox.GetItemDataPtr(index) : null;
}

void CMainFrame::b3UpdateModellerInfo(CAppRenderDoc *pDoc)
{
	m_dlgFulcrum.m_pDoc    = pDoc;
	m_dlgStepMove.m_pDoc   = pDoc;
	m_dlgStepRotate.m_pDoc = pDoc;
	if (pDoc != null)
	{
		m_dlgHierarchy.b3InitTree(pDoc);
	}
}

void CMainFrame::b3UpdateFulcrum()
{
	m_dlgFulcrum.b3SetData();
}

/*************************************************************************
**                                                                      **
**                        Statusbar handling                            **
**                                                                      **
*************************************************************************/

void CMainFrame::b3SetStatusMessage(const char *message)
{
	m_wndStatusBar.SetPaneText(0,message);
	b3PrintF(B3LOG_FULL,"Status message: %s\n",message);
}

void CMainFrame::b3SetStatusMessage(const int message_id)
{
	CString message;

	message.LoadString(message_id);
	b3SetStatusMessage(message);
}

void CMainFrame::b3SetPosition(b3_vector *position)
{
	if (position == null)
	{
		m_wndStatusBar.SetPaneText(1,"x:");
		m_wndStatusBar.SetPaneText(2,"y:");
		m_wndStatusBar.SetPaneText(3,"z:");
	}
	else
	{
		CString x,y,z;

		x.Format("x: %5.2f",position->x);
		y.Format("y: %5.2f",position->y);
		z.Format("z: %5.2f",position->z);
		m_wndStatusBar.SetPaneText(1,x);
		m_wndStatusBar.SetPaneText(2,y);
		m_wndStatusBar.SetPaneText(3,z);
	}
}

void CMainFrame::b3SetPerformance(
	CView    *drawer,
	b3_f64    time_diff,
	b3_count  poly_count)
{
	b3_f64  frame_rate;
	b3_f64  polygon_rate;
	CString text;

	if (GetActiveFrame()->GetActiveView() == drawer)
	{
		frame_rate   = 1.0 / time_diff;
		polygon_rate = poly_count * frame_rate;
		text.Format(IDS_FRAME_RATE,frame_rate,polygon_rate);
		m_wndStatusBar.SetPaneText(4,text);
	}
}

void CMainFrame::OnUpdateCamSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateLightSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
#ifdef USE_MENUBAR
	if (m_wndMenuBar.TranslateFrameMessage(pMsg))
	{
		return TRUE;
	}
#endif
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}
