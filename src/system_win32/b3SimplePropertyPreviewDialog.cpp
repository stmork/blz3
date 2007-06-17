/*
**
**	$Filename:	b3SimplePropertyPreviewDialog.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision: 1467 $
**	$Date: 2006-10-22 11:26:47 +0200 (So, 22 Okt 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog with preview and property sheet
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

#include "b3SystemInclude.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"

/*************************************************************************
**                                                                      **
**                        CB3SimplePropertyPreviewDialog                **
**                                    implementation                    **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3SimplePropertyPreviewDialog, CB3SimplePreviewDialog)

CB3SimplePropertyPreviewDialog::CB3SimplePropertyPreviewDialog(b3Item *item,int dlgId,CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item, dlgId, pParent)
{
	//{{AFX_DATA_INIT(CB3SimplePropertyPreviewDialog)
	//}}AFX_DATA_INIT
}


void CB3SimplePropertyPreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3SimplePropertyPreviewDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3SimplePropertyPreviewDialog, CB3SimplePreviewDialog)
	//{{AFX_MSG_MAP(CB3SimplePropertyPreviewDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePropertyPreviewDialog message handlers

BOOL CB3SimplePropertyPreviewDialog::OnInitDialog() 
{
	BOOL result = FALSE;

	try
	{
		b3PreInitDialog();
		CDialog::OnInitDialog();
		b3PostInitDialog();

		CB3PropertyPage::b3InitPropertySheet(this,m_PropertySheet,m_PropertySheetId);

		OnRefresh();
	}
	catch(exception &exc)
	{
		b3PrintF(B3LOG_NORMAL, 
			"ERROR in CB3Dialog::OnInitDialog()!\n",
			"      %s\n", exc.what());
		result = FALSE;
	}
	return result;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3SimplePropertyPreviewDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CB3SimplePreviewDialog::OnOK();
	m_PropertySheet.PressButton(PSBTN_OK);
}
