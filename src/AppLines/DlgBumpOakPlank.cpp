/*
**
**	$Filename:	DlgBumpOakPlank.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood bump map
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

#include "AppLinesInclude.h"

#include "DlgBumpOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.13  2006/03/05 22:12:31  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.12  2004/05/12 19:10:50  sm
**	- Completed bump mapping dialog.
**	
**	Revision 1.11  2004/05/10 15:12:08  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.10  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**	
**	Revision 1.9  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**	
**	Revision 1.8  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	Revision 1.7  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	Revision 1.6  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**	Revision 1.5  2004/04/23 16:51:09  sm
**	- Color renaming finished.
**	- Bug #18 fixed: The bump amplitude is read out correctly now.
**	
**	Revision 1.4  2004/04/22 20:23:55  sm
**	- Fixed wrong ON_MESSAGE signature on call function.
**	- Reordered context menu of object editor.
**	- Expand root item of tree view in object editor
**	
**	Revision 1.3  2004/04/19 09:00:52  sm
**	- Added bump sampler.
**	- Reactivated bump sampler in bump dialogs.
**	
**	Revision 1.2  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**	
**	Revision 1.1  2004/04/17 17:18:33  sm
**	- Made some include adjustments
**	- Added oakplank bump as dialog
**	- Fixed b3BumpWood and b3BumpOakPlank
**	  bump computation
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpOakPlank::CDlgBumpOakPlank(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgBumpOakPlank::IDD, pParent)
{
	m_Bump                    = (b3BumpOakPlank *)item;
	m_PageBump.m_Bump         = m_Bump;
	m_PageOakPlank.m_OakPlank = m_Bump;
	m_PageWood.m_Wood         = m_Bump;
	m_PageScaling.m_Scaling   = m_Bump;
	//{{AFX_DATA_INIT(CDlgBumpOakPlank)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgBumpOakPlank::~CDlgBumpOakPlank()
{
	delete m_BumpSampler;
}

void CDlgBumpOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpOakPlank)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpOakPlank, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgBumpOakPlank)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpOakPlank message handlers

void CDlgBumpOakPlank::b3Register()
{
	b3Loader::b3AddClassType(BUMP_OAKPLANK,IDS_BUMP_OAKPLANK,IDI_MAT_OAKPLANK,b3Edit,b3Edit);
}

b3_bool CDlgBumpOakPlank::b3Edit(b3Item *item,void *ptr)
{
	CDlgBumpOakPlank dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgBumpOakPlank::b3InitDialog() 
{
	m_PreviewBumpCtrl.b3Init();
	m_BumpSampler = new b3BumpSampler(m_PreviewBumpCtrl,1);
	m_BumpSampler->b3SetBump(m_Bump);

	m_PageBump.b3AddToSheet(&m_PropertySheet);
	m_PageWood.b3AddToSheet(&m_PropertySheet);
	m_PageOakPlank.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgBumpOakPlank::b3UpdateUI()
{
	m_PreviewBumpCtrl.b3Update(m_BumpSampler);
}
