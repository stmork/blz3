/*
**
**	$Filename:	DlgObjectPreview.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Preview actual object editing
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGOBJECTPREVIEW_H__4F9E06D4_3ABC_4D28_BE05_490FF5888DE5__INCLUDED_)
#define AFX_DLGOBJECTPREVIEW_H__4F9E06D4_3ABC_4D28_BE05_490FF5888DE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjectPreview.h : header file
//

#include "b3ShowRaytrace.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectPreview dialog

class CDlgObjectPreview : public CDialog
{
	b3Scene  *m_PreviewScene;

// Construction
public:
	CDlgObjectPreview(b3BBox *bbox,b3CameraPart *camera,CWnd* pParent = NULL);   // standard constructor
	~CDlgObjectPreview();
	
// Dialog Data
	//{{AFX_DATA(CDlgObjectPreview)
	enum { IDD = IDD_OBJECT_PREVIEW };
	CB3ShowRaytrace	m_PreviewMaterialCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjectPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjectPreview)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJECTPREVIEW_H__4F9E06D4_3ABC_4D28_BE05_490FF5888DE5__INCLUDED_)
