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

CDlgNebular::CDlgNebular(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgNebular::IDD)
{
	//{{AFX_DATA_INIT(CDlgNebular)
	m_ActNebular = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgNebular::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNebular)
	DDX_Control(pDX, IDC_NEB_PREVIEW, m_NebularPreviewCtrl);
	DDX_Control(pDX, IDC_NEB_DISTANCE, m_NebularDistanceCtrl);
	DDX_Control(pDX, IDC_NEB_COLOR, m_NebularColorCtrl);
	DDX_Check(pDX, IDC_ACT_NEBULAR, m_ActNebular);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNebular, CDialog)
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
	m_NebularScene = b3ExampleScene::b3CreateGlobal();
	m_EditNebular = new b3Nebular(NEBULAR);
	m_EditNebular->m_NebularColor = m_Nebular->m_NebularColor;
	m_EditNebular->m_NebularVal   = 100;
	m_EditNebular->b3Activate(m_Nebular->b3IsActive());
	m_NebularScene->b3GetSpecialHead()->b3Append(m_EditNebular);

	m_ActNebular = m_EditNebular->b3IsActive();

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_NebularDistanceCtrl.b3SetMin(epsilon);
	m_NebularDistanceCtrl.b3SetDigits(3,0);
	m_NebularDistanceCtrl.b3SetValue(fabs(m_Nebular->m_NebularVal));
	m_NebularPreviewCtrl.b3Update(m_NebularScene);
	m_NebularColorCtrl.b3SetColor(b3Color::b3GetColor(&m_EditNebular->m_NebularColor));
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNebular::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_NebularScene;
}

void CDlgNebular::OnNebularColorChange() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(b3Color::b3GetColorref(&m_EditNebular->m_NebularColor),CC_FULLOPEN,this);

	if (dlg.DoModal() == IDOK)
	{
		b3Color::b3GetColorref(&m_EditNebular->m_NebularColor,dlg.GetColor());
		m_NebularColorCtrl.b3SetColor(b3Color::b3GetColor(&m_EditNebular->m_NebularColor));
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
	BOOL result;

	if (m_NebularDistanceCtrl.b3Check())
	{
		m_Nebular->m_NebularVal   = m_NebularDistanceCtrl.m_Value;
		m_Nebular->m_NebularColor = m_EditNebular->m_NebularColor;
		m_Nebular->b3Activate(m_ActNebular);
		result = CPropertyPage::OnApply();
	}
	else
	{
		B3_BEEP;
		m_NebularDistanceCtrl.SetFocus();
		result = FALSE;
	}
	return result;
}
