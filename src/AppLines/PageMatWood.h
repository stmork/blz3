/*
**
**	$Filename:	PageMatWood.h $
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

#if !defined(AFX_PAGEMATWOOD_H__3817BE68_81D8_4265_BC3D_EA067BEC34B0__INCLUDED_)
#define AFX_PAGEMATWOOD_H__3817BE68_81D8_4265_BC3D_EA067BEC34B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMatWood.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "b3SelectColor.h"

/////////////////////////////////////////////////////////////////////////////
// CPageMatWood dialog

class CPageMatWood : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMatWood)
public:
	b3MaterialWooden *m_Material;

// Construction
public:
	CPageMatWood();
	~CPageMatWood();

// Dialog Data
	//{{AFX_DATA(CPageMatWood)
	enum { IDD = IDD_PAGE_MAT_WOOD };
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3ColorFieldSelector	m_LightCtrl;
	CB3ColorFieldSelector	m_DarkCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMatWood)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMatWood)
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

#endif // !defined(AFX_PAGEMATWOOD_H__3817BE68_81D8_4265_BC3D_EA067BEC34B0__INCLUDED_)
