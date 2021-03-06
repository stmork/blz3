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

#include "AppLinesInclude.h"

#include "DlgAnimation.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgAnimation implementation                  **
**                                                                      **
*************************************************************************/

CDlgAnimation::CDlgAnimation(b3Item *item, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnimation::IDD, pParent)
{
	m_Animation = static_cast<b3Animation *>(item);

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

b3_bool CDlgAnimation::b3Edit(b3Item *item,void *ptr)
{
	CDlgAnimation dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgAnimation::b3Register()
{
	b3Loader::b3AddClassType(ANIMATION,IDS_ITEMDESC_ANIMATION,IDI_BUMP_GLOSSY,b3Edit,b3Edit);
}
