/*
**
**	$Filename:	DlgMatMarble.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_)
#define AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatMarble.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "b3SimplePreviewDialog.h"
#include "b3SelectColor.h"
#include "b3VectorEdit.h"
#include "b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatMarble dialog

class CDlgMatMarble : public CB3SimplePreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatMarble       *m_Material;

	CB3PosGroup        m_ScaleCtrl;

// Construction
public:
	static b3_bool b3Edit(b3Item *item);
	static void b3Register();
	CDlgMatMarble(b3Item *item,CWnd* pParent = NULL);   // standard constructor
	~CDlgMatMarble();

// Dialog Data
	//{{AFX_DATA(CDlgMatMarble)
	enum { IDD = IDD_MAT_MARBLE };
	CB3ColorFieldSelector	m_AmbientCtrl;
	CB3ColorFieldSelector	m_DiffuseCtrl;
	CB3ColorFieldSelector	m_SpecularCtrl;
	CB3FloatSpinButtonCtrl	m_ReflectionCtrl;
	CB3FloatSpinButtonCtrl	m_RefractionCtrl;
	CB3FloatSpinButtonCtrl	m_IorCtrl;
	CB3FloatSpinButtonCtrl	m_SpecularExpCtrl;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	CB3FloatEdit	m_xScaleCtrl;
	CB3FloatEdit	m_yScaleCtrl;
	CB3FloatEdit	m_zScaleCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatMarble)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatMarble)
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

#endif // !defined(AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_)
