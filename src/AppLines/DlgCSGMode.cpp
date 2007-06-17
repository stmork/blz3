/*
**
**	$Filename:	DlgCSGMode.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Change CSG operation mode
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
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        CDlgCSGMode implementation                    **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CDlgCSGMode, CPropertyPage)

CDlgCSGMode::CDlgCSGMode() : CPropertyPage(CDlgCSGMode::IDD)
{
	//{{AFX_DATA_INIT(CDlgCSGMode)
	m_CSGMode = 0;
	//}}AFX_DATA_INIT
	m_Section  = "csg";
	m_Shape    = null;
	m_Creation = false;
}

CDlgCSGMode::~CDlgCSGMode()
{
}

void CDlgCSGMode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSGMode)
	DDX_Radio(pDX, IDC_CSGMODE_UNION, m_CSGMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSGMode, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgCSGMode)
	ON_BN_CLICKED(IDC_CSGMODE_UNION, OnCSGModeChanged)
	ON_BN_CLICKED(IDC_CSGMODE_INTERSECT, OnCSGModeChanged)
	ON_BN_CLICKED(IDC_CSGMODE_SUB, OnCSGModeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSGMode message handlers

void CDlgCSGMode::b3Init()
{
	if (m_Shape != null)
	{
		m_CSGMode = m_Creation ?
			AfxGetApp()->GetProfileInt(CB3ClientString(),m_Section + CString(".csg"),m_CSGMode) :
			m_Shape->b3GetOperationIndex(m_Shape->m_Operation);
	}
}

BOOL CDlgCSGMode::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSGMode::OnCSGModeChanged()
{
	CPropertySheet *sheet;
	CString         text;

	UpdateData();
	m_Shape->m_Operation = b3CSGShape::m_CSGMode[m_CSGMode];
	CB3ImageList::b3ComputeText(m_Shape,text);
	sheet = (CPropertySheet *)GetParent();
	sheet->SetTitle(text);
}

void CDlgCSGMode::b3PostProcess() 
{
	// TODO: Add extra validation here
	if (m_Shape != null)
	{
		if (m_Creation)
		{
			AfxGetApp()->WriteProfileInt(CB3ClientString(),m_Section + CString(".csg"),m_CSGMode);
		}
		m_Shape->m_Operation = b3CSGShape::m_CSGMode[m_CSGMode];
	}
}
