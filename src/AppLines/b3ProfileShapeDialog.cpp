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
**	Revision 1.8  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**
**	Revision 1.7  2002/08/04 13:24:55  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**	
**	Revision 1.6  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**	
**	Revision 1.5  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**	Revision 1.4  2002/03/09 19:48:14  sm
**	- Added a second profile for spline cylinders.
**	- BSpline shape creation dialog added.
**	- Added some features to b3SplineTemplate class:
**	  o call b3ThroughEndControl() for open splines
**	  o optimize subdivision on b3InitCurve()
**	- Fine tuing and fixed much minor bugs.
**	
**	Revision 1.3  2002/03/08 16:46:15  sm
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
	m_Shape->b3Transform(&transform,true);
}
