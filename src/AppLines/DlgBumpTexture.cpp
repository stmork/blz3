/*
**
**	$Filename:	DlgBumpTexture.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture bump mapping
**
**	(C) Copyright 2004  Steffen A. Mork
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
#include "DlgBumpTexture.h"
#include "b3SelectTexture.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpTexture::CDlgBumpTexture(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item, CDlgBumpTexture::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBumpTexture)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpTexture::~CDlgBumpTexture()
{
	delete m_BumpSampler;
}

void CDlgBumpTexture::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpTexture)
	DDX_Control(pDX, IDC_X_START_SPIN, m_xStartCtrl);
	DDX_Control(pDX, IDC_X_SCALE_SPIN, m_xScaleCtrl);
	DDX_Control(pDX, IDC_X_TIMES_SPIN, m_xTimesCtrl);
	DDX_Control(pDX, IDC_Y_START_SPIN, m_yStartCtrl);
	DDX_Control(pDX, IDC_Y_SCALE_SPIN, m_yScaleCtrl);
	DDX_Control(pDX, IDC_Y_TIMES_SPIN, m_yTimesCtrl);
	DDX_Text(pDX, IDC_X_START_LEGEND, m_xStartLegend);
	DDX_Text(pDX, IDC_X_SCALE_LEGEND, m_xScaleLegend);
	DDX_Text(pDX, IDC_Y_START_LEGEND, m_yStartLegend);
	DDX_Text(pDX, IDC_Y_SCALE_LEGEND, m_yScaleLegend);
	DDX_Radio(pDX, IDC_RELATIVE, m_Unit);
	DDX_Control(pDX, IDC_SPIN_AMPLITUDE, m_AmplitudeCtrl);
	DDX_Control(pDX, IDC_PREVIEW_TEXTURE, m_PreviewTexture);
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
	m_xTimesCtrl.b3DDX(pDX,m_Bump->m_xTimes);
	m_yTimesCtrl.b3DDX(pDX,m_Bump->m_yTimes);
}


BEGIN_MESSAGE_MAP(CDlgBumpTexture, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpTexture)
	ON_BN_CLICKED(IDC_TEXTURE_SELECT , OnChangeTexturePath)
	ON_BN_CLICKED(IDC_UNIT, OnUnitChanged)
	ON_BN_CLICKED(IDC_RELATIVE, OnUnitChanged)
	ON_EN_KILLFOCUS(IDC_X_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_X_SCALE, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_X_TIMES, OnEdit)
	ON_EN_KILLFOCUS(IDC_Y_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_SCALE, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_TIMES, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_AMPLITUDE, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_START_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_SCALE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_TIMES_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_START_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_SCALE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_TIMES_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_AMPLITUDE, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpTexture message handlers

void CDlgBumpTexture::b3Register()
{
	b3Loader::b3AddClassType(BUMP_TEXTURE,IDS_BUMP_WOOD,IDI_BUMP_TEXTURE,b3Edit,b3Edit);
}

b3_bool CDlgBumpTexture::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpTexture dlg(item);

	return dlg.DoModal() == IDOK;
}

BOOL CDlgBumpTexture::OnInitDialog() 
{
	// Init legends
	m_xStart.b3Init(&m_xStartCtrl, &m_xStartLegend, &m_Bound.xInfo,B3_COND_CTRL_START);
	m_xScale.b3Init(&m_xScaleCtrl, &m_xScaleLegend, &m_Bound.xInfo,B3_COND_CTRL_LEN);
	m_yStart.b3Init(&m_yStartCtrl, &m_yStartLegend, &m_Bound.yInfo,B3_COND_CTRL_START);
	m_yScale.b3Init(&m_yScaleCtrl, &m_yScaleLegend, &m_Bound.yInfo,B3_COND_CTRL_LEN);

	CB3SimplePreviewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBumpTexture::b3InitDialog() 
{
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl);
	m_BumpSampler->b3SetBump(m_Bump);
}

void CDlgBumpTexture::b3UpdateUI()
{
	b3_bool absolute = m_Unit == 0;

	m_xStart.b3SetPos(absolute,m_Bump->m_xStart);
	m_xScale.b3SetPos(absolute,m_Bump->m_xScale);
	m_yStart.b3SetPos(absolute,m_Bump->m_yStart);
	m_yScale.b3SetPos(absolute,m_Bump->m_yScale);

	m_PreviewTexture.b3Copy(m_Bump->m_Texture);
	m_PreviewTexture.b3Update(true,true);
	m_PreviewBumpCtrl.b3Update(m_BumpSampler);
}

void CDlgBumpTexture::OnUnitChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgBumpTexture::OnLimitChanged() 
{
	// TODO: Add your control notification handler code here
	b3_bool absolute = m_Unit == 0;

	m_Bump->m_xStart = m_xStart.b3GetPos(absolute);
	m_Bump->m_xScale = m_xScale.b3GetPos(absolute);
	m_Bump->m_yStart = m_yStart.b3GetPos(absolute);
	m_Bump->m_yScale = m_yScale.b3GetPos(absolute);
}

void CDlgBumpTexture::OnChangeTexturePath() 
{
	// TODO: Add your command handler code here
	if (CB3SelectLoadTexture::b3Select(&m_Bump->m_Texture,m_Bump->m_Name))
	{
		((b3Tx *)m_PreviewTexture)->b3Copy(m_Bump->m_Texture);
		b3UpdateUI();
	}
}
