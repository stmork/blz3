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

#include "AppLines.h"
#include "DlgCamera.h"
#include "blz3/base/b3Matrix.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCamera, CDialog)
	//{{AFX_MSG_MAP(CDlgCamera)
	ON_BN_CLICKED(IDC_CAMERA_NEW, OnCameraNew)
	ON_BN_CLICKED(IDC_CAMERA_DELETE, OnCameraDelete)
	ON_CBN_SELCHANGE(IDC_CAMERA_LIST, OnSelchangeCamera)
	ON_CBN_KILLFOCUS(IDC_CAMERA_LIST, OnKillfocusCamera)
	ON_BN_CLICKED(IDC_CAMERA_ACTIVATION, OnCameraState)
	ON_EN_KILLFOCUS(IDC_CAMERA_FOCAL_LENGTH, OnKillfocusCameraFocalLength)
	ON_EN_KILLFOCUS(IDC_CAMERA_WIDTH, OnKillfocusCameraWidth)
	ON_EN_KILLFOCUS(IDC_CAMERA_HEIGHT, OnKillfocusCameraHeight)
	ON_CBN_SELCHANGE(IDC_CAMERA_TYPES, OnSelchangeCameraTypes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static struct b3_predefined_camera
{
	b3_f64 m_FocalLength;
	b3_f64 m_Width;
	b3_f64 m_Height;
} camera_definition[] =
{
	{  10, 33, 22 },
	{  35, 33, 22 },
	{  50, 33, 22 },
	{  70, 33, 22 },
	{ 200, 33, 22 }
};

#define MAX_CAMERA_TYPES (sizeof(camera_definition) / sizeof(b3_predefined_camera))

/////////////////////////////////////////////////////////////////////////////
// CDlgCamera message handlers

BOOL CDlgCamera::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_xPosCtrl.b3SetDigits(5,2);
	m_yPosCtrl.b3SetDigits(5,2);
	m_zPosCtrl.b3SetDigits(5,2);
	m_xViewCtrl.b3SetDigits(5,2);
	m_yViewCtrl.b3SetDigits(5,2);
	m_zViewCtrl.b3SetDigits(5,2);
	m_FocalLengthCtrl.b3SetDigits(5,2);
	m_FocalLengthCtrl.b3SetMin(epsilon);
	m_WidthCtrl.b3SetDigits(5,2);
	m_WidthCtrl.b3SetMin(epsilon);
	m_HeightCtrl.b3SetDigits(5,2);
	m_HeightCtrl.b3SetMin(epsilon);
	
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
		strcpy (camera->b3GetName(),dlg.m_NewName);
		m_Scene->b3GetSpecialHead()->b3Append(m_Camera = camera);
		
		b3RefreshList();
		b3GetCamera();
	}
}

void CDlgCamera::OnCameraDelete() 
{
	// TODO: Add your control notification handler code here
	b3CameraPart *select;

	if (AfxMessageBox(IDS_ASK_DELETE_CAMERA,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		select = (b3CameraPart *)m_Camera->Prev;
		if (select == null)
		{
			select = (b3CameraPart *)m_Camera->Succ;
		}
		m_Scene->b3GetSpecialHead()->b3Remove(m_Camera);
		delete m_Camera;

		m_Camera = select;
		b3RefreshList();
		b3GetCamera();
	}
}

void CDlgCamera::b3RefreshList()
{
	b3CameraPart *camera;
	int           index;

	// Fill combobox with light names
	m_CameraListCtrl.ResetContent();
	for (camera = m_Scene->b3GetCamera();camera != null;camera = m_Scene->b3GetNextCamera(camera))
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
}

void CDlgCamera::OnSelchangeCamera() 
{
	b3CameraPart *camera;
	int           index;

	index = m_CameraListCtrl.GetCurSel();
	if (index != CB_ERR)
	{
		camera = (b3CameraPart *)m_CameraListCtrl.GetItemDataPtr(index);
		if (camera != m_Camera)
		{
			b3SetCamera();
			m_Camera = camera;
			b3GetCamera();
		}
	}
}

void CDlgCamera::OnKillfocusCamera() 
{
	CString title;

	m_CameraListCtrl.GetWindowText(title);
	strcpy(m_Camera->b3GetName(),title);
	b3RefreshList();
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

	for (camera = m_Scene->b3GetCamera();camera != null;camera = m_Scene->b3GetNextCamera(camera))
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
		if ((fabs(camera_definition[i].m_FocalLength - m_FocalLengthCtrl.m_Value) < epsilon) &&
		    (fabs(camera_definition[i].m_Width       - m_WidthCtrl.m_Value)       < epsilon) &&
		    (fabs(camera_definition[i].m_Height      - m_HeightCtrl.m_Value)      < epsilon))
		{
			return i;
		}
	}
	return i;
}

void CDlgCamera::b3GetCamera()
{
	m_EnableCamera = m_Camera->b3IsActive();
	m_xPosCtrl.b3SetValue(m_Camera->m_EyePoint.x);
	m_yPosCtrl.b3SetValue(m_Camera->m_EyePoint.y);
	m_zPosCtrl.b3SetValue(m_Camera->m_EyePoint.z);
	m_xViewCtrl.b3SetValue(m_Camera->m_ViewPoint.x);
	m_yViewCtrl.b3SetValue(m_Camera->m_ViewPoint.y);
	m_zViewCtrl.b3SetValue(m_Camera->m_ViewPoint.z);
	m_FocalLengthCtrl.b3SetValue(b3Vector::b3Distance(&m_Camera->m_ViewPoint,&m_Camera->m_EyePoint));
	m_WidthCtrl.b3SetValue(b3Vector::b3Length(&m_Camera->m_Width));
	m_HeightCtrl.b3SetValue(b3Vector::b3Length(&m_Camera->m_Height));
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
	m_Camera->b3Orientate(&eye,&view,m_FocalLengthCtrl.m_Value,m_WidthCtrl.m_Value,m_HeightCtrl.m_Value);

}

void CDlgCamera::OnOK() 
{
	// TODO: Add extra validation here
	b3SetCamera();
	CDialog::OnOK();
}

void CDlgCamera::OnKillfocusCameraFocalLength() 
{
	// TODO: Add your control notification handler code here
	m_TypeCtrl.SetCurSel(b3GetCameraType());
}

void CDlgCamera::OnKillfocusCameraWidth() 
{
	// TODO: Add your control notification handler code here
	m_TypeCtrl.SetCurSel(b3GetCameraType());
}

void CDlgCamera::OnKillfocusCameraHeight() 
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
		m_FocalLengthCtrl.b3SetValue(camera_definition[index].m_FocalLength);
		m_WidthCtrl.b3SetValue(camera_definition[index].m_Width);
		m_HeightCtrl.b3SetValue(camera_definition[index].m_Height);
	}
}
