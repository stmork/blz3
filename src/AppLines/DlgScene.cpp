/*
**
**	$Filename:	DlgScene.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Scene properties
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
#include "DlgScene.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
**
**	Revision 1.1  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgScene implementation                      **
**                                                                      **
*************************************************************************/

static int scene_to_dialog[4] =
{
	0, // TP_NOTHING
	3, // TP_TEXTURE
	1, // TP_SLIDE
	2  // TP_SKY_N_HELL
};

static int dialog_to_scene[4] =
{
	TP_NOTHING,
	TP_SLIDE,
	TP_SKY_N_HELL,
	TP_TEXTURE
};

CDlgScene::CDlgScene(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScene::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgScene)
	m_ResValid = false;
	m_RayDepthLegend = _T("");
	m_BackgroundMode = 0;
	m_xRes = 240;
	m_yRes = 160;
	//}}AFX_DATA_INIT
	m_LastTraceDepth = -1;
}


void CDlgScene::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScene)
	DDX_Control(pDX, IDC_YRES_SPIN, m_yResSpin);
	DDX_Control(pDX, IDC_XRES_SPIN, m_xResSpin);
	DDX_Control(pDX, IDC_RAYDEPTH, m_RayDepth);
	DDX_Control(pDX, IDC_PREVIEW_RAYTRACE, m_PreviewSceneCtrl);
	DDX_Control(pDX, IDC_PREVIEW_BGIMAGE, m_PreviewImageCtrl);
	DDX_Check(pDX, IDC_RES_VALID, m_ResValid);
	DDX_Text(pDX, IDC_RAYDEPTH_LEGEND, m_RayDepthLegend);
	DDX_Radio(pDX, IDC_BG_LIGHT, m_BackgroundMode);
	DDX_Text(pDX, IDC_XRES, m_xRes);
	DDV_MinMaxInt(pDX, m_xRes, 16, 16383);
	DDX_Text(pDX, IDC_YRES, m_yRes);
	DDV_MinMaxInt(pDX, m_yRes, 16, 16383);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScene, CDialog)
	//{{AFX_MSG_MAP(CDlgScene)
	ON_BN_CLICKED(IDC_BG_LIGHT, OnBgModeChanged)
	ON_BN_CLICKED(IDC_BG_IMAGE_SELECT, OnBgImageSelect)
	ON_BN_CLICKED(IDC_RES_VALID, OnResValid)
	ON_BN_CLICKED(IDC_BG_COLOR, OnBgModeChanged)
	ON_BN_CLICKED(IDC_BG_SKY, OnBgModeChanged)
	ON_BN_CLICKED(IDC_BG_IMAGE, OnBgModeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScene message handlers

BOOL CDlgScene::OnInitDialog() 
{
	m_ResValid = (m_Scene->m_Flags & TP_SIZEVALID) != 0;
	m_BackgroundMode = scene_to_dialog[m_Scene->m_BackgroundType];
	m_xRes = m_Scene->m_xSize;
	m_yRes = m_Scene->m_ySize;

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_xResSpin.SetRange(16,16383);
	m_yResSpin.SetRange(16,16383);
	m_RayDepth.SetRangeMin (  1);
	m_RayDepth.SetRangeMax ( 10);
	m_RayDepth.SetPageSize (  1);
	m_RayDepth.SetTicFreq  (  1);
	m_RayDepth.SetPos      (m_Scene->m_TraceDepth);
	b3UpdateUI();
	b3PrintRayDepth();
	m_PreviewScene = b3ExampleScene::b3GetNull();
	m_PreviewSceneCtrl.b3Update(m_PreviewScene = m_Scene);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgScene::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	b3PrintRayDepth();
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgScene::OnBgModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_Scene->m_BackgroundType = dialog_to_scene[m_BackgroundMode];
	m_PreviewSceneCtrl.b3Update(m_PreviewScene);
	b3UpdateUI();
}

void CDlgScene::OnBgImageSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	b3UpdateUI();
}

void CDlgScene::OnResValid() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	b3UpdateUI();
}

void CDlgScene::b3PrintRayDepth()
{
	int pos = m_RayDepth.GetPos();
	if (pos != m_LastTraceDepth)
	{
		m_LastTraceDepth = pos;
		m_RayDepthLegend.Format(IDS_RAYDEPTH,pos);
		UpdateData(FALSE);
	}
}

void CDlgScene::b3UpdateUI()
{
	CWnd *xResWnd = GetDlgItem(IDC_XRES);
	CWnd *yResWnd = GetDlgItem(IDC_YRES);
	CWnd *imgSelect = GetDlgItem(IDC_BG_IMAGE_SELECT);

	xResWnd->EnableWindow(m_ResValid);
	yResWnd->EnableWindow(m_ResValid);
	m_xResSpin.EnableWindow(m_ResValid);
	m_yResSpin.EnableWindow(m_ResValid);
	imgSelect->EnableWindow(m_BackgroundMode == 3);
}

void CDlgScene::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
