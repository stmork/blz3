/*
**
**	$Filename:	DlgMatMarble.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_)
#define AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatMarble.h : header file
//

#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageMaterial.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatMarble dialog

class CDlgMatMarble : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatMarble       *m_Material;

	CPageMaterial      m_PageDark;
	CPageMaterial      m_PageLight;
	CPageScaling       m_PageScaling;
// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatMarble(b3Item *item, CAppObjectDoc *pDoc,CWnd* pParent = NULL);   // standard constructor
	~CDlgMatMarble();

// Dialog Data
	//{{AFX_DATA(CDlgMatMarble)
	enum { IDD = IDD_MAT_MARBLE };
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatMarble)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatMarble)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_)
