/*
**
**	$Filename:	DlgMatMarble.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Marble material
**
**	(C) Copyright 2003  Steffen A. Mork
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
#include "DlgMatMarble.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**
**	Revision 1.2  2003/07/12 10:20:16  sm
**	- Fixed ticketno. 12 (memory leak in b3ItemRegistry)
**	
**	Revision 1.1  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatMarble implementation                  **
**                                                                      **
*************************************************************************/

CDlgMatMarble::CDlgMatMarble(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatMarble::IDD, pParent)
{
	m_Material = (b3MatMarble *)item;
	//{{AFX_DATA_INIT(CDlgMatMarble)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatMarble::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatMarble)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatMarble, CDialog)
	//{{AFX_MSG_MAP(CDlgMatMarble)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatMarble message handlers

void CDlgMatMarble::b3Register()
{
	b3Loader::b3AddClassType(MARBLE,IDS_MAT_MARBLE,IDI_MAT_MARBLE,b3Edit,b3Edit);
}

b3_bool CDlgMatMarble::b3Edit(b3Item *item)
{
	CDlgMatMarble dlg(item);

	return dlg.DoModal() == IDOK;
}
