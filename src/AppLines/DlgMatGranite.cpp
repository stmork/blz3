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
**	Revision 1.3  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**
**	Revision 1.2  2004/03/14 16:18:26  sm
**	- Added Windows support for granite.
**	
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
	b3Loader::b3AddClassType(GRANITE,IDS_MAT_GRANITE,IDI_MAT_GRANITE,b3Edit,b3Edit);
}

b3_bool CDlgMatGranite::b3Edit(b3Item *item)
{
	CDlgMatGranite dlg(item);

	return dlg.DoModal() == IDOK;
}
