/*
**
**	$Filename:	b3SimplePropertyPreviewDialog.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview and property sheet
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3SIMPLEPROPERTYPREVIEWDIALOG_H__1B0A466C_A989_4E39_9535_DB586E0A90A1__INCLUDED_)
#define AFX_B3SIMPLEPROPERTYPREVIEWDIALOG_H__1B0A466C_A989_4E39_9535_DB586E0A90A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3SimplePropertyPreviewDialog.h : header file
//

#include "blz3/b3Config.h"
#include "blz3/system/b3PropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePropertyPreviewDialog dialog

class CB3SimplePropertyPreviewDialog : public CDialog
{
protected:
	CPropertySheet     m_PropertySheet;
// Construction
public:
	CB3SimplePropertyPreviewDialog(int dlgId,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CB3SimplePropertyPreviewDialog)
	BOOL	m_AutoRefresh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3SimplePropertyPreviewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CB3SimplePropertyPreviewDialog)
	afx_msg void OnPreview(WPARAM wParam,LPARAM lParam);
	afx_msg void OnAutoRefresh();
	afx_msg void OnRefresh();
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void b3InitDialog() = 0;
	virtual void b3UpdateUI() = 0;
	        void b3Preview();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3SIMPLEPROPERTYPREVIEWDIALOG_H__1B0A466C_A989_4E39_9535_DB586E0A90A1__INCLUDED_)
