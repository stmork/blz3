/*
**
**	$Filename:	DlgShape2.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for area or disk
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSHAPE2_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
#define AFX_DLGSHAPE2_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShape3.h : header file
//

#include "AppLines.h"
#include "b3ShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 dialog

class CDlgShape2 : public CB3ShapeDialog
{
// Construction
	CB3PosGroup m_Base;
	CB3DirGroup m_Dir1;
	CB3DirGroup m_Dir2;

public:
	b3Shape2 *m_Shape;
	virtual void b3SetDirMode(int dirmode);
	static int b3Edit(b3Item *item,b3_bool create=true);

	CDlgShape2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShape2)
	enum { IDD = IDD_SHAPE2 };
	CB3FloatEdit	m_xBaseCtrl;
	CB3FloatEdit	m_yBaseCtrl;
	CB3FloatEdit	m_zBaseCtrl;
	CB3FloatEdit	m_xDir1Ctrl;
	CB3FloatEdit	m_yDir1Ctrl;
	CB3FloatEdit	m_zDir1Ctrl;
	CB3FloatEdit	m_lenDir1Ctrl;
	CB3FloatEdit	m_xDir2Ctrl;
	CB3FloatEdit	m_yDir2Ctrl;
	CB3FloatEdit	m_zDir2Ctrl;
	CB3FloatEdit	m_lenDir2Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShape2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShape2)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangedDir1();
	afx_msg void OnChangedLen1();
	afx_msg void OnChangedDir2();
	afx_msg void OnChangedLen2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHAPE2_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
