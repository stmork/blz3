/*
**
**	$Filename:	DlgMatWrapTexture.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wrap texture material
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
#include "DlgMatWrapTexture.h"
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
**                        CDlgMatWrapTexture implementation             **
**                                                                      **
*************************************************************************/

CDlgMatWrapTexture::CDlgMatWrapTexture(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatWrapTexture::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMatWrapTexture)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMatWrapTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatWrapTexture)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatWrapTexture, CDialog)
	//{{AFX_MSG_MAP(CDlgMatWrapTexture)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWrapTexture message handlers

void CDlgMatWrapTexture::b3Register()
{
	b3Loader::b3AddClassType(WRAPTEXTURE, IDS_MAT_WRAP_TEXTURE, IDI_MAT_WRAP_TEXTURE,b3Edit,b3Edit);
}

b3_bool CDlgMatWrapTexture::b3Edit(b3Item *item)
{
	return true;
}
