/*
**
**	$Filename:	DlgAnimation.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Animation properties
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGANIMATION_H__CEB54CED_A204_4286_ACA1_97FCA458BA1C__INCLUDED_)
#define AFX_DLGANIMATION_H__CEB54CED_A204_4286_ACA1_97FCA458BA1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAnimation.h : header file
//

#include "blz3/raytrace/b3Animation.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAnimation dialog

class CDlgAnimation : public CDialog
{
	b3Animation *m_Animation;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgAnimation(b3Item *item, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAnimation)
	enum { IDD = IDD_ANIMATION };
	BOOL	m_Enable;
	CB3IntSpinButtonCtrl	m_CtrlFPS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnimation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnimation)
	afx_msg void OnAnimOn();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANIMATION_H__CEB54CED_A204_4286_ACA1_97FCA458BA1C__INCLUDED_)
