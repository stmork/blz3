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

#include "AppLines.h"
#include "DlgBumpOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/04/17 17:18:33  sm
**	- Made some include adjustments
**	- Added oakplank bump as dialog
**	- Fixed b3BumpWood and b3BumpOakPlank
**	  bump computation
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpOakPlank::CDlgBumpOakPlank(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBumpOakPlank::IDD, pParent)
{
	m_Bump = (b3BumpOakPlank *)item;
	//{{AFX_DATA_INIT(CDlgBumpOakPlank)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBumpOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpOakPlank)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpOakPlank, CDialog)
	//{{AFX_MSG_MAP(CDlgBumpOakPlank)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpOakPlank message handlers

void CDlgBumpOakPlank::b3Register()
{
	b3Loader::b3AddClassType(BUMP_OAKPLANK,IDS_BUMP_OAKPLANK,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgBumpOakPlank::b3Edit(b3Item *item)
{
	CDlgBumpOakPlank dlg(item);

	dlg.DoModal();
	return true;
}

BOOL CDlgBumpOakPlank::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
