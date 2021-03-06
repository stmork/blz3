/*
**
**	$Filename:	DlgMatTexture.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture material
**
**	(C) Copyright 2003  Steffen A. Mork
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

#include "DlgMatTexture.h"
#include "blz3/system/b3Plugin.h"
#include "b3ExampleScene.h"
#include "b3SelectTexture.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatTexture implementation                 **
**                                                                      **
*************************************************************************/

CDlgMatTexture::CDlgMatTexture(b3Item *item,CAppObjectDoc *pDoc,CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item,CDlgMatTexture::IDD, pParent)
{
	m_BBox     = pDoc->m_BBox;
	m_Shape    = pDoc->b3GetSelectedShape();
	m_Shape->b3GetStencilBoundInfo(&m_Bound);
	m_Material = (b3MatTexture *)item;
	m_MatScene = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	b3Scene::b3CheckTexture(&m_Material->m_Texture,m_Material->m_Name);
	//{{AFX_DATA_INIT(CDlgMatTexture)
	m_Unit = 1;
	//}}AFX_DATA_INIT
	m_ReflectionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_IorCtrl.b3SetUnit(        CB3FloatSpinButtonCtrl::B3_UNIT_IOR);
	m_SpecularExpCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_SPECULAR_EXP);

	m_xTimesCtrl.b3SetRange(1,100);
	m_yTimesCtrl.b3SetRange(1,100);
}

CDlgMatTexture::~CDlgMatTexture()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatTexture::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatTexture)
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
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_IorCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_SpecularExpCtrl);
	DDX_Control(pDX, IDC_PREVIEW_TEXTURE, m_PreviewTexture);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_IorCtrl.b3DDX(pDX, m_Material->m_Ior);
	m_SpecularExpCtrl.b3DDX(pDX, m_Material->m_SpecularExp);
	m_xTimesCtrl.b3DDX(pDX,m_Material->m_xTimes);
	m_yTimesCtrl.b3DDX(pDX,m_Material->m_yTimes);
}


BEGIN_MESSAGE_MAP(CDlgMatTexture, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatTexture)
	ON_BN_CLICKED(IDC_TEXTURE_SELECT , OnChangeTexturePath)
	ON_BN_CLICKED(IDC_UNIT, OnUnitChanged)
	ON_BN_CLICKED(IDC_RELATIVE, OnUnitChanged)
	ON_EN_KILLFOCUS(IDC_X_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_X_SCALE, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_X_TIMES, OnEdit)
	ON_EN_KILLFOCUS(IDC_Y_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_SCALE, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_TIMES, OnEdit)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_START_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_SCALE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_TIMES_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_START_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_SCALE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_TIMES_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatTexture message handlers

void CDlgMatTexture::b3Register()
{
	b3Loader::b3AddClassType(TEXTURE,IDS_MAT_TEXTURE,IDI_MAT_TEXTURE,b3Edit,b3Edit);
}

b3_bool CDlgMatTexture::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatTexture dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatTexture::b3PreInitDialog() 
{
	m_xStart.b3Init(&m_xStartCtrl, &m_xStartLegend, &m_Bound.xInfo,B3_COND_CTRL_START);
	m_xScale.b3Init(&m_xScaleCtrl, &m_xScaleLegend, &m_Bound.xInfo,B3_COND_CTRL_DIR);
	m_yStart.b3Init(&m_yStartCtrl, &m_yStartLegend, &m_Bound.yInfo,B3_COND_CTRL_START);
	m_yScale.b3Init(&m_yScaleCtrl, &m_yScaleLegend, &m_Bound.yInfo,B3_COND_CTRL_DIR);
}

void CDlgMatTexture::b3PostInitDialog()
{
}

void CDlgMatTexture::b3UpdateUI()
{
	b3_bool absolute = m_Unit == 0;

	m_xStart.b3SetPos(absolute,m_Material->m_xStart);
	m_xScale.b3SetPos(absolute,m_Material->m_xScale);
	m_yStart.b3SetPos(absolute,m_Material->m_yStart);
	m_yScale.b3SetPos(absolute,m_Material->m_yScale);

	m_PreviewTexture.b3Copy(m_Material->m_Texture);
	m_PreviewTexture.b3Update(true,true);
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatTexture::OnUnitChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3Preview();
}

void CDlgMatTexture::OnLimitChanged() 
{
	// TODO: Add your control notification handler code here
	b3_bool absolute = m_Unit == 0;

	m_Material->m_xStart = m_xStart.b3GetPos(absolute);
	m_Material->m_xScale = m_xScale.b3GetPos(absolute);
	m_Material->m_yStart = m_yStart.b3GetPos(absolute);
	m_Material->m_yScale = m_yScale.b3GetPos(absolute);
}

void CDlgMatTexture::OnChangeTexturePath() 
{
	// TODO: Add your command handler code here
	if (CB3SelectLoadTexture::b3Select(&m_Material->m_Texture,m_Material->m_Name))
	{
		((b3Tx *)m_PreviewTexture)->b3Copy(m_Material->m_Texture);
		b3Preview();
	}
}
