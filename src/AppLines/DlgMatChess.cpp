/*
**
**	$Filename:	DlgMatChess.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Chess material
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
#include "DlgMatChess.h"
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
**                        CDlgMatChess implementation                   **
**                                                                      **
*************************************************************************/

CDlgMatChess::CDlgMatChess(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatChess::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMatChess)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatChess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatChess)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatChess, CDialog)
	//{{AFX_MSG_MAP(CDlgMatChess)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatChess message handlers

void CDlgMatChess::b3Register()
{
	b3Loader::b3AddClassType(CHESS,IDS_MAT_CHESS,IDI_MAT_CHESS,b3Edit,b3Edit);
}

b3_bool CDlgMatChess::b3Edit(b3Item *item)
{
	return true;
}
