/*
**
**	$Filename:	DlgHierarchy.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Hierarchy activation
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGHIERARCHY_H__CCE9F5A2_AE95_11D1_A42D_0080AD1927F8__INCLUDED_)
#define AFX_DLGHIERARCHY_H__CCE9F5A2_AE95_11D1_A42D_0080AD1927F8__INCLUDED_

#include <blz3/b3Config.h>
#include "AppLinesDoc.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgHierarchy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHierarchy dialog

class CDlgHierarchy : public CB3Dialogbar, public b3Mem
{
// Construction
public:
	b3BBox *b3GetSelectedBBox();
	void    b3GetData();
	void    b3SetData();
	CAppLinesDoc *m_pDoc;
	 ~CDlgHierarchy();
	CDlgHierarchy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHierarchy)
	enum { IDD = IDD_HIERARCHY };
	CTreeCtrl	m_Hierarchy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHierarchy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHierarchy)
	virtual BOOL OnInitDialog();
	afx_msg void OnEndLabelEditHierarchy(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HTREEITEM  *m_TreeItems;
	CImageList  m_ImageList;
	b3Scene    *m_Scene;
	void        b3InitTree ();
	void        b3AddBBoxes (b3BBox *bbox,HTREEITEM);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHIERARCHY_H__CCE9F5A2_AE95_11D1_A42D_0080AD1927F8__INCLUDED_)
