/*
**
**	$Filename:	DlgStepRotate.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setting angle steps
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

#include "DlgStepRotate.h"

/*************************************************************************
**                                                                      **
**                        CDlgStepRotate implementation                 **
**                                                                      **
*************************************************************************/

CDlgStepRotate::CDlgStepRotate(CWnd* pParent /*=NULL*/)
	: CB3Dialogbar()
{
	//{{AFX_DATA_INIT(CDlgStepRotate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CWinApp *app = AfxGetApp();

	m_CtrlRotLeft.m_hIcon   = app->LoadIcon(IDI_ROT_LEFT);
	m_CtrlRotRight.m_hIcon  = app->LoadIcon(IDI_ROT_RIGHT);
	m_pDoc = null;

	// Init float edit controls
	m_xCtrl.b3SetMin(b3Scene::epsilon);
	m_yCtrl.b3SetMin(b3Scene::epsilon);
	m_zCtrl.b3SetMin(b3Scene::epsilon);
	m_xCtrl.b3SetMax(180);
	m_yCtrl.b3SetMax(180);
	m_zCtrl.b3SetMax(180);
	m_xCtrl.b3SetDigits(5,2);
	m_yCtrl.b3SetDigits(5,2);
	m_zCtrl.b3SetDigits(5,2);
}


void CDlgStepRotate::DoDataExchange(CDataExchange* pDX)
{
	b3ModellerInfo *info = m_pDoc != null ? m_pDoc->m_Info : null;

	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStepRotate)
	DDX_Control(pDX, IDC_STEP_ROT_Z, m_zCtrl);
	DDX_Control(pDX, IDC_STEP_ROT_Y, m_yCtrl);
	DDX_Control(pDX, IDC_STEP_ROT_X, m_xCtrl);
	DDX_Control(pDX, IDC_ROT_RIGHT, m_CtrlRotRight);
	DDX_Control(pDX, IDC_ROT_LEFT, m_CtrlRotLeft);
	//}}AFX_DATA_MAP
	if (info != null)
	{
		m_xCtrl.b3DDX(pDX,info->m_StepRotate.x);
		m_yCtrl.b3DDX(pDX,info->m_StepRotate.y);
		m_zCtrl.b3DDX(pDX,info->m_StepRotate.z);
	}
}


BEGIN_MESSAGE_MAP(CDlgStepRotate, CB3Dialogbar)
	//{{AFX_MSG_MAP(CDlgStepRotate)
	ON_EN_KILLFOCUS(IDC_STEP_ROT_X, OnKillfocusStep)
	ON_EN_KILLFOCUS(IDC_STEP_ROT_Y, OnKillfocusStep)
	ON_EN_KILLFOCUS(IDC_STEP_ROT_Z, OnKillfocusStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStepRotate message handlers

void CDlgStepRotate::b3GetData()
{
	UpdateData();
}

void CDlgStepRotate::b3SetData()
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

void CDlgStepRotate::OnKillfocusStep() 
{
	// TODO: Add your control notification handler code here
	b3GetData();
	if (m_pDoc != null)
	{
		m_pDoc->SetModifiedFlag();
	}
}
