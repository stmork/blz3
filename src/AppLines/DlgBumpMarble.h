/*
**
**	$Filename:	DlgBumpMarble.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble bump mapping
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPMARBLE_H__A29ED4D5_4F4E_48EC_ACAA_5C7141EC66D9__INCLUDED_)
#define AFX_DLGBUMPMARBLE_H__A29ED4D5_4F4E_48EC_ACAA_5C7141EC66D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpMarble.h : header file
//

#include "b3SimplePropertyPreviewDialog.h"
#include "b3ShowRaytrace.h"
#include "PageBump.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpMarble dialog

class CDlgBumpMarble : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_BumpScene;
	b3Base<b3Item>    *m_BumpHead;
	b3BumpMarble      *m_Bump;

	CPageBump          m_PageBump;
	CPageScaling       m_PageScaling;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgBumpMarble(b3Item *item,CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpMarble();

// Dialog Data
	//{{AFX_DATA(CDlgBumpMarble)
	enum { IDD = IDD_BUMP_MARBLE };
	CB3ShowRaytrace	m_PreviewBumpCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpMarble)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpMarble)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPMARBLE_H__A29ED4D5_4F4E_48EC_ACAA_5C7141EC66D9__INCLUDED_)
