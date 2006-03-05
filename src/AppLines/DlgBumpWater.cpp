/*
**
**	$Filename:	DlgBumpWater.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2006/03/05 22:12:31  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.4  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.3  2004/05/29 13:38:10  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.2  2004/05/14 16:16:52  sm
**	- Modified water
**	- Added some water values to its property dialog
**	
**	Revision 1.1  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	
*/

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

void CDlgBumpWater::b3InitDialog() 
{
	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpWater::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}
