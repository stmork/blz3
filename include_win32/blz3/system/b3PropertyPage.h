/*
**
**	$Filename:	b3PropertyPage.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for property page dialog
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3PROPERTYPAGE_H__79B214D2_A224_4126_977F_4DE56CF0AFF7__INCLUDED_)
#define AFX_B3PROPERTYPAGE_H__79B214D2_A224_4126_977F_4DE56CF0AFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// B3PropertyPage.h : header file
//

#include "blz3/b3Config.h"

/////////////////////////////////////////////////////////////////////////////
// CB3PropertyPage dialog

class CB3PropertyPage : public CPropertyPage
{
	CString m_Caption;
// Construction
public:
	CB3PropertyPage(UINT nIDTemplate);
	~CB3PropertyPage();

// Dialog Data
	//{{AFX_DATA(CB3PropertyPage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CB3PropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CB3PropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void b3UpdateUI();

public:
	static void b3InitPropertySheet(CDialog *dlg,CPropertySheet &sheet, int id);
	void b3SetCaption(int id);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3PROPERTYPAGE_H__79B214D2_A224_4126_977F_4DE56CF0AFF7__INCLUDED_)
