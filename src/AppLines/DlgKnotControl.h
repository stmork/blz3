/*
**
**	$Filename:	DlgKnotControl.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - BSpline knot control
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGKNOTCONTROL_H__06395644_15CD_48DB_A1DD_9476A450BDB1__INCLUDED_)
#define AFX_DLGKNOTCONTROL_H__06395644_15CD_48DB_A1DD_9476A450BDB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnotControl.h : header file
//

#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgKnotControl dialog

class CDlgKnotControl : public CDialog
{
	b3Spline  *m_Spline;
	b3_index   m_KnotNum;

// Construction
public:
	CDlgKnotControl(b3Spline *spline,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgKnotControl)
	enum { IDD = IDD_BSPLINE_CTRL };
	CSliderCtrl	m_KnotCtrl;
	CB3IntSpinButtonCtrl m_KnotNumCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgKnotControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgKnotControl)
	virtual BOOL OnInitDialog();
	afx_msg void OnKnotNumEdit();
	afx_msg void OnKnotNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3SetKnotCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNOTCONTROL_H__06395644_15CD_48DB_A1DD_9476A450BDB1__INCLUDED_)
