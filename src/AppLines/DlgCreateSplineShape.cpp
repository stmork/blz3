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
#include "DlgCreateSplineShape.h"

#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2004/07/02 19:28:03  sm
**	- Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**	- Recoupled b3Scene include from CApp*Doc header files to allow
**	  faster compilation.
**	- Removed intersection counter completely because of a mysterious
**	  destruction problem of b3Mutex.
**
**	Revision 1.4  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.3  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.2  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**	
**	Revision 1.1  2002/03/09 19:48:14  sm
**	- Added a second profile for spline cylinders.
**	- BSpline shape creation dialog added.
**	- Added some features to b3SplineTemplate class:
**	  o call b3ThroughEndControl() for open splines
**	  o optimize subdivision on b3InitCurve()
**	- Fine tuing and fixed much minor bugs.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCreateSplineShape implementation          **
**                                                                      **
*************************************************************************/

CDlgCreateSplineShape::CDlgCreateSplineShape() :
	CB3ProfileShapeDialog(CDlgCreateSplineShape::IDD)
{
	//{{AFX_DATA_INIT(CDlgCreateSplineShape)
	m_ySizeLegend = _T("");
	m_xSizeLegend = _T("");
	//}}AFX_DATA_INIT
}

CDlgCreateSplineShape::~CDlgCreateSplineShape()
{
}

void CDlgCreateSplineShape::DoDataExchange(CDataExchange* pDX)
{
	CB3ProfileShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateSplineShape)
	DDX_Control(pDX, IDC_YSIZE, m_ySizeCtrl);
	DDX_Control(pDX, IDC_XSIZE, m_xSizeCtrl);
	DDX_Control(pDX, IDC_XDEGREE_SPIN, m_xDegreeCtrl);
	DDX_Control(pDX, IDC_XCONTROLS_SPIN, m_xControlsCtrl);
	DDX_Control(pDX, IDC_YDEGREE_SPIN, m_yDegreeCtrl);
	DDX_Control(pDX, IDC_YCONTROLS_SPIN, m_yControlsCtrl);
	DDX_Text(pDX, IDC_YSIZE_LEGEND, m_ySizeLegend);
	DDX_Text(pDX, IDC_XSIZE_LEGEND, m_xSizeLegend);
	//}}AFX_DATA_MAP
	m_xDegreeCtrl.b3DDX(  pDX, m_xDegree);
	m_xControlsCtrl.b3DDX(pDX, m_xControls);
	m_xSizeCtrl.b3DDX(    pDX, m_xSize);
	m_yDegreeCtrl.b3DDX(  pDX, m_yDegree);
	m_yControlsCtrl.b3DDX(pDX, m_yControls);
	m_ySizeCtrl.b3DDX(    pDX, m_ySize);
}


BEGIN_MESSAGE_MAP(CDlgCreateSplineShape, CB3ProfileShapeDialog)
	//{{AFX_MSG_MAP(CDlgCreateSplineShape)
	ON_NOTIFY(WM_LBUTTONUP, IDC_XDEGREE_SPIN, OnXDegreeSpin)
	ON_EN_KILLFOCUS(IDC_XDEGREE, OnXDegreeEdit)
	ON_NOTIFY(WM_LBUTTONUP, IDC_YDEGREE_SPIN, OnYDegreeSpin)
	ON_EN_KILLFOCUS(IDC_YDEGREE, OnYDegreeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateSplineShape message handlers

void CDlgCreateSplineShape::b3Init()
{
	CB3App *app        = CB3GetApp();
	b3_u32  class_type = m_Shape->b3GetClassType();

	// Call base class
	CB3ProfileShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(m_Creation);
	m_xDegree   = app->GetProfileInt(CB3ClientString(),b3MakeSection("x degree"),  2);
	m_xControls = app->GetProfileInt(CB3ClientString(),b3MakeSection("x controls"),class_type == SPLINES_AREA ? 4 : 8);
	m_xSize     = app->b3ReadProfileFloat(b3MakeSection("x size"),100);

	m_yDegree   = app->GetProfileInt(CB3ClientString(),b3MakeSection("y degree"),  2);
	m_yControls = app->GetProfileInt(CB3ClientString(),b3MakeSection("y controls"),class_type != SPLINES_RING ? 4 : 8);
	m_ySize     = app->b3ReadProfileFloat(b3MakeSection("y size"),50);

	// Init control ranges
	m_xSizeLegend.LoadString(class_type != SPLINES_AREA ? IDS_RADIUS : IDS_LENGTH);
	m_ySizeLegend.LoadString(class_type == SPLINES_RING ? IDS_RADIUS : IDS_LENGTH);

	m_xDegreeCtrl.b3SetRange(1,B3_MAX_DEGREE);
	m_xControlsCtrl.b3SetRange(m_xDegree + 1,B3_MAX_CONTROLS);
	m_xSizeCtrl.b3SetMin(b3Scene::epsilon);
	m_xSizeCtrl.b3SetDigits(5,2);

	m_yDegreeCtrl.b3SetRange(1,B3_MAX_DEGREE);
	m_yControlsCtrl.b3SetRange(m_yDegree + 1,B3_MAX_CONTROLS);
	m_ySizeCtrl.b3SetMin(b3Scene::epsilon);
	m_ySizeCtrl.b3SetDigits(5,2);
}

void CDlgCreateSplineShape::OnXDegreeEdit() 
{
	// Get value manually
	m_xDegree   = m_xDegreeCtrl.b3GetPos();

	// Adjust range and affected value
	m_xControls = m_xControlsCtrl.b3SetRange(m_xDegree + 1,B3_MAX_DEGREE);
}

void CDlgCreateSplineShape::OnXDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnXDegreeEdit();
	*pResult = 0;
}

void CDlgCreateSplineShape::OnYDegreeEdit() 
{
	// Get value manually
	m_yDegree   = m_yDegreeCtrl.b3GetPos();

	// Adjust range and affected value
	m_yControls = m_yControlsCtrl.b3SetRange(m_yDegree + 1,B3_MAX_DEGREE);
}

void CDlgCreateSplineShape::OnYDegreeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnYDegreeEdit();
	*pResult = 0;
}

void CDlgCreateSplineShape::b3PostProcess()
{
	CB3App        *app = CB3GetApp();
	b3SplineShape *shape =(b3SplineShape *)m_Shape;
	b3_vector     *c;
	b3_coord       x,y;
	b3_f64         rad;

	B3_ASSERT(m_Creation);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("x degree"),  m_xDegree);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("x controls"),m_xControls);
	app->b3WriteProfileFloat(b3MakeSection("x size"),m_xSize);

	app->WriteProfileInt(CB3ClientString(),b3MakeSection("y degree"),  m_yDegree);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("y controls"),m_yControls);
	app->b3WriteProfileFloat(b3MakeSection("y size"),m_ySize);

	shape->b3Init(m_xDegree,m_yDegree,m_xControls,m_yControls);
	c = shape->m_Controls;
	switch(shape->b3GetClassType())
	{
	case SPLINES_AREA:
		for (y = 0;y < m_yControls;y++)
		{
			for (x = 0;x < m_xControls;x++)
			{
				c[x].x = (shape->m_Spline[0].b3ArcLengthParameter(x) - 0.5) * m_xSize;
				c[x].y = (shape->m_Spline[1].b3ArcLengthParameter(y) - 0.5) * m_ySize;
				c[x].z = 0;
			}
			c += shape->m_Spline[1].offset;
		}
		break;

	case SPLINES_CYL:
		for (y = 0;y < m_yControls;y++)
		{
			for (x = 0;x < m_xControls;x++)
			{
				c[x].x = cos(M_PI * 2 * x / m_xControls) * m_xSize;
				c[x].y = sin(M_PI * 2 * x / m_xControls) * m_xSize;
				c[x].z = shape->m_Spline[1].b3ArcLengthParameter(y,m_ySize);
			}
			c += shape->m_Spline[1].offset;
		}
		break;

	case SPLINES_RING:
		for (y = 0;y < m_yControls;y++)
		{
			for (x = 0;x < m_xControls;x++)
			{
				rad    = m_xSize + cos(M_PI * 2 * y / m_yControls) * m_ySize;
				c[x].x = cos(M_PI * 2 * x / m_xControls) * rad;
				c[x].y = sin(M_PI * 2 * x / m_xControls) * rad;
				c[x].z = sin(M_PI * 2 * y / m_yControls) * m_ySize;
			}
			c += shape->m_Spline[1].offset;
		}
		break;

	default:
		// This shouldn't be!
		break;
	}
	CB3ProfileShapeDialog::b3PostProcess();
}
