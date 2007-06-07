/*
**
**	$Filename:	DlgBumpOakPlank.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood bump map
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPOAKPLANK_H__39AE8FE4_AB49_4FBC_85EF_BB94F9AE60F1__INCLUDED_)
#define AFX_DLGBUMPOAKPLANK_H__39AE8FE4_AB49_4FBC_85EF_BB94F9AE60F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpOakPlank.h : header file
//

#include "blz3/raytrace/b3BumpSampler.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowSample.h"
#include "PageBump.h"
#include "PageWood.h"
#include "PageOakPlank.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpOakPlank dialog

class CDlgBumpOakPlank : public CB3SimplePropertyPreviewDialog
{
	b3BumpOakPlank    *m_Bump;
	b3BBox            *m_BBox;
	b3BumpSampler     *m_BumpSampler;

	CPageBump          m_PageBump;
	CPageWood          m_PageWood;
	CPageOakPlank      m_PageOakPlank;
	CPageScaling       m_PageScaling;

	// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgBumpOakPlank(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpOakPlank();

// Dialog Data
	//{{AFX_DATA(CDlgBumpOakPlank)
	enum { IDD = IDD_BUMP_OAKPLANK };
	CB3ShowSample	m_PreviewBumpCtrl;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpOakPlank)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpOakPlank)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPOAKPLANK_H__39AE8FE4_AB49_4FBC_85EF_BB94F9AE60F1__INCLUDED_)
