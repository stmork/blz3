/*
**
**	$Filename:	DlgMatChess.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Chess material
**
**	(C) Copyright 2003  Steffen A. Mork
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
#include "AppObjectDoc.h"
#include "b3ExampleScene.h"
#include "DlgMatChess.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.10  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**
**	Revision 1.9  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.8  2004/05/12 14:13:27  sm
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
**	Revision 1.7  2004/05/10 17:05:37  sm
**	- Added texture material dialog. Now we have completed all
**	  material dialogs. I Continue completing bump mapping dialogs.
**	
**	Revision 1.6  2004/05/10 15:12:08  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.5  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**	
**	Revision 1.4  2004/04/26 12:27:43  sm
**	- Added following dialogs:
**	  o granite
**	  o chess
**	- Added scaling to wood properties
**	
**	Revision 1.3  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	Revision 1.2  2003/07/12 10:20:16  sm
**	- Fixed ticketno. 12 (memory leak in b3ItemRegistry)
**	
**	Revision 1.1  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatChess implementation                   **
**                                                                      **
*************************************************************************/

CDlgMatChess::CDlgMatChess(b3Item *item,CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatChess::IDD, pParent)
{
	m_Material             = (b3MatChess *)item;
	m_PageBlack.m_Material = &m_Material->m_Material[b3MatChess::BLACK];
	m_PageWhite.m_Material = &m_Material->m_Material[b3MatChess::WHITE];
	m_MatScene             = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatChess)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_xTimesCtrl.b3SetRange(1,100);
	m_yTimesCtrl.b3SetRange(1,100);
}

CDlgMatChess::~CDlgMatChess()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatChess::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatChess)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	DDX_Control(pDX, IDC_SPIN_XTIMES, m_xTimesCtrl);
	DDX_Control(pDX, IDC_SPIN_YTIMES, m_yTimesCtrl);
	//}}AFX_DATA_MAP
	m_xTimesCtrl.b3DDX(pDX,m_Material->m_xTimes);
	m_yTimesCtrl.b3DDX(pDX,m_Material->m_yTimes);
}


BEGIN_MESSAGE_MAP(CDlgMatChess, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatChess)
	ON_EN_KILLFOCUS(IDC_SPIN_XTIMES, OnEdit)
	ON_EN_KILLFOCUS(IDC_SPIN_YTIMES, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_XTIMES, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_SPIN_YTIMES, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatChess message handlers

void CDlgMatChess::b3Register()
{
	b3Loader::b3AddClassType(CHESS,IDS_MAT_CHESS,IDI_MAT_CHESS,b3Edit,b3Edit);
}

b3_bool CDlgMatChess::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatChess dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatChess::b3InitDialog()
{
	m_PageBlack.b3AddToSheet(&m_PropertySheet);
	m_PageWhite.b3AddToSheet(&m_PropertySheet);
	m_PageBlack.b3SetCaption(IDS_TITLE_BLACK);
	m_PageWhite.b3SetCaption(IDS_TITLE_WHITE);
}

void CDlgMatChess::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}
