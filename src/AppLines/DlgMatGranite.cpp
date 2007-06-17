/*
**
**	$Filename:	DlgMatGranite.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Granite material
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
#include "DlgMatGranite.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatGranite implementation                 **
**                                                                      **
*************************************************************************/

CDlgMatGranite::CDlgMatGranite(b3Item *item,CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatGranite::IDD, pParent)
{
	m_Material              = (b3MatGranite *)item;
	m_PageDark.m_Material   = &m_Material->m_DarkMaterial;
	m_PageLight.m_Material  = &m_Material->m_LightMaterial;
	m_PageScaling.m_Scaling =  m_Material;

	m_MatScene              = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatGranite)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgMatGranite::~CDlgMatGranite()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatGranite::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatGranite)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatGranite, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatGranite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatGranite message handlers

void CDlgMatGranite::b3Register()
{
	b3Loader::b3AddClassType(GRANITE,IDS_MAT_GRANITE,IDI_MAT_GRANITE,b3Edit,b3Edit);
}

b3_bool CDlgMatGranite::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatGranite dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatGranite::b3PreInitDialog()
{
}

void CDlgMatGranite::b3PostInitDialog()
{
	m_PageDark.b3AddToSheet(&m_PropertySheet);
	m_PageLight.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
	m_PageDark.b3SetCaption(IDS_TITLE_DARK);
	m_PageLight.b3SetCaption(IDS_TITLE_LIGHT);
}

void CDlgMatGranite::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}
