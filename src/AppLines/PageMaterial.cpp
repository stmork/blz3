/*
**
**	$Filename:	PageMaterial.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple material properties
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

#include "PageMaterial.h"

/*************************************************************************
**                                                                      **
**                        CPageMaterial implementation                  **
**                                                                      **
*************************************************************************/

CPageMaterial::CPageMaterial() : CB3PropertyPage(CPageMaterial::IDD)
{
	m_Material = null;
	//{{AFX_DATA_INIT(CPageMaterial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ReflectionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_IorCtrl.b3SetUnit(        CB3FloatSpinButtonCtrl::B3_UNIT_IOR);
	m_SpecularExpCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_SPECULAR_EXP);
}

CPageMaterial::~CPageMaterial()
{
}

void CPageMaterial::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMaterial)
	DDX_Control(pDX, IDC_COLOR_AMBIENT, m_AmbientCtrl);
	DDX_Control(pDX, IDC_COLOR_DIFFUSE, m_DiffuseCtrl);
	DDX_Control(pDX, IDC_COLOR_SPECULAR, m_SpecularCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_IorCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_SpecularExpCtrl);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_IorCtrl.b3DDX(pDX, m_Material->m_Ior);
	m_SpecularExpCtrl.b3DDX(pDX, m_Material->m_SpecularExp);
}


BEGIN_MESSAGE_MAP(CPageMaterial, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageMaterial)
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
// CPageMaterial message handlers

void CPageMaterial::b3PreInitDialog() 
{
}

void CPageMaterial::b3PostInitDialog() 
{
	m_AmbientCtrl.b3Init(&m_Material->m_Ambient,this);
	m_DiffuseCtrl.b3Init(&m_Material->m_Diffuse,this);
	m_SpecularCtrl.b3Init(&m_Material->m_Specular,this);
}

void CPageMaterial::OnColorAmbient() 
{
	// TODO: Add your control notification handler code here
	if (m_AmbientCtrl.b3Select())
	{
		b3UpdateUI();
	}
}

void CPageMaterial::OnColorDiffuse() 
{
	// TODO: Add your control notification handler code here
	if (m_DiffuseCtrl.b3Select())
	{
		b3UpdateUI();
	}
}

void CPageMaterial::OnColorSpecular() 
{
	// TODO: Add your control notification handler code here
	if (m_SpecularCtrl.b3Select())
	{
		b3UpdateUI();
	}
}
