/*
**
**	$Filename:	DlgCreateSplineShape.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for creating spline shapes
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
#include "DlgCreateRotShape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**
**	Revision 1.1  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCreateSplineShape implementation          **
**                                                                      **
*************************************************************************/

CDlgCreateRotShape::CDlgCreateRotShape() : CB3ProfileShapeDialog(CDlgCreateRotShape::IDD)
{
	//{{AFX_DATA_INIT(CDlgCreateRotShape)
	m_Closed = FALSE;
	//}}AFX_DATA_INIT
}

CDlgCreateRotShape::~CDlgCreateRotShape()
{
}

void CDlgCreateRotShape::DoDataExchange(CDataExchange* pDX)
{
	CB3ProfileShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateRotShape)
	DDX_Control(pDX, IDC_SUBDIV_SPIN, m_SubdivCtrl);
	DDX_Control(pDX, IDC_YSIZE_LEGEND, m_LegendCtrl);
	DDX_Control(pDX, IDC_XSIZE, m_xSizeCtrl);
	DDX_Control(pDX, IDC_YSIZE, m_ySizeCtrl);
	DDX_Control(pDX, IDC_YDEGREE_SPIN, m_DegreeCtrl);
	DDX_Control(pDX, IDC_YCONTROLS_SPIN, m_ControlsCtrl);
	DDX_Check(pDX, IDC_CLOSED, m_Closed);
	//}}AFX_DATA_MAP
	m_DegreeCtrl.b3DDX(  pDX, m_Degree);
	m_ControlsCtrl.b3DDX(pDX, m_Controls);
	m_xSizeCtrl.b3DDX(   pDX, m_xSize);
	m_ySizeCtrl.b3DDX(   pDX, m_ySize);
	m_SubdivCtrl.b3DDX(  pDX, m_SubDiv);
}


BEGIN_MESSAGE_MAP(CDlgCreateRotShape, CB3ProfileShapeDialog)
	//{{AFX_MSG_MAP(CDlgCreateRotShape)
	ON_NOTIFY(WM_LBUTTONUP, IDC_YDEGREE_SPIN, OnDegreeSpin)
	ON_EN_KILLFOCUS(IDC_YDEGREE, OnDegreeEdit)
	ON_BN_CLICKED(IDC_CLOSED, OnClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateRotShape message handlers

void CDlgCreateRotShape::b3Init()
{
	CB3App *app        = CB3GetApp();
	b3_u32  class_type = m_Shape->b3GetClassType();

	// Call base class
	CB3ProfileShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(m_Creation);
	m_xSize    = app->b3ReadProfileFloat(b3MakeSection("x size"),100);
	m_ySize    = app->b3ReadProfileFloat(b3MakeSection("y size"), 50);
	m_Closed   = app->GetProfileInt(CB3ClientString(),b3MakeSection("closed"),  m_Closed);
	m_Degree   = app->GetProfileInt(CB3ClientString(),b3MakeSection("degree"),  2);
	m_SubDiv   = app->GetProfileInt(CB3ClientString(),b3MakeSection("subdiv"),  16);
	m_Controls = app->GetProfileInt(CB3ClientString(),b3MakeSection("controls"),m_Closed ? 8 : 4);

	// Init control ranges
	m_DegreeCtrl.b3SetRange(1,B3_MAX_DEGREE);
	m_ControlsCtrl.b3SetRange(m_Degree + 1,B3_MAX_CONTROLS);
	m_SubdivCtrl.b3SetRange(4,64);
	m_xSizeCtrl.b3SetMin(epsilon);
	m_xSizeCtrl.b3SetDigits(5,2);
	m_ySizeCtrl.b3SetMin(epsilon);
	m_ySizeCtrl.b3SetDigits(5,2);
}

BOOL CDlgCreateRotShape::OnInitDialog() 
{
	CString text;

	CB3ProfileShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	text.LoadString(m_Closed ? IDS_RADIUS : IDS_LENGTH);
	m_LegendCtrl.SetWindowText(text);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCreateRotShape::OnDegreeEdit() 
{
	// Get value manually
	m_Degree   = m_DegreeCtrl.b3GetPos();

	// Adjust range and affected value
	m_Controls = m_ControlsCtrl.b3SetRange(m_Degree + 1,B3_MAX_DEGREE);
}

void CDlgCreateRotShape::OnDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnDegreeEdit();
	*pResult = 0;
}

void CDlgCreateRotShape::OnClosed() 
{
	// TODO: Add your control notification handler code here
	CString text;

	UpdateData();
	text.LoadString(m_Closed ? IDS_RADIUS : IDS_LENGTH);
	m_LegendCtrl.SetWindowText(text);
}

void CDlgCreateRotShape::b3PostProcess()
{
	CB3App           *app = CB3GetApp();
	b3SplineRotShape *shape = (b3SplineRotShape *)m_Shape;
	b3_vector        *c;
	b3_coord          x;

	if (m_Creation)
	{
		app->WriteProfileInt(CB3ClientString(),b3MakeSection("degree"),  m_Degree);
		app->WriteProfileInt(CB3ClientString(),b3MakeSection("controls"),m_Controls);
		app->WriteProfileInt(CB3ClientString(),b3MakeSection("subdiv"),  m_SubDiv);
		app->WriteProfileInt(CB3ClientString(),b3MakeSection("closed"),  m_Closed);
		app->b3WriteProfileFloat(b3MakeSection("x size"),m_xSize);
		app->b3WriteProfileFloat(b3MakeSection("y size"),m_ySize);
	}

	shape->b3Init(m_Degree,m_Controls,m_Closed,m_SubDiv);
	c = shape->m_Controls;

	if (m_Closed)
	{
		for (x = 0;x < m_Controls;x++)
		{
			c[x].x = cos(M_PI * 2 * x / m_Controls) * m_ySize + m_xSize;
			c[x].y = 0;
			c[x].z = sin(M_PI * 2 * x / m_Controls) * m_ySize;
		}
	}
	else
	{
		for (x = 0;x < m_Controls;x++)
		{
			c[x].x = m_xSize;
			c[x].y = 0;
			c[x].z = shape->m_Spline.b3ArcLengthParameter(x,m_ySize);
		}
	}

	b3Vector::b3Init(&shape->m_Axis.pos);
	b3Vector::b3Init(&shape->m_Axis.dir,0,0,m_ySize);
	CB3ProfileShapeDialog::b3PostProcess();
}
