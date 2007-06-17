/*
**
**	$Filename:	DlgBumpGlossy.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Glossy surface
**
**	(C) Copyright 2004  Steffen A. Mork
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
#include "DlgBumpGlossy.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpGlossy::CDlgBumpGlossy(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item,CDlgBumpGlossy::IDD, pParent)
{
	m_Bump = (b3BumpGlossy *)item;
	m_BumpScene = b3ExampleScene::b3CreateBump(&m_BumpHead, pDoc->b3GetParentShading());
	m_BumpHead->b3Append(m_Bump);
	//{{AFX_DATA_INIT(CDlgBumpGlossy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_AmplitudeCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_BUMP);
}

CDlgBumpGlossy::~CDlgBumpGlossy()
{
	m_BumpHead->b3RemoveAll();
	delete m_BumpScene;
}

void CDlgBumpGlossy::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpGlossy)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	DDX_Control(pDX, IDC_SPIN_AMPLITUDE, m_AmplitudeCtrl);
	//}}AFX_DATA_MAP
	m_AmplitudeCtrl.b3DDX(pDX,m_Bump->m_Amplitude);
}


BEGIN_MESSAGE_MAP(CDlgBumpGlossy, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpGlossy)
	ON_EN_KILLFOCUS(IDC_EDIT_AMPLITUDE, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_AMPLITUDE, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpGlossy message handlers

void CDlgBumpGlossy::b3Register()
{
	b3Loader::b3AddClassType(BUMP_GLOSSY,IDS_BUMP_GLOSSY,IDI_BUMP_GLOSSY,b3Edit,b3Edit);
}

b3_bool CDlgBumpGlossy::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpGlossy dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpGlossy::b3PreInitDialog() 
{
}

void CDlgBumpGlossy::b3PostInitDialog() 
{
}

void CDlgBumpGlossy::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}
