/*
**
**	$Filename:	DlgObjectPreview.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Preview actual object editing
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
#include "DlgObjectPreview.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/05/11 09:58:25  sm
**	- Added raytraced quick preview for bject editing.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgObjectPreview implementation              **
**                                                                      **
*************************************************************************/

CDlgObjectPreview::CDlgObjectPreview(b3BBox *bbox,b3CameraPart *camera,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjectPreview::IDD, pParent)
{
	m_PreviewScene = b3ExampleScene::b3CreateBBox(bbox,camera);
	//{{AFX_DATA_INIT(CDlgObjectPreview)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CDlgObjectPreview::~CDlgObjectPreview()
{
	delete m_PreviewScene;
}

void CDlgObjectPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjectPreview)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjectPreview, CDialog)
	//{{AFX_MSG_MAP(CDlgObjectPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectPreview message handlers

BOOL CDlgObjectPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PreviewMaterialCtrl.b3Update(m_PreviewScene);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
