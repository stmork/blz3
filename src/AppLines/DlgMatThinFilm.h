/*
**
**	$Filename:	DlgMatChess.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Chess material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATTHINFILM_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
#define AFX_DLGMATTHINFILM_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatChess.h : header file
//

#include "blz3/raytrace/b3Material.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "b3SimplePropertyPreviewDialog.h"
#include "b3ShowRaytrace.h"
#include "PageMaterial.h"
#include "PageScaling.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatChess dialog

class CDlgMatThinFilm : public CB3SimplePropertyPreviewDialog
{
	b3Scene           *m_MatScene;
	b3Base<b3Item>    *m_MatHead;
	b3MatThinFilm     *m_Material;

	CPageMaterial      m_PageMaterial;
	CPageScaling       m_PageScaling;
// Construction
public:
	static b3_bool b3Edit(b3Item *item,void *ptr);
	static void b3Register();
	CDlgMatThinFilm(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	~CDlgMatThinFilm();

// Dialog Data
	//{{AFX_DATA(CDlgMatThinFilm)
	enum { IDD = IDD_MAT_THIN_FILM };
	CB3ColorFieldSelector	m_InterferenceCtrl;
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	CB3FloatSpinButtonCtrl	m_ThicknessCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatThinFilm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatThinFilm)
	afx_msg void OnColorInterference();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3InitDialog();
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATTHINFILM_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
