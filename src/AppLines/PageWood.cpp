/*
**
**	$Filename:	PageWood.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood properties
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
#include "PageWood.h"

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
**                        CPageWood implementation                      **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CPageWood, CPropertyPage)

CPageWood::CPageWood() : CPropertyPage(CPageWood::IDD)
{
	m_Wood = null;
	//{{AFX_DATA_INIT(CPageWood)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_RotYCtrl.b3SetRange(-0.5 * M_PI,0.5 * M_PI);
	m_RotYCtrl.b3SetTicks(M_PI / 18,M_PI / 9);
	m_RotZCtrl.b3SetRange(-0.5 * M_PI,0.5 * M_PI);
	m_RotZCtrl.b3SetTicks(M_PI / 18,M_PI / 9);

	m_RingyCtrl.b3SetRange(0,1);
	m_GrainyCtrl.b3SetRange(0,1);
	m_GrainFrequencyCtrl.b3SetRange(0,50);
	m_RingSpacingCtrl.b3SetRange(0,2);
	m_RingFrequencyCtrl.b3SetRange(0,2);
	m_RingNoiseCtrl.b3SetRange(0,2);
	m_RingNoiseFrequencyCtrl.b3SetRange(0,2);
	m_TrunkWobbleCtrl.b3SetRange(0,2);
	m_TrunkWobbleFrequencyCtrl.b3SetRange(0,0.025);
	m_AngularWobbleCtrl.b3SetRange(0,1);
	m_AngularWobbleFrequencyCtrl.b3SetRange(0,1);
}

CPageWood::~CPageWood()
{
}

void CPageWood::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageWood)
	DDX_Control(pDX, IDC_ROT_Z, m_RotZCtrl);
	DDX_Control(pDX, IDC_ROT_Y, m_RotYCtrl);
	DDX_Control(pDX, IDC_SPIN_RINGY, m_RingyCtrl);
	DDX_Control(pDX, IDC_SPIN_GRAINY, m_GrainyCtrl);
	DDX_Control(pDX, IDC_SPIN_GRAIN_FREQUENCY, m_GrainFrequencyCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_SPACING, m_RingSpacingCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_FREQUENCY, m_RingFrequencyCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_NOISE, m_RingNoiseCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_NOISE_FREQUENCY, m_RingNoiseFrequencyCtrl);
	DDX_Control(pDX, IDC_SPIN_TRUNK_WOBBLE, m_TrunkWobbleCtrl);
	DDX_Control(pDX, IDC_SPIN_TRUNK_WOBBLE_FREQUENCY, m_TrunkWobbleFrequencyCtrl);
	DDX_Control(pDX, IDC_SPIN_ANGULAR_WOBBLE, m_AngularWobbleCtrl);
	DDX_Control(pDX, IDC_SPIN_ANGULAR_WOBBLE_FREQUENCY, m_AngularWobbleFrequencyCtrl);
	//}}AFX_DATA_MAP
	m_RotYCtrl.b3DDX(pDX, m_Wood->m_yRot);
	m_RotZCtrl.b3DDX(pDX, m_Wood->m_zRot);
	m_RingyCtrl.b3DDX(pDX, m_Wood->m_Ringy);
	m_GrainyCtrl.b3DDX(pDX, m_Wood->m_Grainy);
	m_GrainFrequencyCtrl.b3DDX(pDX, m_Wood->m_GrainFrequency);
	m_RingSpacingCtrl.b3DDX(pDX, m_Wood->m_RingSpacing);
	m_RingFrequencyCtrl.b3DDX(pDX, m_Wood->m_RingFrequency);
	m_RingNoiseCtrl.b3DDX(pDX, m_Wood->m_RingNoise);
	m_RingNoiseFrequencyCtrl.b3DDX(pDX, m_Wood->m_RingNoiseFrequency);
	m_TrunkWobbleCtrl.b3DDX(pDX, m_Wood->m_TrunkWobble);
	m_TrunkWobbleFrequencyCtrl.b3DDX(pDX, m_Wood->m_TrunkWobbleFrequency);
	m_AngularWobbleCtrl.b3DDX(pDX, m_Wood->m_AngularWobble);
	m_AngularWobbleFrequencyCtrl.b3DDX(pDX, m_Wood->m_AngularWobbleFrequency);
}


BEGIN_MESSAGE_MAP(CPageWood, CPropertyPage)
	//{{AFX_MSG_MAP(CPageWood)
	ON_EN_KILLFOCUS(IDC_EDIT_RINGY, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_GRAINY, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_GRAIN_FREQUENCY, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_SPACING, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_FREQUENCY, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_NOISE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_NOISE_FREQUENCY, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_TRUNK_WOBBLE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_TRUNK_WOBBLE_FREQUENCY, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_ANGULAR_WOBBLE, OnSurfaceEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_ANGULAR_WOBBLE_FREQUENCY, OnSurfaceEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ROT_Y, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ROT_Z, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RINGY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_GRAINY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_GRAIN_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_SPACING, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_NOISE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_NOISE_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_TRUNK_WOBBLE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_TRUNK_WOBBLE_FREQUENCY, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_ANGULAR_WOBBLE, OnSurfaceSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_ANGULAR_WOBBLE_FREQUENCY, OnSurfaceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageWood message handlers

void CPageWood::OnSurfaceEdit()
{
	UpdateData();
	b3UpdateUI();
}

void CPageWood::OnSurfaceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnSurfaceEdit();
	*pResult = 0;
}

void CPageWood::b3UpdateUI()
{
	CWnd *parent = GetParent();

	if (parent != null)
	{
		parent->SendMessage(WM_USER);
	}
}
