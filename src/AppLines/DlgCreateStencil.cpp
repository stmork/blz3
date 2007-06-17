/*
**
**	$Filename:	DlgCreateStencil.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "DlgCreateStencil.h"

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
	m_yEndLegend = _T("");
	m_yStartLegend = _T("");
	m_xEndLegend = _T("");
	m_xStartLegend = _T("");
	m_Unit = 1;
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
	DDX_Control(pDX, IDC_X_START_SPIN, m_xStartCtrl);
	DDX_Control(pDX, IDC_X_END_SPIN, m_xEndCtrl);
	DDX_Control(pDX, IDC_Y_START_SPIN, m_yStartCtrl);
	DDX_Control(pDX, IDC_Y_END_SPIN, m_yEndCtrl);
	DDX_Check(pDX, IDC_REALLY_CREATE, m_ReallyCreate);
	DDX_Text(pDX, IDC_Y_END_LEGEND, m_yEndLegend);
	DDX_Text(pDX, IDC_Y_START_LEGEND, m_yStartLegend);
	DDX_Text(pDX, IDC_X_END_LEGEND, m_xEndLegend);
	DDX_Text(pDX, IDC_X_START_LEGEND, m_xStartLegend);
	DDX_Radio(pDX, IDC_RELATIVE, m_Unit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateStencil, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgCreateStencil)
	ON_BN_CLICKED(IDC_UNIT, OnUnitChanged)
	ON_BN_CLICKED(IDC_RELATIVE, OnUnitChanged)
	ON_EN_KILLFOCUS(IDC_X_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_X_END, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_START, OnLimitChanged)
	ON_EN_KILLFOCUS(IDC_Y_END, OnLimitChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateStencil message handlers

void CDlgCreateStencil::b3Init()
{
	CB3App *app = CB3GetApp();

	B3_ASSERT(m_Shape != null);
	m_Section.Format("stencil [%08x]",m_Shape->b3GetClassType());

	m_Shape->b3GetStencilBoundInfo(&m_Bound);
	m_Unit         = app->GetProfileInt(CB3ClientString(),m_Section + ".unit",1);
	m_ReallyCreate = app->GetProfileInt(CB3ClientString(),m_Section + ".really create",1);
	m_Limit.x1     = app->b3ReadProfileFloat(m_Section + ".xMin",m_Bound.xInfo.min);
	m_Limit.x2     = app->b3ReadProfileFloat(m_Section + ".xMax",m_Bound.xInfo.max);
	m_Limit.y1     = app->b3ReadProfileFloat(m_Section + ".yMin",m_Bound.yInfo.min);
	m_Limit.y2     = app->b3ReadProfileFloat(m_Section + ".yMax",m_Bound.yInfo.max);
}

BOOL CDlgCreateStencil::OnInitDialog() 
{
	// Init legends
	m_xStart.b3Init(&m_xStartCtrl,&m_xStartLegend,&m_Bound.xInfo,B3_COND_CTRL_START);
	m_xEnd.b3Init(  &m_xEndCtrl,  &m_xEndLegend,  &m_Bound.xInfo,B3_COND_CTRL_END);
	m_yStart.b3Init(&m_yStartCtrl,&m_yStartLegend,&m_Bound.yInfo,B3_COND_CTRL_START);
	m_yEnd.b3Init(  &m_yEndCtrl,  &m_yEndLegend,  &m_Bound.yInfo,B3_COND_CTRL_END);

	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
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

void CDlgCreateStencil::OnLimitChanged() 
{
	// TODO: Add your control notification handler code here
	b3_bool absolute = m_Unit == 0;

	m_Limit.x1 = m_xStart.b3GetPos(absolute);
	m_Limit.x2 = m_xEnd.b3GetPos(absolute);
	m_Limit.y1 = m_yStart.b3GetPos(absolute);
	m_Limit.y2 = m_yEnd.b3GetPos(absolute);
}

void CDlgCreateStencil::b3UpdateUI()
{
	b3_bool absolute = m_Unit == 0;

	m_xStart.b3SetPos(absolute,m_Limit.x1);
	m_xEnd.b3SetPos(  absolute,m_Limit.x2);
	m_yStart.b3SetPos(absolute,m_Limit.y1);
	m_yEnd.b3SetPos(  absolute,m_Limit.y2);
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
		app->b3WriteProfileFloat(m_Section + ".xMin",m_Limit.x1);
		app->b3WriteProfileFloat(m_Section + ".xMax",m_Limit.x2);
		app->b3WriteProfileFloat(m_Section + ".yMin",m_Limit.y1);
		app->b3WriteProfileFloat(m_Section + ".yMax",m_Limit.y2);
		app->WriteProfileInt(CB3ClientString(),m_Section + ".unit",m_Unit);
	}
	app->WriteProfileInt(CB3ClientString(),m_Section + ".really create",m_ReallyCreate);
}
