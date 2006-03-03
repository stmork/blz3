/*
**
**	$Filename:	DlgLensFlare.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3LensFlare properties
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGLENSFLARE_H__5BDBF7F8_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
#define AFX_DLGLENSFLARE_H__5BDBF7F8_D6AE_11D5_89F1_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLensFlare.h : header file
//

#include "blz3/raytrace/b3Special.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3ColorFieldSelector.h"
#include "b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLensFlare dialog

class CDlgLensFlare : public CPropertyPage
{
// Construction
	b3_u32       m_ShadingClassType;
	b3Scene     *m_LensFlareScene;
	b3LensFlare *m_EditLensFlare;
public:
	b3LensFlare *m_LensFlare;
	CDlgLensFlare(b3_u32 class_type,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLensFlare)
	enum { IDD = IDD_LENS_FLARE };
	CB3ShowRaytrace	m_LensFlarePreviewCtrl;
	CB3FloatEdit	m_LensFlareExponCtrl;
	CB3ColorFieldSelector	m_LensFlareColorCtrl;
	BOOL	m_ActLensFlare;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLensFlare)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLensFlare)
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnLensFlareColorChange();
	afx_msg void OnActLensFlare();
	afx_msg void OnDestroy();
	afx_msg void OnLensFlareExponChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLENSFLARE_H__5BDBF7F8_D6AE_11D5_89F1_00E018258F1A__INCLUDED_)
