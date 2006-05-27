/*
**
**	$Filename:	DlgProperties.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Scene properties
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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

#include "DlgProperties.h"
#include "b3CameraVolume.h"
#include "b3Fulcrum.h"
#include "b3PickVector.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.14  2006/05/27 13:32:22  sm
**	- Added CB3Dialog base class for simple dialogs.
**	- Adjusted all tool dialog base classes for better oAW MDA generation
**
**	Revision 1.13  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.12  2004/12/11 18:39:44  sm
**	- Fixed modified object problem in Lines when returning
**	  to scene editor.
**	
**	Revision 1.11  2004/12/04 12:54:07  sm
**	- Disabling VBO check box if VBO not available.
**	
**	Revision 1.10  2004/11/30 19:30:26  sm
**	- Added VBO support settings in properties dialog.
**	
**	Revision 1.9  2004/10/16 17:00:52  sm
**	- Moved lighting into own class to ensure light setup
**	  after view setup.
**	- Fixed lighting for scene and simple overview
**	- Fixed Light cutoff exponent deadloop.
**	- Corrected OpenGL define (BLZ3_USE_OPENGL)
**	
**	Revision 1.8  2003/03/04 20:37:36  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.7  2003/02/27 19:39:05  sm
**	- Added two grid colors for configuration.
**	- Beautified properties dialog.
**	
**	Revision 1.6  2003/02/26 19:13:05  sm
**	- Update scene/object views after color redefinition.
**	- Beautofied the app properties dialog.
**	
**	Revision 1.5  2003/02/26 16:36:16  sm
**	- Sorted drawing colors and added configuration support
**	  to dialog.
**	
**	Revision 1.4  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.3  2002/08/15 13:56:42  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.2  2002/02/10 20:03:18  sm
**	- Added grid raster
**	- Changed icon colors of shapes
**	
**	Revision 1.1  2002/02/08 15:53:37  sm
**	- Cleaned up makefiles for Un*x
**	- New dialog for print buffer size.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgProperties implementation                 **
**                                                                      **
*************************************************************************/

CDlgProperties::CDlgProperties(CWnd* pParent /*=NULL*/)
	: CB3Dialog(CDlgProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProperties)
	m_BBoxVisible = FALSE;
	//}}AFX_DATA_INIT
	m_App = CB3GetLinesApp();
}


void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperties)
	DDX_Control(pDX, IDC_PICK_SIZE, m_PickSizeCtrl);
	DDX_Control(pDX, IDC_COLOR_SHAPE, m_CtrlColorShape);
	DDX_Control(pDX, IDC_COLOR_SELECTED, m_CtrlColorSelected);
	DDX_Control(pDX, IDC_COLOR_PICK, m_CtrlColorPick);
	DDX_Control(pDX, IDC_COLOR_OBJECT, m_CtrlColorObject);
	DDX_Control(pDX, IDC_COLOR_GRID, m_CtrlColorGrid);
	DDX_Control(pDX, IDC_COLOR_FULCRUM, m_CtrlColorFulcrum);
	DDX_Control(pDX, IDC_COLOR_CAMERA, m_CtrlColorCamera);
	DDX_Control(pDX, IDC_COLOR_BG, m_CtrlColorBg);
	DDX_Control(pDX, IDC_COLOR_UNIT, m_CtrlColorUnit);
	DDX_Control(pDX, IDC_COLOR_MOVE, m_CtrlColorMove);
	DDX_Control(pDX, IDC_ROW_REFRESH, m_RowRefreshCtrl);
	DDX_Control(pDX, IDC_PRT_BUFFER_SPIN, m_PrtBufferCtrl);
	DDX_Check(pDX, IDC_BBOX_VISIBLE, m_BBoxVisible);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProperties, CB3Dialog)
	//{{AFX_MSG_MAP(CDlgProperties)
	ON_BN_CLICKED(IDC_CHANGE_BG, OnChangeBg)
	ON_BN_CLICKED(IDC_CHANGE_CAMERA, OnChangeCamera)
	ON_BN_CLICKED(IDC_CHANGE_FULCRUM, OnChangeFulcrum)
	ON_BN_CLICKED(IDC_CHANGE_GRID, OnChangeGrid)
	ON_BN_CLICKED(IDC_CHANGE_OBJECT, OnChangeObject)
	ON_BN_CLICKED(IDC_CHANGE_PICK, OnChangePick)
	ON_BN_CLICKED(IDC_CHANGE_SELECTED, OnChangeSelected)
	ON_BN_CLICKED(IDC_CHANGE_SHAPE, OnChangeShape)
	ON_BN_CLICKED(IDC_CHANGE_MOVE, OnChangeMove)
	ON_BN_CLICKED(IDC_CHANGE_UNIT, OnChangeUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties message handlers

void CDlgProperties::b3PreInitDialog() 
{
	m_BBoxVisible = b3BBox::m_GridVisible;
}
	
void CDlgProperties::b3PostInitDialog() 
{
	m_ColorBg       = CAppRenderDoc::m_BgColor;
	m_ColorUnit     = CAppLinesView::m_GridColorUnit;
	m_ColorMove     = CAppLinesView::m_GridColorMove;
	m_ColorCamera   = b3CameraVolume::m_GridColor;
	m_ColorFulcrum  = b3Fulcrum::m_GridColor;
	m_ColorObject   = b3BBox::m_GridColor;
	m_ColorShape    = b3RenderObject::m_GridColor;
	m_ColorSelected = b3RenderObject::m_SelectedColor;
	m_ColorGrid     = b3PickInfo::m_GridColor;
	m_ColorPick.b3SetColorref(b3PickPoint::m_PickColor);

	m_CtrlColorBg.b3Init(&m_ColorBg,this);
	m_CtrlColorUnit.b3Init(&m_ColorUnit,this);
	m_CtrlColorMove.b3Init(&m_ColorMove,this);
	m_CtrlColorCamera.b3Init(&m_ColorCamera,this);
	m_CtrlColorFulcrum.b3Init(&m_ColorFulcrum,this);
	m_CtrlColorObject.b3Init(&m_ColorObject,this);
	m_CtrlColorShape.b3Init(&m_ColorShape,this);
	m_CtrlColorSelected.b3Init(&m_ColorSelected,this);
	m_CtrlColorPick.b3Init(&m_ColorPick,this);
	m_CtrlColorGrid.b3Init(&m_ColorGrid,this);

	m_PrtBufferCtrl.b3SetRange(1,128);
	m_PrtBufferCtrl.b3SetPos(m_App->m_PrintBufferSize);

	m_RowRefreshCtrl.SetRange(1,32);
	m_RowRefreshCtrl.SetPos(m_App->m_RowRefreshCount);
	
	m_PickSizeCtrl.SetRange(1,4);
	m_PickSizeCtrl.SetPos(b3PickInfo::m_PickSize);
}

void CDlgProperties::b3UpdateUI()
{
}

void CDlgProperties::OnChangeBg() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorBg.b3Select();
}

void CDlgProperties::OnChangeCamera() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorCamera.b3Select();
}

void CDlgProperties::OnChangeFulcrum() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorFulcrum.b3Select();
}

void CDlgProperties::OnChangeGrid() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorGrid.b3Select();
}

void CDlgProperties::OnChangeObject() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorObject.b3Select();
}

void CDlgProperties::OnChangePick() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorPick.b3Select();
}

void CDlgProperties::OnChangeSelected() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorSelected.b3Select();
}

void CDlgProperties::OnChangeShape() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorShape.b3Select();
}

void CDlgProperties::OnChangeMove() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorMove.b3Select();
}

void CDlgProperties::OnChangeUnit() 
{
	// TODO: Add your control notification handler code here
	m_CtrlColorUnit.b3Select();
}

void CDlgProperties::OnOK() 
{
	// TODO: Add extra validation here
	CB3Dialog::OnOK();
	
	CAppRenderDoc::m_BgColor          = m_ColorBg;
	CAppLinesView::m_GridColorUnit    = m_ColorUnit;
	CAppLinesView::m_GridColorMove    = m_ColorMove;
	b3CameraVolume::m_GridColor       = m_ColorCamera;
	b3Fulcrum::m_GridColor            = m_ColorFulcrum;
	b3BBox::m_GridColor               = m_ColorObject;
	b3BBox::m_GridVisible             = m_BBoxVisible;
	b3RenderObject::m_GridColor       = m_ColorShape;
	b3RenderObject::m_SelectedColor   = m_ColorSelected;
	b3PickInfo::m_GridColor           = m_ColorGrid;
	b3PickInfo::m_PickSize            = m_PickSizeCtrl.GetPos();
	b3PickPoint::m_PickColor          = m_ColorPick.b3GetColorref();

	m_App->m_PrintBufferSize = m_PrtBufferCtrl.b3GetPos();
	m_App->m_RowRefreshCount = m_RowRefreshCtrl.GetPos();
	m_App->WriteProfileInt(m_App->b3ClientName(),"print buffer size",m_App->m_PrintBufferSize);
	m_App->WriteProfileInt(m_App->b3ClientName(),"row refresh count",m_App->m_RowRefreshCount);
	m_App->WriteProfileInt(m_App->b3ClientName(),"default color.picker",b3PickPoint::m_PickColor);
	m_App->WriteProfileInt(m_App->b3ClientName(),"pick size",b3PickInfo::m_PickSize);
	m_App->WriteProfileInt(m_App->b3ClientName(),"grid visible",b3BBox::m_GridVisible);
	m_App->WriteProfileInt(m_App->b3ClientName(),"allow vbo",b3VectorBufferObjects::glAllowVBO);

	m_App->b3WriteProfileColor("default color.object grid",b3BBox::m_GridColor);
	m_App->b3WriteProfileColor("default color.unit grid",CAppLinesView::m_GridColorUnit);
	m_App->b3WriteProfileColor("default color.move grid",CAppLinesView::m_GridColorMove);
	m_App->b3WriteProfileColor("default color.fulcrum",b3Fulcrum::m_GridColor);
	m_App->b3WriteProfileColor("default color.camera",b3CameraVolume::m_GridColor);
	m_App->b3WriteProfileColor("default color.background",CAppRenderDoc::m_BgColor);
	m_App->b3WriteProfileColor("default color.shape grid",b3RenderObject::m_GridColor);
	m_App->b3WriteProfileColor("default color.shape selection",b3RenderObject::m_SelectedColor);
	m_App->b3WriteProfileColor("default color.control grid",b3PickInfo::m_GridColor);
	m_App->b3UpdateAllViews();
}

void CDlgProperties::b3ReadConfig()
{
	CAppLinesApp *app = CB3GetLinesApp();

	app->m_PrintBufferSize   = app->GetProfileInt(app->b3ClientName(),"print buffer size",32);
	app->m_RowRefreshCount   = app->GetProfileInt(app->b3ClientName(),"row refresh count",B3_DISPLAY_ROWREFRESHCOUNT);
	b3PickPoint::m_PickColor = app->GetProfileInt(app->b3ClientName(),"default color.picker",b3PickPoint::m_PickColor);
	b3PickInfo::m_PickSize   = app->GetProfileInt(app->b3ClientName(),"pick size",b3PickInfo::m_PickSize);
	b3BBox::m_GridVisible    = app->GetProfileInt(app->b3ClientName(),"grid visible",b3BBox::m_GridVisible);
	b3VectorBufferObjects::glAllowVBO =
		app->GetProfileInt(app->b3ClientName(),"allow vbo", b3VectorBufferObjects::glAllowVBO);

	app->b3ReadProfileColor("default color.object grid",b3BBox::m_GridColor);
	app->b3ReadProfileColor("default color.unit grid",CAppLinesView::m_GridColorUnit);
	app->b3ReadProfileColor("default color.move grid",CAppLinesView::m_GridColorMove);
	app->b3ReadProfileColor("default color.fulcrum",b3Fulcrum::m_GridColor);
	app->b3ReadProfileColor("default color.camera",b3CameraVolume::m_GridColor);
	app->b3ReadProfileColor("default color.background",CAppRenderDoc::m_BgColor);
	app->b3ReadProfileColor("default color.shape grid",b3RenderObject::m_GridColor);
	app->b3ReadProfileColor("default color.shape selection",b3RenderObject::m_SelectedColor);
	app->b3ReadProfileColor("default color.control grid",b3PickInfo::m_GridColor);
}
