/*
**
**	$Filename:	DlgAction.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Acting with active objects
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
#include "DlgAction.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/01/08 15:49:42  sm
**	- Missed new repeating buttons...
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgAction implementation                     **
**                                                                      **
*************************************************************************/

CDlgAction::CDlgAction(CWnd* pParent /*=NULL*/)
	: CB3Dialogbar()
{
	//{{AFX_DATA_INIT(CDlgAction)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAction::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAction)
	DDX_Control(pDX, IDC_MOVE_LEFT, m_CtrlMoveLeft);
	DDX_Control(pDX, IDC_MOVE_RIGHT, m_CtrlMoveRight);
	DDX_Control(pDX, IDC_MOVE_UP, m_CtrlMoveUp);
	DDX_Control(pDX, IDC_MOVE_DOWN, m_CtrlMoveDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAction, CB3Dialogbar)
	//{{AFX_MSG_MAP(CDlgAction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAction message handlers

void CDlgAction::b3GetData()
{
}

void CDlgAction::b3SetData()
{
}
