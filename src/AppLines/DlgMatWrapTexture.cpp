/*
**
**	$Filename:	DlgMatWrapTexture.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wrap texture material
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

#include "DlgMatWrapTexture.h"
#include "blz3/system/b3Plugin.h"
#include "b3ExampleScene.h"
#include "b3SelectTexture.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatWrapTexture implementation             **
**                                                                      **
*************************************************************************/

CDlgMatWrapTexture::CDlgMatWrapTexture(
	b3Item        *item,
	CAppObjectDoc *pDoc,
	CWnd          *pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item,CDlgMatWrapTexture::IDD, pParent)
{
	m_Shape    = pDoc->b3GetSelectedShape();
	m_Shape->b3GetStencilBoundInfo(&m_Bound);
	m_BBox     = pDoc->m_BBox;
	m_Material = (b3MatWrapTexture *)item;
	m_MatScene = b3ExampleScene::b3CreateMaterial(&m_MatHead,pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	b3Scene::b3CheckTexture(&m_Material->m_Texture,m_Material->m_Name);
	//{{AFX_DATA_INIT(CDlgMatWrapTexture)
	m_Unit = 1;
	//}}AFX_DATA_INIT
	m_ReflectionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_IorCtrl.b3SetUnit(        CB3FloatSpinButtonCtrl::B3_UNIT_IOR);
	m_SpecularExpCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_SPECULAR_EXP);
}

CDlgMatWrapTexture::~CDlgMatWrapTexture()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatWrapTexture::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatWrapTexture)
	DDX_Control(pDX, IDC_X_START_SPIN, m_xStartCtrl);
	DDX_Control(pDX, IDC_X_END_SPIN, m_xEndCtrl);
	DDX_Control(pDX, IDC_Y_START_SPIN, m_yStartCtrl);
	DDX_Control(pDX, IDC_Y_END_SPIN, m_yEndCtrl);
	DDX_Text(pDX, IDC_X_START_LEGEND, m_xStartLegend);
	DDX_Text(pDX, IDC_X_END_LEGEND, m_xEndLegend);
	DDX_Text(pDX, IDC_Y_START_LEGEND, m_yStartLegend);
	DDX_Text(pDX, IDC_Y_END_LEGEND, m_yEndLegend);
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
}


BEGIN_MESSAGE_MAP(CDlgMatWrapTexture, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatWrapTexture)
	ON_BN_CLICKED(IDC_TEXTURE_SELECT , OnChangeTexturePath)
	ON_BN_CLICKED(IDC_UNIT, OnUnitChanged)
	ON_BN_CLICKED(IDC_RELATIVE, OnUnitChanged)
	ON_EN_KILLFOCUS(IDC_X_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_X_END, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_END, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_START_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_X_END_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_START_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_Y_END_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWrapTexture message handlers

void CDlgMatWrapTexture::b3Register()
{
	b3Loader::b3AddClassType(WRAPTEXTURE, IDS_MAT_WRAP_TEXTURE, IDI_MAT_WRAP_TEXTURE,b3Edit,b3Edit);
}

b3_bool CDlgMatWrapTexture::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatWrapTexture dlg(item,(CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatWrapTexture::b3PreInitDialog() 
{
	m_xStart.b3Init(&m_xStartCtrl,&m_xStartLegend,&m_Bound.xInfo,B3_COND_CTRL_START);
	m_xEnd.b3Init(  &m_xEndCtrl,  &m_xEndLegend,  &m_Bound.xInfo,B3_COND_CTRL_END);
	m_yStart.b3Init(&m_yStartCtrl,&m_yStartLegend,&m_Bound.yInfo,B3_COND_CTRL_START);
	m_yEnd.b3Init(  &m_yEndCtrl,  &m_yEndLegend,  &m_Bound.yInfo,B3_COND_CTRL_END);
}

void CDlgMatWrapTexture::b3PostInitDialog()
{
}

void CDlgMatWrapTexture::b3UpdateUI()
{
	b3_bool absolute = m_Unit == 0;

	m_xStart.b3SetPos(absolute,m_Material->m_xStart);
	m_xEnd.b3SetPos(  absolute,m_Material->m_xEnd);
	m_yStart.b3SetPos(absolute,m_Material->m_yStart);
	m_yEnd.b3SetPos(  absolute,m_Material->m_yEnd);

	m_PreviewTexture.b3Copy(m_Material->m_Texture);
	m_PreviewTexture.b3Update(true,true);
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatWrapTexture::OnUnitChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3Preview();
}

void CDlgMatWrapTexture::OnLimitChanged() 
{
	// TODO: Add your control notification handler code here
	b3_bool absolute = m_Unit == 0;

	m_Material->m_xStart = m_xStart.b3GetPos(absolute);
	m_Material->m_xEnd   = m_xEnd.b3GetPos(absolute);
	m_Material->m_yStart = m_yStart.b3GetPos(absolute);
	m_Material->m_yEnd   = m_yEnd.b3GetPos(absolute);
}

void CDlgMatWrapTexture::OnChangeTexturePath() 
{
	// TODO: Add your command handler code here
	if (CB3SelectLoadTexture::b3Select(&m_Material->m_Texture,m_Material->m_Name))
	{
		if (!m_AutoRefresh)
		{
			m_PreviewTexture.b3Copy(m_Material->m_Texture);
			m_PreviewTexture.b3Update(true,true);
		}
		b3Preview();
	}
}
