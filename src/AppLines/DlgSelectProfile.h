/*
**
**	$Filename:	DlgSelectProfile.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select profile for triangle or B spline shape
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSELECTPROFILE_H__573DA1F9_8653_4AF9_83DF_6DC0024C6688__INCLUDED_)
#define AFX_DLGSELECTPROFILE_H__573DA1F9_8653_4AF9_83DF_6DC0024C6688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectProfile.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"
#include "b3ProfileShapeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProfile dialog

class CDlgSelectProfile : public CDialog
{
	CString         m_Section;
public:
	b3Shape        *m_Shape;
	CB3ShapeDialog *m_ShapeCreator;

public:
	CDlgSelectProfile(CWnd* pParent = NULL);   // standard constructor

	static  int  b3Edit(b3Item *item,b3_bool create=true);
// Dialog Data
	//{{AFX_DATA(CDlgSelectProfile)
	enum { IDD = IDD_CREATE_SHAPE_PROFILE };
	CListCtrl	m_ListCtrl;
	int		m_CreateMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectProfile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectProfile)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageList m_ImageList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTPROFILE_H__573DA1F9_8653_4AF9_83DF_6DC0024C6688__INCLUDED_)
