/*
**
**	$Filename:	DlgMatCarPaint.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Car paint material
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
#include "blz3/system/b3Plugin.h"
#include "DlgMatCarPaint.h"
#include "b3ExampleScene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/09/27 11:08:54  sm
**	- Added rudimental car paint material dialog.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatCarPaint implementation                **
**                                                                      **
*************************************************************************/

CDlgMatCarPaint::CDlgMatCarPaint(b3Item *item, CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePreviewDialog(item, CDlgMatCarPaint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMatCarPaint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgMatCarPaint::~CDlgMatCarPaint()
{
}


void CDlgMatCarPaint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatCarPaint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatCarPaint, CDialog)
	//{{AFX_MSG_MAP(CDlgMatCarPaint)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCarPaint message handlers

void CDlgMatCarPaint::b3Register()
{
	b3Loader::b3AddClassType(CAR_PAINT,IDS_MAT_CAR_PAINT,IDI_MAT_CAR_PAINT,b3Edit,b3Edit);
}

b3_bool CDlgMatCarPaint::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatCarPaint dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatCarPaint::b3InitDialog()
{
}

void CDlgMatCarPaint::b3UpdateUI()
{
//	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}
