/*
**
**	$Filename:	b3Dialog.cpp $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple dialog
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

#include "b3SystemInclude.h"
#include "blz3/system/b3Dialog.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/05/29 15:13:45  smork
**	- Resource IDs corrected.
**
**	Revision 1.1  2006/05/27 13:32:22  sm
**	- Added CB3Dialog base class for simple dialogs.
**	- Adjusted all tool dialog base classes for better oAW MDA generation
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3Dialog implementation                      **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3Dialog, CDialog)

CB3Dialog::CB3Dialog(int dlgId,CWnd* pParent /*=NULL*/)
	: CDialog(dlgId, pParent)
{
}

CB3Dialog::~CB3Dialog(void)
{
}

void CB3Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3Dialog)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CB3Dialog, CDialog)
	//{{AFX_MSG_MAP(CB3Dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SimplePreviewDialog message handlers

BOOL CB3Dialog::OnInitDialog() 
{
	BOOL result = FALSE;

	try
	{
		b3PreInitDialog();
		result = CDialog::OnInitDialog();
		b3PostInitDialog();
		b3UpdateUI();
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

void CB3Dialog::OnEdit()
{
	UpdateData();
}

void CB3Dialog::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}
