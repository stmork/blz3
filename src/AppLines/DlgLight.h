/*
**
**	$Filename:	DlgLight.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Light properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGLIGHT_H__8DBD88BD_E707_11D5_89FB_00E018258F1A__INCLUDED_)
#define AFX_DLGLIGHT_H__8DBD88BD_E707_11D5_89FB_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLight.h : header file
//

#include "blz3/raytrace/b3Light.h"
#include "blz3/system/b3ColorField.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "b3ControlLDC.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLight dialog

class CDlgLight : public CDialog
{
	// Construction
	b3_u32     m_ShadingClassType;
	b3Scene  * m_LightScene;
public:
	b3Base<b3Item> * m_LightBase;
	b3Light    *    m_Light;
	CDlgLight(b3_u32 class_type, CWnd * pParent = NULL);  // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgLight)
	enum { IDD = IDD_LIGHT };
	CB3ControlLDC	m_CtrlLDC;
	CB3ControlLDC	m_CtrlDiagram;
	CB3ShowRaytrace	m_CtrlPreview;
	CComboBox	m_LightListCtrl;
	CB3FloatEdit	m_SoftSizeCtrl;
	CSliderCtrl	m_SampleCtrl;
	CB3FloatEdit	m_DistanceCtrl;
	CB3ColorField	m_ColorCtrl;
	CB3FloatEdit	m_zPosCtrl;
	CB3FloatEdit	m_yPosCtrl;
	CB3FloatEdit	m_xPosCtrl;
	CB3FloatEdit	m_zDirCtrl;
	CB3FloatEdit	m_yDirCtrl;
	CB3FloatEdit	m_xDirCtrl;
	BOOL	m_EnableLight;
	BOOL	m_EnableLDC;
	BOOL	m_EnableSoft;
	CString	m_SampleLabel;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLight)
public:
	virtual BOOL PreTranslateMessage(MSG * pMsg);
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult);
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLight)
	virtual BOOL OnInitDialog();
	afx_msg void OnLightColorChange();
	afx_msg void OnLightNew();
	afx_msg void OnLightDelete();
	afx_msg void OnLightState();
	afx_msg void OnSelchangeLight();
	afx_msg void OnKillfocusLight();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnReleasedCaptureLightDistr(NMHDR * pNMHDR, LRESULT * pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
	void b3UpdatePreview();
	void b3SetLight();
	void b3GetLight();
	void b3RefreshList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLIGHT_H__8DBD88BD_E707_11D5_89FB_00E018258F1A__INCLUDED_)
