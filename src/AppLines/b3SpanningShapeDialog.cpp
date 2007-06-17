/*
**
**	$Filename:	b3SpanningShapeDialog.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for editing spanning shapes
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
#include "b3SpanningShapeDialog.h"

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

void CB3SpanningShapeDialog::b3Init()
{
	// Call base class
	CB3ShapeDialog::b3Init();

	// Read from registry
	if (m_Creation)
	{
		m_DirMode = AfxGetApp()->GetProfileInt(CB3ClientString(),b3MakeSection("mode"),m_DirMode);
	}
}

void CB3SpanningShapeDialog::OnDirModeChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3SetDirMode(m_DirMode);
}

void CB3SpanningShapeDialog::b3PostProcess() 
{
	CB3ShapeDialog::b3PostProcess();
	if (m_Creation)
	{
		AfxGetApp()->WriteProfileInt(CB3ClientString(),b3MakeSection("mode"),m_DirMode);
	}
}
