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

#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties dialog

class CDlgProperties : public CDialog
{
	CAppLinesApp *m_App;
// Construction
public:
	CDlgProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProperties)
	enum { IDD = IDD_PROPERTIES };
	CSliderCtrl	m_RowRefreshCtrl;
	CB3IntSpinButtonCtrl	m_PrtBufferCtrl;
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
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTIES_H__719C6D93_1C7A_11D6_9D05_080020FDDE74__INCLUDED_)
