/*
**
**	$Filename:	DlgNebular.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for b3Nebular properties
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
#include "DlgNebular.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2006/02/19 18:58:47  sm
**	- Some dialog inheritance corrections.
**
**	Revision 1.7  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.6  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.5  2002/03/08 16:46:14  sm
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
**	Revision 1.4  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.3  2001/11/18 13:49:26  sm
**	- Introduced new CB3FloatEdit derived from CEdit
**	- DlgNebular implemented
**	- DlgLensFlare implemented
**	- Adjusting far clipping plane inside b3RenderView
**	
**	Revision 1.2  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.1  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgNebular implementation                    **
**                                                                      **
*************************************************************************/

CDlgNebular::CDlgNebular(b3_u32 class_type,CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgNebular::IDD)
{
	m_ShadingClassType = class_type;
	//{{AFX_DATA_INIT(CDlgNebular)
	m_ActNebular = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgNebular::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNebular)
	DDX_Control(pDX, IDC_NEB_PREVIEW, m_NebularPreviewCtrl);
	DDX_Control(pDX, IDC_NEB_DISTANCE, m_NebularDistanceCtrl);
	DDX_Control(pDX, IDC_NEB_COLOR, m_NebularColorCtrl);
	DDX_Check(pDX, IDC_ACT_NEBULAR, m_ActNebular);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNebular, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgNebular)
	ON_BN_CLICKED(ID_NEB_CHANGE, OnNebularColorChange)
	ON_BN_CLICKED(IDC_ACT_NEBULAR, OnActNebular)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNebular message handlers

BOOL CDlgNebular::OnInitDialog() 
{
	m_NebularScene = b3ExampleScene::b3CreateGlobal(m_ShadingClassType);
	m_EditNebular = new b3Nebular(NEBULAR);
	m_EditNebular->m_NebularColor = m_Nebular->m_NebularColor;
	m_EditNebular->m_NebularVal   = 100;
	m_EditNebular->b3Activate(m_Nebular->b3IsActive());
	m_NebularScene->b3GetSpecialHead()->b3Append(m_EditNebular);

	m_ActNebular = m_EditNebular->b3IsActive();

	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_NebularDistanceCtrl.b3SetMin(b3Scene::epsilon);
	m_NebularDistanceCtrl.b3SetDigits(3,0);
	m_NebularDistanceCtrl.b3SetPos(fabs(m_Nebular->m_NebularVal));
	m_NebularPreviewCtrl.b3Update(m_NebularScene);
	m_NebularColorCtrl.b3Init(&m_EditNebular->m_NebularColor,this);
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNebular::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_NebularScene;
}

void CDlgNebular::OnNebularColorChange() 
{
	// TODO: Add your control notification handler code here
	if (m_NebularColorCtrl.b3Select())
	{
		m_NebularPreviewCtrl.b3Update(m_NebularScene);
	}
}

void CDlgNebular::OnActNebular() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_EditNebular->b3Activate(m_ActNebular);
	m_NebularPreviewCtrl.b3Update(m_NebularScene);
	b3UpdateUI();
}

void CDlgNebular::b3UpdateUI()
{
	CWnd *distWnd        = GetDlgItem(IDC_NEB_DISTANCE);
	CWnd *colorSelectWnd = GetDlgItem(ID_NEB_CHANGE);

	distWnd->EnableWindow(m_ActNebular);
	colorSelectWnd->EnableWindow(m_ActNebular);
}

BOOL CDlgNebular::OnApply()
{
	// TODO: Add extra validation here
	m_Nebular->m_NebularVal   = m_NebularDistanceCtrl.b3GetPos();
	m_Nebular->m_NebularColor = m_EditNebular->m_NebularColor;
	m_Nebular->b3Activate(m_ActNebular);
	return CPropertyPage::OnApply();
}
