/*
**
**	$Filename:	DlgCreateStencil.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Fast stencil creation on shape creation
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "DlgCreateStencil.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**
**	Revision 1.1  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCreateStencil implementation              **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CDlgCreateStencil, CPropertyPage)

CDlgCreateStencil::CDlgCreateStencil() : CPropertyPage(CDlgCreateStencil::IDD)
{
	//{{AFX_DATA_INIT(CDlgCreateStencil)
	m_ReallyCreate = FALSE;
	m_Unit = 1;
	m_yEnd = _T("");
	m_yStart = _T("");
	m_xEnd = _T("");
	m_xStart = _T("");
	//}}AFX_DATA_INIT
	m_Stencil = null;
}

CDlgCreateStencil::~CDlgCreateStencil()
{
}

void CDlgCreateStencil::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateStencil)
	DDX_Control(pDX, IDC_X_START, m_xStartCtrl);
	DDX_Control(pDX, IDC_X_END, m_xEndCtrl);
	DDX_Control(pDX, IDC_Y_START, m_yStartCtrl);
	DDX_Control(pDX, IDC_Y_END, m_yEndCtrl);
	DDX_Check(pDX, IDC_REALLY_CREATE, m_ReallyCreate);
	DDX_Text(pDX, IDC_Y_END_LEGEND, m_yEnd);
	DDX_Text(pDX, IDC_Y_START_LEGEND, m_yStart);
	DDX_Text(pDX, IDC_X_END_LEGEND, m_xEnd);
	DDX_Text(pDX, IDC_X_START_LEGEND, m_xStart);
	DDX_Radio(pDX, IDC_RELATIVE, m_Unit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateStencil, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgCreateStencil)
	ON_BN_CLICKED(IDC_UNIT, OnUnitChanged)
	ON_BN_CLICKED(IDC_RELATIVE, OnUnitChanged)
	ON_EN_KILLFOCUS(IDC_X_START, OnBoundChanged)
	ON_EN_KILLFOCUS(IDC_X_END, OnBoundChanged)
	ON_EN_KILLFOCUS(IDC_Y_START, OnBoundChanged)
	ON_EN_KILLFOCUS(IDC_Y_END, OnBoundChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateStencil message handlers

BOOL CDlgCreateStencil::OnInitDialog() 
{
	CB3App       *app = CB3GetApp();

	m_Shape->b3GetStencilBoundInfo(&m_Bound);

	m_Unit         = app->GetProfileInt(CB3ClientString(),"stencil.unit",1);
	m_ReallyCreate = app->GetProfileInt(CB3ClientString(),"stencil.really create",1);
	m_Limit.x1 = app->b3ReadProfileFloat("stencil.xMin",m_Bound.xMin);
	m_Limit.x2 = app->b3ReadProfileFloat("stencil.xMax",m_Bound.xMax);
	m_Limit.y1 = app->b3ReadProfileFloat("stencil.yMin",m_Bound.yMin);
	m_Limit.y2 = app->b3ReadProfileFloat("stencil.yMax",m_Bound.yMax);

	// Init horizontal legend
	switch(m_Bound.xUnit)
	{
	case B3_STENCIL_UNIT:
		m_xStart.LoadString(IDS_STENCIL_START_UNIT);
		m_xEnd.LoadString(IDS_STENCIL_END_UNIT);
		break;
	case B3_STENCIL_LENGTH:
		m_xStart.LoadString(IDS_STENCIL_START_LENGTH);
		m_xEnd.LoadString(IDS_STENCIL_END_LENGTH);
		break;
	case B3_STENCIL_ANGLE:
		m_xStart.LoadString(IDS_STENCIL_START_ANGLE);
		m_xEnd.LoadString(IDS_STENCIL_END_ANGLE);
		break;
	}

	// Init vertical legend
	switch(m_Bound.yUnit)
	{
	case B3_STENCIL_UNIT:
		m_yStart.LoadString(IDS_STENCIL_START_UNIT);
		m_yEnd.LoadString(IDS_STENCIL_END_UNIT);
		break;
	case B3_STENCIL_LENGTH:
		m_yStart.LoadString(IDS_STENCIL_START_LENGTH);
		m_yEnd.LoadString(IDS_STENCIL_END_LENGTH);
		break;
	case B3_STENCIL_ANGLE:
		m_yStart.LoadString(IDS_STENCIL_START_ANGLE);
		m_yEnd.LoadString(IDS_STENCIL_END_ANGLE);
		break;
	}
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_xStartCtrl.b3SetDigits(3,2);
	m_xEndCtrl.b3SetDigits(3,2);
	m_yStartCtrl.b3SetDigits(3,2);
	m_yEndCtrl.b3SetDigits(3,2);
	b3UpdateUI();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCreateStencil::OnUnitChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgCreateStencil::OnBoundChanged() 
{
	// TODO: Add your control notification handler code here
	m_Limit.x1 = m_xStartCtrl.m_Value;
	m_Limit.x2 = m_xEndCtrl.m_Value;
	m_Limit.y1 = m_yStartCtrl.m_Value;
	m_Limit.y2 = m_yEndCtrl.m_Value;

	// Normalize values
	if (m_Unit != 0)
	{
		m_Limit.x1 /= m_Bound.xFactor;
		m_Limit.x2 /= m_Bound.xFactor;
		m_Limit.y1 /= m_Bound.yFactor;
		m_Limit.y2 /= m_Bound.yFactor;
	}
}

void CDlgCreateStencil::b3UpdateUI()
{
	if (m_Unit == 0)
	{
		m_xStartCtrl.b3SetRange(m_Bound.xMin,m_Bound.xMax);
		m_xEndCtrl.b3SetRange(  m_Bound.xMin,m_Bound.xMax);
		m_yStartCtrl.b3SetRange(m_Bound.yMin,m_Bound.yMax);
		m_yEndCtrl.b3SetRange(  m_Bound.yMin,m_Bound.yMax);
		m_xStartCtrl.b3SetValue(m_Limit.x1);
		m_xEndCtrl.b3SetValue(  m_Limit.x2);
		m_yStartCtrl.b3SetValue(m_Limit.y1);
		m_yEndCtrl.b3SetValue(  m_Limit.y2);
	}
	else
	{
		m_xStartCtrl.b3SetRange(m_Bound.xMin * m_Bound.xFactor,m_Bound.xMax * m_Bound.xFactor);
		m_xEndCtrl.b3SetRange(  m_Bound.xMin * m_Bound.xFactor,m_Bound.xMax * m_Bound.xFactor);
		m_yStartCtrl.b3SetRange(m_Bound.yMin * m_Bound.yFactor,m_Bound.yMax * m_Bound.yFactor);
		m_yEndCtrl.b3SetRange(  m_Bound.yMin * m_Bound.yFactor,m_Bound.yMax * m_Bound.yFactor);
		m_xStartCtrl.b3SetValue(m_Limit.x1 * m_Bound.xFactor);
		m_xEndCtrl.b3SetValue(  m_Limit.x2 * m_Bound.xFactor);
		m_yStartCtrl.b3SetValue(m_Limit.y1 * m_Bound.yFactor);
		m_yEndCtrl.b3SetValue(  m_Limit.y2 * m_Bound.yFactor);
	}
}

BOOL CDlgCreateStencil::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class

	// Get new values in case the geometry page has changed some values
	// which may affect scaling factors.
	m_Shape->b3GetStencilBoundInfo(&m_Bound);
	b3UpdateUI();
	return CPropertyPage::OnSetActive();
}

void CDlgCreateStencil::b3PostProcess() 
{
	// TODO: Add extra validation here
	CB3App *app = CB3GetApp();

	if (m_ReallyCreate)
	{
		// Create stencil
		m_Stencil = new b3CondRectangle(COND_ARECTANGLE);
		m_Stencil->m_xStart = m_Limit.x1;
		m_Stencil->m_xEnd   = m_Limit.x2;
		m_Stencil->m_yStart = m_Limit.y1;
		m_Stencil->m_yEnd   = m_Limit.y2;
		m_Stencil->m_Flags  = RCF_ACTIVE;

		// Write defaults to registry
		app->b3WriteProfileFloat("stencil.xMin",m_Limit.x1);
		app->b3WriteProfileFloat("stencil.xMax",m_Limit.x2);
		app->b3WriteProfileFloat("stencil.yMin",m_Limit.y1);
		app->b3WriteProfileFloat("stencil.yMax",m_Limit.y2);
		app->WriteProfileInt(CB3ClientString(),"stencil.unit",m_Unit);
	}
	app->WriteProfileInt(CB3ClientString(),"stencil.really create",m_ReallyCreate);
}
