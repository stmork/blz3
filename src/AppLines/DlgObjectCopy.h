/*
**
**	$Filename:	DlgObjectCopy.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for copying objects
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGOBJECTCOPY_H__4DB0D317_038A_11D6_8A1B_00E018258F1A__INCLUDED_)
#define AFX_DLGOBJECTCOPY_H__4DB0D317_038A_11D6_8A1B_00E018258F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjectCopy.h : header file
//

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectCopy dialog

class CDlgObjectCopy : public CDialog
{
// Construction
public:
	b3_vector  m_Move;
	b3_vector  m_Rotate;
	b3_vector *m_Center;
	b3BBox    *m_OrigBBox;
	CDlgObjectCopy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjectCopy)
	enum { IDD = IDD_OBJECT_COPY };
	CB3FloatEdit	m_xCtrlOffset;
	CB3FloatEdit	m_yCtrlOffset;
	CB3FloatEdit	m_zCtrlOffset;
	CB3FloatEdit	m_xCtrlRot;
	CB3FloatEdit	m_yCtrlRot;
	CB3FloatEdit	m_zCtrlRot;
	CB3FloatEdit	m_xCtrlCenter;
	CB3FloatEdit	m_yCtrlCenter;
	CB3FloatEdit	m_zCtrlCenter;
	b3_count	m_NumCopies;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjectCopy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjectCopy)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJECTCOPY_H__4DB0D317_038A_11D6_8A1B_00E018258F1A__INCLUDED_)
