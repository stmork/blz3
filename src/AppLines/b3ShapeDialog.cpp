/*
**
**	$Filename:	b3ShapeDialog.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for simple shapes
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "b3ImageList.h"
#include "b3ShapeDialog.h"
#include "DlgCreateMaterial.h"
#include "DlgCreateStencil.h"
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2002/03/01 20:26:40  sm
**	- Added CB3FloatSpinButtonCtrl for conveniant input.
**	- Made some minor changes and tests.
**
**	Revision 1.5  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.4  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.3  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.2  2002/02/24 17:45:32  sm
**	- Added CSG edit dialogs
**	- Corrected shape edit inheritance.
**	
**	Revision 1.1  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3ShapeDialog implementation                 **
**                                                                      **
*************************************************************************/

CB3ShapeDialog::CB3ShapeDialog(UINT IDD,CWnd* pParent /*=NULL*/)
	: CPropertyPage(IDD)
{
	//{{AFX_DATA_INIT(CB3ShapeDialog)
	//}}AFX_DATA_INIT
	m_Shape    = null;
	m_Creation = false;
}


void CB3ShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3ShapeDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3ShapeDialog, CPropertyPage)
	//{{AFX_MSG_MAP(CB3ShapeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ShapeDialog message handlers

int CB3ShapeDialog::b3Edit(
	CB3ShapeDialog *page,
	b3Item         *item,
	b3_bool         create)
{
	CPropertySheet      sheet;
	CDlgCSGMode         dlg_csg;
	CDlgCreateMaterial  dlg_material;
	CDlgCreateStencil   dlg_stencil;
	CString             text;
	b3Shape            *shape;
	int                 result;

	page->m_Creation = create;
	page->m_Shape    = (b3ShapeRenderObject *)item;
	page->b3Init();
	sheet.AddPage(page);

	switch (item->b3GetClass())
	{
	case CLASS_CSG:
		shape = (b3Shape *)item;
		dlg_csg.m_Creation = create;
		dlg_csg.m_Shape    = (b3CSGShape *)item;
		dlg_csg.m_Section  = page->b3GetSection();
		dlg_csg.b3Init();
		sheet.AddPage(&dlg_csg);
		if (create)
		{
			sheet.AddPage(&dlg_material);
		}
		break;

	case CLASS_SHAPE:
		shape = (b3Shape *)item;
		if (create)
		{
			dlg_stencil.m_Shape = shape;
			sheet.AddPage(&dlg_material);
			sheet.AddPage(&dlg_stencil);
		}
		break;

	default:
		shape = null;
		break;
	}

	CB3ImageList::b3ComputeText(item,text);
	sheet.SetTitle(text);
	result = sheet.DoModal();

	if (result == IDOK)
	{
		page->b3PostProcess();
		dlg_csg.b3PostProcess();
		if (create && (shape != null))
		{
			dlg_stencil.b3PostProcess();
			dlg_material.b3PostProcess(dlg_stencil.m_Stencil);

			if (dlg_material.m_Material != null)
			{
				shape->b3GetMaterialHead()->b3Append(dlg_material.m_Material);
			}

			if (dlg_stencil.m_Stencil != null)
			{
				shape->b3GetConditionHead()->b3Append(dlg_stencil.m_Stencil);
			}
			shape->b3Activate();
		}
	}
	return result;
}

const char *CB3ShapeDialog::b3GetSection()
{
	return "shape";
}

void CB3ShapeDialog::b3Init()
{
}

void CB3ShapeDialog::b3PostProcess() 
{
	m_Shape->b3Recompute();
}
