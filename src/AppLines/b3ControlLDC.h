/*
**
**	$Filename:	b3ControlLDC.h $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Controlling the LDC of a light source
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_B3CONTROLLDC_H__60D085A3_E7BE_11D5_9CFA_080020FDDE74__INCLUDED_)
#define AFX_B3CONTROLLDC_H__60D085A3_E7BE_11D5_9CFA_080020FDDE74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3ControlLDC.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CB3ControlLDC window

class CB3ControlLDC : public CStatic, protected b3Mem
{
// Construction
public:
	CB3ControlLDC();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ControlLDC)
	//}}AFX_VIRTUAL

// Implementation
public:
	void b3Update(b3_bool refresh = true);
	b3Spline * m_LDC;
	virtual ~CB3ControlLDC();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ControlLDC)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	b3_vector * m_Curve;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3CONTROLLDC_H__60D085A3_E7BE_11D5_9CFA_080020FDDE74__INCLUDED_)
