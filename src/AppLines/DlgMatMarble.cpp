/*
**
**	$Filename:	DlgMatMarble.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble material
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

#include "DlgMatMarble.h"
#include "b3ExampleScene.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatMarble implementation                  **
**                                                                      **
*************************************************************************/

CDlgMatMarble::CDlgMatMarble(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatMarble::IDD, pParent)
{
	m_Material = (b3MatMarble *)item;
	m_PageDark.m_Material   = &m_Material->m_DarkMaterial;
	m_PageLight.m_Material  = &m_Material->m_LightMaterial;
	m_PageScaling.m_Scaling =  m_Material;

	m_MatScene = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatMarble)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgMatMarble::~CDlgMatMarble()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatMarble::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatMarble)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatMarble, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatMarble)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatMarble message handlers

void CDlgMatMarble::b3Register()
{
	b3Loader::b3AddClassType(MARBLE,IDS_MAT_MARBLE,IDI_MAT_MARBLE,b3Edit,b3Edit);
}

b3_bool CDlgMatMarble::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatMarble dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatMarble::b3PreInitDialog()
{
}

void CDlgMatMarble::b3PostInitDialog()
{
	m_PageDark.b3AddToSheet(&m_PropertySheet);
	m_PageLight.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
	m_PageDark.b3SetCaption(IDS_TITLE_DARK);
	m_PageLight.b3SetCaption(IDS_TITLE_LIGHT);
}

void CDlgMatMarble::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}
