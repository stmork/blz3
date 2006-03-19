/*
**
**	$Filename:	DlgMatGranite.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Granite material
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATGRANITE_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
#define AFX_DLGMATGRANITE_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatGranite.h : header file
//

#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageMaterial.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatGranite dialog

class CDlgMatGranite : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatGranite      *m_Material;

	CPageMaterial      m_PageDark;
	CPageMaterial      m_PageLight;
	CPageScaling       m_PageScaling;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatGranite(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgMatGranite();

// Dialog Data
	//{{AFX_DATA(CDlgMatGranite)
	enum { IDD = IDD_MAT_GRANITE };
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatGranite)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatGranite)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATGRANITE_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
