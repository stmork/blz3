/*
**
**	$Filename:	DlgEditTriangles.cpp $
**	$Release:	Dortmund 2007 $
**	$Revision: 1447 $
**	$Date$
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for editing triangles
**
**	(C) Copyright 2007  Steffen A. Mork
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

#include "DlgEditTriangles.h"

/*************************************************************************
**                                                                      **
**                        CDlgEditTriangles implementation              **
**                                                                      **
*************************************************************************/

CDlgEditTriangles::CDlgEditTriangles(CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(CDlgEditTriangles::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEditTriangles)
	m_Phong = TRUE;
	//}}AFX_DATA_INIT
}


void CDlgEditTriangles::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditTriangles)
	DDX_Check(pDX, IDC_TRIA_PHONG, m_Phong);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditTriangles, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CDlgEditTriangles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateTriangles message handlers

void CDlgEditTriangles::b3Init()
{
	b3Triangles *shape = (b3Triangles *)m_Shape;

	m_Phong = shape->b3GetFlag(B3_PHONG);

	// Call base class
	CB3ShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(!m_Creation);

}

BOOL CDlgEditTriangles::OnInitDialog() 
{
	CB3ShapeDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditTriangles::b3PostProcess()
{
	b3Triangles *shape = (b3Triangles *)m_Shape;

	B3_ASSERT(!m_Creation);
	CB3ShapeDialog::b3PostProcess();
	shape->b3SetFlag(B3_PHONG, m_Phong);
}
