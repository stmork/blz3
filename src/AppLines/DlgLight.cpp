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
#include "DlgLight.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
}


void CDlgLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLight)
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
}


BEGIN_MESSAGE_MAP(CDlgLight, CDialog)
	//{{AFX_MSG_MAP(CDlgLight)
	ON_BN_CLICKED(IDC_LIGHT_COLOR_CHANGE, OnLightColorChange)
	ON_BN_CLICKED(IDC_LIGHT_NEW, OnLightNew)
	ON_BN_CLICKED(IDC_LIGHT_DELETE, OnLightDelete)
	ON_BN_CLICKED(IDC_LIGHT_ENABLE, OnLightState)
	ON_BN_CLICKED(IDC_LIGHT_SOFT, OnLightState)
	ON_BN_CLICKED(IDC_LIGHT_LDC, OnLightState)
	ON_CBN_SELCHANGE(IDC_LIGHT_LIST, OnSelchangeLight)
	ON_CBN_KILLFOCUS(IDC_LIGHT_LIST, OnKillfocusLight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLight message handlers

BOOL CDlgLight::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SoftSizeCtrl.b3SetMin(epsilon);
	m_SampleCtrl.SetRange(1,10);
	m_SampleCtrl.SetPageSize (  1);
	m_SampleCtrl.SetTicFreq  (  1);
	m_xPosCtrl.b3SetDigits(5,2);
	m_yPosCtrl.b3SetDigits(5,2);
	m_zPosCtrl.b3SetDigits(5,2);
	m_xDirCtrl.b3SetDigits(5,2);
	m_yDirCtrl.b3SetDigits(5,2);
	m_zDirCtrl.b3SetDigits(5,2);
	m_DistanceCtrl.b3SetDigits(5,2);
	m_DistanceCtrl.b3SetMax(epsilon);
	m_SoftSizeCtrl.b3SetDigits(5,2);
	m_SoftSizeCtrl.b3SetMin(epsilon);
	b3RefreshList();
	b3GetLight();
	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLight::OnLightColorChange() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(b3Color::b3GetColorref(&m_Light->m_Color),CC_FULLOPEN,this);

	if (dlg.DoModal() == IDOK)
	{
		b3Color::b3GetColorref(&m_Light->m_Color,dlg.GetColor());
		m_ColorCtrl.b3SetColor(b3Color::b3GetColor(&m_Light->m_Color));
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
		b3UpdateUI();
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
		b3UpdateUI();
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

	GetDlgItem(IDC_LIGHT_DELETE)->EnableWindow(m_LightBase->b3Count() > 1);
}

void CDlgLight::b3GetLight()
{
	m_EnableLight = m_Light->m_LightActive;
	m_EnableSoft  = m_Light->m_SoftShadow;
	m_EnableLDC   = m_Light->m_SpotActive;
	m_xPosCtrl.b3SetValue(m_Light->m_Position.x);
	m_yPosCtrl.b3SetValue(m_Light->m_Position.y);
	m_zPosCtrl.b3SetValue(m_Light->m_Position.z);
	m_xDirCtrl.b3SetValue(m_Light->m_Direction.x);
	m_yDirCtrl.b3SetValue(m_Light->m_Direction.y);
	m_zDirCtrl.b3SetValue(m_Light->m_Direction.z);
	m_DistanceCtrl.b3SetValue(m_Light->m_Distance);
	m_SampleCtrl.SetPos(m_Light->m_JitterEdge);
	m_SoftSizeCtrl.b3SetValue(m_Light->m_Size);
	m_ColorCtrl.b3SetColor(b3Color::b3GetColor(&m_Light->m_Color));
	m_SampleLabel.Format(IDS_LIGHT_SAMPLE_LABEL,m_Light->m_JitterEdge * m_Light->m_JitterEdge);

	m_CtrlLDC.m_LDC = &m_Light->m_Spline;
	m_CtrlLDC.b3Update();
	UpdateData(FALSE);
}

void CDlgLight::b3SetLight()
{
	UpdateData(TRUE);
	m_Light->m_LightActive = m_EnableLight;
	m_Light->m_SoftShadow  = m_EnableSoft;
	m_Light->m_SpotActive  = m_EnableLDC;
	m_Light->m_Position.x  = m_xPosCtrl.m_Value;
	m_Light->m_Position.y  = m_yPosCtrl.m_Value;
	m_Light->m_Position.z  = m_zPosCtrl.m_Value;
	m_Light->m_Direction.x = m_xDirCtrl.m_Value;
	m_Light->m_Direction.y = m_yDirCtrl.m_Value;
	m_Light->m_Direction.z = m_zDirCtrl.m_Value;
	m_Light->m_Distance    = m_DistanceCtrl.m_Value;
	m_Light->m_JitterEdge  = m_SampleCtrl.GetPos();
	m_Light->m_Size        = m_SoftSizeCtrl.m_Value;
}

BOOL CDlgLight::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int pos;

	pos = m_SampleCtrl.GetPos();
	if (m_Light->m_JitterEdge != pos)
	{
		m_Light->m_JitterEdge = pos;
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
