/*
**
**	$Filename:	b3CSGDialog.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for CSG shapes
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
#include "b3CSGDialog.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/24 17:45:32  sm
**	- Added CSG edit dialogs
**	- Corrected shape edit inheritance.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3CSGDialog implementation                   **
**                                                                      **
*************************************************************************/

CB3CSGDialog::CB3CSGDialog(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CB3CSGDialog)
	m_CSGMode = 0;
	//}}AFX_DATA_INIT
}


void CB3CSGDialog::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3CSGDialog)
	DDX_Radio(pDX, IDC_CSGMODE_UNION, m_CSGMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3CSGDialog, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CB3CSGDialog)
	ON_BN_CLICKED(IDC_CSGMODE_UNION, OnCSGModeChanged)
	ON_BN_CLICKED(IDC_CSGMODE_INTERSECT, OnCSGModeChanged)
	ON_BN_CLICKED(IDC_CSGMODE_SUB, OnCSGModeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ShapeDialog message handlers

const char *CB3CSGDialog::b3GetSection()
{
	return "csg";
}

void CB3CSGDialog::OnCSGModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3SetCSGMode(m_CSGMode);
}

void CB3CSGDialog::b3SetCSGMode(int csgmode)
{
}

void CB3CSGDialog::b3UpdateBase()
{
}

BOOL CB3CSGDialog::OnInitDialog() 
{
	b3CSGShape *shape = (b3CSGShape *)m_Shape;

	m_CSGMode = shape->b3GetOperationIndex(shape->m_Operation);
	CB3ShapeDialog::OnInitDialog();
	if (m_Creation)
	{
		m_DirMode = shape->m_Operation;
	}
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3CSGDialog::OnOK() 
{
	// TODO: Add extra validation here
	CB3ShapeDialog::OnOK();
}
