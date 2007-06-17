/*
**
**	$Filename:	DlgEditTriangles.h $
**	$Release:	Dortmund 2007 $
**	$Revision$
**	$Id$
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for editing triangles
**
**	(C) Copyright 2007  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGEDITTRIANGLES_H__681CB6E4_BEDE_4BB0_B647_D28615AFDE2D__INCLUDED_)
#define AFX_DLGEDITTRIANGLES_H__681CB6E4_BEDE_4BB0_B647_D28615AFDE2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditTriangles.h : header file
//

#include "blz3/system/b3FloatEdit.h"
#include "blz3/system/b3IntSpinButtonCtrl.h"
#include "b3ShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateTriangles dialog

class CDlgEditTriangles : public CB3ShapeDialog
{
// Construction
public:
	CDlgEditTriangles(CWnd* pParent = NULL);   // standard constructor

	virtual void b3PostProcess();
// Dialog Data
	//{{AFX_DATA(CDlgEditTriangles)
	enum { IDD = IDD_EDIT_TRIANGLES };
	BOOL	m_Phong;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditTriangles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void        b3Init();

	// Generated message map functions
	//{{AFX_MSG(CDlgEditTriangles)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITTRIANGLES_H__681CB6E4_BEDE_4BB0_B647_D28615AFDE2D__INCLUDED_)
