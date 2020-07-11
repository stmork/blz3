/*
**
**	$Filename:	DlgDistributed.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Distributed properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGDISTRIBUTED_H__5BDBF7F7_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
#define AFX_DLGDISTRIBUTED_H__5BDBF7F7_D6AE_11D5_89F1_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDistributed.h : header file
//

#include "blz3/raytrace/b3Special.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDistributed dialog

class CDlgDistributed : public CPropertyPage
{
	// Construction
public:
	b3Distribute * m_Distributed;
	CDlgDistributed(CWnd * pParent = NULL);  // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgDistributed)
	enum { IDD = IDD_DISTRIBUTED };
	CB3IntSpinButtonCtrl	m_CtrlSPP;
	CB3IntSpinButtonCtrl	m_CtrlSPF;
	BOOL	m_ActDistributed;
	BOOL	m_ActMotionBlur;
	int		m_PixelFilter;
	int		m_TimeFilter;
	int		m_SamplingMethod;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDistributed)
public:
	virtual BOOL PreTranslateMessage(MSG * pMsg);
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDistributed)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnActivation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISTRIBUTED_H__5BDBF7F7_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
