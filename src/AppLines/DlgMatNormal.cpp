/*
**
**	$Filename:	DlgMatNormal.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.13  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.12  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.11  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.10  2004/05/25 19:17:23  sm
**	- Some reflection spin controls didn't map input.
**	- Divided Fresnel computation and reflection/refraction
**	  mixing into two parts.
**	
**	Revision 1.9  2004/05/10 15:12:08  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.8  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**	
**	Revision 1.7  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**	
**	Revision 1.6  2004/04/26 14:28:10  sm
**	- Added marble dialog
**	- Resorted controls
**	
**	Revision 1.5  2004/04/25 19:28:21  sm
**	- Added available b3Items as list to maintain dialog.
**	- Preview is done only on auto refresh activated.
**	
**	Revision 1.4  2004/04/25 14:56:55  sm
**	- Completed simple material dialog.
**	
**	Revision 1.3  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	Revision 1.2  2003/07/12 10:20:16  sm
**	- Fixed ticketno. 12 (memory leak in b3ItemRegistry)
**	
**	Revision 1.1  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
*/

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

BOOL CDlgMatNormal::OnInitDialog() 
{
	CB3SimplePreviewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_AmbientCtrl.b3Init(&m_Material->m_Ambient,this);
	m_DiffuseCtrl.b3Init(&m_Material->m_Diffuse,this);
	m_SpecularCtrl.b3Init(&m_Material->m_Specular,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMatNormal::b3InitDialog()
{
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
