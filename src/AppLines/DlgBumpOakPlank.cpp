/*
**
**	$Filename:	DlgBumpOakPlank.cpp $
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

#include "DlgBumpOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpOakPlank::CDlgBumpOakPlank(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpOakPlank::IDD, pParent)
{
	m_Bump                    = (b3BumpOakPlank *)item;
	m_PageBump.m_Bump         = m_Bump;
	m_PageOakPlank.m_OakPlank = m_Bump;
	m_PageWood.m_Wood         = m_Bump;
	m_PageScaling.m_Scaling   = m_Bump;
	//{{AFX_DATA_INIT(CDlgBumpOakPlank)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpOakPlank::~CDlgBumpOakPlank()
{
	delete m_BumpSampler;
}

void CDlgBumpOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpOakPlank)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpOakPlank, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpOakPlank)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpOakPlank message handlers

void CDlgBumpOakPlank::b3Register()
{
	b3Loader::b3AddClassType(BUMP_OAKPLANK,IDS_BUMP_OAKPLANK,IDI_MAT_OAKPLANK,b3Edit,b3Edit);
}

b3_bool CDlgBumpOakPlank::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpOakPlank dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpOakPlank::b3PreInitDialog()
{
}

void CDlgBumpOakPlank::b3PostInitDialog() 
{
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl,1);
	m_BumpSampler->b3SetBump(m_Bump);

	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageWood.b3AddToSheet(&m_PropertySheet);
	m_PageOakPlank.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpOakPlank::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpSampler);
}
