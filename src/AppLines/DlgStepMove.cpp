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
**	Revision 1.4  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**
**	Revision 1.3  2001/12/31 16:39:40  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.2  2001/12/26 18:17:56  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
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
	CWinApp *app = AfxGetApp();

	m_CtrlMoveLeft.m_hIcon  = app->LoadIcon(IDI_LEFT);
	m_CtrlMoveRight.m_hIcon = app->LoadIcon(IDI_RIGHT);
	m_CtrlMoveUp.m_hIcon    = app->LoadIcon(IDI_UP);
	m_CtrlMoveDown.m_hIcon  = app->LoadIcon(IDI_DOWN);
	m_pDoc = null;
}


void CDlgStepMove::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStepMove)
	DDX_Control(pDX, IDC_STEP_MOVE_Z, m_zCtrl);
	DDX_Control(pDX, IDC_STEP_MOVE_Y, m_yCtrl);
	DDX_Control(pDX, IDC_STEP_MOVE_X, m_xCtrl);
	DDX_Control(pDX, IDC_MOVE_LEFT, m_CtrlMoveLeft);
	DDX_Control(pDX, IDC_MOVE_RIGHT, m_CtrlMoveRight);
	DDX_Control(pDX, IDC_MOVE_UP, m_CtrlMoveUp);
	DDX_Control(pDX, IDC_MOVE_DOWN, m_CtrlMoveDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStepMove, CB3Dialogbar)
	//{{AFX_MSG_MAP(CDlgStepMove)
	ON_EN_KILLFOCUS(IDC_STEP_MOVE_X, OnKillfocusStep)
	ON_EN_KILLFOCUS(IDC_STEP_MOVE_Y, OnKillfocusStep)
	ON_EN_KILLFOCUS(IDC_STEP_MOVE_Z, OnKillfocusStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStepMove message handlers

BOOL CDlgStepMove::OnInitDialog()
{
	if (!CB3Dialogbar::OnInitDialog())
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
	b3ModellerInfo *info = m_pDoc != null ? m_pDoc->m_Info : null;

	if (info != null)
	{
		info->m_StepMove.x = m_xCtrl.m_Value;
		info->m_StepMove.y = m_yCtrl.m_Value;
		info->m_StepMove.z = m_zCtrl.m_Value;
	}
}

void CDlgStepMove::b3SetData()
{
	b3ModellerInfo *info = m_pDoc != null ? m_pDoc->m_Info : null;

	m_xCtrl.EnableWindow(info != null);
	m_yCtrl.EnableWindow(info != null);
	m_zCtrl.EnableWindow(info != null);
	if (info != null)
	{
		m_xCtrl.b3SetValue(info->m_StepMove.x);
		m_yCtrl.b3SetValue(info->m_StepMove.y);
		m_zCtrl.b3SetValue(info->m_StepMove.z);
	}
	else
	{
		m_xCtrl.SetWindowText("");
		m_yCtrl.SetWindowText("");
		m_zCtrl.SetWindowText("");
	}
}

void CDlgStepMove::OnKillfocusStep() 
{
	// TODO: Add your control notification handler code here
	b3GetData();
	if (m_pDoc != null)
	{
		m_pDoc->SetModifiedFlag();
	}
}
