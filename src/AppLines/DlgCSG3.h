/*
**
**	$Filename:	DlgCSG3.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 761 $
**	$Date: 2004-05-29 15:38:11 +0200 (Sa, 29 Mai 2004) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for quadric CSG shapes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCSG3_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
#define AFX_DLGCSG3_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShape3.h : header file
//

#include "DlgCSG1.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 dialog

class CDlgCSG3 : public CDlgCSG1
{
// Construction
protected:
	CB3DirGroup m_Dir2;
	CB3DirGroup m_Dir3;

public:
	CDlgCSG3(CWnd* pParent = NULL);   // standard constructor

	static  int  b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create=true);
	virtual void b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CDlgCSG3)
	enum { IDD = IDD_SHAPE3 };
	CB3FloatEdit	m_xDir2Ctrl;
	CB3FloatEdit	m_yDir2Ctrl;
	CB3FloatEdit	m_zDir2Ctrl;
	CB3FloatEdit	m_lenDir2Ctrl;
	CB3FloatEdit	m_xDir3Ctrl;
	CB3FloatEdit	m_yDir3Ctrl;
	CB3FloatEdit	m_zDir3Ctrl;
	CB3FloatEdit	m_lenDir3Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSG3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();
	virtual void        b3SetDirMode(int dirmode);
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CDlgCSG3)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedDir2();
	afx_msg void OnChangedLen2();
	afx_msg void OnChangedDir3();
	afx_msg void OnChangedLen3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSG3_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
