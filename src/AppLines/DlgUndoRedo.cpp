/*
**
**	$Filename:	DlgUndoRedo.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for Undo/Redo selection
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "DlgUndoRedo.h"

/*************************************************************************
**                                                                      **
**                        CDlgUndoRedo implementation                   **
**                                                                      **
*************************************************************************/

CDlgUndoRedo::CDlgUndoRedo(
	b3LinesUndoBuffer *buffer,
	b3_list_mode       mode,
	CRect             *rect,
	CWnd              *pParent /*=NULL*/)
	: CDialog(CDlgUndoRedo::IDD, pParent)
{
	m_UndoBuffer = buffer;
	m_ListMode   = mode;
	m_ParentRect = rect;
	//{{AFX_DATA_INIT(CDlgUndoRedo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgUndoRedo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUndoRedo)
	DDX_Control(pDX, IDC_UNDO_REDO_LIST, m_OpList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUndoRedo, CDialog)
	//{{AFX_MSG_MAP(CDlgUndoRedo)
	ON_LBN_SELCHANGE(IDC_UNDO_REDO_LIST, OnSelectOperation)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUndoRedo message handlers

int CDlgUndoRedo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CDlgUndoRedo::OnInitDialog() 
{
	CString title;

	if (m_ParentRect != null)
	{
		SetWindowPos(null,
			m_ParentRect->left,
			m_ParentRect->bottom,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_ListMode)
	{
	case B3_LIST_UNDO:
		m_UndoBuffer->b3FillUndoList(m_OpList);
		title.LoadString(IDS_TITLE_UNDO);
		break;

	case B3_LIST_REDO:
		m_UndoBuffer->b3FillRedoList(m_OpList);
		title.LoadString(IDS_TITLE_REDO);
		break;
	}
	SetWindowText(title);
	m_OpList.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUndoRedo::OnSelectOperation() 
{
	// TODO: Add your control notification handler code here
	b3UndoOperation *op;
	int              index;

	index = m_OpList.GetCurSel();
	if (index != LB_ERR)
	{
		op = (b3UndoOperation *)m_OpList.GetItemDataPtr(index);
		switch(m_ListMode)
		{
		case B3_LIST_UNDO:
			m_UndoBuffer->b3UndoList(op);
			break;
		case B3_LIST_REDO:
			m_UndoBuffer->b3RedoList(op);
			break;
		}
	}

	CDialog::OnOK();
}

LRESULT CDlgUndoRedo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((message == WM_NCACTIVATE) && (wParam == 0))
	{
		CDialog::OnCancel();
		return FALSE;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CDlgUndoRedo::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreCreateWindow(cs);
}
