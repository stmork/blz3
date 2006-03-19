/*
**
**	$Filename:	DlgMatNormal.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATNORMAL_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
#define AFX_DLGMATNORMAL_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatNormal.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3ColorFieldSelector.h"
#include "blz3/system/b3SimplePreviewDialog.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "blz3/raytrace/b3Material.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatNormal dialog

class CDlgMatNormal : public CB3SimplePreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatNormal       *m_Material;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatNormal(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgMatNormal();

// Dialog Data
	//{{AFX_DATA(CDlgMatNormal)
	enum { IDD = IDD_MAT_NORMAL };
	CB3ColorFieldSelector	m_AmbientCtrl;
	CB3ColorFieldSelector	m_DiffuseCtrl;
	CB3ColorFieldSelector	m_SpecularCtrl;
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatNormal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatNormal)
	virtual BOOL OnInitDialog();
	afx_msg void OnColorAmbient();
	afx_msg void OnColorDiffuse();
	afx_msg void OnColorSpecular();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATNORMAL_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
