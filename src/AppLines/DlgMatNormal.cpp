/*
**
**	$Filename:	DlgMatNormal.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple material
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
#include "DlgMatNormal.h"
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
**                        CDlgMatNormal implementation                  **
**                                                                      **
*************************************************************************/

CDlgMatNormal::CDlgMatNormal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatNormal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMatNormal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatNormal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatNormal)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatNormal, CDialog)
	//{{AFX_MSG_MAP(CDlgMatNormal)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatNormal message handlers

void CDlgMatNormal::b3Register()
{
	b3Loader::b3AddClassType(MAT_NORMAL,IDS_MAT_NORMAL,IDI_MAT_NORMAL,b3Edit,b3Edit);
}

b3_bool CDlgMatNormal::b3Edit(b3Item *item)
{
	return true;
}
