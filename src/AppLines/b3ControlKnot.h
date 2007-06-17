/*
**
**	$Filename:	b3ControlKnot.cpp $  
**	$Release:	Dortmund 2004 $
**	$Revision: 823 $
**	$Date: 2004-07-27 21:05:59 +0200 (Di, 27 Jul 2004) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Displaying the knot vector of a BSpline
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3CONTROLKNOT_H__6E2AE75B_D412_457F_911F_7A2E064D8252__INCLUDED_)
#define AFX_B3CONTROLKNOT_H__6E2AE75B_D412_457F_911F_7A2E064D8252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3ControlKnot.h : header file
//

#include "blz3/base/b3Spline.h"

/////////////////////////////////////////////////////////////////////////////
// CB3ControlKnot window

class CB3ControlKnot : public CStatic
{
	static int m_LeftOffset;
	static int m_RightOffset;
	b3Spline  *m_Spline;

// Construction
public:
	CB3ControlKnot();

// Attributes
public:

// Operations
public:
	void b3Init(b3Spline *spline);
	void b3Update(b3_bool refresh = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ControlKnot)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CB3ControlKnot();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ControlKnot)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3CONTROLKNOT_H__6E2AE75B_D412_457F_911F_7A2E064D8252__INCLUDED_)
