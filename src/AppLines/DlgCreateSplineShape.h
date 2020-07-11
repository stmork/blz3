/*
**
**	$Filename:	DlgCreateTriangles.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for creating spline shapes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGCREATESPLINESHAPE_H__B74D183F_41B1_433F_8B1B_901B9A6C7631__INCLUDED_)
#define AFX_DLGCREATESPLINESHAPE_H__B74D183F_41B1_433F_8B1B_901B9A6C7631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateSplineShape.h : header file
//

#include "b3ProfileShapeDialog.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateSplineShape dialog

class CDlgCreateSplineShape : public CB3ProfileShapeDialog
{
	b3_s32         m_xDegree;
	b3_s32         m_xControls;
	b3_f64         m_xSize;
	b3_s32         m_yDegree;
	b3_s32         m_yControls;
	b3_f64         m_ySize;

	// Construction
public:
	CDlgCreateSplineShape();
	~CDlgCreateSplineShape();

	virtual void b3PostProcess();
	// Dialog Data
	//{{AFX_DATA(CDlgCreateSplineShape)
	enum { IDD = IDD_CREATE_SPLINESHAPE };
	CB3FloatEdit	m_ySizeCtrl;
	CB3FloatEdit	m_xSizeCtrl;
	CB3IntSpinButtonCtrl	m_xDegreeCtrl;
	CB3IntSpinButtonCtrl	m_xControlsCtrl;
	CB3IntSpinButtonCtrl	m_yDegreeCtrl;
	CB3IntSpinButtonCtrl	m_yControlsCtrl;
	CString	m_ySizeLegend;
	CString	m_xSizeLegend;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateSplineShape)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual void        b3Init();

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateSplineShape)
	afx_msg void OnXDegreeSpin(NMHDR * pNMHDR, LRESULT * pResult) ;
	afx_msg void OnXDegreeEdit();
	afx_msg void OnYDegreeSpin(NMHDR * pNMHDR, LRESULT * pResult) ;
	afx_msg void OnYDegreeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATESPLINESHAPE_H__B74D183F_41B1_433F_8B1B_901B9A6C7631__INCLUDED_)
