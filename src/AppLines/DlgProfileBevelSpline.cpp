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

#include "AppLines.h"
#include "DlgProfileBevelSpline.h"
#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/03/05 20:38:24  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgProfileBevelSpline implementation         **
**                                                                      **
*************************************************************************/

CDlgProfileBevelSpline::CDlgProfileBevelSpline(CWnd* pParent /*=NULL*/)
	: CB3ProfileShapeDialog(CDlgProfileBevelSpline::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProfileBevelSpline)
	m_xEdge = 100.0;
	m_yEdge = 100.0;
	m_Height = 100.0;
	m_Oblique = 10.0;
	m_yControls = 2;
	m_yDegree = 1;
	//}}AFX_DATA_INIT
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
	DDX_Text(pDX, IDC_XEDGE, m_xEdge);
	DDX_Text(pDX, IDC_YEDGE, m_yEdge);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_OBLIQUE, m_Oblique);
	DDX_Text(pDX, IDC_YCONTROLS, m_yControls);
	DDX_Text(pDX, IDC_YDEGREE, m_yDegree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProfileBevelSpline, CB3ProfileShapeDialog)
	//{{AFX_MSG_MAP(CDlgProfileBevelSpline)
	ON_NOTIFY(WM_LBUTTONUP,IDC_OBLIQUE_SPIN, OnObliqueSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_XEDGE_SPIN, OnEdgeSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_YEDGE_SPIN, OnEdgeSpin)
	ON_EN_KILLFOCUS(IDC_OBLIQUE, OnObliqueEdit)
	ON_EN_KILLFOCUS(IDC_XEDGE, OnEdgeEdit)
	ON_EN_KILLFOCUS(IDC_YEDGE, OnEdgeEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_XEDGE_SPIN, OnDegreeSpin)
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

	CB3ProfileShapeDialog::b3Init();
	m_xEdge     = app->b3ReadProfileFloat(b3MakeSection("x edge"), m_xEdge);
	m_xEdge     = app->b3ReadProfileFloat(b3MakeSection("y edge"), m_xEdge);
	m_Oblique   = app->b3ReadProfileFloat(b3MakeSection("oblique"),m_Oblique);
	m_Height    = app->b3ReadProfileFloat(b3MakeSection("height"), m_Height);
	m_yDegree   = app->GetProfileInt(CB3ClientString(),b3MakeSection("y degree"),m_yDegree);
	m_yControls = app->GetProfileInt(CB3ClientString(),b3MakeSection("y controls"),m_yControls);
}

BOOL CDlgProfileBevelSpline::OnInitDialog() 
{
	CB3ProfileShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3UpdateUI();
	m_xEdgeCtrl.b3SetAccel(5.0);
	m_yEdgeCtrl.b3SetAccel(5.0);
	m_HeightCtrl.b3SetAccel(5.0);

	// Init profile values
	m_ObliqueCtrl.b3SetPos(m_Oblique);
	m_xEdgeCtrl.b3SetPos(m_xEdge);
	m_yEdgeCtrl.b3SetPos(m_yEdge);

	// Init vertical spline
	m_HeightCtrl.b3SetPos(m_Height);
	m_yDegreeCtrl.SetPos(m_yDegree);
	m_yControlsCtrl.SetPos(m_yControls);

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
	m_xEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX,2,1.0);
	m_yEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX,2,1.0);

	m_HeightCtrl.b3SetRange(0.1,DBL_MAX);
	m_yDegreeCtrl.SetRange(1,B3_MAX_DEGREE);
	m_yControlsCtrl.SetRange(m_yDegree + 1,B3_MAX_CONTROLS);
}

void CDlgProfileBevelSpline::b3PostProcess()
{
	CB3App        *app = CB3GetApp();
	b3SplineShape *shape = (b3SplineShape *)m_Shape;
	b3_index       index,x,y;
	b3_f64         z;

	// Write values into registry for next creation
	app->b3WriteProfileFloat(b3MakeSection("x edge"), m_xEdge);
	app->b3WriteProfileFloat(b3MakeSection("y edge"), m_xEdge);
	app->b3WriteProfileFloat(b3MakeSection("oblique"),m_Oblique);
	app->b3WriteProfileFloat(b3MakeSection("height"), m_Height);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("y degree"),m_yDegree);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("y controls"),m_yControls);

	// Init spline
	m_Profile->b3ComputeProfile(&m_Spline,m_xEdge,m_yEdge,m_Oblique);

	// Init control points
	shape->b3Init(m_Spline.degree,m_yDegree,m_Spline.control_num,m_yControls);
	for (y = 0;y < shape->m_Spline[1].control_num;y++)
	{
		index = y * shape->m_Spline[1].offset;
		z     = b3Spline::b3ArcLengthParameter(y,m_yControls,m_yDegree,m_Height);
		for (x = 0;x < shape->m_Spline[0].control_num;x++)
		{
			shape->m_Controls[index + x]   = m_Controls[x];
			shape->m_Controls[index + x].z = z;
		}
	}

	CB3ProfileShapeDialog::b3PostProcess();
}

void CDlgProfileBevelSpline::OnObliqueEdit() 
{
	m_Oblique = m_ObliqueCtrl.b3GetPos();
	m_xEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX,2,1.0);
	m_yEdgeCtrl.b3SetRange(m_Oblique * 4,DBL_MAX,2,1.0);
	m_xEdge   = m_xEdgeCtrl.b3GetPos();
	m_yEdge   = m_yEdgeCtrl.b3GetPos();
	
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

	m_xEdge   = m_xEdgeCtrl.b3GetPos();
	m_yEdge   = m_yEdgeCtrl.b3GetPos();
	min = B3_MIN(m_xEdge,m_yEdge);
	m_ObliqueCtrl.b3SetRange(0.0,min * 0.25);
	m_Oblique = m_ObliqueCtrl.b3GetPos();
	
	m_Profile->b3ComputeProfile(&m_Spline,m_xEdge,m_yEdge,m_Oblique);
	m_Preview.b3Update(&m_Spline);
}

void CDlgProfileBevelSpline::OnEdgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdgeEdit();
}

void CDlgProfileBevelSpline::OnDegreeEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_yControlsCtrl.SetRange(m_yDegree + 1,B3_MAX_DEGREE);
}

void CDlgProfileBevelSpline::OnDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_yControlsCtrl.SetRange(m_yDegree + 1,B3_MAX_DEGREE);
	
	*pResult = 0;
}
