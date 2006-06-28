/*
**
**	$Filename:	DlgLensFlare.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3LensFlare properties
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

#include "DlgLensFlare.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        CDlgLensFlare implementation                  **
**                                                                      **
*************************************************************************/

CDlgLensFlare::CDlgLensFlare(b3_u32 class_type,CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgLensFlare::IDD)
{
	m_ShadingClassType = class_type;
	//{{AFX_DATA_INIT(CDlgLensFlare)
	m_ActLensFlare = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgLensFlare::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLensFlare)
	DDX_Control(pDX, IDC_LF_PREVIEW, m_LensFlarePreviewCtrl);
	DDX_Control(pDX, IDC_LF_COLOR, m_LensFlareColorCtrl);
	DDX_Control(pDX, IDC_LF_VALUE, m_LensFlareExponCtrl);
	DDX_Check(pDX, IDC_ACT_LENS_FLARE, m_ActLensFlare);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLensFlare, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgLensFlare)
	ON_BN_CLICKED(IDC_LF_CHANGE, OnLensFlareColorChange)
	ON_BN_CLICKED(IDC_ACT_LENS_FLARE, OnActLensFlare)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_LF_VALUE, OnLensFlareExponChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLensFlare message handlers

BOOL CDlgLensFlare::OnInitDialog() 
{
	b3Light *light;

	m_LensFlareScene = b3ExampleScene::b3CreateGlobal(m_ShadingClassType);
	m_EditLensFlare  = new b3LensFlare(LENSFLARE);
	m_EditLensFlare->m_Color = m_LensFlare->m_Color;
	m_EditLensFlare->m_Expon = m_LensFlare->m_Expon;
	m_EditLensFlare->b3Activate(m_LensFlare->b3IsActive());
	m_LensFlareScene->b3GetSpecialHead()->b3Append(m_EditLensFlare);
	m_LensFlareScene->m_BackgroundType = TP_NOTHING;
	light = m_LensFlareScene->b3GetLight(true);
	light->m_Position.x =  400;
	light->m_Position.y =  200;
	light->m_Position.z =  100;
	light->m_Distance   = 1500;

	m_ActLensFlare = m_EditLensFlare->b3IsActive();

	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LensFlareExponCtrl.b3SetMin(b3Scene::epsilon);
	m_LensFlareExponCtrl.b3SetDigits(3,0);
	m_LensFlareExponCtrl.b3SetPos(fabs(m_LensFlare->m_Expon));
	m_LensFlarePreviewCtrl.b3Update(m_LensFlareScene);
	m_LensFlareColorCtrl.b3Init(&m_EditLensFlare->m_Color,this);
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLensFlare::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_LensFlareScene;
}

void CDlgLensFlare::OnLensFlareColorChange() 
{
	// TODO: Add your control notification handler code here
	if (m_LensFlareColorCtrl.b3Select())
	{
		m_LensFlarePreviewCtrl.b3Update(m_LensFlareScene);
	}
}

void CDlgLensFlare::OnActLensFlare() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_EditLensFlare->b3Activate(m_ActLensFlare);
	m_LensFlarePreviewCtrl.b3Update(m_LensFlareScene);
	b3UpdateUI();
}

void CDlgLensFlare::b3UpdateUI()
{
	CWnd *distWnd        = GetDlgItem(IDC_LF_VALUE);
	CWnd *colorSelectWnd = GetDlgItem(IDC_LF_CHANGE);

	distWnd->EnableWindow(m_ActLensFlare);
	colorSelectWnd->EnableWindow(m_ActLensFlare);
}

BOOL CDlgLensFlare::OnApply()
{
	// TODO: Add extra validation here
	m_LensFlare->m_Expon = m_LensFlareExponCtrl.b3GetPos();
	m_LensFlare->m_Color = m_EditLensFlare->m_Color;
	m_LensFlare->b3Activate(m_ActLensFlare);
	return CPropertyPage::OnApply();
}

void CDlgLensFlare::OnLensFlareExponChanged() 
{
	// TODO: Add your control notification handler code here
	m_EditLensFlare->m_Expon = m_LensFlareExponCtrl.b3GetPos();
	m_LensFlarePreviewCtrl.b3Update(m_LensFlareScene);
}
