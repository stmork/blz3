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
	m_xSize = 1;
	m_ySize = 1;
	m_Phong = TRUE;
	m_xClosed = FALSE;
	//}}AFX_DATA_INIT
	m_xSize   = app->GetProfileInt(CB3ClientString(),b3GetSection() + CString(".horizontal count"), m_xSize);
	m_ySize   = app->GetProfileInt(CB3ClientString(),b3GetSection() + CString(".vertical count"),   m_ySize);
	m_xClosed = app->GetProfileInt(CB3ClientString(),b3GetSection() + CString(".closed horizontal"),m_xClosed);
	m_Phong   = app->GetProfileInt(CB3ClientString(),b3GetSection() + CString(".phong"),            m_Phong);
}


void CDlgCreateTriangles::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateTriangles)
	DDX_Control(pDX, IDC_TRIA_LEGEND, m_LegendCtrl);
	DDX_Control(pDX, IDC_TRIA_HORZ, m_HorzCtrl);
	DDX_Control(pDX, IDC_TRIA_VERT, m_VertCtrl);
	DDX_Text(pDX, IDC_TRIA_XSIZE, m_xSize);
	DDX_Text(pDX, IDC_TRIA_YSIZE, m_ySize);
	DDX_Control(pDX, IDC_TRIA_YSIZE_SPIN, m_ySizeCtrl);
	DDX_Control(pDX, IDC_TRIA_XSIZE_SPIN, m_xSizeCtrl);
	DDX_Check(pDX, IDC_TRIA_PHONG, m_Phong);
	DDX_Check(pDX, IDC_HORZ_CLOSED, m_xClosed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateTriangles, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CDlgCreateTriangles)
	ON_BN_CLICKED(IDC_HORZ_CLOSED, OnHorzClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateTriangles message handlers

const char *CDlgCreateTriangles::b3GetSection()
{
	return "triangles";
}

BOOL CDlgCreateTriangles::OnInitDialog() 
{
	CB3App  *app = CB3GetApp();

	CB3ShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_HorzCtrl.b3SetDigits(5,2);
	m_HorzCtrl.b3SetValue(app->b3ReadProfileFloat(b3GetSection() + CString(".horizontal length"),100));
	m_VertCtrl.b3SetDigits(5,2);
	m_VertCtrl.b3SetValue(app->b3ReadProfileFloat(b3GetSection() + CString(".vertical length"),100));

	b3UpdateUI();
	m_ySizeCtrl.SetRange(1,64);
	
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
	b3_res        xSize,ySize;

	CB3ShapeDialog::b3PostProcess();

	app->WriteProfileInt(CB3ClientString(),b3GetSection() + CString(".horizontal count"), m_xSize);
	app->WriteProfileInt(CB3ClientString(),b3GetSection() + CString(".vertical count"),   m_ySize);
	app->WriteProfileInt(CB3ClientString(),b3GetSection() + CString(".closed horizontal"),m_xClosed);
	app->WriteProfileInt(CB3ClientString(),b3GetSection() + CString(".phong"),            m_Phong);
	app->b3WriteProfileFloat(b3GetSection() + CString(".horizontal length"),m_HorzCtrl.m_Value);
	app->b3WriteProfileFloat(b3GetSection() + CString(".vertical length"),  m_VertCtrl.m_Value);

	xSize = m_xSize;
	ySize = m_ySize;
	TriaCount = xSize * ySize * 2;
	
	if (!m_xClosed) xSize++;
	ySize++;
	VertexCount = xSize * ySize;

	shape->b3Init(VertexCount,TriaCount,m_xSize,m_xSize);

	// Compute vertices
	vertex = shape->m_Vertices;
	for (y = 0;y < ySize;y++)
	{
		for (x = 0;x < xSize;x++)
		{
			if (m_xClosed)
			{
				vertex->Point.x = cos(M_PI * 2 * x / m_xSize) * m_HorzCtrl.m_Value;
				vertex->Point.y = sin(M_PI * 2 * x / m_xSize) * m_HorzCtrl.m_Value;
				vertex->Point.z = m_VertCtrl.m_Value * y / m_ySize;
			}
			else
			{
				vertex->Point.x = m_HorzCtrl.m_Value * x / m_xSize;
				vertex->Point.y = m_VertCtrl.m_Value * y / m_ySize;
				vertex->Point.z = 0;
			}
			vertex++;
		}
	}

	// Compute triangle indizes
	tria   = shape->m_Triangles;
	for (y = 0;y < m_ySize;y++)
	{
		for (x = 0;x < m_xSize;x++)
		{
			tria[0].P1 = index +  x;
			tria[0].P2 = index + (x + 1) % xSize;
			tria[0].P3 = index +  x + xSize;

			tria[1].P1 = index + (x + 1) % xSize + xSize;
			tria[1].P2 = index +  x + xSize;
			tria[1].P3 = index + (x + 1) % xSize;
			tria += 2;
		}
		index += xSize;
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
	m_xSizeCtrl.SetRange(m_xClosed ? 4 : 1,64);
}
