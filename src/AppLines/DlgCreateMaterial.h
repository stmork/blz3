/*
**
**	$Filename:	DlgCreateMaterial.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3ShowImage.h"
#include "b3SelectColor.h"
#include "b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateMaterial dialog

class CDlgCreateMaterial : public CPropertyPage
{
	b3MatNormal  *m_MatNormal;
	b3MatTexture *m_MatTexture;
	DECLARE_DYNCREATE(CDlgCreateMaterial)

// Construction
public:
	b3Material   *m_Material;
	CDlgCreateMaterial();
	~CDlgCreateMaterial();

// Dialog Data
	//{{AFX_DATA(CDlgCreateMaterial)
	enum { IDD = IDD_CREATE_MATERIAL };
	CB3ShowImage	m_PreviewTexture;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	CB3FloatEdit	m_SpecExponentCtrl;
	CB3FloatEdit	m_RefrCtrl;
	CB3FloatEdit	m_ReflCtrl;
	CB3FloatEdit	m_IORCtrl;
	CB3ColorFieldSelector	m_SpecCtrl;
	CB3ColorFieldSelector	m_DiffCtrl;
	CB3ColorFieldSelector	m_AmbCtrl;
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
	afx_msg void OnReallyCreate();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUseTexture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEMATERIAL_H__EAA14545_7EFA_4BAC_B94C_9223B4E93B6C__INCLUDED_)
