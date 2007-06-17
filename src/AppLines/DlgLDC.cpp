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

#include "AppLinesInclude.h"

#include "b3ExampleScene.h"
#include "DlgLDC.h"

/*************************************************************************
**                                                                      **
**                        CDlgLDC implementation                        **
**                                                                      **
*************************************************************************/

CDlgLDC::CDlgLDC(b3_u32 class_type,CWnd* pParent /*=NULL*/)
	: CB3Dialog(CDlgLDC::IDD, pParent)
{
	m_ShadingClassType = class_type;
	//{{AFX_DATA_INIT(CDlgLDC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CtrlDiagram.b3SetMode(LDC_DIAGRAM);
	m_Light = null;
}


void CDlgLDC::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLDC)
	DDX_Control(pDX, IDC_LIGHT_LDC_DIAGRAM, m_CtrlDiagram);
	DDX_Control(pDX, IDC_LIGHT_PREVIEW, m_CtrlPreview);
	DDX_Control(pDX, IDC_LIGHT_LDC_CONTROL, m_CtrlLDC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLDC, CB3Dialog)
	//{{AFX_MSG_MAP(CDlgLDC)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLDC message handlers

void CDlgLDC::b3PreInitDialog() 
{
	b3Light *light;

	m_LightScene = b3ExampleScene::b3CreateGlobal(m_ShadingClassType);
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
}

void CDlgLDC::b3PostInitDialog() 
{
	m_CtrlLDC.b3Init(m_Light);
	m_CtrlLDC.b3Update();
	m_CtrlDiagram.b3Init(m_Light);
	m_CtrlDiagram.b3Update();
}

void CDlgLDC::b3UpdateUI()
{
	b3Light  *light = m_LightScene->b3GetLight();
	b3_index  i;

	B3_ASSERT(light != null);
	for (i = 0;i < m_Light->m_Spline.m_ControlNum;i++)
	{
		light->m_Spline.m_Controls[i] = m_Light->m_Spline.m_Controls[i];
	}
	m_CtrlPreview.b3Update(m_LightScene);
}

void CDlgLDC::OnDestroy() 
{
	CB3Dialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_LightScene;
}

BOOL CDlgLDC::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
	case IDC_LIGHT_LDC_CONTROL:
		NMHDR *pHdr = (NMHDR *)lParam;
		switch(pHdr->code)
		{
		case WM_MOUSEMOVE:
			m_CtrlDiagram.b3Update();
			*pResult = 0;
			return TRUE;

		case WM_LBUTTONUP:
			b3UpdateUI();
			*pResult = 0;
			return TRUE;
		}
		break;
	}
	return CB3Dialog::OnNotify(wParam, lParam, pResult);
}
