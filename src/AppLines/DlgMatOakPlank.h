/*
**
**	$Filename:	DlgMatOakPlank.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden oak plank material
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATOAKPLANK_H__88D2E295_7DA8_4731_88DB_1DAB45D7C601__INCLUDED_)
#define AFX_DLGMATOAKPLANK_H__88D2E295_7DA8_4731_88DB_1DAB45D7C601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatOakPlank.h : header file
//

#include "blz3/raytrace/b3MaterialSampler.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowSample.h"
#include "PageMaterial.h"
#include "PageWood.h"
#include "PageOakPlank.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatOakPlank dialog

class CDlgMatOakPlank : public CB3SimplePropertyPreviewDialog
{
	b3MatOakPlank   *  m_Material;
	b3BBox      *      m_BBox;
	b3MaterialSampler * m_MatSampler;

	CPageMaterial      m_PageDark;
	CPageMaterial      m_PageLight;
	CPageWood          m_PageWood;
	CPageOakPlank      m_PageOakPlank;
	CPageScaling       m_PageScaling;

	// Construction
public:
	static b3_bool b3Edit(b3Item * item, void * ptr);
	static void b3Register();
	CDlgMatOakPlank(b3Item * item, CAppObjectDoc * pDoc, CWnd * pParent = NULL); // standard constructor
	~CDlgMatOakPlank();

	// Dialog Data
	//{{AFX_DATA(CDlgMatOakPlank)
	enum { IDD = IDD_MAT_OAKPLANK };
	CB3ShowSample	m_PreviewMaterialCtrl;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatOakPlank)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatOakPlank)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATOAKPLANK_H__88D2E295_7DA8_4731_88DB_1DAB45D7C601__INCLUDED_)
