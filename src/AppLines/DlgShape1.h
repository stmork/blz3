/*
**
**	$Filename:	DlgShape1.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for spheres
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSHAPE1_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
#define AFX_DLGSHAPE1_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShape3.h : header file
//

#include "AppLines.h"
#include "b3ShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 dialog

class CDlgShape1 : public CB3ShapeDialog
{
// Construction
	CB3PosGroup m_Base;
	CB3DirGroup m_Dir1;

public:
	b3Sphere *m_Shape;
	virtual void b3SetDirMode(int dirmode);
	static int b3Edit(b3Item *item,b3_bool create=true);

	CDlgShape1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShape1)
	enum { IDD = IDD_SHAPE1 };
	CB3FloatEdit	m_xBaseCtrl;
	CB3FloatEdit	m_yBaseCtrl;
	CB3FloatEdit	m_zBaseCtrl;
	CB3FloatEdit	m_xDir1Ctrl;
	CB3FloatEdit	m_yDir1Ctrl;
	CB3FloatEdit	m_zDir1Ctrl;
	CB3FloatEdit	m_lenDir1Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShape3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShape1)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangedDir1();
	afx_msg void OnChangedLen1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHAPE1_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
