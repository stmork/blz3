/*
**
**	$Filename:	DlgCSG1.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for CSG sphere
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCSG1_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
#define AFX_DLGCSG1_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShape3.h : header file
//

#include "AppLines.h"
#include "b3SpanningShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCSG1 dialog

class CDlgCSG1 : public CB3SpanningShapeDialog
{
	// Construction
protected:
	CB3PosGroup m_Base;
	CB3DirGroup m_Dir1;

public:
	CDlgCSG1(CWnd * pParent = NULL);  // standard constructor
	CDlgCSG1(UINT id, CWnd * pParent = NULL);

	static  int  b3Edit(b3_u32 shading_class_type, b3Item * item, b3_bool create = true);
	virtual void b3PostProcess();

	// Dialog Data
	//{{AFX_DATA(CDlgCSG1)
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
	//{{AFX_VIRTUAL(CDlgCSG1)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual void        b3Init();
	virtual void        b3SetDirMode(int dirmode);
	virtual void        b3UpdateBase();

	// Generated message map functions
	//{{AFX_MSG(CDlgCSG1)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedBase();
	afx_msg void OnChangedDir1();
	afx_msg void OnChangedLen1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSG1_H__87A0DE66_D2F3_4FEB_8FEC_31CD5F586D82__INCLUDED_)
