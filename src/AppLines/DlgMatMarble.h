/*
**
**	$Filename:	DlgMatMarble.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_)
#define AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatMarble.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatMarble dialog

class CDlgMatMarble : public CDialog
{
// Construction
public:
	static b3_bool b3Edit(b3Item *item);
	static void b3Register();
	CDlgMatMarble(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMatMarble)
	enum { IDD = IDD_MAT_MARBLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatMarble)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatMarble)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATMARBLE_H__5422E994_1885_4E23_B3D8_3B3D2DED5501__INCLUDED_)
