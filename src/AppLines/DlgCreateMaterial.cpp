/*
**
**	$Filename:	DlgCreateMaterial.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Fast material creation on shape creation
**
**	(C) Copyright 2002  Steffen A. Mork
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

#include "DlgCreateMaterial.h"
#include "b3ExampleScene.h"
#include "b3SelectTexture.h"

/*************************************************************************
**                                                                      **
**                        CDlgCreateMaterial implementation             **
**                                                                      **
*************************************************************************/

CDlgCreateMaterial::CDlgCreateMaterial(b3_u32 class_type) : CPropertyPage(CDlgCreateMaterial::IDD)
{
	CB3App *app = CB3GetApp();

	//{{AFX_DATA_INIT(CDlgCreateMaterial)
	m_ReallyCreate = TRUE;
	m_UseTexture = FALSE;
	//}}AFX_DATA_INIT
	
	// Init Blizzard materials
	m_MatNormal  = new b3MatNormal(MATERIAL);
	m_MatTexture = new b3MatTexture(TEXTURE);
	m_MatScene   = b3ExampleScene::b3CreateMaterial(&m_MatHead,class_type);
	m_Material   = null;

	// Read previous used values
	app->b3ReadProfileColor("material.ambient",  m_MatNormal->m_Ambient);
	app->b3ReadProfileColor("material.diffuse",  m_MatNormal->m_Diffuse);
	app->b3ReadProfileColor("material.specular", m_MatNormal->m_Specular);
	m_Reflection   = app->b3ReadProfileFloat("material.reflection",   0);
	m_Refraction   = app->b3ReadProfileFloat("material.refraction",   0);
	m_Ior          = app->b3ReadProfileFloat("material.ior",          1);
	m_SpecularExp  = app->b3ReadProfileFloat("material.exponent",  1000);
	m_ReallyCreate = app->GetProfileInt(CB3ClientString(),"material.really create",TRUE);
	m_UseTexture   = app->GetProfileInt(CB3ClientString(),"material.use texture",  FALSE);
	m_MatTexture->b3SetTexture(app->GetProfileString(CB3ClientString(),"material.texture",""));
	b3Scene::b3CheckTexture(&m_MatTexture->m_Texture,m_MatTexture->m_Name);

	m_ReflectionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_IorCtrl.b3SetUnit(        CB3FloatSpinButtonCtrl::B3_UNIT_IOR);
	m_SpecularExpCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_SPECULAR_EXP);
}

CDlgCreateMaterial::~CDlgCreateMaterial()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
	delete m_MatTexture;
	delete m_MatNormal;
}

void CDlgCreateMaterial::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateMaterial)
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_SpecularExpCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_IorCtrl);
	DDX_Control(pDX, IDC_PREVIEW_TEXTURE, m_PreviewTexture);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	DDX_Control(pDX, IDC_COLOR_SPECULAR, m_SpecCtrl);
	DDX_Control(pDX, IDC_COLOR_DIFFUSE, m_DiffCtrl);
	DDX_Control(pDX, IDC_COLOR_AMBIENT, m_AmbCtrl);
	DDX_Check(pDX, IDC_REALLY_CREATE, m_ReallyCreate);
	DDX_Check(pDX, IDC_TEXTURE, m_UseTexture);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Refraction);
	m_IorCtrl.b3DDX(pDX,m_Ior);
	m_SpecularExpCtrl.b3DDX(pDX,m_SpecularExp);
}


BEGIN_MESSAGE_MAP(CDlgCreateMaterial, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgCreateMaterial)
	ON_BN_CLICKED(IDC_CHANGE_AMBIENT, OnChangeAmbient)
	ON_BN_CLICKED(IDC_CHANGE_DIFFUSE, OnChangeDiffuse)
	ON_BN_CLICKED(IDC_CHANGE_SPECULAR, OnChangeSpecular)
	ON_BN_CLICKED(IDC_TEXTURE_SELECT , OnChangeTexturePath)
	ON_BN_CLICKED(IDC_TEXTURE, OnUseTexture)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnSurfaceEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSurfaceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateMaterial message handlers

void CDlgCreateMaterial::b3Init()
{
}

BOOL CDlgCreateMaterial::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_AmbCtrl.b3Init( &m_MatNormal->m_Ambient, this);
	m_DiffCtrl.b3Init(&m_MatNormal->m_Diffuse,this);
	m_SpecCtrl.b3Init(&m_MatNormal->m_Specular,this);

	m_ReflectionCtrl.b3SetAccel(0.01);
	m_RefractionCtrl.b3SetAccel(0.01);
	m_IorCtrl.b3SetAccel(0.1);
	m_SpecularExpCtrl.b3SetAccel(50.0);

	m_PreviewTexture.b3Copy(m_MatTexture->m_Texture);
	m_PreviewTexture.b3Update(true,true);
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCreateMaterial::b3UpdateUI()
{
	// Enable/disable some controls
	GetDlgItem(IDC_CHANGE_AMBIENT)->EnableWindow(!m_UseTexture);
	GetDlgItem(IDC_CHANGE_DIFFUSE)->EnableWindow(!m_UseTexture);
	GetDlgItem(IDC_CHANGE_SPECULAR)->EnableWindow(!m_UseTexture);
	GetDlgItem(IDC_TEXTURE_SELECT)->EnableWindow(m_UseTexture);

	// Copy surface values into dummy materials
	m_MatNormal->m_Reflection   = m_Reflection;
	m_MatNormal->m_Refraction   = m_Refraction;
	m_MatNormal->m_Ior          = m_Ior;
	m_MatNormal->m_SpecularExp  = m_SpecularExp;
	m_MatTexture->m_Reflection  = m_Reflection;
	m_MatTexture->m_Refraction  = m_Refraction;
	m_MatTexture->m_Ior         = m_Ior;
	m_MatTexture->m_SpecularExp = m_SpecularExp;

	// Update material preview
	m_MatHead->b3RemoveAll();
	m_MatHead->b3Append(m_UseTexture ? (b3Item *)m_MatTexture : (b3Item *)m_MatNormal);
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgCreateMaterial::OnChangeAmbient() 
{
	// TODO: Add your control notification handler code here
	if (m_AmbCtrl.b3Select())
	{
		m_PreviewMaterialCtrl.b3Update(m_MatScene);
	}
}

void CDlgCreateMaterial::OnChangeDiffuse() 
{
	// TODO: Add your control notification handler code here
	if (m_DiffCtrl.b3Select())
	{
		m_PreviewMaterialCtrl.b3Update(m_MatScene);
	}
}

void CDlgCreateMaterial::OnChangeSpecular() 
{
	// TODO: Add your control notification handler code here
	if (m_SpecCtrl.b3Select())
	{
		m_PreviewMaterialCtrl.b3Update(m_MatScene);
	}
}

void CDlgCreateMaterial::OnChangeTexturePath() 
{
	// TODO: Add your command handler code here
	if (CB3SelectLoadTexture::b3Select(&m_MatTexture->m_Texture,m_MatTexture->m_Name))
	{
		((b3Tx *)m_PreviewTexture)->b3Copy(m_MatTexture->m_Texture);
		m_PreviewTexture.b3Copy(m_MatTexture->m_Texture);
		m_PreviewMaterialCtrl.b3Update(m_MatScene);
	}
}

void CDlgCreateMaterial::OnSurfaceEdit()
{
	UpdateData();
	b3UpdateUI();
}

void CDlgCreateMaterial::OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnSurfaceEdit();
	*pResult = 0;
}

void CDlgCreateMaterial::OnUseTexture() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgCreateMaterial::b3PostProcess(b3CondRectangle *rect) 
{
	// TODO: Add extra validation here
	CB3App       *app = CB3GetApp();
	b3MatNormal  *normal;
	b3MatTexture *texture;

	if (m_ReallyCreate)
	{
		if (m_UseTexture)
		{
			m_Material =
			texture = new b3MatTexture(TEXTURE);
			texture->m_Reflection  = m_Reflection;
			texture->m_Refraction  = m_Refraction;
			texture->m_Ior         = m_Ior;
			texture->m_SpecularExp = m_SpecularExp;
			texture->b3SetTexture(m_MatTexture->m_Name);
			app->WriteProfileString(CB3ClientString(),"material.texture",m_MatTexture->m_Name);

			// Scale texture to match stencil
			if (rect != null)
			{
				texture->m_xStart = rect->m_xStart;
				texture->m_xScale = rect->m_xEnd - rect->m_xStart;
				texture->m_yStart = rect->m_yEnd;
				texture->m_yScale = rect->m_yStart - rect->m_yEnd;
			}
		}
		else
		{
            m_Material =
			normal = new b3MatNormal(MATERIAL);
			normal->m_Reflection  = m_Reflection;
			normal->m_Refraction  = m_Refraction;
			normal->m_Ior         = m_Ior;
			normal->m_SpecularExp = m_SpecularExp;
			normal->m_Ambient     = m_MatNormal->m_Ambient;
			normal->m_Diffuse     = m_MatNormal->m_Diffuse;
			normal->m_Specular    = m_MatNormal->m_Specular;
			app->b3WriteProfileColor("material.ambient", m_MatNormal->m_Ambient);
			app->b3WriteProfileColor("material.diffuse", m_MatNormal->m_Diffuse);
			app->b3WriteProfileColor("material.specular",m_MatNormal->m_Specular);
		}

		B3_ASSERT((m_MatNormal->m_Ior != 0) && (m_MatNormal->m_SpecularExp != 0));

		app->b3WriteProfileFloat("material.reflection",m_Reflection);
		app->b3WriteProfileFloat("material.refraction",m_Refraction);
		app->b3WriteProfileFloat("material.ior",       m_Ior);
		app->b3WriteProfileFloat("material.exponent",  m_SpecularExp);
		app->WriteProfileInt(CB3ClientString(),"material.use texture",  m_UseTexture);
	}
	app->WriteProfileInt(CB3ClientString(),"material.really create",m_ReallyCreate);
}
