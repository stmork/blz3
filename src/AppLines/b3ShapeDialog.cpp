/*
**
**	$Filename:	DlgShape3.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for quadric shapes
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
#include "b3ShapeDialog.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CB3ShapeDialog)
	m_DirMode = 0;
	//}}AFX_DATA_INIT
}


void CB3ShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3ShapeDialog)
	DDX_Radio(pDX, IDC_DIRECTION, m_DirMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3ShapeDialog, CDialog)
	//{{AFX_MSG_MAP(CB3ShapeDialog)
	ON_BN_CLICKED(IDC_DIRECTION, OnDirModeChanged)
	ON_BN_CLICKED(IDC_POSITION, OnDirModeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ShapeDialog message handlers

void CB3ShapeDialog::OnDirModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3SetDirMode(m_DirMode);
}

void CB3ShapeDialog::b3SetDirMode(int dirmode)
{
}
