/*
**
**	$Filename:	DlgMatSlide.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Slide material
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
#include "DlgMatSlide.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**                        CDlgMatSlide implementation                   **
**                                                                      **
*************************************************************************/

CDlgMatSlide::CDlgMatSlide(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatSlide::IDD, pParent)
{
	m_Material = (b3MatSlide *)item;
	//{{AFX_DATA_INIT(CDlgMatSlide)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatSlide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatSlide)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatSlide, CDialog)
	//{{AFX_MSG_MAP(CDlgMatSlide)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatSlide message handlers

void CDlgMatSlide::b3Register()
{
	b3Loader::b3AddClassType(SLIDE,IDS_MAT_SLIDE,IDI_MAT_SLIDE,b3Edit,b3Edit);
}

b3_bool CDlgMatSlide::b3Edit(b3Item *item)
{
	CDlgMatSlide dlg(item);

	dlg.DoModal();
	return true;
}
