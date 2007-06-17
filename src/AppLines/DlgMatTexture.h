/*
**
**	$Filename:	DlgMatTexture.h $
**	$Release:	Dortmund 2003 $
**	$Revision: 1431 $
**	$Date: 2006-05-27 15:32:22 +0200 (Sa, 27 Mai 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATTEXTURE_H__C00C2CB5_AB9D_4B91_95C1_0304DB8575BD__INCLUDED_)
#define AFX_DLGMATTEXTURE_H__C00C2CB5_AB9D_4B91_95C1_0304DB8575BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatTexture.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3SimplePreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "b3ConditionControl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatTexture dialog

class CDlgMatTexture : public CB3SimplePreviewDialog
{
	b3_stencil_bound   m_Bound;
	b3BBox            *m_BBox;
	b3Shape           *m_Shape;
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatTexture      *m_Material;

	b3ConditionControl m_xStart;
	b3ConditionControl m_xScale;
	b3ConditionControl m_yStart;
	b3ConditionControl m_yScale;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatTexture(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgMatTexture();

// Dialog Data
	//{{AFX_DATA(CDlgMatTexture)
	enum { IDD = IDD_MAT_TEXTURE };
	CB3FloatSpinButtonCtrl	m_xStartCtrl;
	CB3FloatSpinButtonCtrl	m_xScaleCtrl;
	CB3IntSpinButtonCtrl	m_xTimesCtrl;
	CB3FloatSpinButtonCtrl	m_yStartCtrl;
	CB3FloatSpinButtonCtrl	m_yScaleCtrl;
	CB3IntSpinButtonCtrl	m_yTimesCtrl;
	CString	m_yScaleLegend;
	CString	m_yStartLegend;
	CString	m_xScaleLegend;
	CString	m_xStartLegend;
	int		m_Unit;
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3ShowImage	m_PreviewTexture;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatTexture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatTexture)
	afx_msg void OnUnitChanged();
	afx_msg void OnLimitChanged();
	afx_msg void OnChangeTexturePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATTEXTURE_H__C00C2CB5_AB9D_4B91_95C1_0304DB8575BD__INCLUDED_)
