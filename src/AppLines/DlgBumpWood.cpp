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

#include "AppLines.h"
#include "DlgBumpWood.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/04/17 17:18:33  sm
**	- Made some include adjustments
**	- Added oakplank bump as dialog
**	- Fixed b3BumpWood and b3BumpOakPlank
**	  bump computation
**
**	Revision 1.1  2004/04/11 18:56:08  sm
**	- Forgotten these files.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpWood::CDlgBumpWood(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBumpWood::IDD, pParent)
{
	m_Bump = (b3BumpWood *)item;
	//{{AFX_DATA_INIT(CDlgBumpWood)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBumpWood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpWood)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpWood, CDialog)
	//{{AFX_MSG_MAP(CDlgBumpWood)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpWood message handlers

void CDlgBumpWood::b3Register()
{
	b3Loader::b3AddClassType(BUMP_WOOD,IDS_BUMP_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgBumpWood::b3Edit(b3Item *item)
{
	CDlgBumpWood dlg(item);

	dlg.DoModal();
	return true;
}

BOOL CDlgBumpWood::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
