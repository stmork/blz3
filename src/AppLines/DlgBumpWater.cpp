/*
**
**	$Filename:	DlgBumpWater.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Water surface
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

#include "b3ExampleScene.h"
#include "DlgBumpWater.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpWater::CDlgBumpWater(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpWater::IDD, pParent)
{
	m_Bump                  = (b3BumpWater *)item;
	m_PageBump.m_Bump       = m_Bump;
	m_PageScaling.m_Scaling = m_Bump;

	m_BumpScene = b3ExampleScene::b3CreateBump(&m_BumpHead, pDoc->b3GetParentShading());
	m_BumpHead->b3Append(m_Bump);
	//{{AFX_DATA_INIT(CDlgBumpWater)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ScaleTimeCtrl.b3SetRange(0.0,100.0);
	m_ScaleTimeCtrl.b3SetIncrement(0.1);
	m_ScaleTimeCtrl.b3SetDigits(3,2);
	m_WindAmpCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_PERCENT);
	m_WindFreqCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_PERCENT);
	m_MinWindCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_PERCENT);
	m_KmCtrl.b3SetUnit(b3ControlUnits::B3_UNIT_PERCENT);
}

CDlgBumpWater::~CDlgBumpWater()
{
	m_BumpHead->b3RemoveAll();
	delete m_BumpScene;
}

void CDlgBumpWater::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpWater)
	DDX_Control(pDX, IDC_SPIN_WIND_AMP, m_WindAmpCtrl);
	DDX_Control(pDX, IDC_SPIN_WIND_FREQ, m_WindFreqCtrl);
	DDX_Control(pDX, IDC_SPIN_WIND_MIN, m_MinWindCtrl);
	DDX_Control(pDX, IDC_SPIN_SCALE_TIME, m_ScaleTimeCtrl);
	DDX_Control(pDX, IDC_SPIN_KM, m_KmCtrl);
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
	m_WindAmpCtrl.b3DDX(pDX,   m_Bump->m_WindAmp);
	m_WindFreqCtrl.b3DDX(pDX,  m_Bump->m_WindFreq);
	m_MinWindCtrl.b3DDX(pDX,   m_Bump->m_MinWind);
	m_ScaleTimeCtrl.b3DDX(pDX, m_Bump->m_ScaleTime);
	m_KmCtrl.b3DDX(pDX,        m_Bump->m_Km);
}


BEGIN_MESSAGE_MAP(CDlgBumpWater, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpWater)
	ON_EN_KILLFOCUS(IDC_EDIT_WIND_AMP,   OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_WIND_FREQ,  OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_WIND_MIN,   OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_SCALE_TIME, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_KM,         OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_WIND_AMP,   OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_WIND_FREQ,  OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_WIND_MIN,   OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_SCALE_TIME, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_KM,         OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWater message handlers

void CDlgBumpWater::b3Register()
{
	b3Loader::b3AddClassType(BUMP_WATER,IDS_BUMP_WATER,IDI_BUMP_WATER,b3Edit,b3Edit);
}

b3_bool CDlgBumpWater::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpWater dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpWater::b3PreInitDialog()
{
}

void CDlgBumpWater::b3PostInitDialog() 
{
	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpWater::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}
