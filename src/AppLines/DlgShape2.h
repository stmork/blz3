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
#include "DlgShape1.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 dialog

class CDlgShape2 : public CDlgShape1
{
// Construction
protected:
	CB3DirGroup m_Dir2;

public:
	CDlgShape2(CWnd* pParent = NULL);   // standard constructor
	CDlgShape2(UINT id,CWnd* pParent = NULL);

	static  int  b3Edit(b3Item *item,b3_bool create=true);
	virtual void b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CDlgShape2)
	enum { IDD = IDD_SHAPE2 };
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
	virtual void        b3Init();
	virtual void        b3SetDirMode(int dirmode);
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CDlgShape2)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedDir2();
	afx_msg void OnChangedLen2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHAPE2_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
