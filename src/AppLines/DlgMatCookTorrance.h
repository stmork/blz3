/*
**
**	$Filename:	DlgMatCookTorrance.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Cook&Torrance material shader
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATCOOKTORRANCE_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
#define AFX_DLGMATCOOKTORRANCE_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatCookTorrance.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/raytrace/b3Material.h"
#include "b3SimplePreviewDialog.h"
#include "b3SelectColor.h"
#include "b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCookTorrance dialog

class CDlgMatCookTorrance : public CB3SimplePreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatCookTorrance *m_Material;

// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatCookTorrance(b3Item *item,CWnd* pParent = NULL);   // standard constructor
	~CDlgMatCookTorrance();

// Dialog Data
	//{{AFX_DATA(CDlgMatCookTorrance)
	enum { IDD = IDD_MAT_COOK_TORRANCE };
	CB3ColorFieldSelector	m_AmbientCtrl;
	CB3ColorFieldSelector	m_DiffuseCtrl;
	CB3ColorFieldSelector	m_SpecularCtrl;
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3FloatSpinButtonCtrl	m_KaCtrl;
	CB3FloatSpinButtonCtrl	m_KdCtrl;
	CB3FloatSpinButtonCtrl	m_KsCtrl;
	CB3FloatSpinButtonCtrl	m_MCtrl;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatCookTorrance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatCookTorrance)
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

#endif // !defined(AFX_DLGMATCOOKTORRANCE_H__7C6BBDA5_AE55_4E64_A4C5_CA0B09524C5C__INCLUDED_)
