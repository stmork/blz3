/*
**
**	$Filename:	b3SimplePropertyPreviewDialog.h $
**	$Release:	Dortmund 2004 $
**	$Revision: 1435 $
**	$Date: 2006-05-29 17:13:45 +0200 (Mo, 29 Mai 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview and property sheet
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_SIMPLEPROPERTYPREVIEWDIALOG_H
#define B3_SYSTEM_SIMPLEPROPERTYPREVIEWDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3SimplePropertyPreviewDialog.h : header file
//

#include "blz3/b3Config.h"
#include "blz3/system/b3PropertyPage.h"
#include "b3SimplePreviewDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePropertyPreviewDialog dialog

class B3_PLUGIN CB3SimplePropertyPreviewDialog : public CB3SimplePreviewDialog
{
protected:
	CPropertySheet     m_PropertySheet;
// Construction
public:
	DECLARE_DYNAMIC(CB3SimplePropertyPreviewDialog)
	CB3SimplePropertyPreviewDialog(b3Item *item,int dlgId,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CB3SimplePropertyPreviewDialog)
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
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(B3_SYSTEM_SIMPLEPROPERTYPREVIEWDIALOG_H)
