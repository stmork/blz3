/*
**
**	$Filename:	DlgMatWood.h $
**	$Release:	Dortmund 2004 $
**	$Revision: 1492 $
**	$Date: 2007-06-07 11:33:07 +0200 (Do, 07 Jun 2007) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview and property sheet
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
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowSample.h"
#include "PageMaterial.h"
#include "PageWood.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWood dialog

class CDlgMatWood : public CB3SimplePropertyPreviewDialog
{
	b3MatWood         *m_Material;
	b3BBox            *m_BBox;
	b3MaterialSampler *m_MatSampler;

	CPageMaterial      m_PageDark;
	CPageMaterial      m_PageLight;
	CPageWood          m_PageWood;
	CPageScaling       m_PageScaling;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatWood(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATWOOD_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
