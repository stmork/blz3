/*
**
**	$Filename:	DlgAnimation.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Animation properties
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "DlgAnimation.h"
#include "blz3/raytrace/b3Special.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/05/06 08:38:32  sm
**	- Demerged raytracing includes of Lines
**
**	Revision 1.2  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.1  2002/08/23 11:35:23  sm
**	- Added motion blur raytracing. The image creation looks very
**	  nice! The algorithm is not as efficient as it could be.
**	
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgAnimation implementation                  **
**                                                                      **
*************************************************************************/

CDlgAnimation::CDlgAnimation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnimation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnimation)
	m_Enable = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgAnimation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnimation)
	DDX_Check(pDX, IDC_ANIM_ON, m_Enable);
	DDX_Control(pDX, IDC_FPS_SPIN, m_CtrlFPS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnimation, CDialog)
	//{{AFX_MSG_MAP(CDlgAnimation)
	ON_BN_CLICKED(IDC_ANIM_ON, OnAnimOn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnimation message handlers

BOOL CDlgAnimation::OnInitDialog() 
{
	m_Enable = m_Animation->b3IsActive();
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_CtrlFPS.b3SetRange(1,100);
	m_CtrlFPS.b3SetPos(m_Animation->m_FramesPerSecond);
	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAnimation::OnAnimOn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	b3UpdateUI();
}

void CDlgAnimation::b3UpdateUI()
{
	GetDlgItem(IDC_FPS)->EnableWindow(m_Enable);
	GetDlgItem(IDC_FPS_SPIN)->EnableWindow(m_Enable);
}

void CDlgAnimation::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	m_Animation->b3Activate(m_Enable);
	m_Animation->m_FramesPerSecond = m_CtrlFPS.b3GetPos();
}
