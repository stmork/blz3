/*
**
**	$Filename:	DlgMatWood.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood material
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
#include "DlgMatWood.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatWood implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatWood::CDlgMatWood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatWood::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMatWood)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatWood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatWood)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatWood, CDialog)
	//{{AFX_MSG_MAP(CDlgMatWood)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWood message handlers

void CDlgMatWood::b3Register()
{
	b3Loader::b3AddClassType(WOOD,IDS_MAT_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgMatWood::b3Edit(b3Item *item)
{
	return true;
}
