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

#include "blz3/system/b3TreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHierarchy dialog

class CDlgHierarchy : public CB3Dialogbar, public b3Mem
{
	CAppRenderDoc *m_pDoc;
	CImageList     m_ImageList;
	HTREEITEM      m_DragItem,m_DropItem;
// Construction
public:
	 ~CDlgHierarchy();
	CDlgHierarchy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHierarchy)
	enum { IDD = IDD_HIERARCHY };
	CB3TreeCtrl	m_Hierarchy;
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
	afx_msg void OnBeginlabeleditHierarchy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	long           b3ComputeImgNum(b3BBox *BBox);
	void           b3AddBBoxes(HTREEITEM parent,b3BBox *BBox);
	b3_bool        b3InitTree(CAppRenderDoc *pDoc,b3_bool force_refresh = false);
	void           b3UpdateActivation();
	b3BBox        *b3GetSelectedBBox();
	b3_bool        b3FindBBox(b3Base<b3Item> *base,b3BBox *search);
	void           b3SelectBBox(b3BBox *BBox);
	void           b3GetData();
	void           b3SetData();

private:
	b3_count       b3Traverse (HTREEITEM parent);
	HTREEITEM      b3FindBBox (HTREEITEM parent,b3BBox *BBox);
	void           b3UpdateIcons(HTREEITEM parent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHIERARCHY_H__CCE9F5A2_AE95_11D1_A42D_0080AD1927F8__INCLUDED_)
