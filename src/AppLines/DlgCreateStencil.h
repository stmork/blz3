/*
**
**	$Filename:	DlgCreateStencil.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Fast stencil creation on shape creation
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCREATESTENCIL_H__F214B12C_75AC_4224_9A9B_7DA7FA7735E7__INCLUDED_)
#define AFX_DLGCREATESTENCIL_H__F214B12C_75AC_4224_9A9B_7DA7FA7735E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateStencil.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateStencil dialog

class CDlgCreateStencil : public CPropertyPage
{
	b3_stencil_bound m_Bound;
	b3_stencil_limit m_Limit;

	static b3_f64 m_Increments[3];
	static b3_f64 m_Accels[3];
	static int    m_Digits[3];

	DECLARE_DYNCREATE(CDlgCreateStencil)

// Construction
public:
	b3Shape         *m_Shape;
	b3CondRectangle *m_Stencil;

public:
	              CDlgCreateStencil();
	             ~CDlgCreateStencil();
	void          b3PostProcess();

// Dialog Data
	//{{AFX_DATA(CDlgCreateStencil)
	enum { IDD = IDD_CREATE_STENCIL };
	CB3FloatSpinButtonCtrl	m_xStartCtrl;
	CB3FloatSpinButtonCtrl	m_xEndCtrl;
	CB3FloatSpinButtonCtrl	m_yStartCtrl;
	CB3FloatSpinButtonCtrl	m_yEndCtrl;
	BOOL	m_ReallyCreate;
	CString	m_yEndLegend;
	CString	m_yStartLegend;
	CString	m_xEndLegend;
	CString	m_xStartLegend;
	int		m_Unit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateStencil)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgCreateStencil)
	virtual BOOL OnInitDialog();
	afx_msg void OnUnitChanged();
	afx_msg void OnLimitChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATESTENCIL_H__F214B12C_75AC_4224_9A9B_7DA7FA7735E7__INCLUDED_)
