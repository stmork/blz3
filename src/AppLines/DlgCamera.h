/*
**
**	$Filename:	DlgCamera.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Camera properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCAMERA_H__4FB4644D_F620_11D5_8A08_00E018258F1A__INCLUDED_)
#define AFX_DLGCAMERA_H__4FB4644D_F620_11D5_8A08_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCamera.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCamera dialog

class CDlgCamera : public CDialog
{
	b3_f64        m_UnitFactor;
// Construction
public:
	b3Scene      *m_Scene;
	b3CameraPart *m_Camera;
	CDlgCamera(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCamera)
	enum { IDD = IDD_CAMERA };
	CComboBox	m_TypeCtrl;
	CComboBox	m_CameraListCtrl;
	CB3FloatEdit	m_FocalLengthCtrl;
	CB3FloatEdit	m_WidthCtrl;
	CB3FloatEdit	m_HeightCtrl;
	CB3FloatEdit	m_zViewCtrl;
	CB3FloatEdit	m_yViewCtrl;
	CB3FloatEdit	m_xViewCtrl;
	CB3FloatEdit	m_zPosCtrl;
	CB3FloatEdit	m_yPosCtrl;
	CB3FloatEdit	m_xPosCtrl;
	BOOL	m_EnableCamera;
	CString	m_UnitString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCamera)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCamera)
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraNew();
	afx_msg void OnCameraDelete();
	afx_msg void OnSelchangeCamera();
	afx_msg void OnKillfocusCamera();
	afx_msg void OnCameraState();
	afx_msg void OnKillfocusCameraType();
	afx_msg void OnSelchangeCameraTypes();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
	void b3SetCamera();
	void b3GetCamera();
	void b3RefreshList();
	int  b3GetCameraType();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAMERA_H__4FB4644D_F620_11D5_8A08_00E018258F1A__INCLUDED_)
