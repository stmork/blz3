/*
**
**	$Filename:	DlgBumpWood.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood bump map
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

#include "DlgBumpWood.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpWood::CDlgBumpWood(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpWood::IDD, pParent)
{
	m_Bump                  = (b3BumpWood *)item;
	m_PageBump.m_Bump       = m_Bump;
	m_PageWood.m_Wood       = m_Bump;
	m_PageScaling.m_Scaling = m_Bump;
	//{{AFX_DATA_INIT(CDlgBumpWood)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpWood::~CDlgBumpWood()
{
	delete m_BumpSampler;
}

void CDlgBumpWood::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpWood)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpWood, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpWood)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWood message handlers

void CDlgBumpWood::b3Register()
{
	b3Loader::b3AddClassType(BUMP_WOOD,IDS_BUMP_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgBumpWood::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpWood dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpWood::b3PreInitDialog()
{
}

void CDlgBumpWood::b3PostInitDialog() 
{
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl);
	m_BumpSampler->b3SetBump(m_Bump);

	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageWood.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpWood::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpSampler);
}
