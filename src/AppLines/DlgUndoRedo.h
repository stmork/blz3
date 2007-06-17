/*
**
**	$Filename:	DlgUndoRedo.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for Undo/Redo selection
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DGLUNDOREDO_H__FEC9DD3D_EACD_4530_B77B_9C7197F54A82__INCLUDED_)
#define AFX_DGLUNDOREDO_H__FEC9DD3D_EACD_4530_B77B_9C7197F54A82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUndoRedo.h : header file
//

#include "b3Undo.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUndoRedo dialog

enum b3_list_mode
{
	B3_LIST_UNDO,B3_LIST_REDO
};

class CDlgUndoRedo : public CDialog
{
	CRect             *m_ParentRect;
	b3LinesUndoBuffer *m_UndoBuffer;
	b3_list_mode       m_ListMode;

// Construction
public:
	CDlgUndoRedo(
		b3LinesUndoBuffer *buffer,
		b3_list_mode       mode,
		CRect             *rect = null,
		CWnd              *pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUndoRedo)
	enum { IDD = IDD_UNDO_REDO };
	CListBox	m_OpList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUndoRedo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUndoRedo)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectOperation();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3FillList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGLUNDOREDO_H__FEC9DD3D_EACD_4530_B77B_9C7197F54A82__INCLUDED_)
