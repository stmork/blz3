/*
**
**	$Filename:	DlgAction.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Acting with active objects
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGACTION_H__FDF44B84_040C_11D6_9D01_080020FDDE74__INCLUDED_)
#define AFX_DLGACTION_H__FDF44B84_040C_11D6_9D01_080020FDDE74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAction.h : header file
//

#include "blz3/system/b3Toolbar.h"
#include "b3ImageButton.h"
#include "AppLinesView.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAction dialog

class CDlgAction : public CB3Dialogbar
{
// Construction
public:
	CAppLinesView *m_pView;
	CDlgAction(CWnd* pParent = NULL);   // standard constructor
	void b3SetData();
	void b3GetData();

// Dialog Data
	//{{AFX_DATA(CDlgAction)
	enum { IDD = IDD_ACTION };
	CB3ImageButton	m_CtrlMoveLeft;
	CB3ImageButton	m_CtrlMoveRight;
	CB3ImageButton	m_CtrlMoveUp;
	CB3ImageButton	m_CtrlMoveDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAction)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACTION_H__FDF44B84_040C_11D6_9D01_080020FDDE74__INCLUDED_)
