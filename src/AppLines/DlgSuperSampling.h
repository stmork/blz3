/*
**
**	$Filename:	DlgSuperSampling.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3SuperSampling properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSUPERSAMPLING_H__5BDBF7FC_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
#define AFX_DLGSUPERSAMPLING_H__5BDBF7FC_D6AE_11D5_89F1_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuperSampling.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSuperSampling dialog

class CDlgSuperSampling : public CPropertyPage
{
// Construction
public:
	b3SuperSample * m_SuperSample;
	CDlgSuperSampling(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuperSampling)
	enum { IDD = IDD_SUPER_SAMPLE };
	CSliderCtrl	m_RedCtrl;
	CSliderCtrl	m_GreenCtrl;
	CSliderCtrl	m_BlueCtrl;
	BOOL	m_ActSuperSampling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuperSampling)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuperSampling)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPERSAMPLING_H__5BDBF7FC_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
