/*
**
**	$Filename:	DlgMatGranite.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Granite material
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
#include "DlgMatGranite.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/03/14 08:19:17  sm
**	- Added granite material.
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatGranite implementation                 **
**                                                                      **
*************************************************************************/

CDlgMatGranite::CDlgMatGranite(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatGranite::IDD, pParent)
{
	m_Material = (b3MatGranite *)item;
	//{{AFX_DATA_INIT(CDlgMatGranite)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatGranite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatGranite)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatGranite, CDialog)
	//{{AFX_MSG_MAP(CDlgMatGranite)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatGranite message handlers

void CDlgMatGranite::b3Register()
{
	b3Loader::b3AddClassType(WOOD,IDS_MAT_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgMatGranite::b3Edit(b3Item *item)
{
	CDlgMatGranite dlg(item);

	dlg.DoModal();
	return true;
}
