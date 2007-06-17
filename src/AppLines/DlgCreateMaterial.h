/*
**
**	$Filename:	DlgCreateMaterial.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 1341 $
**	$Date: 2006-03-19 15:47:18 +0100 (So, 19 Mrz 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Fast material creation on shape creation
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCREATEMATERIAL_H__EAA14545_7EFA_4BAC_B94C_9223B4E93B6C__INCLUDED_)
#define AFX_DLGCREATEMATERIAL_H__EAA14545_7EFA_4BAC_B94C_9223B4E93B6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateMaterial.h : header file
//

#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3ShowImage.h"
#include "blz3/system/b3ColorFieldSelector.h"
#include "blz3/system/b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateMaterial dialog

class CDlgCreateMaterial : public CPropertyPage
{
	b3MatNormal    *m_MatNormal;
	b3MatTexture   *m_MatTexture;
	b3Scene        *m_MatScene;
	b3Base<b3Item> *m_MatHead;
	b3_f64          m_Ior;
	b3_f64          m_Reflection;
	b3_f64          m_Refraction;
	b3_f64          m_SpecularExp;

// Construction
public:
	b3Material   *m_Material;

public:
	              CDlgCreateMaterial(b3_u32 shading_class_type);
	             ~CDlgCreateMaterial();
	void          b3Init();
	void          b3PostProcess(b3CondRectangle *stencil = null);

// Dialog Data
	//{{AFX_DATA(CDlgCreateMaterial)
	enum { IDD = IDD_CREATE_MATERIAL };
	CB3ColorFieldSelector	m_SpecCtrl;
	CB3ColorFieldSelector	m_DiffCtrl;
	CB3ColorFieldSelector	m_AmbCtrl;
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3ShowImage	m_PreviewTexture;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	BOOL	m_ReallyCreate;
	BOOL	m_UseTexture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgCreateMaterial)
	afx_msg void OnChangeAmbient();
	afx_msg void OnChangeDiffuse();
	afx_msg void OnChangeSpecular();
	afx_msg void OnChangeTexturePath();
	virtual BOOL OnInitDialog();
	afx_msg void OnUseTexture();
	afx_msg void OnSurfaceEdit();
	afx_msg void OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEMATERIAL_H__EAA14545_7EFA_4BAC_B94C_9223B4E93B6C__INCLUDED_)
