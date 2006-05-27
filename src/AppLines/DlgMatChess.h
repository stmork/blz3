/*
**
**	$Filename:	DlgMatChess.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Chess material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATCHESS_H__C24952A5_2CEA_482F_852D_74BFCC2CCEE9__INCLUDED_)
#define AFX_DLGMATCHESS_H__C24952A5_2CEA_482F_852D_74BFCC2CCEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatChess.h : header file
//

#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageMaterial.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatChess dialog

class CDlgMatChess : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatChess        *m_Material;

	CPageMaterial      m_PageBlack;
	CPageMaterial      m_PageWhite;
// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatChess(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgMatChess();

// Dialog Data
	//{{AFX_DATA(CDlgMatChess)
	enum { IDD = IDD_MAT_CHESS };
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	CB3IntSpinButtonCtrl	m_xTimesCtrl;
	CB3IntSpinButtonCtrl	m_yTimesCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatChess)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatChess)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATCHESS_H__C24952A5_2CEA_482F_852D_74BFCC2CCEE9__INCLUDED_)
