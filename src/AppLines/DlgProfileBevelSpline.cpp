/*
**
**	$Filename:	DlgProfileBevelSpline.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for shapes which can be created via profiles
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

#include "DlgProfileBevelSpline.h"
#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        CDlgProfileBevelSpline implementation         **
**                                                                      **
*************************************************************************/

CDlgProfileBevelSpline::CDlgProfileBevelSpline(CWnd* pParent /*=NULL*/)
	: CB3ProfileShapeDialog(CDlgProfileBevelSpline::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProfileBevelSpline)
	//}}AFX_DATA_INIT
	m_xEdge     = 100.0;
	m_yEdge     = 100.0;
	m_Height    = 100.0;
	m_Oblique   =  10.0;
	m_yControls =   2;
	m_yDegree   =   1;
	b3Vector::b3Init(&m_Base);
	B3_BSPLINE_GEOMETRY(&m_Spline,m_Controls,m_Knots);
}


void CDlgProfileBevelSpline::DoDataExchange(CDataExchange* pDX)
{
	CB3ProfileShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProfileBevelSpline)
	DDX_Control(pDX, IDC_YCONTROLS_SPIN, m_yControlsCtrl);
	DDX_Control(pDX, IDC_YDEGREE_SPIN, m_yDegreeCtrl);
	DDX_Control(pDX, IDC_PROFILE_PREVIEW, m_Preview);
	DDX_Control(pDX, IDC_OBLIQUE_SPIN, m_ObliqueCtrl);
	DDX_Control(pDX, IDC_HEIGHT_SPIN, m_HeightCtrl);
	DDX_Control(pDX, IDC_XEDGE_SPIN, m_xEdgeCtrl);
	DDX_Control(pDX, IDC_YEDGE_SPIN, m_yEdgeCtrl);
	//}}AFX_DATA_MAP
	m_xEdgeCtrl.b3DDX(pDX, m_xEdge);
	m_yEdgeCtrl.b3DDX(pDX, m_yEdge);
	m_HeightCtrl.b3DDX(pDX, m_Height);
	m_ObliqueCtrl.b3DDX(pDX, m_Oblique);
	m_yControlsCtrl.b3DDX(pDX, m_yControls);
	m_yDegreeCtrl.b3DDX(pDX, m_yDegree);
}


BEGIN_MESSAGE_MAP(CDlgProfileBevelSpline, CB3ProfileShapeDialog)
	//{{AFX_MSG_MAP(CDlgProfileBevelSpline)
	ON_NOTIFY(WM_LBUTTONUP,IDC_OBLIQUE_SPIN, OnObliqueSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_XEDGE_SPIN, OnEdgeSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_YEDGE_SPIN, OnEdgeSpin)
	ON_EN_KILLFOCUS(IDC_OBLIQUE, OnObliqueEdit)
	ON_EN_KILLFOCUS(IDC_XEDGE, OnEdgeEdit)
	ON_EN_KILLFOCUS(IDC_YEDGE, OnEdgeEdit)
	ON_NOTIFY(WM_LBUTTONUP, IDC_YDEGREE_SPIN, OnDegreeSpin)
	ON_EN_KILLFOCUS(IDC_YDEGREE, OnDegreeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProfileBevelSpline message handlers

const char *CDlgProfileBevelSpline::b3GetSection()
{
	return "profile bevel spline";
}

void CDlgProfileBevelSpline::b3Init()
{
	CB3App *app = CB3GetApp();

	// Call base class
	CB3ProfileShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(m_Creation);
	m_xEdge     = app->b3ReadProfileFloat(b3MakeSection("x edge"), m_xEdge);
	m_xEdge     = app->b3ReadProfileFloat(b3MakeSection("y edge"), m_xEdge);
	m_Oblique   = app->b3ReadProfileFloat(b3MakeSection("oblique"),m_Oblique);
	m_Height    = app->b3ReadProfileFloat(b3MakeSection("height"), m_Height);
	m_yDegree   = app->GetProfileInt(CB3ClientString(),b3MakeSection("y degree"),m_yDegree);
	m_yControls = app->GetProfileInt(CB3ClientString(),b3MakeSection("y controls"),m_yControls);
}

BOOL CDlgProfileBevelSpline::OnInitDialog() 
{
	b3UpdateUI();
	CB3ProfileShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_xEdgeCtrl.b3SetAccel(5.0);
	m_yEdgeCtrl.b3SetAccel(5.0);
	m_HeightCtrl.b3SetAccel(5.0);

	m_Profile->b3ComputeProfile(&m_Spline,m_xEdge,m_yEdge,m_Oblique);
	m_Preview.b3Update(&m_Spline);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProfileBevelSpline::b3UpdateUI()
{
	b3_f64 min;

	min = B3_MIN(m_xEdge,m_yEdge);
	m_ObliqueCtrl.b3SetRange(0,min * 0.25);
	m_xEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX);
	m_yEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX);

	m_HeightCtrl.b3SetRange(0.1,DBL_MAX);
	m_yDegreeCtrl.b3SetRange(1,B3_MAX_DEGREE);
	m_yControlsCtrl.b3SetRange(m_yDegree + 1,B3_MAX_CONTROLS);
}

void CDlgProfileBevelSpline::OnObliqueEdit() 
{
	// Get value manually
	m_Oblique = m_ObliqueCtrl.b3GetPos();

	// Adjust range and affected values
	m_xEdge   = m_xEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX);
	m_yEdge   = m_yEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX);
	
	m_Profile->b3ComputeProfile(&m_Spline,m_xEdge,m_yEdge,m_Oblique);
	m_Preview.b3Update(&m_Spline);
}

void CDlgProfileBevelSpline::OnObliqueSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnObliqueEdit();
}

void CDlgProfileBevelSpline::OnEdgeEdit() 
{
	b3_f64 min;

	// Get values manually
	m_xEdge   = m_xEdgeCtrl.b3GetPos();
	m_yEdge   = m_yEdgeCtrl.b3GetPos();
	min       = B3_MIN(m_xEdge,m_yEdge);

	// Adjust range and affected value
	m_Oblique = m_ObliqueCtrl.b3SetRange(0.0,min * 0.25);
	
	m_Profile->b3ComputeProfile(&m_Spline,m_xEdge,m_yEdge,m_Oblique);
	m_Preview.b3Update(&m_Spline);
}

void CDlgProfileBevelSpline::OnEdgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdgeEdit();
}

void CDlgProfileBevelSpline::OnDegreeEdit() 
{
	// Get value manually
	m_yDegree   = m_yDegreeCtrl.b3GetPos();

	// Adjust range and affected value
	m_yControls = m_yControlsCtrl.b3SetRange(m_yDegree + 1,B3_MAX_DEGREE);
}

void CDlgProfileBevelSpline::OnDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnDegreeEdit();
	*pResult = 0;
}

void CDlgProfileBevelSpline::b3PostProcess()
{
	CB3App *app = CB3GetApp();

	// Write values into registry for next creation
	B3_ASSERT(m_Creation);
	app->b3WriteProfileFloat(b3MakeSection("x edge"), m_xEdge);
	app->b3WriteProfileFloat(b3MakeSection("y edge"), m_xEdge);
	app->b3WriteProfileFloat(b3MakeSection("oblique"),m_Oblique);
	app->b3WriteProfileFloat(b3MakeSection("height"), m_Height);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("y degree"),m_yDegree);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("y controls"),m_yControls);

	// Init spline
	m_Profile->b3ComputeProfile(&m_Spline,m_xEdge,m_yEdge,m_Oblique);
	m_Profile->b3ComputeShape(&m_Spline,m_Shape,m_Height,m_yDegree,m_yControls);

	CB3ProfileShapeDialog::b3PostProcess();
}
