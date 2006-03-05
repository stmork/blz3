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

#include "AppLinesInclude.h"

#include "b3ImageList.h"
#include "b3ShapeDialog.h"
#include "DlgCreateMaterial.h"
#include "DlgCreateStencil.h"
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        How to use CB3ShapeDialog derived classes...  **
**                                                                      **
*************************************************************************/

/*
**	Constructor:    Init values
**
**  b3Init()        Call base class
**	                If m_Creation == true read values from registry.
**	                Setup control ranges such as b3SetRange
**	                m_Shape must be initialized!
**
**  OnInitDialog()  Calling base class(es) will init controls via DDX
**
**  b3PostProcess() Before calling base class:
**	                If m_Creation == true write values to registry
**	                Call base class
**	                Initialize shape
**
**	NOTE: b3Init() is called anyway but OnInitDialog() is called when the
**	      user clicks onto the property bar first.
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.13  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.12  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.11  2003/02/22 15:17:18  sm
**	- Added support for selected shapes in object modeller
**	- Glued b3Shape and b3ShapeRenderObject. There was no
**	  distinct access method in use.
**	- Made some b3Shape methods inline and/or static which
**	  saves some memory.
**	
**	Revision 1.10  2002/03/11 13:48:54  sm
**	- Cleaned up dialog titles
**	- Fixed some texture bugs concerning palette copying.
**	- Added a triangles profile.
**	
**	Revision 1.9  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**	
**	Revision 1.8  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.7  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
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
	m_Section  = "shape";
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
	b3_u32          shading_class_type,
	b3Item         *item,
	b3_bool         create)
{
	CPropertySheet      sheet;
	CDlgCSGMode         dlg_csg;
	CDlgCreateMaterial  dlg_material(shading_class_type);
	CDlgCreateStencil   dlg_stencil;
	CString             shape_text,title;
	b3Shape            *shape;
	int                 result;

	if (page != null)
	{
		page->m_Creation = create;
		page->m_Shape    = (b3Shape *)item;
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
				dlg_material.b3Init();
				sheet.AddPage(&dlg_material);

				dlg_stencil.m_Shape = shape;
				dlg_stencil.b3Init();
				sheet.AddPage(&dlg_stencil);
			}
			break;

		default:
			shape = null;
			break;
		}

		CB3ImageList::b3ComputeText(item,shape_text);
		title.Format(create ? IDS_TITLE_CREATE : IDS_TITLE_EDIT,shape_text);
		sheet.SetTitle(title);
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
	}
	else
	{
		result = IDCANCEL;
	}
	return result;
}

const char *CB3ShapeDialog::b3GetSection()
{
	return m_Section;
}

const char *CB3ShapeDialog::b3MakeSection(const char *title)
{
	m_SectionTitle.Format("%s.%s",b3GetSection(),title);
	return m_SectionTitle;
}

void CB3ShapeDialog::b3Init()
{
	B3_ASSERT(m_Shape != null);
	m_Section.Format("shape [%08x]",m_Shape->b3GetClassType());
	b3PrintF(B3LOG_NORMAL,"CB3ShapeDialog::b3Init() called for \"%s\"\n",
		b3GetSection());
}

void CB3ShapeDialog::b3PostProcess() 
{
	m_Shape->b3Recompute();
	b3PrintF(B3LOG_NORMAL,"CB3ShapeDialog::b3PostProcess() called for \"%s\"\n",
		b3GetSection());
}
