/*
**
**	$Filename:	DlgCSGMode.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "AppLines.h"
#include "b3ImageList.h"
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**
**
*/

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
	m_Section = "csg";
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

BOOL CDlgCSGMode::OnInitDialog() 
{
	m_CSGMode = m_Creation ?
		AfxGetApp()->GetProfileInt(CB3ClientString(),m_Section + CString(".csg"),m_CSGMode) :
		m_Shape->b3GetOperationIndex(m_Shape->m_Operation);
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

void CDlgCSGMode::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();
	if (m_Creation)
	{
		AfxGetApp()->WriteProfileInt(CB3ClientString(),m_Section + CString(".csg"),m_CSGMode);
	}
	m_Shape->m_Operation = b3CSGShape::m_CSGMode[m_CSGMode];
}
