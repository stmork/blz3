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

#include "blz3/raytrace/b3Scene.h"
#include "blz3/system/b3ShowImage.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgScene dialog

class CDlgScene : public CPropertyPage
{
// Construction
public:
	b3_u32   m_ShadingClassType;
	b3Scene *m_Scene;
	b3Scene *m_PreviewScene;
	CDlgScene(b3_u32 class_type, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgScene)
	enum { IDD = IDD_SCENE };
	CB3ShowRaytrace	m_PreviewSceneCtrl;
	CB3ShowImage	m_PreviewImageCtrl;
	CSliderCtrl	m_RayDepthCtrl;
	CSliderCtrl	m_ShadowBrightnessCtrl;
	CString	m_RayDepthLegend;
	CString	m_ShadowBrightnessLegend;
	CB3IntSpinButtonCtrl	m_yResSpin;
	CB3IntSpinButtonCtrl	m_xResSpin;
	int		m_BackgroundMode;
	BOOL	m_ResValid;
	BOOL	m_GfxValid;
	int		m_Resolution;
	int		m_Shading;
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
	afx_msg void OnDestroy();
	afx_msg void OnTopColor();
	afx_msg void OnBottomColor();
	afx_msg void OnChangedResolution();
	afx_msg void OnEditedResolution();
	afx_msg void OnShading();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_LastTraceDepth;
	int m_LastShadowBrightness;
	void    b3PrintLegend();
	void    b3UpdateUI();
	b3_bool b3SetResolution(b3_res xRes,b3_res yRes);
	void    b3SetShading();
	b3_u32  b3GetShading();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCENE_H__11DB61C2_D03E_11D5_89E5_00E018258F1A__INCLUDED_)
