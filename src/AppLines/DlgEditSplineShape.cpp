/*
**
**	$Filename:	DlgCreateSplineShape.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "DlgEditSplineShape.h"

/*************************************************************************
**                                                                      **
**                        CDlgCreateSplineShape implementation          **
**                                                                      **
*************************************************************************/

CDlgEditSplineShape::CDlgEditSplineShape(CWnd* pParent /*=NULL*/) :
	CB3ShapeDialog(CDlgEditSplineShape::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEditSplineShape)
	//}}AFX_DATA_INIT
}

CDlgEditSplineShape::~CDlgEditSplineShape()
{
}

void CDlgEditSplineShape::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditSplineShape)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditSplineShape, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CDlgEditSplineShape)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateSplineShape message handlers

void CDlgEditSplineShape::b3Init()
{
	b3SplineShape *shape =(b3SplineShape *)m_Shape;

	// Call base class
	CB3ShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(!m_Creation);
}

void CDlgEditSplineShape::b3PostProcess()
{
	b3SplineShape *shape =(b3SplineShape *)m_Shape;

	B3_ASSERT(!m_Creation);
	CB3ShapeDialog::b3PostProcess();
}
