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

#include "blz3/raytrace/b3Light.h"
#include "blz3/base/b3Pick.h"

/////////////////////////////////////////////////////////////////////////////
// CB3ControlLDC window

typedef CDC b3DrawContext;

enum CB3LDCMode
{
	LDC_EDIT,
	LDC_DIAGRAM
};

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
	void b3SetMode(CB3LDCMode mode);
	void b3Init(b3Light *light);
	void b3Update(b3_bool refresh = true);
	b3Spline * m_LDC;
	virtual ~CB3ControlLDC();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ControlLDC)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CDC         m_DC;
	CBitmap     m_Bitmap;
	CB3LDCMode  m_Mode;
	b3PickBase  m_PickBase;
	b3_vector  *m_Curve;
	b3_res      m_xMax;
	b3_res      m_yMax;
	b3_bool     m_Init;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B3CONTROLLDC_H__60D085A3_E7BE_11D5_9CFA_080020FDDE74__INCLUDED_)
