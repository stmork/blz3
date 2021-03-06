/*
**
**	$Filename:	b3SimplePreviewDialog.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview sheet
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_SIMPLEPREVIEWDIALOG_H
#define B3_SYSTEM_SIMPLEPREVIEWDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3SimplePropertyPreviewDialog.h : header file
//

#include "blz3/base/b3World.h"

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePreviewDialog dialog

class B3_PLUGIN CB3SimplePreviewDialog : public CDialog
{
	b3Item      *     m_Item;
	b3_u32            m_Checksum;
	CString           m_RegKeyAutoRefresh;

protected:
	const static int  m_RefreshId;
	const static int  m_AutoRefreshId;
	const static int  m_PropertySheetId;

	// Construction
public:
	DECLARE_DYNAMIC(CB3SimplePreviewDialog)
	CB3SimplePreviewDialog(b3Item * item, int dlgId, CWnd * pParent = NULL); // standard constructor
	~CB3SimplePreviewDialog();

	// Dialog Data
	//{{AFX_DATA(CB3SimplePreviewDialog)
	BOOL	m_AutoRefresh;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3SimplePreviewDialog)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CB3SimplePreviewDialog)
	afx_msg LRESULT OnPreview(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAutoRefresh();
	afx_msg void OnRefresh();
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnSpin(NMHDR * pNMHDR, LRESULT * pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void b3PreInitDialog() = 0;
	virtual void b3PostInitDialog() = 0;
	virtual void b3UpdateUI() = 0;
	void b3Preview();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(B3_SYSTEM_SIMPLEPREVIEWDIALOG_H)
