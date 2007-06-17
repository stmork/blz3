/*
**
**	$Filename:	DlgBumpWater.h $
**	$Release:	Dortmund 2004 $
**	$Revision: 1431 $
**	$Date: 2006-05-27 15:32:22 +0200 (Sa, 27 Mai 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Water surface
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPWATER_H__2AF3F99E_1977_4C43_95C1_6A6C92D83ADF__INCLUDED_)
#define AFX_DLGBUMPWATER_H__2AF3F99E_1977_4C43_95C1_6A6C92D83ADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpWater.h : header file
//

#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageBump.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWater dialog

class CDlgBumpWater : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_BumpScene;
	b3Base<b3Item>    *m_BumpHead;
	b3BumpWater       *m_Bump;

	CPageBump          m_PageBump;
	CPageScaling       m_PageScaling;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgBumpWater(b3Item *item,CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpWater();

// Dialog Data
	//{{AFX_DATA(CDlgBumpWater)
	enum { IDD = IDD_BUMP_WATER };
	CB3FloatSpinButtonCtrl	m_WindAmpCtrl;
	CB3FloatSpinButtonCtrl	m_WindFreqCtrl;
	CB3FloatSpinButtonCtrl	m_MinWindCtrl;
	CB3FloatSpinButtonCtrl	m_ScaleTimeCtrl;
	CB3FloatSpinButtonCtrl	m_KmCtrl;
	CB3ShowRaytrace	m_PreviewBumpCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpWater)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpWater)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPWATER_H__2AF3F99E_1977_4C43_95C1_6A6C92D83ADF__INCLUDED_)
