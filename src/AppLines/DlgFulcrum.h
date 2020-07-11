/*
**
**	$Filename:	DlgFulcrum.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setting fulcrum coordinates
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGFULCRUM_H__7B00167B_F853_11D5_8A0C_00E018258F1A__INCLUDED_)
#define AFX_DLGFULCRUM_H__7B00167B_F853_11D5_8A0C_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/system/b3Toolbar.h"
#include "blz3/system/b3FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFulcrum dialog

class CDlgFulcrum : public CB3Dialogbar
{
	// Construction
public:
	CAppRenderDoc * m_pDoc;
	CDlgFulcrum(CWnd * pParent = NULL);  // standard constructor
	void b3SetData();
	void b3GetData();

	// Dialog Data
	//{{AFX_DATA(CDlgFulcrum)
	enum { IDD = IDD_FULCRUM };
	CB3FloatEdit	m_zCtrl;
	CB3FloatEdit	m_yCtrl;
	CB3FloatEdit	m_xCtrl;
	//}}AFX_DATA

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFulcrum)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFulcrum)
	afx_msg void OnKillfocusFulcrum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFULCRUM_H__7B00167B_F853_11D5_8A0C_00E018258F1A__INCLUDED_)
