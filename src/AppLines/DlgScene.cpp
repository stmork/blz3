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
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
#include "b3SelectColor.h"
#include "b3SelectTexture.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.19  2004/05/16 09:21:11  sm
**	- Fixed ticket no. 22: Camera deletions are handled
**	  correctly now
**
**	Revision 1.18  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.17  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**	
**	Revision 1.16  2004/04/03 14:07:18  sm
**	- Resolved internal compiler error problem of VC++
**	
**	Revision 1.15  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.14  2003/03/04 20:37:36  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.13  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.12  2003/02/09 13:58:14  sm
**	- cleaned up file selection dialogs
**	
**	Revision 1.11  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.10  2002/08/10 16:07:46  sm
**	- Added some OS version output
**	- Corrected language specifiers for version output.
**	- Changed CDlgScene CSpinButtonCtrl to CB3IntSpinButtonCtrl
**	  to avoid thousands point.
**	
**	Revision 1.9  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.8  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.7  2002/02/20 20:23:57  sm
**	- Some type cleanups done.
**	
**	Revision 1.6  2002/01/08 16:04:08  sm
**	- New copy dialog added
**	- Merge with daily work
**	
**	Revision 1.5  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**	Revision 1.4  2001/11/11 11:51:20  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**	Revision 1.3  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
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

static b3_bg_type dialog_to_scene[4] =
{
	TP_NOTHING,
	TP_SLIDE,
	TP_SKY_N_HELL,
	TP_TEXTURE
};

static struct b3_resolution
{
	b3_res xRes,yRes;
}
resolution[] =
{
	{    0,    0 },
	{  768,  576 },
	{  704,  576 },
	{  352,  288 },
	{ 2048, 1536 },
	{ 1600, 1200 },
	{ 1024,  768 },
	{  640,  480 },
	{  800,  600 },
	{  400,  300 },
	{  200,  150 }
};

CDlgScene::CDlgScene(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgScene::IDD)
{
	//{{AFX_DATA_INIT(CDlgScene)
	m_RayDepthLegend = _T("");
	m_ShadowBrightnessLegend = _T("");
	m_BackgroundMode = 0;
	m_ResValid = false;
	m_GfxValid = FALSE;
	m_Resolution = 0;
	//}}AFX_DATA_INIT
	m_LastTraceDepth = -1;
	m_LastShadowBrightness = -1;
}


void CDlgScene::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScene)
	DDX_Control(pDX, IDC_PREVIEW_RAYTRACE, m_PreviewSceneCtrl);
	DDX_Control(pDX, IDC_PREVIEW_BGIMAGE, m_PreviewImageCtrl);
	DDX_Control(pDX, IDC_RAYDEPTH, m_RayDepthCtrl);
	DDX_Control(pDX, IDC_SHADOW_BRIGHTNESS, m_ShadowBrightnessCtrl);
	DDX_Text(pDX, IDC_RAYDEPTH_LEGEND, m_RayDepthLegend);
	DDX_Text(pDX, IDC_SHADOW_BRIGHTNESS_LEGEND, m_ShadowBrightnessLegend);
	DDX_Control(pDX, IDC_YRES_SPIN, m_yResSpin);
	DDX_Control(pDX, IDC_XRES_SPIN, m_xResSpin);
	DDX_Radio(pDX, IDC_BG_LIGHT, m_BackgroundMode);
	DDX_Check(pDX, IDC_RES_VALID, m_ResValid);
	DDX_Check(pDX, IDC_OPEN_GFX, m_GfxValid);
	DDX_CBIndex(pDX, IDC_RES, m_Resolution);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScene, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgScene)
	ON_BN_CLICKED(IDC_BG_LIGHT, OnBgModeChanged)
	ON_BN_CLICKED(IDC_BG_IMAGE_SELECT, OnBgImageSelect)
	ON_BN_CLICKED(IDC_RES_VALID, OnResValid)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BG_TOP_SELECT, OnTopColor)
	ON_BN_CLICKED(IDC_BG_BOTTOM_SELECT, OnBottomColor)
	ON_CBN_SELCHANGE(IDC_RES, OnChangedResolution)
	ON_BN_CLICKED(IDC_BG_COLOR, OnBgModeChanged)
	ON_BN_CLICKED(IDC_BG_SKY, OnBgModeChanged)
	ON_BN_CLICKED(IDC_BG_IMAGE, OnBgModeChanged)
	ON_EN_KILLFOCUS(IDC_XRES, OnEditedResolution)
	ON_EN_KILLFOCUS(IDC_YRES, OnEditedResolution)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScene message handlers

BOOL CDlgScene::OnInitDialog() 
{
	m_ResValid = (m_Scene->m_Flags & TP_SIZEVALID) != 0;
	m_GfxValid = (m_Scene->m_Flags & TP_NO_GFX) == 0;
	m_BackgroundMode = scene_to_dialog[m_Scene->m_BackgroundType];
	b3SetResolution(m_Scene->m_xSize,m_Scene->m_ySize);

	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PreviewScene = b3ExampleScene::b3CreateGlobal();
	m_PreviewScene->m_Flags          = m_Scene->m_Flags;
	m_PreviewScene->m_BackgroundType = m_Scene->m_BackgroundType;
	m_PreviewScene->m_TopColor       = m_Scene->m_TopColor;
	m_PreviewScene->m_BottomColor    = m_Scene->m_BottomColor;
	m_PreviewScene->b3SetTexture(m_Scene->m_TextureName);
	m_PreviewSceneCtrl.b3Update(m_PreviewScene);
	b3Scene::b3CheckTexture(&m_PreviewScene->m_BackTexture,m_PreviewScene->m_TextureName);
	m_PreviewImageCtrl.b3Copy(m_PreviewScene->m_BackTexture);
	m_PreviewImageCtrl.b3Update(true,true);

	m_xResSpin.b3SetRange(16,16383);
	m_yResSpin.b3SetRange(16,16383);
	m_xResSpin.b3SetPos(m_Scene->m_xSize);
	m_yResSpin.b3SetPos(m_Scene->m_ySize);
	
	m_RayDepthCtrl.SetRangeMin (  1);
	m_RayDepthCtrl.SetRangeMax ( 10);
	m_RayDepthCtrl.SetPageSize (  1);
	m_RayDepthCtrl.SetTicFreq  (  1);
	m_RayDepthCtrl.SetPos      (m_Scene->m_TraceDepth);

	m_ShadowBrightnessCtrl.SetRangeMin (  0);
	m_ShadowBrightnessCtrl.SetRangeMax (100);
	m_ShadowBrightnessCtrl.SetPageSize (  1);
	m_ShadowBrightnessCtrl.SetTicFreq  ( 10);
	m_ShadowBrightnessCtrl.SetPos      (m_Scene->m_ShadowBrightness * 100);

	b3UpdateUI();
	b3PrintLegend();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgScene::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	b3PrintLegend();
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CDlgScene::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_PreviewScene;
}

void CDlgScene::OnBgModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_PreviewScene->m_BackgroundType = dialog_to_scene[m_BackgroundMode];
	m_PreviewSceneCtrl.b3Update(m_PreviewScene);
	b3UpdateUI();
}

void CDlgScene::OnBgImageSelect() 
{
	// TODO: Add your control notification handler code here
	if (CB3SelectLoadTexture::b3Select(&m_PreviewScene->m_BackTexture,m_PreviewScene->m_TextureName))
	{
		m_PreviewImageCtrl.b3Copy(m_PreviewScene->m_BackTexture);
		m_PreviewSceneCtrl.b3Update(m_PreviewScene);
		m_PreviewImageCtrl.b3Update(true,true);
	}
}

void CDlgScene::OnTopColor() 
{
	// TODO: Add your control notification handler code here
	if (CB3ColorFieldSelector::b3Select(m_PreviewScene->m_TopColor,this))
	{
		m_PreviewSceneCtrl.b3Update(m_PreviewScene);
	}
}

void CDlgScene::OnBottomColor() 
{
	// TODO: Add your control notification handler code here
	if (CB3ColorFieldSelector::b3Select(m_PreviewScene->m_BottomColor,this))
	{
		m_PreviewSceneCtrl.b3Update(m_PreviewScene);
	}
}

void CDlgScene::OnResValid() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	b3UpdateUI();
}

void CDlgScene::b3PrintLegend()
{
	int pos;
	bool update = false;

	pos = m_RayDepthCtrl.GetPos();
	if (pos != m_LastTraceDepth)
	{
		m_LastTraceDepth = pos;
		m_RayDepthLegend.Format(IDS_RAYDEPTH,pos);
		m_PreviewScene->m_TraceDepth = pos;
		update = true;
	}

	pos = m_ShadowBrightnessCtrl.GetPos();
	if (pos != m_LastShadowBrightness)
	{
		m_LastShadowBrightness = pos;
		m_ShadowBrightnessLegend.Format(IDS_SHADOW_BRIGHTNESS,pos);
		m_PreviewScene->m_ShadowBrightness = pos / 100.0;
		update = true;
	}

	if (update)
	{
		UpdateData(FALSE);
	}
}

void CDlgScene::b3UpdateUI()
{
	CWnd *xResWnd      = GetDlgItem(IDC_XRES);
	CWnd *yResWnd      = GetDlgItem(IDC_YRES);
	CWnd *imgSelect    = GetDlgItem(IDC_BG_IMAGE_SELECT);
	CWnd *topSelect    = GetDlgItem(IDC_BG_TOP_SELECT);
	CWnd *bottomSelect = GetDlgItem(IDC_BG_BOTTOM_SELECT);

	xResWnd->EnableWindow(m_ResValid);
	yResWnd->EnableWindow(m_ResValid);
	m_xResSpin.EnableWindow(m_ResValid);
	m_yResSpin.EnableWindow(m_ResValid);
	imgSelect->EnableWindow(m_BackgroundMode == 3);
	topSelect->EnableWindow((m_BackgroundMode == 1) || (m_BackgroundMode == 2));
	bottomSelect->EnableWindow((m_BackgroundMode == 1) || (m_BackgroundMode == 2));
}

void CDlgScene::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
	m_Scene->m_BackgroundType   = m_PreviewScene->m_BackgroundType;
	m_Scene->m_TopColor         = m_PreviewScene->m_TopColor;
	m_Scene->m_BottomColor      = m_PreviewScene->m_BottomColor;
	m_Scene->m_xSize            = m_xResSpin.b3GetPos();
	m_Scene->m_ySize            = m_yResSpin.b3GetPos();
	m_Scene->m_TraceDepth       = m_RayDepthCtrl.GetPos();
	m_Scene->m_ShadowBrightness = m_ShadowBrightnessCtrl.GetPos() / 100.0;
	m_Scene->m_Flags            =
		(m_ResValid ? TP_SIZEVALID : 0) |
		(m_GfxValid ? 0 : TP_NO_GFX);
	m_Scene->b3SetTexture(m_PreviewScene->m_TextureName);
}

void CDlgScene::OnChangedResolution() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_xResSpin.b3SetPos(resolution[m_Resolution].xRes);
	m_yResSpin.b3SetPos(resolution[m_Resolution].yRes);
}

void CDlgScene::OnEditedResolution() 
{
	// TODO: Add your control notification handler code here
	b3_res   xRes = m_xResSpin.b3GetPos();
	b3_res   yRes = m_yResSpin.b3GetPos();

	if (b3SetResolution(xRes,yRes))
	{
		UpdateData(FALSE);
	}
}

b3_bool CDlgScene::b3SetResolution(b3_res xRes,b3_res yRes)
{
	b3_index i,max = sizeof(resolution) / sizeof(b3_resolution);
	b3_index pos = 0;
	b3_bool  changed = false;

	for (i = 1;i < max;i++)
	{
		if ((resolution[i].xRes == xRes) && (resolution[i].yRes == yRes))
		{
			pos = i;
			break;
		}
	}
	if (pos != m_Resolution)
	{
		m_Resolution = pos;
		changed = true;
	}
	return changed;
}
