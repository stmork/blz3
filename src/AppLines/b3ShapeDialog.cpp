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
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	m_DirMode = 0;
	//}}AFX_DATA_INIT
	m_Shape = null;
}


void CB3ShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3ShapeDialog)
	DDX_Radio(pDX, IDC_DIRECTION, m_DirMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3ShapeDialog, CPropertyPage)
	//{{AFX_MSG_MAP(CB3ShapeDialog)
	ON_BN_CLICKED(IDC_DIRECTION, OnDirModeChanged)
	ON_BN_CLICKED(IDC_POSITION, OnDirModeChanged)
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
	CString             text;
	b3Shape            *shape = null;
	b3_bool             result;

	page->m_Creation = create;
	page->m_Shape    = (b3CSGShape *)item;
	sheet.AddPage(page);

	switch (item->b3GetClass())
	{
	case CLASS_CSG:
		dlg_csg.m_Creation = create;
		dlg_csg.m_Shape    = (b3CSGShape *)item;
		dlg_csg.m_Section  = page->b3GetSection();
		sheet.AddPage(&dlg_csg);
		// Walk through!
	case CLASS_SHAPE:
		if (create)
		{
			sheet.AddPage(&dlg_material);
		}
		shape = (b3Shape *)item;
		break;
	}

	CB3ImageList::b3ComputeText(item,text);
	sheet.SetTitle(text);
	result = sheet.DoModal();

	if (result && create && (shape != null))
	{
		if (dlg_material.m_Material != null)
		{
			shape->b3GetMaterialHead()->b3Append(dlg_material.m_Material);
		}
	}
	return result;
}

const char *CB3ShapeDialog::b3GetSection()
{
	return "shape";
}

void CB3ShapeDialog::OnDirModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3SetDirMode(m_DirMode);
}

void CB3ShapeDialog::b3SetDirMode(int dirmode)
{
}

void CB3ShapeDialog::b3UpdateBase()
{
}

BOOL CB3ShapeDialog::OnInitDialog() 
{
	m_DirMode = AfxGetApp()->GetProfileInt(CB3ClientString(),b3GetSection() + CString(".mode"),m_DirMode);
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3ShapeDialog::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
	AfxGetApp()->WriteProfileInt(CB3ClientString(),b3GetSection() + CString(".mode"),m_DirMode);
	m_Shape->b3Recompute();
}
