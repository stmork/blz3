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

#include "AppLines.h"
#include "b3ExampleScene.h"
#include "DlgBumpGlossy.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/05/12 19:10:50  sm
**	- Completed bump mapping dialog.
**
**	Revision 1.1  2004/05/12 14:13:27  sm
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
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpGlossy::CDlgBumpGlossy(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item,CDlgBumpGlossy::IDD, pParent)
{
	m_Bump = (b3BumpGlossy *)item;
	m_BumpScene = b3ExampleScene::b3CreateBump(&m_BumpHead);
	m_BumpHead->b3Append(m_Bump);
	//{{AFX_DATA_INIT(CDlgBumpGlossy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_AmplitudeCtrl.b3SetUnit(CB3FloatSpinButtonCtrl::B3_UNIT_PERMILLE);
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
	CDlgBumpGlossy dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpGlossy::b3InitDialog() 
{
}

void CDlgBumpGlossy::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}