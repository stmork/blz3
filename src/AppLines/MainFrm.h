/*
**
**	$Filename:	MainFrm.h $
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

#if !defined(MAINFRM_H)
#define MAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/system/b3Toolbar.h"
#include "blz3/system/b3ComboBox.h"
#include "../smart_gui/CoolMenu.h"
#include "DlgHierarchy.h"
#include "DlgFulcrum.h"
#include "DlgStepMove.h"
#include "DlgStepRotate.h"

#define WM_USER_UPDATE_CONTROLS ((WM_USER) + 100)
#define CB3GetMainFrame() ((CMainFrame *)AfxGetApp()->m_pMainWnd)

class CMainFrame : public CMDIFrameWnd
{
	static UINT m_Indicators[];
	static UINT m_ToolbarBitmaps[];

	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext & dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar         m_wndStatusBar;
	CCoolMenuManager   m_menuManager;		// cool (bitmap button) menus
#ifdef USE_MENUBAR
	CB3Menubar         m_wndMenuBar;
#endif
	CB3Toolbar         m_wndToolBar;
	CB3Toolbar         m_wndViewBar;
	CB3Toolbar         m_wndActnBar;
	CB3Toolbar         m_wndCamrBar;
	CB3Toolbar         m_wndLghtBar;
	CB3Toolbar         m_wndObjtBar;
	CB3Toolbar         m_wndDispBar;
	CB3Toolbar         m_wndAnimBar;
	CDlgFulcrum        m_dlgFulcrum;
	CDlgStepMove       m_dlgStepMove;
	CDlgStepRotate     m_dlgStepRotate;
	CB3ComboBox        m_cameraBox;
	CB3ComboBox        m_lightBox;

public:
	CDlgHierarchy      m_dlgHierarchy;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolbarDropDown(NMHDR * pnmh, LRESULT * plRes);
	afx_msg void OnDestroy();
	afx_msg void OnPrefSave();
	afx_msg void OnPrefAutosave();
	afx_msg void OnUpdatePrefAutosave(CCmdUI * pCmdUI);
	afx_msg void OnCustMain();
	afx_msg void OnCustView();
	afx_msg void OnCustDisplay();
	afx_msg void OnCustAction();
	afx_msg void OnCustCamera();
	afx_msg void OnCustLight();
	afx_msg void OnCustScene();
	afx_msg void OnCustAnim();
	afx_msg void OnBarView();
	afx_msg void OnBarDisplay();
	afx_msg void OnBarAction();
	afx_msg void OnBarCamera();
	afx_msg void OnBarLight();
	afx_msg void OnBarScene();
	afx_msg void OnBarAnim();
	afx_msg void OnBarFulcrum();
	afx_msg void OnBarStepMove();
	afx_msg void OnBarStepRotate();
	afx_msg void OnUpdateBarView(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarDisplay(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarAction(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarCamera(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarLight(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarScene(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarAnim(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarFulcrum(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarStepMove(CCmdUI * pCmdUI);
	afx_msg void OnUpdateBarStepRotate(CCmdUI * pCmdUI);
	afx_msg void OnWindowTileHorz();
	afx_msg void OnWindowTileVert();
	afx_msg LRESULT OnUpdateControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateCamSelect(CCmdUI * pCmdUI);
	afx_msg void OnUpdateLightSelect(CCmdUI * pCmdUI);
	afx_msg void OnHierachy();
	afx_msg void OnUpdateHierachy(CCmdUI * pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void          b3Clear();
	void          b3UpdateCameraBox(b3Scene * scene, b3CameraPart * camera);
	b3CameraPart * b3GetSelectedCamera();
	void          b3UpdateLightBox(b3Scene * scene, b3Light * light);
	b3Light   *   b3GetSelectedLight();
	void          b3UpdateModellerInfo(CAppRenderDoc * pDoc = null);
	void          b3UpdateFulcrum();
	void          b3SetStatusMessage(const char * message);
	void          b3SetStatusMessage(const int message_id);
	void          b3SetPosition(b3_vector * position);
	void          b3SetPerformance(CView * drawing_window, b3_f64 time_diff, b3_count poly_count);
	virtual BOOL PreTranslateMessage(MSG * pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(MAINFRM_H)
