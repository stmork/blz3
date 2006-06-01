/*
**
**	$Filename:	DlgCamera.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Camera properties
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "DlgCamera.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.14  2006/06/01 14:17:24  smork
**	- Added frame work controls for simpler plugin generation:
**	  o CB3Checkbox
**	  o CB3TextEdit
**
**	Revision 1.13  2006/03/05 22:12:31  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.12  2005/01/23 20:57:22  sm
**	- Moved some global static variables into class static ones.
**	
**	Revision 1.11  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.10  2004/05/16 09:21:10  sm
**	- Fixed ticket no. 22: Camera deletions are handled
**	  correctly now
**	
**	Revision 1.9  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.8  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.7  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.6  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
**	Revision 1.5  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.4  2002/03/08 16:46:14  sm
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
**	Revision 1.3  2002/02/13 16:13:08  sm
**	- Created spotlight view
**	- Changed camera properties dialog to reflect scene units
**	  on example camera settings.
**	
**	Revision 1.2  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.1  2001/12/21 16:46:16  sm
**	- New dialog for camera properties
**	- Done some bugfixes concerning CB3FloatEdit
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCamera implementation                     **
**                                                                      **
*************************************************************************/

CDlgCamera::CDlgCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCamera::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCamera)
	m_EnableCamera = FALSE;
	m_UnitString = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCamera)
	DDX_Control(pDX, IDC_CAMERA_TYPES, m_TypeCtrl);
	DDX_Control(pDX, IDC_CAMERA_LIST, m_CameraListCtrl);
	DDX_Control(pDX, IDC_CAMERA_FOCAL_LENGTH, m_FocalLengthCtrl);
	DDX_Control(pDX, IDC_CAMERA_WIDTH, m_WidthCtrl);
	DDX_Control(pDX, IDC_CAMERA_HEIGHT, m_HeightCtrl);
	DDX_Control(pDX, IDC_CAMERA_VIEW_Z, m_zViewCtrl);
	DDX_Control(pDX, IDC_CAMERA_VIEW_Y, m_yViewCtrl);
	DDX_Control(pDX, IDC_CAMERA_VIEW_X, m_xViewCtrl);
	DDX_Control(pDX, IDC_CAMERA_POS_Z, m_zPosCtrl);
	DDX_Control(pDX, IDC_CAMERA_POS_Y, m_yPosCtrl);
	DDX_Control(pDX, IDC_CAMERA_POS_X, m_xPosCtrl);
	DDX_Check(pDX, IDC_CAMERA_ACTIVATION, m_EnableCamera);
	DDX_Text(pDX, IDC_CAMERA_UNIT, m_UnitString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCamera, CDialog)
	//{{AFX_MSG_MAP(CDlgCamera)
	ON_BN_CLICKED(IDC_CAMERA_NEW, OnCameraNew)
	ON_BN_CLICKED(IDC_CAMERA_DELETE, OnCameraDelete)
	ON_CBN_SELCHANGE(IDC_CAMERA_LIST, OnSelchangeCamera)
	ON_CBN_KILLFOCUS(IDC_CAMERA_LIST, OnKillfocusCamera)
	ON_BN_CLICKED(IDC_CAMERA_ACTIVATION, OnCameraState)
	ON_EN_KILLFOCUS(IDC_CAMERA_FOCAL_LENGTH, OnKillfocusCameraType)
	ON_EN_KILLFOCUS(IDC_CAMERA_WIDTH, OnKillfocusCameraType)
	ON_EN_KILLFOCUS(IDC_CAMERA_HEIGHT, OnKillfocusCameraType)
	ON_CBN_SELCHANGE(IDC_CAMERA_TYPES, OnSelchangeCameraTypes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

b3_predefined_camera CDlgCamera::m_CameraDefinition[] =
{
	{  10.0, 34, 22.5 },
	{  35.0, 34, 22.5 },
	{  50.0, 34, 22.5 },
	{  70.0, 34, 22.5 },
	{ 200.0, 34, 22.5 }
};

#define MAX_CAMERA_TYPES (sizeof(m_CameraDefinition) / sizeof(b3_predefined_camera))

/////////////////////////////////////////////////////////////////////////////
// CDlgCamera message handlers

BOOL CDlgCamera::OnInitDialog() 
{
	b3ModellerInfo *info;

	// Get unit in mm
	B3_ASSERT(m_Scene != null);
	info         = m_Scene->b3GetModellerInfo();
	m_UnitFactor = info->b3ScaleUnitToMM();
	m_UnitString.Format(IDS_CAMERA_UNIT,info->b3GetUnitDescr());

	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_xPosCtrl.b3SetDigits(5,2);
	m_yPosCtrl.b3SetDigits(5,2);
	m_zPosCtrl.b3SetDigits(5,2);
	m_xViewCtrl.b3SetDigits(5,2);
	m_yViewCtrl.b3SetDigits(5,2);
	m_zViewCtrl.b3SetDigits(5,2);
	m_FocalLengthCtrl.b3SetDigits(5,2);
	m_FocalLengthCtrl.b3SetMin(b3Scene::epsilon);
	m_WidthCtrl.b3SetDigits(5,2);
	m_WidthCtrl.b3SetMin(b3Scene::epsilon);
	m_HeightCtrl.b3SetDigits(5,2);
	m_HeightCtrl.b3SetMin(b3Scene::epsilon);
	
	b3RefreshList();
	b3GetCamera();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCamera::OnCameraNew() 
{
	// TODO: Add your control notification handler code here
	CDlgCreateItem  dlg;
	b3CameraPart   *camera;

	dlg.m_Label.LoadString(IDS_NEW_CAMERA);
	dlg.m_ItemBase   = m_Scene->b3GetSpecialHead();
	dlg.m_ClassType  = CAMERA;
	dlg.m_MaxNameLen = B3_CAMERANAMELEN;
	dlg.m_Suggest    = m_Camera->b3GetName();
	if (dlg.DoModal() == IDOK)
	{
		b3SetCamera();

		camera = new b3CameraPart(CAMERA);
		camera->m_EyePoint  = m_Camera->m_EyePoint;
		camera->m_ViewPoint = m_Camera->m_ViewPoint;
		camera->m_Width     = m_Camera->m_Width;
		camera->m_Height    = m_Camera->m_Height;
		camera->m_Flags     = CAMERA_ACTIVE;
		camera->b3SetName(dlg.b3GetText());
		m_Scene->b3GetSpecialHead()->b3Append(m_Camera = camera);
		
		b3RefreshList();
		b3GetCamera();
	}
}

void CDlgCamera::OnCameraDelete() 
{
	// TODO: Add your control notification handler code here
	b3CameraPart *select;

	if (m_CameraListCtrl.GetCount() > 1)
	{
		if (AfxMessageBox(IDS_ASK_DELETE_CAMERA,MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
			int index = m_CameraListCtrl.GetCurSel();
			
			if (index != CB_ERR)
			{
				if (index > 0)
				{
					index--;
				}
				else
				{
					index++;
				}
				select = (b3CameraPart *)m_CameraListCtrl.GetItemDataPtr(index);
			}
			m_Scene->b3GetSpecialHead()->b3Remove(m_Camera);
			delete m_Camera;

			m_Camera = select;
			b3RefreshList();
			b3GetCamera();
		}
	}
}

void CDlgCamera::b3RefreshList()
{
	b3CameraPart *camera;
	int           index;

	// Fill combobox with light names
	m_CameraListCtrl.ResetContent();
	for (camera  = m_Scene->b3GetFirstCamera();
	     camera != null;
		 camera  = m_Scene->b3GetNextCamera(camera))
	{
		index  = m_CameraListCtrl.AddString(camera->b3GetName());
		m_CameraListCtrl.SetItemDataPtr(index,camera);
	}

	// Select defined light
	index = m_CameraListCtrl.FindStringExact(-1,m_Camera->b3GetName());
	if (index == CB_ERR)
	{
		index = 0;
	}
	m_CameraListCtrl.SetCurSel(index);
	OnSelchangeCamera();
}

void CDlgCamera::OnSelchangeCamera() 
{
	b3CameraPart *camera;
	int           index;

	index = m_CameraListCtrl.GetCurSel();
	B3_ASSERT (index != CB_ERR);
	camera = (b3CameraPart *)m_CameraListCtrl.GetItemDataPtr(index);
	if (camera != m_Camera)
	{
		b3SetCamera();
		m_Camera = camera;
		m_Scene->b3SetCamera(m_Camera);
		b3GetCamera();
	}
}

void CDlgCamera::OnKillfocusCamera() 
{
	b3CameraPart *found;
	CString       title;

	m_CameraListCtrl.GetWindowText(title);
	found = m_Scene->b3GetCameraByName(title);
	if ((found != null) && (found != m_Camera))
	{
		B3_BEEP;
		m_CameraListCtrl.SetFocus();
	}
	else
	{
		m_Camera->b3SetName(title);
		b3RefreshList();
	}
}

void CDlgCamera::OnCameraState() 
{
	// TODO: Add your control notification handler code here
	b3SetCamera();
	b3UpdateUI();
}

void CDlgCamera::b3UpdateUI()
{
	b3CameraPart *camera;
	b3_count      count = 0;

	m_xPosCtrl.EnableWindow(m_EnableCamera);
	m_yPosCtrl.EnableWindow(m_EnableCamera);
	m_zPosCtrl.EnableWindow(m_EnableCamera);
	m_xViewCtrl.EnableWindow(m_EnableCamera);
	m_yViewCtrl.EnableWindow(m_EnableCamera);
	m_zViewCtrl.EnableWindow(m_EnableCamera);
	m_FocalLengthCtrl.EnableWindow(m_EnableCamera);
	m_WidthCtrl.EnableWindow(m_EnableCamera);
	m_HeightCtrl.EnableWindow(m_EnableCamera);
	m_TypeCtrl.EnableWindow(m_EnableCamera);

	for (camera  = m_Scene->b3GetFirstCamera();
		 camera != null;
	     camera  = m_Scene->b3GetNextCamera(camera))
	{
		count++;
	}
	GetDlgItem(IDC_CAMERA_DELETE)->EnableWindow(count > 1);
}

int CDlgCamera::b3GetCameraType()
{
	int i;

	for (i = 0;i < MAX_CAMERA_TYPES;i++)
	{
		if ((fabs(m_CameraDefinition[i].m_FocalLength / m_UnitFactor - m_FocalLengthCtrl.m_Value) < b3Scene::epsilon) &&
		    (fabs(m_CameraDefinition[i].m_Width       / m_UnitFactor - m_WidthCtrl.m_Value)       < b3Scene::epsilon) &&
		    (fabs(m_CameraDefinition[i].m_Height      / m_UnitFactor - m_HeightCtrl.m_Value)      < b3Scene::epsilon))
		{
			return i;
		}
	}
	return i;
}

void CDlgCamera::b3GetCamera()
{
	m_EnableCamera = m_Camera->b3IsActive();
	m_xPosCtrl.b3SetPos(m_Camera->m_EyePoint.x);
	m_yPosCtrl.b3SetPos(m_Camera->m_EyePoint.y);
	m_zPosCtrl.b3SetPos(m_Camera->m_EyePoint.z);
	m_xViewCtrl.b3SetPos(m_Camera->m_ViewPoint.x);
	m_yViewCtrl.b3SetPos(m_Camera->m_ViewPoint.y);
	m_zViewCtrl.b3SetPos(m_Camera->m_ViewPoint.z);
	m_FocalLengthCtrl.b3SetPos(b3Vector::b3Distance(&m_Camera->m_ViewPoint,&m_Camera->m_EyePoint));
	m_WidthCtrl.b3SetPos(b3Vector::b3Length(&m_Camera->m_Width)   * 2);
	m_HeightCtrl.b3SetPos(b3Vector::b3Length(&m_Camera->m_Height) * 2);
	m_TypeCtrl.SetCurSel(b3GetCameraType());

	b3UpdateUI();
	UpdateData(FALSE);
}

void CDlgCamera::b3SetCamera()
{
	b3_vector eye,view;

	UpdateData(TRUE);
	m_Camera->b3Activate(m_EnableCamera);
	eye.x  = m_xPosCtrl.m_Value;
	eye.y  = m_yPosCtrl.m_Value;
	eye.z  = m_zPosCtrl.m_Value;
	view.x = m_xViewCtrl.m_Value;
	view.y = m_yViewCtrl.m_Value;
	view.z = m_zViewCtrl.m_Value;
	m_Camera->b3Orientate(
		&eye,
		&view,
		m_FocalLengthCtrl.m_Value,
		m_WidthCtrl.m_Value  * 0.5,
		m_HeightCtrl.m_Value * 0.5);

}

void CDlgCamera::OnOK() 
{
	// TODO: Add extra validation here
	b3SetCamera();
	CDialog::OnOK();
}

void CDlgCamera::OnKillfocusCameraType() 
{
	// TODO: Add your control notification handler code here
	m_TypeCtrl.SetCurSel(b3GetCameraType());
}

void CDlgCamera::OnSelchangeCameraTypes() 
{
	// TODO: Add your control notification handler code here
	int index;

	index = m_TypeCtrl.GetCurSel();
	if ((index != CB_ERR) && (index < MAX_CAMERA_TYPES))
	{
		m_FocalLengthCtrl.b3SetPos(m_CameraDefinition[index].m_FocalLength / m_UnitFactor);
		m_WidthCtrl.b3SetPos(      m_CameraDefinition[index].m_Width       / m_UnitFactor);
		m_HeightCtrl.b3SetPos(     m_CameraDefinition[index].m_Height      / m_UnitFactor);
	}
}
