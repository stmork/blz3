/*
**
**	$Filename:	b3CSGDialog.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for CSG shapes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3CSGDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_)
#define AFX_B3CSGDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// b3ShapeDialog.h : header file
//

#include "b3ShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CB3CSGDialog dialog

class CB3CSGDialog : public CB3ShapeDialog
{
// Construction
public:
	CB3CSGDialog(UINT IDD,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CB3CSGDialog)
	int     m_CSGMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3CSGDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3SetCSGMode(int csgmode);
	virtual const char *b3GetSection();
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CB3CSGDialog)
	afx_msg void OnCSGModeChanged();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3CSGDIALOG_H__54EA059D_FC92_40E7_805E_0EC2FF5E5BDB__INCLUDED_)
