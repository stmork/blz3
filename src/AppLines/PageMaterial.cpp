/*
**
**	$Filename:	PageMaterial.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden material properties
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

#include "AppLines.h"
#include "PageMaterial.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CPageMaterial implementation                  **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CPageMaterial, CPropertyPage)

CPageMaterial::CPageMaterial() : CPropertyPage(CPageMaterial::IDD)
{
	//{{AFX_DATA_INIT(CPageMaterial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ReflectionCtrl.b3SetRange(0.0,1.0);
	m_ReflectionCtrl.b3SetIncrement(0.01);
	m_ReflectionCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_ReflectionCtrl.b3SetDigits(2,1);
	m_RefractionCtrl.b3SetRange(0.0,1.0);
	m_RefractionCtrl.b3SetIncrement(0.01);
	m_RefractionCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERCENT);
	m_RefractionCtrl.b3SetDigits(2,1);
	m_RefrValueCtrl.b3SetRange(-5.0,5.0);
	m_RefrValueCtrl.b3SetDigits(0,2);
	m_RefrValueCtrl.b3SetIncrement(0.01);
	m_HighLightCtrl.b3SetRange(1.0,100000);
	m_HighLightCtrl.b3SetDigits(0,1);
	m_HighLightCtrl.b3SetIncrement(10.0);
}

CPageMaterial::~CPageMaterial()
{
}

void CPageMaterial::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMaterial)
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_HighLightCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_RefrValueCtrl);
	DDX_Control(pDX, IDC_COLOR_LIGHT, m_LightCtrl);
	DDX_Control(pDX, IDC_COLOR_DARK, m_DarkCtrl);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_RefrValueCtrl.b3DDX(pDX, m_Material->m_RefrValue);
	m_HighLightCtrl.b3DDX(pDX, m_Material->m_HighLight);
}


BEGIN_MESSAGE_MAP(CPageMaterial, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMaterial)
	ON_BN_CLICKED(IDC_CHANGE_LIGHT, OnColorLight)
	ON_BN_CLICKED(IDC_CHANGE_DARK, OnColorDark)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnSurfaceEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSurfaceSpin)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnSurfaceEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSurfaceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMaterial message handlers

BOOL CPageMaterial::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LightCtrl.b3Init(&m_Material->m_LightWood,this);
	m_DarkCtrl.b3Init(&m_Material->m_DarkWood,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMaterial::OnColorLight() 
{
	// TODO: Add your control notification handler code here
	if (m_LightCtrl.b3Select())
	{
		b3UpdateUI();
	}
}

void CPageMaterial::OnColorDark() 
{
	// TODO: Add your control notification handler code here
	if (m_DarkCtrl.b3Select())
	{
		b3UpdateUI();
	}
}

void CPageMaterial::OnSurfaceEdit()
{
	UpdateData();
	b3UpdateUI();
}

void CPageMaterial::OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnSurfaceEdit();
	*pResult = 0;
}

void CPageMaterial::b3UpdateUI()
{
	CWnd *parent = GetParent();

	if (parent != null)
	{
		parent->PostMessage(WM_USER);
	}
}
