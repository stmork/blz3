/*
**
**	$Filename:	PageMatWood.cpp $
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
#include "PageMatWood.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/04/24 20:15:51  sm
**	- Further slide material dialog development
**
**	Revision 1.2  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**	Revision 1.1  2004/04/23 13:17:17  sm
**	- Added simple material page and renamed wood material page.
**	- Reflect material member renaming.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CPageMatWood implementation                   **
**                                                                      **
*************************************************************************/

CPageMatWood::CPageMatWood() : CB3PropertyPage(CPageMatWood::IDD)
{
	//{{AFX_DATA_INIT(CPageMatWood)
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
	m_IorCtrl.b3SetRange(-5.0,5.0);
	m_IorCtrl.b3SetDigits(0,2);
	m_IorCtrl.b3SetIncrement(0.01);
	m_SpecularExpCtrl.b3SetRange(1.0,100000);
	m_SpecularExpCtrl.b3SetDigits(0,1);
	m_SpecularExpCtrl.b3SetIncrement(10.0);
}

CPageMatWood::~CPageMatWood()
{
}

void CPageMatWood::DoDataExchange(CDataExchange* pDX)
{
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMatWood)
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_SpecularExpCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_IorCtrl);
	DDX_Control(pDX, IDC_COLOR_LIGHT, m_LightCtrl);
	DDX_Control(pDX, IDC_COLOR_DARK, m_DarkCtrl);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_IorCtrl.b3DDX(pDX, m_Material->m_Ior);
	m_SpecularExpCtrl.b3DDX(pDX, m_Material->m_SpecularExp);
}


BEGIN_MESSAGE_MAP(CPageMatWood, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageMatWood)
	ON_BN_CLICKED(IDC_CHANGE_LIGHT, OnColorLight)
	ON_BN_CLICKED(IDC_CHANGE_DARK, OnColorDark)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSpin)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMatWood message handlers

BOOL CPageMatWood::OnInitDialog() 
{
	CB3PropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LightCtrl.b3Init(&m_Material->m_LightWood,this);
	m_DarkCtrl.b3Init(&m_Material->m_DarkWood,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMatWood::OnColorLight() 
{
	// TODO: Add your control notification handler code here
	if (m_LightCtrl.b3Select())
	{
		b3UpdateUI();
	}
}

void CPageMatWood::OnColorDark() 
{
	// TODO: Add your control notification handler code here
	if (m_DarkCtrl.b3Select())
	{
		b3UpdateUI();
	}
}
