/*
**
**	$Filename:	DlgProfileBevelSpline.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for shapes which can be created via profiles
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGPROFILEBEVELSPLINE_H__B9DAF0E5_DD57_429F_8D80_9007062160C4__INCLUDED_)
#define AFX_DLGPROFILEBEVELSPLINE_H__B9DAF0E5_DD57_429F_8D80_9007062160C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProfileBevelSpline.h : header file
//

#include "b3ProfileShapeDialog.h"
#include "blz3/system/b3FloatSpinButtonCtrl.h"
#include "blz3/system/b3FloatEdit.h"
#include "b3SplinePreview.h"
#include "b3VectorEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProfileBevelSpline dialog

class CDlgProfileBevelSpline : public CB3ProfileShapeDialog
{
	b3Spline       m_Spline;
	b3_vector      m_Controls[B3_MAX_CONTROLS];
	b3_f32         m_Knots[B3_MAX_KNOTS];
// Construction
public:
	virtual void b3PostProcess();
	CDlgProfileBevelSpline(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProfileBevelSpline)
	enum { IDD = IDD_PROFILE_BEVEL_SPLINE };
	CSpinButtonCtrl	m_yControlsCtrl;
	CSpinButtonCtrl	m_yDegreeCtrl;
	CB3SplinePreview	m_Preview;
	CB3FloatSpinButtonCtrl	m_ObliqueCtrl;
	CB3FloatSpinButtonCtrl	m_HeightCtrl;
	CB3FloatSpinButtonCtrl	m_xEdgeCtrl;
	CB3FloatSpinButtonCtrl	m_yEdgeCtrl;
	double	m_xEdge;
	double	m_yEdge;
	double	m_Height;
	double	m_Oblique;
	int		m_yControls;
	int		m_yDegree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProfileBevelSpline)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();
	virtual const char *b3GetSection();

	// Generated message map functions
	//{{AFX_MSG(CDlgProfileBevelSpline)
	virtual BOOL OnInitDialog();
	afx_msg void OnObliqueSpin(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnObliqueEdit();
	afx_msg void OnEdgeSpin(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnEdgeEdit();
	afx_msg void OnDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnDegreeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROFILEBEVELSPLINE_H__B9DAF0E5_DD57_429F_8D80_9007062160C4__INCLUDED_)
