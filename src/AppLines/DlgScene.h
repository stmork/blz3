/*
**
**	$Filename:	DlgScene.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Scene properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSCENE_H__11DB61C2_D03E_11D5_89E5_00E018258F1A__INCLUDED_)
#define AFX_DLGSCENE_H__11DB61C2_D03E_11D5_89E5_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScene.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3ShowImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgScene dialog

class CDlgScene : public CDialog
{
// Construction
public:
	b3Scene * m_Scene;
	CDlgScene(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgScene)
	enum { IDD = IDD_SCENE };
	CSpinButtonCtrl	m_yResSpin;
	CSpinButtonCtrl	m_xResSpin;
	CSliderCtrl	m_RayDepth;
	CB3ShowImage	m_PreviewScene;
	CB3ShowImage	m_PreviewImage;
	BOOL	m_ResValid;
	CString	m_RayDepthLegend;
	int		m_BackgroundMode;
	int		m_xRes;
	int		m_yRes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScene)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScene)
	afx_msg void OnBgModeChanged();
	afx_msg void OnBgImageSelect();
	afx_msg void OnResValid();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_LastTraceDepth;
	void b3PrintRayDepth();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCENE_H__11DB61C2_D03E_11D5_89E5_00E018258F1A__INCLUDED_)
