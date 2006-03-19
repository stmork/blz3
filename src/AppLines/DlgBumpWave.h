/*
**
**	$Filename:	DlgBumpWave.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wave surface
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPWAVE_H__25D71774_9F55_4FD1_8919_D5B2266F5FFE__INCLUDED_)
#define AFX_DLGBUMPWAVE_H__25D71774_9F55_4FD1_8919_D5B2266F5FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpWave.h : header file
//

#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageBump.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWave dialog

class CDlgBumpWave : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_BumpScene;
	b3Base<b3Item>    *m_BumpHead;
	b3BumpWave        *m_Bump;

	CPageBump          m_PageBump;
	CPageScaling       m_PageScaling;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgBumpWave(b3Item *item,CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpWave();

// Dialog Data
	//{{AFX_DATA(CDlgBumpWave)
	enum { IDD = IDD_BUMP_WAVE };
	CB3ShowRaytrace	m_PreviewBumpCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpWave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpWave)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPWAVE_H__25D71774_9F55_4FD1_8919_D5B2266F5FFE__INCLUDED_)
