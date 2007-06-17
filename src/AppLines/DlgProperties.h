/*
**
**	$Filename:	DlgProperties.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Scene properties
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGPROPERTIES_H__719C6D93_1C7A_11D6_9D05_080020FDDE74__INCLUDED_)
#define AFX_DLGPROPERTIES_H__719C6D93_1C7A_11D6_9D05_080020FDDE74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProperties.h : header file
//

#include "blz3/system/b3Dialog.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "blz3/system/b3ColorFieldSelector.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties dialog

class CDlgProperties : public CB3Dialog
{
	CAppLinesApp *m_App;
	b3Color       m_ColorBg;
	b3Color       m_ColorUnit;
	b3Color       m_ColorMove;
	b3Color       m_ColorCamera;
	b3Color       m_ColorFulcrum;
	b3Color       m_ColorObject;
	b3Color       m_ColorShape;
	b3Color       m_ColorSelected;
	b3Color       m_ColorGrid;
	b3Color       m_ColorPick;
// Construction
public:
	CDlgProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProperties)
	enum { IDD = IDD_PROPERTIES };
	CSliderCtrl	m_PickSizeCtrl;
	CB3ColorFieldSelector	m_CtrlColorShape;
	CB3ColorFieldSelector	m_CtrlColorSelected;
	CB3ColorFieldSelector	m_CtrlColorPick;
	CB3ColorFieldSelector	m_CtrlColorObject;
	CB3ColorFieldSelector	m_CtrlColorGrid;
	CB3ColorFieldSelector	m_CtrlColorFulcrum;
	CB3ColorFieldSelector	m_CtrlColorCamera;
	CB3ColorFieldSelector	m_CtrlColorBg;
	CB3ColorFieldSelector	m_CtrlColorUnit;
	CB3ColorFieldSelector	m_CtrlColorMove;
	CSliderCtrl	m_RowRefreshCtrl;
	CB3IntSpinButtonCtrl	m_PrtBufferCtrl;
	BOOL	m_BBoxVisible;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProperties)
	virtual void OnOK();
	afx_msg void OnChangeBg();
	afx_msg void OnChangeCamera();
	afx_msg void OnChangeFulcrum();
	afx_msg void OnChangeGrid();
	afx_msg void OnChangeObject();
	afx_msg void OnChangePick();
	afx_msg void OnChangeSelected();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeMove();
	afx_msg void OnChangeUnit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static void b3ReadConfig();

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTIES_H__719C6D93_1C7A_11D6_9D05_080020FDDE74__INCLUDED_)
