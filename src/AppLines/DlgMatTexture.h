/*
**
**	$Filename:	DlgMatTexture.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATTEXTURE_H__C00C2CB5_AB9D_4B91_95C1_0304DB8575BD__INCLUDED_)
#define AFX_DLGMATTEXTURE_H__C00C2CB5_AB9D_4B91_95C1_0304DB8575BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatTexture.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatTexture dialog

class CDlgMatTexture : public CDialog
{
	b3MatTexture *m_Material;

// Construction
public:
	static b3_bool b3Edit(b3Item *item);
	static void b3Register();
	CDlgMatTexture(b3Item *item,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMatTexture)
	enum { IDD = IDD_MAT_TEXTURE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatTexture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatTexture)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATTEXTURE_H__C00C2CB5_AB9D_4B91_95C1_0304DB8575BD__INCLUDED_)
