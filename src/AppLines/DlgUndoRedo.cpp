/*
**
**	$Filename:	DlgUndoRedo.cpp $
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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "DlgUndoRedo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/01/30 19:49:55  sm
**	- Further undo/redo history dialog build.
**
**	Revision 1.1  2003/01/30 16:19:58  sm
**	- Added undo/redo list support.
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgUndoRedo implementation                   **
**                                                                      **
*************************************************************************/

CDlgUndoRedo::CDlgUndoRedo(
	b3LinesUndoBuffer *buffer,
	b3_list_mode       mode,
	CWnd              *pParent /*=NULL*/)
	: CDialog(CDlgUndoRedo::IDD, pParent)
{
	m_UndoBuffer = buffer;
	m_ListMode   = mode;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUndoRedo message handlers

BOOL CDlgUndoRedo::OnInitDialog() 
{
	CString title;

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
