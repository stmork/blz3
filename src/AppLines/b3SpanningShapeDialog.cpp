// b3SpanningShapeDialog.cpp: implementation of the CB3SpanningShapeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "b3SpanningShapeDialog.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3SpanningShapeDialog implementation         **
**                                                                      **
*************************************************************************/

CB3SpanningShapeDialog::CB3SpanningShapeDialog(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(IDD)
{
	//{{AFX_DATA_INIT(CB3ShapeDialog)
	m_DirMode = 0;
	//}}AFX_DATA_INIT
	m_DirMode = AfxGetApp()->GetProfileInt(CB3ClientString(),b3GetSection() + CString(".mode"),m_DirMode);
}


void CB3SpanningShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB3SpanningShapeDialog)
	DDX_Radio(pDX, IDC_DIRECTION, m_DirMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CB3SpanningShapeDialog, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CB3SpanningShapeDialog)
	ON_BN_CLICKED(IDC_DIRECTION, OnDirModeChanged)
	ON_BN_CLICKED(IDC_POSITION, OnDirModeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SpanningShapeDialog message handlers

void CB3SpanningShapeDialog::OnDirModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3SetDirMode(m_DirMode);
}

void CB3SpanningShapeDialog::b3SetDirMode(int dirmode)
{
}

void CB3SpanningShapeDialog::b3UpdateBase()
{
}

void CB3SpanningShapeDialog::b3PostProcess() 
{
	CB3ShapeDialog::b3PostProcess();
	AfxGetApp()->WriteProfileInt(CB3ClientString(),b3GetSection() + CString(".mode"),m_DirMode);
}
