/*
**
**	$Filename:	b3SimplePropertyPreviewDialog.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3SimplePropertyPreviewDialog.h"
#include "AppLines.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**
**	Revision 1.3  2004/04/25 10:34:51  sm
**	- Completed Cook/Torrance dialog
**	
**	Revision 1.2  2004/04/24 20:15:52  sm
**	- Further slide material dialog development
**	
**	Revision 1.1  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3SimplePropertyPreviewDialog                **
**                                    implementation                    **
**                                                                      **
*************************************************************************/

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
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3InitDialog();
	CB3PropertyPage::b3InitPropertySheet(this,m_PropertySheet,IDC_PROPERTY);

	b3UpdateUI();
	OnAutoRefresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
