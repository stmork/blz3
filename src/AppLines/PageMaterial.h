/*
**
**	$Filename:	PageMaterial.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden material properties
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_PAGEMATERIAL_H__B6796F9B_A4EB_4490_A771_37A92523FA9C__INCLUDED_)
#define AFX_PAGEMATERIAL_H__B6796F9B_A4EB_4490_A771_37A92523FA9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMaterial.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "b3SelectColor.h"

/////////////////////////////////////////////////////////////////////////////
// CPageMaterial dialog

class CPageMaterial : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMaterial)

// Construction
public:
	b3MaterialWooden *m_Material;

	CPageMaterial();
	~CPageMaterial();

// Dialog Data
	//{{AFX_DATA(CPageMaterial)
	enum { IDD = IDD_PAGE_MATERIAL };
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_RefrValueCtrl;
	CB3FloatSpinButtonCtrl	m_HighLightCtrl;
	CB3ColorFieldSelector	m_LightCtrl;
	CB3ColorFieldSelector	m_DarkCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageMaterial)
	afx_msg void OnColorLight();
	afx_msg void OnColorDark();
	afx_msg void OnSurfaceEdit();
	afx_msg void OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMATERIAL_H__B6796F9B_A4EB_4490_A771_37A92523FA9C__INCLUDED_)
