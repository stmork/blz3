/*
**
**	$Filename:	DlgLight.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Light properties
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
#include "b3ExampleScene.h"
#include "DlgLight.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.15  2003/03/04 20:37:36  sm
**	- Introducing new b3Color which brings some
**	  performance!
**
**	Revision 1.14  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.13  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.12  2003/01/03 15:47:09  sm
**	- Changed area light optimization.
**	- Fixed some errors in the light dialog.
**	
**	Revision 1.11  2002/08/23 11:35:23  sm
**	- Added motion blur raytracing. The image creation looks very
**	  nice! The algorithm is not as efficient as it could be.
**	
**	Revision 1.10  2002/03/08 16:46:14  sm
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
**	Revision 1.9  2002/03/02 15:24:35  sm
**	- Templetized splines (uhff).
**	- Prepared spline shapes for their creation.
**	  *** And now: Testing! Testing! Testing! ***
**	
**	Revision 1.8  2001/12/21 16:46:16  sm
**	- New dialog for camera properties
**	- Done some bugfixes concerning CB3FloatEdit
**	
**	Revision 1.7  2001/12/07 16:36:12  sm
**	- Added simple LDC editing dialog.
**	
**	Revision 1.6  2001/12/06 19:45:06  sm
**	- Minor bug fixes in DlgLight
**	
**	Revision 1.5  2001/12/06 16:21:56  sm
**	- Finished CB3ControlLDC - very nice!
**	
**	Revision 1.4  2001/12/06 07:08:55  sm
**	- Further control programming
**	
**	Revision 1.3  2001/12/04 18:23:25  sm
**	- Drawing LDC correctly
**	- Added pick point support.
**	
**	Revision 1.2  2001/12/03 18:37:51  sm
**	- Added light distribution curve control.
**	
**	Revision 1.1  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgLight implementation                      **
**                                                                      **
*************************************************************************/

CDlgLight::CDlgLight(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLight)
	m_EnableLight = FALSE;
	m_EnableLDC = FALSE;
	m_EnableSoft = FALSE;
	m_SampleLabel = _T("");
	//}}AFX_DATA_INIT
	m_CtrlDiagram.b3SetMode(LDC_DIAGRAM);
	m_xPosCtrl.b3SetDigits(5,2);
	m_yPosCtrl.b3SetDigits(5,2);
	m_zPosCtrl.b3SetDigits(5,2);
	m_xDirCtrl.b3SetDigits(5,2);
	m_yDirCtrl.b3SetDigits(5,2);
	m_zDirCtrl.b3SetDigits(5,2);
	m_DistanceCtrl.b3SetDigits(5,2);
	m_DistanceCtrl.b3SetMin(b3Scene::epsilon);
	m_SoftSizeCtrl.b3SetDigits(5,3);
	m_SoftSizeCtrl.b3SetMin(b3Scene::epsilon);
}


void CDlgLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLight)
	DDX_Control(pDX, IDC_LIGHT_LDC_DIAGRAM, m_CtrlDiagram);
	DDX_Control(pDX, IDC_LIGHT_PREVIEW, m_CtrlPreview);
	DDX_Control(pDX, IDC_LIGHT_LDC_CONTROL, m_CtrlLDC);
	DDX_Control(pDX, IDC_LIGHT_LIST, m_LightListCtrl);
	DDX_Control(pDX, IDC_LIGHT_SOFT_SIZE, m_SoftSizeCtrl);
	DDX_Control(pDX, IDC_LIGHT_DISTR, m_SampleCtrl);
	DDX_Control(pDX, IDC_LIGHT_DISTANCE, m_DistanceCtrl);
	DDX_Control(pDX, IDC_LIGHT_COLOR, m_ColorCtrl);
	DDX_Control(pDX, IDC_LIGHT_POS_Z, m_zPosCtrl);
	DDX_Control(pDX, IDC_LIGHT_POS_Y, m_yPosCtrl);
	DDX_Control(pDX, IDC_LIGHT_POS_X, m_xPosCtrl);
	DDX_Control(pDX, IDC_LIGHT_DIR_Z, m_zDirCtrl);
	DDX_Control(pDX, IDC_LIGHT_DIR_Y, m_yDirCtrl);
	DDX_Control(pDX, IDC_LIGHT_DIR_X, m_xDirCtrl);
	DDX_Check(pDX, IDC_LIGHT_ENABLE, m_EnableLight);
	DDX_Check(pDX, IDC_LIGHT_LDC, m_EnableLDC);
	DDX_Check(pDX, IDC_LIGHT_SOFT, m_EnableSoft);
	DDX_Text(pDX, IDC_LIGHT_SAMPLE_LABEL, m_SampleLabel);
	//}}AFX_DATA_MAP
	m_xPosCtrl.b3DDX(pDX,    m_Light->m_Position.x);
	m_yPosCtrl.b3DDX(pDX,    m_Light->m_Position.y);
	m_zPosCtrl.b3DDX(pDX,    m_Light->m_Position.z);
	m_xDirCtrl.b3DDX(pDX,    m_Light->m_Direction.x);
	m_yDirCtrl.b3DDX(pDX,    m_Light->m_Direction.y);
	m_zDirCtrl.b3DDX(pDX,    m_Light->m_Direction.z);
	m_DistanceCtrl.b3DDX(pDX,m_Light->m_Distance);
	m_SoftSizeCtrl.b3DDX(pDX,m_Light->m_Size);
}


BEGIN_MESSAGE_MAP(CDlgLight, CDialog)
	//{{AFX_MSG_MAP(CDlgLight)
	ON_BN_CLICKED(IDC_LIGHT_COLOR_CHANGE, OnLightColorChange)
	ON_BN_CLICKED(IDC_LIGHT_NEW, OnLightNew)
	ON_BN_CLICKED(IDC_LIGHT_DELETE, OnLightDelete)
	ON_BN_CLICKED(IDC_LIGHT_ENABLE, OnLightState)
	ON_CBN_SELCHANGE(IDC_LIGHT_LIST, OnSelchangeLight)
	ON_CBN_KILLFOCUS(IDC_LIGHT_LIST, OnKillfocusLight)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LIGHT_SOFT, OnLightState)
	ON_BN_CLICKED(IDC_LIGHT_LDC, OnLightState)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_LIGHT_DISTR, OnReleasedCaptureLightDistr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLight message handlers

BOOL CDlgLight::OnInitDialog() 
{
	b3Light *light;

	m_LightScene = b3ExampleScene::b3CreateGlobal();
	light = m_LightScene->b3GetLight();
	B3_ASSERT(light != null);
	m_LightScene->m_ShadowBrightness = 0.1f;
	light->m_Position.x  =   0;
	light->m_Position.y  =   0;
	light->m_Position.z  = 100;
	light->m_Distance    = 400;
	light->m_Direction.x = -light->m_Position.x;
	light->m_Direction.y = -light->m_Position.y;
	light->m_Direction.z = -light->m_Position.z;

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SampleCtrl.SetRange(1,10);
	m_SampleCtrl.SetPageSize (  1);
	m_SampleCtrl.SetTicFreq  (  1);
	b3RefreshList();
	b3GetLight();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLight::b3UpdatePreview()
{
	b3Light  *light = m_LightScene->b3GetLight();
	b3_index  i;

	B3_ASSERT(light != null);
	light->m_Color        = m_Light->m_Color;
	light->m_LightActive  = m_Light->m_LightActive;
	light->m_SoftShadow   = m_Light->m_SoftShadow;
	light->m_SpotActive   = m_Light->m_SpotActive;
	light->m_JitterEdge   = m_Light->m_JitterEdge;
	for (i = 0;i < m_Light->m_Spline.control_num;i++)
	{
		light->m_Spline.controls[i] = m_Light->m_Spline.controls[i];
	}
	m_CtrlPreview.b3Update(m_LightScene);
}

void CDlgLight::b3UpdateUI()
{
	GetDlgItem(IDC_LIGHT_SOFT)->EnableWindow(m_EnableLight);
	GetDlgItem(IDC_LIGHT_LDC)->EnableWindow(m_EnableLight);
	GetDlgItem(IDC_LIGHT_COLOR_CHANGE)->EnableWindow(m_EnableLight);

	m_xPosCtrl.EnableWindow(m_EnableLight);
	m_yPosCtrl.EnableWindow(m_EnableLight);
	m_zPosCtrl.EnableWindow(m_EnableLight);
	
	m_xDirCtrl.EnableWindow(m_EnableLDC && m_EnableLight);
	m_yDirCtrl.EnableWindow(m_EnableLDC && m_EnableLight);
	m_zDirCtrl.EnableWindow(m_EnableLDC && m_EnableLight);

	m_DistanceCtrl.EnableWindow(m_EnableLight);
	m_SoftSizeCtrl.EnableWindow(m_EnableSoft && m_EnableLight);
	m_SampleCtrl.EnableWindow(m_EnableSoft && m_EnableLight);

	GetDlgItem(IDC_LIGHT_DELETE)->EnableWindow(m_LightBase->b3GetCount() > 1);
	b3UpdatePreview();
}

void CDlgLight::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_LightScene;
}

void CDlgLight::OnLightColorChange() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_Light->m_Color.b3GetColorref(),CC_FULLOPEN,this);

	if (dlg.DoModal() == IDOK)
	{
		m_Light->m_Color.b3SetColorref(dlg.GetColor());
		m_ColorCtrl.b3SetColor(m_Light->m_Color);
		b3UpdatePreview();
	}
}

void CDlgLight::OnLightNew() 
{
	// TODO: Add your control notification handler code here
	CDlgCreateItem  dlg;

	dlg.m_Label.LoadString(IDS_NEW_LIGHT);
	dlg.m_ItemBase   = m_LightBase;
	dlg.m_MaxNameLen = B3_BOXSTRINGLEN;
	dlg.m_Suggest    = m_Light->b3GetName();
	if (dlg.DoModal() == IDOK)
	{
		b3SetLight();

		m_Light = new b3Light(AREA_LIGHT);
		strcpy (m_Light->b3GetName(),dlg.m_NewName);
		m_LightBase->b3Append(m_Light);
		
		b3RefreshList();
		b3GetLight();
	}
}

void CDlgLight::OnLightDelete() 
{
	// TODO: Add your control notification handler code here
	b3Light *select;

	if (AfxMessageBox(IDS_ASK_DELETE_LIGHT,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		select = (b3Light *)m_Light->Prev;
		if (select == null)
		{
			select = (b3Light *)m_Light->Succ;
		}
		m_LightBase->b3Remove(m_Light);
		delete m_Light;

		m_Light = select;
		b3RefreshList();
		b3GetLight();
	}
}

void CDlgLight::OnLightState() 
{
	// TODO: Add your control notification handler code here
	b3SetLight();
	b3UpdateUI();
}

void CDlgLight::b3RefreshList()
{
	b3Item  *item;
	b3Light *light;
	int      index;

	// Fill combobox with light names
	m_LightListCtrl.ResetContent();
	B3_FOR_BASE(m_LightBase,item)
	{
		light  = (b3Light *)item;
		index  = m_LightListCtrl.AddString(light->b3GetName());
		m_LightListCtrl.SetItemDataPtr(index,light);
	}

	// Select defined light
	index = m_LightListCtrl.FindStringExact(-1,m_Light->b3GetName());
	if (index == CB_ERR)
	{
		index = 0;
	}
	m_LightListCtrl.SetCurSel(index);
}

void CDlgLight::OnSelchangeLight() 
{
	b3Light *light;
	int     index;

	index = m_LightListCtrl.GetCurSel();
	if (index != CB_ERR)
	{
		light = (b3Light *)m_LightListCtrl.GetItemDataPtr(index);
		if (light != m_Light)
		{
			b3SetLight();
			m_Light = light;
			b3GetLight();
		}
	}

}

void CDlgLight::OnKillfocusLight() 
{
	CString title;

	m_LightListCtrl.GetWindowText(title);
	strcpy(m_Light->b3GetName(),title);
	b3RefreshList();
}

void CDlgLight::b3GetLight()
{
	m_CtrlLDC.b3Init(m_Light);
	m_EnableLight = m_Light->m_LightActive;
	m_EnableSoft  = m_Light->m_SoftShadow;
	m_EnableLDC   = m_Light->m_SpotActive;
	m_SampleCtrl.SetPos(m_Light->m_JitterEdge);
	m_ColorCtrl.b3SetColor(m_Light->m_Color);
	m_SampleLabel.Format(IDS_LIGHT_SAMPLE_LABEL,m_Light->m_JitterEdge * m_Light->m_JitterEdge);

	m_CtrlLDC.b3Init(m_Light);
	m_CtrlLDC.b3Update();
	m_CtrlDiagram.b3Init(m_Light);
	m_CtrlDiagram.b3Update();
	b3UpdateUI();
	b3UpdatePreview();
	UpdateData(FALSE);
}

void CDlgLight::b3SetLight()
{
	UpdateData(TRUE);
	m_Light->m_LightActive = m_EnableLight;
	m_Light->m_SoftShadow  = m_EnableSoft;
	m_Light->m_SpotActive  = m_EnableLDC;
	m_Light->m_JitterEdge  = m_SampleCtrl.GetPos();
}

BOOL CDlgLight::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int pos;

	pos = m_SampleCtrl.GetPos();
	if (m_Light->m_JitterEdge != pos)
	{
		b3SetLight();
		m_SampleLabel.Format(IDS_LIGHT_SAMPLE_LABEL,pos * pos);
		UpdateData(FALSE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgLight::OnOK() 
{
	// TODO: Add extra validation here
	b3SetLight();
	CDialog::OnOK();
}

BOOL CDlgLight::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	NMHDR *pHdr = (NMHDR *)lParam;
	switch(wParam)
	{
	case IDC_LIGHT_LDC_CONTROL:
		switch(pHdr->code)
		{
		case WM_MOUSEMOVE:
			m_CtrlDiagram.b3Update();
			*pResult = 0;
			return TRUE;

		case WM_LBUTTONUP:
			b3UpdatePreview();
			*pResult = 0;
			return TRUE;
		}
		break;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgLight::OnReleasedCaptureLightDistr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	b3UpdatePreview();
	*pResult = 0;
}
