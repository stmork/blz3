/*
**
**	$Filename:	b3ProfileShapeDialog.cpp $
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

#include "b3ProfileShapeDialog.h"

/*************************************************************************
**                                                                      **
**                        CB3SpanningShapeDialog implementation         **
**                                                                      **
*************************************************************************/

CB3ProfileShapeDialog::CB3ProfileShapeDialog(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(IDD)
{
	//{{AFX_DATA_INIT(CB3ProfileShapeDialog)
	m_Align = 0;
	//}}AFX_DATA_INIT
	m_Profile = null;
	b3Vector::b3Init(&m_Base);
}


void CB3ProfileShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3ProfileShapeDialog)
	DDX_Control(pDX, IDC_BASE_X, m_xBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Y, m_yBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Z, m_zBaseCtrl);
	DDX_Radio(pDX, IDC_ALIGN_UP, m_Align);
	//}}AFX_DATA_MAP
	m_BaseGroup.b3DDX(pDX);
}

CB3ProfileShapeDialog::~CB3ProfileShapeDialog()
{
}


BEGIN_MESSAGE_MAP(CB3ProfileShapeDialog, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CB3ProfileShapeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SpanningShapeDialog message handlers

void CB3ProfileShapeDialog::b3Init()
{
	// Call base class
	CB3ShapeDialog::b3Init();

	// Read from registry
	B3_ASSERT(m_Creation);
	m_BaseGroup.b3Init(&m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_BaseGroup.b3Read(b3MakeSection("base"));
	m_Align = AfxGetApp()->GetProfileInt(CB3ClientString(),b3MakeSection("align"),m_Align);
}

void CB3ProfileShapeDialog::b3PostProcess() 
{
	CB3App    *app = CB3GetApp();
	b3_matrix  transform;

	B3_ASSERT(m_Creation);
	m_BaseGroup.b3Write(b3MakeSection("base"));
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("align"),m_Align);

	CB3ShapeDialog::b3PostProcess();

	// Compute alignment
	switch (m_Align)
	{
	case 0:
		// default
		b3Matrix::b3Unit(&transform);
		break;

	case 1:
		// Align right
		b3Matrix::b3RotateY(null,&transform,null,B3_RAD(90));
		break;

	case 2:
		// Align front
		b3Matrix::b3RotateX(null,&transform,null,B3_RAD(90));
		break;

	case 3:
		// Align left
		b3Matrix::b3RotateY(null,&transform,null,B3_RAD(-90));
		break;

	case 4:
		// Align back
		b3Matrix::b3RotateX(null,&transform,null,B3_RAD(-90));
		break;

	case 5:
		// Align down
		b3Matrix::b3Scale(null,&transform,null,0,0,-1);
		break;
	}

	// Respect base
	b3Matrix::b3Move(&transform,&transform,&m_Base);
	m_Shape->b3Transform(&transform,true);
}
