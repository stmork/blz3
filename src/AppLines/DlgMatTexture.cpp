/*
**
**	$Filename:	DlgMatTexture.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture material
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
#include "DlgMatTexture.h"
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
**                        CDlgMatTexture implementation                 **
**                                                                      **
*************************************************************************/

CDlgMatTexture::CDlgMatTexture(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatTexture::IDD, pParent)
{
	m_Material = (b3MatTexture *)item;
	//{{AFX_DATA_INIT(CDlgMatTexture)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatTexture)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatTexture, CDialog)
	//{{AFX_MSG_MAP(CDlgMatTexture)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatTexture message handlers

void CDlgMatTexture::b3Register()
{
	b3Loader::b3AddClassType(TEXTURE,IDS_MAT_TEXTURE,IDI_MAT_TEXTURE,b3Edit,b3Edit);
}

b3_bool CDlgMatTexture::b3Edit(b3Item *item)
{
	CDlgMatTexture dlg(item);

	dlg.DoModal();
	return true;
}
