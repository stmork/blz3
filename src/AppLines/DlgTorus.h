/*
**
**	$Filename:	DlgTorus.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for torus
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGTORUS_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
#define AFX_DLGTORUS_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShape3.h : header file
//

#include "DlgShape1.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 dialog

class CDlgTorus : public CDlgShape1
{
	b3_vector m_TorusDir1;
	b3_vector m_TorusDir2;
	b3_vector m_TorusDir3;

	// Construction
protected:
	CB3DirGroup m_Dir2;
	CB3DirGroup m_Dir3;

public:
	CDlgTorus(CWnd* pParent = NULL);   // standard constructor

	static  int  b3Edit(b3Item *item,b3_bool create=true);
	virtual void b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CDlgTorus)
	enum { IDD = IDD_TORUS };
	CB3FloatEdit	m_xDir2Ctrl;
	CB3FloatEdit	m_yDir2Ctrl;
	CB3FloatEdit	m_zDir2Ctrl;
	CB3FloatEdit	m_xDir3Ctrl;
	CB3FloatEdit	m_yDir3Ctrl;
	CB3FloatEdit	m_zDir3Ctrl;
	CB3FloatEdit	m_lenDir3Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTorus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();
	virtual void        b3SetDirMode(int dirmode);
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CDlgTorus)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedDir1();
	afx_msg void OnChangedDir2();
	afx_msg void OnChangedLen2();
	afx_msg void OnChangedDir3();
	afx_msg void OnChangedLen3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTORUS_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
