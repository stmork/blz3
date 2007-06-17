/*
**
**	$Filename:	DlgMatNormal.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple material
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
#include "DlgMatNormal.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatNormal implementation                  **
**                                                                      **
*************************************************************************/

CDlgMatNormal::CDlgMatNormal(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item, CDlgMatNormal::IDD, pParent)
{
	m_Material = (b3MatNormal *)item;
	m_MatScene = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatNormal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ReflectionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_IorCtrl.b3SetUnit(        CB3FloatSpinButtonCtrl::B3_UNIT_IOR);
	m_SpecularExpCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_SPECULAR_EXP);
}

CDlgMatNormal::~CDlgMatNormal()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatNormal::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatNormal)
	DDX_Control(pDX, IDC_COLOR_AMBIENT, m_AmbientCtrl);
	DDX_Control(pDX, IDC_COLOR_DIFFUSE, m_DiffuseCtrl);
	DDX_Control(pDX, IDC_COLOR_SPECULAR, m_SpecularCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_IorCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_SpecularExpCtrl);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_IorCtrl.b3DDX(pDX, m_Material->m_Ior);
	m_SpecularExpCtrl.b3DDX(pDX, m_Material->m_SpecularExp);
}


BEGIN_MESSAGE_MAP(CDlgMatNormal, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatNormal)
	ON_BN_CLICKED(IDC_CHANGE_AMBIENT, OnColorAmbient)
	ON_BN_CLICKED(IDC_CHANGE_DIFFUSE, OnColorDiffuse)
	ON_BN_CLICKED(IDC_CHANGE_SPECULAR, OnColorSpecular)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatNormal message handlers

void CDlgMatNormal::b3Register()
{
	b3Loader::b3AddClassType(MAT_NORMAL,IDS_MAT_NORMAL,IDI_MAT_NORMAL,b3Edit,b3Edit);
}

b3_bool CDlgMatNormal::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatNormal dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatNormal::b3PreInitDialog()
{
}

void CDlgMatNormal::b3PostInitDialog()
{
	m_AmbientCtrl.b3Init(&m_Material->m_Ambient,this);
	m_DiffuseCtrl.b3Init(&m_Material->m_Diffuse,this);
	m_SpecularCtrl.b3Init(&m_Material->m_Specular,this);
}

void CDlgMatNormal::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatNormal::OnColorAmbient() 
{
	// TODO: Add your control notification handler code here
	if (m_AmbientCtrl.b3Select())
	{
		b3Preview();
	}
}

void CDlgMatNormal::OnColorDiffuse() 
{
	// TODO: Add your control notification handler code here
	if (m_DiffuseCtrl.b3Select())
	{
		b3Preview();
	}
}

void CDlgMatNormal::OnColorSpecular() 
{
	// TODO: Add your control notification handler code here
	if (m_SpecularCtrl.b3Select())
	{
		b3Preview();
	}
}
