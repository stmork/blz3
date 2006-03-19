/*
**
**	$Filename:	DlgBumpGlossy.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Glossy surface
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPGLOSSY_H__8547D519_EE38_4356_A8CC_E81186C28425__INCLUDED_)
#define AFX_DLGBUMPGLOSSY_H__8547D519_EE38_4356_A8CC_E81186C28425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpGlossy.h : header file
//

#include "blz3/raytrace/b3Bump.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3SimplePreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpGlossy dialog

class CDlgBumpGlossy : public CB3SimplePreviewDialog
{
	b3Scene           *m_BumpScene;
	b3Base<b3Item>    *m_BumpHead;
	b3BumpGlossy      *m_Bump;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgBumpGlossy(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpGlossy();

// Dialog Data
	//{{AFX_DATA(CDlgBumpGlossy)
	enum { IDD = IDD_BUMP_GLOSSY };
	CB3ShowRaytrace	m_PreviewBumpCtrl;
	CB3FloatSpinButtonCtrl	m_AmplitudeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpGlossy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpGlossy)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPGLOSSY_H__8547D519_EE38_4356_A8CC_E81186C28425__INCLUDED_)
