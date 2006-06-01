/*
**
**	$Filename:	DlgModellerInfo.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for defining geometry wide values
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "DlgModellerInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.15  2006/06/01 14:17:24  smork
**	- Added frame work controls for simpler plugin generation:
**	  o CB3Checkbox
**	  o CB3TextEdit
**
**	Revision 1.14  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.13  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.12  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.11  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.10  2002/07/31 11:57:10  sm
**	- The nVidia OpenGL init bug fixed by using following work
**	  around: The wglMakeCurrent() method is invoked on
**	  every OnPaint(). This is configurable depending on the
**	  hostname.
**	
**	Revision 1.9  2002/03/08 16:46:14  sm
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
**	Revision 1.8  2002/02/10 20:03:18  sm
**	- Added grid raster
**	- Changed icon colors of shapes
**	
**	Revision 1.7  2002/02/08 15:53:37  sm
**	- Cleaned up makefiles for Un*x
**	- New dialog for print buffer size.
**	
**	Revision 1.6  2002/02/04 17:18:00  sm
**	- Added Measurement to modeller info.
**	
**	Revision 1.5  2002/02/03 21:42:30  sm
**	- Added measurement printing. The measure itself is missing yet.
**	  The support is done in b3RenderView and CAppRenderView.
**	- Added support for units in b3ModellerInfo
**	- Cleaned up some accelerators. Now arrow keys are working
**	  again. The del key is working correctly inside edit controls again.
**	
**	Revision 1.4  2002/01/05 22:22:50  sm
**	- Code cleanup
**	
**	Revision 1.3  2001/12/26 12:00:36  sm
**	- Fixed modeller info dialog
**	
**	Revision 1.2  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**	Revision 1.1  2001/11/28 19:54:03  sm
**	- Adding modelling info dialog
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgModellerInfo implementation               **
**                                                                      **
*************************************************************************/

CDlgModellerInfo::CDlgModellerInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModellerInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModellerInfo)
	m_Unit = B3_UNIT_CM;
	m_Measure = B3_MEASURE_20;
	m_CustomMeasure = 1;
	m_LightMode = 0;
	//}}AFX_DATA_INIT
	m_SnapToGridCtrl.b3SetDigits(3,2);
	m_SnapToGridCtrl.b3SetMin(b3Scene::epsilon);
	m_SnapToAngleCtrl.b3SetDigits(3,0);
	m_SnapToAngleCtrl.b3SetMin(b3Scene::epsilon);
}

void CDlgModellerInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModellerInfo)
	DDX_Control(pDX, IDC_STEP_GRID, m_SnapToGridCtrl);
	DDX_Control(pDX, IDC_STEP_ANGLE, m_SnapToAngleCtrl);
	DDX_Control(pDX, IDC_FULCRUM_Z, m_zFulcrumCtrl);
	DDX_Control(pDX, IDC_FULCRUM_Y, m_yFulcrumCtrl);
	DDX_Control(pDX, IDC_FULCRUM_X, m_xFulcrumCtrl);
	DDX_Control(pDX, IDC_SNAP_TO_ANGLE, m_SnapToAngle);
	DDX_Control(pDX, IDC_SNAP_TO_GRID, m_SnapToGrid);
	DDX_CBIndex(pDX, IDC_UNIT, m_Unit);
	DDX_CBIndex(pDX, IDC_MEASURE, m_Measure);
	DDX_Text(pDX, IDC_CUSTOM_MEASURE, m_CustomMeasure);
	DDV_MinMaxInt(pDX, m_CustomMeasure, 1, 1000);
	DDX_Radio(pDX, IDC_GL_LIGHTS_SIMPLE, m_LightMode);
	//}}AFX_DATA_MAP
	m_CenterCtrl.b3DDX(pDX);
	m_SnapToGridCtrl.b3DDX (pDX,m_ModellerInfo->m_GridMove);
	m_SnapToAngleCtrl.b3DDX(pDX,m_ModellerInfo->m_GridRot);
	m_SnapToGrid.b3DDX(pDX, m_ModellerInfo->m_GridActive);
	m_SnapToAngle.b3DDX(pDX, m_ModellerInfo->m_AngleActive);
}


BEGIN_MESSAGE_MAP(CDlgModellerInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgModellerInfo)
	ON_BN_CLICKED(IDC_SNAP_TO_GRID, OnSnap)
	ON_BN_CLICKED(IDC_FULCRUM_CLR, OnFulcrumClear)
	ON_BN_CLICKED(IDC_SNAP_TO_ANGLE, OnSnap)
	ON_CBN_SELCHANGE(IDC_MEASURE, OnSelchangeMeasure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModellerInfo message handlers

BOOL CDlgModellerInfo::OnInitDialog() 
{
	m_CenterCtrl.b3Init(&m_ModellerInfo->m_Center,&m_xFulcrumCtrl,&m_yFulcrumCtrl,&m_zFulcrumCtrl);
	m_Unit          = m_ModellerInfo->m_Unit;
	m_Measure       = m_ModellerInfo->m_Measure;
	m_CustomMeasure = m_ModellerInfo->m_CustomMeasure;
	m_LightMode     = m_ModellerInfo->m_UseSceneLights ? 1 : 0;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModellerInfo::OnSelchangeMeasure() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgModellerInfo::OnSnap() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgModellerInfo::OnFulcrumClear() 
{
	// TODO: Add your control notification handler code here
	m_xFulcrumCtrl.b3SetPos(0);
	m_yFulcrumCtrl.b3SetPos(0);
	m_zFulcrumCtrl.b3SetPos(0);
	m_xFulcrumCtrl.SetFocus();
}

void CDlgModellerInfo::b3UpdateUI()
{
	GetDlgItem(IDC_STEP_GRID)->EnableWindow(m_ModellerInfo->m_GridActive);
	GetDlgItem(IDC_STEP_ANGLE)->EnableWindow(m_ModellerInfo->m_AngleActive);
	GetDlgItem(IDC_CUSTOM_MEASURE)->EnableWindow(m_Measure == B3_MEASURE_CUSTOM);
}

void CDlgModellerInfo::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();

	m_ModellerInfo->m_Unit        = (b3_unit)m_Unit;
	m_ModellerInfo->m_UseSceneLights = m_LightMode == 1;
	if (m_Measure != B3_MEASURE_CUSTOM)
	{
		m_ModellerInfo->b3SetMeasure((b3_measure)m_Measure);
	}
	else
	{
		m_ModellerInfo->b3SetMeasure(m_CustomMeasure);
	}

	CB3GetApp()->b3SetData();
}
