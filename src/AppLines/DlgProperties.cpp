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

#include "AppLines.h"
#include "blz3/raytrace/b3PickInfo.h"
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
	: CDialog(CDlgProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProperties)
	//}}AFX_DATA_INIT
	m_App = CB3GetLinesApp();
}


void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperties)
	DDX_Control(pDX, IDC_COLOR_SHAPE, m_CtrlColorShape);
	DDX_Control(pDX, IDC_COLOR_SELECTED, m_CtrlColorSelected);
	DDX_Control(pDX, IDC_COLOR_PICK, m_CtrlColorPick);
	DDX_Control(pDX, IDC_COLOR_OBJECT, m_CtrlColorObject);
	DDX_Control(pDX, IDC_COLOR_GRID, m_CtrlColorGrid);
	DDX_Control(pDX, IDC_COLOR_FULCRUM, m_CtrlColorFulcrum);
	DDX_Control(pDX, IDC_COLOR_CAMERA, m_CtrlColorCamera);
	DDX_Control(pDX, IDC_COLOR_BG, m_CtrlColorBg);
	DDX_Control(pDX, IDC_ROW_REFRESH, m_RowRefreshCtrl);
	DDX_Control(pDX, IDC_PRT_BUFFER_SPIN, m_PrtBufferCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProperties, CDialog)
	//{{AFX_MSG_MAP(CDlgProperties)
	ON_BN_CLICKED(IDC_CHANGE_BG, OnChangeBg)
	ON_BN_CLICKED(IDC_CHANGE_CAMERA, OnChangeCamera)
	ON_BN_CLICKED(IDC_CHANGE_FULCRUM, OnChangeFulcrum)
	ON_BN_CLICKED(IDC_CHANGE_GRID, OnChangeGrid)
	ON_BN_CLICKED(IDC_CHANGE_OBJECT, OnChangeObject)
	ON_BN_CLICKED(IDC_CHANGE_PICK, OnChangePick)
	ON_BN_CLICKED(IDC_CHANGE_SELECTED, OnChangeSelected)
	ON_BN_CLICKED(IDC_CHANGE_SHAPE, OnChangeShape)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties message handlers

BOOL CDlgProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PrtBufferCtrl.b3SetRange(1,128);
	m_PrtBufferCtrl.b3SetPos(m_App->m_PrintBufferSize);

	m_RowRefreshCtrl.SetRange(1,32);
	m_RowRefreshCtrl.SetPos(m_App->m_RowRefreshCount);
	m_ColorBg       = CAppRenderDoc::m_BgColor;
	m_ColorCamera   = b3CameraVolume::m_GridColor;
	m_ColorFulcrum  = b3Fulcrum::m_GridColor;
	m_ColorObject   = b3BBox::m_GridColor;
	m_ColorShape    = b3RenderObject::m_GridColor;
	m_ColorSelected = b3RenderObject::m_SelectedColor;
	m_ColorGrid     = b3PickInfo::m_GridColor;
	b3Color::b3GetColorref(&m_ColorPick,b3PickPoint::m_PickColor);

	m_CtrlColorBg.b3Init(&m_ColorBg,this);
	m_CtrlColorCamera.b3Init(&m_ColorCamera,this);
	m_CtrlColorFulcrum.b3Init(&m_ColorFulcrum,this);
	m_CtrlColorObject.b3Init(&m_ColorObject,this);
	m_CtrlColorShape.b3Init(&m_ColorShape,this);
	m_CtrlColorSelected.b3Init(&m_ColorSelected,this);
	m_CtrlColorPick.b3Init(&m_ColorPick,this);
	m_CtrlColorGrid.b3Init(&m_ColorGrid,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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

void CDlgProperties::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	
	CAppRenderDoc::m_BgColor        = m_ColorBg;
	b3CameraVolume::m_GridColor     = m_ColorCamera;
	b3Fulcrum::m_GridColor          = m_ColorFulcrum;
	b3BBox::m_GridColor             = m_ColorObject;
	b3RenderObject::m_GridColor     = m_ColorShape;
	b3RenderObject::m_SelectedColor = m_ColorSelected;
	b3PickInfo::m_GridColor         = m_ColorGrid;
	b3PickPoint::m_PickColor        = b3Color::b3GetColorref(&m_ColorPick);

	m_App->m_PrintBufferSize = m_PrtBufferCtrl.b3GetPos();
	m_App->m_RowRefreshCount = m_RowRefreshCtrl.GetPos();
	m_App->WriteProfileInt(m_App->b3ClientName(),"print buffer size",m_App->m_PrintBufferSize);
	m_App->WriteProfileInt(m_App->b3ClientName(),"row refresh count",m_App->m_RowRefreshCount);
	m_App->WriteProfileInt(m_App->b3ClientName(),"default color.picker",b3PickPoint::m_PickColor);
	m_App->b3WriteProfileColor("default color.object grid",&b3BBox::m_GridColor);
	m_App->b3WriteProfileColor("default color.fulcrum",&b3Fulcrum::m_GridColor);
	m_App->b3WriteProfileColor("default color.camera",&b3CameraVolume::m_GridColor);
	m_App->b3WriteProfileColor("default color.background",&CAppRenderDoc::m_BgColor);
	m_App->b3WriteProfileColor("default color.shape grid",&b3RenderObject::m_GridColor);
	m_App->b3WriteProfileColor("default color.shape selection",&b3RenderObject::m_SelectedColor);
	m_App->b3WriteProfileColor("default color.control grid",&b3PickInfo::m_GridColor);
	m_App->b3UpdateAllViews();
}

void CDlgProperties::b3ReadConfig()
{
	CAppLinesApp *app = CB3GetLinesApp();

	app->m_PrintBufferSize = app->GetProfileInt(app->b3ClientName(),"print buffer size",32);
	app->m_RowRefreshCount = app->GetProfileInt(app->b3ClientName(),"row refresh count",B3_DISPLAY_ROWREFRESHCOUNT);
	b3PickPoint::m_PickColor = app->GetProfileInt(app->b3ClientName(),"default color.picker",b3PickPoint::m_PickColor);
	app->b3ReadProfileColor("default color.object grid",&b3BBox::m_GridColor);
	app->b3ReadProfileColor("default color.fulcrum",&b3Fulcrum::m_GridColor);
	app->b3ReadProfileColor("default color.camera",&b3CameraVolume::m_GridColor);
	app->b3ReadProfileColor("default color.background",&CAppRenderDoc::m_BgColor);
	app->b3ReadProfileColor("default color.shape grid",&b3RenderObject::m_GridColor);
	app->b3ReadProfileColor("default color.shape selection",&b3RenderObject::m_SelectedColor);
	app->b3ReadProfileColor("default color.control grid",&b3PickInfo::m_GridColor);
}
