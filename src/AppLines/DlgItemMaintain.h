/*
**
**	$Filename:	DlgItemMaintain.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Maintaining an item list
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGITEMMAINTAIN_H__F3A08F89_B109_4FAD_B08F_71AA565243B3__INCLUDED_)
#define AFX_DLGITEMMAINTAIN_H__F3A08F89_B109_4FAD_B08F_71AA565243B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaintain dialog

#include "blz3/system/b3Plugin.h"
#include "blz3/base/b3Hash.h"

class CDlgItemMaintain : public CDialog
{
// Construction
	b3Base<b3Item>        *m_Head;
	b3HashMap<b3_u32,int>  m_ClassTypesToImg;
	b3Loader              *m_Plugins;
	CImageList             m_ImageList;

public:
	CDlgItemMaintain(b3Base<b3Item> *head,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgItemMaintain)
	enum { IDD = IDD_ITEM_MAINTAIN };
	CListCtrl	m_ItemList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgItemMaintain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgItemMaintain)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemNew();
	afx_msg void OnItemEdit();
	afx_msg void OnItemDelete();
	afx_msg void OnItemFirst();
	afx_msg void OnItemUp();
	afx_msg void OnItemDown();
	afx_msg void OnItemLast();
	virtual void OnOK();
	afx_msg void OnDblclkItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickItemList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	b3Item *b3GetSelectedItem();
	void    b3UpdateUI();
	void    b3UpdateList(b3Item *select = null);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGITEMMAINTAIN_H__F3A08F89_B109_4FAD_B08F_71AA565243B3__INCLUDED_)
