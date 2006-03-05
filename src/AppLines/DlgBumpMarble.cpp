/*
**
**	$Filename:	DlgBumpMarble.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble bump mapping
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
#include "DlgBumpMarble.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/03/05 22:12:31  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.3  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.2  2004/05/29 13:38:10  sm
**	- Made shading model visible to material an bump dialogs.
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

CDlgBumpMarble::CDlgBumpMarble(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpMarble::IDD, pParent)
{
	m_Bump                  = (b3BumpMarble *)item;
	m_PageBump.m_Bump       = m_Bump;
	m_PageScaling.m_Scaling = m_Bump;

	m_BumpScene = b3ExampleScene::b3CreateBump(&m_BumpHead, pDoc->b3GetParentShading());
	m_BumpHead->b3Append(m_Bump);
	//{{AFX_DATA_INIT(CDlgBumpMarble)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpMarble::~CDlgBumpMarble()
{
	m_BumpHead->b3RemoveAll();
	delete m_BumpScene;
}

void CDlgBumpMarble::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpMarble)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpMarble, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpMarble)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpMarble message handlers

void CDlgBumpMarble::b3Register()
{
	b3Loader::b3AddClassType(BUMP_MARBLE,IDS_BUMP_MARBLE,IDI_BUMP_MARBLE,b3Edit,b3Edit);
}

b3_bool CDlgBumpMarble::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpMarble dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpMarble::b3InitDialog() 
{
	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpMarble::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}
