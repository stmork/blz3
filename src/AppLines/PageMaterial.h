/*
**
**	$Filename:	PageMaterial.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple material properties
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

#include "blz3/system/b3PropertyPage.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3ColorFieldSelector.h"
#include "blz3/raytrace/b3Material.h"

/////////////////////////////////////////////////////////////////////////////
// CPageMaterial dialog

class CPageMaterial : public CB3PropertyPage
{
public:
	b3_material *m_Material;

// Construction
	CPageMaterial();
	~CPageMaterial();

// Dialog Data
	//{{AFX_DATA(CPageMaterial)
	enum { IDD = IDD_PAGE_MATERIAL };
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3ColorFieldSelector	m_AmbientCtrl;
	CB3ColorFieldSelector	m_DiffuseCtrl;
	CB3ColorFieldSelector	m_SpecularCtrl;
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
	afx_msg void OnColorAmbient();
	afx_msg void OnColorDiffuse();
	afx_msg void OnColorSpecular();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMATERIAL_H__B6796F9B_A4EB_4490_A771_37A92523FA9C__INCLUDED_)
