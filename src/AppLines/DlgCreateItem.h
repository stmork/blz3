/*
**
**	$Filename:	DlgCreateItem.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1438 $
**	$Date: 2006-06-01 16:17:24 +0200 (Do, 01 Jun 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Adding a new name for a new item.
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCREATEITEM_H__8DBD88B9_E707_11D5_89FB_00E018258F1A__INCLUDED_)
#define AFX_DLGCREATEITEM_H__8DBD88B9_E707_11D5_89FB_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateItem.h : header file
//

#include "blz3/system/b3Dialog.h"
#include "blz3/system/b3TextEdit.h"
#include "blz3/base/b3World.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateItem dialog

class CDlgCreateItem : public CB3Dialog
{
	CString         m_NewName;
// Construction
public:
	b3Base<b3Item> *m_ItemBase;
	b3_u32          m_ClassType;
	CString         m_Suggest;
	b3_count        m_MaxNameLen;
	b3_bool         m_NoNameCheck;
	CDlgCreateItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCreateItem)
	enum { IDD = IDD_NEW_ITEM };
	CString	m_Label;
	CB3TextEdit	m_NewNameCtrl;
	//}}AFX_DATA

	void b3GetText(char *buffer);
	const char *b3GetText();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateItem)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void    b3PreInitDialog();
	void    b3PostInitDialog();
	void    b3UpdateUI();
	b3_bool b3IsNameOK(const char *suggest);
	void    b3Suggest();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEITEM_H__8DBD88B9_E707_11D5_89FB_00E018258F1A__INCLUDED_)
