/*
**
**	$Filename:	DlgStepMove.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setting move steps
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "AppLines.h"
#include "DlgStepMove.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgStepMove implementation                   **
**                                                                      **
*************************************************************************/

CDlgStepMove::CDlgStepMove(CWnd* pParent /*=NULL*/)
	: CB3Dialogbar()
{
	//{{AFX_DATA_INIT(CDlgStepMove)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Info = null;
}


void CDlgStepMove::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStepMove)
	DDX_Control(pDX, IDC_STEP_MOVE_Z, m_zCtrl);
	DDX_Control(pDX, IDC_STEP_MOVE_Y, m_yCtrl);
	DDX_Control(pDX, IDC_STEP_MOVE_X, m_xCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStepMove, CB3Dialogbar)
	//{{AFX_MSG_MAP(CDlgStepMove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStepMove message handlers

BOOL CDlgStepMove::OnInitDialogBar()
{
	if (!CB3Dialogbar::OnInitDialogBar())
	{
		return FALSE;
	}
	m_xCtrl.b3SetMin(epsilon);
	m_yCtrl.b3SetMin(epsilon);
	m_zCtrl.b3SetMin(epsilon);
	m_xCtrl.b3SetDigits(5,2);
	m_yCtrl.b3SetDigits(5,2);
	m_zCtrl.b3SetDigits(5,2);
	return TRUE;
}

void CDlgStepMove::b3GetData()
{
	if (m_Info != null)
	{
		m_Info->m_StepMove.x = m_xCtrl.m_Value;
		m_Info->m_StepMove.y = m_yCtrl.m_Value;
		m_Info->m_StepMove.z = m_zCtrl.m_Value;
	}
}

void CDlgStepMove::b3SetData()
{
	m_xCtrl.EnableWindow(m_Info != null);
	m_yCtrl.EnableWindow(m_Info != null);
	m_zCtrl.EnableWindow(m_Info != null);
	if (m_Info != null)
	{
		m_xCtrl.b3SetValue(m_Info->m_StepMove.x);
		m_yCtrl.b3SetValue(m_Info->m_StepMove.y);
		m_zCtrl.b3SetValue(m_Info->m_StepMove.z);
	}
}
