/*
**
**	$Filename:	DlgMatCarPaint.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Car paint material
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

#include "blz3/system/b3Plugin.h"
#include "DlgMatCarPaint.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatCarPaint implementation                **
**                                                                      **
*************************************************************************/

CDlgMatCarPaint::CDlgMatCarPaint(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatCarPaint::IDD, pParent)
{
	m_Material = (b3MatCarPaint *)item;
	m_MatScene = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	m_PageParallel.m_Material      = &m_Material->m_Parallel;
	m_PagePerpendicular.m_Material = &m_Material->m_Perpendicular;
	//{{AFX_DATA_INIT(CDlgMatCarPaint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Metallic = (m_Material->m_Flags & B3_MAT_CP_METALLIC) != 0;
	m_MetallicCtrl.b3SetUnit( CB3FloatSpinButtonCtrl::B3_UNIT_PERMILLE);
}

CDlgMatCarPaint::~CDlgMatCarPaint()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}


void CDlgMatCarPaint::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatCarPaint)
	DDX_Control(pDX, IDC_SPIN_METALLIC, m_MetallicCtrl);
	DDX_Check(pDX, IDC_METALLIC, m_Metallic);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
	m_MetallicCtrl.b3DDX(pDX,m_Material->m_MetallicScale);
}


BEGIN_MESSAGE_MAP(CDlgMatCarPaint, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatCarPaint)
	ON_BN_CLICKED(IDC_METALLIC, OnMetallicMode)
	ON_EN_KILLFOCUS(IDC_EDIT_METALLIC, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_EDIT_METALLIC, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCarPaint message handlers

void CDlgMatCarPaint::b3Register()
{
	b3Loader::b3AddClassType(CAR_PAINT,IDS_MAT_CAR_PAINT,IDI_MAT_CAR_PAINT,b3Edit,b3Edit);
}

b3_bool CDlgMatCarPaint::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatCarPaint dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatCarPaint::b3PreInitDialog()
{
}

void CDlgMatCarPaint::b3PostInitDialog()
{
	m_PagePerpendicular.b3SetCaption(IDS_TITLE_PERPENDICULAR);
	m_PageParallel.b3SetCaption(IDS_TITLE_PARALLEL);

	m_PagePerpendicular.b3AddToSheet(&m_PropertySheet);
	m_PageParallel.b3AddToSheet(&m_PropertySheet);
}

void CDlgMatCarPaint::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatCarPaint::OnMetallicMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_Material->m_Flags = (m_Metallic ? B3_MAT_CP_METALLIC : 0);
	b3Preview();
}
