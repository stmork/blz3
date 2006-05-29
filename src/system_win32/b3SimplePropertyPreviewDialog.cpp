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

#include "b3SystemInclude.h"
#include "blz3/system/b3SimplePropertyPreviewDialog.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/05/29 15:13:45  smork
**	- Resource IDs corrected.
**
**	Revision 1.2  2006/05/27 13:32:22  sm
**	- Added CB3Dialog base class for simple dialogs.
**	- Adjusted all tool dialog base classes for better oAW MDA generation
**	
**	Revision 1.1  2006/03/19 14:47:18  sm
**	- Fixed missing initiailization problems in b3BBox.
**	- Moved some dialog elements into system library.
**	
**	Revision 1.6  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.5  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
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

		b3UpdateUI();
		OnAutoRefresh();
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
