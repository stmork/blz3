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

#include "AppLinesInclude.h"

#include "DlgBumpTexture.h"
#include "b3SelectTexture.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpTexture::CDlgBumpTexture(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item, CDlgBumpTexture::IDD, pParent)
{
	m_BBox     = pDoc->m_BBox;
	m_Shape    = pDoc->b3GetSelectedShape();
	m_Shape->b3GetStencilBoundInfo(&m_Bound);
	m_Bump = (b3BumpTexture *)item;
	b3Scene::b3CheckTexture(&m_Bump->m_Texture,m_Bump->m_Name);
	//{{AFX_DATA_INIT(CDlgBumpTexture)
	m_Unit = 1;
	//}}AFX_DATA_INIT
	m_AmplitudeCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_BUMP);
	m_xTimesCtrl.b3SetRange(1,100);
	m_yTimesCtrl.b3SetRange(1,100);
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
	m_AmplitudeCtrl.b3DDX(pDX,m_Bump->m_Amplitude);
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
	b3Loader::b3AddClassType(BUMP_TEXTURE,IDS_BUMP_TEXTURE,IDI_BUMP_TEXTURE,b3Edit,b3Edit);
}

b3_bool CDlgBumpTexture::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpTexture dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpTexture::b3PreInitDialog() 
{
	m_xStart.b3Init(&m_xStartCtrl, &m_xStartLegend, &m_Bound.xInfo,B3_COND_CTRL_START);
	m_xScale.b3Init(&m_xScaleCtrl, &m_xScaleLegend, &m_Bound.xInfo,B3_COND_CTRL_DIR);
	m_yStart.b3Init(&m_yStartCtrl, &m_yStartLegend, &m_Bound.yInfo,B3_COND_CTRL_START);
	m_yScale.b3Init(&m_yScaleCtrl, &m_yScaleLegend, &m_Bound.yInfo,B3_COND_CTRL_DIR);
}

void CDlgBumpTexture::b3PostInitDialog() 
{
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl, &m_BBox->m_DimSize, 1);
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
	b3Preview();
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
		b3Preview();
	}
}
