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
	ON_COMMAND(IDM_BAR_VIEW, OnBarView)
	ON_COMMAND(IDM_BAR_DISPLAY, OnBarDisplay)
	ON_COMMAND(IDM_BAR_ACTION, OnBarAction)
	ON_COMMAND(IDM_BAR_CAMERA, OnBarCamera)
	ON_COMMAND(IDM_BAR_LIGHT, OnBarLight)
	ON_COMMAND(IDM_BAR_SCENE, OnBarScene)
	ON_UPDATE_COMMAND_UI(IDM_BAR_VIEW, OnUpdateBarView)
	ON_UPDATE_COMMAND_UI(IDM_BAR_DISPLAY, OnUpdateBarDisplay)
	ON_UPDATE_COMMAND_UI(IDM_BAR_ACTION, OnUpdateBarAction)
	ON_UPDATE_COMMAND_UI(IDM_BAR_CAMERA, OnUpdateBarCamera)
	ON_UPDATE_COMMAND_UI(IDM_BAR_LIGHT, OnUpdateBarLight)
	ON_UPDATE_COMMAND_UI(IDM_BAR_SCENE, OnUpdateBarScene)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILE_VERT, OnWindowTileVert)
	ON_MESSAGE(WM_USER_UPDATE_CONTROLS, OnUpdateControls)
	ON_COMMAND(IDM_BAR_FULCRUM, OnBarFulcrum)
	ON_COMMAND(IDM_BAR_STEP_MOVE, OnBarStepMove)
	ON_COMMAND(IDM_BAR_STEP_ROTATE, OnBarStepRotate)
	ON_UPDATE_COMMAND_UI(IDM_BAR_FULCRUM, OnUpdateBarFulcrum)
	ON_UPDATE_COMMAND_UI(IDM_BAR_STEP_MOVE, OnUpdateBarStepMove)
	ON_UPDATE_COMMAND_UI(IDM_BAR_STEP_ROTATE, OnUpdateBarStepRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_DRAWING_PERF,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT toolbar_bitmaps[] =
{
	IDR_MAINFRAME,
	IDR_TOOLBAR_VIEW,
	IDR_TOOLBAR_ACTION,
	IDR_TOOLBAR_CAMERA,
	IDR_TOOLBAR_LIGHT,
	IDR_TOOLBAR_OBJECT,
	IDR_TOOLBAR_DISPLAY
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
	app->b3AddToolbar(&m_wndToolBar,     IDR_MAINFRAME,      IDS_TOOLBAR_MAINFRAME);
	app->b3AddToolbar(&m_wndViewBar,     IDR_TOOLBAR_VIEW,   IDS_TOOLBAR_VIEW);
	app->b3AddToolbar(&m_wndDispBar,     IDR_TOOLBAR_DISPLAY,IDS_TOOLBAR_DISPLAY);
	app->b3AddToolbar(&m_wndObjtBar,     IDR_TOOLBAR_OBJECT, IDS_TOOLBAR_OBJECT);
	app->b3AddToolbar(&m_wndActnBar,     IDR_TOOLBAR_ACTION, IDS_TOOLBAR_ACTION);
	app->b3AddToolbar(&m_wndCamrBar,     IDR_TOOLBAR_CAMERA, IDS_TOOLBAR_CAMERA);
	app->b3AddToolbar(&m_wndLghtBar,     IDR_TOOLBAR_LIGHT,  IDS_TOOLBAR_LIGHT);
	app->b3AddDialogbar(&m_dlgFulcrum,   IDD_FULCRUM,        IDS_DIALOGBAR_FULCRUM);
	app->b3AddDialogbar(&m_dlgStepMove,  IDD_STEP_MOVE,      IDS_DIALOGBAR_STEP_MOVE);
	app->b3AddDialogbar(&m_dlgStepRotate,IDD_STEP_ROTATE,    IDS_DIALOGBAR_STEP_ROTATE);
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
	m_wndStatusBar.SetPaneText(1,"");

	m_cameraBox.b3Create(&m_wndCamrBar,ID_CAM_SELECT);
	m_lightBox.b3Create(&m_wndLghtBar,ID_LIGHT_SELECT);

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

void CMainFrame::OnUpdateControls()
{
	CAppLinesApp *app = (CAppLinesApp *)AfxGetApp();

	app->b3UpdateUI();
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
	for (camera  = scene->b3GetCamera();
	     camera != null;
		 camera  = scene->b3GetNextCamera(camera))
	{
		m_cameraBox.b3AddString(camera->m_CameraName,camera);
	}
	m_cameraBox.b3SetString(act->m_CameraName);
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
	m_lightBox.b3SetString(act->m_Name);
}

b3Light *CMainFrame::b3GetSelectedLight()
{
	b3_s32 index = m_lightBox.GetCurSel();

	return index != CB_ERR ? (b3Light *)m_lightBox.GetItemDataPtr(index) : null;
}

void CMainFrame::b3UpdateModellerInfo(CAppLinesDoc *pDoc)
{
	m_dlgFulcrum.m_pDoc    = pDoc;
	m_dlgStepMove.m_Info   = pDoc != null ? pDoc->m_Info : null;
	m_dlgStepRotate.m_Info = pDoc != null ? pDoc->m_Info : null;
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

void CMainFrame::b3SetPerformance(
	CView    *drawer,
	long      millis,
	b3_count  poly_count)
{
	b3_f64  frame_rate;
	b3_f64  polygon_rate;
	CString text;

	if (GetActiveFrame()->GetActiveView() == drawer)
	{
		frame_rate   = 1000.0 / (b3_f64)millis;
		polygon_rate = poly_count * frame_rate;
		text.Format(IDS_FRAME_RATE,frame_rate,polygon_rate);
		m_wndStatusBar.SetPaneText(1,text);
	}
}
