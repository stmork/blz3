/*
**
**	$Filename:	DlgMatCookTorrance.cpp $
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
#include "DlgMatCookTorrance.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/03/07 12:23:26  sm
**	- Added Cook/Torrance material editors.
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatCookTorrance implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatCookTorrance::CDlgMatWood(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatCookTorrance::IDD, pParent)
{
	m_Material = (b3MatCookTorrance *)item;
	//{{AFX_DATA_INIT(CDlgMatCookTorrance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatCookTorrance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatCookTorrance)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatCookTorrance, CDialog)
	//{{AFX_MSG_MAP(CDlgMatCookTorrance)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCookTorrance message handlers

void CDlgMatCookTorrance::b3Register()
{
	b3Loader::b3AddClassType(WOOD,IDS_MAT_COOK_TORRANCE,IDI_MAT_COOK_TORRANCE,b3Edit,b3Edit);
}

b3_bool CDlgMatCookTorrance::b3Edit(b3Item *item)
{
	CDlgMatCookTorrance dlg(item);

	dlg.DoModal();
	return true;
}
