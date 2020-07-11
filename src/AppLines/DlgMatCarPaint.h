/*
**
**	$Filename:	DlgMatCarPaint.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Car paint material
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATCARPAINT_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
#define AFX_DLGMATCARPAINT_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatNormal.h : header file
//

#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"
#include "blz3/system/b3ShowSample.h"
#include "blz3/system/b3ShowRaytrace.h"
#include "PageMaterial.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCarPaint dialog

class CDlgMatCarPaint : public CB3SimplePropertyPreviewDialog
{
	b3Scene      *     m_MatScene;
	b3Base<b3Item>  *  m_MatHead;
	b3MatCarPaint   *  m_Material;

	CPageMaterial      m_PageParallel;
	CPageMaterial      m_PagePerpendicular;

	// Construction
public:
	static b3_bool b3Edit(b3Item * item, void * ptr);
	static void b3Register();
	CDlgMatCarPaint(b3Item * item, CAppObjectDoc * pDoc, CWnd * pParent = NULL); // standard constructor
	~CDlgMatCarPaint();

	// Dialog Data
	//{{AFX_DATA(CDlgMatCarPaint)
	enum { IDD = IDD_MAT_CAR_PAINT };
	BOOL	m_Metallic;
	CB3FloatSpinButtonCtrl	m_MetallicCtrl;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatCarPaint)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatCarPaint)
	afx_msg void OnMetallicMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3PreInitDialog();
	void b3PostInitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATCARPAINT_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
