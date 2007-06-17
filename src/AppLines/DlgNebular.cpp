/*
**
**	$Filename:	DlgNebular.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Nebular properties
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

#include "DlgNebular.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        CDlgNebular implementation                    **
**                                                                      **
*************************************************************************/

CDlgNebular::CDlgNebular(b3_u32 class_type,CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgNebular::IDD)
{
	m_ShadingClassType = class_type;
	//{{AFX_DATA_INIT(CDlgNebular)
	m_ActNebular = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgNebular::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNebular)
	DDX_Control(pDX, IDC_NEB_PREVIEW, m_NebularPreviewCtrl);
	DDX_Control(pDX, IDC_NEB_DISTANCE, m_NebularDistanceCtrl);
	DDX_Control(pDX, IDC_NEB_COLOR, m_NebularColorCtrl);
	DDX_Check(pDX, IDC_ACT_NEBULAR, m_ActNebular);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNebular, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgNebular)
	ON_BN_CLICKED(ID_NEB_CHANGE, OnNebularColorChange)
	ON_BN_CLICKED(IDC_ACT_NEBULAR, OnActNebular)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNebular message handlers

BOOL CDlgNebular::OnInitDialog() 
{
	m_NebularScene = b3ExampleScene::b3CreateGlobal(m_ShadingClassType);
	m_EditNebular = new b3Nebular(NEBULAR);
	m_EditNebular->m_NebularColor = m_Nebular->m_NebularColor;
	m_EditNebular->m_NebularVal   = 100;
	m_EditNebular->b3Activate(m_Nebular->b3IsActive());
	m_NebularScene->b3GetSpecialHead()->b3Append(m_EditNebular);

	m_ActNebular = m_EditNebular->b3IsActive();

	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_NebularDistanceCtrl.b3SetMin(b3Scene::epsilon);
	m_NebularDistanceCtrl.b3SetDigits(3,0);
	m_NebularDistanceCtrl.b3SetPos(fabs(m_Nebular->m_NebularVal));
	m_NebularPreviewCtrl.b3Update(m_NebularScene);
	m_NebularColorCtrl.b3Init(&m_EditNebular->m_NebularColor,this);
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNebular::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_NebularScene;
}

void CDlgNebular::OnNebularColorChange() 
{
	// TODO: Add your control notification handler code here
	if (m_NebularColorCtrl.b3Select())
	{
		m_NebularPreviewCtrl.b3Update(m_NebularScene);
	}
}

void CDlgNebular::OnActNebular() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_EditNebular->b3Activate(m_ActNebular);
	m_NebularPreviewCtrl.b3Update(m_NebularScene);
	b3UpdateUI();
}

void CDlgNebular::b3UpdateUI()
{
	CWnd *distWnd        = GetDlgItem(IDC_NEB_DISTANCE);
	CWnd *colorSelectWnd = GetDlgItem(ID_NEB_CHANGE);

	distWnd->EnableWindow(m_ActNebular);
	colorSelectWnd->EnableWindow(m_ActNebular);
}

BOOL CDlgNebular::OnApply()
{
	// TODO: Add extra validation here
	m_Nebular->m_NebularVal   = m_NebularDistanceCtrl.b3GetPos();
	m_Nebular->m_NebularColor = m_EditNebular->m_NebularColor;
	m_Nebular->b3Activate(m_ActNebular);
	return CPropertyPage::OnApply();
}
