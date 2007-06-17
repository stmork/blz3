/*
**
**	$Filename:	DlgBumpOceanWave.cpp $
**	$Release:	Dortmund 2006 $
**	$Revision: 1492 $
**	$Date: 2007-06-07 11:33:07 +0200 (Do, 07 Jun 2007) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Ocean wave bump map
**
**	(C) Copyright 2006  Steffen A. Mork
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

#include "DlgBumpOcean.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpOcean::CDlgBumpOcean(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpOcean::IDD, pParent)
{
	m_Bump                  = (b3BumpOcean *)item;
	m_BBox                  = pDoc->m_BBox;
	m_PageBump.m_Bump       = m_Bump;
	m_PageOcean.m_Ocean     = m_Bump;
	m_PageScaling.m_Scaling = m_Bump;
	//{{AFX_DATA_INIT(CDlgBumpOceanWave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpOcean::~CDlgBumpOcean()
{
	delete m_BumpSampler;
}

void CDlgBumpOcean::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpOcean)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpOcean, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpOcean)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpOakPlank message handlers

void CDlgBumpOcean::b3Register()
{
	b3Loader::b3AddClassType(BUMP_OCEAN, IDS_BUMP_OCEAN, IDI_BUMP_OCEAN, b3Edit, b3Edit);
}

b3_bool CDlgBumpOcean::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpOcean dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpOcean::b3PreInitDialog()
{
}

void CDlgBumpOcean::b3PostInitDialog() 
{
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl, &m_BBox->m_DimSize, 1);
	m_BumpSampler->b3SetBump(m_Bump);

	m_PageOcean.b3AddToSheet(&m_PropertySheet);
	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpOcean::b3UpdateUI()
{
	m_Bump->b3Modified();
	m_PreviewBumpCtrl.b3Update(m_BumpSampler);
}
