/*
**
**	$Filename:	DlgBumpWave.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wave surface
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
#include "DlgBumpWave.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        CDlgBumpWave implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpWave::CDlgBumpWave(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpWave::IDD, pParent)
{
	m_Bump                  = (b3BumpWave *)item;
	m_PageBump.m_Bump       = m_Bump;
	m_PageScaling.m_Scaling = m_Bump;

	m_BumpScene = b3ExampleScene::b3CreateBump(&m_BumpHead, pDoc->b3GetParentShading());
	m_BumpHead->b3Append(m_Bump);
	//{{AFX_DATA_INIT(CDlgBumpWave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpWave::~CDlgBumpWave()
{
	m_BumpHead->b3RemoveAll();
	delete m_BumpScene;
}

void CDlgBumpWave::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpWave)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpWave, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpWave)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWave message handlers

void CDlgBumpWave::b3Register()
{
	b3Loader::b3AddClassType(BUMP_WAVE,IDS_BUMP_WAVE,IDI_BUMP_WAVE,b3Edit,b3Edit);
}

b3_bool CDlgBumpWave::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpWave dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpWave::b3PreInitDialog()
{
}

void CDlgBumpWave::b3PostInitDialog() 
{
	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpWave::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpScene);
}
