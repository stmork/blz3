/*
**
**	$Filename:	DlgFulcrum.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setting angle steps
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSTEPROTATE_H__7B00167F_F853_11D5_8A0C_00E018258F1A__INCLUDED_)
#define AFX_DLGSTEPROTATE_H__7B00167F_F853_11D5_8A0C_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStepRotate.h : header file
//

#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3ImageButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgStepRotate dialog

class CAppRenderDoc;

class CDlgStepRotate : public CB3Dialogbar
{
	// Construction
public:
	CAppRenderDoc * m_pDoc;
	CDlgStepRotate(CWnd * pParent = NULL);  // standard constructor
	void b3SetData();
	void b3GetData();

	// Dialog Data
	//{{AFX_DATA(CDlgStepRotate)
	enum { IDD = IDD_STEP_ROTATE };
	CB3FloatEdit	m_zCtrl;
	CB3FloatEdit	m_yCtrl;
	CB3FloatEdit	m_xCtrl;
	CB3ImageButton	m_CtrlRotLeft;
	CB3ImageButton	m_CtrlRotRight;
	//}}AFX_DATA

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStepRotate)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStepRotate)
	afx_msg void OnKillfocusStep();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTEPROTATE_H__7B00167F_F853_11D5_8A0C_00E018258F1A__INCLUDED_)
