/*
**
**	$Filename:	DlgMatSlide.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Slide material
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGMATSLIDE_H__22891A65_F807_4FF2_95F2_117276E31690__INCLUDED_)
#define AFX_DLGMATSLIDE_H__22891A65_F807_4FF2_95F2_117276E31690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatSlide.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatSlide dialog

class CDlgMatSlide : public CDialog
{
	b3MatSlide *m_Material;

// Construction
public:
	static b3_bool b3Edit(b3Item *item);
	static void b3Register();
	CDlgMatSlide(b3Item *item,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMatSlide)
	enum { IDD = IDD_MAT_SLIDE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatSlide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatSlide)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATSLIDE_H__22891A65_F807_4FF2_95F2_117276E31690__INCLUDED_)
