/*
**
**	$Filename:	b3SplinePreview.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Image file select dialog box
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3SPLINEPREVIEW_H__A8092458_8156_4855_8602_E52B2F6D3586__INCLUDED_)
#define AFX_B3SPLINEPREVIEW_H__A8092458_8156_4855_8602_E52B2F6D3586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3SplinePreview.h : header file
//

#include "blz3/base/b3Spline.h"

/////////////////////////////////////////////////////////////////////////////
// CB3SplinePreview window

class CB3SplinePreview : public CStatic
{
	b3Spline * m_Spline;
	b3_vector m_Lower, m_Upper;
	b3_f64    m_xOffset;
	b3_f64    m_yOffset, m_Ratio;
	CRect     m_Rect;
	// Construction
public:
	CB3SplinePreview();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3SplinePreview)
	//}}AFX_VIRTUAL

	// Implementation
public:
	void b3Update(b3Spline * spline);
	virtual ~CB3SplinePreview();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3SplinePreview)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void b3ComputePoint(b3_vector * point, b3_coord & x, b3_coord & y);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3SPLINEPREVIEW_H__A8092458_8156_4855_8602_E52B2F6D3586__INCLUDED_)
