/*
**
**	$Filename:	DlgMatNormal.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATNORMAL_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
#define AFX_DLGMATNORMAL_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatNormal.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatNormal dialog

class CDlgMatNormal : public CDialog
{
	b3MatNormal *m_Material;

// Construction
public:
	static b3_bool b3Edit(b3Item *item);
	static void b3Register();
	CDlgMatNormal(b3Item *item,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMatNormal)
	enum { IDD = IDD_MAT_NORMAL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatNormal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatNormal)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATNORMAL_H__5907206B_4753_42C8_A064_70CAE6DA2D95__INCLUDED_)
