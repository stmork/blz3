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

#include "b3ProfileShapeDialog.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**
**	Revision 1.1  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**
*/

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
}


void CB3ProfileShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3ProfileShapeDialog)
	DDX_Control(pDX, IDC_BASE_Z, m_xBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Y, m_yBaseCtrl);
	DDX_Control(pDX, IDC_BASE_X, m_zBaseCtrl);
	DDX_Radio(pDX, IDC_ALIGN_UP, m_Align);
	//}}AFX_DATA_MAP
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

const char *CB3ProfileShapeDialog::b3GetSection()
{
	return "profile shape";
}

void CB3ProfileShapeDialog::b3Init()
{
	m_BaseGroup.b3Init(&m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_BaseGroup.b3Read(b3MakeSection("base"));
	m_Align = AfxGetApp()->GetProfileInt(CB3ClientString(),b3MakeSection("align"),m_Align);
}

BOOL CB3ProfileShapeDialog::OnInitDialog() 
{
	CB3ShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BaseGroup.b3Set(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3ProfileShapeDialog::b3PostProcess() 
{
	CB3App    *app = CB3GetApp();
	b3_matrix  transform;

	m_BaseGroup.b3Write(b3MakeSection("base"));
	app->WriteProfileInt(CB3ClientString(),b3MakeSection("align"),m_Align);

	CB3ShapeDialog::b3PostProcess();

	// Compute alignment
	switch (m_Align)
	{
	case 0:
		// default
		b3MatrixUnit(&transform);
		break;

	case 1:
		// Align right
		b3MatrixRotY(null,&transform,null,B3_RAD(90));
		break;

	case 2:
		// Align front
		b3MatrixRotX(null,&transform,null,B3_RAD(90));
		break;

	case 3:
		// Align left
		b3MatrixRotY(null,&transform,null,B3_RAD(-90));
		break;

	case 4:
		// Align back
		b3MatrixRotX(null,&transform,null,B3_RAD(-90));
		break;

	case 5:
		// Align down
		b3MatrixScale(null,&transform,null,0,0,-1);
		break;
	}

	// Respect base
	b3MatrixMove(&transform,&transform,&m_Base);
	m_Shape->b3Transform(&transform);
}
