/*
**
**	$Filename:	DlgItemCreate.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Creating an item
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGITEMCREATE_H__464280EB_8091_45AF_B7AD_AB38E4D22BE7__INCLUDED_)
#define AFX_DLGITEMCREATE_H__464280EB_8091_45AF_B7AD_AB38E4D22BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgItemCreate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgItemCreate dialog

class CDlgItemCreate : public CDialog
{
	CImageList m_ImageList;
	b3_u32 m_ClassId;
// Construction
public:
	b3Item * m_Item;
	static b3Item * b3Create(b3_u32 class_id);
	CDlgItemCreate(b3_u32 class_id,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgItemCreate)
	enum { IDD = IDD_ITEM_CREATE };
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgItemCreate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgItemCreate)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkItemlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGITEMCREATE_H__464280EB_8091_45AF_B7AD_AB38E4D22BE7__INCLUDED_)
