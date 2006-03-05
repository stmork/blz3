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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.7  2004/05/06 08:38:32  sm
**	- Demerged raytracing includes of Lines
**	
**	Revision 1.6  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.5  2002/03/08 16:46:14  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
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
