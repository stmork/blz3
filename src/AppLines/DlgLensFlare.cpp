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

#include "AppLines.h"
#include "DlgLensFlare.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**	Revision 1.4  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.3  2001/11/18 13:49:26  sm
**	- Introduced new CB3FloatEdit derived from CEdit
**	- DlgNebular implemented
**	- DlgLensFlare implemented
**	- Adjusting far clipping plane inside b3RenderView
**	
**	Revision 1.2  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.1  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgLensFlare implementation                  **
**                                                                      **
*************************************************************************/

CDlgLensFlare::CDlgLensFlare(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgLensFlare::IDD)
{
	//{{AFX_DATA_INIT(CDlgLensFlare)
	m_ActLensFlare = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgLensFlare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLensFlare)
	DDX_Control(pDX, IDC_LF_PREVIEW, m_LensFlarePreviewCtrl);
	DDX_Control(pDX, IDC_LF_COLOR, m_LensFlareColorCtrl);
	DDX_Control(pDX, IDC_LF_VALUE, m_LensFlareExponCtrl);
	DDX_Check(pDX, IDC_ACT_LENS_FLARE, m_ActLensFlare);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLensFlare, CDialog)
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
	m_LensFlareScene = b3ExampleScene::b3CreateGlobal();
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

	CDialog::OnInitDialog();
	
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
	CDialog::OnDestroy();
	
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
