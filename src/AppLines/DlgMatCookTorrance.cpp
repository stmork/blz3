/*
**
**	$Filename:	DlgMatCookTorrance.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Cook&Torrance material shader
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

#include "blz3/system/b3Plugin.h"
#include "DlgMatCookTorrance.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatCookTorrance implementation            **
**                                                                      **
*************************************************************************/

CDlgMatCookTorrance::CDlgMatCookTorrance(b3Item *item,CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item, CDlgMatCookTorrance::IDD, pParent)
{
	m_Material = (b3MatCookTorrance *)item;
	m_MatScene = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatCookTorrance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_KaCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_KdCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_KsCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_MCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);

	m_ReflectionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_IorCtrl.b3SetUnit(        CB3FloatSpinButtonCtrl::B3_UNIT_IOR);
	m_SpecularExpCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_SPECULAR_EXP);
}

CDlgMatCookTorrance::~CDlgMatCookTorrance()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatCookTorrance::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatCookTorrance)
	DDX_Control(pDX, IDC_COLOR_AMBIENT, m_AmbientCtrl);
	DDX_Control(pDX, IDC_COLOR_DIFFUSE, m_DiffuseCtrl);
	DDX_Control(pDX, IDC_COLOR_SPECULAR, m_SpecularCtrl);
	DDX_Control(pDX, IDC_KA_SPIN, m_KaCtrl);
	DDX_Control(pDX, IDC_KD_SPIN, m_KdCtrl);
	DDX_Control(pDX, IDC_KS_SPIN, m_KsCtrl);
	DDX_Control(pDX, IDC_M_SPIN, m_MCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_IorCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_SpecularExpCtrl);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
	m_KaCtrl.b3DDX(pDX,m_Material->m_ka);
	m_KdCtrl.b3DDX(pDX,m_Material->m_kd);
	m_KsCtrl.b3DDX(pDX,m_Material->m_ks);
	m_MCtrl.b3DDX(pDX,m_Material->m_m);
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_IorCtrl.b3DDX(pDX, m_Material->m_Ior);
	m_SpecularExpCtrl.b3DDX(pDX, m_Material->m_SpecularExp);
}


BEGIN_MESSAGE_MAP(CDlgMatCookTorrance, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatCookTorrance)
	ON_BN_CLICKED(IDC_CHANGE_AMBIENT, OnColorAmbient)
	ON_BN_CLICKED(IDC_CHANGE_DIFFUSE, OnColorDiffuse)
	ON_BN_CLICKED(IDC_CHANGE_SPECULAR, OnColorSpecular)
	ON_EN_KILLFOCUS(IDC_KA, OnEdit)
	ON_EN_KILLFOCUS(IDC_KD, OnEdit)
	ON_EN_KILLFOCUS(IDC_KS, OnEdit)
	ON_EN_KILLFOCUS(IDC_M, OnEdit)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_KA_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_KD_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_KS_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_M_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCookTorrance message handlers

void CDlgMatCookTorrance::b3Register()
{
	b3Loader::b3AddClassType(COOK_TORRANCE,IDS_MAT_COOK_TORRANCE,IDI_MAT_COOK_TORRANCE,b3Edit,b3Edit);
}

b3_bool CDlgMatCookTorrance::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatCookTorrance dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatCookTorrance::b3PreInitDialog()
{
}

void CDlgMatCookTorrance::b3PostInitDialog()
{
	m_AmbientCtrl.b3Init(&m_Material->m_Ambient,this);
	m_DiffuseCtrl.b3Init(&m_Material->m_Diffuse,this);
	m_SpecularCtrl.b3Init(&m_Material->m_Specular,this);
}

void CDlgMatCookTorrance::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatCookTorrance::OnColorAmbient() 
{
	// TODO: Add your control notification handler code here
	if (m_AmbientCtrl.b3Select())
	{
		b3Preview();
	}
}

void CDlgMatCookTorrance::OnColorDiffuse() 
{
	// TODO: Add your control notification handler code here
	if (m_DiffuseCtrl.b3Select())
	{
		b3Preview();
	}
}

void CDlgMatCookTorrance::OnColorSpecular() 
{
	// TODO: Add your control notification handler code here
	if (m_SpecularCtrl.b3Select())
	{
		b3Preview();
	}
}
