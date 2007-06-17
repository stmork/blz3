/*
**
**	$Filename:	DlgNebular.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1341 $
**	$Date: 2006-03-19 15:47:18 +0100 (So, 19 Mrz 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Nebular properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGNEBULAR_H__5BDBF7FB_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
#define AFX_DLGNEBULAR_H__5BDBF7FB_D6AE_11D5_89F1_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNebular.h : header file
//

#include "blz3/raytrace/b3Special.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3ColorFieldSelector.h"
#include "blz3/system/b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgNebular dialog

class CDlgNebular : public CPropertyPage
{
// Construction
	b3_u32       m_ShadingClassType;
	b3Scene     *m_NebularScene;
	b3Nebular   *m_EditNebular;

public:
	b3Nebular   *m_Nebular;
	CDlgNebular(b3_u32 class_type, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNebular)
	enum { IDD = IDD_NEBULAR };
	CB3ShowRaytrace	m_NebularPreviewCtrl;
	CB3FloatEdit	m_NebularDistanceCtrl;
	CB3ColorFieldSelector	m_NebularColorCtrl;
	BOOL	m_ActNebular;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNebular)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNebular)
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnNebularColorChange();
	afx_msg void OnActNebular();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEBULAR_H__5BDBF7FB_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
