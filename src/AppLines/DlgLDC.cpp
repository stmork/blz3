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
#include "DlgLDC.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/12/07 16:36:12  sm
**	- Added simple LDC editing dialog.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgLDC implementation                        **
**                                                                      **
*************************************************************************/

CDlgLDC::CDlgLDC(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLDC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLDC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CtrlDiagram.b3SetMode(LDC_DIAGRAM);
	m_Light = null;
}


void CDlgLDC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLDC)
	DDX_Control(pDX, IDC_LIGHT_LDC_DIAGRAM, m_CtrlDiagram);
	DDX_Control(pDX, IDC_LIGHT_PREVIEW, m_CtrlPreview);
	DDX_Control(pDX, IDC_LIGHT_LDC_CONTROL, m_CtrlLDC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLDC, CDialog)
	//{{AFX_MSG_MAP(CDlgLDC)
	ON_MESSAGE(WM_B3_LDC_MOVED, b3UpdateDiagram)
	ON_MESSAGE(WM_B3_LDC_CHANGED, b3UpdatePreview)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLDC message handlers

BOOL CDlgLDC::OnInitDialog() 
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
	light->m_Color       = m_Light->m_Color;
	light->m_LightActive = m_Light->m_LightActive;
	light->m_SoftShadow  = m_Light->m_SoftShadow;
	light->m_SpotActive  = m_Light->m_SpotActive;

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_CtrlLDC.b3Init(m_Light);
	m_CtrlLDC.b3Update();
	m_CtrlDiagram.b3Init(m_Light);
	m_CtrlDiagram.b3Update();
	b3UpdatePreview();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgLDC::b3UpdateDiagram(WPARAM wParam,LPARAM lParam)
{
	m_CtrlDiagram.b3Update();
	return 0;
}

LRESULT CDlgLDC::b3UpdatePreview(WPARAM wParam,LPARAM lParam)
{
	b3Light  *light = m_LightScene->b3GetLight();
	b3_index  i;

	B3_ASSERT(light != null);
	for (i = 0;i < m_Light->m_Spline.control_num;i++)
	{
		light->m_Spline.controls[i] = m_Light->m_Spline.controls[i];
	}
	m_CtrlPreview.b3Update(m_LightScene);
	return 0;
}

void CDlgLDC::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_LightScene;
}
