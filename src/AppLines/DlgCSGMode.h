/*
**
**	$Filename:	DlgCSGMode.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Change CSG operation mode
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCSGMODE_H__63A07A7D_8E2C_42FE_99D9_A60C5C1D606B__INCLUDED_)
#define AFX_DLGCSGMODE_H__63A07A7D_8E2C_42FE_99D9_A60C5C1D606B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCSGMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCSGMode dialog

class CDlgCSGMode : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgCSGMode)

// Construction
public:
	b3CSGShape *m_Shape;
	b3_bool     m_Creation;
	const char *m_Section;

public:
	      CDlgCSGMode();
	     ~CDlgCSGMode();
	void  b3PostProcess();
	void  b3Init();

// Dialog Data
	//{{AFX_DATA(CDlgCSGMode)
	enum { IDD = IDD_CSGMODE };
	int     m_CSGMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSGMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgCSGMode)
	afx_msg void OnCSGModeChanged();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSGMODE_H__63A07A7D_8E2C_42FE_99D9_A60C5C1D606B__INCLUDED_)
