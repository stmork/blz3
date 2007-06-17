/*
**
**	$Filename:	DlgBumpGroove.h $
**	$Release:	Dortmund 2004 $
**	$Revision: 1431 $
**	$Date: 2006-05-27 15:32:22 +0200 (Sa, 27 Mai 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Groovy surface
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPGROOVE_H__77232104_7FEB_42A6_9D59_B9D3A1A77354__INCLUDED_)
#define AFX_DLGBUMPGROOVE_H__77232104_7FEB_42A6_9D59_B9D3A1A77354__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpGroove.h : header file
//

#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageBump.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpGroove dialog

class CDlgBumpGroove : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_BumpScene;
	b3Base<b3Item>    *m_BumpHead;
	b3BumpGroove      *m_Bump;

	CPageBump          m_PageBump;
	CPageScaling       m_PageScaling;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgBumpGroove(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpGroove();

// Dialog Data
	//{{AFX_DATA(CDlgBumpGroove)
	enum { IDD = IDD_BUMP_GROOVE };
	CB3ShowRaytrace	m_PreviewBumpCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpGroove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpGroove)
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

#endif // !defined(AFX_DLGBUMPGROOVE_H__77232104_7FEB_42A6_9D59_B9D3A1A77354__INCLUDED_)
