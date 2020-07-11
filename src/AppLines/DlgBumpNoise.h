/*
**
**	$Filename:	DlgBumpNoise.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Noise bump mapping
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGBUMPNOISE_H__585894AD_A7C2_436A_8D47_674B8DD71BB3__INCLUDED_)
#define AFX_DLGBUMPNOISE_H__585894AD_A7C2_436A_8D47_674B8DD71BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBumpNoise.h : header file
//

#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageBump.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpNoise dialog

class CDlgBumpNoise : public CB3SimplePropertyPreviewDialog
{
	b3Scene      *     m_BumpScene;
	b3Base<b3Item>  *  m_BumpHead;
	b3BumpNoise    *   m_Bump;

	CPageBump          m_PageBump;
	CPageScaling       m_PageScaling;

	// Construction
public:
	static b3_bool b3Edit(b3Item * item, void * ptr);
	static void    b3Register();
	CDlgBumpNoise(b3Item * item, CAppObjectDoc * pDoc, CWnd * pParent = NULL); // standard constructor
	~CDlgBumpNoise();

	// Dialog Data
	//{{AFX_DATA(CDlgBumpNoise)
	enum { IDD = IDD_BUMP_NOISE };
	CB3ShowRaytrace	m_PreviewBumpCtrl;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBumpNoise)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBumpNoise)
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

#endif // !defined(AFX_DLGBUMPNOISE_H__585894AD_A7C2_436A_8D47_674B8DD71BB3__INCLUDED_)
