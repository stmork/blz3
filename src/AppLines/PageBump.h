/*
**
**	$Filename:	PageBump.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden bump properties
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_PAGEBUMP_H__04883F10_C2CB_4461_B2B4_8EFEE15BFC97__INCLUDED_)
#define AFX_PAGEBUMP_H__04883F10_C2CB_4461_B2B4_8EFEE15BFC97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageBump.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPageBump dialog

class CPageBump : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageBump)

// Construction
public:
	b3BumpWooden *m_Bump;

	CPageBump();
	~CPageBump();

// Dialog Data
	//{{AFX_DATA(CPageBump)
	enum { IDD = IDD_PAGE_BUMP };
	CB3FloatSpinButtonCtrl	m_AmplitudeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageBump)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageBump)
	afx_msg void OnSurfaceEdit();
	afx_msg void OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEBUMP_H__04883F10_C2CB_4461_B2B4_8EFEE15BFC97__INCLUDED_)
