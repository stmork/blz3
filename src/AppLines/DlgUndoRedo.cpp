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
**	Revision 1.1  2003/01/30 16:19:58  sm
**	- Added undo/redo list support.
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgUndoRedo implementation                   **
**                                                                      **
*************************************************************************/

CDlgUndoRedo::CDlgUndoRedo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUndoRedo::IDD, pParent)
{
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUndoRedo message handlers

BOOL CDlgUndoRedo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
