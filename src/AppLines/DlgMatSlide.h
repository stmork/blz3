/*
**
**	$Filename:	DlgMatSlide.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Slide material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATSLIDE_H__22891A65_F807_4FF2_95F2_117276E31690__INCLUDED_)
#define AFX_DLGMATSLIDE_H__22891A65_F807_4FF2_95F2_117276E31690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatSlide.h : header file
//

#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageMaterial.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatSlide dialog

class CDlgMatSlide : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatSlide        *m_Material;

	CPageMaterial      m_PageLeft;
	CPageMaterial      m_PageRight;
// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatSlide(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgMatSlide();

// Dialog Data
	//{{AFX_DATA(CDlgMatSlide)
	enum { IDD = IDD_MAT_SLIDE };
	CB3FloatSpinButtonCtrl	m_FromCtrl;
	CB3FloatSpinButtonCtrl	m_ToCtrl;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	BOOL	m_CutOff;
	int		m_SlideMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatSlide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatSlide)
	afx_msg void OnSlideMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATSLIDE_H__22891A65_F807_4FF2_95F2_117276E31690__INCLUDED_)
