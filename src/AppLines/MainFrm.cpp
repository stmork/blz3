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
#include "b3Splash.h"
#include "blz3/raytrace/b3Light.h"
#include "blz3/raytrace/b3Special.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.42  2005/01/23 20:57:22  sm
**	- Moved some global static variables into class static ones.
**
**	Revision 1.41  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.40  2004/12/22 21:36:37  sm
**	- Changed development environment to Visual C++ .net 2003
**	
**	Revision 1.39  2004/05/16 09:21:11  sm
**	- Fixed ticket no. 22: Camera deletions are handled
**	  correctly now
**	
**	Revision 1.38  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.37  2004/05/06 08:38:32  sm
**	- Demerged raytracing includes of Lines
**	
**	Revision 1.36  2003/02/19 18:27:27  sm
**	- Release bug fix for logging
**	
**	Revision 1.35  2003/02/08 14:04:18  sm
**	- Started support for document wise bar state
**	
**	Revision 1.34  2003/02/01 12:57:17  sm
**	- Ready to undo/redo!
**	
**	Revision 1.33  2003/01/30 16:19:59  sm
**	- Added undo/redo list support.
**	
**	Revision 1.32  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.31  2002/08/19 16:50:39  sm
**	- Now having animation running, running, running...
**	- Activation handling modified to reflect animation
**	  and user transformation actions.
**	- Made some architectual redesigns.
**	
**	Revision 1.30  2002/08/17 17:31:22  sm
**	- Introduced animation support (Puh!)
**	
**	Revision 1.29  2002/01/21 16:56:46  sm
**	- Showing splash dialog only in release version.
**	- Prepared shape icons.
**	
**	Revision 1.28  2002/01/20 12:48:51  sm
**	- Added splash screen
**	- Corrected repeat buttons (capture change)
**	
**	Revision 1.27  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.26  2002/01/09 19:03:53  sm
**	- Docking bar order changed.
**	
**	Revision 1.25  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.24  2002/01/08 15:45:50  sm
**	- Added support for repeating CButtons for button movement/rotation mode.
**	
**	Revision 1.23  2002/01/05 22:17:47  sm
**	- Recomputing bounding boxes correctly
**	- Found key input bug: The accelerator are the problem
**	- Code cleanup
**	
**	Revision 1.22  2002/01/04 17:53:53  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.21  2002/01/03 19:07:27  sm
**	- Cleaned up cut/paste
**	
**	Revision 1.20  2002/01/02 15:48:37  sm
**	- Added automated expand/collapse to hierarchy tree.
**	
**	Revision 1.19  2001/12/31 16:39:41  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.18  2001/12/28 15:17:44  sm
**	- Added clipboard-copy to raytraced view
**	- Added printing to raytraced view
**	- Much minor UI tuning done:
**	  o added accelerators
**	  o open maximized window
**	  o fixed some UpdateUI methods
**	  o changed exception handling in CB3ScrollView and CB3BitmapDxB
**	
**	Revision 1.17  2001/12/26 18:17:56  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
**	Revision 1.16  2001/12/26 12:00:36  sm
**	- Fixed modeller info dialog
**	
**	Revision 1.15  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**	Revision 1.14  2001/12/03 18:37:51  sm
**	- Added light distribution curve control.
**	
**	Revision 1.13  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.12  2001/11/11 11:51:20  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**	Revision 1.11  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
**	Revision 1.10  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.9  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.8  2001/10/03 20:17:55  sm
**	- Minor bugfixes
**	
**	Revision 1.7  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**	Revision 1.6  2001/09/01 15:54:53  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**	
**	Revision 1.5  2001/08/20 14:16:48  sm
**	- Putting data into cmaera and light combobox.
**	- Selecting camera and light.
**	
**	Revision 1.4  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**	
**	Revision 1.3  2001/08/14 13:34:40  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**	
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
#ifndef _DEBUG
	b3_log_level  level = B3LOG_NORMAL;
#endif

#ifndef _DEBUG
	level = (b3_log_level)app->b3ReadInt("Settings","DebugLevel",B3LOG_NORMAL);
	b3Log::b3SetLevel(level);
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

	app->b3AddMenubar(&m_wndMenuBar,     IDR_MAINFRAME);
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
