/*
**
**	$Filename:	DlgBumpWood.h $
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

#if !defined(AFX_DLGBUMPWOOD_H__FE6CEEA1_94B4_4938_AFC4_96AFABD2FEA2__INCLUDED_)
#define AFX_DLGBUMPWOOD_H__FE6CEEA1_94B4_4938_AFC4_96AFABD2FEA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpWood.h : header file
//

#include "blz3/raytrace/b3BumpSampler.h"
#include "b3SimplePropertyPreviewDialog.h"
#include "b3ShowSample.h"
#include "PageBump.h"
#include "PageWood.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWood dialog

class CDlgBumpWood : public CB3SimplePropertyPreviewDialog
{
	b3BumpWood         *m_Bump;
	b3BumpSampler      *m_BumpSampler;

	CPageBump          m_PageBump;
	CPageWood          m_PageWood;
	CPageScaling       m_PageScaling;

	// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void    b3Register();
	CDlgBumpWood(b3Item *item, CWnd* pParent = NULL);   // standard constructor
	~CDlgBumpWood();

// Dialog Data
	//{{AFX_DATA(CDlgBumpWood)
	enum { IDD = IDD_BUMP_WOOD };
	CB3ShowSample	m_PreviewBumpCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpWood)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpWood)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUMPWOOD_H__FE6CEEA1_94B4_4938_AFC4_96AFABD2FEA2__INCLUDED_)
