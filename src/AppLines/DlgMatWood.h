/*
**
**	$Filename:	DlgMatWood.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood material
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATWOOD_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
#define AFX_DLGMATWOOD_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatWood.h : header file
//

#include "blz3/raytrace/b3MaterialSampler.h"
#include "b3ShowSample.h"
#include "PageMaterial.h"
#include "PageWood.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWood dialog

class CDlgMatWood : public CDialog
{
	b3MatWood         *m_Material;
	b3MaterialSampler *m_MatSampler;

	CPropertySheet     m_PropertySheet;
	CPageMaterial      m_PageMaterial;
	CPageWood          m_PageWood;
// Construction
public:
	static b3_bool b3Edit(b3Item *item);
	static void b3Register();
	CDlgMatWood(b3Item *item,CWnd* pParent = NULL);   // standard constructor
	~CDlgMatWood();

// Dialog Data
	//{{AFX_DATA(CDlgMatWood)
	enum { IDD = IDD_MAT_WOOD };
	CB3ShowSample	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatWood)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatWood)
	virtual BOOL OnInitDialog();
	afx_msg void OnPreviewMaterial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATWOOD_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
