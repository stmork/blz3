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
#include "../smart_gui/CoolMenu.h"

class CMainFrame : public CMDIFrameWnd
{
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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar         m_wndStatusBar;
	CCoolMenuManager   m_menuManager;		// cool (bitmap button) menus
	CB3Menubar         m_wndMenuBar;
	CB3Toolbar         m_wndToolBar;
	CB3Toolbar         m_wndViewBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPrefSave();
	afx_msg void OnPrefAutosave();
	afx_msg void OnUpdatePrefAutosave(CCmdUI* pCmdUI);
	afx_msg void OnCustMain();
	afx_msg void OnCustView();
	afx_msg void OnBarView();
	afx_msg void OnUpdateBarView(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(MAINFRM_H)
