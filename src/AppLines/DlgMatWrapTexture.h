/*
**
**	$Filename:	DlgMatWrapTexture.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wrap texture material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATWRAPTEXTURE_H__1D05A560_136A_4A78_957E_CE0D59CA52C7__INCLUDED_)
#define AFX_DLGMATWRAPTEXTURE_H__1D05A560_136A_4A78_957E_CE0D59CA52C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatWrapTexture.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/raytrace/b3Material.h"
#include "b3ConditionControl.h"
#include "b3ShowRaytrace.h"
#include "b3SimplePreviewDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWrapTexture dialog

class CDlgMatWrapTexture : public CB3SimplePreviewDialog
{
	b3_stencil_bound   m_Bound;
	b3BBox            *m_BBox;
	b3Shape           *m_Shape;
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatWrapTexture  *m_Material;

	b3ConditionControl m_xStart;
	b3ConditionControl m_xEnd;
	b3ConditionControl m_yStart;
	b3ConditionControl m_yEnd;


// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatWrapTexture(CAppObjectDoc *pDoc,b3Item *item,CWnd* pParent = NULL);   // standard constructor
	~CDlgMatWrapTexture();

// Dialog Data
	//{{AFX_DATA(CDlgMatWrapTexture)
	enum { IDD = IDD_MAT_WRAP_TEXTURE };
	CB3FloatSpinButtonCtrl	m_xStartCtrl;
	CB3FloatSpinButtonCtrl	m_xEndCtrl;
	CB3FloatSpinButtonCtrl	m_yStartCtrl;
	CB3FloatSpinButtonCtrl	m_yEndCtrl;
	CString	m_yEndLegend;
	CString	m_yStartLegend;
	CString	m_xEndLegend;
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
	//{{AFX_VIRTUAL(CDlgMatWrapTexture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatWrapTexture)
	virtual BOOL OnInitDialog();
	afx_msg void OnUnitChanged();
	afx_msg void OnLimitChanged();
	afx_msg void OnChangeTexturePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATWRAPTEXTURE_H__1D05A560_136A_4A78_957E_CE0D59CA52C7__INCLUDED_)
