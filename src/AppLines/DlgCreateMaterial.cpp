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
**	Revision 1.13  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**
**	Revision 1.12  2003/03/04 20:37:36  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.11  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.10  2003/02/09 13:58:14  sm
**	- cleaned up file selection dialogs
**	
**	Revision 1.9  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.8  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**	
**	Revision 1.7  2002/03/08 16:46:14  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
**	Revision 1.6  2002/03/01 21:25:36  sm
**	- Fixed a problem in create material dialog: Use the
**	  correct function proto types depending on the
**	  message type!
**	
**	Revision 1.5  2002/03/01 20:26:40  sm
**	- Added CB3FloatSpinButtonCtrl for conveniant input.
**	- Made some minor changes and tests.
**	
**	Revision 1.4  2002/02/28 21:32:51  sm
**	- Corrected resource IDs because MFC used the same IDs
**	  generated by Visual C++. This caused printing problems.
**	
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
	CB3App *app = CB3GetApp();

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
	app->b3ReadProfileColor("material.ambient",  m_MatNormal->m_AmbColor);
	app->b3ReadProfileColor("material.diffuse",  m_MatNormal->m_DiffColor);
	app->b3ReadProfileColor("material.specular", m_MatNormal->m_SpecColor);
	m_Reflection   = app->b3ReadProfileFloat("material.reflection",   0);
	m_Refraction   = app->b3ReadProfileFloat("material.refraction",   0);
	m_RefrValue    = app->b3ReadProfileFloat("material.ior",          1);
	m_HighLight    = app->b3ReadProfileFloat("material.exponent",  1000);
	m_ReallyCreate = app->GetProfileInt(CB3ClientString(),"material.really create",TRUE);
	m_UseTexture   = app->GetProfileInt(CB3ClientString(),"material.use texture",  FALSE);
	m_MatTexture->b3SetTexture(app->GetProfileString(CB3ClientString(),"material.texture",""));
	b3Scene::b3CheckTexture(&m_MatTexture->m_Texture,m_MatTexture->m_Name);

	m_ReflectionCtrl.b3SetRange(0.0,100.0);
	m_RefractionCtrl.b3SetRange(0.0,100.0);
	m_RefrValueCtrl.b3SetRange(-5.0,5.0);
	m_RefrValueCtrl.b3SetDigits(0,2);
	m_RefrValueCtrl.b3SetIncrement(0.01);
	m_HighLightCtrl.b3SetRange(1.0,100000);
	m_HighLightCtrl.b3SetDigits(0,1);
	m_HighLightCtrl.b3SetIncrement(10.0);
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
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_HighLightCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_RefrValueCtrl);
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
	m_RefrValueCtrl.b3DDX(pDX,m_RefrValue);
	m_HighLightCtrl.b3DDX(pDX,m_HighLight);
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
	m_AmbCtrl.b3Init( &m_MatNormal->m_AmbColor, this);
	m_DiffCtrl.b3Init(&m_MatNormal->m_DiffColor,this);
	m_SpecCtrl.b3Init(&m_MatNormal->m_SpecColor,this);

	m_ReflectionCtrl.b3SetAccel(1.0);
	m_RefractionCtrl.b3SetAccel(1.0);
	m_RefrValueCtrl.b3SetAccel(0.1);
	m_HighLightCtrl.b3SetAccel(50.0);

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
	m_MatNormal->m_Reflection  = m_Reflection / 100.0;
	m_MatNormal->m_Refraction  = m_Refraction / 100.0;
	m_MatNormal->m_RefrValue   = m_RefrValue;
	m_MatNormal->m_HighLight   = m_HighLight;
	m_MatTexture->m_Reflection = m_Reflection / 100.0;
	m_MatTexture->m_Refraction = m_Refraction / 100.0;
	m_MatTexture->m_RefrValue  = m_RefrValue;
	m_MatTexture->m_HighLight  = m_HighLight;

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
			texture->m_Reflection = m_Reflection / 100.0;
			texture->m_Refraction = m_Refraction / 100.0;
			texture->m_RefrValue  = m_RefrValue;
			texture->m_HighLight  = m_HighLight;
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
			normal->m_Reflection = m_Reflection / 100.0;
			normal->m_Refraction = m_Refraction / 100.0;
			normal->m_RefrValue  = m_RefrValue;
			normal->m_HighLight  = m_HighLight;
			normal->m_AmbColor   = m_MatNormal->m_AmbColor;
			normal->m_DiffColor  = m_MatNormal->m_DiffColor;
			normal->m_SpecColor  = m_MatNormal->m_SpecColor;
			app->b3WriteProfileColor("material.ambient", m_MatNormal->m_AmbColor);
			app->b3WriteProfileColor("material.diffuse", m_MatNormal->m_DiffColor);
			app->b3WriteProfileColor("material.specular",m_MatNormal->m_SpecColor);
		}

		B3_ASSERT((m_MatNormal->m_RefrValue != 0) && (m_MatNormal->m_HighLight != 0));

		app->b3WriteProfileFloat("material.reflection",m_Reflection);
		app->b3WriteProfileFloat("material.refraction",m_Refraction);
		app->b3WriteProfileFloat("material.ior",       m_RefrValue);
		app->b3WriteProfileFloat("material.exponent",  m_HighLight);
		app->WriteProfileInt(CB3ClientString(),"material.use texture",  m_UseTexture);
	}
	app->WriteProfileInt(CB3ClientString(),"material.really create",m_ReallyCreate);
}
