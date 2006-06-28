/*
**
**	$Filename:	DlgBumpGroove.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Groovy surface
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
#include "DlgBumpGroove.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpGroove::CDlgBumpGroove(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpGroove::IDD, pParent)
{
	m_Bump                  = (b3BumpGroove *)item;
	m_PageBump.m_Bump       = m_Bump;
	m_PageScaling.m_Scaling = m_Bump;

	m_BumpScene = b3ExampleScene::b3CreateBump(&m_BumpHead, pDoc->b3GetParentShading());
	m_BumpHead->b3Append(m_Bump);
	//{{AFX_DATA_INIT(CDlgBumpGroove)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpGroove::~CDlgBumpGroove()
{
	m_BumpHead->b3RemoveAll();
	delete m_BumpScene;
}

void CDlgBumpGroove::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpGroove)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpGroove, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpGroove)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpGroove message handlers

void CDlgBumpGroove::b3Register()
{
	b3Loader::b3AddClassType(BUMP_GROOVE,IDS_BUMP_GROOVE,IDI_ITEM_UNKNOWN,b3Edit,b3Edit);
}

b3_bool CDlgBumpGroove::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpGroove dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpGroove::b3PreInitDialog()
{
}

void CDlgBumpGroove::b3PostInitDialog() 
{
	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpGroove::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}
