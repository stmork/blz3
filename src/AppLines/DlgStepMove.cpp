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

#include "AppLinesInclude.h"

#include "DlgStepMove.h"

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

	// Init float edit controls
	m_xCtrl.b3SetMin(b3Scene::epsilon);
	m_yCtrl.b3SetMin(b3Scene::epsilon);
	m_zCtrl.b3SetMin(b3Scene::epsilon);
	m_xCtrl.b3SetDigits(5,2);
	m_yCtrl.b3SetDigits(5,2);
	m_zCtrl.b3SetDigits(5,2);
}


void CDlgStepMove::DoDataExchange(CDataExchange* pDX)
{
	b3ModellerInfo *info = m_pDoc != null ? m_pDoc->m_Info : null;

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
	if (info != null)
	{
		m_xCtrl.b3DDX(pDX,info->m_StepMove.x);
		m_yCtrl.b3DDX(pDX,info->m_StepMove.y);
		m_zCtrl.b3DDX(pDX,info->m_StepMove.z);
	}
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

void CDlgStepMove::b3GetData()
{
	UpdateData();
}

void CDlgStepMove::b3SetData()
{
	b3ModellerInfo *info = m_pDoc != null ? m_pDoc->m_Info : null;

	m_xCtrl.EnableWindow(info != null);
	m_yCtrl.EnableWindow(info != null);
	m_zCtrl.EnableWindow(info != null);
	if (info != null)
	{
		UpdateData(FALSE);
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
