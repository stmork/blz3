/*
**
**	$Filename:	DlgSpinTest.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 304 $
**	$Date: 2002-03-08 17:46:15 +0100 (Fr, 08 Mrz 2002) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for testing spin button cotrols
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSPINTEST_H__3AD75C53_5819_47F8_863F_81CF1B3F8D8B__INCLUDED_)
#define AFX_DLGSPINTEST_H__3AD75C53_5819_47F8_863F_81CF1B3F8D8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSpinTest.h : header file
//

#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSpinTest dialog

class CDlgSpinTest : public CDialog
{
	b3_f64 m_Float;
	b3_s32 m_Int;
	b3_f32 m_Min;
	b3_f64 m_Max;
// Construction
public:
	CDlgSpinTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSpinTest)
	enum { IDD = IDD_SPINTEST };
	CB3FloatEdit	m_FloatMinCtrl;
	CB3FloatEdit	m_FloatMaxCtrl;
	CB3FloatSpinButtonCtrl	m_FloatCtrl;
	CB3IntSpinButtonCtrl	m_IntCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSpinTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSpinTest)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3Validate();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSPINTEST_H__3AD75C53_5819_47F8_863F_81CF1B3F8D8B__INCLUDED_)
