/*
**
**	$Filename:	DlgMatOakPlank.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wooden oak plank material
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
#include "DlgMatOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**
**	Revision 1.6  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**	Revision 1.5  2004/04/23 13:17:17  sm
**	- Added simple material page and renamed wood material page.
**	- Reflect material member renaming.
**	
**	Revision 1.4  2004/04/22 20:23:56  sm
**	- Fixed wrong ON_MESSAGE signature on call function.
**	- Reordered context menu of object editor.
**	- Expand root item of tree view in object editor
**	
**	Revision 1.3  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**	
**	Revision 1.2  2004/04/10 15:59:51  sm
**	- Added control units as base class for
**	  o CB3FloatSliderCtrl
**	  o CB3FloatSpinButtonCtrl
**	
**	Revision 1.1  2004/04/10 14:33:25  sm
**	- Added oak plank support.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatWood implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatOakPlank::CDlgMatOakPlank(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(CDlgMatOakPlank::IDD, pParent)
{
	m_Material                = (b3MatOakPlank *)item;
	m_PageMatWood.m_Material  = m_Material;
	m_PageWood.m_Wood         = m_Material;
	m_PageOakPlank.m_OakPlank = m_Material;
	//{{AFX_DATA_INIT(CDlgMatOakPlank)
	//}}AFX_DATA_INIT
}

CDlgMatOakPlank::~CDlgMatOakPlank()
{
	delete m_MatSampler;
}


void CDlgMatOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatOakPlank)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatOakPlank, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatOakPlank)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatOakPlank message handlers


void CDlgMatOakPlank::b3Register()
{
	b3Loader::b3AddClassType(OAKPLANK,IDS_MAT_OAKPLANK,IDI_MAT_OAKPLANK,b3Edit,b3Edit);
}

b3_bool CDlgMatOakPlank::b3Edit(b3Item *item)
{
	CDlgMatOakPlank dlg(item);

	dlg.DoModal();
	return true;
}

void CDlgMatOakPlank::b3InitDialog() 
{
	m_PreviewMaterialCtrl.b3Init();
	m_MatSampler = new b3MaterialSampler(m_PreviewMaterialCtrl);
	m_MatSampler->b3SetMaterial(m_Material);

	m_PropertySheet.AddPage(&m_PageMatWood);
	m_PropertySheet.AddPage(&m_PageWood);
	m_PropertySheet.AddPage(&m_PageOakPlank);
}

void CDlgMatOakPlank::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatSampler);
}
