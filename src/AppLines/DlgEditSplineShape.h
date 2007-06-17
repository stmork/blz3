/*
**
**	$Filename:	DlgEditTriangles.h $
**	$Release:	Dortmund 2007 $
**	$Revision: 308 $
**	$Date$
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for editing spline shapes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGEDITSPLINESHAPE_H__B74D183F_41B1_433F_8B1B_901B9A6C7631__INCLUDED_)
#define AFX_DLGEDITSPLINESHAPE_H__B74D183F_41B1_433F_8B1B_901B9A6C7631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateSplineShape.h : header file
//

#include "b3ShapeDialog.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateSplineShape dialog

class CDlgEditSplineShape : public CB3ShapeDialog
{
	b3_s32         m_xDegree;
	b3_s32         m_xControls;
	b3_f64         m_xSize;
	b3_s32         m_yDegree;
	b3_s32         m_yControls;
	b3_f64         m_ySize;

// Construction
public:
	CDlgEditSplineShape(CWnd* pParent = NULL);
	~CDlgEditSplineShape();

	virtual void b3PostProcess();
// Dialog Data
	//{{AFX_DATA(CDlgEditSplineShape)
	enum { IDD = IDD_EDIT_SPLINESHAPE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditSplineShape)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();

	// Generated message map functions
	//{{AFX_MSG(CDlgEditSplineShape)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITSPLINESHAPE_H__B74D183F_41B1_433F_8B1B_901B9A6C7631__INCLUDED_)
