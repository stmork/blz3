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
	DDX_Control(pDX, IDC_SNAP_TO_GRID, m_SnapToGrid);
	DDX_Control(pDX, IDC_SNAP_TO_ANGLE_OBJECT, m_SnapToAngleObject);
	DDX_Control(pDX, IDC_SNAP_TO_ANGLE_CAMERA, m_SnapToAngleCamera);
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
	m_SnapToAngleCamera.b3DDX(pDX, m_ModellerInfo->m_AngleGridCamera);
	m_SnapToAngleObject.b3DDX(pDX, m_ModellerInfo->m_AngleGridObjects);
}


BEGIN_MESSAGE_MAP(CDlgModellerInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgModellerInfo)
	ON_BN_CLICKED(IDC_SNAP_TO_GRID, OnSnap)
	ON_BN_CLICKED(IDC_FULCRUM_CLR, OnFulcrumClear)
	ON_BN_CLICKED(IDC_SNAP_TO_ANGLE_CAMERA, OnSnap)
	ON_BN_CLICKED(IDC_SNAP_TO_ANGLE_OBJECT, OnSnap)
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
	GetDlgItem(IDC_STEP_ANGLE)->EnableWindow(m_ModellerInfo->m_AngleGridObjects || m_ModellerInfo->m_AngleGridCamera);
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
