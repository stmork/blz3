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

#include "AppLines.h"
#include "DlgModellerInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	m_SnapToAngle = FALSE;
	m_SnapToGrid = FALSE;
	m_Unit = B3_UNIT_CM;
	m_Measure = B3_MEASURE_20;
	m_CustomMeasure = 1;
	//}}AFX_DATA_INIT
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
	DDX_Check(pDX, IDC_SNAP_TO_ANGLE, m_SnapToAngle);
	DDX_Check(pDX, IDC_SNAP_TO_GRID, m_SnapToGrid);
	DDX_CBIndex(pDX, IDC_UNIT, (int &)m_Unit);
	DDX_CBIndex(pDX, IDC_MEASURE, (int &)m_Measure);
	DDX_Text(pDX, IDC_CUSTOM_MEASURE, m_CustomMeasure);
	DDV_MinMaxInt(pDX, m_CustomMeasure, 1, 1000);
	//}}AFX_DATA_MAP
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
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_xFulcrumCtrl.b3SetDigits(5,2);
	m_xFulcrumCtrl.b3SetValue(m_ModellerInfo->m_Center.x);
	m_yFulcrumCtrl.b3SetDigits(5,2);
	m_yFulcrumCtrl.b3SetValue(m_ModellerInfo->m_Center.y);
	m_zFulcrumCtrl.b3SetDigits(5,2);
	m_zFulcrumCtrl.b3SetValue(m_ModellerInfo->m_Center.z);
	m_SnapToGridCtrl.b3SetDigits(3,2);
	m_SnapToGridCtrl.b3SetMin(epsilon);
	m_SnapToGridCtrl.b3SetValue(m_ModellerInfo->m_GridMove);
	m_SnapToAngleCtrl.b3SetDigits(3,0);
	m_SnapToAngleCtrl.b3SetMin(epsilon);
	m_SnapToAngleCtrl.b3SetValue(m_ModellerInfo->m_GridRot);
	m_SnapToGrid    = m_ModellerInfo->m_GridActive;
	m_SnapToAngle   = m_ModellerInfo->m_AngleActive;
	m_Unit          = m_ModellerInfo->b3GetUnit();
	m_Measure       = m_ModellerInfo->b3GetMeasureType();
	m_CustomMeasure = m_ModellerInfo->b3GetMeasure();
	UpdateData(FALSE);
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
	m_xFulcrumCtrl.b3SetValue(0);
	m_yFulcrumCtrl.b3SetValue(0);
	m_zFulcrumCtrl.b3SetValue(0);
	m_xFulcrumCtrl.SetFocus();
}

void CDlgModellerInfo::b3UpdateUI()
{
	GetDlgItem(IDC_STEP_GRID)->EnableWindow(m_SnapToGrid);
	GetDlgItem(IDC_STEP_ANGLE)->EnableWindow(m_SnapToAngle);
	GetDlgItem(IDC_CUSTOM_MEASURE)->EnableWindow(m_Measure == B3_MEASURE_CUSTOM);
}

void CDlgModellerInfo::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if (!m_xFulcrumCtrl.b3Check())
	{
		B3_BEEP;
		m_yFulcrumCtrl.SetFocus();
		return;
	}

	if (!m_xFulcrumCtrl.b3Check())
	{
		B3_BEEP;
		m_yFulcrumCtrl.SetFocus();
		return;
	}
	
	if (!m_zFulcrumCtrl.b3Check())
	{
		B3_BEEP;
		m_zFulcrumCtrl.SetFocus();
		return;
	}

	if (m_SnapToGrid)
	{
		if (!m_SnapToGridCtrl.b3Check())
		{
			B3_BEEP;
			m_SnapToGridCtrl.SetFocus();
			return;
		}
	}

	if (m_SnapToAngle)
	{
		if (!m_SnapToAngleCtrl.b3Check())
		{
			B3_BEEP;
			m_SnapToAngleCtrl.SetFocus();
			return;
		}
	}

	m_ModellerInfo->m_GridActive  = m_SnapToGrid;
	m_ModellerInfo->m_AngleActive = m_SnapToAngle;
	m_ModellerInfo->m_GridMove    = m_SnapToGridCtrl.m_Value;
	m_ModellerInfo->m_GridRot     = m_SnapToAngleCtrl.m_Value;
	m_ModellerInfo->m_Center.x    = m_xFulcrumCtrl.m_Value;
	m_ModellerInfo->m_Center.y    = m_yFulcrumCtrl.m_Value;
	m_ModellerInfo->m_Center.z    = m_zFulcrumCtrl.m_Value;
	m_ModellerInfo->b3SetUnit(m_Unit);
	if (m_Measure != B3_MEASURE_CUSTOM)
	{
		m_ModellerInfo->b3SetMeasure(m_Measure);
	}
	else
	{
		m_ModellerInfo->b3SetMeasure(m_CustomMeasure);
	}

	CB3GetApp()->b3SetData();
	CDialog::OnOK();
}
