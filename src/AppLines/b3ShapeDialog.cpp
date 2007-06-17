/*
**
**	$Filename:	b3ShapeDialog.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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
