/*
**
**	$Filename:	DlgCreateTriangles.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for creating triangles
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
#include "DlgCreateTriangles.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**
**	Revision 1.3  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**	
**	Revision 1.2  2002/03/08 16:46:14  sm
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
**	Revision 1.1  2002/03/03 21:22:21  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCreateTriangles implementation            **
**                                                                      **
*************************************************************************/

CDlgCreateTriangles::CDlgCreateTriangles(CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(CDlgCreateTriangles::IDD, pParent)
{
	CB3App *app = CB3GetApp();

	//{{AFX_DATA_INIT(CDlgCreateTriangles)
	m_xCount = 4;
	m_yCount = 4;
	m_xSize = 100;
	m_ySize = 100;
	m_Phong = TRUE;
	m_xClosed = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCreateTriangles::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateTriangles)
	DDX_Control(pDX, IDC_TRIA_LEGEND, m_LegendCtrl);
	DDX_Control(pDX, IDC_TRIA_HORZ, m_xSizeCtrl);
	DDX_Control(pDX, IDC_TRIA_VERT, m_ySizeCtrl);
	DDX_Control(pDX, IDC_TRIA_YSIZE_SPIN, m_yCountCtrl);
	DDX_Control(pDX, IDC_TRIA_XSIZE_SPIN, m_xCountCtrl);
	DDX_Check(pDX, IDC_TRIA_PHONG, m_Phong);
	DDX_Check(pDX, IDC_HORZ_CLOSED, m_xClosed);
	//}}AFX_DATA_MAP
	m_xCountCtrl.b3DDX(pDX,m_xCount);
	m_yCountCtrl.b3DDX(pDX,m_yCount);
	m_xSizeCtrl.b3DDX( pDX,m_xSize);
	m_ySizeCtrl.b3DDX( pDX,m_ySize);
}


BEGIN_MESSAGE_MAP(CDlgCreateTriangles, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CDlgCreateTriangles)
	ON_BN_CLICKED(IDC_HORZ_CLOSED, OnHorzClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateTriangles message handlers

void CDlgCreateTriangles::b3Init()
{
	CB3App  *app = CB3GetApp();

	// Call base class
	CB3ShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(m_Creation);
	m_xCount  = app->GetProfileInt(CB3ClientString(),b3MakeSection("horizontal count"), m_xCount);
	m_yCount  = app->GetProfileInt(CB3ClientString(),b3MakeSection("vertical count"),   m_yCount);
	m_xClosed = app->GetProfileInt(CB3ClientString(),b3MakeSection("closed horizontal"),m_xClosed);
	m_Phong   = app->GetProfileInt(CB3ClientString(),b3MakeSection("phong"),            m_Phong);
	m_xSize   = app->b3ReadProfileFloat(b3MakeSection("horizontal length"),m_xSize);
	m_ySize   = app->b3ReadProfileFloat(b3MakeSection("vertical length"),  m_ySize);

	// Init control ranges
	m_xSizeCtrl.b3SetDigits(5,2);
	m_ySizeCtrl.b3SetDigits(5,2);
	m_xCountCtrl.b3SetRange(1,64);
	m_yCountCtrl.b3SetRange(1,64);
}

BOOL CDlgCreateTriangles::OnInitDialog() 
{
	CB3ShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3UpdateUI();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCreateTriangles::b3PostProcess()
{
	CB3App      *app = CB3GetApp();
	b3Triangles *shape = (b3Triangles *)m_Shape;
	b3_vertex    *vertex;
	b3_triangle  *tria;
	b3_index      index = 0;
	b3_count      VertexCount,TriaCount;
	b3_coord      x,y;
	b3_res        xCount,yCount;

	CB3ShapeDialog::b3PostProcess();

	app->WriteProfileInt(CB3ClientString(),b3MakeSection("horizontal count"), m_xCount);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("vertical count"),   m_yCount);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("closed horizontal"),m_xClosed);
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("phong"),            m_Phong);
	app->b3WriteProfileFloat(b3MakeSection("horizontal length"),m_xSize);
	app->b3WriteProfileFloat(b3MakeSection("vertical length"),  m_ySize);

	xCount = m_xCount;
	yCount = m_yCount;
	TriaCount = xCount * yCount * 2;
	
	if (!m_xClosed) xCount++;
	yCount++;
	VertexCount = xCount * yCount;

	shape->b3Init(VertexCount,TriaCount,m_xCount,m_yCount);

	// Compute vertices
	vertex = shape->m_Vertices;
	for (y = 0;y < yCount;y++)
	{
		for (x = 0;x < xCount;x++)
		{
			if (m_xClosed)
			{
				vertex->Point.x = cos(M_PI * 2 * x / m_xCount) * m_xSize;
				vertex->Point.y = sin(M_PI * 2 * x / m_xCount) * m_xSize;
				vertex->Point.z = m_ySize * y / m_yCount;
			}
			else
			{
				vertex->Point.x = m_xSize * x / m_xCount;
				vertex->Point.y = m_ySize * y / m_yCount;
				vertex->Point.z = 0;
			}
			vertex++;
		}
	}

	// Compute triangle indizes
	tria   = shape->m_Triangles;
	for (y = 0;y < m_yCount;y++)
	{
		for (x = 0;x < m_xCount;x++)
		{
			tria[0].P1 = index +  x;
			tria[0].P2 = index + (x + 1) % xCount;
			tria[0].P3 = index +  x + xCount;

			tria[1].P1 = index + (x + 1) % xCount + xCount;
			tria[1].P2 = index +  x + xCount;
			tria[1].P3 = index + (x + 1) % xCount;
			tria += 2;
		}
		index += xCount;
	}
}

void CDlgCreateTriangles::OnHorzClosed() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgCreateTriangles::b3UpdateUI()
{
	CString text;

	text.LoadString(m_xClosed ? IDS_RADIUS : IDS_LENGTH);
	m_LegendCtrl.SetWindowText(text);
	m_xCountCtrl.b3SetRange(m_xClosed ? 4 : 1,64);
}
