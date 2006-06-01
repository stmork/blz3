/*
**
**	$Filename:	DlgModellerInfo.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for defining geometry wide values
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMODELLERINFO_H__9C7BB013_E23D_11D5_9CFA_080020FDDE74__INCLUDED_)
#define AFX_DLGMODELLERINFO_H__9C7BB013_E23D_11D5_9CFA_080020FDDE74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModellerInfo.h : header file
//

#include "blz3/raytrace/b3Special.h"
#include "blz3/system/b3Checkbox.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3VectorEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgModellerInfo dialog

class CDlgModellerInfo : public CDialog
{
	CB3PosGroup     m_CenterCtrl;
// Construction
public:
	b3ModellerInfo *m_ModellerInfo;
	CDlgModellerInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModellerInfo)
	enum { IDD = IDD_MODELLER_INFO };
	CB3FloatEdit	m_SnapToGridCtrl;
	CB3FloatEdit	m_SnapToAngleCtrl;
	CB3FloatEdit	m_zFulcrumCtrl;
	CB3FloatEdit	m_yFulcrumCtrl;
	CB3FloatEdit	m_xFulcrumCtrl;
	CB3Checkbox		m_SnapToAngle;
	CB3Checkbox		m_SnapToGrid;
	int				m_Unit;
	int				m_Measure;
	b3_u32			m_CustomMeasure;
	int		m_LightMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModellerInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModellerInfo)
	afx_msg void OnSnap();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnFulcrumClear();
	afx_msg void OnSelchangeMeasure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODELLERINFO_H__9C7BB013_E23D_11D5_9CFA_080020FDDE74__INCLUDED_)
