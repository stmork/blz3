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
**	Revision 1.5  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**
**	Revision 1.4  2004/04/26 12:27:43  sm
**	- Added following dialogs:
**	  o granite
**	  o chess
**	- Added scaling to wood properties
**	
**	Revision 1.3  2004/04/24 20:15:52  sm
**	- Further slide material dialog development
**	
**	Revision 1.2  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
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

CPageWood::CPageWood() : CB3PropertyPage(CPageWood::IDD)
{
	m_Wood = null;
	//{{AFX_DATA_INIT(CPageWood)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_RotYCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_DEGREE);
    m_RotYCtrl.b3SetRange(-0.5 * M_PI,0.5 * M_PI);
	m_RotYCtrl.b3SetTicks(M_PI / 24,M_PI / 12);
	m_RotZCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_DEGREE);
	m_RotZCtrl.b3SetRange(-0.5 * M_PI,0.5 * M_PI);
	m_RotZCtrl.b3SetTicks(M_PI / 24,M_PI / 12);

	m_RingyCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_PERCENT);
	m_GrainyCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_PERCENT);
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
	CB3PropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageWood)
	DDX_Control(pDX, IDC_LEGEND_ZROT, m_zRotLegend);
	DDX_Control(pDX, IDC_LEGEND_YROT, m_yRotLegend);
	DDX_Control(pDX, IDC_ROT_Z, m_RotZCtrl);
	DDX_Control(pDX, IDC_ROT_Y, m_RotYCtrl);
	DDX_Control(pDX, IDC_SPIN_RINGY, m_RingyCtrl);
	DDX_Control(pDX, IDC_SPIN_GRAINY, m_GrainyCtrl);
	DDX_Control(pDX, IDC_SPIN_GRAIN_FREQUENCY, m_GrainFrequencyCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_SPACING, m_RingSpacingCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_NOISE_FREQUENCY, m_RingNoiseFrequencyCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_NOISE, m_RingNoiseCtrl);
	DDX_Control(pDX, IDC_SPIN_RING_FREQUENCY, m_RingFrequencyCtrl);
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


BEGIN_MESSAGE_MAP(CPageWood, CB3PropertyPage)
	//{{AFX_MSG_MAP(CPageWood)
	ON_EN_KILLFOCUS(IDC_EDIT_RINGY, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_GRAINY, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_GRAIN_FREQUENCY, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_SPACING, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_FREQUENCY, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_NOISE, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_RING_NOISE_FREQUENCY, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_TRUNK_WOBBLE, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_TRUNK_WOBBLE_FREQUENCY, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_ANGULAR_WOBBLE, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_ANGULAR_WOBBLE_FREQUENCY, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ROT_Y, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_ROT_Z, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RINGY, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_GRAINY, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_GRAIN_FREQUENCY, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_SPACING, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_FREQUENCY, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_NOISE, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_RING_NOISE_FREQUENCY, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_TRUNK_WOBBLE, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_TRUNK_WOBBLE_FREQUENCY, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_ANGULAR_WOBBLE, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_ANGULAR_WOBBLE_FREQUENCY, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageWood message handlers

BOOL CPageWood::OnInitDialog() 
{
	CB3PropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3PrintLegend();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageWood::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	b3PrintLegend();
	return CB3PropertyPage::PreTranslateMessage(pMsg);
}

void CPageWood::b3PrintLegend()
{
	CString  title;

	title.Format(IDS_TITLE_YROT,m_RotYCtrl.b3GetPos() * 180 / M_PI);
	GetDlgItem(IDC_LEGEND_YROT)->SetWindowText(title);

	title.Format(IDS_TITLE_ZROT,m_RotZCtrl.b3GetPos() * 180 / M_PI);
	GetDlgItem(IDC_LEGEND_ZROT)->SetWindowText(title);
}
