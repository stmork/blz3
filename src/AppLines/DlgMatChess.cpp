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

#include "AppLinesInclude.h"

#include "b3ExampleScene.h"
#include "DlgMatChess.h"
#include "blz3/system/b3Plugin.h"

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

void CDlgMatChess::b3PreInitDialog()
{
}

void CDlgMatChess::b3PostInitDialog()
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
