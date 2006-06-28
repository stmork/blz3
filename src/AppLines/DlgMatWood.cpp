/*
**
**	$Filename:	DlgMatWood.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood material
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

#include "DlgMatWood.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatWood implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatWood::CDlgMatWood(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatWood::IDD, pParent)
{
	m_Material = (b3MatWood *)item;
	m_PageDark.m_Material   = &m_Material->m_DarkMaterial;
	m_PageLight.m_Material  = &m_Material->m_LightMaterial;
	m_PageWood.m_Wood       =  m_Material;
	m_PageScaling.m_Scaling =  m_Material;
	//{{AFX_DATA_INIT(CDlgMatWood)
	//}}AFX_DATA_INIT
}

CDlgMatWood::~CDlgMatWood()
{
	delete m_MatSampler;
}

void CDlgMatWood::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatWood)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatWood, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatWood)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWood message handlers

void CDlgMatWood::b3Register()
{
	b3Loader::b3AddClassType(WOOD,IDS_MAT_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgMatWood::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatWood dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgMatWood::b3PreInitDialog()
{
}

void CDlgMatWood::b3PostInitDialog()
{
	m_PreviewMaterialCtrl.b3Init();
	m_MatSampler = new b3MaterialSampler(m_PreviewMaterialCtrl);
	m_MatSampler->b3SetMaterial(m_Material);

	m_PageDark.b3SetCaption(IDS_TITLE_DARK);
	m_PageLight.b3SetCaption(IDS_TITLE_LIGHT);
	m_PageDark.b3AddToSheet(&m_PropertySheet);
	m_PageLight.b3AddToSheet(&m_PropertySheet);
	m_PageWood.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgMatWood::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatSampler);
}
