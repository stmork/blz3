/*
**
**	$Filename:	b3TreeCtrl.h $  
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - TreeCtrl handling
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_TREECTRL_H
#define B3_SYSTEM_TREECTRL_H

#include "blz3/b3Config.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// b3TreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CB3TreeCtrl window

class CB3TreeCtrl : public CTreeCtrl
{
// Construction
public:
	CB3TreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3TreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CB3TreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3TreeCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	HTREEITEM b3FindLParam(HTREEITEM parent,LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(B3_SYSTEM_TREECTRL_H)
