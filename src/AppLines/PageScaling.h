/*
**
**	$Filename:	PageScaling.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Scaling properties
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_PAGESCALING_H__80820B5F_EBA9_49A4_BC09_81140225B1D1__INCLUDED_)
#define AFX_PAGESCALING_H__80820B5F_EBA9_49A4_BC09_81140225B1D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageScaling.h : header file
//

#include "blz3/system/b3PropertyPage.h"
#include "blz3/raytrace/b3Scaling.h"
#include "b3VectorEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CPageScaling dialog

class CPageScaling : public CB3PropertyPage
{
	CB3PosGroup        m_ScaleCtrl;

// Construction
public:
	CPageScaling();
	~CPageScaling();

	b3Scaling *m_Scaling;

// Dialog Data
	//{{AFX_DATA(CPageScaling)
	enum { IDD = IDD_PAGE_SCALING };
	CB3FloatEdit	m_xScaleCtrl;
	CB3FloatEdit	m_yScaleCtrl;
	CB3FloatEdit	m_zScaleCtrl;
	int		m_ScaleMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageScaling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageScaling)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESCALING_H__80820B5F_EBA9_49A4_BC09_81140225B1D1__INCLUDED_)
