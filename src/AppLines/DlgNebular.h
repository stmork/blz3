/*
**
**	$Filename:	DlgNebular.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3ColorField.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgNebular dialog

class CDlgNebular : public CPropertyPage
{
// Construction
public:
	b3Nebular * m_Nebular;
	CDlgNebular(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNebular)
	enum { IDD = IDD_NEBULAR };
	CB3ColorField	m_NebColorCtrl;
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
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEBULAR_H__5BDBF7FB_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
