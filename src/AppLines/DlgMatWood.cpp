/*
**
**	$Filename:	DlgMatWood.cpp $
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
#include "DlgMatWood.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.16  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**
**	Revision 1.15  2004/05/07 16:30:33  sm
**	- Bug #13 fixed. The BBox hierarchy is recounted on every
**	  object edit finish.
**	- Wooden materials contain a dark and a separate light
**	  material. Changes were also made in Lines.
**	- Introduced shape property copy including all materials,
**	  bumps and conditions. Multiple copy modes are possible.
**	
**	Revision 1.14  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**	
**	Revision 1.13  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	Revision 1.12  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	Revision 1.11  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**	Revision 1.10  2004/04/23 18:46:17  sm
**	- Fixed bump sampler: Now using initialized derivativs
**	
**	Revision 1.9  2004/04/23 13:17:17  sm
**	- Added simple material page and renamed wood material page.
**	- Reflect material member renaming.
**	
**	Revision 1.8  2004/04/22 20:23:56  sm
**	- Fixed wrong ON_MESSAGE signature on call function.
**	- Reordered context menu of object editor.
**	- Expand root item of tree view in object editor
**	
**	Revision 1.7  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**	
**	Revision 1.6  2004/04/10 15:59:51  sm
**	- Added control units as base class for
**	  o CB3FloatSliderCtrl
**	  o CB3FloatSpinButtonCtrl
**	
**	Revision 1.5  2004/04/09 17:30:31  sm
**	- Wood dialog fine tuning.
**	
**	Revision 1.4  2004/04/09 12:08:05  sm
**	- New CStatic control introduced for material sampling.
**	
**	Revision 1.3  2004/04/04 19:28:25  sm
**	- New wood dialog
**	
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
**                        CDlgMatWood implementation                    **
**                                                                      **
*************************************************************************/

CDlgMatWood::CDlgMatWood(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatWood::IDD, pParent)
{
	m_Material = (b3MatWood *)item;
	m_PageDark.m_Material   = &m_Material->m_DarkMaterial;
	m_PageLight.m_Material  = &m_Material->m_LightMaterial;
	m_PageWood.m_Wood       =  m_Material;
	m_PageScaling.m_Scaling =  m_Material;
	//{{AFX_DATA_INIT(CDlgMatWood)
	//}}AFX_DATA_INIT
}

CDlgMatWood::~CDlgMatWood()
{
	delete m_MatSampler;
}

void CDlgMatWood::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatWood)
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatWood, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatWood)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatWood message handlers

void CDlgMatWood::b3Register()
{
	b3Loader::b3AddClassType(WOOD,IDS_MAT_WOOD,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgMatWood::b3Edit(b3Item *item)
{
	CDlgMatWood dlg(item);

	return dlg.DoModal() == IDOK;
}

void CDlgMatWood::b3InitDialog()
{
	m_PreviewMaterialCtrl.b3Init();
	m_MatSampler = new b3MaterialSampler(m_PreviewMaterialCtrl);
	m_MatSampler->b3SetMaterial(m_Material);

	m_PageDark.b3SetCaption(IDS_TITLE_DARK);
	m_PageLight.b3SetCaption(IDS_TITLE_LIGHT);
	m_PageDark.b3AddToSheet(&m_PropertySheet);
	m_PageLight.b3AddToSheet(&m_PropertySheet);
	m_PageWood.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
}

void CDlgMatWood::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatSampler);
}
