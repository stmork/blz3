/*
**
**	$Filename:	DlgCreateTriangles.h $
**	$Release:	Dortmund 2002 $
**	$Revision: 308 $
**	$Date: 2002-03-10 21:34:18 +0100 (So, 10 Mrz 2002) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for creating spline shapes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCREATEROTSHAPE_H__A0372F03_5F31_4218_9F0E_6B5042AB8610__INCLUDED_)
#define AFX_DLGCREATEROTSHAPE_H__A0372F03_5F31_4218_9F0E_6B5042AB8610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateRotShape.h : header file
//

#include "b3ProfileShapeDialog.h"
#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateRotShape dialog

class CDlgCreateRotShape : public CB3ProfileShapeDialog
{
	b3_f64         m_xSize;
	b3_f64         m_ySize;
	b3_s32         m_Degree;
	b3_s32         m_Controls;
	b3_s32         m_SubDiv;
// Construction
public:
	CDlgCreateRotShape();
	~CDlgCreateRotShape();

	virtual void b3PostProcess();
// Dialog Data
	//{{AFX_DATA(CDlgCreateRotShape)
	enum { IDD = IDD_CREATE_ROTSHAPE };
	CStatic	m_LegendCtrl;
	CB3FloatEdit	m_xSizeCtrl;
	CB3FloatEdit	m_ySizeCtrl;
	CB3IntSpinButtonCtrl	m_DegreeCtrl;
	CB3IntSpinButtonCtrl	m_ControlsCtrl;
	CB3IntSpinButtonCtrl	m_SubdivCtrl;
	BOOL	m_Closed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateRotShape)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateRotShape)
	afx_msg void OnDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnDegreeEdit();
	afx_msg void OnClosed();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEROTSHAPE_H__A0372F03_5F31_4218_9F0E_6B5042AB8610__INCLUDED_)
