/*
**
**	$Filename:	DlgCreateMaterial.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "AppLines.h"
#include "DlgCreateMaterial.h"
#include "b3ExampleScene.h"
#include "b3SelectTexture.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**
**	Revision 1.2  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.1  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCreateMaterial implementation             **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CDlgCreateMaterial, CPropertyPage)

CDlgCreateMaterial::CDlgCreateMaterial() : CPropertyPage(CDlgCreateMaterial::IDD)
{
	CB3App       *app = CB3GetApp();

	//{{AFX_DATA_INIT(CDlgCreateMaterial)
	m_ReallyCreate = TRUE;
	m_UseTexture = FALSE;
	//}}AFX_DATA_INIT
	
	// Init Blizzard materials
	m_MatNormal  = new b3MatNormal(MATERIAL);
	m_MatTexture = new b3MatTexture(TEXTURE);
	m_MatScene   = b3ExampleScene::b3CreateMaterial(&m_MatHead);
	m_Material   = null;

	// Read previous used values
	app->b3ReadProfileColor("material.ambient",  &m_MatNormal->m_AmbColor);
	app->b3ReadProfileColor("material.diffuse",  &m_MatNormal->m_DiffColor);
	app->b3ReadProfileColor("material.specular", &m_MatNormal->m_SpecColor);
	m_MatNormal->m_Reflection = app->b3ReadProfileFloat("material.reflection",   0);
	m_MatNormal->m_Refraction = app->b3ReadProfileFloat("material.refraction",   0);
	m_MatNormal->m_RefrValue  = app->b3ReadProfileFloat("material.ior",          1);
	m_MatNormal->m_HighLight  = app->b3ReadProfileFloat("material.exponent",  1000);
	m_ReallyCreate            = app->GetProfileInt(CB3ClientString(),"material.really create",TRUE);
	m_UseTexture              = app->GetProfileInt(CB3ClientString(),"material.use texture",FALSE);
	strcpy(m_MatTexture->m_Name,app->GetProfileString(CB3ClientString(),"material.texture",""));
	b3CheckTexture(&m_MatTexture->m_Texture,m_MatTexture->m_Name);
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
	DDX_Control(pDX, IDC_PREVIEW_TEXTURE, m_PreviewTexture);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT, m_SpecExponentCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE, m_RefrCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE, m_ReflCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION, m_IORCtrl);
	DDX_Control(pDX, IDC_COLOR_SPECULAR, m_SpecCtrl);
	DDX_Control(pDX, IDC_COLOR_DIFFUSE, m_DiffCtrl);
	DDX_Control(pDX, IDC_COLOR_AMBIENT, m_AmbCtrl);
	DDX_Check(pDX, IDC_REALLY_CREATE, m_ReallyCreate);
	DDX_Check(pDX, IDC_TEXTURE, m_UseTexture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateMaterial, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgCreateMaterial)
	ON_BN_CLICKED(ID_CHANGE_AMBIENT, OnChangeAmbient)
	ON_BN_CLICKED(ID_CHANGE_DIFFUSE, OnChangeDiffuse)
	ON_BN_CLICKED(ID_CHANGE_SPECULAR, OnChangeSpecular)
	ON_BN_CLICKED(IDC_TEXTURE_SELECT , OnChangeTexturePath)
	ON_BN_CLICKED(IDC_TEXTURE, OnUseTexture)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnSurfaceValuesChanged)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnSurfaceValuesChanged)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnSurfaceValuesChanged)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnSurfaceValuesChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateMaterial message handlers

BOOL CDlgCreateMaterial::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_AmbCtrl.b3Init( &m_MatNormal->m_AmbColor, this);
	m_DiffCtrl.b3Init(&m_MatNormal->m_DiffColor,this);
	m_SpecCtrl.b3Init(&m_MatNormal->m_SpecColor,this);

	m_ReflCtrl.b3SetRange(0.0,1.0);
	m_ReflCtrl.b3SetDigits(1,3);
	m_ReflCtrl.b3SetValue(m_MatNormal->m_Reflection);
	m_RefrCtrl.b3SetRange(0.0,1.0);
	m_RefrCtrl.b3SetDigits(1,3);
	m_RefrCtrl.b3SetValue(m_MatNormal->m_Refraction);
	m_IORCtrl.b3SetRange(-5.0,5.0);
	m_IORCtrl.b3SetDigits(1,3);
	m_IORCtrl.b3SetValue(m_MatNormal->m_RefrValue);
	m_SpecExponentCtrl.b3SetRange(1.0,100000.0);
	m_SpecExponentCtrl.b3SetDigits(5,2);
	m_SpecExponentCtrl.b3SetValue(m_MatNormal->m_HighLight);

	m_PreviewTexture.b3Copy(m_MatTexture->m_Texture);
	m_PreviewTexture.b3Update(true,true);
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCreateMaterial::b3UpdateUI()
{
	GetDlgItem(ID_CHANGE_AMBIENT)->EnableWindow(!m_UseTexture);
	GetDlgItem(ID_CHANGE_DIFFUSE)->EnableWindow(!m_UseTexture);
	GetDlgItem(ID_CHANGE_SPECULAR)->EnableWindow(!m_UseTexture);
	GetDlgItem(IDC_TEXTURE_SELECT)->EnableWindow(m_UseTexture);
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
	if (CB3SelectTexture::b3Select(&m_MatTexture->m_Texture,m_MatTexture->m_Name))
	{
		((b3Tx *)m_PreviewTexture)->b3Copy(m_MatTexture->m_Texture);
		m_PreviewTexture.b3Copy(m_MatTexture->m_Texture);
		m_PreviewMaterialCtrl.b3Update(m_MatScene);
	}
}

void CDlgCreateMaterial::OnUseTexture() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgCreateMaterial::OnSurfaceValuesChanged() 
{
	// TODO: Add your control notification handler code here
	m_MatNormal->m_Reflection  = m_ReflCtrl.m_Value;
	m_MatNormal->m_Refraction  = m_RefrCtrl.m_Value;
	m_MatNormal->m_RefrValue   = m_IORCtrl.m_Value;
	m_MatNormal->m_HighLight   = m_SpecExponentCtrl.m_Value;

	m_MatTexture->m_Reflection = m_ReflCtrl.m_Value;
	m_MatTexture->m_Refraction = m_RefrCtrl.m_Value;
	m_MatTexture->m_RefrValue  = m_IORCtrl.m_Value;
	m_MatTexture->m_HighLight  = m_SpecExponentCtrl.m_Value;

	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgCreateMaterial::b3PostProcess() 
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
			texture->m_Reflection = m_MatNormal->m_Reflection;
			texture->m_Refraction = m_MatNormal->m_Refraction;
			texture->m_RefrValue  = m_MatNormal->m_RefrValue; 
			texture->m_HighLight  = m_MatNormal->m_HighLight; 
			strcpy(texture->m_Name,m_MatTexture->m_Name);
			app->WriteProfileString(CB3ClientString(),"material.texture",m_MatTexture->m_Name);
		}
		else
		{
            m_Material =
			normal = new b3MatNormal(MATERIAL);
			normal->m_Reflection = m_MatNormal->m_Reflection;
			normal->m_Refraction = m_MatNormal->m_Refraction;
			normal->m_RefrValue  = m_MatNormal->m_RefrValue;
			normal->m_HighLight  = m_MatNormal->m_HighLight;
			normal->m_AmbColor   = m_MatNormal->m_AmbColor;
			normal->m_DiffColor  = m_MatNormal->m_DiffColor;
			normal->m_SpecColor  = m_MatNormal->m_SpecColor;
			app->b3WriteProfileColor("material.ambient", &m_MatNormal->m_AmbColor);
			app->b3WriteProfileColor("material.diffuse", &m_MatNormal->m_DiffColor);
			app->b3WriteProfileColor("material.specular",&m_MatNormal->m_SpecColor);
		}

		B3_ASSERT((m_MatNormal->m_RefrValue != 0) && (m_MatNormal->m_HighLight != 0));

		app->b3WriteProfileFloat("material.reflection",m_MatNormal->m_Reflection);
		app->b3WriteProfileFloat("material.refraction",m_MatNormal->m_Refraction);
		app->b3WriteProfileFloat("material.ior",       m_MatNormal->m_RefrValue);
		app->b3WriteProfileFloat("material.exponent",  m_MatNormal->m_HighLight);
		app->WriteProfileInt(CB3ClientString(),"material.use texture",  m_UseTexture);
	}
	app->WriteProfileInt(CB3ClientString(),"material.really create",m_ReallyCreate);
}
