/*
**
**	$Filename:	DlgMatOakPlank.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden oak plank material
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
#include "DlgMatOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/04/10 14:33:25  sm
**	- Added oak plank support.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatWood implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatOakPlank::CDlgMatOakPlank(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatOakPlank::IDD, pParent)
{
	m_Material = (b3MatOakPlank *)item;

	//{{AFX_DATA_INIT(CDlgMatOakPlank)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgMatOakPlank::~CDlgMatOakPlank()
{
	delete m_MatSampler;
}


void CDlgMatOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatOakPlank)
	DDX_Control(pDX, IDC_ROT_Z, m_RotZCtrl);
	DDX_Control(pDX, IDC_ROT_Y, m_RotYCtrl);
	DDX_Control(pDX, IDC_RINGY, m_RingyCtrl);
	DDX_Control(pDX, IDC_GRAINY, m_GrainyCtrl);
	DDX_Control(pDX, IDC_GRAIN_FREQUENCY, m_GrainFrequencyCtrl);
	DDX_Control(pDX, IDC_RING_SPACING, m_RingSpacingCtrl);
	DDX_Control(pDX, IDC_RING_FREQUENCY, m_RingFrequencyCtrl);
	DDX_Control(pDX, IDC_RING_NOISE, m_RingNoiseCtrl);
	DDX_Control(pDX, IDC_RING_NOISE_FREQUENCY, m_RingNoiseFrequencyCtrl);
	DDX_Control(pDX, IDC_TRUNK_WOBBLE, m_TrunkWobbleCtrl);
	DDX_Control(pDX, IDC_TRUNK_WOBBLE_FREQUENCY, m_TrunkWobbleFrequencyCtrl);
	DDX_Control(pDX, IDC_ANGULAR_WOBBLE, m_AngularWobbleCtrl);
	DDX_Control(pDX, IDC_ANGULAR_WOBBLE_FREQUENCY, m_AngularWobbleFrequencyCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_HighLightCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_RefrValueCtrl);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	DDX_Control(pDX, IDC_COLOR_LIGHT, m_LightCtrl);
	DDX_Control(pDX, IDC_COLOR_DARK, m_DarkCtrl);
	//}}AFX_DATA_MAP
	m_ReflectionCtrl.b3DDX(pDX,m_Material->m_Reflection);
	m_RefractionCtrl.b3DDX(pDX,m_Material->m_Refraction);
	m_RefrValueCtrl.b3DDX(pDX, m_Material->m_RefrValue);
	m_HighLightCtrl.b3DDX(pDX, m_Material->m_HighLight);
	m_RotYCtrl.b3DDX(pDX, m_Material->m_yRot);
	m_RotZCtrl.b3DDX(pDX, m_Material->m_zRot);
	m_RingyCtrl.b3DDX(pDX, m_Material->m_Ringy);
	m_GrainyCtrl.b3DDX(pDX, m_Material->m_Grainy);
	m_GrainFrequencyCtrl.b3DDX(pDX, m_Material->m_GrainFrequency);
	m_RingSpacingCtrl.b3DDX(pDX, m_Material->m_RingSpacing);
	m_RingFrequencyCtrl.b3DDX(pDX, m_Material->m_RingFrequency);
	m_RingNoiseCtrl.b3DDX(pDX, m_Material->m_RingNoise);
	m_RingNoiseFrequencyCtrl.b3DDX(pDX, m_Material->m_RingNoiseFrequency);
	m_TrunkWobbleCtrl.b3DDX(pDX, m_Material->m_TrunkWobble);
	m_TrunkWobbleFrequencyCtrl.b3DDX(pDX, m_Material->m_TrunkWobbleFrequency);
	m_AngularWobbleCtrl.b3DDX(pDX, m_Material->m_AngularWobble);
	m_AngularWobbleFrequencyCtrl.b3DDX(pDX, m_Material->m_AngularWobbleFrequency);
}


BEGIN_MESSAGE_MAP(CDlgMatOakPlank, CDialog)
	//{{AFX_MSG_MAP(CDlgMatOakPlank)
	ON_BN_CLICKED(IDC_CHANGE_LIGHT, OnColorLight)
	ON_BN_CLICKED(IDC_CHANGE_DARK, OnColorDark)
	ON_EN_KILLFOCUS(IDC_REFLECTANCE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_REFRACTANCE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_INDEX_OF_REFRACTION, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_SPEC_EXPONENT, OnSurfaceEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFLECTANCE_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_REFRACTANCE_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_INDEX_OF_REFRACTION_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPEC_EXPONENT_SPIN, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ROT_Y, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ROT_Z, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_RINGY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_GRAINY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_GRAIN_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_RING_SPACING, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_RING_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_RING_NOISE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_RING_NOISE_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_TRUNK_WOBBLE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_TRUNK_WOBBLE_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ANGULAR_WOBBLE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ANGULAR_WOBBLE_FREQUENCY, OnSurfaceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatOakPlank message handlers


void CDlgMatOakPlank::b3Register()
{
	b3Loader::b3AddClassType(OAKPLANK,IDS_MAT_OAKPLANK,IDI_MAT_OAKPLANK,b3Edit,b3Edit);
}

b3_bool CDlgMatOakPlank::b3Edit(b3Item *item)
{
	CDlgMatOakPlank dlg(item);

	dlg.DoModal();
	return true;
}

BOOL CDlgMatOakPlank::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_LightCtrl.b3Init(&m_Material->m_LightWood,this);
	m_DarkCtrl.b3Init(&m_Material->m_DarkWood,this);

	// TODO: Add extra initialization here
	m_PreviewMaterialCtrl.b3Init();
	m_MatSampler = new b3MaterialSampler(m_PreviewMaterialCtrl);
	m_MatSampler->b3SetMaterial(m_Material);

	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMatOakPlank::OnColorLight() 
{
	// TODO: Add your control notification handler code here
	if (m_LightCtrl.b3Select())
	{
		m_PreviewMaterialCtrl.b3Update(m_MatSampler);
	}
}

void CDlgMatOakPlank::OnColorDark() 
{
	// TODO: Add your control notification handler code here
	if (m_DarkCtrl.b3Select())
	{
		m_PreviewMaterialCtrl.b3Update(m_MatSampler);
	}
}

void CDlgMatOakPlank::OnSurfaceEdit()
{
	UpdateData();
	b3UpdateUI();
}

void CDlgMatOakPlank::OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnSurfaceEdit();
	*pResult = 0;
}

void CDlgMatOakPlank::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatSampler);
}
