/*
**
**	$Filename:	DlgNewObject.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting a new shape or object
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGNEWOBJECT_H__62E90B57_8136_4919_9DCC_FA4D5D88EB8F__INCLUDED_)
#define AFX_DLGNEWOBJECT_H__62E90B57_8136_4919_9DCC_FA4D5D88EB8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewObject.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"
#include "b3ImageList.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgNewObject dialog

class CDlgNewObject : public CDialog
{
// Construction
public:
	b3Item         *m_InsertAfter;
	b3BBox         *m_BBox;
	b3Item         *m_NewItem;
	b3Base<b3Item> *m_Base;
	CDlgNewObject(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewObject)
	enum { IDD = IDD_NEW_OBJECT };
	CListCtrl	m_ListCtrl;
	int		m_ModeCSG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewObject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewObject)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnObjectChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void         b3UpdateUI();
	b3_u32       b3GetSelectedClassType();
	CB3ImageList m_ImageList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWOBJECT_H__62E90B57_8136_4919_9DCC_FA4D5D88EB8F__INCLUDED_)
