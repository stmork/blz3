/*
**
**	$Filename:	DlgMatOakPlank.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1492 $
**	$Date: 2007-06-07 11:33:07 +0200 (Do, 07 Jun 2007) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden oak plank material
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

#include "DlgMatOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgMatWood implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatOakPlank::CDlgMatOakPlank(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatOakPlank::IDD, pParent)
{
	m_Material                = (b3MatOakPlank *)item;
	m_BBox                    = pDoc->m_BBox;
	m_PageDark.m_Material     = &m_Material->m_DarkMaterial;
	m_PageLight.m_Material    = &m_Material->m_LightMaterial;
	m_PageWood.m_Wood         = m_Material;
	m_PageOakPlank.m_OakPlank = m_Material;
	m_PageScaling.m_Scaling   = m_Material;
	//{{AFX_DATA_INIT(CDlgMatOakPlank)
	//}}AFX_DATA_INIT
}

CDlgMatOakPlank::~CDlgMatOakPlank()
{
	delete m_MatSampler;
}


void CDlgMatOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatOakPlank)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatOakPlank, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatOakPlank)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatOakPlank message handlers


void CDlgMatOakPlank::b3Register()
{
	b3Loader::b3AddClassType(OAKPLANK,IDS_MAT_OAKPLANK,IDI_MAT_OAKPLANK,b3Edit,b3Edit);
}

b3_bool CDlgMatOakPlank::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatOakPlank dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatOakPlank::b3PreInitDialog()
{
}

void CDlgMatOakPlank::b3PostInitDialog() 
{
	m_PreviewMaterialCtrl.b3Init();
	m_MatSampler = new b3MaterialSampler(m_PreviewMaterialCtrl, &m_BBox->m_DimSize, 1);
	m_MatSampler->b3SetMaterial(m_Material);

	m_PageDark.b3SetCaption(IDS_TITLE_DARK);
	m_PageLight.b3SetCaption(IDS_TITLE_LIGHT);
	m_PageDark.b3AddToSheet(&m_PropertySheet);
	m_PageLight.b3AddToSheet(&m_PropertySheet);
	m_PageWood.b3AddToSheet(&m_PropertySheet);
	m_PageOakPlank.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgMatOakPlank::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatSampler);
}
